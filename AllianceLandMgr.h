#pragma once

#include <ctime>
#include <map>
#include <set>
#include <queue>
#include <memory>
#include <algorithm>

#include "UnionFind.h"

////////////////////////////////////////////////
// 在移植到服务端时移除
#include <iostream>
#include <Windows.h>

using uint64 = uint64_t;
using uint32 = uint32_t;

class LOGGG
{
public:
    static void log(int i, const std::string& str)
    {
        ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
        std::cout << "ERROR:" << str << ":" << i << std::endl;
    }
};

#define LP_IF(x)  if ((x) ? (LOGGG::log(__LINE__, __FILE__), true) : false)
#define LP_LOG_FAIL(x) {if (!(x)) {LOGGG::log(__LINE__, __FILE__);} }
#define X_OK 0
#define X_SERVER_ERROR 1
#define X_LAND_NOT_SUPPORT 2
#define OUT
#define IN

struct ST_COORDINATE
{
    uint32 x;
    uint32 y;

    ST_COORDINATE() : x(0), y(0) { }
    ST_COORDINATE(const uint32 x_, const uint32 y_) : x(x_), y(y_) { }

    ST_COORDINATE& operator=(const ST_COORDINATE& rCoord)
    {
        x = rCoord.x;
        y = rCoord.y;
        return *this;
    }
};

enum E_MAP_OBJECT_TYPE
{
    E_MAP_OBJECT_TYPE_NOT_SET    = 0, // 未设置
    E_MAP_OBJECT_TYPE_HARBOUR    = 2, // 港口
    E_MAP_OBJECT_TYPE_FORTRESS   = 8, // 联盟要塞
    E_MAP_OBJECT_TYPE_LIGHTHOUSE = 9, // 联盟灯塔
};

class CMapObj : public std::enable_shared_from_this<CMapObj>
{
public:
    CMapObj(const E_MAP_OBJECT_TYPE eType_)
        : eType(eType_), m_stOccupyCoord(0, 0), m_bOccupyCoord(false) { }
    virtual ~CMapObj() { }

public:
    bool GetOccupyCoord(OUT ST_COORDINATE& rCoord)
    {
        rCoord = m_stOccupyCoord;
        return m_bOccupyCoord;
    }

    E_MAP_OBJECT_TYPE GetType() { return eType; }

    void SetOccupyCoord(bool bOccupy, const uint32 x, const uint32 y)
    {
        m_bOccupyCoord = bOccupy;
        m_stOccupyCoord.x = x;
        m_stOccupyCoord.y = y;
    }

private:
    E_MAP_OBJECT_TYPE eType;
    ST_COORDINATE m_stOccupyCoord;
    bool m_bOccupyCoord;

};

using SPTR_MAP_OBJ = std::shared_ptr<CMapObj>;
using WPTR_MAP_OBJ = std::weak_ptr<CMapObj>;

class CStaticMapObject : public CMapObj
{
public:
    CStaticMapObject(const uint64 ui64AllianceId, const time_t time_, const E_MAP_OBJECT_TYPE eType)
        : m_ui64AllianceId(ui64AllianceId), m_tActiveTime(time_), CMapObj(eType) { }

    virtual ~CStaticMapObject() { }

public:
    virtual uint64 GetAllianceId() { return m_ui64AllianceId; }
    virtual time_t GetActiveTime() { return m_tActiveTime; }

private:
    uint64 m_ui64AllianceId;
    time_t m_tActiveTime;
};

using SPTR_STATIC_MAP_OBJECT = std::shared_ptr<CStaticMapObject>;

class CMapObjHarbor : public CStaticMapObject
{
public:
    CMapObjHarbor(const uint64 ui64AllianceId, const time_t time_)
        : CStaticMapObject(ui64AllianceId, time_, E_MAP_OBJECT_TYPE_HARBOUR)
    {
    }
    
    virtual ~CMapObjHarbor() { }

public:
};

class CMapObjFortress : public CStaticMapObject
{
public:
    CMapObjFortress(const uint64 ui64AllianceId, const time_t time_)
        : CStaticMapObject(ui64AllianceId, time_, E_MAP_OBJECT_TYPE_FORTRESS) { }
    virtual ~CMapObjFortress() { }

public:
};

class CMapObjLighthouse : public CStaticMapObject
{
public:
    CMapObjLighthouse(const uint64 ui64AllianceId, const time_t time_)
        : CStaticMapObject(ui64AllianceId, time_, E_MAP_OBJECT_TYPE_LIGHTHOUSE) { }
    virtual ~CMapObjLighthouse() { }
};

using SPTR_MAP_OBJ_FORTRESS   = std::shared_ptr<CMapObjFortress>;
using SPTR_MAP_OBJ_HARBOR     = std::shared_ptr<CMapObjHarbor>;
using SPTR_MAP_OBJ_LIGHTHOUSE = std::shared_ptr<CMapObjLighthouse>;

#define DEFAULE_MAP_X_SIZE 1000
#define DEFAULE_MAP_Y_SIZE 1000
////////////////////////////////////////////////

// 地图坐标从1开始，所以分配的数值要比实际尺寸大1
#define LAND_ARRAY_X_SIZE (DEFAULE_MAP_X_SIZE + 1)
#define LAND_ARRAY_Y_SIZE (DEFAULE_MAP_Y_SIZE + 1)
#define LAND_UNION_FIND_ARR_SIZE ((LAND_ARRAY_X_SIZE * LAND_ARRAY_Y_SIZE) + 1)

// 领地id
using LAND_ID  = uint32;
using LAND_ID_SET = std::set<LAND_ID>;
using LAND_ID_QUE = std::queue<LAND_ID>;

// 领地标记id（用于标记领地连通性）
using LAND_LABEL_ID = uint32;
using LAND_LABEL_ID_SET = std::set<LAND_LABEL_ID>;
using LAND_LABEL_ID_QUE = std::queue<LAND_LABEL_ID>;

struct ST_LAND_DATA
{
    LAND_ID      idLand;         // 领地id
    uint64       ui64AllianceId; // 联盟id
    WPTR_MAP_OBJ wpMapObj;       // 地图对象
    LAND_ID_SET  setOverlapLand; // 记录重叠的领地

    ST_LAND_DATA(LAND_ID idLand_, uint64 ui64AllianceId_, SPTR_MAP_OBJ& rMapObjSptr)
        : idLand(idLand_), ui64AllianceId(ui64AllianceId_), wpMapObj(rMapObjSptr), setOverlapLand() { }
};

using LAND_DATA_MAP = std::map<LAND_ID, ST_LAND_DATA>; // 领地数据，key：领地id，value：领地数据
using ALLIANCE_LAND_MAP = std::map<uint64, LAND_ID_SET>; // 每个联盟拥有的领地，key：联盟id，value：领地集合

using ALLIANCE_ID_SET = std::set<uint64>;

const uint64 HARBOR_ENCLOSURE_RANGE     = 4; // 港口圈地范围
const uint64 FORTRESS_ENCLOSURE_RANGE   = 5; // 要塞圈地范围
const uint64 LIGHTHOUSE_ENCLOSURE_RANGE = 3; // 灯塔圈地范围

class CAllianceLandMgr
{
public:
    CAllianceLandMgr();

public:
    // 添加领地数据
    uint32 AddLandData(SPTR_MAP_OBJ& rMapObjSptr, const bool bReLabel = true); // 注意：bReLabel在服务端初始化时才会为false（快速加载数据），bReLabel=true时会重新标记rMapObjSptr所在联盟的领地连通性
    // 删除领地数据
    uint32 DelLandData(const LAND_ID idLand);
    // 清除指定联盟的所有领地数据（只在解散联盟时调用）
    uint32 ClearAllianceAllLand(const uint64 ui64AllianceId);

    // 判断两坐标是否连通（如果两坐标连通，说明在同一块领地）
    bool IsConnected(const uint32 x1, const uint32 y1, const uint32 x2, const uint32 y2);
    bool IsConnected(const ST_COORDINATE& rCoord1, const ST_COORDINATE& rCoord2);

    // 重新标记指定联盟的所有领地，rExcludeLandSet为被排除的领地，被排除的领地不会被标记（注意：目前用于服务端初始化加载完地图所有领地数据时调用，其它时候请勿使用）
    uint32 ReLabel(const ALLIANCE_ID_SET& rReLabelAllianceSet, const LAND_ID_SET& rExcludeLandSet = LAND_ID_SET());

private:
    // 更新rNewLandData圈地范围的领地id，如果碰到idDelLand则直接覆盖成新的领地id，idDelLand为要删除的领地id
    uint32 UpdateLandId(OUT ALLIANCE_ID_SET& rReLabelAllianceSet, const LAND_ID idDelLand, ST_LAND_DATA& rNewLandData);
    // 标记联盟所有领地，rExcludeLandSet为被排除的领地，被排除的领地不会被标记
    uint32 Label(const uint64 ui64AllianceId, const LAND_ID_SET& rExcludeLandSet = LAND_ID_SET());
    // 标记一个领地，rLandIdSet为同一个联盟的领地id集合
    void LabelOne(const LAND_ID idLand, const LAND_ID_SET& rLandIdSet);

private:
    // 检查MapObj数据，判断是否可圈地等
    bool CheckMapObjData(const SPTR_MAP_OBJ& rMapObjSptr);
    // 检查圈地优先级，=true时rMapObjSptr1优先，=falserMapObjSptr2优先
    bool CheckEnclosurePriority(const SPTR_MAP_OBJ& rMapObjSptr1, const SPTR_MAP_OBJ& rMapObjSptr2);
    // 计算圈地范围
    uint32 CalcEnclosureRange(OUT ST_COORDINATE& rLeftUpCoord, OUT ST_COORDINATE& rRightBtnCoord, const SPTR_MAP_OBJ& rMapObjSptr);

private:
    // 分配/释放领地id
    uint32 MallocLandId(OUT LAND_ID& rLandTerr);
    void   FreeLandId(const LAND_ID idLand);
    // 分配/释放标记id
    uint32 MallocLabelId(OUT LAND_LABEL_ID& rNewLabel);
    void   FreeLabelId(const LAND_LABEL_ID idLabel);

private:
    // 领地idMap，标记地图每个格子属于那个领地
    LAND_ID       m_arrLandMap[LAND_ARRAY_X_SIZE][LAND_ARRAY_Y_SIZE];
    // 标记idMap，根据领地idMap标记出所有连通的区域
    LAND_LABEL_ID m_arrLabelMap[LAND_ARRAY_X_SIZE][LAND_ARRAY_Y_SIZE];
    // 并查集，用于记录哪些标记id是连通着的
    LAND_LABEL_ID m_arrUnionFind[LAND_UNION_FIND_ARR_SIZE];
    // 领地数据，key：领地id
    LAND_DATA_MAP m_mapLandData;
    // 每个联盟的领地数据
    ALLIANCE_LAND_MAP m_mapAllianceLand;

    LAND_ID           m_LandIdDist;    // 领地id分配器
    LAND_LABEL_ID     m_LabelIdDist;   // 标记id分配器
    LAND_ID_QUE       m_queFreeLandId; // 空闲的领地id
    LAND_LABEL_ID_QUE m_queFreeLabelId;// 空闲的标记id

public: // 测试用
    bool CheckLandMap(uint32 uiLeftUpX, uint32 uiLeftUpY, uint32 uiRightBtnX, uint32 uiRightBtnY, uint32 uiValue)
    {
        for (uint32 x = uiLeftUpX; x <= uiRightBtnX; ++x)
        {
            for (uint32 y = uiLeftUpY; y <= uiRightBtnY; ++y)
            {
                if (uiValue != m_arrLandMap[x][y])
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool CheckLabelMap(uint32 uiLeftUpX, uint32 uiLeftUpY, uint32 uiRightBtnX, uint32 uiRightBtnY, uint32 uiValue)
    {
        for (uint32 x = uiLeftUpX; x <= uiRightBtnX; ++x)
        {
            for (uint32 y = uiLeftUpY; y <= uiRightBtnY; ++y)
            {
                if (uiValue != m_arrLabelMap[x][y])
                {
                    return false;
                }
            }
        }

        return true;
    }
};


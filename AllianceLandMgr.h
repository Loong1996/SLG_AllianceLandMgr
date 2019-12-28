#pragma once

#include <ctime>
#include <map>
#include <set>
#include <queue>
#include <memory>
#include <algorithm>

#include "UnionFind.h"

////////////////////////////////////////////////
// ����ֲ�������ʱ�Ƴ�
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
    E_MAP_OBJECT_TYPE_NOT_SET    = 0, // δ����
    E_MAP_OBJECT_TYPE_HARBOUR    = 2, // �ۿ�
    E_MAP_OBJECT_TYPE_FORTRESS   = 8, // ����Ҫ��
    E_MAP_OBJECT_TYPE_LIGHTHOUSE = 9, // ���˵���
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

// ��ͼ�����1��ʼ�����Է������ֵҪ��ʵ�ʳߴ��1
#define LAND_ARRAY_X_SIZE (DEFAULE_MAP_X_SIZE + 1)
#define LAND_ARRAY_Y_SIZE (DEFAULE_MAP_Y_SIZE + 1)
#define LAND_UNION_FIND_ARR_SIZE ((LAND_ARRAY_X_SIZE * LAND_ARRAY_Y_SIZE) + 1)

// ���id
using LAND_ID  = uint32;
using LAND_ID_SET = std::set<LAND_ID>;
using LAND_ID_QUE = std::queue<LAND_ID>;

// ��ر��id�����ڱ�������ͨ�ԣ�
using LAND_LABEL_ID = uint32;
using LAND_LABEL_ID_SET = std::set<LAND_LABEL_ID>;
using LAND_LABEL_ID_QUE = std::queue<LAND_LABEL_ID>;

struct ST_LAND_DATA
{
    LAND_ID      idLand;         // ���id
    uint64       ui64AllianceId; // ����id
    WPTR_MAP_OBJ wpMapObj;       // ��ͼ����
    LAND_ID_SET  setOverlapLand; // ��¼�ص������

    ST_LAND_DATA(LAND_ID idLand_, uint64 ui64AllianceId_, SPTR_MAP_OBJ& rMapObjSptr)
        : idLand(idLand_), ui64AllianceId(ui64AllianceId_), wpMapObj(rMapObjSptr), setOverlapLand() { }
};

using LAND_DATA_MAP = std::map<LAND_ID, ST_LAND_DATA>; // ������ݣ�key�����id��value���������
using ALLIANCE_LAND_MAP = std::map<uint64, LAND_ID_SET>; // ÿ������ӵ�е���أ�key������id��value����ؼ���

using ALLIANCE_ID_SET = std::set<uint64>;

const uint64 HARBOR_ENCLOSURE_RANGE     = 4; // �ۿ�Ȧ�ط�Χ
const uint64 FORTRESS_ENCLOSURE_RANGE   = 5; // Ҫ��Ȧ�ط�Χ
const uint64 LIGHTHOUSE_ENCLOSURE_RANGE = 3; // ����Ȧ�ط�Χ

class CAllianceLandMgr
{
public:
    CAllianceLandMgr();

public:
    // ����������
    uint32 AddLandData(SPTR_MAP_OBJ& rMapObjSptr, const bool bReLabel = true); // ע�⣺bReLabel�ڷ���˳�ʼ��ʱ�Ż�Ϊfalse�����ټ������ݣ���bReLabel=trueʱ�����±��rMapObjSptr�������˵������ͨ��
    // ɾ���������
    uint32 DelLandData(const LAND_ID idLand);
    // ���ָ�����˵�����������ݣ�ֻ�ڽ�ɢ����ʱ���ã�
    uint32 ClearAllianceAllLand(const uint64 ui64AllianceId);

    // �ж��������Ƿ���ͨ�������������ͨ��˵����ͬһ����أ�
    bool IsConnected(const uint32 x1, const uint32 y1, const uint32 x2, const uint32 y2);
    bool IsConnected(const ST_COORDINATE& rCoord1, const ST_COORDINATE& rCoord2);

    // ���±��ָ�����˵�������أ�rExcludeLandSetΪ���ų�����أ����ų�����ز��ᱻ��ǣ�ע�⣺Ŀǰ���ڷ���˳�ʼ���������ͼ�����������ʱ���ã�����ʱ������ʹ�ã�
    uint32 ReLabel(const ALLIANCE_ID_SET& rReLabelAllianceSet, const LAND_ID_SET& rExcludeLandSet = LAND_ID_SET());

private:
    // ����rNewLandDataȦ�ط�Χ�����id���������idDelLand��ֱ�Ӹ��ǳ��µ����id��idDelLandΪҪɾ�������id
    uint32 UpdateLandId(OUT ALLIANCE_ID_SET& rReLabelAllianceSet, const LAND_ID idDelLand, ST_LAND_DATA& rNewLandData);
    // �������������أ�rExcludeLandSetΪ���ų�����أ����ų�����ز��ᱻ���
    uint32 Label(const uint64 ui64AllianceId, const LAND_ID_SET& rExcludeLandSet = LAND_ID_SET());
    // ���һ����أ�rLandIdSetΪͬһ�����˵����id����
    void LabelOne(const LAND_ID idLand, const LAND_ID_SET& rLandIdSet);

private:
    // ���MapObj���ݣ��ж��Ƿ��Ȧ�ص�
    bool CheckMapObjData(const SPTR_MAP_OBJ& rMapObjSptr);
    // ���Ȧ�����ȼ���=trueʱrMapObjSptr1���ȣ�=falserMapObjSptr2����
    bool CheckEnclosurePriority(const SPTR_MAP_OBJ& rMapObjSptr1, const SPTR_MAP_OBJ& rMapObjSptr2);
    // ����Ȧ�ط�Χ
    uint32 CalcEnclosureRange(OUT ST_COORDINATE& rLeftUpCoord, OUT ST_COORDINATE& rRightBtnCoord, const SPTR_MAP_OBJ& rMapObjSptr);

private:
    // ����/�ͷ����id
    uint32 MallocLandId(OUT LAND_ID& rLandTerr);
    void   FreeLandId(const LAND_ID idLand);
    // ����/�ͷű��id
    uint32 MallocLabelId(OUT LAND_LABEL_ID& rNewLabel);
    void   FreeLabelId(const LAND_LABEL_ID idLabel);

private:
    // ���idMap����ǵ�ͼÿ�����������Ǹ����
    LAND_ID       m_arrLandMap[LAND_ARRAY_X_SIZE][LAND_ARRAY_Y_SIZE];
    // ���idMap���������idMap��ǳ�������ͨ������
    LAND_LABEL_ID m_arrLabelMap[LAND_ARRAY_X_SIZE][LAND_ARRAY_Y_SIZE];
    // ���鼯�����ڼ�¼��Щ���id����ͨ�ŵ�
    LAND_LABEL_ID m_arrUnionFind[LAND_UNION_FIND_ARR_SIZE];
    // ������ݣ�key�����id
    LAND_DATA_MAP m_mapLandData;
    // ÿ�����˵��������
    ALLIANCE_LAND_MAP m_mapAllianceLand;

    LAND_ID           m_LandIdDist;    // ���id������
    LAND_LABEL_ID     m_LabelIdDist;   // ���id������
    LAND_ID_QUE       m_queFreeLandId; // ���е����id
    LAND_LABEL_ID_QUE m_queFreeLabelId;// ���еı��id

public: // ������
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


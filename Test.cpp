#include "Test.h"

void Test1()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("单建筑测试");

    TEST_LOG("1、插入要塞，坐标点(1, 1)");
    SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
    sptrFortress->SetOccupyCoord(true, 1 + FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE);
    SPTR_MAP_OBJ sptrMapObj = sptrFortress;

    pLandMgr->AddLandData(sptrMapObj);
    TEST_CHECK_LAND_LOG("", 1, 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_LOG("2、删除要塞");
    pLandMgr->DelLandData(1);
    TEST_CHECK_LAND_LOG("", 1, 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 0);
    TEST_CHECK_LABEL_LOG("", 1, 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 0);

    delete pLandMgr;
}

void Test2()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("地图边界测试");

    std::vector<SPTR_MAP_OBJ> vecMapObj;
    TEST_LOG("1、地图左下角（无效）");
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, 0, 0);
        uint32 uiRet = pLandMgr->AddLandData(sptrMapObj);
        TEST_LOG_EQUAL("错误", X_OK, uiRet);

        TEST_CHECK_LAND_LOG("", 0, 0, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 0);
        TEST_CHECK_LABEL_LOG("", 0, 0, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 0);
    }

    TEST_LOG("2、地图左下角（有效）");
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, 1, 1);
        uint32 uiRet = pLandMgr->AddLandData(sptrMapObj);
        TEST_LOG_NOT_EQUAL("错误", X_OK, uiRet);
        TEST_CHECK_LAND_LOG("", 1, 1, 1 + FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE, 1);
        TEST_CHECK_LABEL_LOG("", 1, 1, 1 + FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE, 1);
    }

    TEST_LOG("3、地图左上角（无效）");
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, 0, DEFAULE_MAP_Y_SIZE);
        uint32 uiRet = pLandMgr->AddLandData(sptrMapObj);
        TEST_LOG_EQUAL("错误", X_OK, uiRet);
        TEST_CHECK_LAND_LOG("", 0, DEFAULE_MAP_Y_SIZE - (FORTRESS_ENCLOSURE_RANGE * 2), FORTRESS_ENCLOSURE_RANGE * 2, DEFAULE_MAP_Y_SIZE, 0);
        TEST_CHECK_LABEL_LOG("", 0, DEFAULE_MAP_Y_SIZE - (FORTRESS_ENCLOSURE_RANGE * 2), FORTRESS_ENCLOSURE_RANGE * 2, DEFAULE_MAP_Y_SIZE, 0);
    }

    TEST_LOG("4、地图左上角（有效）");
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, 1, DEFAULE_MAP_Y_SIZE);
        uint32 uiRet = pLandMgr->AddLandData(sptrMapObj);
        TEST_LOG_NOT_EQUAL("错误", X_OK, uiRet);
        TEST_CHECK_LAND_LOG("", 1, DEFAULE_MAP_Y_SIZE - FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE, DEFAULE_MAP_Y_SIZE, 2);
        TEST_CHECK_LABEL_LOG("", 1, DEFAULE_MAP_Y_SIZE - FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE, DEFAULE_MAP_Y_SIZE, 2);
    }

    TEST_LOG("5、地图右上角（有效）");
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, DEFAULE_MAP_X_SIZE, DEFAULE_MAP_Y_SIZE);
        uint32 uiRet = pLandMgr->AddLandData(sptrMapObj);
        TEST_LOG_NOT_EQUAL("错误", X_OK, uiRet);
        TEST_CHECK_LAND_LOG("", DEFAULE_MAP_X_SIZE - FORTRESS_ENCLOSURE_RANGE, DEFAULE_MAP_Y_SIZE - FORTRESS_ENCLOSURE_RANGE, DEFAULE_MAP_X_SIZE, DEFAULE_MAP_Y_SIZE, 3);
        TEST_CHECK_LABEL_LOG("", DEFAULE_MAP_X_SIZE - FORTRESS_ENCLOSURE_RANGE, DEFAULE_MAP_Y_SIZE - FORTRESS_ENCLOSURE_RANGE, DEFAULE_MAP_X_SIZE, DEFAULE_MAP_Y_SIZE, 3);
    }

    TEST_LOG("6、地图右下角（无效）");
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, DEFAULE_MAP_X_SIZE, 0);
        uint32 uiRet = pLandMgr->AddLandData(sptrMapObj);
        TEST_LOG_EQUAL("错误", X_OK, uiRet);
        TEST_CHECK_LAND_LOG("", DEFAULE_MAP_X_SIZE - FORTRESS_ENCLOSURE_RANGE, FORTRESS_ENCLOSURE_RANGE, DEFAULE_MAP_X_SIZE, 0, 0);
        TEST_CHECK_LABEL_LOG("", DEFAULE_MAP_X_SIZE - FORTRESS_ENCLOSURE_RANGE, FORTRESS_ENCLOSURE_RANGE, DEFAULE_MAP_X_SIZE, 0, 0);
    }

    TEST_LOG("7、地图右下角（有效）");
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, DEFAULE_MAP_X_SIZE, 1);
        uint32 uiRet = pLandMgr->AddLandData(sptrMapObj);
        TEST_LOG_NOT_EQUAL("错误", X_OK, uiRet);
        TEST_CHECK_LAND_LOG("", DEFAULE_MAP_X_SIZE - FORTRESS_ENCLOSURE_RANGE, 1, DEFAULE_MAP_X_SIZE, 1 + FORTRESS_ENCLOSURE_RANGE, 4);
        TEST_CHECK_LABEL_LOG("", DEFAULE_MAP_X_SIZE - FORTRESS_ENCLOSURE_RANGE, 1, DEFAULE_MAP_X_SIZE, 1 + FORTRESS_ENCLOSURE_RANGE, 4);
    }

    delete pLandMgr;
}

void Test3()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("联盟多建筑不重叠");

    std::vector<SPTR_MAP_OBJ> vecMapObj;

    // 要塞
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, 1 +FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    // 灯塔1
    {
        SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
        vecMapObj.push_back(sptrMapObj);

        sptrLighthouse->SetOccupyCoord(true, 2 + (FORTRESS_ENCLOSURE_RANGE*2) + LIGHTHOUSE_ENCLOSURE_RANGE, 2 + (FORTRESS_ENCLOSURE_RANGE*2) + LIGHTHOUSE_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    // 灯塔2
    {
        SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
        vecMapObj.push_back(sptrMapObj);

        sptrLighthouse->SetOccupyCoord(true, 3 + (FORTRESS_ENCLOSURE_RANGE * 2) + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + LIGHTHOUSE_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    TEST_LOG("数据创建成功");

    TEST_LOG("1、要塞领地");
    TEST_CHECK_LAND_LOG("", 1, 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_LOG("2、灯塔1");
    TEST_CHECK_LAND_LOG("", 2 + (FORTRESS_ENCLOSURE_RANGE * 2), 2 + (FORTRESS_ENCLOSURE_RANGE * 2),
        2 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        2 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 2);
    TEST_CHECK_LABEL_LOG("", 2 + (FORTRESS_ENCLOSURE_RANGE * 2), 2 + (FORTRESS_ENCLOSURE_RANGE * 2),
        2 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        2 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 2);

    TEST_LOG("3、灯塔2");
    TEST_CHECK_LAND_LOG("", 3 + (FORTRESS_ENCLOSURE_RANGE * 2), 1,
        3 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 3);
    TEST_CHECK_LABEL_LOG("", 3 + (FORTRESS_ENCLOSURE_RANGE * 2), 1,
        3 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 3);

    delete pLandMgr;
}

void Test4()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("单联盟多建筑重叠（简单）");

    std::vector<SPTR_MAP_OBJ> vecMapObj;

    // 要塞
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, 1 + FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    // 灯塔1
    {
        SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(1000, 3);
        SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
        vecMapObj.push_back(sptrMapObj);

        sptrLighthouse->SetOccupyCoord(true, 1 + (FORTRESS_ENCLOSURE_RANGE * 2) + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + (FORTRESS_ENCLOSURE_RANGE * 2) + LIGHTHOUSE_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    // 灯塔2
    {
        SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(1000, 2);
        SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
        vecMapObj.push_back(sptrMapObj);

        sptrLighthouse->SetOccupyCoord(true, 1 + (FORTRESS_ENCLOSURE_RANGE * 2) + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + (FORTRESS_ENCLOSURE_RANGE * 2) + LIGHTHOUSE_ENCLOSURE_RANGE + 1);
        pLandMgr->AddLandData(sptrMapObj);
    }

    TEST_LOG("数据创建成功");

    TEST_LOG("1、要塞领地");
    TEST_CHECK_LAND_LOG("", 1, 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_LOG("2、灯塔1");
    TEST_CHECK_LAND_LOG("", 1 + (FORTRESS_ENCLOSURE_RANGE * 2) + 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 2);
    TEST_CHECK_LABEL_LOG("", 1 + (FORTRESS_ENCLOSURE_RANGE * 2) + 1, 1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_LOG("3、灯塔2");
    TEST_CHECK_LAND_LOG("", 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2) + 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1, 3);
    TEST_CHECK_LABEL_LOG("", 1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1 + (FORTRESS_ENCLOSURE_RANGE * 2) + 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2) + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1, 1);

    delete pLandMgr;
}

void Test5()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("单联盟多建筑重叠（覆盖问题）");

    std::vector<SPTR_MAP_OBJ> vecMapObj;

    // 要塞
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, 1 + FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    // 灯塔1
    {
        SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
        vecMapObj.push_back(sptrMapObj);

        sptrLighthouse->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + LIGHTHOUSE_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    // 灯塔2
    {
        SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
        vecMapObj.push_back(sptrMapObj);

        sptrLighthouse->SetOccupyCoord(true, 2 + LIGHTHOUSE_ENCLOSURE_RANGE, 2 + LIGHTHOUSE_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    TEST_LOG("1、数据添加完毕");

    // 灯塔1
    TEST_CHECK_LAND_LOG("", 1, 1,
        1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 2);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LAND_LOG("", 1, 1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1, 2);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1, 1);

    // 灯塔2
    TEST_CHECK_LAND_LOG("", 2, 2,
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 3);
    TEST_CHECK_LABEL_LOG("", 2, 2,
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1);

    TEST_CHECK_LAND_LOG("", 2, 2,
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 3);
    TEST_CHECK_LABEL_LOG("", 2, 2,
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1);

    // 要塞

    TEST_CHECK_LAND_LOG("", 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1, 1,
        1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1, 1,
        1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_CHECK_LAND_LOG("", 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_CHECK_LAND_LOG("", 1, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1, 1);
    TEST_CHECK_LABEL_LOG("", 1, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1, 1);

    TEST_CHECK_LAND_LOG("", 1, 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    pLandMgr->DelLandData(3);

    // 灯塔
    TEST_CHECK_LAND_LOG("", 1, 1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 2);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1);

    // 要塞
    TEST_CHECK_LAND_LOG("", 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_CHECK_LAND_LOG("", 1, 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);


    pLandMgr->DelLandData(2);

    TEST_CHECK_LAND_LOG("", 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    pLandMgr->DelLandData(1);

    TEST_CHECK_LAND_LOG("", 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 0);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 0);

    delete pLandMgr;
}

void Test6()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("单联盟多建筑重叠+1个不重叠");

    std::vector<SPTR_MAP_OBJ> vecMapObj;

    // 要塞
    {
        SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrFortress;
        vecMapObj.push_back(sptrMapObj);

        sptrFortress->SetOccupyCoord(true, 1 + FORTRESS_ENCLOSURE_RANGE, 1 + FORTRESS_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    // 灯塔1
    {
        SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
        vecMapObj.push_back(sptrMapObj);

        sptrLighthouse->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + LIGHTHOUSE_ENCLOSURE_RANGE);
        pLandMgr->AddLandData(sptrMapObj);
    }

    // 灯塔2
    {
        SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(1000, 1);
        SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
        vecMapObj.push_back(sptrMapObj);

        sptrLighthouse->SetOccupyCoord(true, 2  + LIGHTHOUSE_ENCLOSURE_RANGE + (FORTRESS_ENCLOSURE_RANGE * 2), 2 + LIGHTHOUSE_ENCLOSURE_RANGE + (FORTRESS_ENCLOSURE_RANGE * 2));
        pLandMgr->AddLandData(sptrMapObj);
    }

    TEST_LOG("1、数据添加完毕");

    // 灯塔1
    TEST_CHECK_LAND_LOG("", 1, 1,
        1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 2);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LAND_LOG("", 1, 1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1, 2);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1, 1);

    // 灯塔2
    TEST_CHECK_LAND_LOG("", 2 + (FORTRESS_ENCLOSURE_RANGE * 2), 2 + (FORTRESS_ENCLOSURE_RANGE * 2),
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + (FORTRESS_ENCLOSURE_RANGE * 2),
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + (FORTRESS_ENCLOSURE_RANGE * 2), 3);
    TEST_CHECK_LABEL_LOG("", 2 + (FORTRESS_ENCLOSURE_RANGE * 2), 2 + (FORTRESS_ENCLOSURE_RANGE * 2),
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + (FORTRESS_ENCLOSURE_RANGE * 2),
        2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + (FORTRESS_ENCLOSURE_RANGE * 2), 2);

    // 要塞
    TEST_CHECK_LAND_LOG("", 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_CHECK_LAND_LOG("", 1, 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    pLandMgr->DelLandData(3);

    // 灯塔
    TEST_CHECK_LAND_LOG("", 1, 1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 2);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1);

    // 要塞
    TEST_CHECK_LAND_LOG("", 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);

    TEST_CHECK_LAND_LOG("", 1, 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 2 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);


    pLandMgr->DelLandData(2);

    TEST_CHECK_LAND_LOG("", 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 1);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 2);

    pLandMgr->DelLandData(1);

    TEST_CHECK_LAND_LOG("", 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 0);
    TEST_CHECK_LABEL_LOG("", 1, 1,
        1 + (FORTRESS_ENCLOSURE_RANGE * 2),
        1 + (FORTRESS_ENCLOSURE_RANGE * 2), 0);

    delete pLandMgr;
}

void ClearAllAllianceLand()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("删除所有联盟领地数据");

    std::srand(1); // 随机种子

    std::vector<SPTR_MAP_OBJ> vecMapObj;
    for (uint64 ui64AllianceId = 1000; ui64AllianceId < 2000; ++ui64AllianceId)
    {
        // 要塞
        {
            SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(ui64AllianceId, rand());
            SPTR_MAP_OBJ sptrMapObj = sptrFortress;
            vecMapObj.push_back(sptrMapObj);

            sptrFortress->SetOccupyCoord(true, 1 + (rand() % (DEFAULE_MAP_X_SIZE - 1)), 1 + (rand() % (DEFAULE_MAP_Y_SIZE - 1)));
            pLandMgr->AddLandData(sptrMapObj, false);
        }

        // 灯塔1
        for (int i = 0; i < 200; ++i)
        {
            SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(ui64AllianceId, rand());
            SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
            vecMapObj.push_back(sptrMapObj);

            sptrLighthouse->SetOccupyCoord(true, 1 + (rand() % (DEFAULE_MAP_X_SIZE - 1)), 1 + (rand() % (DEFAULE_MAP_Y_SIZE - 1)));
            pLandMgr->AddLandData(sptrMapObj, false);
        }
    }

    ALLIANCE_ID_SET setAllianceId;
    for (uint64 ui64AllianceId = 1000; ui64AllianceId < 2000; ++ui64AllianceId)
    {
        setAllianceId.insert(ui64AllianceId);
    }

    LP_LOG_FAIL(X_OK == pLandMgr->ReLabel(setAllianceId));

    for (uint64 ui64AllianceId = 1000; ui64AllianceId < 2000; ++ui64AllianceId)
    {
        LARGE_INTEGER timeStart;    //开始时间
        LARGE_INTEGER timeEnd;      //结束时间
        LARGE_INTEGER frequency;    //计时器频率
        QueryPerformanceFrequency(&frequency);
        double quadpart = (double)frequency.QuadPart;//计时器频率
        QueryPerformanceCounter(&timeStart); // 开始计时

        LP_LOG_FAIL(X_OK == pLandMgr->ClearAllianceAllLand(ui64AllianceId));

        QueryPerformanceCounter(&timeEnd); // 结束计时
        //计算排序耗时
        double elapsed = (timeEnd.QuadPart - timeStart.QuadPart) / quadpart;
        TEST_LOG("删除耗时" << elapsed); //单位为秒，精度为微秒
    }

    TEST_CHECK_LAND_LOG("", 0, 0,
        DEFAULE_MAP_X_SIZE,
        DEFAULE_MAP_Y_SIZE, 0);
    TEST_CHECK_LABEL_LOG("", 0, 0,
        DEFAULE_MAP_X_SIZE,
        DEFAULE_MAP_Y_SIZE, 0);

    delete pLandMgr;
}

void Test7()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("单联盟复杂重叠情况");

    std::vector<SPTR_MAP_OBJ> vecMapObj;
    // 灯塔1
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse1 = std::make_shared<CMapObjLighthouse>(1000, 2);
    SPTR_MAP_OBJ sptrMapObj1 = sptrLighthouse1;
    vecMapObj.push_back(sptrMapObj1);

    sptrLighthouse1->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + LIGHTHOUSE_ENCLOSURE_RANGE);
    pLandMgr->AddLandData(sptrMapObj1);

    // 灯塔2
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse2 = std::make_shared<CMapObjLighthouse>(1000, 4);
    SPTR_MAP_OBJ sptrMapObj2 = sptrLighthouse2;
    vecMapObj.push_back(sptrMapObj2);

    sptrLighthouse2->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE,
        1 + LIGHTHOUSE_ENCLOSURE_RANGE + LIGHTHOUSE_ENCLOSURE_RANGE);
    pLandMgr->AddLandData(sptrMapObj2);

    // 灯塔3
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse3 = std::make_shared<CMapObjLighthouse>(1000, 2);
    SPTR_MAP_OBJ sptrMapObj3 = sptrLighthouse3;
    vecMapObj.push_back(sptrMapObj3);

    sptrLighthouse3->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE,
        1 + LIGHTHOUSE_ENCLOSURE_RANGE + LIGHTHOUSE_ENCLOSURE_RANGE + LIGHTHOUSE_ENCLOSURE_RANGE);
    pLandMgr->AddLandData(sptrMapObj3);

    // 灯塔4
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse4 = std::make_shared<CMapObjLighthouse>(1000, 1);
    SPTR_MAP_OBJ sptrMapObj4 = sptrLighthouse4;
    vecMapObj.push_back(sptrMapObj4);

    sptrLighthouse4->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + LIGHTHOUSE_ENCLOSURE_RANGE + LIGHTHOUSE_ENCLOSURE_RANGE);
    pLandMgr->AddLandData(sptrMapObj4);

    pLandMgr->DelLandData(3);
    pLandMgr->DelLandData(4);
    pLandMgr->DelLandData(1);
    pLandMgr->DelLandData(2);

    pLandMgr->AddLandData(sptrMapObj4); // 3
    pLandMgr->AddLandData(sptrMapObj3); // 4
    pLandMgr->AddLandData(sptrMapObj1); // 1
    pLandMgr->AddLandData(sptrMapObj2); // 2

    pLandMgr->DelLandData(3);
    pLandMgr->DelLandData(4);
    pLandMgr->DelLandData(1);
    pLandMgr->DelLandData(2);

    pLandMgr->AddLandData(sptrMapObj3);
    pLandMgr->AddLandData(sptrMapObj4);
    pLandMgr->AddLandData(sptrMapObj1);
    pLandMgr->AddLandData(sptrMapObj2);

    // 灯塔5(叠在灯塔1下面）
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse5 = std::make_shared<CMapObjLighthouse>(1000, 3);
    SPTR_MAP_OBJ sptrMapObj5 = sptrLighthouse5;
    vecMapObj.push_back(sptrMapObj5);
    sptrLighthouse5->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + LIGHTHOUSE_ENCLOSURE_RANGE);
    pLandMgr->AddLandData(sptrMapObj5);

    pLandMgr->DelLandData(5);
    pLandMgr->DelLandData(2);
    pLandMgr->DelLandData(1);
    pLandMgr->DelLandData(3);
    pLandMgr->DelLandData(4);

    delete pLandMgr;
}

void Test8()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("单联盟新建建筑，导致没连通的变成连通的");

    std::vector<SPTR_MAP_OBJ> vecMapObj;
    // 灯塔1
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse1 = std::make_shared<CMapObjLighthouse>(1000, 1);
    SPTR_MAP_OBJ sptrMapObj1 = sptrLighthouse1;
    vecMapObj.push_back(sptrMapObj1);

    sptrLighthouse1->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + LIGHTHOUSE_ENCLOSURE_RANGE);
    pLandMgr->AddLandData(sptrMapObj1);

    // 灯塔2
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse2 = std::make_shared<CMapObjLighthouse>(1000, 1);
    SPTR_MAP_OBJ sptrMapObj2 = sptrLighthouse2;
    vecMapObj.push_back(sptrMapObj2);

    sptrLighthouse2->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE,
        3 + (LIGHTHOUSE_ENCLOSURE_RANGE * 3));
    pLandMgr->AddLandData(sptrMapObj2);

    // 灯塔3
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse3 = std::make_shared<CMapObjLighthouse>(1000, 1);
    SPTR_MAP_OBJ sptrMapObj3 = sptrLighthouse3;
    vecMapObj.push_back(sptrMapObj3);

    sptrLighthouse3->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE,
        1 + LIGHTHOUSE_ENCLOSURE_RANGE + LIGHTHOUSE_ENCLOSURE_RANGE + LIGHTHOUSE_ENCLOSURE_RANGE);
    pLandMgr->AddLandData(sptrMapObj3);


    pLandMgr->DelLandData(3);
    pLandMgr->DelLandData(1);
    pLandMgr->DelLandData(2);

    delete pLandMgr;
}

void Test9()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("多联盟复杂重叠情况");

    std::vector<SPTR_MAP_OBJ> vecMapObj;
    // 灯塔5
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse5 = std::make_shared<CMapObjLighthouse>(1000, 1);
    SPTR_MAP_OBJ sptrMapObj5 = sptrLighthouse5;
    vecMapObj.push_back(sptrMapObj5);
    sptrLighthouse5->SetOccupyCoord(true, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 5) + 1, 1 + LIGHTHOUSE_ENCLOSURE_RANGE);


    // 灯塔4
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse4 = std::make_shared<CMapObjLighthouse>(2000, 2);
    SPTR_MAP_OBJ sptrMapObj4 = sptrLighthouse4;
    vecMapObj.push_back(sptrMapObj4);
    sptrLighthouse4->SetOccupyCoord(true, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 5), 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2));

    // 灯塔3
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse3 = std::make_shared<CMapObjLighthouse>(1000, 3);
    SPTR_MAP_OBJ sptrMapObj3 = sptrLighthouse3;
    vecMapObj.push_back(sptrMapObj3);
    sptrLighthouse3->SetOccupyCoord(true, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 4), 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 3) + 1);

    // 灯塔2
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse2 = std::make_shared<CMapObjLighthouse>(1000, 4);
    SPTR_MAP_OBJ sptrMapObj2 = sptrLighthouse2;
    vecMapObj.push_back(sptrMapObj2);
    sptrLighthouse2->SetOccupyCoord(true, (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 5) + 2);

    // 灯塔1
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse1 = std::make_shared<CMapObjLighthouse>(2000, 5);
    SPTR_MAP_OBJ sptrMapObj1 = sptrLighthouse1;
    vecMapObj.push_back(sptrMapObj1);
    sptrLighthouse1->SetOccupyCoord(true, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 4) - 1, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 4) + 1);


    pLandMgr->AddLandData(sptrMapObj1);
    pLandMgr->AddLandData(sptrMapObj2);
    pLandMgr->AddLandData(sptrMapObj3);
    pLandMgr->AddLandData(sptrMapObj4);
    pLandMgr->AddLandData(sptrMapObj5);

    // 1
    // pLandMgr->DelLandData(4);
    
    // 2
    // pLandMgr->DelLandData(3);
    
    // 3
    //pLandMgr->DelLandData(2);

    // 4
    // pLandMgr->DelLandData(3);
    // pLandMgr->AddLandData(sptrMapObj3);

    // 5
    // pLandMgr->ClearAllianceAllLand(1000);
    // pLandMgr->ClearAllianceAllLand(2000);

    // 6
    pLandMgr->ClearAllianceAllLand(2000);
    pLandMgr->ClearAllianceAllLand(1000);

    delete pLandMgr;
}

void Test10()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("多联盟复杂连通情况，港口的强制圈地");

    std::vector<SPTR_MAP_OBJ> vecMapObj;
    // 港口
    SPTR_MAP_OBJ_HARBOR sptrHarbor = std::make_shared<CMapObjHarbor>(2000, 10);
    SPTR_MAP_OBJ sptrMapObj5 = sptrHarbor;
    vecMapObj.push_back(sptrMapObj5);
    sptrHarbor->SetOccupyCoord(true, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + HARBOR_ENCLOSURE_RANGE, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + HARBOR_ENCLOSURE_RANGE);


    // 灯塔4
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse4 = std::make_shared<CMapObjLighthouse>(2000, 2);
    SPTR_MAP_OBJ sptrMapObj4 = sptrLighthouse4;
    vecMapObj.push_back(sptrMapObj4);
    sptrLighthouse4->SetOccupyCoord(true, 1 + LIGHTHOUSE_ENCLOSURE_RANGE, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + (HARBOR_ENCLOSURE_RANGE* 2) + 1 + LIGHTHOUSE_ENCLOSURE_RANGE);

    // 灯塔3
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse3 = std::make_shared<CMapObjLighthouse>(2000, 3);
    SPTR_MAP_OBJ sptrMapObj3 = sptrLighthouse3;
    vecMapObj.push_back(sptrMapObj3);
    sptrLighthouse3->SetOccupyCoord(true, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2) + (HARBOR_ENCLOSURE_RANGE * 2), 1 + LIGHTHOUSE_ENCLOSURE_RANGE);

    // 灯塔2
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse2 = std::make_shared<CMapObjLighthouse>(1000, 4);
    SPTR_MAP_OBJ sptrMapObj2 = sptrLighthouse2;
    vecMapObj.push_back(sptrMapObj2);
    sptrLighthouse2->SetOccupyCoord(true, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 4), 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 4) + 1);

    // 灯塔1
    SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse1 = std::make_shared<CMapObjLighthouse>(1000, 5);
    SPTR_MAP_OBJ sptrMapObj1 = sptrLighthouse1;
    vecMapObj.push_back(sptrMapObj1);
    sptrLighthouse1->SetOccupyCoord(true, 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2), 1 + (LIGHTHOUSE_ENCLOSURE_RANGE * 2));


    pLandMgr->AddLandData(sptrMapObj1);
    pLandMgr->AddLandData(sptrMapObj2);
    pLandMgr->AddLandData(sptrMapObj3);
    pLandMgr->AddLandData(sptrMapObj4);

    pLandMgr->AddLandData(sptrMapObj5);

    // 1
    // pLandMgr->ClearAllianceAllLand(1000);
    // pLandMgr->ClearAllianceAllLand(2000);

    // 2
    //pLandMgr->ClearAllianceAllLand(2000);
    //pLandMgr->ClearAllianceAllLand(1000);


    //pLandMgr->DelLandData(2);
    //pLandMgr->DelLandData(5);

    //pLandMgr->AddLandData(sptrMapObj5);


    if (pLandMgr->IsConnected(18, 1, 1, 17))
    {
        TEST_LOG("连通");
    }
    else
    {
        TEST_LOG_FAIL("不连通");
    }

    if (!pLandMgr->IsConnected(18, 1, 4, 4))
    {
        TEST_LOG("不连通");
    }
    else
    {
        TEST_LOG_FAIL("连通");
    }

    //pLandMgr->DelLandData(2);

    delete pLandMgr;
}

void TestConnetedMothd()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("测试判断连通接口");

    //if (pLandMgr->IsConnected(0, 0, 0, 0))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    //if (pLandMgr->IsConnected(0, 1, 0, 0))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    //if (pLandMgr->IsConnected(0, 0, 1, 0))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    //if (pLandMgr->IsConnected(0, 0, 0, 1))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    //if (pLandMgr->IsConnected(0, 1, 1, 1))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    //if (pLandMgr->IsConnected(DEFAULE_MAP_X_SIZE + 1, 1, 1, 1))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    //if (pLandMgr->IsConnected(1, DEFAULE_MAP_Y_SIZE + 1, 1, 1))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    //if (pLandMgr->IsConnected(1, 1, DEFAULE_MAP_X_SIZE + 1, 1))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    //if (pLandMgr->IsConnected(1, 1, 1, DEFAULE_MAP_Y_SIZE + 1))
    //{
    //    TEST_LOG_FAIL("错误");
    //}

    delete pLandMgr;
}

void RandomTest()
{
    CAllianceLandMgr* pLandMgr = new CAllianceLandMgr;

    TEST_TITLE("随机测试，1000联盟，每个联盟1要塞200灯塔测试");

    std::srand(1); // 随机种子

    std::vector<SPTR_MAP_OBJ> vecMapObj;
    for (uint64 ui64AllianceId = 1000; ui64AllianceId < 2000; ++ui64AllianceId)
    {
        // 要塞
        {
            SPTR_MAP_OBJ_FORTRESS sptrFortress = std::make_shared<CMapObjFortress>(ui64AllianceId, rand());
            SPTR_MAP_OBJ sptrMapObj = sptrFortress;
            vecMapObj.push_back(sptrMapObj);

            sptrFortress->SetOccupyCoord(true, 1 + (rand() % (DEFAULE_MAP_X_SIZE - 1)), 1 + (rand() % (DEFAULE_MAP_Y_SIZE - 1)));
            pLandMgr->AddLandData(sptrMapObj, false);
        }

        // 灯塔1
        for (int i = 0; i < 200; ++i)
        {
            SPTR_MAP_OBJ_LIGHTHOUSE sptrLighthouse = std::make_shared<CMapObjLighthouse>(ui64AllianceId, rand());
            SPTR_MAP_OBJ sptrMapObj = sptrLighthouse;
            vecMapObj.push_back(sptrMapObj);

            sptrLighthouse->SetOccupyCoord(true, 1 + (rand() % (DEFAULE_MAP_X_SIZE - 1)), 1 + (rand() % (DEFAULE_MAP_Y_SIZE - 1)));
            pLandMgr->AddLandData(sptrMapObj, false);
        }
    }

    ALLIANCE_ID_SET setAllianceId;
    for (uint64 ui64AllianceId = 1000; ui64AllianceId < 2000; ++ui64AllianceId)
    {
        setAllianceId.insert(ui64AllianceId);
    }

    do
    {
        LARGE_INTEGER timeStart;	//开始时间
        LARGE_INTEGER timeEnd;		//结束时间
        LARGE_INTEGER frequency;	//计时器频率
        QueryPerformanceFrequency(&frequency);
        double quadpart = (double)frequency.QuadPart;//计时器频率
        QueryPerformanceCounter(&timeStart); // 开始计时

        pLandMgr->ReLabel(setAllianceId);

        QueryPerformanceCounter(&timeEnd); // 结束计时
        //计算排序耗时
        double elapsed = (timeEnd.QuadPart - timeStart.QuadPart) / quadpart;
        TEST_LOG("耗时" << elapsed); //单位为秒，精度为微秒
    } while (true);

    delete pLandMgr;
}

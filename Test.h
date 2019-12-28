#pragma once

#include <iostream>
#include "AllianceLandMgr.h"
#include <Windows.h>

#define TEST_CHECK_LAND_LOG(desc , x1, y1, x2, y2, v) \
    if (pLandMgr->CheckLandMap(x1, y1, x2, y2, v)) \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0a); \
        std::cout << desc << "  Land成功" << std::endl; \
    } \
    else \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c); \
        std::cout << desc << "  Land失败" << std::endl; \
    }

#define TEST_CHECK_LABEL_LOG(desc , x1, y1, x2, y2, v) \
    if (pLandMgr->CheckLabelMap(x1, y1, x2, y2, v)) \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0a); \
        std::cout << desc << "  Label成功" << std::endl; \
    } \
    else \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c); \
        std::cout << desc << "  Label失败" << std::endl; \
    }

#define TEST_LOG_EQUAL(desc, a, b) \
    if (a == b) \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c); \
        std::cout << desc << std::endl; \
    }

#define TEST_LOG_NOT_EQUAL(desc, a, b) \
    if (a != b) \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c); \
        std::cout << desc << std::endl; \
    } 

#define TEST_LOG(desc) \
{ \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0a); \
    std::cout << desc << std::endl; \
}

#define TEST_LOG_FAIL(desc) \
{ \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c); \
    std::cout << desc << std::endl; \
}

#define TEST_TITLE(desc) \
{ \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b); \
    std::cout << "==============================================" << std::endl; \
    std::cout << desc << std::endl; \
}


// 单个建筑测试
void Test1();
// 地图边界测试
void Test2();
// 单联盟多建筑不重叠
void Test3();
// 单联盟多建筑重叠（简单）
void Test4();
// 单联盟多建筑重叠（覆盖问题）
void Test5();
// 单联盟多建筑重叠+1个不重叠
void Test6();

// 删除所有联盟领地数据
void ClearAllAllianceLand();

// -----以下部分要手动测试
// 单联盟复杂重叠情况
void Test7();
// 单联盟新建建筑，导致没连通的变成连通的
void Test8();

// 多联盟复杂重叠情况
void Test9();
// 多联盟复杂连通情况，港口的强制圈地
void Test10();

// 测试判断连通接口
void TestConnetedMothd();

// 随机测试，1000联盟，每个联盟1要塞200灯塔测试
void RandomTest();

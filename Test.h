#pragma once

#include <iostream>
#include "AllianceLandMgr.h"
#include <Windows.h>

#define TEST_CHECK_LAND_LOG(desc , x1, y1, x2, y2, v) \
    if (pLandMgr->CheckLandMap(x1, y1, x2, y2, v)) \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0a); \
        std::cout << desc << "  Land�ɹ�" << std::endl; \
    } \
    else \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c); \
        std::cout << desc << "  Landʧ��" << std::endl; \
    }

#define TEST_CHECK_LABEL_LOG(desc , x1, y1, x2, y2, v) \
    if (pLandMgr->CheckLabelMap(x1, y1, x2, y2, v)) \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0a); \
        std::cout << desc << "  Label�ɹ�" << std::endl; \
    } \
    else \
    { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c); \
        std::cout << desc << "  Labelʧ��" << std::endl; \
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


// ������������
void Test1();
// ��ͼ�߽����
void Test2();
// �����˶ཨ�����ص�
void Test3();
// �����˶ཨ���ص����򵥣�
void Test4();
// �����˶ཨ���ص����������⣩
void Test5();
// �����˶ཨ���ص�+1�����ص�
void Test6();

// ɾ�����������������
void ClearAllAllianceLand();

// -----���²���Ҫ�ֶ�����
// �����˸����ص����
void Test7();
// �������½�����������û��ͨ�ı����ͨ��
void Test8();

// �����˸����ص����
void Test9();
// �����˸�����ͨ������ۿڵ�ǿ��Ȧ��
void Test10();

// �����ж���ͨ�ӿ�
void TestConnetedMothd();

// ������ԣ�1000���ˣ�ÿ������1Ҫ��200��������
void RandomTest();

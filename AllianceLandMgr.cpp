#include "AllianceLandMgr.h"

CAllianceLandMgr::CAllianceLandMgr()
{
    memset(m_arrLandMap, 0, sizeof(m_arrLandMap) / sizeof(LAND_ID));
    memset(m_arrLabelMap, 0, sizeof(m_arrLabelMap) / sizeof(LAND_LABEL_ID));
    memset(m_arrUnionFind, 0, sizeof(m_arrUnionFind) / sizeof(LAND_LABEL_ID));
    // id����1��ʼ���䣬0Ϊ��Чֵ
    m_LandIdDist = 1;
    m_LabelIdDist = 1;
}

uint32 CAllianceLandMgr::AddLandData(SPTR_MAP_OBJ& rMapObjSptr, const bool bReLabel /*= true*/)
{
    LP_IF(nullptr == rMapObjSptr)
    {
        return X_SERVER_ERROR;
    }

    if (!this->CheckMapObjData(rMapObjSptr))
    {
        return X_LAND_NOT_SUPPORT;
    }

    SPTR_STATIC_MAP_OBJECT spStaticMapObj = std::dynamic_pointer_cast<CStaticMapObject>(rMapObjSptr);
    LP_IF(nullptr == spStaticMapObj)
    {
        return X_SERVER_ERROR;
    }

    // ����Ȧ�ط�Χ
    ST_COORDINATE stLeftUpCoord;
    ST_COORDINATE stRightBtnCoord;
    uint32 uiRet = this->CalcEnclosureRange(stLeftUpCoord, stRightBtnCoord, rMapObjSptr);
    LP_IF(X_OK != uiRet)
    {
        return uiRet;
    }

    // �������id
    LAND_ID idNewLand = 0;
    uiRet = this->MallocLandId(idNewLand);
    LP_IF(X_OK != uiRet)
    {
        return uiRet;
    }

    ST_LAND_DATA stLandData(idNewLand, spStaticMapObj->GetAllianceId(), rMapObjSptr);
    ALLIANCE_ID_SET setReLabelAlliance; // ��Ҫ���±�ǵ�����
    setReLabelAlliance.insert(spStaticMapObj->GetAllianceId());
    for (uint32 x = stLeftUpCoord.x; x <= stRightBtnCoord.x; ++x)
    {
        for (uint32 y = stLeftUpCoord.y; y <= stRightBtnCoord.y; ++y)
        {
            LAND_ID& rCurLandId = m_arrLandMap[x][y];
            if (0 == rCurLandId) // ������û�б��������ռ��
            {
                rCurLandId = idNewLand;
                continue;
            }

            LAND_DATA_MAP::iterator iterFind = m_mapLandData.find(rCurLandId);
            LP_IF(m_mapLandData.end() == iterFind)
            {
                rCurLandId = idNewLand;
                continue;
            }

            ST_LAND_DATA& rFoundLandData = iterFind->second;
            SPTR_MAP_OBJ spFoundMapObj = rFoundLandData.wpMapObj.lock();
            LP_IF(nullptr == spFoundMapObj)
            {
                rCurLandId = idNewLand;
                continue;
            }

            stLandData.setOverlapLand.insert(rCurLandId);
            rFoundLandData.setOverlapLand.insert(idNewLand);

            // ���ռ�����ȼ�
            if (this->CheckEnclosurePriority(rMapObjSptr, spFoundMapObj))
            {
                rCurLandId = idNewLand;
                setReLabelAlliance.insert(rFoundLandData.ui64AllianceId);
            }
        }
    }

    m_mapLandData.insert(std::make_pair(idNewLand, stLandData));
    m_mapAllianceLand[spStaticMapObj->GetAllianceId()].insert(idNewLand);

    if (bReLabel)
    {
        LP_LOG_FAIL(X_OK == this->ReLabel(setReLabelAlliance));
    }

    return X_OK;
}

uint32 CAllianceLandMgr::ReLabel(const ALLIANCE_ID_SET& rReLabelAllianceSet, const LAND_ID_SET& rExcludeLandSet)
{
    // 1��������˵����б��id
    LAND_LABEL_ID_SET setDelLabel;
    for (const uint64 ui64AllianceId : rReLabelAllianceSet)
    {
        ALLIANCE_LAND_MAP::const_iterator iter = m_mapAllianceLand.find(ui64AllianceId);
        LP_IF(m_mapAllianceLand.end() == iter)
        {
            continue;
        }

        const LAND_ID_SET& rLandIdSet = iter->second;
        LP_IF(rLandIdSet.empty())
        {
            continue;
        }

        for (const LAND_ID idLand : rLandIdSet)
        {
            LAND_DATA_MAP::iterator iterFind = m_mapLandData.find(idLand);
            LP_IF(m_mapLandData.end() == iterFind)
            {
                continue;
            }

            ST_LAND_DATA& rFoundLandData = iterFind->second;
            SPTR_MAP_OBJ spFoundMapObj = rFoundLandData.wpMapObj.lock();
            LP_IF(nullptr == spFoundMapObj)
            {
                continue;
            }

            // ����Ȧ�ط�Χ
            ST_COORDINATE stLeftUpCoord;
            ST_COORDINATE stRightBtnCoord;
            uint32 uiRet = this->CalcEnclosureRange(stLeftUpCoord, stRightBtnCoord, spFoundMapObj);
            LP_IF(X_OK != uiRet)
            {
                continue;
            }

            // ��ձ��
            for (uint32 x = stLeftUpCoord.x; x <= stRightBtnCoord.x; ++x)
            {
                for (uint32 y = stLeftUpCoord.y; y <= stRightBtnCoord.y; ++y)
                {
                    LAND_LABEL_ID& rCurLabelId = m_arrLabelMap[x][y];
                    if ((idLand == m_arrLandMap[x][y]) && (0 != rCurLabelId))
                    {
                        setDelLabel.insert(rCurLabelId);
                        rCurLabelId = 0;
                    }
                }
            }
        }
    }

    // �ͷű��id
    for (LAND_LABEL_ID idLabel : setDelLabel)
    {
        m_arrUnionFind[idLabel] = 0;
        this->FreeLabelId(idLabel);
    }

    // 2����ʼ���
    for (const uint64 ui64AllianceId : rReLabelAllianceSet)
    {
        LP_LOG_FAIL(X_OK == this->Label(ui64AllianceId, rExcludeLandSet));
    }

    return X_OK;
}

uint32 CAllianceLandMgr::DelLandData(const LAND_ID idLand)
{
    // �鵽�����id������
    LAND_DATA_MAP::iterator iter = m_mapLandData.find(idLand);
    LP_IF(m_mapLandData.end() == iter)
    {
        return X_SERVER_ERROR;
    }

    // ��ȡ�����id�ĵ�ͼ����
    const ST_LAND_DATA& rLandData = iter->second;
    SPTR_MAP_OBJ spMapObj = rLandData.wpMapObj.lock();
    LP_IF(nullptr == spMapObj)
    {
        return X_SERVER_ERROR;
    }

    // У�������������
    LP_IF(0 == m_mapAllianceLand.count(rLandData.ui64AllianceId))
    {
        return X_SERVER_ERROR;
    }

    // У�������Ƿ��д����
    LP_IF(0 == m_mapAllianceLand[rLandData.ui64AllianceId].count(idLand))
    {
        return X_SERVER_ERROR;
    }

    // ����Ȧ�ط�Χ
    ST_COORDINATE stLeftUpCoord;
    ST_COORDINATE stRightBtnCoord;
    uint32 uiRet = this->CalcEnclosureRange(stLeftUpCoord, stRightBtnCoord, spMapObj);
    LP_IF(X_OK != uiRet)
    {
        return uiRet;
    }

    ALLIANCE_ID_SET setReLabelAlliance;
    setReLabelAlliance.insert(rLandData.ui64AllianceId);
    for (const LAND_ID id : rLandData.setOverlapLand)
    {
        LAND_DATA_MAP::iterator iterFind = m_mapLandData.find(id);
        LP_IF(m_mapLandData.end() == iterFind)
        {
            continue;
        }

        ST_LAND_DATA& rFoundLandData = iterFind->second;
        setReLabelAlliance.insert(rFoundLandData.ui64AllianceId);
        LP_LOG_FAIL(X_OK == this->UpdateLandId(setReLabelAlliance, idLand, rFoundLandData));

        rFoundLandData.setOverlapLand.erase(idLand);
    }

    // �����漰�������˵���ͨ��
    LAND_ID_SET setExcludeLand; // ���±��ʱ���ų������id
    setExcludeLand.insert(idLand);
    LP_LOG_FAIL(X_OK == this->ReLabel(setReLabelAlliance, setExcludeLand));

    // ���LandMap��Ҫɾ�������id
    for (uint32 x = stLeftUpCoord.x; x <= stRightBtnCoord.x; ++x)
    {
        for (uint32 y = stLeftUpCoord.y; y <= stRightBtnCoord.y; ++y)
        {
            LAND_ID& rCurLandId = m_arrLandMap[x][y];
            if (idLand == rCurLandId)
            {
                rCurLandId = 0;
            }
        }
    }

    this->FreeLandId(idLand);
    m_mapAllianceLand[rLandData.ui64AllianceId].erase(idLand);
    if (m_mapAllianceLand[rLandData.ui64AllianceId].empty())
    {
        m_mapAllianceLand.erase(rLandData.ui64AllianceId);
    }
    
    m_mapLandData.erase(idLand);

    return X_OK;
}

bool CAllianceLandMgr::IsConnected(const uint32 x1, const uint32 y1, const uint32 x2, const uint32 y2)
{
    // ���������Ч��
    LP_IF((0 == x1) || (0 == y1) || (0 == x2) || (0 == y2)
          || (DEFAULE_MAP_X_SIZE < x1) || (DEFAULE_MAP_Y_SIZE < y1)
          || (DEFAULE_MAP_X_SIZE < x2) || (DEFAULE_MAP_Y_SIZE < y2))
    {
        return false;
    }

    const LAND_LABEL_ID idLabel = union_find::find(m_arrUnionFind, m_arrLabelMap[x1][y1]);
    if (0 == idLabel)
    {
        return false;
    }

    return (idLabel == union_find::find(m_arrUnionFind, m_arrLabelMap[x2][y2]));
}

bool CAllianceLandMgr::IsConnected(const ST_COORDINATE& rCoord1, const ST_COORDINATE& rCoord2)
{
    return IsConnected(rCoord1.x, rCoord1.y, rCoord2.x, rCoord2.y);
}

uint32 CAllianceLandMgr::ClearAllianceAllLand(const uint64 ui64AllianceId)
{
    ALLIANCE_LAND_MAP::const_iterator iter = m_mapAllianceLand.find(ui64AllianceId);
    if (m_mapAllianceLand.end() == iter)
    {
        return X_OK;
    }

    ALLIANCE_ID_SET setReLabelAlliance;
    LAND_LABEL_ID_SET setDelLabel;
    const LAND_ID_SET& rLandIdSet = iter->second;
    for (const LAND_ID idLand : rLandIdSet)
    {
        LAND_DATA_MAP::iterator iterFind = m_mapLandData.find(idLand);
        LP_IF(m_mapLandData.end() == iterFind)
        {
            continue;
        }

        ST_LAND_DATA& rFoundLandData = iterFind->second;
        SPTR_MAP_OBJ spFoundMapObj = rFoundLandData.wpMapObj.lock();
        LP_IF(nullptr == spFoundMapObj)
        {
            continue;
        }

        ST_COORDINATE stLeftUpCoord;
        ST_COORDINATE stRightBtnCoord;
        uint32 uiRet = this->CalcEnclosureRange(stLeftUpCoord, stRightBtnCoord, spFoundMapObj);
        LP_IF(X_OK != uiRet)
        {
            continue;
        }

        // ��մ���صı��
        for (uint32 x = stLeftUpCoord.x; x <= stRightBtnCoord.x; ++x)
        {
            for (uint32 y = stLeftUpCoord.y; y <= stRightBtnCoord.y; ++y)
            {
                LAND_LABEL_ID& rCurLabelId = m_arrLabelMap[x][y];
                if ((idLand == m_arrLandMap[x][y]) && (0 != rCurLabelId))
                {
                    setDelLabel.insert(rCurLabelId);
                    rCurLabelId = 0;
                }
            }
        }

        for (const LAND_ID idOverlapLand : rFoundLandData.setOverlapLand)
        {
            LAND_DATA_MAP::iterator iter = m_mapLandData.find(idOverlapLand);
            LP_IF(m_mapLandData.end() == iter)
            {
                continue;
            }

            ST_LAND_DATA& rOverlapLandData = iter->second;
            setReLabelAlliance.insert(rOverlapLandData.ui64AllianceId);
            LP_LOG_FAIL(X_OK == this->UpdateLandId(setReLabelAlliance, idLand, rOverlapLandData));

            rOverlapLandData.setOverlapLand.erase(idLand);
        }

        // ���ɾ����LandMap
        for (uint32 x = stLeftUpCoord.x; x <= stRightBtnCoord.x; ++x)
        {
            for (uint32 y = stLeftUpCoord.y; y <= stRightBtnCoord.y; ++y)
            {
                LAND_ID& rCurLandId = m_arrLandMap[x][y];
                if (idLand == rCurLandId)
                {
                    rCurLandId = 0;
                }
            }
        }
    }

    // �ͷű��id
    for (LAND_LABEL_ID idLabel : setDelLabel)
    {
        m_arrUnionFind[idLabel] = 0;
        this->FreeLabelId(idLabel);
    }

    // �����漰���˵���ͨ��
    setReLabelAlliance.erase(ui64AllianceId);
    LP_LOG_FAIL(X_OK == this->ReLabel(setReLabelAlliance));

    for (const LAND_ID idLand : rLandIdSet)
    {
        this->FreeLandId(idLand);
        m_mapLandData.erase(idLand);
    }

    m_mapAllianceLand.erase(ui64AllianceId);

    return X_OK;
}

uint32 CAllianceLandMgr::UpdateLandId(OUT ALLIANCE_ID_SET& rReLabelAllianceSet, const LAND_ID idDelLand, ST_LAND_DATA& rNewLandData)
{
    SPTR_MAP_OBJ spNewLandMapObj = rNewLandData.wpMapObj.lock();
    LP_IF(nullptr == spNewLandMapObj)
    {
        return X_SERVER_ERROR;
    }

    ST_COORDINATE stLeftUpCoord;
    ST_COORDINATE stRightBtnCoord;
    uint32 uiRet = this->CalcEnclosureRange(stLeftUpCoord, stRightBtnCoord, spNewLandMapObj);
    LP_IF(X_OK != uiRet)
    {
        return X_SERVER_ERROR;
    }

    for (uint32 x = stLeftUpCoord.x; x <= stRightBtnCoord.x; ++x)
    {
        for (uint32 y = stLeftUpCoord.y; y <= stRightBtnCoord.y; ++y)
        {
            LAND_ID& rCurLandId = m_arrLandMap[x][y];
            if ((0 == rCurLandId) || (idDelLand == rCurLandId))
            {
                rCurLandId = rNewLandData.idLand;
                continue;
            }

            // �Ѿ����Լ�������
            if (rCurLandId == rNewLandData.idLand)
            {
                continue;
            }

            LAND_DATA_MAP::iterator iterFind = m_mapLandData.find(rCurLandId);
            LP_IF(m_mapLandData.end() == iterFind)
            {
                rCurLandId = rNewLandData.idLand;
                continue;
            }

            ST_LAND_DATA& rFoundLandData = iterFind->second;
            SPTR_MAP_OBJ spFoundMapObj = rFoundLandData.wpMapObj.lock();
            LP_IF(nullptr == spFoundMapObj)
            {
                rCurLandId = rNewLandData.idLand;
                continue;
            }

            rNewLandData.setOverlapLand.insert(rCurLandId);
            rFoundLandData.setOverlapLand.insert(rNewLandData.idLand);

            // ���ռ�����ȼ�
            if (this->CheckEnclosurePriority(spNewLandMapObj, spFoundMapObj))
            {
                rCurLandId = rNewLandData.idLand;
                rReLabelAllianceSet.insert(rFoundLandData.ui64AllianceId);
            }
        }
    }

    return X_OK;
}

uint32 CAllianceLandMgr::Label(const uint64 ui64AllianceId, const LAND_ID_SET& rExcludeLandSet)
{
    ALLIANCE_LAND_MAP::const_iterator iter = m_mapAllianceLand.find(ui64AllianceId);
    if (m_mapAllianceLand.end() == iter)
    {
        return X_SERVER_ERROR;
    }

    const LAND_ID_SET& rLandIdSet = iter->second;
    LP_IF(rLandIdSet.empty())
    {
        return X_SERVER_ERROR;
    }

    LAND_ID_SET setLandId = rLandIdSet;
    for (const LAND_ID id : rExcludeLandSet)
    {
        setLandId.erase(id);
    }

    for (const LAND_ID id : setLandId)
    {
        this->LabelOne(id, setLandId);
    }

    return X_OK;
}

void CAllianceLandMgr::LabelOne(const LAND_ID idLand, const LAND_ID_SET& rLandIdSet)
{
    LAND_DATA_MAP::iterator iterFind = m_mapLandData.find(idLand);
    LP_IF(m_mapLandData.end() == iterFind)
    {
        return;
    }

    ST_LAND_DATA& rFoundLandData = iterFind->second;
    SPTR_MAP_OBJ spFoundMapObj = rFoundLandData.wpMapObj.lock();
    LP_IF(nullptr == spFoundMapObj)
    {
        return;
    }

    ST_COORDINATE stLeftUpCoord;
    ST_COORDINATE stRightBtnCoord;
    uint32 uiRet = this->CalcEnclosureRange(stLeftUpCoord, stRightBtnCoord, spFoundMapObj);
    LP_IF(X_OK != uiRet)
    {
        return;
    }

    // ��ʼ���
    for (uint32 x = stLeftUpCoord.x; x <= stRightBtnCoord.x; ++x)
    {
        LAND_ID const* const pLandMapRow = m_arrLandMap[x];
        LAND_ID const* const pLandMapRowPrev = pLandMapRow - LAND_ARRAY_Y_SIZE;
        LAND_ID* const pLabelRow = m_arrLabelMap[x];
        LAND_ID* const pLabelRowPrev = pLabelRow - LAND_ARRAY_Y_SIZE;
        for (uint32 y = stLeftUpCoord.y; y <= stRightBtnCoord.y; ++y)
        {
            // �ж��Ƿ����Լ������
            if (0 != rLandIdSet.count(pLandMapRow[y]))
            {
                // ע�������1��ʼ

                if ((x > 1) && (0 != rLandIdSet.count(pLandMapRowPrev[y]))) // ��ߵ����Լ������
                {
                    if ((y > 1) && (0 != rLandIdSet.count(pLandMapRow[y - 1]))) // �ϱߵ����Լ����ֵ�
                    {
                        if (0 != pLabelRow[y]) // ��ǰ���б��id
                        {
                            if (0 == pLabelRowPrev[y]) // ��ߵ��ޱ��id
                            {
                                pLabelRowPrev[y] = pLabelRow[y];
                            }

                            if (0 == pLabelRow[y - 1]) // �ϱߵ�û�б��id
                            {
                                pLabelRow[y - 1] = pLabelRow[y];
                            }

                            // pLabelRow[y] = union_find::set_union(m_arrUnionFind, pLabelRow[y], union_find::set_union(m_arrUnionFind, pLabelRow[y - 1], pLabelRowPrev[y]));
                            union_find::set_union(m_arrUnionFind, pLabelRow[y], union_find::set_union(m_arrUnionFind, pLabelRow[y - 1], pLabelRowPrev[y]));
                        }
                        else // ��ǰ���ޱ��id
                        {
                            if (0 == pLabelRowPrev[y]) // ��ߵ��ޱ��id
                            {
                                if (0 != pLabelRow[y - 1]) // �ϱߵ��б��id
                                {
                                    pLabelRowPrev[y] = pLabelRow[y - 1];
                                }
                                else // ��ߵ��ޱ��id
                                {
                                    LAND_LABEL_ID idNewLabel = 0;
                                    LP_IF(X_OK != this->MallocLabelId(idNewLabel))
                                    {
                                        return;
                                    }

                                    pLabelRowPrev[y] = idNewLabel;
                                    m_arrUnionFind[idNewLabel] = idNewLabel;
                                }
                            }

                            if (0 == pLabelRow[y - 1]) // �ϱߵ��ޱ��id
                            {
                                if (0 != pLabelRowPrev[y]) // ��ߵ��б��id
                                {
                                    pLabelRow[y - 1] = pLabelRowPrev[y];
                                }
                                else // ��ߵ��ޱ��id
                                {
                                    LAND_LABEL_ID idNewLabel = 0;
                                    LP_IF(X_OK != this->MallocLabelId(idNewLabel))
                                    {
                                        return;
                                    }

                                    pLabelRow[y - 1] = idNewLabel;
                                    m_arrUnionFind[idNewLabel] = idNewLabel;
                                }
                            }

                            pLabelRow[y] = union_find::set_union(m_arrUnionFind, pLabelRow[y - 1], pLabelRowPrev[y]);
                        }
                    }
                    else // ��ǰ���������Լ�����أ����ǵ�ǰ�����Լ�������ڵ�ͼ�߽�
                    {
                        if (0 != pLabelRow[y]) // ��ǰ���б��id
                        {
                            if (0 == pLabelRowPrev[y]) // ��ߵ��ޱ��id
                            {
                                pLabelRowPrev[y] = pLabelRow[y];
                            }
                            else // ��ߵ��б��id
                            {
                                // pLabelRow[y] = union_find::set_union(m_arrUnionFind, pLabelRow[y], pLabelRowPrev[y]);
                                union_find::set_union(m_arrUnionFind, pLabelRow[y], pLabelRowPrev[y]);
                            }
                        }
                        else // ��ǰ���ޱ��id
                        {
                            if (0 == pLabelRowPrev[y]) // ��ߵ��ޱ��id
                            {
                                LAND_LABEL_ID idNewLabel = 0;
                                LP_IF(X_OK != this->MallocLabelId(idNewLabel))
                                {
                                    return;
                                }

                                pLabelRowPrev[y] = idNewLabel;
                                m_arrUnionFind[idNewLabel] = idNewLabel;
                            }

                            pLabelRow[y] = pLabelRowPrev[y];
                        }
                    }
                }
                else // ��ߵ㲻���Լ���أ����ǵ�ǰ�����Լ�������ڵ�ͼ�߽�
                {
                    if ((y > 1) && (0 != rLandIdSet.count(pLandMapRow[y - 1]))) // �ϱߵ����Լ������
                    {
                        if (0 != pLabelRow[y]) // ��ǰ���б��id
                        {
                            if (0 == pLabelRow[y - 1]) // �ϱߵ��ޱ��id
                            {
                                pLabelRow[y - 1] = pLabelRow[y];
                            }
                            else
                            {
                                // pLabelRow[y] = union_find::set_union(m_arrUnionFind, pLabelRow[y], pLabelRow[y - 1]);
                                union_find::set_union(m_arrUnionFind, pLabelRow[y], pLabelRow[y - 1]);
                            }
                        }
                        else // ��ǰ���ޱ��id
                        {
                            if (0 == pLabelRow[y - 1]) // �ϱߵ��ޱ��id
                            {
                                LAND_LABEL_ID idNewLabel = 0;
                                LP_IF(X_OK != this->MallocLabelId(idNewLabel))
                                {
                                    return;
                                }

                                pLabelRow[y - 1] = idNewLabel;
                                m_arrUnionFind[idNewLabel] = idNewLabel;
                            }

                            pLabelRow[y] = pLabelRow[y - 1];
                        }
                    }
                    else // �ϱߵ㡢��ߵ㶼�����Լ����
                    {
                        if (0 == pLabelRow[y]) // ��ǰ���ޱ��id
                        {
                            LAND_LABEL_ID idNewLabel = 0;
                            LP_IF(X_OK != this->MallocLabelId(idNewLabel))
                            {
                                return;
                            }

                            pLabelRow[y] = idNewLabel;
                            m_arrUnionFind[idNewLabel] = idNewLabel;
                        }
                    }
                }
            } // End ��ǰ�㲻���Լ������
        }
    }
}

bool CAllianceLandMgr::CheckMapObjData(const SPTR_MAP_OBJ& rMapObjSptr)
{
    LP_IF(nullptr == rMapObjSptr)
    {
        return false;
    }

    // ���˽����Ƿ��Ȧ��
    const E_MAP_OBJECT_TYPE eType = rMapObjSptr->GetType();
    if ((eType != E_MAP_OBJECT_TYPE_HARBOUR) && (eType != E_MAP_OBJECT_TYPE_FORTRESS) && (eType != E_MAP_OBJECT_TYPE_LIGHTHOUSE))
    {
        return false;
    }

    // ����MapObj���������Ƿ���ȷ
    ST_COORDINATE stOccupyCoord;
    rMapObjSptr->GetOccupyCoord(stOccupyCoord);
    if ((0 == stOccupyCoord.x) || (DEFAULE_MAP_X_SIZE < stOccupyCoord.x)
        || (0 == stOccupyCoord.y) || (DEFAULE_MAP_Y_SIZE < stOccupyCoord.y))
    {
        return false;
    }

    SPTR_STATIC_MAP_OBJECT spStaticMapObj = std::dynamic_pointer_cast<CStaticMapObject>(rMapObjSptr);
    LP_IF(nullptr == spStaticMapObj)
    {
        return false;
    }

    // ���˾�̬����Ƿ�������
    LP_IF(0 == spStaticMapObj->GetAllianceId())
    {
        return false;
    }

    return true;
}

bool CAllianceLandMgr::CheckEnclosurePriority(const SPTR_MAP_OBJ& rMapObjSptr1, const SPTR_MAP_OBJ& rMapObjSptr2)
{
    LP_IF((nullptr == rMapObjSptr1) || (nullptr == rMapObjSptr2))
    {
        return false;
    }

    // �ۿ����ȼ����
    if (rMapObjSptr1->GetType() == E_MAP_OBJECT_TYPE_HARBOUR)
    {
        return true;
    }

    SPTR_STATIC_MAP_OBJECT spStaticMapObj1 = std::dynamic_pointer_cast<CStaticMapObject>(rMapObjSptr1);
    LP_IF(nullptr == spStaticMapObj1)
    {
        // MapObj1ת��ʧ�ܣ�Ĭ��MapObj2���ȼ�����
        return false;
    }

    SPTR_STATIC_MAP_OBJECT spStaticMapObj2 = std::dynamic_pointer_cast<CStaticMapObject>(rMapObjSptr2);
    LP_IF(nullptr == spStaticMapObj2)
    {
        // MapObj2ת��ʧ�ܣ�Ĭ��MapObj1���ȼ�����
        return true;
    }

    // ��Чʱ��������ȼ�����ߣ�ͬ������Чʱ��ʱ��ȡ���ڳ����ȼ��ض���
    return spStaticMapObj1->GetActiveTime() <= spStaticMapObj2->GetActiveTime();
}

uint32 CAllianceLandMgr::CalcEnclosureRange(OUT ST_COORDINATE& rLeftUpCoord, OUT ST_COORDINATE& rRightBtnCoord, const SPTR_MAP_OBJ& rMapObjSptr)
{
    LP_IF(nullptr == rMapObjSptr)
    {
        return X_SERVER_ERROR;
    }

    uint32 uiRange = 0;
    switch (rMapObjSptr->GetType())
    {
    case E_MAP_OBJECT_TYPE_FORTRESS: // Ҫ��
        {
            uiRange = FORTRESS_ENCLOSURE_RANGE;
        }
        break;
    case E_MAP_OBJECT_TYPE_LIGHTHOUSE: // ����
        {
            uiRange = LIGHTHOUSE_ENCLOSURE_RANGE;
        }
        break;
    case E_MAP_OBJECT_TYPE_HARBOUR: // �ۿ�
        {
            uiRange = HARBOR_ENCLOSURE_RANGE;
        }
        break;
    default:
        {
            return X_SERVER_ERROR;
        }
        break;
    }

    ST_COORDINATE stOccupyCoord;
    rMapObjSptr->GetOccupyCoord(stOccupyCoord);
    if (stOccupyCoord.x > uiRange)
    {
        rLeftUpCoord.x = (std::max)(static_cast<uint32>(1), stOccupyCoord.x - uiRange);
    }
    else
    {
        rLeftUpCoord.x = 1;
    }

    if (stOccupyCoord.y > uiRange)
    {
        rLeftUpCoord.y = (std::max)(static_cast<uint32>(1), stOccupyCoord.y - uiRange);
    }
    else
    {
        rLeftUpCoord.y = 1;
    }

    rRightBtnCoord.x = (std::min)(static_cast<uint32>(DEFAULE_MAP_X_SIZE), stOccupyCoord.x + uiRange);
    rRightBtnCoord.y = (std::min)(static_cast<uint32>(DEFAULE_MAP_Y_SIZE), stOccupyCoord.y + uiRange);

    return X_OK;
}

uint32 CAllianceLandMgr::MallocLandId(OUT LAND_ID& rNewLandId)
{
    if (!m_queFreeLandId.empty())
    {
        rNewLandId = m_queFreeLandId.front();
        m_queFreeLandId.pop();
        return X_OK;
    }

    LP_IF(0 == m_LandIdDist)
    {
        return X_SERVER_ERROR;
    }

    rNewLandId = m_LandIdDist++;
    return X_OK;
}

void CAllianceLandMgr::FreeLandId(const LAND_ID idLand)
{
    m_queFreeLandId.push(idLand);
}

uint32 CAllianceLandMgr::MallocLabelId(OUT LAND_LABEL_ID& rNewLabelId)
{
    if (!m_queFreeLabelId.empty())
    {
        rNewLabelId = m_queFreeLabelId.front();
        m_queFreeLabelId.pop();
        return X_OK;
    }

    if (LAND_UNION_FIND_ARR_SIZE <= m_LabelIdDist)
    {
        return X_SERVER_ERROR;
    }

    rNewLabelId = m_LabelIdDist++;
    return X_OK;
}

void CAllianceLandMgr::FreeLabelId(const LAND_LABEL_ID idLabel)
{
    m_queFreeLabelId.push(idLabel);
}

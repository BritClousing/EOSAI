
#include "GameData.h"

GameData::GameData()
{
    g_pGameData = this;
/*
    SharedMemory_Poi poi;
    poi.m_fPosX = 50.0f; poi.m_fPosY = 25.0f; poi.m_iOwner = 1;
    m_PoiList[0] = poi;//.push_back(poi);
    poi.m_fPosX = 25.0f; poi.m_fPosY = 50.0f; poi.m_iOwner = 2;
    //m_PoiList.push_back(poi);
    m_PoiList[1] = poi;
    m_iNumberOfPoi = 2;
*/
}

GameData* g_pGameData;

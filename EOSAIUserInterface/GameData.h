#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <QList>
#include "PoiListModel.h"
#include "StatusListModel.h"
#include "../EOSAI/SharedMemory.h"

class GameData
{
    public:
        GameData();

        QString m_strPlayerName[MAX_NUMBER_OF_PLAYERS];

        StatusListModel m_StatusListModel;
        PoiListModel    m_PoiListModel;

        int m_iNumberOfPoi = 0;
        int m_iNumberOfPlayers = 0;
        SharedMemory_Poi  m_PoiList[MAX_NUMBER_OF_POI_IN_LIST];  // The GameData object owns this data
};

extern GameData* g_pGameData;

#endif


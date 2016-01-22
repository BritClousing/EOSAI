
#include "InterprocLoop.h"
#include <QDebug>

#define BOOST_DATE_TIME_NO_LIB
//#include "C:/Programming/boost_1_59_0/boost/interprocess/shared_memory_object.hpp"
//#include "boost/interprocess/shared_memory_object.hpp"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include "../EOSAI/SharedMemory.h"

#include "StatusLine.h"
#include "PoiListModel.h"
#include "GameMapImage.h"
#include "GameData.h"

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QQuickWindow>


using namespace boost::interprocess;

shared_memory_object* g_pSharedMemoryObject = NULL; // shm;
mapped_region* g_pMappedRegion = NULL;
SharedMemoryBuffer* g_pSharedMemoryBuffer = NULL;
extern QQmlApplicationEngine* g_pQmlApplicationEngine;
/*
void CopyString(char10& src, char10& dest){ CopyString( src.m_szText, dest.m_szText, 10); }
void CopyString(char20& src, char20& dest){ CopyString( src.m_szText, dest.m_szText, 20); }
void CopyString(const char* src, char* dest, int iSize)
{
    int iStringLength = strlen(src);
    int iCharactersToCopy = min(iStringLength, iSize - 1);

    memcpy(dest, src, iCharactersToCopy);
    dest[iCharactersToCopy] = 0;
}
*/
/*
QHash<int, QByteArray> StatusModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[SizeRole] = "size";
    return roles;
}
*/

void InterprocLoop::BeginListening()
{
    //g_pQmlApplicationEngine->rootContext()->connect(  )
    //playerComboBoxChanged(int iPlayer);
    //g_pQmlApplicationEngine->rootContext()->

    //QObject::connect( g_pQmlApplicationEngine->rootContext(), SIGNAL(playerComboBoxChanged()), this, SLOT(onPlayerComboBoxChanged()));

    //QObject *topLevel = engine.rootObjects().value(0);
    //QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    // connect our QML signal to our C++ slot
    //QObject::connect(window, SIGNAL(submitTextField(QString)), &handleTextField, SLOT(handleSubmitTextField(QString)));

    QObject* pTopLevel = g_pQmlApplicationEngine->rootObjects().value(0);
    QQuickWindow* pWindow = qobject_cast<QQuickWindow *>(pTopLevel);
    //QObject::connect( pWindow, SIGNAL(playerComboBoxChanged(int)), this, SLOT(onPlayerComboBoxChanged(int)));
    QObject::connect( pWindow, SIGNAL(processSingleTurn()), this, SLOT(onProcessSingleTurn()));
    QObject::connect( pWindow, SIGNAL(autoprocessTurns(bool)), this, SLOT(onAutoprocessTurns(bool)));

    ProcessTimer();

    m_Timer.start(1000);
    connect( &m_Timer, SIGNAL(timeout()), this, SLOT(ProcessTimer()));
    m_Timer.start();
}

void InterprocLoop::onProcessSingleTurn()
{
    qDebug() << "InterprocLoop::onProcessTurn()";
    g_pSharedMemoryBuffer->m_iGameShouldPauseAtTheEndOfTurn = g_pSharedMemoryBuffer->m_iCurrentTurn;
}

void InterprocLoop::onAutoprocessTurns(bool bChecked)
{
    //qDebug() << "InterprocLoop::onAutoprocessTurns() - MAXINT32=" << MAXINT32;
    //if( bChecked.toBool() )
    if( bChecked )
    {
        g_pSharedMemoryBuffer->m_iGameShouldPauseAtTheEndOfTurn = MAXINT32;
    }
    else
    {
        g_pSharedMemoryBuffer->m_iGameShouldPauseAtTheEndOfTurn = 0;
    }
    qDebug() << "InterprocLoop::onAutoprocessTurns() - g_pSharedMemoryBuffer->m_iGameShouldPauseAtTheEndOfTurn=" << g_pSharedMemoryBuffer->m_iGameShouldPauseAtTheEndOfTurn;
}
/*
void InterprocLoop::onPlayerComboBoxChanged(int iPlayer)
{
    //qDebug() << "InterprocLoop::onPlayerComboBoxChanged() " << iPlayer;
    // Change the values
    /-*
    if( iPlayer == 0 ) // Common Data
    {
        g_pQmlApplicationEngine->rootContext()->setContextProperty("m_bDisplayCommonData", true );

        QString strWarningsAndErrors;
        strWarningsAndErrors += QString("0 Warnings, 0 Errors\n");

        g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCommonWarningsAndErrors", strWarningsAndErrors );
        //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCommonUnitTemplates", "25 Unit Templates\n    Infantry (Class 1)\n    Tank (Class 1)" );
    }
    else
    {
        g_pQmlApplicationEngine->rootContext()->setContextProperty("m_bDisplayCommonData", false );

        bool bPlayerXIsAI = ( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == AIPlayer );
        QString strProcessingStateColor;
        QString strDetails;
        g_pSharedMemoryBuffer->lock.wait();
        {
            strDetails += QString("Player %1 \"%2\"\n").arg(iPlayer).arg(g_pSharedMemoryBuffer->m_Players[iPlayer].m_strPlayerName);
            strDetails += QString("Type: %1\n").arg( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == AIPlayer ? "AI" : "Human" );
            if( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == AIPlayer )
            {
                strDetails += QString("Intelligence: %1/3\n").arg(g_pSharedMemoryBuffer->m_Players[iPlayer].m_iIntelligence3);
                strDetails += QString("Personality:\n");
                strDetails += QString("    Holds Grudges\n");
                strDetails += QString("    30% Economy, 20% Technology, 50% Military\n");
                strDetails += QString("    Talkative");
            }
            strProcessingStateColor = InterprocLoop::GetPlayerProcessingStateColor( iPlayer );
        }
        g_pSharedMemoryBuffer->lock.post();

        //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strPlayerXDetails", strDetails );
        g_pQmlApplicationEngine->rootContext()->setContextProperty("m_bPlayerXIsAI", bPlayerXIsAI );
        g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strPlayerXProcessingStateColor", strProcessingStateColor );
    }
    *-/
}
*/
void InterprocLoop::ProcessTimer()
{
    try
    {
        // create a shared memory object.
        if( g_pSharedMemoryObject == NULL )
        {
            g_pSharedMemoryObject = new shared_memory_object(open_only, "EOSAISharedMemory",read_write);
            //g_pSharedMemoryObject = new shared_memory_object(open_only, "shared_memory",read_write);
        }

        // Map the whole shared memory in this process
        if( g_pMappedRegion == NULL )
        {
            g_pMappedRegion = new mapped_region( *g_pSharedMemoryObject, read_write );
        }

        // Obtain the shared structure
        if( g_pSharedMemoryBuffer == NULL )
        {
            void* pAddr = g_pMappedRegion->get_address();
            g_pSharedMemoryBuffer = static_cast<SharedMemoryBuffer*>(pAddr);
        }

        g_pQmlApplicationEngine->rootContext()->setContextProperty("m_bEOSAISharedMemoryFileExists", true );

        {
            // If "reader" is 0, wait. If "reader" is 1, then continue (and decrement the number).
            //wait until a number gets available
            //data->reader.wait();
            // If "lock" is 0, wait. If "lock" is 1, then continue (and decrement the number).
            //g_pSharedMemoryBuffer->lock.wait();
            //boost::posix_time::ptime
            if( false )// g_pSharedMemoryBuffer->lock.try_wait() == false )
            {
                static int iPass1 = 0; iPass1++;
                qDebug() << "InterprocLoop (Reading Data Failed)" << iPass1;
                return;
            }

            static int iPass2 = 0; iPass2++;
            qDebug() << "InterprocLoop (Reading Data)" << iPass2;

            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strEOSAIVersion", g_pSharedMemoryBuffer->m_strEOSAIVersion );
            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strEOSAIBuildDate", g_pSharedMemoryBuffer->m_strEOSAIBuildDate );

            unsigned long long qwTimestamp = g_pSharedMemoryBuffer->m_qwTimestamp;
            QDateTime currentTimestamp = QDateTime::currentDateTime();
            qint64 currentTimestampInMs = currentTimestamp.toMSecsSinceEpoch();
            qint64 currentTimestampInS = currentTimestampInMs / 1000;
            qint64 gap = currentTimestampInS - qwTimestamp;
            //qDebug() << "currentTimestampInMs=" << currentTimestampInS;
            //qDebug() << "qwTimestamp=" << qwTimestamp;
            //qDebug() << "gap=" << gap;
            if( gap < 3 )
            {
                g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strConnectionStatusColor", "#AFA" ); //  "#FF6" : "#FAA"
                g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strConnectionState", "Connected." );
            }
            else
            {
                g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strConnectionStatusColor", "#FF6" ); //  "#FF6" : "#FAA"
                g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strConnectionState", "AI data found. No AI is currently running." );
            }

            if( g_pSharedMemoryBuffer->m_eAIOverallProcessingState == EnumAIOverallProcessingState_Undefined ){}
            if( g_pSharedMemoryBuffer->m_eAIOverallProcessingState == EnumAIOverallProcessingState_Processing )
            {
                g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strOverallProcessingStateColor", "#AFA" );
                g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strOverallProcessingStateText", "Processing" );
                //qDebug() << "m_eAIOverallProcessingState (Processing)";
            }
            if( g_pSharedMemoryBuffer->m_eAIOverallProcessingState == EnumAIOverallProcessingState_Idle )
            {
                g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strOverallProcessingStateColor", "#FAA" );
                g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strOverallProcessingStateText", "Idle" );
                //qDebug() << "m_eAIOverallProcessingState (Idle)";
            }
            //qDebug() << "m_eAIOverallProcessingState" << g_pSharedMemoryBuffer->m_eAIOverallProcessingState;

            // Map
            GameMapImage::s_iGameMapWidth = g_pSharedMemoryBuffer->m_iCommonData_MapWidth;
            GameMapImage::s_iGameMapHeight = g_pSharedMemoryBuffer->m_iCommonData_MapHeight;
            memcpy( GameMapImage::s_GameMap, g_pSharedMemoryBuffer->m_GameMap, sizeof(RGBPixel)*GAME_MAP_IMAGE_WIDTH*GAME_MAP_IMAGE_HEIGHT );
            GameMapImage::s_bUpdateImage = true;

            std::wstring testStr2 = L"testStr2❤☀☆☂☻♞☯☭☢";
            //std::wstring testStr2 = L"testStr2";
            //std::wstring str2 = data->str2;

            //std::string strPlayer1Name = data->m_strPlayerNames[1];

            //print the original value
            //qDebug() << i << " Original Value: " << data->value << " " << data->str << " " << data->str2[0] << "," << data->str2[1] << " " << *(data->str2);
            //qDebug() << i << " Original Value: " << data->value << " " << data->str << " " << " " << str2.c_str() << "/" << testStr2;

            // Output: "286  Original Value:  63   Brit3     0x564c8a8 / 0x34c0278"
            //qDebug() << i << " Original Value: " << data->value << " " << data->str << " " << " " << str2.c_str() << "/" << testStr2.c_str();

            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCurrentTurn", QString("%1").arg(g_pSharedMemoryBuffer->m_iCurrentTurn) );
            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iNumberOfPlayers", g_pSharedMemoryBuffer->m_iNumberOfPlayers );
            //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iNumberOfPlayers", QString("%1").arg(g_pSharedMemoryBuffer->m_iNumberOfPlayers) );
            g_pGameData->m_iNumberOfPlayers = g_pSharedMemoryBuffer->m_iNumberOfPlayers;

            int iPlayer = 2;
            QString strDetails;
            strDetails += QString("Player %1 \"%2\"\n").arg(iPlayer).arg(g_pSharedMemoryBuffer->m_Players[iPlayer].m_czPlayerName.m_szText);
            strDetails += QString("Type: %1\n").arg( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == AIPlayer ? "AI" : "Human" );
            if( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == AIPlayer )
            {
                strDetails += QString("Intelligence: %1/3\n").arg(g_pSharedMemoryBuffer->m_Players[iPlayer].m_iIntelligence3);
                strDetails += QString("Personality:\n");
                strDetails += QString("    Holds Grudges\n");
                strDetails += QString("    30% Economy, 20% Technology, 50% Military\n");
                strDetails += QString("    Talkative");
            }
            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strPlayer2Details", strDetails );

            //qDebug() << "m_strEOSAIVersion: " << g_pSharedMemoryBuffer->m_strEOSAIVersion
            //         << " m_iPlayerNamesUpdate: " << g_pSharedMemoryBuffer->m_iPlayerNamesUpdate
            //         << " m_iGameState: " << g_pSharedMemoryBuffer->m_iGameState
            //         << " m_iTurn: " << g_pSharedMemoryBuffer->m_iTurn
            //         << " m_iNumberOfPlayers: " << g_pSharedMemoryBuffer->m_iNumberOfPlayers;


            //
            //StatusLineModel statusLineModel;
            if( m_bReadAllData || g_pSharedMemoryBuffer->m_bStatusListUpdated )
            {
                g_pSharedMemoryBuffer->m_bStatusListUpdated = false;
                //qDebug() << "g_pSharedMemoryBuffer->m_bStatusListUpdated";

                // Parse the status
                QString strStatus = QString::fromLocal8Bit( g_pSharedMemoryBuffer->m_czCommonData_Status.m_szText );
                //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_szCommonData_Status", g_pSharedMemoryBuffer->m_szCommonData_Status );
                //qDebug() << "g_pSharedMemoryBuffer->m_bStatusListUpdated, strStatus=" << strStatus;

                //g_pGameData->m_StatusListModel.m_StatusLineList.clear();
                QStringList statusList = strStatus.split("\n");
                int iStatusListSize = statusList.count();
                for( int i=0; i<iStatusListSize; i++ )
                {
                    //if( statusList[i] != "" ) statusLineModel.m_StatusLineList.append( StatusLine(statusList[i]) );
                    StatusLine statusLine(statusList[i]);
                    g_pGameData->m_StatusListModel.m_StatusLineList.append( statusLine );
                    //PoiLine poiLine;
                    //if( iPlayer == 0 ){ poiLine.m_strName = QString("Unowned"); }
                    //else{               poiLine.m_strName = QString("Player %1 \"%2\"").arg(iPlayer).arg( g_pGameData->m_strPlayerName[iPlayer] ); }
                    //m_PoiLineList.append(poiLine);
                    //if( i == 0 ){ qDebug() << "statusLine=" << statusLine.m_strIcon << statusLine.m_strText; }
                }
            }
            g_pGameData->m_StatusListModel.recreateList();
            g_pQmlApplicationEngine->rootContext()->setContextProperty( "statusListModel", &g_pGameData->m_StatusListModel );
            //

            g_pQmlApplicationEngine->rootContext()->setContextProperty("testModel2", strDetails );

            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iNumberOfPoi", g_pSharedMemoryBuffer->m_iNumberOfPoi );
            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_szCommonData_Poi", g_pSharedMemoryBuffer->m_szCommonData_Poi );

//            PoiListModel poiListModel;
            int iMaxPoi = min( g_pSharedMemoryBuffer->m_iNumberOfPoi, MAX_NUMBER_OF_POI_IN_LIST );
            g_pGameData->m_iNumberOfPoi = iMaxPoi;
            for( int i=0; i<iMaxPoi; i++ )
            {
                g_pGameData->m_PoiList[i].m_fPosX = g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_fPosX;
                g_pGameData->m_PoiList[i].m_fPosY = g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_fPosY;
                g_pGameData->m_PoiList[i].m_iOwner = g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_iOwner;
                CopyString( g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_szName, g_pGameData->m_PoiList[i].m_szName );
                //CopyString( g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_szName, g_pGameData->m_PoiList[i].m_szName, 10 );
                CopyString( g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_szMapDesc, g_pGameData->m_PoiList[i].m_szMapDesc );
                CopyString( g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_szLongDesc, g_pGameData->m_PoiList[i].m_szLongDesc );
                CopyString( g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_szPoiType, g_pGameData->m_PoiList[i].m_szPoiType );

                //qDebug() << "g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_szName=" << g_pSharedMemoryBuffer->m_CommonData_Poi[i].m_szName.m_szText;
            }
            //g_pQmlApplicationEngine->rootContext()->setContextProperty( "poiListModel", QVariant::fromValue(poiListModel) );
//            g_pQmlApplicationEngine->rootContext()->setContextProperty( "poiListModel", &poiListModel );
            g_pGameData->m_PoiListModel.recreateList();

            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iCommonData_NumberOfUnitTemplates", g_pSharedMemoryBuffer->m_iCommonData_NumberOfUnitTemplates );
            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_szCommonData_UnitTemplates", g_pSharedMemoryBuffer->m_szCommonData_UnitTemplates );

            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iCommonData_NumberOfTechnologies", g_pSharedMemoryBuffer->m_iCommonData_NumberOfTechnologies );
            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_szCommonData_Technologies", g_pSharedMemoryBuffer->m_szCommonData_Technologies );

            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iCommonData_MapWidth", g_pSharedMemoryBuffer->m_iCommonData_MapWidth );
            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iCommonData_MapHeight", g_pSharedMemoryBuffer->m_iCommonData_MapHeight );

            //

            QStringList playerDropdownList;
            playerDropdownList.append("Common Data");
            for( int iPlayer=1; iPlayer<=g_pSharedMemoryBuffer->m_iNumberOfPlayers; iPlayer++ )
            {
               if( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == AIPlayer )
               {
                   g_pQmlApplicationEngine->rootContext()->setContextProperty( QString("m_strPlayer%1Type").arg(iPlayer), "AI" );
/*
                   qDebug() << "   Player" << iPlayer << ": " << g_pSharedMemoryBuffer->m_Players[iPlayer].m_strPlayerName;
                   qDebug() << "     Intelligence: " << g_pSharedMemoryBuffer->m_Players[iPlayer].m_iIntelligence3;
                   qDebug() << "     ProcessingState: " << g_pSharedMemoryBuffer->m_Players[iPlayer].GetProcessingStateText();
                   qDebug() << "     m_eProcessingState=" << g_pSharedMemoryBuffer->m_Players[iPlayer].m_eProcessingState;
                   */
               }
               if( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == HumanPlayer )
               {
                   g_pQmlApplicationEngine->rootContext()->setContextProperty( QString("m_strPlayer%1Type").arg(iPlayer), "Human" );
/*
                   qDebug() << "   Player" << iPlayer << ": " << g_pSharedMemoryBuffer->m_Players[iPlayer].m_strPlayerName;
                   qDebug() << "     Intelligence: Human";
                   */
               }
               QString strColor = GetPlayerProcessingStateColor( iPlayer );
               /*
               QString strColor;
               if( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == HumanPlayer ){ strColor = "#CCC"; }
               else
               {
                   /-*
                   Unprocessed,
                   AnalyzingMap,
                   Desires,
                   TacticalProjects,
                   ProcessingMessages,
                   Complete,
                   *-/
                   if(      g_pSharedMemoryBuffer->m_Players[iPlayer].m_eProcessingState == Unprocessed ) strColor = "#FAA"; // Red
                   else if( g_pSharedMemoryBuffer->m_Players[iPlayer].m_eProcessingState == Complete    ) strColor = "#AFA"; // Green
                   else                                                                                   strColor = "#FFA"; // Yellow
               }
               g_pQmlApplicationEngine->rootContext()->setContextProperty( QString("m_strPlayer%1ProcessingColor").arg(iPlayer), strColor );
               */
               g_pQmlApplicationEngine->rootContext()->setContextProperty( QString("m_strPlayer%1ProcessingColor").arg(iPlayer), strColor );

               //color: m_strPlayer1Type == "Human" ? "#CCC" : "#AFA"
               //color: m_strPlayer1ProcessingColor

               g_pGameData->m_strPlayerName[iPlayer] = g_pSharedMemoryBuffer->m_Players[iPlayer].m_czPlayerName.m_szText;

               QString strPlayer = QString("Player %1 \"%2\"").arg(iPlayer).arg(g_pSharedMemoryBuffer->m_Players[iPlayer].m_czPlayerName.m_szText);
               playerDropdownList.append(strPlayer);
            }

            //model: [ "Common Data", "Player 1 (...)", "Player 2 (...)", "Player 3 (...)" ]
            g_pQmlApplicationEngine->rootContext()->setContextProperty("m_PlayerDropdownList", QVariant::fromValue(playerDropdownList));

            //std::cout << i << " Original Value: " << data->value << " " << data->str << "\n";
            //std::cout << "..a  " << str2.c_str() << "\n";
            //std::wcout << "..b  " << testStr2 << "\n"; //  L"testStr2❤☀☆☂☻♞☯☭☢"; -> testStr2â¤â..a
            //std::cout << "  str: " << data->str << "\n";
            //iPass++;

            //create the sqrt of the original value
            //data->value = sqrt(data->value);
            //data->value = data->str;

            //writer can print sqrt
            // Increment the "writer" value. The other process will handle the data if "writer" is set to 1.
            //data->writer.post();
            // Increment the "lock" value. The other process will handle the data if "lock" is set to 1.
            m_bReadAllData = false;
            g_pSharedMemoryBuffer->lock.post();
        }
    }
    catch (interprocess_exception e)
    {
        const char* pWhat = e.what();
        qDebug() << "Exception pWhat=" << pWhat;

        g_pQmlApplicationEngine->rootContext()->setContextProperty("m_bEOSAISharedMemoryFileExists", false );
    }

/*
    //while(true)
    {
        // If "reader" is 0, wait. If "reader" is 1, then continue (and decrement the number).
        //wait until a number gets available
        //data->reader.wait();
        // If "lock" is 0, wait. If "lock" is 1, then continue (and decrement the number).
        data->lock.wait();

       std::wstring testStr2 = L"testStr2❤☀☆☂☻♞☯☭☢";
       //std::wstring testStr2 = L"testStr2";
       //std::wstring str2 = data->str2;

       //std::string strPlayer1Name = data->m_strPlayerNames[1];


       //print the original value
       //qDebug() << i << " Original Value: " << data->value << " " << data->str << " " << data->str2[0] << "," << data->str2[1] << " " << *(data->str2);
       //qDebug() << i << " Original Value: " << data->value << " " << data->str << " " << " " << str2.c_str() << "/" << testStr2;

       // Output: "286  Original Value:  63   Brit3     0x564c8a8 / 0x34c0278"
       //qDebug() << i << " Original Value: " << data->value << " " << data->str << " " << " " << str2.c_str() << "/" << testStr2.c_str();

       qDebug() << iPass
                << " data->m_strEOSAIVersion: " << data->m_strEOSAIVersion
                << " data->m_iPlayerNamesUpdate: " << data->m_iPlayerNamesUpdate
                << " data->m_iGameState: " << data->m_iGameState
                << " data->m_iTurn: " << data->m_iTurn;
                //<< " Player1Name:" << strPlayer1Name.c_str()
                //<< " Player2Name:" << data->m_strPlayerNames[2]
                //<< " data->m_iValue: " << data->m_iValue
                //<< " data->str: " << data->str;


       for( int iPlayer=0; iPlayer<MAX_NUMBER_OF_PLAYERS; iPlayer++ )
       {
           if( data->m_Players[iPlayer].m_ePlayerType == AIPlayer )
           {
               qDebug() << "   Player" << iPlayer << ": " << data->m_Players[iPlayer].m_strPlayerName;
               qDebug() << "     Intelligence: " << data->m_Players[iPlayer].m_fIntelligence01;
               qDebug() << "     ProcessingState: " << data->m_Players[iPlayer].GetProcessingStateText();
           }
           if( data->m_Players[iPlayer].m_ePlayerType == HumanPlayer )
           {
               qDebug() << "   Player" << iPlayer << ": " << data->m_Players[iPlayer].m_strPlayerName;
               qDebug() << "     Intelligence" << ": Human";
           }
       }

       //std::cout << i << " Original Value: " << data->value << " " << data->str << "\n";
       //std::cout << "..a  " << str2.c_str() << "\n";
       std::wcout << "..b  " << testStr2 << "\n"; //  L"testStr2❤☀☆☂☻♞☯☭☢"; -> testStr2â¤â..a
       //std::cout << "  str: " << data->str << "\n";
       iPass++;

       //create the sqrt of the original value
       //data->value = sqrt(data->value);
       //data->value = data->str;

       //writer can print sqrt
       // Increment the "writer" value. The other process will handle the data if "writer" is set to 1.
       //data->writer.post();
       // Increment the "lock" value. The other process will handle the data if "lock" is set to 1.
       data->lock.post();
    }
    */
}

QString InterprocLoop::GetPlayerProcessingStateColor( int iPlayer )
{
    QString strColor;
    if( g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType == HumanPlayer ){ strColor = "#00FFFFFF"; } //#CCC"; }
    else
    {
        /*
        Unprocessed,
        AnalyzingMap,
        Desires,
        TacticalProjects,
        ProcessingMessages,
        Complete,
        */
        if(      g_pSharedMemoryBuffer->m_Players[iPlayer].m_eProcessingState == Unprocessed ) strColor = "#FAA"; // Red
        else if( g_pSharedMemoryBuffer->m_Players[iPlayer].m_eProcessingState == Complete    ) strColor = "#AFA"; // Green
        else                                                                                   strColor = "#FFA"; // Yellow
    }
    return strColor;
    //g_pQmlApplicationEngine->rootContext()->setContextProperty( QString("m_strPlayer%1ProcessingColor").arg(iPlayer), strColor );
}


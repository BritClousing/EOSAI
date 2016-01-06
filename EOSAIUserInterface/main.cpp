/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <qdebug.h>

#define BOOST_DATE_TIME_NO_LIB
//#include "C:/Programming/boost_1_59_0/boost/interprocess/shared_memory_object.hpp"
//#include "boost/interprocess/shared_memory_object.hpp"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

//#include "MyModel.h"
#include "../EOSAI/SharedMemory.h"
#include "GameData.h"
#include "InterprocLoop.h"
#include "PoiListModel.h"

#include < stdio.h >
#include < stdlib.h >

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
using namespace boost::interprocess;

/*
#define MAX_NUMBER_OF_PLAYERS 10+1
struct shared_memory_buffer {
    //writer initialized with one to start
    //reader have to wait
    //shared_memory_buffer() : writer(1), reader(0), value(0){}
    shared_memory_buffer() : lock(1), value(0){}
    //interprocess_semaphore writer, reader;
    interprocess_semaphore lock;

    // Update flags
    int     m_iPlayerNamesUpdate = 0;

    int     m_iGameState = 0; // 0 = no game, 1 = game happening AI processing, 2 = game happening AI not processing
    int     m_iTurn = 0;
    char    m_strPlayerNames[MAX_NUMBER_OF_PLAYERS][20]; // store upto 20 characters

    int     value;
    char    str[100];
    //wchar_t str2[100];
};
*/
/*
#define min(a, b) (((a) < (b)) ? (a) : (b))

//memcpy(data->m_strPlayerNames[1], "Rommel-Rommel-Rommel-Rommel", 20);
//strncpy_s(data->m_strPlayerNames[1], "Rommel-Rommel-Rommel-Rommel", 20);
//strncpy(data->m_strPlayerNames[1], "Rommel-Rommel", 20);
void string_copy(char* dest, const char* src, int iSize)
{
    int iStringLength = strlen(src);
    int iCharactersToCopy = min(iStringLength, iSize - 1);

    memcpy(dest, src, iCharactersToCopy);
    dest[iCharactersToCopy] = 0;
}

class EOSAIAIPlayer
{
    public:
        EOSAIAIPlayer(){ m_strPlayerName[0] = 0; }
        void SetPlayerName( std::string str ){ string_copy(m_strPlayerName, str.c_str(), 20); }
    //private:
        bool  m_bIsActive = false;
        char  m_strPlayerName[20];
        float m_fIntelligence01 = 0.5f;
};

#define MAX_NUMBER_OF_PLAYERS 10+1
class shared_memory_buffer
{
public:
    //writer initialized with one to start
    //reader have to wait
    //shared_memory_buffer() : writer(1), reader(0), value(0){}
    shared_memory_buffer() : lock(1), m_iValue(0){}
    //interprocess_semaphore writer, reader;
    interprocess_semaphore lock;

    //void SetPlayerName(int iPlayer, std::string str){ m_iPlayerNamesUpdate++; string_copy(m_strPlayerNames[iPlayer], str.c_str(), 20); }
    char    m_strEOSAIVersion[20];// = "0.1.333";

    // Update flags
    int     m_iPlayerNamesUpdate = 0;

    int     m_iGameState = 1; // 0 = no game, 1 = game happening AI processing, 2 = game happening AI not processing
    int     m_iTurn = 2;
    EOSAIAIPlayer  m_Players[MAX_NUMBER_OF_PLAYERS];
    //char    m_strPlayerNames[MAX_NUMBER_OF_PLAYERS][20]; // store upto 20 characters

    int     m_iValue;
    char    str[100];
};
*/
bool m_bEOSAISharedMemoryFileExists = false;
QQmlApplicationEngine* g_pQmlApplicationEngine = NULL;
#include "GameMapImage.h"
#include "StatusLine.h"

int main(int argc, char *argv[])
{
    using namespace boost::interprocess;
    GameData gameData;

    //QtQuickControlsApplication app(argc, argv);
    QApplication app(argc, argv);

    //InterprocLoop loop;

    //qmlRegisterType<TextBalloon>(uri, 1, 0, "TextBalloon");
    qmlRegisterType<GameMapImage>("GameMapLib", 1, 0, "GameMapImage");

    //QQmlApplicationEngine* engine = new QQmlApplicationEngine(this);
    //QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));
    //QQmlApplicationEngine engine;
    g_pQmlApplicationEngine = new QQmlApplicationEngine();

    //MyModel model;
    //g_pQmlApplicationEngine->rootContext()->setContextProperty("theModel", &model);
    //
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strApplicationHeader", "EOSAI User Interface");
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_bEOSAISharedMemoryFileExists", false );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strEOSAIVersion", "" );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strEOSAIBuildDate", "" );

    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCurrentTurn", "-" );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strOverallProcessingStateColor", "#CCC" );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strOverallProcessingStateText", "Idle" );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strConnectionState", "No AI is currently running." );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strConnectionStatusColor", "#FAA" ); //  "#FF6" : "#FAA"

    //
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iNumberOfPlayers", 0 );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iNumberOfPoi", 0 );

    //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_bDisplayCommonData", true );

    //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCommonWarningsAndErrorsCount", "(0)" );
    //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCommonStatus", "" );
    //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCommonWarningsAndErrors", "" );
    //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCommonUnitTemplates", "" );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iCommonData_NumberOfUnitTemplates", 0 );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_szCommonData_UnitTemplates", "" );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iCommonData_NumberOfTechnologies", 0 );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_szCommonData_Technologies", "" );

    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iCommonData_MapWidth", 0 );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_iCommonData_MapHeight", 0 );

    //PoiListModel poiListModel;
    //g_pQmlApplicationEngine->rootContext()->setContextProperty( "poiListModel", &poiListModel );
    g_pQmlApplicationEngine->rootContext()->setContextProperty( "poiListModel", &g_pGameData->m_PoiListModel );

    //
    //StatusLineModel model2;
    //model.m_StatusLineList.append(StatusLine("i-Turn"));
    //model.m_StatusLineList.append(StatusLine("Polar bear", "Large"));
    //model.m_StatusLineList.append(StatusLine("Quoll", "Small"));
    //g_pQmlApplicationEngine->rootContext()->setContextProperty( "statusLineModel", &model2 );
    g_pQmlApplicationEngine->rootContext()->setContextProperty( "statusListModel", &g_pGameData->m_StatusListModel );

    QStringList playerDropdownList;
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_PlayerDropdownList", QVariant::fromValue(playerDropdownList));

    for( int iPlayer=1; iPlayer<MAX_NUMBER_OF_PLAYERS; iPlayer++ )
    {
        QString str = QString("m_strPlayer%1Type").arg(iPlayer);
        g_pQmlApplicationEngine->rootContext()->setContextProperty(str, "" );
        str = QString("m_strPlayer%1ProcessingColor").arg(iPlayer);
        g_pQmlApplicationEngine->rootContext()->setContextProperty(str, "" );
    }

    QString strStatus;
    //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_szCommonData_Status", "" );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_szCommonData_Poi", "" );

    //strStatus  = "Infantry (Class 1)\n";
    //strStatus += "Tank (Class 1)";
    //g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strCommonUnitTemplates", strStatus );

    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strPlayer1Details", "" );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strPlayer2Details", "" );

    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_bPlayerXIsAI", false );
    g_pQmlApplicationEngine->rootContext()->setContextProperty("m_strPlayerXProcessingStateColor", "" );

    g_pQmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));

    InterprocLoop loop;
    loop.BeginListening();
/*
    //qDebug() << "Test";
    try
    {
        //create a shared memory object.
        //shared_memory_object shm(open_only, "shared_memory",read_write);
        g_pSharedMemoryObject = new shared_memory_object(open_only, "shared_memory",read_write);

        //Map the whole shared memory in this process
        //mapped_region region(shm,read_write);
        //mapped_region region( *g_pSharedMemoryObject, read_write );
        g_pMappedRegion = new mapped_region( *g_pSharedMemoryObject, read_write );

        //get the region address
        //void* addr = region.get_address();
        void* pAddr = g_pMappedRegion->get_address();

        //Obtain the shared structure
        SharedMemoryBuffer* data = static_cast<SharedMemoryBuffer*>(pAddr);

        engine.rootContext()->setContextProperty("m_bEOSAISharedMemoryFileExists", true );
        engine.rootContext()->setContextProperty("m_strEOSAIVersion", data->m_strEOSAIVersion );

        int iPass=0;

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
    }
    catch (interprocess_exception e)
    {
        qDebug() << "Exception";
        //std::cout << "Exception";
        const char* pWhat = e.what();
        //int g = 0;
        qDebug() << "Exception pWhat=" << pWhat;

        m_bEOSAISharedMemoryFileExists = false;
        engine.rootContext()->setContextProperty("m_bEOSAISharedMemoryFileExists", false );
    }
*/
/*
#ifdef DO_NOT_COMPILE
    try
    {
        //engine.rootContext()->setContextProperty("m_strApplicationHeader", "childContext");
        shared_memory_object shm (open_only, "EOSAISharedMemory", read_only); // "The system cannot find the file specified."

        //qDebug() << "ChildProcess-2";

        //engine.rootContext()->setContextProperty("m_strApplicationHeader", "childContext-2");
        //qDebug() << "ChildProcess-3";

        //Map the whole shared memory in this process
        mapped_region region(shm, read_only);
        //qDebug() << "ChildProcess-EverythingGood";

        //Check that memory was initialized to 1
        char *mem = static_cast<char*>(region.get_address());
        int iValue = mem[0];

        QString str = QString("x%1y").arg( iValue );
        engine.rootContext()->setContextProperty("m_strApplicationHeader", "EOSAI User Interface-"+str );

        m_bEOSAISharedMemoryFileExists = true;
        engine.rootContext()->setContextProperty("m_bEOSAISharedMemoryFileExists", true );
    }
    catch (interprocess_exception e)
    {
        qDebug() << "Exception";
        //std::cout << "Exception";
        const char* pWhat = e.what();
        int g = 0;
        qDebug() << "Exception pWhat=" << pWhat;

        m_bEOSAISharedMemoryFileExists = false;
        engine.rootContext()->setContextProperty("m_bEOSAISharedMemoryFileExists", false );
    }
#endif

#ifdef DO_NOT_COMPILE
    if(argc == 1){  //Parent process
        qDebug() << "ParentProcess";
        engine.rootContext()->setContextProperty("m_strApplicationHeader", "parentContext");
       //Remove shared memory on construction and destruction
       struct shm_remove
       {
          shm_remove() { shared_memory_object::remove("EOSAISharedMemory"); }
          ~shm_remove(){ shared_memory_object::remove("EOSAISharedMemory"); }
       } remover;

       //Create a shared memory object.
       shared_memory_object shm (create_only, "EOSAISharedMemory", read_write);

       //Set size
       shm.truncate(1000);

       //Map the whole shared memory in this process
       mapped_region region(shm, read_write);

       //Write all the memory to 1
       std::memset(region.get_address(), 3, region.get_size());

       //Launch child process
       std::string s(argv[0]); s += " child ";
       if(0 != std::system(s.c_str()))
       {
          return 1;
       }
    }
    else{
        qDebug() << "ChildProcess-1";
        engine.rootContext()->setContextProperty("m_strApplicationHeader", "childContext");
       //Open already created shared memory object.
       shared_memory_object shm (open_only, "EOSAISharedMemory", read_only);

       engine.rootContext()->setContextProperty("m_strApplicationHeader", "childContext-2");

       //Map the whole shared memory in this process
       mapped_region region(shm, read_only);
       qDebug() << "ChildProcess-EverythingGood";

       //Check that memory was initialized to 1
       char *mem = static_cast<char*>(region.get_address());
       int iValue = mem[0];

       QString str = QString("x%1y").arg( iValue );
       engine.rootContext()->setContextProperty("m_strApplicationHeader", "childContext-EverythingGood-"+str );

       //for(std::size_t i = 0; i < region.get_size(); ++i)
       //{
       //    QString str = QString("x%1y").arg( *mem );
       //    engine.rootContext()->setContextProperty("m_strApplicationHeader", "childContext-EverythingGood-"+str );
       //   if(*mem++ != 1)
       //   {
       //      return 1;   //Error checking memory
       //   }
       //}

       //engine.rootContext()->setContextProperty("m_strApplicationHeader", "childContext-EverythingGood");
    }
#endif
*/

    /*
    // Shared Memory
    try
    {
        qDebug() << "Getting shared memory";
        //std::cout << "Getting shared memory";

        //Open already created shared memory object.
        shared_memory_object shm(open_only, "Global\MySharedMemory", read_only);
        //shared_memory_object shm(open_only, "EOSAISharedMemory", read_only);
        qDebug() << "Getting shared memory-2";

        //Map the whole shared memory in this process
        mapped_region region(shm, read_only);
        qDebug() << "Getting shared memory-3";

        //Check that memory was initialized to 1
        char *mem = static_cast<char*>(region.get_address());
        for (std::size_t i = 0; i < region.get_size(); ++i)
        {
            if (*mem++ != 1)
            {
                return 1;   //Error checking memory
            }
        }
    }
    catch (interprocess_exception e)
    {
        qDebug() << "Exception";
        //std::cout << "Exception";
        const char* pWhat = e.what();
        int g = 0;
        qDebug() << "Exception pWhat=" << pWhat;
    }
    */

    return app.exec();
}

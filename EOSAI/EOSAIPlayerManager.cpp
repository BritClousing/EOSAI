
#include <stdafx.h>
#include "EOSAIPlayerManager.h"
#include "EOSAILogFile.h"
//#include "EOSAIPlayerManager.h"
#include "AIPlayer.h"
#include "EOSAIGamePlayer.h"
#include "EOSAICommonData.h"
//#include "GameAppState.h"
#include "EOSAISettings.h"
#include "EOSAIInterface.h"
//#include "WorldDescServer.h"

CEOSAIPlayerManager* g_pAIPlayerManager = NULL;
CEOSAILogFile g_LogFile;

CEOSAIPlayerManager::CEOSAIPlayerManager()
{
	g_LogFile.Initialize( _T("AILogFile.txt") );

	m_pEOSAIInterface = NULL;

	//m_eCurrentState = enumWaitingForServerToPlayerUpdate;
	//m_bAutosaveAndLocalPlayerTurnReplayEnded = true;
	//m_bWaitForAutosaveAndLocalPlayerTurnReplay = false;
	m_bWaitForAutosave = false;

	m_bDebugPause = false;

	m_bThreadIsRunning = false;
	m_bThreadShouldBeKilled = false;
	m_bThreadShouldBePaused = true;
	m_bThreadShouldBePaused_ForSaveGame = false;
	m_bThreadIsPaused = true;
	m_iProcessingAIPlayer = 0;

	m_AIPlayersArray.SetSize( EOSAI_MAX_NUMBER_OF_PLAYERS+1 );
	for( long iPlayer=0; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		m_AIPlayersArray[iPlayer] = NULL;
	}
	g_pAIPlayerManager = this;
}

CEOSAIPlayerManager::~CEOSAIPlayerManager()
{
	DeleteAIPlayers();
}

void CEOSAIPlayerManager::SetEOSAIInterface( EOSAI::CInterface* pInterface )
{
	m_pEOSAIInterface = pInterface;
	//g_pEOSAIInterface = m_pEOSAIInterface;
}

// Serialize/Deserialize
//
void CEOSAIPlayerManager::Serialize( CEOSAISerial* pSerial )
{
	long iVersion = 1;
	pSerial->Serialize( iVersion );

	long iNumberOfAIPlayers = m_AIPlayersArray.m_iSize;
	pSerial->Serialize( iNumberOfAIPlayers );
	for( long iPlayer=1; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		//CEOSAIBCDumbArray1D< EOSAI::AIPlayer* >  m_AIPlayersArray;
		EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		pAIPlayer->Serialize( pSerial );
	}
}

void CEOSAIPlayerManager::Deserialize( CEOSAISerial* pSerial )
{
	long iVersion = 0;
	pSerial->Deserialize( iVersion );

	long iNumberOfAIPlayers = 0;
	pSerial->Deserialize( iNumberOfAIPlayers );
	m_AIPlayersArray.SetSize( iNumberOfAIPlayers );
	for( long iPlayer=1; iPlayer<iNumberOfAIPlayers; iPlayer++ )
	{
		ASSERT( false );
		EOSAI::AIPlayer* pAIPlayer = new EOSAI::AIPlayer();
		//CEOSAIBCDumbArray1D< EOSAI::AIPlayer* >  m_AIPlayersArray;
		//EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		m_AIPlayersArray[iPlayer] = pAIPlayer;
		pAIPlayer->Deserialize( pSerial );
	}
}

void CEOSAIPlayerManager::SetNumberOfPlayers( long iPlayers )
{
}

void CEOSAIPlayerManager::DeleteAIPlayer( long iPlayer )
{
	EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[ iPlayer ];
	if( m_iProcessingAIPlayer == iPlayer ) // Shut it down after we finish processing
	{
		pAIPlayer->SetFlag_ShutdownAIPlayer( true );
		return;
	}

	// If we're not processing it, then delete it now
	m_AIPlayersArray[iPlayer] = NULL;
	delete pAIPlayer;
}


void CEOSAIPlayerManager::DeleteAIPlayers()
{
	//while( m_AIPlayers.IsEmpty() == FALSE ){ delete m_AIPlayers.RemoveHead(); }
	for( long iPlayer=1; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		if( pAIPlayer )
		{
			delete pAIPlayer;
			m_AIPlayersArray[iPlayer] = NULL;
		}
	}
}

void CEOSAIPlayerManager::AddAIPlayer( EOSAI::AIPlayer* pAIPlayer )
{
	long iPlayer = pAIPlayer->GetPlayerNumber();
	//m_AIPlayers.AddTail( pAIPlayer );
	ASSERT( m_AIPlayersArray[ iPlayer ] == NULL );
	m_AIPlayersArray[ iPlayer ] = pAIPlayer;
}

EOSAI::AIPlayer* CEOSAIPlayerManager::GetAIPlayer( long iAIPlayer )
{
	return m_AIPlayersArray[ iAIPlayer ];
}
//CList< CAIPlayer* >*  CEOSAIPlayerManager::GetAIPlayerList(){ return &m_AIPlayers; }

void CEOSAIPlayerManager::WaitUntilAIPlayerIsRemoved( long iPlayer )
{
	while( m_AIPlayersArray[ iPlayer ] )
	{
		Sleep( 10 );
	}
	/*
	bool bPlayerHasBeenRemoved = false;
	while( bPlayerHasBeenRemoved == false )
	{
		// Wait until the AIPlayer is removed
		POSITION pos = m_AIPlayers.GetHeadPosition();
		while( pos )
		{
			CAIPlayer* pAIPlayer = m_AIPlayers.GetNext( pos );
			if( pAIPlayer->GetPlayerNumber() == iPlayer )
			{
				bPlayerHasBeenRemoved = false;
			}
		}
	}
	*/
}

long  CEOSAIPlayerManager::GetNumberOfAIPlayersWhoAreReadyToSendTurn()
{
	long iCount = 0;
	for( long iPlayer=1; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		if( pAIPlayer )
		{
			EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
			if( pPlayer &&
				pPlayer->IsAlive() &&
				pAIPlayer->IsReadyToSendTurn() ) iCount++;
		}
	}
	return iCount;
}

bool  CEOSAIPlayerManager::AllAIPlayersAreReadyToSendTurn()
{
	for( long iPlayer=1; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		if( pAIPlayer )
		{
			EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
			if( pPlayer &&
				pPlayer->IsAlive() &&
				pAIPlayer->IsReadyToSendTurn() == false ) return false;
		}
	}
	return true;
}

bool  CEOSAIPlayerManager::AllAIPlayersAreReadyToSendTurnOrHaveSentTurn()
{
	for( long iPlayer=1; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		if( pAIPlayer )
		{
			EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
			if( pPlayer && pPlayer->IsAlive() )
			{
				if( pAIPlayer->m_eProcessingState != EOSAI::AIPlayer::enumWaitingForServerToPlayerUpdate &&
					pAIPlayer->IsReadyToSendTurn() == false )
				{
					return false;
				}
			}
		}
	}
	return true;
}


/*
void  CEOSAIPlayerManager::Incoming_AutosaveAndLocalPlayerTurnReplayEnded()
{
	for( long iPlayer=1; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		CAIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		if( pAIPlayer )
		{
			pAIPlayer->Incoming_AutosaveAndLocalPlayerTurnReplayEnded();
		}
	}
}
*/

//
/*
void  CEOSAIPlayerManager::CreateAIPlayerRands()
{
	for( long iPlayer=1; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		if( pAIPlayer )
		{
			pAIPlayer->SetRand( rand() );
		}
	}
}
*/
/*
CAIPlayer*  CEOSAIPlayerManager::GetAIPlayer( long iPlayer )
{
	POSITION pos = m_AIPlayers.GetHeadPosition();
	while( pos )
	{
		CAIPlayer* pAIPlayer = m_AIPlayers.GetNext( pos );
		if( pAIPlayer->GetPlayer()->GetPlayerNumber() == iPlayer )
		{
			return pAIPlayer;
		}
	}
	return NULL;
}
*/
bool  CEOSAIPlayerManager::GetDebugPause()
{
	//#ifdef _DEBUG
	return m_bDebugPause;
	//#endif
	//return false;
}

UINT CEOSAIPlayerManager_ThreadEntranceFunction( LPVOID pParam )
{
	CEOSAIPlayerManager* pAIPlayerManager = (CEOSAIPlayerManager*) pParam;
	//pAIPlayerManager->PauseThread( true );
	pAIPlayerManager->Loop();
	return 0;
}

void  CEOSAIPlayerManager::InstanciateThreadAndPauseIt()
{
	ASSERT( m_bThreadIsRunning == false );
	if( m_bThreadIsRunning == false )
	{
		m_bThreadIsRunning = true;
		m_bThreadShouldBeKilled = false;
		m_bThreadShouldBePaused = true;
		m_bThreadShouldBePaused_ForSaveGame = false;
		m_bThreadIsPaused = true;

		CWinThread* m_pThread = AfxBeginThread(
			CEOSAIPlayerManager_ThreadEntranceFunction,
			(LPVOID) this,
			//THREAD_PRIORITY_NORMAL // this is one point below THREAD_PRIORITY_NORMAL (which is 0)
			THREAD_PRIORITY_NORMAL - 1 // this is one point below THREAD_PRIORITY_NORMAL (which is 0)
			//int nPriority = THREAD_PRIORITY_NORMAL,
			//UINT nStackSize = 0,
			//DWORD dwCreateFlags = 0,
			//LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL 
		);
	}
}
/*
void  CEOSAIPlayerManager::PauseThread( bool b )
{
	m_bThreadShouldBePaused = b;
}
*/
void  CEOSAIPlayerManager::Loop()
{
	/*
	m_bThreadIsRunning = true;
	m_bThreadShouldBeKilled = false;
	m_bThreadShouldBePaused = true;
	m_bThreadShouldBePaused_ForSaveGame = false;
	m_bThreadIsPaused = true;
	*/
	ASSERT( m_pEOSAIInterface != NULL );
	ASSERT( g_pEOSAIInterface != NULL );
	//g_pEOSAIInterface = m_pEOSAIInterface;
	while( true )
	{
		if( m_bThreadShouldBeKilled ) break;
		if( m_bThreadShouldBePaused || m_bThreadShouldBePaused_ForSaveGame )
		{
			m_bThreadIsPaused = true;
		}
		else
		{
			m_bThreadIsPaused = false;
		}

		/*
		if( m_bThreadShouldBePaused != m_bThreadIsPaused )
		{
			m_bThreadIsPaused = m_bThreadShouldBePaused;
		}
		*/
		if( m_bThreadIsPaused )
		{
			Sleep( 100 );
			continue;
		}
		if( m_bWaitForAutosave )//m_bWaitForAutosaveAndLocalPlayerTurnReplay )//m_bAutosaveAndLocalPlayerTurnReplayEnded == false )
		{
			Sleep( 100 );
			continue;
		}
		//if( g_pGameAppState == NULL ){ break; }
		//if( g_pCommonState->GetWorldDescServer()->GetGameHasEnded() ){ break; }
		//if( g_pGameAppState->GetEntireGameIsOver() ){ break; }
		//while( g_pGameAppState->GetCurrentWorldDesc()->GetReplayLastTurn() ){ Sleep( 1000 ); }

		//if( m_eCurrentState == enumWaitingForServerToPlayerUpdate ){ Sleep( 100 ); continue; }

		// How many players are there?
		long iNumberOfPlayers = 0;
		for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
		{
			EOSAI::AIPlayerDesc* pAIPlayerDesc = m_pEOSAIInterface->GetAIPlayerDesc( iPlayer );
			if( pAIPlayerDesc )
			{
				iNumberOfPlayers = iPlayer;
			}
		}

		// Look over the AIPlayerDesc objects, create AIPlayer2 objects if they don't exist
		for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
		{
			EOSAI::AIPlayerDesc* pAIPlayerDesc = m_pEOSAIInterface->GetAIPlayerDesc( iPlayer );
			if( pAIPlayerDesc )
			{
				ASSERT( pAIPlayerDesc->GetPlayerNumber() == iPlayer );

				// Create an AIPlayer?
				EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
				if( pAIPlayer == NULL )
				{
					EOSAI::CGamePlayer* pGamePlayer = m_pEOSAIInterface->GetGamePlayer(iPlayer);
					ASSERT( pGamePlayer );

					EOSAI::AIPlayer* pAIPlayer = new EOSAI::AIPlayer( iNumberOfPlayers, pGamePlayer, pAIPlayerDesc );
					AddAIPlayer( pAIPlayer );
				}
			}
		}
		
		//if( g_pEOSAIInterface->m_bCommonAIObjectsHaveBeenCreated == false )
		if( g_pEOSAICommonData->GetNeedToRebuildData() )
		{
			// Make sure the AIData is up to date
			g_pEOSAICommonData->SetNeedToRebuildData( false ); //m_bCommonAIObjectsHaveBeenCreated = true;
			g_pEOSAIInterface->CreateGameRules();
			g_pEOSAIInterface->CreateTechTree();
			g_pEOSAIInterface->CreateAIRegionsAndMultiRegions();
			g_pEOSAIInterface->CreateAIGeoMap();
			g_pEOSAIInterface->CreateAIObjects();
			g_pEOSAIInterface->UpdateCurrentTurn();
		}

		for( long iPlayer=1; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
		{
			m_iProcessingAIPlayer = 0;
			EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
			if( pAIPlayer == NULL ) continue;

			Sleep( 10 ); // When switching between players, transfer to other threads
			if( m_bDebugPause ){ continue; }
			m_iProcessingAIPlayer = iPlayer;

			if (pAIPlayer->m_eProcessingState == EOSAI::AIPlayer::enumWaitingForServerToPlayerUpdate) { continue; }
			if( pAIPlayer->GetGamePlayer()->IsAlive() == false ){ continue; }
			if( m_bThreadShouldBeKilled ) break;
			if( m_bThreadShouldBePaused ) break;
			if( m_bThreadShouldBePaused_ForSaveGame ) break;
			if( m_bThreadIsPaused ) break;

			if( pAIPlayer->GetFlag_ShutdownAIPlayer() )
			{
				m_AIPlayersArray[iPlayer] = NULL;
				delete pAIPlayer;
				continue;
			}

			//Sleep(20000);
			//CMemoryState MemoryState1, MemoryState2, DiffState;
			//MemoryState1.Checkpoint();

			//Beep( 200,200 );
			pAIPlayer->Process();
			//Beep( 400,200 );

			//for( long i=0; i<1000; i++ ){ Invalidate(); RedrawWindow(); }
			//MemoryState2.Checkpoint();
			//DiffState.Difference( MemoryState1, MemoryState2 );
			//DiffState.DumpStatistics();

			if( pAIPlayer->GetFlag_ShutdownAIPlayer() )
			{
				m_AIPlayersArray[iPlayer] = NULL;
				delete pAIPlayer;
			}

			if( m_bThreadShouldBeKilled ){ break; }
			if( m_bThreadShouldBePaused ){ continue; }
		}
		m_iProcessingAIPlayer = 0;

		/*
		POSITION pos = m_AIPlayers.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;

			CAIPlayer* pAIPlayer = m_AIPlayers.GetNext( pos );
			if( pAIPlayer->GetPlayer()->GetPlayerHasBeenEliminated() )
			{
				continue;
			}
			if( pAIPlayer->GetFlag_ShutdownAIPlayer() )
			{
				m_AIPlayers.RemoveAt( prevPos );
				delete pAIPlayer;
				continue;
			}

			pAIPlayer->Process();

			if( m_bThreadShouldBeKilled ){ break; }
			if( m_bThreadShouldBePaused ){ continue; }
			m_iProcessingAIPlayer = 0;
			Sleep( 0 ); // When switching between players, transfer to other threads
		}
		*/
		Sleep( 100 );
	}
	m_bThreadIsRunning = false;
}

void  CEOSAIPlayerManager::ShutdownThread()
{
	m_bThreadShouldBeKilled = true;

	// Tell all the AIPlayers to shutdown
	for( long iPlayer=0; iPlayer<m_AIPlayersArray.m_iSize; iPlayer++ )
	{
		EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
		if( pAIPlayer )
		{
			pAIPlayer->SetFlag_ShutdownAIPlayer( true );
		}
	}
	while( m_bThreadIsRunning )
	{
		Sleep( 10 );
	}
}

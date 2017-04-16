
#include <stdafx.h>
#include "EOSAIPlayerManager.h"
#include "EOSAILogFile.h"
//#include "EOSAIPlayerManager.h"
#include "AIPlayer.h"
#include "EOSAIGamePlayer.h"
#include "EOSAICommonData.h"
//#include "GameAppState.h"
#include "EOSAISettings.h"
#include "MessageToAI_WarWasDeclared.h"
#include "EOSAIPlayerInteraction_DeclaredWar.h"
#include "EOSAIInterface.h"
//#include "UserInterface.h"
//#include "MessageToAI.h"
#include "Interproc.h"

extern EOSAI::CInterface* g_pEOSAIInterface;
CEOSAIPlayerManager* g_pAIPlayerManager = NULL;
CEOSAILogFile g_LogFile;

//
//Create a shared memory object.
//boost::interprocess::shared_memory_object* g_pInterprocSharedMemoryObject = NULL;// (create_only, "EOSAISharedMemory", read_write);
//boost::interprocess::mapped_region* g_pInterprocMappedRegion = NULL; // (shm, read_write);


CEOSAIPlayerManager::CEOSAIPlayerManager()
{
	g_LogFile.Initialize(_T("AILogFile.txt"));

	// Get interprocess communication setup
	//Interproc interproc;
	//interproc.SetValues();
/*
	// Boost::interprocess
	{
		ASSERT(g_pInterprocSharedMemoryObject == NULL);
		ASSERT(g_pInterprocMappedRegion == NULL);
		//pShm = NULL;// (create_only, "EOSAISharedMemory", read_write);
		//pRegion = NULL; // (shm, read_write);

		using namespace boost::interprocess;

		//qDebug() << "ParentProcess";
		//engine.rootContext()->setContextProperty("MyController", "parentContext");
		//Remove shared memory on construction and destruction
		struct shm_remove
		{
			shm_remove() { shared_memory_object::remove("EOSAISharedMemory"); }
			//~shm_remove(){ shared_memory_object::remove("EOSAISharedMemory"); }
		} remover;

		//Create a shared memory object.
		g_pInterprocSharedMemoryObject = new shared_memory_object(create_only, "EOSAISharedMemory", read_write);

		//Set size
		g_pInterprocSharedMemoryObject->truncate(1000);

		//Map the whole shared memory in this process
		g_pInterprocMappedRegion = new mapped_region(*g_pInterprocSharedMemoryObject, read_write);

		//Write all the memory to 1
		std::memset(g_pInterprocMappedRegion->get_address(), 67, g_pInterprocMappedRegion->get_size());

		boost::interprocess::string str;

		//Launch child process
		//std::string s(argv[0]); s += " child ";
		//if (0 != std::system(s.c_str()))
		//{
		//	return 1;
		//}

		/-*
		//Remove shared memory on construction and destruction
		struct shm_remove
		{
			shm_remove() { shared_memory_object::remove("EOSAISharedMemory"); }
			~shm_remove(){ shared_memory_object::remove("EOSAISharedMemory"); }
		} remover;

		//Create a shared memory object.
		shared_memory_object shm(create_only, "EOSAISharedMemory", read_write);

		//Set size
		shm.truncate(1000);

		//Map the whole shared memory in this process
		mapped_region region(shm, read_write);

		//Write all the memory to 1
		std::memset(region.get_address(), 1, region.get_size());
		*-/

		//UserInterface* pUserInterface = new UserInterface(); //::Create()
		//pUserInterface->Initialize();
		//UserInterface::Instance()->Initialize();
		//UserInterface::Instance()->DisplayUI();
	}
*/

	//m_pEOSAIInterface = NULL;

	//m_eCurrentState = enumWaitingForServerToPlayerUpdate;
	//m_bAutosaveAndLocalPlayerTurnReplayEnded = true;
	//m_bWaitForAutosaveAndLocalPlayerTurnReplay = false;
	//m_bWaitForAutosave = false;

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
/*
void CEOSAIPlayerManager::SetEOSAIInterface( EOSAI::CInterface* pInterface )
{
	m_pEOSAIInterface = pInterface;
	//g_pEOSAIInterface = m_pEOSAIInterface;
}
*/
/*
void CEOSAIPlayerManager::InitializeInterprocessCommunication()
{
	Interproc::Initialize();
}

void CEOSAIPlayerManager::ShutdownInterprocessCommunication()
{
	Interproc::Shutdown();
}
*/
/*
void CEOSAIPlayerManager::ShowUI(bool b)
{
	//void CInterface::ShowUI(bool b)
	UserInterface::Instance()->Initialize();
}
*/
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

bool  CEOSAIPlayerManager::GetAllAIPlayersAreReadyToSendTurn()
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

bool  CEOSAIPlayerManager::GetAllAIPlayersAreReadyToSendTurnOrHaveSentTurn()
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

		// Periodically update the shared memory
		Interproc::UpdateSharedMemory();

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
		/*
		if( m_bWaitForAutosave )//m_bWaitForAutosaveAndLocalPlayerTurnReplay )//m_bAutosaveAndLocalPlayerTurnReplayEnded == false )
		{
			Sleep( 100 );
			continue;
		}
		*/
		//if( g_pGameAppState == NULL ){ break; }
		//if( g_pCommonState->GetWorldDescServer()->GetGameHasEnded() ){ break; }
		//if( g_pGameAppState->GetEntireGameIsOver() ){ break; }
		//while( g_pGameAppState->GetCurrentWorldDesc()->GetReplayLastTurn() ){ Sleep( 1000 ); }

		//if( m_eCurrentState == enumWaitingForServerToPlayerUpdate ){ Sleep( 100 ); continue; }

		// How many players are there?
		long iNumberOfPlayers = 0;
		for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
		{
			EOSAI::AIPlayerDesc* pAIPlayerDesc = g_pEOSAIInterface->GetAIPlayerDesc( iPlayer );
			if( pAIPlayerDesc )
			{
				iNumberOfPlayers = iPlayer;
			}
		}

		// Look over the AIPlayerDesc objects, create AIPlayer2 objects if they don't exist
		for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
		{
			EOSAI::AIPlayerDesc* pAIPlayerDesc = g_pEOSAIInterface->GetAIPlayerDesc( iPlayer );
			if( pAIPlayerDesc )
			{
				ASSERT( pAIPlayerDesc->GetPlayerNumber() == iPlayer );

				// Create an AIPlayer?
				EOSAI::AIPlayer* pAIPlayer = m_AIPlayersArray[iPlayer];
				if( pAIPlayer == NULL )
				{
					EOSAI::CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer(iPlayer);
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
			g_pEOSAICommonData->SetNeedToRebuildData( false );
			int h1 = 0;
			g_pEOSAIInterface->CreateGameRules();
			int h2 = 0;
			g_pEOSAIInterface->CreateTechTree();
			g_pEOSAIInterface->CreateAIRegionsAndMultiRegions();
			g_pEOSAIInterface->CreateAIGeoMap();
			g_pEOSAIInterface->CreateAIObjects();
			g_pEOSAIInterface->UpdateCurrentTurn();
		}

		g_pEOSAICommonData->ProcessPlayerInteractionsIfNecessary();

		// Process any global messages
		while (g_pEOSAIInterface &&
			g_pEOSAIInterface->GetMessagesToAI() &&
			g_pEOSAIInterface->GetMessagesToAI()->IsEmpty() == FALSE)
		{
			EOSAI::Message* pMessageToAI = g_pEOSAIInterface->GetMessagesToAI()->RemoveHead();
			ProcessMessage(pMessageToAI);
		}

		// Update the ownership map?
		if ( g_pEOSAICommonData->m_OwnershipMapLastUpdatedTurn != g_pEOSAIInterface->GetCurrentTurn())
		{
			g_pEOSAIInterface->CopyOwnershipMapsToEOSAI(
				g_pEOSAICommonData->GetCurrentOwnershipPlayerMap8(),
				g_pEOSAICommonData->GetLastTurnOwnershipPlayerMap8() );
			//CEOSAIPlayerOwnershipMap*  GetCurrentOwnershipPlayerMap8() { return &m_CurrentOwnershipPlayerMap8; }
			//CEOSAIPlayerOwnershipMap*  GetLastTurnOwnershipPlayerMap8() { return &m_LastTurnOwnershipPlayerMap8; }
			g_pEOSAICommonData->m_OwnershipMapLastUpdatedTurn = g_pEOSAIInterface->GetCurrentTurn();
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

void  CEOSAIPlayerManager::ProcessMessage(EOSAI::Message* pMessageToAI)
{
	/*
	if (dynamic_cast<EOSAI::MessageToAI_WarWasDeclared*>(pMessageToAI))
	{
		//EOSAI::MessageToAI_WarWasDeclared* pWarWasDeclared = dynamic_cast<EOSAI::MessageToAI_WarWasDeclared*>(pMessageToAI);
		EOSAI::MessageToAI_WarWasDeclared* pWarDeclaration = dynamic_cast<EOSAI::MessageToAI_WarWasDeclared*>(pMessageToAI);

		CEOSAIPlayerInteraction_DeclaredWar* pPlayerInteraction = new CEOSAIPlayerInteraction_DeclaredWar();
		pPlayerInteraction->m_iActor = pWarDeclaration->m_iActor;
		pPlayerInteraction->m_iTarget = pWarDeclaration->m_iTarget;
		pPlayerInteraction->m_iEventTurn = g_pEOSAIInterface->GetCurrentTurn();
		g_pEOSAICommonData->AddNewPlayerInteractionAndSendFeelingsUpdate(pPlayerInteraction);
	}
	*/
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

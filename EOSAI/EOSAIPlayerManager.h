
#pragma once

class CAICommonData;
class CEOSAICity;
class CEOSAIAirfield;
class CAIResource;
class CEOSAIUnit2;
#include "EOSAIGlobalForeignRelations.h"
//#include "Unitset.h"
#include "EOSAIBCDumbArray1D.h"
//#include "AIPlayer.h"
namespace EOSAI
{
	class CInterface;
	class AIPlayer;
};

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIPlayerManager
{
	public:
		CEOSAIPlayerManager();
		~CEOSAIPlayerManager();

		void SetEOSAIInterface( EOSAI::CInterface* pInterface );

		// Serialize/Deserialize
		//
			void  Serialize( CEOSAISerial* pSerial );
			void  Deserialize( CEOSAISerial* pSerial );

		// Setup the AIPlayerManager
		//
			// This is all the players (human players, AI players, dead players).  
			// If a game has one human player and one AI player, this iPlayers = 2
			// This gets set once during the game
			void  SetNumberOfPlayers( long iPlayers );
			//void  SetUnitset( CUnitset* pUnitset ){}

			// This gets setup each turn
			//void  Add( CEOSAICity* pAICity ){}
			//void  Add( CEOSAIAirfield* pAIAirfield ){}
			//void  Add( CEOSAIResource* pAIResource ){}
			//void  Add( CEOSAIUnit2* pAIUnit ){}
			//void  SetMapSize( long iHeight, long iWidth, bool bWrap

			void  AddAIPlayer( EOSAI::AIPlayer* pAIPlayer );
			EOSAI::AIPlayer* GetAIPlayer( long iAIPlayer );

			void  DeleteAIPlayer( long iPlayer );
			void  DeleteAIPlayers();

			void  WaitUntilAIPlayerIsRemoved( long iPlayer );

			//long  GetHandlingAIPlayer(){ return m_iHandlingAIPlayer; }
			long  GetProcessingAIPlayer(){ return m_iProcessingAIPlayer; }

		// Process
		//
			// After setting everything up, call "Process".  If anything changes in the game state after calling process, call "AddEvent".
			// The "Process" call will start a new thread that will process the AI.
			void  Process();

			// Current Processing State
			long  GetNumberOfAIPlayersWhoAreReadyToSendTurn();
			bool  AllAIPlayersAreReadyToSendTurn();
			bool  AllAIPlayersAreReadyToSendTurnOrHaveSentTurn();
			void  CurrentlyProcessingAIPlayer( long i ){ m_iCurrentlyProcessingAIPlayer = i; }
			long  CurrentlyProcessingAIPlayer(){ return m_iCurrentlyProcessingAIPlayer; }

			// If the game is being saved, the AI needs to go into a idle state.  Call "WaitForAutosave" to put the AI into idle.
			void  WaitForAutosave( bool bWait ){ m_bWaitForAutosave = bWait; }
			bool  WaitForAutosave(){ return m_bWaitForAutosave; }

			void  SetDebugPause( bool b ){ m_bDebugPause = b; }
			bool  GetDebugPause();

		// Events
		//
			// Events are changes in the game state.  For example, a declaration of war against an AI might change what they do.
			//void  AddEvent();

		// Information
		//
			long  GetNumberOfActiveAIPlayers(); // the number of AI Players who are still in the game
			//CEOSAIGlobalForeignRelationsState*  GetGlobalForeignRelations(){ return &m_AIGlobalForeignRelationsState; }

		// Thread
		//
			bool  ThreadIsRunning(){ return m_bThreadIsRunning; }
			bool  ThreadShouldBePaused(){ return m_bThreadShouldBePaused || m_bThreadShouldBePaused_ForSaveGame; }
			void  ThreadShouldBePaused( bool b ){ m_bThreadShouldBePaused = b; }
			void  ThreadShouldBePaused_ForSaveGame( bool b ){ m_bThreadShouldBePaused_ForSaveGame = b; }
			bool  ThreadIsPaused(){ return m_bThreadIsPaused; }
			//bool  ThreadShouldBePaused(){ return m_bThreadShouldBePaused; }
			void  InstanciateThreadAndPauseIt();
			//void  StartThread();
			void  Loop();
			void  ShutdownThread();

		// Data common to all AI players
		//
			// AICommonData is some information used by all the AIs (stuff like pathway maps)
			// It gets calculated before any of the AIs handle their turn
			//CAICommonData*  m_pAICommonData;  

		//
			EOSAI::CInterface* m_pEOSAIInterface;

		// Process
		//
			bool  m_bWaitForAutosave;
			bool  m_bDebugPause; // Used to put the AI into an idle state for debugging purposes

		// AIPlayers
		//
			CEOSAIBCDumbArray1D< EOSAI::AIPlayer* >  m_AIPlayersArray;  // easier to add/remove players on-the-fly with an array than a list
			long  m_iCurrentlyProcessingAIPlayer;
			long  m_iProcessingAIPlayer;

		// Thread
		//
			bool   m_bThreadIsRunning;
			bool   m_bThreadShouldBeKilled;
			bool   m_bThreadShouldBePaused;
			bool   m_bThreadShouldBePaused_ForSaveGame;
			bool   m_bThreadIsPaused;
};

extern CEOSAIPlayerManager* g_pAIPlayerManager;


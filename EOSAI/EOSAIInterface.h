
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

//
// The AIInterface class is a base class.  The class "EOSAIInterface" is used by the EOS application, and can be used
//   as an example of how to write a derived class for other applications.
// The AIInterface code (*.h and *.cpp) should be copied into your own application, customized, and compiled by your application. 
//   This class is *not* compiled into the AI dll.
//   It creates the objects used by the AI (you have to customize it for your own application).
//     The CreateAIObjects() method needs to:
//       - Create a list of players in the game
//       - Create a list of technologies the AI can research
//       - Construct a list of buildable objects (e.g. what can the AI cities build?)
//       - Construct the AIUnitTemplates which describe the abilities of units (e.g. the "Tank" has these movement and combat capabilities...)
//       - Add all the game objects to the AI data (this means cities, resource, airfields, units, etc)
//       - Construct a map so that the AI can do pathfinding.  (This includes setting up AIRegions and Geos.)
//       - Pass the AI a distance tool so that the AI can figure out distances
//   Once the AI is done processing, the data will exist in this object for export.
//
// An example of using the AIInterface
//
//   CInterface* g_pEOSAI = NULL; // CEOSAI is derived from CAIInterface
//   YourCode()
//   {
//     g_pEOSAI = new CInterface();
//     pEOSAI->CreateAIObjects();      // This is your customized function
//     pEOSAI->TriggerAIProcessing();  // Start the AI running (in its own thread)
//   }
//
//   OccasionallyCheckIfTheAIIsDoneProcessing()
//   {
//     if( g_pEOSAI->DataHasBeenCopiedToGameObjects() == false && g_pEOSAI->ProcessingIsDone() )
//     {
//       g_pEOSAI->UpdateGameObjectsUsingAIObjects(); // Turn the AI's orders into results in the game-world
//     }
//   }
//

//#include "..\External\BCDumbArray1D.h"
#include "EOSAIGamePlayer.h"
#include "AIPlayerDesc.h"
#include "EOSAIGameRules.h"
#include "EOSAICommonData.h"
#include "EOSAIPlayerManager.h"
#include "MessageFromAI.h"
#include "MessageToAI.h"
//#include "EOSAIRegionManager2.h"
//#include "EOSAIPlayerManager.h"
class CEOSAIBuildingValue;
class CEOSAISerial;

namespace EOSAI
{
	class UnitPathwayResult;
	class CWorldDistanceTool;
};

#define EOSAI_MAX_NUMBER_OF_PLAYERS 10 // 10 players (note: our index starts at 0)

namespace EOSAI
{
class DLLIMPEXP CInterface
{
	public:
		CInterface();

		void CreateEOSAI();

		//
		char* GetEOSAIVersion() { return "0.1"; }
		char* GetEOSAIBuildDate() { return __DATE__; }// "Septembruary 45, 3024"; }

		// Set my WorldDistanceTool
		//  It will contain a variety of methods that can measure distances between locations and other functions.
		void SetAIWorldDistanceTool(EOSAI::CWorldDistanceTool* p);

		// Start/Stop/Pause the AI. The AI runs in it's own thread.
		//
			void SetThreadShouldBePaused(bool bPause);
			void SetThreadShouldBePaused_ForSaveGame(bool b);
			bool GetThreadShouldBePaused();
			void SetDebugPause(bool bPause); // This is called by EOSAIDebugger. The game is also allowed to call this.
			bool GetDebugPause();
			//
			bool ThreadIsRunning();
			bool ThreadIsPaused();
			//
			void ShutdownEOSAI() { m_bShutdownEOSAI = true; }  // The AI must be stopped before shutting down the game (especially if it's still processing)
			void   ShutdownThread();
			void   DeleteAIPlayers();
			//
			bool GetGameHasEnded() { return m_bGameHasEnded; }
		
		// Start/Stop/Pause the Game (this is used if the game can auto-advance to the next turn, when testing the AI)
		//
			long GetGameShouldPauseAtTheEndOfTurn(); // Set by the EOSAIDebugger, used by the game

		// Load/Save
		//
			// The AI needs to remember data (like what happened historically in the game), so data needs to be saved and loaded.
			//   The AI must be paused when saving the data
			void  SerializeData(CEOSAISerial* pSerial) { ASSERT(false); }
			void  DeserializeData(CEOSAISerial* pSerial) { ASSERT(false); }

		// Player information
		//
			void SetNumberOfPlayers(long iNumberOfPlayers);
			void AddGamePlayer(EOSAI::CGamePlayer* pGamePlayer);
			void AddAIPlayerDesc(EOSAI::AIPlayerDesc* pAIPlayerDesc);
			//
			void DeleteAIPlayer(int iCurrentPlayer);
			void WaitUntilAIPlayerIsRemoved(int iCurrentPlayer);
			//
			EOSAI::CGamePlayer*   GetGamePlayer(long iPlayer);
			EOSAI::AIPlayer*      GetAIPlayer(long iPlayer);
			EOSAI::AIPlayerDesc*  GetAIPlayerDesc(long iPlayer);
			//
			long GetNumberOfGamePlayers(); // Dead or Alive
			long GetNumberOfActivePlayers(); // Alive only
			long GetNumberOfHumanPlayers();
			long GetNumberOfActiveAIPlayers();
			//
			void PauseTheAI_GameDataIsBeingUpdated();
			void UnPauseTheAI_GameDataWasUpdated();

		// Data Structures
		//
			/*
			EOSAI::CCommonData*   GetAICommonData() { return &m_AICommonData; }
			EOSAI::CGameRules*    GetAIGameRules() { return &m_AIGameRules; }
			CEOSAIRegionManager2* GetAIRegionManager();
			CEOSAIPlayerManager*  GetAIPlayerManager() { return &m_AIPlayerManager; }
			//
			CEOSAIPoiObject*      GetAIPoiObject(long iAIPoiObject);
			CList< CEOSAIPoiObject* >*  GetAIPoiObjectList() { return m_AICommonData.GetAIPoiObjects(); }
			*/

		// Game->AI state
		//
			bool GetAllActiveHumanPlayersHaveSubmittedTurn(long iCurrentTurn) { return m_bAllActiveHumanPlayersHaveSubmittedTurn; }
			void SetAllActiveHumanPlayersHaveSubmittedTurn(long iCurrentTurn, bool b) { m_bAllActiveHumanPlayersHaveSubmittedTurn = b; }

		// Overrides (must be overridden by the game)
		//
			virtual void CreateGamePlayers(){ ASSERT(false); }
			virtual void CreateGameRules() { ASSERT(false); }
			virtual void CreateTechTree() { ASSERT(false); }
			int             GetNumberOfTechnologyDescs();
			void            AddTechnologyDesc(CEOSAITechnologyDesc* pAITechnologyDesc);
			virtual void CreateAIRegionsAndMultiRegions() { ASSERT(false); }
			virtual void CreateAIGeoMap() { ASSERT(false); }
			virtual void CreateAIObjects() { ASSERT(false); }
			virtual void UpdateCurrentTurn() { ASSERT(false); }
			void            SetCurrentTurn(long iTurn);
			long            GetCurrentTurn() { return m_iCurrentTurn; }

			// TEMPORARY - I'd like to get rid of this and replace it with something else
			virtual void CopyOwnershipMapsToEOSAI(CEOSAIPlayerOwnershipMap* pCurrentMap, CEOSAIPlayerOwnershipMap* pLastTurnMap) { ASSERT(false); }
			// Called by the AI
			virtual long GetGeoId(CEOSAILocation Location) { ASSERT(false); return 0; }
			virtual void GetBuildingValue(CEOSAICity* pInCity, CEOSAIBuildingDescription* pInBuildingDesc, CEOSAIBuildingValue* pOutputBuildingValue){ ASSERT(false); }
			virtual int  GetTerrainColorByPixelLocation(float fPosX, float fPosY){ ASSERT(false); return 0; } // Used for the EOSAIUserInterface

	#ifdef THINGS_TO_COMPILE_EVENTUALLY
			I should move "CalculateNationwidePathways", "CalculateAIUnitCombatCapabilities" and "AddObjectIdsToAIRegionsAndMultiRegions" out of the interface.
				It really doesn't need to be visible to the end-user.
	#endif THINGS_TO_COMPILE_EVENTUALLY
			void CalculateAIUnitCombatCapabilities();
			void CalculateNationwidePathways();
			void AddObjectIdsToAIRegionsAndMultiRegions();

		//
			CEOSAIPoiObject*    GetAIPoiObject(long iAIPoiObject);
			//
			void                           AddAIUnitTemplate(CEOSAIUnitTemplate* pAIUnitTemplate);
			CEOSAIUnitTemplate*            GetAIUnitTemplate(CString strUnitTemplate);
			CList< CEOSAIUnitTemplate* >*  GetAIUnitTemplates();
			//
			//this->GetAICommonData()->GetAIBuildOptionList()->AddTail(pEOSAIBuildOption);
			void AddAIBuildOption(CEOSAIBuildOption* pEOSAIBuildOption);
			void DeleteAllBuildOptions();
			//
			void                       AddBuildingDescription(CEOSAIBuildingDescription* pAIBuildingDescription);
			CEOSAIBuildingDescription* GetAIBuildingDescription(CString strBuildingDesc);
			CList< CEOSAIBuildingDescription* >*  GetBuildingDescriptionList();
			//
			void                     AddCombatUnitType(CEOSAICombatUnitType* p);
			CEOSAICombatUnitType*    GetCombatUnitType(long iCombatUnitType);

			void                     AddMovementUnitType(CEOSAIMovementUnitType* pAIMovementUnitType);
			CEOSAIMovementUnitType*  GetMovementUnitType(long iMovementUnitType);

			void  SetInitialCanBuildUnit(CString strUnitsubset, CString strInternalUnitName);
			void  SetInitialCanBuildBuilding(CString strUnitsubset, CString strInternalBuildingName);


			CEOSAIRegionManager2* GetAIRegionManager(); // TODO: This is temporary, because we highlight pathways
			EOSAI::CCommonData*   GetAICommonData(); // TODO: This is temporary, because we highlight pathways
			EOSAI::CGameRules*    GetAIGameRules(); // TODO: This is temporary, because we highlight pathways

		// Settings
		//
			void  SetAllPlayersPermanentlyAtWar(bool b);

		// AI Processing State
		//
			long GetProcessingAIPlayer(); // The player that's being processed
			bool GetProcessingIsDone() { return m_bProcessingIsDone; }
			long GetNumberOfAIPlayersWhoHaveFinishedProcessing();

		//
			void SetNeedToRebuildData(bool b);

		// Communication to/from the AI - these methods should be overridden
		//
			// Notifications from the AI to the game
			//
				// Messages from AI to the game
				virtual void Notification_NewMessageFromAI() {}
				CList< EOSAI::MessageFromAI* >* GetMessagesFromAI() { return &m_MessagesFromAI; }
				//
				virtual void Notification_AIPlayerHasProcessedTurn(long iAIPlayer, long iCurrentTurn) { ASSERT(false); }; // Called by the AI

				bool GetAllAIPlayersAreReadyToSendTurn();

			// Messages from game to AI
			//
				void SendTradeAgreementResponseToAI(long iToAIPlayer, CString strTradeAgreementId, long iPlayerWhoInitiatedChange, EOSAIEnumTradeAgreementResponse eResponse, EOSAIEnumTradeAgreementState eNewState);
				void SendMessageResponseToAI(long iToAIPlayer, long iFromPlayer, long iAIMessageUID, EOSAI::EnumAIMailResponse eResponse);
				void SendMessageToAI(EOSAI::MessageToAI* pMessageToAI);
				// NOTE: This call happens within the Game's thread. It's the only function where that happens.
				//       I'm not sure if I like that. Maybe I could send a message and have another message respond, although I want it to happen quickly.
				void GetAIPlayersOpinionOnTradeAgreement(int iHumanPlayerNumber, int iAIPlayer, CEOSAITradeAgreement* pTradeAgreement, CString* pstrOpinionText, long* piOpinionSum);

				// PlayerInteraction Events (declarations of war, etc)
				void AddPlayerInteractionEvent( CEOSAIPlayerInteraction* pPlayerInteraction );

				// Foreign Relations
				//   Have the AI recalculate the foreign relations state (should get called every turn)
				//   The reason this gets called every turn is because the feelings of past-events fades over time.
				// TODO: This should not be in the EOSAIInterface.
				void UpdateForeignRelationsState2(int iCurrentTurn);

		// Post-AI information (tells the game what the AI wants to do)
		//
			// Overridden by the game so that the AI can set the city-build and unit-movement orders
			// Turn the AIPoiObject orders into game-world orders (delete the old ones first)
			//virtual void SetBuildOrder1( int iAIPlayer, CEOSAICity* pEOSAICity, CEOSAIBuildOption* pEOSAIBuildOption ){ ASSERT( false ); }
			virtual void SetBuildOrder( int iAIPlayer, CEOSAICity* pEOSAICity, CString strBuildOptionInternalName ){ ASSERT( false ); }
			virtual bool CreateAIUnitOrders( EOSAI::UnitPathwayResult* pAIUnitPathwayResult ){ ASSERT( false ); return false; }

#ifdef CREATE_EOSAI_DLL
	public:
#else
	private:
#endif USE_EOSAI_DLL
		void SendMessageFromAI(EOSAI::MessageFromAI* pAIMessage); // { m_MessagesFromAI.AddTail(pAIMessage); Notification_NewMessageFromAI(); }
		CList< EOSAI::MessageToAI* >* GetMessagesToAI();// { return &m_MessagesToAI; }

	protected:
		EOSAI::CWorldDistanceTool* m_pAIWorldDistanceTool;

		bool  m_bAllActiveHumanPlayersHaveSubmittedTurn;

		CList< EOSAI::MessageFromAI* >  m_MessagesFromAI;
		CList< EOSAI::MessageToAI* >    m_MessagesToAI; // used for global message (like declare war), not messages to individual AIs

		//
		int m_iNumberOfGamePlayers;
		EOSAI::CGamePlayer*    m_GamePlayers[EOSAI_MAX_NUMBER_OF_PLAYERS];   // An array of pointers
		EOSAI::AIPlayerDesc*   m_AIPlayerDesc[EOSAI_MAX_NUMBER_OF_PLAYERS]; // An array of pointers

		//CEOSAIPlayerManager m_AIPlayerManager;
		//EOSAI::CGameRules   m_AIGameRules;
		//EOSAI::CCommonData  m_AICommonData;

		long m_iCurrentTurn;
		bool m_bGameHasEnded;

		bool m_bGameRulesHaveBeenCreated;
		bool m_bTechTreeHasBeenCreated;
		bool m_bAIRegionsAndMultiRegionsHaveBeenCreated;
		bool m_bAIObjectsHaveBeenCreated;

		bool m_bProcessingIsDone;
		//bool m_bPauseEOSAI;
		bool m_bShutdownEOSAI;
};
};
extern EOSAI::CInterface* g_pEOSAIInterface;




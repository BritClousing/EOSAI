
#pragma once

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
//extern CInterface* g_pEOSAIInterface;

namespace EOSAI
{
	class CInterface
	{
	public:
		CInterface();

		//
		char* GetEOSAIVersion() { return "0.1"; }
		char* GetEOSAIBuildDate() { return __DATE__; }// "Septembruary 45, 3024"; }

	// Set my WorldDistanceTool
	//  It will contain a variety of methods that can measure distances between locations and other functions.
		void SetAIWorldDistanceTool(EOSAI::CWorldDistanceTool* p);

		//
		void SetNumberOfPlayers(long iNumberOfPlayers);
		void AddGamePlayer(EOSAI::CGamePlayer* pGamePlayer);
		void AddAIPlayerDesc(EOSAI::AIPlayerDesc* pAIPlayerDesc);

		//void CreateAICommonData();
		EOSAI::CCommonData* GetAICommonData() { return &m_AICommonData; }

		// Players
		//
		virtual void InstanciateAIPlayers();

		CEOSAIPlayerManager*  GetAIPlayerManager() { return &m_AIPlayerManager; }
		EOSAI::CGamePlayer*   GetGamePlayer(long iPlayer);
		EOSAI::AIPlayer*      GetAIPlayer(long iPlayer);
		EOSAI::AIPlayerDesc*  GetAIPlayerDesc(long iPlayer);

		void ActivateAIPlayers();

		long GetNumberOfPlayers(); // Dead or Alive
		long GetNumberOfActivePlayers();
		long GetNumberOfHumanPlayers();
		long GetNumberOfActiveAIPlayers();

		// Overrides
		//
		virtual long GetGeoId(CEOSAILocation Location) { ASSERT(false); return 0; }

		virtual void CreateGameRules() { ASSERT(false); }
		virtual void CreateTechTree() { ASSERT(false); }
		virtual void CreateAIRegionsAndMultiRegions() { ASSERT(false); }
		virtual void CreateAIGeoMap() { ASSERT(false); }
		virtual void CreateAIObjects() { ASSERT(false); }

#ifdef THINGS_TO_COMPILE_EVENTUALLY
		I should move "CalculateNationwidePathways" and "AddObjectIdsToAIRegionsAndMultiRegions" out of the interface.
			It really doesn't need to be visible to the end-user.
#endif THINGS_TO_COMPILE_EVENTUALLY
			void CalculateNationwidePathways();
		void AddObjectIdsToAIRegionsAndMultiRegions();

		// Overrides
		//
		virtual void GetBuildingValue(CEOSAIBuildingValue* BuildingValue) {}

		//
			//bool m_bCommonAIObjectsHaveBeenCreated; // CommonAIObjects = Rules, PoiObjects, Regions, etc

		// Rules
		//
		EOSAI::CGameRules*   GetAIGameRules() { return &m_AIGameRules; }

		// AIPoiObjects
		//
		CEOSAIPoiObject*    GetAIPoiObject(long iAIPoiObject);

		// Regions
		//
		CEOSAIRegionManager2* GetAIRegionManager();

		// Game State
		//
		virtual void UpdateCurrentTurn() { ASSERT(false); }
		void  SetCurrentTurn(long iTurn) { ASSERT(iTurn > 0); m_iCurrentTurn = iTurn; }
		long  GetCurrentTurn() { return m_iCurrentTurn; }
		bool  GetGameHasEnded() { return m_bGameHasEnded; }

	//
		long GetProcessingAIPlayer(); // The player that's being processed

	//
		void SetNeedToRebuildData(bool b);

		// Shutdown
		//
		void KillTheAIPlayerThreads();
		void DeleteAIPlayers();

		// Load/Save
		//
			// The AI needs to remember data (like what happened historically in the game), so data needs to be saved and loaded.
			//   The AI must be paused when saving the data
		void  SerializeData(CEOSAISerial* pSerial) { ASSERT(false); }
		void  DeserializeData(CEOSAISerial* pSerial) { ASSERT(false); }

		// Pre-AI Processing
		//
			// Turn the game objects into AIObjects that the AI can understand
			//   This means creating CAIPoiObjects, AIRegions, AIGameRules, etc.
		CList< CEOSAIPoiObject* >*  GetAIPoiObjectList() { return m_AICommonData.GetAIPoiObjects(); }

		// Triggers
		//
		bool AllActiveHumanPlayersHaveSubmittedTurn(long iCurrentTurn) { return m_bAllActiveHumanPlayersHaveSubmittedTurn; }
		void AllActiveHumanPlayersHaveSubmittedTurn(long iCurrentTurn, bool b) { m_bAllActiveHumanPlayersHaveSubmittedTurn = b; }

		// Start the AI - it runs in its own thread
		void StartProcessing() {}// m_AIPlayerManager.Process(); }
		void PauseTheAI(bool b) { m_bPauseTheAI = b; } // The AI must be paused before saving the game (especially if it's still processing)
		void ShutdownTheAI(bool b) { m_bShutdownTheAI = b; }  // The AI must be stopped before shutting down the game (especially if it's still processing)

		// TEMPORARY - I'd like to get rid of this and replace it with something else
		virtual void CopyOwnershipMapsToEOSAI(
			CEOSAIPlayerOwnershipMap* pCurrentMap,
			CEOSAIPlayerOwnershipMap* pLastTurnMap) { ASSERT(false); }

	// Status of AI Processing
	//
		bool ProcessingIsDone() { return m_bProcessingIsDone; }
		long GetNumberOfAIPlayersWhoHaveFinishedProcessing();

		// Post-AI Processing - these methods should be overridden
		//
			// Messages from AI to a player
			//
			virtual void NewMessageFromAINotification() {}
			CList< EOSAI::MessageFromAI* >* GetMessagesFromAI() { return &m_MessagesFromAI; }

			// Messages from player to AI
			//
			void SendTradeAgreementResponseToAI(long iToAIPlayer, CString strTradeAgreementId, long iPlayerWhoInitiatedChange, EOSAIEnumTradeAgreementResponse eResponse, EOSAIEnumTradeAgreementState eNewState);
			void SendMessageResponseToAI(long iToAIPlayer, long iFromPlayer, long iAIMessageUID, EOSAI::EnumAIMailResponse eResponse);
			void SendMessageToAI(EOSAI::MessageToAI* pMessageToAI);
			// NOTE: This call happens within the Game's thread. It's the only function where that happens.
			//       I'm not sure if I like that. Maybe I could send a message and have another message respond, although I want it to happen quickly.
			void GetAIPlayersOpinionOnTradeAgreement(int iHumanPlayerNumber, int iAIPlayer, CEOSAITradeAgreement* pTradeAgreement, CString* pstrOpinionText, long* piOpinionSum);

			// PlayerInteraction Events
			//
			void AddPlayerInteractionEvent( CEOSAIPlayerInteraction* pPlayerInteraction );

			// Foreign Relations
			//   Have the AI recalculate the foreign relations state (should get called every turn)
			//   The reason this gets called every turn is because the feelings of past-events fades over time.
			void UpdateForeignRelationsState(int iCurrentTurn);

			// Overridden by the game so that the AI can set the city-build and unit-movement orders
			// Turn the AIPoiObject orders into game-world orders (delete the old ones first)
			//virtual void SetBuildOrder1( int iAIPlayer, CEOSAICity* pEOSAICity, CEOSAIBuildOption* pEOSAIBuildOption ){ ASSERT( false ); }
			virtual void SetBuildOrder( int iAIPlayer, CEOSAICity* pEOSAICity, CString strBuildOptionInternalName ){ ASSERT( false ); }
			virtual bool CreateAIUnitOrders( EOSAI::UnitPathwayResult* pAIUnitPathwayResult ){ ASSERT( false ); return false; }
			virtual void SetAIPlayerHasProcessedTurn(long iAIPlayer, long iCurrentTurn) { ASSERT(false); }; // Called by the AI

#ifdef CREATE_EOSAI_DLL
	public:
		virtual void SendMessageFromAI(EOSAI::MessageFromAI* pAIMessage) { m_MessagesFromAI.AddTail(pAIMessage); NewMessageFromAINotification(); }
		virtual CList< EOSAI::MessageToAI* >* GetMessagesToAI() { return &m_MessagesToAI; }
#else
	private:
		virtual void SendMessageFromAI(EOSAI::MessageFromAI* pAIMessage) { m_MessagesFromAI.AddTail(pAIMessage); NewMessageFromAINotification(); }
		virtual CList< EOSAI::MessageToAI* >* GetMessagesToAI() { return &m_MessagesToAI; }
#endif USE_EOSAI_DLL

	protected:
		//bool m_bNeedToRebuildData;
		//bool m_bAIObjectsHaveBeenCreated;
		EOSAI::CWorldDistanceTool* m_pAIWorldDistanceTool;

		bool  m_bAllActiveHumanPlayersHaveSubmittedTurn;

		CList< EOSAI::MessageFromAI* >  m_MessagesFromAI;
		CList< EOSAI::MessageToAI* >    m_MessagesToAI; // used for global message (like declare war), not messages to individual AIs

		//
		int m_iNumberOfGamePlayers;
		EOSAI::CGamePlayer*    m_GamePlayers[EOSAI_MAX_NUMBER_OF_PLAYERS];   // An array of pointers
		EOSAI::AIPlayerDesc*   m_AIPlayerDesc[EOSAI_MAX_NUMBER_OF_PLAYERS]; // An array of pointers

		CEOSAIPlayerManager m_AIPlayerManager;
		EOSAI::CGameRules   m_AIGameRules;
		EOSAI::CCommonData  m_AICommonData;
		//CEOSAIRegionManager2  m_AIRegionManager;

		long m_iCurrentTurn;
		bool m_bGameHasEnded;

		bool m_bGameRulesHaveBeenCreated;
		bool m_bTechTreeHasBeenCreated;
		bool m_bAIRegionsAndMultiRegionsHaveBeenCreated;
		bool m_bAIObjectsHaveBeenCreated;

		bool m_bProcessingIsDone;
		bool m_bPauseTheAI;
		bool m_bShutdownTheAI;
};
};
extern EOSAI::CInterface* g_pEOSAIInterface;




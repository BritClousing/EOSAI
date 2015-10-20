#pragma once

//using namespace EOSAI;

//#include "AIPlayer2Base.h"
#include "AIPlayerDesc.h"
#include "EOSAIBrain.h"
//#include "EOSAIHistoricalSightings.h"
#include "EOSAIStrategicAIOrder.h"
//#include "AIGrandPlan.h"
//#include "TradeAgreementMultiplier.h"
#include "EOSAIStrategicAI.h"
//#include "EOSAIStopwatch2.h"
#include "EOSAIEnumTradeAgreementResponse.h"
#include "EOSAIEnumTradeAgreementState.h"
#include "MessageToAI.h"
//#include "Player.h"
//class CPlayerData;
//class CEOSAIPlayerInteraction;
//class CWorldDescPlayer;
//class CWorldDescPlayerProxy;
//class CWorldDescServer;
class CEOSAITradeAgreement;
//class EOSAI::EOSAI::CGamePlayer;
//namespace EOSAI
//{
namespace EOSAI
{
	class CGamePlayer;
};
//};
class CAIDesireToControlGeo;
class CAIForeignPlan;
//class CSemiGeoManager;
//class CGameEvent_IMail;
//class CGameEvent_IMailResponse;
class CEOSAIMail;
class CEOSAIMailResponse;
class CEOSAITradeAgreementResponse;
class CEOSAIMultiRegionNationwidePathways;
//class CGeneralSubWindow;
class CAICircularAnnotation;
#include <afxmt.h>

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

// The AIPlayer class is where programmers can put additional AIPlayer information that's not included in AI::AIPlayerInterface
//

namespace EOSAI
{
class DLLIMPEXP AIPlayer // : public EOSAI::AIPlayerBase
{
	public:
		enum EnumState
		{
			enumWaitingForServerToPlayerUpdate = 1,
			//enumWaitingForTurnAndRulesUpdate = 1,
			//enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd = 2,
			enumBeginProcessingTurn = 3,
			//enumProcessingTurn = 4,
			enumReadyToSendOrdersToServer = 5, // Waiting For Local Human Player To Submit Turn
		};

	public:
		//CAIPlayer( CWorldDescServer* pWorldDescServer, CPlayer* pPlayer, long iAIBrainIntelligence );
		AIPlayer(); // Need to call SetPlayerData afterward
		AIPlayer( long iNumberOfPlayers, EOSAI::CGamePlayer* pGamePlayer, EOSAI::AIPlayerDesc* pAIPlayerDesc );
		void     SetPlayerData( EOSAI::CGamePlayer* pGamePlayer, EOSAI::AIPlayerDesc* pAIPlayerDesc );
		virtual ~AIPlayer();

		void SetPlayerNumber( long i ){ m_iPlayerNumber = i; }
		long GetPlayerNumber(){ return m_iPlayerNumber; }

		// Functions inherited from CAIPlayer2Interface:
		//   long GetPlayerNumber();

		//virtual CString GetString();

		// Serialize/Deserialize
		//virtual void Serialize( CEOSAISerial* pSerial ){}
		//virtual void Deserialize( CEOSAISerial* pSerial ){}

		bool IsReadyToSendTurn(){ return m_eProcessingState == enumReadyToSendOrdersToServer; }

		//void ProcessTurn();

		//
		//void              SetWorldDescPlayer( CWorldDescPlayer* p );
		//CWorldDescPlayer* GetWorldDescPlayer(){ return m_pWorldDescPlayer; }
		//CWorldDescPlayerProxy*  GetWorldDescPlayerProxy(){ return m_pWorldDescPlayerProxy; }
		//CWorldDescServer* GetWorldDescServer(){ return m_pWorldDescServer; }
		EOSAI::CGamePlayer* GetGamePlayer(){ return m_pGamePlayer; }
		EOSAI::AIPlayerDesc*   GetAIPlayerDesc(){ return m_pAIPlayerDesc; }
		CEOSAIBrain*        GetAIBrain(){ return m_pAIBrain; }
		CEOSAIStrategicAI*  GetStrategicAI(){ return &m_StrategicAI; }

		//bool  IsReadyToSendTurn(){ return m_eProcessingState == enumReadyToSendOrdersToServer; }// && ThereAreUnprocessedEvents() == false; }

		// Serialize/Deserialize
		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );

		// Intelligence/Personality
		long  GetAIBrainIntelligence(){ return m_pAIPlayerDesc->GetAIBrainIntelligence(); } // 1 = WeakAI, 2 = ModerateAI, 3 = StrongAI
		float GetPersonality_HoldsGrudges01(){ return m_pAIPlayerDesc->GetPersonality_HoldsGrudges01(); }
		float GetPersonality_Aggressive01(){ return m_pAIPlayerDesc->GetPersonality_Aggressive01(); }
		float GetPersonality_Fortifier01(){ return m_pAIPlayerDesc->GetPersonality_Fortifier01(); }
		float GetPersonality_Builder01(){ return m_pAIPlayerDesc->GetPersonality_Builder01(); }
		float GetPersonality_ProResearcherPos1_AntiResearcherNeg1(){ return m_pAIPlayerDesc->GetPersonality_ProResearcherPos1_AntiResearcherNeg1(); }
		float GetPersonality_ProSubmarine01(){ return m_pAIPlayerDesc->GetPersonality_ProSubmarine01(); }
		float GetPersonality_ProAirUnit11(){ return m_pAIPlayerDesc->GetPersonality_ProAirUnit11(); }
		float GetPersonality_ProGroundUnit11(){ return m_pAIPlayerDesc->GetPersonality_ProGroundUnit11(); }
		float GetPersonality_ProSeaUnit11(){ return m_pAIPlayerDesc->GetPersonality_ProSeaUnit11(); }
		//float GetPersonality_ProAircraftPos1_ProGroundSeaNeg1(){ return m_fPersonality_ProAircraftPos1_ProGroundSeaNeg1; }
		float GetPersonality_ProStrategicBombing01(){ return m_pAIPlayerDesc->GetPersonality_ProStrategicBombing01(); }
		float GetPersonality_ProMissile01(){ return m_pAIPlayerDesc->GetPersonality_ProMissile01(); }
		float GetPersonality_ProArtillery01(){ return m_pAIPlayerDesc->GetPersonality_ProArtillery01(); }
		float GetPersonality_ProExpensiveShipsPos1_CheapShipsNeg1(){ return m_pAIPlayerDesc->GetPersonality_ProExpensiveShipsPos1_CheapShipsNeg1(); } // 1.0 = Battleships, 0.0 = Destroyers
		float GetPersonality_ProUnitDiversity01(){ return m_pAIPlayerDesc->GetPersonality_ProUnitDiversity01(); } // 1.0 = Battleships, 0.0 = Destroyers

		CEOSAICitResValueMultiplier* GetCitResValueMultiplier(){ return &m_CitResValueMultiplier; }
		float GetCitResValue_FoodMultiplier(){ return m_CitResValueMultiplier.m_fFoodValueMultiplier; }
		float GetCitResValue_IronMultiplier(){ return m_CitResValueMultiplier.m_fIronValueMultiplier; }
		float GetCitResValue_OilMultiplier(){ return m_CitResValueMultiplier.m_fOilValueMultiplier; }
		float GetCitResValue_CityMultiplier(){ return m_CitResValueMultiplier.m_fCityProductionValueMultiplier; }
		float GetCitResValue_ResourceMultiplier( CString strResType ){ return m_CitResValueMultiplier.GetResourceValueMultiplier( strResType ); }

		void  SetRand( long iRand ){ m_iRand = iRand; }
		long  GetRand(){ return m_iRand; }

		// Recalculation flags
		void  NeedToRecalculateTradeValues( bool b ){} // INCOMPLETE: Need to fix this.  It is called after a trade is completed.  Maybe the AI went from war to peace, maybe he sold all his oil, maybe...?

		// Is the AI cheating?
		float AICheat_ExpandTowardsHumanPlayers01(){ return m_pAIPlayerDesc->AICheat_ExpandTowardsHumanPlayers01(); }
		bool  AICheat_BonusMoneyAndResourcesEachTurn(){ return m_pAIPlayerDesc->AICheat_BonusMoneyAndResourcesEachTurn(); }
		bool  AICheat_50PercentFreeResourcesInShortage(){ return m_pAIPlayerDesc->AICheat_50PercentFreeResourcesInShortage(); }
		bool  AICheat_KnowsMapAndPoiInformation(){ return m_pAIPlayerDesc->AICheat_KnowsMapAndPoiInformation(); }
		bool  AICheat_CanInstantUpgrade(){ return m_pAIPlayerDesc->AICheat_CanInstantUpgrade(); }
		float AICheat_GetFreeResearchPoints(){ return m_pAIPlayerDesc->AICheat_GetFreeResearchPoints(); }
		float AICheat_GetCityProductionMultiplier(){ return m_pAIPlayerDesc->AICheat_GetCityProductionMultiplier(); }
		float AICheat_LikesAIPlayers(){ return m_pAIPlayerDesc->AICheat_LikesAIPlayers(); }
		float AICheat_LikesHumanPlayers(){ return m_pAIPlayerDesc->AICheat_LikesHumanPlayers(); }
		float AICheat_AttackMultiplierAgainstMilitia(){ return m_pAIPlayerDesc->AICheat_AttackMultiplierAgainstMilitia(); }

		//
		bool  FieldOrders_AutoAttackMilitia(){ return m_pAIPlayerDesc->FieldOrders_AutoAttackMilitia(); }

		//
		bool  HasSetSneakAttackAgainst( int iTargetPlayer ){ return m_StrategicAI.HasSetSneakAttackAgainst( iTargetPlayer ); }

		//
		void  DeleteAIDebugData();
		void  DeleteAIData();
		void  ClearUnitOrders();
		void  SetMyUnitsFieldOrders();
		void  ClearCityOrders();

		//
		float GetMyLandFoodResources();
		float GetMySeaFoodResources();

		//
		// Multithreading code
		//
			// Flags
			//void  TriggerProcessTurn();
			void  Incoming_ServerToPlayerUpdateWasProcessed();
			//void  Incoming_AutosaveAndLocalPlayerTurnReplayEnded();

			void  SetFlag_ShutdownAIPlayer( bool b ){ m_pAIPlayerDesc->SetFlag_ShutdownAIPlayer( b ); }
			bool  GetFlag_ShutdownAIPlayer(){ return m_pAIPlayerDesc->GetFlag_ShutdownAIPlayer(); }

		// Processing
		//
			void  Process();
			void  SendOrders();

		// Nationwide Pathways - I might want to move this into the WorldDescServer
		//
			//void  CalculateMyNationwidePathways();
			CEOSAIMultiRegionNationwidePathways*  GetMyNationwidePathway();
			CEOSAIMultiRegionNationwidePathways*  GetNationwidePathway( long iPlayer );

		// Resource Tracker
		//
			//CAIResourceTracker*  GetMoneyTracker(){ return &m_MoneyAmount; }
			//CAIResourceTracker*  GetIronTracker(){ return &m_IronAmount; }
			//CAIResourceTracker*  GetOilTracker(){ return &m_OilAmount; }
			//CAIResourceTracker*  GetFoodTracker(){ return &m_FoodAmount; }

		// Foreign Relations
		//
			void  SignedAPeaceAgreement( long iPlayer1, long iPlayer2 );

		//
		// Trade, IMail
		//
			// IMail
			long GetNextAIMessageUID(){ return m_pAIPlayerDesc->GetNextAIMessageUID(); }
		//	long GetNextAIMailId(){ m_iNextAIMailId++; return m_iNextAIMailId; }
			//void Incoming_DeclarationOfWar( long iActor, long iTarget, long iTurn );
		//	void Incoming_IMail( CEOSAIMail* pIMail );
			void Incoming_IMailResponse( long iFromPlayer, long iAIMailId, EOSAI::EnumAIMailResponse eResponse );
			//void IncomingMessageResponse(CEOSAIMailResponse* pResponse) { ASSERT(false); }
			// Trade Agreements
		//	void Incoming_TradeOffer( CEOSAITradeAgreement* pTradeAgreement );
			void Incoming_TradeAgreementResponse( CString strTradeAgreementId, long iPlayerWhoInitiatedChange, EOSAIEnumTradeAgreementResponse eResponse, EOSAIEnumTradeAgreementState eNewState );
			// Events
			void IncomingEvent_PlayerResigned( long iPlayerNumberWhoResigned );
			void IncomingEvent_TeamFormed( CString strTeamName );
			//
			void IncomingEvent_RequestAITradeDesires( long iSendToPlayer );
			void EvaluateTradeAgreement(CEOSAITradeAgreement* pTradeAgreement, CString* pstrOpinionText, long* piOpinionSum);
			void Incoming_TradeOffer(CEOSAITradeAgreement* pEOSAITradeAgreement) { m_UnprocessedIncomingTradeOffers.AddTail(pEOSAITradeAgreement); }
			//
			void Incoming_MessageToAI(EOSAI::MessageToAI* pMessage) { m_UnprocessedIncomingMessages.AddTail(pMessage); }

			//
			void FindUnitsWithinMyNationalBoundaries();
			void SendMessagesAndAdjustForeignRelationsBasedOnBorderViolations();
			//
			void  CalculateCitResValues();
			void  CalculateExperiencingResourceShortage01();
			float GetExperiencingResourceShortage01(){ return m_fExperiencingResourceShortage01; }
			//void AddedBuildItemToResourceConsumption( CBuildOption* pBuildOption );
			//
		//	bool ThereAreUnprocessedEvents();
		//	bool ProcessUnprocessedEvents();
			bool ProcessUnprocessedEvents();//{ ASSERT( false ); return true; }
			/*
			bool ProcessEvent( CEOSAIMail* pIMail )
			{
				bool bWasSignificant = false;
				m_StrategicAI.ProcessMail( pIMail, &bWasSignificant );
				return bWasSignificant;
			}
			bool ProcessEvent( CEOSAITradeAgreement* pTrade )
			{
				bool bWasSignificant = false;
				m_StrategicAI.ProcessTradeOffer( pTrade, &bWasSignificant );
				return bWasSignificant;
			}
			bool ProcessEvent( CEOSAITradeAgreementResponse* pTradeAgreementResponse )
			{
				bool bWasSignificant = false;
				m_StrategicAI.ProcessTradeAgreementResponse( pTradeAgreementResponse, &bWasSignificant );
				return bWasSignificant;
			}
			bool ProcessEvent( CEOSAIMailResponse* pIMailResponse )
			{
				bool bWasSignificant = false;
				m_StrategicAI.ProcessMailResponse( pIMailResponse, &bWasSignificant );
				return bWasSignificant;
			}
			*/


		#ifdef _DEBUG
		CString m_strDebug_ForeignRelations;
		CString m_strDebug_BestTacticalProjectStrategies;

		//void WriteDebugDataButtons_DesiresSortedByScore( CGeneralSubWindow* pParentWindow );
		//CList< CEOSAIDebugData* >  m_DesireAnnotations;
		CList< CEOSAIDebugData* >  m_DesiresBySimpleInterest;
		CList< CEOSAIDebugData* >  m_DesiresByScore;
		CList< CEOSAIDebugData* >  m_TacticalProjectsBySimpleInterest;
		CList< CEOSAIDebugData* >  m_TacticalProjectsByInitialEvaluation;

		CList< CEOSAIDebugData* >  m_TacticalProjects2;
		#endif _DEBUG

	public:
		EnumState  m_eProcessingState;
		bool       m_bTrigger_NeedToRecalculateEntireTurn;
		//CCriticalSection   m_CriticalSection;

		#ifdef _DEBUG
		CMemoryState  m_DebugDialogMemoryState;
		#endif _DEBUG

	private:

		long                  m_iPlayerNumber;
		EOSAI::AIPlayerDesc*     m_pAIPlayerDesc;
		EOSAI::CGamePlayer*   m_pGamePlayer;  // Owned by the game
		CEOSAIBrain*          m_pAIBrain; // Owned by AIPlayer
		//CWorldDescPlayerProxy*  m_pWorldDescPlayerProxy;
		//CWorldDescServer*       m_pWorldDescServer;
/*
		bool   m_bFlag_ShutdownAIPlayer;

	// Intelligence / Personality
	//
		long   m_iAIBrainIntelligence; // 1 = WeakAI, 2 = ModerateAI, 3 = StrongAI
		float  m_fPersonality_HoldsGrudges01;
		float  m_fPersonality_Aggressive01;
		float  m_fPersonality_Fortifier01;
		float  m_fPersonality_Builder01;
		float  m_fPersonality_ProResearcherPos1_AntiResearcherNeg1;
		float  m_fPersonality_ProSubmarine01;
		//float  m_fPersonality_ProAircraftPos1_ProGroundSeaNeg1;
		float  m_fPersonality_ProAirUnit11;
		float  m_fPersonality_ProGroundUnit11;
		float  m_fPersonality_ProSeaUnit11;
		//
		float  m_fPersonality_ProStrategicBombing01;
		float  m_fPersonality_ProMissile01;
		float  m_fPersonality_ProArtillery01;
		float  m_fPersonality_ProExpensiveShipsPos1_CheapShipsNeg1; // 1.0 = Battleships, 0.0 = Destroyers
		float  m_fPersonality_ProUnitDiversity01;

		bool   m_bAICheat_KnowsMapAndPoiInformation; // AICheat
		//float  m_fAICheat_LikesAIPlayers;
		//float  m_fAICheat_LikesHumanPlayers;

		// Processing Situation
		//
			// set to true if foreign relations change (e.g. peace agreement, war declaration, ...)
			//bool   m_bUpdateForeignRelationsCalculations;

		//
			long  m_iNextAIMailId;
		*/
			long  m_iRand; // changes each turn
			long  m_iDebugTempCounter;

		public:

		// Remembered from turn to turn, involves things like:
		//    "build-up a military to attack player X",
		//    "I plan to sneak-attack player X, so I'm secretly moving units towards him"
		//
			CEOSAIStrategicAI  m_StrategicAI;

		// Resource Value, Amounts, Delta, Trajectory
		//
			// "Value Of" variables take into account the lack or surplus of resources to
			//    figure out a "value" for them.  (Note: I should probably break this into two
			//    components - the value *to me*, based on how much I have, and the value
			//    to other players, based on the availability on the whole map (for trade).
			//    Perhaps also the value to other players, based on their availability (for trade).
			//  Large values means that the resource is more valuable (try not to spend it, try to capture it)
			//  I might want to move this into the Plan - ideally, the AI should
			//    anticipate the future value of these resources, too.  The future value
			//    depends partly on what the AI is going to do.
			CEOSAICitResValueMultiplier   m_CitResValueMultiplier;
			// The Value is a measure of how concerned the AI should be about shortages
			//   0.0 = no shortage, 0.01-0.99 = upcoming shortage, 1.0 = experiencing shortage
			float  m_fExperiencingResourceShortage01;

			//CTradeAgreementMultiplier  m_TradeAgreementMultiplier;
			CList< CEOSAIMail* >                    m_UnprocessedIncomingMail;
			CList< CEOSAIMailResponse* >            m_UnprocessedIncomingMailResponses;
			CList< CEOSAITradeAgreement* >          m_UnprocessedIncomingTradeOffers;
			CList< CEOSAITradeAgreementResponse* >  m_UnprocessedIncomingTradeAgreementResponses;
			CList< EOSAI::MessageToAI* >            m_UnprocessedIncomingMessages;
			

		// DEBUG
		//
			//CEOSAIStopwatch2  m_Stopwatch_TotalProcessingTime;
			//CEOSAIStopwatch2  m_Stopwatch_PreSection;
			//CEOSAIStopwatch2  m_Stopwatch_AIGeos;
			//CEOSAIStopwatch2  m_Stopwatch_CreateDesiresAndTacticalProjects;
			//CEOSAIStopwatch2  m_Stopwatch_AIBrainProcess;

	private:
		//int number;
};
}


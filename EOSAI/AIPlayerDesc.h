//#pragma once
#ifndef EOSAI_AI_PLAYER_H
#define EOSAI_AI_PLAYER_H

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAILocation.h"
//class CEOSAISerial2;
class CEOSAISerial;

// The AIPlayerData class is shared between the game and the AI.  It describes the AI player's attributes.
//

class CEOSAIAnnotation
{
	public:
		CEOSAIAnnotation( CEOSAILocation Location, float fRadius, float fRed01, float fGreen01, float fBlue01 )
		{
			m_CenterLocation = Location;
			m_fRadius = fRadius;
			m_Color[0] = fRed01;
			m_Color[1] = fGreen01;
			m_Color[2] = fBlue01;
		}

		CEOSAILocation  m_CenterLocation;
		float      m_fRadius;
		float      m_Color[3];
};

class CEOSAIDebugData
{
	public:
		~CEOSAIDebugData()
		{
			while( m_Annotations.IsEmpty() == FALSE ){ delete m_Annotations.RemoveHead(); }
		}
		void  AddToAnnotations( CEOSAILocation Location, float fRadius, float fRed01, float fGreen01, float fBlue01 );

		CString  m_strText;
		CList< CEOSAIAnnotation* >  m_Annotations;
};

namespace EOSAI
{
//class DLLIMPEXP CPlayerData // : public EOSAI::AIPlayerBase
class DLLIMPEXP AIPlayerDesc // : public EOSAI::AIPlayerBase
{
	public:
		enum EnumState
		{
			enumWaitingForServerToPlayerUpdate = 1,
			//enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd = 2,
			enumBeginProcessingTurn = 3,
			//enumProcessingTurn = 4,
			enumReadyToSendOrdersToServer = 5, // Waiting For Local Human Player To Submit Turn
		};

	public:
		AIPlayerDesc( long iPlayer, long iAIBrainIntelligence3 );
		virtual ~AIPlayerDesc(){}

		//void SetPlayerNumber( long i ){ m_iPlayerNumber = i; }
		long GetPlayerNumber(){ return m_iPlayer; }

		// Functions inherited from EOSAI::AIPlayerInterface:
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
		//EOSAI::CGamePlayer*      GetPlayer(){ return m_pPlayer; }
		//long              GetPlayerNumber();
		//CEOSAIBrain*      GetAIBrain(){ return m_pAIBrain; }
		//CEOSAIStrategicAI*  GetStrategicAI(){ return &m_StrategicAI; }

		//bool  IsReadyToSendTurn(){ return m_eProcessingState == enumReadyToSendOrdersToServer; }// && ThereAreUnprocessedEvents() == false; }

		// Serialize/Deserialize
		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );

		// Intelligence/Personality
		long  GetAIBrainIntelligence(){ return m_iAIBrainIntelligence; } // 1 = WeakAI, 2 = ModerateAI, 3 = StrongAI
		float GetPersonality_HoldsGrudges01(){ return m_fPersonality_HoldsGrudges01; }
		float GetPersonality_Aggressive01(){ return m_fPersonality_Aggressive01; }
		float GetPersonality_Fortifier01(){ return m_fPersonality_Fortifier01; }
		float GetPersonality_Builder01(){ return m_fPersonality_Builder01; }
		float GetPersonality_ProResearcherPos1_AntiResearcherNeg1(){ return m_fPersonality_ProResearcherPos1_AntiResearcherNeg1; }
		float GetPersonality_ProSubmarine01(){ return m_fPersonality_ProSubmarine01; }
		float GetPersonality_ProAirUnit11(){ return m_fPersonality_ProAirUnit11; }
		float GetPersonality_ProGroundUnit11(){ return m_fPersonality_ProGroundUnit11; }
		float GetPersonality_ProSeaUnit11(){ return m_fPersonality_ProSeaUnit11; }
		//float GetPersonality_ProAircraftPos1_ProGroundSeaNeg1(){ return m_fPersonality_ProAircraftPos1_ProGroundSeaNeg1; }
		float GetPersonality_ProStrategicBombing01(){ return m_fPersonality_ProStrategicBombing01; }
		float GetPersonality_ProMissile01(){ return m_fPersonality_ProMissile01; }
		float GetPersonality_ProArtillery01(){ return m_fPersonality_ProArtillery01; }
		float GetPersonality_ProExpensiveShipsPos1_CheapShipsNeg1(){ return m_fPersonality_ProExpensiveShipsPos1_CheapShipsNeg1; } // 1.0 = Battleships, 0.0 = Destroyers
		float GetPersonality_ProUnitDiversity01(){ return m_fPersonality_ProUnitDiversity01; } // 1.0 = Battleships, 0.0 = Destroyers
/*
		CEOSAICitResValueMultiplier* GetCitResValueMultiplier(){ return &m_CitResValueMultiplier; }
		float GetCitResValue_FoodMultiplier(){ return m_CitResValueMultiplier.m_fFoodValueMultiplier; }
		float GetCitResValue_IronMultiplier(){ return m_CitResValueMultiplier.m_fIronValueMultiplier; }
		float GetCitResValue_OilMultiplier(){ return m_CitResValueMultiplier.m_fOilValueMultiplier; }
		float GetCitResValue_CityMultiplier(){ return m_CitResValueMultiplier.m_fCityProductionValueMultiplier; }
		float GetCitResValue_ResourceMultiplier( CString strResType ){ return m_CitResValueMultiplier.GetResourceValueMultiplier( strResType ); }
*/
		//void  SetRand( long iRand ){ m_iRand = iRand; }
		//long  GetRand(){ return m_iRand; }

		// Recalculation flags
		//void  NeedToRecalculateTradeValues( bool b ){} // INCOMPLETE: Need to fix this.  It is called after a trade is completed.  Maybe the AI went from war to peace, maybe he sold all his oil, maybe...?

		// AI cheats?
		float AICheat_ExpandTowardsHumanPlayers01(){ return 1.0f; }
		bool  AICheat_BonusMoneyAndResourcesEachTurn(){ return true; }
		bool  AICheat_50PercentFreeResourcesInShortage(){ return true; }
		bool  AICheat_KnowsMapAndPoiInformation(){ return m_bAICheat_KnowsMapAndPoiInformation; }
		bool  AICheat_CanInstantUpgrade(){ return true; }
		float AICheat_GetFreeResearchPoints(){ return m_fAICheat_FreeResearchPoints; }
		float AICheat_GetCityProductionMultiplier(){ return m_fAICheat_CityProductionMultiplier; }
		float AICheat_LikesAIPlayers(){ return m_fAICheat_LikesAIPlayers; }
		float AICheat_LikesHumanPlayers(){ return m_fAICheat_LikesHumanPlayers; }
		float AICheat_AttackMultiplierAgainstMilitia(){ return m_fAICheat_AttackMultiplierAgainstMilitia; }

		bool  FieldOrders_AutoAttackMilitia(){ return true; }

		//
		//void  DeleteAIDebugData();
		//void  DeleteAIData();
		//void  ClearUnitOrders();
		//void  SetMyUnitsFieldOrders();
		//void  ClearCityOrders();

		//
		//float GetMyLandFoodResources();
		//float GetMySeaFoodResources();

		//
		// Multithreading code
		//
			// Flags
			//void  Incoming_ServerToPlayerUpdateWasProcessed();
			//void  Incoming_AutosaveAndLocalPlayerTurnReplayEnded();

			void  SetFlag_ShutdownAIPlayer( bool b ){ m_bFlag_ShutdownAIPlayer = b; }
			bool  GetFlag_ShutdownAIPlayer(){ return m_bFlag_ShutdownAIPlayer; }

		// Processing
		//
			//void  Process();
			//void  SendOrders();

		// Nationwide Pathways - I might want to move this into the WorldDescServer
		//
			//void  CalculateMyNationwidePathways();
			//CEOSAIMultiRegionNationwidePathways*  GetMyNationwidePathway();
			//CEOSAIMultiRegionNationwidePathways*  GetNationwidePathway( long iPlayer );

		// Resource Tracker
		//
			//CAIResourceTracker*  GetMoneyTracker(){ return &m_MoneyAmount; }
			//CAIResourceTracker*  GetIronTracker(){ return &m_IronAmount; }
			//CAIResourceTracker*  GetOilTracker(){ return &m_OilAmount; }
			//CAIResourceTracker*  GetFoodTracker(){ return &m_FoodAmount; }

		// Foreign Relations
		//
			//void  SignedAPeaceAgreement( long iPlayer1, long iPlayer2 );

		//
		// Trade, IMail
		//
			// IMail
			long GetNextAIMessageUID(){ m_iNextAIMessageUID++; return m_iNextAIMessageUID; }
			//void Incoming_DeclarationOfWar( long iActor, long iTarget, long iTurn );
		//	void Incoming_IMail( CEOSAIMail* pIMail );
		//	void Incoming_IMailResponse( long iIMailId, long iPlayer, EOSAI::EnumAIMailResponse eResponse );
			// Trade Agreements
		//	void Incoming_TradeOffer( CEOSAITradeAgreement* pTradeAgreement );
		//	void Incoming_TradeAgreementResponse( CString strTradeAgreementId, long iPlayerWhoInitiatedChange, EOSAIEnumTradeAgreementResponse eResponse, EOSAIEnumTradeAgreementState eNewState );
			// Events
			//void IncomingEvent_PlayerResigned( long iPlayerNumberWhoResigned );
			//void IncomingEvent_TeamFormed( CString strTeamName );
			//
			//void FindUnitsWithinMyNationalBoundaries();
			//void SendMessagesAndAdjustForeignRelationsBasedOnBorderViolations();
			//
			//void  CalculateCitResValues();
			//void  CalculateExperiencingResourceShortage01();
			//float GetExperiencingResourceShortage01(){ return m_fExperiencingResourceShortage01; }
			//void AddedBuildItemToResourceConsumption( CBuildOption* pBuildOption );
			//
/*
		//	bool ThereAreUnprocessedEvents();
		//	bool ProcessUnprocessedEvents();
			bool ProcessUnprocessedEvents(){ ASSERT( false ); return true; }
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
		long       m_iPlayer;
		EnumState  m_eProcessingState;
		bool       m_bTrigger_NeedToRecalculateEntireTurn;
		//CCriticalSection   m_CriticalSection;

		//#ifdef _DEBUG
		//CMemoryState  m_DebugDialogMemoryState;
		//#endif _DEBUG


	private:

		//EOSAI::CGamePlayer*  m_pPlayer;  // Owned by the game
		//CEOSAIBrain*  m_pAIBrain; // Owned by AIPlayer
		//CWorldDescPlayerProxy*  m_pWorldDescPlayerProxy;
		//CWorldDescServer*       m_pWorldDescServer;

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

		//long   m_iRand; // changes each turn

		// AICheats
		//
			bool   m_bAICheat_KnowsMapAndPoiInformation;
			float  m_fAICheat_FreeResearchPoints;
			float  m_fAICheat_CityProductionMultiplier;
			float  m_fAICheat_LikesAIPlayers;
			float  m_fAICheat_LikesHumanPlayers;
			float  m_fAICheat_AttackMultiplierAgainstMilitia;
		//float  m_fAICheat_LikesAIPlayers;
		//float  m_fAICheat_LikesHumanPlayers;

		// Processing Situation
		//
			// set to true if foreign relations change (e.g. peace agreement, war declaration, ...)
			//bool   m_bUpdateForeignRelationsCalculations;

		//
			long  m_iNextAIMessageUID;  // Helps track what responses are to which messages
		//	long  m_iDebugTempCounter;

		public:

		// Remembered from turn to turn, involves things like:
		//    "build-up a military to attack player X",
		//    "I plan to sneak-attack player X, so I'm secretly moving units towards him"
		//
		//	CEOSAIStrategicAI  m_StrategicAI;

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
		//	CAICitResValueMultiplier   m_CitResValueMultiplier;
			// The Value is a measure of how concerned the AI should be about shortages
			//   0.0 = no shortage, 0.01-0.99 = upcoming shortage, 1.0 = experiencing shortage
		//	float  m_fExperiencingResourceShortage01;

			//CTradeAgreementMultiplier  m_TradeAgreementMultiplier;
			/*
			CList< CEOSAIMail* >                  m_UnprocessedMail;
			CList< CEOSAIMailResponse* >          m_UnprocessedMailResponses;
			CList< CEOSAITradeAgreement* >        m_UnprocessedTrade;
			CList< CEOSAITradeAgreementResponse* >  m_UnprocessedTradeAgreementResponses;
			*/

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
};

//#endif // FOOCLASS_H
#endif EOSAI_AI_PLAYER_H

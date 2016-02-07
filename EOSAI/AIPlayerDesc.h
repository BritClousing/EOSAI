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
	/*
	public:
		enum EnumState
		{
			enumWaitingForServerToPlayerUpdate = 1,
			//enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd = 2,
			enumBeginProcessingTurn = 3,
			//enumProcessingTurn = 4,
			enumReadyToSendOrdersToServer = 5, // Waiting For Local Human Player To Submit Turn
		};
	*/
	public:
		AIPlayerDesc( long iPlayer, long iAIBrainIntelligence3 );
		virtual ~AIPlayerDesc(){}

		//void SetPlayerNumber( long i ){ m_iPlayerNumber = i; }
		long GetPlayerNumber(){ return m_iPlayer; }

		// Serialize/Deserialize
		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );

		// Intelligence/Personality
		long  GetAIBrainIntelligence3(){ return m_iAIBrainIntelligence3; } // 1 = WeakAI, 2 = ModerateAI, 3 = StrongAI
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

		//bool  FieldOrders_AutoAttackMilitia(){ return true; }

/*
		//
		// Multithreading code
		//
			void  SetFlag_ShutdownAIPlayer( bool b ){ m_bFlag_ShutdownAIPlayer = b; }
			bool  GetFlag_ShutdownAIPlayer(){ return m_bFlag_ShutdownAIPlayer; }
*/

	public:
		long       m_iPlayer;

	private:

		//bool   m_bFlag_ShutdownAIPlayer;

	// Intelligence / Personality
	//
		long   m_iAIBrainIntelligence3; // 1 = WeakAI, 2 = ModerateAI, 3 = StrongAI
		//
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

		//
			//long  m_iNextAIMessageUID;  // Helps track what responses are to which messages
		//	long  m_iDebugTempCounter;
	};
};

//#endif // FOOCLASS_H
#endif EOSAI_AI_PLAYER_H

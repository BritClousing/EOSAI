
#include "stdafx.h"
#include "AIPlayerDesc.h"
//#include "EOSAISerial2.h"
#include "EOSAISerial.h"
#include "EOSAIMath.h"

using namespace EOSAI;

AIPlayerDesc::AIPlayerDesc( long iPlayer, long iAIBrainIntelligence3 )
{
	//ASSERT( false );

	m_iPlayer = iPlayer;
	m_iAIBrainIntelligence3 = iAIBrainIntelligence3;
	//m_bFlag_ShutdownAIPlayer = false;

	//m_iNextAIMessageUID = 1;

	/*
	m_fPersonality_HoldsGrudges01 = 0.0f;
	m_fPersonality_Aggressive01 = 0.0f;
	m_fPersonality_Fortifier01 = 0.0f;
	m_fPersonality_Builder01 = 0.0f;
	m_fPersonality_ProResearcherPos1_AntiResearcherNeg1 = 0.0f;
	m_fPersonality_ProSubmarine01 = 0.0f;
	//
	m_fPersonality_ProAirUnit11 = 0.0f;
	m_fPersonality_ProGroundUnit11 = 0.0f;
	m_fPersonality_ProSeaUnit11 = 0.0f;
	//
	m_fPersonality_ProStrategicBombing01 = 0.0f;
	m_fPersonality_ProMissile01 = 0.0f;
	m_fPersonality_ProArtillery01 = 0.0f;
	m_fPersonality_ProExpensiveShipsPos1_CheapShipsNeg1 = 0.0f; // 1.0 = Battleships, 0.0 = Destroyers
	m_fPersonality_ProUnitDiversity01 = 0.0f;
	*/
	m_fPersonality_HoldsGrudges01 = CEOSAIMath::GetRandomFloat( 0.4f,0.6f ); // 0.5f;
	m_fPersonality_Aggressive01 = CEOSAIMath::GetRandomFloat( 0.2f,0.3f )+CEOSAIMath::GetRandomFloat( 0.2f,0.4f ); // 0.4-0.7
	m_fPersonality_Fortifier01 = CEOSAIMath::GetRandomFloat( 0.3f,0.5f );//0.5f;
	m_fPersonality_Builder01 = CEOSAIMath::GetRandomFloat( 0.1f,0.4f )+CEOSAIMath::GetRandomFloat( 0.1f,0.4f ); // 0.5f;
	m_fPersonality_ProResearcherPos1_AntiResearcherNeg1 = CEOSAIMath::GetRandomFloat( 0.0f,0.5f );//0.25f; + CEOSAIMath::GetRandomFloat( 0.0f,0.5f );
	m_fPersonality_ProSubmarine01 = CEOSAIMath::GetRandomFloat( -0.1f,0.2f );
	if( m_fPersonality_ProSubmarine01 < 0.0f ){ m_fPersonality_ProSubmarine01 = 0.0f; }
	//m_fPersonality_ProAircraftPos1_ProGroundSeaNeg1 = -0.3f; // 0.0 = no influence
	m_fPersonality_ProAirUnit11 = -0.3f;
	m_fPersonality_ProGroundUnit11 = 0.2f;
	m_fPersonality_ProSeaUnit11 = CEOSAIMath::GetRandomFloat( 0.3f,0.5f ); // 0.2 - 0.5

	m_fPersonality_ProStrategicBombing01 = 0.0f;    // 0.0 = no influence
	m_fPersonality_ProMissile01 = 0.0f;             // 0.0 = no influence
	m_fPersonality_ProArtillery01 = 0.0f;           // 0.0 = no influence
	m_fPersonality_ProExpensiveShipsPos1_CheapShipsNeg1 = CEOSAIMath::GetRandomFloat( -0.2f,+0.1f ); // 0.0 = no influence
	m_fPersonality_ProUnitDiversity01 = CEOSAIMath::GetRandomFloat( 0.1f,0.3f );//0.2f;

	//m_iRand = 0; // changes each turn

	// AICheats
	//
		m_bAICheat_KnowsMapAndPoiInformation = false; 

		// Setting up the default cheats.  These values can be changed after calling the constructor.
		m_fAICheat_FreeResearchPoints = 0.0f;
		if (m_iAIBrainIntelligence3 == 1) m_fAICheat_FreeResearchPoints = -2.0f; // this is a penalty
		if (m_iAIBrainIntelligence3 == 2) m_fAICheat_FreeResearchPoints = +0.0f;
		if (m_iAIBrainIntelligence3 == 3) m_fAICheat_FreeResearchPoints = +2.5f;

		m_fAICheat_CityProductionMultiplier = 1.0f;
		if (m_iAIBrainIntelligence3 == 1){ m_fAICheat_CityProductionMultiplier = 0.7f; } // this is a penalty
		if (m_iAIBrainIntelligence3 == 2){ m_fAICheat_CityProductionMultiplier = 1.0f; }
		if (m_iAIBrainIntelligence3 == 3){ m_fAICheat_CityProductionMultiplier = 1.3f; }

		m_fAICheat_LikesAIPlayers = 0.0f;
		if (m_iAIBrainIntelligence3 == 1){ m_fAICheat_LikesAIPlayers = -0.03f; } // Novice AI tends to prefer attacking other AIs
		if (m_iAIBrainIntelligence3 == 2){ m_fAICheat_LikesAIPlayers = 0.0f; }
		if (m_iAIBrainIntelligence3 == 3){ m_fAICheat_LikesAIPlayers = 0.0f; }

		m_fAICheat_LikesHumanPlayers = 0.0f;
		if (m_iAIBrainIntelligence3 == 1){ m_fAICheat_LikesHumanPlayers = 0.0f; }
		if (m_iAIBrainIntelligence3 == 2){ m_fAICheat_LikesHumanPlayers = -0.03f; }
		if (m_iAIBrainIntelligence3 == 3){ m_fAICheat_LikesHumanPlayers = -0.06f; } // Elite AI tends to prefer attacking humans

		m_fAICheat_AttackMultiplierAgainstMilitia = 1.0f;
		if (m_iAIBrainIntelligence3 == 1){ m_fAICheat_AttackMultiplierAgainstMilitia = 1.0f; }
		if (m_iAIBrainIntelligence3 == 2){ m_fAICheat_AttackMultiplierAgainstMilitia = 1.4f; }
		if (m_iAIBrainIntelligence3 == 3){ m_fAICheat_AttackMultiplierAgainstMilitia = 2.0f; } // Elite AI tends to prefer attacking humans

}

// Serialize/Deserialize
void  AIPlayerDesc::Serialize( CEOSAISerial* pSerial )
{
	char iVersion = 4;
	pSerial->Serialize( iVersion ); // Version

	//pSerial->Serialize( m_iNextAIMessageUID );
	pSerial->Serialize( m_iAIBrainIntelligence3 );

	pSerial->Serialize( m_fPersonality_HoldsGrudges01 );
	pSerial->Serialize( m_fPersonality_Aggressive01 );
	pSerial->Serialize( m_fPersonality_Fortifier01 );
	pSerial->Serialize( m_fPersonality_Builder01 );
	pSerial->Serialize( m_fPersonality_ProResearcherPos1_AntiResearcherNeg1 );
	pSerial->Serialize( m_fPersonality_ProSubmarine01 );
	/*
	if( iVersion <= 3 )
	{
		pSerial->Serialize( m_fPersonality_ProAircraftPos1_ProGroundSeaNeg1 );
	}
	*/
	if( iVersion >= 4 )
	{
		pSerial->Serialize( m_fPersonality_ProAirUnit11 );
		pSerial->Serialize( m_fPersonality_ProGroundUnit11 );
		pSerial->Serialize( m_fPersonality_ProSeaUnit11 );
	}
	pSerial->Serialize( m_fPersonality_ProStrategicBombing01 );
	pSerial->Serialize( m_fPersonality_ProMissile01 );
	pSerial->Serialize( m_fPersonality_ProArtillery01 );
	pSerial->Serialize( m_fPersonality_ProExpensiveShipsPos1_CheapShipsNeg1 );
	pSerial->Serialize( m_fPersonality_ProUnitDiversity01 );
}

void  AIPlayerDesc::Deserialize( CEOSAISerial* pSerial )
{
	char iVersion = 0;
	pSerial->Deserialize( iVersion );

	//long iProcessingState = (long) m_eProcessingState;
	//pSerial->Deserialize( iProcessingState );
	//m_eProcessingState = (EnumState) iProcessingState;
	// If we are deserializing, then we are loading a game.
	//   If we are loading a game, then restart the processing
	//m_eProcessingState = enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd;
	//m_eProcessingState = enumBeginProcessingTurn;

	//pSerial->Deserialize( m_iNextAIMessageUID );
	pSerial->Deserialize( m_iAIBrainIntelligence3 );

	if( iVersion == 3 )
	{
		pSerial->Deserialize( m_fPersonality_HoldsGrudges01 );
		pSerial->Deserialize( m_fPersonality_Aggressive01 );
		pSerial->Deserialize( m_fPersonality_Fortifier01 );
		pSerial->Deserialize( m_fPersonality_Builder01 );
		pSerial->Deserialize( m_fPersonality_ProResearcherPos1_AntiResearcherNeg1 );
		pSerial->Deserialize( m_fPersonality_ProSubmarine01 );

		float fPersonality_ProAircraftPos1_ProGroundSeaNeg1 = 0.0f;
		pSerial->Deserialize( fPersonality_ProAircraftPos1_ProGroundSeaNeg1 );
		m_fPersonality_ProAirUnit11 = fPersonality_ProAircraftPos1_ProGroundSeaNeg1;
		m_fPersonality_ProGroundUnit11 = 1.0f-fPersonality_ProAircraftPos1_ProGroundSeaNeg1;
		m_fPersonality_ProSeaUnit11 = 1.0f-fPersonality_ProAircraftPos1_ProGroundSeaNeg1;

		pSerial->Deserialize( m_fPersonality_ProStrategicBombing01 );
		pSerial->Deserialize( m_fPersonality_ProMissile01 );
		pSerial->Deserialize( m_fPersonality_ProArtillery01 );
		pSerial->Deserialize( m_fPersonality_ProExpensiveShipsPos1_CheapShipsNeg1 );
		pSerial->Deserialize( m_fPersonality_ProUnitDiversity01 );
	}
	if( iVersion >= 4 )
	{
		pSerial->Deserialize( m_fPersonality_HoldsGrudges01 );
		pSerial->Deserialize( m_fPersonality_Aggressive01 );
		pSerial->Deserialize( m_fPersonality_Fortifier01 );
		pSerial->Deserialize( m_fPersonality_Builder01 );
		pSerial->Deserialize( m_fPersonality_ProResearcherPos1_AntiResearcherNeg1 );
		pSerial->Deserialize( m_fPersonality_ProSubmarine01 );

		pSerial->Deserialize( m_fPersonality_ProAirUnit11 );
		pSerial->Deserialize( m_fPersonality_ProGroundUnit11 );
		pSerial->Deserialize( m_fPersonality_ProSeaUnit11 );

		pSerial->Deserialize( m_fPersonality_ProStrategicBombing01 );
		pSerial->Deserialize( m_fPersonality_ProMissile01 );
		pSerial->Deserialize( m_fPersonality_ProArtillery01 );
		pSerial->Deserialize( m_fPersonality_ProExpensiveShipsPos1_CheapShipsNeg1 );
		pSerial->Deserialize( m_fPersonality_ProUnitDiversity01 );
	}
}

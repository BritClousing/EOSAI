
#include "stdafx.h"
#include "AIPlayer.h"
#include "EOSAIMail.h"
#include "EOSAIMailResponse.h"
#include "EOSAITradeAgreementResponse.h"
#include "EOSAINationalSummary3.h"
#include "EOSAIPlayerManager.h"
#include "EOSAIInterface.h"

using namespace EOSAI;
extern CInterface* g_pEOSAIInterface;

/*
void EOSAI::AIPlayer::ProcessTurn()
{
	Beep(200,200);
}
*/

/*
Sequence:

Basic Data
*. Create a list of production options
*. Create AIObjects for everything in the game.  Attach ownership status to everything,
	but not friend/enemy attributes.
*. Create a AIRegions, MultiRegionMap, AIGeos.
	Record each player's presence in each region, and also their movement pathways.
	(Create a region map, then a multiregion map.)
*. Create NationalSummaries (with no allies/enemies information)
	Make a simplistic (Level1) desire matrix to attack each player

Foreign Relations (Current) (can vary within a single turn)
*. Calculate Resource future excesses and shortages (can vary based on trade agreements)
*. Update NationalSummaries with information based on: past history, War/Peace status
	* Read the current foreign relations information, apply friend/enemy status to multiregions.
	* If the friend/enemy status changes, I need to revisit this step and move through
	  the rest of the subsequent steps.
*. Evaluate Multiregions for accessibility, value, danger (depends on foreign relations)

Planning
*. Evaluate some SubPlans (war, peace, sneak-attacks, ... with the other players)
*. Create/Update the GrandPlan

Negotiations, Hypothetical Foreign Relations
*. Figure out what resources I want to trade
*. Send emails out asking for trade and war/peace negotiations 
   (responses reset me to the "Foreign Relations" step)

Unit/City Tasks
*. Create hypothetical unit tasks, city build tasks
*. Schedule tenative unit and city tasks using AI objects
*. Create orders for units and cities.

*. Submit orders after the human player does
	I want to wait until after the human players submit - just in case they offer me
	an agreement or change their foreign policy towards me.
*/

/*
Evaluate (and assume) the landscape - for the production of new units, sending out units (e.g. the AI should build and send out transports early)
Make decisions about the islands I am going to capture and consolidate.

Evaluate (and assume) the landscape to determine the types of units that will be needed.

Decide where resources will be allocated:
 - If the computer player is playing the Germans, defeating the French and Russians is more important than building transports for the invasion of Britain.
 - If the computer player is playing Britian, it has to realize that the island is consolidated, and therefore, more attention should be given to the air and sea war plus the eventual overseas invasion.
 - Determine the immediacy of each objective

Iterate through the poi scoring
  Detect if the citres is on an island with some of my own poi (cities or citres-capturing units).
  Determine the state of the island (is it me + neutral, me + allies, me + enemies, me + neutral + high danger, or me + enemies + neutral?)
  Detect if the resource is water-based.  If so, consider sending a ship to capture it (balanced against other objectives, danger map, convenience)

Sending units to capture citres on my own island:
  Determine the danger of the objective using the danger map.
  If there are multiple citres, evaluate the traveling salesman algorithm to find the most efficient way of capturing them.
  If it is a low-risk target, find a citres capturing unit.  Give orders to capture it.

Sending units to capture islands:
  Locate transports (air and sea), load city/resource capturing unit(s) into it.  Give orders to go there, unload troops, capture, and consolidate.

Create a scheduling system - 
  Match units with target poi in a variety of ways to find the best schedule
   - Take into account time to arrival (simple method: distance, complex method: actual path)
   - Create a class where we encapsulate unit-to-target calculation
     - include distance, calculation could include multiple targets
*/

#include "stdafx.h"

#include "AIPlayer.h"
//#include "GameAppState.h"
//#include "CommonState.h"
#include "EOSAIUnit2.h"
#include "City.h"
#include "EOSAIResource.h"
//#include "GeoLand.h"
//#include "AIPoiProxyMobile.h"
//#include "AIPoiProxyOther.h"
//#include "AINationalDesires.h"
#include "EOSAICommonData.h"
//#include "City.h"

//#include "AITaskScheduler.h"
#include "EOSAIStopwatch2.h"
//#include "PoiPD.h"
//#include "AIUnresolvedSituation.h"
#include "EOSAIMultiRegionNationwidePathways.h"
#include "EOSAIQuickCombatCalculation.h"

//#include "AIDesireToControlGeo.h"
//#include "AIDesireToCaptureCitRes.h"

//#include "MessageManager.h"

//#include "InternalMail.h"
#include "EOSAIStrategicAI.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"
//#include "Player.h"
//#include "TWDx.h"

#include "EOSAIPlayerInteraction.h"
//#include "GameEvent_IMail.h"
//#include "Message2_AIPlayerReadyToSubmitTurn.h"
//#include "Message2_AIPlayerNotReadyToSubmitTurn.h"
#include "EOSAIEnumFieldOrders.h"
//#include "WorldDescPlayerProxy.h"
#include "EOSAIPlayerInteraction_BorderViolation.h"
//#include "PoiObserver.h"
//#include "MessageTargetManager.h"

//#include "Checkpoint.h"

#include "EOSAIBrain.h"
#include "EOSAIMath.h"

//#include "CombatManager.h" // DEBUG
#include <mmsystem.h>

//#include "LanguagePack.h"
//#include "AISuperPathMap.h"
//extern CString Lang( long iID, CString strEnglish );

//CAISuperPathMap  g_AISuperPathMap;

#ifdef _DEBUG
static long s_iAILastReplayTurn = 0;
#endif _DEBUG

//#include "AISymbolProcessor.h"
//#include "EOSAIBrain.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
/*
UINT CAIPlayer_ThreadEntranceFunction( LPVOID pParam )
{
	CAIPlayer* pAIPlayer = (CAIPlayer*) pParam;
	pAIPlayer->Loop();
	return 0;
}
*/
using namespace EOSAI;

void  CEOSAIDebugData::AddToAnnotations( CEOSAILocation Location, float fRadius, float fRed01, float fGreen01, float fBlue01 )
{
	// Look through the existing Annotations, if there is one at that location, ...
	POSITION pos = m_Annotations.GetHeadPosition();
	while( pos )
	{
		CEOSAIAnnotation* pExisting = m_Annotations.GetNext( pos );
		if( pExisting->m_CenterLocation == Location )
		{
			pExisting->m_fRadius = max( pExisting->m_fRadius, fRadius );
			return;
		}
	}
	m_Annotations.AddTail( new CEOSAIAnnotation( Location, fRadius, fRed01,fGreen01,fBlue01 ) );
}

AIPlayer::AIPlayer()
{
	m_pAIPlayerDesc = NULL;
	m_pGamePlayer = NULL;
	m_iPlayerNumber = 0;
	//
	m_pAIBrain = new CEOSAIBrain();
	m_pAIBrain->Initialize( this );

	// State
	m_fExperiencingResourceShortage01 = 0.0f;

	m_iRand = rand();

	m_eProcessingState = enumBeginProcessingTurn;//enumWaitingForServerToPlayerUpdate;
	m_bTrigger_NeedToRecalculateEntireTurn = false;

//	m_iNextAIMailId = 0;
	m_iDebugTempCounter = 0;

	// Reset the Multiplier values
	m_CitResValueMultiplier.m_fCityProductionValueMultiplier = 2.0f;
	m_CitResValueMultiplier.m_fIronValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fFoodValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fOilValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fGoldValueMultiplier = 1.0f;

	m_StrategicAI.SetAIPlayer( this );
	//m_StrategicAI.InvokePlayers( g_pEOSAICommonData->GetNumberOfPlayers() );
	m_StrategicAI.InvokePlayers( EOSAI_MAX_NUMBER_OF_PLAYERS );
}

//CAIPlayer::CAIPlayer( CWorldDescServer* pWorldDescServer, CWorldDescPlayer* pWorldDescPlayer, long iAIBrainIntelligence )
//AIPlayer::AIPlayer( EOSAI::CGamePlayer* pPlayer, long iAIBrainIntelligence )
AIPlayer::AIPlayer( long iNumberOfPlayers, EOSAI::CGamePlayer* pGamePlayer, EOSAI::AIPlayerDesc* pAIPlayerDesc )
{
	ASSERT(pGamePlayer);
	ASSERT(pAIPlayerDesc);

	m_pGamePlayer = pGamePlayer;//pWorldDescPlayer->GetPlayerMe(); //pPlayer;
	m_pAIPlayerDesc = pAIPlayerDesc;

	m_iPlayerNumber = pGamePlayer->GetPlayerNumber();

	//m_pWorldDescPlayer = pWorldDescPlayer;
	//m_pWorldDescServer = pWorldDescServer;
	//m_pWorldDescPlayerProxy = m_pWorldDescServer->GetWorldDescPlayerProxy( m_pPlayer->GetPlayerNumber() );
	m_pAIBrain = new CEOSAIBrain();
	m_pAIBrain->Initialize( this );

	// State
	m_fExperiencingResourceShortage01 = 0.0f;
/*
	// Personality
	m_iAIBrainIntelligence = iAIBrainIntelligence; // 1 = WeakAI, 2 = ModerateAI, 3 = StrongAI
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
*/
	/*
	m_fAICheat_LikesAIPlayers = 0.0f;
	m_fAICheat_LikesHumanPlayers = 0.0f;
	if(  iAIBrainIntelligence == 0 ){}
	eif( iAIBrainIntelligence == 1 ){ m_fAICheat_LikesAIPlayers =  0.0f; m_fAICheat_LikesHumanPlayers =  0.0f; }
	eif( iAIBrainIntelligence == 2 ){ m_fAICheat_LikesAIPlayers = +0.1f; m_fAICheat_LikesHumanPlayers = -0.1f; }
	eif( iAIBrainIntelligence == 3 ){ m_fAICheat_LikesAIPlayers = +0.2f; m_fAICheat_LikesHumanPlayers = -0.2f; }
	else{ ASSERT( false ); }
	*/

//	m_bFlag_ShutdownAIPlayer = false;

	m_iRand = rand();
//	m_bAICheat_KnowsMapAndPoiInformation = true;//true;

	m_eProcessingState = enumBeginProcessingTurn;//enumWaitingForServerToPlayerUpdate;
	m_bTrigger_NeedToRecalculateEntireTurn = false;

//	m_iNextAIMailId = 0;
	m_iDebugTempCounter = 0;

	// Reset the Multiplier values
	m_CitResValueMultiplier.m_fCityProductionValueMultiplier = 2.0f;
	m_CitResValueMultiplier.m_fIronValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fFoodValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fOilValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fGoldValueMultiplier = 1.0f;

	m_StrategicAI.SetAIPlayer( this );
	//m_StrategicAI.InvokePlayers( g_pEOSAICommonData->GetNumberOfPlayers() );
	//m_StrategicAI.InvokePlayers( EOSAI_MAX_NUMBER_OF_PLAYERS );
	m_StrategicAI.InvokePlayers( iNumberOfPlayers );

	// DEBUG: create a fake PlayerInteraction for testing purposes
	//
	/*
	CEOSAIPlayerInteraction_AskedToJoinInWar* pPlayerInteraction = new CEOSAIPlayerInteraction_AskedToJoinInWar();
	pPlayerInteraction->m_iRequestingPlayer = 2;
	pPlayerInteraction->m_iSentTo = 1;
	pPlayerInteraction->m_iEventTurn = 0;
	pPlayerInteraction->m_eResponse = CTradeAgreement::enum_Accept;
	AddPlayerInteraction( pPlayerInteraction );
	*/
	//SetRand( rand() );
}

void AIPlayer::SetPlayerData( EOSAI::CGamePlayer* pGamePlayer, AIPlayerDesc* pAIPlayerDesc )
{
	ASSERT(pGamePlayer);
	ASSERT(pAIPlayerDesc);

	m_pGamePlayer = pGamePlayer;
	m_pAIPlayerDesc = pAIPlayerDesc;

	m_iPlayerNumber = pGamePlayer->GetPlayerNumber();
}

AIPlayer::~AIPlayer()
{
	DeleteAIDebugData();
	delete m_pAIBrain;
	//while( m_UnprocessedIncomingMail.IsEmpty() == FALSE ){ delete m_UnprocessedIncomingMail.RemoveHead(); }
	//while( m_UnprocessedIncomingMailResponses.IsEmpty() == FALSE ){ delete m_UnprocessedIncomingMailResponses.RemoveHead(); }
	//while( m_UnprocessedIncomingTradeAgreementResponses.IsEmpty() == FALSE ){ delete m_UnprocessedIncomingTradeAgreementResponses.RemoveHead(); }
}


// Serialize/Deserialize
void  AIPlayer::Serialize( CEOSAISerial* pSerial )
{
	char iVersion = 4;
	pSerial->Serialize( iVersion ); // Version
/*
	pSerial->Serialize( m_iNextAIMailId );
	pSerial->Serialize( m_iAIBrainIntelligence );

	pSerial->Serialize( m_fPersonality_HoldsGrudges01 );
	pSerial->Serialize( m_fPersonality_Aggressive01 );
	pSerial->Serialize( m_fPersonality_Fortifier01 );
	pSerial->Serialize( m_fPersonality_Builder01 );
	pSerial->Serialize( m_fPersonality_ProResearcherPos1_AntiResearcherNeg1 );
	pSerial->Serialize( m_fPersonality_ProSubmarine01 );
	/-*
	if( iVersion <= 3 )
	{
		pSerial->Serialize( m_fPersonality_ProAircraftPos1_ProGroundSeaNeg1 );
	}
	*-/
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
*/
}

void  AIPlayer::Deserialize( CEOSAISerial* pSerial )
{
	char iVersion = 0;
	pSerial->Deserialize( iVersion );

	//long iProcessingState = (long) m_eProcessingState;
	//pSerial->Deserialize( iProcessingState );
	//m_eProcessingState = (EnumState) iProcessingState;
	// If we are deserializing, then we are loading a game.
	//   If we are loading a game, then restart the processing
	//m_eProcessingState = enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd;
	m_eProcessingState = enumBeginProcessingTurn;
/*
	pSerial->Deserialize( m_iNextAIMailId );
	pSerial->Deserialize( m_iAIBrainIntelligence );

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
	*/
}
/*
void CAIPlayer::SetWorldDescPlayer( CWorldDescPlayer* p )
{
	m_pWorldDescPlayer = p;
	m_pAIBrain->SetWorldDescPlayer( p );
}
*/
/*
long AIPlayer::GetPlayerNumber()
{
	return m_pPlayer->m_iPlayer;
}
*/
/*
float AIPlayer::AICheat_ExpandTowardsHumanPlayers01()
{
	return 1.0f;
}
*/
/*
float AIPlayer::AICheat_GetFreeResearchPoints()
{
	if( m_iAIBrainIntelligence == 1 ) return -2.0f;
	if( m_iAIBrainIntelligence == 2 ) return +0.0f;
	if( m_iAIBrainIntelligence == 3 ) return +2.5f;
	return 0.0f;
}
*/
/*
float AIPlayer::AICheat_GetCityProductionMultiplier()
{
	if( m_iAIBrainIntelligence == 1 ){ return 0.7f; }
	if( m_iAIBrainIntelligence == 2 ){ return 1.0f; }
	if( m_iAIBrainIntelligence == 3 ){ return 1.3f; }
	else{ ASSERT( false ); }
	return 1.0f;
}
*/
/*
float AIPlayer::AICheat_LikesAIPlayers()//{ return m_fAICheat_LikesAIPlayers; }
{
	if( m_iAIBrainIntelligence == 1 ){ return -0.03f; } // Novice AI tends to prefer attacking other AIs
	if( m_iAIBrainIntelligence == 2 ){ return 0.0f; }
	if( m_iAIBrainIntelligence == 3 ){ return 0.0f; }
	else{ ASSERT( false ); }
	return 0.0f;
}
*/
/*
float AIPlayer::AICheat_LikesHumanPlayers()//{ return m_fAICheat_LikesHumanPlayers; }
{
	if(  m_iAIBrainIntelligence == 1 ){ return  0.0f; }
	eif( m_iAIBrainIntelligence == 2 ){ return -0.03f; }
	eif( m_iAIBrainIntelligence == 3 ){ return -0.06f; } // Elite AI tends to prefer attacking humans
	else{ ASSERT( false ); }
	return 0.0f;
	//if( iAIBrainIntelligence == 1 ){ m_fAICheat_LikesAIPlayers =  0.0f; m_fAICheat_LikesHumanPlayers =  0.0f; }
	//eif( iAIBrainIntelligence == 2 ){ m_fAICheat_LikesAIPlayers = +0.1f; m_fAICheat_LikesHumanPlayers = -0.1f; }
	//eif( iAIBrainIntelligence == 3 ){ m_fAICheat_LikesAIPlayers = +0.2f; m_fAICheat_LikesHumanPlayers = -0.2f; }
}
*/
/*
float AIPlayer::AICheat_AttackMultiplierAgainstMilitia()
{
	if(  m_iAIBrainIntelligence == 1 ){ return 1.0f; }
	eif( m_iAIBrainIntelligence == 2 ){ return 1.4f; }
	eif( m_iAIBrainIntelligence == 3 ){ return 2.0f; } // Elite AI tends to prefer attacking humans
	else{ ASSERT( false ); }
	return 0.0f;
}
*/
void  AIPlayer::DeleteAIDebugData()
{
	#ifdef _DEBUG
	m_strDebug_ForeignRelations = _T("");
	m_strDebug_BestTacticalProjectStrategies = _T("");
	while( m_DesiresBySimpleInterest.IsEmpty() == FALSE ){ delete m_DesiresBySimpleInterest.RemoveHead(); }
	while( m_DesiresByScore.IsEmpty() == FALSE ){ delete m_DesiresByScore.RemoveHead(); }
	while( m_TacticalProjectsBySimpleInterest.IsEmpty() == FALSE ){ delete m_TacticalProjectsBySimpleInterest.RemoveHead(); }
	while( m_TacticalProjectsByInitialEvaluation.IsEmpty() == FALSE ){ delete m_TacticalProjectsByInitialEvaluation.RemoveHead(); }
	while( m_TacticalProjects2.IsEmpty() == FALSE ){ delete m_TacticalProjects2.RemoveHead(); }
	#endif _DEBUG
}

void  AIPlayer::DeleteAIData()
{
/*
	while( m_DesiresBySimpleInterest.IsEmpty() == FALSE ){ delete m_DesiresBySimpleInterest.RemoveHead(); }
	Sleep( 1 );
	while( m_DesiresByScore.IsEmpty() == FALSE ){ delete m_DesiresByScore.RemoveHead(); }
	Sleep( 1 );
	while( m_TacticalProjectsBySimpleInterest.IsEmpty() == FALSE ){ delete m_TacticalProjectsBySimpleInterest.RemoveHead(); }
	Sleep( 1 );
	while( m_TacticalProjectsByInitialEvaluation.IsEmpty() == FALSE ){ delete m_TacticalProjectsByInitialEvaluation.RemoveHead(); }
	Sleep( 1 );
*/
	#ifdef _DEBUG
	m_strDebug_ForeignRelations = _T("");
	m_strDebug_BestTacticalProjectStrategies = _T("");
	while( m_DesiresBySimpleInterest.IsEmpty() == FALSE ){ delete m_DesiresBySimpleInterest.RemoveHead(); }
	while( m_DesiresByScore.IsEmpty() == FALSE ){ delete m_DesiresByScore.RemoveHead(); }
	while( m_TacticalProjectsBySimpleInterest.IsEmpty() == FALSE ){ delete m_TacticalProjectsBySimpleInterest.RemoveHead(); }
	while( m_TacticalProjectsByInitialEvaluation.IsEmpty() == FALSE ){ delete m_TacticalProjectsByInitialEvaluation.RemoveHead(); }
	while( m_TacticalProjects2.IsEmpty() == FALSE ){ delete m_TacticalProjects2.RemoveHead(); }
	#endif _DEBUG

	//Beep( 200,200 );
	Sleep(1);
	//long iCount = 0;
	GetAIBrain()->GetAIThoughtDatabase()->Clear();

	GetAIBrain()->DeleteAIData();

	//	GetAIBrain()->DeleteTransportCombinedAIRegionMaps();
	Sleep(1);
//	GetAIBrain()->DeleteTransportComboMaps();
	//Beep( 200,200 );
}
/*
void  CAIPlayer::RemoveUnitAndCityOrders()
{

void CEOSAIBrain::ClearOldData()
{
	m_AIThoughtDatabase.Clear();
	while( m_AIObjectList.IsEmpty() == FALSE ){ delete m_AIObjectList.RemoveHead(); }
	m_AIPoiObjects.RemoveAll();
	ClearUnitOrders();
	ClearCityBuildOrders();
}
}
*/
/*
void AIPlayer::ClearUnitOrders()
{
	//while( m_pWorldDescPlayer->GetReplayLastTurn() ){ Sleep( 100 ); }
	// This is to prevent the system from drawing the map while I'm deleting orders
	//CSingleLock  SingleLock( &m_pWorldDescPlayer->m_AccessWorldData );
	//SingleLock.Lock();

	// Clear unit orders
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	CList< EOSAI::PoiMobile* >  PoiMobiles;
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		EOSAI::PoiMobile* pPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( pAIPoiObject );
		if( pPoiMobile &&
			pPoiMobile->GetOwner() == iAIPlayer )
			//pPoiMobile->GetOrderList()->IsEmpty() == false ) // >GetFirstOrder() )
		{
			PoiMobiles.AddTail( pPoiMobile );
		}
	}
	pos = PoiMobiles.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pPoiMobile = PoiMobiles.GetNext( pos );
		pPoiMobile->DeleteOrders();
	}
}
*/
void AIPlayer::SetMyUnitsFieldOrders()
{
	// Clear unit orders
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	//POSITION pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		EOSAI::PoiMobile* pPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( pAIPoiObject );
		if( pPoiMobile &&
			pPoiMobile->GetOwner() == iAIPlayer )
		{
			CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pPoiMobile );
			if( pUnit )
			{
				//if( pUnit->GetUnitTemplate()->GetUnitCombatCapability()->IsACombatUnit() &&
				if( pUnit->GetAIUnitTemplate()->IsACombatUnit() )
				{
					if( pPoiMobile->GetFieldOrders() != enumFieldOrders_AttackAll )
					{
						pPoiMobile->SetFieldOrders( enumFieldOrders_AttackAll );
					}
				}
				else
				{
					if( pPoiMobile->GetFieldOrders() != enumFieldOrders_Default )
					{
						pPoiMobile->SetFieldOrders( enumFieldOrders_Default );
					}
				}
			}
		}
	}
}

void AIPlayer::ClearCityOrders()
{
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	//POSITION pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity &&
			pAICity->GetOwner() == iAIPlayer )
		{
			pAICity->RemoveAllItemsFromBuildQueue();
		}
	}
}

float AIPlayer::GetMyLandFoodResources()
{
	float fAmount = 0.0f;
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAIResource* pRes = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
		if( pRes &&
			pRes->GetOwner() == iAIPlayer &&
			pRes->GetResourceType() == _T("Food") &&
			pRes->ResourceSource_IsOnLand() )
		{
			fAmount += pRes->GetResourcePerTurn();
			//pRes->RemoveAllItemsFromBuildQueue();
		}
	}
	return fAmount;
}

float AIPlayer::GetMySeaFoodResources()
{
	float fAmount = 0.0f;
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAIResource* pRes = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
		if( pRes &&
			pRes->GetOwner() == iAIPlayer &&
			pRes->GetResourceType() == _T("Food") &&
			pRes->ResourceSource_IsOnLand() == false )
		{
			fAmount += pRes->GetResourcePerTurn();
			//pRes->RemoveAllItemsFromBuildQueue();
		}
	}
	return fAmount;
}

//
/*
void  CAIPlayer::StartThread()
{
	ASSERT( m_bThreadIsRunning == false );
	if( m_bThreadIsRunning == false )
	{
		CWinThread* m_pThread = AfxBeginThread(
			CAIPlayer_ThreadEntranceFunction,
			(LPVOID) this,
			THREAD_PRIORITY_NORMAL // this is one point below THREAD_PRIORITY_NORMAL (which is 0)
			//THREAD_PRIORITY_NORMAL - 1 // this is one point below THREAD_PRIORITY_NORMAL (which is 0)
			//int nPriority = THREAD_PRIORITY_NORMAL,
			//UINT nStackSize = 0,
			//DWORD dwCreateFlags = 0,
			//LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL 
		);
	}
}
*/

CEOSAIMultiRegionNationwidePathways*  AIPlayer::GetMyNationwidePathway()
{
	CEOSAIMultiRegionNationwidePathways* pPathways = g_pEOSAICommonData->GetNationwidePathways( GetPlayerNumber() );
	ASSERT( pPathways );
	ASSERT( pPathways->GetPlayer() == this->GetPlayerNumber() );
	return pPathways;
	//return GetNationwidePathway( GetPlayer()->GetPlayerNumber() );
}

CEOSAIMultiRegionNationwidePathways*  AIPlayer::GetNationwidePathway( long iPlayer )
{
	return g_pEOSAICommonData->GetNationwidePathways( iPlayer );
	/*
	POSITION pos = m_NationwidePathways.GetHeadPosition();
	while( pos )
	{
		CEOSAIMultiRegionNationwidePathways* pPathways = m_NationwidePathways.GetNext( pos );
		if( pPathways->GetPlayer() == iPlayer ){ return pPathways; }
		//CList< CEOSAIMultiRegionNationwidePathways* >  m_NationwidePathways;
	}
	return NULL;
	*/
}

/*
void  CAIPlayer::Loop_old()
{
	ASSERT( false );

	m_bThreadIsRunning = true;
	// Do an infinite loop - until told to stop by the main thread.
	while( true )
	{
		//if( m_bThreadShouldBeKilled ){ break; }
		if( GetPlayer()->GetPlayerNumber() == 2 )
		{
			int x1=0;
		}
		if( GetPlayer()->GetPlayerNumber() == 3 )
		{
			int x2=0;
		}

		if( m_bThreadShouldBePaused != m_bThreadIsPaused )
		{
			m_bThreadIsPaused = m_bThreadShouldBePaused;
		}
		if( m_bThreadIsPaused )
		{
			Sleep( 100 );
			continue;
		}

		// Update the common MultiRegion data
		/*
		if( m_bUpdateCommonAIData )
		{
			/*
			m_bUpdateCommonAIData = false;
			long iTurn = GetWorldDescPlayer()->GetCurrentTurn();
			//while( GetCommonState()->GetCommonAIData()->GetTurn() < GetWorldDescPlayer()->GetCurrentTurn() )
			//{
				GetCommonState()->GetCommonAIData()->UpdateTurnData( iTurn );
				//Sleep( 500 );
			//}
			*-/
		}
		*-/

		if( g_pGameAppState == NULL ){ break; }
		if( g_pGameAppState->GetEntireGameIsOver() ){ break; }
		if( GetPlayer()->GetPlayerHasBeenEliminated() ){ break; }
		if( m_bThreadShouldBeKilled ){ break; }

		if( m_eProcessingState == enumWaitingForServerToPlayerUpdate )
		{
			Sleep( 100 );
			continue;
		}
		if( m_eProcessingState == enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd )
		{
			// Don't process the AI until after the autosave happens and the TurnReply ended
			Sleep( 100 );
			continue;
		}

		/*
		if( GetWorldDescPlayer()->GetPoiList()->IsEmpty() )
		{
			// This is the first turn, but the ServerToPlayerUpdate hasn't come through yet
			Sleep( 100 );
			continue;
		}
		*-/

		//if( m_bNeedToUpdateTurnInformation )
		if( m_eProcessingState == enumBeginProcessingTurn )
		{
			/*
			while( m_StrategicAI.GetCurrentOrders()->IsEmpty() == FALSE )
			{
				CEOSAIStrategicAIOrder* pOrder = m_StrategicAI.GetCurrentOrders()->RemoveHead();
				m_StrategicAIOrders.AddTail( pOrder );
			}
			*-/
			//

			/*
			if( GetWorldDescPlayer()->GetCurrentTurn() == 1 )
			//if( GetWorldDescPlayer()->GetCurrentTurn() == 1 &&
			//	Public::m_bDoingTesting )
			{
				CInternalMail* pMail = new CInternalMail();
				long iSendTo = 1;
				if( GetPlayer()->GetPlayerNumber() == 1 ){ iSendTo = 2; }
				pMail->InitializeWithPlayerNumbers(
					GetPlayer()->GetPlayerNumber(), 
					iSendTo, 
					"Trade?", 
					"[This is a test message]\r\nDo you want to make a trade?\r\n[This is a test message]" );

				CTradeAgreement* pTradeAgreement = new CTradeAgreement();
				pTradeAgreement->InitializeWithPlayerNumbers( 
					GetPlayer()->GetPlayerNumber(),
					iSendTo,
					GetWorldDescPlayer()->GetNextTradeAgreementId() );
				//pTradeAgreement->m_iPlayerWhoMadeTheOffer = 1;
				//pTradeAgreement->m_iPlayer[0] = GetPlayer()->GetPlayerNumber(); // player number
				//pTradeAgreement->m_iPlayer[1] = iSendTo; // player number
				//pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_BrandNew;
				//pTradeAgreement->m_iTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();
				pTradeAgreement->m_iPlayerMoneyLumpSum[0] = 3;
				pTradeAgreement->m_iPlayerMoneyPerTurn[0] = 3;
				//pTradeAgreement->m_ePlayerResponse[0] = CTradeAgreement::enum_Accept;

				pMail->m_pTradeAgreement = pTradeAgreement;
				GetWorldDescPlayer()->AddTradeAgreement( pTradeAgreement );

				// Send it - Note: all InternalMail goes through the server.
				//    This is because the server needs to assign TradeIds and process Trade "accept"s
				CMessage2_InternalMail InternalMailMessage;
				//InternalMailMessage.InitializeWithMessageIds(
				//	GetPlayer()->GetMessageTargetId(), g_pMessageTargetManager->GetMessageTarget_ServerId(), pMail );
				//InternalMailMessage.SetSentBy( GetPlayer()->GetMessageTargetName() );
				InternalMailMessage.SetSenderId( GetPlayer()->GetMessageTargetId() );
				InternalMailMessage.AddSendTo( g_pMessageTargetManager->GetMessageTarget_ServerId() );
				InternalMailMessage.SetInternalMail( pMail );
				//InternalMailMessage.SetSenderId( pMail->m_iFromMessageId );
				//InternalMailMessage.AddReceiverId( g_pMessageTargetManager->GetMessageTarget_ServerId() ); //&pMail->m_ToMessageIds );
				//InternalMailMessage.SetInternalMail( pMail );
				g_pMessageManager->Send( InternalMailMessage );
			}
			*-/


		}

		if( Public::ActiveAIPlayers() == false )
		{
			if( m_eProcessingState == enumBeginProcessingTurn )
			{
				m_eProcessingState = enumProcessingTurn;

				long iPlayer = GetPlayer()->GetPlayerNumber();
				if( g_pGameAppState )
				{
					g_pMessageManager->CreateIM_AIPlayerIsInactive( iPlayer );
				}

				//m_pAIBrain->GetSpatialMap()->
				m_pAIBrain->Clear();

				//m_bNeedToUpdateTurnInformation = false;
				//m_bReadyToSendOrdersToServer = true;
				//
				m_eProcessingState = enumReadyToSendOrdersToServer;
			}
		}
		if( Public::ActiveAIPlayers() )
		{
			if( m_eProcessingState == enumBeginProcessingTurn )
			{
				m_eProcessingState = enumProcessingTurn;

				// Wait until the turn-replay is done (usually about 5 seconds)
				Sleep( 1000 );
				while( g_pGameAppState->GetWorldDescPlayer()->GetReplayLastTurn() )
				{
					Sleep( 1000 );
				}

				ProcessUnprocessedEvents();

				//
				m_StrategicAI.SetAIPlayer( this );
				//m_StrategicAI.SetPlayer( GetPlayer()->GetPlayerNumber() );
				//m_StrategicAI.SetCurrentTurn( GetWorldDescPlayer()->GetCurrentTurn() );
				m_StrategicAI.InvokePlayers( GetCommonState()->GetNumberOfPlayers() );
				//m_StrategicAI.SetupNationalPowerValues();
				//m_StrategicAI.SetupInputValues();
				//m_StrategicAI.CalculateForeignRelationFeelingsBasedOnPlayerInteractionHistory();
				m_StrategicAI.RunCalculationsCreateOrdersAndStances();
				//m_StrategicAI.CalculateTechnologyStrategy();

				float fPercent = m_StrategicAI.GetPercentageOfCapturedCitRes();

				// debug
				CString strCurrentState = m_StrategicAI.GetAIForeignRelationsState()->OutputDebugString();

				//m_bNeedToUpdateTurnInformation = false;
				if( GetWorldDescPlayer()->GetPoiList()->IsEmpty() )
				{
					continue;
				}
				//m_bAllHumanPlayersHaveSubmittedOrders = false;
				//m_AIBrain.DeletePlan(); // Delete the plan here (instead of later) so we can look at the completed plan later
				//delete m_pAIBrain;
				//m_pAIBrain = NULL;
				m_pAIBrain->Clear();

				// Check for memory leaks
				#ifdef _DEBUG
				CMemoryState MemoryState1;
				MemoryState1.Checkpoint();
				#endif

				CEOSAIStopwatch2 AITimer;
				AITimer.Start();

				m_pWorldDescPlayer->ApplyAllEvents();

				if( m_bThreadShouldBeKilled ){ break; }

				//CPoi* pPoi = m_pWorldDescPlayer->GetPoi( 21 );
				//int x=0;

				//m_AIBrain.Process();
				//delete m_pPlan;
				//m_pAIBrain = new CEOSAIBrain();
				//m_pAIBrain->Initialize( this );

				//this->InstanciateSightings(); // DEBUG

				m_pAIBrain->ResetProcessingFlags();
				m_pAIBrain->Process();

				// Tell the server I am ready to send my orders, but I need to wait and listen
				//   for player-responses and player-offers.  I don't want to end my turn before they
				//   send me an offer which could affect this turn's orders (like a peace offer).

				if( m_bThreadShouldBeKilled ){ break; }

				//EvaluateGameSituation();
				//UpdateBuildOrders();

				if( m_bThreadShouldBeKilled ){ break; }

				//SendServerNotificationThatImReadyToSubmitOrders();
				//m_bReadyToSendOrdersToServer = true;
				ASSERT( m_eProcessingState == enumProcessingTurn );
				if( m_eProcessingState == enumProcessingTurn )
				{
					m_eProcessingState = enumReadyToSendOrdersToServer;
				}

				// Check for memory leaks
				#ifdef _DEBUG
				CMemoryState MemoryState2;
				MemoryState2.Checkpoint();

				CMemoryState DiffState;
				DiffState.Difference( MemoryState1, MemoryState2 );

				// 2735413 bytes in 18897 Normal Blocks ( + m_AIBrain.Process() - m_AIBrain.DeletePlan() )
				// 2164312 bytes in 13631 Normal Blocks ( + m_AIBrain.Process() + m_AIBrain.DeletePlan() )
				// 2102371 bytes in 11986 Normal Blocks ( + m_AIBrain.Process() - InstanciateOrders() + m_AIBrain.DeletePlan() )
				// 2161810 bytes in 13475 Normal Blocks ( + m_AIBrain.Process() + m_AIBrain.DeletePlan() )
				// 2161369 bytes in 13467 Normal Blocks ( + m_AIBrain.Process() + m_AIBrain.DeletePlan() )
				//    2423 bytes in    47 Normal Blocks ( - m_AIBrain.Process() + m_AIBrain.DeletePlan() )

				// Latest:
				// 3839552 bytes in 23361 Normal Blocks.
				//    5621 bytes in    -3 Normal Blocks.
				//    1331 bytes in   -19 Normal Blocks.
				DiffState.DumpStatistics();
				//MemoryState1.DumpAllObjectsSince();
				#endif
				int z=0;

				AITimer.Stop();
				CString strText;
				strText.Format( "CAIPlayer::Loop() - AITimer, player: %d, turn %d, time: %f", 
					GetPlayer()->GetPlayerNumber(), GetWorldDescPlayer()->GetCurrentTurn(), AITimer.GetDeltaSeconds() );
				Checkpoint::Write( strText );

				#ifdef _DEBUG
				strText.Format( "AIPlayer %d ready to send turn.", GetPlayer()->GetPlayerNumber() );
				g_pMessageManager->CreateInstantMessageLineForEachLocalPlayer( GetPlayer()->GetPlayerNumber(), enum_IconClock, strText );
				#endif

				// We'll assume that ProcessTurn takes enough time that
				//   we should check for the paused flag rather than continue.
				continue;
			}
		}
		//if( m_bReadyToSendOrdersToServer )
		if( m_eProcessingState == enumReadyToSendOrdersToServer )
		{
			Sleep( 50 );
			if( ProcessUnprocessedEvents() )
			{
				m_eProcessingState = enumBeginProcessingTurn;
				return; // this will switch to another player
			}
			/*
			if( m_eProcessingState == enumBeginProcessingTurn )
			{
				continue; // go back to the beginning of the loop
			}
			*-/

			long iNumberOfHumanPlayers = GetWorldDescPlayer()->GetWorldBuildDesc()->GetNumberOfHumanPlayers();
			bool bSubmitTurn = false;

			long iServerIsWaitingForTurn = GetCommonState()->GetWorldDescServer()->GetCurrentTurn();
			bool bAllHumanPlayersHaveSubmittedTurn = GetCommonState()->GetWorldDescServer()->AllActiveHumanPlayersHaveSubmittedTurn( iServerIsWaitingForTurn );
			/*
			if( iNumberOfHumanPlayers > 0 && m_bAllHumanPlayersHaveSubmittedOrders )
			{
				bSubmitTurn = true;
			}
			if( iNumberOfHumanPlayers == 0 &&
				GetWorldDescPlayer()->GetWorldBuildDesc()->GetAutoAdvanceTurn() )
			{
				bSubmitTurn = true;
			}
			if( bSubmitTurn )
			{
				SendOrders();
			}
			*-/
			/*
			Sleep( 10 );
			if( GetWorldDescPlayer()->GetWorldBuildDesc()->GetNumberOfHumanPlayers() == 0 &&
				GetWorldDescPlayer()->GetWorldBuildDesc()->GetAutoAdvanceTurn() )
				m_bLocalHumanPlayerHasSubmittedTurn )
			{
				SendOrders();
			}
			*-/

			// If there are no human players (AI-only game), either auto-advance or 
			//   wait for the player to press the "turn" button
			if( iNumberOfHumanPlayers == 0 )
			{
				if( GetWorldDescPlayer()->GetWorldBuildDesc()->GetAutoAdvanceTurn() ||
					m_bAllHumanPlayersHaveSubmittedTurn )
				{
					bSubmitTurn = true;
				}
			}
			else // this is a human-player game, wait for all humans to submit turns
			{
				//if( m_bAllHumanPlayersHaveSubmittedOrders )
				if( bAllHumanPlayersHaveSubmittedTurn )
				{
					bSubmitTurn = true;
					//m_bAllHumanPlayersHaveSubmittedOrders = false;
				}
			}
			if( bSubmitTurn )
			{
				SendOrders();
			}
		}
		/*
		else
		{
			// Wait around for the human player (do nothing)
			Sleep( 50 );
		}
		*-/

		Sleep( 100 );
	}
	//m_AIBrain.DeletePlan();
	m_bThreadIsRunning = false;
}
*/
void  AIPlayer::Process()
{
	CString strText;
	long iAIPlayer = GetPlayerNumber();

	ASSERT( g_pEOSAIInterface != NULL );

	//CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
/*
	if( g_pEOSAICommonData->GetNumberOfHumanPlayers() == 0 )
	{
		// Don't process the turn if the WorldDescServer is in replay mode
		//if( pWorldDescServer->GetReplayLastTurn() ){ return; }
	}
*/
	if( g_pAIPlayerManager->GetDebugPause() ){ return; }
	if( g_pEOSAIInterface->GetNumberOfActivePlayers() <= 1 ){ return; }
	//if( m_pPlayer->GetPlayerHasBeenEliminated() ) return;
	if( m_pGamePlayer->IsAlive() == false ) return;
	if( g_pEOSAIInterface->GetGameHasEnded() ) return;
	//if( GetWorldDescPlayer()->YourGameIsOver() ){ return; }
	//if( GetWorldDescPlayer()->EntireGameIsOver() ){ return; }
	//if( GetPlayer()->GetPlayerHasBeenEliminated() ){ return; }
	//if( GetCommonState()->GetAIPlayerManager()->WaitForAutosaveAndLocalPlayerTurnReplay() ){ return; }
	if( g_pAIPlayerManager->WaitForAutosave() ){ return; }
	if( m_eProcessingState == enumWaitingForServerToPlayerUpdate){ return; }
	//if( m_eProcessingState == enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd ){ return; }

	if( m_bTrigger_NeedToRecalculateEntireTurn )
	{
		m_eProcessingState = enumBeginProcessingTurn;
		m_bTrigger_NeedToRecalculateEntireTurn = false;
	}

	// If there are no Poi, do no processing (this only happens at the beginning of the game before
	//   the ServerToPlayerUpdate happens
	//if( g_pEOSAIInterface->GetAIPoiObjects()->IsEmpty() ){ ASSERT(false); return; } // Did I create the poi objects?
	if( g_pEOSAICommonData->GetAIPoiObjects()->IsEmpty() ){ ASSERT(false); return; } // Did I create the poi objects?
	//if( m_eProcessingState == enumBeginProcessingTurn &&
	//	GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetNumberOfPoiOwnedByPlayer( GetPlayerNumber() ) == 0 )
	if( m_eProcessingState == enumBeginProcessingTurn &&
		g_pEOSAICommonData->GetNumberOfCitiesOwnedByPlayer( GetPlayerNumber() ) == 0 )
	{
		m_eProcessingState = enumReadyToSendOrdersToServer;
	}
/*
	#ifdef _DEBUG
	if( Public::ActiveAIPlayers() == false || m_iAIBrainIntelligence == 0 )
	{
		if( m_eProcessingState == enumBeginProcessingTurn )
		{
			//m_eProcessingState = enumProcessingTurn;
			//if( g_pGameAppState ){ g_pMessageManager->CreateIM_AIPlayerIsInactive( iAIPlayer ); }
			//m_eProcessingState = enumReadyToSendOrdersToServer;
		}
	}
	#endif _DEBUG
*/
/*
	#ifdef _DEBUG
	// This is used to replay each turn when running all AI players
	if( iAIPlayer == 1 &&
		g_pCommonState->GetNumberOfHumanPlayers() == 0 )
	{
		if( s_iAILastReplayTurn < m_pWorldDescServer->GetCurrentTurn() )
		{
			g_pGameAppState->RebuildVisualState( NULL );
			//g_pReportsWindowReplayLastTurnButton->SetReplaySpeed( 5.0f );

			//m_pWorldDescServer->SetReplayLastTurn( true );
			m_pWorldDescServer->SetReplayTime( true, m_pWorldDescServer->GetCurrentTurn() );
			//m_pWorldDescServer->SetReplayIsPaused( false );

			//m_pWorldDescServer->SetReplayTime( >SetReplayLastTurn( true );
			//m_pWorldDescServer->SetReplayLastTurn( true );
		}
		while( m_pWorldDescServer->GetReplayLastTurn() )
		{
			if( g_pAIPlayerManager->ThreadShouldBePaused() ) return;
			if( m_bFlag_ShutdownAIPlayer ){ return; }
			Sleep( 100 );
		}
		s_iAILastReplayTurn = m_pWorldDescServer->GetCurrentTurn();
	}
	#endif _DEBUG
*/
	#ifdef _DEBUG
	if( EOSAISettings::m_bActiveAIPlayers )
	#else
	if( true )
	#endif
	{
		Sleep( 10 );

		if( m_eProcessingState == enumBeginProcessingTurn )
		{
			/*
			if( iAIPlayer == 1 &&
				g_pCommonState->GetNumberOfHumanPlayers() == 0 )
			{
				g_pGameAppState->RebuildVisualState( NULL );

				m_pWorldDescServer->SetReplayLastTurn( true );
				m_pWorldDescServer->SetReplayTime( true, m_pWorldDescServer->GetCurrentTurn()-1 );
				m_pWorldDescServer->SetReplayIsPaused( false );

				//m_pWorldDescServer->SetReplayTime( >SetReplayLastTurn( true );
				//m_pWorldDescServer->SetReplayLastTurn( true );
			}
			*/
			/*
			while( m_pWorldDescServer->GetReplayLastTurn() )
			{
				if( g_pAIPlayerManager->ThreadShouldBePaused() ) return;
				if( m_bFlag_ShutdownAIPlayer ){ return; }
				Sleep( 100 );
			}
			CSingleLock SingleLock( &m_pWorldDescServer->m_AccessWorldData );
			SingleLock.Lock(0);
			if( SingleLock.IsLocked() == FALSE ) return;
			*/

			DeleteAIData();

			#ifdef _DEBUG
			//bool bDebug34098214 = false;
			//if( bDebug34098214 )
			//{
			//	m_DebugDialogMemoryState.Checkpoint();
			//}
			#endif _DEBUG

			CEOSAIStopwatch2  Stopwatch1;
			Stopwatch1.Start();
			g_pEOSAICommonData->RebuildDataIfNecessary();//RebuildAICommonDataIfNecessary();
			Stopwatch1.Stop();

			#ifdef _DEBUG
			//Beep( 500,200 );
			//Beep( 250,200 );
			#endif _DEBUG

			/*
			CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
			if( pWorldDescServer )
			{
				// Rebuild the AICommonData
				pWorldDescServer->GetAICommonData()->RebuildAICommonDataIfNecessary();
			}
			*/

			//ASSERT( g_pEOSAICommonData->GetLastAIUnitCombatCapabilitiesCalculatedTurn() == GetWorldDescServer()->GetCurrentTurn() );
			//GetCommonState()->CalculateUnitCombatCapabilities( GetWorldDescServer()->GetCurrentTurn() );
			g_pAIPlayerManager->CurrentlyProcessingAIPlayer( GetPlayerNumber() );
/*
			while( m_pWorldDescPlayer->GetReplayLastTurn() )
			{
				if( g_pAIPlayerManager->ThreadShouldBePaused() ) return;
				if( m_bFlag_ShutdownAIPlayer ){ return; }
				Sleep( 100 );
			}
*/
			//CSingleLock  SingleLock( &m_pWorldDescPlayer->m_AccessWorldData );
			//SingleLock.Lock(); // wait until I can get a lock

			CEOSAIStopwatch2 m_Stopwatch_TotalProcessingTime;
			m_Stopwatch_TotalProcessingTime.Start();

			//TRACE( _T("CAIPlayer::Process() - 1\n") );
			//Checkpoint::Write( _T("CAIPlayer::Process() - 1") );
			m_pAIBrain->ResetProcessingFlags();

			//m_pAIBrain->CalculateExpansionOrConquestValues();

			// Clear-out old information
			// Pause - because we don't want to turn soon-to-be-deleted PoiObjects / PoiPD into objects
			//   If the game is redrawing everything, then objects won't be deleted.
			//   This conflict can only happen if we are watching the AI player.
			Sleep( 10 );
			//TRACE( _T("CAIPlayer::Clear old data - Start\n") );
			//Checkpoint::Write( _T("CAIPlayer::Clear old data - Start") );
			//GetAIBrain()->ClearOldData();
			this->DeleteAIDebugData();
			//this->ClearUnitOrders();
			this->SetMyUnitsFieldOrders();
			this->ClearCityOrders();
			//TRACE( _T("CAIPlayer::Clear old data - End\n") );
			//Checkpoint::Write( _T("CAIPlayer::Clear old data - End") );
			Sleep( 10 );
			//Beep( 1000,200 );

			if( this->GetFlag_ShutdownAIPlayer() ){ return; }

			#ifdef _DEBUG
			//Beep( 500, 100 );
			#endif
			//m_eProcessingState = enumProcessingTurn;
/*
#ifdef _DEBUG
			CUnitTemplate* pInfantryUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Infantry (Class 1)") );
			CUnitTemplate* pTankUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Tank (Class 1)") );
			//CUnitTemplate* pTactical1UnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Tactical Bomber (Class 1)") );
			CUnitTemplate* pTacticalAircraftUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Tactical Bomber (Class 1)") );
			CUnitTemplate* pDestroyer1UnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Destroyer (Class 1)") );
			CUnitTemplate* pCruiser1UnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Cruiser (Class 1)") );
			CUnitTemplate* pBattleship1UnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Battleship (Class 1)") );
			CUnitTemplate* pCarrier1UnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Carrier (Class 1)") );
			CUnitTemplate* pSubmarine1UnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( _T("Submarine (Class 1)") );

			CEOSAIStopwatch2 Watch2;
			Watch2.Start();
			for( long i=0; i<30; i++ )
			{
				//CEOSAIQuickCombatCalculation  CombatCalc;
				CombatCalc.SetProDefenderBias( 1.5f );//2.0f );
				// Attackers
				//CombatCalc.AddToAttackers( pDestroyer1UnitTemplate );
				//CombatCalc.AddToAttackers( pDestroyer1UnitTemplate );
				//CombatCalc.AddToAttackers( pDestroyer1UnitTemplate );
				//CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				//CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				//CombatCalc.AddToAttackers( pTacticalAircraftUnitTemplate );
				//CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				//CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				//CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				//CombatCalc.AddToAttackers( pInfantryUnitTemplate );
				// Defenders
				//CombatCalc.AddToDefenders( pSubmarine1UnitTemplate );
				//CombatCalc.AddToDefenders( pCruiser1UnitTemplate );
				CombatCalc.AddToDefenders( pInfantryUnitTemplate );
				CombatCalc.AddToDefenders( pInfantryUnitTemplate );
				//CombatCalc.AddToDefenders( pInfantryUnitTemplate );
				//CombatCalc.AddToDefenders( pInfantryUnitTemplate );

				CombatCalc.RunCalculation();
			//	float f1 = CombatCalc.GetAttackerGroundUnitSurvival01();
			//	float f2 = CombatCalc.GetAttackerSeaUnitSurvival01();
			//	float f3 = CombatCalc.GetDefenderGroundUnitSurvival01();
			//	float f4 = CombatCalc.GetDefenderSeaUnitSurvival01();
				float f5 = CombatCalc.GetGroundCombatWinBalance01();
				float f6 = CombatCalc.GetSeaCombatWinBalance01();

				float f4x= CombatCalc.GetAttackerGroundUnitDamageAsProduction();
				float f4b= CombatCalc.GetAttackerSeaUnitDamageAsProduction();
				float f4c= CombatCalc.GetAttackerAirUnitDamageAsProduction();
				float f4d= CombatCalc.GetAttackerDamageAsProduction();

				float f5x= CombatCalc.GetAttackerGroundUnitSurvivalAsCombatSignificance();
				float f5b= CombatCalc.GetAttackerSeaUnitSurvivalAsCombatSignificance();
				float f5c= CombatCalc.GetAttackerAirUnitSurvivalAsCombatSignificance();

				float f6a= CombatCalc.GetDefenderDamageAsProduction();
				float f6b= CombatCalc.GetDefenderDamageAsCombatSignificance();
				float f6c= CombatCalc.GetDefenderGroundUnitSurvivalAsCombatSignificance();
				float f6d= CombatCalc.GetDefenderSeaUnitSurvivalAsCombatSignificance();
				float f6e= CombatCalc.GetDefenderAirUnitSurvivalAsCombatSignificance();

				float fDamageTakenAsProduction1 = CombatCalc.GetAttackerDamageAsProduction();
				float fEnemyDamageAsProduction1 = CombatCalc.GetDefenderDamageAsProduction();
				float fEnemyDamageAsCombatSignificance1 = CombatCalc.GetDefenderDamageAsCombatSignificance();
				CEOSAIQuickCombatResults CombatCalcState1;
				CombatCalc.SaveState( &CombatCalcState1 );

				CombatCalc.AddToAttackers( pDestroyer1UnitTemplate );
				CombatCalc.RunCalculation();
				CEOSAIQuickCombatResults CombatCalcState2;
				CombatCalc.SaveState( &CombatCalcState2 );

				float fDeltaX1 = CombatCalcState2.GetDelta_AttackerDamageAsProduction( &CombatCalcState1 );
				float fDeltaY1 = CombatCalcState2.GetDelta_DefenderDamageAsProduction( &CombatCalcState1 );
				float fDeltaZ1 = CombatCalcState2.GetDelta_DefenderDamageAsCombatSignificance( &CombatCalcState1 );

				float fDamageTakenAsProduction2 = CombatCalc.GetAttackerDamageAsProduction();
				float fEnemyDamageAsProduction2 = CombatCalc.GetDefenderDamageAsProduction();
				float fEnemyDamageAsCombatSignificance2 = CombatCalc.GetDefenderDamageAsCombatSignificance();

				float fDeltaX = fDamageTakenAsProduction2 - fDamageTakenAsProduction1;
				float fDeltaY = fEnemyDamageAsProduction2 - fEnemyDamageAsProduction1;
				float fDeltaZ = fEnemyDamageAsCombatSignificance2 - fEnemyDamageAsCombatSignificance1;

				//float f8 = CombatCalc.GetAIResults(  );
				//float fScore = CombatCalc.GetDefenderDamageAsCombatSignificance();
				//bool  bAllEnemiesDefeated = CombatCalc.

				float f7 = CombatCalc.GetAttackerSuccessValue02();
				int h=0;
			}
			Watch2.Stop();
			float f = Watch2.GetDeltaMilliseconds();
			CString str; str.Format( _T("Milliseconds: %f"), f );
			//AfxMessageBox( str );
#endif _DEBUG
*/

			// Wait until the turn-replay is done (usually about 5 seconds)
			//Sleep( 1000 );
			//while( g_pGameAppState->GetWorldDescPlayer()->GetReplayLastTurn() )
/*
			while( g_pGameAppState->GetWorldDesc()->GetReplayLastTurn() )
			{
				if( g_pAIPlayerManager->ThreadShouldBePaused() ) return;
				if( m_bFlag_ShutdownAIPlayer ){ return; }
				Sleep( 1000 );
			}
*/
			//CEOSAIStopwatch2 AITimer;
			//AITimer.Start();
			//Beep( 1000,200 );

			CEOSAIStopwatch2 m_Stopwatch_PreSection;
			m_Stopwatch_PreSection.Start();

			ProcessUnprocessedEvents();
			//Checkpoint::Write( _T("CAIPlayer::Process() - 2") );
			Sleep( 10 );

			//
			//m_StrategicAI.SetAIPlayer( this );
			//m_StrategicAI.InvokePlayers( GetCommonState()->GetNumberOfPlayers() );

			//TRACE( _T("CAIPlayer::Process - 2a\n") );

			// Initial Turn Information
			m_pAIBrain->CreateInitialTurnData_AIPoiObjects_BuildOptions_NationalSummaries();
			//TRACE( _T("CAIPlayer::Process - 2b\n") );
			Sleep( 10 );
			//Beep( 1000,200 );

			#ifdef _DEBUG
			/*
			CEOSAIPoiObject* pFirstAIPoiObject = NULL;
			POSITION pos = m_pAIBrain->GetAIThoughtDatabase()->GetMyActors()->GetHeadPosition();
			while( pos )
			{
				pFirstAIPoiObject = m_pAIBrain->GetAIThoughtDatabase()->GetMyActors()->GetNext( pos );
				if( dynamic_cast< CEOSAIUnit2* >( pFirstAIPoiObject ) )
				{
					break;
				}
			}
			g_AISuperPathMap.SetAIBrain( m_pAIBrain );
			g_AISuperPathMap.SetAIPoiObject( pFirstAIPoiObject );
			g_AISuperPathMap.InvokeDirectMapToEverywhere();
			*/
			#endif _DEBUG

			//
			// We're disabling this for now, but we need a system for detecting if 
			//   another player's units are moving into the AI's territory.
			#ifdef THINGS_TO_COMPILE_EVENTUALLY
			FindUnitsWithinMyNationalBoundaries();
			SendMessagesAndAdjustForeignRelationsBasedOnBorderViolations();
			#endif THINGS_TO_COMPILE_EVENTUALLY

			// Can be changed by trade, agreements
			CalculateCitResValues();
			CalculateExperiencingResourceShortage01();
			m_StrategicAI.CreateForeignRelationsStances();
			m_StrategicAI.MakeWarDeclarationsAndPeace();
			//Checkpoint::Write( _T("CAIPlayer::Process() - 3") );
			if( this->GetFlag_ShutdownAIPlayer() ){ return; }

			// the AI seems to get stuck here
			m_StrategicAI.CalculateTechnologyStrategy();
			Sleep( 10 );

			// debug
			CString strStrategicAI  = m_StrategicAI.OutputDebugString();
			CString strCurrentState = m_StrategicAI.GetAIForeignRelationsState()->OutputDebugString();

			//Checkpoint::Write( "CAIPlayer::Process() - 4" );

			// Check for memory leaks
			#ifdef _DEBUG
		//	CMemoryState MemoryState1, MemoryState2, DiffState;
			#endif
			//#ifdef _DEBUG
			//MemoryState1.Checkpoint(); // Position #1
			//#endif

			// Do some trade and diplomacy?  (Depending on the response, I might want to redo my orders)
			m_pAIBrain->SendTradeOffersAndNegotiations();
			//Checkpoint::Write( "CAIPlayer::Process() - 4b" );
			Sleep( 10 );

		//	m_pAIBrain->CreateTransportCombinedAIRegionMaps();
			m_Stopwatch_PreSection.Stop();
			Sleep( 10 );

			CEOSAIStopwatch2 m_Stopwatch_AIGeos;
			m_Stopwatch_AIGeos.Start();
			//m_pAIBrain->GenerateAIGeos();
			m_pAIBrain->CalculateAIGeoViewValues();
			m_pAIBrain->CreateAIGeoPlans();
			m_Stopwatch_AIGeos.Stop();

			m_pAIBrain->CreateMultiRegionPlans();

			/*
			m_Stopwatch_CreateDesiresAndTacticalProjects.Start();
			m_pAIBrain->CreateDesiresAndTacticalProjects();
			m_Stopwatch_CreateDesiresAndTacticalProjects.Stop();
			Sleep( 10 );
			*/

			//this->DeleteData();           // DEBUG / DEBUG / DEBUG / DEBUG / DEBUG / DEBUG
			//GetAIBrain()->ClearOldData(); // DEBUG / DEBUG / DEBUG / DEBUG / DEBUG / DEBUG

//MemoryState1.DumpAllObjectsSince();
//MemoryState1.Checkpoint(); // Position #3

			//Checkpoint::Write( _T("CAIPlayer::Process() - 5") );
			if( this->GetFlag_ShutdownAIPlayer() ){ return; }
			CEOSAIStopwatch2 m_Stopwatch_AIBrainProcess;
			m_Stopwatch_AIBrainProcess.Start();
			m_pAIBrain->Process();
			m_Stopwatch_AIBrainProcess.Stop();
			if( this->GetFlag_ShutdownAIPlayer() ){ return; }
			//Checkpoint::Write( _T("CAIPlayer::Process() - 6") );
			Sleep( 1 );

			// Tell the server I am ready to send my orders, but I need to wait and listen
			//   for player-responses and player-offers.  I don't want to end my turn before they
			//   send me an offer which could affect this turn's orders (like a peace offer).

			//ASSERT( m_eProcessingState == enumProcessingTurn );
			//if( m_eProcessingState == enumProcessingTurn )
			{
				m_eProcessingState = enumReadyToSendOrdersToServer;

				#ifdef THINGS_TO_COMPILE_EVENTUALLY
				#endif THINGS_TO_COMPILE_EVENTUALLY
				//ASSERT( false );
				/*
				CMessage2_AIPlayerReadyToSubmitTurn  Message2;
				Message2.m_iAIPlayer = this->GetPlayerNumber();
				Message2.m_iTurnSubmitted = m_pWorldDescServer->GetCurrentTurn();
				Message2.SetSenderId( this->GetPlayer()->GetMessageTargetId() );
				//Message2.SendToAllHumans();
				Message2.SendToAllHumansButNotPBEMPlayers();
				g_pMessageManager->Send( Message2 );
				*/
			}
			if( this->GetFlag_ShutdownAIPlayer() ){ return; }

			//this->DeleteData();           // DEBUG / DEBUG / DEBUG / DEBUG / DEBUG / DEBUG
			//GetAIBrain()->ClearOldData(); // DEBUG / DEBUG / DEBUG / DEBUG / DEBUG / DEBUG
			#ifdef _DEBUG
			//strText.Format( "About to delete data." );
			//g_pMessageManager->CreateInstantMessageLineForEachLocalPlayer( GetPlayer()->GetPlayerNumber(), enum_IconClock, strText );
			//g_pMessageManager->CreateInstantMessageLineForEachLocalPlayer( GetPlayer()->GetPlayerNumber(), enum_IconClock, strText );
			//g_pMessageManager->CreateInstantMessageLineForEachLocalPlayer( GetPlayer()->GetPlayerNumber(), enum_IconClock, strText );
			#endif

			//Sleep( 40 );
			/*
			#ifndef _DEBUG
			if( Public::m_bAIPlayerSubmitTurn )
			{
				// Delete it if we're running the release version
				// If I'm running the debug version, I might want to examine it
				DeleteAIData();
			}
			#endif
			*/
			#ifndef DEBUG
			// Delete the data if we are in Release mode, otherwise, I might want to look at it.
			DeleteAIData();
			#endif

			#ifdef _DEBUG
			//strText.Format( "About to delete data - Done." );
			//g_pMessageManager->CreateInstantMessageLineForEachLocalPlayer( GetPlayer()->GetPlayerNumber(), enum_IconClock, strText );
			#endif

			// Check for memory leaks
			#ifdef _DEBUG
		//	MemoryState2.Checkpoint();
			#endif
			#ifdef _DEBUG
		//	DiffState.Difference( MemoryState1, MemoryState2 );

			// 2735413 bytes in 18897 Normal Blocks ( + m_AIBrain.Process() - m_AIBrain.DeletePlan() )
			// 2164312 bytes in 13631 Normal Blocks ( + m_AIBrain.Process() + m_AIBrain.DeletePlan() )
			// 2102371 bytes in 11986 Normal Blocks ( + m_AIBrain.Process() - InstanciateOrders() + m_AIBrain.DeletePlan() )
			// 2161810 bytes in 13475 Normal Blocks ( + m_AIBrain.Process() + m_AIBrain.DeletePlan() )
			// 2161369 bytes in 13467 Normal Blocks ( + m_AIBrain.Process() + m_AIBrain.DeletePlan() )
			//    2423 bytes in    47 Normal Blocks ( - m_AIBrain.Process() + m_AIBrain.DeletePlan() )

			// Latest:
			// 3839552 bytes in 23361 Normal Blocks.
			//    5621 bytes in    -3 Normal Blocks.
			//    1331 bytes in   -19 Normal Blocks.

			// Latest:
			// 3712690 bytes in 49196 Normal Blocks.
		//	DiffState.DumpStatistics();
		//	MemoryState1.DumpAllObjectsSince();
			//MemoryState1.DumpAllObjectsSince();
			#endif
			int z=0;
			//DeleteAIDebugData();
			//Checkpoint::Write( _T("CAIPlayer::Process() - 7") );

			#ifdef _DEBUG
			//strText.Format( "AIPlayer %d ready to send turn.", GetPlayer()->GetPlayerNumber() );
			//g_pMessageManager->CreateInstantMessageLineForEachLocalPlayer( GetPlayer()->GetPlayerNumber(), enum_IconClock, strText );
			#endif

			// We'll assume that ProcessTurn takes enough time that
			//   we should check for the paused flag rather than continue.
			#ifdef _DEBUG
			//Beep( 200, 100 );
			#endif

			m_Stopwatch_TotalProcessingTime.Stop();
			float fTotalDeltaTime = (float) m_Stopwatch_TotalProcessingTime.GetDeltaSeconds();
/*
			Checkpoint::Write( _T("-----") );
			strText.Format( _T("CAIPlayer::Process() - AICommonData:RebuildData %0.1f Seconds"), Stopwatch1.GetDeltaSeconds() ); Checkpoint::Write( strText );
			Checkpoint::Write( _T("-----") );
			strText.Format( _T("CAIPlayer::Process() - AITimer, player: %d, turn %d"), GetPlayer()->GetPlayerNumber(), m_pWorldDescServer->GetCurrentTurn() ); Checkpoint::Write( strText );
			strText.Format( _T("  Total Time: %f"), m_Stopwatch_TotalProcessingTime.GetDeltaSeconds() ); Checkpoint::Write( strText );
			strText.Format( _T("    m_Stopwatch_PreSection Time: %f (%0.2f)"), m_Stopwatch_PreSection.GetDeltaSeconds(), m_Stopwatch_PreSection.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("    m_Stopwatch_AIGeos Time: %f (%0.2f)"), m_Stopwatch_AIGeos.GetDeltaSeconds(), m_Stopwatch_AIGeos.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			//strText.Format( _T("    Brain::CreateDesiresAndTacticalProjects Time: %f (%0.2f)"), m_pAIBrain->m_Stopwatch_CreateDesiresAndTacticalProjects.GetDeltaSeconds(), m_pAIBrain->m_Stopwatch_CreateDesiresAndTacticalProjects.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("    Brain::Process() Time: %f (%0.2f)"), m_Stopwatch_AIBrainProcess.GetDeltaSeconds(), m_Stopwatch_AIBrainProcess.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("      CreateDesires Time: %f (%0.2f)"), m_pAIBrain->m_stopwatchCreateDesires.GetDeltaSeconds(), m_pAIBrain->m_stopwatchCreateDesires.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("      CreateTacticalProjects Time: %f (%0.2f)"), m_pAIBrain->m_stopwatchCreateTacticalProjects.GetDeltaSeconds(), m_pAIBrain->m_stopwatchCreateTacticalProjects.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("      AllocateTacticalProjects Time: %f (%0.2f)"), m_pAIBrain->m_AllocateTacticalProjectsTime.GetDeltaSeconds(), m_pAIBrain->m_AllocateTacticalProjectsTime.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("        CreateStrategiesAndUnitActionIdeas Time: %f (%0.2f)"), m_pAIBrain->m_CreateStrategiesAndUnitActionIdeasTime.GetDeltaSeconds(), m_pAIBrain->m_CreateStrategiesAndUnitActionIdeasTime.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("          MyCities_InvokePathwaysToEverywhere Time: %f (%0.2f)"), m_pAIBrain->m_MyCities_InvokePathwaysToEverywhere.GetDeltaSeconds(), m_pAIBrain->m_MyCities_InvokePathwaysToEverywhere.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("          MyUnits_CreateUnitActionIdeas Time: %f (%0.2f)"), m_pAIBrain->m_MyUnits_CreateUnitActionIdeas.GetDeltaSeconds(), m_pAIBrain->m_MyUnits_CreateUnitActionIdeas.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("            m_MyUnits_CalculateTimeToTarget Time: %f (%0.2f)"), m_pAIBrain->m_MyUnits_CalculateTimeToTarget.GetDeltaSeconds(), m_pAIBrain->m_MyUnits_CalculateTimeToTarget.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("              m_MyUnits_CalculateTimeToTarget_InvokePathwaysToEverywhere Time: %f (%0.2f)"), m_pAIBrain->m_MyUnits_CalculateTimeToTarget_InvokePathwaysToEverywhere.GetDeltaSeconds(), m_pAIBrain->m_MyUnits_CalculateTimeToTarget_InvokePathwaysToEverywhere.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("              m_MyUnits_CalculateTimeToTarget_TargetDesire_InvokePathToEverywhere Time: %f (%0.2f)"), m_pAIBrain->m_MyUnits_CalculateTimeToTarget_TargetDesire_InvokePathToEverywhere.GetDeltaSeconds(), m_pAIBrain->m_MyUnits_CalculateTimeToTarget_TargetDesire_InvokePathToEverywhere.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("              m_MyUnits_CalculateTimeToTarget_InvokePath2 Time: %f (%0.2f)"), m_pAIBrain->m_MyUnits_CalculateTimeToTarget_InvokePath2.GetDeltaSeconds(), m_pAIBrain->m_MyUnits_CalculateTimeToTarget_InvokePath2.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("              m_MyUnits_CalculateTimeToTarget_TransportComboAssistedMovementTime Time: %f (%0.2f)"), m_pAIBrain->m_MyUnits_CalculateTimeToTarget_TransportComboAssistedMovementTime.GetDeltaSeconds(), m_pAIBrain->m_MyUnits_CalculateTimeToTarget_TransportComboAssistedMovementTime.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("                m_GetTransportAssistedMovementPathTime: %f (%0.2f)"), m_pAIBrain->m_GetTransportAssistedMovementPath.GetDeltaSeconds(), m_pAIBrain->m_GetTransportAssistedMovementPath.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("                m_GetTransportAssistedMovementPath_Count: %d"), m_pAIBrain->m_GetTransportAssistedMovementPath_Count ); Checkpoint::Write( strText );
			strText.Format( _T("            m_MyUnits_InvokeTransportMovementDescs Time: %f (%0.2f)"), m_pAIBrain->m_MyUnits_InvokeTransportMovementDescs.GetDeltaSeconds(), m_pAIBrain->m_MyUnits_InvokeTransportMovementDescs.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("              TransportCalculationsTime Time: %f (%0.2f)"), m_pAIBrain->m_TransportCalculationsTime.GetDeltaSeconds(), m_pAIBrain->m_TransportCalculationsTime.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("        CreateBestCaseActionIdeas Time: %f (%0.2f)"), m_pAIBrain->m_CreateBestCaseActionIdeasTime.GetDeltaSeconds(), m_pAIBrain->m_CreateBestCaseActionIdeasTime.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			strText.Format( _T("        CreateTaskForcesAndPredefinedSteps Time: %f (%0.2f)"), m_pAIBrain->m_CreateTaskForcesAndPredefinedStepsTime.GetDeltaSeconds(), m_pAIBrain->m_CreateTaskForcesAndPredefinedStepsTime.GetDeltaSeconds()/fTotalDeltaTime ); Checkpoint::Write( strText );
			Checkpoint::Write( _T("-----") );
*/
			g_pAIPlayerManager->CurrentlyProcessingAIPlayer( 0 );
		}
	}
	//if( m_bReadyToSendOrdersToServer )
	if( m_eProcessingState == enumReadyToSendOrdersToServer )
	{
		if( this->GetFlag_ShutdownAIPlayer() ){ return; }
		if( g_pAIPlayerManager->ThreadIsPaused() ||
			g_pAIPlayerManager->ThreadShouldBePaused() )
		{
			return;
		}
		Sleep( 1 );
/*
		CSingleLock SingleLock( &m_pWorldDescServer->m_AccessWorldData );
		SingleLock.Lock(0);
		if( SingleLock.IsLocked() == FALSE ) return;
*/
		if( ProcessUnprocessedEvents() )
		{
			//CString strText;
			//strText.Format( _T("CAIPlayer::Process() - Significant UnprocessedEvents, Player: %d"), GetPlayer()->GetPlayerNumber() );
			//Checkpoint::Write( strText );
			m_eProcessingState = enumBeginProcessingTurn;

			ASSERT( false );
			/*
			CMessage2_AIPlayerNotReadyToSubmitTurn  Message2;
			Message2.m_iAIPlayer = this->GetPlayerNumber();
			Message2.m_iTurnUnSubmitted = m_pWorldDescServer->GetCurrentTurn();
			Message2.SetSenderId( this->GetPlayer()->GetMessageTargetId() );
			//Message2.SendToAllHumans();
			POSITION pos = g_pMessageTargetManager->GetMessageTargetList()->GetHeadPosition();
			while( pos )
			{
				CMessageTarget_NetworkPeer* pMessageTarget = g_pMessageTargetManager->GetMessageTargetList()->GetNext( pos );
				if( pMessageTarget->GetType() == CMessageTarget_NetworkPeer::Human &&
					pMessageTarget->GetConnectionType() != CMessageTarget_NetworkPeer::PBEM )
				{
					Message2.AddSendTo( pMessageTarget->GetId() );
				}
			}
			g_pMessageManager->Send( Message2 );
			*/

			return;
		}
		//Checkpoint::Write( "CAIPlayer::Process() - 8" );
		/*
		if( m_eProcessingState == enumBeginProcessingTurn )
		{
			continue; // go back to the beginning of the loop
		}
		*/

		long iNumberOfHumanPlayers = g_pEOSAIInterface->GetNumberOfHumanPlayers();
		long iServerIsWaitingForTurn = g_pEOSAIInterface->GetCurrentTurn();
		//bool bAllHumanPlayersHaveSubmittedTurn = g_pEOSAICommonData->AllActiveHumanPlayersHaveSubmittedTurn( iServerIsWaitingForTurn );
		bool bAllHumanPlayersHaveSubmittedTurn = g_pEOSAIInterface->AllActiveHumanPlayersHaveSubmittedTurn( iServerIsWaitingForTurn );
		bool bAllAIPlayersAreReadyToSendTurnOrHaveSentTurn = g_pAIPlayerManager->AllAIPlayersAreReadyToSendTurnOrHaveSentTurn();

		//DWORD dwTime = timeGetTime();
		/*
		m_iDebugTempCounter++;
		if( m_iDebugTempCounter%100 == 99 )
		{
			CString strText;
			strText.Format( "CAIPlayer::Process() - Player:%d : Turn:%d/%d, %s", 
				GetPlayer()->GetPlayerNumber(), GetWorldDescPlayer()->GetLastTurnSubmittedToServer(), iServerIsWaitingForTurn, bAllHumanPlayersHaveSubmittedTurn?"true":"false" );
			Checkpoint::Write( strText );
			m_iDebugTempCounter = 0;
		}
		*/

		/*
		if( iNumberOfHumanPlayers > 0 && m_bAllHumanPlayersHaveSubmittedOrders )
		{
			bSubmitTurn = true;
		}
		if( iNumberOfHumanPlayers == 0 &&
			GetWorldDescPlayer()->GetWorldBuildDesc()->GetAutoAdvanceTurn() )
		{
			bSubmitTurn = true;
		}
		if( bSubmitTurn )
		{
			SendOrders();
		}
		*/
		/*
		Sleep( 10 );
		if( GetWorldDescPlayer()->GetWorldBuildDesc()->GetNumberOfHumanPlayers() == 0 &&
			GetWorldDescPlayer()->GetWorldBuildDesc()->GetAutoAdvanceTurn() )
			m_bLocalHumanPlayerHasSubmittedTurn )
		{
			SendOrders();
		}
		*/

		/*
		#ifdef _DEBUG
		bool bDebug23213 = false;
		if( bDebug23213 )
		{
			CEOSAIRegionPathwayItem* pDebugResultObject = NULL;

			DeleteAIDebugData();
			DeleteAIData();

			CMemoryState MemoryState2, DiffMemState;
			MemoryState2.Checkpoint();
			DiffMemState.Difference( m_DebugDialogMemoryState, MemoryState2 );
			DiffMemState.DumpStatistics();
			m_DebugDialogMemoryState.DumpAllObjectsSince();

			int h34234=0;
		}
		#endif _DEBUG
		*/

		// If there are no human players (AI-only game), either auto-advance or 
		//   wait for the player to press the "turn" button
		if( iNumberOfHumanPlayers == 0 )
		{
			if( g_pAIPlayerManager->GetDebugPause() ){ return; }
			//if( m_pWorldDescServer->GetWorldBuildDesc()->GetAutoAdvanceTurn() ||
			//	g_pGameAppState->GetAllowTurnAdvancement() == m_pWorldDescServer->GetCurrentTurn() )
			if( g_pEOSAICommonData->AllowAutomaticTurnAdvancement() )
			{
				#ifdef _DEBUG
				if( EOSAISettings::m_bAIPlayerSubmitTurn )
				#else
				if( true )
				#endif
				{
					DeleteAIDebugData();
					DeleteAIData();

					if( g_pEOSAIInterface->GetNumberOfActivePlayers() <= 1 ){ return; }
					//if( GetWorldDescPlayer()->YourGameIsOver() ){ return; }
					//if( GetWorldDescPlayer()->EntireGameIsOver() ){ return; }

					SendOrders();
				}
			}
		}
		else // this is a human-player game, wait for all humans to submit turns
		{
			//if( m_bAllHumanPlayersHaveSubmittedOrders )
			if( bAllHumanPlayersHaveSubmittedTurn &&
				bAllAIPlayersAreReadyToSendTurnOrHaveSentTurn )
			{
				//CString strText;
				//strText.Format( _T("AIPlayer %d submitting turn %d"), GetPlayer()->GetPlayerNumber(), iServerIsWaitingForTurn );
				//Checkpoint::Write( strText );

				DeleteAIDebugData();
				DeleteAIData();

				if( g_pEOSAIInterface->GetNumberOfActivePlayers() <= 1 ){ return; }
				//if( GetWorldDescPlayer()->YourGameIsOver() ){ return; }
				//if( GetWorldDescPlayer()->EntireGameIsOver() ){ return; }

				SendOrders();

				//strText.Format( _T("AIPlayer %d submitted turn %d"), GetPlayer()->GetPlayerNumber(), iServerIsWaitingForTurn );
				//Checkpoint::Write( strText );
				//m_bAllHumanPlayersHaveSubmittedOrders = false;
			}
		}
	}
	//Checkpoint::Write( "CAIPlayer::Process() - End" );
}
/*
void  CAIPlayer::CalculateMyNationwidePathways()
{
	// Delete the existing nationwide pathways
	while( m_NationwidePathways.IsEmpty() == FALSE )
	{
		delete m_NationwidePathways.RemoveHead();
	}
	CEOSAIMultiRegionNationwidePathways* pMyNationwidePathway = new CEOSAIMultiRegionNationwidePathways();
	pMyNationwidePathway->SetPlayer( GetPlayer()->GetPlayerNumber() );
	pMyNationwidePathway->SetWorldDesc( GetWorldDescPlayer() );
	pMyNationwidePathway->SetMultiRegionManager( GetWorldDescServer()->GetMultiRegionManager() );
	pMyNationwidePathway->Calculate();
	m_NationwidePathways.AddTail( pMyNationwidePathway );
}
*/
void AIPlayer::SendOrders()
{
	//m_bAllHumanPlayersHaveSubmittedTurn = false;
	if( g_pAIPlayerManager->GetDebugPause() ){ return; }

	//m_bReadyToSendOrdersToServer = false;
	//m_bAllHumanPlayersHaveSubmittedOrders = false;
	m_eProcessingState = enumReadyToSendOrdersToServer;

	long iCurrentTurn = g_pEOSAIInterface->GetCurrentTurn();
	//m_pWorldDescPlayerProxy->SetLastPlayerToServerTurnReceivedByServer( iCurrentTurn );
	g_pEOSAIInterface->SetAIPlayerHasProcessedTurn( m_iPlayerNumber, iCurrentTurn );

	// Before we send orders, record the money/oil/food/iron amounts and deltas
	//   (I want to track how much of each resource I'm using after giving orders.)
	//m_MoneyAmount.AddDataPoint( GetWorldDescPlayer()->GetTotalMoney(), GetWorldDescPlayer()->GetDeltaMoney() );
	//m_IronAmount.AddDataPoint(  GetWorldDescPlayer()->GetTotalIron(),  GetWorldDescPlayer()->GetDeltaIron()  );
	//m_OilAmount.AddDataPoint(   GetWorldDescPlayer()->GetTotalOil(),   GetWorldDescPlayer()->GetDeltaOil()   );
	//m_FoodAmount.AddDataPoint(  GetWorldDescPlayer()->GetTotalFood(),  GetWorldDescPlayer()->GetDeltaFood()  );

	//GetWorldDescPlayer()->SendWorldUpdateToServer();

	//long iTurn = GetWorldDescPlayer()->GetCurrentTurn();
	//m_iLastTurnSentToServer = iTurn;
}
/*
void AIPlayer::TriggerProcessTurn()
{
	if (m_eProcessingState == enumWaitingForTurnAndRulesUpdate)
	{
		m_eProcessingState = enumBeginProcessingTurn;
	}
}
*/

void  AIPlayer::Incoming_ServerToPlayerUpdateWasProcessed()
{
//	CString str;
//	str.Format( _T("CAIPlayer::Incoming_ServerToPlayerUpdateWasProcessed() - Player %d"), GetPlayer()->GetPlayerNumber() );
//	Checkpoint::Write( str );

	// This might happen when a ServerToPlayerUpdate::TurnUpdate happens,
	//   or this might happen when a TradeAgreement goes through, or some other reason
	if( m_eProcessingState == enumWaitingForServerToPlayerUpdate )
	{
		//CString str;
		//str.Format( _T("CAIPlayer::Incoming_ServerToPlayerUpdateWasProcessed() - Player %d - enumWaitingForServerToPlayerUpdate -> enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd"), GetPlayer()->GetPlayerNumber() );
		//Checkpoint::Write( str );

		//m_eProcessingState = enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd;
		m_eProcessingState = enumBeginProcessingTurn;
	}
}

/*
void  CAIPlayer::Incoming_AutosaveAndLocalPlayerTurnReplayEnded()
{
	// This might happen when a ServerToPlayerUpdate::TurnUpdate happens,
	//   or ths might happen when a TradeAgreement goes through, or some other reason
	if( m_eProcessingState == enumWaitingForServerToPlayerUpdate )
	{
		ASSERT( false ); // This might sometimes happen, I want to know if it does
	}
	if( m_eProcessingState == enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd )
	{
		m_eProcessingState = enumBeginProcessingTurn;
	}
}
*/
/*
void  CAIPlayer::Notify_NewTurn()
{
	//m_bUpdateCommonAIData = true;

	// Don't resend the information if we've already sent it to the server
	//   (This situation could happen if the AI submits a turn, the player saves, reloads game.)
	// Correction: I now allow the AI to resubmit their turn in this case.
	//if( m_pWorldDescPlayer->GetCurrentTurn() > m_pWorldDescPlayer->GetLastTurnSubmittedToServer() )
	//{
		//m_bNeedToUpdateTurnInformation = true;
	//}
	ASSERT( m_eProcessingState == enumWaitingForServerToPlayerUpdate );
	if( m_eProcessingState == enumWaitingForServerToPlayerUpdate )
	{
		m_eProcessingState = enumBeginProcessingTurn;
	}
}
*/

// Foreign Relations
//
void  AIPlayer::SignedAPeaceAgreement( long iPlayer1, long iPlayer2 )
{
	//m_bUpdateForeignRelationsCalculations = true;

	m_pAIBrain->SetFlag_ProcessTurnBasedOnForeignRelations( true );
	//m_pAIBrain->Process();
}

//
//


void AIPlayer::FindUnitsWithinMyNationalBoundaries()
{
	//CEOSAISimpleMap< COwnershipPlayer >  OwnershipMap8;
	long iAIPlayer = GetPlayerNumber();
	long iCurrentTurn = g_pEOSAIInterface->GetCurrentTurn();
	//CWorldDescServer* pWorldDescServer = GetWorldDescServer();
	//POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		//if( pAIPoiObject->IsNeutralPlayer() == false ) continue;
		if( pAIPoiObject->GetForeignRelationsTo( GetPlayerNumber() ) != EOSAIEnumForeignRelations::enum_Neutral ) continue;

		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
		if( pAIUnit )
		{
			// Ignore aircraft ( for now )
			if( pAIUnit->IsAirUnit() ) continue;
			if( pAIUnit->IsHypotheticalPoiObject() ) continue;
			//if( pAIUnit->GetServerPoiObject() == NULL ) continue;
			if( pAIUnit->IsInsideAUnitContainer()  ) continue;

			float fCurrentPixelX = pAIUnit->GetInitialState()->GetLocation().GetRealX();
			float fCurrentPixelY = pAIUnit->GetInitialState()->GetLocation().GetRealY();
			char iLastTurnMapCurrentUnitLocationOwner = g_pEOSAICommonData->GetLastTurnOwnershipPlayerMap8()->m_Player.PixelValue( (int)fCurrentPixelX, (int)fCurrentPixelY );
			//char iCurrentLocationOwner = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Player.PixelValue( fCurrentPixelX, fCurrentPixelY );
			//char iLocationOwnerLevel = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Ownership.PixelValue( fCurrentPixelX, fCurrentPixelY );
			//char iLastLocationOwner = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Player.PixelValue( fCurrentPixelX, fCurrentPixelY );
			//char iLastLocationOwnerLevel = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Ownership.PixelValue( fCurrentPixelX, fCurrentPixelY );

			//pAIUnit->SetLocationOwner( iLocationOwner, iLocationOwnerLevel );
			//pAIUnit->SetLastLocationOwner( iLastLocationOwner, iLastLocationOwnerLevel );

			//if( pAIPoiObject->IsNeutralPlayer() && iLastTurnMapCurrentUnitLocationOwner == iAIPlayer )
			if( iLastTurnMapCurrentUnitLocationOwner == iAIPlayer )
			{
				// Check if this unit is normally visible (submarines can't be seen)
				//float fNormalViewRange = CCity::GetTypicalViewRange( pAIUnit->GetServerPoiObject() );
				/*
				float fNormalViewRange = CCity::GetTypicalViewRange( pAIUnit );
				if( fNormalViewRange < 10.0f )
				{
					bool bWasSeen = false;
					// This is probably a submarine.  Check if it was visible within the last turn
					//   The AI will complain if he sees it, but won't complain if he can't
					ASSERT( false );
					/*
					CPoiMobile* pPoiMobile = pAIUnit->GetServerPoiMobile();
					CPoiObserver* pPoiObserver = pPoiMobile->GetPoiObserver( iAIPlayer );
					if( pPoiObserver )
					{
						if( pPoiObserver->CanSeeThisPoi() ||
							pPoiObserver->WasVisibleWithinTurn( iCurrentTurn-1 ) )
						{
							bWasSeen = true;
							//fInvisibileMultiplier = 2.0f;
						}
					}
					if( bWasSeen == false ) continue;
					*-/
				}
				*/

				// Neutral unit inside my borders
				// Track his movement
				// Where was he one turn ago?
				//CAIPoiObjectState PoiObjectState;
				//pAIUnit->GetAIPoiObjectState( iCurrentTurn-1, &PoiObjectState );
				//CEOSAILocation LastTurnLocation = PoiObjectState.GetLocation( iCurrentTurn-1 );
				CEOSAILocation LastTurnLocation = pAIUnit->GetLastTurnLocation();

				float fLastTurnPixelX = LastTurnLocation.GetRealX();
				float fLastTurnPixelY = LastTurnLocation.GetRealY();
				char iLastTurnMapLastUnitLocationOwner = g_pEOSAICommonData->GetLastTurnOwnershipPlayerMap8()->m_Player.PixelValue( (int)fLastTurnPixelX, (int)fLastTurnPixelY );
				//char iLastTurnLocationOwnerLevel = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Ownership.PixelValue( fLastTurnPixelX, fLastTurnPixelY );

			//	char iLastTurnLocationOwnerDEBUG = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Player.PixelValue( fCurrentPixelX, fCurrentPixelY );
			//	char iCurrentLocationOwnerDEBUG = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Player.PixelValue( fCurrentPixelX, fCurrentPixelY );
			//	char iLastTurnLocationOwnerDEBUG2 = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Player.GridValue( fCurrentPixelX/8.0f, fCurrentPixelY/8.0f );
			//	char iCurrentLocationOwnerDEBUG2 = pWorldDescServer->GetLastTurnOwnershipPlayerMap8()->m_Player.GridValue( fCurrentPixelX/8.0f, fCurrentPixelY/8.0f );

				float fCurrentDistance1 = g_pEOSAICommonData->GetLastTurnOwnershipPlayerMap8()->GetDistanceToEscapeOwnedArea( fCurrentPixelX,fCurrentPixelY, iAIPlayer );
				float fCurrentDistance2 = g_pEOSAICommonData->GetCurrentOwnershipPlayerMap8()->GetDistanceToEscapeOwnedArea( fCurrentPixelX,fCurrentPixelY, iAIPlayer );
				float fCurrentDistance = min( fCurrentDistance1,fCurrentDistance2 );
				float fLastTurnDistance = g_pEOSAICommonData->GetLastTurnOwnershipPlayerMap8()->GetDistanceToEscapeOwnedArea( fLastTurnPixelX,fLastTurnPixelY, iAIPlayer );

				//float fLocationChange = pWorldDescServer->GetPixelDistance( fCurrentPixelX,fCurrentPixelY, fLastTurnPixelX,fLastTurnPixelY );
				CEOSAILocation CurrentLocation( CEOSAILocation::Pixel, fCurrentPixelX,fCurrentPixelY );
				CEOSAILocation LastLocation( CEOSAILocation::Pixel, fLastTurnPixelX,fLastTurnPixelY );
				float fLocationChange = g_pWorldDistanceTool->GetDistance( CurrentLocation, LastLocation );

				// The unit is inside my territory, and they were inside my territory last turn as well
				bool bJustEnteredBorders = false;
				bool bUnitIsLeaving = false;
				bool bUnitIsStationary = false;
				bool bUnitIsGoingDeeper = false;

				if( iLastTurnMapLastUnitLocationOwner != iAIPlayer )
				{
					//bJustEnteredBorders = true;
					pAIUnit->SetBorderViolationState( EOSAIEnumBorderViolation_JustEnteredBorders );
				}
				else if( fLocationChange < 3.0f ) // For some reason, I get an off by 2 error in locations
				{
					pAIUnit->SetBorderViolationState( EOSAIEnumBorderViolation_Stationary );
				}
				else if( fCurrentDistance == fLastTurnDistance )
				{
					//bUnitIsStationary = true;
					pAIUnit->SetBorderViolationState( EOSAIEnumBorderViolation_Stationary );
				}
				else if( fCurrentDistance <  fLastTurnDistance )
				{
					//bUnitIsLeaving = true;
					pAIUnit->SetBorderViolationState( EOSAIEnumBorderViolation_IsLeaving );
				}
				else if( fCurrentDistance >  fLastTurnDistance )
				{
					//bUnitIsGoingDeeper = true;
					pAIUnit->SetBorderViolationState( EOSAIEnumBorderViolation_IsGoingDeeper );
				}
				/*
				if( iLocationOwnerLevel == iLastTurnLocationOwnerLevel )
				{
					bUnitIsStationary = true;
				}
				if( iLocationOwnerLevel <  iLastTurnLocationOwnerLevel )
				{
					bUnitIsLeaving = true;
				}
				if( iLocationOwnerLevel >  iLastTurnLocationOwnerLevel )
				{
					bUnitIsGoingDeeper = true;
				}
				*/
				int g=0;
			}
		}
	}
}

void AIPlayer::SendMessagesAndAdjustForeignRelationsBasedOnBorderViolations()
{
	long iAIPlayer = GetPlayerNumber();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		if( iPlayer == iAIPlayer ) continue;
		//CPlayer* pPlayer = g_pCommonState->GetPlayer( iPlayer );
		EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
		if( pPlayer->IsAI() ) continue; // Don't worry about other AI's violating borders right now

		//EOSAIEnumForeignRelations eRel = m_pWorldDescServer->GetForeignRelations( iPlayer, iAIPlayer );
		EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayer, iAIPlayer );
		if( eRel == EOSAIEnumForeignRelations::enum_Neutral )
		{
			long iJustEnteredCount = 0;
			long iStationaryCount = 0;
			long iGoingDeeperCount = 0;
			long iNumberOfViolations = 0;
			CEOSAILongSet  ViolatingPoiObjects;
			float fNegativeForeignRelations = 0.0f;
			//POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
			POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
			while( pos )
			{
				//CEOSAIPoiObject* pAIPoiObject = m_pAIBrain->GetAIPoiObjectList()->GetNext( pos );
				CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
				if( pAIPoiObject->GetOwner() == iPlayer )
				{
					EOSAIEnumBorderViolation enumBorderViolation = pAIPoiObject->GetBorderViolationState();
					if( enumBorderViolation == EOSAIEnumBorderViolation_JustEnteredBorders )
					{
						iNumberOfViolations++;
						iJustEnteredCount++;
						fNegativeForeignRelations += 1.0f;
						ViolatingPoiObjects.Add( pAIPoiObject->GetObjectId() );
					}
					eif( enumBorderViolation == EOSAIEnumBorderViolation_IsLeaving ){}
					eif( enumBorderViolation == EOSAIEnumBorderViolation_Stationary )
					{
						iNumberOfViolations++;
						iStationaryCount++;
						fNegativeForeignRelations += 1.0f;
						ViolatingPoiObjects.Add( pAIPoiObject->GetObjectId() );
					}
					eif( enumBorderViolation == EOSAIEnumBorderViolation_IsGoingDeeper )
					{
						iNumberOfViolations++;
						iGoingDeeperCount++;
						fNegativeForeignRelations += 4.0f;
						ViolatingPoiObjects.Add( pAIPoiObject->GetObjectId() );
					}
				}
			}
			if( fNegativeForeignRelations > 0.0f )
			{
				// Lookup last border violation interaction
				long iLastBorderViolation = -1;
				long iCurrentTurn = g_pEOSAIInterface->GetCurrentTurn();
				//POSITION pos = g_pCommonState->GetWorldDescServer()->GetPlayerInteractions()->GetHeadPosition();
				POSITION pos = g_pEOSAICommonData->GetPlayerInteractions()->GetHeadPosition();
				while( pos )
				{
					//CEOSAIPlayerInteraction* pPlayerInteraction = g_pCommonState->GetWorldDescServer()->GetPlayerInteractions()->GetNext( pos );
					CEOSAIPlayerInteraction* pPlayerInteraction = g_pEOSAICommonData->GetPlayerInteractions()->GetNext( pos );
					CEOSAIPlayerInteraction_BorderViolation* pBorderViolation = dynamic_cast< CEOSAIPlayerInteraction_BorderViolation* >( pPlayerInteraction );
					if( pBorderViolation )
					{
						if( pBorderViolation->m_iViolateePlayer == iAIPlayer &&
							pBorderViolation->m_iViolatorPlayer == iPlayer )
						{
							iLastBorderViolation = max( iLastBorderViolation, pBorderViolation->m_iEventTurn );
						}
					}
				}
				if( iLastBorderViolation < iCurrentTurn )
				{
					//long iPlayerWhoMadeTheOffer = pTradeAgreement->GetPlayerWhoMadeTheOffer();
					//long iPlayerWhoDidntMakeTheOffer = pTradeAgreement->GetOtherPlayerNumber( iPlayerWhoMadeTheOffer );
					CEOSAIPlayerInteraction_BorderViolation* pBorderViolation = new CEOSAIPlayerInteraction_BorderViolation();
					pBorderViolation->Set( iCurrentTurn, iPlayer, iAIPlayer, fNegativeForeignRelations );
					//g_pCommonState->GetWorldDescServer()->AddNewPlayerInteractionAndSendFeelingsUpdate( pBorderViolation );
					g_pEOSAICommonData->AddNewPlayerInteractionAndSendFeelingsUpdate( pBorderViolation );

					// Send a warning message
					if( iJustEnteredCount >= 1 && iStationaryCount == 0 && iGoingDeeperCount == 0 )
					{
						ASSERT( false );
						#ifdef GAME_CODE
						CString strMessage;
						strMessage = Lang( 16, _T("You have crossed our national borders.  We assume this was an accident.  Please withdraw your forces.") );
						CGameEvent_IMail* pIMail = new CGameEvent_IMail();
						pIMail->SetSender( this->GetPlayerNumber() );
						pIMail->AddSendTo( iPlayer );
						pIMail->SetSubject( Lang( 17, _T("Your Border Crossing") ) );
						pIMail->SetMessage( strMessage );//"[Blank] has declared war on [Blank]" );
						pIMail->SetSelectablePoiObjects( ViolatingPoiObjects );
						pIMail->SendFromPlayerToServer();
						#endif GAME_CODE
					}
					else if( iJustEnteredCount >= 0 && iStationaryCount >= 1 && iGoingDeeperCount == 0 )
					{
						if( rand()%2 == 0 )
						{
							ASSERT( false );
							#ifdef GAME_CODE
							CString strMessage;
							strMessage = Lang( 18, _T("Please remove your units from my territory.") );
							CGameEvent_IMail* pIMail = new CGameEvent_IMail();
							pIMail->SetSender( this->GetPlayerNumber() );
							pIMail->AddSendTo( iPlayer );
							pIMail->SetSubject( Lang( 19, _T("Your Units") ) );
							pIMail->SetMessage( strMessage );//"[Blank] has declared war on [Blank]" );
							pIMail->SetSelectablePoiObjects( ViolatingPoiObjects );
							pIMail->SendFromPlayerToServer();
							#endif GAME_CODE
						}
					}
					else
					{
						ASSERT( false );
						#ifdef GAME_CODE
						CString strMessage;
						strMessage = Lang( 20, _T("You are putting a strain on diplomatic relations between our countries.  We don't want to ask you again to withdraw your forces.") );
						CGameEvent_IMail* pIMail = new CGameEvent_IMail();
						pIMail->SetSender( this->GetPlayerNumber() );
						pIMail->AddSendTo( iPlayer );
						pIMail->SetSubject( Lang( 21, _T("Your Border Violation") ) );
						pIMail->SetMessage( strMessage );//"[Blank] has declared war on [Blank]" );
						pIMail->SetSelectablePoiObjects( ViolatingPoiObjects );
						pIMail->SendFromPlayerToServer();
						#endif GAME_CODE
					}
				}
			}
		}
	}
}

//void CAIPlayer::CalculateResourceShortageAndExcess()
void AIPlayer::CalculateCitResValues()
{
	// Reset the Multiplier values
	m_CitResValueMultiplier.m_fCityProductionValueMultiplier = 2.0f;
	m_CitResValueMultiplier.m_fIronValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fFoodValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fOilValueMultiplier = 1.0f;
	m_CitResValueMultiplier.m_fGoldValueMultiplier = 1.0f;

	//
	float fIronConsumption = 0.0f;
	float fOilConsumption = 0.0f;
	long  iBuildableUnits = 0;
	float fBuildOilPerProduction = 0.0f;
	float fBuildFoodPerProduction = 0.0f;
	float fBuildIronPerProduction = 0.0f;
	CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIBrain()->GetAIThoughtDatabase();
	POSITION pos = pAIThoughtDatabase->GetUnitsICanBuild()->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pBuildableUnitTemplate = pAIThoughtDatabase->GetUnitsICanBuild()->m_List.GetNext( pos );
		float fUnitProductionCost = pBuildableUnitTemplate->GetProductionCost();
		fBuildIronPerProduction += pBuildableUnitTemplate->GetIronCost() / fUnitProductionCost;
		//fBuildOilPerProduction  += pBuildableUnitTemplate->GetOilConsumption() / fUnitProductionCost;
		//fBuildFoodPerProduction += pBuildableUnitTemplate->GetFoodConsumption() / fUnitProductionCost;
		iBuildableUnits++;
	}
	if( iBuildableUnits > 1 )
	{
		fBuildIronPerProduction /= iBuildableUnits;
		fBuildOilPerProduction  /= iBuildableUnits;
		fBuildFoodPerProduction /= iBuildableUnits;
	}

	float fTotalProduction = 0.0f;
	float fEstimatedOilConsumption = 0.0f;
	float fEstimatedIronConsumption = 0.0f;
	float fEstimatedFoodConsumption = 0.0f;
	float fEstimatedOilProduction = 0.0f;
	float fEstimatedIronProduction = 0.0f;
	float fEstimatedFoodProduction = 0.0f;

	//ASSERT( false );
#ifdef THINGS_TO_COMPILE_EVENTUALLY
	// I want to estimate the resource consumption somewhere else.  This really gets into the code.
	//   The AI needs to be smart enough to figure out what resources are valuable, how valuable, what the
	//   "production rate" means compared to the "usage rate" so that I can convert the production rate into a value.
	// Including stuff about the elasticity of the resource might also be useful.
#endif
	pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
		if( pAIUnit )
		{
			//fEstimatedOilConsumption += pAIUnit->GetServerUnit()->GetOilConsumption();
			fEstimatedOilConsumption += -pAIUnit->GetProductionAndConsumptionPerTurnAndMove( _T("Oil") );
			//fEstimatedFoodConsumption += pAIUnit->GetUnitTemplate()->AmountOfFoodNeededThisTurn();
			fEstimatedFoodConsumption += -pAIUnit->GetProductionAndConsumptionPerTurnAndMove( _T("Food") );
		}
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			//float fCityProduction = pAICity->GetTotalProduction();
			float fCityProduction = pAICity->GetProduction();
			fTotalProduction += fCityProduction;
			// City Consumption
			//fEstimatedOilConsumption  += pAICity->AmountOfOilNeededOverTimeperiod( 1.0f );
			fEstimatedOilConsumption  += pAICity->GetProductionAndConsumptionPerTurn( _T("Oil") );
			fEstimatedIronConsumption += fCityProduction * fBuildIronPerProduction;
			//fEstimatedFoodConsumption += pAICity->AmountOfFoodNeededOverTimeperiod( 1.0f );
			fEstimatedFoodConsumption += pAICity->GetProductionAndConsumptionPerTurn( _T("Food") );
			// Built-Unit Consumption
			fEstimatedOilConsumption  += (10.0f) * fCityProduction * fBuildOilPerProduction;
			fEstimatedFoodConsumption += (10.0f) * fCityProduction * fBuildFoodPerProduction;
		}
		CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
		if( pAIResource )
		{
			if( pAIResource->GetResourceType() == _T("Iron") ){ fEstimatedIronProduction += pAIResource->GetResourcePerTurn(); }
			if( pAIResource->GetResourceType() == _T("Food") ){ fEstimatedFoodProduction += pAIResource->GetResourcePerTurn(); }
			if( pAIResource->GetResourceType() == _T("Oil")  ){ fEstimatedOilProduction += pAIResource->GetResourcePerTurn(); }
		}
	}
/////

	fEstimatedOilProduction  += 0.1f * fTotalProduction;
	fEstimatedIronProduction += 0.1f * fTotalProduction;
	fEstimatedFoodProduction += 0.1f * fTotalProduction;

	float fOilDelta  = fEstimatedOilProduction  - (1.1f * fEstimatedOilConsumption);
	float fIronDelta = fEstimatedIronProduction - (1.1f * fEstimatedIronConsumption);
	float fFoodDelta = fEstimatedFoodProduction - (1.1f * fEstimatedFoodConsumption);
	float fOilDeltaOverStorage  =  fOilDelta / ( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalOil() + 1.0f );
	float fIronDeltaOverStorage = fIronDelta / ( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalIron() + 1.0f );
	float fFoodDeltaOverStorage = fFoodDelta / ( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalFood() + 1.0f );
	float fOilDeltaOverConsumption  =  fOilDelta / ( fEstimatedOilConsumption + 5.0f );
	float fIronDeltaOverConsumption = fIronDelta / ( fEstimatedIronConsumption + 5.0f );
	float fFoodDeltaOverConsumption = fFoodDelta / ( fEstimatedFoodConsumption + 5.0f );

	CEOSAIMathFunction  MathFunc_DeltaOverStorage;
	MathFunc_DeltaOverStorage.SetInputOutput( -1.0f,1.7f ); // Gone in 1 turn
	MathFunc_DeltaOverStorage.SetInputOutput( -0.1f,1.3f ); // Gone in 10 turns
	MathFunc_DeltaOverStorage.SetInputOutput(  0.0f,1.0f ); // No change
	MathFunc_DeltaOverStorage.SetInputOutput(  0.1f,0.8f ); // Will double storage in 10 turns
	MathFunc_DeltaOverStorage.SetInputOutput(  0.2f,0.7f ); // Will double storage in  5 turns
	CEOSAIMathFunction  MathFunc_DeltaOverConsumption;
	MathFunc_DeltaOverConsumption.SetInputOutput( -1.0f,1.7f ); // production is  0% of consumption
	MathFunc_DeltaOverConsumption.SetInputOutput( -0.1f,1.1f ); // production is 90% of consumption
	MathFunc_DeltaOverConsumption.SetInputOutput(  0.0f,1.0f ); // production equals consumption
	MathFunc_DeltaOverConsumption.SetInputOutput(  0.1f,0.9f ); // production is 110% of consumption
	MathFunc_DeltaOverConsumption.SetInputOutput(  0.2f,0.7f ); // production is 120% of consumption
	CEOSAIMathFunction  MathFunc_StorageAmount;
	MathFunc_StorageAmount.SetInputOutput(   0.0f,1.8f ); //       0 resource in storage
	MathFunc_StorageAmount.SetInputOutput(  20.0f,1.6f ); //   0- 20 resource in storage
	MathFunc_StorageAmount.SetInputOutput(  50.0f,1.2f ); //  20- 50 resource is storage
	MathFunc_StorageAmount.SetInputOutput( 100.0f,1.0f ); //  40-100 resource is storage

	long iPlayerNumber = GetPlayerNumber();
	CEOSAINationalSummary3* pSummary = g_pEOSAICommonData->GetAINationalSummary3( iPlayerNumber );
	//float fMoney1 = g_pEOSAICommonData->GetAINationalSummary3( iPlayerNumber )->m_ResourceSummary.m_TotalResources.Get( _T("Money") );
	float fTotalMoney = g_pEOSAICommonData->GetAINationalSummary3( iPlayerNumber )->GetTotalMoney();
	float fTotalOil = g_pEOSAICommonData->GetAINationalSummary3( iPlayerNumber )->GetTotalOil();
	float fTotalIron = g_pEOSAICommonData->GetAINationalSummary3( iPlayerNumber )->GetTotalIron();
	float fTotalFood = g_pEOSAICommonData->GetAINationalSummary3( iPlayerNumber )->GetTotalFood();
	ASSERT( fTotalMoney > 0.0f );
	ASSERT( fTotalOil > 0.0f );
	ASSERT( fTotalIron > 0.0f );
	ASSERT( fTotalFood > 0.0f );

	// Update the Multiplier values
	//   I can't use current resource deltas because my units/cities don't have orders
	//CAICommonData* pAICommonData = m_pWorldDescServer->GetAICommonData();
	m_CitResValueMultiplier.m_fCityProductionValueMultiplier = 
		2.0f + g_pEOSAICommonData->GetPercentageOfUnownedCitResOnMap01();
	m_CitResValueMultiplier.m_fOilValueMultiplier =
		MathFunc_DeltaOverStorage.GetOutput( fOilDeltaOverStorage ) *
		MathFunc_DeltaOverConsumption.GetOutput( fOilDeltaOverConsumption );
		MathFunc_StorageAmount.GetOutput( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalOil() );
	m_CitResValueMultiplier.m_fIronValueMultiplier =
		MathFunc_DeltaOverStorage.GetOutput( fIronDeltaOverStorage ) *
		MathFunc_DeltaOverConsumption.GetOutput( fIronDeltaOverConsumption ) *
		MathFunc_StorageAmount.GetOutput( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalIron() );
	m_CitResValueMultiplier.m_fFoodValueMultiplier =
		MathFunc_DeltaOverStorage.GetOutput( fFoodDeltaOverStorage ) *
		MathFunc_DeltaOverConsumption.GetOutput( fFoodDeltaOverConsumption ) *
		MathFunc_StorageAmount.GetOutput( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalFood() );

	// Gold is used for research and to buy resources
	m_CitResValueMultiplier.m_fGoldValueMultiplier =
		0.5f +
		0.1f * m_CitResValueMultiplier.m_fOilValueMultiplier +
		0.1f * m_CitResValueMultiplier.m_fIronValueMultiplier +
		0.1f * m_CitResValueMultiplier.m_fFoodValueMultiplier +
		0.2f * max( m_CitResValueMultiplier.m_fOilValueMultiplier, max( m_CitResValueMultiplier.m_fIronValueMultiplier, m_CitResValueMultiplier.m_fFoodValueMultiplier ) );
}


void AIPlayer::CalculateExperiencingResourceShortage01() // CalculateResourceConsumptionAndDeltas()
{
	//m_pWorldDescPlayerProxy->CalculateCurrentResourceDeltas();
	//g_pEOSAICommonData->GetAINationalSummary3(GetPlayerNumber())->AICalculateResourceConsumptionAndDeltas(false);
	g_pEOSAICommonData->GetAINationalSummary3(GetPlayerNumber())->CalculateResourceDeltas(); //AICalculateResourceConsumptionAndDeltas(false);

	EOSAI::ResourceAmounts ResourceConsumption =
		g_pEOSAICommonData->GetAINationalSummary3(GetPlayerNumber())->GetResourceSummary()->m_ResourceConsumptionAssumingNoOrders +
		g_pEOSAICommonData->GetAINationalSummary3(GetPlayerNumber())->GetResourceSummary()->m_ResourceConsumptionByCurrentOrders;

	float fResourceConsumption =
		ResourceConsumption.Get(_T("Money")) +
		ResourceConsumption.Get(_T("Food")) +
		ResourceConsumption.Get(_T("Iron")) +
		ResourceConsumption.Get(_T("Oil"));
	//float fResourceConsumption =
	//	g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetResourceSummary()->m_ResourceConsumption.Get( _T("Money") ) +
	//	g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetResourceSummary()->m_ResourceConsumption.Get( _T("Food") ) +
	//	g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetResourceSummary()->m_ResourceConsumption.Get( _T("Iron") ) +
	//	g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetResourceSummary()->m_ResourceConsumption.Get( _T("Oil") );
		//m_pWorldDescPlayerProxy->GetMoneyConsumption() +
		//m_pWorldDescPlayerProxy->GetFoodConsumption() +
		//m_pWorldDescPlayerProxy->GetIronConsumption() +
		//m_pWorldDescPlayerProxy->GetOilConsumption();

	// Calculate resource shortage
	//   Because we have cleared all the unit and city orders, the oil and iron deltas will be low
	EOSAI::ResourceAmounts ResourceDelta = g_pEOSAICommonData->GetAINationalSummary3(GetPlayerNumber())->GetResourceSummary()->GetResourceDelta();
	float fResourceDelta =
		ResourceDelta.Get(_T("Money")) + //GetDeltaMoney() + 
		ResourceDelta.Get(_T("Food")) + //GetDeltaFood() + 
		ResourceDelta.Get(_T("Iron")) + //GetDeltaIron() + 
		ResourceDelta.Get(_T("Oil"));  //GetDeltaOil();
//	float fResourceDelta =
//		g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetResourceSummary()->m_ResourceDelta.Get( _T("Money") ) + //GetDeltaMoney() + 
//		g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetResourceSummary()->m_ResourceDelta.Get( _T("Food") ) + //GetDeltaFood() + 
//		g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetResourceSummary()->m_ResourceDelta.Get( _T("Iron") ) + //GetDeltaIron() + 
//		g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetResourceSummary()->m_ResourceDelta.Get( _T("Oil") );  //GetDeltaOil();
		//m_pWorldDescPlayerProxy->GetDeltaMoney() + fOilDelta  + fIronDelta + fFoodDelta;
	float fResourceTotal = 
		g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalMoney() + 
		g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalFood() + 
		g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalIron() + 
		g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalOil();
	float fResourceDeltaOverStorage = (fResourceDelta-2) / max( fResourceTotal,10.0f );

	CEOSAIMathFunction  MathFunc_DeltaOverStorage;
	MathFunc_DeltaOverStorage.SetInputOutput( -1.0f, 1.0f ); // Gone in 1 turn
	MathFunc_DeltaOverStorage.SetInputOutput( -0.5f, 1.0f ); // Gone in 2 turns
	MathFunc_DeltaOverStorage.SetInputOutput( -0.1f, 0.5f ); // Gone in 10 turns
	MathFunc_DeltaOverStorage.SetInputOutput( -0.04f,0.0f ); // Gone in 25 turns
	MathFunc_DeltaOverStorage.SetInputOutput(  0.0f, 0.0f ); // No change
	CEOSAIMathFunction  MathFunc_StorageAmount;
	MathFunc_StorageAmount.SetInputOutput(   0.0f,1.0f ); //       0 resources in storage
	MathFunc_StorageAmount.SetInputOutput( (fResourceConsumption+4.0f)*5.0f, 1.0f ); //   0-100 resources is storage ( 0-25 each)
	MathFunc_StorageAmount.SetInputOutput( (fResourceConsumption+4.0f)*10.0f,0.0f ); // 100-200 resources is storage (25-50 each)
	m_fExperiencingResourceShortage01 = 
		max( MathFunc_DeltaOverStorage.GetOutput( fResourceDeltaOverStorage ),
			 MathFunc_StorageAmount.GetOutput( fResourceTotal ) );
	int g=0;
}
/*
void CAIPlayer::AddedBuildItemToResourceConsumption( CBuildOption* pBuildOption )
{
	/-*
	if( pBuildOption->IsMoney() )
	{
		sdfpjok
	}
	*-/
}
*/

/*
bool AIPlayer::ThereAreUnprocessedEvents()
{
	if( m_UnprocessedIncomingMail.IsEmpty() == FALSE )
	{
		return true;
	}
	if( m_UnprocessedIncomingTradeAgreementResponses.IsEmpty() == FALSE )
	{
		return true;
	}
	if( m_UnprocessedIncomingMailResponses.IsEmpty() == FALSE )
	{
		return true;
	}
	return false;
}
*/

bool AIPlayer::ProcessUnprocessedEvents()
{
	bool bThisEventWasSignificantEnoughToRecalculateTheTurn = false;
	while( m_UnprocessedIncomingMail.IsEmpty() == FALSE )
	{
		//CGameEvent_IMail* pIMail = m_UnprocessedIncomingMail.RemoveHead();
		CEOSAIMail* pIMail = m_UnprocessedIncomingMail.RemoveHead();
		m_StrategicAI.ProcessMail( pIMail, &bThisEventWasSignificantEnoughToRecalculateTheTurn);
	}
	while( m_UnprocessedIncomingTradeOffers.IsEmpty() == FALSE )
	{
		CEOSAITradeAgreement* pTrade = m_UnprocessedIncomingTradeOffers.RemoveHead();
		m_StrategicAI.ProcessTradeOffer( pTrade, &bThisEventWasSignificantEnoughToRecalculateTheTurn);
	}
	while( m_UnprocessedIncomingTradeAgreementResponses.IsEmpty() == FALSE )
	{
		CEOSAITradeAgreementResponse* pTradeAgreementResponse = m_UnprocessedIncomingTradeAgreementResponses.RemoveHead();
		m_StrategicAI.ProcessTradeAgreementResponse( pTradeAgreementResponse, &bThisEventWasSignificantEnoughToRecalculateTheTurn);
	}
	while( m_UnprocessedIncomingMailResponses.IsEmpty() == FALSE )
	{
		CEOSAIMailResponse* pIMailResponse = m_UnprocessedIncomingMailResponses.RemoveHead();
		m_StrategicAI.ProcessMailResponse( pIMailResponse, &bThisEventWasSignificantEnoughToRecalculateTheTurn);
	}

	return bThisEventWasSignificantEnoughToRecalculateTheTurn;
}

/*
void CAIPlayer::WriteDebugDataButtons_DesiresSortedByScore( CGeneralSubWindow* pParentWindow )
{
	long y = 0;
	CBCButton* pButton = new CBCButton();
	pButton = new CBCButton();
	pButton->Create( "Start Server", WS_CHILD | WS_VISIBLE, CRect( 200,y, 350,y+20 ), m_Window.m_pSubWindow, ID_STARTSERVER );
	y+=20;
	m_DesireAnnotations
	//m_AIThoughtDatabase.GetSortedDesiresItems( &m_DesireAnnotations );//CList< CEOSAIDebugData* >* pData )
	//
}
*/
/*
void CAIPlayer::Incoming_TradeOffer( CTradeAgreement* pTradeAgreement )
{
	// Evaluate the Trade Agreement, 


	// INCOMPLETE
	// Right now, the AI will accept all trade offers
	//g_pMessageManager->SendTradeOffer( GetPlayer()->GetMessageTargetId(), g_pMessageTargetManager->GetMessageTarget_ServerId(), pTradeAgreement );

	if( Public::m_bAIShouldBounceBackTradeOffers )
	{
		bool bICreatedThisOffer = ( pTradeAgreement->GetPlayerWhoMadeTheOffer() == GetPlayer()->GetPlayerNumber() );

		// Attach a new Trade id and bounce same offer back to the sender
		//pTradeAgreement->m_iTradeAgreementId = GetWorldDesc()->GetNextTradeAgreementId();
		if( bICreatedThisOffer == false &&
			pTradeAgreement->m_TradeAgreementState == EOSAIEnumTradeAgreementState_WaitingForApproval )
		{
			/*
			g_pMessageManager->SendMessage_ApproveTradeAgreement(
				GetPlayer()->GetMessageTargetId(),
				g_pMessageTargetManager->GetMessageTarget_ServerId(),
				GetPlayer()->GetMessageTargetId(),
				pTradeAgreement->m_iTradeAgreementId );
			*-/

			g_pMessageManager->SendMessage_TradeAgreementStateChange( 
				GetPlayer()->GetMessageTargetId(), 
				g_pMessageTargetManager->GetMessageTarget_ServerId(), 
				GetPlayer()->GetMessageTargetId(), 
				CTradeAgreement::enum_Accept,
				EOSAIEnumTradeAgreementState_CompletedAgreement,
				pTradeAgreement->m_strTradeAgreementId );
/*
			g_pMessageManager->SendMessage_TradeAgreementStateChange(
				GetPlayer()->GetMessageTargetId(),
				g_pMessageTargetManager->GetMessageTarget_ServerId(),
				GetPlayer()->GetMessageTargetId(),
				pTradeAgreement->m_ePlayerResponse[1],
				pTradeAgreement->m_TradeAgreementState,
				//pTradeAgreement->m_iTradeAgreementId );
				pTradeAgreement->m_strTradeAgreementId );

				GetPlayer()->GetMessageTargetId(),
				g_pMessageTargetManager->GetMessageTarget_ServerId(),

			// Send a message back to the player
			g_pMessageManager->SendMessage_TradeAgreementStateChange(
				g_pMessageTargetManager->GetMessageTarget_ServerId(),
				iSenderMessageId,
				g_pMessageTargetManager->GetMessageTarget_ServerId(),
				pTradeAgreement->m_ePlayerResponse[1],
				pTradeAgreement->m_TradeAgreementState,
				//pTradeAgreement->m_iTradeAgreementId );
				pTradeAgreement->m_strTradeAgreementId );

			EOSAIEnumTradeAgreementResponse

			g_pMessageManager->SendMessage_TradeAgreementStateChange(
				GetPlayer()->GetMessageTargetId(),
				g_pMessageTargetManager->GetMessageTarget_ServerId(),
				GetPlayer()->GetMessageTargetId(),
				EOSAIEnumTradeAgreementState_CompletedAgreement,
				pTradeAgreement->m_strTradeAgreementId );
*-/
			/*
			g_pMessageManager->SendMessage_TradeAgreementStateChange(
				GetPlayer()->GetMessageTargetId(),
				g_pMessageTargetManager->GetMessageTarget_ServerId(),
				GetPlayer()->GetMessageTargetId(),
				EOSAIEnumTradeAgreementState_CompletedAgreement,
				pTradeAgreement->m_strTradeAgreementId );
			*-/
			/*
			g_pMessageManager->SendMessage_TradeAgreementStateChange(
				GetPlayer()->GetMessageTargetId(),
				g_pMessageTargetManager->GetMessageTarget_ServerId(),
				GetPlayer()->GetMessageTargetId(),
				EOSAIEnumTradeAgreementState_RejectedOffer,
				pTradeAgreement->m_iTradeAgreementId );
			*-/
		}
		//g_pMessageManager->SendTradeOffer( GetPlayer()->GetMessageTargetId(), g_pMessageTargetManager->GetMessageTarget_ServerId(), pTradeAgreement );
	}
}
*/
/*
void AIPlayer::Incoming_IMail( CGameEvent_IMail* pIMail )
{
	m_UnprocessedIMail.AddTail( pIMail );
}
*/
void AIPlayer::Incoming_IMailResponse( long iFromPlayer, long iAIMailId, EOSAI::EnumAIMailResponse eResponse )
{
	//ASSERT(false); // Uncomment -- m_UnprocessedIMailResponses.AddTail( pResponse );
	//CIMailResponse* pResponse = new CIMailResponse( iIMailId, iFromPlayer, eResponse );
	CEOSAIMailResponse* pResponse = new CEOSAIMailResponse(iFromPlayer, iAIMailId, eResponse );
	m_UnprocessedIncomingMailResponses.AddTail( pResponse );
}
/*
void AIPlayer::Incoming_TradeOffer( CEOSAITradeAgreement* pTradeAgreement )
{
	m_UnprocessedIncomingTrade.AddTail( pTradeAgreement );
}
*/

/*
void CAIPlayer::ProcessIMail( CGameEvent_IMail* pIMail, bool* bResetProcessing )
{
	m_StrategicAI.ProcessIMail( pIMail );
}

void CAIPlayer::ProcessIMailResponse( CIMailResponse* pIMailResponse, bool* bResetProcessing )
{
	m_StrategicAI.ProcessIMailResponse( pIMailResponse );
}

void CAIPlayer::ProcessTradeAgreementResponse( CTradeAgreementResponse* pTradeAgreementResponse, bool* bResetProcessing )
{
	m_StrategicAI.ProcessTradeAgreementResponse( pTradeAgreementResponse );
}
*/

//void CAIPlayer::Incoming_TradeAgreementResponse( long iTradeAgreement, long iPlayerWhoResponded, CTradeAgreement::EOSAIEnumTradeAgreementResponse eResponse, EOSAIEnumTradeAgreementState eNewState )

void AIPlayer::Incoming_TradeAgreementResponse( CString strTradeAgreementId, long iPlayerWhoResponded, EOSAIEnumTradeAgreementResponse eResponse, EOSAIEnumTradeAgreementState eNewState )
{
	//ASSERT(false); // need to add this back in below: m_UnprocessedIncomingTradeAgreementResponses.AddTail( pResponse );
	CEOSAITradeAgreementResponse* pResponse = new CEOSAITradeAgreementResponse( strTradeAgreementId, iPlayerWhoResponded, eResponse, eNewState );
	m_UnprocessedIncomingTradeAgreementResponses.AddTail( pResponse );
}

void AIPlayer::IncomingEvent_PlayerResigned( long iPlayerNumberWhoResigned )
{
	ASSERT(false);
	m_bTrigger_NeedToRecalculateEntireTurn = true;
}

void AIPlayer::IncomingEvent_RequestAITradeDesires( long iSendToPlayer )
{
	m_StrategicAI.IncomingEvent_RequestAITradeDesires( iSendToPlayer );
}

void AIPlayer::EvaluateTradeAgreement(CEOSAITradeAgreement* pTradeAgreement, CString* pstrOpinionText, long* piOpinionSum)
{
	//void EvaluateTradeAgreement(pTradeAgreement, pstrOpinionText, piOpinionSum);
	//pAIPlayer->EvaluateTradeAgreement(pTradeAgreement, pstrOpinionText, piOpinionSum);
	GetStrategicAI()->EvaluateTradeAgreement(pTradeAgreement, pstrOpinionText, piOpinionSum);
}


/*
void AIPlayer::IncomingEvent_TeamFormed( CString strTeamName )
{
	m_bTrigger_NeedToRecalculateEntireTurn = true;
}
*/

/*
void CAIPlayer::Incoming_DeclarationOfWar( long iActor, long iTarget, long iTurn )
{
	// Create a player interaction object
	CEOSAIPlayerInteraction_DeclaredWar* pDeclarationOfWar = new CEOSAIPlayerInteraction_DeclaredWar;
	pDeclarationOfWar->m_iActor = iActor;
	pDeclarationOfWar->m_iTarget = iTarget;
	pDeclarationOfWar->m_iEventTurn = iTurn;
	m_UnprocessedPlayerInteractions.AddTail( pDeclarationOfWar );
}
*/

/*
void CAIPlayer::InstanciateSightings()
{
	m_HistoricalSightings.Initialize( GetWorldDescPlayer(), this );

	// Create some sightings for the border map
	POSITION pos = GetWorldDescPlayer()->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = GetWorldDescPlayer()->GetPoiList()->GetNext( pos )->GetPtr();
		CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		if( pPoiObject )
		{
			m_HistoricalSightings.AddSighting( pPoiObject, GetWorldDescPlayer()->GetCurrentTurn(), pPoiObject->GetLocation() );
		}
	}
	m_HistoricalSightings.UpdateSignificanceOfSightings( 1.0f );
}
*/
/*
void CAIPlayer::UpdateForeignRelationFeelingsBasedOnPlayerInteractionHistory()
{
	POSITION pos = m_PlayerInteractions.GetHeadPosition();
	while( pos )
	{
		CEOSAIPlayerInteraction*  pPlayerInteraction = m_PlayerInteractions.GetNext( pos );
		//pPlayerInteraction->UpdateForeignRelationFeelings( m_pAIBrain->GetAIForeignRelationsSituation() );
	}
}
*/
/*
void  CAIPlayer::CalculateResourceShortageState()
{
	// Calculate Resource Production/Consumption
	m_pWorldDescPlayerProxy->CalculateCurrentResourceDeltas();
	float fTotalMoney = m_pWorldDescPlayerProxy->GetTotalMoney();
	float fTotalFood = m_pWorldDescPlayerProxy->GetTotalFood();
	float fTotalIron = m_pWorldDescPlayerProxy->GetTotalIron();
	float fTotalOil = m_pWorldDescPlayerProxy->GetTotalOil();

	float fDeltaMoney = m_pWorldDescPlayerProxy->GetDeltaMoney();
	float fDeltaFood = m_pWorldDescPlayerProxy->GetDeltaFood();
	float fDeltaIron = m_pWorldDescPlayerProxy->GetDeltaOil();
	float fDeltaOil = m_pWorldDescPlayerProxy->GetDeltaOil();

	// The oil delta is going to be low because my units have had their movement orders cleared
	//   So, add-in some oil consumption
	float fExpectedUnitOilConsumption = 0.0f;
	

	// The iron delta is going to be low because my cities have had their build orders cleared
	float fExpectedCityIronConsumption = 0.0f;
}
*/


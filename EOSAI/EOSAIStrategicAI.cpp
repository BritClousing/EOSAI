
#include <stdafx.h>
#include "EOSAIStrategicAI.h"
#include "EOSAIStrategicAIOrder_SuggestTeam.h"
#include "EOSAIStrategicAIOrder_OfferPeaceTreaty.h"
#include "EOSAIStrategicAIOrder_SuggestMultilateralWar.h"
#include "EOSAIStrategicAIOrder_DeclareWar.h"
#include "EOSAIStrategicAIOrder_OfferAlliance.h"
//#include "AIAction_DeclareWar.h"
//#include "EOSAIStrategicAIOrder_OfferSurrender.h"
#include "EOSAIStrategicAIOrder_AIOffersToResign.h"
#include "EOSAIStrategicAIOrder_Trade.h"
#include "EOSAIStrategicAIOrder_SendReinforcePeaceMail.h"
#include "EOSAIPlayerInteraction.h"
#include "EOSAIPlayerInteraction_Trade.h"
#include "EOSAINationalSummary3.h"

#include "MessageFromAI_TradeDesires.h"
#include "MessageFromAI_TradeOfferResponse.h"

#include "EOSAIStrategicAI.h"
#include "AIPlayer.h"
#include "EOSAIGamePlayer.h"
//#include "TWDx.h"
#include "EOSAIMath.h"
//#include "WorldDescServer.h"
//#include "WorldDescPlayerProxy.h"
//#include "BCString.h"

//#include "Message2_DeclareWarMessage.h"

#include "EOSAICommonData.h"
#include "City.h"
//#include "BuildingDescription.h"

#include "EOSAIPlayerInteraction_DeclaredWar.h"
#include "EOSAIPlayerInteraction_Trade.h"
#include "EOSAITradeAgreement.h"
#include "EOSAITradeAgreementMultiplier.h"
#include "EOSAITechnologyDesc.h"
#include "EOSAIBuildingDescription.h"
//#include "Message2_TradeDesires.h"

//#include "Checkpoint.h"
//#include "DebugDialogModal.h"

#include "MessageFromAI_TradeOfferOpinion.h"

#include "EOSAISerial.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIBCArray2.h"
#include <math.h>
#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

#include "EOSAIPublic.h"
//#include "EOSAIMath.h"
//#include "LanguagePack.h"
//extern CString Lang( long iID, CString strEnglish );

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*
class CForeignRelationsIntermediateNode
{
	public:
		CForeignRelationsIntermediateNode(){}

		float  m_fWarDesire; // 0 = peace, 1 = fight
};
*/
	// ForeignRelationsSituation does several tasks:
	//
	//	Creates NationalSummaries for each nation
	//	Creates NationalAlliedAndEnemyPower objects for each nation
	//	Comes up with some ideas for ForeignRelations changes (alliances, wars, gang-up, etc)
	//
	// Overall situation:
	//	Lost - One player has overwhelming strength (70%+).  Probably just a matter of time.
	//	DangerousPlayer - One player has 40-70% of world power.  Other players should align.
	//	DuelingPlayers - Two powerful players fighting for the win.  Other players should align.
	//	DangerousGroup - One group has a disproportionate amount of world power.  
	//		Non-group players should align against them, with them,
	//		and attempt to steal away in-group players.

	//
	// Algorithm:
	//
	//   LEVEL 1: Calculate the foreign relations thoughts for all players about all players.
	//   This doesn't take into account the feasibility of attack because we don't know
	//     yet what player's will join us, form peace with us, whether they want to attack
	//     the player, too.
	//     + Player is getting too strong (consider attacking, and forming a coalition against them)
	//     + Player is accessible (accessible players make better targets)
	//     + Don't like player (want to attack players who I don't like)
	//     Normalize my attack desires then multiply by my combat capability
	//       I may be looking for someone to attack
	//     = Single desire to attack value
	//
	//   LEVEL 2: Consider other's players thoughts on these matters, think about adding allies
	//     based on common desires.  Increase desire to attack players who want to attack me
	//     (I'd rather attack the guy who doesn't like me / has no one else to attack than attack
	//     a third player and then have this guy attack me).
	//


	// Want to strengthen myself, weaken the other powerful players
	//   Maintain some constant relationship (alliance or war) to players,
	//   Avoid disloyal allies (whether they are disloyal to me or other players)
	//   Seek to retaliate against players disloyal to me
	// Accessibility:
	//   PlayerX has no accessibility to PlayerY:
	//     Alliance has little value to either player, except as a deterent to other players (double-team)
	//     War against between them isn't very dangerous
	//   PlayerX has high accessibility to PlayerY:
	//     Alliance has more value (worry less about invasion), can more easily double-team an enemy
	//     War between them is more dangerous

	//   PlayerX getting PlayerY to attack PlayerZ
	//     If PlayerX is on the defensive,
	//       Is valuable to PlayerX because it allows for a relief from attack
	//       PlayerX is on the defensive anyway, so he wasn't going to capture any new territory anyway
	//     If PlayerX is on the offensive,
	//       Can be valuable for weakening PlayerZ further
	//       Can be valuable to avoid overextending PlayerX (making him weak for other players)
	//       But, PlayerX might lose some territory to PlayerY that he might've captured for himself

	// What affects the desire to fight someone versus ally with them?
	//   - If there are plenty of unowned CitRes, this cuts down on the desire to fight
	//     - Although, it might be useful to do an early rush
	//   - Proximity to other player (does this favor being an ally or enemy?)
	//     - Closer promitity increases the chances of double-cross (both me double-crossing him and vice-versa)
	//       So distant allies are "safer", but offer less opportunity for me to double-cross him.
	//     - Enemies who are far away aren't useful enemies because you can't capture their territory
	//   - Trade (if I need to get oil and they have extra oil, I can setup a trade relationship)
	//   - Feelings (which includes past warfare)
	//   - My Production/Military power versus Other Player's Production/Military power
	//   - My current combat situation (am I already in over my head?)
	//   - If I have reached the limits of my expansion, increase combat desire and look for an enemy
	//     - This is especially true if the other player might not have reached their expansion limit
	//         It's better to start a war now than wait until they capture the easy unowned territory.
	//   - If the player is already at war, it might be good to jump in and grab a chunk of territory
	//   - The shape of the proximity affects things too 
	//       (e.g. if i barely touch one player, but share a decent border with a second player, I might
	//        want to go after the second player so I don't end up with a stretched empire, which is hard
	//        to defend.  The AI should prefer rounder empires, preferably in a map corner or edge.)
	//   - Try to avoid allying with players much stronger than me (this can be ignored if I need
	//       to form an alliance to oppose a third, even stronger player.
	//  Note: The AI should start to pre-plan unit positions and production based on who I am going 
	//     to attack.  I might want to move ground units and air to an island to serve as a defended
	//     forward base.

	//
	// There are several levels of player power:
	//   Weak: (might become allies with him, but he won't be very valuable, and don't let
	//     him suck you into a war - he'll probably draw wars to him).  May want to backstab him,
	//     may be able to keep him around as essentially a vassal state until all the other
	//     players have been eliminated.  (No reason to waste your units and his capturing
	//     his country when he's perfectly willing to do what you want and fight a common enemy.)
	//   Moderate: a decent ally, a respectable enemy
	//   Strong: a strong ally, dangerous enemy.  Won't want to be enemies with him unless
	//     you have backup.  As an ally, you have to be careful not to allow him to gain
	//     too much more power.  Want to increase power more quickly than he does.
	//   V.Strong: Makes a bad ally because he's going to run over you once you help him
	//     eliminate the other players.  Best strategy is to form an alliance with the other
	//     players against him.  Hope that other players take the brunt of his attacks.
	//   Overwhelming: so powerful that he already overpowers all the other players combined.
	//     The game is essentially over.  Might as well form an alliance with everyone else
	//     and make a go at it, maybe you'll get lucky.
	//

	// The way this algorithm works:
	//   Figure out how much I want to attack each player.
	//     This is based on the player's strength, player's alliance, geographical accessibility, ..
	//   (What about forming alliances with players? What about asking another player to gang
	//    up on a player? Need to calculate how much the other players want to fight the other
	//    players, too.)  Also, players can't just end wars - it must negotiate peace.  This is
	//    relevant to having the player break-off a war to fight a third player.


	// Alter the relationships, aim for a good score
	//   The definition of a "good score" is: 
	//     Strengthens me (by giving me allies),
	//     Weakens the strongest players,
	//     Gives me enemies that I can handle and at least one that is accessible
	//       (Enemies that I can handle includes weaker enemies and enemies at war with others)
	//     Maintains stable relationships,
	//     Avoids forming alliances with unreliable players
	//   There are some potential conflicts here: forming an alliance with a powerful
	//     player "strengthens me" AND "strengthens the strongest players".  It can be 
	//     useful if I'm on the defensive, but I risk being overrun by my powerful ally.
	//   Also, if I don't have accessibility to PlayerZ, it doesn't make much sense to
	//     talk players X and Y into attacking PlayerZ since I can't get the spoils (unless
	//     I'm hoping to weaken the other players for an attack).
	//   And if PlayerX doesn't have access to PlayerZ, it doesn't make much sense to
	//     talk him into attacking PlayerZ - he can't do anything anyway.
	//   Also, I don't want to get into a situation where PlayerZ is attacking me, and I get
	//     PlayerX to attack him from behind.  This will weaken PlayerZ (which is good), but
	//     allow PlayerX to gain some easy territory (since PlayerZ's units are near me).
	//     This might be okay if PlayerX is small and weak because it won't substantially 
	//     increase PlayerX's power.

	// What about attacking enemies that are nearby for the purpose of gaining territory + CitRes?
	// Note: in the case that one player is too powerful, players should set-aside their wars
	// Calculate the desire of the other players to attack the other players (especially me).

float LimitValue( float fValue, float fMin, float fMax )
{
	if( fValue < fMin ){ fValue = fMin; }
	if( fValue > fMax ){ fValue = fMax; }
	return fValue;
}

// Used internally by the AI.  It's adjusted by feelings and other factors.
// It's used as the basis for the "EvaluateTrade" and "AITradeDesires" information
class CAITradePrices
{
	public:
		CAITradePrices()
		{
			m_iAIPlayer = 0;
			m_iHumanPlayer = 0;
			m_bTheAIDidNotProvideAnyDataBecauseYouAreAtWar = false;
			// Resources
			m_iWantsToBuyAndSellOil = 0;  // +10 = wants to buy 10 oil,  -10 = wants to sell 10 oil
			m_fPriceOfOil = 1.0f;         // Buy or Sell Price
			m_iWantsToBuyAndSellIron = 0; // +10 = wants to buy 10 iron, -10 = wants to sell 10 iron
			m_fPriceOfIron = 1.0f;        // Buy or Sell Price
			m_iWantsToBuyAndSellFood = 0; // +10 = wants to buy 10 food, -10 = wants to sell 10 food
			m_fPriceOfFood = 1.0f;        // Buy or Sell Price
			// Technology
			m_fBuyTechnologyMultiplier = 0.5f;
			m_fSellTechnologyMultiplier = 2.0f;
			// Peace, Alliances
			m_fWantsPeace01 = 1.0f;       // 0.0 = wants to continue war, 1.0 = overwhelmed, wants peace
		}

		long  m_iAIPlayer;
		long  m_iHumanPlayer;
		//
		bool  m_bTheAIDidNotProvideAnyDataBecauseYouAreAtWar;
		// Resources
		long  m_iWantsToBuyAndSellOil;    // +10 = wants to buy 10 oil,  -10 = wants to sell 10 oil
		float m_fPriceOfOil;              // Buy or Sell Price
		long  m_iWantsToBuyAndSellIron;   // +10 = wants to buy 10 iron, -10 = wants to sell 10 iron
		float m_fPriceOfIron;             // Buy or Sell Price
		long  m_iWantsToBuyAndSellFood;   // +10 = wants to buy 10 food, -10 = wants to sell 10 food
		float m_fPriceOfFood;             // Buy or Sell Price
		// Technology
		float m_fBuyTechnologyMultiplier; // Low Values = Won't pay you much for your tech
		float m_fSellTechnologyMultiplier;// High Values = You have to pay me a lot for my technology
		// Peace, Alliances
		float m_fWantsPeace01;    // 0.0 = wants to continue war, 1.0 = overwhelmed, wants peace
};

class CAITradeEvaluationResult
{
	public:
		CAITradeEvaluationResult()
		{
			m_fTradeValue11 = 0.0f; // -1.0 (AI doesn't want it) to 0.0 (okay trade) to +1.0 (gift)
			m_fLumpSumValueToAI = 0.0f;
			m_fLumpSumValueToPlayer = 0.0f;
			m_fForeignRelationsBonus = 0.0f;
			m_bAIDoesntWantToSellThatMuch = false;
		}

		float  m_fTradeValue11; // -1.0 (AI doesn't want it) to 0.0 (okay trade) to +1.0 (gift)
		float  m_fLumpSumValueToAI;
		float  m_fLumpSumValueToPlayer;
		float  m_fForeignRelationsBonus;
		bool   m_bAIDoesntWantToSellThatMuch;
};

CEOSAIStrategicAI::CEOSAIStrategicAI()
{
	// Whole-Map Totals
	//m_fTotalCityProductionOnMap = 0.0f;
	//m_fTotalResourcesOnMap = 0.0f;


	// Buy/Sell Desires
	//
	//m_iWantsToBuyAndSellOil = +10;    // +10 = wants to buy 10 oil,  -10 = wants to sell 10 oil
	//m_iWantsToBuyAndSellIron = +10;   // +10 = wants to buy 10 iron, -10 = wants to sell 10 iron
	//m_iWantsToBuyAndSellFood = +10;   // +10 = wants to buy 10 food, -10 = wants to sell 10 food
	// Technology
	//m_fWantsToBuyTechnology01 = 0.3f;  // 1.0 = wants to buy technology (higher when behind in tech)
	// Peace, Alliances
	//m_fWantsPeace01 = 0.3f;    // If the AI player is overwhelmed, this is high
	//float m_fWantsAlliance01; // Wants allies
	//float m_fWantsNonAggressionPact01;

	m_fMoneyToSpendOnResearch = 0.0f;

	//m_pStrategicOrder = NULL;

	//m_iPlayer = 0;
	//m_bStrategicAIOrderResponseReceived = false;
	m_fStrategy_CaptureUnownedCitRes01 = 0.0f;
	m_fStrategy_CombatOrPlanForCombat01 = 0.0f;
}

/*
EOSAI::CGamePlayer* CEOSAIStrategicAI::GetPlayer()
{
	return m_pAIPlayer->GetPlayer();
}
*/

long CEOSAIStrategicAI::GetPlayerNumber()
{
	//return m_pAIPlayer->GetPlayer()->GetPlayerNumber();
	return m_pAIPlayer->GetPlayerNumber();
}

long CEOSAIStrategicAI::GetCurrentTurn()
{
	return g_pEOSAIInterface->GetCurrentTurn();
	//return m_pAIPlayer->GetWorldDescServer()->GetCurrentTurn();
}
/*
CWorldDescPlayerProxy* CEOSAIStrategicAI::GetWorldDescPlayerProxy()
{
	return m_pAIPlayer->GetWorldDescPlayerProxy();
}

CWorldDescServer* CEOSAIStrategicAI::GetWorldDescServer()
{
	return m_pAIPlayer->GetWorldDescServer();
}
*/
/*
CWorldDescPlayer* CEOSAIStrategicAI::GetWorldDescPlayer()
{
	return m_pAIPlayer->GetWorldDescPlayer();
}
*/
/*
// Serialize/Deserialize
void  CEOSAIStrategicAI::Serialize( CEOSAISerial* pSerial )
{
	/-*
	long iCount = (long) m_PlayerInteractions.GetCount();
	pSerial->Serialize( iCount );
	POSITION pos = m_PlayerInteractions.GetHeadPosition();
	while( pos )
	{
		CEOSAIPlayerInteraction* pPlayerInteraction = m_PlayerInteractions.GetNext( pos );
		pPlayerInteraction->Serialize( pSerial );
	}
	*-/
}

void  CEOSAIStrategicAI::Deserialize( CEOSAISerial* pSerial )
{
	/-*
	long iCount = 0;
	pSerial->Deserialize( iCount );
	for( long i=0; i<iCount; i++ )
	{
		CEOSAIPlayerInteraction* pPlayerInteraction = (CEOSAIPlayerInteraction*) pSerial->Instanciate( NULL );
		//CEOSAIPlayerInteraction* pPlayerInteraction = new CEOSAIPlayerInteraction();
		//pPlayerInteraction->Deserialize( pSerial );
		m_PlayerInteractions.AddTail( pPlayerInteraction );
	}
	*-/
}
*/
CString  CEOSAIStrategicAI::OutputDebugString()
{
	CString str, strTemp;

	long iNumberOfPlayers = 5;
	//if( g_pCommonState )
	//{
		iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	//}

	str += _T("\n");
	if( m_pAIPlayer ){ strTemp.Format( _T("AIPlayer = %d\n"), m_pAIPlayer->GetGamePlayer()->GetPlayerNumber() ); }
	else{              strTemp.Format( _T("AIPlayer = 1\n") ); }
	str += strTemp;

	str += _T("CurrentScore\n");
	for( long x=1; x<iNumberOfPlayers+1; x++ )
	{
		strTemp.Format( _T("  Player: %d  PositionalScore: %0.2f\n"), x, m_CurrentForeignRelations.GetPositionalScore(x) );
		str += strTemp;
	}

	float fUnownedCitRes01 = 
		g_pEOSAICommonData->GetTotalUnownedCitResProductionOnMap() /
		g_pEOSAICommonData->GetTotalCitResProductionOnMap();
	strTemp.Format( _T("  PercentageOfunownedCitRes %0.2f\n"), fUnownedCitRes01 );
	str += strTemp;
	//strTemp.Format( "  TotalCityProductionOnMap %0.2f\n", m_fTotalCityProductionOnMap );
	//str += strTemp;
	//strTemp.Format( "  TotalResourcesOnMap %0.2f\n", m_fTotalResourcesOnMap );
	//str += strTemp;
	strTemp.Format( _T("  Strategy_CaptureUnownedCitRes01 %0.2f\n"), m_fStrategy_CaptureUnownedCitRes01 );
	str += strTemp;
	strTemp.Format( _T("  Strategy_CombatOrPlanForCombat01 %0.2f\n"), m_fStrategy_CombatOrPlanForCombat01 );
	str += strTemp;

	// CList< CEOSAIStrategicAIOrder* >  m_Orders; // Output: What should the AI do now?
	// CEOSAIBCDumbArray1D< CForeignRelationsStance* >  m_ForeignRelationsStance;

	// Show the NationalSummary2
//	CWorldDescServer* pWorldDescServer = GetAIPlayer()->GetWorldDescServer();
	for( long i=1; i<iNumberOfPlayers+1; i++ )
	{
		//CAINationalSummary2*  pSummary = pWorldDescServer->GetAICommonData()->GetAINationalSummary3( i );
		CEOSAINationalSummary3*  pSummary = g_pEOSAICommonData->GetAINationalSummary3( i );
		strTemp.Format( _T("NationalSummary %d\n"), i );
		str += strTemp;
		strTemp.Format( _T("  UnitProductionValue %0.2f\n"), pSummary->m_UnitSummary.GetProductionValue() );
		str += strTemp;
		strTemp.Format( _T("  CityProduction      %0.2f\n"), pSummary->m_CitResSummary.GetCityValue() );
		str += strTemp;
		strTemp.Format( _T("  ResourceProduction  %0.2f\n"), pSummary->m_CitResSummary.GetResValue() );
		str += strTemp;
		strTemp.Format( _T("  MaintenanceCosts    %0.2f\n"), pSummary->m_fMaintenanceCosts );
		str += strTemp;

		str += _T("  Accessibility\n");
		for( long j=0; j<iNumberOfPlayers+1; j++ )
		{
			if( i==j ) continue;
			strTemp.Format( _T("    Player %d: %0.2f\n"), j, pSummary->GetPlayerAccessibilty(j) );
			str += strTemp;
		}
	}

	return str;
}

void CEOSAIStrategicAI::InvokePlayers( long iNumberOfPlayers )
{
	//if( m_ForeignRelationsStance.m_iSize != iNumberOfPlayers+1 )
	if( m_LastMakePeaceMessage.m_iSize != iNumberOfPlayers+1 )
	{
		//m_ForeignRelationsStance.SetSize( iNumberOfPlayers+1 );
		m_LastMakePeaceMessage.SetSize( iNumberOfPlayers+1 );
		m_LastMaintainPeaceMessage.SetSize( iNumberOfPlayers+1 );

		//m_ForeignRelationsStance[0] = NULL;
		m_LastMakePeaceMessage[0] = -1000;
		m_LastMaintainPeaceMessage[0] = -1000;
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			//m_ForeignRelationsStance[iPlayer] = new CForeignRelationsStance();
			//m_ForeignRelationsStance[iPlayer]->m_iTowardsPlayer = iPlayer;
			m_LastMakePeaceMessage[iPlayer] = -1000;
			m_LastMaintainPeaceMessage[iPlayer] = -1000;
		}

		/*
		m_AINationalSummaries.SetSize( iNumberOfPlayers+1 );
		m_AINationalSummaries[0] = NULL;
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			m_AINationalSummaries[iPlayer] = new CAINationalSummary2();
			m_AINationalSummaries[iPlayer]->SetPlayer( iPlayer );
		}
		*/

		// Setup the current Foreign Relations state
		m_CurrentForeignRelations.SetNumberOfPlayers( iNumberOfPlayers );
	}
	m_CurrentForeignRelations.SetStrategicAI( this );
	m_CurrentForeignRelations.ClearValues();
}


//void CEOSAIStrategicAI::RunCalculationsCreateOrdersAndStances()
void CEOSAIStrategicAI::CreateForeignRelationsStances()
{
	long iAIPlayer = GetPlayerNumber();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	long iNumberOfActivePlayers = g_pEOSAIInterface->GetNumberOfActivePlayers();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CAICommonData* pAICommonData = GetCommonState()->GetWorldDescServer()->GetAICommonData();
	float fUnownedCitRes01 = 
		g_pEOSAICommonData->GetTotalUnownedCitResProductionOnMap() /
		g_pEOSAICommonData->GetTotalCitResProductionOnMap();

	float fAccessToUnownedCitRes = g_pEOSAICommonData->GetAINationalSummary3( iAIPlayer )->GetPlayerAccessibilty( 0 );
	float fCombatPotential01 = 0.0f;
	float fTotalCityProductionOnMap = g_pEOSAICommonData->GetTotalCityProductionOnMap();
	for( long iPlayer=1; iPlayer<iNumberOfPlayers; iPlayer++ )
	{
		if( iPlayer == iAIPlayer ) continue;
		EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
		if( pPlayer->IsAlive() == false ) continue;
		EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iAIPlayer, iPlayer );
		float fSignificance = 0.0f;
		if( eRel == EOSAIEnumForeignRelations::enum_War ){ fSignificance = 1.0f; }
		if( eRel == EOSAIEnumForeignRelations::enum_Neutral ){ fSignificance = 0.65f; }
		if( fSignificance > 0.0f )
		{
			float fAccess01 = g_pEOSAICommonData->GetAINationalSummary3( iPlayer )->GetPlayerAccessibilty( iAIPlayer );
			float fAccessSignificance01 = fAccess01*fSignificance;

			fCombatPotential01 = ( 1.0f-fCombatPotential01 ) * fAccessSignificance01;
			int g=0;
		}
	}

	m_fStrategy_CaptureUnownedCitRes01 = ( fUnownedCitRes01 * 1.4f ) - 0.1f;
	if( m_fStrategy_CaptureUnownedCitRes01 < 0.0f ) m_fStrategy_CaptureUnownedCitRes01 = 0.0f;
	if( m_fStrategy_CaptureUnownedCitRes01 > 1.0f ) m_fStrategy_CaptureUnownedCitRes01 = 1.0f;

	m_fStrategy_CombatOrPlanForCombat01 = 
		0.8f*( 1.0f - fUnownedCitRes01 ) + ( 0.5f*fCombatPotential01 ) - 0.1f;
	if( m_fStrategy_CombatOrPlanForCombat01 < 0.0f ) m_fStrategy_CombatOrPlanForCombat01 = 0.0f;
	if( m_fStrategy_CombatOrPlanForCombat01 > 1.0f ) m_fStrategy_CombatOrPlanForCombat01 = 1.0f;

	//m_CurrentForeignRelations.RunCalculations();
	{
		m_CurrentForeignRelations.ClearValues();

		// Compile data
		//

		// Create a fake event (for debug)
		/*
		if( GetCommonState()->GetWorldDescServer()->GetCurrentTurn() == 1 )
		{
			CEOSAIPlayerInteraction_DeclaredWar* pDeclaredWar = new CEOSAIPlayerInteraction_DeclaredWar();
			pDeclaredWar->m_iActor = 1;
			pDeclaredWar->m_iTarget = 2;
			pDeclaredWar->m_iEventTurn = 1;
			//GetCommonState()->GetWorldDescServer()->GetPlayerInteractions()->AddTail( pDeclaredWar );
		}
		*/
		m_CurrentForeignRelations.ApplyHistoricalEventsToFeelings();

		m_CurrentForeignRelations.CalculateExpectedUnownedExpansion();
		m_CurrentForeignRelations.CalculateGeneralWarDesire();
		m_CurrentForeignRelations.CalculatePlayerRelationsMatrix();
		m_CurrentForeignRelations.CalculateSATGlobalPower();

		// Create first-level analysis
		m_CurrentForeignRelations.CalculateRawWarDesire();
		m_CurrentForeignRelations.NormalizeWarDesires();
		//m_CurrentForeignRelations.CalculateFinalMakePeaceDesiresUsingPlayerMatrix();
		m_CurrentForeignRelations.CalculateFinalWarDesiresUsingPlayerMatrix();

		// Do I want to start a war, even if my existing wars are still going on?
		//   Check with the power balance (I need to be able to handle both enemies + have excess)

		// Situations:
		//   - I am currently at war with players (x,y)
		//     - Start an (additional) war with the player Z
		//     - Make peace with player x and/or y
		//     - Make peace with player x and/or y, and if peace: Start a war with the player Z
		//   - Not at war
		//     - Start a war with player Z
		//
		//   * Maintain peace with player Z (to prevent gang-ups)

		// Normalize desires, Create war/peace predictions for all players
		m_CurrentForeignRelations.CalculateRawPursueFriendlyRelations01();
		m_CurrentForeignRelations.CalculateCurrentStances(); // based on existing foreign relations (which can change)

		m_CurrentForeignRelations.CalculateDesireToConcedeGame();
	}
/*
	float fAIPlayerPower =
		40.0f * pAICommonData->GetTotalUnownedCitResProductionOnMap() / (float) iNumberOfActivePlayers +
		40.0f * pAICommonData->GetAINationalSummary3( iAIPlayer )->m_CitResSummary.GetCitResValue() +
				pAICommonData->GetAINationalSummary3( iAIPlayer )->m_UnitSummary.GetCombatSignificance();

	float fTotalGlobalPower = 0.0f;
	for( long iPlayer=0; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		float fPlayerPower = 
			40.0f * pAICommonData->GetAINationalSummary3( iPlayer )->m_CitResSummary.GetCitResValue() +
					pAICommonData->GetAINationalSummary3( iPlayer )->m_UnitSummary.GetCombatSignificance();

		fTotalGlobalPower += fPlayerPower;
	}
	float fAIPlayerPower01 = fAIPlayerPower / fTotalGlobalPower;
	float fAveragePlayerPower01 = fTotalGlobalPower / (float) iNumberOfActivePlayers;

	CEOSAIMathFunction  PowerRelative;
	MathFunc.SetInputOutput( 0.0f
	m_fDesireToSurrender = 

	float fSATGlobalPower = m_CurrentForeignRelations.m_SATGlobalPower.();

	// 0 to 1.  1 = no enemies, 0.5 = equal power with enemies, 0 = overwhelmed
	float fAIPlayerSATEPowerBalance = GetSATEPowerBalanceBasedOnMatrix01( iAIPlayer ); 
	if( 
*/


	CString strStrategicAI = OutputDebugString();
	CString strForeignRelations = m_CurrentForeignRelations.OutputDebugString();

	CString strTemp;
	strTemp.Format( _T("StrategicAI - Player = %d, Turn %d"), GetPlayerNumber(), g_pEOSAIInterface->GetCurrentTurn() );
//	Checkpoint::Write( strTemp );
//	Checkpoint::Write( strStrategicAI );
//	Checkpoint::Write( strForeignRelations );

	//
	#ifdef _DEBUG
	CString str1;
	// Personality
	str1 += _T("Personality\n");
	strTemp.Format( _T("HoldsGrudges01 %0.2f\n"), m_pAIPlayer->GetPersonality_HoldsGrudges01() ); str1 += strTemp;
	strTemp.Format( _T("Aggressive01 %0.2f\n"), m_pAIPlayer->GetPersonality_Aggressive01() ); str1 += strTemp;
	strTemp.Format( _T("Fortifier01 %0.2f\n"), m_pAIPlayer->GetPersonality_Fortifier01() ); str1 += strTemp;
	strTemp.Format( _T("Builder01 %0.2f\n"), m_pAIPlayer->GetPersonality_Builder01() ); str1 += strTemp;
	strTemp.Format( _T("ProResearcherPos1 %0.2f\n"), m_pAIPlayer->GetPersonality_ProResearcherPos1_AntiResearcherNeg1() ); str1 += strTemp;
	strTemp.Format( _T("ProSubmarine01 %0.2f\n"), m_pAIPlayer->GetPersonality_ProSubmarine01() ); str1 += strTemp;
	//strTemp.Format( _T("ProAircraftPos1 %0.2f\n"), m_pAIPlayer->GetPersonality_ProAircraftPos1_ProGroundSeaNeg1() ); str1 += strTemp;
	strTemp.Format( _T("ProAirUnit11 %0.2f\n"), m_pAIPlayer->GetPersonality_ProAirUnit11() ); str1 += strTemp;
	strTemp.Format( _T("ProSeaUnit11 %0.2f\n"), m_pAIPlayer->GetPersonality_ProSeaUnit11() ); str1 += strTemp;
	strTemp.Format( _T("ProGroundUnit11 %0.2f\n"), m_pAIPlayer->GetPersonality_ProGroundUnit11() ); str1 += strTemp;

	strTemp.Format( _T("ProStrategicBombing01 %0.2f\n"), m_pAIPlayer->GetPersonality_ProStrategicBombing01() ); str1 += strTemp;
	strTemp.Format( _T("ProMissile01 %0.2f\n"), m_pAIPlayer->GetPersonality_ProMissile01() ); str1 += strTemp;
	strTemp.Format( _T("ProArtillery01 %0.2f\n"), m_pAIPlayer->GetPersonality_ProArtillery01() ); str1 += strTemp;
	strTemp.Format( _T("ProExpensiveShipsPos1 %0.2f\n"), m_pAIPlayer->GetPersonality_ProExpensiveShipsPos1_CheapShipsNeg1() ); str1 += strTemp;
	str1 += _T("\n");
	str1 += _T("Foreign Relations\n");
	str1 += m_CurrentForeignRelations.OutputDebugString();
	str1.Replace( _T("\n"), _T("\r\n") );
	m_pAIPlayer->m_strDebug_ForeignRelations = str1;
	//CDebugDialogModal DebugDialog;
	//DebugDialog.Write( str1 );
	//DebugDialog.DoModal();
	#endif

	/*
	CDebugDialog* pDebugDialog = new CDebugDialog();
	pDebugDialog->Create( IDD_DEBUG_DIALOG );
	pDebugDialog->ShowWindow( SW_SHOW );
	CDebugDialog::Write( str1 );
	*/

	// Create Stances and Orders based on the m_CurrentForeignRelations
	//long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	//long iAIPlayer = m_pAIPlayer->GetPlayer()->GetPlayerNumber();

	// Intermediate layer
	//   The intermediate layer rates each player's desire to attack other players, and then
	//   it takes into account the player's power to figure out what one player each player
	//   wants to attack.  Note: this calculates the desire of human players to attack the AI,
	//   and that calculation is imprecise, so it should avoid coming up with a single predicted target.
	// What this layer does is figure out who to attack.  For example, if the RawWarDesire is:
	//   Player A: 0.8
	//   Player B: 0.7
	//   Player C: 0.5
	// The final output of the AI war desire should be:
	//   Player A: 1.0
	//   Player B: 0.0
	//   Player C: 0.0
	// It would be easy to simply pick the top player, but the calculation also needs to include
	//   other player's desires to attack him, as well.  For example, if Player B want to attack the AI,
	//   and Player A doesn't want to, then the AI should take that into account and the result will be:
	//   Player A: 0.0
	//   Player B: 1.0
	//   Player C: 0.0

//	Do I want to consider hypotheticals here?
//	How do I track the WarBalance unless I do some MinMax stuff?
//	Or do I just do a shallow calculation - I can 

/*
	Algorithm:
		(1) Pretend I have no enemies (except ones I can't make peace with), figure out who I want to attack
			(b) If I anticipate an attack, hold back a little bit on making new enemies, do more
				planning rather than jumping into war (this will avoid making enemies while building
				the military)
		(2) If there are enemies I don't want to be at war with, try making peace
			(b) If I can't make peace (either assumed or after a response), lock the "war" state 
				and consider adding new enemies
		(3) If there are players I want to be at war with, begin OffensiveWar/PlanInvasion, etc
*/

/*
	long iCurrentPlayer = GetPlayerNumber();
	//float fPositionalScore = m_CurrentForeignRelations.GetPositionalScore( iCurrentPlayer );

	// Who would I most like to attack?

	CEOSAIForeignRelationsState  ForeignState;
	ForeignState.Copy( &m_CurrentForeignRelations );
	ForeignState.CalculatePlayerRelationsMatrix();
	float fGeneralWarDesire = ForeignState.GetGeneralDesireForWar01( iCurrentPlayer );
	float fWarBalance = ForeignState.GetSATEPowerBalanceBasedOnMatrix01( iCurrentPlayer );
	long iNumberOfEnemies = ForeignState.GetNumberOfDirectEnemies( iCurrentPlayer );
	long iPreferedTarget = ForeignState.GetPlayerIWouldMostLikeToAttackBasedOnRawWarDesire( iCurrentPlayer );
	float fPositionalScore = ForeignState.GetPositionalScore( iCurrentPlayer );
	// Run some hypotheticals - 
	//   What if I added a war against my prefered target
	//   What if I made peace with each enemy

	//ForeignState.m_ForeignRelations.Value( 1,2 ) = EOSAIEnumForeignRelations::enum_War;
	//ForeignState.m_ForeignRelations.Value( 2,1 ) = EOSAIEnumForeignRelations::enum_War;
	CEOSAIPlayerInteraction_DeclaredWar  DeclaredWar( 1,2 );
	DeclaredWar.UpdateForeignRelationsState( 1,&ForeignState.m_ForeignRelations,&ForeignState.m_Feelings01 );

	ForeignState.CalculatePlayerRelationsMatrix();
	float fPositionalScore2 = ForeignState.GetPositionalScore( iCurrentPlayer );

	int x=0;

	//
	CEOSAIBCDumbArray1D< float >  PlayerAtWarBalance; // based on the 'PlayerAtWar' values
	CEOSAIBCDumbArray2D< bool >   PlayerAtWar;
	CEOSAIBCDumbArray2D< float >  PlayerWarDesire;
	PlayerAtWarBalance.SetSize( iNumberOfPlayers+1 );
	PlayerAtWar.SetSize( iNumberOfPlayers+1,iNumberOfPlayers+1 );
	PlayerWarDesire.SetSize( iNumberOfPlayers+1,iNumberOfPlayers+1 );
	for( long x=0; x<iNumberOfPlayers+1; x++ )
	{
		PlayerAtWarBalance.Value( x ) = 0.0f;
		for( long y=0; y<iNumberOfPlayers+1; y++ )
		{
			PlayerAtWar.Value( x,y ) = false;
			PlayerWarDesire.Value( x,y ) = 0.0f;
		}
	}

	// What is the AIPlayer's current state? Consider some hypotheticals?

	float fMaxReasearchPointsSpent = 0.0f;
	for( long iPlayer=1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		CPlayer* pPlayer = GetCommonState()->GetPlayer( iPlayer );
		if( pPlayer->GetPlayerHasBeenEliminated() ) continue;

		if( iAIPlayer != iPlayer )
		{
			m_ForeignRelationsStance[iPlayer]->m_fPowerDesire = 0.0f;
			if( m_CurrentForeignRelations.m_ForeignRelations.Value( iAIPlayer, iPlayer ) == EOSAIEnumForeignRelations::enum_Teammate )
			{
				m_ForeignRelationsStance[iPlayer]->m_fPowerDesire = 1.0f;
			}
			if( m_CurrentForeignRelations.m_ForeignRelations.Value( iAIPlayer, iPlayer ) == EOSAIEnumForeignRelations::enum_War )
			{
				m_ForeignRelationsStance[iPlayer]->m_fPowerDesire = -1.0f;
			}

			float fFriendly = m_CurrentForeignRelations.m_PursueFriendlyRelations.Value( iAIPlayer, iPlayer );
			float fRawWarDesire = m_CurrentForeignRelations.m_RawWarDesire.Value( iAIPlayer, iPlayer );
			//
			// Need to convert to 01 values
			m_ForeignRelationsStance[iPlayer]->m_fFriendly01 = CEOSAIMath::Sigma01( 0.0f, 1.0f, fFriendly );
			m_ForeignRelationsStance[iPlayer]->m_fOffensiveWar01 = CEOSAIMath::Sigma01( 0.0f, 1.0f, fRawWarDesire );
		}
	}
*/
	//CalculateWhichTechnologyToResearchAndSpending();

	long j1=0;
	long j2=0;
	long j3=0;

	// Current Foreign Relations
	//CAIForeignRelationsHypotheticalState  m_CurrentForeignRelations;
}

void CEOSAIStrategicAI::MakeWarDeclarationsAndPeace()
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	//long iAIPlayer = m_pAIPlayer->GetPlayer()->GetPlayerNumber();
	long iAIPlayer = m_pAIPlayer->GetPlayerNumber();
	long iRand = m_pAIPlayer->GetRand();
	long iCurrentTurn = g_pEOSAIInterface->GetCurrentTurn();

	//
	if( m_CurrentForeignRelations.GetDesireToConcedeGame() )//m_fDesireToConcedeGame > 1.0f )
	{
		//ASSERT( GetCommonState()->GetNumberOfActivePlayers() == 2 );

		long iOtherPlayer = 0;
		//POSITION pos = GetCommonState()->GetWorldBuildDesc()->GetPlayerList()->GetHeadPosition();
		//while( pos )
		for( long iPlayer=1; iPlayer<g_pEOSAICommonData->GetNumberOfPlayers(); iPlayer++ )
		{
			//CPlayer* pPlayer = GetCommonState()->GetWorldBuildDesc()->GetPlayerList()->GetNext( pos );
			EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer(iPlayer);
			ASSERT( pPlayer );
			if( pPlayer->IsAlive() &&
				pPlayer->GetPlayerNumber() != iAIPlayer )
			{
				iOtherPlayer = pPlayer->GetPlayerNumber();
			}
		}

		long iTurnsSinceLastMessage = CEOSAIStrategicAIOrder_AIOffersToResign::HowManyTurnsAgoDidILastSendThisMessage( this );
		if( iOtherPlayer != 0 &&
			iTurnsSinceLastMessage > 10+(rand()%8) )
		{
			CEOSAIStrategicAIOrder_AIOffersToResign* pConcedeGameMessage = new CEOSAIStrategicAIOrder_AIOffersToResign( this );
			pConcedeGameMessage->SurrenderToPlayer( iOtherPlayer );
			//pConcedeGameMessage->Execute( GetWorldDescServer()->GetCurrentTurn() );
			pConcedeGameMessage->Execute( g_pEOSAIInterface->GetCurrentTurn() );
			m_StrategicAIOrders.AddTail( pConcedeGameMessage );
		}
	}

	for( long iOtherPlayer=1; iOtherPlayer<iNumberOfPlayers+1; iOtherPlayer++ )
	{
		if( iAIPlayer == iOtherPlayer ) continue;
		EOSAI::CGamePlayer* pOtherPlayer = g_pEOSAIInterface->GetGamePlayer( iOtherPlayer );
		ASSERT( pOtherPlayer );

		//
		CEOSAIForeignRelationsState::EnumRelationsPlan ePlan = m_CurrentForeignRelations.m_RelationsPlan.Value( iAIPlayer,iOtherPlayer );
/*
#ifdef _DEBUG
		ePlan = CEOSAIForeignRelationsState::enum_MakePeace;
#endif
*/
/*
#ifdef _DEBUG
		ePlan = CEOSAIForeignRelationsState::enum_MakePeace;
#endif
*/
		//ePlan = CEOSAIForeignRelationsState::enum_MakePeace; // 3409834
		if( ePlan == CEOSAIForeignRelationsState::enum_StartWar )
		{
			ASSERT( m_pAIPlayer->GetGamePlayer()->m_bScenarioPlayerRestriction_CannotDeclareWar == false );

			// Send an imail message
			/*
			I don't want to send a global message to everyone. The game might have a particular way of handling this - i.e. the
				game might only tell the person getting the declaration knows about it.

			ASSERT( false );
			#ifdef GAME_CODE
			The code below is specific to the game. I need to construct an EOSAI message that get recieved by the game, which then
				triggers a 'declaration of war' message. Otherwise, the AI is too closely tied to the game code.

			CString strMessage;
			strMessage.Format( Lang( 82, _T("%s has declared war on %s.") ), 
				m_pAIPlayer->GetPlayer()->GetPlayerName(), 
				pOtherPlayer->GetPlayerName() );

			CEOSAIIntSet SendTo; 
			SendTo.AddRange( 1,g_pEOSAICommonData->GetNumberOfPlayers() );
			SendTo.Remove( m_pAIPlayer->GetPlayer()->GetPlayerNumber() );

			CGameEvent_IMail* pIMail = new CGameEvent_IMail();
			pIMail->SetSender( m_pAIPlayer->GetPlayer()->GetPlayerNumber() );
			pIMail->SetSendTo( SendTo );
			//pIMail->SetSubject( "Declaration of War" );
			pIMail->SetSubject( "Declaration of War against " + pOtherPlayer->GetPlayerName() );
			pIMail->SetMessage( strMessage );//"[Blank] has declared war on [Blank]" );
			pIMail->SendFromPlayerToServer();
			#endif
			*/

			// I'm going to immediately change the server foreign-relations
			//   otherwise, I could run into a timing issue
			//g_pCommonState->GetWorldDescServer()->SetForeignRelations( iOtherPlayer, this->GetPlayerNumber(), EOSAIEnumForeignRelations::enum_War );
			//g_pCommonState->GetWorldDescServer()->SetForeignRelations( this->GetPlayerNumber(), iOtherPlayer, EOSAIEnumForeignRelations::enum_War );
			g_pEOSAICommonData->SetForeignRelations( iOtherPlayer, this->GetPlayerNumber(), EOSAIEnumForeignRelations::enum_War );
			g_pEOSAICommonData->SetForeignRelations( this->GetPlayerNumber(), iOtherPlayer, EOSAIEnumForeignRelations::enum_War );

			// Declare war (I could also do a sneak attack)
			CEOSAIStrategicAIOrder_DeclareWar* pDeclareWar = new CEOSAIStrategicAIOrder_DeclareWar( this );
			//DeclarationMessage.SendToServer();//SendToAll();
			//pDeclareWar->SetSender( m_pAIPlayer->GetPlayerNumber() );
			//pDeclareWar->m_iTargetPlayer = iOtherPlayer;
			pDeclareWar->SetTarget( iOtherPlayer );
			//this->AddAIAction( pDeclareWar );
			pDeclareWar->Execute( g_pEOSAIInterface->GetCurrentTurn() );
			m_StrategicAIOrders.AddTail( pDeclareWar );
			/*
			CMessage2_DeclareWarMessage  DeclarationMessage;
			DeclarationMessage.SendToServer();//SendToAll();
			DeclarationMessage.SetSenderId( m_pAIPlayer->GetPlayer()->GetMessageTargetId() );
			DeclarationMessage.m_iTargetPlayer = iOtherPlayer;
			g_pMessageManager->Send( DeclarationMessage );
			*/
			// Do I send a declaration-of-war message to the server, or does the StrategicOrder take care of this?
		}
		if( ePlan == CEOSAIForeignRelationsState::enum_MakePeace )
		{
			ASSERT( g_pEOSAICommonData->GetAllPlayersPermanentlyAtWar() == false );
			ASSERT( m_pAIPlayer->GetGamePlayer()->m_bScenarioPlayerRestriction_CannotMakePeace == false );

			long iTimeSinceLastMessage = iCurrentTurn - GetMostRecentMakePeaceMessage( iOtherPlayer );
			//long iWarDuration = GetCommonState()->GetWorldDescServer()->GetPlayerInteraction_WarDuration( GetPlayerNumber(), iOtherPlayer );
			long iWarDuration = g_pEOSAICommonData->GetPlayerInteraction_WarDuration( GetPlayerNumber(), iOtherPlayer );
/*
#ifdef _DEBUG
			// 987987
			iTimeSinceLastMessage = 10;
			iWarDuration = 10;
#endif _DEBUG
*/
			if( iTimeSinceLastMessage >= 3 + (iRand%3) &&
			 	iWarDuration          >= 3 + (iRand%3) ) // every 3-5 turns
			{
				// Declare war (I could also do a sneak attack)
				/*
				CMessage2_DeclareWarMessage  DeclarationMessage;
				DeclarationMessage.SendToAll();
				DeclarationMessage.SetSenderId( m_pAIPlayer->GetPlayer()->GetMessageTargetId() );
				DeclarationMessage.m_iTargetPlayer = iOtherPlayer;
				g_pMessageManager->Send( DeclarationMessage );
				*/
				ASSERT( false );
				#ifdef GAME_CODE
				CString strMessage;
				if( iWarDuration <= 10 )
				{
					strMessage = Lang( 83, _T("We should sign a peace agreement.") );
				}
				else if( iWarDuration <= 20 )
				{
					strMessage = Lang( 84, _T("We should sign a peace agreement.  This war has gone on for too long.") );
				}
				else if( iWarDuration > 20 )
				{
					strMessage = Lang( 85, _T("We should sign a peace agreement.  There is nothing to be gained from perpetual war.") );
				}

				//CEOSAIStrategicAIOrder_OfferPeaceTreaty  OfferPeaceTreaty( this );
				//OfferPeaceTreaty.SetTargetPlayer( iOtherPlayer );
				CEOSAIStrategicAIOrder_OfferPeaceTreaty* pSuggestPeace = new CEOSAIStrategicAIOrder_OfferPeaceTreaty( this );
				pSuggestPeace->SetTargetPlayer( iOtherPlayer );
				pSuggestPeace->SetIMailMessage( strMessage );
				pSuggestPeace->Execute( g_pEOSAICommonData->GetCurrentTurn() );
				m_StrategicAIOrders.AddTail( pSuggestPeace );

				SetMostRecentMakePeaceMessage( iOtherPlayer, iCurrentTurn );
				#endif GAME_CODE
				CEOSAIStrategicAIOrder_OfferPeaceTreaty* pSuggestPeace = new CEOSAIStrategicAIOrder_OfferPeaceTreaty( this );
				pSuggestPeace->SetTargetPlayer( iOtherPlayer );
				pSuggestPeace->Execute( g_pEOSAIInterface->GetCurrentTurn() );
				m_StrategicAIOrders.AddTail( pSuggestPeace );

				SetMostRecentMakePeaceMessage( iOtherPlayer, iCurrentTurn );
			}
		}
		if( ePlan == CEOSAIForeignRelationsState::enum_MaintainPeace )
		{
			long iDeltaTime = iCurrentTurn - GetMostRecentMaintainPeaceMessage( iOtherPlayer );
			if( iDeltaTime >= 5 + (iRand%6) ) // every 5-10 turns
			{
				ASSERT( false );

				CEOSAIStrategicAIOrder_SendReinforcePeaceMail* pPeaceMail = new CEOSAIStrategicAIOrder_SendReinforcePeaceMail( this );
				//pPeaceMail->SetSendFrom( m_pAIPlayer->GetPlayerNumber() );
				pPeaceMail->SendTo( iOtherPlayer );
				this->AddStrategicAIOrder( pPeaceMail );
				/*
				// Try to shmoze (want to track my past imail messages - don't send one every turn)
				CString strMessage;
				strMessage = Lang( 86, _T("Our continued peace has been a benefit to our people, and our nations.") );
				//strMessage.Format( "%s has declared war on %s.", 
				//	m_pAIPlayer->GetPlayer()->GetPlayerName(), 
				//	pOtherPlayer->GetPlayerName() );

				CGameEvent_IMail* pIMail = new CGameEvent_IMail();
				pIMail->SetSender( m_pAIPlayer->GetPlayer()->GetPlayerNumber() );
				pIMail->AddSendTo( iOtherPlayer );
				pIMail->SetSubject( Lang( 87, _T("Continued Peace") ) );
				pIMail->SetMessage( strMessage );//"[Blank] has declared war on [Blank]" );
				pIMail->SendFromPlayerToServer();
				*/

				SetMostRecentMaintainPeaceMessage( iOtherPlayer, iCurrentTurn );
			}
		}
	}
}

void CEOSAIStrategicAI::SetMostRecentMakePeaceMessage( long iOtherPlayer, long iTime )
{
	m_LastMakePeaceMessage[ iOtherPlayer ] = iTime;
}

long CEOSAIStrategicAI::GetMostRecentMakePeaceMessage( long iOtherPlayer )
{
	return m_LastMakePeaceMessage[ iOtherPlayer ];
}

void CEOSAIStrategicAI::SetMostRecentMaintainPeaceMessage( long iOtherPlayer, long iTime )
{
	m_LastMakePeaceMessage[ iOtherPlayer ] = iTime;
}

long CEOSAIStrategicAI::GetMostRecentMaintainPeaceMessage( long iOtherPlayer )
{
	return m_LastMakePeaceMessage[ iOtherPlayer ];
}

void CEOSAIStrategicAI::CalculateTechnologyStrategy()
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	long iAIPlayer = m_pAIPlayer->GetPlayerNumber();

	CEOSAIBCDumbArray1D< float >  ResearchDone;
	CEOSAIBCDumbArray1D< float >  ResearchPointsPerTurn;

	//
	// The algorithm does this:
	//    For each player, look at the total research points spent, and current spending
	//    Look at the player's ForeignRelations with me
	//    Figure out how much the player will have researched in X turns, and try to keep up 
	//       with the player's research, especially if the player is an enemy
	//    The AIPlayer's cash excess/shortage also plays a role
	//
	ResearchDone.SetSize( iNumberOfPlayers+1 );
	ResearchPointsPerTurn.SetSize( iNumberOfPlayers+1 );

	//float fMaxReasearchPointsSpent = 0.0f;
	//float fMaxReasearchDone = 0.0f;
	for( long iPlayer=1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
		//if( pPlayer->GetPlayerHasBeenEliminated() ) continue;
		if( pPlayer->m_bIsAlive == false ) continue;

		// Technology
		//CWorldDescPlayerProxy* pOtherPlayerWorldDescPlayerProxy = GetWorldDescServer()->GetWorldDescPlayerProxy( iPlayer );
		float fResearchDone = g_pEOSAICommonData->GetAINationalSummary3( iPlayer )->GetDiscoveredTechnologyResearchPoints();
		float fResearchPointsPerTurn = g_pEOSAICommonData->GetAINationalSummary3( iPlayer )->GetResearchPointsProducedThisTurn();
		ResearchDone[iPlayer] = fResearchDone;
		//
		//CWorldDescPlayerProxy* pProxy = m_pAIPlayer->GetWorldDescPlayerProxy();//GetCommonState()->GetWorldDescServer()->GetWorldDescPlayerProxy( iPlayer );
		//if( pProxy )
		{
			//float fResearchPointsPerTurn = pProxy->ResearchPointsProducedThisTurn();
			ResearchPointsPerTurn[iPlayer] = fResearchPointsPerTurn;
		}
	}

	m_fMoneyToSpendOnResearch = 0.0f;
	//
	//if( m_pAIPlayer->GetWorldDescPlayerProxy()->AllTechnologiesHaveBeenDiscovered() == false )
	if( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->AllTechnologiesHaveBeenDiscovered() == false )
	{
		// Reduce spending based on money shortages (multiply the research money)
		CEOSAIMathFunction ReducedSpending;
		ReducedSpending.SetInputOutput(    0.0f,0.0f );
		ReducedSpending.SetInputOutput(   25.0f,0.0f );
		ReducedSpending.SetInputOutput(   75.0f,1.0f );
		// Increase spending based on money excess (add to research money)
		CEOSAIMathFunction AdditionalSpending;
		AdditionalSpending.SetInputOutput(  100.0f, 0.0f );
		AdditionalSpending.SetInputOutput(  500.0f, 4.0f );
		AdditionalSpending.SetInputOutput( 5000.0f,10.0f );
		//float fCashAvailability = MathFunc.GetOutput( GetWorldDescPlayer()->GetTotalMoney() );
		float fReducedSpendingBasedOnCashAvailability    = ReducedSpending.GetOutput( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalMoney() );
		float fAdditionalSpendingBasedOnCashAvailability = AdditionalSpending.GetOutput( g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalMoney() );

		//
		float fTotalResearchSpending = 0.0f;
		long  iTotalResearchSpendingCount = 0;
		float fAIPlayerResearchDone = ResearchDone[iAIPlayer];
		float fAIPlayerResearchPointsPerTurn = ResearchPointsPerTurn[iAIPlayer];
		float fNeededResearchPointsPerTurn1 = 1.0f;
		for( long iPlayer=1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
		{
			if( iPlayer == iAIPlayer ) continue;
			EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
			if( pPlayer->IsAlive() == false ) continue;

			// Calculate the ResearchPoints Delta
		//	ResearchPointsSpent[iPlayer] = ResearchPointsSpent[iPlayer];// - fAIPlayerResearchPointsSpent;
		//	ResearchPointsPerTurn[iPlayer] = ResearchPointsPerTurn[iPlayer];// - fAIPlayerResearchPointsPerTurn;
			//float f1 = ResearchPointsSpent[iPlayer];
			//float f2 = ResearchPointsPerTurn[iPlayer];

			// The money to spend on research depends on several factors:
			//   1. What's the tech spending of neutral and enemy players (allies matters, but not as much)
			//   2. How much money does the player have to spend?  If there is an excess of cash, then
			//      spend more, if cash is short, then spend less

			EOSAIEnumForeignRelations eForeignRelations = m_CurrentForeignRelations.m_ForeignRelations.Value( iAIPlayer, iPlayer );
			if( eForeignRelations <= EOSAIEnumForeignRelations::enum_Alliance )
			{
				float fTargetTime = 50.0f;
				if( eForeignRelations == EOSAIEnumForeignRelations::enum_Alliance )
				{
					fTargetTime = 100.0f;
				}

				//fTotalResearchSpending += GetCommonState()->GetActiveUnitset()->GetSpendingRequiredForXResearchPoints( ResearchPointsPerTurn[iPlayer] );
				fTotalResearchSpending += g_pEOSAIInterface->GetAIGameRules()->GetSpendingRequiredForXResearchPoints( ResearchPointsPerTurn[iPlayer] );
				iTotalResearchSpendingCount++;

				float fPlayerResearchPointsPerTurn = ResearchPointsPerTurn[iPlayer];
				float fPlayerResearchPointsInXTurns = ResearchDone[iPlayer] + fTargetTime*fPlayerResearchPointsPerTurn;
				float fNeededResearchPointsPerTurn2 = (fPlayerResearchPointsInXTurns - ResearchDone[iAIPlayer])/fTargetTime;
				if( fNeededResearchPointsPerTurn2 < 0.0f ) fNeededResearchPointsPerTurn2 = 0.0f;

				// I currently have a tech advantage over the other player
				if( ResearchDone[iAIPlayer] > ResearchDone[iPlayer] )
				{
					// Try to keep the advantage - match at least 80% of his RP growth
					fNeededResearchPointsPerTurn2 = max( fNeededResearchPointsPerTurn2, 0.8f*ResearchPointsPerTurn[iPlayer] );
				}
				fNeededResearchPointsPerTurn1 = max( fNeededResearchPointsPerTurn1, fNeededResearchPointsPerTurn2 );
				// Adjust these numbers using the fCashAvailability
				//fNeedResearchPointsPerTurn *= fCashAvailability;
			}
		}

		// Increase/Decrease RP based on AIPlayer's money shortage/excess
		// How much money do I need to spend for this number of ResearchPoints?
		//float fOtherPlayerBasedSpendingRequiredForRP = GetCommonState()->GetActiveUnitset()->GetSpendingRequiredForXResearchPoints( fNeededResearchPointsPerTurn1 );
		float fOtherPlayerBasedSpendingRequiredForRP = (float) g_pEOSAIInterface->GetAIGameRules()->GetSpendingRequiredForXResearchPoints( fNeededResearchPointsPerTurn1 );

		//float fSpendingRequiredForRP2 = GetCommonState()->GetActiveUnitset()->GetSpendingRequiredForXResearchPoints( 
		float fIndependentSpendingRequiredForRP2 = 0.7f + 3.5f*m_pAIPlayer->GetPersonality_ProResearcherPos1_AntiResearcherNeg1();
		if( fIndependentSpendingRequiredForRP2 < 0.0f ) fIndependentSpendingRequiredForRP2 = 0.0f;

		float fAverageRPSpending = 0.0f;
		if( iTotalResearchSpendingCount > 0.0f )
		{
			fAverageRPSpending = fTotalResearchSpending / iTotalResearchSpendingCount;
		}

		float fRPSpending = 
			0.2f * max( fOtherPlayerBasedSpendingRequiredForRP, fIndependentSpendingRequiredForRP2 ) +
			0.6f * fIndependentSpendingRequiredForRP2 + 
			0.2f * fAverageRPSpending;

		// Decrease RP based on spending (to prevent run-away spending)
		CEOSAIMathFunction ReWriteSpending;
		ReWriteSpending.SetInputOutput(  0.0f, 0.0f );
		ReWriteSpending.SetInputOutput(  4.0f, 4.0f );
		ReWriteSpending.SetInputOutput(  6.0f, 5.5f );
		ReWriteSpending.SetInputOutput(  8.0f, 7.0f );
		ReWriteSpending.SetInputOutput( 10.0f, 9.0f );
		ReWriteSpending.SetInputOutput( 15.0f,12.0f );
		ReWriteSpending.SetInputOutput( 20.0f,15.0f );
		fRPSpending = ReWriteSpending.GetOutput( fRPSpending );

		// Increase/Decrease RP based on personality
		CEOSAIMathFunction ResearcherPersonalityMultiplier;
		ResearcherPersonalityMultiplier.SetInputOutput( -1.0f, 0.0f );
		ResearcherPersonalityMultiplier.SetInputOutput(  0.0f, 1.0f );
		ResearcherPersonalityMultiplier.SetInputOutput(  1.0f, 1.5f );
		fRPSpending *= ResearcherPersonalityMultiplier.GetOutput( m_pAIPlayer->GetPersonality_ProResearcherPos1_AntiResearcherNeg1() );
		//fSpendingRequiredForRP += 2.0f * m_pAIPlayer->GetPersonality_ProResearcherPos1_AntiResearcherNeg1();
		if( fRPSpending < 0.0f ){ fRPSpending = 0.0f; }

		//fSpendingRequiredForRP *= fCashAvailability;
		//fSpendingRequiredForRP += (fSpendingRequiredForRP+0.5f)*fCashAvailability;
		fRPSpending *= fReducedSpendingBasedOnCashAvailability;
		fRPSpending += fAdditionalSpendingBasedOnCashAvailability;

		// Don't overspend
		if( fRPSpending > g_pEOSAICommonData->GetAINationalSummary3( m_pAIPlayer->GetPlayerNumber() )->GetTotalMoney()/5.0f )
		{
			//fRPSpending = GetWorldDescPlayerProxy()->GetTotalMoney()/5.0f;
			fRPSpending = g_pEOSAICommonData->GetAINationalSummary3( m_pAIPlayer->GetPlayerNumber() )->GetTotalMoney()/5.0f;
		}
		m_fMoneyToSpendOnResearch = max( m_fMoneyToSpendOnResearch, fRPSpending );

		//long iMoneyToSpend = CEOSAIMath::RoundUp( m_fMoneyToSpendOnResearch );
		long iMoneyToSpend = EOSAIRoundToNearest( m_fMoneyToSpendOnResearch );
		//GetWorldDescPlayerProxy()->SetMoneyBeingSpentOnResearch( iMoneyToSpend );
		m_pAIPlayer->GetStrategicAI()->SetMoneyBeingSpentOnResearch( iMoneyToSpend );

		// Decide what technology to research
		CList< CEOSAITechnologyDesc* >  NearbyAppliedTech;
		//GetWorldDescPlayerProxy()->GetNearbyAppliedTechnologies( &NearbyAppliedTech );
		m_pAIPlayer->GetStrategicAI()->GetNearbyAppliedTechnologies( &NearbyAppliedTech );
		int g=0;

		CEOSAITechnologyDesc* pBestTech = NULL;
		float fBestTechValue = 0.0f;
		POSITION pos = NearbyAppliedTech.GetHeadPosition();
		while( pos )
		{
			CEOSAITechnologyDesc* pTechDesc = NearbyAppliedTech.GetNext( pos );
			float fTechValue = GetTechValue( pTechDesc );
			if( pBestTech == NULL || fTechValue > fBestTechValue )
			{
				pBestTech = pTechDesc;
				fBestTechValue = fTechValue;
			}
		}
		if( pBestTech )
		{
			// Evaluate the technologies based on benefits + cost
			//   Ideally, this would choose technologies based on the current situation
			//   (example: if the AI is in a ground war, then sea units aren't very valuable)
			//GetWorldDescPlayerProxy()->ClearTechnologyResearchQueue();
			//GetWorldDescPlayerProxy()->AddTechnologyAndPrerequisitesToResearchQueue( pBestTech->GetInternalName() );
			ClearTechnologyResearchQueue();
			AddTechnologyAndPrerequisitesToResearchQueue( pBestTech->GetInternalName() );
		}
	}
	else
	{
		//GetWorldDescPlayerProxy()->SetMoneyBeingSpentOnResearch( 0 );
		//GetWorldDescPlayerProxy()->ClearTechnologyResearchQueue();
		SetMoneyBeingSpentOnResearch( 0 );
		ClearTechnologyResearchQueue();
	}
	//float fBaseResearchPointsPerTurn = GetCommonState()->GetActiveUnitset()->GetResearchPoints( 0 );
	int g=0;
}

void CEOSAIStrategicAI::SetMoneyBeingSpentOnResearch( long iMoney ) // The AI was calling WorldDescProxy directly
{
	m_fMoneyToSpendOnResearch = (float) iMoney;
}

void  CEOSAIStrategicAI::ClearTechnologyResearchQueue() // The AI was calling WorldDescProxy directly
{
	m_TechnologyToResearch.RemoveAll();
	//ASSERT( false );
}

void CEOSAIStrategicAI::AddTechnologyAndPrerequisitesToResearchQueue( CString strTechnologyName ) // The AI was calling WorldDescProxy directly
{
	m_TechnologyToResearch.AddTail( strTechnologyName );
}

bool CEOSAIStrategicAI::GetTechnologyHasBeenDiscovered( CString strTech )
{
	long iPlayer = m_pAIPlayer->GetPlayerNumber();
	CEOSAINationalSummary3* pNationalSummary = g_pEOSAIInterface->GetAICommonData()->GetAINationalSummary3( iPlayer );
	return pNationalSummary->GetTechnologyHasBeenDiscovered( strTech );
}

float CEOSAIStrategicAI::GetResearchPointsUntilCompletion( CString strTechName )
{
	#ifdef THINGS_TO_COMPILE_EVENTUALLY
	We need a way to track what technologies have been partially researched.
	The 'technology hint' stuff I wrote can be mimiced by making some 'partially researched' values for those tech.
	POSITION pos = m_PartiallyCompletedTechnologyList.GetHeadPosition();
	while( pos )
	{
		CTechnologyResearchLevel* pResearchLevel = m_PartiallyCompletedTechnologyList.GetNext( pos );
		if( pResearchLevel->m_strTechnologyName == strTechName )
		{
			float fDelta = pResearchLevel->m_fResearchNeededToComplete - pResearchLevel->m_fResearchPointsSpent;
			return fDelta;
		}
	}
	#endif THINGS_TO_COMPILE_EVENTUALLY
/*
	CTechnologyResearchLevel* pResearchLevel = InvokePartiallyCompletedTechnology( strTechName );
	if( pResearchLevel )
	{
		if( pResearchLevel->m_bHasFinishedResearch ) return 0.0f;
		float fDelta = pResearchLevel->m_fResearchNeededToComplete - pResearchLevel->m_fResearchPointsSpent;
		return fDelta;
	}
	*/
/*
	POSITION pos = m_PartiallyCompletedTechnologyList.GetHeadPosition();
	while( pos )
	{
		CTechnologyResearchLevel* pResearchLevel = m_PartiallyCompletedTechnologyList.GetNext( pos );
		if( pResearchLevel->m_strTechnologyName == strTechName )
		{
			if( pResearchLevel->m_bHasFinishedResearch ) return 0.0f;
			float fDelta = pResearchLevel->m_fResearchNeededToComplete - pResearchLevel->m_fResearchPointsSpent;
			return fDelta;
		}
	}
*/
	CEOSAITechnologyDesc* pTechDesc = g_pEOSAIInterface->GetAIGameRules()->GetTechnologyDesc( strTechName );
	if( pTechDesc )
	{
		return (float) pTechDesc->GetCost();
	}
	ASSERT( false ); // this shouldn't happen
	return 0.0f;
	//ASSERT( false );
	//return 0.0f;
}

void CEOSAIStrategicAI::GetNearbyAppliedTechnologies( CList< CEOSAITechnologyDesc* >* pNearbyAppliedTech )
{
	pNearbyAppliedTech->RemoveAll();

	int iPass = 0;

	long iPlayer = m_pAIPlayer->GetPlayerNumber();
	//long iPlayer = GetLocalPlayerNumber();
	//CUnitset* pUnitset = GetCommonState()->GetActiveUnitset();
	EOSAI::CGameRules* pGameRules = g_pEOSAIInterface->GetAIGameRules();
	POSITION pos = pGameRules->GetTechnologyDescsList()->GetHeadPosition();
	while( pos )
	{
		CEOSAITechnologyDesc* pCurrentTechDesc = pGameRules->GetTechnologyDescsList()->GetNext( pos );
		iPass++;

		//if( pCurrentTechDesc->GetInternalName() == _T("Pesticides") )
		//{
		//	int g=0;
		//}

		if( pCurrentTechDesc->EnablesBuildingOrUnitOrNationalImprovement() && //EnablesBuildingOrUnit() &&
			GetTechnologyHasBeenDiscovered( pCurrentTechDesc->GetInternalName() ) == false )
			//pCurrentTechDesc->GetHasBeenDiscovered_CheckWorldDescPlayer( iPlayer ) == false )
		{
			if( pGameRules->GetTechTreeNode( pCurrentTechDesc->GetInternalName() ) == false )
			{
				continue; // if it's not in the TechTree, then don't add it
			}

			// If the technology enables a building or unit, AND
			// If the technology hasn't been discovered, lookup the prerequisites.
			//   If there are applied technologies as prerequisites, then don't add it to the list
			//   (not used) If there are 2 or fewer undiscovered prerequisites, put it in the list
			CStringList  PrereqTechList;
			pCurrentTechDesc->GetPrereqTechList( &PrereqTechList );

			bool bAddThisTechnology = true;
			POSITION pos2 = PrereqTechList.GetHeadPosition();
			while( pos2 )
			{
				CString strPrereqTech = PrereqTechList.GetNext( pos2 );
				CEOSAITechnologyDesc* pPrereqTechDesc = pGameRules->GetTechnologyDesc( strPrereqTech );
				if( GetTechnologyHasBeenDiscovered( pPrereqTechDesc->GetInternalName() ) == false &&
					//pPrereqTechDesc->GetHasBeenDiscovered_CheckWorldDescPlayer( iPlayer ) == false &&
					pPrereqTechDesc->IsBasicResearch() == false )
				{
					bAddThisTechnology = false;
					break;
				}
			}
			if( bAddThisTechnology )
			{
				pNearbyAppliedTech->AddTail( pCurrentTechDesc );
			}
		}
	}
	int g=0;
}

float CEOSAIStrategicAI::GetTechValue( CEOSAITechnologyDesc* pTechDesc )
{
	//CUnitset* pUnitset = GetCommonState()->GetActiveUnitset();
	long iPlayer = m_pAIPlayer->GetPlayerNumber();//GetWorldDescPlayer()->GetLocalPlayerNumber();
	CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIPlayer()->GetAIBrain()->GetAIThoughtDatabase();

	// Based on value and cost (including cost of prereqs)

	// Value - based on a number of factors:
	//   How much of an advantage does the upgrade provide? (+10% or +40% to combat, movement, etc?)
	//     How does an increased production cost affect the value of the upgrade?
	//   What units does the enemy have (that the AI needs to counter?)
	//   What is the AI's current situation?  (Island hopping?  Ground war?  etc)
	//   How will this upgrade increase the value of the AI's current military
	//     (if the AI has lots of tanks, then a tank-upgrade value is multiplied by the number of tanks)
	//
	float fValue = 10.0f; // all upgrades have a base value (to prevent skewing too hard to one tech lineage)

	// Count the number of existing units affected by the upgrade (but don't weigh this too heavily)
	long iNumberOfUpgradableUnits = 0;
	POSITION pos = pTechDesc->GetEnableUnitList()->GetHeadPosition();
	while( pos )
	{
		CString strEnableUnit = pTechDesc->GetEnableUnitList()->GetNext( pos );
		//CEOSAIUnitTemplate* pEnableUnitTemplate = pUnitset->GetUnitTemplate( strEnableUnit );
		CEOSAIUnitTemplate* pEnableUnitTemplate = g_pEOSAIInterface->GetAIGameRules()->GetAIUnitTemplate( strEnableUnit );
		if( pEnableUnitTemplate )
		{
			CEOSAIUnitTemplate* pUpgradesFrom = pEnableUnitTemplate->GetUpgradesFrom();
			POSITION pos2 = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos2 );
				//CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject->GetServerPoiObject() );
				CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
				//
				if( pUnit && pUnit->GetAIUnitTemplate() == pUpgradesFrom )
				{
					iNumberOfUpgradableUnits++;
				}
			}
		}
	}
	fValue += iNumberOfUpgradableUnits;

	// Add some value for buildings
	float fProductionValue = 0.0f;
	pos = pTechDesc->GetEnableBuildingList()->GetHeadPosition();
	while( pos )
	{
		fProductionValue += 1.0f;

		CString strEnableBuilding = pTechDesc->GetEnableBuildingList()->GetNext( pos );
		ASSERT( strEnableBuilding != _T("") );
		if( strEnableBuilding != _T("") )
		{
			//CBuildingDescription* pBuildingDescription = pUnitset->GetBuildingDescription( strEnableBuilding );
			CEOSAIBuildingDescription* pBuildingDescription = g_pEOSAIInterface->GetAIGameRules()->GetAIBuildingDescription( strEnableBuilding );
			if( pBuildingDescription )
			{
				POSITION pos2 = pAIThoughtDatabase->GetMyCities()->GetHeadPosition();
				while( pos2 )
				{
					CEOSAICity* pAICity = pAIThoughtDatabase->GetMyCities()->GetNext( pos2 );
					fProductionValue += 0.5f;

					CEOSAIBuildingValue BuildingValue( pAICity, pBuildingDescription );

					// Add value for production bonus
					//float fProductionBonus = pBuildingDescription->GetProductionBonus( pAICity->GetPopulation() );
					float fProductionBonus = BuildingValue.m_fBonusProduction; //->GetProductionBonus( pAICity->GetPopulation() );
					if( fProductionBonus > 0.0f )
					{
						float fValue = (40.0f * fProductionBonus) - pBuildingDescription->GetProductionCost();
						fValue = max( 0.0f,fValue );
						fProductionValue += fValue / ( 2.0f * (1.0f+pBuildingDescription->GetProductionCost()) );
					}
				}
			}
		}
	}
	fValue += fProductionValue;

	// National Value
	float fLandFoodBonus = pTechDesc->GetNationalLandFoodBonusMultiplier();
	if( fLandFoodBonus > 0.0f )
	{
		float fLandFoodResources = m_pAIPlayer->GetMyLandFoodResources();
		fValue += fLandFoodBonus * fLandFoodResources * m_pAIPlayer->GetCitResValue_FoodMultiplier();
	}
	float fSeaFoodBonus = pTechDesc->GetNationalSeaFoodBonusMultiplier();
	if( fSeaFoodBonus > 0.0f )
	{
		float fSeaFoodResources = m_pAIPlayer->GetMySeaFoodResources();
		fValue += fSeaFoodBonus * fSeaFoodResources * m_pAIPlayer->GetCitResValue_FoodMultiplier();
	}

	// Cost
	CStringList PrereqTechList;
	pTechDesc->GetPrereqTechList( &PrereqTechList );
	PrereqTechList.AddTail( pTechDesc->GetInternalName() );
	float fTotalCost = 0.0f;
	pos = PrereqTechList.GetHeadPosition();
	while( pos )
	{
		CString strTech = PrereqTechList.GetNext( pos );
		//if( GetWorldDescPlayerProxy()->GetTechnologyHasBeenDiscovered( strTech ) == false )
		if( GetTechnologyHasBeenDiscovered( strTech ) == false )
		{
			//fTotalCost += GetWorldDescPlayerProxy()->GetResearchPointsUntilCompletion( strTech );
			fTotalCost += GetResearchPointsUntilCompletion( strTech );
		}
	}
	return fValue / fTotalCost;
}


void CEOSAIStrategicAI::ProcessMail( CEOSAIMail* pIMail, bool* pbWasSignificant )
{
	// Process the IMail
	//   The AI can only understand TradeAgreements, and IMail+m_eAIMailType
	if( pIMail->GetAIMessageType() != CEOSAIMail::enumType_Undefined )
	{
		if( pIMail->GetAIMessageType() == CEOSAIMail::enumType_SuggestTeam )
		{
			// Create a PlayerInteraction object, and process it
			//CEOSAIPlayerInteraction_DeclaredWar* pDeclarationOfWar = new CEOSAIPlayerInteraction_DeclaredWar;
			//pDeclarationOfWar->m_iActor = iActor;
			//pDeclarationOfWar->m_iTarget = iTarget;
			//pDeclarationOfWar->m_iEventTurn = iTurn;
			//m_UnprocessedPlayerInteractions.AddTail( pDeclarationOfWar );
		}
		if( pIMail->GetAIMessageType() == CEOSAIMail::enumType_MultilateralWar )
		{
			// Create a PlayerInteraction object
		}
	}
	if( pIMail->GetTradeAgreement() )
	{
		ASSERT( false );
		//EvaluateAndRespondToTradeAgreement( pIMail->GetTradeAgreement() );
	}
}

void CEOSAIStrategicAI::ProcessTradeOffer( CEOSAITradeAgreement* pTrade, bool* pbWasSignificant )
{
	EvaluateAndRespondToTradeAgreement( pTrade );
	*pbWasSignificant = true;

	GetAIPlayer()->NeedToRecalculateTradeValues( true );
}

void  CEOSAIStrategicAI::CalculateAITradePrices( long iHumanPlayer, CAITradePrices* pPrices )
{
	long iAIPlayer = this->GetPlayerNumber();

	// First, get feelings and ForeignRelations
	//long iOtherPlayer = pTradeAgreement->GetOtherPlayerNumber( iAIPlayer );
	// Calculate the TradeAgreementMultiplier
	//ASSERT( m_pAIPlayer->GetPlayerNumber() == pTradeAgreement->m_iPlayer[1] );
	float fFeelings01 = g_pEOSAICommonData->GetGlobalForeignRelations()->GetFeelings01( iAIPlayer, iHumanPlayer );
	EOSAIEnumForeignRelations eRelations = g_pEOSAICommonData->GetForeignRelations( iAIPlayer, iHumanPlayer );

	// What are my resource surpluses and deltas?
	if(      eRelations == EOSAIEnumForeignRelations::enum_War      ){ fFeelings01 -= 0.3f; fFeelings01 = LimitValue( fFeelings01, 0.0f, 0.4f ); }
	else if( eRelations == EOSAIEnumForeignRelations::enum_Neutral  ){                      fFeelings01 = LimitValue( fFeelings01, 0.1f, 0.6f ); }
	else if( eRelations == EOSAIEnumForeignRelations::enum_Alliance ){ fFeelings01 += 0.1f; fFeelings01 = LimitValue( fFeelings01, 0.4f, 0.8f ); }
	else if( eRelations == EOSAIEnumForeignRelations::enum_Teammate ){ fFeelings01 += 0.4f; fFeelings01 = LimitValue( fFeelings01, 0.8f, 1.0f ); }
	else{ ASSERT( false ); }

	if( fFeelings01 < 0.05f ){ fFeelings01 -= 0.03f; }
	if( fFeelings01 < 0.10f ){ fFeelings01 -= 0.03f; }
	if( fFeelings01 < 0.15f ){ fFeelings01 -= 0.03f; }
	if( fFeelings01 < 0.20f ){ fFeelings01 -= 0.03f; }
	if( fFeelings01 < 0.25f ){ fFeelings01 -= 0.03f; }
	if( fFeelings01 < 0.30f ){ fFeelings01 -= 0.03f; }
	if( fFeelings01 < 0.35f ){ fFeelings01 -= 0.03f; }
	if( fFeelings01 < 0.40f ){ fFeelings01 -= 0.03f; }
	if( fFeelings01 < 0.45f ){ fFeelings01 -= 0.03f; }
	if( g_pEOSAIInterface->GetNumberOfActivePlayers() <= 2 && eRelations != EOSAIEnumForeignRelations::enum_Teammate )
	{
		fFeelings01 -= 0.1f;
		fFeelings01 = LimitValue( fFeelings01, 0.0f, 0.6f );
	}
	fFeelings01 = LimitValue( fFeelings01, 0.0f, 1.0f );
	//#ifdef _DEBUG
	//fFeelings01 = 0.0f;
	//#endif

	//CWorldDescPlayerProxy* pAIWorldDescPlayerProxy = GetWorldDescPlayerProxy();
	//pAIWorldDescPlayerProxy->CalculateCitResValues();
	//g_pEOSAICommonData->GetAINationalSummary3(GetPlayerNumber())->AICalculateResourceConsumptionAndDeltas(false);
	g_pEOSAICommonData->GetAINationalSummary3(GetPlayerNumber())->CalculateResourceDeltas();

	EOSAI::ResourceAmounts ResourceDelta = g_pEOSAICommonData->GetAINationalSummary3(GetPlayerNumber())->GetResourceSummary()->GetResourceDelta();
	//GetAITradePrice( iHumanPlayer, iAIPlayer, fFeelings01, pAIWorldDescPlayerProxy->GetTotalFood(), pAIWorldDescPlayerProxy->GetResourceEffectsSummary()->m_ResourceDelta.Get( _T("Food") )>GetDeltaFood(), &pPrices->m_iWantsToBuyAndSellFood, &pPrices->m_fPriceOfFood );
	GetAITradePrice( iHumanPlayer, iAIPlayer, fFeelings01, g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalFood(), ResourceDelta.Get( _T("Food") ), &pPrices->m_iWantsToBuyAndSellFood, &pPrices->m_fPriceOfFood );
	//GetAITradePrice( iHumanPlayer, iAIPlayer, fFeelings01, pAIWorldDescPlayerProxy->GetTotalIron(), pAIWorldDescPlayerProxy->GetDeltaIron(), &pPrices->m_iWantsToBuyAndSellIron, &pPrices->m_fPriceOfIron );
	GetAITradePrice( iHumanPlayer, iAIPlayer, fFeelings01, g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalIron(), ResourceDelta.Get( _T("Iron") ), &pPrices->m_iWantsToBuyAndSellIron, &pPrices->m_fPriceOfIron );
	//GetAITradePrice( iHumanPlayer, iAIPlayer, fFeelings01, pAIWorldDescPlayerProxy->GetTotalOil(),  pAIWorldDescPlayerProxy->GetDeltaOil(),  &pPrices->m_iWantsToBuyAndSellOil,  &pPrices->m_fPriceOfOil );
	GetAITradePrice( iHumanPlayer, iAIPlayer, fFeelings01, g_pEOSAICommonData->GetAINationalSummary3( GetPlayerNumber() )->GetTotalOil(), ResourceDelta.Get( _T("Oil") ),  &pPrices->m_iWantsToBuyAndSellOil,  &pPrices->m_fPriceOfOil );

	#ifdef _DEBUG
	// Debug: 24987124
	pPrices->m_iWantsToBuyAndSellOil = +10; // wants to buy oil
	//pPrices->m_iWantsToBuyAndSellOil = -10; // wants to sell oil
	pPrices->m_fPriceOfOil = 1.5f;
	#endif _DEBUG

	/*
#ifdef _DEBUG
	pPrices->m_fPriceOfFood = 2.5f;
	pPrices->m_iWantsToBuyAndSellFood = 100; // +10 = wants to buy 10 food
#endif
*/
	// Buy Prices:
	//   Teammates => BuyTechnologyMult 0.0, SellTechnologyMult 0.0
	//   fFeelings01 1.0 => BuyTechnologyMult 0.8, SellTechnologyMult 1.3
	//   fFeelings01 0.5 => BuyTechnologyMult 0.6, SellTechnologyMult 1.6
	//   fFeelings01 0.0 => BuyTechnologyMult 0.4, SellTechnologyMult 3.0
	if( eRelations == EOSAIEnumForeignRelations::enum_Teammate )
	{
		pPrices->m_fBuyTechnologyMultiplier = 0.0f;
		pPrices->m_fSellTechnologyMultiplier = 0.0f;
	}
	else
	{
		CEOSAIMathFunction  FeelingsToBuyTechFunc;
		FeelingsToBuyTechFunc.SetInputOutput( 0.0f,0.4f );
		FeelingsToBuyTechFunc.SetInputOutput( 1.0f,0.8f );
		pPrices->m_fBuyTechnologyMultiplier = FeelingsToBuyTechFunc.GetOutput( fFeelings01 );

		CEOSAIMathFunction  FeelingsToSellTechFunc;
		FeelingsToSellTechFunc.SetInputOutput( 0.0f,3.0f );
		FeelingsToSellTechFunc.SetInputOutput( 0.5f,1.6f );
		FeelingsToSellTechFunc.SetInputOutput( 1.0f,1.3f );
		pPrices->m_fSellTechnologyMultiplier = FeelingsToSellTechFunc.GetOutput( fFeelings01 );
	}

	//pPrices->m_iWantsToBuyAndSellFood = -10;
	//pPrices->m_fPriceOfFood = 2.0f;
	//pPrices->m_iWantsToBuyAndSellIron = -10;
	//pPrices->m_fPriceOfIron = 2.5f;
	//pPrices->m_iWantsToBuyAndSellOil = +10;
	//pPrices->m_fPriceOfOil = 2.0f;
	/*
	long iWantsToBuyAndSellRes = 0;
	float fPrice = 0.0f;
	GetAITradePrice( iHumanPlayer, iAIPlayer, fFeelings01, 300.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,        0.0f, 300.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.25f, 300.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.50f, 300.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.75f, 300.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,        1.0f, 300.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );

	GetAITradePrice( iHumanPlayer, iAIPlayer,        0.0f, 500.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.25f, 500.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.50f, 500.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.75f, 500.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,        1.0f, 500.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );

	GetAITradePrice( iHumanPlayer, iAIPlayer,        0.0f, 900.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.25f, 900.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.50f, 900.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.75f, 900.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,        1.0f, 900.0f, +10.0f, &iWantsToBuyAndSellRes, &fPrice );

	GetAITradePrice( iHumanPlayer, iAIPlayer,        0.0f,  50.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.25f,  50.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.50f,  50.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.75f,  50.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,        1.0f,  50.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );

	GetAITradePrice( iHumanPlayer, iAIPlayer,        0.0f,  10.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.25f,  10.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.50f,  10.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,       0.75f,  10.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
	GetAITradePrice( iHumanPlayer, iAIPlayer,        1.0f,  10.0f, -10.0f, &iWantsToBuyAndSellRes, &fPrice );
*/
	//pPrices->m_iWantsToBuyAndSellFood = iWantsToBuyAndSellRes;
	//pPrices->m_fPriceOfFood = fPrice;
}

void CEOSAIStrategicAI::GetAITradePrice( long iHumanPlayer, long iAIPlayer, float fFeelings01, float fTotalResource, float fDeltaResource, long* piWantsToBuyAndSellRes, float* pfPrice )
{
	*piWantsToBuyAndSellRes = 0;
	*pfPrice = 0.0f;

	//fTotalResource = 10.0f;
	//fDeltaResource = -5.0f;

	float fMinPrice1 = g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X();
	float fMaxPrice3 = g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X();
	float fMidPrice2 = (fMinPrice1+fMaxPrice3)/2.0f;
	float fPriceGap  = (fMaxPrice3-fMinPrice1);

	float fMinBuyPrice  = fMinPrice1;
	float fMaxBuyPrice  = fMinPrice1 + (0.8f*fFeelings01*fPriceGap);
	float fMinSellPrice = fMaxPrice3 - (fFeelings01*fPriceGap);
	float fMaxSellPrice = fMaxPrice3;

	float fResPrice = 1.0f;
	long iMaxResToSell = 0;
	long iMaxResToBuy  = 0;
	//float fTotalFood = pAIWorldDescPlayer->GetTotalFood();
	//float fDeltaFood = pAIWorldDescPlayer->GetDeltaFood();
	//#ifdef _DEBUG
	//fTotalFood = 500.0f;
	//fDeltaFood = 10.0f;
	//#endif
	float fFutureRes5Turns = fTotalResource + 5.0f*(fDeltaResource-1);
	if( fTotalResource < 50.0f || fFutureRes5Turns < 50.0f )
	{
		iMaxResToSell = 0;
		if( fFutureRes5Turns < 50.0f && fDeltaResource < 1.0f )
		{
			iMaxResToBuy = (long)( 50.0f - fFutureRes5Turns );
			if( fFeelings01 < 0.3f ){ iMaxResToBuy /= 2; }
			if( fFeelings01 < 0.1f ){ iMaxResToBuy /= 4; }
			iMaxResToBuy = 10 * (iMaxResToBuy/10); // Round to the nearest 10
			fResPrice = fMinPrice1 - 0.8f + sqrt( ((float)iMaxResToBuy)/70.0f ) + fFeelings01;
			fResPrice = LimitValue( fResPrice, fMinBuyPrice, fMaxBuyPrice );

			//pPrices->m_iWantsToBuyAndSellFood = iMaxFoodToBuy;
			//pPrices->m_fPriceOfFood = fFoodPrice;
			//if( pPrices->m_fPriceOfFood == fMinPrice1 ){ pPrices->m_iWantsToBuyAndSellFood = 0.0f; }
			if( fResPrice == fMinPrice1 ){ iMaxResToBuy = 0; }

			*piWantsToBuyAndSellRes = iMaxResToBuy;
			*pfPrice = fResPrice;
		}
	}
	eif( fTotalResource > 200.0f && fDeltaResource > 4.0f )
	{
		iMaxResToBuy  = 0;
		iMaxResToSell = (long) (( fTotalResource - 200 ) / 3);
		iMaxResToSell = 10 * (iMaxResToSell/10); // Round to the nearest 10
		fResPrice = fMaxPrice3 + 1.0f - sqrt( fFutureRes5Turns/300.0f ) - fFeelings01;
		fResPrice = LimitValue( fResPrice, fMinSellPrice, fMaxSellPrice );

		if( fResPrice == fMaxPrice3 ){ iMaxResToSell = 0; }

		//pPrices->m_iWantsToBuyAndSellFood = -iMaxFoodToSell;
		//pPrices->m_fPriceOfFood = fFoodPrice;
		*piWantsToBuyAndSellRes = -iMaxResToSell;
		*pfPrice = fResPrice;
	}
}

void  CEOSAIStrategicAI::IncomingEvent_RequestAITradeDesires( long iSendToPlayer )
{
	EOSAI::CGamePlayer* pHumanPlayer = g_pEOSAIInterface->GetGamePlayer( iSendToPlayer );
	if( pHumanPlayer == NULL ) return;
	if( pHumanPlayer->IsHuman() == false ) return;

	EOSAI::MessageFromAI_TradeDesires* pTradeDesires = new EOSAI::MessageFromAI_TradeDesires();
	pTradeDesires->m_iSendToPlayer = iSendToPlayer;
	pTradeDesires->m_iFromAIPlayer = this->GetPlayerNumber();

	// Send a message to the human player about what stuff the AI wants to buy and sell
	EOSAIEnumForeignRelations eForeignRelations = g_pEOSAICommonData->GetForeignRelations(iSendToPlayer, m_pAIPlayer->GetPlayerNumber());
	if (eForeignRelations == EOSAIEnumForeignRelations::enum_War)
	{
		// TODO
		pTradeDesires->m_bTheAIDidNotProvideAnyDataBecauseYouAreAtWar = true;
		//Message.m_AITradeDesires.m_bTheAIDidNotProvideAnyDataBecauseYouAreAtWar = true;
	}
	else
	{
		CAITradePrices  AITradePrices;
		CalculateAITradePrices(iSendToPlayer, &AITradePrices);
		pTradeDesires->m_iWantsToBuyAndSellIron = AITradePrices.m_iWantsToBuyAndSellIron;
		pTradeDesires->m_fPriceOfIron = AITradePrices.m_fPriceOfIron;
		pTradeDesires->m_iWantsToBuyAndSellFood = AITradePrices.m_iWantsToBuyAndSellFood;
		pTradeDesires->m_fPriceOfFood = AITradePrices.m_fPriceOfFood;
		pTradeDesires->m_iWantsToBuyAndSellOil = AITradePrices.m_iWantsToBuyAndSellOil;
		pTradeDesires->m_fPriceOfOil = AITradePrices.m_fPriceOfOil;
/*
		Message.m_AITradeDesires.m_iWantsToBuyAndSellIron = AITradePrices.m_iWantsToBuyAndSellIron;
		Message.m_AITradeDesires.m_fPriceOfIron = AITradePrices.m_fPriceOfIron;
		Message.m_AITradeDesires.m_iWantsToBuyAndSellFood = AITradePrices.m_iWantsToBuyAndSellFood;
		Message.m_AITradeDesires.m_fPriceOfFood = AITradePrices.m_fPriceOfFood;
		Message.m_AITradeDesires.m_iWantsToBuyAndSellOil = AITradePrices.m_iWantsToBuyAndSellOil;
		Message.m_AITradeDesires.m_fPriceOfOil = AITradePrices.m_fPriceOfOil;
*/
	}
	g_pEOSAIInterface->SendMessageFromAI(pTradeDesires);
	//g_pMessageManager->Send(Message);

	//ASSERT( false );
	#ifdef GAME_CODE
	CMessage2_TradeDesires  Message;
	Message.SetSenderId( m_pAIPlayer->GetPlayer()->GetMessageTargetId() );
	Message.AddSendTo( pHumanPlayer->GetMessageTargetId() );

	Message.m_AITradeDesires.m_iSendingToPlayer = iSendToPlayer;
	Message.m_AITradeDesires.m_iAIPlayer = m_pAIPlayer->GetPlayerNumber();

	//CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
	//if( pWorldDescServer == NULL ) return;
	EOSAIEnumForeignRelations eForeignRelations = g_pEOSAICommonData->GetForeignRelations( iSendToPlayer, m_pAIPlayer->GetPlayerNumber() );
	if( eForeignRelations == EOSAIEnumForeignRelations::enum_War )
	{
		Message.m_AITradeDesires.m_bTheAIDidNotProvideAnyDataBecauseYouAreAtWar = true;
	}
	else
	{
		CAITradePrices  AITradePrices;
		CalculateAITradePrices( iSendToPlayer, &AITradePrices );
		Message.m_AITradeDesires.m_iWantsToBuyAndSellIron = AITradePrices.m_iWantsToBuyAndSellIron;
		Message.m_AITradeDesires.m_fPriceOfIron = AITradePrices.m_fPriceOfIron;
		Message.m_AITradeDesires.m_iWantsToBuyAndSellFood = AITradePrices.m_iWantsToBuyAndSellFood;
		Message.m_AITradeDesires.m_fPriceOfFood = AITradePrices.m_fPriceOfFood;
		Message.m_AITradeDesires.m_iWantsToBuyAndSellOil  = AITradePrices.m_iWantsToBuyAndSellOil;
		Message.m_AITradeDesires.m_fPriceOfOil  = AITradePrices.m_fPriceOfOil;
	}

	g_pMessageManager->Send( Message );
	#endif GAME_CODE
}

void CEOSAIStrategicAI::EvaluateTradeAgreement( CEOSAITradeAgreement* pTradeAgreement, CString* strOpinionText, long* pOpinionSum )
{
	#ifdef _DEBUG
	if( EOSAISettings::m_bAIShouldAcceptAllTradeOffers )
	{
		*strOpinionText = _T("I accept all trade offers.");
		*pOpinionSum = 0;
		//*pOpinion01 = 1.0f;
		return;
	}
	#endif

	CAITradeEvaluationResult  TradeEvaluationResult;
	CalculateTradeValue( pTradeAgreement, &TradeEvaluationResult );

//	EOSAI::MessageFromAI_TradeOfferOpinion TradeOfferOpinion;

	// The AI will reject all trade offers if he is at war (and the trade doesn't contain a peace agreement)
	// The AI will accept free stuff, though
	//CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
	//if( pWorldDescServer )
	{
		EOSAIEnumForeignRelations eRelations = g_pEOSAICommonData->GetGlobalForeignRelations()->GetForeignRelations( pTradeAgreement->m_iPlayer[0], pTradeAgreement->m_iPlayer[1] );
		if( eRelations == EOSAIEnumForeignRelations::enum_War &&
			pTradeAgreement->m_bPeaceTreaty == false &&
			TradeEvaluationResult.m_fTradeValue11 < 1.0f )
		{
			//ASSERT( false );
			//#ifdef GAME_CODE
			*strOpinionText = "I will not accept trade offers while at war, unless it contains a peace agreement.";
			*pOpinionSum = -100;
			//#endif GAME_CODE
			return;
		}
	}

	if( TradeEvaluationResult.m_bAIDoesntWantToSellThatMuch )
	{
		//ASSERT( false );
		//#ifdef GAME_CODE
		*strOpinionText = "Sorry, that's more than I'm willing to sell.";
		*pOpinionSum = -100;
		//#endif GAME_CODE
		return;
	}
	//CWorldDescPlayer* pWorldDescPlayer = m_pAIPlayer->GetWorldDescPlayer();
	if( pTradeAgreement->DoesTheAIHaveEnoughResourcesToCompleteTheTrade( this->GetAIPlayer(), true, true, 1.0f ) == false )
	{
		//ASSERT( false );
		//#ifdef GAME_CODE
		*strOpinionText = "Sorry, that's more than I'm willing to sell.";
		*pOpinionSum = -100;
		//#endif GAME_CODE
		return;
	}

	if( TradeEvaluationResult.m_fTradeValue11 >= 0.0f )
	{
		/*
		CWorldDescPlayer* pWorldDescPlayer = m_pAIPlayer->GetWorldDescPlayer();
		if( pTradeAgreement->IsLimitedToPlayerState( pWorldDescPlayer, true, true ) == false )
		{
			*strOpinionText = "That's more than I'm willing to sell.";
			*pOpinionSum = -1;
			//*pOpinion01 = 0.0f;
			return;
		}
		else
		*/
		{
			*pOpinionSum = (long)( 100.0f * TradeEvaluationResult.m_fTradeValue11 );
			if( TradeEvaluationResult.m_fForeignRelationsBonus < 0.1f )//fTradeValue11 < 0.02f )
			{
				//ASSERT( false );
				//#ifdef GAME_CODE
				*strOpinionText = _T("This is an acceptable trade.");
				//#endif GAME_CODE
			}
			else
			{
				CString str;
				CString strTemp;
				ASSERT( TradeEvaluationResult.m_fLumpSumValueToAI >= 0.0f );
				ASSERT( TradeEvaluationResult.m_fLumpSumValueToPlayer >= 0.0f );
				if( TradeEvaluationResult.m_fLumpSumValueToPlayer == 0.0f )
				{
					//ASSERT( false );
					//#ifdef GAME_CODE
					str = _T("This gift will help bring our nations and people together. ");
					//#endif GAME_CODE
				}
				if( TradeEvaluationResult.m_fLumpSumValueToPlayer > 0.0f )
				{
					//ASSERT( false );
					//#ifdef GAME_CODE
					str = _T("This trade will help bring our nations and people together. ");
					//str.Format( "This trade will help bring our nations and people together. (+%1.0f Foreign Relations)", 
					//	TradeEvaluationResult.m_fForeignRelationsBonus );
					//#endif GAME_CODE
				}
				if( TradeEvaluationResult.m_fForeignRelationsBonus >= 10.0f )
				{
					//ASSERT( false );
					//#ifdef GAME_CODE
					strTemp.Format( _T("(+%1.0f Foreign Relations)"), TradeEvaluationResult.m_fForeignRelationsBonus );
					//#endif GAME_CODE
				}
				else
				{
					//ASSERT( false );
					//#ifdef GAME_CODE
					strTemp.Format( _T("(+%1.1f Foreign Relations)"), TradeEvaluationResult.m_fForeignRelationsBonus );
					//#endif GAME_CODE
				}
				str += strTemp;
				*strOpinionText = str;//"This is an acceptable trade.  (+15 Foreign Relations)";
			}
			//*pOpinion01 = 1.0f + fTradeValue;
			return;
		}
	}
	else
	{
		*pOpinionSum = (long)( 100.0f * TradeEvaluationResult.m_fTradeValue11 );
		if( TradeEvaluationResult.m_fTradeValue11 < -0.8 )
		{
			//ASSERT( false );
			//#ifdef GAME_CODE
			*strOpinionText = "No thanks.";
			//#endif GAME_CODE
		}
		eif( TradeEvaluationResult.m_fTradeValue11 < -0.4 )
		{
			//ASSERT( false );
			//#ifdef GAME_CODE
			*strOpinionText = "You're going to have to give me more than that.";
			//#endif GAME_CODE
		}
		eif( TradeEvaluationResult.m_fTradeValue11 < -0.1 )
		{
			//ASSERT( false );
			//#ifdef GAME_CODE
			*strOpinionText = "You're getting closer to an acceptable trade offer.";
			//#endif GAME_CODE
		}
		eif( TradeEvaluationResult.m_fTradeValue11 < 0.0 )
		{
			//ASSERT( false );
			//#ifdef GAME_CODE
			*strOpinionText = "I think we're getting close to a mutually beneficial trade.";
			//#endif GAME_CODE
		}
		//*pOpinion01 = 1.0f + fTradeValue;
		return;
	}

	*pOpinionSum = 0;
	//*pOpinion01 = 1.0f;
	return;
}

void CEOSAIStrategicAI::EvaluateAndRespondToTradeAgreement( CEOSAITradeAgreement* pTradeAgreement )
{
	//CString strOpinionText;
	//long iOpinionSum = 0;
	//float fOpinion01 = 0.0f;
	//EvaluateTradeAgreement( pTradeAgreement, &strOpinionText, &iOpinionSum );
	CAITradeEvaluationResult  TradeEvaluationResult;
	CalculateTradeValue( pTradeAgreement, &TradeEvaluationResult );

	//if( iResult == +1 )
	if( TradeEvaluationResult.m_fTradeValue11 >= 0.0f )//fOpinion01 >= 1.0f )
	{
		EOSAI::MessageFromAI_TradeOfferResponse* pResponse = new EOSAI::MessageFromAI_TradeOfferResponse();
		pResponse->m_iFromAIPlayer = this->GetPlayerNumber();
		pResponse->m_iSendToPlayer = pTradeAgreement->GetOtherPlayerNumber(pResponse->m_iFromAIPlayer);
		pResponse->m_strTradeAgreementId = pTradeAgreement->m_strTradeAgreementId;
		pResponse->m_eResponse = EOSAIEnumTradeAgreementResponse_Accept;
		g_pEOSAIInterface->SendMessageFromAI(pResponse);

		if (TradeEvaluationResult.m_fForeignRelationsBonus > 0.1f)
		{
			long iPlayerWhoMadeTheOffer = pTradeAgreement->GetPlayerWhoMadeTheOffer();
			long iPlayerWhoDidntMakeTheOffer = pTradeAgreement->GetOtherPlayerNumber(iPlayerWhoMadeTheOffer);
			CEOSAIPlayerInteraction_Trade* pTradeInteraction = new CEOSAIPlayerInteraction_Trade();
			pTradeInteraction->m_iEventTurn = g_pEOSAIInterface->GetCurrentTurn();
			pTradeInteraction->m_fImprovedForeignRelationsValue01 = TradeEvaluationResult.m_fForeignRelationsBonus;
			pTradeInteraction->m_iImprovedForeignRelationsAIPlayer = this->GetPlayerNumber();
			pTradeInteraction->m_iImprovedForeignRelationsHumanPlayer = pTradeAgreement->GetOtherPlayerNumber(pResponse->m_iFromAIPlayer);
			pTradeInteraction->m_eResponse = EOSAIEnumTradeAgreementResponse_Accept;
			pTradeInteraction->m_strTradeAgreement = pTradeAgreement->m_strTradeAgreementId;
			g_pEOSAICommonData->AddNewPlayerInteractionAndSendFeelingsUpdate(pTradeInteraction);
			/*
			pTrade->Set(
				GetWorldDescServer()->GetCurrentTurn(),
				iPlayerWhoMadeTheOffer, iPlayerWhoDidntMakeTheOffer, pTradeAgreement->m_strTradeAgreementId,
				CTradeAgreement::enum_Accept, iPlayerWhoDidntMakeTheOffer,
				iPlayerWhoMadeTheOffer, TradeEvaluationResult.m_fForeignRelationsBonus);
			g_pCommonState->GetWorldDescServer()->AddNewPlayerInteractionAndSendFeelingsUpdate(pTrade);
			*/
		}

		//ASSERT( false );
		#ifdef GAME_CODE
		I need to send back a StrategicAIOrder_TradeAgreementResponse
		g_pMessageManager->SendMessage_PlayerToServer_TradeAgreementResponse(
			GetPlayer()->GetMessageTargetId(), 
			CTradeAgreement::enum_Accept, 
			pTradeAgreement->m_strTradeAgreementId );

		if( TradeEvaluationResult.m_fForeignRelationsBonus > 0.1f )
		{
			long iPlayerWhoMadeTheOffer = pTradeAgreement->GetPlayerWhoMadeTheOffer();
			long iPlayerWhoDidntMakeTheOffer = pTradeAgreement->GetOtherPlayerNumber( iPlayerWhoMadeTheOffer );
			CEOSAIPlayerInteraction_Trade* pTrade = new CEOSAIPlayerInteraction_Trade();
			pTrade->Set( 
				GetWorldDescServer()->GetCurrentTurn(),
				iPlayerWhoMadeTheOffer, iPlayerWhoDidntMakeTheOffer, pTradeAgreement->m_strTradeAgreementId,
				CTradeAgreement::enum_Accept, iPlayerWhoDidntMakeTheOffer, 
				iPlayerWhoMadeTheOffer, TradeEvaluationResult.m_fForeignRelationsBonus );
			g_pCommonState->GetWorldDescServer()->AddNewPlayerInteractionAndSendFeelingsUpdate( pTrade );
		}
		#endif
	}
	else
	{
		EOSAI::MessageFromAI_TradeOfferResponse* pResponse = new EOSAI::MessageFromAI_TradeOfferResponse();
		pResponse->m_iFromAIPlayer = this->GetPlayerNumber();
		pResponse->m_iSendToPlayer = pTradeAgreement->GetOtherPlayerNumber(pResponse->m_iFromAIPlayer);
		pResponse->m_strTradeAgreementId = pTradeAgreement->m_strTradeAgreementId;
		pResponse->m_eResponse = EOSAIEnumTradeAgreementResponse_Decline;
		g_pEOSAIInterface->SendMessageFromAI(pResponse);

		#ifdef GAME_CODE
		I need to send back a StrategicAIOrder_TradeAgreementResponse
		g_pMessageManager->SendMessage_PlayerToServer_TradeAgreementResponse(
			GetPlayer()->GetMessageTargetId(), 
			CTradeAgreement::enum_Decline, 
			pTradeAgreement->m_strTradeAgreementId );
		#endif
	}
}

//float CEOSAIStrategicAI::CalculateTradeValue( long iPlayerPerspective, CTradeAgreementMultiplier* pMultiplier )
void CEOSAIStrategicAI::CalculateTradeValue( CEOSAITradeAgreement* pTradeAgreement, CAITradeEvaluationResult* pTradeEval )
						//float* pfTradeValue11, float* pfForeignRelationsBonus )
{
	#ifdef _DEBUG
	if( EOSAISettings::m_bAIShouldAcceptAllTradeOffers )
	{
		pTradeEval->m_fTradeValue11 = 0.0f;
		pTradeEval->m_fLumpSumValueToAI = 0.0f;
		pTradeEval->m_fLumpSumValueToPlayer = 0.0f;
		pTradeEval->m_fForeignRelationsBonus = 0.0f;
		return;
	}
	#endif

	pTradeEval->m_fTradeValue11 = -1.0f;
	pTradeEval->m_fLumpSumValueToAI = 0.0f;
	pTradeEval->m_fLumpSumValueToPlayer = 0.0f;
	pTradeEval->m_fForeignRelationsBonus = 0.0f;
	pTradeEval->m_bAIDoesntWantToSellThatMuch = false;

	long iAIPlayer = m_pAIPlayer->GetPlayerNumber();
	ASSERT( pTradeAgreement->m_iPlayer[0] == iAIPlayer || pTradeAgreement->m_iPlayer[1] == iAIPlayer );

	long iOtherPlayer = pTradeAgreement->GetOtherPlayerNumber( iAIPlayer );
	// Calculate the TradeAgreementMultiplier
	//ASSERT( m_pAIPlayer->GetPlayerNumber() == pTradeAgreement->m_iPlayer[1] );
	float fFeelings01 = g_pEOSAICommonData->GetGlobalForeignRelations()->GetFeelings01( m_pAIPlayer->GetPlayerNumber(), iOtherPlayer );

	//CTradeAgreementMultiplier* pTradeAgreementMultiplier = &m_pAIPlayer->m_TradeAgreementMultiplier;
	CEOSAITradeAgreementMultiplier  Multiplier;
	Multiplier.CalculateBuyAndSellPrices( fFeelings01, m_pAIPlayer->GetCitResValueMultiplier() );

	CAITradePrices  AITradePrices;
	CalculateAITradePrices( iOtherPlayer, &AITradePrices );

	//
	bool bAutomaticallyRejectOffer = false;

	float fLumpSumValueToAIPlayer = 0.0f;
	float fLumpSumValueToOtherPlayer = 0.0f;
	float fOngoingValueToAIPlayer = 0.0f;
	float fOngoingValueToOtherPlayer = 0.0f;
	//float fImmediatePlayerValue[2];//, f20TurnPlayerValue[2];
	//fImmediatePlayerValue[0] = 0.0f;
	//fImmediatePlayerValue[1] = 0.0f;

	long iAIPlayerIndex = 0;
	long iOtherPlayerIndex = 0;
	if( pTradeAgreement->m_iPlayer[0] == iAIPlayer ){ iAIPlayerIndex = 0; iOtherPlayerIndex = 1; }
	if( pTradeAgreement->m_iPlayer[1] == iAIPlayer ){ iAIPlayerIndex = 1; iOtherPlayerIndex = 0; }
	if( iAIPlayerIndex == 0 && iOtherPlayerIndex == 0 ){ ASSERT( false ); pTradeEval->m_fTradeValue11 = -1.0f; return; }

	EOSAI::CGamePlayer* pThisPlayer = g_pEOSAIInterface->GetGamePlayer( pTradeAgreement->m_iPlayer[iAIPlayerIndex] );
	EOSAI::CGamePlayer* pOtherPlayer = g_pEOSAIInterface->GetGamePlayer( pTradeAgreement->m_iPlayer[iOtherPlayerIndex] );
	if( pThisPlayer == NULL || pOtherPlayer == NULL ){ pTradeEval->m_fTradeValue11 = -1.0f; return; }

	/*
	if( iAIPlayerIndex > 0 || iOtherPlayerIndex > 0 )
	{
		fValueToThisPlayer +=                         1.0f * m_iPlayerMoneyLumpSum[iOtherPlayerIndex];
		fValueToThisPlayer += pMultiplier->m_fBuyIronPrice * m_iPlayerIronLumpSum[iOtherPlayerIndex];
		fValueToThisPlayer += pMultiplier->m_fBuyOilPrice  * m_iPlayerOilLumpSum[iOtherPlayerIndex];
		fValueToThisPlayer += pMultiplier->m_fBuyFoodPrice * m_iPlayerFoodLumpSum[iOtherPlayerIndex];

		fValueToThisPlayer -=                          1.0f * m_iPlayerMoneyLumpSum[iAIPlayerIndex];
		fValueToThisPlayer -= pMultiplier->m_fSellIronPrice * m_iPlayerIronLumpSum[iAIPlayerIndex];
		fValueToThisPlayer -= pMultiplier->m_fSellOilPrice  * m_iPlayerOilLumpSum[iAIPlayerIndex];
		fValueToThisPlayer -= pMultiplier->m_fSellFoodPrice * m_iPlayerFoodLumpSum[iAIPlayerIndex];
	}
	*/
	if( AITradePrices.m_iWantsToBuyAndSellFood >= 0 ) // AI wants to buy food
	{
		// If the AI is selling food, don't allow trade
		if( pTradeAgreement->m_iPlayerFoodLumpSum[iAIPlayerIndex] > 0 ){ pTradeEval->m_fTradeValue11 = -1.0f; return; }
		// If the AI is buying more food than they want, don't allow trade
		long iAmountOfFoodToCount = min( pTradeAgreement->m_iPlayerFoodLumpSum[iOtherPlayerIndex], AITradePrices.m_iWantsToBuyAndSellFood );
		fLumpSumValueToAIPlayer += iAmountOfFoodToCount * AITradePrices.m_fPriceOfFood;
	}
	if( AITradePrices.m_iWantsToBuyAndSellFood <= 0 ) // AI wants to sell food
	{
		if( pTradeAgreement->m_iPlayerFoodLumpSum[iOtherPlayerIndex] > 0 )
		{
			// If the AI is buying food in this TradeAgreement, offer a bad price
			//float fMaxPrice3 = OPENMARKET_COST_TO_BUY_RESOURCE;
			float fMarketPrice = g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X();
			fLumpSumValueToAIPlayer +=
				pTradeAgreement->m_iPlayerFoodLumpSum[iOtherPlayerIndex] / fMarketPrice;
		}
		if( pTradeAgreement->m_iPlayerFoodLumpSum[iAIPlayerIndex] > 0 )
		{
			// If the AI is selling more food than they want, don't count the extra food
			long iAmountOfFoodToCount = min( pTradeAgreement->m_iPlayerFoodLumpSum[iAIPlayerIndex], -AITradePrices.m_iWantsToBuyAndSellFood );
			fLumpSumValueToOtherPlayer += iAmountOfFoodToCount * AITradePrices.m_fPriceOfFood;

			if( iAmountOfFoodToCount < pTradeAgreement->m_iPlayerFoodLumpSum[iAIPlayerIndex] )
			{
				pTradeEval->m_bAIDoesntWantToSellThatMuch = true;
				bAutomaticallyRejectOffer = true;
			}
		}
	}
	//
	if( AITradePrices.m_iWantsToBuyAndSellIron >= 0 ) // AI wants to buy food
	{
		// If the AI is selling food, don't allow trade
		if( pTradeAgreement->m_iPlayerIronLumpSum[iAIPlayerIndex] > 0 ){ pTradeEval->m_fTradeValue11 = -1.0f; return; }
		// If the AI is buying more food than they want, don't allow trade
		long iAmountOfIronToCount = min( pTradeAgreement->m_iPlayerIronLumpSum[iOtherPlayerIndex], AITradePrices.m_iWantsToBuyAndSellIron );
		fLumpSumValueToAIPlayer += iAmountOfIronToCount * AITradePrices.m_fPriceOfIron;
	}
	if( AITradePrices.m_iWantsToBuyAndSellIron <= 0 ) // AI wants to sell food
	{
		// If the AI is buying food in this TradeAgreement, offer a bad price
		if( pTradeAgreement->m_iPlayerIronLumpSum[iOtherPlayerIndex] > 0 )
		{
			float fMarketPrice = g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X();
			fLumpSumValueToAIPlayer +=
				pTradeAgreement->m_iPlayerIronLumpSum[iOtherPlayerIndex] / fMarketPrice;
		}
		if( pTradeAgreement->m_iPlayerIronLumpSum[iAIPlayerIndex] > 0 )
		{
			// If the AI is selling more food than they want, only count the resource amount it wants
			long iAmountOfIronToCount = min( pTradeAgreement->m_iPlayerIronLumpSum[iAIPlayerIndex], -AITradePrices.m_iWantsToBuyAndSellIron );
			fLumpSumValueToOtherPlayer += iAmountOfIronToCount * AITradePrices.m_fPriceOfIron;

			if( iAmountOfIronToCount < pTradeAgreement->m_iPlayerIronLumpSum[iAIPlayerIndex] )
			{
				pTradeEval->m_bAIDoesntWantToSellThatMuch = true;
				bAutomaticallyRejectOffer = true;
			}
		}
	}
	if( AITradePrices.m_iWantsToBuyAndSellOil >= 0 ) // AI wants to buy food
	{
		// If the AI is selling food, don't allow trade
		if( pTradeAgreement->m_iPlayerOilLumpSum[iAIPlayerIndex] > 0 ){ pTradeEval->m_fTradeValue11 = -1.0f; return; }
		// If the AI is buying more food than they want, don't allow trade
		long iAmountOfOilToCount = min( pTradeAgreement->m_iPlayerOilLumpSum[iOtherPlayerIndex], AITradePrices.m_iWantsToBuyAndSellOil );
		fLumpSumValueToAIPlayer += iAmountOfOilToCount * AITradePrices.m_fPriceOfOil;
	}
	if( AITradePrices.m_iWantsToBuyAndSellOil <= 0 ) // AI wants to sell food
	{
		// If the AI is buying food in this TradeAgreement, offer a bad price
		if( pTradeAgreement->m_iPlayerOilLumpSum[iOtherPlayerIndex] > 0 )
		{
			float fMarketPrice = g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X();
			fLumpSumValueToAIPlayer +=
				pTradeAgreement->m_iPlayerOilLumpSum[iOtherPlayerIndex] / fMarketPrice;
		}
		if( pTradeAgreement->m_iPlayerOilLumpSum[iAIPlayerIndex] > 0 )
		{
			// If the AI is selling more food than they want, only count the resource amount it wants
			long iAmountOfOilToCount = min( pTradeAgreement->m_iPlayerOilLumpSum[iAIPlayerIndex], -AITradePrices.m_iWantsToBuyAndSellOil );
			fLumpSumValueToOtherPlayer += iAmountOfOilToCount * AITradePrices.m_fPriceOfOil;

			if( iAmountOfOilToCount < pTradeAgreement->m_iPlayerOilLumpSum[iAIPlayerIndex] )
			{
				pTradeEval->m_bAIDoesntWantToSellThatMuch = true;
				bAutomaticallyRejectOffer = true;
			}
		}
	}

	// Calculate the LumpSum value of the Money + Resources
	fLumpSumValueToAIPlayer  +=                       1.0f * pTradeAgreement->m_iPlayerMoneyLumpSum[iOtherPlayerIndex];
	//fLumpSumValueToAIPlayer  += Multiplier.m_fBuyIronPrice * pTradeAgreement->m_iPlayerIronLumpSum[iOtherPlayerIndex];
	//fLumpSumValueToAIPlayer  += Multiplier.m_fBuyOilPrice  * pTradeAgreement->m_iPlayerOilLumpSum[iOtherPlayerIndex];
	//fLumpSumValueToAIPlayer  += Multiplier.m_fBuyFoodPrice * pTradeAgreement->m_iPlayerFoodLumpSum[iOtherPlayerIndex];
	fLumpSumValueToOtherPlayer +=                        1.0f * pTradeAgreement->m_iPlayerMoneyLumpSum[iAIPlayerIndex];
	//fLumpSumValueToOtherPlayer += Multiplier.m_fSellIronPrice * pTradeAgreement->m_iPlayerIronLumpSum[iAIPlayerIndex];
	//fLumpSumValueToOtherPlayer += Multiplier.m_fSellOilPrice  * pTradeAgreement->m_iPlayerOilLumpSum[iAIPlayerIndex];
	//fLumpSumValueToOtherPlayer += Multiplier.m_fSellFoodPrice * pTradeAgreement->m_iPlayerFoodLumpSum[iAIPlayerIndex];

	// Calculate the Ongoing value of the Money + Resources
	fOngoingValueToAIPlayer  +=                       1.0f * pTradeAgreement->m_iPlayerMoneyPerTurn[iOtherPlayerIndex];
	fOngoingValueToAIPlayer  += Multiplier.m_fBuyIronPrice * pTradeAgreement->m_iPlayerIronPerTurn[iOtherPlayerIndex];
	fOngoingValueToAIPlayer  += Multiplier.m_fBuyOilPrice  * pTradeAgreement->m_iPlayerOilPerTurn[iOtherPlayerIndex];
	fOngoingValueToAIPlayer  += Multiplier.m_fBuyFoodPrice * pTradeAgreement->m_iPlayerFoodPerTurn[iOtherPlayerIndex];
	fOngoingValueToOtherPlayer +=                        1.0f * pTradeAgreement->m_iPlayerMoneyPerTurn[iAIPlayerIndex];
	fOngoingValueToOtherPlayer += Multiplier.m_fSellIronPrice * pTradeAgreement->m_iPlayerIronPerTurn[iAIPlayerIndex];
	fOngoingValueToOtherPlayer += Multiplier.m_fSellOilPrice  * pTradeAgreement->m_iPlayerOilPerTurn[iAIPlayerIndex];
	fOngoingValueToOtherPlayer += Multiplier.m_fSellFoodPrice * pTradeAgreement->m_iPlayerFoodPerTurn[iAIPlayerIndex];

	if( fOngoingValueToOtherPlayer > 0.0f ) bAutomaticallyRejectOffer = true;

	// Calculate the value of Units, Resources, and Cities
	// What Cities/Resources/Units is the AI giving away?
	if( pTradeAgreement->m_PlayerUnitObjectId[iAIPlayerIndex].IsEmpty() == FALSE ) bAutomaticallyRejectOffer = true;
	if( pTradeAgreement->m_PlayerCitiesObjectId[iAIPlayerIndex].IsEmpty() == FALSE ) bAutomaticallyRejectOffer = true;
	if( pTradeAgreement->m_PlayerResourcesObjectId[iAIPlayerIndex].IsEmpty() == FALSE ) bAutomaticallyRejectOffer = true;
	// What Cities/Resources/Units is the human giving away?
	if( pTradeAgreement->m_PlayerUnitObjectId[iOtherPlayerIndex].IsEmpty() == FALSE ) fLumpSumValueToAIPlayer += 0.1f;
	if( pTradeAgreement->m_PlayerCitiesObjectId[iOtherPlayerIndex].IsEmpty() == FALSE ) fLumpSumValueToAIPlayer += 0.1f;
	if( pTradeAgreement->m_PlayerResourcesObjectId[iOtherPlayerIndex].IsEmpty() == FALSE ) fLumpSumValueToAIPlayer += 0.1f;

	// CList< CEOSAIPoiObjectIdAndName* >  m_PlayerUnitObjectId[2];
	// CList< CEOSAIPoiObjectIdAndName* >  m_PlayerCitiesObjectId[2];
	// CList< CEOSAIPoiObjectIdAndName* >  m_PlayerResourcesObjectId[2];

	// Calculate the value of Map-Views
	if( pTradeAgreement->m_bPlayerMapPeek[iAIPlayerIndex] ) bAutomaticallyRejectOffer = true;
	if( pTradeAgreement->m_bPlayerMapViewOngoing[iAIPlayerIndex] ) bAutomaticallyRejectOffer = true;
	// bool m_bPlayerMapPeek[2];
	// bool m_bPlayerMapViewOngoing[2];

	// Calculate the value of the technology
	//   How should technology be valued?
	//   The simplistic method is to ask "what's the RP cost of the technology?"
	//     Getting a technology means the AI doesn't have to research it.
	//   However, it might be more valuable that that because:
	//     + The AI gets the technology immediately (rather than spending time researching it)
	//     + The AI doesn't have to spend all the time researching the prerequisites to use it
	//       (If the technology gives them something useful - and not all technologies do.)
	//     - If the AI is almost at the end of the tech-tree, then he might as well wait and get it
	//       In this case, the value of technology goes down because he's almost maxed-out anyway
	//     + The technology might be very useful for the AI (especially if it matches his personality)
	//     - The technology might not be useful for the AI (e.g. ship technology in a land campaign)
	//CWorldDescPlayer* pWorldDescPlayer = m_pAIPlayer->GetWorldDescPlayer();//GetCommonState()->GetWorldDescPlayerByPlayerNumber( iPlayerPerspective );//g_pGameAppState->GetWorldDescPlayer();//GetCommonState()->GetLo
	//CPlayerTechnologySet* pPlayerTechnologySet = GetWorldDescPlayerProxy()->GetPlayerTechnologySet();
	//ASSERT( false ); // This may not work
	//CUnitset* pActiveUnitset = GetCommonState()->GetActiveUnitset();
	CString strThisPlayerUnitsubset = pThisPlayer->GetUnitsubset();
	CString strOtherPlayerUnitsubset = pOtherPlayer->GetUnitsubset();
	for( long iPlayerIndex=0; iPlayerIndex<2; iPlayerIndex++ )
	{
		long iCurrentPlayer = pTradeAgreement->m_iPlayer[iPlayerIndex];
		long iOtherPlayer = pTradeAgreement->m_iPlayer[1-iPlayerIndex];
		//CWorldDescPlayerProxy* pOtherPlayerProxy = GetWorldDescServer()->GetWorldDescPlayerProxy( iOtherPlayer );
		POSITION pos = pTradeAgreement->m_PlayerTechnologyList[iPlayerIndex].GetHeadPosition();
		while( pos )
		{
			CString strTechnology = pTradeAgreement->m_PlayerTechnologyList[iPlayerIndex].GetNext( pos );

			float fResearchCost = 0.0f;
			if( iOtherPlayer == iAIPlayer )
			{
				if( HasTechnologyHint( strTechnology ) == false )
				{
					fResearchCost += GetResearchPointsUntilCompletion( strTechnology );
				}
			}
			else
			{
				CEOSAITechnologyDesc* pTechnologyDesc = g_pEOSAIInterface->GetAIGameRules()->GetTechnologyDesc( strTechnology );
				if( g_pEOSAICommonData->GetAINationalSummary3( iOtherPlayer )->GetTechnologyHasBeenDiscovered( strTechnology ) == false )
				{
					fResearchCost += pTechnologyDesc->GetCost();
				}
				/*
				CPlayerTechnologyObject* pPlayerTechnologyObject = pPlayerTechnologySet->GetPlayerTechnologyObject( iOtherPlayer );
				if( pPlayerTechnologyObject->TechnologyExists( strTechnology ) == false )
				{
					fResearchCost += pTechnologyDesc->GetCost();
				}
				*/
			}

			if( iCurrentPlayer == iAIPlayer )
			{
				fLumpSumValueToOtherPlayer += AITradePrices.m_fSellTechnologyMultiplier * fResearchCost/2.0f;
			}
			if( iCurrentPlayer != iAIPlayer )
			{
				fLumpSumValueToAIPlayer += AITradePrices.m_fBuyTechnologyMultiplier * fResearchCost/2.0f;
			}
		}
	}

	if( pTradeAgreement->m_bPeaceTreaty )
	{
		//if( g_pCommonState->GetWorldBuildDesc()->GetAllPlayersPermanentlyAtWar() )
		if( g_pEOSAICommonData->GetAllPlayersPermanentlyAtWar() )
		{
			bAutomaticallyRejectOffer = true;
		}
		if( pThisPlayer->m_bScenarioPlayerRestriction_CannotMakePeace )
		{
			bAutomaticallyRejectOffer = true;
		}

		CEOSAIForeignRelationsState::EnumRelationsPlan ePlan = m_CurrentForeignRelations.m_RelationsPlan.Value( iAIPlayer,iOtherPlayer );
		if( ePlan == CEOSAIForeignRelationsState::enum_MakePeace )
		{
			// If the AI wants to make peace, this is okay
			fLumpSumValueToAIPlayer += 0.01f;
		}
		else
		{
			bAutomaticallyRejectOffer = true;
		}
	}
	if( pTradeAgreement->m_bNonAggressionPact ) bAutomaticallyRejectOffer = true;
	if( pTradeAgreement->m_bAlliance ) bAutomaticallyRejectOffer = true;
	if( pTradeAgreement->m_AgreementToAttackPlayer[iAIPlayerIndex].GetSize() > 0 ) bAutomaticallyRejectOffer = true;

	if( bAutomaticallyRejectOffer ){ pTradeEval->m_fTradeValue11 = -1.0f; return; }
	if( fLumpSumValueToAIPlayer + fLumpSumValueToOtherPlayer == 0.0f ){ pTradeEval->m_fTradeValue11 = -1.0f; return; } // avoid divide by 0

	float fRatio01 = fLumpSumValueToAIPlayer / ( fLumpSumValueToAIPlayer + fLumpSumValueToOtherPlayer );
	//return ( fRatio01 * 2.0 ) - 1.0f; // -1.0 to +1.0
	float fTradeValue11 = ( fRatio01 * 2.0f ) - 1.0f;
	pTradeEval->m_fTradeValue11 = fTradeValue11;
	pTradeEval->m_fLumpSumValueToAI = fLumpSumValueToAIPlayer;
	pTradeEval->m_fLumpSumValueToPlayer = fLumpSumValueToOtherPlayer;

	// Figure out the excess LumpSum value to the AI, and normalize it with the size of the nations
	if( fTradeValue11 >= 0.0f )
	{
		float fExcessValue = fLumpSumValueToAIPlayer - fLumpSumValueToOtherPlayer;
		ASSERT( fExcessValue >= 0.0f );
		if( fExcessValue < 0.0f ){ fExcessValue = 0.0f; }
		if( fExcessValue > 0.0f )
		{
			// Multiply the value based on the size of the player and size of the AIPlayer
			//CAICommonData* pAICommonData = g_pCommonState->GetWorldDescServer()->GetAICommonData();
			while( g_pEOSAICommonData->GetDataIsValid() == false )
			{
				Sleep( 100 );
			}
			CEOSAINationalSummary3* pAIPlayerSummary = g_pEOSAICommonData->GetAINationalSummary3( iAIPlayer );
			CEOSAINationalSummary3* pHumanPlayerSummary = g_pEOSAICommonData->GetAINationalSummary3( iOtherPlayer );
			float fAIPower = pAIPlayerSummary->GetPower();
			float fHumanPower = pHumanPlayerSummary->GetPower();

			float fPower = ( max( fAIPower,30.0f ) + max( fHumanPower,30.0f ) ) / 2.0f;

			pTradeEval->m_fForeignRelationsBonus = (20.0f/fPower) * fExcessValue;
		}
	}
}

void CEOSAIStrategicAI::ProcessMailResponse( CEOSAIMailResponse* pIMailResponse, bool* pbWasSignificant )
{
	if (pIMailResponse->m_iAIMessageUID == 0) { ASSERT(false); return; }

	POSITION pos = m_StrategicAIOrders.GetHeadPosition();
	while( pos )
	{
		CEOSAIStrategicAIOrder* pStrategicAIOrder = m_StrategicAIOrders.GetNext( pos );
		if( pStrategicAIOrder->ContainsAIMailId( pIMailResponse->m_iAIMessageUID) &&
			pStrategicAIOrder->GetCurrentState() == CEOSAIStrategicAIOrder::enumState_Pending )
		{
			pStrategicAIOrder->IncomingResponse( pIMailResponse ); //iAIMailId, iPlayer, eResponse );
			return;
		}
	}
	ASSERT( false );

	/*
	ASSERT( m_pStrategicOrder );
	ASSERT( m_pStrategicOrder->ContainsAIMailId( pIMailResponse->m_iAIMessageUID ) );
	if( m_pStrategicOrder->ContainsAIMailId( pIMailResponse->m_iAIMessageUID ) &&
		m_pStrategicOrder->GetCurrentState() == CEOSAIStrategicAIOrder::enumState_Pending )
	{
		m_pStrategicOrder->SetResponse( pIMailResponse ); //iAIMailId, iPlayer, eResponse );
	}
	*/

	/*
	POSITION pos = m_Orders.GetHeadPosition();
	while( pos )
	{
		CEOSAIStrategicAIOrder* pOrder = m_Orders.GetNext( pos );
		if( pOrder->ContainsAIMailId( pIMailResponse->m_iAIMessageUID ) &&
			pOrder->GetCurrentState() == CEOSAIStrategicAIOrder::enumState_Pending )
		{
			pOrder->SetResponse( pIMailResponse ); //iAIMailId, iPlayer, eResponse );
		}
	}
	*/
}

void CEOSAIStrategicAI::ProcessTradeAgreementResponse( CEOSAITradeAgreementResponse* pTradeAgreementResponse, bool* pbWasSignificant )
{
	if( pTradeAgreementResponse->m_iFromPlayer == m_pAIPlayer->GetPlayerNumber() )
	{
		return; // this was my response to someone else
	}
	if( pTradeAgreementResponse->m_iFromPlayer == 0 )
	{
		return; // this was the server's response to my open-market trade
	}

	/*
	//long iTradeAgreement = pTradeAgreementResponse->m_pTradeAgreement->m_iTradeAgreementId;
	ASSERT( m_pStrategicOrder );
	ASSERT( m_pStrategicOrder->ContainsTradeAgreement( pTradeAgreementResponse->m_iTradeAgreement ) );
	if( m_pStrategicOrder->ContainsTradeAgreement( pTradeAgreementResponse->m_iTradeAgreement ) &&
		m_pStrategicOrder->GetCurrentState() == CEOSAIStrategicAIOrder::enumState_Pending )
	{
		m_pStrategicOrder->SetResponse( pTradeAgreementResponse ); //iAIMailId, iPlayer, eResponse );
	}
	*/
	POSITION pos = m_StrategicAIOrders.GetHeadPosition();
	while( pos )
	{
		CEOSAIStrategicAIOrder* pStrategicAIOrder = m_StrategicAIOrders.GetNext( pos );
		//if( pStrategicAIOrder->ContainsTradeAgreement( pTradeAgreementResponse->m_iTradeAgreement ) &&
		if( pStrategicAIOrder->ContainsTradeAgreement( pTradeAgreementResponse->m_strTradeAgreementId ) &&
			pStrategicAIOrder->GetCurrentState() == CEOSAIStrategicAIOrder::enumState_Pending )
		{
			// What if the trade agreement was not part of a pre-existing strategic order?
			pStrategicAIOrder->SetResponse( pTradeAgreementResponse ); //iAIMailId, iPlayer, eResponse );
			*pbWasSignificant = true;
			return;
		}
	}
	if( pTradeAgreementResponse->m_iFromPlayer == 0 ) // Expired (because of insufficient funds)
	{
		return;
	}
	ASSERT( false ); // The AI needs to deal with this situation

	/*
	POSITION pos = m_Orders.GetHeadPosition();
	while( pos )
	{
		CEOSAIStrategicAIOrder* pOrder = m_Orders.GetNext( pos );
		if( pOrder->ContainsTradeAgreement( pTradeAgreementResponse->m_iTradeAgreement ) &&
			pOrder->GetCurrentState() == CEOSAIStrategicAIOrder::enumState_Pending )
		{
			pOrder->SetResponse( pTradeAgreementResponse );
		}
	}
	*/
}

//
/*
float CEOSAIStrategicAI::GetPercentageOfCapturedCitRes()
{
	CWorldDescServer* pWorldDescServer = GetAIPlayer()->GetWorldDescServer();
	return 
		1.0f - ( pWorldDescServer->GetAICommonData()->GetTotalUnownedCitResProductionOnMap() /
				 pWorldDescServer->GetAICommonData()->GetTotalCitResProductionOnMap() );
	/*
	float fOwnedCityProduction = 0.0f;
	float fOwnedResProduction = 0.0f;
	long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	for( long i=1; i<iNumberOfPlayers; i++ )
	{
		fOwnedCityProduction += pWorldDescServer->GetAICommonData()->GetAINationalSummary3( i )->m_CitResSummary.GetCityValue();
		fOwnedResProduction += pWorldDescServer->GetAICommonData()->GetAINationalSummary3( i )->m_CitResSummary.GetResValue();
	}
	return
		( fOwnedCityProduction + fOwnedResProduction ) / 
		( pWorldDescServer->GetAICommonData()->GetTotalCityProductionOnMap() + pWorldDescServer->GetAICommonData()->GetTotalResourcesOnMap() );
}
*/

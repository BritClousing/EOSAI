
#include "stdafx.h"
#include "EOSAIForeignRelationsSituation.h"
//#include "HypotheticalForeignPowerBalance.h"
//#include "CommonState.h"
//#include "WorldBuildDesc.h"
//#include "WorldDescServer.h"
//#include "WorldDescPlayer.h"
//#include "EOSAIBrain.h"
#include "AIPlayer.h"
#include "EOSAIGamePlayer.h"
//#include "InternalMail.h"
//#include "MessageManager.h"
//#include "AIUnresolvedSituation.h"
//#include "TWDx.h"
//#include "GameAppState.h"
#include "EOSAICommonData2.h"
#include "EOSAIInterface.h"

extern EOSAI::CInterface* g_pEOSAIInterface;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Largest values come first
bool SortSingleNationByWorldPowerBalance( CEOSAINationalSummary* pSummary1, CEOSAINationalSummary* pSummary2 )
{
	if( pSummary1->m_fPlayerVsWorldPowerBalance > pSummary2->m_fPlayerVsWorldPowerBalance )
	{
		return true;
	}
	else if( pSummary1->m_fPlayerVsWorldPowerBalance < pSummary2->m_fPlayerVsWorldPowerBalance )
	{
		return false;
	}
	return true;  // always return true when values are equal
}

// Largest values come first
bool SortNationAndTheirAlliesByWorldPowerBalanceIgnoreAIPlayer( EOSAI::CAINationalAlliedAndEnemyPower* pPower1, EOSAI::CAINationalAlliedAndEnemyPower* pPower2 )
{
	//float fVal1 = pPower1->m_fPlayerAndAlliesVsWorldPowerBalance - pPower1->m_fThinkingPlayersContributionToAlliedAndEnemyPower;
	//float fVal2 = pPower2->m_fPlayerAndAlliesVsWorldPowerBalance - pPower2->m_fThinkingPlayersContributionToAlliedAndEnemyPower;
	float fVal1 = pPower1->m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution;
	float fVal2 = pPower2->m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution;
	if( fVal1 > fVal2 )
	{
		return true;
	}
	else if( fVal1 < fVal2 )
	{
		return false;
	}
	return true;  // always return true when values are equal
}

// Largest values come first
bool SortNationByDangerIndex( EOSAI::CAINationalAlliedAndEnemyPower* pPower1, EOSAI::CAINationalAlliedAndEnemyPower* pPower2 )
{
	if( pPower1->m_fDangerIndex > pPower2->m_fDangerIndex )
	{
		return true;
	}
	else if( pPower1->m_fDangerIndex < pPower2->m_fDangerIndex )
	{
		return false;
	}
	return true;  // always return true when values are equal
}

// Largest values come first
bool NationalSummariesSortedByL3Desire( CEOSAINationalSummary* pSummary1, CEOSAINationalSummary* pSummary2 )
{
	if( pSummary1->m_fOtherPlayersCombinedL3DesireToAttackMe > pSummary2->m_fOtherPlayersCombinedL3DesireToAttackMe )
	{
		return true;
	}
	else if( pSummary1->m_fOtherPlayersCombinedL3DesireToAttackMe < pSummary2->m_fOtherPlayersCombinedL3DesireToAttackMe )
	{
		return false;
	}
	return true;  // always return true when values are equal
}

///

CEOSAIForeignRelationsSituation::CEOSAIForeignRelationsSituation()
{
	m_bHasBeenInitialized = false;

	m_fTotalCombatPowerOfAllPlayers = 0.0f;
	m_fTotalProductionOfAllPlayers = 0.0f;
//	m_fTotalProductionOnMap = 0.0f;
//	m_fTotalUnownedCitResOnMap = 0.0f;
//	m_fTotalCitResOnMap = 0.0f;

	// Overall situation
	m_fUnownedCitResStrategy = 0.0f;
	m_fCombatStrategy = 0.0f;

	// Attack planning
	m_iWouldLikeToAttackPlayer = 0;
	m_fMyPowerBalanceIfISimplyDeclareWar = 0.0f;
	m_fMyPowerBalanceIfIMakePeaceWithExistingEnemies = 0.0f;
	m_fMyPowerBalanceIfIMakePeaceAndGetAllOtherPlayersToDeclareWar = 0.0f;

	m_iPlanToAttackPlayer = 0;
	m_MyEnemiesOrSoonToBeEnemies.Clear();
}

void  CEOSAIForeignRelationsSituation::DeleteExistingObjects()
{
	while( m_NationalSummaries.IsEmpty() == FALSE )
	{
		delete m_NationalSummaries.RemoveHead();
	}
	m_SingleNationsSortedByPower.RemoveAll();
	m_NationsSortedByDangerIndex.RemoveAll();

	m_PlayersWhoMightBeWillingToMakePeace.Clear();
	m_PlayersWhoMightBeWillingToJoinMeInWar.Clear();
	m_PotentialAlliesSet.Clear();
	m_MyEnemiesOrSoonToBeEnemies.Clear();
}

//

long CEOSAIForeignRelationsSituation::GetAIPlayerNumber()
{
	return m_pAIPlayer->GetPlayerNumber();
}

float CEOSAIForeignRelationsSituation::GetPercentageOfCitResThatAreUnowned()
{
	ASSERT( m_bHasBeenInitialized );

	//CAICommonData* pAICommonData = GetCommonState()->GetWorldDescServer()->GetAICommonData();
	if( g_pEOSAICommonData->GetTotalCitResProductionOnMap() == 0.0f ) return 0.0f;
	//return m_fTotalUnownedCitResOnMap / m_fTotalCitResOnMap;
	return g_pEOSAICommonData->GetTotalUnownedCitResProductionOnMap() / g_pEOSAICommonData->GetTotalCitResProductionOnMap();//m_fTotalCitResOnMap;
}

void  CEOSAIForeignRelationsSituation::ResetForeignRelationsFeelings()
{
	POSITION pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pNationalSummary = m_NationalSummaries.GetNext( pos );
		for( long iPlayer=1; iPlayer<pNationalSummary->m_aPositionTowardsPlayer.m_iSize; iPlayer++ )
		{
			pNationalSummary->m_aPositionTowardsPlayer[ iPlayer ].ResetFeelings();
			/*
			CPositionTowardsOtherPlayer* pPosition = pNationalSummary->GetPositionTowardsPlayer( iPlayer );
			ASSERT( pPosition );
			if( pPosition )
			{
				pPosition->ResetFeelings();
			}
			*/
		}
	}
}

void  CEOSAIForeignRelationsSituation::AdjustForeignRelationFeelings( long iPlayer, long iTarget, float fValue, float fWeight )
{
	CEOSAINationalSummary* pNationalSummary = GetNationalSummary( iPlayer );
	ASSERT( pNationalSummary );
	if( pNationalSummary )
	{
		EOSAI::CPositionTowardsOtherPlayer* pPosition = pNationalSummary->GetPositionTowardsPlayer( iTarget );
		ASSERT( pPosition );
		if( pPosition )
		{
			pPosition->AdjustFeelings( fValue, fWeight );
		}
	}
}

// Used when agreements are signed
/*
void  CEOSAIForeignRelationsSituation::SignedAPeaceAgreement( long iPlayer1, long iPlayer2 )
{
	CEOSAINationalSummary* pPlayer1Summary = GetNationalSummary( iPlayer1 );
	if( pPlayer1Summary )
	{
		pPlayer1Summary->SetForeignRelationship( iPlayer2, EOSAIEnumForeignRelations::enum_Defensive );
	}
	CAISummary* pPlayer2Summary = GetNationalSummary( iPlayer2 );
	if( pPlayer2Summary )
	{
		pPlayer2Summary->SetForeignRelationship( iPlayer1, EOSAIEnumForeignRelations::enum_Defensive );
	}
}
*/
//

CEOSAINationalSummary* CEOSAIForeignRelationsSituation::GetNationalSummary( long iPlayer )
{
	/*
	std::list< CEOSAINationalSummary* >::iterator iter;
	for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	{
		CEOSAINationalSummary* pNationalSummary = *iter;
		if( pNationalSummary->m_iPlayer == iPlayer )
		{
			return pNationalSummary;
		}
	}
	*/
	POSITION pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pNationalSummary = m_NationalSummaries.GetNext( pos );
		if( pNationalSummary->m_iPlayer == iPlayer )
		{
			return pNationalSummary;
		}
	}
	return NULL;
}
/*
CAINationalAlliedAndEnemyPower*  CEOSAIForeignRelationsSituation::GetAINationalAlliedAndEnemyPower( long iPlayer )
{
	POSITION pos = m_SingleNationsSortedByPower.GetHeadPosition();
	while( pos )
	{
		CAINationalAlliedAndEnemyPower* pPower = m_SingleNationsSortedByPower.GetNext( pos );
		if( pPower->m_pNationalSummary->m_iPlayer == iPlayer )
		{
			return pPower;
		}
	}
	ASSERT( false );
	return NULL;
}
*/
// Level1 desire of all players
float  CEOSAIForeignRelationsSituation::GetCombinedLevel1DesireToAttackPlayer( long iPotentialTargetPlayer )
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	ASSERT( iPotentialTargetPlayer > 0 );
	ASSERT( iPotentialTargetPlayer <= iNumberOfPlayers );

	float fTotal = 0.0f;
	if( iPotentialTargetPlayer <= iNumberOfPlayers )
	{
		POSITION pos = m_NationalSummaries.GetHeadPosition();
		while( pos )
		{
			CEOSAINationalSummary* pNationalSummary = m_NationalSummaries.GetNext( pos );
			fTotal += pNationalSummary->m_aPositionTowardsPlayer[ iPotentialTargetPlayer ].m_fLevel1DesireToAttackPlayer;
		}
	}
	return fTotal;
}

float  CEOSAIForeignRelationsSituation::GetLevel1DesireToAttackPlayer( long iPlayer, long iPotentialTargetPlayer )
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	ASSERT( iPlayer > 0 );
	ASSERT( iPlayer <= iNumberOfPlayers );
	ASSERT( iPotentialTargetPlayer > 0 );
	ASSERT( iPotentialTargetPlayer <= iNumberOfPlayers );

	if( iPotentialTargetPlayer <= iNumberOfPlayers )
	{
		POSITION pos = m_NationalSummaries.GetHeadPosition();
		while( pos )
		{
			CEOSAINationalSummary* pNationalSummary = m_NationalSummaries.GetNext( pos );
			if( pNationalSummary->m_iPlayer == iPlayer )
			{
				return pNationalSummary->m_aPositionTowardsPlayer[ iPotentialTargetPlayer ].m_fLevel1DesireToAttackPlayer;
			}
		}
	}
	return 0.0f;
}

//

float CEOSAIForeignRelationsSituation::GetDesireToAttackPlayer( long iPotentialTargetPlayer )
{
	return 1.0f;
}

//

void  CEOSAIForeignRelationsSituation::CreateNationalSummaries( EOSAI::AIPlayer* pAIPlayer )
{
	// Clear old information
	DeleteExistingObjects();

	m_pAIPlayer = pAIPlayer;

	m_bHasBeenInitialized = true;

	//ASSERT( m_fEstimatedCitResProductionPerLandArea != 0.0f );
	//CAICommonData* pAICommonData = g_pEOSAICommonData;
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
//	m_fTotalProductionOnMap = pAICommonData->GetTotalCityProductionOnMap();
//	m_fTotalUnownedCitResOnMap = pAICommonData->GetTotalUnownedCitResProductionOnMap();//>AICheat_GetTotalUnownedCitResOnMap();
//	m_fTotalCitResOnMap = pAICommonData->GetTotalCitResProductionOnMap();//>AICheat_GetTotalCitResOnMap();

	// fUnownedPercentage 0.0  (All CitRes owned), then m_fUnownedCitResStrategy = 0
	// fUnownedPercentage 0.25 (25% of CitRes unowned), then m_fUnownedCitResStrategy = 0.5
	// fUnownedPercentage 0.5  (50% of CitRes unowned), then m_fUnownedCitResStrategy = 1.0
	// fUnownedPercentage 0.75 (75% of CitRes unowned), then m_fUnownedCitResStrategy = 1.0
	// fUnownedPercentage 1.0  (100% of CitRes unowned), then m_fUnownedCitResStrategy = 1.0
	if( GetPercentageOfCitResThatAreUnowned() > 0.5f )
	{
		m_fUnownedCitResStrategy = 1.0f;
	}
	else
	{
		m_fUnownedCitResStrategy = 2.0f*GetPercentageOfCitResThatAreUnowned();
	}

	// Delete any old PlayerPower objects
	//DeleteExistingObjects();

	// Estimate the production capability of all the players
	m_fTotalCombatPowerOfAllPlayers = 0.0f;
	m_fTotalProductionOfAllPlayers = 0.0f;
	//POSITION pos = g_pEOSAICommonData->GetPlayerList()->GetHeadPosition();
	//while( pos )
	//long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		//EOSAI::CGamePlayer* pPlayer = g_pEOSAICommonData->GetPlayerList()->GetNext( pos );
		EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );

		//long iPlayer = pPlayer->GetPlayerNumber();
		//CEOSAINationalSummary* pNationalSummary = new CAINationalSummary( GetCommonState()->GetWorldDescServer(), this, iPlayer );
		CEOSAINationalSummary* pNationalSummary = new CEOSAINationalSummary( this, iPlayer );

		m_fTotalCombatPowerOfAllPlayers += pNationalSummary->m_fPlayerCombatPower;
		m_fTotalProductionOfAllPlayers += pNationalSummary->m_CitResSummary.m_fCityProduction;

		m_NationalSummaries.AddTail( pNationalSummary );
		//m_NationalSummaries.push_back( pNationalSummary );
	}

	// Calculate the player's individual values
	//m_fAdjustedTotalProduction = ( m_fTotalProductionOfAllPlayers + m_fTotalProductionOnMap )/2.0f;
	float fTotalPlayerPower = 0.0f;
	float fStrongestPlayerPower = 0.0f;

	POSITION posx = m_NationalSummaries.GetHeadPosition();
	while( posx )
	//std::list< CEOSAINationalSummary* >::iterator iter;
	//for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	{
		CEOSAINationalSummary* pNationalSummary = m_NationalSummaries.GetNext( posx );

		// Calculate the Production Balance
		float fPlayerProduction = pNationalSummary->m_CitResSummary.m_fCityProduction;

		// Calculate the Power balance (Player + Allies) versus the rest of the world
		float fProductionPercentage = fPlayerProduction / m_fTotalProductionOfAllPlayers;
		float fCombatPercentage = pNationalSummary->m_fPlayerCombatPower / m_fTotalCombatPowerOfAllPlayers;
		pNationalSummary->m_fPlayerPower = ( fProductionPercentage + fCombatPercentage ) / 2.0f;

		if( pNationalSummary->m_fPlayerPower > fStrongestPlayerPower )
		{
			fStrongestPlayerPower = pNationalSummary->m_fPlayerPower;
		}

		fTotalPlayerPower += pNationalSummary->m_fPlayerPower;
	}

	// Calculate the Player vs AveragePlayer power
	//for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	posx = m_NationalSummaries.GetHeadPosition();
	while( posx )
	{
		CEOSAINationalSummary* pNationalSummary = m_NationalSummaries.GetNext( posx );
		pNationalSummary->m_fPlayerVsWorldPowerBalance = pNationalSummary->m_fPlayerPower / fTotalPlayerPower;
		pNationalSummary->m_fPlayerVsAveragePlayerPowerBalance = pNationalSummary->m_fPlayerPower / (fTotalPlayerPower/iNumberOfPlayers);
		pNationalSummary->m_fPlayerVsStrongestPlayerPower = pNationalSummary->m_fPlayerPower / fStrongestPlayerPower;

		// Calculate the CurrentWarPowerSum, CurrentWarPowerBalance
		float fPro = pNationalSummary->m_fPlayerPower;
		float fCon = 0.0f;
		pNationalSummary->m_fCurrentWarPowerSum = pNationalSummary->m_fPlayerPower;

		long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		for( long i=1; i<=iNumberOfPlayers; i++ )
		{
			if( pNationalSummary->m_iPlayer == i ) continue;
			//EOSAIEnumForeignRelations  eRel = g_pGameAppState->GetCurrentWorldDesc()->GetForeignRelations( pNationalSummary->m_iPlayer, i );
			//EOSAIEnumForeignRelations  eRel = GetCommonState()->GetWorldDescPlayerByPlayerNumber( pNationalSummary->m_iPlayer )->GetForeignRelations( i );
			EOSAIEnumForeignRelations  eRel = g_pEOSAICommonData->GetForeignRelations( pNationalSummary->m_iPlayer, i );
		//POSITION pos = pNationalSummary->m_ForeignRelationshipList.GetHeadPosition();
		//while( pos )
		//{
			//EOSAIEnumForeignRelations* pRel = pNationalSummary->m_ForeignRelationshipList.GetNext( pos );
			//if( pRel->GetRelationship() == EOSAIEnumForeignRelations::enum_Alliance ||
			//	pRel->GetRelationship() == EOSAIEnumForeignRelations::enum_PermanentAlliance )
			//if( eRel == EOSAIEnumForeignRelations::enum_Alliance ||
			//	eRel == EOSAIEnumForeignRelations::enum_PermanentAlliance )
			if( eRel == EOSAIEnumForeignRelations::enum_Alliance )
			{
				CEOSAINationalSummary* pOtherSummary = GetNationalSummary( i );
				pNationalSummary->m_fCurrentWarPowerSum += pOtherSummary->m_fPlayerPower;
				fPro += pOtherSummary->m_fPlayerPower;
			}
			if( eRel == EOSAIEnumForeignRelations::enum_War )
			{
				CEOSAINationalSummary* pOtherSummary = GetNationalSummary( i );
				pNationalSummary->m_fCurrentWarPowerSum -= pOtherSummary->m_fPlayerPower;
				fCon += pOtherSummary->m_fPlayerPower;
			}
		}
		pNationalSummary->m_fCurrentWarPowerBalance = fPro / (fPro+fCon);

		int x=0;
	}

	// Sort the nations (so I have a sorted list)
	/*
	CEOSAIListSort< CEOSAINationalSummary* >  SingleNationsSortedByPower;
	m_SingleNationsSortedByPower.SetSortFunction( SortSingleNationByWorldPowerBalance );
	for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	{
		CEOSAINationalSummary* pNationalSummary = *iter;
		m_SingleNationsSortedByPower.InsertSorted( pNationalSummary );
	}
	*/

	// Sort the nations by their power (so I have a sorted list)
	m_SingleNationsSortedByPower.SetSortFunction( SortSingleNationByWorldPowerBalance );

	// Create CAINationalAlliedAndEnemyPower objects
	CList< EOSAI::CAINationalAlliedAndEnemyPower* >  PowerList;
	//for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	posx = m_NationalSummaries.GetHeadPosition();
	while( posx )
	{
		CEOSAINationalSummary* pNationalSummary = m_NationalSummaries.GetNext( posx );
		EOSAI::CAINationalAlliedAndEnemyPower* pPower =
			pNationalSummary->AllocateAINationalAlliedAndEnemyPowerObject();
		PowerList.AddTail( pPower );

		// Sort the nations by their power (so I have a sorted list)
		m_SingleNationsSortedByPower.InsertSorted( pNationalSummary );
	}

	// Give each nation a power-position (1,2,3,... 1 being the best)
	long iCount = 0;
	POSITION pos = m_SingleNationsSortedByPower.GetHeadPosition();
	while( pos )
	{
		iCount++;
		CEOSAINationalSummary* pSummary = m_SingleNationsSortedByPower.GetNext( pos );
		pSummary->m_iPlayerPowerPosition = iCount;
	}
/*
	// Calculate my feelings about attacking the other players
	//for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	posx = m_NationalSummaries.GetHeadPosition();
	while( posx )
	{
		CEOSAINationalSummary* pNationalSummary = m_NationalSummaries.GetNext( posx );

		// Want to weaken all players more powerful than me,
		//   want to strengthen all players weaker than me who:
		//     oppose my enemies, oppose players more powerful than me, are permanently allied
		for( long iPlayer=1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
		{
			if( iPlayer == pNationalSummary->m_iPlayer ) continue;

			float fAverage = ( pNationalSummary->m_fCurrentWarPowerSum + pNationalSummary->m_fPlayerPower ) / 2.0f;
			if( fAverage < 0.0f ){ fAverage = 0.0f; }
			//pNationalSummary->m_aPositionTowardsPlayer[iPlayer].
		}
	}
*/
	/*
	CAINationalAlliedAndEnemyPower* pStrongestPower = m_SingleNationsSortedByPower.GetHead();
	float fStrongestPlayerPower = pStrongestPower->m_fPlayerPower;
	for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	{
		CEOSAINationalSummary* pNationalSummary = *iter;
		pNationalSummary->m_fPlayerVsStrongestPlayerPower = pNationalSummary->m_fPlayerPower / fStrongestPlayerPower;
	}
	*/

	// Sort the nations (so I have a sorted list)
	/*
	m_SingleNationsSortedByPower.SetSortFunction( SortSingleNationByWorldPowerBalance );
	for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	{
		CEOSAINationalSummary* pNationalSummary = *iter;
		m_SingleNationsSortedByPower.InsertSorted( pNationalSummary );
	}
	*/
}

void  CEOSAIForeignRelationsSituation::Calculate()
{
	CalculatePositionTowardsOtherPlayer_Level1Thoughts(); // Based on getting-too-powerful, easy-prey, accessibility
	//CalculatePositionTowardsOtherPlayer_Level2Thoughts(); // Sum other people's thoughts so I know what they are thinking

	CalculateAlliedAndEnemyPowerForEachPlayer();
	CalculatePositionTowardsOtherPlayer_WarsAndAlliancesEffects();
	//CreateSomeHypotheticalForeignRelationScenarios();
	CreateForeignRelationsPlanForThePlayer();
	TalkToOtherPlayersAboutCooperating();
}


// Input: pPowerList with the Relationships and Player values filled in
// Results:
//     the pPowerList objects with Allied + Enemy values filled in
//     m_NationsSortedByDangerIndex is filled in
//void CEOSAIForeignRelationsSituation::CalculateAlliedAndEnemyBalances( CList< CAINationalAlliedAndEnemyPower* >* pPowerList )
void CEOSAIForeignRelationsSituation::CalculateAlliedAndEnemyPowerForEachPlayer()
{
	//long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();

    // Update the Allied, Enemies values
	//float fTotalPlayerPower = 0.0f;
	//for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	POSITION pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pSummary = m_NationalSummaries.GetNext( pos );
		EOSAI::CAINationalAlliedAndEnemyPower* pPower = pSummary->m_pPower; // pPowerList->GetNext( pos );

		// Look over this national summary, get the strength of the allies and enemies
		for( long iPlayer=1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
		{
			if( pPower->m_Relationship[iPlayer] == EOSAIEnumForeignRelations::enum_Alliance )//||
				//pPower->m_Relationship[iPlayer] == EOSAIEnumForeignRelations::enum_PermanentAlliance )
			{
				CEOSAINationalSummary* pOtherPlayerSummary = GetNationalSummary( iPlayer );
				pPower->m_fAlliesCombatPower += pOtherPlayerSummary->m_fPlayerCombatPower;
				pPower->m_fAlliesProduction += pOtherPlayerSummary->m_CitResSummary.m_fCityProduction;
				pPower->m_fAlliesPower = 
					( pPower->m_fAlliesCombatPower / m_fTotalCombatPowerOfAllPlayers +
					  pPower->m_fAlliesProduction / m_fTotalProductionOfAllPlayers )/2.0f;

				if( iPlayer == m_pAIPlayer->GetPlayerNumber() )
				{
					pPower->m_fThinkingPlayersContributionToAlliedAndEnemyPower =
						( pOtherPlayerSummary->m_fPlayerCombatPower / m_fTotalCombatPowerOfAllPlayers +
						  pOtherPlayerSummary->m_CitResSummary.m_fCityProduction / m_fTotalProductionOfAllPlayers )/2.0f;
				}
			}
			if( pPower->m_Relationship[iPlayer] == EOSAIEnumForeignRelations::enum_War )
			{
				CEOSAINationalSummary* pOtherPlayerSummary = GetNationalSummary( iPlayer );
				pPower->m_fEnemiesCombatPower += pOtherPlayerSummary->m_fPlayerCombatPower;
				pPower->m_fEnemiesProduction += pOtherPlayerSummary->m_CitResSummary.m_fCityProduction;
				pPower->m_fEnemiesPower = 
					( pPower->m_fEnemiesCombatPower / m_fTotalCombatPowerOfAllPlayers +
					  pPower->m_fEnemiesProduction / m_fTotalProductionOfAllPlayers )/2.0f;

				if( iPlayer == m_pAIPlayer->GetPlayerNumber() )
				{
					pPower->m_fThinkingPlayersContributionToAlliedAndEnemyPower =
						-( pOtherPlayerSummary->m_fPlayerCombatPower / m_fTotalCombatPowerOfAllPlayers +
						   pOtherPlayerSummary->m_CitResSummary.m_fCityProduction / m_fTotalProductionOfAllPlayers )/2.0f;
				}
			}
		}

		// Calculate the Combat Balance
		pPower->m_fPlayerAlliesAndEnemiesCombatPowerSum =
			pPower->m_pNationalSummary->m_fPlayerCombatPower + pPower->m_fAlliesCombatPower - pPower->m_fEnemiesCombatPower;
		pPower->m_fPlayerAlliesAndEnemiesCombatPowerBalance =
			( pPower->m_pNationalSummary->m_fPlayerCombatPower + pPower->m_fAlliesCombatPower ) /
			( pPower->m_pNationalSummary->m_fPlayerCombatPower + pPower->m_fAlliesCombatPower + pPower->m_fEnemiesCombatPower );

		// Calculate the Production Balance
		float fPlayerProduction = pPower->m_pNationalSummary->GetPlayerProduction();
		pPower->m_fPlayerAlliesAndEnemiesProductionSum =
			fPlayerProduction + pPower->m_fAlliesProduction - pPower->m_fEnemiesProduction;
		pPower->m_fPlayerAlliesAndEnemiesProductionBalance =
			( fPlayerProduction + pPower->m_fAlliesProduction ) /
			( fPlayerProduction + pPower->m_fAlliesProduction + pPower->m_fEnemiesProduction );

		// Calculate the Power balance (Player + Allies) versus the rest of the world
		//float fAdjustedTotalProduction = ( m_fTotalProductionOfAllPlayers + m_fTotalProductionOnMap )/2.0f;
		//float fProductionPercentage = fPlayerProduction / fAdjustedTotalProduction;
		//float fCombatPercentage = pNationalSummary->m_fPlayerCombatPower / m_fTotalCombatPowerOfAllPlayers;
		//pNationalSummary->m_fPlayerPower = ( fProductionPercentage + fCombatPercentage ) / 2.0f;
		//pNationalSummary->m_fPlayerVsWorldPowerBalance = pNationalSummary->m_fPlayerPower / 2.0f;

		float fProductionPercentage = ( fPlayerProduction + pPower->m_fAlliesProduction ) / m_fTotalProductionOfAllPlayers;
		float fCombatPercentage = ( pPower->m_pNationalSummary->m_fPlayerCombatPower + pPower->m_fAlliesCombatPower ) / m_fTotalCombatPowerOfAllPlayers;
		pPower->m_fPlayerAndAlliesVsWorldPowerBalance = ( fProductionPercentage + fCombatPercentage ) / 2.0f;
		pPower->m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution = pPower->m_fPlayerAndAlliesVsWorldPowerBalance;
		if( pPower->m_fThinkingPlayersContributionToAlliedAndEnemyPower > 0.0f )
		{
			pPower->m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution -= pPower->m_fThinkingPlayersContributionToAlliedAndEnemyPower;
		}
		int z=0;
		//fTotalPlayerPower += pPower->m_fPlayerPower;
	}

	CEOSAIListSort< EOSAI::CAINationalAlliedAndEnemyPower* >  NationsAndTheirAlliesSortedByPower;
	NationsAndTheirAlliesSortedByPower.SetSortFunction( SortNationAndTheirAlliesByWorldPowerBalanceIgnoreAIPlayer );

	pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pSummary = m_NationalSummaries.GetNext( pos );
		EOSAI::CAINationalAlliedAndEnemyPower* pPower = pSummary->m_pPower; // pPowerList->GetNext( pos );
		NationsAndTheirAlliesSortedByPower.InsertSorted( pPower );
	}
	//for( iter = m_NationalSummaries.begin(); iter != m_NationalSummaries.end(); ++iter )
	//{
		//CEOSAINationalSummary* pNationalSummary = *iter;
		//NationsAndTheirAlliesSortedByPower.InsertSorted( pNationalSummary );
	//}

	// Calculate a danger index for each player
	pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pSummary = m_NationalSummaries.GetNext( pos );
		EOSAI::CAINationalAlliedAndEnemyPower* pPower = pSummary->m_pPower; // pPowerList->GetNext( pos );

		pPower->m_fDangerIndex = 0.0f;

		// If the player's power is greater than the average player's, add points
		if( pPower->m_pNationalSummary->m_fPlayerVsAveragePlayerPowerBalance >= 1.0f )
		{
			pPower->m_fDangerIndex += 2.0f*( pPower->m_pNationalSummary->m_fPlayerVsAveragePlayerPowerBalance - 0.5f );
		}

		// If the player (alone) is in the top 0.4 of m_fPlayerVsWorldPowerBalance, add points
		if( pPower->m_pNationalSummary->m_fPlayerVsWorldPowerBalance > 0.35f )
		{
			// +0.0 points for 0.35
			// +1.0 points for 0.4
			// +3.0 points for 0.5
			// +5.0 points for 0.6
			// +7.0 points for 0.7
			pPower->m_fDangerIndex += 10.0f*( pPower->m_pNationalSummary->m_fPlayerVsWorldPowerBalance - 0.35f );
		}

		// If the player is the single most powerful player, add more points
		// If the player is the second most powerful player, add some points
		// If the player is in the top half of players, add a few points
		pPower->m_fDangerIndex += 4.0f / (float)(pSummary->m_iPlayerPowerPosition+1); // 2.0, 1.33, 1.0, 0.8, 0.67, ...
		/*
		long iCount = 0;
		POSITION pos2 = m_SingleNationsSortedByPower.GetHeadPosition();
		while( pos2 )
		{
			iCount++;
			CAINationalAlliedAndEnemyPower* pPowerInList = m_SingleNationsSortedByPower.GetNext( pos2 );
			if( pPowerInList == pPower )
			{
				if( iCount < iNumberOfPlayers/2 )
				{
					float fPos = 4.0f / (float)(iCount+1); // 2.0, 1.33, 1.0, 0.8, 0.67, ...
					pPower->m_fDangerIndex += fPos;
				}
				//if( iCount == 1 ){ pNationalSummary->m_fDangerIndex += 3.0f; }
				//else if( iNumberOfPlayers > 2 && iCount == 2 ){ pNationalSummary->m_fDangerIndex += 2.0f; }
				//else if( iCount < iNumberOfPlayers/2 ){ pNationalSummary->m_fDangerIndex += 1.0f; }
			}
		}
		*/

		// If the player+allies is the most powerful group, add 2.0
		// Add based on the strength of the player+allies
		float fBestScore = NationsAndTheirAlliesSortedByPower.GetHead()->m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution;
		POSITION pos2 = NationsAndTheirAlliesSortedByPower.GetHeadPosition();
		while( pos2 )
		{
			EOSAI::CAINationalAlliedAndEnemyPower* pPowerInList = NationsAndTheirAlliesSortedByPower.GetNext( pos2 );
			if( pPowerInList == pPower )
			{
				if( pPower->m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution == fBestScore )
				{
					pPower->m_fDangerIndex += 1.0f;
				}
				// m_fPlayerAndAlliesVsWorldPowerBalance goes from 0.0 to 1.0, so multiply it
				pPower->m_fDangerIndex += 4.0f*( pPower->m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution );
			}
		}
	}

	m_NationsSortedByDangerIndex.SetSortFunction( SortNationByDangerIndex );
	pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pSummary = m_NationalSummaries.GetNext( pos );
		EOSAI::CAINationalAlliedAndEnemyPower* pPower = pSummary->m_pPower; // pPowerList->GetNext( pos );
		m_NationsSortedByDangerIndex.InsertSorted( pPower );
	}
	int x=0;
}

void CEOSAIForeignRelationsSituation::CalculatePositionTowardsOtherPlayer_Level1Thoughts()
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	POSITION pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pSummary = m_NationalSummaries.GetNext( pos );
		long iPlayer = pSummary->m_iPlayer;

		// Look over this national summary, get the strength of the allies and enemies
		for( long iPotentialTargetPlayer=1; iPotentialTargetPlayer<iNumberOfPlayers+1; iPotentialTargetPlayer++ )
		{
			if( iPlayer == iPotentialTargetPlayer ) continue;

			// I should calculate accessibility some other way
			//   What about WW2 Japan-US?  Neither are accessible under my current definition.
			// Do I run a spatial-map calculation to determine accessibility?
			CalculateLevel1DesiresTowardsPlayer( iPlayer, iPotentialTargetPlayer );
		}
	}

	// Sum-up the values
	pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pPlayerSummary = m_NationalSummaries.GetNext( pos );
		long iPlayer = pPlayerSummary->m_iPlayer;

		pPlayerSummary->m_fOtherPlayersCombinedL1DesireToAttackMe =
			GetCombinedLevel1DesireToAttackPlayer( iPlayer );

		for( long iPotentialTargetPlayer=1; iPotentialTargetPlayer<iNumberOfPlayers+1; iPotentialTargetPlayer++ )
		{
			// A player never has a desire to attack himself, so skip that calculation
			if( iPlayer == iPotentialTargetPlayer ) continue;

			EOSAI::CPositionTowardsOtherPlayer* pPosTowardsPlayer = &pPlayerSummary->m_aPositionTowardsPlayer[ iPotentialTargetPlayer ];
			CEOSAINationalSummary* pOtherPlayerSummary = GetNationalSummary( iPotentialTargetPlayer );

			pPosTowardsPlayer->m_fThisPlayersL1DesireToAttackMe = 
				pOtherPlayerSummary->m_aPositionTowardsPlayer[ iPlayer ].m_fLevel1DesireToAttackPlayer;

			int z=0;
		}
	}
}

void CEOSAIForeignRelationsSituation::CalculateLevel1DesiresTowardsPlayer( long iPlayer, long iPotentialTargetPlayer )
{
	//CAINationalAlliedAndEnemyPower* pPlayerPower = GetAINationalAlliedAndEnemyPower( iPlayer );
	CEOSAINationalSummary* pPlayerSummary = GetNationalSummary( iPlayer );
	EOSAI::CPositionTowardsOtherPlayer* pPosTowardsPlayer = &pPlayerSummary->m_aPositionTowardsPlayer[ iPotentialTargetPlayer ];
	//CAINationalAlliedAndEnemyPower* pTargetPower = GetAINationalAlliedAndEnemyPower( iPotentialTargetPlayer );
	CEOSAINationalSummary* pTargetSummary = GetNationalSummary( iPotentialTargetPlayer );
	//
	pPosTowardsPlayer->m_fPlayerIsGettingTooPowerful = 0.0f;
	//float fPlayerIsTooPowerful = 0.0f;
	if( pTargetSummary->m_fPlayerPower > 0.35f )
	{
		// fPlayerIsTooPowerful = 0.0 if m_fPlayerVsWorldPowerBalance = 0.35
		// fPlayerIsTooPowerful = 0.5 if m_fPlayerVsWorldPowerBalance = 0.40
		// fPlayerIsTooPowerful = 1.0 if m_fPlayerVsWorldPowerBalance = 0.45
		// fPlayerIsTooPowerful = 1.5 if m_fPlayerVsWorldPowerBalance = 0.50
		pPosTowardsPlayer->m_fPlayerIsGettingTooPowerful += 
			10.0f * ( pTargetSummary->m_fPlayerVsWorldPowerBalance - 0.35f );
	}
	if( pTargetSummary->m_fPlayerVsStrongestPlayerPower > 0.6f )
	{
		// fPlayerIsTooPowerful += 0.0 if m_fPlayerVsStrongestPlayerPower = 0.6
		// fPlayerIsTooPowerful += 0.4 if m_fPlayerVsStrongestPlayerPower = 1.0
		pPosTowardsPlayer->m_fPlayerIsGettingTooPowerful += 
			pTargetSummary->m_fPlayerVsStrongestPlayerPower - 0.6f;
	}
	if( pTargetSummary->m_pPower->m_fPlayerAndAlliesVsWorldPowerBalance > 0.4f )
	{
		// fPlayerIsTooPowerful += 0.0 if m_fPlayerAndAlliesVsWorldPowerBalance = 0.4
		// fPlayerIsTooPowerful += 0.5 if m_fPlayerAndAlliesVsWorldPowerBalance = 0.9
		pPosTowardsPlayer->m_fPlayerIsGettingTooPowerful += 
			pTargetSummary->m_pPower->m_fPlayerAndAlliesVsWorldPowerBalance - 0.4f;
	}

	// I should calculate accessibility some other way
	//   What about WW2 Japan-US?  Neither are accessible under my current definition.
	//   My current definition of Accessibility makes no differentiation between distances
	//     of empty ocean versus distances of hostile enemy nations.
	// Do I run a spatial-map calculation to determine accessibility?

	// fPlayerIsNearMe of 1.0  if the player is 0-200 pixels away
	// fPlayerIsNearMe of 0.67 if the player is  400 pixels away
	// fPlayerIsNearMe of 0.5  if the player is  600 pixels away
	// fPlayerIsNearMe of 0.4  if the player is  800 pixels away
	// fPlayerIsNearMe of 0.33 if the player is 1000 pixels away
	//float fPlayerIsNearMe = pPosTowardsPlayer->m_fAccessibility;

	// Player is weak and can be overcome easily
	//float fPlayerIsEasyPrey = 0.0f;
	// Power ratios close to 1.0 are easy prey.  Power ratios near 0.5 are equals.
	pPosTowardsPlayer->m_fSoloPowerRatio = pPlayerSummary->m_fPlayerPower / ( pPlayerSummary->m_fPlayerPower + pTargetSummary->m_fPlayerPower );
	pPosTowardsPlayer->m_fPlayerIsEasyPrey = 0.0f;
	if( pPosTowardsPlayer->m_fSoloPowerRatio > 0.55f )
	{
		// fPlayerIsEasyPrey of 0.5 if the Power ratio is 0.6 (60:40 advantage)
		// fPlayerIsEasyPrey of 1.5 if the Power ratio is 0.7 (70:30 advantage)
		// fPlayerIsEasyPrey of 2.5 if the Power ratio is 0.8 (80:20 advantage)
		pPosTowardsPlayer->m_fPlayerIsEasyPrey = 10.0f*(pPosTowardsPlayer->m_fSoloPowerRatio - 0.55f);
	}

/*
	// Calculate power balance assuming I start a war with this player (0.0-1.0)
	//   Assume that I make peace with my existing enemies, fight this player by myself
	//   0.9 means I strongly overpower him, 0.1 means he overpowers me.
	float fSoloPowerBalance = 
		( pPlayerPower->m_fPlayerPower ) / ( pPlayerPower->m_fPlayerPower + pTargetPower->m_fPlayerPower );

	// What's the power balance if I fight him and all my current enemies?
	float fCurrentEnemyPower = 0.0f;
	bool bAlreadyAtWarWithThatPlayer = false;
	POSITION pos = pPlayerPower->m_pNationalSummary->m_ForeignRelationshipList.GetHeadPosition();
	while( pos )
	{
		CForeignRelationship* pRel = pPlayerPower->m_pNationalSummary->m_ForeignRelationshipList.GetNext( pos );
		if( pRel->GetRelationship() == CForeignRelationship::enum_War )
		{
			CEOSAINationalSummary* pAPlayer = GetNationalSummary( pRel->m_iPlayer );
			fCurrentEnemyPower += pAPlayer->m_fPlayerPower;
			if( pRel->m_iPlayer == iPotentialTargetPlayer )
			{
				bAlreadyAtWarWithThatPlayer = true;
			}
		}
	}
	if( bAlreadyAtWarWithThatPlayer == false )
	{
		fCurrentEnemyPower += pTargetPower->m_fPlayerPower;
	}
	float fAdditionalPowerBalance =
		( pPlayerPower->m_fPlayerPower ) / ( pPlayerPower->m_fPlayerPower + fCurrentEnemyPower );
*/
	//
	//float fFeelingsBasedDesireToAttack = ( 1.0f - pPosTowardsPlayer->m_fFeelings );

	// Levels of warfare:
	//   I go to war with this player by myself, keep fighting all my other enemies, too
	//   I go to war with this player by myself, make peace with my existing enemies
	//   I go to war and get everyone else to attack him too
	//

	// m_fAccessibility of 1.0  if the player is 0-200 pixels away
	// m_fAccessibility of 0.67 if the player is   400 pixels away
	// m_fAccessibility of 0.5  if the player is   600 pixels away
	// m_fAccessibility of 0.4  if the player is   800 pixels away
	// m_fAccessibility of 0.33 if the player is  1000 pixels away
	float fAdjustedAccessibility = (1.5f * pPosTowardsPlayer->m_fAccessibility) - 0.25f;
	if( fAdjustedAccessibility < 0.0f ){ fAdjustedAccessibility = 0.0f; }
	if( fAdjustedAccessibility > 1.0f ){ fAdjustedAccessibility = 1.0f; }

	//
	// Level1 desires include desire to capture isolated players
	//	 Isolated players make good targets because capturing their territory won't increase
	//   my exposure.
	pPosTowardsPlayer->m_fPlayerAccessibilityToWorld = 0.0f;
	CEOSAINationalSummary* pTargetNationalSummary = GetNationalSummary( iPotentialTargetPlayer );
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	for( long i=1; i<iNumberOfPlayers+1; i++ )
	{
		if( i == iPotentialTargetPlayer ) continue; // ignore a players accessibility to self

		// Ignore a potential targets accessibility to player (because, in this case,
		//   increased accessibilty is bad)
		if( i == iPlayer ) continue;

		if( pTargetNationalSummary->m_aPositionTowardsPlayer[i].m_fAccessibility > 0.75f )
		{
			pPosTowardsPlayer->m_fPlayerAccessibilityToWorld += 1.0f;
		}
	}
	// Final value of fWorldAccessibility = 0.5 lots of neighbors, 1.5 few neighbors
	float fWorldAccessibility = pPosTowardsPlayer->m_fPlayerAccessibilityToWorld / (float) iNumberOfPlayers;
	fWorldAccessibility = 1.5f - fWorldAccessibility; // 0.5-1.5

	// fAdjustedFeelings of 2.0 means I want to attack this guy
	// fAdjustedFeelings of 0.0 means I don't want to attack this guy (he's probably an ally)
	float fBadFeelingsTowardsPlayer = 2.0f * ( 1.0f - pPosTowardsPlayer->m_fFeelings );

	pPosTowardsPlayer->m_fLevel1DesireToAttackPlayer = 
		max( pPosTowardsPlayer->m_fPlayerIsGettingTooPowerful,
			 pPosTowardsPlayer->m_fPlayerIsEasyPrey * fAdjustedAccessibility * fWorldAccessibility );

	// If the player isn't too powerful or weak, base the desire to attack on the accessibility.
	//   If the player is more powerful than me, I'll need to try to get allies to go in with me.
	pPosTowardsPlayer->m_fLevel1DesireToAttackPlayer = 
		max( pPosTowardsPlayer->m_fLevel1DesireToAttackPlayer, fAdjustedAccessibility );

	pPosTowardsPlayer->m_fLevel1DesireToAttackPlayer += fBadFeelingsTowardsPlayer;
}
/*
void CEOSAIForeignRelationsSituation::NormalizeDesiresTowardsPlayers()
{

}
*/
/*
void CEOSAIForeignRelationsSituation::CalculatePositionTowardsOtherPlayer_Level2Thoughts()
{
	long iMyPlayerNumber = GetAIPlayerNumber();
	long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();

	float fL1DesireToAttackPlayer1 = GetCombinedLevel1DesireToAttackPlayer( 1 );
	float fL1DesireToAttackPlayer2 = GetCombinedLevel1DesireToAttackPlayer( 2 );

	float fL1DesireToAttackPlayer3 = 0.0f;
	if( iNumberOfPlayers >= 3 )
	{
		fL1DesireToAttackPlayer3 = GetCombinedLevel1DesireToAttackPlayer( 3 );
	}
	float fL1DesireToAttackPlayer4 = 0.0f;
	if( iNumberOfPlayers >= 4 )
	{
		fL1DesireToAttackPlayer4 = GetCombinedLevel1DesireToAttackPlayer( 3 );
	}

	// Initialize the Level2 values
	POSITION pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pPlayerSummary = m_NationalSummaries.GetNext( pos );
		long iPlayer = pPlayerSummary->m_iPlayer;

		pPlayerSummary->m_fOtherPlayersCombinedL1DesireToAttackMe =
			GetCombinedLevel1DesireToAttackPlayer( iPlayer );

		for( long iPotentialTargetPlayer=1; iPotentialTargetPlayer<iNumberOfPlayers+1; iPotentialTargetPlayer++ )
		{
			// A player never has a desire to attack himself, so skip that calculation
			if( iPlayer == iPotentialTargetPlayer ) continue;

			CPositionTowardsOtherPlayer* pPosTowardsPlayer = &pPlayerSummary->m_aPositionTowardsPlayer[ iPotentialTargetPlayer ];
			CEOSAINationalSummary* pOtherPlayerSummary = GetNationalSummary( iPotentialTargetPlayer );

			pPosTowardsPlayer->m_fThisPlayersL1DesireToAttackMe = 
				pOtherPlayerSummary->m_aPositionTowardsPlayer[ iPlayer ].m_fLevel1DesireToAttackPlayer;

			int z=0;
		}
	}
	int z=0;
}
*/

void CEOSAIForeignRelationsSituation::CalculatePositionTowardsOtherPlayer_WarsAndAlliancesEffects()
{
	// Players with lots of allies make bad targets
	// Players with lots of enemies make better targets

	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();

	POSITION pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pPlayerSummary = m_NationalSummaries.GetNext( pos );
		EOSAI::CAINationalAlliedAndEnemyPower* pPower = pPlayerSummary->m_pPower; // pPowerList->GetNext( pos );
		long iPlayer = pPlayerSummary->m_iPlayer;

		// Look over this national summary, get the strength of the allies and enemies
		for( long iPotentialTargetPlayer=1; iPotentialTargetPlayer<iNumberOfPlayers+1; iPotentialTargetPlayer++ )
		{
			// A player never has a desire to attack himself, so skip that calculation
			if( iPlayer == iPotentialTargetPlayer ) continue;

			CEOSAINationalSummary* pOtherPlayerSummary = GetNationalSummary( iPotentialTargetPlayer );
			EOSAI::CPositionTowardsOtherPlayer* pPosTowardsPlayer = &pPlayerSummary->m_aPositionTowardsPlayer[ iPotentialTargetPlayer ];

			/*
			if( pPlayerSummary->GetForeignRelations( iPotentialTargetPlayer ) == EOSAIEnumForeignRelations::enum_PermanentAlliance )
			{
				pPosTowardsPlayer->m_fLevel3DesireToAttackPlayer = 0.0f;
			}
			else
			*/
			{
				/*
				// What's the PotentialTarget's current balance excluding my contribution?
				CHypotheticalForeignPowerBalance  HypotheticalPowerBalance( this );
				HypotheticalPowerBalance.SetToNeutral( iPlayer, iPotentialTargetPlayer );
				HypotheticalPowerBalance.CalculatePowerBalance();
				float fCurrentPowerBalance = HypotheticalPowerBalance.GetPowerBalance( iPotentialTargetPlayer );
				pPosTowardsPlayer->m_fTargetPlayersCurrentPowerBalanceExcludingMyContribution = fCurrentPowerBalance;

				/-*
				// What would our power balances be if we went to war?
				HypotheticalPowerBalance.SetToEnemies( iPlayer, iPotentialTargetPlayer );
				HypotheticalPowerBalance.CalculatePowerBalance();
				pPosTowardsPlayer->m_fMyPowerBalanceIfIAddedThisPlayerAsAnEnemy = 
					HypotheticalPowerBalance.GetPowerBalance( iPlayer );
				pPosTowardsPlayer->m_fTargetPlayersPowerBalanceIfIAddedThisPlayerAsAnEnemy = 
					HypotheticalPowerBalance.GetPowerBalance( iPotentialTargetPlayer );
				*-/

				// DesireToAttackPlayer += 4 if Balance is 0.2
				// DesireToAttackPlayer += 2.5 if Balance is 0.5
				// DesireToAttackPlayer += 1 if Balance is 0.8
				pPosTowardsPlayer->m_fLevel3DesireToAttackPlayer =
					pPosTowardsPlayer->GetLevel2DesireToAttackPlayer() + 
					5.0f * ( 1.0 - fCurrentPowerBalance );
				*/
			}
		}
	}
}

// Look over my Level3 desires to attack, think about the power balance, and come up with
//   some options (attack by myself, try to make peace with my enemies, try to get other
//   people to gang-up on a player)
void CEOSAIForeignRelationsSituation::CreateForeignRelationsPlanForThePlayer()
{
	long iMyPlayerNumber = GetAIPlayerNumber();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();

	// Order the attack desires for each player
	//   Each player considers only the top desire and any desire within 1/2 of its score
	/*
	POSITION pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pPlayerSummary = m_NationalSummaries.GetNext( pos );
		long iPlayer = pPlayerSummary->m_iPlayer;

		pPlayerSummary->m_fOtherPlayersCombinedL3DesireToAttackMe = 0.0f;
		for( long iPotentialAttacker=1; iPotentialAttacker<iNumberOfPlayers+1; iPotentialAttacker++ )
		{
			// A player never has a desire to attack himself, so skip that idea
			if( iPlayer == iPotentialAttacker ) continue;
			CEOSAINationalSummary* pPotentialAttacker = GetNationalSummary( iPotentialAttacker );

			//AttackIdeasSortedByL3DesireList.InsertSorted( &pPlayerSummary->m_aPositionTowardsPlayer[iPotentialTargetPlayer] );
			float fDesire = pPotentialAttacker->m_aPositionTowardsPlayer[iPlayer].m_fLevel3DesireToAttackPlayer;
			pPlayerSummary->m_fOtherPlayersCombinedL3DesireToAttackMe += fDesire;
		}
	}
	*/

	// Who is everyone's favorite targets?
	//   I need to know this in order to figure out who is thinking about attacking me.

	// What's my prefered target?
	CEOSAINationalSummary* pPlayerSummary = GetNationalSummary( GetAIPlayerNumber() );
	long iMyTopPotentialTarget = pPlayerSummary->GetPotentialTarget( 1 );
	long iMyTopPotentialTarget2 = pPlayerSummary->GetPotentialTarget( 2 );
	/*
	for( long iPotentialTarget=1; iPotentialTarget<iNumberOfPlayers+1; iPotentialTarget++ )
	{
		if( iPotentialTarget == iMyPlayerNumber ) continue;
		if( pPlayerSummary->m_aPositionTowardsPlayer[iPotentialTarget].m_fLevel3DesireToAttackPlayer > fTopTargetDesire )
		{
			fTopTargetDesire = pPlayerSummary->m_aPositionTowardsPlayer[iPotentialTarget].m_fLevel3DesireToAttackPlayer;
			iMyTopPotentialTarget = iPotentialTarget;
		}
	}
	*/

	// If I am already at war with this player, then there's nothing to change - actually, there is:
	//   I can get other players to join in and make peace with my current enemies
	//if( pPlayerSummary->GetForeignRelationship( iMyTopPotentialTarget ) == EOSAIEnumForeignRelations::enum_War )
	//{
		// Exit out of this function?
		//   I could consider attacking my #2 target, but only if I'm overly-powerful
		//   (Even if I'm more powerful than both players combined, I don't want to weaken
		//   myself to the point that a fourth player can take me.)
		//int z=0;
	//}
/*
	// What if I attacked this player (making no other changes to my or anyones foreign relations)?
	CHypotheticalForeignPowerBalance  HypotheticalPowerBalance( this );
	//HypotheticalPowerBalance.ResetForeignRelations(); // (debug) Set everyone to neutral
	HypotheticalPowerBalance.SetToEnemies( iMyTopPotentialTarget, iMyPlayerNumber );
	//HypotheticalPowerBalance.SetToEnemies( 1, 2 );
	//HypotheticalPowerBalance.SetToEnemies( 1, 3 );
	//HypotheticalPowerBalance.SetToNeutral( 3, 2 );
	HypotheticalPowerBalance.CalculatePowerBalance();
	float fMyPowerBalanceIfISimplyDeclareWar = HypotheticalPowerBalance.GetPowerBalance( iMyPlayerNumber );

	bool bGoingToStartThisWar1 = false;
	bool bWantToStartWarByMyself = false;
	bool bCouldUseSomeAlliesButDontNeedThem = false;
	bool bNeedSomeAlliesIfImGoingToDoThis = false;
	if( ( pPlayerSummary->m_fCurrentWarPowerBalance < 1.0f && fMyPowerBalanceIfISimplyDeclareWar > 0.7f ) ||
		fMyPowerBalanceIfISimplyDeclareWar > 0.6f )
	{
		// I can actually take this war on without changing anything
		bGoingToStartThisWar1 = true;
	}

	// What if I made peace with my existing enemies?
	m_PlayersWhoMightBeWillingToMakePeace.Clear();
	for( long iSomePlayer=1; iSomePlayer<iNumberOfPlayers+1; iSomePlayer++ )
	{
		if( iSomePlayer == iMyPlayerNumber ) continue;
		if( iSomePlayer == iMyTopPotentialTarget ) continue;

		if( HypotheticalPowerBalance.AreEnemies( iSomePlayer, iMyPlayerNumber ) )
		{
			// Would they make peace?
			// There are two main reasons they might make peace:
			//   more afraid of another player than they are of me
			//   I am winning the war and they want out
			// Note: With human players, I can get a sense of their willingness to make peace,
			//   but I have to ask them.
			CEOSAINationalSummary* pSomePlayerSummary = GetNationalSummary( iSomePlayer );
			long iSomePlayerTopPotentialTarget = pSomePlayerSummary->GetPotentialTarget( 1 );

			if( iSomePlayerTopPotentialTarget != iMyPlayerNumber )
			{
				HypotheticalPowerBalance.SetToNeutral( iSomePlayer, iMyPlayerNumber );
				m_PlayersWhoMightBeWillingToMakePeace.Add( iSomePlayer );
			}
		}
	}
	float fMyPowerBalanceIfIMakePeaceWithExistingEnemies = fMyPowerBalanceIfISimplyDeclareWar;
	if( m_PlayersWhoMightBeWillingToMakePeace.GetSize() > 0 )
	{
		HypotheticalPowerBalance.CalculatePowerBalance();
		fMyPowerBalanceIfIMakePeaceWithExistingEnemies = HypotheticalPowerBalance.GetPowerBalance( iMyPlayerNumber );
	}

	// What if I got other players to declare war on this guy?
	m_PlayersWhoMightBeWillingToJoinMeInWar.Clear();
	for( long iSomePlayer=1; iSomePlayer<iNumberOfPlayers+1; iSomePlayer++ )
	{
		if( iSomePlayer == iMyPlayerNumber ) continue;
		if( iSomePlayer == iMyTopPotentialTarget ) continue;

		//CEOSAINationalSummary* pSomePlayerSummary = GetNationalSummary( iSomePlayer );
		//if( pSomePlayerSummary->GetForeignRelationship( iMyTopPotentialTarget ) != EOSAIEnumForeignRelations::enum_War )
		if( HypotheticalPowerBalance.AreEnemies( iSomePlayer, iMyTopPotentialTarget ) == false )
		{
			// Would they make war?
			//   Other players might wage war if they are worried about the third player
			//   or if the third player represents a decent target

			CEOSAINationalSummary* pSomePlayerSummary = GetNationalSummary( iSomePlayer );
			//long iSomePlayerTopPotentialTarget = pSomePlayerSummary->GetPotentialTarget( 1 );
			CPlayer* pPlayer = GetCommonState()->GetPlayer( iSomePlayer );

			// If this is a human player, we might not estimate this correctly, so give some
			//   extra "wiggle-room".
			bool bMightJoinMeInWar = false;
			if( pPlayer->IsHuman() )
			{
				if( pSomePlayerSummary->GetPotentialTarget( 1 ) == iMyTopPotentialTarget ||
					pSomePlayerSummary->GetPotentialTarget( 2 ) == iMyTopPotentialTarget )
				{
					bMightJoinMeInWar = true;
				}
			}
			else
			{
				if( pSomePlayerSummary->GetPotentialTarget( 1 ) == iMyTopPotentialTarget )
				{
					bMightJoinMeInWar = true;
				}
			}
			if( bMightJoinMeInWar )
			{
				//bSomePlayersAreWillingToMakeWar = true;
				HypotheticalPowerBalance.SetToEnemies( iSomePlayer, iMyTopPotentialTarget );
				m_PlayersWhoMightBeWillingToJoinMeInWar.Add( iSomePlayer );
			}
		}
	}
	float fMyPowerBalanceIfIMakePeaceAndGetAllOtherPlayersToDeclareWar = fMyPowerBalanceIfIMakePeaceWithExistingEnemies;
	if( m_PlayersWhoMightBeWillingToJoinMeInWar.GetSize() > 0 )
	{
		HypotheticalPowerBalance.CalculatePowerBalance();
		fMyPowerBalanceIfIMakePeaceAndGetAllOtherPlayersToDeclareWar = HypotheticalPowerBalance.GetPowerBalance( iMyPlayerNumber );
	}

	// What about making allies?
	//   (What's the logic for making allies?)
	// Want allies that don't change sides frequently
	// Allies who will have similar goals to me (i.e. common apparent enemy/neighbor)
	//   e.g. France's historical alliance with Russia against Germany
	// Don't want to box myself in with allies - I want some neighbor that I can attack
	// Basically, allies are the players I am least likely to declare war on
	//   (except I want to chose one or more neighbors to keep as potential enemies)
	m_PotentialAlliesSet.Clear();
	float fTopPotentialPlayerTarget = 0.0f;
	long iCount = 0;
	POSITION pos = pPlayerSummary->GetSortedListOfPotentialTargets()->GetHeadPosition();
	while( pos )
	{
		CPositionTowardsOtherPlayer* pPos = pPlayerSummary->GetSortedListOfPotentialTargets()->GetNext( pos );

		if( pPos->m_iTargetPlayer == 0 )
		{
			continue; // skip the "peace" option for allies
		}

		iCount++;
		if( iCount == 1 )
		{
			fTopPotentialPlayerTarget = pPos->m_fLevel3DesireToAttackPlayer;
			continue;
		}

		// Top potential target is 1.0.  Bottom potential target is 0.0.
		float fPercentile = 0.0f;
		if( iNumberOfPlayers == 2 )
		{
			fPercentile = 1.0f;
		}
		else
		{
			// if iCount = 1, iNumberOfPlayers = 3 then fPercentile = 1.0
			// if iCount = 2, iNumberOfPlayers = 3 then fPercentile = 0.0
			// if iCount = 1, iNumberOfPlayers = 4 then fPercentile = 1.0
			// if iCount = 2, iNumberOfPlayers = 4 then fPercentile = 0.5
			// if iCount = 3, iNumberOfPlayers = 4 then fPercentile = 0.0
			fPercentile = (float)((iNumberOfPlayers-1) - iCount) / (float)(iNumberOfPlayers-2);
		}

		//
		pPos->m_fValueAsAnAlly = 1.0f - fPercentile;

		// The first one is my top potential target.  Don't make allies with them.
		//   The second one is the next potential target (may or may not want to become allies)
		//   If this is a three-person game, I want to make them my ally.
		//   If this is a twenty-person game, I probably don't want to make them an ally.
		//   More willing to attack allies of enemies, enemies of allies
		//   More willing to make peace with allies of allies, enemies of enemies
		if( fPercentile < 0.75f )
		{
			// Try to make allies with these players?
			m_PotentialAlliesSet.Add( pPos->m_iTargetPlayer );
		}
	}

	int z=0;

	// I may or may not go to war with this player (depending on my neighbors willingness to join)
	m_iWouldLikeToAttackPlayer = iMyTopPotentialTarget;
	m_fMyPowerBalanceIfISimplyDeclareWar = fMyPowerBalanceIfISimplyDeclareWar;
	m_fMyPowerBalanceIfIMakePeaceWithExistingEnemies = fMyPowerBalanceIfIMakePeaceWithExistingEnemies;
	m_fMyPowerBalanceIfIMakePeaceAndGetAllOtherPlayersToDeclareWar = fMyPowerBalanceIfIMakePeaceAndGetAllOtherPlayersToDeclareWar;

	m_PlayersWhoMightBeWillingToMakePeace.Add( 1 );

	// Who am I going to attack?
	m_MyEnemiesOrSoonToBeEnemies.Clear();
	m_iPlanToAttackPlayer = 0;
	if( m_fMyPowerBalanceIfISimplyDeclareWar > 0.6f )
	{
		m_iPlanToAttackPlayer = iMyTopPotentialTarget;
		m_MyEnemiesOrSoonToBeEnemies.Add( m_iPlanToAttackPlayer );
	}

	for( long iSomePlayer=1; iSomePlayer<iNumberOfPlayers+1; iSomePlayer++ )
	{
		if( iSomePlayer == iMyPlayerNumber ) continue;

		if( pPlayerSummary->GetForeignRelations( iSomePlayer ) == EOSAIEnumForeignRelations::enum_War )
		{
			m_MyEnemiesOrSoonToBeEnemies.Add( iSomePlayer );
		}

		CEOSAINationalSummary* pSomePlayerSummary = GetNationalSummary( iSomePlayer );
		if( pSomePlayerSummary->GetForeignRelations( iMyPlayerNumber ) == EOSAIEnumForeignRelations::enum_War )
		{
			m_MyEnemiesOrSoonToBeEnemies.Add( iSomePlayer );
		}
	}
	//m_MyEnemiesOrSoonToBeEnemies.Add( m_iPlanToAttackPlayer );
*/

	/*
	// What would our power balances be if we went to war?
	HypotheticalPowerBalance.SetToEnemies( iPlayer, iPotentialTargetPlayer );
	HypotheticalPowerBalance.CalculatePowerBalance();
	pPosTowardsPlayer->m_fMyPowerBalanceIfIAddedThisPlayerAsAnEnemy = 
		HypotheticalPowerBalance.GetPowerBalance( iPlayer );
	pPosTowardsPlayer->m_fTargetPlayersPowerBalanceIfIAddedThisPlayerAsAnEnemy = 
		HypotheticalPowerBalance.GetPowerBalance( iPotentialTargetPlayer );
	*/

	// Order all the attack desires for all the players
	/*
	CEOSAIListSort< CEOSAINationalSummary* >  NationalSummariesSortedByL3DesireList;
	NationalSummariesSortedByL3DesireList.SetSortFunction( NationalSummariesSortedByL3Desire );
	pos = m_NationalSummaries.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pPlayerSummary = m_NationalSummaries.GetNext( pos );
		NationalSummariesSortedByL3DesireList.InsertSorted( pPlayerSummary );
	}
	*/

	// Iterate over the sorted list
	/*
	bool bAPlayerIsGoingToAttack = false;
	pos = NationalSummariesSortedByL3DesireList.GetHeadPosition();
	while( pos )
	{
		CEOSAINationalSummary* pPotdentialTargetSummary = NationalSummariesSortedByL3DesireList.GetNext( pos );
		long iPotentialTargetPlayer = pPotentialTargetSummary->m_iPlayer;

		// What kind of support can I get for attacking this player?
		for( long iPotentialAttacker=1; iPotentialAttacker<iNumberOfPlayers+1; iPotentialAttacker++ )
		{
			// A player never has a desire to attack himself, so skip that idea
			if( iPotentialTargetPlayer == iPotentialAttacker ) continue;

			CEOSAINationalSummary* pPotentialAttackerSummary = GetNationalSummary( iPotentialAttacker );
		}
	}
	*/
}


void CEOSAIForeignRelationsSituation::TalkToOtherPlayersAboutCooperating()
{
	if( m_iWouldLikeToAttackPlayer != 0 && m_iPlanToAttackPlayer == 0 )
	{
		/*
		CAIUnresolvedSituation_ForeignRelationPlan* pUnresolvedSituation = 
			new CAIUnresolvedSituation_ForeignRelationPlan( m_pAIPlayer );
		m_pAIPlayer->GetAIBrain()->AddToUnresolvedForeignRelationsOffers( pUnresolvedSituation );

		// Send messages to other players

		// Make peace with some players?
		POSITION pos = m_PlayersWhoMightBeWillingToMakePeace.GetList()->GetHeadPosition();
		while( pos )
		{
			long iSendTo = m_PlayersWhoMightBeWillingToMakePeace.GetList()->GetNext( pos );
		}
		*/
	}
}


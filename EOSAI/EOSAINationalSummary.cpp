
#include "stdafx.h"
#include "EOSAINationalSummary.h"
//#include "WorldDescServer.h"
//#include "WorldDescPlayerProxy.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIForeignRelationsSituation.h"
#include "EOSAIGamePlayer.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace EOSAI;

void CPositionTowardsOtherPlayer::AdjustFeelings( float fValue, float fWeight )
{
	//float fCurrentFeelings = m_fFeelings;
	//float fCurrentFeelingsWeight = m_fFeelingsWeight;

	//m_fFeelings = 
	//	( fCurrentFeelings * fCurrentFeelingsWeight + fValue * fWeight ) / ( fCurrentFeelingsWeight + fWeight );
	m_fFeelings =
		( m_fFeelings * m_fFeelingsWeight + fValue * fWeight ) / ( m_fFeelingsWeight + fWeight );
	m_fFeelingsWeight += fWeight;
}

// Largest values come first
bool PosTowardsPlayerSortedByL3Desire( CPositionTowardsOtherPlayer* pPos1, CPositionTowardsOtherPlayer* pPos2 )
{
	if( pPos1->m_fLevel3DesireToAttackPlayer > pPos2->m_fLevel3DesireToAttackPlayer )
	{
		return true;
	}
	else if( pPos1->m_fLevel3DesireToAttackPlayer < pPos2->m_fLevel3DesireToAttackPlayer )
	{
		return false;
	}
	return true;  // always return true when values are equal
}


float CPositionTowardsOtherPlayer::GetLevel2DesireToAttackPlayer()
{
	CEOSAINationalSummary* pOtherPlayerSummary = 
		m_pNationalSummary->m_pAIForeignRelationsSituation->GetNationalSummary( m_iTargetPlayer );
	return 
		m_fLevel1DesireToAttackPlayer +
		m_fThisPlayersL1DesireToAttackMe +
		0.5f*pOtherPlayerSummary->m_fOtherPlayersCombinedL1DesireToAttackMe;
}

//CEOSAINationalSummary::CEOSAINationalSummary( CWorldDescServer* pWorldDescServer, CEOSAIForeignRelationsSituation* pAIForeignRelationsSituation, long iPlayer )
CEOSAINationalSummary::CEOSAINationalSummary( CEOSAIForeignRelationsSituation* pAIForeignRelationsSituation, long iPlayer )
{
	m_pAIForeignRelationsSituation = pAIForeignRelationsSituation;
	m_iPlayer = iPlayer;
	//
	m_pPower = NULL;
	//
	//long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	m_aPositionTowardsPlayer.SetSize( iNumberOfPlayers+1 );
	for( long i=0; i<iNumberOfPlayers+1; i++ )
	{
		m_aPositionTowardsPlayer[i].m_pNationalSummary = this;
		m_aPositionTowardsPlayer[i].m_iTargetPlayer = i;
	}
	/*
	m_aPositionTowardsPlayer = new CPositionTowardsOtherPlayer[ iNumberOfPlayers+1 ];
	for( long i=0; i<iNumberOfPlayers+1; i++ )
	{
		m_aPositionTowardsPlayer[i].m_pNationalSummary = this;
		m_aPositionTowardsPlayer[i].m_iTargetPlayer = i;
	}
	*/
	//
	m_fPlayerCombatPower = 0.0f;
	m_fPlayerPower = 0.0f;
	m_fPlayerVsWorldPowerBalance = 0.0f;
	m_fPlayerVsAveragePlayerPowerBalance = 0.0f;
	m_fPlayerVsStrongestPlayerPower = 0.0f;
	m_iPlayerPowerPosition = 0;

	m_fCurrentWarPowerSum = 0.0f;
	m_fCurrentWarPowerBalance = 0.0f;

	m_fDesireForPeace = 0.0f;

	m_fOtherPlayersCombinedL1DesireToAttackMe = 0.0f;
	m_fOtherPlayersCombinedL3DesireToAttackMe = 0.0f;

	// Copy information from the pWorldDescServer

	// Get the foreign relations information
	//CWorldDescPlayerProxy* pWorldDescPlayerProxy = pWorldDescServer->GetWorldDescPlayerProxy( iPlayer );
	//ASSERT( pWorldDescPlayerProxy );
	//if( pWorldDescPlayerProxy )
	{
		long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		for( long i=1; i<=iNumberOfPlayers; i++ )
		{
			if( iPlayer == i ) continue;
			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayer,i );
			//SetForeignRelations( i, eRel );
			m_ForeignRelations[ i ] = eRel;
		}
		/*
		CList< EOSAIEnumForeignRelations* >* pForeignRelationshipList = pWorldDescPlayerProxy->GetForeignRelationshipList();
		POSITION pos = pForeignRelationshipList->GetHeadPosition();
		while( pos )
		{
			EOSAIEnumForeignRelations* pForeignRel = pForeignRelationshipList->GetNext( pos );
			SetForeignRelationship( pForeignRel->m_iPlayer, pForeignRel->m_Relationship );
		}
		*/
	}

	CalculateAccessibility( iPlayer );
	// Find all my CitRes first
	/*
	CList< CPoiObject* > PlayerPoiObjectList;
	POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CCity* pCity = dynamic_cast< CCity* >( pPoi );
		if( pCity && pCity->GetOwner() == iPlayer )
		{
			PlayerPoiObjectList.AddTail( pCity );
		}
		CResourceSource* pResource = dynamic_cast< CResourceSource* >( pPoi );
		if( pResource && pResource->GetOwner() == iPlayer )
		{
			PlayerPoiObjectList.AddTail( pCity );
		}
	}
	// Find distances to other players
	for( long iOtherPlayer=1; iOtherPlayer<iNumberOfPlayers+1; iOtherPlayer++ )
	{
		if( iOtherPlayer == iPlayer ) continue; // don't calculate accessibility for self

		CList< CPoiObject* > OtherPlayerPoiObjectList;
		POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
		while( pos )
		{
			CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
			CCity* pCity = dynamic_cast< CCity* >( pPoi );
			if( pCity && pCity->GetOwner() == iOtherPlayer )
			{
				OtherPlayerPoiObjectList.AddTail( pCity );
			}
			CResourceSource* pResource = dynamic_cast< CResourceSource* >( pPoi );
			if( pResource && pResource->GetOwner() == iOtherPlayer )
			{
				OtherPlayerPoiObjectList.AddTail( pCity );
			}
		}
		// Find distances
		float fTotalWeight = 0.0f;
		float fWeightedDistance = 0.0f;
		pos = PlayerPoiObjectList.GetHeadPosition();
		while( pos )
		{
			CPoiObject* pPlayerPoiObject = PlayerPoiObjectList.GetNext( pos );
			POSITION pos2 = OtherPlayerPoiObjectList.GetHeadPosition();
			while( pos2 )
			{
				CPoiObject* pOtherPlayerPoiObject = OtherPlayerPoiObjectList.GetNext( pos2 );
				float fDistance = pWorldDescServer->GetPixelDistance( pPlayerPoiObject, pOtherPlayerPoiObject );

				// We want closer CitRes to be weighted heavier
				//   Otherwise, more distant CitRes (mine or his) can throw-off the average distance
				// fDistance of    0 = weight of 2
				// fDistance of  400 = weight of 0.67
				// fDistance of  800 = weight of 0.4
				// fDistance of 1200 = weight of 0.29
				float fWeight = 400.0f / (fDistance+200.0f);
				fWeightedDistance =
					( fDistance*fWeight + fWeightedDistance*fTotalWeight ) / ( fWeight + fTotalWeight );
				fTotalWeight += fWeight;
			}
		}

		// fWeightedDistance of   0 = fAccessibility of 1
		// fWeightedDistance of 200 = fAccessibility of 1
		// fWeightedDistance of 400 = fAccessibility of 0.67
		// fWeightedDistance of 600 = fAccessibility of 0.5
		// fWeightedDistance of 800 = fAccessibility of 0.4
		float fAccessibility = 400.0f/(fWeightedDistance+200.0f);
		if( fAccessibility > 1.0f ){ fAccessibility = 1.0f; }
		m_aPositionTowardsPlayer[ iOtherPlayer ].m_fAccessibility = fAccessibility;
	}
	*/

	//pWorldDescServer->AICheat_GetPlayerCitResSummary( iPlayer, &m_CitResSummary );
	//pWorldDescServer->AICheat_GetPlayerUnitSummary( iPlayer, &m_UnitSummary );

	m_fPlayerCombatPower = m_UnitSummary.GetTotalCombatPower();
}

CEOSAINationalSummary::~CEOSAINationalSummary()
{
	delete m_pPower;
	//delete[] m_aPositionTowardsPlayer;
	/*
	while( m_ForeignRelationshipList.IsEmpty() == FALSE )
	{
		delete m_ForeignRelationshipList.RemoveHead();
	}
	*/
}

CAINationalAlliedAndEnemyPower* CEOSAINationalSummary::AllocateAINationalAlliedAndEnemyPowerObject()
{
	delete m_pPower;
	m_pPower = new CAINationalAlliedAndEnemyPower();
	m_pPower->m_pNationalSummary = this;

	// Copy the Relationship information into the AINationalPower object
	m_pPower->SetForeignRelationsToActualValues();

	// Copy the player power information
	//m_pPower->m_fPlayerCombatPower = m_fPlayerCombatPower;
	//m_pPower->m_fPlayerProduction = m_CitResSummary.m_fCityProduction;
	//m_pPower->m_fPlayerPower = m_fPlayerPower;
	//m_pPower->m_fPlayerVsAveragePlayerPowerBalance = m_fPlayerVsAveragePlayerPowerBalance;
	//m_pPower->m_fPlayerVsWorldPowerBalance = m_fPlayerVsWorldPowerBalance;

	return m_pPower;
}

CPositionTowardsOtherPlayer*  CEOSAINationalSummary::GetPositionTowardsPlayer( long iPlayer )
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	ASSERT( iPlayer > 0 );
	ASSERT( iPlayer <= iNumberOfPlayers );
	return &m_aPositionTowardsPlayer[ iPlayer ];
}

// 0 = would like peace/cease-fire, otherwise it returns the player number
void CEOSAINationalSummary::SortPotentialTargets()
{
	m_SortedListOfPotentialTargets.SetSortFunction( PosTowardsPlayerSortedByL3Desire );

	// I set this up this way for convenience
	m_aPositionTowardsPlayer[0].m_fLevel3DesireToAttackPlayer = m_fDesireForPeace;
	m_aPositionTowardsPlayer[0].m_iTargetPlayer = 0;

	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	for( long iSomePlayer=0; iSomePlayer<=iNumberOfPlayers; iSomePlayer++ )
	{
		if( iSomePlayer == m_iPlayer ) continue;
		CPositionTowardsOtherPlayer* pPos = &m_aPositionTowardsPlayer[iSomePlayer];
		m_SortedListOfPotentialTargets.InsertSorted( pPos );
	}
}

CEOSAIListSort< CPositionTowardsOtherPlayer* >*  CEOSAINationalSummary::GetSortedListOfPotentialTargets()
{
	if( m_SortedListOfPotentialTargets.IsEmpty() )
	{
		SortPotentialTargets();
	}
	return &m_SortedListOfPotentialTargets;
}

long  CEOSAINationalSummary::GetPotentialTarget( long iPosition )
{
	if( m_SortedListOfPotentialTargets.IsEmpty() )
	{
		SortPotentialTargets();
	}

	long iPos = 0;
	POSITION pos = m_SortedListOfPotentialTargets.GetHeadPosition();
	while( pos )
	{
		CPositionTowardsOtherPlayer* pPos = m_SortedListOfPotentialTargets.GetNext( pos );
		iPos++;
		if( iPos == iPosition )
		{
			return pPos->m_iTargetPlayer;
		}
	}
	ASSERT( false );
	return -1;
/*
	if( iPosition == 1 )
	{
		// An optimized algorithm that only works when iPosition == 1
		long  iPreferedTarget = 0;
		float fPreferedTargetValue = m_fDesireForPeace;

		long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();
		for( long iSomePlayer=1; iSomePlayer<=iNumberOfPlayers; iSomePlayer++ )
		{
			if( m_aPositionTowardsPlayer[iSomePlayer].m_fLevel3DesireToAttackPlayer > fPreferedTargetValue )
			{
				iPreferedTarget = iSomePlayer;
				fPreferedTargetValue = m_aPositionTowardsPlayer[iSomePlayer].m_fLevel3DesireToAttackPlayer;
			}
		}
		return iPreferedTarget;
	}

	// I set this up this way for convenience
	m_aPositionTowardsPlayer[0].m_fLevel3DesireToAttackPlayer = m_fDesireForPeace;
	m_aPositionTowardsPlayer[0].m_iTargetPlayer = 0;

	CEOSAIListSort< CPositionTowardsOtherPlayer* >  SortedList;
	SortedList.SetSortFunction( PosTowardsPlayerSortedByL3Desire );

	long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();
	for( long iSomePlayer=0; iSomePlayer<=iNumberOfPlayers; iSomePlayer++ )
	{
		if( iSomePlayer == m_iPlayer ) continue;
		CPositionTowardsOtherPlayer* pPos = &m_aPositionTowardsPlayer[iSomePlayer];
		SortedList.InsertSorted( pPos );
	}

	long iPos = 0;
	POSITION pos = SortedList.GetHeadPosition();
	while( pos )
	{
		CPositionTowardsOtherPlayer* pPos = SortedList.GetNext( pos );
		iPos++;
		if( iPos == iPosition )
		{
			return pPos->m_iTargetPlayer;
		}
	}
	ASSERT( false );
	return -1;
*/

/*
	long  iPreferedTarget = 0;
	float fPreferedTargetValue = m_fDesireForPeace;

	long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();
	for( long iSomePlayer=1; iSomePlayer<=iNumberOfPlayers; iSomePlayer++ )
	{
		if( m_aPositionTowardsPlayer[iSomePlayer].m_fLevel3DesireToAttackPlayer > fPreferedTargetValue )
		{
			iPreferedTarget = iSomePlayer;
			fPreferedTargetValue = m_aPositionTowardsPlayer[iSomePlayer].m_fLevel3DesireToAttackPlayer;
		}
	}
	return iPreferedTarget;
*/
}


EOSAIEnumForeignRelations  CEOSAINationalSummary::GetForeignRelations( long iOtherPlayer )
{
	/*
	POSITION pos = m_ForeignRelationshipList.GetHeadPosition();
	while( pos )
	{
		EOSAIEnumForeignRelations* pRel = m_ForeignRelationshipList.GetNext( pos );
		if( pRel->m_iPlayer == iOtherPlayer )
		{
			return pRel->m_Relationship;
		}
	}
	ASSERT( false );
	return EOSAIEnumForeignRelations::enum_Undefined;
	*/
	ASSERT( iOtherPlayer > 0 && iOtherPlayer <= EOSAI_MAX_NUMBER_OF_PLAYERS );
	if( iOtherPlayer > 0 && iOtherPlayer <= EOSAI_MAX_NUMBER_OF_PLAYERS )
	{
		return m_ForeignRelations[iOtherPlayer];
	}
	return EOSAIEnumForeignRelations::enum_Undefined;
}

void CEOSAINationalSummary::CalculateAccessibility( long iPlayer )
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();

	// Calculate the accessibility
	// Find all my CitRes first
	CList< CEOSAIPoiObject* > PlayerPoiObjectList;
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity && pAICity->GetOwner() == iPlayer )
		{
			PlayerPoiObjectList.AddTail( pAICity );
		}
		CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
		if( pAIResource && pAIResource->GetOwner() == iPlayer )
		{
			PlayerPoiObjectList.AddTail( pAIResource );
		}
	}
	// Find distances to other players
	for( long iOtherPlayer=1; iOtherPlayer<iNumberOfPlayers+1; iOtherPlayer++ )
	{
		if( iOtherPlayer == iPlayer ) continue; // don't calculate accessibility for self

		CList< CEOSAIPoiObject* > OtherPlayerPoiObjectList;
		POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity && pAICity->GetOwner() == iOtherPlayer )
			{
				OtherPlayerPoiObjectList.AddTail( pAICity );
			}
			CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
			if( pAIResource && pAIResource->GetOwner() == iOtherPlayer )
			{
				OtherPlayerPoiObjectList.AddTail( pAIResource );
			}
		}
		// Find distances
		float fTotalWeight = 0.0f;
		float fWeightedDistance = 0.0f;
		pos = PlayerPoiObjectList.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pPlayerPoiObject = PlayerPoiObjectList.GetNext( pos );
			POSITION pos2 = OtherPlayerPoiObjectList.GetHeadPosition();
			while( pos2 )
			{
				CEOSAIPoiObject* pOtherPlayerPoiObject = OtherPlayerPoiObjectList.GetNext( pos2 );
				//float fDistance = pWorldDescServer->GetPixelDistance( pPlayerPoiObject, pOtherPlayerPoiObject );
				float fDistance = g_pWorldDistanceTool->GetDistance( pPlayerPoiObject->GetLocation(), pOtherPlayerPoiObject->GetLocation() );

				// We want closer CitRes to be weighted heavier
				//   Otherwise, more distant CitRes (mine or his) can throw-off the average distance
				// fDistance of    0 = weight of 2
				// fDistance of  400 = weight of 0.67
				// fDistance of  800 = weight of 0.4
				// fDistance of 1200 = weight of 0.29
				float fWeight = 400.0f / (fDistance+200.0f);
				fWeightedDistance =
					( fDistance*fWeight + fWeightedDistance*fTotalWeight ) / ( fWeight + fTotalWeight );
				fTotalWeight += fWeight;
			}
		}

		// fWeightedDistance of    0 = fAccessibility of 1
		// fWeightedDistance of  200 = fAccessibility of 1
		// fWeightedDistance of  400 = fAccessibility of 0.67
		// fWeightedDistance of  600 = fAccessibility of 0.5
		// fWeightedDistance of  800 = fAccessibility of 0.4
		// fWeightedDistance of 1000 = fAccessibility of 0.33
		float fAccessibility = 400.0f/(fWeightedDistance+200.0f);
		if( fAccessibility > 1.0f ){ fAccessibility = 1.0f; }
		m_aPositionTowardsPlayer[ iOtherPlayer ].m_fAccessibility = fAccessibility;
	}
}

// Reset the Foreign Relations values to their actual values
//   (This function exists because we alter the foreign relations values for planning.)
void CAINationalAlliedAndEnemyPower::SetForeignRelationsToActualValues()
{
	// Copy the Relationship information into the AINationalPower object
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		if( m_pNationalSummary->m_iPlayer == iPlayer ) continue;
		m_Relationship[ iPlayer ] = g_pEOSAICommonData->GetForeignRelations( m_pNationalSummary->m_iPlayer, iPlayer );
	}
	/*
	POSITION pos = m_pNationalSummary->m_ForeignRelationshipList.GetHeadPosition();
	while( pos )
	{
		EOSAIEnumForeignRelations* pForeignRelationship = m_pNationalSummary->m_ForeignRelationshipList.GetNext( pos );
		m_Relationship[ pForeignRelationship->m_iPlayer ] = pForeignRelationship->m_Relationship;
	}
	*/
}


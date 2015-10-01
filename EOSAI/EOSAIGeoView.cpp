
#include <stdafx.h>
#include <math.h>
#include "AIPlayer.h"
//#include "City.h"
#include "EOSAIGeo.h"
#include "EOSAIGeoView.h"
#include "EOSAIInterface.h"
#include "EOSAIMultiRegionNationwidePathways.h"
#include <math.h>

//CEOSAIGeoView::CEOSAIGeoView( long iAIPlayer, CEOSAIGeo* pEOSAIGeo )
CEOSAIGeoView::CEOSAIGeoView( EOSAI::AIPlayer* pAIPlayer, CEOSAIGeo* pAIGeo )
{
	m_pAIPlayer = pAIPlayer;
	m_pAIGeo = pAIGeo;
	//m_pAIPlayer = g_pEOSAIInterface->GetAIPlayer( iAIPlayer );

	/*
	for( long iPlayer=0; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
	{
		m_eGeoSituation[iPlayer] = EOSAIEnumGeoSituation_Undefined;
		m_eGeoPlan[iPlayer] = EOSAIEnumGeoPlan_Undefined;
	}
	*/
	m_eGeoSituation = EOSAIEnumGeoSituation_Undefined;
	m_eGeoPlan = EOSAIEnumGeoPlan_Undefined;
	//
	m_iUnownedPoiObjects = 0;
	m_iMyPoiObjects = 0;
	m_iTeamPoiObjects = 0;
	m_iAlliesPoiObjects = 0;
	m_iNeutralPoiObjects = 0;
	m_iEnemyPoiObjects = 0;

	//m_fConsolidation01 = 0.0f;
	m_fGeoDistanceCitResScore = 0.0f;
	m_iGeoDistanceCitResScoreRank = 0;

	m_fMyGroundSeaUnitAccess01 = 0.0f;
	m_fMyAirUnitAccess01 = 0.0f;
	//
	m_fMyCitResDistance = 1000000.0f;
	m_fMyCitResUnitDistance = 1000000.0f;
	m_fNeutralEnemyCitResDistance = 1000000.0f;
	m_fNeutralEnemyCitResUnitDistance = 1000000.0f;
	m_fHumanNeutralEnemyCitResDistance = 1000000.0f;
	m_fHumanNeutralEnemyCitResUnitDistance = 1000000.0f;

	//m_fDangerIndex = 0.0f;
	m_fDangerOfEnemyInvasion01 = 0.0f;
/*
	m_fSendCitResHuntersToThisGeo = 0.0f;
	m_fSendCombatUnitsToInvadeThisGeo = 0.0f;
	m_fGeneralSupport = 0.0f;
	m_fCaptureLocalUnownedCitRes = 0.0f;
	//m_fExploreThisGeo = 0.0f;
	m_fPrepareAttackLocalEnemy = 0.0f;
	m_fDefendAgainstLocalEnemy = 0.0f;
	m_fPrepareAgainstPossibleInvasion = 0.0f; 
*/
	m_fDesire_InvadeExpandAndControlArea_PowerNeeded = 0.0f; // Power needed
	m_fDesire_DegradeArea_PowerNeeded = 0.0f; // Based on the enemy units, cities, in the area
	m_fDesire_ProtectArea_PowerNeeded = 0.0f; // Based on nearby enemy units, pathways
	m_fArrivalTimePriority01 = 0.0f; // Used when a geo is in dispute or near an enemy, need to get units there immediately

}

void CEOSAIGeoView::CalculateAIGeoViewValues()
{
	//CEOSAIGeo* pAIGeo = m_pAIGeo;
	long iAIPlayer = m_pAIPlayer->GetPlayerNumber();
	POSITION pos = m_pAIGeo->GetMultiRegions()->GetHeadPosition();
	while( pos )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_pAIGeo->GetMultiRegions()->GetNext( pos );

		CEOSAIMultiRegionNationwidePathways* pPathways = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );
		CEOSAIRegionPathwayItem* pItem = pPathways->GetDistGeoPathfindingValueFromMyCitRes( pMultiRegion->m_iId );
		if( pItem )
		{
			m_fMyCitResDistance = min( m_fMyCitResDistance, pItem->m_fDistance );
		}
		pItem = pPathways->GetDistGeoPathfindingValueFromMyCitResUnits( pMultiRegion->m_iId );
		if( pItem )
		{
			m_fMyCitResUnitDistance = min( m_fMyCitResUnitDistance, pItem->m_fDistance );
		}
		//CWorldDescServer* pWorldDescServer = m_pAIPlayer->GetWorldDescServer();
		//long iNumberOfPlayers = g_pCommonState->GetNumberOfPlayers();
		long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
			if( pPlayer->IsAlive() == false ) continue;
			if( iPlayer == iAIPlayer ) continue;
			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayer, iAIPlayer );
			if( eRel.IsNeutral() || eRel.IsEnemy() )
			{
				CEOSAIMultiRegionNationwidePathways* pPathways = g_pEOSAICommonData->GetNationwidePathways( iPlayer );
				CEOSAIRegionPathwayItem* pItem = pPathways->GetDistGeoPathfindingValueFromMyCitRes( pMultiRegion->m_iId );
				if( pItem )
				{
					m_fNeutralEnemyCitResDistance = min( m_fNeutralEnemyCitResDistance, pItem->m_fDistance );
				}
				pItem = pPathways->GetDistGeoPathfindingValueFromMyCitResUnits( pMultiRegion->m_iId );
				if( pItem )
				{
					m_fNeutralEnemyCitResUnitDistance = min( m_fNeutralEnemyCitResUnitDistance, pItem->m_fDistance );
				}
			}
		}

		POSITION pos = pMultiRegion->GetPoiObjectsInsideRegion()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = pMultiRegion->GetPoiObjectsInsideRegion()->GetNext( pos );
			//m_pAIGeo->m_AIPoiObjectList.AddTail( pAIPoiObject );

			long iPoiObjectOwner = pAIPoiObject->GetOwner();
			if( iPoiObjectOwner == 0 )
			{
				m_iUnownedPoiObjects++;
			}
			eif( iPoiObjectOwner == iAIPlayer )
			{
				m_iMyPoiObjects++;
			}
			else
			{
				EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iAIPlayer, iPoiObjectOwner );
				if( eRel.IsTeammate() )
				{
					m_iTeamPoiObjects++;
				}
				if( eRel.IsAlly() )
				{
					m_iAlliesPoiObjects++;
				}
				if( eRel.IsNeutral() )
				{
					m_iNeutralPoiObjects++;
				}
				if( eRel.IsEnemy() )
				{
					m_iEnemyPoiObjects++;
				}
			}
		}

		// Update the AIGeo values
		/*
		m_TotalCitResSummary += *pMultiRegion->GetTotalCitResSummary();

		for( long iPlayer=0; iPlayer<m_PlayerCitResUnitSummary.m_iSize; iPlayer++ )
		{
			CEOSAICitResUnitSummary* pGeoSummary = &m_PlayerCitResUnitSummary[ iPlayer ];
			CEOSAICitResUnitSummary* pMultiRegionSummary = pMultiRegion->GetCitResUnitSummary( iPlayer );
			*pGeoSummary += *pMultiRegionSummary;
			int g=0;
		}
		*/
	}
}

void  CEOSAIGeoView::CalculateGeoDistanceCitResScore()
{
	m_fGeoDistanceCitResScore = GetDistanceMult() * sqrt( m_pAIGeo->GetTotalCitResValue() );
}

float CEOSAIGeoView::GetSubjectiveCitResValue()
{
	CEOSAICitResValueMultiplier* pAICitResValueMultiplier = m_pAIPlayer->GetCitResValueMultiplier();
	float fValue =
		( m_pAIGeo->GetTotalCitResSummary()->m_fCityProduction * pAICitResValueMultiplier->m_fCityProductionValueMultiplier ) +
		( m_pAIGeo->GetTotalCitResSummary()->m_fGoldProduction * pAICitResValueMultiplier->m_fGoldValueMultiplier ) +
		( m_pAIGeo->GetTotalCitResSummary()->m_fFoodProduction * pAICitResValueMultiplier->m_fFoodValueMultiplier ) +
		( m_pAIGeo->GetTotalCitResSummary()->m_fIronProduction * pAICitResValueMultiplier->m_fIronValueMultiplier ) +
		( m_pAIGeo->GetTotalCitResSummary()->m_fOilProduction  * pAICitResValueMultiplier->m_fOilValueMultiplier );
	return fValue;
}


// DistanceMult gives higher values to closer geos, and higher values to geos closer to neutral/enemies
float CEOSAIGeoView::GetDistanceMult()
{
	// MyDistance of    0 = 1.0
	// MyDistance of  400 = 0.5
	// MyDistance of  800 = 0.33
	// MyDistance of 1200 = 0.25
	CEOSAIMathFunction  MathFunc1;
	MathFunc1.SetInputOutput(    0.0f,  1.0f );
	MathFunc1.SetInputOutput(  200.0f,  1.0f );
	MathFunc1.SetInputOutput(  400.0f,  0.8f );
	MathFunc1.SetInputOutput(  600.0f,  0.5f );
	MathFunc1.SetInputOutput( 1200.0f, 0.25f );
	MathFunc1.SetInputOutput( 3000.0f, 0.05f );
	//float fMyMult = 400.0f / ( m_fMyCitResDistance + 400.0f );
	float fMyMult = MathFunc1.GetOutput( m_fMyCitResDistance );

	// NeutralEnemyDistance of    0 = 1.0
	// NeutralEnemyDistance of  200 = 1.2
	// NeutralEnemyDistance of 1000 = 1.0
	CEOSAIMathFunction  MathFunc2;
	MathFunc2.SetInputOutput(    0.0f, 1.0f );
	MathFunc2.SetInputOutput(  200.0f, 1.2f );
	MathFunc2.SetInputOutput( 1000.0f, 1.0f );
	float fNEMult = MathFunc2.GetOutput( m_fNeutralEnemyCitResDistance );

	// HumanNeutralEnemyDistance of    0 = 1.0
	// HumanNeutralEnemyDistance of  200 = 1.4
	// HumanNeutralEnemyDistance of 1000 = 1.0
	float fExpandTowardsHumanPlayersMult = 1.0f;
	float fExpandTowardsHumanPlayers01 = m_pAIPlayer->AICheat_ExpandTowardsHumanPlayers01();
	if( fExpandTowardsHumanPlayers01 > 0.0f )
	{
		CEOSAIMathFunction  MathFunc3;
		MathFunc3.SetInputOutput(    0.0f, 1.0f );
		MathFunc3.SetInputOutput(  200.0f, 1.0f + 0.4f*fExpandTowardsHumanPlayers01 );
		MathFunc3.SetInputOutput( 1000.0f, 1.0f );
		fExpandTowardsHumanPlayersMult = MathFunc3.GetOutput( m_fHumanNeutralEnemyCitResDistance );
	}

	return fMyMult * fNEMult * fExpandTowardsHumanPlayersMult;
}


long  CEOSAIGeoView::GetNumberOfUnownedAndEnemyPoiObjects() // Number of tasks
{
	long iCount = 0;
	long iAIPlayer = m_pAIPlayer->GetPlayerNumber();
	//CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	for( long iPlayer=0; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayer, iAIPlayer );
		if( iPlayer == 0 || eRel.IsEnemy() )
		{
			iCount += m_pAIGeo->GetPlayerCitResUnitSummary( iPlayer )->m_CitRes.m_iCitResCount;
			iCount += m_pAIGeo->GetPlayerCitResUnitSummary( iPlayer )->m_Units.m_AIPoiObjects.GetCount();
		}
	}
	return iCount;
}


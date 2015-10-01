
#include <stdafx.h>

#include "City.h"

//#include "Player.h"
#include "EOSAIGeo.h"
#include "PoiObject.h"
//#include "Geo.h"
//#include "CommonState.h"
#include "EOSAIMultiRegion2.h"
//#include "WorldDescServer.h"
//#include "WorldDescPlayer.h"
#include "AIPlayer.h"
#include "EOSAICommonData2.h"
#include "EOSAIMultiRegionNationwidePathways.h"

#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// AIGeos are player-specific

//CEOSAIGeo::CEOSAIGeo( EOSAI::AIPlayer* pAIPlayer, long iGeoId )//, CGeo* pGeo )
CEOSAIGeo::CEOSAIGeo( long iGeoId )//, CGeo* pGeo )
{
	//m_pAIPlayer = pAIPlayer;
	//m_pGeo = pGeo;
	m_iId = iGeoId;

	m_eGeoType = Unknown;
	m_iSize = 0;

	m_fTotalCitResValue = 0.0f;

	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	m_PlayerCitResUnitSummary.SetSize( iNumberOfPlayers+1 );
	m_AIGeoView.SetSize( iNumberOfPlayers+1 );
	for( long i=0; i<iNumberOfPlayers+1; i++ ){ m_AIGeoView[i] = NULL; }
}

CEOSAIGeoView*  CEOSAIGeo::CreateView( long iAIPlayer )
{
	if( m_AIGeoView[iAIPlayer] == NULL )
	{
		EOSAI::AIPlayer* pAIPlayer = g_pEOSAIInterface->GetAIPlayer( iAIPlayer );
		m_AIGeoView[iAIPlayer] = new CEOSAIGeoView( pAIPlayer, this );//iAIPlayer );
	}
	return m_AIGeoView[iAIPlayer];
}

/*
CGeo::GeoType  CEOSAIGeo::GetGeoType()
{
	return m_pGeo->GetGeoType();
}
*/

void  CEOSAIGeo::AddMultiRegion( CEOSAIMultiRegion2* pMultiRegion )
{
	m_MultiRegions.AddTail( pMultiRegion );
/*
#ifdef THINGS_TO_COMPILE_EVENTUALLY - nevermind, moved to AIGeoView
	the AIPlayer needs to be set in order to call this function.
	I can either create a list of values (i.e. turn every AIGeo member into an array) or create separate AIGeos for each player.

	long iAIPlayer = m_pAIPlayer->GetPlayerNumber();

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
	*/
	//ASSERT( m_AIPoiObjectList.IsEmpty() );
	POSITION pos = pMultiRegion->GetPoiObjectsInsideRegion()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pMultiRegion->GetPoiObjectsInsideRegion()->GetNext( pos );
		m_AIPoiObjectList.AddTail( pAIPoiObject );
	}
	/*
	POSITION pos = pMultiRegion->GetPoiObjectsInsideRegion()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pMultiRegion->GetPoiObjectsInsideRegion()->GetNext( pos );
		m_AIPoiObjectList.AddTail( pAIPoiObject );

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
#endif THINGS_TO_COMPILE_EVENTUALLY
*/
	// Update the AIGeo values
	m_TotalCitResSummary += *pMultiRegion->GetTotalCitResSummary();

	for( long iPlayer=0; iPlayer<m_PlayerCitResUnitSummary.m_iSize; iPlayer++ )
	{
		CEOSAICitResUnitSummary* pGeoSummary = &m_PlayerCitResUnitSummary[ iPlayer ];
		CEOSAICitResUnitSummary* pMultiRegionSummary = pMultiRegion->GetCitResUnitSummary( iPlayer );
		*pGeoSummary += *pMultiRegionSummary;
		int g=0;
	}
}


CEOSAIUnit2Summary* CEOSAIGeo::GetUnitSummary( long iPlayer )
{
	return &m_PlayerCitResUnitSummary[ iPlayer ].m_Units;
}

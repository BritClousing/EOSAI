
#include "stdafx.h"
#include "EOSAIDesireConnection.h"
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIMultiRegion2.h"
//#include "WorldDescServer.h"
#include "EOSAIRegionPathwayMap.h"
#include "EOSAICommonData2.h"
#include "EOSAIDesireSpatial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIDesireConnection::Calculate( long iPlayer, CEOSAIDesireSpatial* pStartDesire, CEOSAILocation Location2 )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	CEOSAIRegionPathwayMap* pPathwayMap = pStartDesire->InvokePathToEverywhere();
	CEOSAIRegionPathwayItem* pPathwayItem = pPathwayMap->GetRegionResult( Location2 );

	m_fAirDistance = g_pWorldDistanceTool->GetDistance( pStartDesire->GetLocation(), Location2 );
	//m_bStartsOnLand = pWorldDescServer->IsOnLand( pStartDesire->GetLocation() );
	m_bStartsOnLand = pStartDesire->IsOnLand();
	m_fSingleGeoDistance = m_fAirDistance;
	m_iMinimumGeoSwitches = 0;

	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();
	CEOSAIMultiRegion2* pStartMultiRegion = pMultiRegionManager->GetMultiRegion( pStartDesire->GetLocation() );
	CEOSAIMultiRegion2* pEndMultiRegion = pMultiRegionManager->GetMultiRegion( Location2 );

	ASSERT( pPathwayItem );
	if( pPathwayItem )
	{
		m_iMinimumGeoSwitches = pPathwayItem->m_iGeoSwitches;
		if( m_iMinimumGeoSwitches == 0 )
		{
			m_fSingleGeoDistance = pPathwayItem->m_fDistance;
		}
		else
		{
			m_fSingleGeoDistance = 1000000.0f;
		}

		if( pStartMultiRegion->GetGeoId() == pEndMultiRegion->GetGeoId() )
		{
			// The GeoSwitches can be higher than 0 if the route over water is much shorter than
			//   the route over land
			//ASSERT( m_iMinimumGeoSwitches == 0 );
		}
		else
		{
			// Different Geos
			//   If I start on water, and end on a coastal city, the GeoSwitches will say 0,
			//   but it should say 1 or higher.
			if( pStartMultiRegion->GetCityObjectId() || pEndMultiRegion->GetCityObjectId() )
			{
				if( m_iMinimumGeoSwitches == 0 ) m_iMinimumGeoSwitches = 1;
			}
			else
			{
				if( EOSAISettings::m_bAssertWhenAIPathwayFails )
				{
					// This can happen if a pathway leads from water through a city to water
					ASSERT( m_iMinimumGeoSwitches > 0 );
				}
			}
		}
	}
}

/*
void CEOSAIDesireConnection::Calculate( long iPlayer, CEOSAILocation Location1, CEOSAILocation Location2 )
{
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//
	m_fAirDistance = pWorldDescServer->GetPixelDistance( Location1, Location2 );
	m_bStartsOnLand = pWorldDescServer->IsOnLand( Location1 );
	m_fSingleGeoDistance = m_fAirDistance;
	m_iMinimumGeoSwitches = 0;

	// Is there a single-geo path?
	CEOSAIMultiRegionManager2* pMultiRegionManager = pWorldDescServer->GetAICommonData()->GetMultiRegionManager();
	CMultiRegion* pStartMultiRegion = pMultiRegionManager->GetMultiRegion( Location1 );
	CMultiRegion* pEndMultiRegion = pMultiRegionManager->GetMultiRegion( Location2 );

	ASSERT( pStartMultiRegion && pEndMultiRegion );
	if( pStartMultiRegion && pEndMultiRegion )
	{
		bool bCheckIfThereIsASingleGeoPath = true;
		bool bFindTheMinimumNumberOfGeoSwitches = true;
		if( pStartMultiRegion == pEndMultiRegion )
		{
			m_iMinimumGeoSwitches = 0;
			bCheckIfThereIsASingleGeoPath = false;
			bFindTheMinimumNumberOfGeoSwitches = false;
		}
		if( pWorldDescServer->IsOnLand( Location1 ) != pWorldDescServer->IsOnLand( Location2 ) )
		{
			// One Location is on land, the other is on water.  There cannot be a single-geo path
			bCheckIfThereIsASingleGeoPath = false;
			bFindTheMinimumNumberOfGeoSwitches = true;
		}

		if( bCheckIfThereIsASingleGeoPath )
		{
			CEOSAIRegionPathwayMap GlobalResult;
			m_iMinimumGeoSwitches = 0;
			CEOSAIRegionPathfinder& RegionPathfinder = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager()->GetPathfinder();
			RegionPathfinder.ClearPathfindingSettings();
			RegionPathfinder.AddStartingRegion( pStartMultiRegion );
			RegionPathfinder.AddEndingRegion( pEndMultiRegion );
			RegionPathfinder.SetToIgnoreOtherPlayersPathway( iPlayer );
			RegionPathfinder.SetDistanceMultiplier( 1.0f );
			RegionPathfinder.SetMaximumGeoSwitches( 0 );
			RegionPathfinder.Solve( iPlayer, true );
			RegionPathfinder.GetResult( false, &GlobalResult );

			CEOSAIRegionPathwayItem* pRegionResult = GlobalResult.GetRegionResult( pEndMultiRegion->m_iId );
			if( pRegionResult ){ m_iMinimumGeoSwitches = 0; }
			else{ bFindTheMinimumNumberOfGeoSwitches = true; }
		}
		if( bFindTheMinimumNumberOfGeoSwitches )
		{
			CEOSAIRegionPathfinder& RegionPathfinder = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager()->GetPathfinder();
			CEOSAIRegionPathwayMap GlobalResult;
			RegionPathfinder.ClearPathfindingSettings();
			RegionPathfinder.AddStartingRegion( pStartMultiRegion );
			RegionPathfinder.AddEndingRegion( pEndMultiRegion );
			RegionPathfinder.SetToIgnoreOtherPlayersPathway( iPlayer );
			RegionPathfinder.SetDistanceMultiplier( 0.0f );
			RegionPathfinder.SetGeoSwitchMultiplier( 1000.0f );
			RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
			RegionPathfinder.Solve( iPlayer, true );
			RegionPathfinder.GetResult( false, &GlobalResult );

			CEOSAIRegionPathwayItem* pRegionResult = GlobalResult.GetRegionResult( pEndMultiRegion->m_iId );
			if( pRegionResult ){ m_iMinimumGeoSwitches = pRegionResult->m_iGeoSwitches; }
			else{ ASSERT( false ); }

			if( pStartMultiRegion->GetGeoId() == pEndMultiRegion->GetGeoId() )
			{
				ASSERT( m_iMinimumGeoSwitches == 0 );
				//m_iMinimumGeoSwitches = 0;
			}
			else
			{
				// Different Geos
				//   If I start on water, and end on a coastal city, the GeoSwitches will say 0,
				//   but it should say 1 or higher.
				if( pStartMultiRegion->GetCity() || pEndMultiRegion->GetCity() )
				{
					if( m_iMinimumGeoSwitches == 0 ) m_iMinimumGeoSwitches = 1;
				}
				else
				{
					ASSERT( m_iMinimumGeoSwitches > 0 );
				}
				//m_iMinimumGeoSwitches = 1;
			}
		}

		/*
		if( m_iMinimumGeoSwitches == 0 )
		{
			ASSERT( pStartMultiRegion->GetGeoId() == pEndMultiRegion->GetGeoId() );
		}
		else
		{
			ASSERT( pStartMultiRegion->GetGeoId() != pEndMultiRegion->GetGeoId() );
		}
		*-/
	}
	//m_fSingleGeoDistance = 0.0f;
	//m_iMinimumGeoSwitches = 0;
}
*/

float  CEOSAIDesireConnection::GetConnectionValue01() // Based on values below
{
	float fValue = 1.0f;
	if( m_iMinimumGeoSwitches == 0 )
	{
		float fDistance = min( m_fAirDistance, 250.0f ) + min( m_fSingleGeoDistance, 250.0f );

		// m_fAirDistance = 0,   result = 1.0
		// m_fAirDistance = 100, result = 1.0
		// m_fAirDistance = 200, result = 0.33
		// m_fAirDistance = 250, result = 0.0
		if(      fDistance < 100.0f ){ fValue *= 1.0f; }
		else if( fDistance < 250.0f ){ fValue *= (150.0f-(fDistance-100))/150.0f; }
		else{ fValue = 0.0f; }
	}
	else
	{
		if( m_iMinimumGeoSwitches == 1 ) fValue *= 0.8f;
		if( m_iMinimumGeoSwitches == 2 ) fValue *= 0.6f;
		if( m_iMinimumGeoSwitches == 3 ) fValue *= 0.4f;
		if( m_iMinimumGeoSwitches >= 4 ) fValue *= 0.2f;

		float fDistance = m_fAirDistance;
		// m_fAirDistance = 0,   result = 1.0
		// m_fAirDistance = 100, result = 1.0
		// m_fAirDistance = 200, result = 0.33
		// m_fAirDistance = 250, result = 0.0
		if(      fDistance < 100.0f ){ fValue *= 1.0f; }
		else if( fDistance < 250.0f ){ fValue *= (150.0f-(fDistance-100))/150.0f; }
		//else if( fDistance < 300.0f ){ fValue *= ( 200.0f - (fDistance-100.0f) ) / 200.0f; }
		else{ fValue = 0.0f; }
	}
	return fValue;
}


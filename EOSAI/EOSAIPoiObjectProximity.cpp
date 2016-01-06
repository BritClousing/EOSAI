
#include <stdafx.h>
#include "EOSAIPoiObjectProximity.h"
#include "EOSAIUnit2.h"
#include "City.h"
//#include "AIPoiObjectsOrderedByFutureProximityToLocation.h"
#include "EOSAIBrain.h"
//#include "EOSAIUnitTemplate.h"
//#include "UnitTemplate.h"
//#include "WorldDesc.h"
//#include "AIThought_WhatUnitsCanIGetToMultiRegion.h"
//#include "9MovementPathwaysFromEverywhereToMRX.h"
#include "EOSAIMultiRegion2.h"
//#include "WorldDescServer.h"
/*
CEOSAIPoiObjectProximity_City::CEOSAIPoiObjectProximity_City(
	C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
	CEOSAICity*  pAICity )
{
	m_pAICity = pAICity;
	//CMultiRegion* pTargetMultiRegion = pParent_WhatUnitsCanIGetToMultiRegion->m_pTargetMultiRegion;

	//CWorldDesc* pWorldDesc = pAICity->GetAIBrain()->GetWorldDesc();

	//
	//m_fAirDistanceToTarget = pWorldDesc->GetPixelDistance( m_pAICity->GetInitialLocation(), pTargetMultiRegion->GetLocation() );
	//m_fAirDistanceToTarget -= pTargetMultiRegion->m_fRadiusOfRegions;
	//if( m_fAirDistanceToTarget < 0.0f ) m_fAirDistanceToTarget = 0.0f;

	CMultiRegion* pCityMultiRegion = m_pAICity->GetInitialState()->GetMultiRegion();
	ASSERT( pCityMultiRegion );
	m_pPathwaysToTargetMultiRegion = pParent_PathwaysFromEverywhereToMRX->GetPathway( pCityMultiRegion->m_iId );

	/-*
	m_fCrowsTimeBasedOnFastestValidUnit = 0.0f;
	float fFastestMovementRate = pParent_WhatUnitsCanIGetToMultiRegion->GetFastestMovementRateOfValidUnits();
	ASSERT( fFastestMovementRate > 0.0f );
	if( fFastestMovementRate > 0.0f )
	{
		m_fCrowsTimeBasedOnFastestValidUnit = m_fAirDistanceToTarget / fFastestMovementRate;
	}
	*-/

	//m_bLocationIsWithinAirRangeOfAValidAirUnit = 
	//	m_fAirDistanceToTarget < pParent_WhatUnitsCanIGetToMultiRegion->GetLargestMovementRangeOfValidAirUnits();

	// Look over the valid units, 
	//  determine which can actually reach the Location (based on range), 
	//  figure out how long it will take to get there
	/-*
	POSITION pos = pParent_WhatUnitsCanIGetToMultiRegion->GetValidUnitTemplates()->m_List.GetHeadPosition();
	while( pos )
	{
		bool bUnitCanReachTarget = false;
		CEOSAIUnitTemplate* pAIUnitTemplate = pParent_WhatUnitsCanIGetToMultiRegion->GetValidUnitTemplates()->m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->HasARange() )
		{
			if( m_fAirDistanceToTarget < pAIUnitTemplate->GetUnitTemplate()->GetMovementLimit() )
			{
				bUnitCanReachTarget = true;
			}
		}
		else
		{
			// Assume we have transports for ground units
			bUnitCanReachTarget = true;
		}

		float fMovementRate = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		if( bUnitCanReachTarget &&
			fMovementRate > 0.0f )
		{
			float fTime = m_fAirDistanceToTarget / fMovementRate;
			if( m_fCrowsTimeBasedOnFastestValidUnit < fTime )
			{
				m_fCrowsTimeBasedOnFastestValidUnit = fTime;
			}
			//m_fCrowsTimeBasedOnFastestValidUnit = m_fAirDistanceToTarget / fMovementRate;
		}
	}
	*-/
}


CEOSAIPoiObjectProximity_Airbase::CEOSAIPoiObjectProximity_Airbase(
	C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
	//CAIPoiObjectsOrderedByFutureProximityToLocation* pOrderedByProximity,
	CEOSAIPoiObject*  pAIAirbase )
{
	//m_pOrderedByProximity = pOrderedByProximity;
	m_pAIAirbase = pAIAirbase;
	//m_fCrowsFlightDistanceToLocation = fCrowsFlightDistanceToLocation;
	m_bAirbaseIsMobile = false;
	if( dynamic_cast< CEOSAIUnit* >( m_pAIAirbase ) )
	{
		m_bAirbaseIsMobile = true;
	}

	CWorldDesc* pWorldDesc = pAIAirbase->GetAIBrain()->GetWorldDescServer();
	//CWorldDesc* pWorldDesc = m_pOrderedByProximity->GetAIPlan()->GetWorldDesc();
	//m_fAirFlightDistanceToLocation = pWorldDesc->GetPixelDistance( pAIAirbase->GetInitialLocation(), m_pOrderedByProximity->GetLocation() );

	CMultiRegion* pTargetMultiRegion = pParent_PathwaysFromEverywhereToMRX->m_pTargetMultiRegion;

	//
	m_fCurrentAirDistanceToTarget = pWorldDesc->GetPixelDistance( m_pAIAirbase->GetInitialState()->GetLocation(), pTargetMultiRegion->GetLocation() );
	m_fCurrentAirDistanceToTarget -= pTargetMultiRegion->m_fRegionRadius;
	if( m_fCurrentAirDistanceToTarget < 0.0f ) m_fCurrentAirDistanceToTarget = 0.0f;

	CMultiRegion* pAirbaseMultiRegion = pAIAirbase->GetInitialState()->GetMultiRegion();
	ASSERT( pAirbaseMultiRegion );
	//m_pPathwaysToTargetMultiRegion = pParent_WhatUnitsCanIGetToMultiRegion->m_pTargetMultiRegion->InvokePathwaysToMultiRegion()->m_LandSeaMovementFromMultiRegionArray[ pAirbaseMultiRegion->m_iId ];
	CEOSAIMovementPathwaysFromMRXtoMRY* pPathway = pParent_PathwaysFromEverywhereToMRX->GetPathway( pAirbaseMultiRegion->m_iId );
	m_PathwaysToTargetMultiRegion.AddPathway( pPathway );

	/-*
	m_fAirTimeBasedOnFastestValidAirUnit = 0.0f;
	float fFastestMovementRate = pParent_WhatUnitsCanIGetToMultiRegion->GetFastestMovementRateOfValidAirUnits();
	ASSERT( fFastestMovementRate > 0.0f );
	if( fFastestMovementRate > 0.0f )
	{
		m_fAirTimeBasedOnFastestValidAirUnit = m_fAirFlightDistanceToLocation / fFastestMovementRate;
	}

	m_bLocationIsWithinAirRangeOfAValidAirUnit = 
		m_fAirFlightDistanceToLocation < pParent_WhatUnitsCanIGetToMultiRegion->GetLargestMovementRangeOfValidAirUnits();

	// Look over the valid units, 
	//  determine which can actually reach the Location (based on range), 
	//  figure out how long it will take to get there
	POSITION pos = pParent_WhatUnitsCanIGetToMultiRegion->GetValidUnitTemplates()->m_List.GetHeadPosition();
	while( pos )
	{
		bool bUnitCanReachTarget = false;
		CEOSAIUnitTemplate* pAIUnitTemplate = pParent_WhatUnitsCanIGetToMultiRegion->GetValidUnitTemplates()->m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->HasARange() )
		{
			if( m_fAirFlightDistanceToLocation < pAIUnitTemplate->GetUnitTemplate()->GetMovementLimit() )
			{
				bUnitCanReachTarget = true;
			}
		}
		else
		{
			// Assume we have transports for ground units
			bUnitCanReachTarget = true;
		}

		float fMovementRate = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		if( bUnitCanReachTarget &&
			fMovementRate > 0.0f )
		{
			float fTime = m_fAirFlightDistanceToLocation / fMovementRate;
			if( m_fAirTimeBasedOnFastestValidAirUnit < fTime )
			{
				m_fAirTimeBasedOnFastestValidAirUnit = fTime;
			}
		}
	}
	*-/
}
*/
/*
CEOSAIPoiObjectProximity_Unit::CEOSAIPoiObjectProximity_Unit(
	CAIPoiObjectsOrderedByFutureProximityToLocation* pOrderedByProximity,
	CEOSAIUnit*  pAIUnit )
{
	m_pOrderedByProximity = pOrderedByProximity;
	m_pAIUnit = pAIUnit;
	m_iDepthOfThought = 1;

	CWorldDesc* pWorldDesc = m_pOrderedByProximity->GetAIPlan()->GetWorldDesc();
	m_fCrowsFlightDistanceToLocation = pWorldDesc->GetPixelDistance( m_pAIUnit->GetInitialLocation(), m_pOrderedByProximity->GetLocation() );
	m_fCrowsFlightTimeToLocation = 1000000.0f;
	if( m_pAIUnit->GetMovementRate() > 0.0f )
	{
		m_fCrowsFlightTimeToLocation = m_fCrowsFlightDistanceToLocation / m_pAIUnit->GetMovementRate();
	}
}
*/
/*
CEOSAIPoiObjectProximity_Unit::CEOSAIPoiObjectProximity_Unit(
	//CAIPoiObjectsOrderedByFutureProximityToLocation* pOrderedByProximity,
	C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
	CEOSAIUnit*  pAIUnit )
{
	CWorldDesc* pWorldDesc = pAIUnit->GetAIBrain()->GetWorldDescServer();

	//m_pOrderedByProximity = pOrderedByProximity;
	m_pAIUnit = pAIUnit;

	// Iterate over the unit's CAIPoiObjectStateProgression to find the MultiRegions and times
	/-*
	CAIPoiObjectStateProgression* pStateProgression = pAIUnit->GetAIPoiObjectStateProgression();
	POSITION pos = pStateProgression->m_StateProgression.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiObjectState* pAIPoiObjectState = pStateProgression->m_StateProgression.GetNext( pos );
	}
	*-/

	CMultiRegion* pUnitMultiRegion = m_pAIUnit->GetInitialState()->GetMultiRegion();
	ASSERT( pUnitMultiRegion );
	CEOSAIMovementPathwaysFromMRXtoMRY* pPathway = pParent_PathwaysFromEverywhereToMRX->GetPathway( pUnitMultiRegion->m_iId );
	m_PathwaysToTargetMultiRegion.AddPathway( pPathway );
}
*/
/*
CEOSAIPoiObjectProximity_GroundUnit::CEOSAIPoiObjectProximity_GroundUnit(
	CAIPoiObjectsOrderedByFutureProximityToLocation* pOrderedByProximity,
	CEOSAIUnit* pAIUnit ) : 
	CEOSAIPoiObjectProximity_Unit( pOrderedByProximity, pAIUnit )
{
	
}
*/
void CEOSAIPoiObjectProximityDescription::SetAIPoiObject( CEOSAIPoiObject* pAIPoiObject )
{
	m_pAIPoiObject = pAIPoiObject;
}

float CEOSAIPoiObjectProximityDescription::GetTimeToLocation()
{
	if( m_iDepthOfThought == 0 )
	{
		ASSERT( false );
	}
	if( m_iDepthOfThought == 1 )
	{
		return m_fCrowsFlightTime;
	}
	ASSERT( false );
	return 1000.0f;
}


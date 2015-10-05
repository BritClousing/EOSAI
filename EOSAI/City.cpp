
#include "stdafx.h"
#include "City.h"
#include "EOSAIUnit2.h"

#include "AIPlayer.h"
//#include "EOSAIBrain.h"
//#include "City.h"
#include "EOSAIBrain.h"
#include "EOSAIGeo.h"
#include "EOSAIRegionPathway.h"
//#include "AICityDesireMatrix.h"
//#include "EOSAIProductionOption.h"
//#include "EOSAIProductionTask.h"
#include "EOSAIUnitTemplate.h"
//#include "AIThought_ShouldICaptureThatCity.h"
//#include "WorldDescPlayer.h"
//#include "AIActionDefinition_ProtectCitRes.h"
//#include "AIActionDefinition_CaptureCitRes.h"
//#include "AIActionDefinition_DegradeCity.h"
//#include "AITacticalProject.h"
#include "EOSAITacticalProject2.h"
//#include "AIActionDefinition_CaptureCitRes.h"
#include "EOSAIBuildingDescription.h"
//#include "AICityBuildAction.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAICommonData.h"
//#include "WorldDescServer.h"
#include "EOSAIMultiRegionNationwidePathways.h"
#include "EOSAIMultiRegionAttackAccess.h"
//#include "UnitTemplate.h"
#include "EOSAIBuildCompletion.h"
#include "EOSAIBuildOption.h"
#include "EOSAIPlayerManager.h"
#include "EOSAINationalSummary3.h"
#include "EOSAIInterface.h"
#include "EOSAICommonData.h"
#include <math.h>
extern EOSAI::CInterface* g_pEOSAIInterface;
extern EOSAI::CCommonData* g_pEOSAICommonData;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAICity::CEOSAICity() : CEOSAIPoiObject()
{
	//m_pCity = pCity;
	//SetPoiObject( m_pCity );
	//m_pServerCity = NULL;
	//m_pPlayerCity = NULL;

	m_fProduction = 0.0f;

	//m_pAIGeo = NULL;
	//m_pAIGeoLand = NULL;
	m_bIAmACoastalCity = false;

	m_fStrategicBomberDanger01 = -1.0f;
	m_fGroundSeaDanger01 = -1.0f;

	m_fSortValue = 0.0f;
	m_fPreferenceForLowCostUnits01 = 0.0f;
	m_fPreferenceForGroundUnits01 = 0.0f;
	m_fPreferenceForAirUnits01 = 0.0f;
	m_fPreferenceForSeaUnits01 = 0.0f;
	
	m_iAdjacentWaterSize = 0;
	//m_bIsOnAMajorBodyOfWater = false;

	///m_pBestAIProductionOption = NULL;
	//m_fBestAIProductionOptionScore = 0.0f;

	//m_pAssignedProductionTask = NULL;
	//m_fAssignedProductionTaskTimeUntilCompletion = 0.0f;

	m_iTempItemsInTaskForceStack = 0;
	//m_pAIThought_ShouldICaptureThatCitRes = new CAIThought_ShouldICaptureThatCitRes( this );
/*
	m_fArea_Range100 = 0.0f;
	m_fArea_Range150 = 0.0f;
	m_fArea_Range200 = 0.0f;
	m_fArea_Range250 = 0.0f;
	m_fArea_Range300 = 0.0f;
	m_fArea_Range400 = 0.0f;
	m_fArea_Range500 = 0.0f;
	m_fArea_Range600 = 0.0f;
	m_fArea_Range700 = 0.0f;
	m_fArea_Range800 = 0.0f;
	m_fArea_Range900 = 0.0f;
	m_fArea_Range1000 = 0.0f;
	m_fArea_Range1200 = 0.0f;
	m_fArea_Range1500 = 0.0f;

	m_fUnexploredArea_Range100 = 0.0f;
	m_fUnexploredArea_Range150 = 0.0f;
	m_fUnexploredArea_Range200 = 0.0f;
	m_fUnexploredArea_Range250 = 0.0f;
	m_fUnexploredArea_Range300 = 0.0f;
	m_fUnexploredArea_Range400 = 0.0f;
	m_fUnexploredArea_Range500 = 0.0f;
	m_fUnexploredArea_Range600 = 0.0f;
	m_fUnexploredArea_Range700 = 0.0f;
	m_fUnexploredArea_Range800 = 0.0f;
	m_fUnexploredArea_Range900 = 0.0f;
	m_fUnexploredArea_Range1000 = 0.0f;
	m_fUnexploredArea_Range1200 = 0.0f;
	m_fUnexploredArea_Range1500 = 0.0f;
*/
}

CEOSAICity::~CEOSAICity()
{}
/*
void  CEOSAICity::SetServerPoiObject( CPoiObject* pPoiObject )
{
	ASSERT( dynamic_cast< CCity* >( pPoiObject ) );
	m_pServerCity = dynamic_cast< CCity* >( pPoiObject );
	CEOSAIPoiObject::SetServerPoiObject( pPoiObject );
}
*/
/*
void  CEOSAICity::SetPlayerPoiObject( CPoiObject* pPoiObject )
{
	if( pPoiObject == NULL ) return;
	//
	ASSERT( dynamic_cast< CCity* >( pPoiObject ) );
	m_pPlayerCity = dynamic_cast< CCity* >( pPoiObject );
	CAIPoiObject::SetPlayerPoiObject( pPoiObject );
}
*/
//CPoiObject* CEOSAICity::GetPoiObject(){ return m_pCity; }

float CEOSAICity::GetAIValueOfCity( long iAccordingToPlayer ) // based on pAIBrain valuation multipliers, production, strategic value (e.g. as an airbase)
{
	//float fValueMultiplier = m_pAIBrain->GetCitResValueMultiplier()->m_fCityProductionValueMultiplier;
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iAccordingToPlayer );
	ASSERT( pAIPlayer );
	//float fValueMultiplier = m_pAIBrain->GetAIPlayer()->GetCitResValue_CityMultiplier();
	float fValueMultiplier = pAIPlayer->GetCitResValue_CityMultiplier();
	//float fValue = ( fValueMultiplier * m_pServerCity->GetPopulation() ) + 2.0f;
	float fValue = ( fValueMultiplier * GetProduction() ) + 2.0f;
	return fValue;
}

//
bool  CEOSAICity::HasPredefinedBuildOrders()
{
	return false;
}

// Resources
//
void CEOSAICity::GetResourcesProducedPerTurn(std::map<CString, float>& ResourcesProducedAssumingNoOrders, std::map<CString, float>& ResourcesProducedByCurrentOrders)
{
	// TODO: This is very game specific. It needs to be changed.

	// If the city isn't producing anything, it produces wealth
	ResourcesProducedAssumingNoOrders[_T("Money")] += 0.5f * m_fProduction;
}

void CEOSAICity::GetResourcesConsumedPerTurn(std::map<CString, float>& ResourcesConsumedAssumingNoOrders, std::map<CString, float>& ResourcesConsumedByCurrentOrders)
{
	// TODO: This is very game specific. It needs to be changed.

	// Consume food
	ResourcesConsumedAssumingNoOrders[_T("Food")] += 0.2f * m_fPopulation;
	// Consume oil
	ResourcesConsumedAssumingNoOrders[_T("Oil")] += 0.2f * m_fPopulation;

	// TODO: Additional consumption will happen depending on the city's orders
	ResourcesConsumedByCurrentOrders[_T("Iron")] += 0;
}

float CEOSAICity::GetProductionAndConsumptionPerTurn(CString strResource)
{
	// TODO: This is very game specific. It needs to be changed.
	if (strResource == _T("Food")) return 0.2f * m_fPopulation;
	if (strResource == _T("Oil")) return 0.2f * m_fPopulation;
	if (strResource == _T("Iron")) return 0;
	return 0;
}

// AITacticalProject methods
//
/*
float CEOSAICity::GetBuildTimeAndTimeToTarget( CAITacticalProject* pAITacticalProject, CUnitTemplate* pUnitTemplate )
{
	//CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = pAITacticalProject->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
	CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();

	float fTimeToTarget = 1000000.0f;

	// Build time at this city (include partially completed data)
	float fBuildTime = this->GetServerCity()->GetTimeToBuild( pUnitTemplate, true );

	//CEOSAILocation TargetLocation = pAITacticalProject->GetClosestPrimaryTargetLocation( GetServerCity()->GetLocation() );
	CEOSAILocation TargetLocation = pAITacticalProject->GetClosestPrimaryTargetLocation( GetServerCity()->GetLocation() );
	//CEOSAIDesireSpatial* pClosestDesire = NULL;
	//pAITacticalProject->GetClosestPrimaryGoal( this->GetServerCity()->GetLocation(), &pClosestDesire );
	//CEOSAILocation TargetLocation = pClosestDesire->GetLocation();
	if( pUnitTemplate->IsGroundUnit() )
	{
		// Land-Route
		CEOSAIRegionPathwayItem* pLandPathResult = this->InvokeLandMultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandPathResult )
		{
			float fLandPath_TimeToTarget = pLandPathResult->m_fDistance / pUnitTemplate->GetMovementRate();
			fTimeToTarget = fBuildTime + min( fTimeToTarget, fLandPath_TimeToTarget );
		}

		// Land + SeaTransport Route (uses an ideal Transport)
		CEOSAIRegionPathwayItem* pLandSea1PathResult = this->InvokeLandAndSeaTransport1MultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		CEOSAIRegionPathwayItem* pLandSea2PathResult = this->InvokeLandAndSeaTransport2MultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandSea1PathResult )
		{
			ASSERT( pLandSea2PathResult );

			float fSeaTransportMovementRate = pUnitsICanBuildOrHave->GetFastestSeaTransportMovementRate( pUnitTemplate );
			float fThisUnitMovementRate = pUnitTemplate->GetMovementRate();

			float fLandDistance1, fWaterDistance1, fCityDistance1;
			pLandSea1PathResult->GetDistance( &fCityDistance1, &fLandDistance1, &fWaterDistance1 );
			float fLandSeaPath_TimeToTarget1 =
				fCityDistance1  / ( max( fSeaTransportMovementRate, fThisUnitMovementRate )+0.1f ) +
				fLandDistance1  / ( fThisUnitMovementRate+0.1f ) +
				fWaterDistance1 / ( fSeaTransportMovementRate+0.1f );

			float fLandDistance2, fWaterDistance2, fCityDistance2;
			pLandSea2PathResult->GetDistance( &fCityDistance2, &fLandDistance2, &fWaterDistance2 );
			float fLandSeaPath_TimeToTarget2 =
				fCityDistance2  / ( max( fSeaTransportMovementRate, fThisUnitMovementRate )+0.1f ) +
				fLandDistance2  / ( fThisUnitMovementRate+0.1f ) +
				fWaterDistance2 / ( fSeaTransportMovementRate+0.1f );

			ASSERT( fLandDistance1  == fLandDistance2  );
			ASSERT( fWaterDistance1 == fWaterDistance2 );
			float fLandSeaPath_TimeToTarget = min( fLandSeaPath_TimeToTarget1, fLandSeaPath_TimeToTarget2 );

			fTimeToTarget = min( fTimeToTarget, fLandSeaPath_TimeToTarget );

			float fTransportArrivalTime = this->GetBestTransportArrivalTime_CrowsFlight( pUnitTemplate );

			fTimeToTarget = max( fBuildTime, fTransportArrivalTime ) + fTimeToTarget;
		}
	}
	if( pUnitTemplate->IsSeaUnit() )
	{
		// Calculate SeaUnit pathway
		CEOSAIRegionPathwayItem* pSeaPathResult = this->InvokeWaterAIRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pSeaPathResult )
		{
			fTimeToTarget = fBuildTime + (pSeaPathResult->m_fDistance / pUnitTemplate->GetMovementRate());
		}
	}
	if( pUnitTemplate->IsAirUnit() )
	{
		// Calculate AirUnit pathway (estimates time, doesn't actually calculate airbase-step pathway)
		fTimeToTarget = 
			fBuildTime +
			CEOSAIUnit2ActionIdea::GetAirPathwayTimeToTarget(
					this->GetAIPlayer()->GetPlayerNumber(),
					GetAIBrain(),
					pUnitTemplate,
					this->GetServerCity()->GetLocation(),//UnitStart,
					TargetLocation,
					CEOSAIUnit2ActionIdea::MovementMotive_RoundTrip ); // ASSUME a round trip
	}
	return fTimeToTarget;
}

CEOSAIUnit2ActionIdea* CEOSAICity::CreateAIUnitActionIdea( CAITacticalProject* pAITacticalProject, CUnitTemplate* pUnitTemplate )
//CEOSAIUnit2ActionIdea* CEOSAICityActionIdeaNode::CreateAIUnitActionIdea( CUnitTemplate* pUnitTemplate )
{
	CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();

	// Create a UnitActionIdea and write data into it
	CEOSAIUnit2ActionIdea* pAIUnitActionIdea = new CEOSAIUnit2ActionIdea( pAITacticalProject, this, pUnitTemplate );

	float fTimeToTarget = 1000000.0f;

	// Build time at this city (include partially completed data)
	float fBuildTime = this->GetServerCity()->GetTimeToBuild( pUnitTemplate, true );
	pAIUnitActionIdea->SetBuildTime( fBuildTime );

	CEOSAILocation TargetLocation = pAITacticalProject->GetClosestPrimaryTargetLocation( GetServerCity()->GetLocation() );
	//CEOSAIDesireSpatial* pClosestDesire = NULL;
	//pAITacticalProject->GetClosestPrimaryGoal( this->GetServerCity()->GetLocation(), &pClosestDesire );
	//CEOSAILocation TargetLocation = pClosestDesire->GetLocation();//m_pTacticalProject->GetLocation();
	if( pUnitTemplate->IsGroundUnit() )
	{
		// Land-Route
		CEOSAIRegionPathwayItem* pLandPathResult = this->InvokeLandMultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandPathResult )
		{
			float fLandPath_TimeToTarget = pLandPathResult->m_fDistance / pUnitTemplate->GetMovementRate();
			//fTimeToTarget = min( fTimeToTarget, fLandPath_TimeToTarget );
			pAIUnitActionIdea->SetDirectMoveToTargetTime( fLandPath_TimeToTarget );
		}

		// Land + SeaTransport Route (uses an ideal Transport)
		CEOSAIRegionPathwayItem* pLandSea1PathResult = this->InvokeLandAndSeaTransport1MultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		CEOSAIRegionPathwayItem* pLandSea2PathResult = this->InvokeLandAndSeaTransport2MultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandSea1PathResult )
		{
			ASSERT( pLandSea2PathResult );

			float fSeaTransportMovementRate = pUnitsICanBuildOrHave->GetFastestSeaTransportMovementRate( pUnitTemplate );
			float fThisUnitMovementRate = pUnitTemplate->GetMovementRate();

			float fLandDistance, fWaterDistance, fCityDistance;
			pLandSea1PathResult->GetDistance( &fCityDistance, &fLandDistance, &fWaterDistance );
			float fLandSeaPath_TimeToTarget1 =
				fCityDistance  / ( max( fSeaTransportMovementRate, fThisUnitMovementRate )+0.1f ) +
				fLandDistance  / ( fThisUnitMovementRate+0.1f ) +
				fWaterDistance / ( fSeaTransportMovementRate+0.1f );

			pLandSea2PathResult->GetDistance( &fCityDistance, &fLandDistance, &fWaterDistance );
			float fLandSeaPath_TimeToTarget2 =
				fCityDistance  / ( max( fSeaTransportMovementRate, fThisUnitMovementRate )+0.1f ) +
				fLandDistance  / ( fThisUnitMovementRate+0.1f ) +
				fWaterDistance / ( fSeaTransportMovementRate+0.1f );

			float fLandSeaPath_TimeToTarget = min( fLandSeaPath_TimeToTarget1, fLandSeaPath_TimeToTarget2 );

			//pAIUnitActionIdea->SetTransportAssistedMoveToTargetTime( fLandSeaPath_TimeToTarget );

			//float fTransportArrivalTime = this->GetBestTransportArrivalTime_CrowsFlight( pUnitTemplate );
			//pAIUnitActionIdea->SetTransportArrivalTime( fTransportArrivalTime );
		}
		/*
		CEOSAIRegionPathwayItem* pLandSeaPathResult = this->InvokeLandAndSeaTransportAIRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandSeaPathResult )
		{
			float fSeaTransportMovementRate = pUnitsICanBuildOrHave->GetFastestSeaTransportMovementRate( pUnitTemplate );
			float fThisUnitMovementRate = pUnitTemplate->GetMovementRate();
			float fMovementRate = max( fSeaTransportMovementRate, fThisUnitMovementRate );
			float fLandSeaPath_TimeToTarget = pLandSeaPathResult->m_fDistance / fMovementRate;
			//fTimeToTarget = min( fTimeToTarget, fLandSeaPath_TimeToTarget );

			pAIUnitActionIdea->SetTransportMoveToTargetTime( fLandSeaPath_TimeToTarget );

			// REAL TRANSPORTS - I might want to use this
			//CEOSAITransportsSet  TransportsSet( GetTacticalProject()->GetAIBrain()->GetAIThoughtDatabase(), m_pUnitActorTemplate );
			//m_fTransportToActorTime = TransportsSet.GetEarliestArrivalTimeToLocation( GroundUnitLocation );
			//
		}
		*-/
	}
	if( pUnitTemplate->IsSeaUnit() )
	{
		// Calculate SeaUnit pathway
		CEOSAIRegionPathwayItem* pSeaPathResult = this->InvokeWaterAIRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pSeaPathResult )
		{
			float fTimeToTarget = pSeaPathResult->m_fDistance / pUnitTemplate->GetMovementRate();
			pAIUnitActionIdea->SetDirectMoveToTargetTime( fTimeToTarget );
		}
	}
	if( pUnitTemplate->IsAirUnit() )
	{
		// Calculate AirUnit pathway (estimates time, doesn't actually calculate airbase-step pathway)
		float fTimeToTarget = CEOSAIUnit2ActionIdea::GetAirPathwayTimeToTarget(
					this->GetAIPlayer()->GetPlayerNumber(),
					pAITacticalProject->GetAIBrain(),
					pUnitTemplate,
					this->GetServerCity()->GetLocation(),//UnitStart,
					TargetLocation,
					CEOSAIUnit2ActionIdea::MovementMotive_RoundTrip ); // ASSUME a round trip
		pAIUnitActionIdea->SetDirectMoveToTargetTime( fTimeToTarget );
	}
	return pAIUnitActionIdea;
	//return fTimeToTarget;
}
*/
float CEOSAICity::GetBuildTimeAndTimeToTarget( CEOSAITacticalProject2* pAITacticalProject2, CEOSAIUnitTemplate* pUnitTemplate )
{
	//CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = pAITacticalProject->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
	//CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
	long iAIPlayer = pAITacticalProject2->GetAIPlayer()->GetPlayerNumber();
	CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = pAITacticalProject2->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();

	float fTimeToTarget = 1000000.0f;

	// Build time at this city (include partially completed data)
	float fBuildTime = this->GetTimeToBuild( pUnitTemplate, true );

	//CEOSAILocation TargetLocation = pAITacticalProject->GetClosestPrimaryTargetLocation( GetServerCity()->GetLocation() );
	CEOSAILocation TargetLocation = pAITacticalProject2->GetClosestPrimaryTargetLocation( this->GetLocation() );
	//CEOSAIDesireSpatial* pClosestDesire = NULL;
	//pAITacticalProject->GetClosestPrimaryGoal( this->GetServerCity()->GetLocation(), &pClosestDesire );
	//CEOSAILocation TargetLocation = pClosestDesire->GetLocation();
	if( pUnitTemplate->IsGroundUnit() )
	{
		// Land-Route
		CEOSAIRegionPathwayItem* pLandPathResult = this->InvokeLandMultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandPathResult )
		{
			float fLandPath_TimeToTarget = pLandPathResult->m_fDistance / pUnitTemplate->GetMovementRate();
			fTimeToTarget = fBuildTime + min( fTimeToTarget, fLandPath_TimeToTarget );
		}

		// Land + SeaTransport Route (uses an ideal Transport)
		CEOSAIRegionPathwayItem* pLandSea1PathResult = this->InvokeLandAndSeaTransport1MultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		CEOSAIRegionPathwayItem* pLandSea2PathResult = this->InvokeLandAndSeaTransport2MultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandSea1PathResult )
		{
			ASSERT( pLandSea2PathResult );

			float fSeaTransportMovementRate = pUnitsICanBuildOrHave->GetFastestSeaTransportMovementRate( pUnitTemplate );
			float fThisUnitMovementRate = pUnitTemplate->GetMovementRate();

			float fLandDistance1, fWaterDistance1, fCityDistance1;
			pLandSea1PathResult->GetDistance( &fCityDistance1, &fLandDistance1, &fWaterDistance1 );
			float fLandSeaPath_TimeToTarget1 =
				fCityDistance1  / ( max( fSeaTransportMovementRate, fThisUnitMovementRate )+0.1f ) +
				fLandDistance1  / ( fThisUnitMovementRate+0.1f ) +
				fWaterDistance1 / ( fSeaTransportMovementRate+0.1f );

			float fLandDistance2, fWaterDistance2, fCityDistance2;
			pLandSea2PathResult->GetDistance( &fCityDistance2, &fLandDistance2, &fWaterDistance2 );
			float fLandSeaPath_TimeToTarget2 =
				fCityDistance2  / ( max( fSeaTransportMovementRate, fThisUnitMovementRate )+0.1f ) +
				fLandDistance2  / ( fThisUnitMovementRate+0.1f ) +
				fWaterDistance2 / ( fSeaTransportMovementRate+0.1f );

			ASSERT( fLandDistance1  == fLandDistance2  );
			ASSERT( fWaterDistance1 == fWaterDistance2 );
			float fLandSeaPath_TimeToTarget = min( fLandSeaPath_TimeToTarget1, fLandSeaPath_TimeToTarget2 );

			fTimeToTarget = min( fTimeToTarget, fLandSeaPath_TimeToTarget );

			float fTransportArrivalTime = this->GetBestTransportArrivalTime_CrowsFlight( iAIPlayer, pUnitTemplate );

			fTimeToTarget = max( fBuildTime, fTransportArrivalTime ) + fTimeToTarget;
		}
	}
	if( pUnitTemplate->IsSeaUnit() )
	{
		// Calculate SeaUnit pathway
		CEOSAIRegionPathwayItem* pSeaPathResult = this->InvokeWaterAIRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pSeaPathResult )
		{
			fTimeToTarget = fBuildTime + (pSeaPathResult->m_fDistance / pUnitTemplate->GetMovementRate());
		}
	}
	if( pUnitTemplate->IsAirUnit() )
	{
		// Calculate AirUnit pathway (estimates time, doesn't actually calculate airbase-step pathway)
		fTimeToTarget = 
			fBuildTime +
			CEOSAIUnit2ActionIdea::GetAirPathwayTimeToTarget(
					pAITacticalProject2->GetAIBrain()->GetAIPlayer()->GetPlayerNumber(),
					pAITacticalProject2->GetAIBrain(),
					pUnitTemplate,
					this->GetLocation(),//UnitStart,
					TargetLocation,
					CEOSAIUnit2ActionIdea::MovementMotive_RoundTrip ); // ASSUME a round trip
	}
	return fTimeToTarget;
}

CEOSAIUnit2ActionIdea* CEOSAICity::CreateAIUnitActionIdea( CEOSAITacticalProject2* pAITacticalProject2, CEOSAIUnitTemplate* pAIUnitTemplate )
//CEOSAIUnit2ActionIdea* CEOSAICityActionIdeaNode::CreateAIUnitActionIdea( CUnitTemplate* pUnitTemplate )
{
	CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = pAITacticalProject2->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();

	// Create a UnitActionIdea and write data into it
	CEOSAIUnit2ActionIdea* pAIUnitActionIdea = new CEOSAIUnit2ActionIdea( pAITacticalProject2, this, pAIUnitTemplate );

	float fTimeToTarget = 1000000.0f;

	// Build time at this city (include partially completed data)
	float fBuildTime = this->GetTimeToBuild( pAIUnitTemplate, true );
	pAIUnitActionIdea->SetBuildTime( fBuildTime );

	CEOSAILocation TargetLocation = pAITacticalProject2->GetClosestPrimaryTargetLocation( this->GetLocation() );
	//CEOSAIDesireSpatial* pClosestDesire = NULL;
	//pAITacticalProject->GetClosestPrimaryGoal( this->GetServerCity()->GetLocation(), &pClosestDesire );
	//CEOSAILocation TargetLocation = pClosestDesire->GetLocation();//m_pTacticalProject->GetLocation();
	if( pAIUnitTemplate->IsGroundUnit() )
	{
		// Land-Route
		CEOSAIRegionPathwayItem* pLandPathResult = this->InvokeLandMultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandPathResult )
		{
			float fLandPath_TimeToTarget = pLandPathResult->m_fDistance / pAIUnitTemplate->GetMovementRate();
			//fTimeToTarget = min( fTimeToTarget, fLandPath_TimeToTarget );
			pAIUnitActionIdea->SetDirectMoveToTargetTime( fLandPath_TimeToTarget );
		}

		// Land + SeaTransport Route (uses an ideal Transport)
		CEOSAIRegionPathwayItem* pLandSea1PathResult = this->InvokeLandAndSeaTransport1MultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		CEOSAIRegionPathwayItem* pLandSea2PathResult = this->InvokeLandAndSeaTransport2MultiRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandSea1PathResult )
		{
			ASSERT( pLandSea2PathResult );

			float fSeaTransportMovementRate = pUnitsICanBuildOrHave->GetFastestSeaTransportMovementRate( pAIUnitTemplate );
			float fThisUnitMovementRate = pAIUnitTemplate->GetMovementRate();

			float fLandDistance, fWaterDistance, fCityDistance;
			pLandSea1PathResult->GetDistance( &fCityDistance, &fLandDistance, &fWaterDistance );
			float fLandSeaPath_TimeToTarget1 =
				fCityDistance  / ( max( fSeaTransportMovementRate, fThisUnitMovementRate )+0.1f ) +
				fLandDistance  / ( fThisUnitMovementRate+0.1f ) +
				fWaterDistance / ( fSeaTransportMovementRate+0.1f );

			pLandSea2PathResult->GetDistance( &fCityDistance, &fLandDistance, &fWaterDistance );
			float fLandSeaPath_TimeToTarget2 =
				fCityDistance  / ( max( fSeaTransportMovementRate, fThisUnitMovementRate )+0.1f ) +
				fLandDistance  / ( fThisUnitMovementRate+0.1f ) +
				fWaterDistance / ( fSeaTransportMovementRate+0.1f );

			float fLandSeaPath_TimeToTarget = min( fLandSeaPath_TimeToTarget1, fLandSeaPath_TimeToTarget2 );

			//pAIUnitActionIdea->SetTransportAssistedMoveToTargetTime( fLandSeaPath_TimeToTarget );

			//float fTransportArrivalTime = this->GetBestTransportArrivalTime_CrowsFlight( pUnitTemplate );
			//pAIUnitActionIdea->SetTransportArrivalTime( fTransportArrivalTime );
		}
		/*
		CEOSAIRegionPathwayItem* pLandSeaPathResult = this->InvokeLandAndSeaTransportAIRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pLandSeaPathResult )
		{
			float fSeaTransportMovementRate = pUnitsICanBuildOrHave->GetFastestSeaTransportMovementRate( pUnitTemplate );
			float fThisUnitMovementRate = pUnitTemplate->GetMovementRate();
			float fMovementRate = max( fSeaTransportMovementRate, fThisUnitMovementRate );
			float fLandSeaPath_TimeToTarget = pLandSeaPathResult->m_fDistance / fMovementRate;
			//fTimeToTarget = min( fTimeToTarget, fLandSeaPath_TimeToTarget );

			pAIUnitActionIdea->SetTransportMoveToTargetTime( fLandSeaPath_TimeToTarget );

			// REAL TRANSPORTS - I might want to use this
			//CEOSAITransportsSet  TransportsSet( GetTacticalProject()->GetAIBrain()->GetAIThoughtDatabase(), m_pUnitActorTemplate );
			//m_fTransportToActorTime = TransportsSet.GetEarliestArrivalTimeToLocation( GroundUnitLocation );
			//
		}
		*/
	}
	if( pAIUnitTemplate->IsSeaUnit() )
	{
		// Calculate SeaUnit pathway
		CEOSAIRegionPathwayItem* pSeaPathResult = this->InvokeWaterAIRegionMapToEverywhere()->GetRegionResult( TargetLocation );
		if( pSeaPathResult )
		{
			float fTimeToTarget = pSeaPathResult->m_fDistance / pAIUnitTemplate->GetMovementRate();
			pAIUnitActionIdea->SetDirectMoveToTargetTime( fTimeToTarget );
		}
	}
	if( pAIUnitTemplate->IsAirUnit() )
	{
		// Calculate AirUnit pathway (estimates time, doesn't actually calculate airbase-step pathway)
		float fTimeToTarget = CEOSAIUnit2ActionIdea::GetAirPathwayTimeToTarget(
					pAITacticalProject2->GetAIPlayer()->GetPlayerNumber(),
					pAITacticalProject2->GetAIBrain(),
					pAIUnitTemplate,
					this->GetLocation(),//UnitStart,
					TargetLocation,
					CEOSAIUnit2ActionIdea::MovementMotive_RoundTrip ); // ASSUME a round trip
		pAIUnitActionIdea->SetDirectMoveToTargetTime( fTimeToTarget );
	}
	return pAIUnitActionIdea;
	//return fTimeToTarget;
}

long CEOSAICity::GetBuildingCount( CEOSAIBuildingDescription* pBuildingDescription )
{
	long iCount = 0;
	POSITION pos = m_BuildingList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuildingDescriptionInList = m_BuildingList.GetNext( pos );
		if( pBuildingDescriptionInList == pBuildingDescription ){ iCount++; }
	}
	return iCount;
}

long CEOSAICity::GetBuildingCount( CString strBuilding )
{
	long iCount = 0;
	POSITION pos = m_BuildingList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuildingDescriptionInList = m_BuildingList.GetNext( pos );
		if( pBuildingDescriptionInList->GetInternalName() == strBuilding ){ iCount++; }
	}
	return iCount;
}

long CEOSAICity::GetBuildingCountIncludingPartiallyCompleted( CString strBuilding )
{
	long iCount = GetBuildingCount( strBuilding );
	if( GetPartiallyCompletedItem( strBuilding ) ) iCount++;
	return iCount;
}

long CEOSAICity::GetBuildingCountIncludingBuildQueue( CString strBuilding )
{
	return GetBuildingCount( strBuilding ) + GetBuildQueueCount( strBuilding );
}

long  CEOSAICity::GetBuildQueueCount( CString strItemName )
{
	long iCount = 0;
	POSITION pos = m_AIBuildOrders.GetHeadPosition();
	while( pos )
	{
		//CEOSAIBuildOrder* pOrder = m_AIBuildOrders.GetNext(pos);
		CEOSAIBuildOption* pOrder = m_AIBuildOrders.GetNext(pos);
		//if (pOrder->GetBuildTarget() == strItemName) { iCount++; }
		if (pOrder->GetInternalName() == strItemName) { iCount++; }
	}
	return iCount;
}


long CEOSAICity::GetBuildingLineCount( CString strBuildingLine )
{
	long iCount = 0;
	POSITION pos = m_BuildingList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuildingDescriptionInList = m_BuildingList.GetNext( pos );
		if( pBuildingDescriptionInList->m_strUpgradeLine == strBuildingLine ){ iCount++; }
	}
	return iCount;
}

long CEOSAICity::GetBuildingLineCurrentUpgradeLevel( CString strBuildingLine )
{
	long iUpgradeLevel = 0;
	POSITION pos = m_BuildingList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuildingDescriptionInList = m_BuildingList.GetNext( pos );
		if( pBuildingDescriptionInList->m_strUpgradeLine == strBuildingLine )
		{
			iUpgradeLevel = max( iUpgradeLevel, pBuildingDescriptionInList->m_iUpgradeLevel );
		}
	}
	return iUpgradeLevel;
}

CEOSAIBuildCompletion* CEOSAICity::GetPartiallyCompletedItem( CEOSAIBuildOption* pBuildOption )
{
	POSITION pos = m_PartiallyCompletedList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildCompletion* pItem = m_PartiallyCompletedList.GetNext( pos );
		if( pItem->GetAIBuildOption() == pBuildOption )
		{
			return pItem;
		}
	}
	return NULL;
}

CEOSAIBuildCompletion* CEOSAICity::GetPartiallyCompletedItem( CString strBuildTarget )
{
	POSITION pos = m_PartiallyCompletedList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildCompletion* pItem = m_PartiallyCompletedList.GetNext( pos );
		if( pItem->GetAIBuildOption()->GetInternalName() == strBuildTarget )
		{
			return pItem;
		}
	}

	return NULL;
}



/*
float CEOSAICity::GetHPRepairPerTurn( CUnitTemplate* pUnitTemplate )
{
	float fHPRepairPerTurn = 0.0f;
	POSITION pos = GetServerCity()->GetBuildingList()->GetHeadPosition();
	while( pos )
	{
		CBuildingDescription* pBuildingDesc = GetServerCity()->GetBuildingList()->GetNext( pos );
		if( pUnitTemplate->IsAirUnit() )
		{
			fHPRepairPerTurn += pBuildingDesc->m_fRepairAircraftRate;
		}
		if( pUnitTemplate->IsSeaUnit() )
		{
			fHPRepairPerTurn += pBuildingDesc->m_fRepairSeaUnitRate;
		}
		if( pUnitTemplate->IsGroundUnit() )
		{
			fHPRepairPerTurn += pBuildingDesc->m_fRepairGroundUnitRate;
		}
	}
	return fHPRepairPerTurn;
}
*/
//float CEOSAICity::GetBuildingValue( CEOSAIBuildingDescription* pBuildingDesc )
float CEOSAICity::GetBuildingValue( CEOSAIBuildingDescription* pBuildingDesc )
{
	CEOSAIBuildingValue BuildingValue( this, pBuildingDesc );
	g_pEOSAIInterface->GetBuildingValue( &BuildingValue );

	// If this building already exists and the building count < max count, then the value is 0
	long iBuildingCount = this->GetBuildingCount( pBuildingDesc );
	if( iBuildingCount >= pBuildingDesc->GetMaximumNumberOfCopiesOfThisBuildingInACity() ){ return 0.0f; }

	CEOSAIBrain* pCityOwnerBrain = g_pAIPlayerManager->GetAIPlayer( GetOwner() )->GetAIBrain();
	float fAirUnitMaxHP = pCityOwnerBrain->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetMaxAirUnitHP();
	float fGroundUnitMaxHP = pCityOwnerBrain->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetMaxGroundUnitHP();
	float fSeaUnitMaxHP = pCityOwnerBrain->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetMaxSeaUnitHP();

	// INCOMPLETE - the value needs to take 'national context' into account
	//   Example: Do not build oil refineries if they don't help
	//   Example: Repair functionality is more important near the front; not deep inside the nation
	//CBuildingBasicValue  BuildingValue( GetServerCity(), pBuildingDesc );
	//if( BuildingValue.m_fRepairAircraftRate   > fAirUnitMaxHP ) BuildingValue.m_fRepairAircraftRate = fAirUnitMaxHP;
	//if( BuildingValue.m_fRepairGroundUnitRate > fGroundUnitMaxHP ) BuildingValue.m_fRepairGroundUnitRate = fGroundUnitMaxHP;
	//if( BuildingValue.m_fRepairSeaUnitRate    > fSeaUnitMaxHP ) BuildingValue.m_fRepairSeaUnitRate = fSeaUnitMaxHP;
	float fValue = 0.0f;
	float fInitialCost = pBuildingDesc->GetProductionAndIronCost1();
	float fPerTurnValue = 
		BuildingValue.m_fBonusProduction +
		BuildingValue.m_fBonusAirUnitProduction/2.0f +
		BuildingValue.m_fBonusSeaUnitProduction/2.0f +
		BuildingValue.m_fBonusGroundUnitProduction/2.0f +
		BuildingValue.m_fAdditionalOilProduction/2.0f +
		BuildingValue.m_fReducedOilConsumption/2.0f;
		//( BuildingValue.m_fRepairAircraftRate   / max(1.0f,fAirUnitMaxHP) ) +
		//( BuildingValue.m_fRepairGroundUnitRate / max(1.0f,fGroundUnitMaxHP) ) +
		//( BuildingValue.m_fRepairSeaUnitRate    / max(1.0f,fSeaUnitMaxHP) );
	if( fAirUnitMaxHP    > 1.5f ){ fPerTurnValue += min( fAirUnitMaxHP, pBuildingDesc->m_fRepairAircraftRate ) / fAirUnitMaxHP; }
	if( fGroundUnitMaxHP > 1.5f ){ fPerTurnValue += min( fGroundUnitMaxHP, pBuildingDesc->m_fRepairGroundUnitRate ) / fGroundUnitMaxHP; }
	if( fSeaUnitMaxHP    > 1.5f ){ fPerTurnValue += min( fSeaUnitMaxHP, pBuildingDesc->m_fRepairSeaUnitRate ) / fSeaUnitMaxHP; }

	long iEnablesUnits = pBuildingDesc->m_EnablesUnits.GetCount();
	if( iEnablesUnits > 0 )
	{
		long iNumberOfCities = pCityOwnerBrain->GetAIThoughtDatabase()->GetNumberOfCities();
		long iNumberOfCitiesWithImprovement = pCityOwnerBrain->GetAIThoughtDatabase()->GetNumberOfCitiesWithImprovement( pBuildingDesc->GetInternalName() );
		if( this->GetBuildingCountIncludingPartiallyCompleted( pBuildingDesc->GetInternalName() ) )
		{
			iNumberOfCitiesWithImprovement--;
		}
		ASSERT( iNumberOfCitiesWithImprovement >= 0 && iNumberOfCitiesWithImprovement <= iNumberOfCities );
		float fPercentageWithCityImprovement01 = (float)iNumberOfCitiesWithImprovement / (float)max( 1.0f, iNumberOfCities );

		fPerTurnValue += 
			min( 1.0f, ((float)iNumberOfCities)/2.5f ) * // If I don't have a lot of cities, maybe I shouldn't build it
			(1.0f-fPercentageWithCityImprovement01) *        // If a lot of cities have this improvement, I don't need it so much
			min( 0.8f, 0.4f*sqrt( (float)iEnablesUnits ) );
	}

	float fResShortage01 = pCityOwnerBrain->GetAIPlayer()->GetExperiencingResourceShortage01();
	if( fResShortage01 > 0.1f )
	{
		fPerTurnValue += max( 0.5f, fResShortage01 ) *
			( BuildingValue.m_fBonusProduction +
			  BuildingValue.m_fAdditionalOilProduction/2.0f +
			  BuildingValue.m_fReducedOilConsumption/2.0f );
	}

	if( fPerTurnValue == 0.0f ) return 0.0f;

	float fTimeToRepay = fInitialCost / fPerTurnValue;
	CEOSAIMathFunction  TurnsToRepayValueFunc;
	TurnsToRepayValueFunc.SetInputOutput(   0.0, 10.0f );
	TurnsToRepayValueFunc.SetInputOutput(  10.0,  5.0f );
	TurnsToRepayValueFunc.SetInputOutput(  20.0,  3.5f );
	TurnsToRepayValueFunc.SetInputOutput(  40.0,  2.0f );
	TurnsToRepayValueFunc.SetInputOutput( 100.0,  1.0f );
	TurnsToRepayValueFunc.SetInputOutput( 200.0,  0.0f );
	fValue = TurnsToRepayValueFunc.GetOutput( fTimeToRepay );

	fValue *= ( 1.0f - GetGroundSeaDanger01() );
	fValue *= ( 1.0f - 0.5f*GetStrategicBomberDanger01() );
	return fValue;

	// The full value of a building depends on a number of national factors.
	//   For example, what's the value of an Oil Refinery?
	//   It's based on:
	//      Likeilhood of this city's capture or bombing (and my anti-aircraft/missile defense)
	//        Past history of bombing (this city and against my nation in general)
	//      The nation's oil field/oil refinery balance
	//      The nation's oil surplus/oil shortage
	//   There may be times when the AI needs to identify which cities are best for a building
	//     For example, if I could use one more Oil Refinery, the AI should look around for the best
	//     city rather than having each city come up with a value (although, this is complicated since
	//     a city's other build options are relevant for finding the best city for an Oil Refinery)
}

float CEOSAICity::GetStrategicBomberDanger01()
{
	if( m_fStrategicBomberDanger01 >= 0.0f ) return m_fStrategicBomberDanger01;
	m_fStrategicBomberDanger01 = 0.0f;

	long iCityOwner = GetOwner();

	// Check how close I am to the "front" of my enemies
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CWorldBuildDesc* pWorldBuildDesc = m_pAIBrain->GetWorldDescServer()->GetWorldBuildDesc();
	//long iNumberOfPlayers = pWorldBuildDesc->GetNumberOfPlayers();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	//CAICommonData* pAICommonData = pWorldDescServer->GetAICommonData();
	for( long iPlayer=1; iPlayer<iNumberOfPlayers; iPlayer++ )
	{
		if( iPlayer == iCityOwner ) continue;
		EOSAIEnumForeignRelations eRelations = g_pEOSAICommonData->GetForeignRelations( iPlayer, iCityOwner );
		float fFeelings = g_pEOSAICommonData->GetGlobalForeignRelations()->GetFeelings01( iCityOwner, iPlayer );
		if( eRelations == EOSAIEnumForeignRelations::enum_War || fFeelings < 0.4f )
		{
			// Does the player know about this?
			float fDiscoveredMultiplier = 1.0f;
			/* 
			This is good code - it's used to figure out if the other player knows about this area.
			But, with the change to GenieAI, it was removed because the objects don't exist in the AI system
			EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iPlayer );
			if( pAIPlayer && pAIPlayer->AICheat_KnowsMapAndPoiInformation() )
			{
				fDiscoveredMultiplier = 1.0f;
			}
			else
			{
				CPoiObserver* pPoiObserver = GetServerCity()->GetPoiObserver( iPlayer );
				if( pPoiObserver == NULL ){ fDiscoveredMultiplier = 0.5f; } // Player hasn't discovered this city
			}
			*/

			// Lookup AirUnit Access
			CEOSAIMultiRegionNationwidePathways* pNationwidePathways = g_pEOSAICommonData->GetNationwidePathways( GetOwner() );
			CEOSAIMultiRegionAttackAccess* pAttackAccess = pNationwidePathways->GetMultiRegionAttackAccess( GetInitialState()->GetMultiRegion()->GetId() );
			if( pAttackAccess )
			{
				float fAttackAccess01 = fDiscoveredMultiplier * pAttackAccess->GetAirUnitAccess01();
				m_fStrategicBomberDanger01 = max( m_fStrategicBomberDanger01, fAttackAccess01 );
			}
		}
	}
	return m_fStrategicBomberDanger01;
}

// Based on Enemy GroundSeaAttackAccess01, nearby Enemy-Neutral versus Friendly units, 
float CEOSAICity::GetGroundSeaDanger01()
{
	if( m_fGroundSeaDanger01 >= 0.0f ) return m_fGroundSeaDanger01;
	m_fGroundSeaDanger01 = 0.0f;

	// Check how close I am to the "front" of my enemies
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CWorldBuildDesc* pWorldBuildDesc = m_pAIBrain->GetWorldDescServer()->GetWorldBuildDesc();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	//CAICommonData* pAICommonData = pWorldDescServer->GetAICommonData();
	float fEnemyGroundSeaAttackAccess01 = 0.0f;
	for( long iPlayer=1; iPlayer<iNumberOfPlayers; iPlayer++ )
	{
		if( g_pEOSAICommonData->GetForeignRelations( iPlayer, GetOwner() ).IsEnemy() )
		{
			CEOSAIMultiRegionNationwidePathways* pNationwidePathways = g_pEOSAICommonData->GetNationwidePathways( GetOwner() );
			CEOSAIMultiRegionAttackAccess* pAttackAccess = pNationwidePathways->GetMultiRegionAttackAccess( GetInitialState()->GetMultiRegion()->GetId() );
			if( pAttackAccess )
			{
				float fAttackAccess01 = pAttackAccess->GetGroundSeaUnitAccess01();
				fEnemyGroundSeaAttackAccess01 = max( fEnemyGroundSeaAttackAccess01, fAttackAccess01 );
			}
		}
	}

	// Calculate nearby enemies, nearby friendly units
	float fFriendlyCombatSignificance = 0.0f;
	float fEnemyCombatSignificance = 0.0f;
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
		if( pAIUnit && pAIUnit->GetOwner() > 0 &&
			( pAIUnit->IsSeaUnit() || pAIUnit->IsGroundUnit() ) )
		{
			if( pAIUnit->GetForeignRelationsTo( GetOwner() ) <= EOSAIEnumForeignRelations::enum_Neutral )
			{
				//float fDistance = pWorldBuildDesc->GetPixelDistance( GetInitialState()->GetLocation(), pAIUnit->GetInitialState()->GetLocation() );
				float fDistance = g_pWorldDistanceTool->GetDistance( GetInitialState()->GetLocation(), pAIUnit->GetInitialState()->GetLocation() );
				if( fDistance < 150.0f )
				{
					float fForeignRelationsSignificance01 = 1.0f;
					if( pAIUnit->GetForeignRelationsTo( GetOwner() ) == EOSAIEnumForeignRelations::enum_Neutral ){ fForeignRelationsSignificance01 = 0.2f; }

					float fDistanceSignificance01 = fDistance / 75.0f;
					if( fDistanceSignificance01 > 1.0f ) fDistanceSignificance01 = 1.0f;

					float fCombatSignificance = pAIUnit->GetAIUnitTemplate()->GetAICombatSignificance();
					if( pAIUnit->IsGroundUnit() == false ) fCombatSignificance *= 0.5f;

					fEnemyCombatSignificance += 
						fForeignRelationsSignificance01 * fDistanceSignificance01 * fCombatSignificance;
				}
			}
			if( pAIUnit->GetForeignRelationsTo( GetOwner() ) >= EOSAIEnumForeignRelations::enum_Alliance )
			{
				//float fDistance = pWorldBuildDesc->GetPixelDistance( GetInitialState()->GetLocation(), pAIUnit->GetInitialState()->GetLocation() );
				float fDistance = g_pWorldDistanceTool->GetDistance( GetInitialState()->GetLocation(), pAIUnit->GetInitialState()->GetLocation() );
				if( fDistance < 40.0f )
				{
					float fForeignRelationsSignificance01 = 1.0f;
					if( pAIUnit->GetForeignRelationsTo( GetOwner() ) == EOSAIEnumForeignRelations::enum_Alliance ){ fForeignRelationsSignificance01 = 0.5f; }

					float fCombatSignificance = pAIUnit->GetAIUnitTemplate()->GetAICombatSignificance();
					if( pAIUnit->IsGroundUnit() == false ) fCombatSignificance = 0.0f;

					fFriendlyCombatSignificance += 
						fForeignRelationsSignificance01 * fCombatSignificance;
				}
			}
		}
	}

	if( fEnemyCombatSignificance > 0.0f && fFriendlyCombatSignificance == 0.0f )
	{
		m_fGroundSeaDanger01 = 1.0f;
	}
	else
	{
		m_fGroundSeaDanger01 = 
			max( 0.0f, fEnemyGroundSeaAttackAccess01-0.6f ) + 
			fEnemyCombatSignificance / 40.0f +
			( fEnemyCombatSignificance - fFriendlyCombatSignificance ) / 40.0f;

		if( m_fGroundSeaDanger01 < 0.0f ) m_fGroundSeaDanger01 = 0.0f;
		if( m_fGroundSeaDanger01 > 1.0f ) m_fGroundSeaDanger01 = 1.0f;
	}
	return m_fGroundSeaDanger01;
}

//
/*
void  CEOSAICity::CalculateUnexploredArea()
{
	long iNumberOfRegions = 0;

	// Construct the unexplored area range information
	CWorldDesc* pWorldDesc = m_pAIBrain->GetAIPlayer()->GetWorldDescPlayer();
	POSITION pos = m_pAIBrain->GetAIObjectList()->GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_pAIBrain->GetAIObjectList()->GetNext( pos );
		CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pAIObject );
		if( pAIRegion )
		{
			iNumberOfRegions++;
			float fDistance = pWorldDesc->GetPixelDistance( m_pCity->GetLocation(), pAIRegion->GetLocation() );
			AddRegion( fDistance, pAIRegion->GetSizeOfRegion() );
			if( pAIRegion->HasBeenExplored() == false )
			{
				AddUnexploredRegion( fDistance, pAIRegion->GetSizeOfRegion() );
			}
		}
	}
	// If this ASSERT fails, it means that the AIRegions haven't been constructed yet.
	//   Code elsewhere needs to be adjusted so that they exist before calling this function.
	ASSERT( iNumberOfRegions > 0 );
}
*/
float CEOSAICity::GetTotalProduction()
{
	return m_fProduction;
	//ASSERT( false );
	//return GetTotalProduction( false,false );
	//return 0.0f;
}

float CEOSAICity::GetTotalProduction( bool bIncludeRefugeesInTheCalculation, CEOSAIBuildOption* pBuildOption )
{
	// TODO
	//ASSERT( false );
	//return 0.0f;
	return m_fProduction;
}

float CEOSAICity::GetTotalProduction( bool bIncludeRefugeesInTheCalculation, CEOSAIUnitTemplate* pBuildUnitTemplate )
{
	// TODO
	//ASSERT( false );
	//return 0.0f;
	return m_fProduction;
}

/*
float CEOSAICity::AmountOfOilNeededOverTimeperiod( float fDeltaTime )
{
	float fResNeededThisTurn = 0.0f;
	if( m_AIBuildOrders.IsEmpty() == FALSE && m_bPauseProduction == false )
	{
		float fProduction = GetTotalProduction( true, true );// * GetPlayerProductionMultiplier();

		CEOSAIBuildOrder* pOrder = m_AIBuildOrders.GetHead();
		CEOSAIBuildOption* pBuildOption = pOrder->GetAIBuildOption();

		if( pBuildOption->IsMoney() == false )
		{
			// Assume all production points will go to the current item
			float fBuildPercentageThisTimeperiod = fDeltaTime * fProduction / pBuildOption->ProductionNeeded();
			fResNeededThisTurn = fBuildPercentageThisTimeperiod * pBuildOption->GetResourceCost( _T("Oil") );
		}
	}
	// Production does not consume oil (once I generalize the build system, it might)
	return fResNeededThisTurn + AmountOfOilNeededOverTimeperiodIgnoreProduction( fDeltaTime );
}

float CCity::AmountOfOilNeededOverTimeperiodIgnoreProduction( float fDeltaTime )
{
	// Oil consumed over one turn
	float fOilConsumption = m_fPopulation * g_pEOSAICommonData-->GetActiveUnitset()->GetCityOilConsumption();

	float fOilConsumptionReduction = 0.0f;
	POSITION pos = m_AIBuildingList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuildingDesc = m_AIBuildingList.GetNext( pos );
		fOilConsumptionReduction += pBuildingDesc->m_fCityOilConsumptionReduction;
	}
	fOilConsumption -= fOilConsumptionReduction;
	if( fOilConsumption < 0.0f ){ fOilConsumption = 0.0f; }
	float fOilNeededThisTurn = fDeltaTime * fOilConsumption;

	// Add-up the iron used by buildings
	pos = m_BuildingList.GetHeadPosition();
	while( pos )
	{
		CBuildingDescription* pBuildingDesc = m_BuildingList.GetNext( pos );
		//fOilNeededThisTurn += fDeltaTime * pBuildingDesc->GetMaintenanceCost( _T("Oil") );
		float fDelta = pBuildingDesc->GetProductionAndConsumption( _T("Oil") );
		if( fDelta < 0.0f )
		{
			fOilNeededThisTurn += fDeltaTime * -fDelta;
		}
	}

	// Add the Player-production multiplier
	//float fProduction = m_fPopulation;// * GetPlayerProductionMultiplier();
	return fOilNeededThisTurn;
}

float CEOSAICity::AmountOfFoodNeededOverTimeperiod( float fDeltaTime )
{
	float fResNeededThisTurn = 0.0f;
	if( m_AIBuildOrders.IsEmpty() == FALSE && m_bPauseProduction == false )
	{
		float fProduction = GetTotalProduction( true, true );// * GetPlayerProductionMultiplier();

		CEOSAIBuildOrder* pOrder = m_AIBuildOrders.GetHead();
		CEOSAIBuildOption* pBuildOption = pOrder->GetAIBuildOption();

		if( pBuildOption->IsMoney() == false )
		{
			// Assume all production points will go to the current item
			float fBuildPercentageThisTimeperiod = fDeltaTime * fProduction / pBuildOption->ProductionNeeded();
			fResNeededThisTurn = fBuildPercentageThisTimeperiod * pBuildOption->GetResourceCost( _T("Food") );
		}
	}
	// Production does not consume oil (once I generalize the build system, it might)
	return fResNeededThisTurn + AmountOfFoodNeededOverTimeperiodIgnoreProduction( fDeltaTime );
}

float CCity::AmountOfFoodNeededOverTimeperiodIgnoreProduction( float fDeltaTime )
{
	// Add the Player-production multiplier
	//float fProduction = m_fPopulation;// * GetPlayerProductionMultiplier();
	//return ( m_fPopulation * GetCommonState()->GetActiveUnitset()->GetCityFoodConsumption() ) * ( 1.0f-GetFoodCredits() );
	// Cities don't get food credits
	float fFoodNeededThisTurn = fDeltaTime * m_fPopulation * GetCommonState()->GetActiveUnitset()->GetCityFoodConsumption();

	// Add-up the iron used by buildings
	POSITION pos = m_AIBuildingList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuildingDesc = m_BuildingList.GetNext( pos );
		//fFoodNeededThisTurn += fDeltaTime * pBuildingDesc->GetMaintenanceCost( _T("Food") );
		float fDelta = pBuildingDesc->GetProductionAndConsumption( _T("Food") );
		if( fDelta < 0.0f )
		{
			fFoodNeededThisTurn += fDeltaTime * -fDelta;
		}
	}
	return fFoodNeededThisTurn;
}
*/
/*
long  CEOSAICity::GetObjectId()
{
	return m_pServerCity->ObjectId();
}
*/
/*
void CEOSAICity::AppendBuildAction( CEOSAICityBuildAction* pAICityBuildAction )
{
	m_CityBuildActions.AddTail( pAICityBuildAction );
	m_pServerCity->AddToBuildQueue( pAICityBuildAction->GetBuildOption() );
}
*/
/*
float CEOSAICity::GetCompletionTimeForNewBuildOption( CEOSAIBuildOption* pNewBuildOption )
{
	//return m_pServerCity->GetCompletionTimeForNewBuildOption( pNewBuildOption );
}
*/

void CEOSAICity::InvokeMultiRegionPathwaysToEverywhere()
{
	InvokeLandMultiRegionMapToEverywhere();
	InvokeLandAndSeaTransport1MultiRegionMapToEverywhere();
	InvokeLandAndSeaTransport2MultiRegionMapToEverywhere();
	InvokeWaterMultiRegionMapToEverywhere();
	InvokeAirMultiRegionMapToEverywhere();
	/*
	InvokeLandAIRegionMapToEverywhere();
	InvokeLandAndSeaTransport1AIRegionMapToEverywhere();
	InvokeLandAndSeaTransport2AIRegionMapToEverywhere();
	InvokeWaterAIRegionMapToEverywhere();
	InvokeAirAIRegionMapToEverywhere();
	*/
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeWaterAIRegionMapToEverywhere()
{
	if( m_WaterAIRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		CEOSAITerrainBasedValue WaterMovementDesc;
		WaterMovementDesc.SetAllWaterValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsWater( true );//false );//true );
		RegionPathfinder.SetMovementRate( &WaterMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_WaterAIRegionMapToEverywhere );

		#ifdef _DEBUG
		long iCount = 0;
		for( long i=0; i<m_WaterAIRegionMapToEverywhere.GetArraySize(); i++ )
		{
			if( m_WaterAIRegionMapToEverywhere.GetRegionResult( i ) != NULL )
			{
				iCount++;
			}
		}
		ASSERT( iCount > 0 );
		#endif _DEBUG
	}
	return &m_WaterAIRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeWaterMultiRegionMapToEverywhere()
{
	if( m_WaterMultiRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		CEOSAITerrainBasedValue WaterMovementDesc;
		WaterMovementDesc.SetAllWaterValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetMultiRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsWater( true );//false );//true );
		RegionPathfinder.SetMovementRate( &WaterMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_WaterMultiRegionMapToEverywhere );

		#ifdef _DEBUG
		long iCount = 0;
		for( long i=0; i<m_WaterMultiRegionMapToEverywhere.GetArraySize(); i++ )
		{
			if( m_WaterMultiRegionMapToEverywhere.GetRegionResult( i ) != NULL )
			{
				iCount++;
			}
		}
		ASSERT( iCount > 0 );
		#endif _DEBUG
	}
	return &m_WaterMultiRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeLandAIRegionMapToEverywhere()
{
	if( m_LandAIRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		CEOSAITerrainBasedValue LandMovementDesc;
		LandMovementDesc.SetAllLandValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandAIRegionMapToEverywhere );
	}
	return &m_LandAIRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeLandAndSeaTransport1AIRegionMapToEverywhere()
{
	if( m_LandAndImaginarySeaTransport1AIRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		//CTerrainBasedValue LandMovementDesc;
		//LandMovementDesc.SetAllLandValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetLandDistanceMultiplier( 1.0f );
		RegionPathfinder.SetSeaDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandAndImaginarySeaTransport1AIRegionMapToEverywhere );
	}
	return &m_LandAndImaginarySeaTransport1AIRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeLandAndSeaTransport2AIRegionMapToEverywhere()
{
	if( m_LandAndImaginarySeaTransport2AIRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		//CTerrainBasedValue LandMovementDesc;
		//LandMovementDesc.SetAllLandValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetLandDistanceMultiplier( 1.0f );
		RegionPathfinder.SetSeaDistanceMultiplier( 0.5f );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandAndImaginarySeaTransport2AIRegionMapToEverywhere );
	}
	return &m_LandAndImaginarySeaTransport2AIRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeLandMultiRegionMapToEverywhere()
{
	if( m_LandMultiRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		CEOSAITerrainBasedValue LandMovementDesc;
		LandMovementDesc.SetAllLandValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetMultiRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandMultiRegionMapToEverywhere );
	}
	return &m_LandMultiRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeLandAndSeaTransport1MultiRegionMapToEverywhere() // Favor land-pathway
{
	if( m_LandAndImaginarySeaTransport1MultiRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		//CTerrainBasedValue LandMovementDesc;
		//LandMovementDesc.SetAllLandValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetMultiRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetLandDistanceMultiplier( 1.0f );
		RegionPathfinder.SetSeaDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandAndImaginarySeaTransport1MultiRegionMapToEverywhere );
	}
	return &m_LandAndImaginarySeaTransport1MultiRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeLandAndSeaTransport2MultiRegionMapToEverywhere() // Favor sea-pathway
{
	if( m_LandAndImaginarySeaTransport2MultiRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		//CTerrainBasedValue LandMovementDesc;
		//LandMovementDesc.SetAllLandValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetMultiRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetLandDistanceMultiplier( 1.0f );
		RegionPathfinder.SetSeaDistanceMultiplier( 0.5f );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandAndImaginarySeaTransport2MultiRegionMapToEverywhere );
	}
	return &m_LandAndImaginarySeaTransport2MultiRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeAirAIRegionMapToEverywhere()
{
	if( m_AirAIRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		//CTerrainBasedValue MovementDesc;
		//MovementDesc.SetAllValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		//RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_AirAIRegionMapToEverywhere );
	}
	return &m_AirAIRegionMapToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAICity::InvokeAirMultiRegionMapToEverywhere()
{
	if( m_AirMultiRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		//CTerrainBasedValue MovementDesc;
		//MovementDesc.SetAllValuesToOne();

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetMultiRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		//RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_AirMultiRegionMapToEverywhere );
	}
	return &m_AirMultiRegionMapToEverywhere;
}

/*
float CEOSAICity::GetUnexploredPercent( float fRange )
{
	if( fRange <  100.0f ){ return 0.0f; }
	if( fRange <  150.0f ){ return m_fUnexploredArea_Range100 / m_fArea_Range100; }
	if( fRange <  200.0f ){ return m_fUnexploredArea_Range150 / m_fArea_Range150; }
	if( fRange <  250.0f ){ return m_fUnexploredArea_Range200 / m_fArea_Range200; }
	if( fRange <  300.0f ){ return m_fUnexploredArea_Range250 / m_fArea_Range250; }
	if( fRange <  400.0f ){ return m_fUnexploredArea_Range300 / m_fArea_Range300; }
	if( fRange <  500.0f ){ return m_fUnexploredArea_Range400 / m_fArea_Range400; }
	if( fRange <  600.0f ){ return m_fUnexploredArea_Range500 / m_fArea_Range500; }
	if( fRange <  700.0f ){ return m_fUnexploredArea_Range600 / m_fArea_Range600; }
	if( fRange <  800.0f ){ return m_fUnexploredArea_Range700 / m_fArea_Range700; }
	if( fRange <  900.0f ){ return m_fUnexploredArea_Range800 / m_fArea_Range800; }
	if( fRange < 1000.0f ){ return m_fUnexploredArea_Range900 / m_fArea_Range900; }
	if( fRange < 1200.0f ){ return m_fUnexploredArea_Range1000 / m_fArea_Range1000; }
	if( fRange < 1500.0f ){ return m_fUnexploredArea_Range1200 / m_fArea_Range1200; }
	return m_fUnexploredArea_Range1500 / m_fArea_Range1500;
}

void CEOSAICity::AddRegion( float fDistance, long iSize )
{
	if( fDistance >  1500.0f ) return;
	if( fDistance <=  100.0f ){ m_fArea_Range100 += iSize; }
	if( fDistance <=  150.0f ){ m_fArea_Range150 += iSize; }
	if( fDistance <=  200.0f ){ m_fArea_Range200 += iSize; }
	if( fDistance <=  250.0f ){ m_fArea_Range250 += iSize; }
	if( fDistance <=  300.0f ){ m_fArea_Range300 += iSize; }
	if( fDistance <=  400.0f ){ m_fArea_Range400 += iSize; }
	if( fDistance <=  500.0f ){ m_fArea_Range500 += iSize; }
	if( fDistance <=  600.0f ){ m_fArea_Range600 += iSize; }
	if( fDistance <=  700.0f ){ m_fArea_Range700 += iSize; }
	if( fDistance <=  800.0f ){ m_fArea_Range800 += iSize; }
	if( fDistance <=  900.0f ){ m_fArea_Range900 += iSize; }
	if( fDistance <= 1000.0f ){ m_fArea_Range1000 += iSize; }
	if( fDistance <= 1200.0f ){ m_fArea_Range1200 += iSize; }
	if( fDistance <= 1500.0f ){ m_fArea_Range1500 += iSize; }
}

void CEOSAICity::AddUnexploredRegion( float fDistance, long iSize )
{
	if( fDistance >  1500.0f ) return;
	if( fDistance <=  100.0f ){ m_fUnexploredArea_Range100 += iSize; }
	if( fDistance <=  150.0f ){ m_fUnexploredArea_Range150 += iSize; }
	if( fDistance <=  200.0f ){ m_fUnexploredArea_Range200 += iSize; }
	if( fDistance <=  250.0f ){ m_fUnexploredArea_Range250 += iSize; }
	if( fDistance <=  300.0f ){ m_fUnexploredArea_Range300 += iSize; }
	if( fDistance <=  400.0f ){ m_fUnexploredArea_Range400 += iSize; }
	if( fDistance <=  500.0f ){ m_fUnexploredArea_Range500 += iSize; }
	if( fDistance <=  600.0f ){ m_fUnexploredArea_Range600 += iSize; }
	if( fDistance <=  700.0f ){ m_fUnexploredArea_Range700 += iSize; }
	if( fDistance <=  800.0f ){ m_fUnexploredArea_Range800 += iSize; }
	if( fDistance <=  900.0f ){ m_fUnexploredArea_Range900 += iSize; }
	if( fDistance <= 1000.0f ){ m_fUnexploredArea_Range1000 += iSize; }
	if( fDistance <= 1200.0f ){ m_fUnexploredArea_Range1200 += iSize; }
	if( fDistance <= 1500.0f ){ m_fUnexploredArea_Range1500 += iSize; }
}
*/
/*
void CEOSAICity::PlanWhatToBuild_old()
{
	if( m_ProductionList.IsEmpty() == FALSE ) return;

	CEOSAIGeoLand* pMyAIGeoLand = m_pAIBrain->GetAIGeo( GetInitialGeo() );
	ASSERT( pMyAIGeoLand );

	// INCOMPLETE: Figure out what is on the geo, what is being produced, what is on it's way (transport) to this geo
	//   Then I can figure out what needs aren't being adequately met.

	float fBestScore = 0.0f;
	CAIProductionOption* pBestAIProductionOption = NULL;

	//CAIMilitarySummary UnsatisfiedCapability = *(pMyAIGeoLand->GetUnsatisfiedMilitaryCapability());

	CString strNationName = m_pAIBrain->GetAIPlayer()->GetWorldDesc()->GetNationName();
	POSITION pos = m_pAIBrain->GetAIBrain()->GetProductionOptions()->GetHeadPosition();
	while( pos )
	{
		CAIProductionOption* pProdOption = m_pAIBrain->GetAIBrain()->GetProductionOptions()->GetNext( pos );

		if( GetCommonState()->GetBuildOptions()->CanBuild( strNationName, pProdOption->m_pBuildOption ) )
		{
			if( pProdOption->m_pBuildOption->GetUnitTemplate() )
			{
				CUnitTemplate* pUnitTemplate = pProdOption->m_pBuildOption->GetUnitTemplate();

				if( m_pCity->IsOnAMajorBodyOfWater() == false && 
					pUnitTemplate->IsSeaUnit() )
				{
					continue;
				}

				// If I have partially completed this item, consider finishing it.
				float fCost = pProdOption->m_fCost;

				POSITION pos = m_pCity->GetBuildCompletionList()->GetHeadPosition();
				while( pos )
				{
					CBuildCompletion* pBuildCompletion = m_pCity->GetBuildCompletionList()->GetNext( pos );
					if( pBuildCompletion->GetBuildOption() == pProdOption->m_pBuildOption )
					{
						float fPercentComplete = pBuildCompletion->GetPercentComplete();
						fCost *= ( 1.0f - fPercentComplete );
					}
				}
				//CList< CBuildCompletion*, CBuildCompletion* > m_PartiallyCompletedList; // Items which are partially completed (may or may not be in build queue).

				/-*
				CAIMilitarySummary UnitCapability;
				UnitCapability.SetAndDivideByCost( pUnitTemplate, fCost/10.0f );
				CAIMilitarySummary* pUnsatisfiedNeeds = pMyAIGeoLand->GetCombinedUnsatisfiedMilitaryCapability();
				float fScore = pUnsatisfiedNeeds->CalculateSatisfaction( UnitCapability );
				if( fScore > fBestScore )
				{
					pBestAIProductionOption = pProdOption;
					fBestScore = fScore;
				}
				*-/
			}
		}
	}
	// Build that unit
	if( pBestAIProductionOption )
	{
		CAIProductionTask* pAIProductionTask = new CAIProductionTask( this, pBestAIProductionOption );
		m_ProductionList.AddTail( pAIProductionTask );

		//
		// I disabled this because I need to rewrite it.
		// I disabled this because I need to rewrite it.
		// I disabled this because I need to rewrite it.
		//
		/-*
		// Update the geo and Nation Capabilities
		pMyAIGeoLand->EvaluateAvailableMilitaryCapability();
		m_pAIBrain->EvaluateAvailableMilitaryCapability();
		m_pAIBrain->CalculateUnsatisfiedMilitaryCapability();
		pMyAIGeoLand->CalculateUnsatisfiedMilitaryCapability();
		*-/
	}

	// debug
	#ifdef _DEBUG
	if( m_ProductionList.IsEmpty() )
	{
		ASSERT( false );
		//pMyAIGeoLand->EvaluateGeoNeeds();
		//PlanWhatToBuild();
	}
	#endif
}
*/
/*
void CEOSAICity::SetProductionTask( CAIProductionOption* pProductionOption )
{
	m_pAssignedProductionTask = pProductionOption;
	m_fAssignedProductionTaskTimeUntilCompletion = pProductionOption->GetSimpleTimeToComplete( m_pServerCity );
}
*/
/*
void CEOSAICity::AddProductionTask( CAIProductionTask* pPreviousProd,
								 CAIProductionTask* pNewProd )
{
	if( pPreviousProd )
	{
		POSITION pos = m_ProductionList.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CAIProductionTask* pExistingProd = m_ProductionList.GetNext( pos );
			if( pExistingProd )
			{
				// I need to make sure it is inserting this at the right position
				//   I might need to call InsertAfter( pos, pNewProd ) instead of
				//   InsertAfter( prevPos, pNewProd )
				ASSERT( false );
				m_ProductionList.InsertAfter( prevPos, pNewProd );
			}
		}
	}
	else
	{
		m_ProductionList.AddTail( pNewProd );
	}
	RecalculateTheEndTimesOfAllProductionTasks();
	UpdateTheUnitSymbolInitialTimes();
}
*/
long CEOSAICity::GetNumberOfGroundUnitsInside()
{
	long iNumberOfGroundUnitsInCity = 0;
	//POSITION pos = GetServerCity()->GetUnitsInsideMe()->GetHeadPosition();
	//POSITION pos = m_UnitsInsideMe.GetHeadPosition();
	POSITION pos = m_InitialState.GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		//CEOSAIUnit2* pAIUnit = m_UnitsInsideMe.GetNext( pos );
		//CEOSAIUnit2* pAIUnit = m_InitialState.GetContaineesList()->GetNext( pos );
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialState.GetContaineesList()->GetNext( pos );
		//if( pAIPoiObject->GetAIPoiObjectType() == EnumPoiObjectType::enum_Unit )
		CEOSAIUnit2* pAIUnit = dynamic_cast<CEOSAIUnit2*>( pAIPoiMobile );
		if( pAIUnit ) //pAIPoiMobile->GetAIPoiObjectType() == EnumPoiObjectType::enum_Unit )
		{
			//CEOSAIUnit2* pAIUnit = (CEOSAIUnit2*) pAIPoiObject;
			if( pAIUnit->IsGroundUnit() ){ iNumberOfGroundUnitsInCity++; }
		}
	}
	return iNumberOfGroundUnitsInCity;
}

void CEOSAICity::ActionScheduler_CreateBuildOrder( CEOSAIBuildOption* pEOSAIBuildOption )
{
	// I don't actually create build orders anymore, instead, the game looks at the AI values and changes them locally.
	g_pEOSAIInterface->SetBuildOrder( this->GetOwner(), this, pEOSAIBuildOption );
	AppendBuildOrder( pEOSAIBuildOption);

	//ASSERT( false ); // Fix this
	/*
	ASSERT( m_pServerCity->GetOwner() == m_pAIBrain->GetAIPlayerNumber() );
	//if( m_pServerCity->GetOwner() == m_pAIBrain->GetAIPlayerNumber() )
	if( this->GetOwner() == m_pAIBrain->GetAIPlayerNumber() )
	{
		m_pServerCity->AddToBuildQueue( pBuildOption );//GetProductionTask()->m_pBuildOption );
	}
	*/
}

void CEOSAICity::ActionScheduler_CreateBuildOrder( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	//ASSERT( false ); // Fix this
	//ASSERT( m_pServerCity->GetOwner() == m_pAIBrain->GetAIPlayerNumber() );
	//if( m_pServerCity->GetOwner() == m_pAIBrain->GetAIPlayerNumber() )
	//{
	//	m_pServerCity->AddToBuildQueue( pAIUnitTemplate->GetInternalName() );//GetProductionTask()->m_pBuildOption );
	//}
	g_pEOSAIInterface->SetBuildOrder( this->GetOwner(), this, pAIUnitTemplate->GetInternalName() );
}

float CEOSAICity::GetTimeUntilCompletionOfBuildQueue()
{
	if( m_AIBuildOrders.IsEmpty() ) return 0.0f;

	//if( m_bRepeatLastItem ) return 1000.f; // infinite

	float fTime = 0.0f;
	float fCityProduction = GetTotalProduction( true,true );

	long iCount = 0;
	POSITION pos = m_AIBuildOrders.GetHeadPosition();
	while( pos )
	{
		//CEOSAIBuildOrder* pBuildOrder = m_AIBuildOrders.GetNext(pos);
		CEOSAIBuildOption* pBuildOrder = m_AIBuildOrders.GetNext(pos);
		iCount++;

		bool bExistsPreviouslyInTheList = false;
		long iSecondCount = 0;
		POSITION pos2 = m_AIBuildOrders.GetHeadPosition();
		while( pos2 )
		{
			//CEOSAIBuildOrder* pBuildOrder2 = m_AIBuildOrders.GetNext(pos2);
			CEOSAIBuildOption* pBuildOrder2 = m_AIBuildOrders.GetNext(pos2);
			iSecondCount++;

			if( iSecondCount < iCount )
			{
				//if (pBuildOrder2->GetAIBuildOption() == pBuildOrder->GetAIBuildOption())
				if (pBuildOrder2 == pBuildOrder)
				{
					bExistsPreviouslyInTheList = true;
				}
			}
			if( iSecondCount == iCount ){ break; }
		}

		ASSERT( fCityProduction > 0.0f );
		//float fTimeForThisBuildOption = pBuildOrder->GetAIBuildOption()->GetProductionCost() / max(0.1f, fCityProduction);
		float fTimeForThisBuildOption = pBuildOrder->GetProductionCost() / max(0.1f, fCityProduction);
		if( pBuildOrder->IsMoney() ){ fTimeForThisBuildOption = 1000.0f; }
		if( bExistsPreviouslyInTheList == false )
		{
			//CEOSAIBuildCompletion* pBuildCompletion = GetPartiallyCompletedItem(pBuildOrder->GetAIBuildOption()->GetInternalName());
			CEOSAIBuildCompletion* pBuildCompletion = GetPartiallyCompletedItem(pBuildOrder->GetInternalName());
			if( pBuildCompletion )
			{
				ASSERT( fCityProduction > 0.0f );
				float fTimeInvested = pBuildCompletion->GetProductionInvested() / max( 0.1f, fCityProduction );
				fTimeForThisBuildOption -= fTimeInvested;
			}
		}
		fTime += fTimeForThisBuildOption;
	}

	return fTime;
}

float CEOSAICity::GetPartiallyCompletedItems01()
{
	float fCompletion01 = 0.0f;
	POSITION pos = m_PartiallyCompletedList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildCompletion* pItem = m_PartiallyCompletedList.GetNext( pos );
		fCompletion01 = max( fCompletion01, pItem->GetPercentComplete() );
	}
	return fCompletion01;
}


bool CEOSAICity::CanBuild( CEOSAIBuildOption* pBuildOption, EnumGoal eGoal, bool bAllowOldTechnologies, CStringList* pTextualReasonsForFailure, bool* pbRequiresBetterTechnology, bool* bpShowInBuildList )
{
	bool bCanBuild = true;
	if( pbRequiresBetterTechnology ){ *pbRequiresBetterTechnology = false; }
	if( GetOwner() == 0 ) return false;

	// Do I have the technology to build it?
	//CString strNationName = GetCommonState()->GetPlayer( GetOwner() )->GetNationName();
	//long iPlayer = GetCommonState()->GetPlayer( GetOwner() )->GetPlayerNumber();
	long iPlayer = GetOwner();
	if( g_pEOSAIInterface->GetAIGameRules()->CanBuild( iPlayer, pBuildOption, bAllowOldTechnologies ) == false )
	{
		if( pbRequiresBetterTechnology != NULL ){ *pbRequiresBetterTechnology = true; }
		return false;
	}

	// If this is a landlocked city, and the build requires water, we can't build it
	//   Don't bother looking at the buildings that might be lacking
	if( pBuildOption->CanOnlyBeBuiltInCitiesAdjacentToWater() )
	{
		if( m_bIAmACoastalCity == false ) //GetWaterGeoNumber7() == 0 )
		{
			if( pTextualReasonsForFailure != NULL )
			{
				pTextualReasonsForFailure->AddTail( _T("Requires Water") );
			}
			return false;
		}
	}

	// If it fails because there is no coastline, don't bother looking up buildings - it doesnt matter
	if( pBuildOption->IsAUnit() )
	{
		CEOSAIUnitTemplate* pUnitTemplate = pBuildOption->GetAIUnitTemplate();
		POSITION pos = pUnitTemplate->GetCityMustHaveList()->GetHeadPosition();
		while( pos )
		{
			CString strReq = pUnitTemplate->GetCityMustHaveList()->GetNext( pos );
			if( this->GetBuildingCount( strReq ) == 0 )
			{
				if( pTextualReasonsForFailure != NULL )
				{
					CString strReason;
					strReason.Format( _T("Requires %s"), strReq );
					//strReason.Format( Lang( 428, _T("Requires %s") ), strReq );
					pTextualReasonsForFailure->AddTail( strReason );
				}
				bCanBuild = false;
			}
		}
	}
	if( pBuildOption->IsABuilding() )
	{
		CEOSAIBuildingDescription* pBuildingDesc = pBuildOption->GetBuildingDescription();

		// Do I already have the maximum number of this type of building in the city?
		if( pBuildingDesc->m_iUpgradeLevel > 0 && pBuildingDesc->m_strUpgradeLine != _T("") )
		{
			if( pBuildingDesc->m_iUpgradeLevel == 1 )
			{
				if( GetBuildingLineCount( pBuildingDesc->m_strUpgradeLine ) >= 
					pBuildingDesc->m_iMaximumNumberOfCopiesOfThisBuildingInACity )
				{
					bCanBuild = false;
				}
			}
			// If this is built, but the upgrade is not available, show it
			// If this is built, but the upgrade is available, then hide it
			// If this is a level 2, but a level 3 exists, then hide it
			long iCurrentlyBuiltLevel = this->GetBuildingLineCurrentUpgradeLevel( pBuildingDesc->m_strUpgradeLine );
			if( pBuildingDesc->m_iUpgradeLevel == iCurrentlyBuiltLevel )
			{
				// Can't build this because it's already built
				// Only show this if there isn't an upgrade available
				CEOSAIBuildingDescription* pUpgradedBuilding = g_pEOSAIInterface->GetAIGameRules()->GetAIBuildingDescriptionFromUpgradeLineAndLevel( pBuildingDesc->m_strUpgradeLine, pBuildingDesc->m_iUpgradeLevel+1 );
				if( pUpgradedBuilding )
				{
					if( g_pEOSAIInterface->GetAIGameRules()->CanBuildBuilding( iPlayer, pUpgradedBuilding->GetInternalName() ) )
					{
						if( bpShowInBuildList ){ *bpShowInBuildList = false; }
					}
				}
				bCanBuild = false;
				return bCanBuild;
			}
			else if( pBuildingDesc->m_iUpgradeLevel == iCurrentlyBuiltLevel+1 )
			{
				// This is the next upgrade (e.g. I have factory Level1, this is factory Level2)
				// Show this (as long as all the other tests pass okay)
			}
			else
			{
				// Never show this
				// Example: I have Factory Level1, This is Factory Level3,4,5,etc
				// Example: I have Factory Level4, This is Factory Level1,2
				if( bpShowInBuildList ){ *bpShowInBuildList = false; }
				bCanBuild = false;
				return bCanBuild;
			}
			// This building is an upgrade, make sure the immediate prerequisite exists
			CEOSAIBuildingDescription* pUpgradesFromBuildingDesc = g_pEOSAIInterface->GetAIGameRules()->GetAIBuildingDescriptionFromUpgradeLineAndLevel( pBuildingDesc->m_strUpgradeLine, pBuildingDesc->m_iUpgradeLevel-1 );
			if( pUpgradesFromBuildingDesc )
			{
				if( GetBuildingCount( pUpgradesFromBuildingDesc ) == 0 )
				{
					if( bpShowInBuildList ){ *bpShowInBuildList = false; }
					return false;
				}
			}
		}

		// Do I have enough population?
		if( GetPopulation() < pBuildingDesc->m_fMinimumPopulationRequired )
		{
			if( pTextualReasonsForFailure != NULL )
			{
				CString strReason;
				//strReason.Format( Lang( 429, _T("Requires %2.1f Population") ), pBuildingDesc->m_fMinimumPopulationRequired );
				strReason.Format( _T("Requires %2.1f Population"), pBuildingDesc->m_fMinimumPopulationRequired );
				pTextualReasonsForFailure->AddTail( strReason );
			}
			bCanBuild = false;
		}
		//if( GetBuildingCount( pBuildingDesc->GetInternalName() ) >=
		if( eGoal == eGoal_AddAnotherBuildItem )
		{
			if( GetBuildingCountIncludingBuildQueue( pBuildingDesc->GetInternalName() ) >=
				pBuildingDesc->m_iMaximumNumberOfCopiesOfThisBuildingInACity )
			{
				bCanBuild = false;
			}
		}
		if( eGoal == eGoal_BuildThis )
		{
			if( GetBuildingCount( pBuildingDesc->GetInternalName() ) >=
				pBuildingDesc->m_iMaximumNumberOfCopiesOfThisBuildingInACity )
			{
				bCanBuild = false;
			}
		}
		// Have I already reached the maximum number of copies for my nation?
		if( pBuildingDesc->m_iNumberAllowedInNation > -1 )
		{
			/*
			ASSERT( false );
			*/
			/*
			CWorldDesc* pWorldDesc = dynamic_cast< CWorldDesc* >( GetWorldDesc() );
			ASSERT( pWorldDesc );
			if( pWorldDesc )
			{
				long iExistingBuildings = pWorldDesc->GetNumberOfBuildingsOwnedByPlayer( GetOwner(), pBuildingDesc );
				long iInProductionBuildings = pWorldDesc->GetNumberOfBuildingsInProductionByPlayer( GetOwner(), pBuildingDesc );
				if( eGoal == eGoal_BuildThis &&
					GetBuildOrders()->IsEmpty() == FALSE )
				{
					CBuildOrder* pBuildOrder = GetBuildOrders()->GetHead();
					if( pBuildOrder->GetBuildOption()->IsABuilding() &&
						pBuildOrder->GetBuildOption()->GetBuildingDescription() == pBuildingDesc )
					{
						// Don't count this city's current production item towards the nations max
						//   Or else all buildings with a national maximum of 1 won't build
						iInProductionBuildings--;
					}
				}
				if( iExistingBuildings+iInProductionBuildings >= pBuildingDesc->m_iNumberAllowedInNation )
				{
					CString strReason;
					strReason.Format( Lang( 2466, _T("Only %d per nation") ), pBuildingDesc->m_iNumberAllowedInNation );
					if( pTextualReasonsForFailure )
					{
						pTextualReasonsForFailure->AddTail( strReason );
					}

					bCanBuild = false;
				}
			}
			*/
			//CWorldDesc* pWorldDesc = dynamic_cast< CWorldDesc* >( GetWorldDesc() );
			//ASSERT( pWorldDesc );
			//if( pWorldDesc )
			{
				long iExistingBuildings = g_pEOSAIInterface->GetAICommonData()->GetNumberOfBuildingsOwnedByPlayer( iPlayer, pBuildingDesc );
				long iInProductionBuildings = g_pEOSAIInterface->GetAICommonData()->GetNumberOfBuildingsInProductionByPlayer( iPlayer, pBuildingDesc );
				if( eGoal == eGoal_BuildThis &&
					m_AIBuildOrders.IsEmpty() == FALSE )
				{
					CEOSAIBuildOption* pAIBuildOrder = m_AIBuildOrders.GetHead();
					if( pAIBuildOrder->IsABuilding() &&
						pAIBuildOrder->GetBuildingDescription() == pBuildingDesc )
					{
						// Don't count this city's current production item towards the nations max
						//   Or else all buildings with a national maximum of 1 won't build
						iInProductionBuildings--;
					}
				}
				if( iExistingBuildings+iInProductionBuildings >= pBuildingDesc->m_iNumberAllowedInNation )
				{
					/*
					CString strReason;
					strReason.Format( Lang( 2466, _T("Only %d per nation") ), pBuildingDesc->m_iNumberAllowedInNation );
					if( pTextualReasonsForFailure )
					{
						pTextualReasonsForFailure->AddTail( strReason );
					}
					*/

					bCanBuild = false;
				}
			}

		}
	}
	return bCanBuild;
}

float CEOSAICity::GetTimeToBuild( CEOSAIUnitTemplate* pAIUnitTemplate, bool bUsePartiallyCompletedItems )
{
	float fProductionCost = pAIUnitTemplate->GetProductionCost();
	float fCityProduction = GetTotalProduction( true, pAIUnitTemplate );

	//if( pUnitTemplate->IsGroundUnit() ){ fCityProduction *= GetGroundUnitProductionXPopulation(); }
	//if( pUnitTemplate->IsSeaUnit() ){    fCityProduction *= GetSeaUnitProductionXPopulation(); }
	//if( pUnitTemplate->IsAirUnit() ){    fCityProduction *= GetAirUnitProductionXPopulation(); }

	if( bUsePartiallyCompletedItems )
	{
		CEOSAIBuildCompletion* pAIBuildCompletion = GetPartiallyCompletedItem( pAIUnitTemplate->GetInternalName() );
		if( pAIBuildCompletion )
		{
			fProductionCost -= pAIBuildCompletion->GetProductionInvested();
		}
	}
	ASSERT( fCityProduction > 0.0f );
	return fProductionCost / max( 0.1f, fCityProduction );
}

void  CEOSAICity::AppendBuildOrder(CEOSAIBuildOption* pEOSAIBuildOption)
{
	if (this->GetOwner() > 0)
	{
		EOSAI::ResourceAmounts  ResourceProduction = pEOSAIBuildOption->GetResourceProductionPerTurn(m_fProduction);
		EOSAI::ResourceAmounts  ResourceConsumption = pEOSAIBuildOption->GetResourceConsumptionPerTurn(m_fProduction);
		g_pEOSAICommonData->GetAINationalSummary3(GetOwner())->m_ResourceSummary.m_ResourceProductionByCurrentOrders += ResourceProduction;
		g_pEOSAICommonData->GetAINationalSummary3(GetOwner())->m_ResourceSummary.m_ResourceConsumptionByCurrentOrders += ResourceConsumption;
	}
	//m_pAIPlayer->AddBuildOrderToResourceProductionAndConsumption(pBestBuildOption);
	m_AIBuildOrders.AddTail(pEOSAIBuildOption);
}

void  CEOSAICity::RemoveAllItemsFromBuildQueue()
{
	while( m_AIBuildOrders.IsEmpty() == FALSE )
	{
		delete m_AIBuildOrders.RemoveHead();
	}
}

/*
bool CEOSAICity::ActionScheduler_TurnTasksIntoOrders()
{
	if( m_pCity->IsMine() && GetProductionTask() == NULL )
	{
		//ASSERT( false );
	}
	if( m_pCity->IsMine() == false && GetProductionTask() != NULL )
	{
		ASSERT( false );
	}

	if( m_pCity->IsMine() && GetProductionTask() )
	{
		m_pCity->AddToBuildQueue( GetProductionTask()->m_pBuildOption );
		/-*
		POSITION pos = m_ProductionList.GetHeadPosition();
		while( pos )
		{
			CAIProductionTask* pAIProductionTask = m_ProductionList.GetNext( pos );
			m_pCity->AddToBuildQueue( pAIProductionTask->m_pProductionOption->m_pBuildOption );

			//ASSERT( false );
			int x=0;
		}
		*-/
	}
	return true;
}
*/

//
// Region pathway
//
/*
void CEOSAICity::UpdateAIRegionPathway()
{
	m_AIRegionPathway.Clear();

	CEOSAIRegion2* pInitialRegion = GetAIBrain()->GetSpatialMap()->GetRegion( GetInitialLocation() );
	m_AIRegionPathway.Add( 0.0f, pInitialRegion );
}
*/

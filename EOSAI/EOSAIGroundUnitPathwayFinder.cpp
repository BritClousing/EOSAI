
#include "stdafx.h"
#include "EOSAIGroundUnitPathwayFinder.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAIUnitPathwayResult.h"
#include "EOSAIUnitPathwayResultStep.h"
#include "EOSAIUnit2.h"
#include "EOSAIMultiRegion2.h"

#include "EOSAIMultiRegionManager2.h"
#include "EOSAIRegionManager2.h"
#include "EOSAIRegionPathfinder.h"

//#include "WorldDescServer.h"
#include "EOSAIUnitTemplate.h"
#include "City.h"
#include "EOSAIAirfield.h"
#include "EOSAIBrain.h"
#include "EOSAIThoughtDatabase.h"
#include "EOSAIRoad2.h"
#include "EOSAICommonData2.h"
class CEOSAIMultiRegion2;
class CEOSAIRegion2;

extern CList< long >  g_HighlightAIRegions;
CEOSAIRegionPathwayMap  g_ViewBuilderDrawMap;
CGroundUnitAndTransportPathway* g_pGroundUnitAndTransportPathway = NULL;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//CAISeaUnitPathway::CAISeaUnitPathway( CEOSAIBrain* pAIBrain, long iPlayer, CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate )
CAIGroundUnitPathwayFinder::CAIGroundUnitPathwayFinder( CEOSAIUnit2* pAIUnitActor )
	: CEOSAIUnit2PathwayFinder( pAIUnitActor )
{
	ASSERT( pAIUnitActor->GetAIUnitTemplate()->IsGroundUnit() );
	if( pAIUnitActor->GetAIUnitTemplate()->IsGroundUnit() == false ) return;
}

//CAISeaUnitPathway::CAISeaUnitPathway( CEOSAIBrain* pAIBrain, long iPlayer, CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate )
CAIGroundUnitPathwayFinder::CAIGroundUnitPathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime )
	: CEOSAIUnit2PathwayFinder( pAICityActor, pAIUnitTemplate, fBuildTime )
{
	ASSERT( pAIUnitTemplate->IsGroundUnit() );
	if( pAIUnitTemplate->IsGroundUnit() == false ) return;
}

CGroundUnitAndTransportPathway::~CGroundUnitAndTransportPathway()
{
	int g=0;
}
/*
CEOSAIRegionPathwayMap* CAIGroundUnitPathwayFinder::InvokeDirectAIRegionMapToEverywhere()
{
	ASSERT( false ); // depreciate this?
	if( m_AIRegionMapToEverywhere.GetArraySize() == 0 )
	{
		CEOSAIRegionPathfinder& RegionPathfinder = m_pAIBrain->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( m_iUnitOwner );
		RegionPathfinder.SetMovementRate( m_pUnitTemplate );
		RegionPathfinder.SetDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetGeoSwitchMultiplier( 300.0f );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iUnitOwner );
		RegionPathfinder.AddStartLocation( m_StartLocation );
		//RegionPathfinder.AddEndLocation( pPoint->m_Location );//pMultiRegion );
		//RegionPathfinder.Solve2( iOwner, true );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_AIRegionMapToEverywhere );

		// Enlarge the Pathway map - find coastal locations
	}
	//
	return &m_AIRegionMapToEverywhere;
}
*/
/*
CEOSAIRegionPathwayMap* CAIGroundUnitPathwayFinder::InvokeDirectAIRegionMapToCoasts()
{
	ASSERT( m_pUnitTemplate->IsGroundUnit() );
	if( m_pUnitTemplate->IsGroundUnit() == false ){ return NULL; }

	InvokeDirectAIRegionMapToEverywhere();
	if( m_AIRegionMapToCoasts.GetArraySize() == 0 )
	{
		float fMovementRate = m_pUnitTemplate->GetMovementRate();
		CEOSAIRegionPathfinder::AddBorderRegions( &m_AIRegionMapToEverywhere, &m_AIRegionMapToCoasts, fMovementRate );
	}
	return &m_AIRegionMapToCoasts;
}
*/
/*
CEOSAIRegionPathwayMap* CAIGroundUnitPathwayFinder::InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere()
{
	ASSERT( m_pUnitTemplate->IsGroundUnit() );
	if( m_pUnitTemplate->IsGroundUnit() == false ){ return NULL; }
	if( m_LandAndImaginarySeaTransportAIRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = m_iUnitOwner;//this->GetOwner();
		CEOSAILocation StartLocation = m_StartLocation; //GetPoiObject()->GetLocation();

		CEOSAIUnitTemplateSet* pUnitTemplateSet = m_pAIBrain->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
		float fFastestSeaTransport = pUnitTemplateSet->GetFastestSeaTransportMovementRate( m_pUnitTemplate );

		CTerrainBasedValue MovementDesc;
		MovementDesc.Set( m_pUnitTemplate );
		MovementDesc.m_fTerrainValue[TERRAIN_COAST] = fFastestSeaTransport;
		MovementDesc.m_fTerrainValue[TERRAIN_WATER] = fFastestSeaTransport;

		CEOSAIRegionPathfinder& RegionPathfinder = m_pAIBrain->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		//RegionPathfinder.SetLandDistanceMultiplier( GetUnitTemplate() );
		RegionPathfinder.SetMovementRate( &MovementDesc );
		//RegionPathfinder.SetSeaDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandAndImaginarySeaTransportAIRegionMapToEverywhere );
	}
	return &m_LandAndImaginarySeaTransportAIRegionMapToEverywhere;
}
*/
// FinalPath = Transport-Supported Pathway
//

void  CAIGroundUnitPathwayFinder::PreprocessPath()
{
	long iNumberOfTransportsRequired = 0;
	CEOSAILocation CurrentLocation = m_StartLocation;

	//CEOSAIRegionManager2* pAIRegionManager = m_pAIBrain->GetAIRegionManager();
	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
	CEOSAIRegionPathfinder& RegionPathfinder = pAIRegionManager->GetPathfinder();
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );

		if( pPredefinedStep->GetStartLocation() != CurrentLocation )
		{
			pPredefinedStep->SetStartLocation( CurrentLocation );
		}

		CurrentLocation = pPredefinedStep->GetEndLocation();
	}
}

void  CAIGroundUnitPathwayFinder::CalculateAIRegionPathFromPredefinedSteps()
{
	PreprocessPath();
	CEOSAILocation CurrentLocation = m_StartLocation;

	//CEOSAIRegionManager2* pAIRegionManager = m_pAIBrain->GetAIRegionManager();
	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
	CEOSAIRegionPathfinder& RegionPathfinder = pAIRegionManager->GetPathfinder();

	// Look at the InitialPath - compare the LandPath vs LandAndTransportPath
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->ClearDirectMovementPath();
		pPredefinedStep->GetAIRegionAllowedMovementPath()->RemoveAll();

		// If this step requires a transport, setup a fake path (that will get fixed later)
		// If this step doesn't require a transport, setup a real path

		if( pPredefinedStep->RequiresTransport() )
		{
			/*
			// Setup a fake path?  Or just setup some estimated data which will get overwritten by the transport
			EOSAI::UnitPathwayResultStep* pResultStep = new EOSAI::UnitPathwayResultStep( m_pResultPath );
			m_pResultPath->m_Steps.AddTail( pResultStep );

			pResultStep->SetPredefinedStep( pPredefinedStep );
			pResultStep->m_bTransportNeeded = true;

			// Fake data (will get replaced by the transport)
			pResultStep->m_fDistance = pPredefinedStep->GetCrowsFlightDistance();
			pResultStep->m_fStepTime = pPredefinedStep->GetCrowsFlightDistance() / ( m_pUnitTemplate->GetMovementRate() + 0.1f );
			*/
			// Update the pathway with a RealTransport route - if one has been set
			if( pPredefinedStep->GetTransportToUse() )
			{
				CEOSAIUnit2* pAIUnitTransport = pPredefinedStep->GetTransportToUse();

				// Create Pickup/Dropoff Predefined Steps for the Transport
				pPredefinedStep->UseTransportAndCreatePickupDropoffSteps( pAIUnitTransport );
				//pResultStep->UpdateWithTransportAssistedPath( pPredefinedStep->GetTransportToUse() );
				// Update Pickup/Dropoff
				pAIUnitTransport->GetAIUnitPathwayFinder()->UpdateTransportPickupDropoffPath( pPredefinedStep );
			}

			// The Transport is going to need some path information -
			//   If the Transport is a sea-transport, then we need:
			//     TargetLocation -> CoastalPickupLocations (includes Pickup->Dropoff, Dropoff->Target)
			//   If the Transport is an air-transport, then we need:
			//     TargetLocation -> PickupLocations (Pickup->Dropoff, Dropoff->Target)
			// I won't bother pre-calculating them - it can be done just as easily by the transport,
			//   And the transport knows what kind of data it needs anyway (i.e. SeaTransport or AirTransport)
			// Copy the pathway into the AIRegionPath
			/*
			CEOSAIRegionPathwayItem* pCurrentItem = pEndLocationToEverywhereDirectMap->GetRegionResult( pPredefinedStep->GetStartLocation() );
			while( pCurrentItem )
			{
				pPredefinedStep->GetActualMovementPath()->AddTail( pCurrentItem->m_pPathfinderPoint );
			}
			*/
		}
		else
		{
			CEOSAIRegionPathwayMap* pEndLocationToEverywhereDirectMap = 
				pPredefinedStep->InvokeEndLocationToEverywhereDirectMap();

			// Copy the pathway into the AIRegionPath
			CEOSAIRegionPathwayItem* pCurrentItem = pEndLocationToEverywhereDirectMap->GetRegionResult( pPredefinedStep->GetStartLocation() );
			while( pCurrentItem )
			{
				//pPredefinedStep->GetDirectMovementPath()->AddTail( pCurrentItem->m_pPathfinderPoint );
				pPredefinedStep->AddToDirectMovementPathTail( pCurrentItem->m_pPathfinderPoint );

				CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentItem->m_pPathfinderPoint );
				if( pAIRegion )
				{
					pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRegion );
				}
				CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pCurrentItem->m_pPathfinderPoint );
				if( pAIRoad )
				{
					pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRoad->GetAIRegion1() );
					pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRoad->GetAIRegion2() );
				}
				pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
			}
		}
	}
}
/*
void  CAIGroundUnitPathwayFinder::CalculateAIRegionPathTiming()
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->UpdatePathTimingIfNecessary();
	}
}
*/

//float CAIGroundUnitPathwayFinder::GetTransporteeArrivalTimeAtPickupLocation( CEOSAIUnit2PathwayPredefinedStep* pTransportPickupStep )
/*
float CAIGroundUnitPathwayFinder::GetTransporteeArrivalTimeAtPickupLocation( CEOSAIUnit2PathwayPredefinedStep* pTransportPickupStep )
{
	ASSERT( pTransportPickupStep->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsTransport() );
	CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep = pTransportPickupStep->GetTransporteeStep();

	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		if( pStep == pTransporteeStep )
		{
			ASSERT( pStep->GetTransportToUse() );
			return pStep->GetRedMovementPath_ArrivalTimeAtPickupLocation();
		}
	}
	ASSERT( false );
	return 0.0f;
}
*/
void  CAIGroundUnitPathwayFinder::CalculateResultPath()
{
	PreprocessPath();

	//m_bPathIsValid = true; // INCOMPLETE: not all paths will be valid (a double-land circle can make this invalid)
	CEOSAILocation CurrentLocation = m_StartLocation;

	// Clear existing Results
	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }
	m_pResultPath = new EOSAI::UnitPathwayResult( this );

	//CEOSAIRegionManager2* pAIRegionManager = m_pAIBrain->GetAIRegionManager();
	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
	CEOSAIRegionPathfinder& RegionPathfinder = pAIRegionManager->GetPathfinder();
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	// Look at the InitialPath - compare the LandPath vs LandAndTransportPath
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		//pPredefinedStep->GetActualMovementPath()->RemoveAll();
		//pPredefinedStep->GetAIRegionAllowedMovementPath()->RemoveAll();

		// If this step requires a transport, setup a fake path (that will get fixed later)
		// If this step doesn't require a transport, setup a real path

		if( pPredefinedStep->RequiresTransport() )
		{
			// Setup a fake path?  Or just setup some estimated data which will get overwritten by the transport
			EOSAI::UnitPathwayResultStep* pResultStep = new EOSAI::UnitPathwayResultStep( m_pResultPath );
			m_pResultPath->m_Steps.AddTail( pResultStep );

			pResultStep->SetPredefinedStep( pPredefinedStep );
			pResultStep->m_bTransportNeeded = true;
			//pResultStep->m_StartLocation = pPredefinedStep->GetStartLocation();
			//pResultStep->m_EndLocation = pPredefinedStep->GetEndLocation();

			// Fake data (will get replaced by the transport)
			pResultStep->m_fDistance = pPredefinedStep->GetCrowsFlightDistance();
			pResultStep->m_fStepTime = pPredefinedStep->GetCrowsFlightDistance() / ( m_pAIUnitTemplate->GetMovementRate() + 0.1f );

			// Update the pathway with a RealTransport route - if one has been set
			if( pPredefinedStep->GetTransportToUse() )
			{
				CEOSAIUnit2* pAITransportToUse = pPredefinedStep->GetTransportToUse();
				//m_pResultPath->UseTransport( pPredefinedStep->GetTransportToUse() );
				pPredefinedStep->UseTransportAndCreatePickupDropoffSteps( pAITransportToUse );
				//pResultStep->UpdateWithTransportAssistedPath( pPredefinedStep->GetTransportToUse() );
				// Update Pickup/Dropoff
				pAITransportToUse->GetAIUnitPathwayFinder()->UpdateTransportPickupDropoffPath( pPredefinedStep );
				//if( pPredefinedStep->GetPathwayResult() )
				//{
				//	pPredefinedStep->GetPathwayResult()->UpdateWithTransportAssistedPath( pPredefinedStep->GetTransportToUse() );
					//pPredefinedStep->GetPathwayResult()->UpdateStepWithTransportAssistedPath( pPredefinedStep->GetTransportToUse() );
				//}
			}

			// The Transport is going to need some path information -
			//   If the Transport is a sea-transport, then we need:
			//     TargetLocation -> CoastalPickupLocations (includes Pickup->Dropoff, Dropoff->Target)
			//   If the Transport is an air-transport, then we need:
			//     TargetLocation -> PickupLocations (Pickup->Dropoff, Dropoff->Target)
			// I won't bother pre-calculating them - it can be done just as easily by the transport,
			//   And the transport knows what kind of data it needs anyway (i.e. SeaTransport or AirTransport)
		}
		else
		{
			CEOSAIRegionPathwayMap* pEndLocationToEverywhereDirectMap = 
				pPredefinedStep->InvokeEndLocationToEverywhereDirectMap();

			// Copy the pathway
			EOSAI::UnitPathwayResultStep* pResultStep = new EOSAI::UnitPathwayResultStep( m_pResultPath );
			pResultStep->SetPredefinedStep( pPredefinedStep );
			m_pResultPath->m_Steps.AddTail( pResultStep );

			// Copy the pathway into the AIRegionPath
			long iCurrentItem = 1;
			CEOSAIRegionPathwayItem* pCurrentItem = pEndLocationToEverywhereDirectMap->GetRegionResult( pPredefinedStep->GetStartLocation() );
			while( pCurrentItem )
			{
				if( iCurrentItem == 1 )
				{
					pResultStep->m_bIsValid = true;
					pResultStep->m_fDistance = pCurrentItem->m_fDistance;
					pResultStep->m_fDanger = pCurrentItem->m_fDanger;
					pResultStep->m_fSighting = pCurrentItem->m_fSightings;
					pResultStep->m_fStepTime = pCurrentItem->m_fTime;
					pResultStep->m_bTransportNeeded = false;
				}
				iCurrentItem++;

				/*
				pPredefinedStep->GetActualMovementPath()->AddHead( pCurrentItem->m_pPathfinderPoint );

				CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentItem->m_pPathfinderPoint );
				if( pAIRegion )
				{
					pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRegion );
				}
				CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pCurrentItem->m_pPathfinderPoint );
				if( pAIRoad )
				{
					pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRoad->GetAIRegion1() );
					pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRoad->GetAIRegion2() );
				}
				*/
				pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
			}
		}
	}
	m_pResultPath->m_bPathIsValid = true;
}

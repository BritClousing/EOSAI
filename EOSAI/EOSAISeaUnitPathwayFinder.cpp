
#include "stdafx.h"
#include "EOSAISeaUnitPathwayFinder.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAIUnitPathwayResultStep.h"
#include "EOSAIUnit2.h"
//#include "PoiObject.h"
#include "EOSAIResource.h"
//#include "ResourceSource.h"
#include "City.h"
#include "EOSAIAirfield.h"
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIRegionManager2.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTemplate.h"
//#include "WorldDescServer.h"
#include "EOSAIGroundUnitPathwayFinder.h"
#include "EOSAIUnitPathwayResult.h"
#include "EOSAICommonData.h"
#include "EOSAIRoad2.h"
//#include "GeoLand.h"
class CEOSAIMultiRegion2;
class CEOSAIRegion2;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CList< long >  g_HighlightAIRegions;

//

//CEOSAISeaUnitPathwayFinder::CEOSAISeaUnitPathwayFinder( CEOSAIBrain* pAIBrain, long iPlayer, CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate )
CEOSAISeaUnitPathwayFinder::CEOSAISeaUnitPathwayFinder( CEOSAIUnit* pAIUnitActor )
	: CEOSAIUnitPathwayFinder( pAIUnitActor )
{
	ASSERT( pAIUnitActor->GetAIUnitTemplate()->IsSeaUnit() );
	if( pAIUnitActor->GetAIUnitTemplate()->IsSeaUnit() == false ) return;
/*
	m_iUnitOwner = pAIUnitActor->GetOwner();
	m_pActor = pAIUnitActor;
	m_pUnitTemplate = pAIUnitActor->GetUnitTemplate();
	m_StartLocation = pAIUnitActor->GetPoiObject()->GetLocation();
*/
	//SetUnit( iPlayer, pActor, pUnitTemplate );
	//pSeaUnitPathway->m_pAIBrain = pActor->GetAIBrain();
	//pSeaUnitPathway->m_iUnitOwner = pActor->GetOwner();
	//pSeaUnitPathway->m_pActor = pActor;
	//pSeaUnitPathway->m_pUnitTemplate = pUnitTemplate;
	//m_StartLocation = pActor->GetPoiObject()->GetLocation();
}

//CEOSAISeaUnitPathwayFinder::CEOSAISeaUnitPathwayFinder( CEOSAIBrain* pAIBrain, long iPlayer, CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate )
CEOSAISeaUnitPathwayFinder::CEOSAISeaUnitPathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime )
	: CEOSAIUnitPathwayFinder( pAICityActor, pAIUnitTemplate, fBuildTime )
{
	ASSERT( pAIUnitTemplate->IsSeaUnit() );
	if( pAIUnitTemplate->IsSeaUnit() == false ) return;
/*
	m_iUnitOwner = pAICityActor->GetOwner();
	m_pActor = pAICityActor;
	m_pUnitTemplate = pUnitTemplate;
	m_StartLocation = pAICityActor->GetPoiObject()->GetLocation();
	m_fStartTime = fBuildTime;
*/
	//SetUnit( iPlayer, pActor, pUnitTemplate );
	//pSeaUnitPathway->m_pAIBrain = pActor->GetAIBrain();
	//pSeaUnitPathway->m_iUnitOwner = pActor->GetOwner();
	//pSeaUnitPathway->m_pActor = pActor;
	//pSeaUnitPathway->m_pUnitTemplate = pUnitTemplate;
	//m_StartLocation = pActor->GetPoiObject()->GetLocation();
}

CEOSAISeaUnitPathwayFinder::~CEOSAISeaUnitPathwayFinder()
{
	int g=0;
}
/*
CEOSAIRegionPathwayMap* CEOSAISeaUnitPathwayFinder::InvokeDirectAIRegionMapToEverywhere()
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
		//RegionPathfinder.AddEndLocation( pPoint->m_Location );
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
void  CEOSAISeaUnitPathwayFinder::CalculateTransportPath( 
		CEOSAIUnit* pTransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeTargetLocation,
		EOSAI::UnitPathwayResult* pAIUnitPathwayResult )
{
	pAIUnitPathwayResult->Clear();

	ASSERT( m_pUnitTemplate->IsTransport() );
	if( m_pUnitTemplate->IsTransport() == false ) return;
	ASSERT( pTransportee->IsGroundUnit() );
	if( pTransportee->IsGroundUnit() == false ) return;
	//
	// If the [target] is on water (like a ship or sea-resource), then I don't need a transport, do I?
	CEOSAIRegion2* pTargetAIRegion = m_pAIBrain->GetAIRegionManager()->GetAIRegion( TransporteeTargetLocation );
	ASSERT( pTargetAIRegion->IsLand() || pTargetAIRegion->IsCity() );
	//
	CAIGroundUnitPathwayFinder* pTransporteePathFinder = pTransportee->GetAIGroundUnitPathwayFinder();
	CEOSAIRegionPathwayMap* pGroundUnitToPickupMap = pTransporteePathFinder->InvokeDirectAIRegionMapToCoasts();
	CEOSAIRegionPathwayMap* pTransportToEverywhereMap = InvokeDirectAIRegionMapToEverywhere();

	/-*
	I should use this block of code later - it caches the path data

	// Now I need the Target-to-Pickup map (input Transport/GroundUnit Movement values into the mix)
	CMovementDescription TypicalGroundUnitMovementDesc;
	m_pAIBrain->GetAIThoughtDatabase()->GetTypicalGroundUnitMovementDescription( &TypicalGroundUnitMovementDesc );
	CMovementDescription TypicalSeaTransportMovementDesc;
	m_pAIBrain->GetAIThoughtDatabase()->GetTypicalSeaTransportMovementDescription( &TypicalSeaTransportMovementDesc );

	CTerrainBasedValue TerrainBasedMovementDesc;
	for( long iTerrain=0; iTerrain<TERRAIN_COUNT; iTerrain++ )
	{
		TerrainBasedMovementDesc.m_fTerrainValue[iTerrain]  = TypicalGroundUnitMovementDesc.GetMovementRate( iTerrain );
	}
	TerrainBasedMovementDesc.m_fTerrainValue[TERRAIN_COAST] = TypicalSeaTransportMovementDesc.GetMovementRate();
	TerrainBasedMovementDesc.m_fTerrainValue[TERRAIN_WATER] = TypicalSeaTransportMovementDesc.GetMovementRate();

	CEOSAIRegionPathwayMap* pTargetToEverywhereMap =
		m_pAIBrain->GetAIRegionManager()->InvokePathwayToEverywhereTypicalGroundUnitAndSeaTransport( 
		//m_pAIBrain->GetAIRegionManager()->InvokePathwayToEverywhere( 
			CEOSAIRegionManager2::EnumFrontalAssault, pTargetAIRegion, TerrainBasedMovementDesc );
	*-/
	//CEOSAIRegionPathwayMap* pTargetToEverywhereMap =
	//	InvokePathwayToEverywhereWithTypicalGroundUnit( pTargetAIRegion );

	CTerrainBasedValue MovementDesc;
	MovementDesc.Set( pTransportee->GetUnitTemplate() ); // GroundUnit Movement Rates
	MovementDesc.m_fTerrainValue[TERRAIN_COAST] = m_pUnitTemplate->GetMovementRate(); // Transport
	MovementDesc.m_fTerrainValue[TERRAIN_WATER] = m_pUnitTemplate->GetMovementRate(); // Transport

	CEOSAIRegionPathwayMap  TargetToEverywhereMap;
	CEOSAIRegionPathfinder& RegionPathfinder = m_pAIBrain->GetAIRegionManager()->GetPathfinder();
	RegionPathfinder.ClearPathfindingSettings();
	RegionPathfinder.SetPlayer( m_iUnitOwner );
	RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
	RegionPathfinder.SetMaximumGeoSwitches( 1 );
	RegionPathfinder.SetMovementRate( &MovementDesc );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iUnitOwner );
	RegionPathfinder.AddStartLocation( TransporteeTargetLocation );
	RegionPathfinder.Solve3();
	RegionPathfinder.GetResult( true, &TargetToEverywhereMap );

	// Now, Read-over the Maps - figure out the best pickup/dropoff locations

	// Now iterate over the coastal locations, find the results
	float fTransportMovementRate = m_pUnitTemplate->GetMovementRate();
	float fBestTime = 1000000.0f;
	long  iBestPickupRegion = 0;
	//CEOSAIRegionPathwayMap* pGroundUnitToCoastsMap = InvokeDirectAIRegionMapToCoasts();
	//long iNumberOfRegions = pGroundUnitToCoastsMap->GetRegionManager()->GetNumberOfGenericRegions();
	long iNumberOfRegions = pGroundUnitToPickupMap->GetRegionManager()->GetNumberOfGenericRegions();
	for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
	{
		CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
		CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToEverywhereMap->GetRegionResult( iRegion );
		CEOSAIRegionPathwayItem* pTargetToPickupRegion = TargetToEverywhereMap.GetRegionResult( iRegion );
		if( pGroundUnitToPickupRegion && pTransportToPickupRegion && pTargetToPickupRegion )
		{
			float fGroundUnitToPickupTime = pGroundUnitToPickupRegion->m_fTime;
			float fTransportToPickupTime = m_fStartTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
			float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
			float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
			if( fGroundUnitToPickupTime < 0.6f && fTransportToPickupTime < 0.5f )
			{
				// If GroundUnit and Transport are both close to a pickup location, use it
				// tweak the time to favor this option
				fTotalTime -= 1.0f; 
			}
			if( fTotalTime < fBestTime )
			{
				fBestTime = fTotalTime;
				iBestPickupRegion = iRegion;
			}
			int g=0;
		}
	}
	//CEOSAITransportAssistedPath* pPathResults = NULL;
	//if( pPathResults && iBestPickupRegion > 0 )
	if( pAIUnitPathwayResult && iBestPickupRegion > 0 )
	{
		CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iBestPickupRegion );
		CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToEverywhereMap->GetRegionResult( iBestPickupRegion );
		CEOSAIRegionPathwayItem* pTargetToPickupRegion = TargetToEverywhereMap.GetRegionResult( iBestPickupRegion );

		//
		CList< long >  GroundUnitToPickupPath;
		CList< long >  TransportToPickupPath;
		CList< long >  GroundUnitFromDropoffToTargetPath;
		CList< long >  TransportPickupToDropoffPath;
		long iLandPickupRegion = 0;
		long iWaterPickupRegion = 0;
		long iLandDropoffRegion = 0;
		long iWaterDropoffRegion = 0;
		//long iStep = 0;
		CEOSAIRegionPathwayItem* pCurrentPathwayItem = pGroundUnitToPickupRegion;
		while( pCurrentPathwayItem )
		{
			//iStep++;
			if( iWaterPickupRegion == 0 )//iStep == 1 )
			{
				iWaterPickupRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
			}
			if( iLandPickupRegion == 0 )
			{
				if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
					pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					iLandPickupRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
			}
			GroundUnitToPickupPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pGroundUnitToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}
		pCurrentPathwayItem = pTransportToPickupRegion;
		while( pCurrentPathwayItem )
		{
			TransportToPickupPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTransportToEverywhereMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}

		// Drop the pathway information into a path and then walk it backwards
		CList< long > Path;
		pCurrentPathwayItem = pTargetToPickupRegion;
		while( pCurrentPathwayItem )
		{
			Path.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = TargetToEverywhereMap.GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}
		bool bMovementOnLand = true;
		POSITION pos = Path.GetHeadPosition();
		while( pos )
		{
			long iRegion = Path.GetNext( pos );
			CEOSAIRegionPathwayItem* pPathItem = TargetToEverywhereMap.GetRegionResult( iRegion );

			if( pPathItem->m_pPathfinderPoint->IsWater() ){ bMovementOnLand = false; }
			if( bMovementOnLand )
			{
				iLandDropoffRegion = pPathItem->m_pPathfinderPoint->GetId();
				ASSERT( pPathItem->m_pPathfinderPoint->IsLand() || pPathItem->m_pPathfinderPoint->IsCity() );
				//pPathResults->m_GroundUnitFromDropoffToTargetPath.AddTail( iRegion );
				GroundUnitFromDropoffToTargetPath.AddTail( iRegion );
			}
			else
			{
				if( iWaterDropoffRegion == 0 )
				{
					iWaterDropoffRegion = pPathItem->m_pPathfinderPoint->GetId();
				}
				ASSERT( pPathItem->m_pPathfinderPoint->IsWater() || pPathItem->m_pPathfinderPoint->IsCity() );
				//pPathResults->m_TransportPickupToDropoffPath.AddTail( iRegion );
				TransportPickupToDropoffPath.AddTail( iRegion );
			}
		}
		int g=0;
	}
}
*/
/*
void  CEOSAISeaUnitPathwayFinder::CalculateInitialEvaluation()
{
	ASSERT( m_bInitialEvaluationHasBeenDone == false );
	m_bInitialEvaluationHasBeenDone = true;

	POSITION pos;

	// Do two calculations: (in both cases, ignore all enemy/neutral units)
	//   One thats ground-path only
	//   Another that uses a Movement amalgamation (best movement rates for ground unit + transport)

	CEOSAILocation CurrentLocation = m_StartLocation;
	//m_SimpleRoute.Clear();
	pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );

		//bool bStepIsValid = false;
		//bool bCalculateThisStep = false;
		//if( pPredefinedStep->m_EndLocation != CurrentLocation ){ bCalculateThisStep = true; } // Start Location changed

		//if( pPathStep->m_Calculated_MultiRegionPath.IsEmpty() ){ bCalculateThisStep = true; }
		//if( pPathStep->m_Calculated_AIRegionPath.IsEmpty() ){ bCalculateThisStep = true; }
		//if( bCalculateThisStep )
		//{
			//pPathStep->m_fDistance = 0.0f;
			//pPathStep->m_Calculated_AIRegionPath.RemoveAll();

			//
			//CEOSAIMultiRegionManager2* pMultiRegionManager = m_pAIBrain->GetMultiRegionManager();
			CEOSAIRegionManager2* pAIRegionManager = m_pAIBrain->GetAIRegionManager();
			CEOSAIRegionPathwayMap  PathwayResult;
			CEOSAIRegionPathfinder& RegionPathfinder = pAIRegionManager->GetPathfinder();
			RegionPathfinder.ClearPathfindingSettings();
			RegionPathfinder.SetPlayer( m_iUnitOwner );
			RegionPathfinder.SetForeignRelations( &m_OwnerForeignRelations );
			RegionPathfinder.SetMovementRate( m_pUnitTemplate );
			//RegionPathfinder.SetDistanceMultiplier( 1.0f );
			//RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
			//RegionPathfinder.SetGeoSwitchMultiplier( 300.0f );
			RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iUnitOwner );
			RegionPathfinder.AddStartLocation( CurrentLocation );//pMultiRegion );
			RegionPathfinder.AddEndLocation( pPredefinedStep->m_EndLocation );//pMultiRegion );
			RegionPathfinder.Solve3();
			RegionPathfinder.GetResult( true, &PathwayResult );

			// Copy the pathway into the MultiRegionList
			EOSAI::UnitPathwayResultStep* pResultStep = new EOSAI::UnitPathwayResultStep();
			pResultStep->m_ePathType = enum_FinalPath;enum_InitialEvaluation_WaterPath;
			pResultStep->m_pPredefinedStep = pPredefinedStep;
			pResultStep->m_pAIActor = GetAIPoiObjectActor();//m_pActor;
			pResultStep->m_pUnitTemplate = m_pUnitTemplate;
			pResultStep->m_StartLocation = CurrentLocation;
			pResultStep->m_EndLocation = pPredefinedStep->m_EndLocation;

			long iCurrentItem = 1;
			CEOSAIRegionPathwayItem* pCurrentItem = PathwayResult.GetRegionResult( pPredefinedStep->m_EndLocation );
			while( pCurrentItem )
			{
				if( iCurrentItem == 1 )
				{
					pResultStep->m_bIsValid = true;
					pResultStep->m_fDistance = pCurrentItem->m_fDistance;
					pResultStep->m_fDanger = pCurrentItem->m_fDanger;
					pResultStep->m_fSighting = pCurrentItem->m_fSightings;
					pResultStep->m_fStepTime = pCurrentItem->m_fTime; //pCurrentItem->m_fDistance / fBestMovementRate;
					pResultStep->m_iTransportsNeeded = (pCurrentItem->m_iGeoSwitches > 0) ? 1 : 0;
				}
				iCurrentItem++;

				CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( pCurrentItem->m_pPathfinderPoint->m_iId );
				//pPathStep->m_Calculated_AIRegionPath.AddHead( pAIRegion );
				pResultStep->m_AIRegionPath.AddTail( pAIRegion );
				pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
			}

			pPredefinedStep->AddResult( pResultStep );
			//m_SimpleRoute.AddResultStep( pResultStep );

			//if( iCurrentItem == 1 ){ bStepIsValid = false; }
			//if( iCurrentItem >  1 ){ bStepIsValid = true; }
		//}

		CurrentLocation = pPredefinedStep->m_EndLocation;//>GetStepFinalLocation();
		/*
		if( bStepIsValid )
		{
			m_bPathIsValid = true;
		}
		else
		{
			m_bPathIsValid = false;
			break;
		}
		*-/
	}
}
*/

/*
void  CEOSAISeaUnitPathwayFinder::RecalculateCurrentPathway()
{
	ASSERT( m_bInitialEvaluationHasBeenDone );
	//m_bPathIsValid = false;

	// iLevel 1 = Calculate MultiRegion pathway
	// iLevel 2 = Calculate AIRegion pathway
	long iRegionType = 1;  // 1 = MultiRegion, 2 = AIRegion
	if( m_iCurrentPathfindingLevel == 1 ){ iRegionType = 1; }
	if( m_iCurrentPathfindingLevel >  1 ){ iRegionType = 2; }

	CEOSAILocation CurrentLocation = m_StartLocation;

	CEOSAIUnitTemplateSet* pBuildOrHaveUnits = m_pAIBrain->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
	float fTransportMovementRate = pBuildOrHaveUnits->GetFastestTransportMovementRate( m_pUnitTemplate );
	float fBestMovementRate = max( fTransportMovementRate, m_pUnitTemplate->GetMovementRate() );
	if( fBestMovementRate <= 0.0f ){ return; }

	//m_fPathwayDanger = 0.0f;
	//m_fPathwayEnemySighting = 0.0f;

	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		//CAISeaPathPoint* pPoint = m_PreDefinedPath.GetNext( pos );
		//CAISeaPathPoint* pPathStep = m_PreDefinedPath.GetNext( pos );
		CEOSAIUnitPathwayPredefinedStep* pPathStep = m_PreDefinedPath.GetNext( pos );

		bool bStepIsValid = false;
		bool bCalculateThisStep = false;
		//if( pPathStep->m_TargetLocation != CurrentLocation ){ bCalculateThisStep = true; } // Start Location changed

		if( iRegionType == 1 )
		{
			//if( pPathStep->m_Calculated_MultiRegionPath.IsEmpty() ){ bCalculateThisStep = true; }
			//if( bCalculateThisStep )
			{
				//pPathStep->m_fDistance = 0.0f;
				//pPathStep->m_Calculated_MultiRegionPath.RemoveAll();

				//
				//CEOSAIMultiRegionManager2* pMultiRegionManager = m_pAIBrain->GetMultiRegionManager();
				//CEOSAIRegionPathfinder& RegionPathfinder = pMultiRegionManager->GetPathfinder();
				CEOSAIRegionManager2* pAIRegionManager = m_pAIBrain->GetAIRegionManager();
				CEOSAIRegionPathfinder& RegionPathfinder = pAIRegionManager->GetPathfinder();

				// Setup the Real multipliers
				//RegionPathfinder.SetAllyNeutralEnemyDangerMultiplier(

				CEOSAIRegionPathwayMap  PathwayResult;
				RegionPathfinder.ClearPathfindingSettings();
				RegionPathfinder.SetDistanceMultiplier( 1.0f );
				//RegionPathfinder.SetGeoSwitchMultiplier( 400.0f );
				RegionPathfinder.SetMovementRate( m_pUnitTemplate );
				RegionPathfinder.SetMaximumGeoSwitches( 0 );
				RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iUnitOwner );
				RegionPathfinder.AddStartLocation( CurrentLocation );//pMultiRegion );
				RegionPathfinder.AddEndLocation( pPathStep->m_EndLocation );//pMultiRegion );
				RegionPathfinder.Solve2( m_iUnitOwner, false );
				RegionPathfinder.GetResult( true, &PathwayResult );

				// Copy the pathway into the MultiRegionList
				EOSAI::UnitPathwayResultStep* pResultStep = new EOSAI::UnitPathwayResultStep();
				pResultStep->m_pPredefinedStep = NULL;//pPathStep;
				pResultStep->m_pAIActor = m_pActor;
				pResultStep->m_pUnitTemplate = m_pUnitTemplate;
				pResultStep->m_StartLocation = CurrentLocation;
				pResultStep->m_EndLocation = pPathStep->m_EndLocation;//>m_Location;

				// Copy the pathway into the MultiRegionList
				long iCurrentItem = 1;
				CEOSAIRegionPathwayItem* pCurrentItem = PathwayResult.GetRegionResult( pPathStep->m_EndLocation );
				while( pCurrentItem )
				{
					if( iCurrentItem == 1 )
					{
						//pPathStep->m_fDistance = pCurrentItem->m_fDistance;
						//pPathStep->m_fStepTime = pCurrentItem->m_fDistance / fBestMovementRate;
						pResultStep->m_bIsValid = true;
						pResultStep->m_fDistance = pCurrentItem->m_fDistance;
						pResultStep->m_fStepTime = pCurrentItem->m_fTime; //pCurrentItem->m_fDistance / fBestMovementRate;

						//m_fPathwayDanger += pCurrentItem->m_fDanger;
						//m_fPathwayEnemySighting += pCurrentItem->m_fSightings;
					}
					iCurrentItem++;

					//CMultiRegion* pMultiRegion = pMultiRegionManager->GetMultiRegion( pCurrentItem->m_pPathfinderPoint->m_iId );
					//pPathStep->m_Calculated_MultiRegionPath.AddHead( pMultiRegion );
					CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( pCurrentItem->m_pPathfinderPoint->m_iId );
					pResultStep->m_AIRegionPath.AddTail( pAIRegion );
					pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
				}
				if( iCurrentItem == 1 ){ bStepIsValid = false; }
				if( iCurrentItem >  1 ){ bStepIsValid = true; }
				int g=0;
			}
		}
		if( iRegionType == 2 )
		{
			CalculateFinalPath();
		}

		CurrentLocation = pPathStep->m_EndLocation;//GetStepFinalLocation();
	}
}
*/

//bool CEOSAISeaUnitPathwayFinder::UpdateResultStepWithTransportAssistedPath( EOSAI::UnitPathwayResultStep* pTransporteeResultStep )
//bool CEOSAISeaUnitPathwayFinder::UpdateResultStepWithTransportAssistedPath( EOSAI::UnitPathwayResultStep* pGroundUnitResultStep )

/*
void CEOSAISeaUnitPathwayFinder::UpdatePickupPredefinedStep( CEOSAIUnitPathwayPredefinedStep* pGroundUnitPredefinedStep )
{
	ASSERT( GetUnitTemplate()->IsTransport() );
	ASSERT( pGroundUnitResultStep->m_pPredefinedStep->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsGroundUnit() );

	m_pPredefinedStep->
}
*/

// This gets called if you insert a new step before the pickup/dropoff.  If the transport's initial
//   location changes, then the pickup/dropoff location may need to change as well.
#ifdef DO_NOT_COMPILE_THIS_WAS_AN_ATTEMPTED_NEW_FUNCTION
bool CEOSAISeaUnitPathwayFinder::UpdateEndLocation( CEOSAIUnitPathwayPredefinedStep* pTransportPickupOrDropoff )
{
	if( pTransportPickupOrDropoff->GetTask() != CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget &&
		pTransportPickupOrDropoff->GetTask() != CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
	{
		return true; // nothing to update
	}

	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	ASSERT( GetUnitTemplate()->IsTransport() );

	CEOSAIUnitPathwayPredefinedStep* pTransportPickupPredefinedStep = NULL;
	if( pTransportPickupOrDropoff->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
	{
		pTransportPickupPredefinedStep = pTransportPickupOrDropoff;
	}
	CEOSAIUnitPathwayPredefinedStep* pTransportDropoffPredefinedStep = NULL;
	if( pTransportPickupOrDropoff->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
	{
		pTransportDropoffPredefinedStep = pTransportPickupOrDropoff;
	}
	CEOSAIRegionManager2* pAIRegionManager = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetAIRegionManager();

	ASSERT( pTransportPickupOrDropoff->GetAIUnitPathwayFinder() == this );
	CEOSAIUnit* pTransport = GetAIUnitActor();
	/*
	bool bFound = false;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		if( pStep == pTransportPickup ){ bFound = true; }
	}
	ASSERT( bFound );
	*/

	CEOSAIUnitPathwayPredefinedStep* pGroundUnitPredefinedStep = pTransportPickupOrDropoff->GetTransporteeStep();
	CEOSAIUnit* pGroundUnit = pGroundUnitPredefinedStep->GetAIUnitPathwayFinder()->GetAIUnitActor();

	//
	// Calculate pTransportPickupStep->StartLocation + this->StartLocation => Next Transport Step End
	//if( pPredefinedTransportPickupStep )
	if( pTransportPickupPredefinedStep )
	{
		// How do I calculate this?
		//   Transportee (ThisStart / PrevEndLocation).InvokeDirectAIRegionMapToCoasts();
		//   Transport (ThisStart / PrevEndLocation).InvokeDirectAIRegionMap();
		//   If the next step is a pickup or dropoff:
		//     Other Transportee (Start/End).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   -----If the next step is the corresponding dropoff:
		//   -----  Transportee (ThisEnd).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   If the next step isn't a corresponding dropoff:
		//     Transport.InvokeDirectAIRegionMap();
		// Find the best AIRegion Pickup using these three maps

		// Transport -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pTransportPickupStepPrev = pTransportPickupPredefinedStep->GetPrevStep();
		CEOSAIUnitPathwayPredefinedStep* pTransportPickupStepNext = pTransportPickupPredefinedStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportToPickupMap = NULL;
		if( pTransportPickupStepPrev ){ pTransportToPickupMap = pTransportPickupStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
		else{                           pTransportToPickupMap = pTransport->InvokeDirectAIRegionMapToEverywhere(); }

		// GroundUnit -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pGroundUnitPickupStepPrev = pGroundUnitPredefinedStep->GetPrevStep();
		CEOSAIRegionPathwayMap* pGroundUnitToPickupMap = NULL;
		if( pGroundUnitPickupStepPrev ){ pGroundUnitToPickupMap = pGroundUnitPickupStepPrev->InvokeEndLocationToEverywhereDirectMapPlusCoasts(); }
		//else{                            pGroundUnitToPickupMap = pGroundUnit->InvokeDirectAIRegionMapToCoasts(); }//>InvokeDirectAIRegionMapToEverywhere(); }
		else{                            pGroundUnitToPickupMap = pGroundUnit->InvokeDirectAIRegionMapToEverywhere(); }

		// Next Transport Location -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pTransporteePickupStepNext = pTransportPickupPredefinedStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportNextLocationToEverywhereMap = NULL;
		if( pTransportPickupStepNext )
		{
			// DEBUG
			#ifdef _DEBUG
			CEOSAILocation NextLocation = pTransportPickupStepNext->GetEndLocation();
			#endif _DEBUG
			// DEBUG
			pTransportNextLocationToEverywhereMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			/*
			if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				//CEOSAIUnitPathwayPredefinedStep*  GetTransporteeStep(){ return m_pTransporteeStep; }
				CEOSAIUnitPathwayPredefinedStep*  pTransporteeStep = pTransportPickupStepNext->GetTransporteeStep();
				if( pTransporteePickupStepPrev ){ pGroundUnitToPickup = pTransporteePickupStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
				else{                             pGroundUnitToPickup = pTransport->InvokeDirectAIRegionMapToEverywhere(); }
				pTransportToNextLocationMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			}
			else if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
			{
				EndLocation = pTransportPickupStepNext->GetImaginaryTransportDropoffLocation();
			}
			else
			{
				EndLocation = pTransportPickupStepNext->GetEndLocation();
			}

			if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				pTransportToNextLocationMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			}
			*/
		}
		else // The transport has no more PredefinedSteps after this one.  Do what the GroundUnit wants
		{
			// This shouldn't happen
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}
			pTransportNextLocationToEverywhereMap = pGroundUnitPredefinedStep->InvokeEndLocationToEverywhereGroundUnitAndSeaTransportMap();
		}

		float fGroundUnitStepStartTime = pGroundUnitPredefinedStep->GetStartTime();
		float fTransportStepStartTime = pTransportPickupPredefinedStep->GetStartTime();
		float fTransportMovementRate = m_pUnitTemplate->GetMovementRate();

		// If the transport and GroundUnit are near each other at the beginning of this step,
		//   then just do a direct pickup
		//float fTimeGap = fabs( fGroundUnitStepStartTime - fTransportStepStartTime );
		//float fDistance = pWorldBuildDesc->GetPixelDistance( GroundUnitStepStartLocation, TransportStepStartLocation );

		long iNumberOfRegions = pGroundUnitToPickupMap->GetRegionManager()->GetNumberOfGenericRegions();
		ASSERT( pGroundUnitToPickupMap->GetRegionManager() == GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetAIRegionManager() );

		// DEBUG
#ifdef DO_NOT_COMPILE
		long iValidGroundToPickupRegions = 0;
		long iValidTransportToPickupRegions = 0;
		long iValidTargetToPickupRegions = 0;
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
			if( pGroundUnitToPickupRegion != NULL ){ iValidGroundToPickupRegions++; }
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iRegion );
			if( pTransportToPickupRegion != NULL ){ iValidTransportToPickupRegions++; }
			CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iRegion );
			if( pTargetToPickupRegion != NULL ){ iValidTargetToPickupRegions++; }
		}
		if( Public::m_bAssertWhenAIPathwayFails )
		{
			ASSERT( iValidGroundToPickupRegions > 0 );
			ASSERT( iValidTransportToPickupRegions > 0 );
			ASSERT( iValidTargetToPickupRegions > 0 );
		}

		if( iValidTransportToPickupRegions == 0 )
		{
			CEOSAIRegionPathwayMap* pTransportPathwayMap = pTransport->InvokeDirectAIRegionMapToEverywhere();
			pTransportPathwayMap->Clear();
			pTransport->InvokeDirectAIRegionMapToEverywhere();
		}
		// DEBUG

		//
		long iGroundUnitToPickupRegionCount = 0;
		long iTransportToPickupRegionCount = 0;
		long iTargetToPickupRegionCount = 0;
		// Find the best pickup, where "best" = earliest [time to next location]
		float fBestTime = 1000000.0f;
		long  iBestPickupWaterRegion = 0;
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iRegion );
			CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iRegion );

			if( pGroundUnitToPickupRegion ){ iGroundUnitToPickupRegionCount++; }
			if( pTransportToPickupRegion ){ iTransportToPickupRegionCount++; }
			if( pTargetToPickupRegion ){ iTargetToPickupRegionCount++; }

			if( pGroundUnitToPickupRegion == NULL ) continue;
			if( pTransportToPickupRegion == NULL ) continue;
			if( pTargetToPickupRegion == NULL ) continue;

			// Don't pickup in cities I don't own
			if( pTransportToPickupRegion->m_pPathfinderPoint->IsCity() )
			{
				CEOSAIRegion2* pPickupAIRegion = dynamic_cast< CEOSAIRegion2* >( pTransportToPickupRegion->m_pPathfinderPoint );
				ASSERT( pPickupAIRegion );
				if( pPickupAIRegion )
				{
					if( pPickupAIRegion->GetCity()->GetOwner() != iAIPlayer )
					{
						continue;
					}
				}
			}

			//
			float fGroundUnitToPickupTime = fGroundUnitStepStartTime + pGroundUnitToPickupRegion->m_fTime;
			float fTransportToPickupTime = fTransportStepStartTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
			float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
			float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
			if( fGroundUnitToPickupTime < 0.7f && fTransportToPickupTime < 0.7f )
			{
				float fTime = min( fGroundUnitToPickupTime, fTransportToPickupTime );
				// If GroundUnit and Transport are both close to a pickup location, use it
				// tweak the time to favor this option
				fTotalTime -= 1.0f - fTime; 
			}
			if( pTransportToPickupRegion->m_pPathfinderPoint->IsCity() &&
				fTransportToPickupTime > 0.0f )
			{
				// Avoid pickups in cities, unless the transport is already in the city
				fTotalTime += 1.0f;
			}
			if( fTotalTime < fBestTime )
			{
				fBestTime = fTotalTime;
				iBestPickupWaterRegion = iRegion;
			}
			int g=0;
		}
		if( Public::m_bAssertWhenAIPathwayFails )
		{
			ASSERT( iBestPickupWaterRegion > 0 );
		}
		if( iBestPickupWaterRegion == 0 ) 
		{
			// DEBUG
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
				CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iRegion );
				if( pTransportToPickupRegion )
				{
					int g=0;
				}
				CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iRegion );

				if( pGroundUnitToPickupRegion == NULL ) continue;
				if( pTransportToPickupRegion == NULL ) continue;
				if( pTargetToPickupRegion == NULL ) continue;

				//
				float fGroundUnitToPickupTime = fGroundUnitStepStartTime + pGroundUnitToPickupRegion->m_fTime;
				float fTransportToPickupTime = fTransportStepStartTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
				float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
				float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
				if( fGroundUnitToPickupTime < 0.7f && fTransportToPickupTime < 0.7f )
				{
					float fTime = min( fGroundUnitToPickupTime, fTransportToPickupTime );
					// If GroundUnit and Transport are both close to a pickup location, use it
					// tweak the time to favor this option
					fTotalTime -= 1.0f - fTime; 
				}
				if( pTransportToPickupRegion->m_pPathfinderPoint->IsCity() &&
					fTransportToPickupTime > 0.0f )
				{
					// Avoid pickups in cities, unless the transport is already in the city
					fTotalTime += 1.0f;
				}
				if( fTotalTime < fBestTime )
				{
					fBestTime = fTotalTime;
					iBestPickupWaterRegion = iRegion;
				}
				int g=0;
			}
			// DEBUG

			return false; // Couldn't find a pickup point
		}

		//
		// Record the values
		//
		CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iBestPickupWaterRegion );
		CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iBestPickupWaterRegion );
		//CEOSAIRegionPathwayItem* pTargetToPickupRegion = TargetToEverywhereMap.GetRegionResult( iBestPickupWaterRegion );

		//
		CList< long >  GroundUnitToPickupPath;
		CList< long >  TransportToPickupPath;
		float fGroundUnitDistance = 0.0f;
		long iLandPickupRegion = 0;
		CEOSAIRegionPathwayItem* pCurrentPathwayItem = pGroundUnitToPickupRegion;
		if( pGroundUnitToPickupRegion )
		{
			fGroundUnitDistance += pGroundUnitToPickupRegion->m_fDistance;
		}
		while( pCurrentPathwayItem )
		{
			if( iLandPickupRegion == 0 )
			{
				if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
					pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					iLandPickupRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
			}
			CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			//ASSERT( pAIRegion );
		//	pGroundUnitResultStep->m_AIRegionMovementPath.AddTail( pAIRegion );
			pCurrentPathwayItem = pGroundUnitToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}
		pCurrentPathwayItem = pTransportToPickupRegion;
		while( pCurrentPathwayItem )
		{
			//pResultStep->m_AIRegionMovementPath.AddHead( pAIRegion );
			TransportToPickupPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTransportToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}

		pTransportPickupPredefinedStep->SetRealTransport_PickupWaterAIRegion( iBestPickupWaterRegion );
		pTransportPickupPredefinedStep->SetRealTransport_PickupGroundAIRegion( iLandPickupRegion );
		pGroundUnitPredefinedStep->SetRealTransport_PickupWaterAIRegion( iBestPickupWaterRegion );
		pGroundUnitPredefinedStep->SetRealTransport_PickupGroundAIRegion( iLandPickupRegion );
		ASSERT( iLandPickupRegion > 0 );

		CEOSAILocation WaterLocation, LandLocation;
		CEOSAIUnitPathwayFinder::FindPickupDropoffLocations( 
			pTransportPickupPredefinedStep->GetRealTransport_PickupWaterAIRegion(),
			pTransportPickupPredefinedStep->GetRealTransport_PickupGroundAIRegion(),
			&WaterLocation, &LandLocation );
		pTransportPickupPredefinedStep->SetEndLocation( WaterLocation );

		// Update the Transport's Pickup StepTime
		//pTransportPickupPredefinedStep->

		//fGroundUnitMovementTime += fGroundUnitDistance / (pGroundUnitTemplate->GetMovementRate()+0.1f);

		pTransportPickupPredefinedStep->SetEndLocation( WaterLocation );
		if( pTransportPickupPredefinedStep->GetResultStep() )
		{
			pTransportPickupPredefinedStep->GetResultStep()->m_EndLocation = WaterLocation;
		}

		return true;
#endif DO_NOT_COMPILE
		long iGroundUnitGeo = pGroundUnit->GetInitialState()->GetGeo();
		CGeo* pGeo = GetAIBrain()->GetWorldDescServer()->GetGeo( iGroundUnitGeo );
		ASSERT( dynamic_cast< CGeoLand* >( pGeo ) );

		long iBestLandPickupRegion = 0;
		long iBestWaterPickupRegion = 0;
		float fBestTime = 1000000.0f;

		//float fGroundUnitStepStartTime = pGroundUnitPredefinedStep->GetStartTime();
		//float fTransportStepStartTime = pTransportPickupPredefinedStep->GetStartTime();

		CEOSAIRegionPathwayMap* pGroundUnitDirectAIRegionMap = pTransport->InvokeDirectAIRegionMapToEverywhere();
		CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
		CGeoCoastalData* pGeoCoastalData = pAIRegionManager->GetGeoCoastalData( iGroundUnitGeo );
		if( pGeoCoastalData )
		{
			long iGroundUnitToPickupRegionCount = 0;
			long iTransportToPickupRegionCount = 0;
			long iTargetToPickupRegionCount = 0;
			POSITION pos = pGeoCoastalData->m_GeoSwitchList.GetHeadPosition();
			while( pos )
			{
				CGeoSwitchItem* pGeoSwitchItem = pGeoCoastalData->m_GeoSwitchList.GetNext( pos );
				long iLandAIRegion = pGeoSwitchItem->m_iLandAIRegion;
				long iWaterAIRegion = pGeoSwitchItem->m_iWaterAIRegion;
				//
				CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitDirectAIRegionMap->GetRegionResult( iLandAIRegion );
				CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iWaterAIRegion );
				CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iWaterAIRegion );

				//
				if( pGroundUnitToPickupRegion ) iGroundUnitToPickupRegionCount++;
				if( pTransportToPickupRegion ) iTransportToPickupRegionCount++;
				if( pTargetToPickupRegion ) iTargetToPickupRegionCount++;

				if( pGroundUnitToPickupRegion && pTransportToPickupRegion && pTargetToPickupRegion )
				{
					float fGroundUnitToPickupTime = fGroundUnitStepStartTime + pGroundUnitToPickupRegion->m_fTime;
					float fTransportToPickupTime = fTransportStepStartTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
					float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
					float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
					if( fGroundUnitToPickupTime < 0.6f && fTransportToPickupTime < 0.5f )
					{
						// If GroundUnit and Transport are both close to a pickup location, use it
						// tweak the time to favor this option
						fTotalTime -= 1.0f; 
					}
					if( fTotalTime < fBestTime )
					{
						fBestTime = fTotalTime;
						iBestLandPickupRegion = iLandAIRegion;
						iBestWaterPickupRegion = iWaterAIRegion;
					}
					int g=0;
				}
			}
			int sdsad=0;
		}
		//if( pPathResults && iBestLandPickupRegion > 0 && iBestWaterPickupRegion > 0 )
		//ASSERT( iBestLandPickupRegion > 0 && iBestWaterPickupRegion > 0 );
		if( Public::m_bAssertWhenAIPathwayFails )
		{
			ASSERT( iBestLandPickupRegion > 0 || iBestWaterPickupRegion > 0 );
		}
		if( iBestLandPickupRegion == 0 && iBestWaterPickupRegion == 0 ) 
		{
			/*
			// DEBUG
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
				CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iRegion );
				if( pTransportToPickupRegion )
				{
					int g=0;
				}
				CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iRegion );

				if( pGroundUnitToPickupRegion == NULL ) continue;
				if( pTransportToPickupRegion == NULL ) continue;
				if( pTargetToPickupRegion == NULL ) continue;

				//
				float fGroundUnitToPickupTime = fGroundUnitStepStartTime + pGroundUnitToPickupRegion->m_fTime;
				float fTransportToPickupTime = fTransportStepStartTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
				float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
				float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
				if( fGroundUnitToPickupTime < 0.7f && fTransportToPickupTime < 0.7f )
				{
					float fTime = min( fGroundUnitToPickupTime, fTransportToPickupTime );
					// If GroundUnit and Transport are both close to a pickup location, use it
					// tweak the time to favor this option
					fTotalTime -= 1.0f - fTime; 
				}
				if( pTransportToPickupRegion->m_pPathfinderPoint->IsCity() &&
					fTransportToPickupTime > 0.0f )
				{
					// Avoid pickups in cities, unless the transport is already in the city
					fTotalTime += 1.0f;
				}
				if( fTotalTime < fBestTime )
				{
					fBestTime = fTotalTime;
					iBestPickupWaterRegion = iRegion;
				}
				int g=0;
			}
			*/
			// DEBUG

			return false; // Couldn't find a pickup point
		}
		if( iBestLandPickupRegion > 0 && iBestWaterPickupRegion > 0 )
		{
			//
			// Record the values
			//
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iBestLandPickupRegion );
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iBestWaterPickupRegion );
			//CEOSAIRegionPathwayItem* pTargetToPickupRegion = TargetToEverywhereMap.GetRegionResult( iBestPickupWaterRegion );

			//
			CList< long >  GroundUnitToPickupPath;
			CList< long >  TransportToPickupPath;
			float fGroundUnitDistance = 0.0f;
			long iLandPickupRegion = 0;
			CEOSAIRegionPathwayItem* pCurrentPathwayItem = pGroundUnitToPickupRegion;
			if( pGroundUnitToPickupRegion )
			{
				fGroundUnitDistance += pGroundUnitToPickupRegion->m_fDistance;
			}
			while( pCurrentPathwayItem )
			{
				if( iLandPickupRegion == 0 )
				{
					if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
						pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
					{
						iLandPickupRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
					}
				}
				CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentPathwayItem->m_pPathfinderPoint );
				//ASSERT( pAIRegion );
			//	pGroundUnitResultStep->m_AIRegionMovementPath.AddTail( pAIRegion );
				pCurrentPathwayItem = pGroundUnitToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}
			pCurrentPathwayItem = pTransportToPickupRegion;
			while( pCurrentPathwayItem )
			{
				//pResultStep->m_AIRegionMovementPath.AddHead( pAIRegion );
				TransportToPickupPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				pCurrentPathwayItem = pTransportToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}

			pTransportPickupPredefinedStep->SetRealTransport_PickupWaterAIRegion( iBestWaterPickupRegion );
			pTransportPickupPredefinedStep->SetRealTransport_PickupGroundAIRegion( iBestLandPickupRegion );
			pGroundUnitPredefinedStep->SetRealTransport_PickupWaterAIRegion( iBestWaterPickupRegion );
			pGroundUnitPredefinedStep->SetRealTransport_PickupGroundAIRegion( iBestLandPickupRegion );
			ASSERT( iLandPickupRegion > 0 );

			CEOSAILocation WaterLocation, LandLocation;
			CEOSAIUnitPathwayFinder::FindPickupDropoffLocations( 
				pTransportPickupPredefinedStep->GetRealTransport_PickupWaterAIRegion(),
				pTransportPickupPredefinedStep->GetRealTransport_PickupGroundAIRegion(),
				&WaterLocation, &LandLocation );
			pTransportPickupPredefinedStep->SetEndLocation( WaterLocation );

			// Update the Transport's Pickup StepTime
			//pTransportPickupPredefinedStep->

			//fGroundUnitMovementTime += fGroundUnitDistance / (pGroundUnitTemplate->GetMovementRate()+0.1f);

			pTransportPickupPredefinedStep->SetEndLocation( WaterLocation );
			if( pTransportPickupPredefinedStep->GetResultStep() )
			{
				pTransportPickupPredefinedStep->GetResultStep()->m_EndLocation = WaterLocation;
			}

			return true;
		}
	}

	// Calculate pTransportDropoffStep->StartLocation => this->EndLocation + Next Transport Step Start
	if( pTransportDropoffPredefinedStep )
	{
		if( pTransport &&
			pTransport->GetServerUnit() == NULL &&
			pTransport->GetUnitTemplate()->GetInternalName() == _T("Transport (Class 2)") &&
			pTransport->GetBuiltByCity() &&
			pTransport->GetBuiltByCity()->GetObjectId() == 3 )
		{
			int g=0;
		}
		if( pTransport &&
			pTransport->GetServerUnit() &&
			pTransport->GetServerUnit()->GetObjectId() == 149 )
		{
			int h=0;
		}

		//CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pTransportDropoffStep->GetNextStep();
		/*
		CEOSAILocation TransportStartLocation = pTransportDropoffStep->GetStartLocation();
		CEOSAILocation TransporteeEndLocation = pGroundUnitResultStep->m_EndLocation;
		CEOSAILocation TransportNextEndLocation; // Either the Transport's Next End Location, or Transportee's Target Location

		if( pTransportDropoffStepNext )
		{
			if( pTransportDropoffStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetImaginaryTransportPickupLocation();
			}
			else if( pTransportDropoffStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetImaginaryTransportDropoffLocation();
			}
			else
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetEndLocation();
			}
		}
		else
		{
			// The transport has no 'next' step - just use the Transportee's End location
			TransportNextEndLocation = pGroundUnitResultStep->m_EndLocation;
		}
		*/

		// How do I calculate this?
		//   Transportee (ThisStart / PrevEndLocation).InvokeDirectAIRegionMapToCoasts();
		//   Transport (ThisStart / PrevEndLocation).InvokeDirectAIRegionMap();
		//   If the next step is a pickup or dropoff:
		//     Other Transportee (Start/End).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   -----If the next step is the corresponding dropoff:
		//   -----  Transportee (ThisEnd).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   If the next step isn't a corresponding dropoff:
		//     Transport.InvokeDirectAIRegionMap();
		// Find the best AIRegion Pickup using these three maps

		// Transport -> Dropoff
		CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepPrev = pTransportDropoffPredefinedStep->GetPrevStep();
		//CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pTransportDropoffPredefinedStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportToDropoffMap = NULL;
		if( pTransportDropoffStepPrev ){ pTransportToDropoffMap = pTransportDropoffStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
		else{                            pTransportToDropoffMap = pTransport->InvokeDirectAIRegionMapToEverywhere(); }
		// DEBUG
		{
			long iNumberOfValidRegions = 0;
			long iNumberOfRegions = pTransportToDropoffMap->GetRegionManager()->GetNumberOfGenericRegions();
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iRegion );
				if( pTargetToDropoffRegion == NULL ) continue;
				//if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;
				iNumberOfValidRegions++;
			}
			ASSERT( iNumberOfValidRegions > 0 );
		}
		// DEBUG

		// GroundUnit (End) -> Everywhere (helps determine dropoff point)
		CEOSAIRegionPathwayMap* pTargetToCoastsMap = pGroundUnitPredefinedStep->InvokeEndLocationToEverywhereDirectMapPlusCoasts();
		// DEBUG
		{
			long iNumberOfValidRegions = 0;
			long iNumberOfRegions = pTargetToCoastsMap->GetRegionManager()->GetNumberOfGenericRegions();
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iRegion );
				if( pTargetToDropoffRegion == NULL ) continue;
				if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;
				iNumberOfValidRegions++;
			}
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( iNumberOfValidRegions > 0 );
			}
		}
		// DEBUG

		// Next Transport Location to Everywhere
		CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pTransportDropoffPredefinedStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportEndToEverywhereMap = NULL;
		if( pTransportDropoffStepNext )
		{
			pTransportEndToEverywhereMap = pTransportDropoffStepNext->InvokeEndLocationToEverywhereDirectMap();
		}

		// Find the best dropoff, where "best" = earliest [time to next location]
		float fTransportMovementRate = m_pUnitTemplate->GetMovementRate();
		float fBestTime = 1000000.0f;
		float fBestTime_GroundUnitDropoffToTargetTime = 1000000.0f;
		long  iBestDropoffWaterRegion = 0;
		long iNumberOfRegions = pTargetToCoastsMap->GetRegionManager()->GetNumberOfGenericRegions();
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iRegion );
			if( pTargetToDropoffRegion == NULL ) continue;
			CEOSAIRegionPathwayItem* pTransportToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iRegion );
			if( pTransportToDropoffRegion == NULL ) continue;

			float fTransportToNextStepTime = 0.0f;
			CEOSAIRegionPathwayItem* pTransportEndToEverywhereRegion = NULL;
			if( pTransportEndToEverywhereMap )
			{
				pTransportEndToEverywhereRegion = pTransportEndToEverywhereMap->GetRegionResult( iRegion );
				//if( pTransportEndToEverywhereRegion == NULL ) continue;
				//fTransportToNextStepTime = pTransportEndToEverywhereRegion->m_fTime;
				if( pTransportEndToEverywhereRegion )
				{
					fTransportToNextStepTime = pTransportEndToEverywhereRegion->m_fTime;
				}
			}

			// Transports can't enter other player's cities
			//if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;
			CEOSAIRegion2* pTargetToDropoffAIRegion = dynamic_cast< CEOSAIRegion2* >( pTargetToDropoffRegion->m_pPathfinderPoint );
			CEOSAIRegion2* pTransportDropoffAIRegion = dynamic_cast< CEOSAIRegion2* >( pTransportToDropoffRegion->m_pPathfinderPoint );
			ASSERT( pTargetToDropoffAIRegion );
			ASSERT( pTransportDropoffAIRegion );
			if( pTargetToDropoffAIRegion->IsCity() && 
				pTargetToDropoffAIRegion->GetCity()->GetOwner() != iAIPlayer )
			{
				continue;
			}
			CEOSAIRegionPathwayItem* pPrevTargetToDropoffRegion = pTargetToDropoffRegion->GetPreviousPathwayItem();
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( pPrevTargetToDropoffRegion );
			}
			if( pPrevTargetToDropoffRegion )
			{
				CEOSAIRegion2* pPrevTargetToDropoffAIRegion = dynamic_cast< CEOSAIRegion2* >( pPrevTargetToDropoffRegion->m_pPathfinderPoint );
				if( pPrevTargetToDropoffAIRegion &&
					pPrevTargetToDropoffAIRegion->IsCity() &&
					pPrevTargetToDropoffAIRegion->GetCity()->GetOwner() != iAIPlayer )
				{
					continue;
				}
			}
			if( pTransportDropoffAIRegion->IsCity() && 
				pTransportDropoffAIRegion->GetCity()->GetOwner() != iAIPlayer )
			{
				continue;
			}

			//
			float fGroundUnitDropoffToTargetTime = pTargetToDropoffRegion->m_fTime;
			float fTransportToDropoffTime = (pTransportToDropoffRegion->m_fDistance/fTransportMovementRate);
			float fTotalTime = fTransportToDropoffTime + fGroundUnitDropoffToTargetTime + fTransportToNextStepTime;
			/*
			if( fGroundUnitToPickupTime < 0.5f && fTransportToPickupTime < 0.5f )
			{
				// If GroundUnit and Transport are both close to a pickup location, use it
				// tweak the time to favor this option
				fTotalTime -= 1.0f; 
			}
			*/
			if( fTotalTime < fBestTime )
			{
				fBestTime = fTotalTime;
				fBestTime_GroundUnitDropoffToTargetTime = fGroundUnitDropoffToTargetTime;
				iBestDropoffWaterRegion = iRegion;
			}
			int g=0;
		}
		if( Public::m_bAssertWhenAIPathwayFails )
		{
			ASSERT( iBestDropoffWaterRegion > 0 );
		}
		if( iBestDropoffWaterRegion == 0 ) return false; // Couldn't find a dropoff point

		//
		long iLandDropoffRegion = 0;

		// Drop the pathway information into a path and then walk it backwards
		CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iBestDropoffWaterRegion );
		CEOSAIRegionPathwayItem* pTransportToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iBestDropoffWaterRegion );
		//
		pGroundUnitPredefinedStep->GetAIRegionMovementPath()->RemoveAll();
		CList< long > GroundUnitPath;
		CEOSAIRegionPathwayItem* pCurrentPathwayItem = pTargetToDropoffRegion;
		while( pCurrentPathwayItem )
		{
			if( iLandDropoffRegion == 0 )
			{
				if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
					pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					iLandDropoffRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
			}
			CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			if( pAIRegion )
			{
				pGroundUnitPredefinedStep->GetAIRegionMovementPath()->AddTail( pAIRegion );
			}
			CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			if( pAIRoad )
			{
				CEOSAIRegion2* pLastAIRegion = NULL;
				if( pGroundUnitPredefinedStep->GetAIRegionMovementPath()->IsEmpty() == false )
				{
					pLastAIRegion = pGroundUnitPredefinedStep->GetAIRegionMovementPath()->GetTail();
				}

				if( pAIRoad->GetAIRegion1() != pLastAIRegion )
				{
					pGroundUnitPredefinedStep->GetAIRegionMovementPath()->AddTail( pAIRoad->GetAIRegion1() );
				}
				if( pAIRoad->GetAIRegion2() != pLastAIRegion )
				{
					pGroundUnitPredefinedStep->GetAIRegionMovementPath()->AddTail( pAIRoad->GetAIRegion2() );
				}
			}
		//	pGroundUnitResultStep->m_AIRegionMovementPath.AddTail( pAIRegion );

			//GroundUnitPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTargetToCoastsMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}
		CList< long > TransportPath;
		pCurrentPathwayItem = pTransportToDropoffRegion;
		while( pCurrentPathwayItem )
		{
			TransportPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTransportToDropoffMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}

		pTransportDropoffPredefinedStep->SetRealTransport_DropoffWaterAIRegion( iBestDropoffWaterRegion );
		pTransportDropoffPredefinedStep->SetRealTransport_DropoffGroundAIRegion( iLandDropoffRegion );
		pGroundUnitPredefinedStep->SetRealTransport_DropoffWaterAIRegion( iBestDropoffWaterRegion );
		pGroundUnitPredefinedStep->SetRealTransport_DropoffGroundAIRegion( iLandDropoffRegion );

		CEOSAILocation WaterLocation, LandLocation;
		CEOSAIUnitPathwayFinder::FindPickupDropoffLocations( 
			pTransportDropoffPredefinedStep->GetRealTransport_DropoffWaterAIRegion(),
			pTransportDropoffPredefinedStep->GetRealTransport_DropoffGroundAIRegion(),
			&WaterLocation, &LandLocation );
		pTransportDropoffPredefinedStep->SetEndLocation( WaterLocation );

		// DEBUG
		CEOSAIRegionPathwayItem* pCurrentItem1 = pTransportToDropoffMap->GetRegionResult( pTransportDropoffPredefinedStep->GetStartLocation() );
		ASSERT( pCurrentItem1 );
		CEOSAIRegionPathwayMap* pEndLocationToEverywhere = pTransportDropoffPredefinedStep->InvokeEndLocationToEverywhereDirectMap();
		CEOSAIRegionPathwayItem* pCurrentItem2 = pEndLocationToEverywhere->GetRegionResult( pTransportDropoffPredefinedStep->GetStartLocation() );
		ASSERT( pCurrentItem2 );
		// DEBUG

		// Update the Transport StepTime
		//pPredefinedTransportDropoffStep->m_fSt

		// Update the GroundUnit StepTime
//		fGroundUnitMovementTime += fBestTime; // fBestTime = Transport->Dropoff + GroundUnit->Target
//		pGroundUnitResultStep->m_fStepTime = fGroundUnitMovementTime;
//		pGroundUnitResultStep->m_fDropoffTimeToTargetTime = fBestTime_GroundUnitDropoffToTargetTime;

		pTransportDropoffPredefinedStep->SetEndLocation( WaterLocation );
		if( pTransportDropoffPredefinedStep->GetResultStep() )
		{
			pTransportDropoffPredefinedStep->GetResultStep()->m_EndLocation = WaterLocation;
		}

		/*
		bool bMovementOnLand = true;
		POSITION pos = Path.GetHeadPosition();
		while( pos )
		{
			long iRegion = Path.GetNext( pos );
			CEOSAIRegionPathwayItem* pPathItem = TargetToEverywhereMap.GetRegionResult( iRegion );

			if( pPathItem->m_pPathfinderPoint->IsWater() ){ bMovementOnLand = false; }
			if( bMovementOnLand )
			{
				iLandDropoffRegion = pPathItem->m_pPathfinderPoint->GetId();
				ASSERT( pPathItem->m_pPathfinderPoint->IsLand() || pPathItem->m_pPathfinderPoint->IsCity() );
				//pPathResults->m_GroundUnitFromDropoffToTargetPath.AddTail( iRegion );
				GroundUnitFromDropoffToTargetPath.AddTail( iRegion );
			}
			else
			{
				if( iWaterDropoffRegion == 0 )
				{
					iWaterDropoffRegion = pPathItem->m_pPathfinderPoint->GetId();
				}
				ASSERT( pPathItem->m_pPathfinderPoint->IsWater() || pPathItem->m_pPathfinderPoint->IsCity() );
				//pPathResults->m_TransportPickupToDropoffPath.AddTail( iRegion );
				TransportPickupToDropoffPath.AddTail( iRegion );
			}
		}
		*/

		return true;
	}
	return false;
}
#endif DO_NOT_COMPILE_THIS_WAS_AN_ATTEMPTED_NEW_FUNCTION

// This gets called if you insert a new step before the pickup/dropoff.  If the transport's initial
//   location changes, then the pickup/dropoff location may need to change as well.
//#ifdef DO_NOT_COMPILE_OLD_FUNCTION
bool CEOSAISeaUnitPathwayFinder::UpdateEndLocation( CEOSAIUnitPathwayPredefinedStep* pTransportPickupOrDropoff )
{
	if( pTransportPickupOrDropoff->GetTask() != CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget &&
		pTransportPickupOrDropoff->GetTask() != CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
	{
		return true; // nothing to update
	}

	long iAIPlayer = m_iUnitOwner; //GetAIBrain()->GetAIPlayerNumber();
	ASSERT( GetAIUnitTemplate()->IsTransport() );

	CEOSAIUnitPathwayPredefinedStep* pTransportPickupPredefinedStep = NULL;
	if( pTransportPickupOrDropoff->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
	{
		pTransportPickupPredefinedStep = pTransportPickupOrDropoff;
	}
	CEOSAIUnitPathwayPredefinedStep* pTransportDropoffPredefinedStep = NULL;
	if( pTransportPickupOrDropoff->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
	{
		pTransportDropoffPredefinedStep = pTransportPickupOrDropoff;
	}
	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();

	ASSERT( pTransportPickupOrDropoff->GetAIUnitPathwayFinder() == this );
	CEOSAIUnit* pTransport = GetAIUnitActor();
	/*
	bool bFound = false;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		if( pStep == pTransportPickup ){ bFound = true; }
	}
	ASSERT( bFound );
	*/

	CEOSAIUnitPathwayPredefinedStep* pGroundUnitPredefinedStep = pTransportPickupOrDropoff->GetTransporteeStep();
	CEOSAIUnit* pGroundUnit = pGroundUnitPredefinedStep->GetAIUnitPathwayFinder()->GetAIUnitActor();

	#ifdef _DEBUG
	if (pTransport->GetObjectId() == 139)
	{
		int h=0;
	}
	#endif

	//
	// Calculate pTransportPickupStep->StartLocation + this->StartLocation => Next Transport Step End
	//if( pPredefinedTransportPickupStep )
	if( pTransportPickupPredefinedStep )
	{
		// How do I calculate this?
		//   Transportee (ThisStart / PrevEndLocation).InvokeDirectAIRegionMapToCoasts();
		//   Transport (ThisStart / PrevEndLocation).InvokeDirectAIRegionMap();
		//   If the next step is a pickup or dropoff:
		//     Other Transportee (Start/End).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   -----If the next step is the corresponding dropoff:
		//   -----  Transportee (ThisEnd).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   If the next step isn't a corresponding dropoff:
		//     Transport.InvokeDirectAIRegionMap();
		// Find the best AIRegion Pickup using these three maps

		// Transport -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pTransportPickupStepPrev = pTransportPickupPredefinedStep->GetPrevStep();
		CEOSAIUnitPathwayPredefinedStep* pTransportPickupStepNext = pTransportPickupPredefinedStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportToPickupMap = NULL;
		if( pTransportPickupStepPrev ){ pTransportToPickupMap = pTransportPickupStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
		else{                           pTransportToPickupMap = pTransport->InvokeDirectAIRegionMapToEverywhere(); }

		// GroundUnit -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pGroundUnitPickupStepPrev = pGroundUnitPredefinedStep->GetPrevStep();
		CEOSAIRegionPathwayMap* pGroundUnitToPickupMap = NULL;
		if( pGroundUnitPickupStepPrev ){ pGroundUnitToPickupMap = pGroundUnitPickupStepPrev->InvokeEndLocationToEverywhereDirectMapPlusCoasts(); }
		else{                            pGroundUnitToPickupMap = pGroundUnit->InvokeDirectAIRegionMapToCoasts(); }//>InvokeDirectAIRegionMapToEverywhere(); }
		//else{                            pGroundUnitToPickupMap = pGroundUnit->InvokeDirectAIRegionMapToEverywhere(); }

		// Next Transport Location -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pTransporteePickupStepNext = pTransportPickupPredefinedStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportNextLocationToEverywhereMap = NULL;
		if( pTransportPickupStepNext )
		{
			// DEBUG
			#ifdef _DEBUG
			CEOSAILocation NextLocation = pTransportPickupStepNext->GetEndLocation();
			#endif _DEBUG
			// DEBUG
			pTransportNextLocationToEverywhereMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			/*
			if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				//CEOSAIUnitPathwayPredefinedStep*  GetTransporteeStep(){ return m_pTransporteeStep; }
				CEOSAIUnitPathwayPredefinedStep*  pTransporteeStep = pTransportPickupStepNext->GetTransporteeStep();
				if( pTransporteePickupStepPrev ){ pGroundUnitToPickup = pTransporteePickupStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
				else{                             pGroundUnitToPickup = pTransport->InvokeDirectAIRegionMapToEverywhere(); }
				pTransportToNextLocationMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			}
			else if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
			{
				EndLocation = pTransportPickupStepNext->GetImaginaryTransportDropoffLocation();
			}
			else
			{
				EndLocation = pTransportPickupStepNext->GetEndLocation();
			}

			if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				pTransportToNextLocationMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			}
			*/
		}
		else // The transport has no more PredefinedSteps after this one.  Do what the GroundUnit wants
		{
			// This shouldn't happen
			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				//g_LogFile.WriteString();
				ASSERT( false );
			}
			pTransportNextLocationToEverywhereMap = pGroundUnitPredefinedStep->InvokeEndLocationToEverywhereGroundUnitAndSeaTransportMap();
		}

		float fGroundUnitStepStartTime = pGroundUnitPredefinedStep->GetStartTime();
		float fTransportStepStartTime = pTransportPickupPredefinedStep->GetStartTime();

		// If the transport and GroundUnit are near each other at the beginning of this step,
		//   then just do a direct pickup
		//float fTimeGap = fabs( fGroundUnitStepStartTime - fTransportStepStartTime );
		//float fDistance = pWorldBuildDesc->GetPixelDistance( GroundUnitStepStartLocation, TransportStepStartLocation );

		long iNumberOfRegions = pGroundUnitToPickupMap->GetRegionManager()->GetNumberOfGenericRegions();
		ASSERT( pGroundUnitToPickupMap->GetRegionManager() == g_pEOSAICommonData->GetAIRegionManager() );

		// DEBUG
		long iValidGroundToPickupRegions = 0;
		long iValidTransportToPickupRegions = 0;
		long iValidTargetToPickupRegions = 0;
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
			if( pGroundUnitToPickupRegion != NULL ){ iValidGroundToPickupRegions++; }
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iRegion );
			if( pTransportToPickupRegion != NULL ){ iValidTransportToPickupRegions++; }
			CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iRegion );
			if( pTargetToPickupRegion != NULL ){ iValidTargetToPickupRegions++; }
		}
		if( EOSAISettings::m_bAssertWhenAIPathwayFails )
		{
			//ASSERT( iValidGroundToPickupRegions > 0 );
			ASSERT( iValidTransportToPickupRegions > 0 );
			ASSERT( iValidTargetToPickupRegions > 0 );
		}

		if( iValidTransportToPickupRegions == 0 )
		{
			CEOSAIRegionPathwayMap* pTransportPathwayMap = pTransport->InvokeDirectAIRegionMapToEverywhere();
			pTransportPathwayMap->Clear();
			pTransport->InvokeDirectAIRegionMapToEverywhere();
		}
		// DEBUG

		if (pTransport->GetObjectId() == 139)
		{
			int h = 0;
		}

		//
		long iGroundUnitToPickupRegionCount = 0;
		long iTransportToPickupRegionCount = 0;
		long iTargetToPickupRegionCount = 0;
		// Find the best pickup, where "best" = earliest [time to next location]
		float fTransportMovementRate = m_pAIUnitTemplate->GetMovementRate();
		float fBestTime = 1000000.0f;
		long  iBestPickupWaterRegion = 0;
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iRegion );
			CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iRegion );

			if( pGroundUnitToPickupRegion ){ iGroundUnitToPickupRegionCount++; }
			if( pTransportToPickupRegion ){ iTransportToPickupRegionCount++; }
			if( pTargetToPickupRegion ){ iTargetToPickupRegionCount++; }

			if( pGroundUnitToPickupRegion == NULL ) continue;
			if( pTransportToPickupRegion == NULL ) continue;
			if( pTargetToPickupRegion == NULL ) continue;

			// Don't pickup in cities I don't own
			if( pTransportToPickupRegion->m_pPathfinderPoint->IsCity() )
			{
				CEOSAIRegion2* pPickupAIRegion = dynamic_cast< CEOSAIRegion2* >( pTransportToPickupRegion->m_pPathfinderPoint );
				ASSERT( pPickupAIRegion );
				if( pPickupAIRegion )
				{
					if( pPickupAIRegion->GetEOSAICity()->GetOwner() != iAIPlayer )
					{
						continue;
					}
				}
			}

			//
			float fGroundUnitToPickupTime = fGroundUnitStepStartTime + pGroundUnitToPickupRegion->m_fTime;
			float fTransportToPickupTime = fTransportStepStartTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
			float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
			float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
			if( fGroundUnitToPickupTime < 0.7f && fTransportToPickupTime < 0.7f )
			{
				float fTime = min( fGroundUnitToPickupTime, fTransportToPickupTime );
				// If GroundUnit and Transport are both close to a pickup location, use it
				// tweak the time to favor this option
				fTotalTime -= 1.0f - fTime; 
			}
			if( pTransportToPickupRegion->m_pPathfinderPoint->IsCity() &&
				fTransportToPickupTime > 0.0f )
			{
				// Avoid pickups in cities, unless the transport is already in the city
				fTotalTime += 1.0f;
			}
			if( fTotalTime < fBestTime )
			{
				fBestTime = fTotalTime;
				iBestPickupWaterRegion = iRegion;

				if (pTransport->GetObjectId() == 139)
				{
					int h = 0;
				}
			}
			int g=0;
		}
		if( EOSAISettings::m_bAssertWhenAIPathwayFails )
		{
			// The ai needs to record the fact that the infantry is already inside the transport, and check it before complaining.
			//   Example: Hansa 124 (Transport)pickup 6th Infantry(125)
			ASSERT( iBestPickupWaterRegion > 0 );
		}
		if( iBestPickupWaterRegion == 0 ) 
		{
			// DEBUG
			#ifdef _DEBUG
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
				CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iRegion );
				if( pTransportToPickupRegion )
				{
					int g=0;
				}
				CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iRegion );

				if( pGroundUnitToPickupRegion == NULL ) continue;
				if( pTransportToPickupRegion == NULL ) continue;
				if( pTargetToPickupRegion == NULL ) continue;

				//
				float fGroundUnitToPickupTime = fGroundUnitStepStartTime + pGroundUnitToPickupRegion->m_fTime;
				float fTransportToPickupTime = fTransportStepStartTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
				float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
				float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
				if( fGroundUnitToPickupTime < 0.7f && fTransportToPickupTime < 0.7f )
				{
					float fTime = min( fGroundUnitToPickupTime, fTransportToPickupTime );
					// If GroundUnit and Transport are both close to a pickup location, use it
					// tweak the time to favor this option
					fTotalTime -= 1.0f - fTime; 
				}
				if( pTransportToPickupRegion->m_pPathfinderPoint->IsCity() &&
					fTransportToPickupTime > 0.0f )
				{
					// Avoid pickups in cities, unless the transport is already in the city
					fTotalTime += 1.0f;
				}
				if( fTotalTime < fBestTime )
				{
					fBestTime = fTotalTime;
					iBestPickupWaterRegion = iRegion;
				}
				int g=0;
			}
			#endif _DEBUG
			// DEBUG

			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}
			return false; // Couldn't find a pickup point
		}

		//
		// Record the values
		//
		CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iBestPickupWaterRegion );
		CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iBestPickupWaterRegion );
		//CEOSAIRegionPathwayItem* pTargetToPickupRegion = TargetToEverywhereMap.GetRegionResult( iBestPickupWaterRegion );

		#ifdef _DEBUG
		if( pGroundUnitPredefinedStep->GetAIUnitPathwayFinder()->GetAIUnitActor()->GetObjectId() == 19 )
		//if( pGroundUnitPredefinedStep->GetAIUnitActionIdea()->GetAIPoiObjectActor()->GetObjectId() == 19 )
		{
			int h=0;
		}
		#endif _DEBUG

		//
		CList< long >  GroundUnitToPickupPath;
		CList< long >  TransportToPickupPath;
		float fGroundUnitDistance = 0.0f;
		long iLandPickupRegion = 0;
		CEOSAIRegionPathwayItem* pCurrentPathwayItem = pGroundUnitToPickupRegion;
		if( pGroundUnitToPickupRegion )
		{
			fGroundUnitDistance += pGroundUnitToPickupRegion->m_fDistance;
		}
		pGroundUnitPredefinedStep->ClearPreTransportMovementPath();
		while( pCurrentPathwayItem )
		{
			if( iLandPickupRegion == 0 )
			{
				if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
					pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					iLandPickupRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
			}

			//pGroundUnitPredefinedStep->GetPreTransportMovementPath()->AddHead( pCurrentPathwayItem->m_pPathfinderPoint );
			pGroundUnitPredefinedStep->AddToPreTransportMovementPathHead( pCurrentPathwayItem->m_pPathfinderPoint );

			CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			if( pAIRegion )
			{
				pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->AddTail( pAIRegion );
			}
			CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			if( pAIRoad )
			{
				CEOSAIRegion2* pLastAIRegion = NULL;
				if( pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->IsEmpty() == false )
				{
					pLastAIRegion = pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->GetTail();
				}

				if( pAIRoad->GetAIRegion1() != pLastAIRegion )
				{
					pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->AddTail( pAIRoad->GetAIRegion1() );
				}
				if( pAIRoad->GetAIRegion2() != pLastAIRegion )
				{
					pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->AddTail( pAIRoad->GetAIRegion2() );
				}
			}

			//ASSERT( pAIRegion );
		//	pGroundUnitResultStep->m_AIRegionMovementPath.AddTail( pAIRegion );
			pCurrentPathwayItem = pGroundUnitToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}
		pCurrentPathwayItem = pTransportToPickupRegion;
		while( pCurrentPathwayItem )
		{
			//pResultStep->m_AIRegionMovementPath.AddHead( pAIRegion );
			TransportToPickupPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTransportToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}

		pTransportPickupPredefinedStep->SetRealTransport_PickupWaterAIRegion( iBestPickupWaterRegion );
		pTransportPickupPredefinedStep->SetRealTransport_PickupGroundAIRegion( iLandPickupRegion );
		//pTransportPickupPredefinedStep->SetRealTransport_PickupTime( fBestTime );
		pGroundUnitPredefinedStep->SetRealTransport_PickupWaterAIRegion( iBestPickupWaterRegion );
		pGroundUnitPredefinedStep->SetRealTransport_PickupGroundAIRegion( iLandPickupRegion );
		//pGroundUnitPredefinedStep->SetRealTransport_PickupTime( fBestTime );
		ASSERT( iLandPickupRegion > 0 );

		CEOSAILocation WaterLocation, LandLocation;
		CEOSAIUnitPathwayFinder::FindPickupDropoffLocations( 
			pTransportPickupPredefinedStep->GetRealTransport_PickupWaterAIRegion(),
			pTransportPickupPredefinedStep->GetRealTransport_PickupGroundAIRegion(),
			&WaterLocation, &LandLocation );
		pTransportPickupPredefinedStep->SetEndLocation( WaterLocation );

		//pGroundUnitPredefinedStep->GetPreTransportMovementPath()->AddTail( pCurrentPathwayItem->m_pPathfinderPoint );

		// Update the Transport's Pickup StepTime
		//pTransportPickupPredefinedStep->

		//fGroundUnitMovementTime += fGroundUnitDistance / (pGroundUnitTemplate->GetMovementRate()+0.1f);

		pTransportPickupPredefinedStep->SetEndLocation( WaterLocation );
		if( pTransportPickupPredefinedStep->GetResultStep() )
		{
			pTransportPickupPredefinedStep->GetResultStep()->m_EndLocation = WaterLocation;
		}

		return true;
	}

	// Calculate pTransportDropoffStep->StartLocation => this->EndLocation + Next Transport Step Start
	if( pTransportDropoffPredefinedStep )
	{
		if( pTransport &&
			//pTransport->GetServerUnit() == NULL &&
			pTransport->GetAIUnitTemplate()->GetInternalName() == _T("Transport (Class 2)") &&
			pTransport->GetBuiltByCity() &&
			pTransport->GetBuiltByCity()->GetObjectId() == 3 )
		{
			int g=0;
		}
		/*
		if( pTransport &&
			pTransport->GetServerUnit() &&
			pTransport->GetServerUnit()->GetObjectId() == 149 )
		{
			int h=0;
		}
		*/

		//CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pTransportDropoffStep->GetNextStep();
		/*
		CEOSAILocation TransportStartLocation = pTransportDropoffStep->GetStartLocation();
		CEOSAILocation TransporteeEndLocation = pGroundUnitResultStep->m_EndLocation;
		CEOSAILocation TransportNextEndLocation; // Either the Transport's Next End Location, or Transportee's Target Location

		if( pTransportDropoffStepNext )
		{
			if( pTransportDropoffStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetImaginaryTransportPickupLocation();
			}
			else if( pTransportDropoffStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetImaginaryTransportDropoffLocation();
			}
			else
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetEndLocation();
			}
		}
		else
		{
			// The transport has no 'next' step - just use the Transportee's End location
			TransportNextEndLocation = pGroundUnitResultStep->m_EndLocation;
		}
		*/

		// How do I calculate this?
		//   Transportee (ThisStart / PrevEndLocation).InvokeDirectAIRegionMapToCoasts();
		//   Transport (ThisStart / PrevEndLocation).InvokeDirectAIRegionMap();
		//   If the next step is a pickup or dropoff:
		//     Other Transportee (Start/End).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   -----If the next step is the corresponding dropoff:
		//   -----  Transportee (ThisEnd).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   If the next step isn't a corresponding dropoff:
		//     Transport.InvokeDirectAIRegionMap();
		// Find the best AIRegion Pickup using these three maps

		// Transport -> Dropoff
		CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepPrev = pTransportDropoffPredefinedStep->GetPrevStep();
		//CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pTransportDropoffPredefinedStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportToDropoffMap = NULL;
		if( pTransportDropoffStepPrev ){ pTransportToDropoffMap = pTransportDropoffStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
		else{                            pTransportToDropoffMap = pTransport->InvokeDirectAIRegionMapToEverywhere(); }
		// DEBUG
		{
			long iNumberOfValidRegions = 0;
			long iNumberOfRegions = pTransportToDropoffMap->GetRegionManager()->GetNumberOfGenericRegions();
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iRegion );
				if( pTargetToDropoffRegion == NULL ) continue;
				//if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;
				iNumberOfValidRegions++;
			}
			ASSERT( iNumberOfValidRegions > 0 );
		}
		// DEBUG

		// GroundUnit (End) -> Everywhere (helps determine dropoff point)
		CEOSAIRegionPathwayMap* pTargetToCoastsMap = pGroundUnitPredefinedStep->InvokeEndLocationToEverywhereDirectMapPlusCoasts();
		// DEBUG
		{
			long iNumberOfValidRegions = 0;
			long iNumberOfRegions = pTargetToCoastsMap->GetRegionManager()->GetNumberOfGenericRegions();
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iRegion );
				if( pTargetToDropoffRegion == NULL ) continue;
				if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;
				iNumberOfValidRegions++;
			}
			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( iNumberOfValidRegions > 0 );
			}
		}
		// DEBUG

		// Next Transport Location to Everywhere
		CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pTransportDropoffPredefinedStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportEndToEverywhereMap = NULL;
		if( pTransportDropoffStepNext )
		{
			pTransportEndToEverywhereMap = pTransportDropoffStepNext->InvokeEndLocationToEverywhereDirectMap();
		}

		// Find the best dropoff, where "best" = earliest [time to next location]
		float fTransportMovementRate = m_pAIUnitTemplate->GetMovementRate();
		float fWeightedBestTime = 1000000.0f;
		//float fWeightedBestTime_GroundUnitDropoffToTargetTime = 1000000.0f;
		long  iBestDropoffWaterRegion = 0;
		long iNumberOfRegions = pTargetToCoastsMap->GetRegionManager()->GetNumberOfGenericRegions();
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iRegion );
			if( pTargetToDropoffRegion == NULL ) continue;
			CEOSAIRegionPathwayItem* pTransportToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iRegion );
			if( pTransportToDropoffRegion == NULL ) continue;

			float fTransportToNextStepTime = 0.0f;
			CEOSAIRegionPathwayItem* pTransportEndToEverywhereRegion = NULL;
			if( pTransportEndToEverywhereMap )
			{
				pTransportEndToEverywhereRegion = pTransportEndToEverywhereMap->GetRegionResult( iRegion );
				//if( pTransportEndToEverywhereRegion == NULL ) continue;
				//fTransportToNextStepTime = pTransportEndToEverywhereRegion->m_fTime;
				if( pTransportEndToEverywhereRegion )
				{
					fTransportToNextStepTime = pTransportEndToEverywhereRegion->m_fTime;
				}
			}

			// Transports can't enter other player's cities
			//if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;
			CEOSAIRegion2* pTargetToDropoffAIRegion = dynamic_cast< CEOSAIRegion2* >( pTargetToDropoffRegion->m_pPathfinderPoint );
			CEOSAIRegion2* pTransportDropoffAIRegion = dynamic_cast< CEOSAIRegion2* >( pTransportToDropoffRegion->m_pPathfinderPoint );
			ASSERT( pTargetToDropoffAIRegion );
			ASSERT( pTransportDropoffAIRegion );
			if( pTargetToDropoffAIRegion->IsCity() && 
				pTargetToDropoffAIRegion->GetEOSAICity()->GetOwner() != iAIPlayer )
			{
				continue;
			}
			CEOSAIRegionPathwayItem* pPrevTargetToDropoffRegion = pTargetToDropoffRegion->GetPreviousPathwayItem();
			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( pPrevTargetToDropoffRegion );
			}
			if( pPrevTargetToDropoffRegion )
			{
				CEOSAIRegion2* pPrevTargetToDropoffAIRegion = dynamic_cast< CEOSAIRegion2* >( pPrevTargetToDropoffRegion->m_pPathfinderPoint );
				if( pPrevTargetToDropoffAIRegion &&
					pPrevTargetToDropoffAIRegion->IsCity() &&
					pPrevTargetToDropoffAIRegion->GetEOSAICity()->GetOwner() != iAIPlayer )
				{
					continue;
				}
			}
			if( pTransportDropoffAIRegion->IsCity() && 
				pTransportDropoffAIRegion->GetEOSAICity()->GetOwner() != iAIPlayer )
			{
				continue;
			}

			//
			float fGroundUnitDropoffToTargetTime = pTargetToDropoffRegion->m_fTime;
			// Weigh the transport to dropoff time more heavily so we avoid having a vulnerable transport
			//   for a length of time.
			//float fTransportToDropoffTime = (pTransportToDropoffRegion->m_fDistance/fTransportMovementRate);
			float fRealTransportToDropoffTime = (pTransportToDropoffRegion->m_fDistance/fTransportMovementRate);
			float fWeightedTransportToDropoffTime = 2.0f*(pTransportToDropoffRegion->m_fDistance/fTransportMovementRate);
			//float fTotalTime = fTransportToDropoffTime + fGroundUnitDropoffToTargetTime + fTransportToNextStepTime;
			float fWeightedTotalTime = fWeightedTransportToDropoffTime + fGroundUnitDropoffToTargetTime + fTransportToNextStepTime;
			/*
			if( fGroundUnitToPickupTime < 0.5f && fTransportToPickupTime < 0.5f )
			{
				// If GroundUnit and Transport are both close to a pickup location, use it
				// tweak the time to favor this option
				fTotalTime -= 1.0f; 
			}
			*/
			if( fWeightedTotalTime < fWeightedBestTime )
			{
				fWeightedBestTime = fWeightedTotalTime;
				//fWeightedBestTime_GroundUnitDropoffToTargetTime = fGroundUnitDropoffToTargetTime;
				iBestDropoffWaterRegion = iRegion;
			}
			int g=0;
		}
		if( EOSAISettings::m_bAssertWhenAIPathwayFails )
		{
			ASSERT( iBestDropoffWaterRegion > 0 );
		}
		if( iBestDropoffWaterRegion == 0 )
		{
			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}
			return false; // Couldn't find a dropoff point
		}

		//
		long iLandDropoffRegion = 0;

		if( pGroundUnitPredefinedStep->GetAIUnitPathwayFinder()->GetAIUnitActor()->GetObjectId() == 14 )
		{
			int h=0;
		}

		// Drop the pathway information into a path and then walk it backwards
		CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iBestDropoffWaterRegion );
		CEOSAIRegionPathwayItem* pTransportToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iBestDropoffWaterRegion );
		//
		pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->RemoveAll();
		CList< long > GroundUnitPath;
		CEOSAIRegionPathwayItem* pCurrentPathwayItem = pTargetToDropoffRegion;
		//pGroundUnitPredefinedStep->GetPostTransportMovementPath()->RemoveAll();
		pGroundUnitPredefinedStep->ClearPostTransportMovementPath();
		while( pCurrentPathwayItem )
		{
			if( iLandDropoffRegion == 0 )
			{
				if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
					pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					iLandDropoffRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
			}

			//pGroundUnitPredefinedStep->GetPostTransportMovementPath()->AddTail( pCurrentPathwayItem->m_pPathfinderPoint );
			pGroundUnitPredefinedStep->AddToPostTransportMovementPathTail( pCurrentPathwayItem->m_pPathfinderPoint );

			CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			if( pAIRegion )
			{
				pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->AddTail( pAIRegion );
			}
			CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			if( pAIRoad )
			{
				CEOSAIRegion2* pLastAIRegion = NULL;
				if( pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->IsEmpty() == false )
				{
					pLastAIRegion = pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->GetTail();
				}

				if( pAIRoad->GetAIRegion1() != pLastAIRegion )
				{
					pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->AddTail( pAIRoad->GetAIRegion1() );
				}
				if( pAIRoad->GetAIRegion2() != pLastAIRegion )
				{
					pGroundUnitPredefinedStep->GetAIRegionAllowedMovementPath()->AddTail( pAIRoad->GetAIRegion2() );
				}
			}
		//	pGroundUnitResultStep->m_AIRegionMovementPath.AddTail( pAIRegion );

			//GroundUnitPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTargetToCoastsMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}
		CList< long > TransportPath;
		pCurrentPathwayItem = pTransportToDropoffRegion;
		while( pCurrentPathwayItem )
		{
			TransportPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTransportToDropoffMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}

		pTransportDropoffPredefinedStep->SetRealTransport_DropoffWaterAIRegion( iBestDropoffWaterRegion );
		pTransportDropoffPredefinedStep->SetRealTransport_DropoffGroundAIRegion( iLandDropoffRegion );
		//pTransportDropoffPredefinedStep->SetRealTransport_DropoffTime( fBestTime );
		pGroundUnitPredefinedStep->SetRealTransport_DropoffWaterAIRegion( iBestDropoffWaterRegion );
		pGroundUnitPredefinedStep->SetRealTransport_DropoffGroundAIRegion( iLandDropoffRegion );
		//pGroundUnitPredefinedStep->SetRealTransport_DropoffTime( fBestTime );

		CEOSAILocation WaterLocation, LandLocation;
		CEOSAIUnitPathwayFinder::FindPickupDropoffLocations( 
			pTransportDropoffPredefinedStep->GetRealTransport_DropoffWaterAIRegion(),
			pTransportDropoffPredefinedStep->GetRealTransport_DropoffGroundAIRegion(),
			&WaterLocation, &LandLocation );
		pTransportDropoffPredefinedStep->SetEndLocation( WaterLocation );

		// DEBUG
		if( EOSAISettings::m_bAssertWhenAIPathwayFails )
		{
			CEOSAIRegionPathwayItem* pCurrentItem1 = pTransportToDropoffMap->GetRegionResult( pTransportDropoffPredefinedStep->GetStartLocation() );
			ASSERT( pCurrentItem1 );
			CEOSAIRegionPathwayMap* pEndLocationToEverywhere = pTransportDropoffPredefinedStep->InvokeEndLocationToEverywhereDirectMap();
			CEOSAIRegionPathwayItem* pCurrentItem2 = pEndLocationToEverywhere->GetRegionResult( pTransportDropoffPredefinedStep->GetStartLocation() );
			ASSERT( pCurrentItem2 );
		}
		// DEBUG

		// Update the Transport StepTime
		//pPredefinedTransportDropoffStep->m_fSt

		// Update the GroundUnit StepTime
//		fGroundUnitMovementTime += fBestTime; // fBestTime = Transport->Dropoff + GroundUnit->Target
//		pGroundUnitResultStep->m_fStepTime = fGroundUnitMovementTime;
//		pGroundUnitResultStep->m_fDropoffTimeToTargetTime = fBestTime_GroundUnitDropoffToTargetTime;

		pTransportDropoffPredefinedStep->SetEndLocation( WaterLocation );
		if( pTransportDropoffPredefinedStep->GetResultStep() )
		{
			pTransportDropoffPredefinedStep->GetResultStep()->m_EndLocation = WaterLocation;
		}

		/*
		bool bMovementOnLand = true;
		POSITION pos = Path.GetHeadPosition();
		while( pos )
		{
			long iRegion = Path.GetNext( pos );
			CEOSAIRegionPathwayItem* pPathItem = TargetToEverywhereMap.GetRegionResult( iRegion );

			if( pPathItem->m_pPathfinderPoint->IsWater() ){ bMovementOnLand = false; }
			if( bMovementOnLand )
			{
				iLandDropoffRegion = pPathItem->m_pPathfinderPoint->GetId();
				ASSERT( pPathItem->m_pPathfinderPoint->IsLand() || pPathItem->m_pPathfinderPoint->IsCity() );
				//pPathResults->m_GroundUnitFromDropoffToTargetPath.AddTail( iRegion );
				GroundUnitFromDropoffToTargetPath.AddTail( iRegion );
			}
			else
			{
				if( iWaterDropoffRegion == 0 )
				{
					iWaterDropoffRegion = pPathItem->m_pPathfinderPoint->GetId();
				}
				ASSERT( pPathItem->m_pPathfinderPoint->IsWater() || pPathItem->m_pPathfinderPoint->IsCity() );
				//pPathResults->m_TransportPickupToDropoffPath.AddTail( iRegion );
				TransportPickupToDropoffPath.AddTail( iRegion );
			}
		}
		*/

		return true;
	}
	if( EOSAISettings::m_bAssertWhenAIPathwayFails )
	{
		ASSERT( false );
	}
	return false;
}
//#endif DO_NOT_COMPILE_OLD_FUNCTION


/*
bool CEOSAISeaUnitPathwayFinder::UpdateResultStepWithTransportAssistedPath( EOSAI::UnitPathwayResultStep* pGroundUnitResultStep )
{
	ASSERT( pGroundUnitResultStep->m_pPredefinedStep->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsGroundUnit() );

	CEOSAIUnit* pTransport = this->GetAIUnitActor();
	ASSERT( pTransport->IsTransport() );

	float fGroundUnitMovementTime = 0.0f;
	CEOSAIUnitPathwayPredefinedStep* pGroundUnitPredefinedStep = pGroundUnitResultStep->m_pPredefinedStep;
	CEOSAIUnit* pGroundUnit = pGroundUnitResultStep->m_pPredefinedStep->GetAIUnitPathwayFinder()->GetAIUnitActor();
	CUnitTemplate* pGroundUnitTemplate = pGroundUnitResultStep->m_pPredefinedStep->GetAIUnitPathwayFinder()->GetUnitTemplate();

//	pGroundUnitResultStep->m_AIRegionMovementPath.RemoveAll();

	// Update the path
	// Lookup the Transport's pickup/dropoff steps
	CEOSAIUnitPathwayPredefinedStep* pPredefinedTransportPickupStep = GetTransportPickupStep( pGroundUnitPredefinedStep );
	CEOSAIUnitPathwayPredefinedStep* pPredefinedTransportDropoffStep = GetTransportDropoffStep( pGroundUnitPredefinedStep );

	// Calculate pTransportPickupStep->StartLocation + this->StartLocation => Next Transport Step End
	if( pPredefinedTransportPickupStep )
	{
		// How do I calculate this?
		//   Transportee (ThisStart / PrevEndLocation).InvokeDirectAIRegionMapToCoasts();
		//   Transport (ThisStart / PrevEndLocation).InvokeDirectAIRegionMap();
		//   If the next step is a pickup or dropoff:
		//     Other Transportee (Start/End).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   -----If the next step is the corresponding dropoff:
		//   -----  Transportee (ThisEnd).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   If the next step isn't a corresponding dropoff:
		//     Transport.InvokeDirectAIRegionMap();
		// Find the best AIRegion Pickup using these three maps

		// Transport -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pTransportPickupStepPrev = pPredefinedTransportPickupStep->GetPrevStep();
		CEOSAIUnitPathwayPredefinedStep* pTransportPickupStepNext = pPredefinedTransportPickupStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportToPickupMap = NULL;
		if( pTransportPickupStepPrev ){ pTransportToPickupMap = pTransportPickupStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
		else{                           pTransportToPickupMap = pTransport->InvokeDirectAIRegionMapToEverywhere(); }

		// GroundUnit -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pGroundUnitPickupStepPrev = pGroundUnitPredefinedStep->GetPrevStep();
		CEOSAIRegionPathwayMap* pGroundUnitToPickupMap = NULL;
		if( pGroundUnitPickupStepPrev ){ pGroundUnitToPickupMap = pGroundUnitPickupStepPrev->InvokeEndLocationToEverywhereDirectMapPlusCoasts(); }
		else{                            pGroundUnitToPickupMap = pGroundUnit->InvokeDirectAIRegionMapToCoasts(); }//>InvokeDirectAIRegionMapToEverywhere(); }

		// Next Transport Location -> Pickup
		CEOSAIUnitPathwayPredefinedStep* pTransporteePickupStepNext = pPredefinedTransportPickupStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportNextLocationToEverywhereMap = NULL;
		if( pTransportPickupStepNext )
		{
			// DEBUG
			#ifdef _DEBUG
			CEOSAILocation NextLocation = pTransportPickupStepNext->GetEndLocation();
			#endif _DEBUG
			// DEBUG
			pTransportNextLocationToEverywhereMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			/-*
			if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				//CEOSAIUnitPathwayPredefinedStep*  GetTransporteeStep(){ return m_pTransporteeStep; }
				CEOSAIUnitPathwayPredefinedStep*  pTransporteeStep = pTransportPickupStepNext->GetTransporteeStep();
				if( pTransporteePickupStepPrev ){ pGroundUnitToPickup = pTransporteePickupStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
				else{                             pGroundUnitToPickup = pTransport->InvokeDirectAIRegionMapToEverywhere(); }
				pTransportToNextLocationMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			}
			else if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
			{
				EndLocation = pTransportPickupStepNext->GetImaginaryTransportDropoffLocation();
			}
			else
			{
				EndLocation = pTransportPickupStepNext->GetEndLocation();
			}

			if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				pTransportToNextLocationMap = pTransportPickupStepNext->InvokeEndLocationToEverywhereDirectMap();
			}
			*-/
		}
		else // The transport has no more PredefinedSteps after this one.  Do what the GroundUnit wants
		{
			// This shouldn't happen
			ASSERT( false );
			pTransportNextLocationToEverywhereMap = pGroundUnitPredefinedStep->InvokeEndLocationToEverywhereGroundUnitAndSeaTransportMap();
		}

		float fGroundUnitStepStartTime = pGroundUnitPredefinedStep->GetStartTime();
		float fTransportStepStartTime = pPredefinedTransportPickupStep->GetStartTime();

		// If the transport and GroundUnit are near each other at the beginning of this step,
		//   then just do a direct pickup
		//float fTimeGap = fabs( fGroundUnitStepStartTime - fTransportStepStartTime );
		//float fDistance = pWorldBuildDesc->GetPixelDistance( GroundUnitStepStartLocation, TransportStepStartLocation );


		// Find the best pickup, where "best" = earliest [time to next location]
		float fTransportMovementRate = m_pUnitTemplate->GetMovementRate();
		float fBestTime = 1000000.0f;
		long  iBestPickupWaterRegion = 0;
		long iNumberOfRegions = pGroundUnitToPickupMap->GetRegionManager()->GetNumberOfGenericRegions();
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iRegion );
			if( pGroundUnitToPickupRegion == NULL ) continue;
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iRegion );
			if( pTransportToPickupRegion == NULL ) continue;
			CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTransportNextLocationToEverywhereMap->GetRegionResult( iRegion );
			if( pTargetToPickupRegion == NULL ) continue;

			//
			float fGroundUnitToPickupTime = fGroundUnitStepStartTime + pGroundUnitToPickupRegion->m_fTime;
			float fTransportToPickupTime = fTransportStepStartTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
			float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
			float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
			if( fGroundUnitToPickupTime < 0.7f && fTransportToPickupTime < 0.7f )
			{
				// If GroundUnit and Transport are both close to a pickup location, use it
				// tweak the time to favor this option
				fTotalTime -= 2.0f; 
			}
			if( fTotalTime < fBestTime )
			{
				fBestTime = fTotalTime;
				iBestPickupWaterRegion = iRegion;
			}
			int g=0;
		}
		ASSERT( iBestPickupWaterRegion > 0 );
		if( iBestPickupWaterRegion == 0 ) return false; // Couldn't find a pickup point

		//
		// Record the values
		//
		CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToPickupMap->GetRegionResult( iBestPickupWaterRegion );
		CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportToPickupMap->GetRegionResult( iBestPickupWaterRegion );
		//CEOSAIRegionPathwayItem* pTargetToPickupRegion = TargetToEverywhereMap.GetRegionResult( iBestPickupWaterRegion );

		//
		CList< long >  GroundUnitToPickupPath;
		CList< long >  TransportToPickupPath;
		float fGroundUnitDistance = 0.0f;
		long iLandPickupRegion = 0;
		CEOSAIRegionPathwayItem* pCurrentPathwayItem = pGroundUnitToPickupRegion;
		if( pGroundUnitToPickupRegion )
		{
			fGroundUnitDistance += pGroundUnitToPickupRegion->m_fDistance;
		}
		while( pCurrentPathwayItem )
		{
			if( iLandPickupRegion == 0 )
			{
				if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
					pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					iLandPickupRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
			}
			CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			ASSERT( pAIRegion );
			//pGroundUnitPredefinedStep->GetAIRegionMovementPath()->AddTail( pAIRegion );
			//pGroundUnitResultStep->m_AIRegionMovementPath.AddTail( pAIRegion );
			//GroundUnitToPickupPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pGroundUnitToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}
		pCurrentPathwayItem = pTransportToPickupRegion;
		while( pCurrentPathwayItem )
		{
			//pResultStep->m_AIRegionMovementPath.AddHead( pAIRegion );
			TransportToPickupPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTransportToPickupMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}

		pPredefinedTransportPickupStep->SetRealTransport_PickupWaterAIRegion( iBestPickupWaterRegion );
		pPredefinedTransportPickupStep->SetRealTransport_PickupGroundAIRegion( iLandPickupRegion );
		ASSERT( iLandPickupRegion > 0 );

		CEOSAILocation WaterLocation, LandLocation;
		CEOSAIUnitPathwayFinder::FindPickupDropoffLocations( 
			pPredefinedTransportPickupStep->GetRealTransport_PickupWaterAIRegion(),
			pPredefinedTransportPickupStep->GetRealTransport_PickupGroundAIRegion(),
			&WaterLocation, &LandLocation );
		pPredefinedTransportPickupStep->SetEndLocation( WaterLocation );

		// Update the Transport's Pickup StepTime
		//pPredefinedTransportPickupStep->

		fGroundUnitMovementTime += fGroundUnitDistance / (pGroundUnitTemplate->GetMovementRate()+0.1f);

		if( pPredefinedTransportPickupStep->GetResultStep() )
		{
			pPredefinedTransportPickupStep->GetResultStep()->m_EndLocation = WaterLocation;
		}

		/-*
		CEOSAILocation TransportStartLocation = pTransportPickupStep->GetStartLocation();
		CEOSAILocation TransporteeStartLocation = pGroundUnitResultStep->m_StartLocation;
		CEOSAILocation EndLocation; // Either the Transport's Next End Location, or Transportee's Target Location
		if( pTransportPickupStepNext )
		{
			if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				EndLocation = pTransportPickupStepNext->GetImaginaryTransportPickupLocation();
			}
			else if( pTransportPickupStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
			{
				EndLocation = pTransportPickupStepNext->GetImaginaryTransportDropoffLocation();
			}
			else
			{
				EndLocation = pTransportPickupStepNext->GetEndLocation();
			}
		}
		else
		{
			// The transport has no 'next' step - just use the Transportee's End location
			EndLocation = pGroundUnitResultStep->m_EndLocation;
		}
		*-/

	}

	// Calculate pTransportDropoffStep->StartLocation => this->EndLocation + Next Transport Step Start
	if( pPredefinedTransportDropoffStep )
	{
		//CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pTransportDropoffStep->GetNextStep();
		/-*
		CEOSAILocation TransportStartLocation = pTransportDropoffStep->GetStartLocation();
		CEOSAILocation TransporteeEndLocation = pGroundUnitResultStep->m_EndLocation;
		CEOSAILocation TransportNextEndLocation; // Either the Transport's Next End Location, or Transportee's Target Location

		if( pTransportDropoffStepNext )
		{
			if( pTransportDropoffStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget )
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetImaginaryTransportPickupLocation();
			}
			else if( pTransportDropoffStepNext->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetImaginaryTransportDropoffLocation();
			}
			else
			{
				TransportNextEndLocation = pTransportDropoffStepNext->GetEndLocation();
			}
		}
		else
		{
			// The transport has no 'next' step - just use the Transportee's End location
			TransportNextEndLocation = pGroundUnitResultStep->m_EndLocation;
		}
		*-/

		// How do I calculate this?
		//   Transportee (ThisStart / PrevEndLocation).InvokeDirectAIRegionMapToCoasts();
		//   Transport (ThisStart / PrevEndLocation).InvokeDirectAIRegionMap();
		//   If the next step is a pickup or dropoff:
		//     Other Transportee (Start/End).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   -----If the next step is the corresponding dropoff:
		//   -----  Transportee (ThisEnd).InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();
		//   If the next step isn't a corresponding dropoff:
		//     Transport.InvokeDirectAIRegionMap();
		// Find the best AIRegion Pickup using these three maps

		// Transport -> Dropoff
		CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepPrev = pPredefinedTransportDropoffStep->GetPrevStep();
		//CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pPredefinedTransportDropoffStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportToDropoffMap = NULL;
		if( pTransportDropoffStepPrev ){ pTransportToDropoffMap = pTransportDropoffStepPrev->InvokeEndLocationToEverywhereDirectMap(); }
		else{                            pTransportToDropoffMap = pTransport->InvokeDirectAIRegionMapToEverywhere(); }
		// DEBUG
		{
			long iNumberOfValidRegions = 0;
			long iNumberOfRegions = pTransportToDropoffMap->GetRegionManager()->GetNumberOfGenericRegions();
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iRegion );
				if( pTargetToDropoffRegion == NULL ) continue;
				//if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;
				iNumberOfValidRegions++;
			}
			ASSERT( iNumberOfValidRegions > 0 );
		}
		// DEBUG

		// GroundUnit (End) -> Everywhere (helps determine dropoff point)
		CEOSAIRegionPathwayMap* pTargetToCoastsMap = pGroundUnitPredefinedStep->InvokeEndLocationToEverywhereDirectMapPlusCoasts();
		// DEBUG
		{
			long iNumberOfValidRegions = 0;
			long iNumberOfRegions = pTargetToCoastsMap->GetRegionManager()->GetNumberOfGenericRegions();
			for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
			{
				CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iRegion );
				if( pTargetToDropoffRegion == NULL ) continue;
				if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;
				iNumberOfValidRegions++;
			}
			ASSERT( iNumberOfValidRegions > 0 );
		}
		// DEBUG

		// Next Transport Location to Everywhere
		CEOSAIUnitPathwayPredefinedStep* pTransportDropoffStepNext = pPredefinedTransportDropoffStep->GetNextStep();
		CEOSAIRegionPathwayMap* pTransportEndToEverywhereMap = NULL;
		if( pTransportDropoffStepNext )
		{
			pTransportEndToEverywhereMap = pTransportDropoffStepNext->InvokeEndLocationToEverywhereDirectMap();
		}

		// Find the best dropoff, where "best" = earliest [time to next location]
		float fTransportMovementRate = m_pUnitTemplate->GetMovementRate();
		float fBestTime = 1000000.0f;
		float fBestTime_GroundUnitDropoffToTargetTime = 1000000.0f;
		long  iBestDropoffWaterRegion = 0;
		long iNumberOfRegions = pTargetToCoastsMap->GetRegionManager()->GetNumberOfGenericRegions();
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iRegion );
			if( pTargetToDropoffRegion == NULL ) continue;
			CEOSAIRegionPathwayItem* pTransportToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iRegion );
			if( pTransportToDropoffRegion == NULL ) continue;

			float fTransportToNextStepTime = 0.0f;
			CEOSAIRegionPathwayItem* pTransportEndToEverywhereRegion = NULL;
			if( pTransportEndToEverywhereMap )
			{
				pTransportEndToEverywhereRegion = pTransportEndToEverywhereMap->GetRegionResult( iRegion );
				//if( pTransportEndToEverywhereRegion == NULL ) continue;
				//fTransportToNextStepTime = pTransportEndToEverywhereRegion->m_fTime;
				if( pTransportEndToEverywhereRegion )
				{
					fTransportToNextStepTime = pTransportEndToEverywhereRegion->m_fTime;
				}
			}

			// Transports can't enter other player's cities
			if( pTargetToDropoffRegion->m_pPathfinderPoint->IsCity() ) continue;

			//
			float fGroundUnitDropoffToTargetTime = pTargetToDropoffRegion->m_fTime;
			float fTransportToDropoffTime = (pTransportToDropoffRegion->m_fDistance/fTransportMovementRate);
			float fTotalTime = fTransportToDropoffTime + fGroundUnitDropoffToTargetTime + fTransportToNextStepTime;
			/-*
			if( fGroundUnitToPickupTime < 0.5f && fTransportToPickupTime < 0.5f )
			{
				// If GroundUnit and Transport are both close to a pickup location, use it
				// tweak the time to favor this option
				fTotalTime -= 1.0f; 
			}
			*-/
			if( fTotalTime < fBestTime )
			{
				fBestTime = fTotalTime;
				fBestTime_GroundUnitDropoffToTargetTime = fGroundUnitDropoffToTargetTime;
				iBestDropoffWaterRegion = iRegion;
			}
			int g=0;
		}
		ASSERT( iBestDropoffWaterRegion > 0 );
		if( iBestDropoffWaterRegion == 0 ) return false; // Couldn't find a dropoff point

		//
		long iLandDropoffRegion = 0;

		// Drop the pathway information into a path and then walk it backwards
		CEOSAIRegionPathwayItem* pTargetToDropoffRegion = pTargetToCoastsMap->GetRegionResult( iBestDropoffWaterRegion );
		CEOSAIRegionPathwayItem* pTransportToDropoffRegion = pTransportToDropoffMap->GetRegionResult( iBestDropoffWaterRegion );
		//
		CList< long > GroundUnitPath;
		CEOSAIRegionPathwayItem* pCurrentPathwayItem = pTargetToDropoffRegion;
		while( pCurrentPathwayItem )
		{
			if( iLandDropoffRegion == 0 )
			{
				if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
					pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					iLandDropoffRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
			}
			CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pCurrentPathwayItem->m_pPathfinderPoint );
			ASSERT( pAIRegion );
			//pGroundUnitPredefinedStep->GetAIRegionMovementPath()->AddTail( pAIRegion );
			//pGroundUnitResultStep->m_AIRegionMovementPath.AddTail( pAIRegion );

			//GroundUnitPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTargetToCoastsMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}
		CList< long > TransportPath;
		pCurrentPathwayItem = pTransportToDropoffRegion;
		while( pCurrentPathwayItem )
		{
			TransportPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
			pCurrentPathwayItem = pTransportToDropoffMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
		}

		pPredefinedTransportDropoffStep->SetRealTransport_DropoffWaterAIRegion( iBestDropoffWaterRegion );
		pPredefinedTransportDropoffStep->SetRealTransport_DropoffGroundAIRegion( iLandDropoffRegion );

		CEOSAILocation WaterLocation, LandLocation;
		CEOSAIUnitPathwayFinder::FindPickupDropoffLocations( 
			pPredefinedTransportDropoffStep->GetRealTransport_DropoffWaterAIRegion(),
			pPredefinedTransportDropoffStep->GetRealTransport_DropoffGroundAIRegion(),
			&WaterLocation, &LandLocation );
		pPredefinedTransportDropoffStep->SetEndLocation( WaterLocation );

		// DEBUG
		CEOSAIRegionPathwayItem* pCurrentItem1 = pTransportToDropoffMap->GetRegionResult( pPredefinedTransportDropoffStep->GetStartLocation() );
		ASSERT( pCurrentItem1 );
		CEOSAIRegionPathwayMap* pEndLocationToEverywhere = pPredefinedTransportDropoffStep->InvokeEndLocationToEverywhereDirectMap();
		CEOSAIRegionPathwayItem* pCurrentItem2 = pEndLocationToEverywhere->GetRegionResult( pPredefinedTransportDropoffStep->GetStartLocation() );
		ASSERT( pCurrentItem2 );
		// DEBUG

		// Update the Transport StepTime
		//pPredefinedTransportDropoffStep->m_fSt

		// Update the GroundUnit StepTime
		fGroundUnitMovementTime += fBestTime; // fBestTime = Transport->Dropoff + GroundUnit->Target
		pGroundUnitResultStep->m_fStepTime = fGroundUnitMovementTime;
		pGroundUnitResultStep->m_fDropoffTimeToTargetTime = fBestTime_GroundUnitDropoffToTargetTime;

		if( pPredefinedTransportDropoffStep->GetResultStep() )
		{
			pPredefinedTransportDropoffStep->GetResultStep()->m_EndLocation = WaterLocation;
		}

		return true;
	}
	return false;
}
*/

void  CEOSAISeaUnitPathwayFinder::PreprocessPath()
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
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );

		pPredefinedStep->SetStartLocation( CurrentLocation );
		CEOSAIRegionPathwayMap* pEndLocationToEverywhereDirectMap = pPredefinedStep->InvokeEndLocationToEverywhereDirectMap();
		CEOSAIRegionPathwayItem* pStartPointPathwayItem = pEndLocationToEverywhereDirectMap->GetRegionResult( pPredefinedStep->GetStartLocation() );

		pPredefinedStep->SetDirectPathDistance( 1000000.0f );
		pPredefinedStep->SetCrowsFlightDistance( g_pWorldDistanceTool->GetDistance( pPredefinedStep->GetStartLocation(), pPredefinedStep->GetEndLocation() ) );
		if( pStartPointPathwayItem )
		{
			pPredefinedStep->SetDirectPathDistance( pStartPointPathwayItem->m_fDistance );
		}

		CurrentLocation = pPredefinedStep->GetEndLocation();
	}
}

void  CEOSAISeaUnitPathwayFinder::CalculateAIRegionPathFromPredefinedSteps()
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
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		//pPredefinedStep->GetDirectMovementPath()->RemoveAll();
		pPredefinedStep->ClearDirectMovementPath();
		pPredefinedStep->GetAIRegionAllowedMovementPath()->RemoveAll();

		// If this step requires a transport, setup a fake path (that will get fixed later)
		// If this step doesn't require a transport, setup a real path

		CEOSAIRegionPathwayMap* pEndLocationToEverywhereDirectMap = 
			pPredefinedStep->InvokeEndLocationToEverywhereDirectMap();

		// Copy the pathway into the AIRegionPath
		CEOSAIRegionPathwayItem* pCurrentItem = pEndLocationToEverywhereDirectMap->GetRegionResult( pPredefinedStep->GetStartLocation() );

		long iCurrentItem = 1;
		while( pCurrentItem )
		{
			CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( pCurrentItem->m_pPathfinderPoint->m_iId );
			ASSERT( pAIRegion );
			//pPredefinedStep->GetDirectMovementPath()->AddTail( pAIRegion );
			pPredefinedStep->AddToDirectMovementPathTail( pAIRegion );
			pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRegion );
			//pResultStep->m_AIRegionMovementPath.AddHead( pAIRegion );

			if( iCurrentItem == 1 )
			{
				//pResultStep->m_bIsValid = true;
				//pResultStep->m_fDistance = pCurrentItem->m_fDistance;
				//pResultStep->m_fDanger = pCurrentItem->m_fDanger;
				//pResultStep->m_fSighting = pCurrentItem->m_fSightings;
				//pResultStep->m_fStepTime = pCurrentItem->m_fTime; //pCurrentItem->m_fDistance / fBestMovementRate;

				// This is the Start Location.  Grab all the adjacent AIRegions, too.  Allow them.
				CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( pCurrentItem->m_pPathfinderPoint->m_iId );
				POSITION pos2 = pAIRegion->m_AdjacentRegions.GetHeadPosition();
				while( pos2 )
				{
					CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pAIRegion->m_AdjacentRegions.GetNext( pos2 );
					CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pAdjRegionDesc->m_pRegion );
					ASSERT( pAIRegion );
					pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRegion );
					//pResultStep->m_AIRegionMovementPath.AddHead( pAIRegion );
				}
			}
			iCurrentItem++;

			pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
		}
	}
}
/*
void  CEOSAISeaUnitPathwayFinder::CalculateAIRegionPathTiming()
{
	CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->UpdatePathTimingIfNecessary();
	}
}
*/
/*
float CEOSAISeaUnitPathwayFinder::GetTransportArrivalTimeAtDropoffLocation( CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep )
{
	ASSERT( pGroundUnitStep->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsGroundUnit() );
	ASSERT( this->GetUnitTemplate()->IsTransport() );
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		if( pPredefinedStep->IsADropoff() &&
			pPredefinedStep->GetTransporteeStep() == pGroundUnitStep )
		{
			return pPredefinedStep->GetEndTime();
		}
	}
	ASSERT( false );
	return 0.0f;
}
*/
/*
float CEOSAISeaUnitPathwayFinder::GetTransporteeArrivalTimeAtPickupLocation( CEOSAIUnitPathwayPredefinedStep* pGroundUnitsStep )
{
	ASSERT( false );
	//ASSERT( this->GetUnitTemplate()->IsGroundUnit() );
	//ASSERT( pGroundUnitStep->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsTransport() );
	//ASSERT( false );
	return 0.0f;
}
*/
/*
void CEOSAISeaUnitPathwayFinder::CalculateAIRegionPathTiming_UptoDropoffTask( CEOSAIUnitPathwayPredefinedStep* pGroundUnitsStep )
{
	CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->UpdatePathTimingIfNecessary();

		if( pPredefinedStep->IsADropoff() && 
			pPredefinedStep->GetTransporteeStep() == pGroundUnitsStep )
		{
			return;
		}
	}
}
*/
void CEOSAISeaUnitPathwayFinder::CalculateResultPath()
{
	// DEBUG
	CEOSAIUnit* pAIUnit = GetAIUnitActor();
	if( pAIUnit->IsTransport() )
	{
		int g=0;
	}
	// DEBUG

	PreprocessPath();

	//m_bFinalPathWasCalculated = true;
	//m_bFinalPathIsValid = false;
	CEOSAILocation CurrentLocation = m_StartLocation;
	//CEOSAIRegionManager2* pAIRegionManager = m_pAIBrain->GetAIRegionManager();
	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();

	// Clear existing Results
	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }
	m_pResultPath = new EOSAI::UnitPathwayResult( this );
	//m_pResultPath->m_bPathIsValid = true;

	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		CEOSAIRegionPathwayMap* pEndLocationToEverywhere = pPredefinedStep->InvokeEndLocationToEverywhereDirectMap();

		//bool bStepIsValid = false;
		//bool bCalculateThisStep = false;
/*
		//
		CEOSAIRegionManager2* pAIRegionManager = m_pAIBrain->GetAIRegionManager();

		CEOSAIRegionPathwayMap  PathwayResult;
		CEOSAIRegionPathfinder& RegionPathfinder = pAIRegionManager->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		//RegionPathfinder.SetDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetGeoSwitchMultiplier( 400.0f );
		RegionPathfinder.SetMovementRate( m_pUnitTemplate );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsWater( true );
		//RegionPathfinder.SetMaximumGeoSwitches( 0 );
		//RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iUnitOwner );

		RegionPathfinder.SetPlayer( m_iUnitOwner );
		RegionPathfinder.SetForeignRelations( &m_OwnerForeignRelations );

		//RegionPathfinder.SetToPolitePathway( m_iUnitOwner );
		//ApplyStrategyToRegionPathfinder( &RegionPathfinder );
		RegionPathfinder.ApplyStrategy( m_eNeutralsPathwayStategy, m_eEnemiesPathwayStategy, 1.0f );

/-*
		//bool bIgnoreEnemies     = ( m_ePathwayType & CEOSAIUnitPathway::IgnoreEnemies ) > 0;
		//bool bAvoidEnemyContact = ( m_ePathwayType & CEOSAIUnitPathway::AvoidEnemyContact ) > 0;
		//long iCount = 0;
		//if( bIgnoreEnemies ) iCount++;
		//if( bAvoidEnemyContact ) iCount++;
		//ASSERT( iCount <= 1 );
		if( m_ePathwayStategy == IgnoreEveryone )
		{
			//RegionPathfinder.SetEnemyDangerAdjustedMultiplier(   m_iUnitOwner, 0.0f );
			//RegionPathfinder.SetEnemyPresenceAdjustedMultiplier( m_iUnitOwner, 0.0f );
			//RegionPathfinder.SetEnemySightingAdjustedMultiplier( m_iUnitOwner, 0.0f );
		}
		if( m_ePathwayStategy == PoliteToNeutrals_IgnoreEnemies )
		{
			RegionPathfinder.SetNeutralDangerAdjustedMultiplier(   m_iUnitOwner, 0.0f );
			RegionPathfinder.SetNeutralPresenceAdjustedMultiplier( m_iUnitOwner, 400.0f );
			RegionPathfinder.SetNeutralSightingAdjustedMultiplier( m_iUnitOwner, 0.0f );
		}
		if( m_ePathwayStategy == PoliteToNeutrals_AllowSomeEnemyContact )
		{
			RegionPathfinder.SetNeutralDangerAdjustedMultiplier(   m_iUnitOwner, 0.0f );
			RegionPathfinder.SetNeutralPresenceAdjustedMultiplier( m_iUnitOwner, 400.0f );
			RegionPathfinder.SetNeutralSightingAdjustedMultiplier( m_iUnitOwner, 0.0f );
			//
			RegionPathfinder.SetEnemyDangerAdjustedMultiplier(   m_iUnitOwner, 50.0f );
			RegionPathfinder.SetEnemyPresenceAdjustedMultiplier( m_iUnitOwner, 50.0f );
			RegionPathfinder.SetEnemySightingAdjustedMultiplier( m_iUnitOwner, 50.0f );
		}
		if( m_ePathwayStategy == PoliteToNeutrals_AvoidEnemyContact )
		{
			RegionPathfinder.SetNeutralDangerAdjustedMultiplier(   m_iUnitOwner, 0.0f );
			RegionPathfinder.SetNeutralPresenceAdjustedMultiplier( m_iUnitOwner, 400.0f );
			RegionPathfinder.SetNeutralSightingAdjustedMultiplier( m_iUnitOwner, 0.0f );
			//
			RegionPathfinder.SetEnemyDangerAdjustedMultiplier(   m_iUnitOwner, 200.0f );
			RegionPathfinder.SetEnemyPresenceAdjustedMultiplier( m_iUnitOwner, 200.0f );
			RegionPathfinder.SetEnemySightingAdjustedMultiplier( m_iUnitOwner, 200.0f );
		}
		if( m_ePathwayStategy == IgnoreNeutrals_AvoidEnemyContact )
		{
			RegionPathfinder.SetEnemyDangerAdjustedMultiplier(   m_iUnitOwner, 200.0f );
			RegionPathfinder.SetEnemyPresenceAdjustedMultiplier( m_iUnitOwner, 200.0f );
			RegionPathfinder.SetEnemySightingAdjustedMultiplier( m_iUnitOwner, 200.0f );
		}
*-/
		RegionPathfinder.AddStartLocation( CurrentLocation );//pMultiRegion );
		RegionPathfinder.AddEndLocation( pPredefinedStep->m_EndLocation );//pMultiRegion );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsWater( true );
		//RegionPathfinder.Solve2( m_iUnitOwner, false );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &PathwayResult );
*/
		// Copy the pathway into the MultiRegionList
		EOSAI::UnitPathwayResultStep* pResultStep = new EOSAI::UnitPathwayResultStep( m_pResultPath );
		pResultStep->SetPredefinedStep( pPredefinedStep );
		m_pResultPath->m_Steps.AddTail( pResultStep );
		//pResultStep->m_ePathType = enum_FinalPath;
		//pResultStep->m_pPredefinedStep = pPredefinedStep;
		//pResultStep->m_pAIActor = GetAIPoiObjectActor();//m_pActor;
		//pResultStep->m_pUnitTemplate = m_pUnitTemplate;
	//	pResultStep->m_StartLocation = CurrentLocation;
	//	pResultStep->m_EndLocation = pPredefinedStep->GetEndLocation();//>m_Location;

		// DEBUG
		CEOSAIUnitPathwayPredefinedStep* pPrevPredefinedStep = pPredefinedStep->GetPrevStep();
		EOSAI::UnitPathwayResultStep*     pPrevResultStep = NULL; if( pPrevPredefinedStep ){ pPrevResultStep = pPrevPredefinedStep->GetResultStep(); }
		CEOSAIUnitPathwayPredefinedStep* pNextPredefinedStep = pPredefinedStep->GetNextStep();
		EOSAI::UnitPathwayResultStep*     pNextResultStep = NULL;  if( pNextPredefinedStep ){ pNextResultStep = pNextPredefinedStep->GetResultStep(); }
		// DEBUG

		// Copy the pathway into the MultiRegionList
		long iCurrentItem = 1;
		CEOSAIRegionPathwayItem* pCurrentItem = pEndLocationToEverywhere->GetRegionResult( pPredefinedStep->GetStartLocation() );
		if( pCurrentItem == NULL )
		{
			// Failed to find a pathway
			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}
			m_pResultPath->m_bPathIsValid = false;
			return;
		}

		//pPredefinedStep->GetActualMovementPath()->RemoveAll();
		//pPredefinedStep->GetAIRegionAllowedMovementPath()->RemoveAll();
		while( pCurrentItem )
		{
			CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( pCurrentItem->m_pPathfinderPoint->m_iId );
			ASSERT( pAIRegion );
			//pPredefinedStep->GetActualMovementPath()->AddTail( pAIRegion );
			//pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRegion );

			if( iCurrentItem == 1 )
			{
				pResultStep->m_bIsValid = true;
				pResultStep->m_fDistance = pCurrentItem->m_fDistance;
				pResultStep->m_fDanger = pCurrentItem->m_fDanger;
				pResultStep->m_fSighting = pCurrentItem->m_fSightings;
				pResultStep->m_fStepTime = pCurrentItem->m_fTime; //pCurrentItem->m_fDistance / fBestMovementRate;

				/*
				// This is the Start Location.  Grab all the adjacent AIRegions, too.  Allow them.
				CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( pCurrentItem->m_pPathfinderPoint->m_iId );
				POSITION pos2 = pAIRegion->m_AdjacentRegions.GetHeadPosition();
				while( pos2 )
				{
					CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pAIRegion->m_AdjacentRegions.GetNext( pos2 );
					CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pAdjRegionDesc->m_pRegion );
					pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRegion );
					//pResultStep->m_AIRegionMovementPath.AddHead( pAIRegion );
				}
				*/
			}
			iCurrentItem++;

			pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
		}

		CurrentLocation = pPredefinedStep->GetEndLocation();//GetStepFinalLocation();
	}

	m_pResultPath->m_bPathIsValid = true;
}


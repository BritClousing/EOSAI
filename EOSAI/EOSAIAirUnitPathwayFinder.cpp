
#include "stdafx.h"
#include "EOSAIAirUnitPathwayFinder.h"
#include "PoiObject.h"
#include "EOSAIUnit2.h"
//#include "Unit.h"
#include "EOSAIBrain.h"
//#include "WorldBuildDesc.h"
//#include "CommonState.h"
//#include "CombatManager.h"
#include "EOSAIAirbasesSet.h"
#include "City.h"
#include "EOSAIAirfield.h"
#include "EOSAIMath.h"
#include "EOSAIAirUnitPathwayData.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAIUnitPathwayResult.h"
#include "EOSAIUnitPathwayResultStep.h"
#include "EOSAICommonData.h"
#include "EOSAIInterface.h"
#include "AIPlayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIAirUnitPathwayFinder::CEOSAIAirUnitPathwayFinder( CEOSAIUnit2* pAIUnitActor )
	: CEOSAIUnit2PathwayFinder( pAIUnitActor )
{
	ASSERT( pAIUnitActor->GetAIUnitTemplate()->IsAirUnit() );
	if( pAIUnitActor->GetAIUnitTemplate()->IsAirUnit() == false ) return;

	m_fInitialMovementConsumed = pAIUnitActor->GetMovementConsumed();
	m_bAirbasesHaveBeenAddedToMapPoints = false;
}

CEOSAIAirUnitPathwayFinder::CEOSAIAirUnitPathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime )
	: CEOSAIUnit2PathwayFinder( pAICityActor, pAIUnitTemplate, fBuildTime )
{
	ASSERT( pAIUnitTemplate->IsAirUnit() );
	if( pAIUnitTemplate->IsAirUnit() == false ) return;

	m_fInitialMovementConsumed = 0.0f;
	m_bAirbasesHaveBeenAddedToMapPoints = false;
}
/*
CEOSAIAirUnitPathwayFinder::CEOSAIAirUnitPathwayFinder( CEOSAIBrain* pAIBrain ) : CEOSAIUnit2Pathway( pAIBrain )
{
	m_fInitialDistanceSinceLastAirbase = 0.0f;
	m_bAirbasesHaveBeenAddedToMapPoints = false;
	//m_bAppendLandingToTheEnd = false;
	//m_bNoPathExistsUsingCurrentAirbases = false;
}
*/
CEOSAIAirUnitPathwayFinder::~CEOSAIAirUnitPathwayFinder()
{
	while( m_AllMapPoints.IsEmpty() == FALSE ){ delete m_AllMapPoints.RemoveHead(); }
	//while( m_PathfinderResults.IsEmpty() == FALSE ){ delete m_PathfinderResults.RemoveHead(); }
	delete m_pResultPath; m_pResultPath = NULL;
}

// Setup
/*
void CEOSAIAirUnitPathwayFinder::SetMovementConsumed( float f )
{
	m_fInitialDistanceSinceLastAirbase = f;
}
*/
/*
void CEOSAIAirUnitPathwayFinder::SetUnit( CEOSAIPoiObject* pAIActor, CUnitTemplate* pUnitTemplate )
{
	if( pAIActor )
	{
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIActor );
		if( pAIUnit )
		{
			SetConsumedMovement( pAIUnit->GetMovementConsumed() );
			//m_fInitialDistanceSinceLastAirbase = pAIUnit->GetMovementConsumed();
		}
	}
	CEOSAIUnit2Pathway::SetUnit( iOwner, pAIActor, pUnitTemplate );
}
*/
//void CEOSAIAirUnitPathwayFinder::TurnAirbasesIntoMapPoints()
void CEOSAIAirUnitPathwayFinder::InvokeAirbaseMapPoints()
{
	if( m_bAirbasesHaveBeenAddedToMapPoints ) return; // this has already been done
	m_bAirbasesHaveBeenAddedToMapPoints = true;

	// Turn Useable Airbases into MapPoints
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	if( m_pAIUnitTemplate->CanLandAfterLaunch() )
	{
		CEOSAIAirbasesSet  UsableAirbases;
		//m_pAIBrain->GetAIThoughtDatabase()->GetAirbases( m_iUnitOwner, m_pAIUnitTemplate, &UsableAirbases );
		//g_pEOSAICommonData->GetAIThoughtDatabase( m_iUnitOwner )->GetAirbases( m_iUnitOwner, m_pAIUnitTemplate, &UsableAirbases );
		CEOSAIThoughtDatabase* pThoughtDatabase = g_pEOSAIInterface->GetAIPlayer(m_iUnitOwner)->GetAIBrain()->GetAIThoughtDatabase();
		pThoughtDatabase->GetAirbases( m_iUnitOwner, m_pAIUnitTemplate, &UsableAirbases );
		POSITION pos = UsableAirbases.m_Airbases.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIAirbase = UsableAirbases.m_Airbases.GetNext( pos );
			//CPoiObject* pAirbase = UsableAirbases.m_Airbases.GetNext( pos );
			//CEOSAIPoiObject* pAIAirbase = m_pAIBrain->GetAIPoiObject( pAIAirbase->GetObjectId() );
			ASSERT( pAIAirbase );
			if( pAIAirbase == NULL )
			{
				long iAirbaseObjectId = pAIAirbase->GetObjectId();
				//CEOSAIPoiObject* pAIAirbase2 = m_pAIBrain->GetAIPoiObject( iAirbaseObjectId );
				int h=0;
			}
			//
			if( pAIAirbase )
			{
				CEOSAIAirPathfinderMapPoint* pPathfinderMapPoint = new CEOSAIAirPathfinderMapPoint();
				pPathfinderMapPoint->m_pAIPoiObject = pAIAirbase;
				pPathfinderMapPoint->m_Location = pAIAirbase->GetLocation();
				pPathfinderMapPoint->m_bIsAnAirbase = true;
				m_AllMapPoints.AddTail( pPathfinderMapPoint );
			}
		}
	}

	float fMovementLimit = m_pAIUnitTemplate->GetMovementLimit();
	float fMovementRate = m_pAIUnitTemplate->GetMovementRate();

	// Expand to all possible airbases
	CList< CEOSAIAirPathfinderMapPoint* > CurrentActiveAirbases;
	POSITION pos = m_AllMapPoints.GetHeadPosition();
	while( pos )
	{
		CEOSAIAirPathfinderMapPoint* pMapPoint = m_AllMapPoints.GetNext( pos );
		if( pMapPoint->m_bIsAnAirbase == false ) continue;
		//
		//float fDistance = pWorldBuildDesc->GetPixelDistance( m_StartLocation, pMapPoint->m_Location );
		float fDistance = g_pWorldDistanceTool->GetDistance( m_StartLocation, pMapPoint->m_Location );
		if( fDistance + m_fInitialMovementConsumed < fMovementLimit )
		{
			float fAdditionalTime = fDistance / fMovementRate;
			pMapPoint->m_InitialEvaluation_bIsReachableAirbase = true;
			pMapPoint->m_InitialEvaluation_fArrivalTime = CEOSAIMath::RoundDown( m_fStartTime + fAdditionalTime + 1.0f );
			CurrentActiveAirbases.AddTail( pMapPoint );
		}
	}

	pos = CurrentActiveAirbases.GetHeadPosition();
	while( pos )
	{
		CEOSAIAirPathfinderMapPoint* pMapPoint1 = CurrentActiveAirbases.GetNext( pos );
		//
		POSITION pos2 = m_AllMapPoints.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIAirPathfinderMapPoint* pMapPoint2 = m_AllMapPoints.GetNext( pos2 );
			if( pMapPoint2->m_bIsAnAirbase == false ) continue;
			if( pMapPoint2->m_InitialEvaluation_bIsReachableAirbase ) continue;
			//
			//float fDistance = pWorldBuildDesc->GetPixelDistance( pMapPoint1->m_Location, pMapPoint2->m_Location );
			float fDistance = g_pWorldDistanceTool->GetDistance( pMapPoint1->m_Location, pMapPoint2->m_Location );
			if( fDistance < fMovementLimit )
			{
				float fAdditionalTime = fDistance / fMovementRate;
				pMapPoint2->m_InitialEvaluation_bIsReachableAirbase = true;
				pMapPoint2->m_InitialEvaluation_fArrivalTime = CEOSAIMath::RoundDown( pMapPoint1->m_InitialEvaluation_fArrivalTime + fAdditionalTime + 1.0f );
				CurrentActiveAirbases.AddTail( pMapPoint2 );
			}
		}
	}
}

// Transports
//
bool CEOSAIAirUnitPathwayFinder::UpdateEndLocation( CEOSAIUnit2PathwayPredefinedStep* pTransportPickupOrDropoffStep )
{
	//ASSERT( false );
	return true;
}

bool CEOSAIAirUnitPathwayFinder::QuickCheck_PathwayCanExist()
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	if( m_PreDefinedPath.IsEmpty() ) return true;
	float fMovementLimit = m_pAIUnitTemplate->GetMovementLimit();
	//
	CEOSAILocation CurrentLocation = m_StartLocation;
	//
	// Do a quick sanity check - make sure all points are within airbase range
	//   If the Unit fails any of these tests, then the movement isn't possible
	CEOSAIAirbasesSet  AirbasesSet;
	//m_pAIBrain->GetAIThoughtDatabase()->GetAirbases( m_iUnitOwner, m_pAIUnitTemplate, &AirbasesSet );
	//g_pEOSAICommonData->GetAIThoughtDatabase( m_iUnitOwner )->GetAirbases( m_iUnitOwner, m_pAIUnitTemplate, &AirbasesSet );
	CEOSAIThoughtDatabase* pThoughtDatabase = g_pEOSAIInterface->GetAIPlayer(m_iUnitOwner)->GetAIBrain()->GetAIThoughtDatabase();
	pThoughtDatabase->GetAirbases( m_iUnitOwner, m_pAIUnitTemplate, &AirbasesSet );
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		if( pStep->IsAWaypoint() )
		{
			float fClosestAirbase = AirbasesSet.GetClosestAirbaseDistance( pStep->GetEndLocation() );
			if( fClosestAirbase > fMovementLimit/2.0f )
			{
				return false;
			}
		}
		else if( pStep->IsALandAtAirbase() )
		{
			if( pStep->GetAITarget() ) // Land at specific airbase
			{
				// Because this airbase is part of the AirbasesSet, I can't use GetDistanceFromAirbases; it will always return 0
				//float fClosestAirbase = AirbasesSet.GetClosestAirbaseDistance_IgnoreOneAirbase( pStep->GetAITarget()->GetServerPoiObject(), pStep->GetEndLocation() );
				float fClosestAirbase = AirbasesSet.GetClosestAirbaseDistance_IgnoreOneAirbase( pStep->GetAITarget(), pStep->GetEndLocation() );
				if( fClosestAirbase > fMovementLimit )
				{
					return false;
				}
			}
		}
		else if( pStep->IsAnUpgrade() )
		{
			float fClosestAirbase = AirbasesSet.GetClosestAirbaseDistance( pStep->GetEndLocation() );
			if( fClosestAirbase > fMovementLimit ) 
			{
				return false;
			}
		}
		else if( pStep->IsAnAttack() )
		{
			// Don't worry about post-attack landing, allow for attack+crash
			float fClosestAirbase = AirbasesSet.GetClosestAirbaseDistance( pStep->GetEndLocation() );
			if( fClosestAirbase > fMovementLimit ) 
			{
				return false;
			}
			if( m_pAIUnitTemplate->SelfDestructsOnAttack() )
			{
				ASSERT( pos == NULL ); // this needs to be the last step in the pathway
				//ASSERT( m_bAppendLandingToTheEnd == false );
			}
		}
		else if( pStep->IsAPickup() || pStep->IsADropoff() )
		{
			float fClosestAirbase = AirbasesSet.GetClosestAirbaseDistance( pStep->GetEndLocation() );
			if( fClosestAirbase > fMovementLimit/2.0f )
			{
				return false;
			}
		}
		else
		{
			ASSERT( false );
		}

		CurrentLocation = pStep->GetEndLocation();
	}
	/*
	if( m_bAppendLandingToTheEnd )
	{
		float fClosestAirbase = AirbasesSet.GetClosestAirbaseDistance( CurrentLocation );
		if( fClosestAirbase > fMovementLimit )
		{
			return false;
		}
	}
	*/

	return true;
}

//   Make sure all points can be reached (check ranges)

bool CEOSAIAirUnitPathwayFinder::RunInitialEvaluation()
{
	// Start with the unit's current Location, MovementConsumed
	//   Create MapPoints (all airbases, all PredefinedSteps)
	//   Expand to all reachable MapPoints
	//   Check if all PredefinedSteps have been reached - if not, then the InitialEvaluation fails
	//m_bInitialEvaluationHasBeenDone = false;
	//m_bInitialEvaluationPathIsValid = false;

	if( QuickCheck_PathwayCanExist() == false ){ return false; }

	// Turn Useable Airbases into MapPoints
	InvokeAirbaseMapPoints();

	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	float fMovementLimit = m_pAIUnitTemplate->GetMovementLimit();

	// Try to reach all PredefinedSteps
	//   Start with the current location
	long iCurrentStep = 0;
	long iNumberOfSteps = m_PreDefinedPath.GetCount();
	bool bUnreachableStepsFound = false;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		iCurrentStep++;

		bool bWantToLandAfterThisStep = false;

		if( pPredefinedStep->IsAWaypoint() ){ bWantToLandAfterThisStep = true; } // Scouting
		else if( pPredefinedStep->IsALandAtAirbase() ){ bWantToLandAfterThisStep = false; } // Area-defense
		else if( pPredefinedStep->IsAnAttack() )
		{
			//if( m_bAppendLandingToTheEnd || iCurrentStep < iNumberOfSteps ){ bWantToLandAfterThisStep = true; }
		}
		else
		{
			ASSERT( false );
		}

		// Distance from [Start] or from closest Airbases
		//float fShortestDistance = pWorldBuildDesc->GetPixelDistance( m_StartLocation, pPredefinedStep->GetEndLocation() );
		float fShortestDistance = g_pWorldDistanceTool->GetDistance( m_StartLocation, pPredefinedStep->GetEndLocation() );
		POSITION pos2 = m_AllMapPoints.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIAirPathfinderMapPoint* pMapPoint2 = m_AllMapPoints.GetNext( pos2 );
			if( pMapPoint2->m_bIsAnAirbase == false ) continue;
			if( pMapPoint2->m_InitialEvaluation_bIsReachableAirbase == false ) continue;
			if( pMapPoint2->m_pPredefinedStep == pPredefinedStep ) continue;
			//float fDistance = pWorldBuildDesc->GetPixelDistance( pPredefinedStep->GetEndLocation(), pMapPoint2->m_Location );
			float fDistance = g_pWorldDistanceTool->GetDistance( pPredefinedStep->GetEndLocation(), pMapPoint2->m_Location );
			fShortestDistance = min( fShortestDistance, fDistance );
		}

		bool bStepIsReachable = false;
		if( bWantToLandAfterThisStep && fShortestDistance < fMovementLimit/2.0f )
		{
			bStepIsReachable = true;
		}
		if( bWantToLandAfterThisStep == false && fShortestDistance < fMovementLimit )
		{
			bStepIsReachable = true;
		}

		if( bStepIsReachable == false )
		{
			bUnreachableStepsFound = true;
		}
	}
	//m_bInitialEvaluationPathIsValid = !bUnreachableStepsFound;
	return ( bUnreachableStepsFound == false );
}

void  CEOSAIAirUnitPathwayFinder::CalculateAIRegionPathFromPredefinedSteps()
{
	//PreprocessPath();
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
			//CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pCurrentItem->m_pPathfinderPoint );
			//if( pAIRoad )
			//{
			//	pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRoad->GetAIRegion1() );
			//	pPredefinedStep->GetAIRegionAllowedMovementPath()->AddHead( pAIRoad->GetAIRegion2() );
			//}
			pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
		}
	}
}
/*
void  CEOSAIAirUnitPathwayFinder::CalculateAIRegionPathTiming()
{
	CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->UpdatePathTimingIfNecessary();
	}
}
*/
void CEOSAIAirUnitPathwayFinder::CalculateResultPath()
{
	// Clear existing Results
	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }
	m_pResultPath = new EOSAI::UnitPathwayResult( this );

	if( m_PreDefinedPath.IsEmpty() ){ return; } // No result path

	POSITION pos;
	InvokeAirbaseMapPoints();

	if( m_pAIUnitTemplate->IsABallisticMissile() )
	{
		// Just check some things
		ASSERT( m_fInitialMovementConsumed == 0.0f );
		ASSERT( m_PreDefinedPath.GetCount() == 1 );
	}

	// Do a quick sanity check - make sure all points are within airbase range
	//   If the Unit fails any of these tests, then the movement isn't possible
	if( QuickCheck_PathwayCanExist() == false )
	{
		m_pResultPath->m_bPathIsValid = false;
		return;
	}

	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	float fMovementLimit = m_pAIUnitTemplate->GetMovementLimit();
	float fMovementRate = m_pAIUnitTemplate->GetMovementRate();
	bool bSelfDestructsOnAttack = m_pAIUnitTemplate->SelfDestructsOnAttack();

	//
	// Delete previously created non-airbase MapPoints
	pos = m_AllMapPoints.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIAirPathfinderMapPoint* pMapPoint = m_AllMapPoints.GetNext( pos );
		if( pMapPoint->m_bIsAnAirbase == false )
		{
			m_AllMapPoints.RemoveAt( prevPos );
			delete pMapPoint;
		}
	}

	AppendALandingIfPossible();
	long iNumberOfSteps = m_PreDefinedPath.GetCount();
	//if( m_bAppendLandingToTheEnd ){ iNumberOfSteps++; }

	// Turn the start location into a pathpoint
	CList< CAirPathfinderSubPoint* >   CurrentSubPoints;
	CEOSAIAirPathfinderMapPoint* pStartMapPoint = new CEOSAIAirPathfinderMapPoint();
	pStartMapPoint->SetNumberOfPredefinedSteps( iNumberOfSteps );
	pStartMapPoint->m_pAIPoiObject = NULL;
	pStartMapPoint->m_Location = m_StartLocation;//pStartPoint->m_Location;
	pStartMapPoint->m_bIsStartPoint = true;
	//
	CAirPathfinderSubPoint* pStartSubPoint = pStartMapPoint->InvokeSubPoint( 0 );
	pStartSubPoint->m_fDistanceSinceLastAirbase = m_fInitialMovementConsumed;//pStartPoint->m_fDistanceSinceLastAirbase;
	pStartSubPoint->m_fTotalTime = m_fStartTime;//pStartPoint->m_fTime;
	//pStartSubPoint->m_pCurrentList = &CurrentSubPoints; why is it pointing to the list on the heap
	pStartSubPoint->m_bTemp_IsInsideCurrentSubPointsList = true;
	CurrentSubPoints.AddTail( pStartSubPoint );
	//
	m_AllMapPoints.AddTail( pStartMapPoint );

	// Turn the Useable Airbases into pathpoints
	InvokeAirbaseMapPoints();

	// Setup the SubPointArrays
	pos = m_AllMapPoints.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIAirPathfinderMapPoint* pMapPoint = m_AllMapPoints.GetNext( pos );
		pMapPoint->SetNumberOfPredefinedSteps( iNumberOfSteps );
	}
	// Clear EndPoint data
	pos = m_AllMapPoints.GetHeadPosition();
	while( pos )
	{
		CEOSAIAirPathfinderMapPoint* pMapPoint = m_AllMapPoints.GetNext( pos );
		pMapPoint->m_EndPointSet.Clear();

		if( pMapPoint->m_bIsAnAirbase )
		{
			ASSERT( pMapPoint->m_pAIPoiObject );
		}
	}

	// Turn the PreDefinedPath Points into AirPathfinderMapPoints
	long iCount = 0;
	pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		iCount++;
		//
		if( pStep->IsALandAtAirbase() )
		{
			POSITION pos2 = m_AllMapPoints.GetHeadPosition();
			while( pos2 )
			{
				CEOSAIAirPathfinderMapPoint* pPathfinderMapPoint = m_AllMapPoints.GetNext( pos2 );
				if( pPathfinderMapPoint->m_bIsAnAirbase )
				{
					if( pStep->GetAITarget() == NULL )
					{
						pPathfinderMapPoint->m_EndPointSet.Add( iCount-1 );
					}
					else if( pStep->GetAITarget() == pPathfinderMapPoint->m_pAIPoiObject )
					{
						pPathfinderMapPoint->m_EndPointSet.Add( iCount-1 );
					}
				}
			}
		}
		else if( pStep->IsAnUpgrade() || pStep->IsAWaypoint() || pStep->IsAnAttack() || 
				 pStep->IsAPickup() || pStep->IsADropoff() )
		{
			CEOSAIAirPathfinderMapPoint* pPathfinderMapPoint = new CEOSAIAirPathfinderMapPoint();
			pPathfinderMapPoint->SetNumberOfPredefinedSteps( iNumberOfSteps );
			//pPathfinderMapPoint->m_pAirPathPoint = pPathPoint;
			pPathfinderMapPoint->m_pPredefinedStep = pStep;
			if( pStep->GetAITarget() )
			{
				pPathfinderMapPoint->m_pAIPoiObject = pStep->GetAITarget();//->GetPoiObject();
			}
			//pPathfinderMapPoint->m_Location = pPathPoint->m_Location;
			pPathfinderMapPoint->m_Location = pStep->GetEndLocation();
			pPathfinderMapPoint->m_bIsAnAirbase = pStep->IsALandAtAirbase();
			pPathfinderMapPoint->m_bIsAnAttack = pStep->IsAnAttack();//dynamic_cast< CAIAirPathAttackTarget* >( pPathPoint ) != NULL;
			pPathfinderMapPoint->m_bIsAWaypoint = pStep->IsAWaypoint();//dynamic_cast< CAIAirPathWaypoint* >( pPathPoint ) != NULL;
			pPathfinderMapPoint->m_pAIPoiObject = pStep->GetAITarget();
			pPathfinderMapPoint->m_EndPointSet.Add( iCount-1 );
			m_AllMapPoints.AddTail( pPathfinderMapPoint );
		}
		else
		{
			ASSERT( false );
		}
	}
	/*
	if( m_bAppendLandingToTheEnd )
	{
		// Flag all airbases as an end-point
		POSITION pos2 = m_AllMapPoints.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIAirPathfinderMapPoint* pPathfinderMapPoint = m_AllMapPoints.GetNext( pos2 );
			pPathfinderMapPoint->m_EndPointSet.Clear();
			if( pPathfinderMapPoint->m_bIsAnAirbase )
			{
				pPathfinderMapPoint->m_EndPointSet.Add( iNumberOfSteps-1 );
			}
		}
	}
	*/

	//
	while( CurrentSubPoints.IsEmpty() == FALSE )
	{
		CAirPathfinderSubPoint* pCurrentSubPoint = CurrentSubPoints.RemoveHead();
		CEOSAIAirPathfinderMapPoint* pCurrentMapPoint = pCurrentSubPoint->m_pMapPoint;
		//
		pCurrentSubPoint->m_bTemp_IsInsideCurrentSubPointsList = false;

		// Look over the nearby PathfindingPoints
		POSITION pos = m_AllMapPoints.GetHeadPosition();
		while( pos )
		{
			CEOSAIAirPathfinderMapPoint* pAdjMainPoint = m_AllMapPoints.GetNext( pos );
			if( pCurrentMapPoint == pAdjMainPoint ) continue;
			if( pAdjMainPoint->m_bIsStartPoint ) continue;

			//float fDistance = pWorldBuildDesc->GetPixelDistance( pCurrentMapPoint->m_Location, pAdjMainPoint->m_Location );
			float fDistance = g_pWorldDistanceTool->GetDistance( pCurrentMapPoint->m_Location, pAdjMainPoint->m_Location );
			if( pCurrentMapPoint->m_bIsStartPoint == false && fDistance == 0.0f )
			{
				// This might be okay if the unit needs to upgrade and it's moving 
				//   from a start point to an airbase
				// This is bad if it's just jumping around from place to place
				continue; // same point
			}
			if( pCurrentSubPoint->m_fDistanceSinceLastAirbase + fDistance > fMovementLimit )
			{
				continue; // can't fly directly to AdjPathfinderPoint
			}
			float fAdditionalTime = fDistance / fMovementRate;

			float fNewTime = pCurrentSubPoint->m_fTotalTime + fAdditionalTime;
			if( pAdjMainPoint->m_bIsAnAirbase ){ fNewTime = CEOSAIMath::RoundDown( fNewTime + 1.0f ); } // if the airbase is [Distance] = 0, this still takes one turn before flying again

			float fNewDistance = pCurrentSubPoint->m_fTotalDistance + fDistance;
			float fNewDistanceSinceLastAirbase = pCurrentSubPoint->m_fDistanceSinceLastAirbase + fDistance;
			float fNewPathfindingValue = fNewTime;

			CAirPathfinderSubPoint* pAdjSubPoint =
				pAdjMainPoint->ConsiderNewSubPointObject( &CurrentSubPoints, pCurrentSubPoint->m_iStepNumber, fNewPathfindingValue,fNewDistanceSinceLastAirbase );
			if( pAdjSubPoint )
			{
				if( pAdjMainPoint->m_bIsAnAirbase ) fNewDistanceSinceLastAirbase = 0.0f; // reset

				pAdjSubPoint->m_iStepNumber = pCurrentSubPoint->m_iStepNumber;
				pAdjSubPoint->m_fPathfindingValue = fNewPathfindingValue;
				pAdjSubPoint->m_fTotalTime = fNewTime;
				pAdjSubPoint->m_fTotalDistance = fNewDistance;
				pAdjSubPoint->m_fDistanceSinceLastAirbase = fNewDistanceSinceLastAirbase;
				pAdjSubPoint->m_pPrevStep = pCurrentSubPoint;

				bool bMakeSureTheNewSubPointIsInTheCurrentList = true;
				if( pAdjMainPoint->m_EndPointSet.Exists( pCurrentSubPoint->m_iStepNumber ) )
				{
					pAdjSubPoint->m_pPredefinedStep = GetPredefinedPathStep( pCurrentSubPoint->m_iStepNumber );
					//ASSERT( pAdjSubPoint->m_pPredefinedStep == pAdjMainPoint->m_pPredefinedStep );
					pAdjSubPoint->m_iStepNumber = pCurrentSubPoint->m_iStepNumber+1;
					if( pAdjSubPoint->m_iStepNumber == iNumberOfSteps )
					{
						// If this is the final end point, don't put it back into the CurrentList
						bMakeSureTheNewSubPointIsInTheCurrentList = false;
					}
				}
				if( bMakeSureTheNewSubPointIsInTheCurrentList && 
					pAdjSubPoint->m_bTemp_IsInsideCurrentSubPointsList == false )
				{
					CurrentSubPoints.AddTail( pAdjSubPoint );
					pAdjSubPoint->m_bTemp_IsInsideCurrentSubPointsList = true;
				}
			}
		}
	}
	//
	// Find the best path (there might be several paths that accomplish the PreDefinedSteps)
	// Grab the EndPoints
	//   If some of the PredefinedTasks could not be accomplished, keep stepping through until
	//   I find the one that was accomplished
	float fBestPathfindingValue = 1000000.0f; // Best = lowest number
	bool bPathFound = false;
	CAirPathfinderSubPoint* pBestAirPathfinderSubPoint = NULL;
	//for( long iLookForStep=iNumberOfSteps-1; iLookForStep>=0; iLookForStep-- )
	long iLookForStep=iNumberOfSteps-1;
	{
		pos = m_AllMapPoints.GetHeadPosition();
		while( pos )
		{
			CEOSAIAirPathfinderMapPoint* pMainPoint = m_AllMapPoints.GetNext( pos );

			if( pMainPoint->m_EndPointSet.Exists( iLookForStep ) )
			{
				CAirPathfinderSubPointsList* pSubPoints = pMainPoint->m_SubPointsArray.Value( iLookForStep );
				POSITION pos2 = pSubPoints->m_List.GetHeadPosition();
				while( pos2 )
				{
					CAirPathfinderSubPoint* pCurrentSubPoint = pSubPoints->m_List.GetNext( pos2 );
					float fPathfindingValue = pCurrentSubPoint->m_fPathfindingValue;

					// Favor landing at the closest airbase
					float fLastStepMovementDistance = pCurrentSubPoint->m_fTotalDistance;
					//float fLastStepMovementTime = pCurrentSubPoint->m_fTotalDistance / ( GetUnitTemplate()->GetMovementRate()+1.0f );
					float fLastStepMovementTime = pCurrentSubPoint->m_fTotalDistance / ( GetAIUnitTemplate()->GetMovementRate()+1.0f );
					if( pCurrentSubPoint->m_pPrevStep )
					{
						fLastStepMovementDistance -= pCurrentSubPoint->m_pPrevStep->m_fTotalDistance;
						fLastStepMovementTime = fLastStepMovementDistance / ( GetAIUnitTemplate()->GetMovementRate()+1.0f );
					}
					fPathfindingValue += fLastStepMovementTime / 100.0f;

					// If this the best path?
					if( fPathfindingValue < fBestPathfindingValue )
					{
						pBestAirPathfinderSubPoint = pCurrentSubPoint;
						fBestPathfindingValue = fPathfindingValue;
						bPathFound = true;
					}
				}
			}
		}
		//if( bPathFound ) break;
	}
	// Now copy the path
	CEOSAIUnitTemplate* pAIUnitTemplate = this->GetAIUnitTemplate();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	if( pBestAirPathfinderSubPoint )
	{
		// Copy the sub-points
		CAirPathfinderSubPoint* pTempPoint = pBestAirPathfinderSubPoint;
		while( pTempPoint )
		{
			// Don't store the start point
			if( pTempPoint->m_pMapPoint->m_bIsStartPoint == false )
			{
				EOSAI::UnitPathwayResultStep* pResultStep = new EOSAI::UnitPathwayResultStep( m_pResultPath );
				if( pTempPoint->m_pPredefinedStep )
				{
					pResultStep->SetPredefinedStep( pTempPoint->m_pPredefinedStep );
				}

				// Overwrite the values
				CAirPathfinderSubPoint* pPrevSubPoint = pTempPoint->m_pPrevStep;
				if( pPrevSubPoint )
				{
					pResultStep->m_StartLocation = pPrevSubPoint->m_pMapPoint->m_Location;
				}
				else
				{
					pResultStep->m_StartLocation = GetStartLocation();
				}
				pResultStep->m_EndLocation = pTempPoint->m_pMapPoint->m_Location;
				pResultStep->m_bIsValid = true;

				//pResultStep->m_fDistance = pWorldBuildDesc->GetPixelDistance( pResultStep->m_StartLocation, pResultStep->m_EndLocation );
				pResultStep->m_fDistance = g_pWorldDistanceTool->GetDistance( pResultStep->m_StartLocation, pResultStep->m_EndLocation );
				pResultStep->m_fStepTime = pResultStep->m_fDistance / (pAIUnitTemplate->GetMovementRate()+0.1f);

				// What type of Map point is this?  Is it an intermediate landing?
				if( pTempPoint->m_pMapPoint->m_bIsAnAirbase )
				{
					ASSERT( pTempPoint->m_pMapPoint->m_pAIPoiObject );
					pResultStep->LandAtAirbase( pTempPoint->m_pMapPoint->m_pAIPoiObject );
				}
				m_pResultPath->m_Steps.AddHead( pResultStep );
				ASSERT( pResultStep->m_eTask != EOSAI::UnitPathwayResultStep::enum_Undefined );
				//m_pResultPath->m_Steps.AddTail( pResultStep );
			}
			pTempPoint = pTempPoint->m_pPrevStep;
		}

		//m_AirUnitPathway.m_pPathfinderSubPoint = pCurrentSubPoint;
		m_pResultPath->m_fPathfindingValue = pBestAirPathfinderSubPoint->m_fPathfindingValue;
		m_pResultPath->m_fCompletionTime = pBestAirPathfinderSubPoint->m_fTotalTime;
		if( IncludesAnAttack() )
		{
			CAirPathfinderSubPoint* pTempPoint = pBestAirPathfinderSubPoint;
			while( pTempPoint )
			{
				if( pTempPoint->m_pMapPoint->m_bIsAnAttack ){ m_pResultPath->m_fAttackTime = pTempPoint->m_fTotalTime; break; }
				pTempPoint = pTempPoint->m_pPrevStep;
			}
		}
		if( IncludesAnAttack() && m_pAIUnitTemplate->SelfDestructsOnAttack() )
		{
			m_pResultPath->m_bResultsInDeath = true;
			m_pResultPath->m_fSurvivalOdds = 0.0f;
		}
	}
/*
	float fBestPathfindingValue = 1000000.0f; // Best = lowest number
	bool bPathFound = false;
	for( long iLookForStep=iNumberOfSteps-1; iLookForStep>=0; iLookForStep-- )
	{
		if( bPathFound ) break;

		pos = m_AllMapPoints.GetHeadPosition();
		while( pos )
		{
			CEOSAIAirPathfinderMapPoint* pMainPoint = m_AllMapPoints.GetNext( pos );

			if( pMainPoint->m_EndPointSet.Exists( iLookForStep ) )
			{
				CAirPathfinderSubPointsList* pSubPoints = pMainPoint->m_SubPointsArray.Value( iLookForStep );
				POSITION pos2 = pSubPoints->m_List.GetHeadPosition();
				while( pos2 )
				{
					CAirPathfinderSubPoint* pCurrentSubPoint = pSubPoints->m_List.GetNext( pos2 );
					float fPathfindingValue = pCurrentSubPoint->m_fPathfindingValue;

					//bool bSelfDestructsOnAttack = false;
					//if( IncludesAnAttack() && m_pUnitTemplate->SelfDestructsOnAttack() )
					//{
					//	bSelfDestructsOnAttack = true;
					//}
					if( fPathfindingValue < m_pResultPath->m_fPathfindingValue )
					{
						bPathFound = true;
						//m_bPathIsValid = true;

						m_pResultPath->Clear();

						// Copy the sub-points
						CAirPathfinderSubPoint* pTempPoint = pCurrentSubPoint;
						while( pTempPoint )
						{
							// Don't store the start point
							if( pTempPoint->m_pMapPoint->m_bIsStartPoint == false )
							{
								EOSAI::UnitPathwayResultStep* pResultStep = new EOSAI::UnitPathwayResultStep( m_pResultPath );
								if( pTempPoint->m_pPredefinedStep )
								{
									pResultStep->SetPredefinedStep( pTempPoint->m_pPredefinedStep );
								}

								// Overwrite the values
								CAirPathfinderSubPoint* pPrevSubPoint = pTempPoint->m_pPrevStep;
								if( pPrevSubPoint )
								{
									pResultStep->m_StartLocation = pPrevSubPoint->m_pMapPoint->m_Location;
								}
								else
								{
									pResultStep->m_StartLocation = GetStartLocation();
								}
								pResultStep->m_EndLocation = pTempPoint->m_pMapPoint->m_Location;
								pResultStep->m_bIsValid = true;

								// What type of Map point is this?  Is it an intermediate landing?
								if( pTempPoint->m_pMapPoint->m_bIsAnAirbase )
								{
									ASSERT( pTempPoint->m_pMapPoint->m_pAIPoiObject );
									pResultStep->LandAtAirbase( pTempPoint->m_pMapPoint->m_pAIPoiObject );
								}
								m_pResultPath->m_Steps.AddHead( pResultStep );
								/-*
								else if( pTempPoint->m_pMapPoint->m_bIsAnAttack )
								{
									if( pTempPoint->m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_AttackUnit )
									{
										pResultStep->AttackUnit( pTempPoint->m_pMapPoint->m_pAIPoiObject );
									}
									if( pTempPoint->m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_CaptureCitRes )
									{
										pResultStep->CaptureCitRes( pTempPoint->m_pMapPoint->m_pAIPoiObject );
									}
									if( pTempPoint->m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_DegradeCityOrAirfield )
									{
										pResultStep->DegradeCityOrAirfield( pTempPoint->m_pMapPoint->m_pAIPoiObject );
									}
									m_pResultPath->m_Steps.AddHead( pResultStep );
								}
								else if( pTempPoint->m_pMapPoint->m_bIsAWaypoint )
								{
									pResultStep->MoveTo( pTempPoint->m_pMapPoint->m_Location );
									m_pResultPath->m_Steps.AddHead( pResultStep );
								}
								else
								{
									ASSERT( false ); // TEMPORARY
								}
								*-/

								//CurrentLocation = pTempPoint->m_pMapPoint->m_Location;
								//
								//m_pResultPath->m_Steps.AddHead( pTempPoint );
							}
							pTempPoint = pTempPoint->m_pPrevStep;
						}

						//m_AirUnitPathway.m_pPathfinderSubPoint = pCurrentSubPoint;
						m_pResultPath->m_fPathfindingValue = pCurrentSubPoint->m_fPathfindingValue;
						m_pResultPath->m_fCompletionTime = pCurrentSubPoint->m_fTotalTime;
						if( IncludesAnAttack() )
						{
							CAirPathfinderSubPoint* pTempPoint = pCurrentSubPoint;
							while( pTempPoint )
							{
								if( pTempPoint->m_pMapPoint->m_bIsAnAttack ){ m_pResultPath->m_fAttackTime = pTempPoint->m_fTotalTime; break; }
								pTempPoint = pTempPoint->m_pPrevStep;
							}
						}
						if( IncludesAnAttack() && m_pUnitTemplate->SelfDestructsOnAttack() )
						{
							m_pResultPath->m_bResultsInDeath = true;
							m_pResultPath->m_fSurvivalOdds = 0.0f;
						}
					}
				}
			}
		}
		/-*
		// Find EndPoints
		for( long iCycle=0; iCycle<2; iCycle++ )
		{
			if( iCycle==1 && m_bAppendLandingToTheEnd == false ) continue;

			long iLookForLevel;
			if( iCycle==0 )
			{
				iLookForLevel = iNumberOfSteps-1;
			}
			if( iCycle==1 )
			{
				iLookForLevel = iNumberOfSteps-2;
			}
			//
			if( pMainPoint->m_EndPointSet.Exists( iLookForLevel ) ) //iNumberOfSteps-1 ) )
			{
				CAirPathfinderSubPointsList* pSubPoints = pMainPoint->m_SubPointsArray.Value( iLookForLevel );
				POSITION pos = pSubPoints->m_List.GetHeadPosition();
				while( pos )
				{
					CAirPathfinderSubPoint* pCurrentSubPoint = pSubPoints->m_List.GetNext( pos );
					float fPathfindingValue = pCurrentSubPoint->m_fPathfindingValue;

					if( iCycle == 0 )
					{
						bool bSelfDestructsOnAttack = false;
						if( IncludesAnAttack() && m_pUnitTemplate->SelfDestructsOnAttack() ){ bSelfDestructsOnAttack = true; }
						//
						CAirPathfinderResult* pPathfinderResult = ConsiderNewPathfinderResult( bSelfDestructsOnAttack, fPathfindingValue );
						if( pPathfinderResult )
						{
							m_bFinalPathIsValid = true;

							// These are actual endpoints
							//CAirPathfinderResult* pPathfinderResult = new CAirPathfinderResult();
							pPathfinderResult->m_pPathfinderSubPoint = pCurrentSubPoint;
							pPathfinderResult->m_fPathfindingValue = pCurrentSubPoint->m_fPathfindingValue;
							pPathfinderResult->m_fCompletionTime = pCurrentSubPoint->m_fTotalTime;
							if( IncludesAnAttack() )
							{
								CAirPathfinderSubPoint* pTempPoint = pCurrentSubPoint;
								while( pTempPoint )
								{
									if( pTempPoint->m_pMapPoint->m_bIsAnAttack ){ pPathfinderResult->m_fAttackTime = pTempPoint->m_fTotalTime; break; }
									pTempPoint = pTempPoint->m_pPrevStep;
								}
							}
							if( bSelfDestructsOnAttack )
							{
								pPathfinderResult->m_bResultsInDeath = true;
								pPathfinderResult->m_fSurvivalOdds = 0.0f;
							}
							//m_PathfinderResults.AddTail( pPathfinderResult );
						}
					}
					if( iCycle == 1 && m_bAppendLandingToTheEnd )
					{
						// These are endpoints where I wanted an AppendAirbase, but didn't find one
						bool bAddThisResult = false;
						if( pCurrentSubPoint->m_pNextStep == NULL )
						{
							bAddThisResult = true;
						}
						else
						{
							bAddThisResult = true;
							CAirPathfinderSubPoint* pTempPoint = pCurrentSubPoint;
							while( pTempPoint )
							{
								if( pTempPoint->m_pMapPoint->m_bIsAnAirbase ) bAddThisResult = false;
								pTempPoint = pTempPoint->m_pNextStep;
							}
						}
						if( bAddThisResult )
						{
							bool bSelfDestructsOnAttack = true;
							if( IncludesAnAttack() && m_pUnitTemplate->SelfDestructsOnAttack() ){ bSelfDestructsOnAttack = true; }
							//
							CAirPathfinderResult* pPathfinderResult = ConsiderNewPathfinderResult( bSelfDestructsOnAttack, fPathfindingValue );
							if( pPathfinderResult )
							{
								m_bFinalPathIsValid = true;

								// Either there are no sub-points, or it goes to a non-airbase (i.e. dead-end)
								pPathfinderResult->m_pPathfinderSubPoint = pCurrentSubPoint;
								pPathfinderResult->m_fPathfindingValue = pCurrentSubPoint->m_fPathfindingValue;
								pPathfinderResult->m_fCompletionTime = pCurrentSubPoint->m_fTotalTime;
								if( IncludesAnAttack() )
								{
									CAirPathfinderSubPoint* pTempPoint = pCurrentSubPoint;
									while( pTempPoint )
									{
										if( pTempPoint->m_pMapPoint->m_bIsAnAttack ){ pPathfinderResult->m_fAttackTime = pTempPoint->m_fTotalTime; break; }
										pTempPoint = pTempPoint->m_pPrevStep;
									}
								}
								pPathfinderResult->m_bResultsInDeath = true;
								pPathfinderResult->m_fSurvivalOdds = 0.0f;
								//m_PathfinderResults.AddTail( pPathfinderResult );
							}
						}
					}
					int t=0;
				}
			}
		}
		*-/
		int z=0;
	}
*/
	m_pResultPath->m_bPathIsValid = true;
	int g=0;
}

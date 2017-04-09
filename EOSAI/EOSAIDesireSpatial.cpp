
#include "stdafx.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAIDesireConnection.h"
#include "EOSAIBrain.h"
#include "AIPlayer.h"
#include "EOSAIMultiRegion2.h"
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIMultiRegionNationwidePathways.h"
#include "City.h"
#include "EOSAIUnit.h"
#include "EOSAIMathFunction.h"
//#include "CommonState.h"
//#include "WorldBuildDesc.h"
//#include "WorldDescServer.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIAirbasesSet.h"
#include "EOSAIStopwatch2.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAIDesireDestroyUnit.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIDesireSpatial::CEOSAIDesireSpatial()
{
	//m_eProcessingLevel = EnumNoProcessing;
	//
	m_bNationwideLandRouteExists = false;
	m_bNationwideSeaRouteExists = false;
	//
	m_fAccessibility01 = 0.0f;
	m_fNationwidePathwayDistance = 0.0f;
	//
	m_bPotentialResistanceHasBeenCalculated = false;

	m_fFulfillment = 0.0f;
}

CEOSAIDesireSpatial::~CEOSAIDesireSpatial()
{
	while( m_ConnectedDesires.IsEmpty() == FALSE )
	{
		delete m_ConnectedDesires.RemoveHead();
	}
}

CString  CEOSAIDesireSpatial::Dump()
{
	return _T("-\r\n");
}

/*
void CEOSAIDesireSpatial::InvokeProcessingLevel( EnumDesireProcessingLevel eNewProcessingLevel )
{
	ASSERT( false );
	/-*
	if( m_eProcessingLevel < eNewProcessingLevel )
	{
		if( m_eProcessingLevel  <  EnumInitialCalculation &&
			eNewProcessingLevel >= EnumInitialCalculation )
		{
			CalculateAccessibility01();
		}
		if( m_eProcessingLevel  <  EnumCustomPathwayCalculation &&
			eNewProcessingLevel >= EnumCustomPathwayCalculation )
		{
			CreatePathwayFromEverywhereToHere();
		}
	}
	*-/
}
*/
void CEOSAIDesireSpatial::CalculateAccessibility01()
{
	//ASSERT( m_eProcessingLevel == EnumNoProcessing );
	//m_eProcessingLevel = EnumInitialCalculation;

	// Make the "starting point" this desire's multiregion, we can then work backwards to the rest
	//   of the world.
	/*
	// Iterate over each of my units, find an attack pathway to the target (using required units)
	//   This uses imaginary transports, imaginary carriers, and city-build options
	CWorldDesc* pWorldDesc = m_pAIBrain->GetWorldDesc();
	CEOSAIMultiRegionManager2* pMultiRegionManager = m_pAIBrain->GetMultiRegionManager();
	long iPlayer = m_pAIBrain->GetAIPlayerNumber();
	CMultiRegion* pMultiRegion = pMultiRegionManager->GetMultiRegion( GetLocation() );

	// Make the "target" the start location (I'll check the end locations later)
	pMultiRegionManager->ClearPathfindingSettings();
	pMultiRegionManager->SetDistanceMultiplier( 1.0f );
	pMultiRegionManager->SetGeoSwitchMultiplier( 200.0f );
	pMultiRegionManager->SetToIgnoreOtherPlayersPathway( iPlayer );
	pMultiRegionManager->AddStartingMultiRegion( pMultiRegion );
	pMultiRegionManager->Solve( iPlayer, true, &m_MultiRegionPathwayResult );
	*/

	//
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	CEOSAIAirbasesSet AirbasesSet;
	GetAIThoughtDatabase()->GetAirbases( iAIPlayer, &AirbasesSet );
	float f1 = AirbasesSet.GetClosestAirbaseDistance( CEOSAILocation( CEOSAILocation::Pixel,0,0 ) );
	float f2 = AirbasesSet.GetClosestAirbaseDistanceX5Turns( CEOSAILocation( CEOSAILocation::Pixel,0,0 ) );

	// 1. Get the distance from my nationwide pathway to the target
	// 2. Get the best movement rate of units that can complete the action

	//CEOSAIMultiRegionNationwidePathways* pPathways = m_pAIBrain->GetAIPlayer()->GetMyNationwidePathway();
	CEOSAIMultiRegionNationwidePathways* pPathways = g_pEOSAICommonData->GetNationwidePathways(iAIPlayer);
	ASSERT( pPathways );
	CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( GetLocation() );
	CEOSAIRegionPathwayItem* pMultiRegionResult = pPathways->GetPlayerAttackPathwayBasedOnCitResUnits( pMultiRegion->m_iId );
	ASSERT( pMultiRegionResult );
	float fDistanceAndGeoSwitches = pMultiRegionResult->m_fDistance + 100.0f*pMultiRegionResult->m_iGeoSwitches;

	if( pMultiRegionResult->m_iGeoSwitches == 0 )
	{
		m_bNationwideLandRouteExists = pMultiRegion->IsLand() || pMultiRegion->IsCity();
		m_bNationwideSeaRouteExists = pMultiRegion->IsWater();
	}

	CEOSAIUnitTemplateSet Units;
	Units.SetToUsefulUnits( m_JobsToDo, GetAIThoughtDatabase()->GetUnitsICanBuildOrHave() );
	//m_Jobs.GetUnitsToCompleteJobs( GetAIThoughtDatabase()->GetUnitsICanBuildOrHave(), &Units );

	float fEstimatedTime = 1000000.0f;
	POSITION pos = Units.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = Units.m_List.GetNext( pos );
		float fTime = 1000000.0f;
		float fMovementRate = pAIUnitTemplate->GetMovementRate();
		if( pAIUnitTemplate->GetMovementRate() > 0.0f )
		{
			// Aircraft accessibility should take range into account.
			//    We don't look at airbases because we have 'nationwide' distance to compare it against.
			// INCOMPLETE: Although, that would ignore the airbases of allies
			if( pAIUnitTemplate->HasARange() &&
				pMultiRegionResult->m_fDistance > pAIUnitTemplate->GetMovementRangeForAttack() )
			{
				fTime = 1000000.0f;
			}
			else
			{
				fTime = fDistanceAndGeoSwitches / fMovementRate;
			}
			fEstimatedTime = min( fEstimatedTime, fTime );
		}
	}

	m_fNationwideEstimatedTimeToTarget = fEstimatedTime;
	m_fNationwidePathwayDistance = pMultiRegionResult->m_fDistance; // for debug

	CEOSAIMathFunction AccessibilityFunc;
	AccessibilityFunc.SetInputOutput(  0.0f, 1.0f  );
	AccessibilityFunc.SetInputOutput(  1.0f, 0.8f  );
	AccessibilityFunc.SetInputOutput(  5.0f, 0.4f  );
	AccessibilityFunc.SetInputOutput( 10.0f, 0.1f  );
	AccessibilityFunc.SetInputOutput( 20.0f, 0.05f );
	m_fAccessibility01 = AccessibilityFunc.GetOutput( fEstimatedTime );
}

/*
void CEOSAIDesireSpatial::IsBeingFulfilled( CAITacticalProject* pTacticalProjectFulfillingThisDesire )
{
	ASSERT( m_pTacticalProjectFulfillingThisDesire == NULL );
	m_pTacticalProjectFulfillingThisDesire = pTacticalProjectFulfillingThisDesire;
}
*/
/*
bool CEOSAIDesireSpatial::InvolvesGroundTargets()
{
	EOSAIEnumAIDesireType eType = GetType();
	if( eType == EnumBuildAirfield || eType == EnumCaptureCity )
	{
		return true;
	}
	if( eType == EnumCaptureResource )
	{
		if( GetMultiRegion()->GetGeo()->GetGeoType() == CGeo::Land ) return true;
	}
	return false;
}
*/

//void CEOSAIDesireSpatial::GetDesiresInTheImmediateArea( CList< CEOSAIDesireSpatial* >* pDesires )
void CEOSAIDesireSpatial::GetDestroyUnitDesiresInTheImmediateArea( CList< CEOSAIDesireSpatial* >* pDesires )
{
	// Include this desire
	pDesires->AddTail( this );

	//bool bThisIsACaptureCitRes = dynamic_cast< CEOSAIDesireCaptureCitRes* >( this );
	//bool bThisIsADamageCity = dynamic_cast< CAIDesireDamageCity* >( this );

	POSITION pos = GetNearbyDesires()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireConnection* pConn = GetNearbyDesires()->GetNext( pos );
		if( pConn->m_fAirDistance < 30.0f && 
			dynamic_cast< CAIDesireDestroyUnit* >( pConn->m_pConnectedAISpatialDesire ) )
		{
			// Might handle this Desire as well - it might be combat
			pDesires->AddTail( pConn->m_pConnectedAISpatialDesire );
		}
	}
}

// AttachedUnits = Units within 30 pixels of this target
//
void CEOSAIDesireSpatial::GetUnitsInTheImmediateArea( CList< CEOSAIUnit* >* pEnemyUnits )
{
	CEOSAIPoiObject* pAIPoiObject = this->GetAIPoiObjectTarget();
	CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
	if( pAIUnit )
	{
		pEnemyUnits->AddTail( pAIUnit );
	}

	POSITION pos = GetNearbyDesires()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireConnection* pConn = GetNearbyDesires()->GetNext( pos );
		if( pConn->m_fAirDistance < 30.0f )
		{
			// Must handle this Desire as well - it might be combat
			CEOSAIDesireSpatial* pOtherDesire = pConn->m_pConnectedAISpatialDesire;
			CEOSAIPoiObject* pAIPoiObject = pOtherDesire->GetAIPoiObjectTarget();
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				pEnemyUnits->AddTail( pAIUnit );
			}
		}
	}
}

bool CEOSAIDesireSpatial::RequiresLandUnits()
{
	EOSAIEnumAIDesireType eType = GetType();
	if( eType == EnumBuildAirfield || eType == EnumCaptureCity )
	{
		return true;
	}
	if( eType == EnumCaptureResource )
	{
		if( GetMultiRegion()->GetGeo()->GetGeoType() == CEOSAIGeo::Land ) return true;
	}
	return false;
}

bool  CEOSAIDesireSpatial::RequiresSeaUnits()
{
	EOSAIEnumAIDesireType eType = GetType();
	if( eType == EnumCaptureResource )
	{
		if( GetMultiRegion()->GetGeo()->GetGeoType() == CEOSAIGeo::Water ) return true;
	}
	return false;
}
/*
float CEOSAIDesireSpatial::GetTransportNeed01() // 0 = no need, 1 = must have transport, 0.5 = transport useful
{
	if( RequiresLandUnits() && NationwideLandRouteExists() == false ) return 1.0f;
	return 0.0f;
}
*/
// Effort
/*
float CEOSAIDesireSpatial::GetAreaExpectedEffort()
{
	// Note: "Effort" varies depending on my method of attack
	//   Firing a nuke = very little effort (but does cost me a nuke)
	//   Bombing a city = worried about anti-aircraft fire, ignore most units
	//   Using ground units + capturing a city = have to fight all the units in the area
}
*/
void  CEOSAIDesireSpatial::CalculateExpectedPathwayResistance()
{
	ASSERT( m_bPotentialResistanceHasBeenCalculated == false );
	m_bPotentialResistanceHasBeenCalculated = true;

	m_PathwayResistance.RemoveAll();

	// Add local militias to the list (don't need to add enemies because we'll add them later)
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	//POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
	//POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit && pAIUnit->GetOwner() == 0 )
		{
			float fDistance = g_pWorldDistanceTool->GetDistance( pAIUnit->GetInitialState()->GetLocation(), GetLocation() );
			if( fDistance < 2.0f )
			{
				m_PathwayResistance.AddTail( pAIUnit );
			}
		}
	}

	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	//if( eScoringLevel == EnumInitialScoring )
	{
		// Iterate over the nationwide pathway steps
		//CEOSAIMultiRegionNationwidePathways* pPathways = m_pAIBrain->GetAIPlayer()->GetMyNationwidePathway();
		CEOSAIMultiRegionNationwidePathways* pPathways = g_pEOSAICommonData->GetNationwidePathways(iAIPlayer);
		ASSERT( pPathways );
		CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( GetLocation() );
		CEOSAIRegionPathwayItem* pMultiRegionResult = pPathways->GetPlayerAttackPathwayBasedOnCitResUnits( pMultiRegion->m_iId );
		ASSERT( pMultiRegionResult );

		//
		float fInsideRadius = 200.0f;

		//POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
		POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit == NULL ) continue; // ignore non-units
			//CPoiObject* pServerPoiObject = pAIPoiObject->GetServerPoiObject();
			//if( pAIUnit->GetForeignRelationsTo( iAIPlayer ).IsEnemy() == false ) continue;
			if( pAIPoiObject->GetForeignRelationsTo( iAIPlayer ).IsEnemy() == false ) continue;

			long iCurrentMultiRegion = pMultiRegion->m_iId;
			while( iCurrentMultiRegion )
			{
				// Check the details of the MultiRegion
				CEOSAIRegionPathwayItem* pCurrentResult = pPathways->GetPlayerAttackPathwayBasedOnCitResUnits( iCurrentMultiRegion );

				// Do stuff - collect all the units in the area
				//CEOSAIGenericRegion* pCurrentRegion = pCurrentResult->m_pPathfinderPoint;
				CEOSAIPathfinderPoint* pPoint = pCurrentResult->m_pPathfinderPoint;
				CEOSAILocation PointLocation = pPoint->GetLocation();
				//ASSERT( pCurrentRegion->m_fRegionRadius < fInsideRadius );

				//if( pWorldBuildDesc->GetPixelDistance( PointLocation, pAIUnit->GetLocation() ) < fInsideRadius )
				if( g_pWorldDistanceTool->GetDistance( PointLocation, pAIUnit->GetLocation() ) < fInsideRadius )
				{
					m_PathwayResistance.AddTail( pAIUnit );
					break; // break-out of the while loop
				}

				// Move to the previous one
				iCurrentMultiRegion = pCurrentResult->m_iPreviousPathwayItem;
			}
		}
	}

	//m_ePathwayResistanceScoringLevel = eScoringLevel;
}

void CEOSAIDesireSpatial::CalculateAllocatedFulfillment()
{
	// Requires 1 unit for fulfillment, except when it involves combat
	//   (What about matching up "capture city" desire with CitResHunter?)
	CList< CEOSAIDesireSpatial* >  Desires;
	GetDestroyUnitDesiresInTheImmediateArea( &Desires );

	CEOSAIJobsToDo  JobsToDo;
	POSITION pos = Desires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = Desires.GetNext( pos );
		if( pDesire->CurrentForeignRelationsPreventsPersuingThisDesire() == false )
		{
			JobsToDo.Add( pDesire->GetJobsToDo() );
		}
	}
	CEOSAIJobCapability JobCapability;
	pos = m_UnitActionIdeaAllocations.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitActionIdea* pActionIdea = m_UnitActionIdeaAllocations.GetNext( pos );
		JobCapability.Add( pActionIdea->GetAIUnitTemplate() );
	}
	m_fFulfillment = JobsToDo.GetMinimumFulfillment01( &JobCapability );
}

float CEOSAIDesireSpatial::GetAllocatedFulfillment()
{
	return m_fFulfillment;
}


/*
float CEOSAIDesireSpatial::GetTimeToReachTargetViaPathwaysAndMyUnits()
{
	InvokeProcessingLevel( CustomPathwayCalculation );
	/-*
	if( m_eProcessingLevel == EnumInitialCalculation )
	{
		CreatePathwayFromEverywhereToHere();
		m_eProcessingLevel = CustomPathwayCalculation;
	}
	*-/
	return m_fTimeToReachTargetViaPathwaysAndMyUnits;
}
*/

CEOSAIRegionPathwayMap* CEOSAIDesireSpatial::InvokePathToEverywhere()
{
	//ASSERT( GetCommonState() );
	//ASSERT( GetCommonState()->GetWorldDescServer() );
	//ASSERT( GetCommonState()->GetWorldDescServer()->GetAICommonData() );
	//ASSERT( GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager() );

	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();
	CEOSAIMultiRegion2* pStartMultiRegion = pMultiRegionManager->GetMultiRegion( GetLocation() );
	return pStartMultiRegion->InvokePathToEverywhere();
/*
//	I changed this code so that MultiRegions create 'pathways to everywhere' rather than
//	  desires.  The reason for this is efficiency.  If I have two desires in the same Multiregion,
//	  the calculation happens twice when I use desires.
	if( m_PathwayToEverywhereResult.GetArraySize() == 0 )
	{
		CWorldDesc* pWorldDesc = m_pAIBrain->GetWorldDesc();
		//CEOSAIMultiRegionManager2* pMultiRegionManager = m_pAIBrain->GetMultiRegionManager();
		CEOSAIMultiRegionManager2* pMultiRegionManager = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager();
		long iPlayer = m_pAIBrain->GetAIPlayerNumber();
		CMultiRegion* pStartMultiRegion = pMultiRegionManager->GetMultiRegion( GetLocation() );

		// Timer test
		//CEOSAIStopwatch2  Stopwatch1a, Stopwatch1b, Stopwatch2;
		//Stopwatch1a.Start();

		// Make the "target" the start location (I'll check the end locations later)
		CEOSAIRegionPathfinder& RegionPathfinder = pMultiRegionManager->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iPlayer );
		RegionPathfinder.SetDistanceMultiplier( 1.0f );
		RegionPathfinder.SetGeoSwitchMultiplier( 200.0f );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iPlayer );
		RegionPathfinder.AddStartingRegion( pStartMultiRegion );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		//Stopwatch1a.Stop();
		//Stopwatch1b.Start();
		RegionPathfinder.Solve3();//.Solve( iPlayer, true );
		RegionPathfinder.GetResult( true, &m_PathwayToEverywhereResult );
		//Stopwatch1b.Stop();
	}
	return &m_PathwayToEverywhereResult;
	*/
}

// Look over all my units, figure out how quickly the required units can get there
//float CEOSAIDesireSpatial::TimeToReachTargetViaPathwaysAndMyUnits()
//void CEOSAIDesireSpatial::CreateUnitActionIdeas()
//void CEOSAIDesireSpatial::CalculateTimeToReachTargetViaPathwaysAndMyUnits()
/*
void CEOSAIDesireSpatial::CreatePathwayFromEverywhereToHere()
{
	//ASSERT( m_eProcessingLevel == EnumInitialCalculation );
	//m_eProcessingLevel = EnumCustomPathwayCalculation;

	// Iterate over each of my units, find an attack pathway to the target (using required units)
	//   This uses imaginary transports, imaginary carriers, and city-build options
	//m_pAIBrain->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
	CWorldDesc* pWorldDesc = m_pAIBrain->GetWorldDesc();
	CEOSAIMultiRegionManager2* pMultiRegionManager = m_pAIBrain->GetMultiRegionManager();
	long iPlayer = m_pAIBrain->GetAIPlayerNumber();
	CMultiRegion* pMultiRegion = pMultiRegionManager->GetMultiRegion( GetLocation() );

	// Timer test
	CEOSAIStopwatch2  Stopwatch1a, Stopwatch1b, Stopwatch2;
	Stopwatch1a.Start();

	// Make the "target" the start location (I'll check the end locations later)
	CEOSAIRegionPathfinder& RegionPathfinder = pMultiRegionManager->GetPathfinder();
	RegionPathfinder.ClearPathfindingSettings();
	//RegionPathfinder.SetDistanceMultiplier( 1.0f );
	RegionPathfinder.SetGeoSwitchMultiplier( 200.0f );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( iPlayer );
	RegionPathfinder.AddStartingRegion( pMultiRegion );
	Stopwatch1a.Stop();
	Stopwatch1b.Start();
	RegionPathfinder.Solve( iPlayer, true );
	RegionPathfinder.GetResult( true, &m_PathwayResult );
	Stopwatch1b.Stop();

	/-*
	Stopwatch2.Start();

	// For comparison
	float f = 1.1f;
	for( long i=0; i<1000; i++ )
	{
		f = f*f;
	}

	Stopwatch2.Stop();
	float fResult1a = Stopwatch1a.GetDeltaSeconds();
	float fResult1b = Stopwatch1b.GetDeltaSeconds();
	float fResult2 = Stopwatch2.GetDeltaSeconds();
	*-/
	int x=0;
	//CString str; str.Format( "%f, %f, %f", fResult1a, fResult1b, fResult2 );
	//AfxMessageBox( str );
}
*/
float CEOSAIDesireSpatial::GetDistance( CEOSAILocation Location )
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	//return pWorldBuildDesc->GetPixelDistance( this->GetLocation(), Location );
	return g_pWorldDistanceTool->GetDistance( this->GetLocation(), Location );
}
/*
long CEOSAIDesireSpatial::GetMultiRegionId()
{
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	ASSERT( pWorldDescServer );
	CMultiRegion* pMultiRegion = pWorldDescServer->GetMultiRegionManager()->GetMultiRegion( GetLocation() );
	ASSERT( pMultiRegion );

	return pMultiRegion->m_iId;
}
*/
CEOSAIMultiRegion2* CEOSAIDesireSpatial::GetMultiRegion()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//ASSERT( pWorldDescServer );
	CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( GetLocation() );
	ASSERT( pMultiRegion );
	return pMultiRegion;
}

bool CEOSAIDesireSpatial::IsOnLand()
{
	CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( GetLocation() );
	ASSERT( pMultiRegion );
	return pMultiRegion->IsLand() || pMultiRegion->IsCity();
	//return g_pEOSAICommonData->GetAIGeo( GetLocation() )->GetGeoType() == CEOSAIGeo::Land;
	//ASSERT( false ); return false;
}


float CEOSAIDesireSpatial::GetDistanceFromMyClosestAirbase()
{
	float fDistanceFromMyClosestAirbase = 1000000.0f;
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );

		if( pAIPoiObject &&
			( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
			  pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Airfield ) &&
			( pAIPoiObject->GetForeignRelationsTo( iAIPlayer ) >= EOSAIEnumForeignRelations::enum_Alliance ) )
		{
			float fDistance = g_pWorldDistanceTool->GetDistance( pAIPoiObject->GetLocation(), GetLocation() );
			fDistanceFromMyClosestAirbase = min( fDistance, fDistanceFromMyClosestAirbase );
		}
	}
	return fDistanceFromMyClosestAirbase;
}

float CEOSAIDesireSpatial::GetDistanceFromMyClosestCity()
{
	float fDistanceFromMyClosestCity = 1000000.0f;
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );

		if( pAIPoiObject &&
			pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City &&
			pAIPoiObject->GetOwner() == iAIPlayer )
		{
			float fDistance = g_pWorldDistanceTool->GetDistance( pAIPoiObject->GetLocation(), GetLocation() );
			fDistanceFromMyClosestCity = min( fDistance, fDistanceFromMyClosestCity );
		}
	}
	return fDistanceFromMyClosestCity;
}

float CEOSAIDesireSpatial::GetDistanceFromMyClosestCitRes()
{
	float fDistanceFromMyClosestCitRes = 1000000.0f;
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		//CEOSAIPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );

		if( pAIPoiObject &&
			pAIPoiObject->GetOwner() == iAIPlayer &&
		  ( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City || 
		    pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Resource ) )
		  //( dynamic_cast< CEOSAICity* >( pAIPoiObject ) || dynamic_cast< CEOSAIResource* >( pAIPoiObject ) ) )
		{
			float fDistance = g_pWorldDistanceTool->GetDistance( pAIPoiObject->GetLocation(), GetLocation() );
			fDistanceFromMyClosestCitRes = min( fDistance, fDistanceFromMyClosestCitRes );
		}
	}
	return fDistanceFromMyClosestCitRes;
}
/*
bool CEOSAIDesireSpatial::IsOnTheSameGeoAsMyCitRes()
{
	long iDesireGeo = GetMultiRegion()->GetGeoId();

	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );

		if( pPoiObject &&
			pPoiObject->GetOwner() == iAIPlayer &&
		  ( dynamic_cast< CCity* >( pPoiObject ) && dynamic_cast< CResourceSource* >( pPoiObject ) ) )
		{
			if( iDesireGeo == pPoiObject->GetGeo() ) return true;
		}
	}
	return false;
}
*/
void CEOSAIDesireSpatial::CreateConnectedDesire( long iPlayer, CEOSAIDesireSpatial* pSpatialDesire )
{
	ASSERT( this != pSpatialDesire );

	long iSynergy = 1;
	bool bOneIsCaptureCity = false;
	bool bOneIsDamageCity = false;
	if( GetType() == EnumCaptureCity || pSpatialDesire->GetType() == EnumCaptureCity ) bOneIsCaptureCity = true;
	if( GetType() == EnumDegradeCity || pSpatialDesire->GetType() == EnumDegradeCity ) bOneIsDamageCity = true;
	if( bOneIsCaptureCity && bOneIsDamageCity ) iSynergy = -1;

	CEOSAIDesireConnection* pConnection = new CEOSAIDesireConnection();
	pConnection->Calculate( iPlayer, this, pSpatialDesire->GetLocation() );
	//pConnection->Calculate( iPlayer, GetLocation(), pSpatialDesire->GetLocation() );
	pConnection->m_iSynergy = iSynergy;
	pConnection->m_pConnectedAISpatialDesire = pSpatialDesire;
	m_ConnectedDesires.AddTail( pConnection );
}

float CEOSAIDesireSpatial::GetEnemyCombatPowerWithinRange( float fRange ) // Checks the connected desires
{
	float fPower = 0.0f;
	POSITION pos = m_ConnectedDesires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireConnection* pConnection = m_ConnectedDesires.GetNext( pos );
		if( pConnection->m_fAirDistance < fRange )
		{
			CAIDesireDestroyUnit* pNearbyUnit = dynamic_cast< CAIDesireDestroyUnit* >( pConnection->m_pConnectedAISpatialDesire );
			if( pNearbyUnit &&
				pNearbyUnit->CurrentForeignRelationsPreventsPersuingThisDesire() == false )
			{
				CEOSAIUnit* pTargetAIUnit = dynamic_cast< CEOSAIUnit* >( pNearbyUnit->m_pAITarget );
				ASSERT( pTargetAIUnit );
				fPower += pTargetAIUnit->GetAIUnitTemplate()->GetAIUnitCombatCapability()->GetCombatSignificance();
			}
		}
	}
	return fPower;
}

/*
void  CEOSAIDesireSpatial::GetSingleGeoConnectedDesires( CList< CEOSAIDesireSpatial* >* pConnectedDesires )
{
	POSITION pos = m_ConnectedDesires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireConnection* pConnection = m_ConnectedDesires.GetNext( pos );
		CEOSAIDesireSpatial* pOtherDesire = pConnection->m_pConnectedAISpatialDesire;
		ASSERT( this != pOtherDesire );

		if( pConnection->m_iMinimumGeoSwitches > 0 ) continue;

		if( GetType() == EnumCaptureCity )
		{
			if( pOtherDesire->GetType() == EnumCaptureCity ||
				pOtherDesire->GetType() == EnumCaptureResource ||
				pOtherDesire->GetType() == EnumDestroyUnit )
			{
				pConnectedDesires->AddTail( pOtherDesire );
			}
			eif( pOtherDesire->GetType() == EnumDegradeCity ||
				 pOtherDesire->GetType() == EnumBuildAirfield ||
				 pOtherDesire->GetType() == EnumDestroyAirfield )
			{
				//
			}
			else
			{
				ASSERT( false );
			}
		}
	}
}
*/
float CEOSAIDesireSpatial::GetAreaSimpleInterest() // based on my RawScore + RawScores of connected
{
	// I multiply all connected tasks by 0.9 so that the primary task is the most important one
	//   Example: if I want to capture a city with an infantry, the "capture city" task should
	//   be more important than the "destroy infantry".  Without a 1.1 multiplier, the "destroy infantry"
	//   will have the same AreaSimpleInterest as "capture city".
	float fRawScore = GetSimpleInterest();
	if( fRawScore == 0.0f ) return 0.0f;

	POSITION pos = m_ConnectedDesires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireConnection* pConnection = m_ConnectedDesires.GetNext( pos );
		if( pConnection->m_iSynergy == 1 )
		{
			float fGeoSwitchMultiplier = 1.0f;
			if( pConnection->m_iMinimumGeoSwitches > 1 ) fGeoSwitchMultiplier = 0.7f;
			//
			fRawScore += 
				0.9f * // multiply this by 0.9 so that the primary task is weighted more heavily
				fGeoSwitchMultiplier *
				pConnection->GetConnectionValue01() *
				pConnection->m_pConnectedAISpatialDesire->GetSimpleInterest();
		}
	}
	return fRawScore;
}


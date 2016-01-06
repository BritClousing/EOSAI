
#include "stdafx.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAIUnitPathwayResultStep.h"
#include "PoiObject.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAIRegionManager2.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIBrain.h"
#include "City.h"
#include "EOSAIUnit2.h"
#include "EOSAIUnitPathwayResult.h"
//#include "WorldDescServer.h"
#include "EOSAICommonData.h"
#include "EOSAIMath.h"
#include <math.h>
//#include "EOSAIMath.h"
#ifdef _DEBUG
#include "EOSAIUnitActionIdea.h"
#include "EOSAITacticalProject2.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
// 0.0 = e1 is best, 0.5 = are equal, 1.0 = e2 is best
/*
float  CEOSAIUnitPathwayPredefinedStep::CompareResultTimes( EnumPathwayResultStep e1, EnumPathwayResultStep e2 )
{
	EOSAI::UnitPathwayResultStep* p1 = GetResult( e1 );
	EOSAI::UnitPathwayResultStep* p2 = GetResult( e2 );
	ASSERT( p1 );
	ASSERT( p2 );
	if( p1 && p2 )
	{
		if( p1->m_bIsValid && p2->m_bIsValid )
		{
			return p1->m_fStepTime / ( p1->m_fStepTime + p2->m_fStepTime );
		}
		else if( p1->m_bIsValid )
		{
			return 0.0f;
		}
		else if( p2->m_bIsValid )
		{
			return 1.0f;
		}
		ASSERT( false );
		return 0.5f;
	}
	else if( p1 )
	{
		return 0.0f;
	}
	else if( p2 )
	{
		return 1.0f;
	}
	ASSERT( false );
	return 0.5f;
}
*/
/*
CEOSAILocation  CEOSAIUnitPathwayPredefinedStepSeq::GetEndLocation()
{
	return m_pPredefinedStep->GetSubStepEndLocation( m_iSubStep );
}
*/
CEOSAIUnitPathwayPredefinedStep::CEOSAIUnitPathwayPredefinedStep( CEOSAIUnitPathwayFinder* pAIUnitPathwayFinder )
{
	m_pAIUnitPathwayFinder = pAIUnitPathwayFinder;
	m_pAIUnitActionIdea = NULL;
	m_eTask = enum_Undefined;
	m_pAITarget = NULL;
	m_fDirectPathDistance = 1000000.0f;
	m_fCrowsFlightDistance = 1000000.0f;
	m_fCrowsFlightEstimatedMovementTime = 0.0f;
	m_bAvoidDangerousPathways = false;
	//m_fStartTime = 0.0f;

	// Used by Transportees
	m_iImaginaryTransport_PickupGroundAIRegion = 0;
	m_iImaginaryTransport_PickupWaterAIRegion = 0;
	m_iImaginaryTransport_DropoffGroundAIRegion = 0;
	m_iImaginaryTransport_DropoffWaterAIRegion = 0;
	m_pTransportToUse = NULL;

	// Used by Transports
	m_iRealTransport_PickupGroundAIRegion = 0;
	m_iRealTransport_PickupWaterAIRegion = 0;
	m_iRealTransport_DropoffGroundAIRegion = 0;
	m_iRealTransport_DropoffWaterAIRegion = 0;
	m_pTransporteeStep = NULL;
	m_pTransportPickupStep = NULL;
	m_pTransportDropoffStep = NULL;

	m_fRedMovementPath_ArrivalOffsetTimeAtPickupLocation = 0.0f;
	m_fRedMovementPath_ArrivalOffsetTimeAtDropoffLocation = 0.0f;

	m_bDirectRedMovementPathIsValid = false;
	m_bPreTransportRedMovementPathIsValid = false;
	m_bPostTransportRedMovementPathIsValid = false;
	//m_bDirectMovementStepTimeIsAccurate = false;
	//m_bPreTransportMovementStepTimeIsAccurate = false;
	//m_bPostTransportMovementStepTimeIsAccurate = false;

	m_fDirectMovementStepTime = 0.0f;
	m_fPreTransportMovementStepTime = 0.0f;
	//m_fPreTransportMovementStepEndTime = 0.0f;
	m_fPostTransportMovementStepTime = 0.0f;
	//m_fPostTransportMovementStepEndTime = 0.0f;
	//m_fStepEndTime = 0.0f;
	//m_fStepTime = 0.0f;
}

CEOSAIUnitPathwayPredefinedStep::~CEOSAIUnitPathwayPredefinedStep()
{
	while( m_DirectRedMovementPath.IsEmpty() == FALSE ){ delete m_DirectRedMovementPath.RemoveHead(); }
	while( m_PreTransportRedMovementPath.IsEmpty() == FALSE ){ delete m_PreTransportRedMovementPath.RemoveHead(); }
	while( m_PostTransportRedMovementPath.IsEmpty() == FALSE ){ delete m_PostTransportRedMovementPath.RemoveHead(); }
}

EOSAI::UnitPathwayResultStep*  CEOSAIUnitPathwayPredefinedStep::GetResultStep()
{
	if( m_pAIUnitPathwayFinder->GetResultPath() )
	{
		POSITION pos = m_pAIUnitPathwayFinder->GetResultPath()->m_Steps.GetHeadPosition();
		while( pos )
		{
			EOSAI::UnitPathwayResultStep* pResultStep = m_pAIUnitPathwayFinder->GetResultPath()->m_Steps.GetNext( pos );
			if( pResultStep->m_pPredefinedStep == this ) return pResultStep;
		}
	}
	return NULL;
}

CEOSAIUnitPathwayPredefinedStep*  CEOSAIUnitPathwayPredefinedStep::GetPrevStep()
{
	CEOSAIUnitPathwayPredefinedStep* pPrevStep = NULL;
	POSITION pos = m_pAIUnitPathwayFinder->GetPreDefinedPath()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_pAIUnitPathwayFinder->GetPreDefinedPath()->GetNext( pos );
		if( pExistingStep == this ){ return pPrevStep; }
		pPrevStep = pExistingStep;
	}
	return NULL;
}

CEOSAIUnitPathwayPredefinedStep*  CEOSAIUnitPathwayPredefinedStep::GetNextStep()
{
	POSITION pos = m_pAIUnitPathwayFinder->GetPreDefinedPath()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_pAIUnitPathwayFinder->GetPreDefinedPath()->GetNext( pos );
		if( pExistingStep == this )
		{
			if( pos )
			{
				return m_pAIUnitPathwayFinder->GetPreDefinedPath()->GetNext( pos );
			}
			return NULL;
		}
	}
	return NULL;
}

void  CEOSAIUnitPathwayPredefinedStep::SetStartLocation( CEOSAILocation StartLocation )
{
	if( m_StartLocation != StartLocation )
	{
		m_StartLocation = StartLocation;
		UpdateStartToEndDistancesAndTimes();
	}
}

void  CEOSAIUnitPathwayPredefinedStep::SetEndLocation( CEOSAILocation EndLocation )
{
	if( fabs( EndLocation.GetRealX() - 506.0f ) < 0.1f && fabs( EndLocation.GetRealY() - 1058.0f ) < 0.1f ){ ASSERT( false ); }

	if( m_EndLocation != EndLocation )
	{
		// Clear the Map-pathway information
		CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
		CEOSAIRegion2* pOldAIRegion = pAIRegionManager->GetAIRegion( m_EndLocation );
		CEOSAIRegion2* pNewAIRegion = pAIRegionManager->GetAIRegion( EndLocation );
		if( pOldAIRegion != pNewAIRegion )
		{
			ClearCachedMapData();
		}
	}
	m_EndLocation = EndLocation;
	UpdateStartToEndDistancesAndTimes();

	// update next start
	CEOSAIUnitPathwayPredefinedStep* pNextStep = GetNextStep();
	if( pNextStep )
	{
		pNextStep->SetStartLocation( EndLocation );
	}
}

void  CEOSAIUnitPathwayPredefinedStep::UpdateStartToEndDistancesAndTimes()
{
	/*
	if( m_pAIUnitActionIdea &&
		m_pAIUnitActionIdea->GetAIPoiObjectActor()->GetObjectId() == 151 &&
		m_pAIUnitActionIdea->GetTacticalProject2()->ProjectIncludesObjectIdAsPrimaryTask(89) )
	{
		int h=0;
	}
	*/

	// There are times when there is a land-path (North Africa->Europe) but it's VERY slow
	// Crows-flight distance
	//SetCrowsFlightDistance( GetCommonState()->GetWorldDescServer()->GetPixelDistance( this->GetStartLocation(), this->GetEndLocation() ) );
	SetCrowsFlightDistance( g_pWorldDistanceTool->GetDistance( this->GetStartLocation(), this->GetEndLocation() ) );
	//
	if( GetAIUnitPathwayFinder()->GetAIUnitTemplate()->IsGroundUnit() )
	{
		CEOSAIRegionPathwayMap* pEndLocationToEverywhere = this->InvokeEndLocationToEverywhereDirectMap();
		CEOSAIRegionPathwayItem* pStartPointPathwayItem = pEndLocationToEverywhere->GetRegionResult( this->GetStartLocation() );
		SetDirectPathDistance( 1000000.0f );
		if( pStartPointPathwayItem ){ this->SetDirectPathDistance( pStartPointPathwayItem->m_fDistance ); }
	}
	//
	if( GetAIUnitPathwayFinder()->GetAIUnitTemplate()->IsGroundUnit() )
	{
		//CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIUnitPathwayFinder()->GetAIBrain()->GetAIThoughtDatabase();
		CEOSAIThoughtDatabase* pAIThoughtDatabase = g_pEOSAICommonData->GetAIThoughtDatabase( GetAIUnitPathwayFinder()->GetUnitOwner() );
		//CEOSAIThoughtDatabase* pAIThoughtDatabase = g_pEOSAIInterface->GetAIPlayer(m_iUnitOwner)->GetAIBrain()->GetAIThoughtDatabase();
		float fFastestTransport = pAIThoughtDatabase->GetUnitsICanBuildOrHave()->GetFastestTransportMovementRate( GetAIUnitPathwayFinder()->GetAIUnitTemplate() );
		/*
		float fMovementRate = 
			0.3f * GetAIUnitPathwayFinder()->GetUnitTemplate()->GetMovementRate()+0.1f) +
			0.7f * max( fFastestTransport, GetAIUnitPathwayFinder()->GetUnitTemplate()->GetMovementRate()+0.1f );
		m_fEstimatedMovementTime = m_fCrowsFlightDistance / fMovementRate;
		*/
		m_fCrowsFlightEstimatedMovementTime = m_fCrowsFlightDistance / (GetAIUnitPathwayFinder()->GetAIUnitTemplate()->GetMovementRate()+0.1f);
		if( m_fCrowsFlightEstimatedMovementTime > 2.0f )
		{
			float fTransportPercentage = 0.7f;
			if( m_fCrowsFlightEstimatedMovementTime < 3.0f )
			{
				fTransportPercentage = 0.7f * (m_fCrowsFlightEstimatedMovementTime-2.0f);
			}
			float fMovementRate = 
				(1.0f - fTransportPercentage) * (GetAIUnitPathwayFinder()->GetAIUnitTemplate()->GetMovementRate()+0.1f) +
				(       fTransportPercentage) * max( fFastestTransport, GetAIUnitPathwayFinder()->GetAIUnitTemplate()->GetMovementRate()+0.1f );
			m_fCrowsFlightEstimatedMovementTime = m_fCrowsFlightDistance / fMovementRate;
		}
	}
	else
	{
		m_fCrowsFlightEstimatedMovementTime = m_fCrowsFlightDistance / ( GetAIUnitPathwayFinder()->GetAIUnitTemplate()->GetMovementRate()+0.1f );
	}
	/*
	if( GetAIUnitPathwayFinder()->GetUnitTemplate()->IsAirUnit() )
	{
		if( this->IsALandAtAirbase() ){ m_fEstimatedMovementTime = CEOSAIMath::RoundUp( m_fEstimatedMovementTime ); }
	}
	*/
}
/*
float CEOSAIUnitPathwayPredefinedStep::GetEstimatedPickupTime()
{
	ASSERT( this->RequiresTransport() );
}
*/

float CEOSAIUnitPathwayPredefinedStep::GetStartTime()
{
	CEOSAIUnitPathwayPredefinedStep* pPreviousStep = GetPrevStep();
	if( pPreviousStep )
	{
		//return pPreviousStep->GetEndTime();
		return pPreviousStep->GetRedPathEndTime( enum_EndOfStep );//GetEndTime();
	}
	else
	{
		return GetAIUnitPathwayFinder()->GetStartTime();
	}
}

/*
float CEOSAIUnitPathwayPredefinedStep::GetTransportTimeToPickupLocation()
{
	ASSERT( this->IsAPickup() );
	if( IsAPickup() )
	{
		CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep = this->GetTransporteeStep();
		float fGroundUnitArrivalTime = pGroundUnitStep->GetGroundUnitTimeToPickupLocation();

		/-*
		float fTransportArrivalTime = 0.0f;
		CEOSAIUnitPathwayPredefinedStep* pPreviousStep = GetPrevStep();
		if( pPreviousStep )
		{
			fTransportArrivalTime = this->GetStepTime() + pPreviousStep->GetEndTime();
		}
		else
		{
			fTransportArrivalTime = this->GetStepTime() + GetAIUnitPathwayFinder()->GetStartTime();
		}
		return fTransportArrivalTime;
		*-/
		return GetStepTime() + GetStartTime();
	}
	return 0.0f;
}
*/
/*
float CEOSAIUnitPathwayPredefinedStep::GetEndTime()
{
	return m_fStepEndTime;

	// How to solve:
	//   Calculate the time taken by this step.  Add it to the time taken by the previous step.
	float fEndTime = 0.0f;
	CUnitTemplate* pUnitTemplate = m_pAIUnitPathwayFinder->GetUnitTemplate();

	EOSAI::UnitPathwayResultStep* pResultStep = this->GetResultStep();
	if( pResultStep )
	{
		return pResultStep->GetEndTime();
	}

	// If this step uses a transport, then figure out the Transport-dropoff time + Dropoff->Target time
	if( this->GetTransportToUse() )
	{
		CEOSAIUnitPathwayPredefinedStep* pTransportDropoffPredefinedStep = this->GetTransportToUse()->GetAIUnitPathwayFinder()->GetTransportDropoffStep( this );
		ASSERT( pTransportDropoffPredefinedStep );
		float fTransportDropoffEndTime = pTransportDropoffPredefinedStep->GetEndTime();

		float fDropoffToTargetTime = 0.0f;
		if( GetResultStep() )
		{
			fDropoffToTargetTime = GetResultStep()->GetGroundUnitDropoffTimeToTargetTime();
		}
		else
		{
			float fDropoffToTargetDistance = GetCommonState()->GetWorldDescServer()->GetPixelDistance( pTransportDropoffPredefinedStep->GetEndLocation(), this->GetEndLocation() );
			fDropoffToTargetTime = fDropoffToTargetDistance / (pUnitTemplate->GetMovementRate()+0.1f);
		}

		return fTransportDropoffEndTime + fDropoffToTargetTime;
	}
	// If this is a transport, and I am picking-up a unit, check my arrival time and GroundUnit arrival time
	else if( this->IsAPickup() )
	{
		CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep = this->GetTransporteeStep();
		float fGroundUnitArrivalTime = pGroundUnitStep->GetGroundUnitTimeToPickupLocation();

		float fTransportArrivalTime = GetStepTime() + GetStartTime();
		/-*
		float fTransportArrivalTime = 0.0f;
		CEOSAIUnitPathwayPredefinedStep* pPreviousStep = GetPrevStep();
		if( pPreviousStep )
		{
			fTransportArrivalTime = this->GetStepTime() + pPreviousStep->GetEndTime();
		}
		else
		{
			fTransportArrivalTime = this->GetStepTime() + GetAIUnitPathwayFinder()->GetStartTime();
		}
		*-/
		return max( fGroundUnitArrivalTime, fTransportArrivalTime );
	}
	else
	{
		float fEndTime = GetStepTime() + GetStartTime();
		/-*
		CEOSAIUnitPathwayPredefinedStep* pPreviousStep = GetPrevStep();
		if( pPreviousStep )
		{
			fEndTime = this->GetStepTime() + pPreviousStep->GetEndTime();
		}
		else
		{
			fEndTime = this->GetStepTime() + GetAIUnitPathwayFinder()->GetStartTime();
		}
		*-/

		if( pUnitTemplate->IsAirUnit() && this->IsALandAtAirbase() )
		{
			fEndTime = CEOSAIMath::RoundUp( fEndTime );
		}
		return fEndTime;
	}
/-*
	CUnitTemplate* pUnitTemplate = m_pAIUnitPathwayFinder->GetUnitTemplate();
	float fEndTime = m_fStartTime;
	POSITION pos = m_pAIUnitPathwayFinder->GetPreDefinedPath()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_pAIUnitPathwayFinder->GetPreDefinedPath()->GetNext( pos );

		// If I am a GroundUnit using a Transport, then I have to wait for the transport to move me
		if( pPredefinedStep->GetTransportToUse() )
		{
			CEOSAIUnitPathwayPredefinedStep* pTransportDropoffPredefinedStep = pPredefinedStep->GetTransportToUse()->GetAIUnitPathwayFinder()->GetTransportDropoffStep( pPredefinedStep );
			ASSERT( pTransportDropoffPredefinedStep );
			float fTransportDropoffEndTime = pTransportDropoffPredefinedStep->GetEndTime();

			float fDropoffToTargetDistance =
				GetCommonState()->GetWorldDescServer()->GetPixelDistance( pTransportDropoffPredefinedStep->GetEndLocation(), pPredefinedStep->GetEndLocation() );
			float fDropoffToTargetTime =
				fDropoffToTargetDistance/(pUnitTemplate->GetMovementRate()+0.1f);

			fEndTime = fTransportDropoffEndTime + fDropoffToTargetTime;
		}
		// If I am a Transport picking up a GroundUnit, then I have to wait for the GroundUnit to arrive
		if( pPredefinedStep->IsAPickup() )
		{
			fEndTime += pPredefinedStep->GetEstimatedMovementTime();
		}
		// Otherwise:
		else
		{
			fEndTime += pPredefinedStep->GetEstimatedMovementTime();
		}

		if( pPredefinedStep == this ) break;

		if( pUnitTemplate->IsAirUnit() && pPredefinedStep->IsALandAtAirbase() )
		{
			fEndTime = CEOSAIMath::RoundUp( fEndTime );
		}
	}
*-/
	//return fEndTime;
}
*/

/*
float CEOSAIUnitPathwayPredefinedStep::GetStepTime()
{
	EOSAI::UnitPathwayResultStep* pResultStep = GetResultStep();
	if( pResultStep )
	{
		return pResultStep->GetStepTime();
	}
	else
	{
		return m_fCrowsFlightEstimatedMovementTime;
	}
}
*/

/*
float CEOSAIUnitPathwayPredefinedStep::GetEndTime() // EstimatedEndTime, or the ResultStep's EndTime
{
	if( GetResultStep() ) return GetResultStep()->GetEndTime();
	return GetEstimatedEndTime();
}
*/
/*
float CEOSAIUnitPathwayPredefinedStep::GetGroundUnitTimeToPickupLocation() //GetTimeToPickup()
{
	CUnitTemplate* pUnitTemplate = this->GetAIUnitPathwayFinder()->GetUnitTemplate();
	ASSERT( this->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsGroundUnit() );
	ASSERT( this->RequiresTransport() );

	float fStepStartTime = this->GetAIUnitPathwayFinder()->GetStartTime();
	CEOSAIUnitPathwayPredefinedStep* pPreviousStep = this->GetPrevStep();
	if( pPreviousStep ){ fStepStartTime = pPreviousStep->GetEndTime(); }

	EOSAI::UnitPathwayResultStep* pResultStep = GetResultStep();
	if( pResultStep )
	{
		return fStepStartTime + pResultStep->GetGroundUnitStartTimeToPickupTime();
	}
	return fStepStartTime + GetStartToPickupDistance()/(pUnitTemplate->GetMovementRate()+0.1f);
}
*/
/*
float CEOSAIUnitPathwayPredefinedStep::GetGroundUnitWaitTimeAtPickupLocation()
{
	CUnitTemplate* pUnitTemplate = this->GetAIUnitPathwayFinder()->GetUnitTemplate();
	ASSERT( this->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsGroundUnit() );
	ASSERT( this->RequiresTransport() );
	ASSERT( this->m_pTransportToUse );

	float fGroundUnitArrivalTime = GetGroundUnitTimeToPickupLocation();
	CEOSAIUnitPathwayPredefinedStep* pTransportPickupStep = m_pTransportToUse->GetAIUnitPathwayFinder()->GetTransportPickupStep( this );
	float fTransportArrivalTime = pTransportPickupStep->GetTransportTimeToPickupLocation();

	return fTransportArrivalTime - fGroundUnitArrivalTime;
	/-*
	float fStepStartTime = this->GetAIUnitPathwayFinder()->GetStartTime();
	CEOSAIUnitPathwayPredefinedStep* pPreviousStep = this->GetPrevStep();
	if( pPreviousStep ){ fStepStartTime = pPreviousStep->GetEndTime(); }

	EOSAI::UnitPathwayResultStep* pResultStep = GetResultStep();
	if( pResultStep )
	{
		return fStepStartTime + pResultStep->GetGroundUnitStartTimeToPickupTime();
	}
	return fStepStartTime + GetStartToPickupDistance()/(pUnitTemplate->GetMovementRate()+0.1f);
	*-/
}
*/
void  CEOSAIUnitPathwayPredefinedStep::InvokeImaginaryTransportPath()
{
	CEOSAIUnitTemplate* pUnitTemplate = GetAIUnitPathwayFinder()->GetAIUnitTemplate();
	bool bRequiresTransport = this->RequiresTransport();
	//
	if( pUnitTemplate->IsGroundUnit() && 
		this->RequiresTransport() &&
		m_iImaginaryTransport_DropoffGroundAIRegion == 0 )
	{
		ASSERT( m_StartLocation.GetRealX() != 0.0f );

		CEOSAIRegionPathwayMap* pEndLocationToEverywhereByGroundSeaMap = InvokeEndLocationToEverywhereGroundUnitAndSeaTransportMap();
		//CEOSAIRegionPathwayMap* pEndLocationToEverywhereByAirMap = this->InvokeEndLocationToEverywhereByAirMap();
		//CEOSAIRegionPathwayItem* pCurrentRegionPathwayItem = pEndLocationToEverywhereByAirMap->GetRegionResult( pResultStep->m_StartLocation );
		CEOSAIRegionPathwayItem* pCurrentRegionPathwayItem = pEndLocationToEverywhereByGroundSeaMap->GetRegionResult( this->m_StartLocation );
		bool bOnInitialLand = true;
		bool bWasOnWater = true;
		long iPickupLandAIRegion = 0;
		long iPickupWaterAIRegion = 0;
		long iDropoffLandAIRegion = 0;
		long iDropoffWaterAIRegion = 0;
		//bool bGroundUnitIsOverWater = false;
		while( pCurrentRegionPathwayItem )
		{
			if( bOnInitialLand )
			{
				if( pCurrentRegionPathwayItem->m_pPathfinderPoint->IsCity() || 
					pCurrentRegionPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					iPickupLandAIRegion = pCurrentRegionPathwayItem->m_pPathfinderPoint->GetId();
				}
				eif( pCurrentRegionPathwayItem->m_pPathfinderPoint->IsWater() )
				{
					// If the ground-unit is over water, then there will be no Land Pickup AIRegion
					//if( iPickupLandAIRegion == 0 )
					//{
					//	bGroundUnitIsOverWater = true;
					//	iPickupLandAIRegion = pCurrentRegionPathwayItem->m_pPathfinderPoint->GetId();
					//}
					iPickupWaterAIRegion = pCurrentRegionPathwayItem->m_pPathfinderPoint->GetId();
					iDropoffWaterAIRegion = pCurrentRegionPathwayItem->m_pPathfinderPoint->GetId();
					bOnInitialLand = false;
				}
			}
			else
			{
				if( pCurrentRegionPathwayItem->m_pPathfinderPoint->IsCity() || 
					pCurrentRegionPathwayItem->m_pPathfinderPoint->IsLand() )
				{
					if( iDropoffLandAIRegion == 0 )
					{
						iDropoffLandAIRegion = pCurrentRegionPathwayItem->m_pPathfinderPoint->GetId();
					}
				}
				else if( pCurrentRegionPathwayItem->m_pPathfinderPoint->IsWater() )
				{
					iDropoffWaterAIRegion = pCurrentRegionPathwayItem->m_pPathfinderPoint->GetId();
					iDropoffLandAIRegion = 0;
				}
			}
			//
			pCurrentRegionPathwayItem = pEndLocationToEverywhereByGroundSeaMap->GetRegionResult( pCurrentRegionPathwayItem->m_iPreviousPathwayItem );
		}
		/*
		if( bGroundUnitIsOverWater )
		{
			// I had to write this code to solve a specific problem
			// If the ground unit is on a road above the water, then there won't be a proper pickup AIRegion
			// There are no pickup/dropoff locations, so the GroundUnit never gets picked up.
			// So, I do a search of adjacent AIRegions for land and say the pickup/dropoff is there.
			// This will (hopefully) give me a proper pickup/dropoff.
			// I don't really like this solution, since it might actually give me an inaccessible AIRegion.
			CEOSAIGenericRegionManager* pRegionManager = pEndLocationToEverywhereByGroundSeaMap->GetRegionManager();
			CEOSAIRegionManager2* pAIRegionManager = dynamic_cast< CEOSAIRegionManager2* >( pRegionManager );
			ASSERT( pAIRegionManager );
			CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( iPickupLandAIRegion );
			if( pAIRegion )
			{
				POSITION pos = pAIRegion->GetAdjacentRegions()->GetHeadPosition();
				while( pos )
				{
					CEOSAIAdjacentRegionDesc* pDesc = pAIRegion->GetAdjacentRegions()->GetNext( pos );
					if( pDesc->m_pRegion->IsCity() || pDesc->m_pRegion->IsLand() )
					{
						iPickupLandAIRegion = pDesc->m_pRegion->GetId();
						break;
					}
				}
			}
		}
		*/

		//ASSERT( iPickupLandAIRegion > 0 ); - this can happen if the ground unit is already in the transport
		ASSERT( iPickupWaterAIRegion > 0 );
		ASSERT( iDropoffLandAIRegion > 0 );
		ASSERT( iDropoffWaterAIRegion > 0 );

		SetImaginaryTransport_PickupGroundAIRegion( iPickupLandAIRegion );
		SetImaginaryTransport_PickupWaterAIRegion( iPickupWaterAIRegion );
		SetImaginaryTransport_DropoffGroundAIRegion( iDropoffLandAIRegion );
		SetImaginaryTransport_DropoffWaterAIRegion( iDropoffWaterAIRegion );
		//ASSERT( iDropoffWaterAIRegion != 1189 );
		int g=0;
	}
}

void  CEOSAIUnitPathwayPredefinedStep::AvoidDangerousPathways( bool b )
{
	if( m_bAvoidDangerousPathways == b ) return; // Nothing changed
	m_bAvoidDangerousPathways = b;
	ClearCachedMapData();
}
/*
CEOSAILocation CEOSAIUnitPathwayPredefinedStep::GetSubStepEndLocation( long iSubStep )
{
	if( m_eTask == enum_Transport )
	{
		if( iSubStep == 1 ) return m_TransporteeStartLocation;
		if( iSubStep == 2 ) return m_TransporteeEndLocation;
	}
	return m_EndLocation;
}
*/
void  CEOSAIUnitPathwayPredefinedStep::GotoWaypoint( CEOSAILocation WaypointLocation )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_Waypoint;
	m_EndLocation = WaypointLocation;
	ClearCachedMapData();
}
/*
void  CEOSAIUnitPathwayPredefinedStep::Attack( CEOSAIPoiObject* pAITarget )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_AttackUnit;
	m_pAITarget = pAITarget;
	m_EndLocation = m_pAITarget->GetInitialState()->GetLocation();
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::CaptureCitRes( CEOSAIPoiObject* pAITarget )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_CaptureCitRes;
	m_pAITarget = pAITarget;
	m_EndLocation = m_pAITarget->GetInitialState()->GetLocation();
	ClearCachedMapData();
}
*/
void  CEOSAIUnitPathwayPredefinedStep::BuildAirfield( CEOSAILocation BuildLocation )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_BuildAirfield;
	m_EndLocation = BuildLocation;
	ClearCachedMapData();
}
/*
void  CEOSAIUnitPathwayPredefinedStep::Pickup( CEOSAIPoiObject* pAIPoiObjectToPickup, CEOSAILocation TransportEndLocation )
{
	//pStep->Pickup( pAIUnitToPickup, OtherUnitLocation );
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget;
	m_pAITarget = pAIUnitToPickup;
	m_EndLocation = TransportEndLocation;
}

void  CEOSAIUnitPathwayPredefinedStep::Dropoff( CEOSAIPoiObject* pAIPoiObjectToDropoff, CEOSAILocation TransporteeEndLocation )
{
	//pStep->Pickup( pAIUnitToPickup, OtherUnitLocation );
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget;
	m_pAITarget = pAIPoiObjectToDropoff;
	m_EndLocation = TransporteeEndLocation;
}
*/
/*
void  CEOSAIUnitPathwayPredefinedStep::Transport( CEOSAIPoiObject* pAITransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeEndLocation )
{
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_Transport;
	m_pAITarget = pAITransportee;
	m_TransporteeStartLocation = TransporteeStartLocation;
	m_TransporteeEndLocation = TransporteeEndLocation;
	ClearCachedMapData();
}
*/
void  CEOSAIUnitPathwayPredefinedStep::Pickup( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep )
{
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget;
	m_pAITarget = pTransporteeStep->GetAIUnitPathwayFinder()->GetAIPoiObjectActor();
	m_pTransporteeStep = pTransporteeStep;
	pTransporteeStep->m_pTransportPickupStep = this;

	// This is fake data - it's actually the location of the GroundUnit (at the start of their step)
	if( GetAIUnitPathwayFinder()->GetAIUnitTemplate()->IsSeaUnit() )
	{
		if( pTransporteeStep->m_iImaginaryTransport_PickupGroundAIRegion > 0 &&
			pTransporteeStep->m_iImaginaryTransport_PickupWaterAIRegion > 0 )
		{
			m_EndLocation = pTransporteeStep->GetImaginaryTransportPickupLocation();
			//if( fabs( m_EndLocation.GetRealX() - 506.0f ) < 0.1f && fabs( m_EndLocation.GetRealY() - 1058.0f ) < 0.1f ){ ASSERT( false ); }
			int h=0;
		}
		else
		{
			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}
		}
	}
	else
	{
		m_EndLocation = pTransporteeStep->GetStartLocation();
		//if( fabs( m_EndLocation.GetRealX() - 506.0f ) < 0.1f && fabs( m_EndLocation.GetRealY() - 1058.0f ) < 0.1f ){ ASSERT( false ); }
	}

	//m_TransporteeStartLocation = pAITransporteePredefinedStep->m_StartLocation; //TransporteeStartLocation;
	//m_TransporteeEndLocation = pAITransporteePredefinedStep->m_EndLocation; //TransporteeEndLocation;
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::Dropoff( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep )
{
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget;
	m_pAITarget = pTransporteeStep->GetAIUnitPathwayFinder()->GetAIPoiObjectActor();
	m_pTransporteeStep = pTransporteeStep;
	pTransporteeStep->m_pTransportDropoffStep = this;

	// This is fake data - it's actually the location of the GroundUnit's target
	m_EndLocation = pTransporteeStep->GetEndLocation();
	if( GetAIUnitPathwayFinder()->GetAIUnitTemplate()->IsSeaUnit() )
	{
		if( pTransporteeStep->m_iImaginaryTransport_DropoffGroundAIRegion > 0 )
		{
			m_EndLocation = pTransporteeStep->GetImaginaryTransportDropoffLocation();
			if( fabs( m_EndLocation.GetRealX() - 506.0f ) < 0.1f && fabs( m_EndLocation.GetRealY() - 1058.0f ) < 0.1f ){ ASSERT( false ); }
			int h=0;
		}
		else
		{
			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}
		}
	}
	else
	{
		m_EndLocation = pTransporteeStep->GetEndLocation();
		if( fabs( m_EndLocation.GetRealX() - 506.0f ) < 0.1f && fabs( m_EndLocation.GetRealY() - 1058.0f ) < 0.1f ){ ASSERT( false ); }
	}

	//m_TransporteeStartLocation = pAITransporteePredefinedStep->m_StartLocation; //TransporteeStartLocation;
	//m_TransporteeEndLocation = pAITransporteePredefinedStep->m_EndLocation; //TransporteeEndLocation;
	ClearCachedMapData();
}

/*
void  CEOSAIUnitPathwayPredefinedStep::Transport( CEOSAIUnitPathwayPredefinedStep* pAITransporteePredefinedStep )
{
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_Transport;
	m_pAITarget = pAITransporteePredefinedStep->GetAIUnitPathwayFinder()->GetAIPoiObjectActor();
	m_TransporteeStartLocation = pAITransporteePredefinedStep->m_StartLocation; //TransporteeStartLocation;
	m_TransporteeEndLocation = pAITransporteePredefinedStep->m_EndLocation; //TransporteeEndLocation;
	ClearCachedMapData();
}
*/

void  CEOSAIUnitPathwayPredefinedStep::LandAtAirbase( CEOSAIPoiObject* pAIAirbase )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_LandAtAirbase;
	m_pAITarget = pAIAirbase;
	m_EndLocation = m_pAITarget->GetInitialState()->GetLocation();
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::LandAtAnyAirbase()
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_LandAtAirbase;
	m_pAITarget = NULL;
	//m_EndLocation = m_pAITarget->GetInitialState()->GetLocation();
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::UpgradeAt( CEOSAIPoiObject* pAIPoiObject )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_Upgrade;
	m_pAITarget = pAIPoiObject;
	m_EndLocation = pAIPoiObject->GetInitialState()->GetLocation();
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::RepairAt( CEOSAIPoiObject* pAIPoiObject )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_Repair;
	m_pAITarget = pAIPoiObject;
	m_EndLocation = pAIPoiObject->GetInitialState()->GetLocation();
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::RepairAnywhere()
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = enum_Repair;
	//m_pAITarget = pAIAirbase;
	//m_EndLocation = m_pAITarget->GetInitialState()->GetLocation();
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::CaptureCitRes( CEOSAIPoiObject* pAITarget )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_CaptureCitRes;
	m_pAITarget = pAITarget;
	m_EndLocation = pAITarget->GetLocation();
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::DegradeTarget( CEOSAIPoiObject* pAITarget )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_DegradeCityOrAirfield;
	m_pAITarget = pAITarget;
	m_EndLocation = pAITarget->GetLocation();
	ClearCachedMapData();
}

void  CEOSAIUnitPathwayPredefinedStep::AttackUnit( CEOSAIUnit* pAITarget )
{
	ASSERT( m_eTask == enum_Undefined );
	m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_AttackUnit;
	m_pAITarget = pAITarget;
	m_EndLocation = pAITarget->GetLocation();
	ClearCachedMapData();
}

CEOSAILocation  CEOSAIUnitPathwayPredefinedStep::GetImaginaryTransportPickupLocation()
{
	ASSERT( m_iImaginaryTransport_PickupWaterAIRegion > 0 );
	CEOSAIRegion2* pPickupRegion =
		g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( m_iImaginaryTransport_PickupWaterAIRegion );
	return pPickupRegion->GetLocation();
}

CEOSAILocation  CEOSAIUnitPathwayPredefinedStep::GetImaginaryTransportDropoffLocation()
{
	ASSERT( m_iImaginaryTransport_DropoffWaterAIRegion > 0 );
	CEOSAIRegion2* pDropoffRegion =
		g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( m_iImaginaryTransport_DropoffWaterAIRegion );
	return pDropoffRegion->GetLocation();
}
/*
float CEOSAIUnitPathwayPredefinedStep::GetImaginaryTransport_DropoffToTargetDistance()
{
	ASSERT( m_iImaginaryTransport_DropoffWaterAIRegion > 0 );
	CEOSAIRegion2* pDropoffRegion =
		m_pAIUnitPathwayFinder->GetAIBrain()->GetAIRegionManager()->GetAIRegion( m_iImaginaryTransport_DropoffWaterAIRegion );
	return GetCommonState()->GetWorldDescServer()->GetPixelDistance( pDropoffRegion->GetLocation(), this->m_EndLocation );
}
*/

float CEOSAIUnitPathwayPredefinedStep::GetStartToPickupDistance() // Using Real or Imaginary Transport
{
	if( GetRealTransport_PickupGroundAIRegion() > 0 )
	{
		//return GetCommonState()->GetWorldDescServer()->GetPixelDistance( m_StartLocation, GetRealTransportPickupLocation() );
		return g_pWorldDistanceTool->GetDistance( m_StartLocation, GetRealTransportPickupLocation() );
	}
	if( m_iImaginaryTransport_PickupGroundAIRegion > 0 )
	{
		//return GetCommonState()->GetWorldDescServer()->GetPixelDistance( m_StartLocation, GetImaginaryTransportPickupLocation() );
		return g_pWorldDistanceTool->GetDistance( m_StartLocation, GetImaginaryTransportPickupLocation() );
	}
	ASSERT( false );
	return 0.0f;
}
/*
void  CEOSAIUnitPathwayPredefinedStep::UpdateEndLocation() // Pickup or Dropoff Locations can change
{
	if( m_eTask == enum_PickupTarget || m_eTask == enum_DropoffTarget )
	{
		m_pAIUnitPathwayFinder->UpdateEndLocation( this );
	}
}
*/
void  CEOSAIUnitPathwayPredefinedStep::UseTransportAndCreatePickupDropoffSteps( CEOSAIUnit* pTransportToUse )
{
	if( m_pTransportToUse == pTransportToUse ) return;

	// Remove any previous Transport Steps
	if( m_pTransportToUse )
	{
		m_pTransportToUse->GetAIUnitPathwayFinder()->DeleteTransportSteps( this );
	}

	m_pTransportToUse = pTransportToUse;

	// Create Pickup/Dropoff Steps
	pTransportToUse->GetAIUnitPathwayFinder()->InsertTransportSteps( this );

	// If there are result steps, then update those with this transport, too
	/*
	if( GetResultStep() )
	{
		GetResultStep()->UpdateWithTransportAssistedPath( pTransportToUse );
	}
	*/
}

CEOSAILocation  CEOSAIUnitPathwayPredefinedStep::GetRealTransportPickupLocation()
{
	if( EOSAISettings::m_bAssertWhenAIPathwayFails )
	{
		ASSERT( m_iImaginaryTransport_PickupWaterAIRegion > 0 );
	}
	CEOSAIRegion2* pPickupRegion =
		g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( m_iRealTransport_PickupWaterAIRegion );
	return pPickupRegion->GetLocation();
}

void  CEOSAIUnitPathwayPredefinedStep::ClearCachedMapData()
{
	m_EndLocationToEverywhereDirectMap.Clear();
	m_EndLocationToEverywhereDirectMapPlusCoasts.Clear();
	m_EndLocationToEverywhereGroundUnitAndSeaTransportMap.Clear();
	m_EndLocationToEverywhereByAirMap.Clear();
}

void CEOSAIUnitPathwayPredefinedStep::ClearDirectMovementPath()
{
	m_bDirectRedMovementPathIsValid = false;
	m_DirectMovementPath.RemoveAll();
}

void CEOSAIUnitPathwayPredefinedStep::AddToDirectMovementPathTail( CEOSAIPathfinderPoint* p )
{
	m_bDirectRedMovementPathIsValid = false;
	m_DirectMovementPath.AddTail( p );
}

void CEOSAIUnitPathwayPredefinedStep::ClearPreTransportMovementPath()
{
	m_bPreTransportRedMovementPathIsValid = false;
	m_PreTransportMovementPath.RemoveAll();
}

void CEOSAIUnitPathwayPredefinedStep::AddToPreTransportMovementPathHead( CEOSAIPathfinderPoint* p )
{
	m_bPreTransportRedMovementPathIsValid = false;
	m_PreTransportMovementPath.AddHead( p );
}

void CEOSAIUnitPathwayPredefinedStep::ClearPostTransportMovementPath()
{
	m_bPostTransportRedMovementPathIsValid = false;
	m_PostTransportMovementPath.RemoveAll();
}

void CEOSAIUnitPathwayPredefinedStep::AddToPostTransportMovementPathTail( CEOSAIPathfinderPoint* p )
{
	m_bPostTransportRedMovementPathIsValid = false;
	m_PostTransportMovementPath.AddTail( p );
}

float CEOSAIUnitPathwayPredefinedStep::GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation()
{
	return m_fRedMovementPath_ArrivalOffsetTimeAtPickupLocation;
}

float CEOSAIUnitPathwayPredefinedStep::GetRedMovementPath_ArrivalOffsetTimeAtDropoffLocation()
{
	return m_fRedMovementPath_ArrivalOffsetTimeAtDropoffLocation;
}

/*
float CEOSAIUnitPathwayPredefinedStep::GetGroundUnitArrivalTimeAtPickupLocation()
{
	if( this->GetTransportToUse() )
	{
		this->UpdateRedMovementPathTimingIfNecessary();
		return GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation();
	}
	if( this->IsAPickup() )
	{
		GetTransporteeStep()->UpdateRedMovementPathTimingIfNecessary();
		return GetTransporteeStep()->GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation();
	}
	ASSERT( false );
	return 0.0f;
}
*/
/*
float CEOSAIUnitPathwayPredefinedStep::GetTransportArrivalTimeAtPickupLocation()
{
	if( this->GetTransportToUse() )
	{
		GetTransportToUse()->UpdateRedPathTimingIfNecessary();
		CEOSAIUnitPathwayPredefinedStep* pPickupTask = GetTransportToUse()->GetPickupStep( this );
		return pPickupTask->GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation();
	}
	if( this->IsAPickup() )
	{
		return GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation();
	}
	ASSERT( false );
	return 0.0f;
}
*/
/*
float CEOSAIUnitPathwayPredefinedStep::GetTransportArrivalTimeAtDropoffLocation()
{
	if( this->GetTransportToUse() )
	{
		GetTransportToUse()->UpdateRedPathTimingIfNecessary();
		CEOSAIUnitPathwayPredefinedStep* pDropoffTask = GetTransportToUse()->GetDropoffStep( this );
		return pDropoffTask->GetRedMovementPath_ArrivalOffsetTimeAtDropoffLocation();
	}
	if( this->IsADropoff() )
	{
		return GetRedMovementPath_ArrivalOffsetTimeAtDropoffLocation();
	}
	ASSERT( false );
	return 0.0f;
}
*/
void CEOSAIUnitPathwayPredefinedStep::DeleteDirectTimedMovementPath()
{
	m_bDirectRedMovementPathIsValid = false;
	while( m_DirectRedMovementPath.IsEmpty() == FALSE ){ delete m_DirectRedMovementPath.RemoveHead(); }
}

void CEOSAIUnitPathwayPredefinedStep::DeletePreTransportTimedMovementPath()
{
	m_bPreTransportRedMovementPathIsValid = false;
	while( m_PreTransportRedMovementPath.IsEmpty() == FALSE ){ delete m_PreTransportRedMovementPath.RemoveHead(); }
}

void CEOSAIUnitPathwayPredefinedStep::DeletePostTransportTimedMovementPath()
{
	m_bPreTransportRedMovementPathIsValid = false;
	while( m_PostTransportRedMovementPath.IsEmpty() == FALSE ){ delete m_PostTransportRedMovementPath.RemoveHead(); }
}
/*
float CEOSAIUnitPathwayPredefinedStep::GetStepStartTime()
{
	float fThisStepStartTime = 0.0f;
	CEOSAIUnitPathwayPredefinedStep* pPrevStep = this->GetPrevStep();
	if( pPrevStep )
	{
		fThisStepStartTime = pPrevStep->GetEndTime();
	}
	else
	{
		fThisStepStartTime = GetAIUnitPathwayFinder()->GetStartTime();
	}
	return fThisStepStartTime;
}
*/
void CEOSAIUnitPathwayPredefinedStep::UpdateRedMovementPath()
{
	if( GetTransportToUse() )
	{
		UpdatePreTransportRedMovementPath();
		UpdatePostTransportRedMovementPath();
	}
	else
	{
		UpdateDirectRedMovementPath();
	}
}
/*
void CEOSAIUnitPathwayPredefinedStep::UpdateRedMovementPathTimingIfNecessary()
{
	UpdateRedMovementPath();

	if( GetTransportToUse() )
	{
		//CalculatePreTransportRedPathTiming();
		UpdatePreTransportRedMovementPath();
		SetRedMovementPath_ArrivalOffsetTimeAtPickupLocation( m_fPreTransportMovementStepTime );

		//CalculatePostTransportRedPathTiming();
		UpdatePostTransportRedMovementPath();
	}
	else
	{
		//CalculateDirectRedPathTiming();
		UpdateDirectRedMovementPath();
		//float fThisStepStartTime = GetStepStartTime();
		//m_fStepTime = m_fDirectMovementStepTime;
		//m_fStepEndTime = fThisStepStartTime + m_fDirectMovementStepTime;
	}
}
*/
/*
void CEOSAIUnitPathwayPredefinedStep::UpdatePathTimingIfNecessary_UptoDropoffTask( CEOSAIUnitPathwayPredefinedStep* pGroundUnitsStep )
{

}

float CEOSAIUnitPathwayPredefinedStep::GetTransportsArrivalTimeAtPickupPoint( CEOSAIUnitPathwayPredefinedStep* pGroundUnitsStep )
{
	
}
*/

void CEOSAIUnitPathwayPredefinedStep::UpdateDirectRedMovementPath()
{
	// Calculate the pre-transport pathway time and add the pathway to the TimedMovementPath
	if( m_bDirectRedMovementPathIsValid == false )
	{
		float fMovementRate = m_pAIUnitPathwayFinder->GetAIUnitTemplate()->GetMovementRate();
		//CWorldBuildDesc* pWorldBuildDesc = g_pCommonState->GetWorldBuildDesc();
		CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;

		while( m_DirectRedMovementPath.IsEmpty() == FALSE ){ delete m_DirectRedMovementPath.RemoveHead(); }

		float fCurrentOffsetTime = 0.0f;
		POSITION pos2 = GetDirectMovementPath()->GetHeadPosition();
		while( pos2 )
		{
			CEOSAIPathfinderPoint* pCurrentPathfinderPoint = GetDirectMovementPath()->GetNext( pos2 );
			if( pCurrentPathfinderPoint && pLastPathfinderPoint )
			{
				//float fDistance = pWorldBuildDesc->GetPixelDistance(
				float fDistance = g_pWorldDistanceTool->GetDistance(
					pCurrentPathfinderPoint->GetLocation(), pLastPathfinderPoint->GetLocation() );
				float fAdditionalTime = fDistance / fMovementRate;

				fCurrentOffsetTime += fAdditionalTime;
			}
			m_DirectRedMovementPath.AddTail(
				new CEOSAIUnitPathwayPredefinedStepPathStep( pCurrentPathfinderPoint, fCurrentOffsetTime ) );

			pLastPathfinderPoint = pCurrentPathfinderPoint;
		}

		// If this unit is a transport, write down the arrival at the pickup point
		if( this->IsAPickup() )
		{
			SetRedMovementPath_ArrivalOffsetTimeAtPickupLocation( fCurrentOffsetTime );
		}
		if( this->IsADropoff() )
		{
			SetRedMovementPath_ArrivalOffsetTimeAtDropoffLocation( fCurrentOffsetTime );
		}

		m_fDirectMovementStepTime = fCurrentOffsetTime;
		m_bDirectRedMovementPathIsValid = true;
	}
}

float CEOSAIUnitPathwayPredefinedStep::GetRedPathEndTime( EnumEndType eEndType )
{
	// Calculate the full pathway
	float fTime = GetAIUnitPathwayFinder()->GetStartTime();
	POSITION pos = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetNext( pos );
		if( pStep == this && eEndType == enum_MyArrivalAtPickupLocation )
		{
			float fMyArrivalAtPickupLocation = 
				fTime + pStep->GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation();
			return fMyArrivalAtPickupLocation;
		}

		if( pStep->IsAPickup() )
		{
			// I'm picking up a unit. Check when the transportee arrives.
			float fMyArrivalAtPickupLocation = 
				fTime + pStep->GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation();
			float fTransporteeArrivalAtPickupLocation =
				pStep->GetTransporteeStep()->GetRedPathEndTime( enum_MyArrivalAtPickupLocation );

			fTime = max( fMyArrivalAtPickupLocation, fTransporteeArrivalAtPickupLocation );
		}
		eif( pStep->GetTransportToUse() )
		{
			// I'm using a transport - need to check out the pickup time and dropoff time
			if( pStep->GetTransportPickupStep() )
			{
				float fMyArrivalAtPickupLocation = 
					fTime + pStep->m_fPreTransportMovementStepTime; //GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation();
				float fTransportArrivalAtPickupLocation =
					pStep->GetTransportPickupStep()->GetRedPathEndTime( enum_EndOfStep );

				fTime = max( fMyArrivalAtPickupLocation, fTransportArrivalAtPickupLocation );
			}

			// Now, how long does it take for the transport to drop me off?
			ASSERT( pStep->GetTransportDropoffStep() );
			if( pStep->GetTransportDropoffStep() )
			{
				float fTransportArrivalAtDropoffLocation =
					pStep->GetTransportDropoffStep()->GetRedPathEndTime( enum_EndOfStep );
				fTime = fTransportArrivalAtDropoffLocation;
			}

			// How long does it take to go from the dropoff to the target?
			fTime += m_fPostTransportMovementStepTime;
		}
		else
		{
			// This is a direct movement, just add the step time
			//fTime += pStep->GetStepTime();
			fTime += pStep->GetDirectMovementStepTime();
		}

		if( pStep == this )
		{
			return fTime;
		}
	}
	return fTime;
}
/*
void CEOSAIUnitPathwayPredefinedStep::CalculateDirectRedPathTiming()
{
	UpdateDirectRedMovementPath();
	//float fThisStepStartTime = GetStepStartTime();
	m_fStepTime = m_fDirectMovementStepTime;
	//m_fStepEndTime = fThisStepStartTime + m_fDirectMovementStepTime;
}
*/
void CEOSAIUnitPathwayPredefinedStep::UpdatePreTransportRedMovementPath()
{
	// Calculate the pre-transport pathway time and add the pathway to the TimedMovementPath
	if( m_bPreTransportRedMovementPathIsValid == false )
	{
		float fMovementRate = m_pAIUnitPathwayFinder->GetAIUnitTemplate()->GetMovementRate();
		//CWorldBuildDesc* pWorldBuildDesc = g_pCommonState->GetWorldBuildDesc();
		CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;

		while( m_PreTransportRedMovementPath.IsEmpty() == FALSE ){ delete m_PreTransportRedMovementPath.RemoveHead(); }

		float fCurrentOffsetTime = 0.0f;
		POSITION pos2 = GetPreTransportMovementPath()->GetHeadPosition();
		while( pos2 )
		{
			CEOSAIPathfinderPoint* pCurrentPathfinderPoint = GetPreTransportMovementPath()->GetNext( pos2 );
			if( pCurrentPathfinderPoint && pLastPathfinderPoint )
			{
				//float fDistance = pWorldBuildDesc->GetPixelDistance(
				float fDistance = g_pWorldDistanceTool->GetDistance(
					pCurrentPathfinderPoint->GetLocation(), pLastPathfinderPoint->GetLocation() );
				float fAdditionalTime = fDistance / fMovementRate;

				fCurrentOffsetTime += fAdditionalTime;
			}
			m_PreTransportRedMovementPath.AddTail(
				new CEOSAIUnitPathwayPredefinedStepPathStep( pCurrentPathfinderPoint, fCurrentOffsetTime ) );

			pLastPathfinderPoint = pCurrentPathfinderPoint;
		}

		m_fPreTransportMovementStepTime = fCurrentOffsetTime;
		m_bPreTransportRedMovementPathIsValid = true;

		SetRedMovementPath_ArrivalOffsetTimeAtPickupLocation( m_fPreTransportMovementStepTime );
	}
}
/*
void CEOSAIUnitPathwayPredefinedStep::CalculatePreTransportRedPathTiming()
{
	UpdatePreTransportRedMovementPath();

	//ASSERT( GetTransportToUse() );
	//float fThisStepStartTime = GetStepStartTime();
	//m_fPreTransportMovementStepEndTime = fThisStepStartTime+m_fPreTransportMovementStepTime;

	SetRedMovementPath_ArrivalOffsetTimeAtPickupLocation( m_fPreTransportMovementStepTime );
	//SetRedMovementPath_ArrivalTimeAtPickupLocation( m_fPreTransportMovementStepEndTime );
}
*/
void CEOSAIUnitPathwayPredefinedStep::UpdatePostTransportRedMovementPath()
{
	// Calculate the pre-transport pathway time and add the pathway to the TimedMovementPath
	if( m_bPostTransportRedMovementPathIsValid == false )
	{
		float fMovementRate = m_pAIUnitPathwayFinder->GetAIUnitTemplate()->GetMovementRate();
		//CWorldBuildDesc* pWorldBuildDesc = g_pCommonState->GetWorldBuildDesc();
		CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;

		while( m_PostTransportRedMovementPath.IsEmpty() == FALSE ){ delete m_PostTransportRedMovementPath.RemoveHead(); }

		float fCurrentOffsetTime = 0.0f;
		POSITION pos2 = GetPostTransportMovementPath()->GetHeadPosition();
		while( pos2 )
		{
			CEOSAIPathfinderPoint* pCurrentPathfinderPoint = GetPostTransportMovementPath()->GetNext( pos2 );
			if( pCurrentPathfinderPoint && pLastPathfinderPoint )
			{
				//float fDistance = pWorldBuildDesc->GetPixelDistance(
				float fDistance = g_pWorldDistanceTool->GetDistance(
					pCurrentPathfinderPoint->GetLocation(), pLastPathfinderPoint->GetLocation() );
				float fAdditionalTime = fDistance / fMovementRate;

				fCurrentOffsetTime += fAdditionalTime;
			}
			m_PostTransportRedMovementPath.AddTail(
				new CEOSAIUnitPathwayPredefinedStepPathStep( pCurrentPathfinderPoint, fCurrentOffsetTime ) );

			pLastPathfinderPoint = pCurrentPathfinderPoint;
		}

		m_fPostTransportMovementStepTime = fCurrentOffsetTime;
		m_bPostTransportRedMovementPathIsValid = true;

		SetRedMovementPath_ArrivalOffsetTimeAtDropoffLocation( m_fPostTransportMovementStepTime );
	}
}
/*
void CEOSAIUnitPathwayPredefinedStep::CalculatePostTransportRedPathTiming()
{
	UpdatePostTransportRedMovementPath();
/-*
	ASSERT( GetTransportToUse() );
	float fTransportArrivalAtDropoffTime = 0.0f;
	if( GetTransportToUse() )
	{
		fTransportArrivalAtDropoffTime = GetTransportToUse()->GetAIUnitPathwayFinder()->GetTransportArrivalTimeAtDropoffLocation( this );
	}

	m_fPostTransportMovementStepEndTime = fTransportArrivalAtDropoffTime + m_fPostTransportMovementStepTime;
	m_fStepEndTime = m_fPostTransportMovementStepEndTime;
*-/
//	SetRedMovementPath_ArrivalOffsetTimeAtDropoffLocation( fTransportArrivalAtDropoffTime );
}
*/

void CEOSAIUnitPathwayPredefinedStep::GetFullRedMovementPath( float fStartTime, CList< CEOSAIUnitPathwayPredefinedStepPathStep* >* pTimedMovementPath )
{
	if( this->GetTransportToUse() )
	{
		POSITION pos;
		pos = m_PreTransportRedMovementPath.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitPathwayPredefinedStepPathStep* pStep = m_PreTransportRedMovementPath.GetNext( pos );
			pTimedMovementPath->AddTail( 
				new CEOSAIUnitPathwayPredefinedStepPathStep( pStep->m_pPathfinderPoint, fStartTime+pStep->m_fOffsetTime ) );
		}

		float fTransporteeTime = fStartTime+m_fRedMovementPath_ArrivalOffsetTimeAtPickupLocation;
		CEOSAIUnit* pAIUnitTransport = GetTransportToUse();
		ASSERT( pAIUnitTransport );
		pAIUnitTransport->GetAIUnitPathwayFinder()->AddPickupToDropoffToTimedMovementPath( this, &fTransporteeTime, pTimedMovementPath );

		pos = m_PostTransportRedMovementPath.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitPathwayPredefinedStepPathStep* pStep = m_PostTransportRedMovementPath.GetNext( pos );
			pTimedMovementPath->AddTail( 
				new CEOSAIUnitPathwayPredefinedStepPathStep( pStep->m_pPathfinderPoint, fTransporteeTime+pStep->m_fOffsetTime ) );
			//pTimedMovementPath->AddTail( m_PostTransportRedMovementPath.GetNext( pos ) );
		}
	}
	else
	{
		POSITION pos = m_DirectRedMovementPath.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitPathwayPredefinedStepPathStep* pStep = m_DirectRedMovementPath.GetNext( pos );
			//pTimedMovementPath->AddTail( m_DirectRedMovementPath.GetNext( pos ) );
			pTimedMovementPath->AddTail( 
				new CEOSAIUnitPathwayPredefinedStepPathStep( pStep->m_pPathfinderPoint, fStartTime+pStep->m_fOffsetTime ) );
		}
	}
}

CEOSAIRegionPathwayMap*  CEOSAIUnitPathwayPredefinedStep::InvokeEndLocationToEverywhereDirectMap()
{
	//bool bRecreateMap = false;
	//if( m_MapCache_EndLocation != m_EndLocation ){ bRecreateMap = true; }
	//if( m_bMapCache_AvoidDangerousPathways != m_bAvoidDangerousPathways ){ bRecreateMap = true; }
	//m_MapCache_EndLocation = m_EndLocation;
	//m_bMapCache_AvoidDangerousPathways = m_bAvoidDangerousPathways;

	if( m_EndLocationToEverywhereDirectMap.IsEmpty() )
	{
		CEOSAIUnitTemplate* pUnitTemplate = m_pAIUnitPathwayFinder->GetAIUnitTemplate();
		long iUnitOwner = m_pAIUnitPathwayFinder->GetUnitOwner();
		//CEOSAIBrain* pAIBrain = m_pAIUnitPathwayFinder->GetAIBrain();

		// If this is a GroundUnit, try to find a Land-Route (we'll look for TransportAssist routes later)
		// If this is a SeaUnit, try to find a Sea-Route
		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iUnitOwner );
		RegionPathfinder.SetMovementRate( pUnitTemplate );
		if( pUnitTemplate->IsGroundUnit() )
		{
			RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsWater( false );
		}
		if( pUnitTemplate->IsSeaUnit() )
		{
			RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsWater( true );
		}
		RegionPathfinder.SetForeignRelations( m_pAIUnitPathwayFinder->GetForeignRelations() );
		if( m_bAvoidDangerousPathways )
		{
			RegionPathfinder.ApplyStrategy( m_pAIUnitPathwayFinder->GetNeutralsPathwayStategy(), m_pAIUnitPathwayFinder->GetEnemiesPathwayStategy(), 1.0f );
		}
		else
		{
			RegionPathfinder.ApplyStrategy( CEOSAIRegionPathfinder::PoliteToNeutrals, m_pAIUnitPathwayFinder->GetEnemiesPathwayStategy(), 1.0f );
			//RegionPathfinder.SetToIgnoreOtherPlayersPathway( iUnitOwner );
		}
		RegionPathfinder.AddStartLocation( m_EndLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_EndLocationToEverywhereDirectMap );
	}
	return &m_EndLocationToEverywhereDirectMap;
}

CEOSAIRegionPathwayMap* CEOSAIUnitPathwayPredefinedStep::InvokeEndLocationToEverywhereDirectMapPlusCoasts()
{
	ASSERT( m_pAIUnitPathwayFinder->GetAIUnitTemplate()->IsGroundUnit() );

	InvokeEndLocationToEverywhereDirectMap();
	if( m_EndLocationToEverywhereDirectMapPlusCoasts.GetArraySize() == 0 )
	{
		float fMovementRate = m_pAIUnitPathwayFinder->GetAIUnitTemplate()->GetMovementRate();
		CEOSAIRegionPathfinder::AddBorderRegions( &m_EndLocationToEverywhereDirectMap, &m_EndLocationToEverywhereDirectMapPlusCoasts, fMovementRate );
	}
	return &m_EndLocationToEverywhereDirectMapPlusCoasts;
}

CEOSAIRegionPathwayMap*  CEOSAIUnitPathwayPredefinedStep::InvokeEndLocationToEverywhereByAirMap()
{
	ASSERT( m_pAIUnitPathwayFinder->GetAIUnitTemplate()->IsGroundUnit() );
	if( m_EndLocationToEverywhereByAirMap.IsEmpty() )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_pAIUnitPathwayFinder->GetAIUnitTemplate();
		long iUnitOwner = m_pAIUnitPathwayFinder->GetUnitOwner();
		//CEOSAIBrain* pAIBrain = m_pAIUnitPathwayFinder->GetAIBrain();

		CEOSAIRegion2* pAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( m_EndLocation );
		ASSERT( pAIRegion->IsLand() || pAIRegion->IsCity() );

		float fTransportMovementRate =
			max( g_pEOSAICommonData->GetAIThoughtDatabase(iUnitOwner)->GetMovementRate_FastestTransport( pAIUnitTemplate,g_pEOSAICommonData->GetAIThoughtDatabase(iUnitOwner)->GetUnitsICanBuildOrHave() ),
				 pAIUnitTemplate->GetMovementRate() );

		CEOSAITerrainBasedValue MovementDesc;
		MovementDesc.Set( fTransportMovementRate );

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iUnitOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		//RegionPathfinder.SetMaximumGeoSwitches( 1 );
		RegionPathfinder.SetMovementRate( &MovementDesc );
		if( m_bAvoidDangerousPathways )
		{
			RegionPathfinder.SetForeignRelations( m_pAIUnitPathwayFinder->GetForeignRelations() );
			RegionPathfinder.ApplyStrategy( m_pAIUnitPathwayFinder->GetNeutralsPathwayStategy(), m_pAIUnitPathwayFinder->GetEnemiesPathwayStategy(), 1.0f );
		}
		else
		{
			//RegionPathfinder.ApplyStrategy( CEOSAIRegionPathfinder::PoliteToNeutrals, m_pAIUnitPathwayFinder->GetEnemiesPathwayStategy(), 1.0f );
			RegionPathfinder.SetToIgnoreOtherPlayersPathway( iUnitOwner );
		}
		RegionPathfinder.AddStartLocation( m_EndLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_EndLocationToEverywhereByAirMap );
	}
	return &m_EndLocationToEverywhereByAirMap;
}

CEOSAIRegionPathwayMap*  CEOSAIUnitPathwayPredefinedStep::InvokeEndLocationToEverywhereGroundUnitAndSeaTransportMap()
{
	ASSERT( m_pAIUnitPathwayFinder->GetAIUnitTemplate()->IsGroundUnit() );
	if( m_EndLocationToEverywhereGroundUnitAndSeaTransportMap.IsEmpty() )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_pAIUnitPathwayFinder->GetAIUnitTemplate();
		long iUnitOwner = m_pAIUnitPathwayFinder->GetUnitOwner();
		//CEOSAIBrain* pAIBrain = m_pAIUnitPathwayFinder->GetAIBrain();

		CEOSAIRegion2* pAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( m_EndLocation );
		if( EOSAISettings::m_bAssertWhenAIPathwayFails )
		{
			ASSERT( pAIRegion->IsLand() || pAIRegion->IsCity() );
		}

		// Try to reduce the sea-transport time with units
		float fTransportMovementRate =
			g_pEOSAICommonData->GetAIThoughtDatabase(iUnitOwner)->GetMovementRate_FastestSeaTransport( pAIUnitTemplate,g_pEOSAICommonData->GetAIThoughtDatabase(iUnitOwner)->GetUnitsICanBuildOrHave() );

		CEOSAITerrainBasedValue MovementDesc;
		MovementDesc.Set( fTransportMovementRate );
		MovementDesc.SetGroundUnitMovementRate( 2.0f*fTransportMovementRate );
		//MovementDesc.m_fTerrainValue[ TERRAIN_ROAD ] = 2.0f*fTransportMovementRate;
		//MovementDesc.Set( m_pAIUnitPathwayFinder->GetUnitTemplate() );
		//MovementDesc.m_fTerrainValue[TERRAIN_COAST] = fTransportMovementRate;
		//MovementDesc.m_fTerrainValue[TERRAIN_WATER] = fTransportMovementRate;

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iUnitOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetMaximumGeoSwitches( 2 ); // We know the EndLocation will be Land or City
		RegionPathfinder.SetMovementRate( &MovementDesc );
		if( m_bAvoidDangerousPathways )
		{
			RegionPathfinder.SetForeignRelations( m_pAIUnitPathwayFinder->GetForeignRelations() );
			RegionPathfinder.ApplyStrategy( m_pAIUnitPathwayFinder->GetNeutralsPathwayStategy(), m_pAIUnitPathwayFinder->GetEnemiesPathwayStategy(), 1.0f );
		}
		else
		{
			RegionPathfinder.ApplyStrategy( CEOSAIRegionPathfinder::PoliteToNeutrals, m_pAIUnitPathwayFinder->GetEnemiesPathwayStategy(), 1.0f );
			//RegionPathfinder.SetToIgnoreOtherPlayersPathway( iUnitOwner );
		}
		RegionPathfinder.AddStartLocation( m_EndLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_EndLocationToEverywhereGroundUnitAndSeaTransportMap );
	}
	return &m_EndLocationToEverywhereGroundUnitAndSeaTransportMap;
}

/*
EOSAI::UnitPathwayResultStep*  CEOSAIUnitPathwayPredefinedStep::GetResult( EnumPathwayResultStep e )
{
	POSITION pos = m_PathwayResults.GetHeadPosition();
	while( pos )
	{
		EOSAI::UnitPathwayResultStep* pResultStep = m_PathwayResults.GetNext( pos );
		if( pResultStep->m_ePathType == e ) return pResultStep;
	}
	return NULL;
}

void  CEOSAIUnitPathwayPredefinedStep::AddResult( EOSAI::UnitPathwayResultStep* pResultStep )
{
	// Check if it already exists in the list
	ASSERT( pResultStep->m_ePathType != enum_Undefined );
	EOSAI::UnitPathwayResultStep* pResultStepInList = GetResult( pResultStep->m_ePathType );
	ASSERT( pResultStepInList == NULL );
	//
	m_PathwayResults.AddTail( pResultStep );
}
*/
/*
CAITransportPossibility*  CEOSAIUnitPathwayPredefinedStep::GetTransportPossibilityShortestTime()
{
	CAITransportPossibility* pBestAITransportPossibility = NULL;
	POSITION pos = m_AITransportPossibility.GetHeadPosition();
	while( pos )
	{
		CAITransportPossibility* pAITransportPossibility = m_AITransportPossibility.GetNext( pos );
		if( pBestAITransportPossibility == NULL )
		{
			pBestAITransportPossibility = pAITransportPossibility;
		}
		else if( pBestAITransportPossibility->m_fTotalTime > pAITransportPossibility->m_fTotalTime )
		{
			pBestAITransportPossibility = pAITransportPossibility;
		}
	}
	return pBestAITransportPossibility;
}
*/
/*
CAITransportTemplatePathway* CEOSAIUnitPathwayPredefinedStep::GetDropoffToPickupMap( CUnitTemplate* pTransportUnitTemplate, bool bSneakAttackPathway )
{
	POSITION pos = m_SeaRoutes_DropoffToPickup.GetHeadPosition();
	while( pos )
	{
		CAITransportTemplatePathway* pPathway = m_SeaRoutes_DropoffToPickup.GetNext( pos );
		if( pPathway->m_pTransportUnitTemplate == pTransportUnitTemplate &&
			pPathway->m_bSneakAttackPathway == bSneakAttackPathway )
		{
			return pPathway;
		}
	}
	return NULL;
}
*/

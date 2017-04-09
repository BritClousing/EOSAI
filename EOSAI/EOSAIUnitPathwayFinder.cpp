
#include "stdafx.h"
//#include "WorldDescServer.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAIAirUnitPathwayFinder.h"
#include "EOSAIGroundUnitPathwayFinder.h"
#include "EOSAISeaUnitPathwayFinder.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAIUnitPathwayResult.h"
#include "EOSAIUnitPathwayResultStep.h"
#include "PoiObject.h"
#include "EOSAIUnit.h"
#include "City.h"
#include "EOSAIAirfield.h"
#include "PoiObject.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIBrain.h"
#include "EOSAICommonData.h"
#include "EOSAIMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIUnitPathwayFinder*  CEOSAIUnitPathwayFinder::CreatePathwayFinder( CEOSAIUnit* pAIUnitActor )
{
	CEOSAIUnitTemplate* pAIUnitTemplate = pAIUnitActor->GetAIUnitTemplate();
	if( pAIUnitTemplate->IsAirUnit() )
	{
		CEOSAIAirUnitPathwayFinder* pAirUnitPathway = new CEOSAIAirUnitPathwayFinder( pAIUnitActor );
		//pAirUnitPathway->SetMovementConsumed( pAIUnitActor->GetMovementConsumed() );
		return pAirUnitPathway;
	}
	if( pAIUnitTemplate->IsGroundUnit() )
	{
		CAIGroundUnitPathwayFinder* pGroundUnitPathway = new CAIGroundUnitPathwayFinder( pAIUnitActor );
		return pGroundUnitPathway;
	}
	if( pAIUnitTemplate->IsSeaUnit() )
	{
		CEOSAISeaUnitPathwayFinder* pSeaUnitPathway = new CEOSAISeaUnitPathwayFinder( pAIUnitActor );
		return pSeaUnitPathway;
	}
	ASSERT( false );
	return NULL;
}

CEOSAIUnitPathwayFinder*  CEOSAIUnitPathwayFinder::CreatePathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime )
{
	if( pAIUnitTemplate->IsAirUnit() )
	{
		CEOSAIAirUnitPathwayFinder* pAirUnitPathway = new CEOSAIAirUnitPathwayFinder( pAICityActor, pAIUnitTemplate, fBuildTime );
		//pAirUnitPathway->SetMovementConsumed( 0.0f );
		return pAirUnitPathway;
	}
	if( pAIUnitTemplate->IsGroundUnit() )
	{
		CAIGroundUnitPathwayFinder* pGroundUnitPathway = new CAIGroundUnitPathwayFinder( pAICityActor, pAIUnitTemplate, fBuildTime );
		return pGroundUnitPathway;
	}
	if( pAIUnitTemplate->IsSeaUnit() )
	{
		CEOSAISeaUnitPathwayFinder* pSeaUnitPathway = new CEOSAISeaUnitPathwayFinder( pAICityActor, pAIUnitTemplate, fBuildTime );
		return pSeaUnitPathway;
	}
	ASSERT( false );
	return NULL;
}
/*
CEOSAIUnitPathway* CEOSAIUnitPathway::CreatePathway( long iPlayer, CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate )
{
	if( dynamic_cast< CEOSAIUnit* >( pActor ) )
	{
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pActor );
		pUnitTemplate = pAIUnit->GetUnitTemplate();
	}

	ASSERT( pUnitTemplate );
	if( pUnitTemplate->IsAirUnit() )
	{
		CAIAirUnitPathway* pAirUnitPathway = new CAIAirUnitPathway( pActor->GetAIBrain() );
		pAirUnitPathway->SetUnit( pActor, pUnitTemplate );
		//pAirUnitPathway->m_pAIBrain = pActor->GetAIBrain();
		//pAirUnitPathway->m_iUnitOwner = pActor->GetOwner();
		//pAirUnitPathway->m_pActor = pActor;
		//pAirUnitPathway->m_pUnitTemplate = pUnitTemplate;
		pAirUnitPathway->m_StartLocation = pActor->GetPoiObject()->GetLocation();
		return pAirUnitPathway;
	}
	if( pUnitTemplate->IsGroundUnit() )
	{
		CAIGroundUnitPathway* pGroundUnitPathway = new CAIGroundUnitPathway( pActor->GetAIBrain() );
		pGroundUnitPathway->SetUnit( iPlayer, pActor, pUnitTemplate );
		//pGroundUnitPathway->m_pAIBrain = pActor->GetAIBrain();
		//pGroundUnitPathway->m_iUnitOwner = pActor->GetOwner();
		//pGroundUnitPathway->m_pActor = pActor;
		//pGroundUnitPathway->m_pUnitTemplate = pUnitTemplate;
		pGroundUnitPathway->m_StartLocation = pActor->GetPoiObject()->GetLocation();
		return pGroundUnitPathway;
	}
	if( pUnitTemplate->IsSeaUnit() )
	{
		//
		// THIS IS THE NEW WAY - swich GroundUnit and AirUnit Pathways to take this format, too
		//
		CAISeaUnitPathway* pSeaUnitPathway = new CAISeaUnitPathway( pActor->GetAIBrain() );
		/*
		pSeaUnitPathway->SetUnit( iPlayer, pActor, pUnitTemplate );
		//pSeaUnitPathway->m_pAIBrain = pActor->GetAIBrain();
		//pSeaUnitPathway->m_iUnitOwner = pActor->GetOwner();
		//pSeaUnitPathway->m_pActor = pActor;
		//pSeaUnitPathway->m_pUnitTemplate = pUnitTemplate;
		pSeaUnitPathway->m_StartLocation = pActor->GetPoiObject()->GetLocation();
		*-/
		return pSeaUnitPathway;
	}
	ASSERT( false );
	return NULL;
}
*/

CEOSAIUnitPathwayFinder::CEOSAIUnitPathwayFinder( CEOSAIUnit* pAIUnitActor )
{
	//m_pAIBrain = pAIBrain;//pAIUnitActor->GetAIBrain();

	m_iUnitOwner = pAIUnitActor->GetOwner();
	m_pAIUnitActor = pAIUnitActor;
	m_pAICityActor = NULL;
	m_pAIUnitTemplate = pAIUnitActor->GetAIUnitTemplate();
	m_StartLocation = pAIUnitActor->GetInitialState()->GetLocation();//>GetPoiObject()->GetLocation();
	m_fStartTime = 0.0f;
	//
	m_eNeutralsPathwayStategy = CEOSAIRegionPathfinder::IgnoreNeutrals;
	m_eEnemiesPathwayStategy = CEOSAIRegionPathfinder::IgnoreEnemies;

	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	m_OwnerForeignRelations.SetSize( iNumberOfPlayers+1 );
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	for( long i=0; i<iNumberOfPlayers+1; i++ )
	{
		//m_OwnerForeignRelations[i] = pWorldDescServer->GetForeignRelations( m_pAIBrain->GetAIPlayerNumber(), i );
		//m_OwnerForeignRelations[i] = g_pEOSAICommonData->GetForeignRelations( m_pAIBrain->GetAIPlayerNumber(), i );
		m_OwnerForeignRelations[i] = g_pEOSAICommonData->GetForeignRelations( m_iUnitOwner, i );
	}
	//
	//m_bPathIsValid = false;
	//m_bInitialEvaluationHasBeenDone = false;
	//m_bInitialEvaluationPathIsValid = false;
	//
	//m_bFinalPathWasCalculated = false;
	//m_bFinalPathIsValid = false;
	//
	//m_iCurrentPathfindingLevel = 0;
	m_bAvoidDangerousPathways = false;
	m_pResultPath = NULL;
	//m_bOrdersCreated = false;

	//m_fCacheGeoArrivalTime = 0.0f;
}

CEOSAIUnitPathwayFinder::CEOSAIUnitPathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime )
{
	//m_pAIBrain = pAICityActor->GetAIBrain();

	m_iUnitOwner = pAICityActor->GetOwner();
	m_pAIUnitActor = NULL;
	m_pAICityActor = pAICityActor;
	//m_pActor = pAICityActor;
	m_pAIUnitTemplate = pAIUnitTemplate;
	m_StartLocation = pAICityActor->GetLocation();
	m_fStartTime = fBuildTime;
	//
	m_eNeutralsPathwayStategy = CEOSAIRegionPathfinder::IgnoreNeutrals;
	m_eEnemiesPathwayStategy = CEOSAIRegionPathfinder::IgnoreEnemies;

	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	m_OwnerForeignRelations.SetSize( iNumberOfPlayers+1 );
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	for( long i=0; i<iNumberOfPlayers+1; i++ )
	{
		//m_OwnerForeignRelations[i] = g_pEOSAICommonData->GetForeignRelations( m_pAIBrain->GetAIPlayerNumber(), i );
		m_OwnerForeignRelations[i] = g_pEOSAICommonData->GetForeignRelations( m_iUnitOwner, i );
	}
	//
	//m_bInitialEvaluationHasBeenDone = false;
	//m_bInitialEvaluationPathIsValid = false;
	//
	//m_bFinalPathWasCalculated = false;
	//m_bFinalPathIsValid = false;
	//
	//m_iCurrentPathfindingLevel = 0;
	//m_bPathIsValid = false;
	m_bAvoidDangerousPathways = false;
	m_pResultPath = NULL;
	//m_bOrdersCreated = false;

	//m_fCacheGeoArrivalTime = 0.0f;
}
/*
CEOSAIUnitPathwayFinder::CEOSAIUnitPathwayFinder( CEOSAIBrain* pAIBrain )
{
	m_pAIBrain = pAIBrain;

	m_iUnitOwner = 0;
	m_pActor = NULL;
	m_pUnitTemplate = NULL;

	m_fStartTime = 0.0f;
	//
	m_eNeutralsPathwayStategy = CEOSAIRegionPathfinder::IgnoreNeutrals;
	m_eEnemiesPathwayStategy = CEOSAIRegionPathfinder::IgnoreEnemies;
	//m_ePathwayStategy = Undefined;
	//m_pGlobalForeignRelations = GetCommonState()->GetWorldDescServer()->GetGlobalForeignRelations();

	long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();
	m_OwnerForeignRelations.SetSize( iNumberOfPlayers+1 );
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	for( long i=0; i<iNumberOfPlayers+1; i++ )
	{
		m_OwnerForeignRelations[i] = pWorldDescServer->GetForeignRelations( pAIBrain->GetAIPlayerNumber(), i );
	}

	//
	m_bInitialEvaluationHasBeenDone = false;
	m_bInitialEvaluationPathIsValid = false;
	//
	m_bFinalPathWasCalculated = false;
	m_bFinalPathIsValid = false;
	//
	//m_bAppendLandingToTheEnd = false;
	//
	m_iCurrentPathfindingLevel = 0;
	//m_bPathIsValid = false;
	m_bOrdersCreated = false;

	//m_fPathwayDanger = 0.0f;
	//m_fPathwayEnemySighting = 0.0f;
}
*/
CEOSAIUnitPathwayFinder::~CEOSAIUnitPathwayFinder()
{
	ClearPredefinedAndResultPaths();
	//while( m_PreDefinedPath.IsEmpty() == FALSE ){ delete m_PreDefinedPath.RemoveHead(); }
	while( m_FullRedMovementPath.IsEmpty() == FALSE ){ delete m_FullRedMovementPath.RemoveHead(); }
}

void CEOSAIUnitPathwayFinder::ClearPredefinedAndResultPaths()
{
	int g = 0;
	while( m_PreDefinedPath.IsEmpty() == FALSE )
	{
		delete m_PreDefinedPath.RemoveHead();
	}
	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }
}

/*
long CEOSAIUnitPathwayFinder::GetPlayer()
{
	return m_pAIBrain->GetAIPlayerNumber();
}
*/
CEOSAIPoiObject*  CEOSAIUnitPathwayFinder::GetAIPoiObjectActor()
{
	if( m_pAIUnitActor ){ return m_pAIUnitActor; } 
	if( m_pAICityActor ){ return m_pAICityActor; } 
	ASSERT( false );
	return NULL;
}
/*
float CEOSAIUnitPathwayFinder::GetEndTime()
{
	ValidatePredefinedStepsStartEndLocations();

	float fEndTime = m_fStartTime;
	/-*
	if( m_pResultPath && m_pResultPath->m_Steps.IsEmpty() == FALSE )
	{
		return m_pResultPath->m_Steps.GetTail()->GetEndTime();
	}
	*-/
	if( m_PreDefinedPath.IsEmpty() == FALSE )
	{
		return m_PreDefinedPath.GetTail()->GetEndTime();
	}
	/-*
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		fEndTime += pStep->GetEstimatedMovementTime();

		if( GetUnitTemplate()->IsAirUnit() && pStep->IsALandAtAirbase() )
		{
			fEndTime = CEOSAIMath::RoundUp( fEndTime );
		}
	}
	*-/
	return fEndTime;
}
*/
/*
void  CEOSAIUnitPathwayFinder::SetUnit( CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate )
{
	//m_pAIBrain = pActor->GetAIBrain();
	m_iUnitOwner = pActor->GetOwner();
	m_pActor = pActor;
	m_pUnitTemplate = pUnitTemplate;
	m_StartLocation = pActor->GetPoiObject()->GetLocation();
}
*/
void  CEOSAIUnitPathwayFinder::ResetForeignRelations()
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	for( long i=0; i<iNumberOfPlayers+1; i++ )
	{
		m_OwnerForeignRelations[i] = g_pEOSAICommonData->GetForeignRelations( m_iUnitOwner, i );
	}
}

bool CEOSAIUnitPathwayFinder::NeedsTransport()
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		if( pPredefinedStep->RequiresTransport() ) return true;
	}
	return false;
	/*
	if( m_pResultPath )
	{
		POSITION pos = m_pResultPath->m_Steps.GetHeadPosition();
		while( pos )
		{
			m_pResultPath->m_Steps.GetNext( pos );
		}
	}
	*/
}


// Pathway setup
void CEOSAIUnitPathwayFinder::AddWaypoint( CEOSAILocation Location )
{
	CEOSAIUnitPathwayPredefinedStep* pGroundPathPoint = new CEOSAIUnitPathwayPredefinedStep( this );
	pGroundPathPoint->GotoWaypoint( Location );
//	pGroundPathPoint->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_Waypoint;
//	pGroundPathPoint->m_EndLocation = Location;
	//m_PreDefinedPath.AddTail( pGroundPathPoint );
	AppendStep( pGroundPathPoint );
}

// Pickup/Dropff
/*
void  CEOSAIUnitPathwayFinder::AddPickupTarget( CEOSAIUnit* pAIUnitToPickup, CEOSAILocation OtherUnitLocation )
{
	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->Pickup( pAIUnitToPickup, OtherUnitLocation );
//	pStep->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget;
//	pStep->m_pAITarget = pAIUnitToPickup;
//	pStep->m_EndLocation = OtherUnitLocation;
	//m_PreDefinedPath.AddTail( pStep );
	AppendPreDefinedStep( pStep );
	//CAISeaPathPoint* pPoint = new CAISeaPathPoint();
	//pPoint->m_pAITarget = pAIUnitToPickup;
	//pPoint->m_TargetLocation = OtherUnitLocation;//pAIUnitToPickup->GetPoiObject()->GetLocation();
	//m_PreDefinedPath.AddTail( pPoint );
}

void  CEOSAIUnitPathwayFinder::AddDropoffTarget( CEOSAIUnit* pAIUnitToDropoff, CEOSAILocation OtherUnitTargetLocation ) // Dropoff near this location
{
	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->Dropoff( pAIUnitToDropoff, OtherUnitTargetLocation );
//	pStep->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget;
//	pStep->m_pAITarget = pAIUnitToDropoff;
//	pStep->m_EndLocation = OtherUnitTargetLocation;
	//m_PreDefinedPath.AddTail( pStep );
	AppendPreDefinedStep( pStep );
	//CAISeaPathPoint* pPoint = new CAISeaPathPoint();
	//pPoint->m_pTarget = pAIUnitToDropoff;
	//pPoint->m_TargetLocation = OtherUnitTargetLocation;
	//m_PreDefinedPath.AddTail( pPoint );
}
*/
/*
CEOSAIUnitPathwayPredefinedStep* CEOSAIUnitPathwayFinder::CreateTransportAction( CEOSAIUnit* pAITransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeEndLocation )
{
	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->Transport( pAITransportee, TransporteeStartLocation, TransporteeEndLocation );
	return pStep;
}
*/
void CEOSAIUnitPathwayFinder::AppendTransportSteps( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep )
{
	ASSERT( GetAIUnitTemplate()->CanContain( pTransporteeStep->GetAIUnitPathwayFinder()->GetAIUnitTemplate() ) );
	CEOSAIUnitPathwayPredefinedStep* pStep1 = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep1->Pickup( pTransporteeStep );
	AppendStep( pStep1 );

	CEOSAIUnitPathwayPredefinedStep* pStep2 = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep2->Dropoff( pTransporteeStep );
	AppendStep( pStep2 );
}

CEOSAIUnit* CEOSAIUnitPathwayFinder::GetCurrentTransport()
{
	if( GetAIUnitActor() )
	{
		//CUnit* pGroundUnit = GetAIUnitActor()->GetServerUnit();
		CEOSAIUnit* pGroundAIUnit = GetAIUnitActor();//->GetServerUnit();
		if( pGroundAIUnit )
		{
			CEOSAIPoiObject* pAIPoiObject = pGroundAIUnit->GetContainerAIPoiObject();
			if( pAIPoiObject )
			{
				//long iUnitObjectId = pAIPoiObject->GetObjectId();
				//CEOSAIPoiObject* pAIPoiObject = m_pAIBrain->GetAIPoiObject( iUnitObjectId );
				return dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			}
		}
	}
	return NULL;
}

void CEOSAIUnitPathwayFinder::InsertTransportSteps( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep )
{
	ASSERT( GetAIUnitTemplate()->CanContain( pTransporteeStep->GetAIUnitPathwayFinder()->GetAIUnitTemplate() ) );

	CEOSAIUnit* pAIGroundUnitContainerUnit = pTransporteeStep->GetAIUnitPathwayFinder()->GetCurrentTransport();
	bool bTransporteeIsAlreadyInsideThisTransport = false;
	if( pAIGroundUnitContainerUnit ) // The Transportee is inside a unit transport
	{
		if( pAIGroundUnitContainerUnit == GetAIUnitActor() )
		{
			bTransporteeIsAlreadyInsideThisTransport = true; // The Transportee is inside [this] transport
		}
		else
		{
			if( EOSAISettings::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false ); // The Transportee is inside a different transport?
				return;
			}
		}
	}

	if( bTransporteeIsAlreadyInsideThisTransport == false )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep1 = new CEOSAIUnitPathwayPredefinedStep( this );
		pStep1->Pickup( pTransporteeStep );
		InsertPickupOrDropoffStep( pStep1 );
	}

	CEOSAIUnitPathwayPredefinedStep* pStep2 = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep2->Dropoff( pTransporteeStep );
	InsertPickupOrDropoffStep( pStep2 );

	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }

	// Update Predefined Path
	CEOSAIUnit* pAITransport = GetAIUnitActor();
	pAITransport->GetAIUnitPathwayFinder()->UpdateTransportPickupDropoffPath( pTransporteeStep );
}

void CEOSAIUnitPathwayFinder::DeleteTransportSteps( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep )
{
	bool bStepWasRemoved = false;

	ASSERT( pTransporteeStep != NULL );
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		if( pExistingStep->GetTransporteeStep() == pTransporteeStep )
		{
			delete pExistingStep;
			m_PreDefinedPath.RemoveAt( prevPos );

			bStepWasRemoved = true;
		}
	}
	if( bStepWasRemoved )
	{
		// Delete the Results - they aren't valid anymore
		if( m_pResultPath )
		{
			delete m_pResultPath;
			m_pResultPath = NULL;
		}
		ValidatePredefinedStepsStartEndLocations();
	}

	// Update Predefined Path - because this can change other Predefined Steps
	//UpdateEndLocationsOfPredefinedPath();
}

CEOSAIUnitPathwayPredefinedStep* CEOSAIUnitPathwayFinder::GetTransportPickupStep( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep )
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		if( pExistingStep->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget &&
			pExistingStep->GetTransporteeStep() == pTransporteeStep )
		{
			return pExistingStep;
		}
	}
	return NULL;
}

CEOSAIUnitPathwayPredefinedStep* CEOSAIUnitPathwayFinder::GetTransportDropoffStep( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep )
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		if( pExistingStep->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget &&
			pExistingStep->GetTransporteeStep() == pTransporteeStep )
		{
			return pExistingStep;
		}
	}
	return NULL;
}

bool  CEOSAIUnitPathwayFinder::UpdateTransportPickupDropoffPath( CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep )
{
	ASSERT( GetAIUnitTemplate()->IsTransport() );
	bool bAirTransport = GetAIUnitTemplate()->IsAirUnit();
	//
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		if( pExistingStep->GetTransporteeStep() == pGroundUnitStep )
		{
			bool bUpdated = UpdateEndLocation( pExistingStep );
			if( bUpdated == false ) 
			{
				// This situation isn't supposed to happen - but if it does, I don't want the app to crash.
				//   (I've seen it crash in the past when it couldn't update the pickup/dropoff location.)
				if( EOSAISettings::m_bAssertWhenAIPathwayFails )
				{
					ASSERT( false );
				}
				m_PreDefinedPath.RemoveAt( prevPos );
				delete pExistingStep;
			}
		}
	}
	return NULL;
	//virtual bool UpdateTransportAssistedPath( CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep );
}

//

void  CEOSAIUnitPathwayFinder::SetTransportToUse( CEOSAIUnit* pAITransportToUse )
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		if( pStep->RequiresTransport() )
		{
			pStep->UseTransportAndCreatePickupDropoffSteps( pAITransportToUse );
		}
	}
}

/*
void CEOSAIUnitPathwayFinder::AddTransportAction( CEOSAIUnit* pAITransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeEndLocation )
{
	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->Transport( pAITransportee, TransporteeStartLocation, TransporteeEndLocation );
//	pStep->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_Transport;
//	pStep->m_pAITarget = pAITransportee;
//	pStep->m_TransporteeStartLocation = TransporteeStartLocation;
//	pStep->m_TransporteeEndLocation = TransporteeEndLocation;
	//pStep->m_EndLocation = OtherUnitTargetLocation;
	AppendStep( pStep );
}
*/
// Airfield (ground unit only)
void CEOSAIUnitPathwayFinder::AddBuildAirfield( CEOSAILocation Location )
{
	ASSERT( m_pAIUnitTemplate->CanBuildAirfields() );

	// The location must be a ground location
	//CWorldDesc* pWorldDesc = m_pAIBrain->GetWorldDescServer();
	//if( pWorldDesc->IsOnLand( Location ) )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
		pStep->BuildAirfield( Location );
	//	pGroundPathPoint->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_BuildAirfield;
		//pGroundPathPoint->m_bBuildAirfield = true;
	//	pGroundPathPoint->m_EndLocation = Location;
		//m_PreDefinedPath.AddTail( pGroundPathPoint );
	//	AppendPreDefinedStep( pGroundPathPoint );
		AppendStep( pStep );
	}
	//else
	//{
	//	ASSERT( false );
	//}
}

void  CEOSAIUnitPathwayFinder::LandAtAirbase( CEOSAIPoiObject* pAirbase ) // city,airbase,carrier
{
	ASSERT( m_pAIUnitTemplate->HasARange() );
	//ASSERT( m_PreDefinedPath.IsEmpty() == FALSE );

	// Make sure the AirUnit can land in the Airbase
	ASSERT( m_pAIUnitTemplate->CanLandAfterLaunch() );
	ASSERT( pAirbase->CanContain_IgnoreForeignRelations( m_pAIUnitTemplate ) );
	ASSERT( pAirbase->IsLandingPadForAirUnits() );
	ASSERT( pAirbase->IsLaunchingPadForAirUnits() );

	if( m_pAIUnitTemplate->CanLandAfterLaunch() &&
		pAirbase->CanContain_IgnoreForeignRelations( m_pAIUnitTemplate ) &&
		pAirbase->IsLandingPadForAirUnits() &&
		pAirbase->IsLaunchingPadForAirUnits() )
	{
		// Where is the airbase?

		CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
		pStep->LandAtAirbase( pAirbase );
	//	pStep->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_LandAtAirbase;
		//pStep->m_bLandAtAirfield = true;
	//	pStep->m_pAITarget = pAirbase;
	//	pStep->m_EndLocation = pAirbase->GetInitialState()->GetLocation();
		//m_PreDefinedPath.AddTail( pStep );
		AppendStep( pStep );
		//CEOSAILocation AirbaseLocation = pAirbase->GetInitialState()->GetLocation();
		//CAIAirPathAirbase* pPoint = new CAIAirPathAirbase( pAirbase, AirbaseLocation );
		//m_PreDefinedPath.AddTail( pPoint );
	}
}

void  CEOSAIUnitPathwayFinder::LandAtAnyAirbase()
{
	ASSERT( m_pAIUnitTemplate->HasARange() );

	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->LandAtAnyAirbase();
//	pStep->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_LandAtAirbase;
	//pStep->m_bLandAtAirfield = true;
//	pStep->m_pAITarget = NULL;
	//m_PreDefinedPath.AddTail( pStep );
	AppendStep( pStep );
	//CAIAirPathAirbase* pPoint = new CAIAirPathAirbase();
	//m_PreDefinedPath.AddTail( pPoint );
}

// Repair
//void CEOSAIUnitPathwayFinder::AddRepairTask( CEOSAICity* pAICity )
void CEOSAIUnitPathwayFinder::AddRepairTask( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->RepairAt( pAIPoiObject );
//	pStep->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_Repair;
	//pStep->m_bRepair = true;
//	pStep->m_pAITarget = pAICity;
//	pStep->m_EndLocation = pAICity->GetPoiObject()->GetLocation();
	//m_PreDefinedPath.AddTail( pStep );
	AppendStep( pStep );
}
/*
void CEOSAIUnitPathwayFinder::AddRepairTaskAnywhere()
{
	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->RepairAnywhere();
//	pStep->m_eTask = CEOSAIUnitPathwayPredefinedStep::enum_Repair;
	//pStep->m_bRepair = true;
//	pStep->m_pAITarget = NULL;
	//m_PreDefinedPath.AddTail( pStep );
	AppendStep( pStep );
}
*/

// Attack
CEOSAIUnitPathwayPredefinedStep* CEOSAIUnitPathwayFinder::CreateCaptureTarget( CEOSAIPoiObject* pTarget )
{
	ASSERT( m_pAIUnitTemplate->CanCapture( pTarget ) );
	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->CaptureCitRes( pTarget );
	//InsertStep( pStep );
	return pStep;
}

CEOSAIUnitPathwayPredefinedStep* CEOSAIUnitPathwayFinder::CreateDegradeTarget( CEOSAIPoiObject* pTarget )
{
	ASSERT( dynamic_cast< CEOSAICity* >( pTarget ) || dynamic_cast< CEOSAIAirfield* >( pTarget ) );
	ASSERT( m_pAIUnitTemplate->CanDegrade( pTarget ) );

	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->DegradeTarget( pTarget );
	//InsertStep( pStep );
	return pStep;
}

CEOSAIUnitPathwayPredefinedStep* CEOSAIUnitPathwayFinder::CreateAttackUnit( CEOSAIPoiObject* pAITarget )
{
	CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAITarget );
	ASSERT( pAIUnit );
	ASSERT( m_pAIUnitTemplate->CanAttack( pAITarget ) );

	CEOSAIUnitPathwayPredefinedStep* pStep = new CEOSAIUnitPathwayPredefinedStep( this );
	pStep->AttackUnit( pAIUnit );
	//InsertStep( pStep );
	return pStep;
}


bool CEOSAIUnitPathwayFinder::PathwayShouldAppendALanding()
{
	if( m_pAIUnitTemplate->HasARange() == false ) return false;
	if( m_PreDefinedPath.GetCount() == 0 ) return false;

	// If the air unit is going to self-destruct during the pathway, don't bother finding a landing
	if( IncludesAnAttack() && m_pAIUnitTemplate->SelfDestructsOnAttack() ) return false;

	// If the air unit cannot land, don't append a landing
	if( m_pAIUnitTemplate->CanLandAfterLaunch() == false ) return false;

	// Already has an airbase at the end?
	CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetTail();
	if( pPredefinedStep->IsALandAtAirbase() ) return false;

	return true;
}


void  CEOSAIUnitPathwayFinder::AppendStep( CEOSAIUnitPathwayPredefinedStep* pNewStep )
{
	CEOSAILocation StartLocation = m_StartLocation;
	if( m_PreDefinedPath.IsEmpty() == FALSE )
	{
		StartLocation = m_PreDefinedPath.GetTail()->GetEndLocation();
	}
	pNewStep->SetStartLocation( StartLocation );
	//m_PreDefinedPath.AddTail( pNewStep );
	//InvokeImaginaryTransport( pPredefinedStep );
	AddToPredefinedSteps_InsertBefore( pNewStep, NULL );

	//for( long iSubStep=1; iSubStep<pNewStep->GetNumberOfSubSteps(); iSubStep++ )
	//{
	//	AppendPredefinedSubStep( pNewStep,iSubStep );
	//}
}


void  CEOSAIUnitPathwayFinder::InsertPickupOrDropoffStep( CEOSAIUnitPathwayPredefinedStep* pNewStep )
{
	// This is called by the transport

	// Find the best place to insert this step
	//   INCOMPLETE: a more complete model could take into account a variety of complications, including:
	//   (1) How does this slow-down existing Predefined steps?  Is it worth displacing them?
	//   (2) How does this new step mess-up the stealth?
	//       Example: if I am invading, I might want to skip past a water-oil resource and catch it later
	//   (3) Should I do non-combat tasks (capture resource) before combat tasks (capture city)?

	bool bThisIsADropoffStep = false;
	bool bThereIsAPickupStep = false;
	bool bPickupStepHasBeenFound = false;
	if( pNewStep->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_DropoffTarget )
	{
		bThisIsADropoffStep = true;
		POSITION pos = m_PreDefinedPath.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
			if( pExistingStep->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget &&
				pExistingStep->GetTransporteeStep() == pNewStep->GetTransporteeStep() )
			{
				bThereIsAPickupStep = true;
				break;
			}
		}
	}

	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	CEOSAILocation CurrentLocation = m_StartLocation;
	CEOSAILocation NewLocation = pNewStep->GetEndLocation();

	float fBestValue = 1000000.0f; // Lower is better
	CEOSAIUnitPathwayPredefinedStep* pBestValue_InsertBeforeStep = NULL;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		CEOSAILocation NextLocation = pExistingStep->GetEndLocation();

		// If this is a dropoff step:
		//   Wait until I find the pickup step in the list
		if( bThisIsADropoffStep &&
			bThereIsAPickupStep &&
			bPickupStepHasBeenFound == false )
		{
			if( pExistingStep->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget &&
				pExistingStep->GetTransporteeStep() == pNewStep->GetTransporteeStep() )
			{
				// I found my pickup step
				bPickupStepHasBeenFound = true;
				CurrentLocation = NextLocation;
				continue;
			}
			else
			{
				// Skip evaluating this step, I have't found the pickup yet
				CurrentLocation = NextLocation;
				continue;
			}
		}
		/*
		if( bThisIsADropoffStep &&
			bThereIsAPickupStep &&
			bPickupStepHasBeenFound == false &&
			pExistingStep->GetTask() == CEOSAIUnitPathwayPredefinedStep::enum_PickupTarget &&
			pExistingStep->GetTransporteeStep() == pNewStep->GetTransporteeStep() )
		{
			bPickupStepHasBeenFound = true;
			CurrentLocation = NextLocation;
			continue;
		}
		if( bThisIsADropoffStep &&
			bThereIsAPickupStep &&
			bPickupStepHasBeenFound == false )
		{
			// Skip evaluating this step, I have't found the pickup yet
			CurrentLocation = NextLocation;
			continue;
		}
		*/

		float fOriginalDistance = 
			g_pWorldDistanceTool->GetDistance( CurrentLocation, NextLocation );
		float fNewDistance = 
			g_pWorldDistanceTool->GetDistance( CurrentLocation, NewLocation ) +
			g_pWorldDistanceTool->GetDistance( NewLocation, NextLocation );

		float fDeltaValue = fNewDistance - fOriginalDistance;
		if( fDeltaValue < fBestValue )
		{
			fBestValue = fDeltaValue;
			pBestValue_InsertBeforeStep = pExistingStep;
		}
		CurrentLocation = NextLocation;
	}
	// What's the distance if I add it as the last step?
	{
		float fOriginalDistance = 0.0f;
			//pWorldBuildDesc->GetPixelDistance( CurrentLocation, NextLocation );
		float fNewDistance = 
			g_pWorldDistanceTool->GetDistance( CurrentLocation, NewLocation );

		float fDeltaValue = fNewDistance - fOriginalDistance;
		if( fDeltaValue < fBestValue )
		{
			fBestValue = fDeltaValue;
			pBestValue_InsertBeforeStep = NULL;
		}
	}

	AddToPredefinedSteps_InsertBefore( pNewStep, pBestValue_InsertBeforeStep );
}

void  CEOSAIUnitPathwayFinder::InsertAsFirstStep( CEOSAIUnitPathwayPredefinedStep* pNewStep )
{
	if( m_PreDefinedPath.IsEmpty() )
	{
		AddToPredefinedSteps_InsertBefore( pNewStep, NULL );
	}
	else
	{
		AddToPredefinedSteps_InsertBefore( pNewStep, m_PreDefinedPath.GetHead() );
	}
}

void  CEOSAIUnitPathwayFinder::AddToPredefinedSteps_InsertBefore( CEOSAIUnitPathwayPredefinedStep* pNewStep, CEOSAIUnitPathwayPredefinedStep* pInsertBeforeStep )
{
	CEOSAILocation NewStepStartLocation = m_StartLocation;

	if( pInsertBeforeStep == NULL )
	{
		if( m_PreDefinedPath.IsEmpty() == FALSE ){ NewStepStartLocation = m_PreDefinedPath.GetTail()->GetEndLocation(); }
		pNewStep->SetStartLocation( NewStepStartLocation );
		m_PreDefinedPath.AddTail( pNewStep );

		if( m_pAIUnitActor->GetObjectId() == 14591 && pNewStep->IsAnUpgrade() )
		{
			int g=0;
		}
	}
	else
	{
		CEOSAIUnitPathwayPredefinedStep* pInsertAfterStep = pInsertBeforeStep->GetPrevStep();
		if( pInsertAfterStep )
		{
			NewStepStartLocation = pInsertAfterStep->GetEndLocation();
		}
		pNewStep->SetStartLocation( NewStepStartLocation );

		POSITION pos = m_PreDefinedPath.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
			if( pExistingStep == pInsertBeforeStep )
			{
				m_PreDefinedPath.InsertBefore( prevPos, pNewStep );

				if( m_pAIUnitActor->GetObjectId() == 14591 && pNewStep->IsAnUpgrade() )
				{
					int g=0;
				}

				// Update the NextItem Start Location
				CEOSAIUnitPathwayPredefinedStep* pNextStep = pNewStep->GetNextStep();
				if( pNextStep )
				{
					pNextStep->SetStartLocation( pNewStep->GetEndLocation() );
				}
				break;
			}
		}
	}
/*
	if( this->GetAIUnitActor() &&
		this->GetAIUnitActor()->GetObjectId() == 143 )
	{
		int g=0;
	}
*/
	pNewStep->UpdateStartToEndDistancesAndTimes();
	pNewStep->InvokeImaginaryTransportPath();
}



/*
void  CEOSAIUnitPathwayFinder::InsertPredefinedSubStep( CEOSAIUnitPathwayPredefinedStep* pStep, long iSubStep )
{
	// Find the best place to insert this step
	//   INCOMPLETE: a more complete model could take into account a variety of complications, including:
	//   (1) How does this slow-down existing Predefined steps?  Is it worth displacing them?
	//   (2) How does this new step mess-up the stealth?
	//       Example: if I am invading, I might want to skip past a water-oil resource and catch it later
	//   (3) Should I do non-combat tasks (capture resource) before combat tasks (capture city)?

	CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	CEOSAILocation CurrentLocation = m_StartLocation;

	CEOSAILocation NewLocation = pStep->GetSubStepEndLocation( iSubStep );

	float fBestValue = 1000000.0f; // Lower is better
	CEOSAIUnitPathwayPredefinedStepSeq* pBestValue_InsertBeforeStepSeq = NULL;
	POSITION pos = m_PreDefinedPathSequence.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStepSeq* pSeq = m_PreDefinedPathSequence.GetNext( pos );
		CEOSAILocation NextLocation = pSeq->GetEndLocation();

		float fOriginalDistance = pWorldBuildDesc->GetPixelDistance( CurrentLocation, NextLocation );
		float fNewDistance = 
			pWorldBuildDesc->GetPixelDistance( CurrentLocation, NewLocation ) +
			pWorldBuildDesc->GetPixelDistance( NewLocation, NextLocation );

		float fValue = fNewDistance - fOriginalDistance;
		if( fValue < fBestValue )
		{
			fBestValue = fValue;
			pBestValue_InsertBeforeStepSeq = pSeq;
		}
		CurrentLocation = NextLocation;
	}

	CEOSAIUnitPathwayPredefinedStepSeq* pNewSeq = new CEOSAIUnitPathwayPredefinedStepSeq( pStep, iSubStep );
	if( pBestValue_InsertBeforeStepSeq == NULL )
	{
		m_PreDefinedPathSequence.AddTail( pNewSeq );
	}
	else
	{
		POSITION pos = m_PreDefinedPathSequence.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CEOSAIUnitPathwayPredefinedStepSeq* pSeq = m_PreDefinedPathSequence.GetNext( pos );
			if( pSeq == pBestValue_InsertBeforeStepSeq )
			{
				m_PreDefinedPathSequence.InsertBefore( prevPos, pNewSeq );
			}
		}
	}
}

void  CEOSAIUnitPathwayFinder::AppendPredefinedSubStep( CEOSAIUnitPathwayPredefinedStep* pStep, long iSubStep )
{
	CEOSAIUnitPathwayPredefinedStepSeq* pNewSeq = new CEOSAIUnitPathwayPredefinedStepSeq( pStep, iSubStep );
	m_PreDefinedPathSequence.AddTail( pNewSeq );
}

void  CEOSAIUnitPathwayFinder::DeletePredefinedSubSteps()
{
	while( m_PreDefinedPathSequence.IsEmpty() == FALSE ){ delete m_PreDefinedPathSequence.RemoveHead(); }
}
*/

void  CEOSAIUnitPathwayFinder::DeletePredefinedStep( CEOSAIUnitPathwayPredefinedStep* pStep )
{
	/*
	POSITION pos = m_PreDefinedPathSequence.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnitPathwayPredefinedStepSeq* pSeq = m_PreDefinedPathSequence.GetNext( pos );
		if( pSeq->m_pPredefinedStep == pStep )
		{
			delete pSeq;
			m_PreDefinedPath.RemoveAt( prevPos );
		}
	}
	*/
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnitPathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		if( pExistingStep == pStep )
		{
			m_PreDefinedPath.RemoveAt( prevPos );
			delete pExistingStep;
		}
	}
}


bool CEOSAIUnitPathwayFinder::FindPickupDropoffLocations( 
		long iWaterAIRegion, long iLandAIRegion,
		CEOSAILocation* pWaterLocation, CEOSAILocation* pLandLocation )
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
	CEOSAIRegion2* pWaterAIRegion = pAIRegionManager->GetAIRegion( iWaterAIRegion );
	CEOSAIRegion2* pLandAIRegion = pAIRegionManager->GetAIRegion( iLandAIRegion );

	if( pWaterAIRegion == pLandAIRegion )
	{
		*pWaterLocation = pWaterAIRegion->GetLocation();
		*pLandLocation = pWaterAIRegion->GetLocation();
		return true;
	}

	ASSERT( pWaterAIRegion );
	ASSERT( pWaterAIRegion->IsWater() );
	ASSERT( pLandAIRegion );
	ASSERT( pLandAIRegion->IsLand() || pLandAIRegion->IsCity() );

	// Sanity Check
	ASSERT( pWaterAIRegion->IsAdjacentToRegion( pLandAIRegion ) );

	if( pWaterAIRegion->GetAdjacentRegionJumpOffLocation( iLandAIRegion, pWaterLocation ) &&
		pLandAIRegion->GetAdjacentRegionJumpOffLocation( iWaterAIRegion, pLandLocation ) )
	{
		// DEBUG
		ASSERT( pAIRegionManager->GetAIRegion( *pWaterLocation )->GetId() == iWaterAIRegion );
		ASSERT( pAIRegionManager->GetAIRegion( *pLandLocation )->GetId() == iLandAIRegion );
		// DEBUG

		return true;
	}
	return false;
}


//

CEOSAIUnitPathwayPredefinedStep* CEOSAIUnitPathwayFinder::GetPredefinedPathStep( long iStep )
{
	long iCurrentStep = 0;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPoint = m_PreDefinedPath.GetNext( pos );
		if( iCurrentStep == iStep ) return pPoint;
		iCurrentStep++;
	}
	return NULL;
}

/*
void CEOSAIUnitPathwayFinder::ClearResults()
{
	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }
}
*/

void  CEOSAIUnitPathwayFinder::UpdateRedPath()
{
	//CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->UpdateRedMovementPath();
	}
}

float CEOSAIUnitPathwayFinder::CalculateRedPathArrivalTimeAtGeo( long iTargetGeo )
{
	// Make sure the RedPath is accurate
	UpdateRedPath();

	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();

	if( m_FullRedMovementPath.IsEmpty() ){ return 0.0f; }
	//ASSERT( m_FullRedMovementPath.IsEmpty() == FALSE ); - this can happen with paratroopers
	//  But, I should get it working properly with paratroopers (it involves arrival time calculations)
	POSITION pos = m_FullRedMovementPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStepPathStep* pStep = m_FullRedMovementPath.GetNext( pos );
		long iGeo = pAIRegionManager->GetAIRegion( pStep->m_pPathfinderPoint->m_Location )->GetGeoId();
		if( iTargetGeo == iGeo )
		{
			return pStep->m_fOffsetTime;
		}
	}
	//ASSERT( false );
	return -1.0f;
}

bool CEOSAIUnitPathwayFinder::ResultPathIsValid()
{
	if( m_pResultPath == NULL ) return false;
	return m_pResultPath->PathIsValid();
}

void CEOSAIUnitPathwayFinder::GetFullRedMovementPath( CList< CEOSAIUnitPathwayPredefinedStepPathStep* >* pTimedMovementPath )
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		float fStartTime = pPredefinedStep->GetStartTime();
		pPredefinedStep->GetFullRedMovementPath( fStartTime, pTimedMovementPath );
	}
}

void CEOSAIUnitPathwayFinder::ConstructFullRedPath()
{
	while( m_FullRedMovementPath.IsEmpty() == FALSE ){ delete m_FullRedMovementPath.RemoveHead(); }
	GetFullRedMovementPath( &m_FullRedMovementPath );
}

void CEOSAIUnitPathwayFinder::AddPickupToDropoffToTimedMovementPath( 
		CEOSAIUnitPathwayPredefinedStep* pTransporteeStep,
		float* fTransporteeTime,
		CList< CEOSAIUnitPathwayPredefinedStepPathStep* >* pTimedPath )
{
	float fPickupTime = *fTransporteeTime;
	//float fTransporteeStartTime = *fTransporteeTime;
	bool bPickupExists = false;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		if( pPredefinedStep->IsAPickup() &&
			pPredefinedStep->GetTransporteeStep() == pTransporteeStep )
		{
			fPickupTime = pPredefinedStep->GetRedPathEndTime( CEOSAIUnitPathwayPredefinedStep::enum_EndOfStep );
			*fTransporteeTime = fPickupTime;

			bPickupExists = true;
			break;
		}
	}
	bool bPickupFound = false;
	if( bPickupExists == false ){ bPickupFound = true; }
	pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );

		if( bPickupFound == false )
		{
			if( pPredefinedStep->IsAPickup() &&
				pPredefinedStep->GetTransporteeStep() == pTransporteeStep )
			{
				bPickupFound = true;
				continue;
			}
		}
		else // Pickup has been found
		{
			float fMovementRate = m_pAIUnitTemplate->GetMovementRate();
			//CWorldBuildDesc* pWorldBuildDesc = g_pCommonState->GetWorldBuildDesc();
			//float fTransportStartTime = pPredefinedStep->GetStartTime();

			// Add the pathway
			//float fTimeOffset = 0.0f;
			CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;
			POSITION pos2 = pPredefinedStep->GetDirectMovementPath()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIPathfinderPoint* pCurrentPathfinderPoint = pPredefinedStep->GetDirectMovementPath()->GetNext( pos2 );
				if( pCurrentPathfinderPoint && pLastPathfinderPoint )
				{
					float fDistance = g_pWorldDistanceTool->GetDistance(
						pCurrentPathfinderPoint->GetLocation(), pLastPathfinderPoint->GetLocation() );
					//float fDistance = pWorldBuildDesc->GetPixelDistance(
					//	pCurrentPathfinderPoint->GetLocation(), pLastPathfinderPoint->GetLocation() );
					float fAdditionalTime = fDistance / fMovementRate;
					//fTimeOffset += fAdditionalTime;
					*fTransporteeTime += fAdditionalTime;
				}
				pTimedPath->AddTail(
					new CEOSAIUnitPathwayPredefinedStepPathStep( pCurrentPathfinderPoint, *fTransporteeTime ) );

				pLastPathfinderPoint = pCurrentPathfinderPoint;
			}

			// Found the dropoff, can exit out now
			if( pPredefinedStep->IsADropoff() &&
				pPredefinedStep->GetTransporteeStep() == pTransporteeStep )
			{
				return;
			}
		}
	}
}
/*
void CEOSAIUnitPathwayFinder::AddPickupToDropoffToTimedMovementPath( 
		CEOSAIUnitPathwayPredefinedStep* pTransporteeStep,
		float* fTransporteeTime )
{
	bool bPickupExists = false;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		if( pPredefinedStep->IsAPickup() &&
			pPredefinedStep->GetTransporteeStep() == pTransporteeStep )
		{
			bPickupExists = true;
			break;
		}
	}
	bool bPickupFound = false;
	if( bPickupExists == false ){ bPickupFound = true; }
	pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );

		if( bPickupFound == false )
		{
			if( pPredefinedStep->IsAPickup() &&
				pPredefinedStep->GetTransporteeStep() == pTransporteeStep )
			{
				bPickupFound = true;
				continue;
			}
		}
		else // Pickup has been found
		{
			float fMovementRate = m_pUnitTemplate->GetMovementRate();
			CWorldBuildDesc* pWorldBuildDesc = g_pCommonState->GetWorldBuildDesc();

			// Add the pathway
			CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;
			POSITION pos2 = pPredefinedStep->GetDirectMovementPath()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIPathfinderPoint* pCurrentPathfinderPoint = pPredefinedStep->GetDirectMovementPath()->GetNext( pos2 );
				if( pCurrentPathfinderPoint && pLastPathfinderPoint )
				{
					float fDistance = pWorldBuildDesc->GetPixelDistance(
						pCurrentPathfinderPoint->GetLocation(), pLastPathfinderPoint->GetLocation() );
					float fAdditionalTime = fDistance / fMovementRate;
					*fTransporteeTime += fAdditionalTime;
				}
				pTransporteeStep->GetTimedMovementPath()->AddTail(
					new CEOSAIUnitPathwayPredefinedStepPathStep( pCurrentPathfinderPoint, *fTransporteeTime ) );

				pLastPathfinderPoint = pCurrentPathfinderPoint;
			}

			// Found the dropoff, can exit out now
			if( pPredefinedStep->IsADropoff() &&
				pPredefinedStep->GetTransporteeStep() == pTransporteeStep )
			{
				return;
			}
		}
	}
}
*/

bool CEOSAIUnitPathwayFinder::IncludesAnAttack()
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		//CAIAirPathPoint* pPoint = m_PreDefinedPath.GetNext( pos );
		//if( dynamic_cast< CAIAirPathAttackTarget* >( pPoint ) ) return true;
		CEOSAIUnitPathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		if( pStep->IsAnAttack() ) return true;
	}
	return false;
}

void  CEOSAIUnitPathwayFinder::ValidatePredefinedStepsStartEndLocations()
{
	CEOSAILocation CurrentLocation = m_StartLocation;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->SetStartLocation( CurrentLocation );
		CurrentLocation = pPredefinedStep->GetEndLocation();
	}
}

// Create orders
//
void  CEOSAIUnitPathwayFinder::CreateOrders() // Create a Pathway Result (if it doesn't exist) + Create Orders
{
	if( m_PreDefinedPath.IsEmpty() )
	{
		return; // No orders to create
	}

	if( m_pResultPath == NULL )
	{
		CalculateResultPath();
	}
	if( EOSAISettings::m_bAssertWhenAIPathwayFails )
	{
		ASSERT( ResultPathIsValid() );
	}
	if( m_pResultPath->AllOrdersWereCreated() == false &&
		ResultPathIsValid() )
	{
		//bool bAllOrdersWereCreated = m_pResultPath->CreateOrders();
		m_pResultPath->m_bAllOrdersWereCreated = m_pResultPath->CreateOrders();
	}
}

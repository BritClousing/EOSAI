
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
#include "EOSAIUnit2.h"
#include "City.h"
#include "EOSAIAirfield.h"
#include "PoiObject.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIBrain.h"
#include "EOSAICommonData2.h"
#include "EOSAIMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIUnit2PathwayFinder*  CEOSAIUnit2PathwayFinder::CreatePathwayFinder( CEOSAIUnit2* pAIUnitActor )
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

CEOSAIUnit2PathwayFinder*  CEOSAIUnit2PathwayFinder::CreatePathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime )
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
CEOSAIUnit2Pathway* CEOSAIUnit2Pathway::CreatePathway( long iPlayer, CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate )
{
	if( dynamic_cast< CEOSAIUnit2* >( pActor ) )
	{
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pActor );
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

CEOSAIUnit2PathwayFinder::CEOSAIUnit2PathwayFinder( CEOSAIUnit2* pAIUnitActor )
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

CEOSAIUnit2PathwayFinder::CEOSAIUnit2PathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime )
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
CEOSAIUnit2PathwayFinder::CEOSAIUnit2PathwayFinder( CEOSAIBrain* pAIBrain )
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
CEOSAIUnit2PathwayFinder::~CEOSAIUnit2PathwayFinder()
{
	ClearPredefinedAndResultPaths();
	//while( m_PreDefinedPath.IsEmpty() == FALSE ){ delete m_PreDefinedPath.RemoveHead(); }
	while( m_FullRedMovementPath.IsEmpty() == FALSE ){ delete m_FullRedMovementPath.RemoveHead(); }
}

void CEOSAIUnit2PathwayFinder::ClearPredefinedAndResultPaths()
{
	while( m_PreDefinedPath.IsEmpty() == FALSE )
	{
		delete m_PreDefinedPath.RemoveHead();
	}
	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }
}

/*
long CEOSAIUnit2PathwayFinder::GetPlayer()
{
	return m_pAIBrain->GetAIPlayerNumber();
}
*/
CEOSAIPoiObject*  CEOSAIUnit2PathwayFinder::GetAIPoiObjectActor()
{
	if( m_pAIUnitActor ){ return m_pAIUnitActor; } 
	if( m_pAICityActor ){ return m_pAICityActor; } 
	ASSERT( false );
	return NULL;
}
/*
float CEOSAIUnit2PathwayFinder::GetEndTime()
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
		CEOSAIUnit2PathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
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
void  CEOSAIUnit2PathwayFinder::SetUnit( CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate )
{
	//m_pAIBrain = pActor->GetAIBrain();
	m_iUnitOwner = pActor->GetOwner();
	m_pActor = pActor;
	m_pUnitTemplate = pUnitTemplate;
	m_StartLocation = pActor->GetPoiObject()->GetLocation();
}
*/
void  CEOSAIUnit2PathwayFinder::ResetForeignRelations()
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	for( long i=0; i<iNumberOfPlayers+1; i++ )
	{
		m_OwnerForeignRelations[i] = g_pEOSAICommonData->GetForeignRelations( m_iUnitOwner, i );
	}
}

bool CEOSAIUnit2PathwayFinder::NeedsTransport()
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
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
void CEOSAIUnit2PathwayFinder::AddWaypoint( CEOSAILocation Location )
{
	CEOSAIUnit2PathwayPredefinedStep* pGroundPathPoint = new CEOSAIUnit2PathwayPredefinedStep( this );
	pGroundPathPoint->GotoWaypoint( Location );
//	pGroundPathPoint->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_Waypoint;
//	pGroundPathPoint->m_EndLocation = Location;
	//m_PreDefinedPath.AddTail( pGroundPathPoint );
	AppendStep( pGroundPathPoint );
}

// Pickup/Dropff
/*
void  CEOSAIUnit2PathwayFinder::AddPickupTarget( CEOSAIUnit2* pAIUnitToPickup, CEOSAILocation OtherUnitLocation )
{
	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->Pickup( pAIUnitToPickup, OtherUnitLocation );
//	pStep->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_PickupTarget;
//	pStep->m_pAITarget = pAIUnitToPickup;
//	pStep->m_EndLocation = OtherUnitLocation;
	//m_PreDefinedPath.AddTail( pStep );
	AppendPreDefinedStep( pStep );
	//CAISeaPathPoint* pPoint = new CAISeaPathPoint();
	//pPoint->m_pAITarget = pAIUnitToPickup;
	//pPoint->m_TargetLocation = OtherUnitLocation;//pAIUnitToPickup->GetPoiObject()->GetLocation();
	//m_PreDefinedPath.AddTail( pPoint );
}

void  CEOSAIUnit2PathwayFinder::AddDropoffTarget( CEOSAIUnit2* pAIUnitToDropoff, CEOSAILocation OtherUnitTargetLocation ) // Dropoff near this location
{
	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->Dropoff( pAIUnitToDropoff, OtherUnitTargetLocation );
//	pStep->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_DropoffTarget;
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
CEOSAIUnit2PathwayPredefinedStep* CEOSAIUnit2PathwayFinder::CreateTransportAction( CEOSAIUnit2* pAITransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeEndLocation )
{
	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->Transport( pAITransportee, TransporteeStartLocation, TransporteeEndLocation );
	return pStep;
}
*/
void CEOSAIUnit2PathwayFinder::AppendTransportSteps( CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep )
{
	ASSERT( GetAIUnitTemplate()->CanContain( pTransporteeStep->GetAIUnitPathwayFinder()->GetAIUnitTemplate() ) );
	CEOSAIUnit2PathwayPredefinedStep* pStep1 = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep1->Pickup( pTransporteeStep );
	AppendStep( pStep1 );

	CEOSAIUnit2PathwayPredefinedStep* pStep2 = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep2->Dropoff( pTransporteeStep );
	AppendStep( pStep2 );
}

CEOSAIUnit2* CEOSAIUnit2PathwayFinder::GetCurrentTransport()
{
	if( GetAIUnitActor() )
	{
		//CUnit* pGroundUnit = GetAIUnitActor()->GetServerUnit();
		CEOSAIUnit2* pGroundAIUnit = GetAIUnitActor();//->GetServerUnit();
		if( pGroundAIUnit )
		{
			CEOSAIPoiObject* pAIPoiObject = pGroundAIUnit->GetContainerAIPoiObject();
			if( pAIPoiObject )
			{
				//long iUnitObjectId = pAIPoiObject->GetObjectId();
				//CEOSAIPoiObject* pAIPoiObject = m_pAIBrain->GetAIPoiObject( iUnitObjectId );
				return dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
			}
		}
	}
	return NULL;
}

void CEOSAIUnit2PathwayFinder::InsertTransportSteps( CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep )
{
	ASSERT( GetAIUnitTemplate()->CanContain( pTransporteeStep->GetAIUnitPathwayFinder()->GetAIUnitTemplate() ) );

	CEOSAIUnit2* pAIGroundUnitContainerUnit = pTransporteeStep->GetAIUnitPathwayFinder()->GetCurrentTransport();
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
		CEOSAIUnit2PathwayPredefinedStep* pStep1 = new CEOSAIUnit2PathwayPredefinedStep( this );
		pStep1->Pickup( pTransporteeStep );
		InsertPickupOrDropoffStep( pStep1 );
	}

	CEOSAIUnit2PathwayPredefinedStep* pStep2 = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep2->Dropoff( pTransporteeStep );
	InsertPickupOrDropoffStep( pStep2 );

	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }

	// Update Predefined Path
	CEOSAIUnit2* pAITransport = GetAIUnitActor();
	pAITransport->GetAIUnitPathwayFinder()->UpdateTransportPickupDropoffPath( pTransporteeStep );
}

void CEOSAIUnit2PathwayFinder::DeleteTransportSteps( CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep )
{
	bool bStepWasRemoved = false;

	ASSERT( pTransporteeStep != NULL );
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnit2PathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
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

CEOSAIUnit2PathwayPredefinedStep* CEOSAIUnit2PathwayFinder::GetTransportPickupStep( CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep )
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		if( pExistingStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_PickupTarget &&
			pExistingStep->GetTransporteeStep() == pTransporteeStep )
		{
			return pExistingStep;
		}
	}
	return NULL;
}

CEOSAIUnit2PathwayPredefinedStep* CEOSAIUnit2PathwayFinder::GetTransportDropoffStep( CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep )
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		if( pExistingStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_DropoffTarget &&
			pExistingStep->GetTransporteeStep() == pTransporteeStep )
		{
			return pExistingStep;
		}
	}
	return NULL;
}

bool  CEOSAIUnit2PathwayFinder::UpdateTransportPickupDropoffPath( CEOSAIUnit2PathwayPredefinedStep* pGroundUnitStep )
{
	ASSERT( GetAIUnitTemplate()->IsTransport() );
	bool bAirTransport = GetAIUnitTemplate()->IsAirUnit();
	//
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnit2PathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
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
	//virtual bool UpdateTransportAssistedPath( CEOSAIUnit2PathwayPredefinedStep* pGroundUnitStep );
}

//

void  CEOSAIUnit2PathwayFinder::SetTransportToUse( CEOSAIUnit2* pAITransportToUse )
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		if( pStep->RequiresTransport() )
		{
			pStep->UseTransportAndCreatePickupDropoffSteps( pAITransportToUse );
		}
	}
}

/*
void CEOSAIUnit2PathwayFinder::AddTransportAction( CEOSAIUnit2* pAITransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeEndLocation )
{
	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->Transport( pAITransportee, TransporteeStartLocation, TransporteeEndLocation );
//	pStep->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_Transport;
//	pStep->m_pAITarget = pAITransportee;
//	pStep->m_TransporteeStartLocation = TransporteeStartLocation;
//	pStep->m_TransporteeEndLocation = TransporteeEndLocation;
	//pStep->m_EndLocation = OtherUnitTargetLocation;
	AppendStep( pStep );
}
*/
// Airfield (ground unit only)
void CEOSAIUnit2PathwayFinder::AddBuildAirfield( CEOSAILocation Location )
{
	ASSERT( m_pAIUnitTemplate->CanBuildAirfields() );

	// The location must be a ground location
	//CWorldDesc* pWorldDesc = m_pAIBrain->GetWorldDescServer();
	//if( pWorldDesc->IsOnLand( Location ) )
	{
		CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
		pStep->BuildAirfield( Location );
	//	pGroundPathPoint->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_BuildAirfield;
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

void  CEOSAIUnit2PathwayFinder::LandAtAirbase( CEOSAIPoiObject* pAirbase ) // city,airbase,carrier
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

		CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
		pStep->LandAtAirbase( pAirbase );
	//	pStep->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_LandAtAirbase;
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

void  CEOSAIUnit2PathwayFinder::LandAtAnyAirbase()
{
	ASSERT( m_pAIUnitTemplate->HasARange() );

	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->LandAtAnyAirbase();
//	pStep->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_LandAtAirbase;
	//pStep->m_bLandAtAirfield = true;
//	pStep->m_pAITarget = NULL;
	//m_PreDefinedPath.AddTail( pStep );
	AppendStep( pStep );
	//CAIAirPathAirbase* pPoint = new CAIAirPathAirbase();
	//m_PreDefinedPath.AddTail( pPoint );
}

// Repair
//void CEOSAIUnit2PathwayFinder::AddRepairTask( CEOSAICity* pAICity )
void CEOSAIUnit2PathwayFinder::AddRepairTask( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->RepairAt( pAIPoiObject );
//	pStep->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_Repair;
	//pStep->m_bRepair = true;
//	pStep->m_pAITarget = pAICity;
//	pStep->m_EndLocation = pAICity->GetPoiObject()->GetLocation();
	//m_PreDefinedPath.AddTail( pStep );
	AppendStep( pStep );
}
/*
void CEOSAIUnit2PathwayFinder::AddRepairTaskAnywhere()
{
	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->RepairAnywhere();
//	pStep->m_eTask = CEOSAIUnit2PathwayPredefinedStep::enum_Repair;
	//pStep->m_bRepair = true;
//	pStep->m_pAITarget = NULL;
	//m_PreDefinedPath.AddTail( pStep );
	AppendStep( pStep );
}
*/

// Attack
CEOSAIUnit2PathwayPredefinedStep* CEOSAIUnit2PathwayFinder::CreateCaptureTarget( CEOSAIPoiObject* pTarget )
{
	ASSERT( m_pAIUnitTemplate->CanCapture( pTarget ) );
	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->CaptureCitRes( pTarget );
	//InsertStep( pStep );
	return pStep;
}

CEOSAIUnit2PathwayPredefinedStep* CEOSAIUnit2PathwayFinder::CreateDegradeTarget( CEOSAIPoiObject* pTarget )
{
	ASSERT( dynamic_cast< CEOSAICity* >( pTarget ) || dynamic_cast< CEOSAIAirfield* >( pTarget ) );
	ASSERT( m_pAIUnitTemplate->CanDegrade( pTarget ) );

	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->DegradeTarget( pTarget );
	//InsertStep( pStep );
	return pStep;
}

CEOSAIUnit2PathwayPredefinedStep* CEOSAIUnit2PathwayFinder::CreateAttackUnit( CEOSAIPoiObject* pAITarget )
{
	CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAITarget );
	ASSERT( pAIUnit );
	ASSERT( m_pAIUnitTemplate->CanAttack( pAITarget ) );

	CEOSAIUnit2PathwayPredefinedStep* pStep = new CEOSAIUnit2PathwayPredefinedStep( this );
	pStep->AttackUnit( pAIUnit );
	//InsertStep( pStep );
	return pStep;
}


bool CEOSAIUnit2PathwayFinder::PathwayShouldAppendALanding()
{
	if( m_pAIUnitTemplate->HasARange() == false ) return false;
	if( m_PreDefinedPath.GetCount() == 0 ) return false;

	// If the air unit is going to self-destruct during the pathway, don't bother finding a landing
	if( IncludesAnAttack() && m_pAIUnitTemplate->SelfDestructsOnAttack() ) return false;

	// If the air unit cannot land, don't append a landing
	if( m_pAIUnitTemplate->CanLandAfterLaunch() == false ) return false;

	// Already has an airbase at the end?
	CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetTail();
	if( pPredefinedStep->IsALandAtAirbase() ) return false;

	return true;
}


void  CEOSAIUnit2PathwayFinder::AppendStep( CEOSAIUnit2PathwayPredefinedStep* pNewStep )
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


void  CEOSAIUnit2PathwayFinder::InsertPickupOrDropoffStep( CEOSAIUnit2PathwayPredefinedStep* pNewStep )
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
	if( pNewStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_DropoffTarget )
	{
		bThisIsADropoffStep = true;
		POSITION pos = m_PreDefinedPath.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit2PathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
			if( pExistingStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_PickupTarget &&
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
	CEOSAIUnit2PathwayPredefinedStep* pBestValue_InsertBeforeStep = NULL;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		CEOSAILocation NextLocation = pExistingStep->GetEndLocation();

		// If this is a dropoff step:
		//   Wait until I find the pickup step in the list
		if( bThisIsADropoffStep &&
			bThereIsAPickupStep &&
			bPickupStepHasBeenFound == false )
		{
			if( pExistingStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_PickupTarget &&
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
			pExistingStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_PickupTarget &&
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

void  CEOSAIUnit2PathwayFinder::InsertAsFirstStep( CEOSAIUnit2PathwayPredefinedStep* pNewStep )
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

void  CEOSAIUnit2PathwayFinder::AddToPredefinedSteps_InsertBefore( CEOSAIUnit2PathwayPredefinedStep* pNewStep, CEOSAIUnit2PathwayPredefinedStep* pInsertBeforeStep )
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
		CEOSAIUnit2PathwayPredefinedStep* pInsertAfterStep = pInsertBeforeStep->GetPrevStep();
		if( pInsertAfterStep )
		{
			NewStepStartLocation = pInsertAfterStep->GetEndLocation();
		}
		pNewStep->SetStartLocation( NewStepStartLocation );

		POSITION pos = m_PreDefinedPath.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CEOSAIUnit2PathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
			if( pExistingStep == pInsertBeforeStep )
			{
				m_PreDefinedPath.InsertBefore( prevPos, pNewStep );

				if( m_pAIUnitActor->GetObjectId() == 14591 && pNewStep->IsAnUpgrade() )
				{
					int g=0;
				}

				// Update the NextItem Start Location
				CEOSAIUnit2PathwayPredefinedStep* pNextStep = pNewStep->GetNextStep();
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
void  CEOSAIUnit2PathwayFinder::InsertPredefinedSubStep( CEOSAIUnit2PathwayPredefinedStep* pStep, long iSubStep )
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
	CEOSAIUnit2PathwayPredefinedStepSeq* pBestValue_InsertBeforeStepSeq = NULL;
	POSITION pos = m_PreDefinedPathSequence.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStepSeq* pSeq = m_PreDefinedPathSequence.GetNext( pos );
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

	CEOSAIUnit2PathwayPredefinedStepSeq* pNewSeq = new CEOSAIUnit2PathwayPredefinedStepSeq( pStep, iSubStep );
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
			CEOSAIUnit2PathwayPredefinedStepSeq* pSeq = m_PreDefinedPathSequence.GetNext( pos );
			if( pSeq == pBestValue_InsertBeforeStepSeq )
			{
				m_PreDefinedPathSequence.InsertBefore( prevPos, pNewSeq );
			}
		}
	}
}

void  CEOSAIUnit2PathwayFinder::AppendPredefinedSubStep( CEOSAIUnit2PathwayPredefinedStep* pStep, long iSubStep )
{
	CEOSAIUnit2PathwayPredefinedStepSeq* pNewSeq = new CEOSAIUnit2PathwayPredefinedStepSeq( pStep, iSubStep );
	m_PreDefinedPathSequence.AddTail( pNewSeq );
}

void  CEOSAIUnit2PathwayFinder::DeletePredefinedSubSteps()
{
	while( m_PreDefinedPathSequence.IsEmpty() == FALSE ){ delete m_PreDefinedPathSequence.RemoveHead(); }
}
*/

void  CEOSAIUnit2PathwayFinder::DeletePredefinedStep( CEOSAIUnit2PathwayPredefinedStep* pStep )
{
	/*
	POSITION pos = m_PreDefinedPathSequence.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnit2PathwayPredefinedStepSeq* pSeq = m_PreDefinedPathSequence.GetNext( pos );
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
		CEOSAIUnit2PathwayPredefinedStep* pExistingStep = m_PreDefinedPath.GetNext( pos );
		if( pExistingStep == pStep )
		{
			m_PreDefinedPath.RemoveAt( prevPos );
			delete pExistingStep;
		}
	}
}


bool CEOSAIUnit2PathwayFinder::FindPickupDropoffLocations( 
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

CEOSAIUnit2PathwayPredefinedStep* CEOSAIUnit2PathwayFinder::GetPredefinedPathStep( long iStep )
{
	long iCurrentStep = 0;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPoint = m_PreDefinedPath.GetNext( pos );
		if( iCurrentStep == iStep ) return pPoint;
		iCurrentStep++;
	}
	return NULL;
}

/*
void CEOSAIUnit2PathwayFinder::ClearResults()
{
	if( m_pResultPath ){ delete m_pResultPath; m_pResultPath = NULL; }
}
*/

void  CEOSAIUnit2PathwayFinder::UpdateRedPath()
{
	//CEOSAIPathfinderPoint* pLastPathfinderPoint = NULL;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->UpdateRedMovementPath();
	}
}

float CEOSAIUnit2PathwayFinder::CalculateRedPathArrivalTimeAtGeo( long iTargetGeo )
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
		CEOSAIUnit2PathwayPredefinedStepPathStep* pStep = m_FullRedMovementPath.GetNext( pos );
		long iGeo = pAIRegionManager->GetAIRegion( pStep->m_pPathfinderPoint->m_Location )->GetGeoId();
		if( iTargetGeo == iGeo )
		{
			return pStep->m_fOffsetTime;
		}
	}
	//ASSERT( false );
	return -1.0f;
}

bool CEOSAIUnit2PathwayFinder::ResultPathIsValid()
{
	if( m_pResultPath == NULL ) return false;
	return m_pResultPath->PathIsValid();
}

void CEOSAIUnit2PathwayFinder::GetFullRedMovementPath( CList< CEOSAIUnit2PathwayPredefinedStepPathStep* >* pTimedMovementPath )
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		float fStartTime = pPredefinedStep->GetStartTime();
		pPredefinedStep->GetFullRedMovementPath( fStartTime, pTimedMovementPath );
	}
}

void CEOSAIUnit2PathwayFinder::ConstructFullRedPath()
{
	while( m_FullRedMovementPath.IsEmpty() == FALSE ){ delete m_FullRedMovementPath.RemoveHead(); }
	GetFullRedMovementPath( &m_FullRedMovementPath );
}

void CEOSAIUnit2PathwayFinder::AddPickupToDropoffToTimedMovementPath( 
		CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep,
		float* fTransporteeTime,
		CList< CEOSAIUnit2PathwayPredefinedStepPathStep* >* pTimedPath )
{
	float fPickupTime = *fTransporteeTime;
	//float fTransporteeStartTime = *fTransporteeTime;
	bool bPickupExists = false;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		if( pPredefinedStep->IsAPickup() &&
			pPredefinedStep->GetTransporteeStep() == pTransporteeStep )
		{
			fPickupTime = pPredefinedStep->GetRedPathEndTime( CEOSAIUnit2PathwayPredefinedStep::enum_EndOfStep );
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
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );

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
					new CEOSAIUnit2PathwayPredefinedStepPathStep( pCurrentPathfinderPoint, *fTransporteeTime ) );

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
void CEOSAIUnit2PathwayFinder::AddPickupToDropoffToTimedMovementPath( 
		CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep,
		float* fTransporteeTime )
{
	bool bPickupExists = false;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
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
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );

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
					new CEOSAIUnit2PathwayPredefinedStepPathStep( pCurrentPathfinderPoint, *fTransporteeTime ) );

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

bool CEOSAIUnit2PathwayFinder::IncludesAnAttack()
{
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		//CAIAirPathPoint* pPoint = m_PreDefinedPath.GetNext( pos );
		//if( dynamic_cast< CAIAirPathAttackTarget* >( pPoint ) ) return true;
		CEOSAIUnit2PathwayPredefinedStep* pStep = m_PreDefinedPath.GetNext( pos );
		if( pStep->IsAnAttack() ) return true;
	}
	return false;
}

void  CEOSAIUnit2PathwayFinder::ValidatePredefinedStepsStartEndLocations()
{
	CEOSAILocation CurrentLocation = m_StartLocation;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_PreDefinedPath.GetNext( pos );
		pPredefinedStep->SetStartLocation( CurrentLocation );
		CurrentLocation = pPredefinedStep->GetEndLocation();
	}
}

// Create orders
//
void  CEOSAIUnit2PathwayFinder::CreateOrders() // Create a Pathway Result (if it doesn't exist) + Create Orders
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

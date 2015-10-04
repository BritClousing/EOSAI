
#include "stdafx.h"
#include "PoiObject.h"
#include "EOSAIInterface.h"

#include "AIPlayer.h"
//#include "ResourceSource.h"
#include "City.h"
//#include "Player.h"
#include "EOSAIBrain.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"
#include "EOSAIAction.h"
#include "EOSAITask.h"
#include "EOSAITacticalProject2.h"
#include "EOSAIDesire2.h"
#include "EOSAIUnitAction.h"
#include "EOSAIUnitActionIdea.h"
//#include "Unit.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAIUnitTask_Dropoff.h"
#include "EOSAIUnitTask_Pickup.h"
#include "EOSAIUnitTask_Enter.h"
#include "EOSAIUnitTask_Exit.h"
#include "EOSAIUnitTask_MoveTo.h"
#include "EOSAIDesireCaptureCitRes.h"
#include "EOSAIUnitTask_CaptureCitRes.h"
#include "EOSAITransportAssistedPath.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
//bool SortAIUnitActionValuesByTTTScore( CEOSAIUnit2ActionIdea* p1, CEOSAIUnit2ActionIdea* p2 );
//bool SortAIUnitActionValuesByTotalTimeToTarget( CEOSAIUnit2ActionIdea* p1, CEOSAIUnit2ActionIdea* p2 );

CEOSAIPoiObject::CEOSAIPoiObject()// CEOSAIBrain* pAIBrain ) //: CAIObject( pAIBrain )
{
	//m_pAIPlayer = pAIBrain->GetAIPlayer();
	m_iPoiObjectId = 0;
	//m_AllMyUnitActionIdeas.SetSortFunction( SortAIUnitActionValuesByTTTScore );
	//m_AllMyUnitActionIdeas.SetSortFunction( SortAIUnitActionValuesByTotalTimeToTarget );

	//m_bIPlanOnCapturingThis = false;
	m_eBorderViolation = EOSAIEnumBorderViolation_None;
	//m_iLocationOwner = 0;
	//m_iLocationOwnershipLevel = 0;
	//m_iLastLocationOwner = 0;
	//m_iLastLocationOwnershipLevel = 0;

	m_pAllocatedUnitActionIdea = NULL;
	//m_pTransportAllocateToTacticalProject = NULL;

	//m_pServerPoiObject = NULL;
	//m_pPlayerPoiObject = NULL;

	//m_pAIBrain = pAIBrain;
	m_bHypotheticalPoiObject = false;
	m_bIsAlive = true;

	m_bIsLandingPadForAirUnits = false;
	m_bIsLaunchingPadForAirUnits = false;

	m_fGroundUnitRepairRate = 0.0f;
	m_fAirUnitRepairRate = 0.0f;
	m_fSeaUnitRepairRate = 0.0f;

	//m_InitialState.SetAIBrain( pAIBrain );
	//m_AIObjectActionProgression.SetAIObject( this );
}

CEOSAIPoiObject::~CEOSAIPoiObject()
{
	//while( m_AllMyUnitActionIdeas.IsEmpty() == FALSE ){ delete m_AllMyUnitActionIdeas.RemoveHead(); }
	/*
	while( m_AIActions.IsEmpty() == FALSE )
	{
		delete m_AIActions.RemoveHead();
	}
	while( m_AITasks.IsEmpty() == FALSE )
	{
		delete m_AITasks.RemoveHead();
	}
	*/
}
/*
void CEOSAIPoiObject::SetServerPoiObject( CPoiObject* pPoiObject )
{
	m_pServerPoiObject = pPoiObject;

	pPoiObject->SetAIPoiObject( this );

	m_bIPlanOnCapturingThis = false;

	CEOSAILocation InitialLocation = pPoiObject->GetLocation();
	if( pPoiObject->GetContainer() == NULL )
	{
		if( pPoiObject->IsGroundUnit() )
		{
			CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
			pWorldDescServer->GetValidLocation( InitialLocation, 1, EOSAIEnumMovementType3_Land, &InitialLocation );
		}
		if( pPoiObject->IsSeaUnit() )
		{
			CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
			pWorldDescServer->GetValidLocation( InitialLocation, 1, EOSAIEnumMovementType3_Water, &InitialLocation );
		}
	}

	m_InitialState.SetGroundUnit( pPoiObject->IsGroundUnit() );
	m_InitialState.SetLocationAndRegions( InitialLocation );//pPoiObject->GetLocation() );
	m_InitialState.SetOwner( pPoiObject->GetOwner() );
	m_InitialState.SetTime( 0.0f );
	m_InitialState.SetContainer( NULL );
}
*/
/*
void CEOSAIPoiObject::SetPlayerPoiObject( CPoiObject* pPoiObject )
{
	m_pPlayerPoiObject = pPoiObject;
}
*/
/*
void CEOSAIPoiObject::SetPoiObject( CPoiObject* pPoiObject )
{
	pPoiObject->SetAIPoiObject( this );

	m_bIPlanOnCapturingThis = false;

	m_InitialState.SetLocationAndRegions( pPoiObject->GetLocation() );
	m_InitialState.SetOwner( pPoiObject->GetOwner() );
	m_InitialState.SetTime( 0.0f );
	m_InitialState.SetContainer( NULL );
}
*/
/*
void CEOSAIPoiObject::SetInitialContainer( CAIPlan* pAIPlan, long iContainer )
{
	CAIObject* pAIObject = pAIPlan->GetAIObject( iContainer );
	if( pAIObject )
	{
		CEOSAIPoiObject2* pPoiObject = dynamic_cast< CEOSAIPoiObject* >( pAIObject );
		if( pPoiObject )
		{
			m_InitialState.m_pContainer = pPoiObject;
			//m_pInitialContainer = pPoiObject;

			EOSAI::PoiMobile* pThisPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( this );
			if( pThisPoiMobile )
			{
				pPoiObject->sSetInitialContainee( pThisPoiMobile );
				//pPoiObject->GetInitialUnitsInsideMe()->AddTail( pThisPoiMobile );
			}
		}
	}
}
*/

long CEOSAIPoiObject::GetGeoId()
{
	//return g_pEOSAIInterface->GetAICommonData()->GetAIGeo( m_Location )->GetId();
	return g_pEOSAIInterface->GetAICommonData()->GetAIGeo( GetLocation() )->GetId();
}

EOSAIEnumForeignRelations  CEOSAIPoiObject::GetForeignRelationsTo( long iOtherPlayer )
{
	//{ ASSERT( false ); return EOSAIEnumForeignRelations::enum_Undefined; }
	//long iOwner = this->GetOwner();
	return g_pEOSAIInterface->GetAICommonData()->GetForeignRelations( this->GetOwner(), iOtherPlayer );
}

void CEOSAIPoiObject::SetInitialContainer( CEOSAIPoiObject* pContainer )
{
	//EOSAI::PoiObjectState* pInitialState = GetAIPoiObjectStateProgression()->GetInitialState();
	//ASSERT( m_InitialState.GetContainer() == NULL );

	m_InitialState.SetContainer( pContainer );
	EOSAI::PoiMobile* pThisPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( this );
	//ASSERT( pThisPoiMobile );
	if( pThisPoiMobile )
	{
		if( m_InitialState.GetContainer() )
		{
			m_InitialState.GetContainer()->sSetInitialContainee( pThisPoiMobile );
		}
	}
}

void CEOSAIPoiObject::sSetInitialContainee( EOSAI::PoiMobile* pPoiMobile )
{
	//EOSAI::PoiObjectState* pInitialState = GetAIPoiObjectStateProgression()->GetInitialState();
	//m_InitialState.m_Containees.AddTail( pPoiMobile );
	//pInitialState->AddContainee( pPoiMobile );
	m_InitialState.AddContainee( pPoiMobile );
}

void CEOSAIPoiObject::AddMeAndChildrenToList( CList< CEOSAIPoiObject* >* pAIPoiObjects )
{
	pAIPoiObjects->AddTail( this );

	//EOSAI::PoiObjectState* pInitialState = GetAIPoiObjectStateProgression()->GetInitialState();
	POSITION pos = m_InitialState.GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_InitialState.GetContaineesList()->GetNext( pos );
		pAIPoiObject->AddMeAndChildrenToList( pAIPoiObjects );
	}
}

// This unit plus any units inside it
void CEOSAIPoiObject::AddMeAndChildrenToUnitSummary( CEOSAIUnit2Summary* pUnitSummary )
{
	pUnitSummary->AddToSummary( this );
	//
	//EOSAI::PoiObjectState* pInitialState = GetAIPoiObjectStateProgression()->GetInitialState();
	POSITION pos = m_InitialState.GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_InitialState.GetContaineesList()->GetNext( pos );
		pAIPoiObject->AddMeAndChildrenToUnitSummary( pUnitSummary );
	}
}

void CEOSAIPoiObject::AddToCombatGroup( CList< CEOSAIPoiObject* >* pCombatGroupList )
{
	// Cities are always placed in the combat group.
	//    If the enemy wants to capture it, he has to be "at target"
	pCombatGroupList->AddTail( this );

	// If there is anything in the city, it should defend and the City is not in the CombatGroup
	//POSITION pos = m_UnitsInsideMe.GetHeadPosition();
	POSITION pos = m_InitialState.GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		//CEOSAIPoiObject* pPoiObject = m_UnitsInsideMe.GetNext( pos );
		CEOSAIPoiObject* pPoiObject = m_InitialState.GetContaineesList()->GetNext( pos );
		if( pPoiObject->IsDead() == false )
		{
			pPoiObject->AddToCombatGroup( pCombatGroupList );
		}
	}
}

CEOSAIDesireSpatial* CEOSAIPoiObject::GetAIDesire( EOSAIEnumAIDesireType eType )
{
	POSITION pos = m_AIDesires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_AIDesires.GetNext( pos );
		if( pDesire->GetType() == eType ) return pDesire;
	}
	return NULL;
}

/*
void CEOSAIPoiObject::AddMeToUnitSummary( CEOSAIUnit2Summary* pUnitSummary )
{
	pUnitSummary->AddToSummary( this );
}
*/
/*
CAITacticalProject* CEOSAIPoiObject::GetAllocatedTacticalProject()
{
	if( m_pAllocatedUnitActionIdea )
	{
		return m_pAllocatedUnitActionIdea->GetTacticalProject();
	}
	return NULL;
}
*/
void  CEOSAIPoiObject::SetAllocatedUnitActionIdea( CEOSAIUnit2ActionIdea* pUnitActionIdea )
{
	ASSERT( m_pAllocatedUnitActionIdea == NULL );
	m_pAllocatedUnitActionIdea = pUnitActionIdea;
}

void  CEOSAIPoiObject::ReleaseAllocatedUnitActionIdea( CEOSAIUnit2ActionIdea* pUnitActionIdea )
{
	ASSERT( m_pAllocatedUnitActionIdea == pUnitActionIdea );
	if( m_pAllocatedUnitActionIdea == pUnitActionIdea )
	{
		m_pAllocatedUnitActionIdea = NULL;//pUnitActionIdea;
	}
}

void  CEOSAIPoiObject::ReleaseAllocatedUnitActionIdea()
{
	m_pAllocatedUnitActionIdea = NULL;
}

CEOSAIUnit2ActionIdea* CEOSAIPoiObject::GetAllocatedUnitActionIdea(){ return m_pAllocatedUnitActionIdea; }

bool CEOSAIPoiObject::IsAllocated(){ return m_pAllocatedUnitActionIdea != NULL; }


/*
void  CEOSAIPoiObject::SetAllocation( CEOSAIUnit2ActionIdea* pUnitActionIdea )
{
	ASSERT( m_pAllocatedUnitActionIdea == NULL );
	m_pAllocatedUnitActionIdea = pUnitActionIdea;
}
*/
/*
void  CEOSAIPoiObject::AddAllocatedTransportActionIdea( CEOSAIUnit2ActionIdea* pAITransportActionIdea )
{
	m_AllocatedTransportActionIdeas.AddTail( pAITransportActionIdea );
}
*/
/*
long  CEOSAIPoiObject::GetContainerSize()
{
	CPoiObject* pPoiObject = GetPoiObject();
	if( pPoiObject )
	{
		CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
		if( pUnit )
		{
			return pUnit->GetContainerSi
		}
	}
}

long  CEOSAIPoiObject::GetAvailableContainerSize()
{
}
*/
float CEOSAIPoiObject::GetBestTransportArrivalTime_CrowsFlight( long iPlayer, CEOSAIUnitTemplate* pAIGroundUnit )
{
	CEOSAITransportsSet  TransportsSet( g_pEOSAICommonData->GetAIThoughtDatabase( iPlayer ), pAIGroundUnit );
	return TransportsSet.GetEarliestArrivalTimeToLocation_UseCrowsFlight( m_InitialState.GetLocation() );
}

// UnitActionValues that I'm thinking of doing
/*
void CEOSAIPoiObject::AddUnitActionIdea( CEOSAIUnit2ActionIdea* pUnitActionValue )
{
	m_AllMyUnitActionIdeas.InsertSorted( pUnitActionValue );
}

void CEOSAIPoiObject::RemoveUnitActionIdea( CEOSAIUnit2ActionIdea* pUnitActionValue )
{
	POSITION pos = m_AllMyUnitActionIdeas.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnit2ActionIdea* pUnitActionValueInList = m_AllMyUnitActionIdeas.GetNext( pos );
		if( pUnitActionValueInList == pUnitActionValue )
		{
			m_AllMyUnitActionIdeas.RemoveAt( prevPos );
		}
	}
}
*/
/*
void CEOSAIPoiObject::AllocateToTacticalProject( CEOSAIUnit2ActionIdea* pIdea )
{
	//m_pAllocatedToTacticalProject = pProject;
	m_pAllocatedUnitActionIdea = pIdea;
	//m_pActor->AllocateToTacticalProject( pProject, pIdea );
}
*/
/*
void CEOSAIPoiObject::UnitActionIdeaWasAllocated( CEOSAIUnit2ActionIdea* pUnitActionValue, bool bAllocate )
{
	if( bAllocate )
	{
		ASSERT( m_pAllocatedUnitActionIdea == NULL );
		m_pAllocatedUnitActionIdea = pUnitActionValue;
	}
	else
	{
		ASSERT( m_pAllocatedUnitActionIdea == pUnitActionValue );
		m_pAllocatedUnitActionIdea = NULL;
	}
}
*/
/*
void CEOSAIPoiObject::UnitActionIdeaWasAllocated( CEOSAIUnit2ActionIdea* pUnitActionValue, bool bAllocate )
{
	if( bAllocate )
	{
		ASSERT( m_pAllocatedUnitActionIdea == NULL );
		m_pAllocatedUnitActionIdea = pUnitActionValue;
	}
	else
	{
		ASSERT( m_pAllocatedUnitActionIdea == pUnitActionValue );
		m_pAllocatedUnitActionIdea = NULL;
	}
}
*/
/*
float CEOSAIPoiObject::GetBestUnitActionValueSimpleScore()
{
	if( m_AllMyUnitActionIdeas.IsEmpty() ) return 0.0f;
	return m_AllMyUnitActionIdeas.GetHead()->m_fMoveToTargetTime;//>m_fSimpleTimeToTargetScore;
}
*/
/*
float CEOSAIPoiObject::GetBestUnscheduledUnitActionValueSimpleScore()
{
	if( m_AllMyUnitActionIdeas.IsEmpty() ) return 0.0f;
	POSITION pos = m_AllMyUnitActionIdeas.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionIdea* pAIUnitActionValue = m_AllMyUnitActionIdeas.GetNext( pos );
		if( pAIUnitActionValue->GetAIUnitAction() == NULL )
		{
			return pAIUnitActionValue->GetBestCaseTotalTimeToTarget();//m_fSimpleTimeToTargetScore;
		}
	}
	return 0.1f; // to avoid divide by zero errors
}
*/
/*
void  CEOSAIPoiObject::CreateRelativeActionScoresAccordingToAIPoiObject()
{
	float fBestSimpleScore = GetBestUnitActionValueSimpleScore();

	// Iterate over the UnitActionScores, get the value, lookup the AIPoiObject's relative score
	POSITION pos = m_AllMyUnitActionIdeas.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionIdea* pUnitActionValue = m_AllMyUnitActionIdeas.GetNext( pos );
		float fThisActionValuesScore = pUnitActionValue->m_fMoveToTargetTime;//m_fSimpleTimeToTargetScore;

		pUnitActionValue->m_fRelativeScoreAccordingToAIPoiObject = fThisActionValuesScore / fBestSimpleScore;
		int z=0; // debug
	}
}
*/
/*
void CEOSAIPoiObject::ReScoreAIUnitActionValues()
{
	/-*
	since i am recalculating the paths and scores for all the aiobject's UnitActionValues,
	what i should do is recalculate their simple score first, then recalculate the relative
	scores, then resort the RelativeScore lists

	What I need to do is 
	*-/

	//float fBestAIObjectScore = this->GetBestUnscheduledUnitActionValueSimpleScore();

	POSITION pos = m_AllMyUnitActionIdeas.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionIdea* pAIUnitActionValue = m_AllMyUnitActionIdeas.GetNext( pos );

		//float fBestActionDefinitionScore = pAIUnitActionValue->GetTaskForce()->GetTacticalProject()->GetBestUnscheduledUnitActionValueSimpleScore();

		// Grab the pAIUnitActionValue's AIActionDefinition best score
		// Grab the AIObjects's best score
		// After I rescore the UnitActionValue, I'll need this information to know whether
		//   to recalculate the relative scores

		pAIUnitActionValue->ReCalculateAndReScore();
	}
	//m_AllMyUnitActionIdeas.InsertSorted( pUnitActionValue );
}
*/


// Predefined Path, Result Path, Orders
//
/*
void  CEOSAIPoiObject::TurnAllocatedUnitActionIdeaIntoPredefinedStep()
{
	if( m_pAllocatedUnitActionIdea )
	{
		m_pAllocatedUnitActionIdea->CreatePredefinedStep();
	}
}
*/

float CEOSAIPoiObject::GetHPRepairPerTurn( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	//if( m_pServerPoiObject == NULL ) return 0.0f;

	if( pAIUnitTemplate->IsGroundUnit() )
	{
		return pAIUnitTemplate->GetGroundUnitRepairRate();
	}
	if( pAIUnitTemplate->IsAirUnit() )
	{
		return pAIUnitTemplate->GetAirUnitRepairRate();
	}
	if( pAIUnitTemplate->IsSeaUnit() )
	{
		return pAIUnitTemplate->GetSeaUnitRepairRate();
	}
	ASSERT( false );
	return 0.0f;
}

void  CEOSAIPoiObject::AddOnTheWayAndWaitTasksAsPredefinedSteps(){} // over-ridden by AIUnit

void  CEOSAIPoiObject::TurnPredefinedPathIntoResultPath()
{
	CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( this );
	if( pAIUnit )
	{
		pAIUnit->GetAIUnitPathwayFinder()->CalculateResultPath();
	}
}

void  CEOSAIPoiObject::TurnResultPathIntoOrders()
{
	CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( this );
	if( pAIUnit )
	{
		pAIUnit->GetAIUnitPathwayFinder()->CreateOrders();
	}
	/*
	if( m_pAllocatedUnitActionIdea &&
		m_pAllocatedUnitActionIdea->GetPredefinedStep() )
	{
		m_pAllocatedUnitActionIdea->GetPredefinedStep()->>CreateResultStep();
	}
	*/
}

// Actions, Tasks, Orders
//
#ifdef DO_NOT_COMPILE
void  CEOSAIPoiObject::TurnActionsIntoTasks()
{
	ASSERT( false );

	// INCOMPLETE: this needs to be a lot more intelligent - picking and choosing how to setup
	//   the sequence of tasks.
	// Right now, the only case where an AIPoiObject has several Actions is with Transports.

	POSITION pos = m_AIObjectActionProgression.GetAIActions()->GetHeadPosition();
	while( pos )
	{
		CEOSAIAction* pAIAction = m_AIObjectActionProgression.GetAIActions()->GetNext( pos );
		CEOSAIUnit2Action* pAIUnitAction = dynamic_cast< CEOSAIUnit2Action* >( pAIAction );
		if( pAIUnitAction == NULL ) continue;

		if( pAIAction->HasBeenTurnedIntoTasks() ) continue;

		//pAIAction->TurnActionsIntoTasks();
		CEOSAIUnit2ActionIdea* pUnitActionIdea = pAIAction->GetAIUnitActionIdea();

		ASSERT( pUnitActionIdea );

		CEOSAIUnit2* pThisAIUnitActor = dynamic_cast< CEOSAIUnit2* >( this );//m_pUnitActionIdea->GetAIUnitActor();
		ASSERT( pThisAIUnitActor );

		// If I am the transport, then setup the transport pickup/dropoff tasks
		//
		if( pUnitActionIdea->GetActionType() == CEOSAIUnit2ActionIdea::EnumActionType_Transport )
		{
			CEOSAIUnit2* pAITransport = pThisAIUnitActor;
			CEOSAIUnit2ActionIdea* pAITransportActionIdea = pUnitActionIdea;
			CEOSAIUnit2ActionIdea* pAITransporteeActionIdea = pAITransportActionIdea->m_pAITransporteeUnitActionIdea;
			pAITransportActionIdea->m_pAITransportAssistedPath = new CEOSAITransportAssistedPath();
			CEOSAIUnit2* pAITransportee = pUnitActionIdea->m_pAITransportee;

			//CEOSAITransportAssistedPathResults  PathResults;
			//CEOSAIUnit2ActionIdea* pAITransportActionIdea = m_pUnitActionIdea->GetAITransportActionIdea();
			bool bTransporteeIsAlreadyInsideTheTransport = false;
			if( pAITransportee &&
				pThisAIUnitActor &&
				pAITransportee->GetContainerAIPoiObject() == pThisAIUnitActor )
			{
				bTransporteeIsAlreadyInsideTheTransport = true;
			}

			CEOSAIUnit2PathwayFinder* pPathwayFinder = pAITransportee->GetAIUnitPathwayFinder();
			//pPathwayFinder->AddTransportAction( pAITransportee, pAITransportee->GetInitialState()->GetLocation(), pAITransporteeActionIdea->GetTargetLocation() );





			//pAITransporteeActionIdea->GetAIUnitAction()->GetTargetDesire()->GetLocation()
			pAITransportee->GetTransportAssistedAIRegionMovementPath( pAITransportActionIdea, pAITransporteeActionIdea->GetTargetLocation(), pAITransportActionIdea->m_pAITransportAssistedPath );
			//pAITransportee->GetTransportAssistedMovementPath( pAITransportActionIdea, pAITransporteeActionIdea->GetAIUnitAction()->GetTargetDesire()->GetLocation(), pAITransportActionIdea->m_pAITransportAssistedPath );
			//pAITransportee->GetTransportAssistedMovementPath( pAITransportActionIdea, pAIUnitAction->GetTargetDesire()->GetLocation(), pAITransportActionIdea->m_pAITransportAssistedPath );
			//pAIUnitActor->GetTransportAssistedMovementPath( pAITransportActionIdea, m_pTargetDesire->GetLocation(), &PathResults );

			CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
			CEOSAIRegion2* pLandPickupAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iLandPickupAIRegion );
			CEOSAIRegion2* pWaterPickupAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iWaterPickupAIRegion );
			CEOSAIRegion2* pLandDropoffAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iLandDropoffAIRegion );
			CEOSAIRegion2* pWaterDropoffAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iWaterDropoffAIRegion );

			if( bTransporteeIsAlreadyInsideTheTransport )
			{
				ASSERT( pWaterDropoffAIRegion );
				ASSERT( pWaterDropoffAIRegion->IsWater() || pWaterDropoffAIRegion->IsCity() );
				ASSERT( pLandDropoffAIRegion );
				ASSERT( pLandDropoffAIRegion->IsLand() || pLandDropoffAIRegion->IsCity() );

				// Setup the pickup/dropoff tasks
				CEOSAIUnitTemplate* pTransportUnitTemplate = pAITransportActionIdea->GetAIUnitTemplate();

				if( pAITransport && pAITransport->IsSeaUnit() )
				{
					// Dropoff
					CEOSAILocation WaterDropoffLocation;
					bool bLocationFound = pWaterDropoffAIRegion->GetAdjacentRegionJumpOffLocation( pLandDropoffAIRegion->GetId(), &WaterDropoffLocation );
					ASSERT( bLocationFound );
					EOSAI::UnitTask_Dropoff* pAIUnitTask_Dropoff = 
						new EOSAI::UnitTask_Dropoff( pAITransport, pAITransportee, pAIUnitAction, pAITransport->GetFinalState(), WaterDropoffLocation );
					pAIUnitTask_Dropoff->AddToPermittedPath( &pAITransportActionIdea->m_pAITransportAssistedPath->m_TransportPickupToDropoffAIRegionPath );
					pAITransport->GetAIObjectActionProgression()->InsertAITask( pAITransport->GetFinalState(), pAIUnitTask_Dropoff );

//					pAITransportActionIdea->m_pAIUnitTask_Dropoff = pAIUnitTask_Dropoff;

					// Exit
					/*
					CEOSAILocation LandDropoffLocation;
					bLocationFound = pLandDropoffRegion->GetAdjacentRegionJumpOffLocation( pWaterDropoffRegion->GetId(), &LandDropoffLocation );
					ASSERT( bLocationFound );
					EOSAI::UnitTask_Exit* pAIUnitTask_Exit = 
						new EOSAI::UnitTask_Exit( pAIUnitActor, pAITransport, this, pAIUnitActor->GetFinalState(), LandDropoffLocation );
					pAIUnitTask_Exit->AddToPermittedPath( &PathResults.m_GroundUnitToPickupPath );
					pAIUnitTask_Exit->SetAssociatedDropoffTask( pAIUnitTask_Dropoff );
					pAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pAIUnitActor->GetFinalState(), pAIUnitTask_Exit );
					*/
				}
			}
			else
			{
				ASSERT( pWaterPickupAIRegion );
				ASSERT( pWaterPickupAIRegion->IsWater() || pWaterPickupAIRegion->IsCity() );
				ASSERT( pLandPickupAIRegion );
				ASSERT( pLandPickupAIRegion->IsLand() || pLandPickupAIRegion->IsCity() );
				ASSERT( pWaterDropoffAIRegion );
				ASSERT( pWaterDropoffAIRegion->IsWater() || pWaterDropoffAIRegion->IsCity() );
				ASSERT( pLandDropoffAIRegion );
				ASSERT( pLandDropoffAIRegion->IsLand() || pLandDropoffAIRegion->IsCity() );

				// Setup the pickup/dropoff tasks
				CEOSAIUnitTemplate* pTransportUnitTemplate = pAITransportActionIdea->GetAIUnitTemplate();

				if( pAITransport && pAITransport->IsSeaUnit() )
				{
					// Pickup
					CEOSAILocation WaterPickupLocation;
					bool bLocationFound = pWaterPickupAIRegion->GetAdjacentRegionJumpOffLocation( pLandPickupAIRegion->GetId(), &WaterPickupLocation );
					ASSERT( bLocationFound );
					EOSAI::UnitTask_Pickup* pAIUnitTask_Pickup = 
						new EOSAI::UnitTask_Pickup( pAITransport, pAITransportee, pAIUnitAction, pAITransport->GetFinalState(), WaterPickupLocation );
					pAIUnitTask_Pickup->AddToPermittedPath( &pAITransportActionIdea->m_pAITransportAssistedPath->m_TransportToPickupAIRegionPath );
					pAITransport->GetAIObjectActionProgression()->InsertAITask( pAITransport->GetFinalState(), pAIUnitTask_Pickup );
					//
//					pAITransportActionIdea->m_pAIUnitTask_Pickup = pAIUnitTask_Pickup;

					// Dropoff
					CEOSAILocation WaterDropoffLocation;
					bLocationFound = pWaterDropoffAIRegion->GetAdjacentRegionJumpOffLocation( pLandDropoffAIRegion->GetId(), &WaterDropoffLocation );
					ASSERT( bLocationFound );
					EOSAI::UnitTask_Dropoff* pAIUnitTask_Dropoff = 
						new EOSAI::UnitTask_Dropoff( pAITransport, pAITransportee, pAIUnitAction, pAITransport->GetFinalState(), WaterDropoffLocation );
					pAIUnitTask_Dropoff->AddToPermittedPath( &pAITransportActionIdea->m_pAITransportAssistedPath->m_TransportPickupToDropoffAIRegionPath );
					pAITransport->GetAIObjectActionProgression()->InsertAITask( pAITransport->GetFinalState(), pAIUnitTask_Dropoff );
					//
//					pAITransportActionIdea->m_pAIUnitTask_Dropoff = pAIUnitTask_Dropoff;

					// Enter
					/*
					CEOSAILocation LandPickupLocation;
					bLocationFound = pLandPickupRegion->GetAdjacentRegionJumpOffLocation( pWaterPickupRegion->GetId(), &LandPickupLocation );
					ASSERT( bLocationFound );
					EOSAI::UnitTask_Enter* pAIUnitTask_Enter = 
						new EOSAI::UnitTask_Enter( pAIUnitActor, pAITransport, this, pAIUnitActor->GetFinalState(), LandPickupLocation );
					pAIUnitTask_Enter->AddToPermittedPath( &PathResults.m_GroundUnitToPickupPath );
					pAIUnitTask_Enter->SetAssociatedPickupTask( pAIUnitTask_Pickup );
					pAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pAIUnitActor->GetFinalState(), pAIUnitTask_Enter );

					// Exit
					CEOSAILocation LandDropoffLocation;
					bLocationFound = pLandDropoffRegion->GetAdjacentRegionJumpOffLocation( pWaterDropoffRegion->GetId(), &LandDropoffLocation );
					ASSERT( bLocationFound );
					EOSAI::UnitTask_Exit* pAIUnitTask_Exit = 
						new EOSAI::UnitTask_Exit( pAIUnitActor, pAITransport, this, pAIUnitActor->GetFinalState(), LandDropoffLocation );
					pAIUnitTask_Exit->AddToPermittedPath( &PathResults.m_GroundUnitToPickupPath );
					pAIUnitTask_Exit->SetAssociatedDropoffTask( pAIUnitTask_Dropoff );
					pAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pAIUnitActor->GetFinalState(), pAIUnitTask_Exit );
					*/
				}
				else // If the AITransport doesn't actually exist yet, then give the GroundUnit Movement orders (get there before the transport)
				{
					// MoveTo
					/*
					CEOSAILocation LandPickupLocation;
					bool bLocationFound = pLandPickupRegion->GetAdjacentRegionJumpOffLocation( pWaterPickupRegion->GetId(), &LandPickupLocation );
					ASSERT( bLocationFound );
					EOSAI::UnitTask_MoveTo* pAIUnitTask_MoveTo = 
						new EOSAI::UnitTask_MoveTo( pAIUnitActor, this, pAIUnitActor->GetFinalState(), LandPickupLocation );
					pAIUnitTask_MoveTo->AddToPermittedPath( &PathResults.m_GroundUnitToPickupPath );
					pAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pAIUnitActor->GetFinalState(), pAIUnitTask_MoveTo );
					*/

					// Should probably pause so the unit doesn't do other tasks after this
					//   It needs to stick around at this location
				}
			}
		}
		// If this action requires a transport, make sure the transport pickup/dropoff tasks are created
		if( pUnitActionIdea->GetAITransportUnitActionIdea() )
		{
			// I need to setup transports to pickup/dropoff several units
			//   This needs to be done in the transport code (not here) because the transport may need to
			//   coordinate pickup/dropoff for several units
			CEOSAIUnit2ActionIdea* pAITransportActionIdea = pUnitActionIdea->GetAITransportUnitActionIdea();
			CEOSAIUnit2* pAITransport = pAITransportActionIdea->GetAIUnitActor();
			pAITransport->TurnActionsIntoTasks();

			ASSERT( pAITransportActionIdea->m_pAITransportAssistedPath );

			CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
			CEOSAIRegion2* pLandPickupAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iLandPickupAIRegion );
			CEOSAIRegion2* pWaterPickupAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iWaterPickupAIRegion );
			CEOSAIRegion2* pLandDropoffAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iLandDropoffAIRegion );
			CEOSAIRegion2* pWaterDropoffAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iWaterDropoffAIRegion );

			// If the AITransport doesn't actually exist yet, then give the GroundUnit Movement orders 
			//    (get there before the transport)
			if( pAITransportActionIdea->GetAIUnitActor() == NULL ) 
			{
				// MoveTo
				CEOSAILocation LandPickupLocation;
				bool bLocationFound = pLandPickupAIRegion->GetAdjacentRegionJumpOffLocation( pWaterPickupAIRegion->GetId(), &LandPickupLocation );
				ASSERT( bLocationFound );
				EOSAI::UnitTask_MoveTo* pAIUnitTask_MoveTo = 
					new EOSAI::UnitTask_MoveTo( pThisAIUnitActor, pAIUnitAction, pThisAIUnitActor->GetFinalState(), LandPickupLocation );
				pAIUnitTask_MoveTo->AddToPermittedPath( &pAITransportActionIdea->m_pAITransportAssistedPath->m_GroundUnitToPickupAIRegionPath );
				pThisAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pThisAIUnitActor->GetFinalState(), pAIUnitTask_MoveTo );

				// Should probably pause so the unit doesn't do other tasks after this
				//   It needs to stick around at this location
			}

			if( pAITransportActionIdea->GetAIPickupTask() )
			{
				// Need to perform a pickup/enter
				CEOSAIRegion2* pLandPickupAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iLandPickupAIRegion );
				CEOSAIRegion2* pWaterPickupAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iWaterPickupAIRegion );

				// Enter
				CEOSAILocation LandPickupLocation;
				bool bLocationFound = pLandPickupAIRegion->GetAdjacentRegionJumpOffLocation( pWaterPickupAIRegion->GetId(), &LandPickupLocation );
				ASSERT( bLocationFound );
				EOSAI::UnitTask_Enter* pAIUnitTask_Enter = 
					new EOSAI::UnitTask_Enter( pThisAIUnitActor, pAITransport, pAIUnitAction, pThisAIUnitActor->GetFinalState(), LandPickupLocation );
				pAIUnitTask_Enter->AddToPermittedPath( &pAITransportActionIdea->m_pAITransportAssistedPath->m_GroundUnitToPickupAIRegionPath );
				pAIUnitTask_Enter->SetAssociatedPickupTask( pAITransportActionIdea->GetAIPickupTask() );//m_pAIUnitTask_Pickup );
				pThisAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pThisAIUnitActor->GetFinalState(), pAIUnitTask_Enter );
			}
			if( pAITransportActionIdea->GetAIDropoffTask() )
			{
				// Need to perform a dropoff/exit
				CEOSAIRegion2* pLandDropoffAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iLandDropoffAIRegion );
				CEOSAIRegion2* pWaterDropoffAIRegion = pAIRegionManager->GetAIRegion( pAITransportActionIdea->m_pAITransportAssistedPath->m_iWaterDropoffAIRegion );

				// Exit
				CEOSAILocation LandDropoffLocation;
				bool bLocationFound = pLandDropoffAIRegion->GetAdjacentRegionJumpOffLocation( pWaterDropoffAIRegion->GetId(), &LandDropoffLocation );
				ASSERT( bLocationFound );
				EOSAI::UnitTask_Exit* pAIUnitTask_Exit = 
					new EOSAI::UnitTask_Exit( pThisAIUnitActor, pAITransport, pAIUnitAction, pThisAIUnitActor->GetFinalState(), LandDropoffLocation );
				pAIUnitTask_Exit->AddToPermittedPath( &pAITransportActionIdea->m_pAITransportAssistedPath->m_GroundUnitToPickupAIRegionPath );
				pAIUnitTask_Exit->SetAssociatedDropoffTask( pAITransportActionIdea->GetAIDropoffTask() );//m_pAIUnitTask_Dropoff );
				pThisAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pThisAIUnitActor->GetFinalState(), pAIUnitTask_Exit );
			}
		}

		// Setup the transport pickup/dropoff tasks
		/*
		if( m_pUnitActionIdea->GetAITransportActionIdea() )
		{
			CEOSAITransportAssistedPathResults  PathResults;
			CEOSAIUnit2ActionIdea* pAITransportActionIdea = m_pUnitActionIdea->GetAITransportActionIdea();

			bool bThisUnitIsAlreadyInsideTheTransport = false;
			if( m_pUnitActionIdea->GetAIUnitActor() &&
				m_pUnitActionIdea->GetAIUnitActor()->GetUnit() &&
				pAITransportActionIdea->GetAIUnitActor() &&
				pAITransportActionIdea->GetAIUnitActor()->GetUnit() &&
				m_pUnitActionIdea->GetAIUnitActor()->GetUnit()->GetContainer() == pAITransportActionIdea->GetAIUnitActor()->GetUnit() )
			{
				bThisUnitIsAlreadyInsideTheTransport = true;
			}

			pAIUnitActor->GetTransportAssistedMovementPath( pAITransportActionIdea, m_pTargetDesire->GetLocation(), &PathResults );

			CEOSAIRegionManager2* pAIRegionManager = GetCommonState()->GetWorldDescServer()->GetAIRegionManager();
			CEOSAIRegion2* pLandPickupRegion = pAIRegionManager->GetAIRegion( PathResults.m_iLandPickupRegion );
			CEOSAIRegion2* pWaterPickupRegion = pAIRegionManager->GetAIRegion( PathResults.m_iWaterPickupRegion );
			CEOSAIRegion2* pLandDropoffRegion = pAIRegionManager->GetAIRegion( PathResults.m_iLandDropoffRegion );
			CEOSAIRegion2* pWaterDropoffRegion = pAIRegionManager->GetAIRegion( PathResults.m_iWaterDropoffRegion );

			if( bThisUnitIsAlreadyInsideTheTransport )
			{
				ASSERT( pWaterDropoffRegion );
				ASSERT( pWaterDropoffRegion->IsWater() || pWaterDropoffRegion->IsCity() );
				ASSERT( pLandDropoffRegion );
				ASSERT( pLandDropoffRegion->IsLand() || pLandDropoffRegion->IsCity() );

				// Setup the pickup/dropoff tasks
				CUnitTemplate* pTransportUnitTemplate = pAITransportActionIdea->GetUnitTemplate();

				if( pTransportUnitTemplate->IsSeaUnit() )
				{
					CEOSAIUnit2* pAITransport = pAITransportActionIdea->GetAIUnitActor();
					if( pAITransport )
					{
						// Dropoff
						CEOSAILocation WaterDropoffLocation;
						bool bLocationFound = pWaterDropoffRegion->GetAdjacentRegionJumpOffLocation( pLandDropoffRegion->GetId(), &WaterDropoffLocation );
						ASSERT( bLocationFound );
						EOSAI::UnitTask_Dropoff* pAIUnitTask_Dropoff = 
							new EOSAI::UnitTask_Dropoff( pAITransport, pAIUnitActor, this, pAITransport->GetFinalState(), WaterDropoffLocation );
						pAIUnitTask_Dropoff->AddToPermittedPath( &PathResults.m_TransportPickupToDropoffPath );
						pAITransport->GetAIObjectActionProgression()->InsertAITask( pAITransport->GetFinalState(), pAIUnitTask_Dropoff );

						// Exit
						CEOSAILocation LandDropoffLocation;
						bLocationFound = pLandDropoffRegion->GetAdjacentRegionJumpOffLocation( pWaterDropoffRegion->GetId(), &LandDropoffLocation );
						ASSERT( bLocationFound );
						EOSAI::UnitTask_Exit* pAIUnitTask_Exit = 
							new EOSAI::UnitTask_Exit( pAIUnitActor, pAITransport, this, pAIUnitActor->GetFinalState(), LandDropoffLocation );
						pAIUnitTask_Exit->AddToPermittedPath( &PathResults.m_GroundUnitToPickupPath );
						pAIUnitTask_Exit->SetAssociatedDropoffTask( pAIUnitTask_Dropoff );
						pAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pAIUnitActor->GetFinalState(), pAIUnitTask_Exit );
					}
				}
			}
			else
			{
				ASSERT( pWaterPickupRegion );
				ASSERT( pWaterPickupRegion->IsWater() || pWaterPickupRegion->IsCity() );
				ASSERT( pLandPickupRegion );
				ASSERT( pLandPickupRegion->IsLand() || pLandPickupRegion->IsCity() );
				ASSERT( pWaterDropoffRegion );
				ASSERT( pWaterDropoffRegion->IsWater() || pWaterDropoffRegion->IsCity() );
				ASSERT( pLandDropoffRegion );
				ASSERT( pLandDropoffRegion->IsLand() || pLandDropoffRegion->IsCity() );

				// Setup the pickup/dropoff tasks
				CUnitTemplate* pTransportUnitTemplate = pAITransportActionIdea->GetUnitTemplate();

				if( pTransportUnitTemplate->IsSeaUnit() )
				{
					CEOSAIUnit2* pAITransport = pAITransportActionIdea->GetAIUnitActor();
					if( pAITransport )
					{
						// Pickup
						CEOSAILocation WaterPickupLocation;
						bool bLocationFound = pWaterPickupRegion->GetAdjacentRegionJumpOffLocation( pLandPickupRegion->GetId(), &WaterPickupLocation );
						ASSERT( bLocationFound );
						EOSAI::UnitTask_Pickup* pAIUnitTask_Pickup = 
							new EOSAI::UnitTask_Pickup( pAITransport, pAIUnitActor, this, pAITransport->GetFinalState(), WaterPickupLocation );
						pAIUnitTask_Pickup->AddToPermittedPath( &PathResults.m_TransportToPickupPath );
						pAITransport->GetAIObjectActionProgression()->InsertAITask( pAITransport->GetFinalState(), pAIUnitTask_Pickup );

						// Dropoff
						CEOSAILocation WaterDropoffLocation;
						bLocationFound = pWaterDropoffRegion->GetAdjacentRegionJumpOffLocation( pLandDropoffRegion->GetId(), &WaterDropoffLocation );
						ASSERT( bLocationFound );
						EOSAI::UnitTask_Dropoff* pAIUnitTask_Dropoff = 
							new EOSAI::UnitTask_Dropoff( pAITransport, pAIUnitActor, this, pAITransport->GetFinalState(), WaterDropoffLocation );
						pAIUnitTask_Dropoff->AddToPermittedPath( &PathResults.m_TransportPickupToDropoffPath );
						pAITransport->GetAIObjectActionProgression()->InsertAITask( pAITransport->GetFinalState(), pAIUnitTask_Dropoff );

						// Enter
						CEOSAILocation LandPickupLocation;
						bLocationFound = pLandPickupRegion->GetAdjacentRegionJumpOffLocation( pWaterPickupRegion->GetId(), &LandPickupLocation );
						ASSERT( bLocationFound );
						EOSAI::UnitTask_Enter* pAIUnitTask_Enter = 
							new EOSAI::UnitTask_Enter( pAIUnitActor, pAITransport, this, pAIUnitActor->GetFinalState(), LandPickupLocation );
						pAIUnitTask_Enter->AddToPermittedPath( &PathResults.m_GroundUnitToPickupPath );
						pAIUnitTask_Enter->SetAssociatedPickupTask( pAIUnitTask_Pickup );
						pAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pAIUnitActor->GetFinalState(), pAIUnitTask_Enter );

						// Exit
						CEOSAILocation LandDropoffLocation;
						bLocationFound = pLandDropoffRegion->GetAdjacentRegionJumpOffLocation( pWaterDropoffRegion->GetId(), &LandDropoffLocation );
						ASSERT( bLocationFound );
						EOSAI::UnitTask_Exit* pAIUnitTask_Exit = 
							new EOSAI::UnitTask_Exit( pAIUnitActor, pAITransport, this, pAIUnitActor->GetFinalState(), LandDropoffLocation );
						pAIUnitTask_Exit->AddToPermittedPath( &PathResults.m_GroundUnitToPickupPath );
						pAIUnitTask_Exit->SetAssociatedDropoffTask( pAIUnitTask_Dropoff );
						pAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pAIUnitActor->GetFinalState(), pAIUnitTask_Exit );
					}
					else // If the AITransport doesn't actually exist yet, then give the GroundUnit Movement orders (get there before the transport)
					{
						// MoveTo
						CEOSAILocation LandPickupLocation;
						bool bLocationFound = pLandPickupRegion->GetAdjacentRegionJumpOffLocation( pWaterPickupRegion->GetId(), &LandPickupLocation );
						ASSERT( bLocationFound );
						EOSAI::UnitTask_MoveTo* pAIUnitTask_MoveTo = 
							new EOSAI::UnitTask_MoveTo( pAIUnitActor, this, pAIUnitActor->GetFinalState(), LandPickupLocation );
						pAIUnitTask_MoveTo->AddToPermittedPath( &PathResults.m_GroundUnitToPickupPath );
						pAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pAIUnitActor->GetFinalState(), pAIUnitTask_MoveTo );

						// Should probably pause so the unit doesn't do other tasks after this
						//   It needs to stick around at this location
					}
				}
				else
				{
					ASSERT( false );
				}
			}
		}
		*/


		// Create the actual task now
		// Create AITask
		CEOSAIDesireSpatial* pTargetDesire = pAIUnitAction->GetTargetDesire();
		//ASSERT( pTargetDesire );
		//if( pTargetDesire == NULL ) continue; // Right now, this can happen with transports

		CEOSAIDesireCaptureCitRes* pCaptureDesire = dynamic_cast< CEOSAIDesireCaptureCitRes* >( pTargetDesire );
		if( pCaptureDesire )
		{
			CEOSAIPoiObject* pTargetPoiObject = pTargetDesire->GetAIPoiObjectTarget();

			EOSAI::UnitTask_CaptureCitRes* pAIUnitTask_CaptureCitRes = 
				new EOSAI::UnitTask_CaptureCitRes( pThisAIUnitActor, pAIUnitAction, pThisAIUnitActor->GetFinalState(), pTargetPoiObject );
			pThisAIUnitActor->GetAIObjectActionProgression()->InsertAITask( pThisAIUnitActor->GetFinalState(), pAIUnitTask_CaptureCitRes );
		}

		pAIAction->HasBeenTurnedIntoTasks( true );
	}

	// Turn AIUnitActions into AITasks
	/*
	if( GetAllocatedUnitActionIdea() )
	{
		GetAllocatedUnitActionIdea()->GetAIUnitAction()->TurnActionsIntoTasks();
	}
	*/
/*
	// Turn AIUnitActions into AITasks
	CList< CEOSAIUnit2Action* >  UnitActions;
	POSITION pos = m_AllMyUnitActionIdeas.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionIdea* pIdea = m_AllMyUnitActionIdeas.GetNext( pos );
		if( pIdea->GetAIUnitAction() )
		{
			UnitActions.AddTail( pIdea->GetAIUnitAction() );
		}
	}
	pos = UnitActions.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2Action* pAIAction = UnitActions.GetNext( pos );
		pAIAction->TurnActionsIntoTasks();
	}
*/
	/*
	pos = m_UnitActionStack.m_Items.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_Items.GetNext( pos );
		if( pStackItem->ActorIsAllocated() )
		{
			CEOSAIUnit2ActionIdea* pUnitActionIdea = pStackItem->m_pUnitActionIdea;
			//ASSERT( pUnitActionIdea->IsScheduled() == false );

			if( pUnitActionIdea->GetAIUnitActor() )
			{
				//CEOSAIUnit2* pAIUnit = pUnitActionIdea->GetAIUnitActor();
				pUnitActionIdea->GetAIUnitAction()->TurnActionsIntoTasks();
				//pAIUnitAction->ActionScheduler_TurnActionsIntoTasks();
			}
		}
	}
	*/
}
#endif DO_NOT_COMPILE


/*
bool  CEOSAIPoiObject::TurnTasksIntoOrders()
{
	// Turn AIUnitActions into AITasks
	return m_AIObjectActionProgression.TurnTasksIntoOrders();
	/-*
	POSITION pos = m_AllMyUnitActionIdeas.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionIdea* pIdea = m_AllMyUnitActionIdeas.GetNext( pos );
		if( pIdea->GetAIUnitAction() )
		{
			pIdea->GetAIUnitAction()->TurnActionsIntoTasks();
		}
	}
	*-/
}
*/

/*
void CEOSAIPoiObject::AppendAIActionAndAITasks( CEOSAIAction* pAIAction )
{
	// Append the AIAction to the sequence, create Tasks, update PoiObjectStates
	m_AIActions.AddTail( pAIAction );

	pAIAction->CreateAndAppendAITasks();
}

void CEOSAIPoiObject::AppendAITask( CEOSAITask* pAITask )
{
	m_AITasks.AddTail( pAITask );
}

void CEOSAIPoiObject::InsertAITask( EOSAI::PoiObjectState* pPreviousState, CEOSAITask* pAITask )
{
	if( pPreviousState == &m_InitialState )
	{
		m_AITasks.AddHead( pAITask );
		return;
	}
	POSITION pos = m_AITasks.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAITask* pTaskInList = m_AITasks.GetNext( pos );
		if( pTaskInList->GetFinalState() == pPreviousState )
		{
			m_AITasks.InsertAfter( prevPos, pAITask );
			return;
		}
	}
}

void CEOSAIPoiObject::UpdateTaskTimes()
{
	// There are several things that could happen here.
	//   If it is merely a time delay, I just need to shift the start and end times.
	//   If there was a change in location, it changes the duration and requires a recalculation.
	//CEOSAILocation location = this->GetInitialLocation();
	float fCurrentTime = this->GetInitialTime();
	//long  iCurrentGeo  = this->GetInitialGeo();

	POSITION pos = m_AITasks.GetHeadPosition();
	while( pos )
	{
		CEOSAITask* pTask = m_AITasks.GetNext( pos );

		ASSERT( pTask->GetTaskDuration() != 0.0f );

		pTask->SetStartTime( fCurrentTime );
		pTask->SetEndTime( fCurrentTime + pTask->GetTaskDuration() );
		//pTask->SetStartGeo( iCurrentGeo );
		//pTask->SetEndGeo( iCurrentGeo );

		fCurrentTime += pTask->GetTaskDuration();
	}
}

void CEOSAIPoiObject::sRemoveTask( CEOSAITask* pAITask )
{
	POSITION pos = m_AITasks.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAITask* pAITaskFromList = m_AITasks.GetNext( pos );
		if( pAITaskFromList == pAITask )
		{
			m_AITasks.RemoveAt( prevPos );
		}
	}
}

bool CEOSAIPoiObject::CreateOrders()
{
	// I can only give orders to my own stuff
	//bool bAllOrdersWereCreated = true;
	if( this->IsMine() )
	{
		POSITION pos = m_AITasks.GetHeadPosition();
		while( pos )
		{
			CEOSAITask* pTask = m_AITasks.GetNext( pos );
			// Some orders have prerequisite orders in other units.
			//   We might need to cycle back later and construct the order.
			if( pTask->OrdersWereCreated() == false )
			{
				pTask->CreateOrders();
				if( pTask->OrdersWereCreated() == false )
				{
					//bAllOrdersWereCreated = false;
					return false;
				}
			}
		}
	}
	return true;
}
*/

/*
bool CEOSAIPoiObject::IAmContainedInside( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAIPoiObject* pCurrObject = GetFinalState()->GetContainer();
	while( pCurrObject )
	{
		if( pCurrObject == pAIPoiObject )
		{
			return true;
		}
		pCurrObject = pCurrObject->GetFinalState()->GetContainer();
	}
	return false;
}
*/
/*
CEOSAIGeo* CEOSAIPoiObject::GetAIGeo()
{
	long iGeo = GetServerPoiObject()->GetGeo();

	//CEOSAIGeo* pAIGeo = m_pAIBrain->GetAIGeo( iGeo );
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CEOSAIGeo* pAIGeo = pWorldDescServer->GetAICommonData()->GetAIGeo( iGeo );
	return pAIGeo;
}
*/
bool CEOSAIPoiObject::IsUnowned()
{
	return ( GetOwner() == 0 );
}
/*
EOSAIEnumForeignRelations  CEOSAIPoiObject::GetForeignRelations()
{
	if( GetOwner() == 0 ) return EOSAIEnumForeignRelations::enum_Neutral;
	return g_pEOSAICommonData->GetForeignRelations( m_pAIBrain->GetAIPlayerNumber(), GetOwner() );
}
*/
bool CEOSAIPoiObject::IsTeammateTo( long iPlayer )
{
	if( GetOwner() == 0 ) return false;
	//return m_pAIBrain->GetWorldDescPlayer()->IsTeammate( GetOwner() ); //>GetForeignRelations( GetOwner() ).IsTeammate();
	return g_pEOSAICommonData->GetForeignRelations( iPlayer, GetOwner() ).IsTeammate();
}

bool CEOSAIPoiObject::IsAnAllyTo( long iPlayer )
{
	if( GetOwner() == 0 ) return false;
	return g_pEOSAICommonData->GetForeignRelations( iPlayer, GetOwner() ).IsAlly();
	//return m_pAIBrain->GetAIBrain()->IsAlly( GetOwner() );
}
/*
bool CEOSAIPoiObject::IsTAlly()
{
	if( GetOwner() == 0 ) return false;
	return m_pAIBrain->GetWorldDescPlayer()->GetForeignRelations( GetOwner() ).IsTAlly();
}

bool CEOSAIPoiObject::IsPAlly()
{
	if( GetOwner() == 0 ) return false;
	return m_pAIBrain->GetWorldDescPlayer()->GetForeignRelations( GetOwner() ).IsPAlly();
}
*/
/*
bool CEOSAIPoiObject::IsMine()
{
	return ( GetOwner() == m_pAIPlayer->GetPlayerNumber() );
}
*/
bool CEOSAIPoiObject::IsNeutralPlayerTo( long iPlayer )
{
	if( GetOwner() == 0 ) return false;
	return g_pEOSAICommonData->GetForeignRelations( iPlayer, GetOwner() ).IsNeutral();
}

bool CEOSAIPoiObject::IsEnemyTo( long iPlayer )
{
	if( GetOwner() == 0 ) return false;
	return g_pEOSAICommonData->GetForeignRelations( iPlayer, GetOwner() ).IsEnemy();
}

bool CEOSAIPoiObject::IsOwnedByOrAlliedTo( long iPlayer )
{
	if( iPlayer == GetOwner() ) return true;

	if( GetOwner() == 0 ) return false;
	EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayer, GetOwner() );
	return ( eRel >= EOSAIEnumForeignRelations::enum_Alliance );
}

/*
void CEOSAIPoiObject::InvokeDecentWeaklyDefinedActions( CAIPlan* pAIPlan )
{
	if( this->IsMine() ) // only create WeaklyDefinedActions for my PoiObjects
	{
		if( GetFinalState()->WeaklyDefinedActionsExist() == false )
		{
			POSITION pos = pAIPlan->GetDesireList()->GetHeadPosition();
			while( pos )
			{
				CAIDesire* pDesire = pAIPlan->GetDesireList()->GetNext( pos );

				CEOSAIPoiObject* pThisPoiObject = dynamic_cast< CEOSAIPoiObject* >( this );
				if( pThisPoiObject )
				{
					pDesire->CreateWeaklyDefinedActions( pThisPoiObject );
				}
			}
		}
	}
}
*/
/*
long CEOSAIPoiObject::GetFinalGeo()
{
	//return m_pAIBrain->GetAIPlayer()->GetWorldDesc()->GetGeoNumber( GetFinalLocation() );
	return GetFinalState()->GetGeo();
}
*/
/*
float CEOSAIPoiObject::GetBestUnitActionValueSimpleScore()
{
	if( m_AllMyUnitActionIdeas.IsEmpty() == FALSE )
	{
		return m_AllMyUnitActionIdeas.GetHead()->m_fSimpleTimeToTargetScore;
	}
	return 0.0f;
}
*/



// 0-1, how does my UnitActionValue for this ActionDefinition definition compare
//   to the my top UnitActionValue?
/*
float CEOSAIPoiObject::GetRelativeActionScore( CAITacticalProject* pActionDefinition )
{
	ASSERT( m_AllMyUnitActionIdeas.IsEmpty() == FALSE );
	if( m_AllMyUnitActionIdeas.IsEmpty() == FALSE )
	{
		CEOSAIUnit2ActionIdea* pTopUnitActionValue = m_AllMyUnitActionIdeas.GetHead();
		if( pTopUnitActionValue->m_pActionDefinition == pActionDefinition )
		{
			return 1.0f;
		}
		// Now find the UnitActionValue in the list
		POSITION pos = m_AllMyUnitActionIdeas.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit2ActionIdea* pUnitActionValue = m_AllMyUnitActionIdeas.GetNext( pos );
			if( pUnitActionValue->m_pActionDefinition == pActionDefinition )
			{
				return pUnitActionValue->m_fSimpleTimeToTargetScore / pTopUnitActionValue->m_fSimpleScore;
			}
		}
	}
	return 0.0f;
}
*/


#include "stdafx.h"
/*
#include "AIObject.h"
#include "AIBrain.h"
#include "AIPlayer2.h"
#include "WorldDesc.h"
#include "MultiRegion2.h"
#include "9MovementPathwaysFromEverywhereToMRX.h"
#include "AIMovementPathwaysFromMRXtoMRY.h"
*/
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
CAIObject::CAIObject( CAIBrain* pAIBrain )
{
	m_pAIBrain = pAIBrain;
	m_bHypotheticalPoiObject = false;

	//m_fActionDemand = 0.0f;

	m_InitialState.SetAIBrain( pAIBrain );
	m_AIObjectActionProgression.SetAIObject( this );
}
*/
/*
float CAIObject::GetCurrentAirDistanceToMultiRegion( CMultiRegion* pTargetMultiRegion )
{
	C9MovementPathwaysFromEverywhereToMRX* pPathwaysFromEverywhereToMRX = pTargetMultiRegion->InvokePathwaysToMultiRegion( m_pAIBrain->GetAIPlayerNumber() );

	CAIMovementPathwaysFromMRXtoMRY* pPathwaySummary =
		pPathwaysFromEverywhereToMRX->GetPathway( m_InitialState.GetMultiRegion()->m_iId );
	ASSERT( pPathwaySummary );
	return pPathwaySummary->GetAirDistance();
}
*/
/*
bool CAIObject::ActionScheduler_TurnTasksIntoOrders()
{
	return m_AIObjectActionProgression.TurnTasksIntoOrders();
}
*/
		//friend class CAIActionScheduler;
		//void  ActionScheduler_TurnTasksIntoOrders();

/*
float CAIObject::GetAirDistanceFromAIObjectPathwayToMultiRegion( CMultiRegion* pTargetMultiRegion )
{
	C9MovementPathwaysFromEverywhereToMRX* pPathwaysFromEverywhereToMRX = pTargetMultiRegion->InvokePathwaysToMultiRegion();

	float fMinimumDistance = 1000000.0f;
	long iLastMultiRegion = 0;

	POSITION pos = m_AIPoiObjectStateProgression.m_StateProgression.GetHeadPosition();
	while( pos )
	{
		CAIPoiObjectState2* pAIPoiObjectState = m_AIPoiObjectStateProgression.m_StateProgression.GetNext( pos );
		if( pAIPoiObjectState )
		{
			long iCurrentMultiRegionId = pAIPoiObjectState->GetMultiRegion()->m_iId;
			if( iCurrentMultiRegionId != iLastMultiRegion )
			{
				iLastMultiRegion = iCurrentMultiRegionId;

				CAIMovementPathwaysFromMRXtoMRY* pPathwaySummary =
					pPathwaysFromEverywhereToMRX->GetPathway( iCurrentMultiRegionId );
				float fAirDistance = pPathwaySummary->GetAirDistance();
				if( fMinimumDistance > fAirDistance ){ fMinimumDistance = fAirDistance; }
			}
		}
	}
	return fMinimumDistance;
}
*/

/*
void CAIObject::SetInitialState( CAIPoiObjectStateAndActions* pInitialState )
{
	//m_InitialState.SetWorldDesc( pInitialState->GetWorldDesc() );
	m_InitialState.SetLocationAndGeo( pInitialState->GetLocation() );
	/*
	if( pAIPlan )
	{
		m_InitialState.SetWorldDesc( m_pAIBrain->GetAIPlayer()->GetWorldDescPlayer() );
	}
	else
	{
		m_InitialState.SetWorldDesc( pWorldDesc );
	}
	m_InitialState.SetLocationAndGeo( InitialLocation );
	*-/
}
*/
// Score
//
/*
float CAIObject::GetPrimaryActorScheduledScore()
{
	float fTotalScore = 0.0f;
	POSITION pos = m_ScheduledActions.GetHeadPosition();
	while( pos )
	{
		CAIDefinedAction* pAction = m_ScheduledActions.GetNext( pos );
		if( pAction )
		{
			if( pAction->GetActor() == this )
			{
				fTotalScore += pAction->GetScore();
			}
		}
	}
	return fTotalScore;
}
*/
// Information
/*
void CAIObject::AddLogicalObjectType( CString strLogicalObjectType )
{
	// make sure the type doesn't already exist in the list
	ASSERT( IAmOfLogicalObjectType( strLogicalObjectType ) == false );
	m_IAmOfLogicalObjectType.AddTail( strLogicalObjectType );
}

bool CAIObject::IAmOfLogicalObjectType( CString strLogicalObjectType )
{
	POSITION pos = m_IAmOfLogicalObjectType.GetHeadPosition();
	while( pos )
	{
		CString strType = m_IAmOfLogicalObjectType.GetNext( pos );
		if( strType == strLogicalObjectType )
		{
			return true;
		}
	}
	return false;
}
*/
/*
void CAIObject::SetInitialLocation( CLocation loc )
{
	m_InitialLocation = loc;
	if( m_iInitialGeo == 0 )
	{
		m_iInitialGeo = m_pAIBrain->GetAIPlayer()->GetWorldDesc()->GetGeoNumber( m_InitialLocation );
	}
}
*/
/*
long CAIObject::GetInitialGeo()
{
	//return m_pAIBrain->GetAIPlayer()->GetWorldDesc()->GetGeo( m_iInitialGeo );
	//return m_iInitialGeo;
	return m_InitialState.GetGeo();
}
*/
/*
void CAIObject::AddDecentAction( CAIDefinedAction* pAction, float fScore )
{
	ASSERT( pAction->GetScore() == fScore );

	// Add it to my decent actions list (this might delete the action)
	m_DecentPotentialActions.m_DefinedActions.Add( pAction, fScore );

	// Add it to the desire's "decent actions" list
	pAction->m_pAIDesire->AddToDecentActions( pAction );
}
*/
/*
void CAIObject::DeleteUnscheduledDecentActions()
{
	for( long i=0; i<m_DecentPotentialActions.m_DefinedActions.GetMaximumSize(); i++ )
	{
		CAIDefinedAction* pAction = m_DecentPotentialActions.m_DefinedActions.m_Array[i];
		if( pAction )
		{
			if( pAction->IsScheduled() == false )
			{
				// If it isn't scheduled, delete the object
				delete pAction;
			}
			// Remove the action from the "DecentActions" list
			m_DecentPotentialActions.m_DefinedActions.m_Array[i] = NULL;
			m_DecentPotentialActions.m_DefinedActions.m_ValueArray[i] = 0.0f;
		}
	}
}
*/
/*
void CAIObject::DetachDesires()
{
	m_AttachedDesires.RemoveAll();
}

//bool CAIObject::LogicalActionIsAssigned( CAIDefinedAction* pAction )
bool CAIObject::ActionOfThisDescriptionIsScheduled( CAIDefinedAction* pAction )
{
	POSITION pos = m_ScheduledActions.GetHeadPosition();
	while( pos )
	{
		CAIDefinedAction* pActionInList = m_ScheduledActions.GetNext( pos );
		if( pActionInList->IsSameAs( pAction ) )
		{
			return true;
		}
	}
	return false;
}
*/
/*
long CAIObject::GetNumberOfScheduledActionsNotIncludingThisOne( CAIDefinedAction* pDefinedAction )
{
	long iCount = 0;
	POSITION pos = m_ScheduledActions.GetHeadPosition();
	while( pos )
	{
		CAIDefinedAction* pDefinedActionInList = m_ScheduledActions.GetNext( pos );
		if( pDefinedActionInList != pDefinedAction )
		{
			iCount++;
		}
	}
	return iCount;
}
*/


/*
void CAIObject::RemoveDefinedLogicalActionAndEverythingAfter( CAIDefinedAction* pAction )
{
	bool bActionFound = false;

	POSITION pos = m_ScheduledActions.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CAIDefinedAction* pActionInList = m_ScheduledActions.GetNext( pos );
		if( pAction == pActionInList )
		{
			bActionFound = true;
		}
	}
	ASSERT( bActionFound );

	if( bActionFound )
	{
		while( m_ScheduledActions.IsEmpty() == FALSE )
		{
			CAIDefinedAction* pActionInList = m_ScheduledActions.GetTail();
			RemoveDefinedLogicalAction( pActionInList );
		}
	}
}
*/


/*
void CAIObject::sRemoveDefinedAction( CAIDefinedAction* pAction )
{
	// We need to remove all DefinedActions which occur after this one
	//   (I may change this in the future, if I can do it intelligently.)

	//if( pAction->IsScheduled() == false ) return;

	// First, the action needs to actually exist
	bool bActionWasScheduled = false;

	POSITION pos = m_ScheduledActions.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CAIDefinedAction* pActionInList = m_ScheduledActions.GetNext( pos );
		if( pAction == pActionInList )
		{
			bActionWasScheduled = true;
			m_ScheduledActions.RemoveAt( prevPos );
		}
	}

	//m_DecentPotentialActions.Remove( pAction );
	/*
	for( long i=0; i<m_DecentDefinedActions.GetMaximumSize(); i++ )
	{
		if( m_DecentDefinedActions.m_Array[i] == pAction )
		{
		}
	}
	*-/

	// Under certain conditions, the action will not be scheduled.
	//   In the case where an action needs a transport, the TransportAction will
	//   be scheduled with the Transport unit, but will be a prerequisite action
	//   for one of the ground unit's actions (and, therefore, not scheduled 
	//   for the ground unit).
	if( bActionWasScheduled == false ) return;

	if( bActionWasScheduled )
	{
		//pAction->UnscheduleThisAction();

		/*
		// Delete all the subsequent DefinedActions
		CAIDefinedAction* pActionDefinedInList = m_ScheduledActions.GetTail();
		while( pActionDefinedInList != pAction )
		{
			pActionDefinedInList->UnscheduleThisAction();
			pActionDefinedInList = m_ScheduledActions.GetTail();
		}

		CAIDefinedAction* pActionInList = m_ScheduledActions.RemoveTail();
		ASSERT( pActionInList == pAction );
		*/

		// Remove the DefinedAction from the Desire
		//pAction->m_pAIDesire->RemoveDefinedAction( pAction );
		//pAction->m_pAIDesire->sActionWasUnscheduled( pAction );
		/*
		pos = m_AttachedDesires.GetHeadPosition();
		while( pos )
		{
			CAIDesire* pAIDesire = m_AttachedDesires.GetNext( pos );
			pAIDesire->RemoveDefinedAction( pAction );
		}
		*-/

		// Remove the Action's Supertasks
		/*
		pos = pAction->m_SuperTasks.GetHeadPosition();
		while( pos )
		{
			CAISuperTask* pSuperTask = pAction->m_SuperTasks.GetNext( pos );
			CAIObject* pAIObject = pSuperTask->m_pAIObject;
			POSITION pos2 = pSuperTask->m_SubTasks.GetHeadPosition();
			while( pos2 )
			{
				CAITask* pTask = pSuperTask->m_SubTasks.GetNext( pos2 );
				pAIObject->sRemoveTask( pTask );
			}
			// This causes a cascade of time and location altering effects.
			//   Make sure to update the AIObject's task progression
			pAIObject->UpdateTaskProgression();
		}
		*-/
		/*
		if( pAction->m_pPrerequisiteTransportAction )
		{
			pAction->m_pPrerequisiteTransportAction->UnscheduleThisAction();
		}
		*-/
	}
}

// return the action that appears before this one
CAIDefinedAction* CAIObject::GetPreviousAction( CAIDefinedAction* pAction )
{
	CAIDefinedAction* pPreviousActionInList = NULL;
	POSITION pos = m_ScheduledActions.GetHeadPosition();
	while( pos )
	{
		CAIDefinedAction* pActionInList = m_ScheduledActions.GetNext( pos );
		if( pAction == pActionInList )
		{
			return pPreviousActionInList;
		}
		pPreviousActionInList = pActionInList;
	}
	ASSERT( false );
	return NULL;
}
*/

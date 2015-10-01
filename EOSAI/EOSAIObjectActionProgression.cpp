
#include "stdafx.h"
#include "EOSAIObjectActionProgression.h"
#include "EOSAIAction.h"
#include "EOSAITask.h"
//#include "EOSAIObject.h"
#include "PoiObject.h"
#include "EOSAIMultiRegion2.h"
//#include "9MovementPathwaysFromEverywhereToMRX.h"
//#include "EOSAIMovementPathwaysFromMRXtoMRY.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
/*
CEOSAIObjectActionProgression::CEOSAIObjectActionProgression()
{
	m_pAIObject = NULL;
}

CEOSAIObjectActionProgression::~CEOSAIObjectActionProgression()
{
	//while( m_AIActions.IsEmpty() == FALSE ){ delete m_AIActions.RemoveHead(); }
	//while( m_AITasks.IsEmpty() == FALSE ){ delete m_AITasks.RemoveHead(); }
}

//
// Information
//

EOSAI::PoiObjectState* CEOSAIObjectActionProgression2::GetInitialState()
{
	return m_pAIObject->GetInitialState();
}

EOSAI::PoiObjectState* CEOSAIObjectActionProgression2::GetFinalState()
{
	if( m_AITasks.IsEmpty() == FALSE )
	{
		CEOSAITask* pFinalTask = m_AITasks.GetTail();
		return pFinalTask->GetFinalState();
	}
	return GetInitialState();
}

float CEOSAIObjectActionProgression2::GetMovementDistance()
{
	float fDistance = 0.0f;
	POSITION pos = m_AITasks.GetHeadPosition();
	while( pos )
	{
		CEOSAITask* pTask = m_AITasks.GetNext( pos );
		fDistance += pTask->GetMovementDistance();
	}
	return fDistance;
}
*/
/*
float CAIObjectActionProgression::GetFutureAirDistanceToMultiRegion( CMultiRegion* pTargetMultiRegion )
{
	return 0.0f;
	/-*
	long iPlayer = m_pAIObject->GetAIBrain()->GetAIPlayerNumber();
	C9MovementPathwaysFromEverywhereToMRX* pPathwaysFromEverywhereToMRX = pTargetMultiRegion->InvokePathwaysToMultiRegion( iPlayer );

	long iLastMultiRegion = GetInitialState()->GetMultiRegion()->m_iId;
	CEOSAIMovementPathwaysFromMRXtoMRY* pPathwaySummary =
		pPathwaysFromEverywhereToMRX->GetPathway( iLastMultiRegion );
	float fMinimumDistance = pPathwaySummary->GetAirDistance();

	POSITION pos = m_AITasks.GetHeadPosition();
	while( pos )
	{
		CEOSAITask* pAITask = m_AITasks.GetNext( pos );
		if( pAITask )
		{
			long iCurrentMultiRegionId = pAITask->GetFinalState()->GetMultiRegion()->m_iId;
			if( iCurrentMultiRegionId != iLastMultiRegion )
			{
				iLastMultiRegion = iCurrentMultiRegionId;

				CEOSAIMovementPathwaysFromMRXtoMRY* pPathwaySummary =
					pPathwaysFromEverywhereToMRX->GetPathway( iCurrentMultiRegionId );
				float fAirDistance = pPathwaySummary->GetAirDistance();
				if( fMinimumDistance > fAirDistance ){ fMinimumDistance = fAirDistance; }
			}
		}
	}
	*-/


	/-*
	POSITION pos = m_AIPoiObjectStateProgression.m_StateProgression.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiObjectState* pAIPoiObjectState = m_AIPoiObjectStateProgression.m_StateProgression.GetNext( pos );
		if( pAIPoiObjectState )
		{
			long iCurrentMultiRegionId = pAIPoiObjectState->GetMultiRegion()->m_iId;
			if( iCurrentMultiRegionId != iLastMultiRegion )
			{
				iLastMultiRegion = iCurrentMultiRegionId;

				CEOSAIMovementPathwaysFromMRXtoMRY* pPathwaySummary =
					pPathwaysFromEverywhereToMRX->GetPathway( iCurrentMultiRegionId );
				float fAirDistance = pPathwaySummary->GetAirDistance();
				if( fMinimumDistance > fAirDistance ){ fMinimumDistance = fAirDistance; }
			}
		}
	}
	*-/
	float fMinimumDistance = 0.0f;
	return fMinimumDistance;
}
*/
//
// AIActions
//

// Add an AIAction (and it's AITasks) to the sequence.  
//   Doesn't specify where in the sequence to add the AIAction or the AITasks.
//   This method must take into account a number of factors:
//     How does insertion of AIActions, AITasks affect the overall score of this unit and
//       of the units associated with it (e.g. transports, units on those transports).
//     What about danger (e.g. attacking a city makes it less likely that I'll survive to do
//       other tasks), and timing issues (need to get to a task as quickly as possible - e.g.
//       rush to the defense of a damaged ship or city that is under seige).
/*
void CAIObjectActionProgression::AddAIAction( CEOSAIAction* pAIAction )
{
	// Not yet implimented
	ASSERT( false );
}
*/
/*
void CEOSAIObjectActionProgression2::AppendAIAction( CEOSAIAction* pAIAction )
{
	// Append the AIAction to the sequence, create Tasks, update PoiObjectStates
	m_AIActions.AddTail( pAIAction );

	//pAIAction->CreateAndAppendAITasks();
}

void CEOSAIObjectActionProgression2::RemoveAIActionAndAITasks( CEOSAIAction* pAIAction )
{
	POSITION pos = m_AIActions.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIAction* pAIActionInList = m_AIActions.GetNext( pos );
		if( pAIAction == pAIActionInList )
		{
			m_AIActions.RemoveAt( prevPos );
		}
	}
	pos = m_AITasks.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAITask* pAITaskInList = m_AITasks.GetNext( pos );
		if( pAITaskInList->GetAIAction() == pAIAction )
		{
			m_AITasks.RemoveAt( prevPos );
		}
	}
}

//
// AITasks
//



void CEOSAIObjectActionProgression2::AppendAITask( CEOSAITask* pAITask )
{
	m_AITasks.AddTail( pAITask );
}

void CEOSAIObjectActionProgression2::InsertAITask( EOSAI::PoiObjectState* pPreviousState, CEOSAITask* pAITask )
{
	ASSERT( m_pAIObject );
	if( pPreviousState == m_pAIObject->GetInitialState() )
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

void CEOSAIObjectActionProgression2::UpdateTaskTimes()
{
	ASSERT( m_pAIObject );
	// There are several things that could happen here.
	//   If it is merely a time delay, I just need to shift the start and end times.
	//   If there was a change in location, it changes the duration and requires a recalculation.
	//CEOSAILocation location = this->GetInitialLocation();
	float fCurrentTime = m_pAIObject->GetInitialState()->GetTime();
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

void CEOSAIObjectActionProgression2::sRemoveTask( CEOSAITask* pAITask )
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
*/

//
// Orders
//
/*
bool CAIObjectActionProgression::TurnTasksIntoOrders()
{
	ASSERT( false );
	ASSERT( m_pAIObject );

	// I can only give orders to my own stuff
	//bool bAllOrdersWereCreated = true;
	CEOSAIPoiObject* pAIPoiObject = dynamic_cast< CEOSAIPoiObject* >( m_pAIObject );
	if( pAIPoiObject && pAIPoiObject->IsMine() )
	{
		POSITION pos = m_AITasks.GetHeadPosition();
		while( pos )
		{
			CEOSAITask* pTask = m_AITasks.GetNext( pos );
			if( pTask->OrdersWereCreated() ) continue; // Don't re-create orders

			// If this requires a transport, make sure the pickup/dropoff tasks are created first
			pTask->TurnPrerequisiteTasksIntoOrders();
			/-*
			pTask->InvokePrerequisiteTaskOrders();
			if( pTask->HasDependencyOnTask() )
			{
				pTask->HasDependencyOnTask()->TurnTasksIntoOrders();
			}
			*-/

			// Some orders have prerequisite orders in other units.
			//   We might need to cycle back later and construct the order.
			if( pTask->OrdersWereCreated() == false )
			{
				pTask->CreateOrders();
				if( pTask->OrdersWereCreated() == false )
				{
					ASSERT( false );
					//bAllOrdersWereCreated = false;
					return false;
				}
			}
		}
	}
	return true;
}
*/

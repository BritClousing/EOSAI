
#include "stdafx.h"
#include "EOSAIUnitTask.h"
#include "AIPlayer.h"
//#include "Unit.h"
//#include "City.h"
//#include "WorldDescPlayer.h"
#include "EOSAIAction.h"

/*
Simple situation:

Ground unit scheduled to capture city.
Ground Unit:
	Action:CaptureCity
	Task:CaptureCity (Move from current location to city)
	Order:AttackCity

Transport situation:

Ground unit scheduled to capture city, but will take a transport for part of the journey.
Ground Unit:
	Action:CaptureCity [PrerequisiteAction:Transport]
	Task:EnterTransport, ExitTransport, CaptureCity
	Order:EnterTransport, ExitTransport, AttackCity

Should the prerequisite action (transport) alter the Task:CaptureCity so that 
	it's start point is different?  
Should the Task:EnterTransport, Task:ExitTransport tasks be assigned to the 
	Action:CaptureCity?
*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitTask::UnitTask( EOSAI::PoiMobile* pParentUnit, CEOSAIAction* pAIAction )
		: CEOSAITask( pParentUnit, pAIAction )
{
	m_bIsTheFinalTaskInTheAction = false;

	//m_pUnitOrder = NULL;

	m_fDistance = -1.0f;
	m_fDuration =  0.0f;
	m_fWaitTime =  0.0f;
	m_fArrivalTime = 0.0f;
}

EOSAI::UnitTask::~UnitTask()
{
	int x=0;
}

/*
bool SortByValue( CEOSAITask* pObj1, CEOSAITask* pObj2 )
{
	if( pObj1->GetScore() < pObj2->GetScore() )
	{
		return false;
	}
	return true;
}

bool SortByValue( EOSAI::UnitTask* pObj1, EOSAI::UnitTask* pObj2 )
{
	if( pObj1->GetScore() < pObj2->GetScore() )
	{
		return false;
	}
	return true;
}
*/

void EOSAI::UnitTask::SetNewStartState( EOSAI::PoiObjectState* pNewState )
{
	//
	// Figure out if there are any differences.  If there are, do the appropriate
	//    thing: recalculate the end time, recalculate the score, etc
	//

	// If the initial location changed, recalculate the distance
	bool bRecalculateDistance = false;
	if( m_MemoryOfInitialState.GetLocation() != pNewState->GetLocation() )
	{
		m_MemoryOfInitialState.SetLocationAndRegions( pNewState->GetLocation(), pNewState->GetMultiRegion() );
		bRecalculateDistance = true;
	}
	if( m_fDistance == -1.0f )
	{
		bRecalculateDistance = true;
	}

	if( bRecalculateDistance )
	{
		CalculateTaskDistanceAndDuration();

		if( this->IsValid() == false )
		{
			return;
		}
	}

	// If the end time changes, the score changes
	/*
	bool bEndTimeWasChanged = false;
	float fDuration = GetTaskDuration();
	SetStartTime( pNewState->GetTime() );
	if( GetEndTime() != pNewState->GetTime() + fDuration )
	{
		bEndTimeWasChanged = true;
		SetEndTime( pNewState->GetTime() + fDuration );
	}
	*/

	//
	// Okay, now copy the CurrentState information into the StartState.
	//
	m_MemoryOfInitialState.Copy( pNewState );

	//
	// Copy the CurrentState information into the FinalState and then
	//   make changes to the FinalState as per the task information
	//
	float fEndTime = GetEndTime();
	UpdateFinalState();
/*
	if( this->IsValid() == false )
	{
		return;
	}

	// If the end time changes, the score changes
	if( m_bIsTheFinalTaskInTheAction && fEndTime != GetEndTime() )
	{
		if( m_pDefinedAction )
		{
			m_pDefinedAction->GetPoiMobileFinalState( m_pActor )->SetTime( GetEndTime() );
			m_pDefinedAction->UpdateScore();
		}
	}
	int z=0;
*/
}

void EOSAI::UnitTask::UpdateFinalState()
{
	CEOSAILocation FinalLocation = GetFinalLocation(); // preserve the FinalLocation (it gets overwritten)
	//long iFinalGeo = GetFinalState()->GetGeo();
	CEOSAIMultiRegion2* pFinalMultiRegion = GetFinalState()->GetMultiRegion();

	float fOldEndTime = GetFinalState()->GetTime();

	GetFinalState()->Copy( &m_MemoryOfInitialState );
	GetFinalState()->SetTime( m_MemoryOfInitialState.GetTime() + m_fDuration );
	GetFinalState()->SetLocationAndRegions( FinalLocation, pFinalMultiRegion );

	// If the end time changes, the score changes
	if( m_bIsTheFinalTaskInTheAction && fOldEndTime != GetFinalState()->GetTime() )
	{
		if( m_pAIAction )
		{
			m_pAIAction->UpdateFinalTime( m_FinalState.GetTime() );
			m_pAIAction->UpdateScore();
		}
		/*
		if( m_pDefinedAction )
		{
			m_pDefinedAction->UpdateFinalTime( GetEndTime() );
			//m_pDefinedAction->GetPoiMobileFinalState()->SetTime( GetEndTime() );
			m_pDefinedAction->UpdateScore();
		}
		*/
	}
}

void EOSAI::UnitTask::AddToPermittedPath( CList< long >* pAIRegionList )
{
	m_AIRegionsPermittedPath.AddTail( pAIRegionList );
}

void EOSAI::UnitTask::SetStartTime( float f )
{
	m_MemoryOfInitialState.SetTime( f );
	//m_FinalState.m_fTime = m_InitialState.m_fTime + m_fDuration;
}



#include "stdafx.h"
#include "EOSAITask.h"
//#include "AIDesire.h"
#include "EOSAIUnit2.h"
//#include "AISpatialMap.h"
#include "EOSAIUnitTask.h"
//#include "AILogicalObject.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAITask::CEOSAITask( CEOSAIPoiObject* pActor, CEOSAIAction* pAIAction )
{
	m_fScore = 0.0f;
	m_bTaskIsValid = true;

	m_pActor = pActor;
	m_pAIAction = pAIAction;

//	m_MemoryOfInitialState.SetAIBrain( pActor->GetAIBrain() );
//	m_FinalState.SetAIBrain( pActor->GetAIBrain() );
	m_bOrdersWereCreated = false;

	//m_pHasDependencyOnTask = NULL;
}

/*
CEOSAITask* CEOSAITask::CreateTask( CAIObject* pActor, CAILogicalAction* pLogicalAction, CAIObject* pPassiveObject, CString strProduct )
{
	CString strAction = pLogicalAction->m_strAction;
	if( strAction == "CaptureCity" )
	{
		CEOSAIUnit* pAIUnitSymbolActor = dynamic_cast< CEOSAIUnit* >( pActor );
		CEOSAICity* pCity              = dynamic_cast< CEOSAICity* >( pPassiveObject );
		ASSERT( pAIUnitSymbolActor );
		ASSERT( pCity );
		EOSAI::UnitTask_CaptureCity* pCaptureTask = new EOSAI::UnitTask_CaptureCity( pAIUnitSymbolActor, pLogicalAction, pCity );
		return pCaptureTask;
	}
	if( strAction == "Explore" )
	{
		CEOSAIUnit* pAIUnitSymbolActor = dynamic_cast< CEOSAIUnit* >( pActor );
		CEOSAIRegion2*     pAIRegion          = dynamic_cast< CEOSAIRegion2*     >( pPassiveObject );
		ASSERT( pAIUnitSymbolActor );
		ASSERT( pAIRegion );
		EOSAI::UnitTask_MoveTo* pMoveToTask = new EOSAI::UnitTask_MoveTo( pAIUnitSymbolActor, pLogicalAction, pAIRegion, pAIRegion->GetInitialLocation() );
		return pMoveToTask;
	}
	return NULL;
}
*/
/*
CEOSAITask::CEOSAITask( CAIPlayer* pAIPlayer, CAIDesire* pDesire )
{
	m_pAIPlayer = pAIPlayer;
	m_pDesire = pDesire;
}
*/

/*
#include "stdafx.h"
#include "EOSAITask.h"
#include "AIDesire.h"
#include "AIScheduleModifier.h"
//#include "AIProxyPoiMobileMine.h"

CEOSAITask::CEOSAITask( CAIPlayer* pAIPlayer, CAIDesire* pDesire )
{
	m_pAIPlayer = pAIPlayer;
	m_pDesire = pDesire;
	m_bTaskIsScheduled = false;

	m_bUpdateContextualScore = true;
	//m_bScoreIsInvalid = true;
	m_fRawScore = 0.0f;
	m_fContextualScore = 0.0f;

	m_iLastRestoreId = 0;
	m_Undo_bTaskIsScheduled = false;
	m_Undo_fRawScore = 0.0f;
	m_Undo_fContextualScore = 0.0f;
}


void  CEOSAITask::SetTaskToScheduled( bool bVal, CAIScheduleModifier* pScheduleModifier )
{
	if( bVal == m_bTaskIsScheduled ) return;

	ASSERT( m_bTaskIsScheduled != bVal );
	Remember( pScheduleModifier );

	m_bTaskIsScheduled = bVal;
}


void  CEOSAITask::Remember( CAIScheduleModifier* pScheduleModifier )
{
	long iRestoreId = pScheduleModifier->GetCurrentRestoreId();

	if( m_iLastRestoreId != iRestoreId )
	{
		pScheduleModifier->ChangedTask( this );

		m_iLastRestoreId = iRestoreId;
		m_Undo_bTaskIsScheduled = m_bTaskIsScheduled;

		m_Undo_fRawScore = m_fRawScore;
		m_Undo_fContextualScore = m_fContextualScore;
	}
}

void  CEOSAITask::Restore( CAIScheduleModifier* pScheduleModifier )
{
	long iRestoreId = pScheduleModifier->GetCurrentRestoreId();

	if( m_iLastRestoreId == iRestoreId )
	{
		m_iLastRestoreId = 0;
		m_bTaskIsScheduled = m_Undo_bTaskIsScheduled;

		m_fRawScore = m_Undo_fRawScore;
		m_fContextualScore = m_Undo_fContextualScore;
	}
}

void  CEOSAITask::SetRawScore( float fNewValue, CAIScheduleModifier* pScheduleModifier )
{
	if( fNewValue != m_fRawScore )
	{
		m_fRawScore = fNewValue;

		if( this->GetTaskIsScheduled() )
		{
			if( m_pDesire && pScheduleModifier )
			{
				m_pDesire->ResortTasks( pScheduleModifier );
			}
		}
	}
}
*/
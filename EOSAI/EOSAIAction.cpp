
#include "stdafx.h"
#include "EOSAIAction.h"
//#include "AIActionOutline.h"
#include "EOSAIUnitActionIdea.h"
//#include "AITacticalProject.h"
#include "EOSAIDesire2.h"
#include "EOSAITaskForce3.h"
#include "EOSAITacticalProject2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIAction::CEOSAIAction( CEOSAIUnitActionIdea* pUnitActionIdea )
{
	ASSERT( pUnitActionIdea );
	ASSERT( pUnitActionIdea->GetTacticalProject2() );
	ASSERT( pUnitActionIdea->GetTacticalProject2()->GetAIBrain() );
	//m_FinalState.SetAIBrain( pAIUnitActionIdea->m_pActionDefinition->GetAIBrain() );

	//ASSERT( pUnitActionIdea->m_pActor );
	//m_pActionOutline = new CEOSAIActionOutline( pAIUnitActionIdea->m_pActionDefinition, pAIUnitActionIdea->m_pActor );

	m_pUnitActionIdea = pUnitActionIdea;
	m_bHasBeenTurnedIntoTasks = false;
}

CEOSAIAction::~CEOSAIAction()
{
	//delete m_pActionOutline;
}

void  CEOSAIAction::UpdateFinalTime( float fTime )
{
	ASSERT( false );
	//{ m_FinalState.SetTime( fTime ); }
}

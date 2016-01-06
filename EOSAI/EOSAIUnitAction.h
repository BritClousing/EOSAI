
#pragma once

#include "EOSAIAction.h"
#include "PoiObjectState.h"
class CAITacticalProject;
class CEOSAIUnit;
class EOSAI::UnitTask;
class CEOSAIUnitActionIdea;
class CEOSAIDesireSpatial;
class CEOSAIBrain;

// The AIUnitAction is created when I decided to use an AIUnitActionIdea.
// It describes the action undertaken by a single unit - CaptureCitRes, Attack Enemy, etc
//   AIUnitActionIdeas are used to fulfill TacticalProjects, and aren't attached directly to desires.
class CEOSAIUnitAction : public CEOSAIAction
{
	public:
		CEOSAIUnitAction( CEOSAIUnitActionIdea* pAIUnitActionValue ) : CEOSAIAction( pAIUnitActionValue )
		{
			m_pTargetDesire = NULL;
		}
		virtual ~CEOSAIUnitAction(){}

		void               SetTargetDesire( CEOSAIDesireSpatial* pTargetDesire ){ m_pTargetDesire = pTargetDesire; }
		CEOSAIDesireSpatial*  GetTargetDesire(){ return m_pTargetDesire; }

		//void  TurnActionsIntoTasks();

		float GetScore(){ return 0.0f; }

		CEOSAIDesireSpatial*  m_pTargetDesire;
};
/*
class CEOSAIUnitAction_CaptureCitRes : public CEOSAIUnitAction
{
	public:
		CEOSAIUnitAction_CaptureCitRes( CEOSAIUnitActionIdea* pAIUnitActionValue ) :
			CEOSAIUnitAction( pAIUnitActionValue ){}

		virtual void CreateAndAppendAITasks();
};
*/

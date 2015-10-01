
#pragma once

#include "EOSAIAction.h"
#include "PoiObjectState.h"
class CAITacticalProject;
class CEOSAIUnit2;
class EOSAI::UnitTask;
class CEOSAIUnit2ActionIdea;
class CEOSAIDesireSpatial;
class CEOSAIBrain;

// The AIUnitAction is created when I decided to use an AIUnitActionIdea.
// It describes the action undertaken by a single unit - CaptureCitRes, Attack Enemy, etc
//   AIUnitActionIdeas are used to fulfill TacticalProjects, and aren't attached directly to desires.
class CEOSAIUnit2Action : public CEOSAIAction
{
	public:
		CEOSAIUnit2Action( CEOSAIUnit2ActionIdea* pAIUnitActionValue ) : CEOSAIAction( pAIUnitActionValue )
		{
			m_pTargetDesire = NULL;
		}
		virtual ~CEOSAIUnit2Action(){}

		void               SetTargetDesire( CEOSAIDesireSpatial* pTargetDesire ){ m_pTargetDesire = pTargetDesire; }
		CEOSAIDesireSpatial*  GetTargetDesire(){ return m_pTargetDesire; }

		//void  TurnActionsIntoTasks();

		float GetScore(){ return 0.0f; }

		CEOSAIDesireSpatial*  m_pTargetDesire;
};
/*
class CEOSAIUnit2Action_CaptureCitRes : public CEOSAIUnit2Action
{
	public:
		CEOSAIUnit2Action_CaptureCitRes( CEOSAIUnit2ActionIdea* pAIUnitActionValue ) :
			CEOSAIUnit2Action( pAIUnitActionValue ){}

		virtual void CreateAndAppendAITasks();
};
*/

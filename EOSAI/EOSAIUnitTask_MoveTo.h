
#pragma once

#include "EOSAIUnitTask.h"

class CAISuperTask;
class CEOSAIUnit2;

namespace EOSAI
{
	class UnitTask_MoveTo : public EOSAI::UnitTask
	{
	public:
		UnitTask_MoveTo(EOSAI::PoiMobile* pActor,
			CEOSAIAction* pAIAction,
			EOSAI::PoiObjectState* pMyUnitsInitialState,
			CEOSAILocation TargetLocation);
		~UnitTask_MoveTo();
		/*
		EOSAI::UnitTask_MoveTo( EOSAI::PoiMobile* pParentUnit,
							//CAISuperTask* pSuperTask,
							CAIDefinedAction* pDefinedAction,
							//CEOSAILocation InitialLocation,
							EOSAI::PoiObjectState* pMyUnitsPreviousState,
							CEOSAILocation TargetLocation );
		*/
		virtual void UpdateFinalState(); // can be overridden (based on what the task is doing)

		virtual void CalculateTaskDistanceAndDuration();
		//virtual void CreateOrders();

		//float m_fExploreVolume;
	protected:
		CEOSAILocation  m_TargetLocation;
	};
}
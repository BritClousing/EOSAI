
#pragma once

#include "EOSAIUnitTask.h"

namespace EOSAI
{
	class UnitTask_Pickup;
}
class CAITransportDefinition;

// "Enter" task is given to the soon-to-be containee

namespace EOSAI
{
	class UnitTask_Enter : public EOSAI::UnitTask
	{
	public:
		UnitTask_Enter(EOSAI::PoiMobile* pActor,
			CEOSAIPoiObject* pTargetToEnter,
			CEOSAIAction* pAIAction,
			EOSAI::PoiObjectState* pMyUnitsInitialState,
			CEOSAILocation JumpOffLocation);
		//CMultiRegion* pMultiRegion );
		virtual ~UnitTask_Enter();

		void SetAssociatedPickupTask(EOSAI::UnitTask_Pickup* pPickupTask) { m_pAssociatedPickupTask = pPickupTask; } // Optional
		void UpdatePickupTime() {}
		void SetWaitTime(float f) { m_fWaitTime = f; }

		virtual void UpdateFinalState(); // can be overridden (based on what the task is doing)

		virtual void CalculateTaskDistanceAndDuration();
		//virtual void CreateOrders();

	//
		//virtual void TurnPrerequisiteTasksIntoOrders();

	protected:
		//CAITransportDefinition* m_pAITransportDefinition;
		EOSAI::UnitTask_Pickup*  m_pAssociatedPickupTask;  // Optional, only valid if m_pTargetToEnter is a PoiMobile ("Land on carrier", "Enter airfield", and "Enter City" don't need a pickup task)
		CEOSAIPoiObject*        m_pTargetToEnter; // PoiMobile, City, Fort, or something else
	};
}
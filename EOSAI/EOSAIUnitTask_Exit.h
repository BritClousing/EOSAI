
#pragma once

#include "EOSAIUnitTask.h"

class CAISuperTask;
class CEOSAIUnit;
namespace EOSAI
{
	class UnitTask_Dropoff;
}
class CAITransportDefinition;

// "Exit" task is given to the soon-to-be containee
namespace EOSAI
{
	class UnitTask_Exit : public EOSAI::UnitTask
	{
	public:
		UnitTask_Exit(EOSAI::PoiMobile* pActor,
			EOSAI::PoiMobile* pUnitToEnter,
			CEOSAIAction* pAIAction,
			EOSAI::PoiObjectState* pMyUnitsInitialState,
			CEOSAILocation JumpOffLocation);
		//CMultiRegion* pMultiRegion );
		virtual ~UnitTask_Exit() {}

		void  SetAssociatedDropoffTask(EOSAI::UnitTask_Dropoff* pDropoffTask) { m_pAssociatedDropoffTask = pDropoffTask; } // Optional
		void  UpdateDropoffTime() {}

		virtual void UpdateFinalState(); // can be overridden (based on what the task is doing)

		virtual void CalculateTaskDistanceAndDuration();
		//virtual void CreateOrders();
	//
		//virtual void TurnPrerequisiteTasksIntoOrders();

		//
		//CAITransportDefinition* m_pAITransportDefinition;
		EOSAI::UnitTask_Dropoff*  m_pAssociatedDropoffTask;  // Optional
	};
}

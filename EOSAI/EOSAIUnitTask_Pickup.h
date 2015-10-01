
#pragma once

#include "EOSAIUnitTask.h"
class CPoiPD;
namespace EOSAI
{
	class UnitTask_Enter;
}
class CAITransportDefinition;

// "Pickup" task is given to transports
namespace EOSAI
{
	class UnitTask_Pickup : public EOSAI::UnitTask
	{
	public:
		UnitTask_Pickup(EOSAI::PoiMobile* pActor,
			EOSAI::PoiMobile* pUnitToPickup,
			CEOSAIAction* pAIAction,
			EOSAI::PoiObjectState* pMyUnitsInitialState,
			CEOSAILocation WaterPickupLocation);
		//CEOSAIRegion2* pWaterAIRegion );
		virtual ~UnitTask_Pickup() {}

		//
		void SetAssociatedEnterTask(EOSAI::UnitTask_Enter* pEnterTask) { m_pAssociatedEnterTask = pEnterTask; } // Optional
		void SetWaitTime(float f) { m_fWaitTime = f; }
		void UpdatePickupTime() {}

		virtual void UpdateFinalState(); // can be overridden (based on what the task is doing)

		virtual void CalculateTaskDistanceAndDuration();
		//virtual void CreateOrders();
		CPoiPD*      GetPickupPoiPD() { return m_pPickupPoint; } // valid after CreateOrders() is called

	protected:
		//CAITransportDefinition* m_pAITransportDefinition;
		EOSAI::UnitTask_Enter*  m_pAssociatedEnterTask; // Optional
		EOSAI::PoiMobile*       m_pUnitToPickup;
		CPoiPD*             m_pPickupPoint;  // valid after CreateOrders() is called
	};
}

#pragma once

#include "EOSAIUnitTask.h"
class CPoiPD;
namespace EOSAI
{
	class UnitTask_Exit;
}
class CAITransportDefinition;

// "Dropoff" task is given to transports
namespace EOSAI
{
	class UnitTask_Dropoff : public EOSAI::UnitTask
	{
	public:
		UnitTask_Dropoff(EOSAI::PoiMobile* pTransportActor,
			EOSAI::PoiMobile* pUnitToDropoff,
			CEOSAIAction* pAIAction,
			EOSAI::PoiObjectState* pMyUnitsPreviousState,
			CEOSAILocation WaterDropoffLocation);
		//CEOSAIRegion2* pAIRegion );
		//CMultiRegion* pMultiRegion );
		virtual ~UnitTask_Dropoff() {}

		void  SetAssociatedExitTask(EOSAI::UnitTask_Exit* pExitTask) { m_pAssociatedExitTask = pExitTask; } // Optional
		void  UpdateDropoffTime() {}

		virtual void UpdateFinalState(); // can be overridden (based on what the task is doing)

		virtual void CalculateTaskDistanceAndDuration();
		//virtual void CreateOrders();
		CPoiPD*      GetDropoffPoiPD() { return m_pDropoffPoint; } // valid after CreateOrders() is called

	protected:
		//CAITransportDefinition* m_pAITransportDefinition;
		EOSAI::UnitTask_Exit*  m_pAssociatedExitTask;
		EOSAI::PoiMobile*      m_pUnitToDropoff;
		CPoiPD*            m_pDropoffPoint;  // valid after CreateOrders() is called
	};
}
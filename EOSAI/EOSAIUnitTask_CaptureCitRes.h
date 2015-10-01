
#pragma once

#include "EOSAIUnitTask.h"
//#include "City.h"

class CAISuperTask;
class CEOSAICity;
class CEOSAIPoiObject;
class CEOSAIUnit2;
namespace EOSAI
{
	class UnitTask_CaptureCitRes : public EOSAI::UnitTask
	{
	public:
		UnitTask_CaptureCitRes(EOSAI::PoiMobile* pParentUnit,
			CEOSAIAction* pAIAction,
			EOSAI::PoiObjectState* pMyUnitsPreviousState,
			CEOSAIPoiObject* pTargetCitRes);
		virtual ~UnitTask_CaptureCitRes() {}
		//
		CEOSAIPoiObject* GetTargetCitRes() { return m_pTargetCitRes; }

		virtual void UpdateFinalState(); // can be overridden (based on what the task is doing)

		virtual void CalculateTaskDistanceAndDuration();
		//virtual void CreateOrders();

		//CEOSAICity* m_pTargetCity;
		CEOSAIPoiObject* m_pTargetCitRes;
	};
}
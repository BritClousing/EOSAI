
#pragma once

#include "EOSAILocation.h"
class CEOSAIUnit;
//class CAISymbolProcessor;
class CAIObject;
#include "EOSAIUnit.h"
#include "EOSAITask.h"
//#include "AILogicalObject.h"
#include "PoiObjectState.h"
//class CUnitOrder;

bool SortByValue( CEOSAITask* pObj1, CEOSAITask* pObj2 );
bool SortByValue( EOSAI::UnitTask* pObj1, EOSAI::UnitTask* pObj2 );

// Types of tasks:
//   Destroy enemy units
//   Create defensive barrier against enemy units
//   Capture Citres
//   Explore
//   Search for enemy units (similar to explore, but works in discovered areas)
//   Transport other units
//   Degrade enemy cities (strategic bombing)
namespace EOSAI
{
	class EOSAI::UnitTask : public CEOSAITask
	{
	public:

		UnitTask(EOSAI::PoiMobile* pParentUnit, CEOSAIAction* pAIAction);
		virtual ~UnitTask();

		EOSAI::PoiMobile* GetParentUnit() { return (EOSAI::PoiMobile*) m_pActor; }

		void    IsTheFinalTaskInTheAction(bool b) { m_bIsTheFinalTaskInTheAction = b; }

		void         SetNewStartState(EOSAI::PoiObjectState* pNewState);
		virtual void UpdateFinalState(); // can be overridden (based on what the task is doing)

		void  AddToPermittedPath(long iAIRegion) { m_AIRegionsPermittedPath.AddTail(iAIRegion); }
		void  AddToPermittedPath(CList< long >* AIRegionList);

		//virtual void CreateOrders(){}
		bool  OrdersWereCreated() { return m_bOrdersWereCreated; }

		//
		//void      SetInitialLocation( CEOSAILocation loc ){ m_InitialState.SetLocationAndGeo( loc ); }
		//CEOSAILocation GetInitialLocation(){ return m_InitialState.GetLocation(); }
		CEOSAILocation GetFinalLocation() { return m_FinalState.GetLocation(); }

		// Read the PreviousLocation, FinalLocation and update the 
		//  m_fTimeOfTaskBegin and m_fTimeOfTaskEnd values.
		virtual void  CalculateTaskDistanceAndDuration() = 0;
		virtual void  SetTaskDuration(float fDuration) { m_fDuration = fDuration; }
		virtual float GetTaskDuration() { return m_fDuration; }
		virtual float GetStartTime() { return m_MemoryOfInitialState.GetTime(); }//m_fStartTime; }
		virtual void  SetStartTime(float f);//{ m_InitialState.m_fTime = f; }//m_fStartTime = f; }
		virtual float GetEndTime() { return m_FinalState.GetTime(); }//m_fEndTime; }
		virtual void  SetEndTime(float f) { m_FinalState.SetTime(f); }//m_fEndTime; }

		float GetArrivalTime() { return m_fArrivalTime; }

		//EOSAI::PoiObjectState* GetStartState(){ return &m_InitialState; }
		//EOSAI::PoiObjectState* GetFinalState(){ return &m_FinalState; }

		virtual float  GetMovementDistance() { return m_fDistance; }

		//void SetCheckpoint( CAISymbolProcessor* Proc );
		//void RestoreToCheckpoint( CAISymbolProcessor* Proc );

	protected:

		bool  m_bIsTheFinalTaskInTheAction;  // if this is true, then any changes to this task affects the parent DefinedAction
		//
		float m_fArrivalTime; // If Pickup or Dropoff, this is the arrival time at the rendezvous point
		float m_fWaitTime;    // Pickup and Enter tasks might have wait times associated with them
		float m_fDistance;    // Important for ranged units (aircraft)
		float m_fDuration;    // Time to get to the rendezvous point.
		//
		//CAIPoiObjectState m_MemoryOfInitialState;
		//CAIPoiObjectState m_FinalState;
		CList< long >  m_AIRegionsPermittedPath; // If I want to restrict the pathway to certain AIRegions
		//
		//CUnitOrder* m_pUnitOrder;
	};
}

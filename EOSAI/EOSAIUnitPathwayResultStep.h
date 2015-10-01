
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

//#include "AIUnitPathway.h"
//#include "EOSAIUnitPathwayPredefinedStep.h"
//#include "AIAirResistanceSet.h"
//#include "EnumPathwayResultStep.h"
//#include "UnitTemplate.h"
#include "EOSAILocation.h"
class CEOSAIUnit2PathwayPredefinedStep;
namespace EOSAI
{
	class UnitPathwayResult;
}
class CEOSAIUnit2PathwayFinder;
//class CEOSAIUnit2PathwayFinder;
class CUnitTemplate;
class CEOSAIRegion2;
class CEOSAIPoiObject;
class CEOSAIUnit2;
class CUnitOrder;

//
// Example Results (for a transport):
//   Transport Step: Pickup  GroundUnit (move to TransportAIRegion, create PoiPD for enter)
//   Transport Step: Dropoff GroundUnit (move to TransportAIRegion, create PoiPD for exit)
//   GroundUnit Step: Enter Transport   (move to TransporteeAIRegion, enter the PoiPD of the RelatedStep)
//   GroundUnit Step: Exit Transport    (exit the PoiPD of the RelatedStep)
//   GroundUnit Step: Attack Target,... (move to TransporteeAIRegion, enter the PoiPD)
//
namespace EOSAI
{
	class DLLIMPEXP UnitPathwayResultStep
	{
	public:
		enum EnumTask
		{
			enum_Undefined,
			//
			enum_Waypoint,
			enum_LandAtAirbase,  // Aircraft lands in cities, airfield, carriers
			//
			enum_BuildAirfield,
			enum_Repair,
			enum_Upgrade,
			//
			enum_AttackUnit,
			enum_CaptureCitRes,
			enum_DegradeCityOrAirfield,
			//
			enum_Pickup,
			enum_Dropoff,
			//enum_Enter,
			//enum_Exit,
			//enum_AircraftLand,  // Aircraft lands in cities, airfield, carriers
		};

		UnitPathwayResultStep(EOSAI::UnitPathwayResult* pPathwayResult);
		~UnitPathwayResultStep();

		CEOSAIUnit2PathwayFinder*  GetAIUnitPathwayFinder();

		// Initialization
		//
		void  SetPredefinedStep(CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep);

		//
		EOSAI::UnitPathwayResultStep*  GetPrevStep();
		EOSAI::UnitPathwayResultStep*  GetNextStep();

		// (Used by the aircraft)
		void  LandAtAirbase(CEOSAIPoiObject* pAIPoiObject) { m_eTask = enum_LandAtAirbase; m_pAITarget = pAIPoiObject; }
		bool  IsALandAtAirbase() { return m_eTask == enum_LandAtAirbase; }
		bool  IsAPickup() { return m_eTask == enum_Pickup; }

		//	float GetStartTime();
		float GetGroundUnitStartTimeToPickupTime() { return m_fStartTimeToPickupTime; }
		float GetGroundUnitDropoffTimeToTargetTime() { return m_fDropoffTimeToTargetTime; } // Delta time
		float GetStepTime() { return m_fStepTime; }
		//	float GetEndTime(); // based on previous steps + StepTime + waiting periods
			//void  UpdateWithTransportAssistedPath( CEOSAIUnit2* pAITransport );
			//
		CEOSAILocation  GetGroundPickupLocation();
		//
		void  AddSupportUnitOrder(CUnitOrder* pNewOrder) { m_SupportOrders.AddTail(pNewOrder); }
		void         SetUnitOrder(CUnitOrder* pNewOrder) { m_pUnitOrder = pNewOrder; }
		CUnitOrder*  GetUnitOrder() { return m_pUnitOrder; }

		void  SetUnitOrderEndTime(float fEndTime) { m_fUnitOrderEndTime = fEndTime; }
		float GetUnitOrderEndTime() { return m_fUnitOrderEndTime; }

		//
		EOSAI::UnitPathwayResult*  m_pResult;
		CEOSAIUnit2PathwayPredefinedStep*  m_pPredefinedStep; // NULL if this is a Pickup or Dropoff
		EnumTask   m_eTask; // Pickup, Dropoff, etc
		//
		CEOSAILocation  m_StartLocation;
		CEOSAILocation  m_EndLocation;

		bool   m_bIsValid;

		float  m_fDistance;
		float  m_fDanger;
		float  m_fSighting;
		float  m_fStepTime; // time to complete this step (not cummulative with earlier steps)

		float  m_fStartTimeToPickupTime; // used only if this step is using a transport
		float  m_fDropoffTimeToTargetTime; // used only if this step is using a transport
		//long   m_iTransportsNeeded;
	//	CList< CEOSAIRegion2* >  m_AIRegionMovementPath;

		CEOSAIPoiObject*  m_pAITarget; // Attack Target, Transportee

		// Used by GroundUnits
		bool   m_bTransportNeeded;

		// This is especially important for airunits taking anti-aircraft fire
		//CAIAirResistanceSet  m_Resistance;
		float  m_fSurvivalOdds;
		bool   m_bResultsInDeath; // Self-Destruct or ran-out-of-fuel

		// Transportee-only information (I don't have explicit enter/exit steps - they are embedded in another step)
	//	EOSAI::UnitPathwayResultStep*  m_pPickupStep; // corresponding Pickup/Enter or Dropoff/Exit Step
	//	EOSAI::UnitPathwayResultStep*  m_pDropffStep; // corresponding Pickup/Enter or Dropoff/Exit Step
		// Transport-only information
	//	CList< CEOSAIRegion2* >  m_TransporteeAIRegionMovementPath; // Transportee->Enter, Transportee Exit->Target
	//	long   m_iEnterExitAIRegion;
	//	long   m_iPickupDropoffAIRegion;
		//long   m_iPickupPoiPD;
		//long   m_iDropoffPoiPD;

		CList< CUnitOrder* >  m_SupportOrders;
		CUnitOrder*  m_pUnitOrder;
		float        m_fUnitOrderEndTime;

		//CAITransportPossibility*  m_pAllocatedTransport;

		// Support units (Transports, Carriers used in movement)
		//CList< EOSAI::UnitPathwayResult* >  m_SupportUnits; // Transports, Carriers used in movement
		//CList< CEOSAIUnit2PathwayFinder* >  m_SupportUnitPathways; // Defines the Unit + PredefinedSteps + Pathway
};
}

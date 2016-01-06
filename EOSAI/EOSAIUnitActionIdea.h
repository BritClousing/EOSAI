
#pragma once

//#include "AIPoiObjectActionIdea.h"
#include "EOSAIListSort.h"
#include "EOSAIJobRole.h"
#include "EOSAIEnumMovementType.h"
#include "EOSAILocation.h"
class CAITacticalProject;
class CEOSAITacticalProject2;
class CEOSAIUnit;
class CEOSAICity;
class CEOSAIPoiObjectProximity_Unit;
class CEOSAIUnitPathwayIdea;
class CAIPoiObjectStateProgressionValue;
class CEOSAIActionOutline;
class CEOSAIBrain;
class CEOSAIPoiObject;
class CEOSAIMultiRegion2;
class CUnitTemplate;
class CEOSAIAction;
class CEOSAIUnitAction;
//class CMovementPathIdeas;
class CMovementPath;
class CEOSAIUnitProximity;
class CEOSAIPoiObjectProximity2;
class CEOSAIDesire2;
class CEOSAITaskForce3;
class CEOSAIUnitPathwayFinder;
class CEOSAIPoiObject;
class CEOSAIUnitActionIdea;
class CEOSAITransportAssistedPath;
namespace EOSAI
{
	class UnitTask_Pickup;
	class UnitTask_Dropoff;
}
class CEOSAIUnitPathwayPredefinedStep;
class CEOSAIDesireSpatial;
class CEOSAIUnitTemplate;

//class CEOSAICityActionIdeaNode;

//class CEOSAIPoiObjectProximity2;

//
// Overall, the UnitActionValues are used for a simple movement-time calculation, answering
//   these questions:
//   - Can I get UnitX/CityBuildX to location? Do I need a transport or air-transport to get there?
//   - How long will it take to get from the Unit/City pathway to the location?
//   It can also help to answer the question of whether a transport would be useful in this case.
//
// In the case of ground units, it does this by calculating multiple routes (no-transport,
//   one-sea-transport, air-transport).  
// 

//
class CEOSAIUnitTransportMovementDesc
{
	public:
		CEOSAIUnitTransportMovementDesc( CEOSAIUnit* pAITransportee, CEOSAIUnitActionIdea* pAITransporteeUnitActionIdea );
		~CEOSAIUnitTransportMovementDesc();

		CEOSAIPoiObject* GetAIPoiActor();
		//CEOSAIUnitActionIdea* CreateAIUnitActionIdea( CAITacticalProject* pAITacticalProject );
		CEOSAIUnitActionIdea* CreateAIUnitActionIdea( CEOSAITacticalProject2* pAITacticalProject );

	//private:
		CEOSAIUnit*            m_pAITransportee;
		CEOSAIUnitActionIdea*  m_pAITransporteeUnitActionIdea; // not owned
		CEOSAIUnitActionIdea*  m_pAITransportUnitActionIdea; // owned, gets filled in by "CreateAIUnitActionIdea"
		//CEOSAIUnit*            m_pHypotheticalAITransportUnit; // owned

		CEOSAIUnit*          m_pAITransport; // required    // optional
		//CEOSAICity*        m_pAICity;          // optional (if I build a transport, it gets built here)
		CEOSAIUnitTemplate*  m_pTransportAIUnitTemplate;

		float   m_fBuildTime;
		float   m_fTotalTime;
};

class CEOSAIUnitActionIdeaTargetValue
{
	public:
		CEOSAIUnitActionIdeaTargetValue( CEOSAIUnitActionIdea* pAIUnitActionIdea, CEOSAIDesireSpatial* pDesire )
		{
			m_pAIUnitActionIdea = pAIUnitActionIdea;
			m_pDesire = pDesire;
			m_fTimeToTarget = 0.0f;
			m_fAttrition = 0.0f;
			m_fTotalExpectedProductionDamageToEnemy = 0.0f;
			m_fPerTurnExpectedProductionDamageToEnemy = 0.0f;
			m_fTargetValue = 0.0f;
		}
		// Should include pathway information (for bombardment, anti-air), combat difficultly, etc
		void  CalculateValue(); 

		CEOSAIUnitActionIdea* m_pAIUnitActionIdea;
		CEOSAIDesireSpatial*  m_pDesire;
		float  m_fTimeToTarget;
		float  m_fAttrition;
		float  m_fTotalExpectedProductionDamageToEnemy;   // If I can add-up to the enemy unit's cost, it will be destroyed (within combat variability)
		float  m_fPerTurnExpectedProductionDamageToEnemy;

		float  m_fTargetValue;
};

//class CEOSAIUnitActionIdea
class CEOSAIUnitActionIdea //: public CAIPoiObjectActionIdea
{
	public:
		// This is important for a number of reasons:
		//   It helps the AI know whether the unit needs to return when calculating paths (missiles don't)
		//   It also it necessary for the next step - creating AIActions
		
		enum EnumActionType
		{
			EnumActionType_Undefined,
		//	EnumActionType_Capture,
		//	EnumActionType_Attack,
		//	EnumActionType_BuildAirfield,
		//	EnumActionType_MoveTo,
			EnumActionType_Transport, // Transport unit X (help it fulfill its UnitActionIdea)
		};
		enum EnumMovementMotive
		{
			MovementMotive_Attack,   // I will need to return - unless the unit self-destructs
			MovementMotive_RoundTrip // I need to return
		};
		enum EnumPathwayCalculationLevel
		{
			EnumPathwayCalculationLevel_SimplePath, // find reasonable path, ignore transports
		};
		enum EnumMovementDescFilter
		{
			EnumMovementDescFilter_BestPossible,
			EnumMovementDescFilter_BestAvailable,
		};

		// This is a static function (can be used for any units, including transports, enemy units, etc)
		//   It takes MovementRange and Airbases into account, and returns 1000000 if there is
		//   no airbase close enough to bring the AirUnit to the UnitEnd Location
		static float GetAirPathwayTimeToTarget(
					long iAirUnitOwner,
					CEOSAIBrain* pAIBrain,
					CEOSAIUnitTemplate* pAirUnit,
					CEOSAILocation UnitStart,
					CEOSAILocation UnitEnd,
					EnumMovementMotive eMovementMotive );

	public:

		CEOSAIUnitActionIdea();//{ m_pTacticalProject = NULL; Clear(); }
		//CEOSAIUnitActionIdea( CAITacticalProject* pTacticalProject, CEOSAIUnit* pAIUnit );
		CEOSAIUnitActionIdea( CEOSAITacticalProject2* pTacticalProject, CEOSAIUnit* pAIUnit );
		//CEOSAIUnitActionIdea( CAITacticalProject* pTacticalProject, CEOSAICity* pAICity, CUnitTemplate* pUnitTemplate );
		CEOSAIUnitActionIdea( CEOSAITacticalProject2* pTacticalProject, CEOSAICity* pAICity, CEOSAIUnitTemplate* pUnitTemplate );
		~CEOSAIUnitActionIdea();
		//void  SetAITacticalProject( CAITacticalProject* pTacticalProject ){ m_pTacticalProject = pTacticalProject; }

		// Initialization
		//
			void Clear();
			void SetActor( CEOSAIPoiObject* pAIActor );

		// Actor
		//
			CEOSAIPoiObject*  GetAIPoiObjectActor();
			CEOSAIUnit*       GetAIUnitActor(){ return m_pAIUnitActor; } // May be NULL
			CEOSAICity*       GetAICityActor();//{ return m_pAICityActor; } // May be NULL
			CEOSAIUnitTemplate* GetAIUnitTemplate(){ return m_pAIUnitTemplate; }
			//CAITacticalProject*  GetTacticalProject(){ return m_pTacticalProject; }
			CEOSAITacticalProject2* GetTacticalProject2(){ return m_pTacticalProject2; }
			CEOSAIBrain*      GetAIBrain(){ return m_pAIBrain; }

			CEOSAIJobRole*    GetJobRole(){ return &m_JobRole; } // include combat in this unit's role?

		// Action
		//
			EnumActionType GetActionType(){ return m_eActionType; }

			//void  Capture(){ m_eActionType = EnumActionType_Capture; }
			void  Transport( CEOSAIUnit* pAITransportee, CEOSAIUnitActionIdea* pAITransporteeUnitActionIdea ){ m_eActionType = EnumActionType_Transport; m_pAITransportee = pAITransportee; m_pAITransporteeUnitActionIdea = pAITransporteeUnitActionIdea; }
			
			CEOSAILocation  GetTargetLocation();

		//
			void  SetBuildTime( float f ){ m_fBuildTime = f; }
			float GetBuildTime(){ return m_fBuildTime; }

		// Transport Pathway information
		//
			//bool  RequiresTransport(){ return m_iTransportsRequired > 0; }
			bool  RequiresTransport(){ return m_bTransportRequired; }
			//float GetBestTransportArrivalTime(){ return m_fTransportArrivalTime; }
			//void  SetTransportArrivalTime( float f ){ m_fTransportArrivalTime = f; }
			void  GetTransportIdeas2( EnumMovementDescFilter eFilter, CEOSAITaskForce3* pAITaskForce, CEOSAIUnitTransportMovementDesc** pBestMovementDesc );

			CEOSAIUnitActionIdea*  GetAITransportUnitActionIdea(){ return m_pAITransportUnitActionIdea; }
			void                SetAITransportUnitActionIdea( CEOSAIUnitActionIdea* pTransportUnitActionIdea );

		// Task Force
		//
			//CEOSAITaskForce3* GetTaskForce(){ return m_pTaskForce; }

		//
			//void  CalculatePathway( EnumPathwayCalculationLevel ePathwayLevel );			
			void  CalculateTimeToTarget(); // Uses DirectPaths, ComboTransports, (Air Units use AirPathways)
			//void  CalculatePathwayAndTimeToTarget();
			void  InvokeMultiRegionTransportMovementDescs(); // Creates a bunch of MovementDescs
			//void  InvokeAIRegionTransportMovementDescs(); // Creates a bunch of MovementDescs

			float GetBestCaseTotalTimeToTarget(){ return min( m_fDirectMoveToTargetTime, m_fTransportComboTime ); } // Build + Transport|DirectPath
			//float GetBuildAndTransportAndMoveToTargetTime();
			//float GetBuildAndDirectMoveToTargetTime();

			//float GetMoveToTargetTime(){ return m_fBuildTime + min( m_fDirectMoveToTargetTime, m_fTransportMoveToTargetTime ); }
			void  SetDirectMoveToTargetTime( float f ){ m_fDirectMoveToTargetTime = f; }
			float GetDirectMoveToTargetTime(){ return m_fDirectMoveToTargetTime; }
			//void  SetTransportAssistedMoveToTargetTime( float f ){ m_fTransportAssistedMoveToTargetTime = f; }
			//float GetTransportAssistedMoveToTargetTime(){ return m_fTransportAssistedMoveToTargetTime; }
			float GetComboTransportMoveToTargetTime(){ return m_fTransportComboTime; }

			//float GetMoveToTargetTimeScore(){ return 1.0f / m_fMoveToTargetTime+0.1f; }

			//float GetTimeToTargetUsingNoTransports();
			//float GetTimeToTargetUsingTransport( CEOSAIUnit* pAITransport );
			//float GetTimeToTargetUsingTransport( CEOSAIPoiObject* pAITransportOrCity );
			//float GetTimeToTargetUsingTransport( CEOSAIPoiObjectProximity2* pAITransportOrCity );

			// Based on MovementPathIdeas, and (if transport is useful) the best case movement time
			//float GetTimeToTargetForHypotheticalTaskForce_AllowTransport();
		//
			float GetUnitProductionIronCost1();

		// Information
		//
			//float GetShortestPathDistance();

		// Role
		//
			//void  CalculateActionRoleScores();

		// Task Values
		//
			//float  GetGroundCitResHunterCapacity01();
			//float  GetSeaResHunterCapacity01();
			//float  GetAirfieldBuilderCapacity01();

		//
		// Requirements Fulfilled?
		//
			// Ground Units
			//
				//float GetTransportValueBasedOnDistance(); // 0-1

			// Air Units - Within Air-range?
			//
				//bool  TargetIsWithinMyAirRange(); // from current location
				//bool  TargetIsWithinMyAirRangeFromOneOfMyAirbases(){ return m_bTargetIsWithinMyMovementRangeFromOneOfMyAirbases; }

		// Recalculate
		//
			//void  ReCalculateAndReScore();

		// Allocation - I plan to use this UnitActionIdea, but haven't worked out any details
		//
			bool ActorIsAllocated();

		// Scheduling - I plan to use this UnitActionIdea, and have created AIActions for the Unit
		//
			//bool IsScheduled(){ return m_pScheduledAIUnitAction != NULL; }
			//void SetAIUnitAction( CEOSAIUnitAction* pScheduledAIAction ){ m_pScheduledAIUnitAction = pScheduledAIAction; }
			//CEOSAIUnitAction* GetAIUnitAction(){ return m_pScheduledAIUnitAction; }

		// Desire Allocation (one of the Desires in the TacticalProject)
		//
			//void AddDesireAllocation( CEOSAIDesireSpatial* pDesire ){ m_DesireAllocations.AddTail( pDesire ); }
			void AddDesireAllocation( CEOSAIDesireSpatial* pDesire ){ m_DesireAllocations.AddTail( pDesire ); }
			long GetNumberOfDesireAllocations(){ return (long) m_DesireAllocations.GetCount(); }

		// Role:
		//
			//bool  IsAPrimaryUnit(){ return m_bIsAPrimaryUnit; }
			// CombatValue against target depends on a number of factors:
			//   Attrition value (cost, attack, defense),
			//   My best units for this task (if all my units are bad, the best ones have the best score)
			//float GetCombatValueAgainstTarget( CList< CEOSAIPoiObject* >* pTargetList );

			// Combat value / Production cost
			//float GetCombatEfficiency();


	//private:

		//
		CEOSAIBrain*  m_pAIBrain;
		//CAITacticalProject*  m_pTacticalProject;
		CEOSAITacticalProject2* m_pTacticalProject2;

		bool m_bDebug_HasBeenDeleted;

		//CEOSAITaskForce3*  m_pTaskForce;
		EnumActionType    m_eActionType;
		//CEOSAIPoiObject*   m_pActor; // always valid - might be AIUnit, might be AICity (build)
		CEOSAIUnit*          m_pAIUnitActor; // (valid if Unit) includes current HP information
		//CEOSAICityActionIdeaNode*  m_pAICityActionIdeaNode; // (valid if City)
		CEOSAICity*          m_pAICityActor; // (valid if City)
		CEOSAIUnitTemplate*  m_pAIUnitTemplate; // (always valid) if the actor is a city, this is the build

		//
		void   CompileDesireValues();
		long   GetNumberOfDesireValues(){ return (long) m_DesireValues.GetCount(); }
		long   GetNumberOfAvailableDesireTargets();
		void   CreatePredefinedStep();
		CEOSAIUnitPathwayPredefinedStep*  GetPredefinedStep();
		CEOSAIUnitPathwayPredefinedStep*  m_pPredefinedStep; // not owned (ownership passed to AIUnit)

		// The three pathways are used for determining if a new transport would be useful
		//bool  ValidSingleGeoRouteExists(){ return m_bSingleGeoRouteExists; }
		//bool  LandRouteExists(){ return m_pLandRouteActionOutline != NULL; }
		//bool  SeaTransportRouteExists(){ return m_pSeaTransportActionOutline != NULL; }
		//bool  AirRouteExists(){ return m_pAirActionOutline != NULL; }

		//bool   m_bSingleGeoRouteExists;
		//bool   m_bSeaRouteExists; // used by ships
		//bool   m_bLandRouteExists; // used by ground units
		//bool   m_bSeaTransportRouteExists; // used by ground units
		//bool   m_bAirRouteExists;
		//CEOSAIActionOutline*  m_pSingleGeoActionOutline;

		//CMovementPathIdeas* m_pMovementPathIdeas;
/*
		// Ground Unit routes
		CEOSAIActionOutline*  m_pLandRouteActionOutline;
		CEOSAIActionOutline*  m_pSeaTransportActionOutline;
		// Sea Unit routes
		CEOSAIActionOutline*  m_pSeaRouteActionOutline;
		// Air Unit + Air Transport routes
		CEOSAIActionOutline*  m_pAirActionOutline;
		float              m_fDirectAirDistance; // PixelDistance (minimum since MultiRegion is an area)
*/
		// Air pathway information
		//bool   m_bTargetIsWithinMyMovementRangeFromOneOfMyAirbases;

		//bool   m_bAllocatedByTaskForce;
		//CMovementPath*  m_pAllocatedMovementPath; // unowned; owned by m_MovementPathIdeas

		// Ground pathway comparisons ("Imaginary" means imaginary best-case)
		float  m_fImaginarySeaTransportDividedByGroundRoute; // 0-1 (0 means sea is best, 1 means equal)
		float  m_fImaginaryAirTransportDividedByGroundRoute; // 0-1 (0 means air is best, 1 means equal)
		float  m_fImaginaryAirTransportDividedByImaginarySeaTransport; // 0-1 (0 means air is best, 1 means equal)

		// Transport Proximity to this actor
		//float  m_fProximityToClosestTransport;

		// Current Values (updated based on the latest information)
	//	float  m_fAccessibility; // 0-1 based on distance (ignoring geo-switches)
	//	float  m_fRisk;
		float  m_fDistance;
		//
		float  m_fBuildTime; // used only if this is a hypothetical unit built by a city
		float  m_fDirectMoveToTargetTime; // Air/Sea/GroundUnit+NoTransports
		//float  m_fTransportAssistedMoveToTargetTime; // GroundUnit+Transport, assumes Transport is already at the GroundUnit
		//long   m_iTransportsRequired;
		bool   m_bTransportRequired;
		//float  m_fTransportArrivalTime; // (this is just an estimate based on CrowsFlight Distance)
		float  m_fTransportComboTime;

		//CList< CEOSAIUnitTransportMovementDesc* >  m_MultiRegionTransportMovementDescs; // owned
		//
		bool   m_bMultiRegionTransportMovementDescs_Exist;
		bool   m_bAIRegionTransportMovementDescs_Exist;
		CList< CEOSAIUnitTransportMovementDesc* >  m_UnitTransportMovementDescs; // owned
		//bool   m_bAIRegionTransportMovementDescs_Exist;
		//CList< CEOSAIUnitTransportMovementDesc* >  m_AIRegionTransportMovementDescs; // owned
		//
		CEOSAIUnitActionIdea*  m_pAITransportUnitActionIdea;

		bool   m_bFailureCondition_TargetIsNotWithinAirRange;

		// m_fSimpleTimeToTargetScore ignores combat, transport needs.  Score based on
		//   SimpleTimeToTarget, ActionDefinition.SimpleInterest, ActionDefinition.TimeFunction
		//float  m_fSimpleTimeToTargetScore;
		//float  m_fEstimatedMovementTime;
		//float  m_fTimeToTargetTimeValue;  // m_TimeValueFunction( time-to-target )

		//
		//float  m_fRelativeScoreAccordingToAIActionDefinition; // 0-1
		//float  m_fRelativeScoreAccordingToAIPoiObject; // 0-1

		//float  m_fTTTScoreAndAIPoiObjectRelativeScore;

		// Role: The Actor's role in the Action.  This might be "anti-armor firepower", 
		//   "anti-aircraft firepower", "GroundCitResHunter", "AirfieldBuilder", etc

		// There are cases where I will bring a unit along for one part of an action
		//   For example, transports have combat ability, but I might only use them for their transport capability
		//   Aircraft might play an attack role (to destroy units in a city), but cannot capture the CitRes
		CEOSAIJobRole  m_JobRole;

		//CList< CEOSAIUnitProximity* >  m_PotentialTransports;
		//CList< CEOSAIPoiObjectProximity2* >  m_PotentialTransports;

		// Pathway Ideas - sorted by value (which is based on distance, danger, transports, etc)
		//CEOSAIListSort< CEOSAIUnitPathwayIdea* >  m_PathwayIdeas;
		//CEOSAIListSort< CAIPoiObjectStateProgressionValue* >  m_PathwayIdeas;

		// Used by AIHypotheticalTaskForce
		//
			//bool   m_bUseInHypotheticalTaskForce;
			//float  m_fSimpleTaskForceValue;
			//bool   m_bRequiresCarrier;
			//bool   m_bRequiresTransport;

		// Used only if my Actor is Transport, and this is a Transport Action
		//
			EOSAI::UnitTask_Pickup*   GetAIPickupTask(){ return NULL; }//m_pAIUnitTask_Pickup; }
			EOSAI::UnitTask_Dropoff*  GetAIDropoffTask(){ return NULL; }//m_pAIUnitTask_Dropoff; }

			CEOSAITransportAssistedPath*  m_pAITransportAssistedPath;
			CEOSAIUnit*              m_pAITransportee;
			CEOSAIUnitActionIdea*    m_pAITransporteeUnitActionIdea;
		//	EOSAI::UnitTask_Pickup*   m_pAIUnitTask_Pickup;
		//	EOSAI::UnitTask_Dropoff*  m_pAIUnitTask_Dropoff;

		// Possible Targets (out of the subset of Desires in the TacticalProject)
		//
			float  m_fBestTimeToTargetDesire;
			CList< CEOSAIUnitActionIdeaTargetValue* >  m_DesireValues; // owned
			CList< CEOSAIDesireSpatial* >              m_DesireAllocations; // not owned
};

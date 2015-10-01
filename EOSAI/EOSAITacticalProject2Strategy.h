
#pragma once

#include "EOSAIJobsToDo.h"
#include "EOSAIJobCapability.h"
#include "EOSAITaskForce3.h"
class CEOSAITacticalProject2;
class CEOSAIJobsToDo;
class CEOSAIDesireSpatial;

class CEOSAITacticalProject2Strategy
{
	public:

		//                Handle PrimaryGoals, LocalResistance, PathwayResistance, OnTheWay
		// Invasion              Yes                Yes                Yes          Maybe
		// AirDrop               Yes                Yes                Maybe        No
		// AirStrike             Yes                Avoid              Avoid        No
		// Bombardment           Yes                Yes                Yes          Yes
		// AirSearchAndDestroyStrategy              Avoid              Avoid        No
		// SubmarineStrategy     Yes                Avoid              Avoid        Maybe
		enum EnumStrategy
		{
			// "Seed" Strategy is only available if there are no enemy units in the area
			//
				// Seed - Used when there is a city target, all CitResUnits are all neutral
				//    This involves capturing a single city and using that city to build units to capture rest
				//enum_SeedIsland,
				//
				//enum_SeedIslandWithAirDrop,

			// Invasion strategy variations:
			//   Must always bring enough force to deal with local enemies
			//   Seed Strategy?
			//     A "seed" strategy: capture one neutral city, build to capture others
			//     No Seed strategy: come in quickly with a bunch of force to capture entire area quickly
			//   AirDrop?
			//     Can do an AirDrop-only strategy to skip past intermediate forces, get there quickly
			//
			// Invasion - Ground/Sea Capture strategy: forces must fight through enemy - fight all Resistance
			//    Move Ground/Sea units to target (air units for support)
			//    Handle: LocalResistance, PathwayResistance, OnTheWay
			enum_Invasion,

			// Only available when the targets are ground-targets
			// Ground/Sea AirDrop strategy: air-transport over enemy - fight all local Resistance
			//    This isn't always useful.  I can't really hold-on to the target, but it can put
			//    the enemy into disarray, forcing them to draw-back forces.
			//    It's most useful when the enemy hasn't properly defended its interior.
			//    Use Air-Transport to bring Ground Units to target
			//    Handle: LocalResistance, Maybe-PathwayResistance, No-OnTheWay
			enum_InvasionWithAirDrop,

			// Air Strike: (can't control territory, but I can destroy units)
			//    Use AirUnits: Aircraft and Missiles, (Carriers, Airfields optional)
			//    Handle: Avoid-LocalResistance, Avoid-PathwayResistance, No-OnTheWay
			enum_AirStrike,

			// Bombardment: 
			//    Use Artillery, Ship Bombardment
			//    Handle: LocalResistance, PathwayResistance, OnTheWay
			enum_Bombardment,

			// Air Search And Destroy: Similar to air-strike, but target is unknown
			//    Use: Attack Aircraft, Recon, (Carriers, Airfields optional)
			//    Handle: Avoid-LocalResistance, Avoid-PathwayResistance, No-OnTheWay
			enum_AirSearchAndDestroyStrategy, 

			// Submarines (I might need to fight the fleet)
			//    Handle: Avoid-LocalResistance, Avoid-PathwayResistance, Maybe-OnTheWay
			enum_ShipStrategy,
			enum_SubmarineStrategy,
		};
		enum EnumTaskPursuit
		{
			enum_IgnoreTasks,
			enum_AvoidEnemies,
			enum_OptionalTasks,
			enum_HandleTasks,
		};
		enum EnumCurrentPlan
		{
			enum_Undefined,
			enum_ActivelyPursueThisAction, // 
			enum_ConsiderThisAction,
			enum_PlanToDoThisAction,       // Recommend the units for building
			enum_PostponeThisActionForNow, //
			enum_SkipThisActionBecauseOfForeignRelations,
			enum_Ignore, // lack sufficient forces, it belongs to my ally, or something
		};

		CEOSAITacticalProject2Strategy( CEOSAITacticalProject2* pProject, EnumStrategy eStrategy );
		~CEOSAITacticalProject2Strategy();

		//
		CEOSAITacticalProject2*  GetTacticalProject2(){ return m_pTacticalProject2; }

		void             SetPlan( EnumCurrentPlan e ){ m_eCurrentPlan = e; }
		EnumCurrentPlan  GetPlan(){ return m_eCurrentPlan; }

		void   SetBestCaseValueIndex( long i ){ m_iBestCaseValueIndex = i; }
		long   GetBestCaseValueIndex(){ return m_iBestCaseValueIndex; }

		float  GetPlannedAttackTime();

		// Debug
		//
			void  Dump( CString& strData );

		// Describes details about the Strategy
		//
			// What subtasks need to be accomplished?
			EnumTaskPursuit  HandleLocalResistance(); 
			EnumTaskPursuit  HandlePathwayResistance();
			EnumTaskPursuit  HandleOnTheWayOpportunities();
			// What units can I use?
			bool  UseOnlyAirUnits();
			bool  UseAirTransports();

		//
			void  GetDesires( CList< CEOSAIDesireSpatial* >* pDesiresList );

		//
			CEOSAITaskForce3*  GetBestCaseTaskForce(){ return &m_BestCaseTaskForce; }
			CEOSAITaskForce3*  GetNegotiatedTaskForce(){ return &m_NegotiatedTaskForce; }

		//
			void  ConstructBestCaseTaskForce();
			void  ConstructNegotiatedTaskForce();
			void  UpdateTaskForceGoalsAndJobs( CEOSAITaskForce3* pTaskForce );

		//
		// The BestCase TaskForce is based on:
		//		TacticalProject SimpleInterest,
		//		TacticalProject InitialEvalBasicScore,
		//		TacticalProject ResistancePower,
		//		TimeValue, MinNonCombatFulfillment01, RealCombatScore, AppropriateUnitScore01
		//
			float GetBestCaseTaskForceValue(){ return m_BestCaseTaskForce.GetBestCaseRealScore(); }//GetRealScore(); }//m_fBestCaseTaskForceValue; }
			float GetNegotiatedTaskForceValue(){ return m_NegotiatedTaskForce.GetRealScore(); }//m_fNegotiatedTaskForceValue; }
			//
			bool  NegotiatedTaskForceHasBeenAllocated();
			//
			void  CreatePredefinedSteps();

	private:
		//
		CEOSAITacticalProject2* m_pTacticalProject2;
		EnumStrategy         m_eStrategy;

		long  m_iBestCaseValueIndex;

		//CEOSAIJobsToDo       m_JobsToDo; // PrimaryGoals + LocalResistance (maybe) + PathwayResistance (maybe)
		//CEOSAIJobCapability  m_JobCapability; // based on current reserved units
		EnumCurrentPlan  m_eCurrentPlan;
		//float  m_fPlannedAttackTime;  // Used for coordinating units

		CEOSAITaskForce3    m_BestCaseTaskForce;
		CEOSAITaskForce3    m_NegotiatedTaskForce;

		// Results of this strategy
		// I want to get rid of this
		float  m_fSufficientForcesTime;
		float  m_fHisExpectedLosses;
		float  m_fMyExpectedLosses;

};

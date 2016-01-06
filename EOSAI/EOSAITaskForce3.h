
#pragma once

//class CAITacticalProject;
#include "EOSAIListSort.h"
#include "EOSAIUnitTemplateSet.h"
#include "EOSAIMathFunction.h"
#include "EOSAIUnitActionStack.h"
#include "EOSAIJobsToDo.h"
#include "EOSAIJobCapability.h"
#include "EOSAISettings.h"
#include "EOSAIQuickCombatCalculation.h"
#include "EOSAIQuickCombatResults.h"
#include "EOSAIPublic.h"
class CEOSAIPoiObject;
class CEOSAIJobsToDo;
//class CEOSAIUnit;
class CEOSAIPoiObject;
class CEOSAIUnit;
class CEOSAICity;
class CEOSAIUnitTemplate;
class CEOSAIUnitActionIdea;
class CEOSAICity;
//class CEOSAICityActionIdeaNode;
class CEOSAIJobCapability;
class CEOSAIUnitActionStack;
class CEOSAIBrain;
class CEOSAIMathFunction;
class CAITacticalProjectStrategy;
class CEOSAITacticalProject2;
class CEOSAITacticalProject2Strategy;

//
// This class tracks a "Needs" object, and a bunch of UnitActionIdeas
//
// HypotheticalTaskForce -
//   A set of UnitActionIdeas that can fulfill the task (no excess)
//     Tries to minimize unit cost, get the task done quickly, with low attrition
//   It does not take into consideration the other TaskForces (i.e. it can use all actors)
//   The HypotheticalTaskForce can be used to figure out what ActionDefinitions can be done
//
// AllocatedTaskForce -
//   A set of UnitActionIdeas that can fulfill the task (includes excess)
//     Tries to minimize unit cost, get the task done quickly, with low attrition
//   It must negotiate the use of actors with other TaskForces (does not get unfettered access)
//
/*
class CEOSAITaskForce3Result
{
	public:
		float  m_fJobFulfillment01;
		float  m_fCompletionTime;
};
*/

class CEOSAICityAndTime
{
	public:
		CEOSAICityAndTime()
		{
			m_pAICity = NULL;
			m_fBuildAndMoveTime = 0.0f;
		}
		CEOSAICity* m_pAICity;
		float m_fBuildAndMoveTime;
};
/*
class CTimeAndRealScore
{
	public:
		CTimeAndRealScore(){ m_fTime = 0.0f; m_fRealScore = 0.0f; }
		CTimeAndRealScore( float fTime, float fRealScore ){ m_fTime = fTime; m_fRealScore = fRealScore; }
		float  m_fTime;
		float  m_fRealScore;
};
*/
class CEOSAITaskForce3
{
	public:

		//enum EnumTaskForceType
		enum EnumBestCaseOrRealTaskForce
		{
			EnumBestCaseOrRealTaskForce_Undefined,//EnumTaskForceType_Undefined,
			BestCaseTaskForce, //EnumTaskForceType_BestCaseScenario, // don't negotiate for Units, assume I can use all of them
			RealTaskForce // EnumTaskForceType_NegotiateForUnits,
		};
		enum EnumUnitNegotiation
		{
			BestCaseUnits, // Ignore existing allocations, Allocate whoever I want
			AllocateOnlyUnallocatedUnits,
			//NegotiateForUnits,
		};
		enum EnumTransportStrategy
		{
			GroundUnitArrivalASAP,
			//GroundUnitSimultaneousArrival,
		};
		/*
		enum EnumTaskForceStrategy
		{
			EnumTaskForceStrategy_SecretInvasion,
			EnumTaskForceStrategy_UnitsASAP,
			EnumTaskForceStrategy_Seed,
		};
		*/
		/*
		enum EnumCombatStrategy
		{
			EnumCombatStrategy_Seed,         // Bring 1+ units to start capturing and building, arrive ASAP
			EnumCombatStrategy_Reinforcement,// Bring as many units as possible, arrive ASAP
			EnumCombatStrategy_Invasion,     // Full Combat Numbers, Arrive Simultaneously/Secretly
		};
		*/

	public:

		// Construction / Initialization
		//
			CEOSAITaskForce3();
			~CEOSAITaskForce3();

			bool IsBeingDeleted(){ return m_bIsBeingDeleted; }

		// Initialization
		//
			void                 SetTacticalProject2( CEOSAITacticalProject2* pTacticalProject2 ){ m_pTacticalProject2 = pTacticalProject2; }
			CEOSAITacticalProject2* GetTacticalProject2(){ return m_pTacticalProject2; }
			void                         SetTacticalProject2Strategy( CEOSAITacticalProject2Strategy* pTacticalProject2Strategy ){ m_pTacticalProject2Strategy = pTacticalProject2Strategy; }
			CEOSAITacticalProject2Strategy* GetTacticalProject2Strategy(){ return m_pTacticalProject2Strategy; }

			// BestCase, Negotiated
			void                         SetBestCaseOrRealTaskForce( EnumBestCaseOrRealTaskForce eBestCaseOrReal );
			EnumBestCaseOrRealTaskForce  GetBestCaseOrRealTaskForce(){ return m_eBestCaseOrRealTaskForce; }

			//void  SetStrategy( EnumTaskForceStrategy eStrategy );

			// If this is a big area, more CitResHunters is better
			//void  SetCombatStrategy_Seed( float fTargets ){ m_eCombatStrategy = EnumCombatStrategy_Seed; }
			//void  SetCombatStrategy_Invasion(){ m_eCombatStrategy = EnumCombatStrategy_Invasion; }
			//void  SetCombatStrategy_Reinforcements(){ m_eCombatStrategy = EnumCombatStrategy_Reinforcement; }

			// BestCase, Unallocated Only, Negotiate for units
			//void  SetUnitNegotiation( EnumUnitNegotiation eNegotiation );
			//void  SetUnitNegotiation( EnumUnitNegotiation eNegotiation ){ m_eUnitNegotiation = eNegotiation; }
			//void               SetTransportStrategy( EnumTransportStrategy eStrategy ){ m_eTransportStrategy = eStrategy; }

			// Filter Units
			void  UseOnlyAirUnits( bool b ){ m_bUseOnlyAirUnits = b; } // Air-strikes
			void  UseAirTransports( bool b ){ m_bUseAirTransports = b; } // AirTransport strategy

			CEOSAIBrain*   GetAIBrain();
			CEOSAIJobsToDo*      GetJobsToDo(){ return &m_JobsToDo; }
			CEOSAIJobCapability* GetJobCapabilityOfAllocatedUnits(){ return &m_JobCapabilityOfAllocatedUnits; }

			float GetEstimatedNumberOfUnitsNeeded();

		// Stack
		//
			void  CopyUnitActionIdeasIntoStack();
			long  GetNumberOfAllocatedUnitsAndCities();
			void  ReleaseStackAllocations(){ m_bNeedToUpdateScores = true; m_UnitActionStack.ReleaseAllAllocations(); }

		// Create
		//
			// I want to get the "best satisfaction".  What is that?
			//   (1) All jobs must be satisfied
			//   (2) Get jobs done quickly (sooner is better than later) (At N turns, value might drop to 0)
			//   (3) Low attrition
			//   (4) Perferably bring a little extra combat force (just to make sure I win)
			//   (5) If I build something, it would be better if I can reuse later for other tasks
			// How do I accomplish this?  I need a way to balance these things against each other.
			void  ConstructTaskForce();

			// Done only if I'm going ahead with this TaskForce
			void  CreateTransportUnitActionIdeaFromSuggestedTransportMoveDescs();
		//
		// UnitActionStack
		//
			CEOSAIUnitActionStack*  GetUnitActionStack(){ return &m_UnitActionStack; }

		// Result
		//   Note: The task force is allowed to negotiate for extra units - like transports
		//
		// GrowthScore = (Points for all useful units)
		//    based on:
		//       Do I have a good arrival time?
		//       Do I have sufficient units for the non-combat tasks?
		//       Do I have sufficient units to win combat? (known+unknown enemies)
		//       Do I have a good attrition?
		// RealScore = 
		//    if MinSatisfaction < 1.0 = 0.0 points 
		//    else based on:
		//       Do I have a good arrival time?
		//       Do I have sufficient units for the non-combat tasks?
		//       Do I have sufficient units to win combat? (known+unknown enemies)
		//       Do I have a good attrition?
		//
			void  UpdateScoresIfNecessary();
			void  CalculateScores();
				void  CalculateCombatSuccess();
				float GetGrowthCombatScore();
				float GetRealCombatScore();
			float GetGrowthScore(){ UpdateScoresIfNecessary(); return m_fGrowthScore; } // Gives points for half-fulfilled tasks
			float GetRealScore(){ UpdateScoresIfNecessary(); return m_fRealScore; } // Gives no points for half-fulfilled tasks
			float GetBestCaseRealScore(){ return m_fBestCaseRealScore; }

		// Record/Restore Current Scores
		//
			void RecordCurrentScores();
			void RestoreOldScores();

		// Schedule
		//
			// This involves matching up units with targets, and also determining the 
			//   best direction of approach (head-on?  flank?)
			void  CreatePredefinedSteps();

			//float GetCompletionTime(){ return m_fCompletionTime; }
			//float GetShoreArrivalTime(){ return m_fShoreArrivalTime; }

			void  SynchronizeInvasionUsingRedPath();

	private:

		void  AllocateUnitsInStack();

		// Complications:
		//   One city can produce multiple units (important for when the AI starts with no units, one city, needs transport+infantry)
		//     When a city builds something, all other build possibilities get pushed to later
		//   Need a way to rearrange city+build groups.  For example, if I need a transport+infantry,
		//     do I want City1:Infantry, City2:Transport OR City1:Transport, City2:Infantry?
		//     I want the better city to build the more-expensive unit (so the complete group is earlier)
		void  TryInsertingBuildActionsIntoStack();
		bool  TryInsertingOneBuildActionIntoStack();
		void  GetCityBuildArrivalTimes( CEOSAIUnitTemplate* pUnitTemplate, CList< CEOSAICityAndTime >* pCityUnitTemplateTimeList );

		void  SimpleCalc_SetSuggestedTransport( CEOSAIUnitActionStackItem* pUnitStackItem );//CEOSAIUnitActionIdea* pUnitActionIdea );
		void  OptimizeTransportAndGroundUnitMatchup();

		//
		//
			bool  m_bIsBeingDeleted;

		// Parent TacticalProject
		//
			CEOSAITacticalProject2*         m_pTacticalProject2;
			CEOSAITacticalProject2Strategy* m_pTacticalProject2Strategy;

			// Limits on Units that I can use
			bool  m_bUseOnlyAirUnits;  // Air-strikes, Air SearchAndDestroy
			bool  m_bUseAirTransports; // AirTransport strategy

		//
			EnumBestCaseOrRealTaskForce  m_eBestCaseOrRealTaskForce; // BestCase or Negotiated
			//EnumCombatStrategy           m_eCombatStrategy;  // Seed, Invasion, Reinforcement, etc
			//EnumTaskForceStrategy        m_eTaskForceStrategy;

		// Used by Invasions
		//
			bool  m_PlayersToTreatAsEnemies[ EOSAI_MAX_NUMBER_OF_PLAYERS+1 ];

			void  CalculateEnemyUnitCount();
			long  m_iNumberOfPlayer0Units; // Calculated
			long  m_iNumberOfPlayerNUnits; // Calculated
			CList< CEOSAIUnit* >  m_TargetUnitsList;  // All PlayerN units + one Player0 units
			//CList< CUnit* >  m_MinimalTargetUnitsList;  // All PlayerN units + one Player0 units
			//CList< CUnit* >  m_CompleteTargetUnitsList; // All PlayerN units + all Player0 units

		// Unit Action Stack
		//
			CEOSAIUnitActionStack  m_UnitActionStack;

			// Scoring Functions
			CEOSAIMathFunction  m_TimeValueFunction;
			CEOSAIMathFunction  m_FulfillmentToScoreFunc; // Need to add

		// Current TaskForce State - (not currently used, but is important for finding error conditions and during negotiations)
		//
			CEOSAIJobsToDo       m_JobsToDo;
			CEOSAIJobCapability  m_JobCapabilityOfAllocatedUnits; // Capability of allocated units

		// Scores
		//
			bool  m_bNeedToUpdateScores;

			// Methods for judging the value of a unit or task-force:
			// - Unit arrival time.  (If I'm doing a coordinated attack, then early arrivals might not be more valuable)
			// - Task Completion time.
			// - Using appropriate units; based on attrition, current HP, other potential tasks
			//      This can also be keyed off of UnitTemplate types.
			// - Task forces need to end-up with good fulfillment. Lesser fulfillment is okay for seed strategies.
			//      Overfulfillment is bad since I'm not using those units elsewhere.
			//      Task Forces may also want to add excess capacity to account for 
			//        random combat and reinforcements.

			// Combat Fulfillment
			bool  GetTaskInvolvesCombat(){ return m_JobsToDo.m_EnemyUnits.IsEmpty() == FALSE; }

			float m_fCompletionTime;
			//float m_fShoreArrivalTime;

			float m_fGrowthScore; // Gives points for half-fulfilled tasks
			float m_fRealScore;   // Gives no points for half-fulfilled tasks
			float m_fBestCaseRealScore;

		// Combat Calculation
		//
			void  GetValueOfAddingAttackerToCombatForce( CEOSAIUnitTemplate* pUnitTemplate, CEOSAIQuickCombatResults* pNewResults );
			CEOSAIQuickCombatCalculation  m_CombatCalculation;
			CEOSAIQuickCombatResults      m_CombatResultsOfAllocatedUnits;
			//CEOSAIQuickCombatResults  m_SeedCombatResults;

		// NonCombat Fulfillment
		//
			float m_fMinimumNonCombatFulfillment02; // 0.0 = no fulfillment, 1.0 = full fulfillment
			float m_fAverageNonCombatFulfillment01; // 0.0 = no fulfillment, 1.0 = full fulfillment

		// Record/Restore
		//
			CEOSAIQuickCombatResults  m_PrevCombatResults;
			//CEOSAIQuickCombatResults  m_PrevSeedCombatResults;
			float m_fPrevGrowthScore; // Gives points for half-fulfilled tasks
			float m_fPrevRealScore;   // Gives no points for half-fulfilled tasks
};

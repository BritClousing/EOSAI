
#pragma once
class CEOSAIDesireSpatial;
class CEOSAIBrain;
class CEOSAITacticalProject2Strategy;
namespace EOSAI
{
	class AIPlayer;
	class UnitPathwayResultStep;
};
class CEOSAIIntSet;
class CEOSAIUnitTemplate;
class CEOSAIUnit2ActionIdea;
class CEOSAIMultiRegion2;
class CEOSAIPoiObjectProximity;
class CEOSAITaskForce3;
#include "EOSAIJobsToDo.h"
#include "EOSAILocation.h"
//namespace EOSAI{};
//class EOSAI::CActionScheduler;
namespace EOSAI
{
class CActionScheduler;
};

/*
I need to break-out the tasks into types.  This way, I can do things like send submarines to a warzone
even if no "submarine-warzone" tasks rise to the level of being in the list based on InitialEvaluation score.

Big Targets First: I should group some TacticalProjects by their size because I want to do big ones first.
Value Targets: I should also go after easy, valuable targets.  Based on the value-to-resistance ratio.
Easy Targets: Targets that have little or no resistance (unowned CitRes, weak enemy units, etc)
Urgent Targets: Incoming transports/invasions.

I should be able to swap units to other tactical-projects, in case I find a good trade-off.
I should be able to add units to existing tactical-projects because I've got idle units around.

BigTarget = 301+ Resistance
300ResistanceTarget = 51-300 Resistance
50ResistanceTarget = 1-50 Resistance (1-2 militia or infantry)
0ResistanceTarget = 0 Resistance (sea-resources, small islands)

CaptureArea - Ground, Sea
DegradeCity - Ground (especially if the ruleset has long-range units), Sea, Air
DestroyUnits - Ground, Sea, Air
SubmarineWarzone - Submarines only
ShipWarzone - Ships only
ProtectArea - Ground, Sea, Air

Up next:
A scoring system for units doing jobs in a tactical project
  - This is based on: unit attrition, damage, needs-upgrade, and the unit's other options.
Using Idle Units - like aircraft
A way to add additional units to a TacticalProject and a value curve
A way to consume idle units, perhaps by picking-up new TacticalProjects
  Example: I have 4 idle submarines.  What TacticalProjects can be fulfilled with these units?
*/

// Sometimes there are desires that are important or semi-important to the project
//   Example: if invading an island, it might be important to destroy enemy sea-units in the area 
class CAIDesireImportance
{
	public:
		CAIDesireImportance()
		{
			m_fImportanceToProject01 = 0.0f;
			m_pAIDesire = NULL;
		}

		float m_fImportanceToProject01;
		CEOSAIDesireSpatial* m_pAIDesire;
};

class CEOSAITacticalProject2
{
	public:
		//enum EnumEvaluationLevel
		//{
		//	enum_NoCalculations = 0,
		//	enum_DistanceFromBordersAndCombatPower = 1,
		//	enum_BestCaseTaskForce = 2,
		//	enum_NegotiatedTaskForce = 3,
		//};

		enum EnumPrimaryAction
		{
			enum_PrimaryActionUndefined,
			enum_CaptureArea,        // Capture CitRes, (secondary: degrade cities, destroy units)
			enum_DegradeCity,        // Use bombers, missiles, etc to degrade cities (airfield?); 1 desire
			enum_DestroyUnits,       // Destroy an incoming force (naval fleet, bombers, ground units)

			enum_SubmarineWarzone, // Submarines search-and-destroy easy targets
			                       //    Even though the AI knows where everything is, 
								   //    he should position subs in enemy territory
			enum_ShipWarzone, // Submarines search-and-destroy easy targets
			                  //    Even though the AI knows where everything is, 
							  //    he should position subs in enemy territory
			//enum_StrategicBombing, // Bombers search-and-destroy easy targets
			enum_ProtectArea,      // (always on) Prevent my stuff from being captured/bombed (defined or undefined enemies)
		};
		enum EnumCurrentPlan
		{
			enum_Undefined,
			enum_ActivelyPursueThisAction, // 
			enum_ConsiderThisAction,       // Try to allocate any unallocated units
			enum_PlanToDoThisAction,       // Recommend the units for building
			enum_PostponeThisActionForNow, //
			enum_SkipThisActionBecauseOfForeignRelations,
			enum_Ignore, // lack sufficient forces, it belongs to my ally, or something
		};
		/*
		enum EnumTaskForceStrategy
		{
			EnumTaskForceStrategy_SecretInvasion, // Simultaneous arrival, lots of units
			EnumTaskForceStrategy_UnitsASAP, // lots of units
			EnumTaskForceStrategy_Seed, // 1+ units
		};
		*/


		CEOSAITacticalProject2( CEOSAIBrain* pAIBrain );
		~CEOSAITacticalProject2(){ m_bDebug_HasBeenDeleted = true; }

		CEOSAIBrain*   GetAIBrain(){ return m_pAIBrain; }
		EOSAI::AIPlayer*  GetAIPlayer();
		long               GetAIPlayerNumber();

		void  SetId( long id ){ m_Id = id; } // Used for debugging
		long  GetId(){ return m_Id; } // Used for debugging

		void       AddLocation( CEOSAILocation Location ){ m_Locations.AddTail( Location ); }
		CEOSAILocation  GetLocation(){ return m_Locations.GetHead(); }
		CEOSAILocation  CalculateCenterLocation();
		long  GetTargetGeo(){ return m_iTargetGeo; }

		void  IsASecretInvasion( bool b ){ m_bIsASecretInvasion = b; }
		bool  IsASecretInvasion(){ return m_bIsASecretInvasion; }

		EnumPrimaryAction  GetPrimaryAction(){ return m_ePrimaryAction; }
		//EnumTaskForceStrategy  GetTaskForceStrategy(){ return m_eTaskForceStrategy; }

		//
		// Debug
		//
			virtual void Dump( CString& strData );

		// Results
		//
			void             SetPlan( EnumCurrentPlan e ){ m_eCurrentPlan = e; }
			EnumCurrentPlan  GetPlan(){ return m_eCurrentPlan; }

		//
		// Desires
		//
			void  GetUnitTargets( CList< CEOSAIDesireSpatial* >* pList );

			void  GetDesires( CList< CEOSAIDesireSpatial* >* pList );
			CList< CEOSAIDesireSpatial* >* GetPrimaryGoals(){ return &m_PrimaryGoals; }
			CList< CEOSAIDesireSpatial* >* GetLocalResistance(){ return &m_LocalResistance; }
			CList< CEOSAIDesireSpatial* >* GetPathwayResistance(){ return &m_PathwayResistance; }
			CList< CEOSAIDesireSpatial* >* GetOnTheWayOpportunities(){ return &m_OnTheWayOpportunities; }

			bool  IsPrimaryGoal( CEOSAIDesireSpatial* pDesire );
			bool  IsLocalResistance( CEOSAIDesireSpatial* pDesire );
			bool  IsPathwayResistance( CEOSAIDesireSpatial* pDesire );
			bool  IsOnTheWayOpportunity( CEOSAIDesireSpatial* pDesire );

			float GetClosestPrimaryGoal( CEOSAILocation Location, CEOSAIDesireSpatial** pOutClosestDesire = NULL );
			CEOSAILocation GetClosestPrimaryTargetLocation( CEOSAILocation Location );
			float     GetClosestPrimaryTargetDistance( CEOSAILocation Location );

			// AIJobsToDo
			CEOSAIJobsToDo*  GetPrimaryGoals_JobsToDo(){ return &m_PrimaryGoals_JobsToDo; }
			CEOSAIJobsToDo*  GetLocalResistance_JobsToDo(){ return &m_LocalResistance_JobsToDo; }
			CEOSAIJobsToDo*  GetPathwayResistance_JobsToDo(){ return &m_PathwayResistance_JobsToDo; }
			CEOSAIJobsToDo*  GetOnTheWayOpportunities_JobsToDo(){ return &m_OnTheWayOpportunities_JobsToDo; }

		//
		// Initial Evaluation
		//
			void  CalculateInitialEvaluation(); // National Pathway, Resistance, Distance, etc

			void  SetSimpleInterest( float f ){ m_fSimpleInterest = f; }
			float GetSimpleInterest(){ return m_fSimpleInterest; }
			float GetUrgency01(){ return m_fUrgency01; }
			float GetResistanceCombatSignificance(){ return m_fResistanceCombatSignificance; }
			float GetOrderOfProcessing(){ return m_fOrderOfProcessing; }
			void  SetOrderOfProcessingIndex( long i ){ m_iOrderOfProcessingIndex = i; }
			long  GetOrderOfProcessingIndex(){ return m_iOrderOfProcessingIndex; }

			float GetDistanceFromMyCitResToPrimaryGoals(){ return m_fDistanceFromMyCitResToPrimaryGoals; }
			float GetDistanceFromMyCitResUnitsToPrimaryGoals(){ return m_fDistanceFromMyCitResUnitsToPrimaryGoals; }
			float GetDistanceFromNeutralPlayerCitResToPrimaryGoals(){ return m_fDistanceFromNeutralPlayerCitResToPrimaryGoals; }
			float GetDistanceFromEnemyPlayerCitResToPrimaryGoals(){ return m_fDistanceFromEnemyPlayerCitResToPrimaryGoals; }

			float GetInitialEvaluation_NearbyScore(){ return m_fInitialEvaluation_NearbyScore; }
			float GetInitialEvaluation_ValueScore(){ return m_fInitialEvaluation_ValueScore; }
			float GetInitialEvaluation_BasicScore(){ return m_fInitialEvaluation_BasicScore; }
			float GetInitialEvaluation_EasyTargetScore(){ return m_fInitialEvaluation_EasyTargetScore; }
			float GetInitialEvaluation_NoResistanceScore(){ return m_fInitialEvaluation_NoResistanceScore; }

			void  SetInitialEvaluation_NearbyScoreIndex( long i ){ m_iInitialEvaluation_NearbyScoreIndex = i; }
			long  GetInitialEvaluation_NearbyScoreIndex(){ return m_iInitialEvaluation_NearbyScoreIndex; }
			void  SetInitialEvaluation_ValueScoreIndex( long i ){ m_iInitialEvaluation_ValueScoreIndex = i; }
			long  GetInitialEvaluation_ValueScoreIndex(){ return m_iInitialEvaluation_ValueScoreIndex; }
			void  SetInitialEvaluation_BasicScoreIndex( long i ){ m_iInitialEvaluation_BasicScoreIndex = i; }
			long  GetInitialEvaluation_BasicScoreIndex(){ return m_iInitialEvaluation_BasicScoreIndex; }
			void  SetInitialEvaluation_EasyTargetScoreIndex( long i ){ m_iInitialEvaluation_EasyTargetScoreIndex = i; }
			long  GetInitialEvaluation_EasyTargetScoreIndex(){ return m_iInitialEvaluation_EasyTargetScoreIndex; }
			void  SetInitialEvaluation_NoResistanceScoreIndex( long i ){ m_iInitialEvaluation_NoResistanceScoreIndex = i; }
			long  GetInitialEvaluation_NoResistanceScoreIndex(){ return m_iInitialEvaluation_NoResistanceScoreIndex ; }

			//float GetInitialEvaluationScore(){ ASSERT( m_bInitialEvaluationDone ); return m_fInitialEvaluationScore; }
			//float GetInitialEvaluationScoreIgnoreResistance(){ ASSERT( m_bInitialEvaluationDone ); return m_fInitialEvaluationScoreIgnoreResistance; }
			float GetNationalPathwayTimeToTarget(){ ASSERT( m_bInitialEvaluationDone ); return m_fNationalPathwayTimeToTarget; }

			long  GetNumberOfCitiesOnTheSameGeo(){ return m_iNumberOfMyCitiesOnTheSameGeo; }
			bool  InvolvesGroundTargets(); // Cities, Land Res, Ground Units (use this for AirDrop ideas)
			bool  RequiresATransport(){ return m_iNumberOfMyCitiesOnTheSameGeo == 0; }

			CList< CEOSAIMultiRegion2* >* GetNationwideMultiRegionPathway(){ return &m_NationwideMultiRegionPathway; }

			// Who is this action hostile against?
			void  GetActionIsHostileAgainstPlayers( CEOSAIIntSet* pSet );
			bool  PrimaryGoalsAreHostileActionAgainstAnotherPlayer();
			bool  PrimaryGoalsAreHostileActionAgainstPlayerN( long iOtherPlayer );
			bool  CurrentForeignRelationsPreventsPersuingThisTacticalProject();

			bool  GetShouldBeInAllocateTacticalProjectList( long iMaximumValueIndex );
			void  IsInsideTacticalProjectsList( bool b ){ m_bIsInsideTacticalProjectsList = b; }
			bool  IsInsideTacticalProjectsList(){ return m_bIsInsideTacticalProjectsList; }

		//
		// Strategies and UnitActionIdeas
		//
			void  CreateStrategiesAndUnitActionIdeas();
			void  CreateBestCaseTaskForce();

			//void AddUnitActionIdea( CEOSAIUnit2ActionIdea* pUnitActionIdea );
			//void RemoveUnitActionIdea( CEOSAIUnit2ActionIdea* pUnitActionIdea );
			CList< CEOSAIUnitTemplate* >*  GetUsefulAvailableUnitTemplates(){ return &m_UsefulAvailableUnitTemplates; }
			CList< CEOSAIUnitTemplate* >*  GetUsefulBuildableUnitTemplates(){ return &m_UsefulBuildableUnitTemplates; }
			CList< CEOSAIUnit2ActionIdea* >*  GetUnitActionIdeas(){ return &m_UnitActionIdeas; } // CitResHunters, AntiGroundCombat, ...

			private:
			// Action Scheduler only
			friend class EOSAI::CActionScheduler;
			void   ActionScheduler_SetPrimaryAction( EnumPrimaryAction eType );
			void   ActionScheduler_AddPrimaryGoal( CEOSAIDesireSpatial* pSpatialDesire );
			void   ActionScheduler_AddNearbyRelatedDesires( CEOSAIDesireSpatial* pDesire );
			void   ActionScheduler_AddAsLocalResistance( CEOSAIDesireSpatial* pDesire );
			void   ActionScheduler_AddAsPathwayResistance( CEOSAIDesireSpatial* pDesire );
			void   ActionScheduler_AddAsOnTheWayOpportunity( CEOSAIDesireSpatial* pDesire );
			bool   GetFlag_AddOnTheWayOpportunities()
			{
				if( m_ePrimaryAction == enum_CaptureArea ){ return true; } return false;
			}
			public:

			// Best Strategy
			CEOSAITacticalProject2Strategy*  GetBestStrategy();
			CEOSAITacticalProject2Strategy*  GetAllocatedStrategy();
			bool  NegotiatedTaskForceHasBeenAllocated();

		//
		// Strategies
		//
			void  GetAITacticalProjectStrategies( CList< CEOSAITacticalProject2Strategy* >* pStrategyList );

		//
		// Data from the TaskForce
		//
			void           SetTaskForceWithPredefinedSteps( CEOSAITaskForce3* p ){ m_pTaskForceWithPredefinedSteps = p; }
			CEOSAITaskForce3* GetTaskForceWithPredefinedSteps(){ return m_pTaskForceWithPredefinedSteps; }
		//
		// Arrival Time information (used for simultaneous arrival)
		//
			void  AddToResultSteps( EOSAI::UnitPathwayResultStep* pResultStep ){ m_ResultSteps.AddTail( pResultStep ); }
			CList< EOSAI::UnitPathwayResultStep* >*  GetResultSteps(){ return &m_ResultSteps; }
			void  AdjustWaitTimeForSimultaneousAttack();

		//
		// Debug
		//
			bool  ProjectIncludesObjectIdAsPrimaryTask( long iObjectId );

	public:
			bool m_bDebug_HasBeenDeleted;
	private:

		void CalculateNationalPathway();
		void CalculateExpectedPathwayResistance_UsingNationalPathway(); // Uses National-pathway
		void CalculatePathwayDistances_UsingNationalPathway();

		void CalculateProductionCostOfUnitNeeds();

		long m_Id; // Used for debugging
		CEOSAIBrain*  m_pAIBrain;
		EnumPrimaryAction  m_ePrimaryAction;
		EnumCurrentPlan    m_eCurrentPlan;
		//EnumTaskForceStrategy  m_eTaskForceStrategy;

		bool  m_bIsASecretInvasion;

		//
		CList< CEOSAILocation >  m_Locations; // This can be a multiregion, or a series of PrimaryTarget locations

		//
		// Desires
		//
			CList< CEOSAIDesireSpatial* >  m_PrimaryGoals;
			CList< CEOSAIDesireSpatial* >  m_LocalResistance;   // Near target location
			CList< CEOSAIDesireSpatial* >  m_PathwayResistance; // May or may not be a problem
			CList< CEOSAIDesireSpatial* >  m_OnTheWayOpportunities;

			// AIJobsToDo
			CEOSAIJobsToDo  m_PrimaryGoals_JobsToDo;
			CEOSAIJobsToDo  m_LocalResistance_JobsToDo;
			CEOSAIJobsToDo  m_PathwayResistance_JobsToDo;
			CEOSAIJobsToDo  m_OnTheWayOpportunities_JobsToDo;

		// Initial Evaluation
		//
			// NationalPathway information
			bool   m_bInitialEvaluationDone;

			float  m_fSimpleInterest; // Based on PrimaryGoals and Local Resistance
			float  m_fUrgency01; // 0.0 = do this whenever I want, 1.0 = Do it immediately (e.g. incoming transport)
			float  m_fOrderOfProcessing;
			long   m_iOrderOfProcessingIndex;

			long   m_iTargetGeo;
			float  m_fTargetGroundSeaAttackAccess01; // AIPlayer's Ground-Sea Attack Access
			float  m_fTargetAirAttackAccess01;       // AIPlayer's Air Attack Access
			float  m_fDistanceFromMyCitResToPrimaryGoals;
			float  m_fDistanceFromMyCitResUnitsToPrimaryGoals;
			float  m_fDistanceFromNeutralPlayerCitResToPrimaryGoals;
			float  m_fDistanceFromEnemyPlayerCitResToPrimaryGoals;
			float  m_fNationalPathwayTimeToTarget;

			// These influence the initial evaluation score
			long   m_iNumberOfMyCitiesOnTheSameGeo;
			bool   m_bSameGeoAsMyResources;
			bool   m_bSameGeoAsMyGroundUnits;

			CList< CEOSAIMultiRegion2* >  m_NationwideMultiRegionPathway;
			CList< CEOSAIPoiObjectProximity* >  m_PoiObjectsNearNationwidePathway;

			CList< CEOSAIUnitTemplate* >  m_UsefulAvailableUnitTemplates;
			CList< CEOSAIUnitTemplate* >  m_UsefulBuildableUnitTemplates;

			float  m_fResistanceCombatSignificance;

			float  m_fInitialEvaluation_NearbyScore;
			float  m_fInitialEvaluation_ValueScore;
			float  m_fInitialEvaluation_BasicScore;
			float  m_fInitialEvaluation_EasyTargetScore;
			float  m_fInitialEvaluation_NoResistanceScore;

			long   m_iInitialEvaluation_NearbyScoreIndex;
			long   m_iInitialEvaluation_ValueScoreIndex;
			long   m_iInitialEvaluation_BasicScoreIndex;
			long   m_iInitialEvaluation_EasyTargetScoreIndex;
			long   m_iInitialEvaluation_NoResistanceScoreIndex;

		//
		// UnitActionIdeas
		//
			CList< CEOSAIUnit2ActionIdea* >  m_UnitActionIdeas;

			// Strategies
			CEOSAITacticalProject2Strategy*  m_pInvasionStrategy;  // Fight all expected resistance
			CEOSAITacticalProject2Strategy*  m_pAirDropStrategy;   // Fight local resistance
			CEOSAITacticalProject2Strategy*  m_pAirStrikeStrategy; // Dodge expected resistance, hit target
			CEOSAITacticalProject2Strategy*  m_pBombardmentStrategy; // Dodge expected resistance, hit target
			CEOSAITacticalProject2Strategy*  m_pAirSearchAndDestroyStrategy; // Dodge expected resistance, hit target
			CEOSAITacticalProject2Strategy*  m_pSubmarineStrategy; // Dodge expected resistance, hit target

			bool  m_bIsInsideTacticalProjectsList;

		//
			CList< EOSAI::UnitPathwayResultStep* > m_ResultSteps; // used for simultaneous attacks, not owned
			CEOSAITaskForce3*  m_pTaskForceWithPredefinedSteps; // not owned

			//float  m_fTaskForceCompletionTime;
};

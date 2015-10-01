
#pragma once

#include "EOSAIListSort.h"
class CAITacticalProject;
class CEOSAIBrain;
class CEOSAIThoughtDatabase;
class CEOSAIDesireSpatial;
class CEOSAITaskForce3;

//
namespace EOSAI
{

class CActionSchedulerCheckpoint
{
	public:
		CActionSchedulerCheckpoint(){}
};

class CActionScheduler
{
	public:
		CActionScheduler();
		// Initialization
		void  SetAIBrain( CEOSAIBrain* pAIBrain ){ m_pAIBrain = pAIBrain; }
		// Information
		CEOSAIThoughtDatabase*  GetAIThoughtDatabase();
		CEOSAIListSort< CEOSAIDesireSpatial* >*  GetDesiresSortedByScore();

		//void  CreateTacticalProjects();
		//void  AllocateTacticalProjects();

		void  CreateTacticalProjects2();
		void  AllocateTacticalProjects2();

		void  CreateRepairNearbyTaskOrWarzoneForIdleUnits();
		void  AddUpgradePredefinedStepToAllUnitsWhoNeedAnUpgradeAndWithin100Pixels();
		//void  AllocateTaskForceUnits( CEOSAITaskForce3* pTaskForce );
		//void  ScheduleTaskForce( CEOSAITaskForce3* pTaskForce );

		//
		//void  AddDesireToTacticalProject( CAITacticalProject* pTacticalProject, CEOSAIDesireSpatial* pDesire );
		//void  AddNearbyRelatedDesiresToTacticalProject( CAITacticalProject* pTacticalProject );
		//void  AddDesireAndRelatedDesiresToTacticalProject( CAITacticalProject* pTacticalProject, CEOSAIDesireSpatial* pDesire );

		//
		//
		// Create a hypothetical list of units to accomplish the action
		//   This can include city production actions, and completion time estimates
		//void  CreateHypotheticalTaskForceForAllTacticalProjects();

		// This function is influenced by the existing actions (and the ability to get
		//   the actors that it needs to complete the task relatively soon)
		float GetCurrentValueAndFeasibility( CAITacticalProject* pTacticalProject );

		// Involves negotiation with existing actions to gain control of actors
		void  ScheduleTacticalProject( CAITacticalProject* pTacticalProject );
		void  UnscheduleTacticalProject( CAITacticalProject* pTacticalProject );

		// Results
		//
			// Score of the entire schedule
			//float  GetCurrentScore();
			float  GetAllocatedUnitsAndCities01();

	private:

		CEOSAIBrain* m_pAIBrain;

		// Values - these are used when scoring AIActionDefinition solutions.
		//   Do I want to send a bunch of aircraft (which will cost me attrition), or slow things
		//   down and send a bunch of ground units?  
		// If the AI has a big advantage over the other players, then the attrition value is lower.
		// If the other player has a production advantage, then time becomes more important.
		//
		float  m_fAttritionValue; // 0.5 = don't worry too much about my attrition, 1.5 worry about it.
		float  m_fTimeValue;      // 0.5 = don't worry about doing things quickly

		// Result
		//long   m_iNumberOfMyUnitsAndCities;
		//long   m_iUnitsAndCitiesAllocatedToTacticalProjects;
		//long   m_iNumberOfAllocatedTacticalProjects;
		//float  m_fInsufficientTacticalProjectsUnderCurrentForeignRelations01;
};
};


#pragma once

//
// AIDesires are a low-level description of things the AI would like to accomplish
//   AIDesires can be fulfilled by AIActionDefinitions (which might fulfill several at the same time)
// AIDesires contain information on how to be fulfilled.
//

#include "EOSAILocation.h"
#include "EOSAITaskForceDesc.h"
#include "EOSAIMathFunction.h"
#include "EOSAIJobsToDo.h"
class CUnitTemplate;
class CPoiObject;
class CEOSAIBrain;
class CUnit;
class CEOSAIThoughtDatabase;
class CAITacticalProject;
class CEOSAITacticalProject2;

class CEOSAIDesire2
{
	public:
		enum EnumScoringLevel
		{
			EnumUndefined,
			EnumInitialScoring, // Use nationwide pathways for distance, resistance, accessibility
		};
		/*
		enum EnumResistanceCalculation
		{
			EnumUndefined,
			EnumNationwidePathway, // Use nationwide pathways for distance, resistance, accessibility
		};
		*/

	public:
		CEOSAIDesire2();
		virtual ~CEOSAIDesire2(){}

		void      SetAIBrain( CEOSAIBrain* pAIBrain ){ m_pAIBrain = pAIBrain; }
		CEOSAIBrain* GetAIBrain(){ return m_pAIBrain; }

		// Some desires (destroy unit) can be part of multiple (unscheduled) TacticalProjects
		// Other desires (capture city) should be part of only one (unscheduled) TacticalProject
		bool  IsAPrimaryGoalOfTacticalProject( CAITacticalProject* p );
		bool  IsAPrimaryGoalOfOneOrMoreTacticalProjects();
		void  AddAsPrimaryGoalOfTacticalProject( CAITacticalProject* p );
		//void  RemoveAsPrimaryGoalOfTacticalProject( CAITacticalProject* p );

		bool  IsAPrimaryGoalOfTacticalProject2( CEOSAITacticalProject2* p );
		bool  IsAPrimaryGoalOfOneOrMoreTacticalProjects2();
		void  AddAsPrimaryGoalOfTacticalProject2( CEOSAITacticalProject2* p );

		//
		// ForeignRelations complications
		//   (I want to track desires against neutral players - to influence my desire to attack them)
		//
		void  IsAggressiveAgainstPlayer( long iPlayer ){ m_iAggressiveTowardsPlayer = iPlayer; }
		long  IsAggressiveAgainstPlayer(){ return m_iAggressiveTowardsPlayer; }
		bool  CurrentForeignRelationsPreventsPersuingThisDesire();

		//
		// Time-Value functions
		//
		void  SetTimeValueFunction_OnlyAvailableNow();   // peak from 0-1 turns, zero value at turn 4
		void  SetTimeValueFunction_PlanForNext10Turns();
		void  SetTimeValueFunction_PlanLongterm();
		float GetTimeValueFunction( float fArrivalTime ){ return m_TimeValueFunction.GetOutput( fArrivalTime ); }

		bool  ActionInvolvesCapturingAndOccupyingGroundTerritory();

		// Feasbility is based on proximity (based on the available units)
		//   Capturing a city 500 pixels away might be infeasible, but 
		//   bombing a city 500 pixels is feasible if I have aircraft that can reach it
		//   But, bombing a city 600 pixels away is not feasible if out of range (except with airfield, carrier, capturing something, etc)
		//virtual void  CalculateAccessibility01(){}

		// Simple Interest
		virtual float GetSimpleInterest(){ return 0.0f; }
		virtual float GetAreaSimpleInterest(){ return 0.0f; }
		// Effort
		virtual float GetAccessibility01(){ return 0.0f; }

	// Note: "Effort" varies depending on my method of attack
	//   Firing a nuke = very little effort (but does cost me a nuke)
	//   Bombing a city = worried about anti-aircraft fire, ignore most units
	//   Using ground units + capturing a city = have to fight all the units in the area
		//virtual float GetExpectedEffort(){ return 0.0f; }
		//virtual float GetAreaExpectedEffort(){ return 0.0f; }

		//virtual void  GetExpectedResistance( CList< CUnit* > pResistance ){} // includes pathway enemies, and enemies near the target
		// Score
		void    CalculateScore( EnumScoringLevel eScoringLevel );
		float   GetScore(){ return m_fScore; }

		virtual bool  CanFulfillDesire( CEOSAIPoiObject* pActor ){ return false; }
		virtual bool  CanFulfillDesire( CEOSAIUnitTemplate* pActor ){ return false; }
		virtual float GetUnitJobValue01( CEOSAIUnitTemplate* pActor ){ return 0.0f; }
		//
		CEOSAIJobsToDo*  GetJobsToDo(){ return &m_JobsToDo; }
		//float         GetBestMovementRateForUnitsCompletingAllTheJobs();

	protected:
		CEOSAIThoughtDatabase* GetAIThoughtDatabase();
		//
		CEOSAIBrain*           m_pAIBrain;
		CList< CAITacticalProject* >  m_TacticalProjectsIAmAPrimaryMemberOf; // Right now, I'm setting up desires to belong to one TacticalProject
		CList< CEOSAITacticalProject2* >  m_TacticalProjects2IAmAPrimaryMemberOf; // Right now, I'm setting up desires to belong to one TacticalProject
		//CAITacticalProject*  m_pTacticalProject; // Right now, I'm setting up desires to belong to one TacticalProject
		//CAITacticalProject*  m_pTacticalProject; // desires can only belong to one ActionDefinition
		//CEOSAITaskForceDesc  m_SimpleTaskForceDesc;

		long  m_iAggressiveTowardsPlayer;

		CEOSAIMathFunction  m_TimeValueFunction;

		// Scoring
		EnumScoringLevel m_eScoringLevel;
		float  m_fScore;

		// Jobs to do - what jobs have to be done to fulfill this desire?
		CEOSAIJobsToDo  m_JobsToDo;
};



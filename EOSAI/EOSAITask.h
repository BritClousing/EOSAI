
#pragma once

#include "PoiObjectState.h"
//class CAIObject;
class CEOSAIPoiObject;
class CEOSAIAction;

/*
	AITask is the base class for CEOSAITaskForSingleUnit and compound tasks (which involve coordination 
		of multiple units at once).

	The following methods have to be overridden:

		//
		// Calculations
		//
			virtual void UpdateStateAndScore( CAIScheduleModifier* pScheduleModifier );
			virtual void RecalculateContextualValue( CAIScheduleModifier* pScheduleModifier );
		//
		// Actions
		//
			virtual void RemoveThisTaskFromUnitSchedules( CAIScheduleModifier* pScheduleModifier );
			virtual void RecalculateScheduledTaskScoresOfUnitsWithThisTask( CAIScheduleModifier* pScheduleModifier );
*/

class CEOSAITask
{
	public:
		//
		CEOSAITask( CEOSAIPoiObject* pActor, CEOSAIAction* pAIAction );
		virtual ~CEOSAITask(){}

		CEOSAIAction*    GetAIAction(){ return m_pAIAction; }

		bool          IsValid(){ return m_bTaskIsValid; }
		void          IsValid( bool b ){ m_bTaskIsValid = b; }

		//virtual void  CreateOrders() = 0;
		bool          OrdersWereCreated(){ return m_bOrdersWereCreated; }
		float         GetScore(){ return m_fScore; }

		EOSAI::PoiObjectState* GetMemoryOfInitialState(){ return &m_MemoryOfInitialState; }
		EOSAI::PoiObjectState* GetFinalState(){ return &m_FinalState; }

		virtual float GetStartTime() = 0;
		virtual void  SetStartTime( float f ){}
		virtual float GetEndTime() = 0;
		virtual void  SetEndTime( float f ){}
		virtual float GetTaskDuration() = 0;

		virtual float GetMovementDistance(){ return 0.0f; }

		//virtual void  TurnPrerequisiteTasksIntoOrders(){}
		//void          HasDependencyOnTask( CEOSAITask* pTask ){ m_pHasDependencyOnTask = pTask; }
		//CEOSAITask*      HasDependencyOnTask();

	protected:
		float m_fScore;

		bool  m_bTaskIsValid; // tasks can become invalid if, for example, an earlier transport task gets cancelled

		//
		CEOSAIAction*     m_pAIAction;
		CEOSAIPoiObject*  m_pActor;

		// Final State is based on the initial state + effects of this task
		EOSAI::PoiObjectState  m_MemoryOfInitialState;
		EOSAI::PoiObjectState  m_FinalState;

		// These are tasks that are inter-related with this AITask.  This can be pickup/dropoff
		//   tasks with a transport, coordinated attacks with other units, etc
		//CList< CEOSAITask* >  m_AssociatedAITasks;
		CList< CEOSAITask* >  m_ConnectedTasks;
		//CEOSAITask*           m_pHasDependencyOnTask;

		bool  m_bOrdersWereCreated; // set to true when this task instanciates orders
};

/*
class CEOSAITask
{
	public:
		//
			CEOSAITask( CAIPlayer* pAIPlayer, CAIDesire* pDesire );
		//
			CAIDesire*  GetDesire(){ return m_pDesire; }
			virtual CEOSAITask* GetParentTask(){ return NULL; }
			virtual float    GetTotalContextualScore(){ return m_fContextualScore; }
			//virtual float    GetCurrentContextualScoreOfTenativeTasksOfUnitsThatWouldDoThisTask() = 0;

			virtual void     SetThisTaskAsTenativeNextTask( CAIScheduleModifier* pScheduleModifier ) = 0;
			virtual void     RemoveThisTaskAsTenativeNextTask( CAIScheduleModifier* pScheduleModifier ) = 0;
			virtual long     GetNumberOfUnitsInvolvedInTask(){ return 1; }
			virtual CAIProxyPoiMobileMine* GetUnit( long index ) = 0;

			float  GetRawScore(){ return m_fRawScore; }
			float  GetContextualScore(){ return m_fContextualScore; }

			virtual void UpdatePreviousStateInformation() = 0;
		//
			void         SetFlag_UpdateContextualScore(){ m_bUpdateContextualScore = true; }
			//virtual void SetFlag_InvalidateScoreCalculation( CAIScheduleModifier* pScheduleModifier );
			//virtual void InvalidateTaskAndSubsequentTasks();
			void         SetFlag_PreviousStateWasChanged( bool bVal ){ m_bPreviousStateWasChanged = bVal; if( bVal ){ SetFlag_FinalStateWasChanged(); } }
			bool         GetFlag_PreviousStateWasChanged(){ return m_bPreviousStateWasChanged; }
			virtual void SetFlag_FinalStateWasChanged(){}
		//
		// Calculations
		//
			virtual void UpdateStateAndScore( CAIScheduleModifier* pScheduleModifier ) = 0;
			virtual void RecalculateContextualValue( CAIScheduleModifier* pScheduleModifier ) = 0;
		//
		// Actions
		//
			virtual bool ScheduleTask( CAIScheduleModifier* pScheduleModifier ) = 0;

			virtual void RemoveThisTaskFromUnitSchedules( CAIScheduleModifier* pScheduleModifier ) = 0;
			virtual void RecalculateScheduledTaskScoresOfUnitsWithThisTask( CAIScheduleModifier* pScheduleModifier ) = 0;
		//
		// Scheduled state
		//
			void   SetTaskToScheduled( bool bVal, CAIScheduleModifier* pScheduleModifier );
			bool   GetTaskIsScheduled(){ return m_bTaskIsScheduled; }
			bool   GetUndoTaskIsScheduled(){ return m_Undo_bTaskIsScheduled; }
		//
		// AIScheduleModifier / Undo
		//
			long          GetLastRestoreId(){ return m_iLastRestoreId; }
			virtual void  Remember( CAIScheduleModifier* pScheduleModifier );
			virtual void  Restore( CAIScheduleModifier* pScheduleModifier );

			float         GetUndoContextualScore(){ return m_Undo_fContextualScore; }

	protected:

			void  SetRawScore( float fNewValue, CAIScheduleModifier* pScheduleModifier );
			void  SetContextualScore( float fNewValue ){ m_fContextualScore = fNewValue; }

		//
			CAIPlayer*  m_pAIPlayer;
			CAIDesire*  m_pDesire;
			bool        m_bTaskIsScheduled;
		//
		// Score
		//
			bool   m_bPreviousStateWasChanged;

			// Scores
			bool   m_bUpdateContextualScore;
			//bool   m_bScoreIsInvalid;
			//bool   m_bScoreCalculationIsInvalid;
		private:
			float  m_fRawScore;
			float  m_fContextualScore;
		//
		// Undo information
		//
			long        m_iLastRestoreId;
		//protected:
			bool        m_Undo_bTaskIsScheduled;
			float       m_Undo_fRawScore;
			float       m_Undo_fContextualScore;
};
*/

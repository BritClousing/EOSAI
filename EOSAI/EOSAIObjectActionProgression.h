
#pragma once

class CEOSAIPoiObject;
class CEOSAIAction;
class CEOSAITask;
namespace EOSAI
{
	class PoiObjectState;
}
class CEOSAIMultiRegion2;

// This class handles the addition, insertion, subtraction of actions and tasks.
//   This class can be used to handle hypotheticals?
//   It also handles the StateProgression information.
// I may want to give this class some intelligence to rearrange tasks and actions.
//
// What this class does:
//   - When an AITask is inserted, removed, or altered, it knows to:
//     - Update the subsequent AITasks and the AIActions that are related to it.
//       (This includes keeping AITasks and AIActions in sync.)
//     - Update the associated AITasks (e.g. pickups, dropoffs, etc)
//
// Do I want to keep an independent list that contains the movement progression 
//   (exactly which MultiRegions/AIRegions, when, etc)?  This adds another thing to keep in 
///  sync, although it might be something that I only update when it is requested (I don't 
//   need to build a list each time it is requested or each time the Tasks change; I can set flags.)

/*
class CEOSAIObjectActionProgression
{
	/-*
	public:
		enum EnumActionProgressionError
		{
			Undefined,
			EnumGoesOutsideRange,  // aircraft has too much movement between landings
			EnumTooManyContainees, // transport/carrier will contain too many units at one time
			EnumUnitCannotMoveThroughMountains,
		};
	*-/

	public:
		CEOSAIObjectActionProgression();
		~CEOSAIObjectActionProgression();

		void           SetAIObject( CEOSAIPoiObject* pAIObject ){ m_pAIObject = pAIObject; }
		CEOSAIPoiObject* GetAIObject(){ return m_pAIObject; }

		// Information
		//
			EOSAI::PoiObjectState* GetInitialState();
			EOSAI::PoiObjectState* GetFinalState();

			// This is the movement distance according to the tasks.
			// It's not completely useful for aircraft because it doesn't take landing into account.
			float  GetMovementDistance();
			//float  GetFutureAirDistanceToMultiRegion( CMultiRegion* pTargetMultiRegion );
/*
		// AIActions
		//
			// Add an AIAction (and it's AITasks) to the sequence.  
			//   Doesn't specify where in the sequence to add the AIAction or the AITasks.
			//   Not implimented yet.
			//void AddAIAction( CEOSAIAction* pAIAction );
			// Add an AIAction to the end of the action sequence
			void AppendAIAction( CEOSAIAction* pAIAction );
			void RemoveAIActionAndAITasks( CEOSAIAction* pAIAction );
			CList< CEOSAIAction* >*  GetAIActions(){ return &m_AIActions; }

		// AITasks
		//
			void AppendAITask( CEOSAITask* pAITask );
			void InsertAITask( EOSAI::PoiObjectState* pPreviousState, CEOSAITask* pAITask );
			void UpdateTaskTimes();
			void sRemoveTask( CEOSAITask* pAITask );
			CList< CEOSAITask* >*  GetAITasks(){ return &m_AITasks; }
*-/
		// Orders
		//
			//bool TurnTasksIntoOrders();

	private:
		CEOSAIPoiObject* m_pAIObject;
		//CList< CEOSAIAction* > m_AIActions;
		//CList< CEOSAITask* >   m_AITasks;
};
*/

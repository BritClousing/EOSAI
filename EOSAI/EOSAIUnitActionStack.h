
#pragma once

//
// The AIUnitActionStack is used for ordering Units/Cities in accomplishing some task and finding
//   the best minimal set of units to use.
// For example, if I have two tanks and two infantry, and I have some needs (capture city, build airfield)
//   then I place the four unit on the stack.  The result will find the minimal task force
//   to accomplish the tasks (one infantry is necessary because it's the only unit that can build
//   airfields, and the need for the tanks is partially dependent on the need for extra firepower).
//

//#include "EOSAITaskForce3.h"
#include "EOSAIListSort.h"
//#include "EOSAIJobCapability.h"
//#include "EOSAIJobRole.h"
class CEOSAIJobRole;
class CEOSAITaskForce3;
class CEOSAIPoiObject;
//class CUnitTemplate;
class CEOSAIJobsToDo;
class CEOSAIUnitActionIdea;
class CEOSAIMathFunction;
class CEOSAIUnitActionStack;
class CEOSAITaskForce3;
class CEOSAIUnit;
class CEOSAIUnitTemplate;
//class CUnit;
class CEOSAIUnitTransportMovementDesc;

class CEOSAIUnitActionStackItem
{
	public:
		CEOSAIUnitActionStackItem()
		{
			m_bHypotheticallyAllocated = false;
			m_iSanityCheck231987 = 231987;
			//
			m_pUnitActionStack = NULL;
			m_pUnitActionIdea = NULL;
			//m_pAIActor = NULL;
			//
			m_pSuggestedTransportMoveDesc = NULL;
			m_fArrivalTime = 0.0f;
		}
		~CEOSAIUnitActionStackItem();
		//
		CEOSAITaskForce3* GetParentTaskForce();
		//
		void  Allocate( bool bAllocate );
		bool  ActorIsAllocated();
		bool  ActorIsAllocatedByThisTaskForce();
		bool  ActorIsAllocatedByOtherTaskForce();
		//
		CEOSAIUnit*         GetAIUnitActor();
		CEOSAIUnitTemplate* GetAIUnitTemplate();
		CEOSAIJobRole*      GetJobRole();
		//
		void  ClearSuggestedTransportMoveDesc();
		void  SetSuggestedTransportMoveDesc( CEOSAIUnit* pAITransportUnit );
		void  SetSuggestedTransportMoveDesc( CEOSAIUnitTransportMovementDesc* pMoveDesc );
		CEOSAIUnitTransportMovementDesc*  GetSuggestedTransportMoveDesc(){ return m_pSuggestedTransportMoveDesc; }
		//
		void  AllocateSuggestedTransportSpace();
		void  DeAllocateSuggestedTransportSpace();
		void  CreateTransportUnitActionIdeaFromSuggestedTransportMoveDesc();
		//void  AllocateSuggestedTransportMoveDesc();
		//void  DeAllocateSuggestedTransportMoveDesc();
		//
		//void  ReleaseAllocations();

		bool  m_bHypotheticallyAllocated; // Used in best-case scenario

		long  m_iSanityCheck231987;

		CEOSAIUnitActionStack* m_pUnitActionStack; // Parent
		CEOSAIUnitActionIdea*  m_pUnitActionIdea;
		//CEOSAIPoiObject*       m_pAIActor; // City or Unit
		CEOSAIUnitTransportMovementDesc*  m_pSuggestedTransportMoveDesc;
		float               m_fArrivalTime;
};

class CEOSAIUnitActionStack
{
	public:
		CEOSAIUnitActionStack();
		~CEOSAIUnitActionStack(){ Delete(); }

		void  Delete();
		CEOSAIUnitActionStackItem*  Insert( CEOSAIUnitActionIdea* pIdea );
		void                     Remove( CEOSAIUnitActionStackItem* pItem );
		bool                     UpdateStackPosition( CEOSAIUnitActionStackItem* pItem ); // Update the sorted position of this item

		long                     GetNumberOfAllocatedItems();
		CEOSAIUnitActionStackItem*  GetLastAllocatedItem();
		float                    GetLastAllocatedItemTime();

		//bool   InvolvesTransports();
		//float  GetLastShoreArrivalTime();

		void   ReleaseAllAllocations();
		//void   ReleaseAllocationsRelatedToAIUnit( CEOSAIUnit* pAIUnit );
		//void   DeleteStackItemsRelatedToAIUnit( CEOSAIUnit* pAIUnit );

		//void  Insert( CEOSAIPoiObject* pAIActor, CUnitTemplate* pUnitTemplate, float fArrivalTime );
		//void  CalculateSet( CEOSAIJobsToDo* pAIJobsToDo, CEOSAIMathFunction* pTimeValueFunction, float fAcceptableAttritionBalance );

		CEOSAITaskForce3* m_pParentTaskForce;
		CEOSAIListSort< CEOSAIUnitActionStackItem* >  m_ItemsSortedByArrivalTime; // sorted by arrival time
};


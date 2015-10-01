
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
class CEOSAIUnit2ActionIdea;
class CEOSAIMathFunction;
class CEOSAIUnit2ActionStack;
class CEOSAITaskForce3;
class CEOSAIUnit2;
class CEOSAIUnitTemplate;
//class CUnit;
class CEOSAIUnit2TransportMovementDesc;

class CEOSAIUnit2ActionStackItem
{
	public:
		CEOSAIUnit2ActionStackItem()
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
		~CEOSAIUnit2ActionStackItem();
		//
		CEOSAITaskForce3* GetParentTaskForce();
		//
		void  Allocate( bool bAllocate );
		bool  ActorIsAllocated();
		bool  ActorIsAllocatedByThisTaskForce();
		bool  ActorIsAllocatedByOtherTaskForce();
		//
		CEOSAIUnit2*         GetAIUnitActor();
		CEOSAIUnitTemplate* GetAIUnitTemplate();
		CEOSAIJobRole*      GetJobRole();
		//
		void  ClearSuggestedTransportMoveDesc();
		void  SetSuggestedTransportMoveDesc( CEOSAIUnit2* pAITransportUnit );
		void  SetSuggestedTransportMoveDesc( CEOSAIUnit2TransportMovementDesc* pMoveDesc );
		CEOSAIUnit2TransportMovementDesc*  GetSuggestedTransportMoveDesc(){ return m_pSuggestedTransportMoveDesc; }
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

		CEOSAIUnit2ActionStack* m_pUnitActionStack; // Parent
		CEOSAIUnit2ActionIdea*  m_pUnitActionIdea;
		//CEOSAIPoiObject*       m_pAIActor; // City or Unit
		CEOSAIUnit2TransportMovementDesc*  m_pSuggestedTransportMoveDesc;
		float               m_fArrivalTime;
};

class CEOSAIUnit2ActionStack
{
	public:
		CEOSAIUnit2ActionStack();
		~CEOSAIUnit2ActionStack(){ Delete(); }

		void  Delete();
		CEOSAIUnit2ActionStackItem*  Insert( CEOSAIUnit2ActionIdea* pIdea );
		void                     Remove( CEOSAIUnit2ActionStackItem* pItem );
		bool                     UpdateStackPosition( CEOSAIUnit2ActionStackItem* pItem ); // Update the sorted position of this item

		long                     GetNumberOfAllocatedItems();
		CEOSAIUnit2ActionStackItem*  GetLastAllocatedItem();
		float                    GetLastAllocatedItemTime();

		//bool   InvolvesTransports();
		//float  GetLastShoreArrivalTime();

		void   ReleaseAllAllocations();
		//void   ReleaseAllocationsRelatedToAIUnit( CEOSAIUnit2* pAIUnit );
		//void   DeleteStackItemsRelatedToAIUnit( CEOSAIUnit2* pAIUnit );

		//void  Insert( CEOSAIPoiObject* pAIActor, CUnitTemplate* pUnitTemplate, float fArrivalTime );
		//void  CalculateSet( CEOSAIJobsToDo* pAIJobsToDo, CEOSAIMathFunction* pTimeValueFunction, float fAcceptableAttritionBalance );

		CEOSAITaskForce3* m_pParentTaskForce;
		CEOSAIListSort< CEOSAIUnit2ActionStackItem* >  m_ItemsSortedByArrivalTime; // sorted by arrival time
};


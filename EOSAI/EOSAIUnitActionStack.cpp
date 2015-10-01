
#include "stdafx.h"
#include "EOSAIUnitActionStack.h"
#include "EOSAIJobsToDo.h"
#include "EOSAITacticalProject2.h"
#include "EOSAITaskForce3.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAIJobCapability.h"
#include "EOSAIUnit2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Large values first
bool SortDescByArrivalTime( CEOSAIUnit2ActionStackItem* p1, CEOSAIUnit2ActionStackItem* p2 )
{
	if( p1->m_fArrivalTime < p2->m_fArrivalTime ){ return true; }
	else if( p1->m_fArrivalTime > p2->m_fArrivalTime ){ return false; }
	return true;  // always return true when values are equal
}

CEOSAIUnit2ActionStackItem::~CEOSAIUnit2ActionStackItem()
{
	m_iSanityCheck231987 = 0;//231987;
	int g=0;
}

CEOSAITaskForce3* CEOSAIUnit2ActionStackItem::GetParentTaskForce()
{
	return m_pUnitActionStack->m_pParentTaskForce;
}

void CEOSAIUnit2ActionStackItem::Allocate( bool bAllocate )
{
	if( bAllocate )
	{
		m_pUnitActionStack->m_pParentTaskForce->GetJobCapabilityOfAllocatedUnits()->Add( GetAIUnitTemplate() );

		if( this->GetSuggestedTransportMoveDesc() )
		{
			/*
			if( pUnitActionIdea->GetAIUnitActor() &&
				pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer() )
			{
				ASSERT( pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport ==
						pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer() );
			}
			else
			{
				pUnitStackItem->AllocateSuggestedTransportSpace();
				pUnitStackItem->AllocateSuggestedTransportMoveDesc();
			}
			*/
			ASSERT( this->m_pUnitActionIdea->GetDirectMoveToTargetTime() > 999999.0f );

			this->AllocateSuggestedTransportSpace();
			//this->AllocateSuggestedTransportMoveDesc();
		}
	}
	else
	{
		m_pUnitActionStack->m_pParentTaskForce->GetJobCapabilityOfAllocatedUnits()->Remove( GetAIUnitTemplate() );

		// Deallocate the Transport allocation, too (if that was done)
		if( this->GetSuggestedTransportMoveDesc() )
		{
			this->DeAllocateSuggestedTransportSpace();
			//this->DeAllocateSuggestedTransportMoveDesc();
		}
	}

	if( GetParentTaskForce()->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::BestCaseTaskForce )
	{
		ASSERT( m_bHypotheticallyAllocated != bAllocate );
		m_bHypotheticallyAllocated = bAllocate;
	}
	else if( GetParentTaskForce()->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::RealTaskForce )
	{
		ASSERT( m_pUnitActionStack );
		ASSERT( m_pUnitActionIdea );
		ASSERT( m_pUnitActionIdea->GetAIPoiObjectActor() );
		CEOSAIPoiObject* pAIPoiObject = m_pUnitActionIdea->GetAIPoiObjectActor();
		CEOSAIUnit2ActionIdea* pAllocatedUnitActionIdea = pAIPoiObject->GetAllocatedUnitActionIdea();
		if( bAllocate )
		{
			ASSERT( pAllocatedUnitActionIdea == NULL );
			m_pUnitActionIdea->GetAIPoiObjectActor()->SetAllocatedUnitActionIdea( m_pUnitActionIdea );
		}
		else
		{
			ASSERT( pAllocatedUnitActionIdea == m_pUnitActionIdea );
			m_pUnitActionIdea->GetAIPoiObjectActor()->ReleaseAllocatedUnitActionIdea( m_pUnitActionIdea );
		}
	}
}


CEOSAIUnit2* CEOSAIUnit2ActionStackItem::GetAIUnitActor()
{
	CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( m_pUnitActionIdea->GetAIPoiObjectActor() );
	//if( pAIUnit ){ return pAIUnit->GetServerUnit(); }
	if( pAIUnit ){ return pAIUnit; }
	return NULL;
}

CEOSAIUnitTemplate* CEOSAIUnit2ActionStackItem::GetAIUnitTemplate()
{
	return m_pUnitActionIdea->GetAIUnitTemplate();
}

CEOSAIJobRole* CEOSAIUnit2ActionStackItem::GetJobRole()
{
	return m_pUnitActionIdea->GetJobRole();
	//m_JobRole; // include combat in this unit's role?
}
/*
bool CEOSAIUnit2ActionStackItem::ActionIdeaIsAllocated()
{
	if( GetParentTaskForce()->GetTaskForceType() == CEOSAITaskForce3::EnumTaskForceType_BestCaseScenario )
	{
		return m_bHypotheticallyAllocated;
	}
	if( GetParentTaskForce()->GetTaskForceType() == CEOSAITaskForce3::EnumTaskForceType_NegotiateForUnits )
	{
		return m_pUnitActionIdea->GetAIPoiObjectActor()->GetAllocation() == m_pUnitActionIdea;
	}
	ASSERT( false );
	return false;
}
*/

void CEOSAIUnit2ActionStackItem::ClearSuggestedTransportMoveDesc()
{
	if( m_pSuggestedTransportMoveDesc != NULL )
	{
		m_pSuggestedTransportMoveDesc = NULL;
		m_fArrivalTime = m_pUnitActionIdea->GetDirectMoveToTargetTime();
	}
}

void CEOSAIUnit2ActionStackItem::SetSuggestedTransportMoveDesc( CEOSAIUnit2* pAITransportUnit )
{
	// Lookup the new MoveDesc
	ASSERT( pAITransportUnit != NULL );
	POSITION pos = m_pUnitActionIdea->m_UnitTransportMovementDescs.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TransportMovementDesc* pDesc = m_pUnitActionIdea->m_UnitTransportMovementDescs.GetNext( pos );
		if( pDesc->GetAIPoiActor() == pAITransportUnit )
		{
			SetSuggestedTransportMoveDesc( pDesc );
		}
	}
}

void CEOSAIUnit2ActionStackItem::SetSuggestedTransportMoveDesc( CEOSAIUnit2TransportMovementDesc* pMoveDesc )
{
	ASSERT( pMoveDesc != NULL );
	if( m_pSuggestedTransportMoveDesc != pMoveDesc )
	{
		m_pSuggestedTransportMoveDesc = pMoveDesc;

		//if( pMoveDesc == NULL )
		//{
		//	m_fArrivalTime = m_pUnitActionIdea->GetDirectMoveToTargetTime();
		//}
		if( pMoveDesc && m_fArrivalTime != pMoveDesc->m_fTotalTime )
		{
			m_fArrivalTime = pMoveDesc->m_fTotalTime;
		}
	}
}

void CEOSAIUnit2ActionStackItem::AllocateSuggestedTransportSpace()
{
	if( m_pSuggestedTransportMoveDesc )
	{
		CEOSAIUnit2* pAITransportUnit = m_pSuggestedTransportMoveDesc->m_pAITransport;
		//ASSERT( m_pSuggestedTransportMoveDesc->m_pAITransport );
		if( m_pSuggestedTransportMoveDesc->m_pAITransport )
		{
			CEOSAITaskForce3* pAITaskForce = GetParentTaskForce();

			#ifdef _DEBUG
			CEOSAITransportSpaceAllocation* pAllocationObj = pAITransportUnit->GetTransportSpaceAllocationObj( pAITaskForce );
			ASSERT( pAITransportUnit->HasAllocatedSpace( pAITaskForce,m_pUnitActionIdea ) == false );
			bool b = pAITransportUnit->HasAllocatedSpace( pAITaskForce,m_pUnitActionIdea );
			#endif _DEBUG

			pAITransportUnit->AllocateTransportSpace( pAITaskForce,m_pUnitActionIdea );
			int h=0;
		}
	}
}

void CEOSAIUnit2ActionStackItem::DeAllocateSuggestedTransportSpace()
{
	if( GetParentTaskForce()->IsBeingDeleted() ) return;
	if( m_pSuggestedTransportMoveDesc )
	{
		CEOSAIUnit2* pAITransportUnit = m_pSuggestedTransportMoveDesc->m_pAITransport;
		if( m_pSuggestedTransportMoveDesc->m_pAITransport )
		{
			CEOSAITaskForce3* pAITaskForce = GetParentTaskForce();

			#ifdef _DEBUG
			CEOSAITransportSpaceAllocation* pAllocationObj = pAITransportUnit->GetTransportSpaceAllocationObj( pAITaskForce );
			ASSERT( pAITransportUnit->HasAllocatedSpace( pAITaskForce,m_pUnitActionIdea ) == true );
			#endif _DEBUG

			pAITransportUnit->DeAllocateTransportSpace( pAITaskForce,m_pUnitActionIdea );
		}
	}
}

void CEOSAIUnit2ActionStackItem::CreateTransportUnitActionIdeaFromSuggestedTransportMoveDesc()
{
	if( m_pSuggestedTransportMoveDesc )
	{
		// Look-up the transport
		CEOSAITaskForce3* pAITaskForce = GetParentTaskForce();
		//CAITacticalProject* pAITacticalProject = pAITaskForce->GetTacticalProject();
		CEOSAITacticalProject2* pAITacticalProject2 = pAITaskForce->GetTacticalProject2();
		if( pAITaskForce->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::BestCaseTaskForce )
		{
			// This is only used for turning UnitActionIdeas into Actions
			//CEOSAIUnit2ActionIdea* pAITransportActionIdea = m_pSuggestedTransportMoveDesc->CreateAIUnitActionIdea( pAITacticalProject );
			//pUnitActionIdea->SetAITransportActionIdea( pAITransportActionIdea );
		}
		if( pAITaskForce->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::RealTaskForce )
		{
			CEOSAIUnit2ActionIdea* pAITransportActionIdea;
			/*
			if( pAITacticalProject )
			{
				pAITransportActionIdea = m_pSuggestedTransportMoveDesc->CreateAIUnitActionIdea( pAITacticalProject );
			}
			*/
			if( pAITacticalProject2 )
			{
				pAITransportActionIdea = m_pSuggestedTransportMoveDesc->CreateAIUnitActionIdea( pAITacticalProject2 );
			}

			// Allocate the Transport to this UnitActionIdea
			//pAITransportActionIdea->GetAIPoiObjectActor()->SetAllocation( pAITransportActionIdea );
			//m_pSuggestedTransportMoveDesc->GetAIPoiActor()->TransportAllocatedToTacticalProject( pAITacticalProject );
			//pAITransportActionIdea->GetAIPoiObjectActor()->AddAllocatedTransportActionIdea( pAITransportActionIdea );

			// Set the ActionIdea to point to the Transport
			m_pUnitActionIdea->SetAITransportUnitActionIdea( pAITransportActionIdea );
		}
	}
}

/*
void CEOSAIUnit2ActionStackItem::AllocateSuggestedTransportMoveDesc()
{
	if( m_pSuggestedTransportMoveDesc )
	{
		/-*
		if( m_pSuggestedTransportMoveDesc->m_pAITransportUnit )
		{
			ASSERT( m_pSuggestedTransportMoveDesc->m_pAITransportUnit->HasAllocatedSpace( GetParentTaskForce(),m_pUnitActionIdea ) == false );
			m_pSuggestedTransportMoveDesc->m_pAITransportUnit->AllocateTransportSpace( GetParentTaskForce(),m_pUnitActionIdea );
		}
		*-/

		// Look-up the transport
		CEOSAITaskForce3* pAITaskForce = GetParentTaskForce();
		CAITacticalProject* pAITacticalProject = pAITaskForce->GetTacticalProject();
		if( pAITaskForce->GetTaskForceType() == CEOSAITaskForce3::EnumTaskForceType_BestCaseScenario )
		{
			// This is only used for turning UnitActionIdeas into Actions
			//CEOSAIUnit2ActionIdea* pAITransportActionIdea = m_pSuggestedTransportMoveDesc->CreateAIUnitActionIdea( pAITacticalProject );
			//pUnitActionIdea->SetAITransportActionIdea( pAITransportActionIdea );
		}
		if( pAITaskForce->GetTaskForceType() == CEOSAITaskForce3::EnumTaskForceType_NegotiateForUnits )
		{
			CEOSAIUnit2ActionIdea* pAITransportActionIdea = m_pSuggestedTransportMoveDesc->CreateAIUnitActionIdea( pAITacticalProject );

			// Allocate the Transport to this UnitActionIdea
			//pAITransportActionIdea->GetAIPoiObjectActor()->SetAllocation( pAITransportActionIdea );
			//m_pSuggestedTransportMoveDesc->GetAIPoiActor()->TransportAllocatedToTacticalProject( pAITacticalProject );
			//pAITransportActionIdea->GetAIPoiObjectActor()->AddAllocatedTransportActionIdea( pAITransportActionIdea );

			// Set the ActionIdea to point to the Transport
			m_pUnitActionIdea->SetAITransportUnitActionIdea( pAITransportActionIdea );
		}
	}
}

void CEOSAIUnit2ActionStackItem::DeAllocateSuggestedTransportMoveDesc()
{
	if( GetParentTaskForce()->IsBeingDeleted() ) return;
	if( m_pSuggestedTransportMoveDesc )
	{
		// Look-up the transport
		CEOSAITaskForce3* pAITaskForce = GetParentTaskForce();
		CAITacticalProject* pAITacticalProject = pAITaskForce->GetTacticalProject();
		if( pAITaskForce->GetTaskForceType() == CEOSAITaskForce3::EnumTaskForceType_BestCaseScenario )
		{
			// This is only used for turning UnitActionIdeas into Actions
			//CEOSAIUnit2ActionIdea* pAITransportActionIdea = m_pSuggestedTransportMoveDesc->CreateAIUnitActionIdea( pAITacticalProject );
			//pUnitActionIdea->SetAITransportActionIdea( pAITransportActionIdea );
		}
		if( pAITaskForce->GetTaskForceType() == CEOSAITaskForce3::EnumTaskForceType_NegotiateForUnits )
		{
			// This can only be deallocated when deleting the whole TaskForce
			ASSERT( false );
			/-*
			CEOSAIUnit2ActionIdea* pAITransportActionIdea = m_pSuggestedTransportMoveDesc->CreateAIUnitActionIdea( pAITacticalProject );

			// Allocate the Transport to this UnitActionIdea
			pAITransportActionIdea->GetAIPoiObjectActor()->RemoveAllocatedTransportActionIdea( pAITransportActionIdea );

			// Set the ActionIdea to point to the Transport
			m_pUnitActionIdea->SetAITransportUnitActionIdea( pAITransportActionIdea );
			*-/
		}
	}
}
*/

bool CEOSAIUnit2ActionStackItem::ActorIsAllocated()
{
	if( GetParentTaskForce()->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::BestCaseTaskForce )
	{
		return m_bHypotheticallyAllocated;
	}
	if( GetParentTaskForce()->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::RealTaskForce )
	{
		return m_pUnitActionIdea->GetAIPoiObjectActor()->GetAllocatedUnitActionIdea() != NULL;
	}
	ASSERT( false );
	return false;
}

bool  CEOSAIUnit2ActionStackItem::ActorIsAllocatedByThisTaskForce()
{
	if( GetParentTaskForce()->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::BestCaseTaskForce )
	{
		return m_bHypotheticallyAllocated;
	}
	if( GetParentTaskForce()->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::RealTaskForce )
	{
		return m_pUnitActionIdea->GetAIPoiObjectActor()->GetAllocatedUnitActionIdea() == m_pUnitActionIdea;
		//ASSERT( m_pUnitActionStack );
		//CEOSAIUnit2ActionIdea* pCurrentAllocation = m_pUnitActionIdea->GetAIPoiObjectActor()->GetAllocation();
		//return ( pCurrentAllocation != NULL && pCurrentAllocation != m_pUnitActionIdea );
	}
	ASSERT( false );
	return false;
}

bool  CEOSAIUnit2ActionStackItem::ActorIsAllocatedByOtherTaskForce()
{
	if( GetParentTaskForce()->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::BestCaseTaskForce )
	{
		return false;
	}
	if( GetParentTaskForce()->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::RealTaskForce )
	{
		ASSERT( m_pUnitActionStack );
		CEOSAIUnit2ActionIdea* pCurrentAllocation = m_pUnitActionIdea->GetAIPoiObjectActor()->GetAllocatedUnitActionIdea();
		if( pCurrentAllocation == NULL ) return false;
		if( pCurrentAllocation == m_pUnitActionIdea ) return false;
		return true;
	}
	ASSERT( false );
	return false;
}
/*
void  CEOSAIUnit2ActionStackItem::ReleaseAllocations()
{
	Allocate( false );
}
*/
CEOSAIUnit2ActionStack::CEOSAIUnit2ActionStack()
{
	m_ItemsSortedByArrivalTime.SetSortFunction( SortDescByArrivalTime );
}

void  CEOSAIUnit2ActionStack::Delete()
{
	while( m_ItemsSortedByArrivalTime.IsEmpty() == FALSE ){ delete m_ItemsSortedByArrivalTime.RemoveHead(); }
}

CEOSAIUnit2ActionStackItem*  CEOSAIUnit2ActionStack::Insert( CEOSAIUnit2ActionIdea* pUnitActionIdea )
{
	CEOSAIUnit2* pAIUnitActor = pUnitActionIdea->GetAIUnitActor();

	CEOSAIUnit2ActionStackItem* pItem = new CEOSAIUnit2ActionStackItem();
	pItem->m_pUnitActionStack = this;
	pItem->m_pUnitActionIdea = pUnitActionIdea;
	//pItem->m_pAIActor = pAIUnitActor;
	//pItem->m_pUnitTemplate = pUnitActionIdea->GetUnitTemplate();
	//pItem->m_fArrivalTime = pUnitActionIdea->GetBuildAndTransportAndMoveToTargetTime();
	pItem->m_fArrivalTime = pUnitActionIdea->GetBestCaseTotalTimeToTarget();
	//pItem->m_JobCapability.Set( pUnitActionIdea->GetUnitTemplate() );
	m_ItemsSortedByArrivalTime.InsertSorted( pItem );
	return pItem;
}

void  CEOSAIUnit2ActionStack::Remove( CEOSAIUnit2ActionStackItem* pItem )
{
	POSITION pos = m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnit2ActionStackItem* pItemInList = m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pItemInList == pItem ){ m_ItemsSortedByArrivalTime.RemoveAt( prevPos ); }
	}
}

bool CEOSAIUnit2ActionStack::UpdateStackPosition( CEOSAIUnit2ActionStackItem* pItem ) // Update the sorted position of this item
{
	bool bNeedToRepositionItem = false;
	POSITION ItemPos = NULL;
	POSITION pos = m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		ItemPos = pos;
		CEOSAIUnit2ActionStackItem* pItemInList = m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pItemInList == pItem )
		{
			// Check if the item is already in the correct position
			POSITION PointsAtItemPos;

			CEOSAIUnit2ActionStackItem* pPrevItem = NULL;
			PointsAtItemPos = ItemPos;
			m_ItemsSortedByArrivalTime.GetPrev( PointsAtItemPos );
			if( PointsAtItemPos ){ pPrevItem = m_ItemsSortedByArrivalTime.GetAt( PointsAtItemPos ); }

			CEOSAIUnit2ActionStackItem* pNextItem = NULL;
			PointsAtItemPos = pos;
			if( PointsAtItemPos ){ pNextItem = m_ItemsSortedByArrivalTime.GetAt( PointsAtItemPos ); }

			if( pPrevItem && pPrevItem->m_fArrivalTime > pItem->m_fArrivalTime )
			{
				// pItem is in the wrong position
				bNeedToRepositionItem = true;
			}
			if( pNextItem && pItem->m_fArrivalTime > pNextItem->m_fArrivalTime )
			{
				// pItem is in the wrong position
				bNeedToRepositionItem = true;
			}
		}
	}
	if( bNeedToRepositionItem )
	{
		m_ItemsSortedByArrivalTime.RemoveAt( ItemPos );
		m_ItemsSortedByArrivalTime.InsertSorted( pItem );
	}
	return bNeedToRepositionItem;
}

long CEOSAIUnit2ActionStack::GetNumberOfAllocatedItems()
{
	long iAllocatedItems = 0;
	POSITION pos = m_ItemsSortedByArrivalTime.GetTailPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pItem = m_ItemsSortedByArrivalTime.GetPrev( pos );
		//if( pItem->ActionIdeaIsAllocated() ) iAllocatedItems++;
		if( pItem->ActorIsAllocatedByThisTaskForce() ) iAllocatedItems++;
	}
	return iAllocatedItems;
}

CEOSAIUnit2ActionStackItem*  CEOSAIUnit2ActionStack::GetLastAllocatedItem()
{
	POSITION pos = m_ItemsSortedByArrivalTime.GetTailPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pItem = m_ItemsSortedByArrivalTime.GetPrev( pos );
		//if( pItem->IsReallyAllocated() || pItem->IsHypotheticallyAllocated() ) return pItem;
		//if( pItem->ActionIdeaIsAllocated() ) return pItem;
		ASSERT( pItem->m_iSanityCheck231987 == 231987 );
		if( pItem->ActorIsAllocatedByThisTaskForce() ) return pItem;
	}
	return NULL;
}

float CEOSAIUnit2ActionStack::GetLastAllocatedItemTime()
{
	CEOSAIUnit2ActionStackItem* pItem = GetLastAllocatedItem();
	if( pItem ){ return pItem->m_fArrivalTime; }
	return 1000000.0f;
}
/*
bool  CEOSAIUnit2ActionStack::InvolvesTransports()
{
	POSITION pos = m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			if( pStackItem->GetSuggestedTransportMoveDesc() )
			{
				return true;
			}
		}
	}
	return false;
}

float  CEOSAIUnit2ActionStack::GetLastShoreArrivalTime()
{
	float fLastArrivalTime = 0.0f;
	POSITION pos = m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			if( pStackItem->GetSuggestedTransportMoveDesc() )
			{
				CEOSAIUnit2TransportMovementDesc* pMovementDesc = pStackItem->GetSuggestedTransportMoveDesc();

				float fTime = pMovementDesc->m_fTotalTime;//>m_pAITransportUnitActionIdea->GetDirectMoveToTargetTime();
				fLastArrivalTime = max( fLastArrivalTime, fTime );
			}
		}
	}
	return fLastArrivalTime;
}
*/
void  CEOSAIUnit2ActionStack::ReleaseAllAllocations()
{
	// Clear the 'allocated' flag
	if( m_pParentTaskForce->IsBeingDeleted() )
	{
		ASSERT( false );
		return;
	}
	POSITION pos = m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_ItemsSortedByArrivalTime.GetNext( pos );
		//if( pStackItem->ActionIdeaIsAllocated() )
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			if( pStackItem->GetSuggestedTransportMoveDesc() )
			{
				int g=0;
			}
			pStackItem->Allocate( false ); // ReleaseAllocations();
			//ASSERT( pStackItem->GetSuggestedTransportMoveDesc() == NULL );
			//ASSERT( pStackItem->GetSuggestedTransportMoveDesc() == NULL );
		}
	}
}
/*
void  CEOSAIUnit2ActionStack::ReleaseAllocationsRelatedToAIUnit( CEOSAIUnit2* pAIUnit )
{
	POSITION pos = m_ItemsSortedByArrivalTime.GetTailPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pItem = m_ItemsSortedByArrivalTime.GetPrev( pos );
		if( pItem->m_pUnitActionIdea->GetAIUnitActor() == pAIUnit )
		{
			//pItem->ReleaseAllocations();
			pItem->Allocate( false );//>ReleaseAllocations();
		}
	}
}

void  CEOSAIUnit2ActionStack::DeleteStackItemsRelatedToAIUnit( CEOSAIUnit2* pAIUnit )
{
	POSITION pos = m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnit2ActionStackItem* pItem = m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pItem->m_pUnitActionIdea->GetAIUnitActor() == pAIUnit )
		{
			pItem->Allocate( false );//>ReleaseAllocations();
			m_ItemsSortedByArrivalTime.RemoveAt( prevPos );
			delete pItem;
		}
	}
}
*/

/*
void  CEOSAIUnit2ActionStack::CalculateSet( CEOSAIJobsToDo* pAIJobsToDo, CEOSAIMathFunction* pTimeValueFunction, float fAcceptableAttritionBalance )
{
	ASSERT( pAIJobsToDo->GetNumberOfActiveJobs() != 0 );

	// Clear the 'allocated' flag
	POSITION pos = m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pItem = m_ItemsSortedByArrivalTime.GetNext( pos );
		pItem->m_bAllocated = false;
	}

	// One by one, add each unit to the capability until the JobsToDo is satisfied.
	//   When it is finally satisfied, this is the minimum completion time.
	//   Then I can see about removing earlier units 
	//     Some of these earlier units: might not help things, might be overkill, or might have bad attrition
	//   Also, I can see about having cities fulfill some roles
	float fSufficientForcesTime = 0.0f;
	CEOSAIJobCapability  AIJobCapability;
	pos = m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pItem = m_ItemsSortedByArrivalTime.GetNext( pos );
		AIJobCapability.Add( pItem->m_pUnitTemplate );

		if( pAIJobsToDo->GetFulfillment01( &AIJobCapability ) >= 1.0f )
		{
			fSufficientForcesTime = pItem->m_fArrivalTime;
		}
	}

	// Get the time-value
/-*
	float fTimeValue = pTimeValueFunction->GetOutput( fCurrentTime );
	if( fTimeValue == 0.0f )
	{
		fails - but maybe cities can solve this
	}

	int z=0;

	Score is a balance between ArrivalTime and Attrition.

	I can go back and check the attrition (if it has a combat-role), then 
		check if there is a later unit that has a better attrition
*-/
	// Go back through the list and attempt to remove non-essential units?
	// 

	// What about adjusting the task force to improve attrition?

	// What about adding city build-targets to the mix?
}
*/
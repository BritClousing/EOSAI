
#include "stdafx.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTask_Pickup.h"
#include "EOSAIUnitTask_Enter.h"
//#include "UnitTemplate.h"
//#include "Unit.h"
//#include "UnitOrder.h"
//#include "PoiPD.h"
//#include "AISuperTask.h"
//#include "Checkpoint.h"
//#include "AITransportDefinition.h"
#include "EOSAIAction.h"
#include "EOSAIUnitTask_Pickup.h"
#include "EOSAIWorldDistanceTool.h"
//#include "WorldDescServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitTask_Enter::UnitTask_Enter(  
							EOSAI::PoiMobile* pActor,
							CEOSAIPoiObject* pTargetToEnter,
							CEOSAIAction* pAIAction,
							EOSAI::PoiObjectState* pMyUnitsInitialState,
							CEOSAILocation JumpOffLocation ) : EOSAI::UnitTask( pActor, pAIAction )
//							CMultiRegion* pMultiRegion ) : EOSAI::UnitTask( pActor, pAIAction )
{
	//m_pAITransportDefinition = NULL;

	m_MemoryOfInitialState.Copy( pMyUnitsInitialState );
	//m_FinalState.SetWorldDesc( pWorldDesc );
	m_FinalState.Copy( pMyUnitsInitialState );
	//m_FinalState.SetLocationAndRegions( pMultiRegion );
	m_FinalState.SetLocationAndRegions( JumpOffLocation );
	m_FinalState.SetContainer( pTargetToEnter );

	m_pAssociatedPickupTask = NULL;
	m_pTargetToEnter = pTargetToEnter;

	//CString strText;
	//strText.Format( _T("EOSAI::UnitTask_Enter (%x) created"), this );
	//Checkpoint::Write( strText );
}

EOSAI::UnitTask_Enter::~UnitTask_Enter()
{
	//CString strText;
	//strText.Format( _T("EOSAI::UnitTask_Enter (%x) deleted"), this );
	//Checkpoint::Write( strText );
}


void EOSAI::UnitTask_Enter::UpdateFinalState()
{
	float fOldFinalTime = GetFinalState()->GetTime();
	CEOSAILocation FinalLocation = GetFinalLocation(); // preserve the FinalLocation (it gets overwritten)
	//long iFinalGeo = GetEndGeo();
	CEOSAIMultiRegion2* pFinalMultiRegion = GetFinalState()->GetMultiRegion();

	GetFinalState()->Copy( GetMemoryOfInitialState() );
	//GetFinalState()->SetTime( GetMemoryOfInitialState()->GetTime() + m_fDuration );
	m_fArrivalTime = GetMemoryOfInitialState()->GetTime() + m_fDuration;
	GetFinalState()->SetTime( fOldFinalTime );
	GetFinalState()->SetLocationAndRegions( FinalLocation, pFinalMultiRegion );
	GetFinalState()->SetContainer( m_pTargetToEnter );

	/*
	if( m_pAITransportDefinition )
	{
		m_pAITransportDefinition->UpdateTimesOfPickupTasks();
	}
	*/
	if( m_pAssociatedPickupTask )
	{
		ASSERT( false );
		m_pAssociatedPickupTask->UpdatePickupTime();
	}

	// If the end time changes, the score changes
	if( m_bIsTheFinalTaskInTheAction )//&& fOldFinalTime != GetEndTime() )
	{
		if( m_pAIAction )
		{
			m_pAIAction->UpdateFinalTime( m_FinalState.GetTime() );
			m_pAIAction->UpdateScore();
		}
		/*
		if( m_pDefinedAction )
		{
			m_pDefinedAction->UpdateFinalTime( GetEndTime() );
			//m_pDefinedAction->GetPoiMobileFinalState()->SetTime( GetEndTime() );
			//m_pDefinedAction->UpdateScore();
		}
		*/
	}
}

void EOSAI::UnitTask_Enter::CalculateTaskDistanceAndDuration()
{
	EOSAI::PoiMobile* pUnitSymbol = GetParentUnit();
	//
	ASSERT( m_MemoryOfInitialState.GetLocation().GetRealX() != 0.0f || m_MemoryOfInitialState.GetLocation().GetRealY() != 0.0f );
	ASSERT( m_FinalState.GetLocation().GetRealX() != 0.0f || m_FinalState.GetLocation().GetRealY() != 0.0f );

	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//m_fDistance = pWorldDescServer->GetPixelDistance( m_MemoryOfInitialState.GetLocation(), m_FinalState.GetLocation() );
	m_fDistance = g_pWorldDistanceTool->GetDistance( m_MemoryOfInitialState.GetLocation(), m_FinalState.GetLocation() );
	/*
	INCOMPLETE: This should be replaced by an actual path
	m_fDistance = pUnitSymbol->GetAIBrain()->GetAIRegionManager()->GetPixelDistance(
		m_MemoryOfInitialState.GetLocation(),
		m_FinalState.GetLocation(),
		pUnitSymbol->GetMovementType(), 0 );
	*/
	if( m_fDistance == -1.0f )
	{
		m_bTaskIsValid = false;
		SetTaskDuration( 0.0f );
		return;
	}
	else
	{
		m_bTaskIsValid = true;
	}

	// DEBUG
	#ifdef _DEBUG
	if( m_fDistance <= 0.0f )
	{
		ASSERT( false );
		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		//m_fDistance = pWorldDescServer->GetPixelDistance( m_MemoryOfInitialState.GetLocation(), m_FinalState.GetLocation() );
		/*
		INCOMPLETE: This should be replaced by an actual path
		m_fDistance = pUnitSymbol->GetAIBrain()->GetAIRegionManager()->GetPixelDistance(
			m_MemoryOfInitialState.GetLocation(),
			m_FinalState.GetLocation(),
			pUnitSymbol->GetMovementType(), 0 );
		*/
	}
	#endif _DEBUG

	m_fDuration = m_fDistance / pUnitSymbol->GetMovementRate();
	m_fArrivalTime = m_MemoryOfInitialState.GetTime() + m_fDuration;

	ASSERT( m_fDistance > 0.0f );
	ASSERT( GetTaskDuration() > 0.0f );

	UpdateFinalState();
}
/*
void EOSAI::UnitTask_Enter::CreateOrders()
{
	ASSERT( m_bOrdersWereCreated == false );

	if( GetParentUnit()->GetServerPoiMobile() )
	{
		CPoiMobile* pUnit = GetParentUnit()->GetServerPoiMobile();

		//EOSAI::UnitTask_Pickup* pPickupTask = NULL;
		EOSAI::UnitTask_Pickup* pPickupTask = m_pAssociatedPickupTask;
		//if( m_pAITransportDefinition )
		{
			//pPickupTask = m_pAITransportDefinition->m_pPickupTask;
			if( pPickupTask )
			{
				if( pPickupTask->GetPickupPoiPD() == NULL )
				{
					return;
				}
				ASSERT( pPickupTask && pPickupTask->GetPickupPoiPD() );

				ASSERT( m_pUnitOrder == NULL );
				m_pUnitOrder = new CUnitOrder();
				m_pUnitOrder->SetWorldDesc( pUnit->GetWorldDesc() );
				m_pUnitOrder->SetParent( pUnit->ObjectId() );
				m_pUnitOrder->Enter( pPickupTask->GetPickupPoiPD()->ObjectId() );
				pUnit->AppendOrder( m_pUnitOrder );
				if( m_pUnitOrder->CalculateAStar() == false )
				{
					ASSERT( false );
					pUnit->DeleteOrder( m_pUnitOrder );
					m_bOrdersWereCreated = false;
					return;
				}
			}
			else
			{
				ASSERT( m_pUnitOrder == NULL );
				m_pUnitOrder = new CUnitOrder();
				m_pUnitOrder->SetWorldDesc( pUnit->GetWorldDesc() );
				m_pUnitOrder->SetParent( pUnit->ObjectId() );
				//m_pUnitOrder->MoveTo( m_FinalState.m_Location.GetRealX(), m_FinalState.m_Location.GetRealY() );
				m_pUnitOrder->Enter( m_pTargetToEnter->GetObjectId() );
				pUnit->AppendOrder( m_pUnitOrder );
				if( m_pUnitOrder->CalculateAStar() == false )
				{
					ASSERT( false );
					pUnit->DeleteOrder( m_pUnitOrder );
					m_bOrdersWereCreated = false;
					return;
				}
			}
		}
	}
	m_bOrdersWereCreated = true;
}
*/
/*
void EOSAI::UnitTask_Enter::TurnPrerequisiteTasksIntoOrders()
{
	if( m_pAssociatedPickupTask )
	{
		// This task-order must be created before the EnterTask can be turned into orders
		if( m_pAssociatedPickupTask->OrdersWereCreated() == false &&
			m_pAssociatedPickupTask->GetParentUnit() )
		{
			m_pAssociatedPickupTask->GetParentUnit()->TurnTasksIntoOrders();
		}
	}
}
*/

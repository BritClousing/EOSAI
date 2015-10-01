
#include "stdafx.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTask_Exit.h"
#include "EOSAIUnitTask_Dropoff.h"
//#include "UnitTemplate.h"
//#include "Unit.h"
//#include "UnitOrder.h"
//#include "PoiPD.h"
//#include "AISuperTask.h"
//#include "AITransportDefinition.h"
#include "EOSAIAction.h"
#include "EOSAIUnitTask_Dropoff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitTask_Exit::UnitTask_Exit( 
					EOSAI::PoiMobile* pActor,
					EOSAI::PoiMobile* pUnitToEnter,
					CEOSAIAction* pAIAction,
					EOSAI::PoiObjectState* pMyUnitsInitialState,
					CEOSAILocation JumpOffLocation ) : EOSAI::UnitTask( pActor, pAIAction )
					//CMultiRegion* pMultiRegion ) : EOSAI::UnitTask( pActor, pAIAction )
{
	//m_pAITransportDefinition = NULL;
	m_pAssociatedDropoffTask = NULL;

	//CWorldDesc* pWorldDesc = pActor->GetAIPlayer()->GetWorldDesc();
	//m_InitialState.SetWorldDesc( pWorldDesc );
	m_MemoryOfInitialState.Copy( pMyUnitsInitialState );
	//m_FinalState.SetWorldDesc( pWorldDesc );
	m_FinalState.Copy( pMyUnitsInitialState );
	//m_FinalState.SetLocationAndRegions( pMultiRegion );
	m_FinalState.SetLocationAndRegions( JumpOffLocation );
	m_FinalState.SetContainer( NULL );

	//m_InitialLocation = InitialLocation;
	//m_InitialState.m_Location = InitialLocation;
	//m_FinalLocation = TargetLocation;
	//m_FinalState.m_Location = TargetLocation;

	//m_pUnitToEnter = pUnitToEnter;
	//m_pDropoffTask = pDropoffTask;
}

void EOSAI::UnitTask_Exit::UpdateFinalState()
{
	float fOldFinalTime = GetFinalState()->GetTime();
	CEOSAILocation FinalLocation = GetFinalLocation(); // preserve the FinalLocation (it gets overwritten)
	//long iFinalGeo = GetEndGeo();
	CEOSAIMultiRegion2* pFinalMultiRegion = GetFinalState()->GetMultiRegion();

	GetFinalState()->Copy( GetMemoryOfInitialState() );
	GetFinalState()->SetTime( fOldFinalTime );
	GetFinalState()->SetLocationAndRegions( FinalLocation, pFinalMultiRegion );
	GetFinalState()->SetContainer( NULL );

	/*
	if( m_pAITransportDefinition )
	{
		m_pAITransportDefinition->UpdateTimesOfDropoffTasks();// m_pActor );
	}
	*/
	if( m_pAssociatedDropoffTask )
	{
		m_pAssociatedDropoffTask->UpdateDropoffTime();
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

void EOSAI::UnitTask_Exit::CalculateTaskDistanceAndDuration()
{
	EOSAI::PoiMobile* pUnitSymbol = GetParentUnit();
	//
	m_fDistance = 0.0f;
	m_fDuration = 0.0f;
	/*
	ASSERT( m_InitialState.GetLocation().GetRealX() != 0.0f || m_InitialState.GetLocation().GetRealY() != 0.0f );
	ASSERT( m_FinalState.GetLocation().GetRealX() != 0.0f || m_FinalState.GetLocation().GetRealY() != 0.0f );
	m_fDistance = pUnitSymbol->GetAIPlan()->GetSpatialMap()->GetPixelDistance(
		m_InitialState.GetLocation(),
		m_FinalState.GetLocation(),
		pUnitSymbol->GetMovementType(), 0 );

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
	if( m_fDistance == 0.0f )
	{
		m_fDistance = pUnitSymbol->GetAIPlan()->GetSpatialMap()->GetPixelDistance(
			m_InitialState.GetLocation(),
			m_FinalState.GetLocation(),
			pUnitSymbol->GetMovementType(), 0 );
	}
	#endif _DEBUG
	*/

	//SetTaskDuration( m_fDistance / pUnitSymbol->GetMovementRate() );
	//ASSERT( m_fDistance > 0.0f );
	//ASSERT( m_fTaskDuration > 0.0f );

	// The end time is whenever I get dropped off
	//ASSERT( m_pDropoffTask );
	m_fDuration = 0.0f;
	//m_FinalState.SetTime( m_pDropoffTask->GetFinalState()->GetTime() );
	m_fArrivalTime = m_FinalState.GetTime();

	UpdateFinalState();
}
/*
void EOSAI::UnitTask_Exit::CreateOrders()
{
	ASSERT( m_bOrdersWereCreated == false );

	if( GetParentUnit()->GetServerPoiMobile() )
	{
		CPoiMobile* pUnit = GetParentUnit()->GetServerPoiMobile();

		//if( m_pAITransportDefinition )
		{
			//EOSAI::UnitTask_Dropoff* pDropoffTask = m_pAITransportDefinition->m_pDropoffTask;
			EOSAI::UnitTask_Dropoff* pDropoffTask = m_pAssociatedDropoffTask;
			if( pDropoffTask )
			{
				if( pDropoffTask->GetDropoffPoiPD() == NULL )
				{
					return;
				}
				ASSERT( pDropoffTask && pDropoffTask->GetDropoffPoiPD() );

				ASSERT( m_pUnitOrder == NULL );
				m_pUnitOrder = new CUnitOrder();
				m_pUnitOrder->SetWorldDesc( pUnit->GetWorldDesc() );
				m_pUnitOrder->SetParent( pUnit->ObjectId() );
				m_pUnitOrder->Exit( pDropoffTask->GetDropoffPoiPD()->ObjectId() );
				pUnit->AppendOrder( m_pUnitOrder );
				/*
				// There is no A-star needed for "exit" tasks
				if( m_pUnitOrder->CalculateAStar() == false )
				{
					pUnit->DeleteOrder( m_pUnitOrder );
				}
				*-/
			}
			/-*
			ASSERT( m_pUnitOrder == NULL );
			m_pUnitOrder = new CUnitOrder();
			m_pUnitOrder->SetContext( pUnit->GetWorldDesc() );
			m_pUnitOrder->SetParent( pUnit->ObjectId() );
			m_pUnitOrder->MoveTo( m_FinalState.m_Location.GetRealX(), m_FinalState.m_Location.GetRealY() );
			pUnit->AppendOrder( m_pUnitOrder );
			if( m_pUnitOrder->CalculateAStar() == false )
			{
				pUnit->DeleteOrder( m_pUnitOrder );
			}
			*-/
		}
	}
	m_bOrdersWereCreated = true;
}
*/
/*
void EOSAI::UnitTask_Exit::TurnPrerequisiteTasksIntoOrders()
{
	if( m_pAssociatedDropoffTask )
	{
		// This task-order must be created before the EnterTask can be turned into orders
		if( m_pAssociatedDropoffTask->OrdersWereCreated() == false &&
			m_pAssociatedDropoffTask->GetParentUnit() )
		{
			m_pAssociatedDropoffTask->GetParentUnit()->TurnTasksIntoOrders();
		}
	}
}
*/
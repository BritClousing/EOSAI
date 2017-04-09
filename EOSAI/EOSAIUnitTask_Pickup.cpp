
#include "stdafx.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTask_Pickup.h"
//#include "UnitTemplate.h"
#include "EOSAIUnit.h"
//#include "UnitOrder.h"
//#include "AISuperTask.h"
//#include "WorldDesc.h"
#include "EOSAIUnitTask_Enter.h"
//#include "AITransportDefinition.h"
#include "EOSAIAction.h"
#include "EOSAIUnitTask_Enter.h"
//#include "WorldDescServer.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitTask_Pickup::UnitTask_Pickup( 
					EOSAI::PoiMobile* pActor,
					EOSAI::PoiMobile* pUnitToPickup,
					CEOSAIAction* pAIAction,
					EOSAI::PoiObjectState* pMyUnitsInitialState,
					CEOSAILocation WaterPickupLocation ) : EOSAI::UnitTask( pActor, pAIAction )
					//CEOSAIRegion2* pWaterAIRegion ) : EOSAI::UnitTask( pActor, pAIAction )
					//CMultiRegion* pMultiRegion ) : EOSAI::UnitTask( pActor, pAIAction )
{
	//m_pAITransportDefinition = NULL;
	m_pAssociatedEnterTask = NULL;

	m_MemoryOfInitialState.Copy( pMyUnitsInitialState );
	m_FinalState.Copy( pMyUnitsInitialState );
	m_FinalState.SetLocationAndRegions( WaterPickupLocation );
	//m_FinalState.SetLocationAndRegions( pWaterAIRegion );
	//m_FinalState.SetLocationAndMultiRegion( pMultiRegion );
	m_FinalState.AddContainee( pUnitToPickup );

	m_pUnitToPickup = pUnitToPickup;
	m_pPickupPoint = NULL;
}

void EOSAI::UnitTask_Pickup::UpdateFinalState()
{
	float fOldFinalTime = GetFinalState()->GetTime();
	CEOSAILocation FinalLocation = GetFinalLocation(); // preserve the FinalLocation (it gets overwritten)
	//long iFinalGeo = GetEndGeo();
	CEOSAIMultiRegion2* pFinalMultiRegion = GetFinalState()->GetMultiRegion();

	GetFinalState()->Copy( GetMemoryOfInitialState() );
	m_fArrivalTime = GetMemoryOfInitialState()->GetTime() + m_fDuration;
	GetFinalState()->SetTime( fOldFinalTime );
	GetFinalState()->SetLocationAndRegions( FinalLocation, pFinalMultiRegion );
	GetFinalState()->AddContainee( m_pUnitToPickup );

	/*
	CAITransportDefinition* pTranDef = m_pAITransportDefinition; // debug;
	if( m_pAITransportDefinition )
	{
		int y=0;
		m_pAITransportDefinition->UpdateTimesOfPickupTasks();// m_pActor );
		int z=0;
	}
	*/
	if( m_pAssociatedEnterTask )
	{
		ASSERT( false );
		m_pAssociatedEnterTask->UpdatePickupTime();
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
			//m_pDefinedAction->UpdateScore();
		}
		*/
	}
}

void EOSAI::UnitTask_Pickup::CalculateTaskDistanceAndDuration()
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
	if( m_fDistance == 0.0f )
	{
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
	}
	#endif _DEBUG

	m_fDuration = m_fDistance / pUnitSymbol->GetMovementRate();
	m_fArrivalTime = m_MemoryOfInitialState.GetTime() + m_fDuration;

	ASSERT( m_fDistance > 0.0f );
	ASSERT( GetTaskDuration() > 0.0f );

	UpdateFinalState();
}
/*
void EOSAI::UnitTask_Pickup::SetEnterTask( EOSAI::UnitTask_Enter* pEnterTask ) // makes adjustments to the time
{
	// Update the Task progression if (1) I don't have an m_pEnterTask, or (2) the enter task information
	//   has changed.
	if( m_pEnterTask )
	{
		float fTaskCompleteTime = max( m_pEnterTask->GetArrivalTime(), GetArrivalTime() );

		m_fWaitTime = fTaskCompleteTime - GetArrivalTime();
		if( fTaskCompleteTime != m_FinalState.GetTime() )
		{
			m_FinalState.SetTime( fTaskCompleteTime );

			// Tell the counterpart to update
			m_pEnterTask->m_pActor->UpdateTaskProgression();
		}
		m_pEnterTask = pEnterTask;
	}
	else
	{
		m_pEnterTask = pEnterTask;
		m_pActor->UpdateTaskProgression();
	}
}
*/
/*
void EOSAI::UnitTask_Pickup::CreateOrders()
{
	ASSERT( m_bOrdersWereCreated == false );
	m_bOrdersWereCreated = true;

	if( GetParentUnit() )
	{
		//CPoiMobile* pUnit = GetParentUnit()->GetServerPoiMobile();

		ASSERT( m_pUnitOrder == NULL );
		m_pUnitOrder = new CUnitOrder();
		m_pUnitOrder->SetWorldDesc( pUnit->GetWorldDesc() );
		m_pUnitOrder->SetParent( pUnit->ObjectId() );
		m_pUnitOrder->MoveTo( m_FinalState.GetLocation().GetRealX(), m_FinalState.GetLocation().GetRealY() );
		m_pUnitOrder->CreateChildPoiPD();
		//m_pUnitOrder->AddAIRegionToPermittedPath( &m_AIRegionsPermittedPath );
		pUnit->AppendOrder( m_pUnitOrder );
		if( m_pUnitOrder->CalculateAStar() )
		{
			m_pPickupPoint = m_pUnitOrder->GetChildPoiPD();
		}
		else
		{
			ASSERT( false );
			pUnit->DeleteOrder( m_pUnitOrder );
		}
	}
}
*/

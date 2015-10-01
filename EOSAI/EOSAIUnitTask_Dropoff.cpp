
#include "stdafx.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTask_Dropoff.h"
//#include "UnitTemplate.h"
//#include "Unit.h"
//#include "UnitOrder.h"
//#include "AITransportDefinition.h"
#include "EOSAIAction.h"
#include "EOSAIUnitTask_Exit.h"
//#include "WorldDescServer.h"
#include "EOSAIWorldDistanceTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitTask_Dropoff::UnitTask_Dropoff(
						EOSAI::PoiMobile* pTransportActor,
						EOSAI::PoiMobile* pUnitToDropoff,
						CEOSAIAction* pAIAction,
						EOSAI::PoiObjectState* pMyUnitsPreviousState,
						CEOSAILocation WaterDropoffLocation ) : EOSAI::UnitTask( pTransportActor, pAIAction )
						//CEOSAIRegion2* pAIRegion ) : EOSAI::UnitTask( pTransportActor, pAIAction )
						//CMultiRegion* pMultiRegion ) : EOSAI::UnitTask( pTransportActor, pAIAction )
{
	//m_pAITransportDefinition = NULL;
	m_pAssociatedExitTask = NULL;

	//CWorldDesc* pWorldDesc = pActor->GetAIPlayer()->GetWorldDesc();
	//m_MemoryOfInitialState.SetWorldDesc( pWorldDesc );
	m_MemoryOfInitialState.Copy( pMyUnitsPreviousState );
	//m_FinalState.SetWorldDesc( pWorldDesc );
	m_FinalState.Copy( pMyUnitsPreviousState );
	m_FinalState.SetLocationAndRegions( WaterDropoffLocation );
	//m_FinalState.SetLocationAndRegions( pAIRegion );
	m_FinalState.RemoveContainee( pUnitToDropoff );

	//m_InitialLocation = InitialLocation;
	//m_InitialState.m_Location = InitialLocation;
	//m_FinalLocation = TargetLocation;
	//m_FinalState.m_Location = TargetLocation;

	m_pUnitToDropoff = pUnitToDropoff;
	m_pDropoffPoint = NULL;
}

void EOSAI::UnitTask_Dropoff::UpdateFinalState()
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
	GetFinalState()->RemoveContainee( m_pUnitToDropoff );

	/*
	if( m_pAITransportDefinition )
	{
		m_pAITransportDefinition->UpdateTimesOfDropoffTasks();// m_pActor );
	}
	*/
	if( m_pAssociatedExitTask )
	{
		m_pAssociatedExitTask->UpdateDropoffTime();
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

	/*
	if( fOldFinalTime != GetFinalState()->GetTime() )
	{
		m_pAITransportDefinition->UpdateDropoffTime( GetFinalState()->GetTime() );
	}
	*/
}

void EOSAI::UnitTask_Dropoff::CalculateTaskDistanceAndDuration()
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
void EOSAI::UnitTask_Dropoff::CreateOrders()
{
	ASSERT( m_bOrdersWereCreated == false );
	m_bOrdersWereCreated = true;

	if( GetParentUnit()->GetServerPoiMobile() )
	{
		CPoiMobile* pUnit = GetParentUnit()->GetServerPoiMobile();

		ASSERT( m_pUnitOrder == NULL );
		m_pUnitOrder = new CUnitOrder();
		m_pUnitOrder->SetWorldDesc( pUnit->GetWorldDesc() );
		m_pUnitOrder->SetParent( pUnit->ObjectId() );
		//m_pUnitOrder->MoveTo( m_FinalState.GetLocation().GetRealX(), m_FinalState.GetLocation().GetRealY() );
		m_pUnitOrder->MoveTo( m_FinalState.GetLocation() );
		m_pUnitOrder->CreateChildPoiPD();
		//m_pUnitOrder->AddAIRegionToPermittedPath( &m_AIRegionsPermittedPath );
		pUnit->AppendOrder( m_pUnitOrder );
		if( m_pUnitOrder->CalculateAStar() )
		{
			m_pDropoffPoint = m_pUnitOrder->GetChildPoiPD();
		}
		else
		{
			ASSERT( false );
			pUnit->DeleteOrder( m_pUnitOrder );
		}
	}
}
*/
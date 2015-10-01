
#include "stdafx.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTask_CaptureCitRes.h"
//#include "UnitTemplate.h"
//#include "Unit.h"
//#include "UnitOrder.h"
//#include "AISuperTask.h"
#include "City.h"
#include "EOSAIAction.h"
//#include "WorldDescServer.h"
#include "EOSAIWorldDistanceTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitTask_CaptureCitRes::UnitTask_CaptureCitRes(
							EOSAI::PoiMobile* pParentUnit,
							CEOSAIAction* pAIAction,
							//CAIDefinedAction* pDefinedAction,
							EOSAI::PoiObjectState* pMyUnitsPreviousState,
							CEOSAIPoiObject* pTargetCitRes ) : EOSAI::UnitTask( pParentUnit, pAIAction )
{
	m_MemoryOfInitialState.Copy( pMyUnitsPreviousState );
	// [Final State] is the same as [Initial State] + a new location
	m_FinalState.Copy( pMyUnitsPreviousState );
	m_FinalState.SetLocationAndRegions( pTargetCitRes->GetLocation() );

	m_pTargetCitRes = pTargetCitRes;
}

void EOSAI::UnitTask_CaptureCitRes::UpdateFinalState()
{
	float fOldFinalTime = GetEndTime();
	CEOSAILocation FinalLocation = GetFinalLocation(); // preserve the FinalLocation (it gets overwritten)
	//long iFinalGeo = GetEndGeo();
	CEOSAIMultiRegion2* pFinalMultiRegion = GetFinalState()->GetMultiRegion();

	GetFinalState()->Copy( GetMemoryOfInitialState() );
	GetFinalState()->SetTime( GetMemoryOfInitialState()->GetTime() + m_fDuration );
	GetFinalState()->SetLocationAndRegions( FinalLocation, pFinalMultiRegion );

	// If the end time changes, the score changes
	if( m_bIsTheFinalTaskInTheAction && fOldFinalTime != GetEndTime() )
	{
		if( m_pAIAction )
		{
			m_pAIAction->UpdateFinalTime( m_FinalState.GetTime() );
			m_pAIAction->UpdateScore();
		}
		/*
		if( m_pDefinedAction )
		{
			m_pDefinedAction->GetPoiMobileFinalState()->SetTime( GetEndTime() );
			m_pDefinedAction->UpdateScore();
		}
		*/
	}
}

void EOSAI::UnitTask_CaptureCitRes::CalculateTaskDistanceAndDuration()
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

	m_fDuration = m_fDistance / pUnitSymbol->GetMovementRate();
	m_fArrivalTime = m_MemoryOfInitialState.GetTime() + m_fDuration;

	UpdateFinalState();
}
/*
void EOSAI::UnitTask_CaptureCitRes::CreateOrders()
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
		m_pUnitOrder->Attack( GetTargetCitRes()->GetObjectId() );
		pUnit->AppendOrder( m_pUnitOrder );
		if( m_pUnitOrder->CalculateAStar() == false )
		{
			pUnit->DeleteOrder( m_pUnitOrder );
		}
	}
}
*/
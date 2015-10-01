
#include "stdafx.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTask_MoveTo.h"
//#include "UnitTemplate.h"
//#include "Unit.h"
//#include "UnitOrder.h"
//#include "WorldDesc.h"
//#include "AISuperTask.h"
#include "EOSAIAction.h"
//#include "WorldDescServer.h"
//#include "Checkpoint.h"
#include "EOSAIWorldDistanceTool.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitTask_MoveTo::UnitTask_MoveTo(  
							EOSAI::PoiMobile* pActor,
							CEOSAIAction* pAIAction,
							EOSAI::PoiObjectState* pMyUnitsInitialState,
							CEOSAILocation TargetLocation ) : EOSAI::UnitTask( pActor, pAIAction )
{
	m_MemoryOfInitialState.Copy( pMyUnitsInitialState );
	//m_FinalState.SetWorldDesc( pWorldDesc );
	m_FinalState.Copy( pMyUnitsInitialState );
	//m_FinalState.SetLocationAndRegions( pMultiRegion );
	m_FinalState.SetLocationAndRegions( TargetLocation );
	//m_FinalState.SetContainer( pTargetToEnter );

	//CString strText;
	//strText.Format( _T("EOSAI::UnitTask_MoveTo (%x) created"), this );
	//Checkpoint::Write( strText );
}

EOSAI::UnitTask_MoveTo::~UnitTask_MoveTo()
{
	//CString strText;
	//strText.Format( _T("EOSAI::UnitTask_MoveTo (%x) deleted"), this );
	//Checkpoint::Write( strText );
}

void EOSAI::UnitTask_MoveTo::UpdateFinalState()
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
	//GetFinalState()->SetContainer( m_pTargetToEnter );

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

void EOSAI::UnitTask_MoveTo::CalculateTaskDistanceAndDuration()
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
void EOSAI::UnitTask_MoveTo::CreateOrders()
{
	ASSERT( m_bOrdersWereCreated == false );

	if( GetParentUnit()->GetServerPoiMobile() )
	{
		CPoiMobile* pUnit = GetParentUnit()->GetServerPoiMobile();

		ASSERT( m_pUnitOrder == NULL );
		m_pUnitOrder = new CUnitOrder();
		m_pUnitOrder->SetWorldDesc( pUnit->GetWorldDesc() );
		m_pUnitOrder->SetParent( pUnit->ObjectId() );
		//m_pUnitOrder->MoveTo( m_FinalState.m_Location.GetRealX(), m_FinalState.m_Location.GetRealY() );
		m_pUnitOrder->MoveTo( GetFinalState()->GetLocation() );
		pUnit->AppendOrder( m_pUnitOrder );
		if( m_pUnitOrder->CalculateAStar() == false )
		{
			pUnit->DeleteOrder( m_pUnitOrder );
		}
	}
	m_bOrdersWereCreated = true;
}
*/

/*
EOSAI::UnitTask_MoveTo::EOSAI::UnitTask_MoveTo( 
					EOSAI::PoiMobile* pParentUnit, 
					//CAISuperTask* pSuperTask, 
					CAIDefinedAction* pDefinedAction,
					//CEOSAILocation InitialLocation,
					EOSAI::PoiObjectState* pMyUnitsPreviousState,
					CEOSAILocation TargetLocation ) : EOSAI::UnitTask( pParentUnit, pDefinedAction )
{
	//CWorldDesc* pWorldDesc = pParentUnit->GetAIPlayer()->GetWorldDesc();
	//m_InitialState.SetWorldDesc( pWorldDesc );
	m_InitialState.Copy( pMyUnitsPreviousState );
	//m_InitialState.SetLocationAndGeo( InitialLocation );
	//m_FinalState.SetWorldDesc( pWorldDesc );
	m_FinalState.Copy( pMyUnitsPreviousState );
	m_FinalState.SetLocationAndGeo( TargetLocation );
	int z=0;
}

void EOSAI::UnitTask_MoveTo::UpdateFinalState()
{
	float fOldFinalTime = GetEndTime();
	CEOSAILocation FinalLocation = GetFinalLocation(); // preserve the FinalLocation (it gets overwritten)
	long iFinalGeo = GetEndGeo();

	GetFinalState()->Copy( GetStartState() );
	GetFinalState()->SetTime( GetStartState()->GetTime() + m_fDuration );
	GetFinalState()->SetLocationAndGeo( FinalLocation, iFinalGeo );

	// If the end time changes, the score changes
	if( m_bIsTheFinalTaskInTheAction && fOldFinalTime != GetEndTime() )
	{
		/*
		if( m_pDefinedAction )
		{
			m_pDefinedAction->GetPoiMobileFinalState()->SetTime( GetEndTime() );
			m_pDefinedAction->UpdateScore();
		}
		*-/
	}
}

void EOSAI::UnitTask_MoveTo::CalculateTaskDistanceAndDuration()
{
	EOSAI::PoiMobile* pUnitSymbol = GetParentUnit();
	//
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
	if( m_fDistance <= 0.0f )
	{
		ASSERT( false );
		m_fDistance = pUnitSymbol->GetAIPlan()->GetSpatialMap()->GetPixelDistance(
			m_InitialState.GetLocation(),
			m_FinalState.GetLocation(),
			pUnitSymbol->GetMovementType(), 0 );
	}
	#endif _DEBUG

	m_fDuration = m_fDistance / pUnitSymbol->GetMovementRate();
	m_fArrivalTime = m_InitialState.GetTime() + m_fDuration;

	ASSERT( m_fDistance > 0.0f );
	ASSERT( GetTaskDuration() > 0.0f );

	UpdateFinalState();
}

void EOSAI::UnitTask_MoveTo::CreateOrders()
{
	ASSERT( m_bOrdersWereCreated == false );
	m_bOrdersWereCreated = true;

	if( GetParentUnit()->GetPoiMobile() )
	{
		CPoiMobile* pUnit = GetParentUnit()->GetPoiMobile();

		ASSERT( m_pUnitOrder == NULL );
		m_pUnitOrder = new CUnitOrder();
		m_pUnitOrder->SetWorldDesc( pUnit->GetWorldDesc() );
		m_pUnitOrder->SetParent( pUnit->ObjectId() );
		m_pUnitOrder->MoveTo( m_FinalState.GetLocation().GetRealX(), m_FinalState.GetLocation().GetRealY() );
		pUnit->AppendOrder( m_pUnitOrder );
		//pOrder->CalculateAStar();
		if( m_pUnitOrder->CalculateAStar() == false )
		{
			pUnit->DeleteOrder( m_pUnitOrder );
		}
	}
}
*/

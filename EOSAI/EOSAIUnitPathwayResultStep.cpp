
#include "stdafx.h"
#include "EOSAIUnitPathwayResultStep.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAIUnitPathwayResult.h"
#include "EOSAIUnitPathwayFinder.h"
//#include "UnitTemplate.h"
#include "EOSAIUnit2.h"
#include "EOSAIMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitPathwayResultStep::UnitPathwayResultStep( EOSAI::UnitPathwayResult* pPathwayResult )
{
	ASSERT( pPathwayResult != NULL );
	m_pResult = pPathwayResult;//NULL;
	m_pPredefinedStep = NULL;
	//m_ePathType = enum_Undefined;
	m_eTask = enum_Undefined;
	//
	m_bIsValid = false;
	//
	m_fDistance = 0.0f;
	m_fDanger = 0.0f;
	m_fSighting = 0.0f;
	m_fStepTime = 0.0f;
	//
	m_fStartTimeToPickupTime = 0.0f;
	m_fDropoffTimeToTargetTime = 0.0f;
	//
	m_pAITarget = NULL;
	// GroundUnit information
	m_bTransportNeeded = false;
	//
	m_fSurvivalOdds = 1.0f;
	m_bResultsInDeath = false; // Self-Destruct or ran-out-of-fuel
	// Transportee-only information
//	m_pPickupStep = NULL;
//	m_pDropffStep = NULL;
	// Transport-only information
//	m_iEnterExitAIRegion = 0;
//	m_iPickupDropoffAIRegion = 0;
	//m_iPickupPoiPD = 0;
	//m_iDropoffPoiPD = 0;
	//
	m_pUnitOrder = NULL;
	m_fUnitOrderEndTime = 0.0f;
}

EOSAI::UnitPathwayResultStep::~UnitPathwayResultStep()
{
	int g=0;
}

CEOSAIUnit2PathwayFinder*  EOSAI::UnitPathwayResultStep::GetAIUnitPathwayFinder()
{
	return m_pResult->m_pAIUnitPathwayFinder;
}

void  EOSAI::UnitPathwayResultStep::SetPredefinedStep( CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep )
{
	m_pPredefinedStep = pPredefinedStep;

	m_StartLocation = pPredefinedStep->GetStartLocation();
	m_EndLocation = pPredefinedStep->GetEndLocation();

	if(      m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_Waypoint ){ m_eTask = enum_Waypoint; }
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_LandAtAirbase ){ m_eTask = enum_LandAtAirbase; }
	//
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_BuildAirfield ){ m_eTask = enum_BuildAirfield; }
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_Repair ){ m_eTask = enum_Repair; }
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_Upgrade ){ m_eTask = enum_Upgrade; }
	//
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_AttackUnit ){ m_eTask = enum_AttackUnit; }
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_CaptureCitRes ){ m_eTask = enum_CaptureCitRes; }
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_DegradeCityOrAirfield ){ m_eTask = enum_DegradeCityOrAirfield; }
	//
	/*
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_Transport )
	{
		// This task gets split into Pickup + Dropoff
		ASSERT( false );
	}
	*/
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_PickupTarget ){ m_eTask = enum_Pickup; }
	else if( m_pPredefinedStep->GetTask() == CEOSAIUnit2PathwayPredefinedStep::enum_DropoffTarget ){ m_eTask = enum_Dropoff; }
	else{ ASSERT( false ); }

	m_pAITarget = m_pPredefinedStep->GetAITarget();
}

EOSAI::UnitPathwayResultStep*  EOSAI::UnitPathwayResultStep::GetPrevStep()
{
	EOSAI::UnitPathwayResultStep* pPrevStep = NULL;
	POSITION pos = m_pResult->m_Steps.GetHeadPosition();
	while( pos )
	{
		EOSAI::UnitPathwayResultStep* pExistingStep = m_pResult->m_Steps.GetNext( pos );
		if( pExistingStep == this ){ return pPrevStep; }
		pPrevStep = pExistingStep;
	}
	return NULL;
}

EOSAI::UnitPathwayResultStep*  EOSAI::UnitPathwayResultStep::GetNextStep()
{
	POSITION pos = m_pResult->m_Steps.GetHeadPosition();
	while( pos )
	{
		EOSAI::UnitPathwayResultStep* pExistingStep = m_pResult->m_Steps.GetNext( pos );
		if( pExistingStep == this )
		{
			if( pos )
			{
				return m_pResult->m_Steps.GetNext( pos );
			}
			return NULL;
		}
	}
	return NULL;
}
/*
float EOSAI::UnitPathwayResultStep::GetStartTime()
{
	EOSAI::UnitPathwayResultStep* pPreviousStep = GetPrevStep();
	if( pPreviousStep )
	{
		return pPreviousStep->GetEndTime();
	}
	else
	{
		return GetAIUnitPathwayFinder()->GetStartTime();
	}
}
*/
/*
float EOSAI::UnitPathwayResultStep::GetGroundUnitTimeToPickupLocation() //GetPickupTime()
{
	ASSERT( this->m_pPredefinedStep->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsGroundUnit() );
	ASSERT( this->m_pPredefinedStep->RequiresTransport() );

	float fEndTime = m_pResult->m_pAIUnitPathwayFinder->GetStartTime();
	POSITION pos = m_pResult->m_Steps.GetHeadPosition();
	while( pos )
	{
		EOSAI::UnitPathwayResultStep* pResultStep = m_pResult->m_Steps.GetNext( pos );

		if( pResultStep == this )
		{
			fEndTime += m_fStartToPickupTime;
		}
		else
		{
			// If I am a GroundUnit using a Transport, then I have to wait for the transport to move me
			if( pResultStep->m_pPredefinedStep->GetTransportToUse() )
			{
				CEOSAIUnit2PathwayPredefinedStep* pTransportDropoffPredefinedStep = pResultStep->m_pPredefinedStep->GetTransportToUse()->GetAIUnitPathwayFinder()->GetTransportDropoffStep( pResultStep->m_pPredefinedStep );
				ASSERT( pTransportDropoffPredefinedStep );
				float fTransportDropoffEndTime = pTransportDropoffPredefinedStep->GetEndTime();

				//float fDropoffToTargetDistance =
				//	GetCommonState()->GetWorldDescServer()->GetPixelDistance( pTransportDropoffPredefinedStep->GetEndLocation()

				//float fThisStepEndTime = fTransportDropoffEndTime + m_fDropoffToTargetTime;
				//fCurrentTime += fTransportDropoffEndTime + pResultStep->m_fDropoffToTargetTime;
				fEndTime = fTransportDropoffEndTime + pResultStep->m_fDropoffToTargetTime;
				return fEndTime;
			}
			// If I am a Transport picking up a GroundUnit, then I have to wait for the GroundUnit to arrive
			if( pResultStep->m_eTask == enum_Pickup )
			{
				CEOSAIUnit2PathwayPredefinedStep* pGroundUnitPredefinedStep = pResultStep->m_pPredefinedStep->GetTransporteeStep();
				//float fEndTime = pGroundUnitPredefinedStep->GetRealOrEstimatedPickupTime();
				//CEOSAIUnit2PathwayPredefinedStep*  GetTransporteeStep(){ return m_pTransporteeStep; }
				fEndTime = max( fEndTime+pResultStep->m_fStepTime, pGroundUnitPredefinedStep->GetGroundUnitTimeToPickupLocation() );
			}
			// Otherwise
			else
			{
				fEndTime += pResultStep->m_fStepTime;
			}
		}
		if( pResultStep == this ) break;
	}
	return fEndTime;
}
*/

/*
float EOSAI::UnitPathwayResultStep::GetEndTime()
{
	// How to solve:
	//   Calculate the time taken by this step.  Add it to the time taken by the previous step.
	float fEndTime = 0.0f;
	CUnitTemplate* pUnitTemplate = GetAIUnitPathwayFinder()->GetUnitTemplate();

	// If this step uses a transport, then figure out the Transport-dropoff time + Dropoff->Target time
	if( m_pPredefinedStep && m_pPredefinedStep->GetTransportToUse() )
	{
		// Use the transport result path - in case this is an aircraft
		CEOSAIUnit2PathwayPredefinedStep* pTransportDropoffPredefinedStep = m_pPredefinedStep->GetTransportToUse()->GetAIUnitPathwayFinder()->GetTransportDropoffStep( m_pPredefinedStep );
		ASSERT( pTransportDropoffPredefinedStep );
		float fTransportDropoffEndTime = pTransportDropoffPredefinedStep->GetEndTime();
/-*
		EOSAI::UnitPathwayResultStep* pTransportDropoffResultStep = pTransportDropoffPredefinedStep->GetResultStep();
		if( pTransportDropoffResultStep )
		{
			fTransportDropoffEndTime = pTransportDropoffResultStep->GetEndTime();
		}
		else
		{
			fTransportDropoffEndTime = pTransportDropoffPredefinedStep->GetEndTime();
		}
*-/
		float fDropoffToTargetTime = GetGroundUnitDropoffTimeToTargetTime();
		/-*
		if( GetResultStep() )
		{
			fDropoffToTargetTime = GetResultStep()->GetGroundUnitDropoffTimeToTargetTime();
		}
		else
		{
			float fDropoffToTargetDistance = GetCommonState()->GetWorldDescServer()->GetPixelDistance( pTransportDropoffPredefinedStep->GetEndLocation(), this->GetEndLocation() );
			fDropoffToTargetTime = fDropoffToTargetDistance / (pUnitTemplate->GetMovementRate()+0.1f);
		}
		*-/

		return fTransportDropoffEndTime + fDropoffToTargetTime;
	}
	// If this is a transport, and I am picking-up a unit, check my arrival time and GroundUnit arrival time
	else if( this->IsAPickup() )
	{
		CEOSAIUnit2PathwayPredefinedStep* pGroundUnitStep = m_pPredefinedStep->GetTransporteeStep();
		float fGroundUnitArrivalTime = pGroundUnitStep->GetGroundUnitTimeToPickupLocation();

		float fTransportArrivalTime = 0.0f;
		EOSAI::UnitPathwayResultStep* pPreviousStep = GetPrevStep();
		if( pPreviousStep )
		{
			fTransportArrivalTime = this->GetStepTime() + pPreviousStep->GetEndTime();
		}
		else
		{
			fTransportArrivalTime = this->GetStepTime() + GetAIUnitPathwayFinder()->GetStartTime();
		}
		return max( fGroundUnitArrivalTime, fTransportArrivalTime );
	}
	else
	{
		EOSAI::UnitPathwayResultStep* pPreviousStep = GetPrevStep();
		if( pPreviousStep )
		{
			fEndTime = this->GetStepTime() + pPreviousStep->GetEndTime();
		}
		else
		{
			fEndTime = this->GetStepTime() + GetAIUnitPathwayFinder()->GetStartTime();
		}

		if( pUnitTemplate->IsAirUnit() && this->IsALandAtAirbase() )
		{
			fEndTime = CEOSAIMath::RoundUp( fEndTime );
		}
		return fEndTime;
	}
	/-*
	float fEndTime = m_pResult->m_pAIUnitPathwayFinder->GetStartTime();
	POSITION pos = m_pResult->m_Steps.GetHeadPosition();
	while( pos )
	{
		EOSAI::UnitPathwayResultStep* pResultStep = m_pResult->m_Steps.GetNext( pos );

		// If I am a GroundUnit using a Transport, then I have to wait for the transport to move me
		if( pResultStep->m_pPredefinedStep->GetTransportToUse() )
		{
			CEOSAIUnit2PathwayPredefinedStep* pTransportDropoffPredefinedStep = pResultStep->m_pPredefinedStep->GetTransportToUse()->GetAIUnitPathwayFinder()->GetTransportDropoffStep( pResultStep->m_pPredefinedStep );
			ASSERT( pTransportDropoffPredefinedStep );
			float fTransportDropoffEndTime = pTransportDropoffPredefinedStep->GetEndTime();

			//float fDropoffToTargetDistance =
			//	GetCommonState()->GetWorldDescServer()->GetPixelDistance( pTransportDropoffPredefinedStep->GetEndLocation()

			//float fThisStepEndTime = fTransportDropoffEndTime + m_fDropoffToTargetTime;
			//fCurrentTime += fTransportDropoffEndTime + pResultStep->m_fDropoffToTargetTime;
			fEndTime = fTransportDropoffEndTime + pResultStep->m_fDropoffToTargetTime;
			//return fEndTime;
		}
		// If I am a Transport picking up a GroundUnit, then I have to wait for the GroundUnit to arrive
		if( pResultStep->m_eTask == enum_Pickup )
		{
			CEOSAIUnit2PathwayPredefinedStep* pGroundUnitPredefinedStep = pResultStep->m_pPredefinedStep->GetTransporteeStep();
			//float fEndTime = pGroundUnitPredefinedStep->GetRealOrEstimatedPickupTime();
			//CEOSAIUnit2PathwayPredefinedStep*  GetTransporteeStep(){ return m_pTransporteeStep; }
			fEndTime = max( fEndTime+pResultStep->m_fStepTime, pGroundUnitPredefinedStep->GetGroundUnitTimeToPickupLocation() );
		}
		// Otherwise
		else
		{
			fEndTime += pResultStep->m_fStepTime;
		}

		if( pResultStep == this ) break;
	}
	return fEndTime;
	*-/
}
*/
/*
void EOSAI::UnitPathwayResultStep::UpdateWithTransportAssistedPath( CEOSAIUnit2* pAITransport )
{
	// Update the result - if it exists
	//    I put this code inside "UpdateResultStepWithTransportAssistedPath" because it requires
	//    special Sea/Air transport code
	//pAITransport->GetAIUnitPathwayFinder()->UpdateResultStepWithTransportAssistedPath( this );

	// Update Pickup/Dropoff
	pAITransport->GetAIUnitPathwayFinder()->UpdateTransportAssistedPath( m_pPredefinedStep );

	/-*
	EOSAI::UnitPathwayResult* pAIUnitPathwayResult = m_pAIUnitPathwayFinder->GetPathwayResult();
	if( pAIUnitPathwayResult )
	{
		POSITION pos = pAIUnitPathwayResult->m_Steps.GetHeadPosition();
		while( pos )
		{
			EOSAI::UnitPathwayResultStep* pResultStep = pAIUnitPathwayResult->m_Steps.GetNext( pos );
			if( pResultStep->m_pPredefinedStep == this )
			{
				bool bSuccess = m_pTransportToUse->GetAIUnitPathwayFinder()->UpdateResultStepWithTransportAssistedPath( pResultStep );
				ASSERT( bSuccess );
				//pResultStep->UpdateStepWithTransportAssistedPath( pTransportToUse );
			}
		}
	}
	*-/
}
*/
CEOSAILocation  EOSAI::UnitPathwayResultStep::GetGroundPickupLocation()
{
	//m_pPredefinedStep->GetRealTransport_PickupGroundAIRegion()
	CEOSAILocation WaterLocation, LandLocation;
	CEOSAIUnit2PathwayFinder::FindPickupDropoffLocations( 
		m_pPredefinedStep->GetRealTransport_PickupWaterAIRegion(),
		m_pPredefinedStep->GetRealTransport_PickupGroundAIRegion(),
		//pTransportPickupPredefinedStep->GetRealTransport_PickupWaterAIRegion(),
		//pTransportPickupPredefinedStep->GetRealTransport_PickupGroundAIRegion(),
		&WaterLocation, &LandLocation );
	return LandLocation;
}

/*
void EOSAI::UnitPathwayResultStep::UpdateStepWithTransportAssistedPath( CEOSAIUnit2* pTransport )
{
	bool bSuccess = pTransport->GetAIUnitPathwayFinder()->UpdateTransportResultStep( this );
	ASSERT( bSuccess );
}
*/

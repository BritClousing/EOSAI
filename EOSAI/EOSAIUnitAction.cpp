
#include "stdafx.h"
#include "EOSAIUnitAction.h"
//#include "AITacticalProject.h"
//#include "AIActionDefinition_CaptureCitRes.h"
#include "EOSAIUnit.h"
#include "PoiObjectState.h"
//#include "AIActionOutline.h"
#include "EOSAIMultiRegion2.h"
//
//#include "ResourceSource.h"
//
//#include "WorldDescServer.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAIDesireCaptureCitRes.h"
//#include "AITask_Enter.h"
//#include "AITask_Pickup.h"
//#include "AITask_Exit.h"
//#include "AITask_Dropoff.h"
//#include "AITransportDefinition.h"
#include "EOSAIUnitTask_CaptureCitRes.h"
#include "EOSAIUnitTask_Enter.h"
#include "EOSAIUnitTask_Pickup.h"
#include "EOSAIUnitTask_Exit.h"
#include "EOSAIUnitTask_Dropoff.h"
#include "EOSAIUnitTask_MoveTo.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAIResource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*
void CEOSAIUnitAction::CreateAITasksBasedOnAIActionOutline()
{
	//return;

	//
	POSITION pos = m_pActionOutline->m_PreMovementSteps.GetHeadPosition();
	while( pos )
	{
		CEOSAIActionStep* pStep = m_pActionOutline->m_PreMovementSteps.GetNext( pos );
		//
	}

	// Movement Pathway
	CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( m_pAIUnitActionIdea->m_pActor );
	ASSERT( pAIUnit );
	if( pAIUnit )
	{
		if( pAIUnit->IsGroundUnit() )
		{
			bool bAirTransport = false;
			bool bSeaTransport = true;
			if( m_pActionOutline->m_pTransportOrCarrier )
			{
				bAirTransport = m_pActionOutline->m_pTransportOrCarrier->IsAircraft();
				bSeaTransport = !bAirTransport;
			}

			//
			if( m_pActionOutline->InvolvesLandMovementOnly() )
			{
				// Nothing to do, go back
				/*
				I could create a movement task - it handles movement between MultiRegions
					it might be good to have that code in one place (rather than having
					movement code all over the place in every function)
					(Movement code involves finding the fastest way, the way that avoids danger,
					and i'm sure it does other things I can't think of right now)
				it would also allow me to setup a movementaction here
				*-/
				return;
			}
			//else
			//{
			//	CAITransportDefinition* pTransportDefinition = new CAITransportDefinition();
			//}
			//

			if( bSeaTransport )
			{
				//
				// There are some things to keep in mind with the ActionOutline.
				//   Imagine that we have a long, narrow island.  We want to move north.
				//   The action outline might recommend moving out of the west side of the 
				//   island to be picked up by a transport.  But, the east side is also good, 
				//   but it wasn't chosen.  Now, if the transport is on the east side of the 
				//   island, it's actually much better to pickup on the east side.  I need to
				//   be able to make these switches.  Maybe I need to set values on the coastal
				//   MultiRegions for both the ground unit and the transport unit.  The combined
				//   value determines the actual pickup MultiRegion.
				//

				long iGeoState = 0; // 0 = initial land, 1 = water, 2 = final land
				long iCurrentMultiRegion = 0;
				CMultiRegion* pPreviousMultiRegion = NULL;
				POSITION pos = m_pActionOutline->m_Pathway.GetHeadPosition();
				while( pos )
				{
					CMultiRegion* pMultiRegion = m_pActionOutline->m_Pathway.GetNext( pos );
					iCurrentMultiRegion++;

					bool bSwitchFromLandToWater = false;
					bool bSwitchFromWaterToLand = false;

					if( pMultiRegion->IsLand() || pMultiRegion->IsCity() )
					{
						//bIsOnGround = true;
						//if( iCurrentMultiRegion == 1 ){ bPrevIsOnGround = true; }
						if( iCurrentMultiRegion == 1 ){ iGeoState = 0; }
						if( iCurrentMultiRegion > 1 && iGeoState == 1 )
						{
							bSwitchFromWaterToLand = true;
							iGeoState = 2;
						}
					}
					else
					{
						//bIsOnGround = false;
						//if( iCurrentMultiRegion == 1 ){ bPrevIsOnGround = false; }
						if( iCurrentMultiRegion == 1 ){ iGeoState = 1; }
						if( iCurrentMultiRegion > 1 && iGeoState == 0 )
						{
							bSwitchFromLandToWater = true;
							iGeoState = 1;
						}
						if( iCurrentMultiRegion > 1 && iGeoState == 2 )
						{
							ASSERT( false );
						}
					}

					//
					if( bSwitchFromLandToWater )
					{
						ASSERT( m_pActionOutline->m_pTransportOrCarrier );
						CEOSAIUnit* pAITransport = m_pActionOutline->m_pTransportOrCarrier;

						// pPreviousMultiRegion is the previous land MultiRegion
						EOSAI::UnitTask_Enter* pEnterTransport = new EOSAI::UnitTask_Enter( pAIUnit, pAITransport, this, pAIUnit->GetFinalState(), pPreviousMultiRegion );
						EOSAI::UnitTask_Pickup* pPickup = new EOSAI::UnitTask_Pickup( pAITransport, pAIUnit, this, pAITransport->GetFinalState(), pMultiRegion );
						pEnterTransport->SetAssociatedPickupTask( pPickup );
						pPickup->SetAssociatedEnterTask( pEnterTransport );
					}
					if( bSwitchFromWaterToLand )
					{
						ASSERT( m_pActionOutline->m_pTransportOrCarrier );
						CEOSAIUnit* pAITransport = m_pActionOutline->m_pTransportOrCarrier;

						// pPreviousMultiRegion is the previous water MultiRegion
						EOSAI::UnitTask_Exit* pExitTransport = new EOSAI::UnitTask_Exit( pAIUnit, pAITransport, this, pAIUnit->GetFinalState(), pMultiRegion );
						EOSAI::UnitTask_Dropoff* pDropoff = new EOSAI::UnitTask_Dropoff( pAITransport, pAIUnit, this, pAITransport->GetFinalState(), pPreviousMultiRegion );
						pExitTransport->SetAssociatedDropoffTask( pDropoff );
						pDropoff->SetAssociatedExitTask( pExitTransport );
					}

					pPreviousMultiRegion = pMultiRegion;
				}
			}
		}
	}
	/*
	pos = m_pActionOutline->m_Pathway.GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pMultiRegion = m_pActionOutline->m_Pathway.GetNext( pos );

		if( m_pActionOutline->m_pTransportOrCarrier )
		{
		}
		int x=0;

		// Create movement tasks
		
	}
	*-/
}
*/

/*
void CEOSAIUnitAction_CaptureCitRes::CreateAndAppendAITasks()
{
	/-*  Temporarily disabled

	CEOSAIDesireCaptureCitRes* pDesire = dynamic_cast< CEOSAIDesireCaptureCitRes* >( m_pAIUnitActionIdea->GetAIDesire() );
	ASSERT( pDesire );
	CEOSAIPoiObject* pTargetAIPoiObject = pActionDef->m_pTarget;
	ASSERT( pTargetAIPoiObject );
	CEOSAICity* pTargetAICity = dynamic_cast< CEOSAICity* >( pTargetAIPoiObject );
	CEOSAIResource* pTargetAIResource = dynamic_cast< CEOSAIResource* >( pTargetAIPoiObject );
	ASSERT( pTargetAICity || pTargetAIResource );

	EOSAI::PoiMobile* pPoiMobileActor = dynamic_cast< EOSAI::PoiMobile* >( m_pAIUnitActionIdea->m_pActor );

	// Create the last task
	EOSAI::UnitTask_CaptureCitRes* pAIUnitTask_CaptureCitRes = 
		new EOSAI::UnitTask_CaptureCitRes( pPoiMobileActor, this, pPoiMobileActor->GetFinalState(), pTargetAIPoiObject );
	//pPoiMobileActor->AppendAITask( pAIUnitTask_CaptureCity );
	pPoiMobileActor->GetAIObjectActionProgression()->InsertAITask( pPoiMobileActor->GetFinalState(), pAIUnitTask_CaptureCitRes );

	*-/
}
*/
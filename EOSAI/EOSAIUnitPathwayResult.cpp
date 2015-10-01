
#include "stdafx.h"
#include "EOSAIUnitPathwayResult.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAIRegionManager2.h"
#include "EOSAIUnit2.h"
#include "EOSAIBrain.h"
//#include "Unit.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"
//#include "WorldDescPlayerProxy.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAITacticalProject2.h"
#include "EOSAICommonData2.h"
#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;
//#include "Checkpoint.h"
//#ifdef _DEBUG
//#include "WorldDescServer.h"
//#endif _DEBUG
extern CList< long >  g_HighlightAIRegions;
extern CList< long >  g_AIPathwayData;

//

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::UnitPathwayResult::~UnitPathwayResult()
{
	Clear();
}

void  EOSAI::UnitPathwayResult::Clear()
{
	//m_pAIUnitPathwayFinder = pPathwayFinder;
	m_bPathIsValid = false;
	m_fPathfindingValue = 1000000.0f;
	m_fCompletionTime = 1000000.0f;
	m_fAttackTime = 1000000.0f;
	m_bResultsInDeath = false;
	m_fSurvivalOdds = 1.0f;
	m_bAllOrdersWereCreated = false;

	while( m_Steps.IsEmpty() == FALSE ){ delete m_Steps.RemoveHead(); }
}

void EOSAI::UnitPathwayResult::UseTransport( CEOSAIUnit2* pAITransport )
{
	// I should try to insert pickup/dropoff tasks into the Results
	ASSERT( pAITransport->CanContain_IgnoreForeignRelations( m_pAIUnitPathwayFinder->GetAIUnitTemplate() ) );

	POSITION pos = m_Steps.GetHeadPosition();
	while( pos )
	{
		EOSAI::UnitPathwayResultStep* pTransporteeStep = m_Steps.GetNext( pos );
		if( pTransporteeStep->m_bTransportNeeded )
		{
			//pResultStep->m_ImaginarySeaTransport_PickupAIRegion
			//pResultStep->m_ImaginarySeaTransport_DropoffAIRegion

			// Lookup the usefulness of this Transport - can I fit it into the Transport's movement path?
			//   The algorithm will vary depending on whether this is a Sea or Air Transport
			//   Also, does this slow-down the other (existing) tasks?
			// float fUsefulness = pAITransport->GetTransportUsefulness( pTransporteeStep );

			// this won't work with the Pathways objects - I need to use each AIUnit's objects
			//
			//pAITransport->AddTransportAction( pTransporteeStep->m_pPredefinedStep );
			pTransporteeStep->m_pPredefinedStep->UseTransportAndCreatePickupDropoffSteps( pAITransport );
		}
	}
}

bool  EOSAI::UnitPathwayResult::CreateOrders()
{
	//Does this get called?
	//This is probably supposed to get called by some method in EOSAIPoiObject
	//ASSERT( false );
	return g_pEOSAIInterface->CreateAIUnitOrders( this );
}

#ifdef DO_NOT_COMPILE
bool  EOSAI::UnitPathwayResult::CreateOrders()
{
	//CWorldDescPlayer* pWorldDescPlayer = m_pAIUnitPathwayFinder->GetAIBrain()->GetWorldDescPlayer();
	//CWorldDescServer* pWorldDescServer = m_pAIUnitPathwayFinder->GetAIBrain()->GetWorldDescServer();
	//CWorldDescPlayerProxy* pWorldDescPlayerProxy = m_pAIUnitPathwayFinder->GetAIBrain()->GetWorldDescPlayerProxy();

	CEOSAIUnit2* pAIUnitActor = m_pAIUnitPathwayFinder->GetAIUnitActor();//>GetAIPoiObjectActor();
	//EOSAI::UnitPathwayResult* pPath = &m_SimpleRoute;
	//CEOSAIBrain* pAIBrain = pAIUnitActor->GetAIBrain();
	//long iAIPlayer = pAIBrain->GetAIPlayerNumber();
	long iAIPlayer = pAIUnitActor->GetOwner();

	ASSERT( m_pAIUnitPathwayFinder->GetStartLocation() == pAIUnitActor->GetInitialState()->GetLocation() );

	//CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( m_pActor );
	//if( m_pAIUnitActor == NULL ) return;
	CEOSAIUnit2* pUnit = pAIUnitActor;//->GetServerUnit();
	if( pUnit == NULL ) return false;
	pUnit->DeleteOrders();

	long iNumberOfOrdersCreated = 0;
	long iNumberOfOrdersFailed = 0;
	m_bAllOrdersWereCreated = false;

	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
	CEOSAILocation LastStepEnd = pAIUnitActor->GetInitialState()->GetLocation(); // LastStepEnd is for sanity check
	long iCurrentStep = 0;

/*
	// We want to check through the steps to figure out if I need to put any delays for simultaneous arrival
	POSITION pos = m_Steps.GetHeadPosition();
	while( pos )
	{
		EOSAI::UnitPathwayResultStep* pResultStep = m_Steps.GetNext( pos );
		if( pResultStep->m_pPredefinedStep &&
			pResultStep->m_pPredefinedStep->GetAIUnitActionIdea() &&
			pResultStep->m_pPredefinedStep->GetAIUnitActionIdea()->GetTacticalProject2() )
		{
			CEOSAITacticalProject2* pTacticalProject2 = pResultStep->m_pPredefinedStep->GetAIUnitActionIdea()->GetTacticalProject2();
			//long id = pTacticalProject2->m_Id;
			if( pTacticalProject2->IsASecretInvasion() )
			{
				int h=0;
			}
			if( pTacticalProject2->IsASecretInvasion() &&
				pTacticalProject2->GetTaskForceWithPredefinedSteps() )
			{
				float fCompletionTime = pTacticalProject2->GetTaskForceWithPredefinedSteps()->GetCompletionTime();
				int h=0;
			}
		}
	}
*/
	if( m_pAIUnitPathwayFinder->GetAIUnitActor() )
	{
		float fWaitTime = m_pAIUnitPathwayFinder->GetAIUnitActor()->GetInitialWaitTime();
		if( fWaitTime > 0.1f )
		{
			CUnitOrder* pNewOrder1 = new CUnitOrder();
			pNewOrder1->SetWorldDesc( pWorldDescServer );//pUnit->GetWorldDesc() );
			pNewOrder1->SetParent( pUnit->ObjectId() );
			pNewOrder1->Wait( 0.5f );//fWaitTime );
			//pResultStep->AddSupportUnitOrder( pNewOrder1 );
			pUnit->AppendOrder( pNewOrder1 );
		}
	}

	//POSITION pos = pPath->m_PathwaySteps.GetHeadPosition();
	POSITION pos = m_Steps.GetHeadPosition();
	while( pos )
	{
		//CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = m_Steps.GetNext( pos );
		EOSAI::UnitPathwayResultStep* pResultStep = m_Steps.GetNext( pos );

		bool bSimultaneousArrival = false;
		CEOSAITacticalProject2* pTacticalProject2 = NULL;
		if( pResultStep->m_pPredefinedStep &&
			pResultStep->m_pPredefinedStep->GetAIUnitActionIdea() &&
			pResultStep->m_pPredefinedStep->GetAIUnitActionIdea()->GetTacticalProject2() )
		{
			pTacticalProject2 = pResultStep->m_pPredefinedStep->GetAIUnitActionIdea()->GetTacticalProject2();
			if( pTacticalProject2->IsASecretInvasion() &&
				pTacticalProject2->GetTaskForceWithPredefinedSteps() )
			{
				bSimultaneousArrival = true;
				//float fCompletionTime = pTacticalProject2->GetTaskForceWithPredefinedSteps()->GetCompletionTime();
				//int h=0;
			}
		}

		if( pResultStep->m_bTransportNeeded )
		{
			// This can return false if this is a hypothetical (unbuilt) transport
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( pResultStep->m_pPredefinedStep->GetTransportToUse() );
			}
		}

		//
		if( pResultStep->m_pPredefinedStep && // Can be null if this is an AirMovement (land at airbase)
			pResultStep->m_pPredefinedStep->GetTransportToUse() )
		{
			// Make sure this transport has setup its orders
			CEOSAIUnit2* pAITransport = pResultStep->m_pPredefinedStep->GetTransportToUse();
			pAITransport->GetAIUnitPathwayFinder()->CreateOrders();

			bool bUsingHypotheticalTransport = false;
			CEOSAILocation TransporteePickupLocation;
			//
			CEOSAIUnit2PathwayPredefinedStep* pPredefinedTransportPickupStep = pAITransport->GetAIUnitPathwayFinder()->GetTransportPickupStep( pResultStep->m_pPredefinedStep );
			EOSAI::UnitPathwayResultStep* pResultTransportPickupStep = NULL;
			CUnitOrder* pTransportPickupOrder = NULL;
			if( pPredefinedTransportPickupStep ) // Not all transport tasks have a 'pickup'
			{
				pResultTransportPickupStep = pPredefinedTransportPickupStep->GetResultStep();
				if( pResultTransportPickupStep )
				{
					pTransportPickupOrder = pResultTransportPickupStep->GetUnitOrder();
					if( pTransportPickupOrder == NULL )
					{
						bUsingHypotheticalTransport = true;
						TransporteePickupLocation = pResultTransportPickupStep->GetGroundPickupLocation(); 
					}
				}
				else
				{
					if( Public::m_bAssertWhenAIPathwayFails ){ ASSERT( false ); }
				}
			}

			CEOSAIUnit2PathwayPredefinedStep* pPredefinedTransportDropoffStep = pAITransport->GetAIUnitPathwayFinder()->GetTransportDropoffStep( pResultStep->m_pPredefinedStep );
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( pPredefinedTransportDropoffStep );
			}
			if( pPredefinedTransportDropoffStep == NULL ) continue;
			EOSAI::UnitPathwayResultStep* pResultTransportDropoffStep = pPredefinedTransportDropoffStep->GetResultStep();
			if( pResultTransportDropoffStep == NULL )
			{
				if( Public::m_bAssertWhenAIPathwayFails ){ ASSERT( false ); }
				continue;
			}
			CUnitOrder* pTransportDropoffOrder = pResultTransportDropoffStep->GetUnitOrder();
			if( pTransportDropoffOrder == NULL )
			{
				bUsingHypotheticalTransport = true;
			}

			// Given a WaterAIRegion and a LandAIRegion, return Water and Land Locations for Pickup/Dropoff
			/*
			CEOSAILocation WaterLocation, LandLocation;
			CEOSAIUnit2PathwayFinder::FindPickupDropoffLocations( 
				pPredefinedTransportPickupStep->GetRealTransport_PickupWaterAIRegion(),
				pPredefinedTransportPickupStep->GetRealTransport_PickupLandAIRegion(),
				&WaterLocation, &LandLocation );
			*/

			if( bUsingHypotheticalTransport )
			{
				CUnitOrder* pNewOrder1 = new CUnitOrder();
				pNewOrder1->SetWorldDesc( pWorldDescServer );//pUnit->GetWorldDesc() );
				pNewOrder1->SetParent( pUnit->ObjectId() );
				pNewOrder1->MoveTo( TransporteePickupLocation );
				pResultStep->AddSupportUnitOrder( pNewOrder1 );
				pUnit->AppendOrder( pNewOrder1 );
				if( pNewOrder1->CalculateAStar() )
				{
					iNumberOfOrdersCreated++;
				}
				else
				{
					pUnit->DeleteOrder( pNewOrder1 );
					pNewOrder1 = NULL;
					iNumberOfOrdersFailed++;
					if( Public::m_bAssertWhenAIPathwayFails )
					{
						ASSERT( false );
					}
				}

				// Can't add any more orders after this one, so exit
				pos = NULL;
				continue;
			}

			// This will result in several orders
			if( pTransportPickupOrder )
			{
				ASSERT( pTransportPickupOrder->GetChildPoiPD() );

				CUnitOrder* pNewOrder1 = new CUnitOrder();
				pNewOrder1->SetWorldDesc( pWorldDescServer );//pUnit->GetWorldDesc() );
				pNewOrder1->SetParent( pUnit->ObjectId() );
				pNewOrder1->Enter( pTransportPickupOrder->GetChildPoiPD()->GetObjectId() );
				pUnit->AppendOrder( pNewOrder1 );
				if( pNewOrder1->CalculateAStar() )
				{
					pResultStep->AddSupportUnitOrder( pNewOrder1 );
					iNumberOfOrdersCreated++;
				}
				else
				{
					pUnit->DeleteOrder( pNewOrder1 );
					pNewOrder1 = NULL;
					iNumberOfOrdersFailed++;
					if( Public::m_bAssertWhenAIPathwayFails )
					{
						ASSERT( false );
					}
				}
			}

			// This will result in several orders
			{
				ASSERT( pTransportDropoffOrder );
				ASSERT( pTransportDropoffOrder->GetChildPoiPD() );
				if( pTransportDropoffOrder )
				{
					CUnitOrder* pNewOrder2 = new CUnitOrder();
					pNewOrder2->SetWorldDesc( pWorldDescServer );//pUnit->GetWorldDesc() );
					pNewOrder2->SetParent( pUnit->ObjectId() );
					pNewOrder2->Exit( pTransportDropoffOrder->GetChildPoiPD()->GetObjectId() );
					pResultStep->AddSupportUnitOrder( pNewOrder2 );
					pUnit->AppendOrder( pNewOrder2 );
					
					// Exit Orders don't need the AStar Calculation
					iNumberOfOrdersCreated++;
				}
				/*
				if( pNewOrder2->CalculateAStar() )
				{
					iNumberOfOrdersCreated++;
				}
				else
				{
					pUnit->DeleteOrder( pNewOrder2 );
					pNewOrder2 = NULL;
					iNumberOfOrdersFailed++;
				}
				*/
			}
		}
		//EOSAI::UnitPathwayResultStep* pResultStep = pPath->m_PathwaySteps.GetNext( pos );
		/*
		EOSAI::UnitPathwayResultStep* pResultStep = pPredefinedStep->GetResult( enum_FinalPath );
		if( pResultStep->m_bIsValid == false )
		{
			break; // this step isn't valid, and the subsequent steps won't be either
		}
		*/

		ASSERT( LastStepEnd == pResultStep->m_StartLocation );
		#ifdef _DEBUG
		if( LastStepEnd != pResultStep->m_StartLocation )
		{
			POSITION pos4 = m_pAIUnitPathwayFinder->GetResultPath()->m_Steps.GetHeadPosition();
			while( pos4 )
			{
				EOSAI::UnitPathwayResultStep* pResultStep = m_pAIUnitPathwayFinder->GetResultPath()->m_Steps.GetNext( pos4 );
				int hg=0;
			}
			CList< EOSAI::UnitPathwayResultStep* >* pResults = &m_pAIUnitPathwayFinder->GetResultPath()->m_Steps;
			int g=0;
			CList< CEOSAIUnit2PathwayPredefinedStep* >* pPredefinedList = m_pAIUnitPathwayFinder->GetPreDefinedPath();
			int g2=0;
		}
		#endif _DEBUG
		//CEOSAIUnit2PathwayPredefinedStep* pPoint = pResultStep->m_pPredefinedStep;
		//CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = pResultStep->m_pPredefinedStep;
		CWorldDescBase* pDEBUG_WorldDesc = pUnit->GetWorldDesc();

		/*
		CEOSAIUnit2PathwayFinder* pPathwayFinder = m_pAIUnitPathwayFinder;
		POSITION pos3 = pPathwayFinder->GetPreDefinedPath()->GetHeadPosition();
		while( pos3 )
		{
			CEOSAIUnit2PathwayPredefinedStep* p = pPathwayFinder->GetPreDefinedPath()->GetNext( pos3 );
			int g=0;
		}
		*/
		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

		CUnitOrder* pNewOrder = new CUnitOrder();
		pNewOrder->SetWorldDesc( pWorldDescServer );
		pNewOrder->SetParent( pUnit->ObjectId() );

		//
		CEOSAILocation LocationEnd = pResultStep->m_EndLocation;
		ASSERT( LocationEnd.GetRealX() != 0.0f || LocationEnd.GetRealY() != 0.0f );
		if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_Waypoint )
		{
			pNewOrder->MoveTo( LocationEnd.GetRealX(), LocationEnd.GetRealY() );
		}
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_LandAtAirbase )
		{
			ASSERT( m_pAIUnitPathwayFinder->GetUnitTemplate()->IsAirUnit() );
			pNewOrder->Enter( pResultStep->m_pAITarget->GetObjectId() );
		}
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_Repair )
		{
			pNewOrder->Enter( pResultStep->m_pAITarget->GetObjectId() );
		}
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_Upgrade )
		{
			// The upgrade order is a separate order
			pNewOrder->Enter( pResultStep->m_pAITarget->GetObjectId() );
			//pNewOrder->Upgrade( pResultStep->m_pAITarget->GetObjectId() );
		}
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_BuildAirfield )
		{
			ASSERT( m_pAIUnitPathwayFinder->GetAIUnitTemplate()->IsGroundUnit() );
			pNewOrder->BuildAirfield( LocationEnd );
		}
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_CaptureCitRes )
		{
			pNewOrder->Attack( pResultStep->m_pAITarget->GetObjectId() );
			pNewOrder->SetAttackGoal( EOSAIEnumAttackGoal_Capture );

			// DEBUG - I MUST either be at war with this player, or have 'sneak attack' setup
			long iTargetOwner = pResultStep->m_pAITarget->GetOwner();
			ASSERT(
				pWorldDescServer->GetForeignRelations( iAIPlayer, iTargetOwner ).IsEnemy() ||
				pWorldDescPlayerProxy->GetSneakAttack( iTargetOwner ) );
			// DEBUG
		}
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_AttackUnit )
		{
			//bool bIsABombing = pUnit->OneAttackPerLandingVs(pResultStep->m_pAITarget->GetServerPoiObject());
			bool bIsABombing = pUnit->OneAttackPerLandingVs( pResultStep->m_pAITarget );

			pNewOrder->Attack( pResultStep->m_pAITarget->GetObjectId() );
			pNewOrder->IsABombing( bIsABombing );
			pNewOrder->SetAttackGoal( EOSAIEnumAttackGoal_KillUnits );

			// DEBUG - I MUST either be at war with this player, or have 'sneak attack' setup
			long iTargetOwner = pResultStep->m_pAITarget->GetOwner();
			ASSERT(
				pWorldDescServer->GetForeignRelations( iAIPlayer, iTargetOwner ).IsEnemy() ||
				pWorldDescPlayerProxy->GetSneakAttack( iTargetOwner ) );
			// DEBUG
		}
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_DegradeCityOrAirfield )
		{
			//bool bIsABombing = pUnit->OneAttackPerLandingVs(pResultStep->m_pAITarget->GetServerPoiObject());
			bool bIsABombing = pUnit->OneAttackPerLandingVs( pResultStep->m_pAITarget );

			pNewOrder->Attack( pResultStep->m_pAITarget->GetObjectId() );
			pNewOrder->IsABombing( bIsABombing );
			pNewOrder->SetAttackGoal( EOSAIEnumAttackGoal_DegradeCity );

			// DEBUG - I MUST either be at war with this player, or have 'sneak attack' setup
			long iTargetOwner = pResultStep->m_pAITarget->GetOwner();
			ASSERT(
				pWorldDescServer->GetForeignRelations( iAIPlayer, iTargetOwner ).IsEnemy() ||
				pWorldDescPlayerProxy->GetSneakAttack( iTargetOwner ) );
			// DEBUG
		}

		// Pickup/Dropoff Orders
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_Pickup )
		{
			pNewOrder->MoveTo( LocationEnd.GetRealX(), LocationEnd.GetRealY() );
			pNewOrder->CreateChildPoiPD();
			//pResultStep->m_iPickupPoiPD = pNewOrder->CreateChildPoiPD();
			ASSERT( pNewOrder->GetChildPoiPD() );
		}
		else if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_Dropoff )
		{
			pNewOrder->MoveTo( LocationEnd.GetRealX(), LocationEnd.GetRealY() );
			pNewOrder->CreateChildPoiPD();
			ASSERT( pNewOrder->GetChildPoiPD() );

			//bCalculateAStar = false;
			//pResultStep->m_iDropoffPoiPD = pNewOrder->CreateChildPoiPD();
		}
		else
		{
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}
		}

		CEOSAIUnit2PathwayPredefinedStep* pPredefinedStep = pResultStep->m_pPredefinedStep;
		//if( pUnit->GetObjectId() == 232 &&
		//	pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_Dropoff )
		//{
		CString strDebug;
		long iNumberOfGridsInAllowedPath = 0;
		if( pPredefinedStep )
		{
			//POSITION pos2 = pResultStep->m_AIRegionMovementPath.GetHeadPosition();
			CEOSAIRegion2* pAIRegionEnd = m_pAIUnitPathwayFinder->GetAIBrain()->GetAIRegionManager()->GetAIRegion( LocationEnd );

			if( pUnit->GetObjectId() == 11 ) // Transport=11, Infantry=16, Aircraft=24
			{
				bool bClearData = false;
				if( bClearData )
				{
					g_AIPathwayData.RemoveAll();
				}
				POSITION pos2 = pPredefinedStep->GetDirectMovementPath()->GetHeadPosition();
				while( pos2 )
				{
					CEOSAIPathfinderPoint* pPathfinderPoint = pPredefinedStep->GetDirectMovementPath()->GetNext( pos2 );
					g_AIPathwayData.AddTail( pPathfinderPoint->m_iId );
				}
			}
			POSITION pos2 = pPredefinedStep->GetAIRegionAllowedMovementPath()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIRegion2* pAIRegion = pPredefinedStep->GetAIRegionAllowedMovementPath()->GetNext( pos2 );
				iNumberOfGridsInAllowedPath += pAIRegion->m_iRegionSize;
				//pNewOrder->AddAIRegionToPermittedPath( pAIRegion->m_iId );
				pNewOrder->AddAIRegionToPreferredPath( pAIRegion->m_iId );

				//if( pAIRegionEnd->m_iId == 307 )
				if( pUnit->GetObjectId() == 11 ) // Transport=11, Infantry=16, Aircraft=24
				{
					bool bClearData = false;
					if( bClearData )
					{
						g_HighlightAIRegions.RemoveAll();
					}
					g_HighlightAIRegions.AddTail( pAIRegion->m_iId );

					CString str; str.Format( _T("Added %d"), pAIRegion->m_iId );
					Checkpoint::Write( str );
					strDebug += str + _T("\n");
				}
			}
			if( pAIRegionEnd->m_iId == 307 )
			{
				CString str; str.Format( _T("Added UnitOrder ptr:%x id:%d"), pNewOrder, pNewOrder->GetObjectId() );
				Checkpoint::Write( str );
				strDebug += str + _T("\n");
			}
		}
		CEOSAIRegion2* pAIRegionStart = m_pAIUnitPathwayFinder->GetAIBrain()->GetAIRegionManager()->GetAIRegion( pNewOrder->GetStartingPoint() );
		CEOSAIRegion2* pAIRegionEnd = m_pAIUnitPathwayFinder->GetAIBrain()->GetAIRegionManager()->GetAIRegion( LocationEnd );

		// When the blocking-unit is an enemy unit, the AI might (sometimes) want to fight-through the unit
		// But, sometimes the blocking-unit is neutral, and the AI shouldn't try to walk through them
		pNewOrder->WalkAroundBlockingUnits( true );//false );
		pUnit->AppendOrder( pNewOrder );
		CString str; str.Format( _T("Added UnitOrder ptr:%x id:%d"), pNewOrder, pNewOrder->GetObjectId() );
		strDebug += str + _T("\n");
		if( pNewOrder->CalculateAStar() )
		{
			iNumberOfOrdersCreated++;
			pResultStep->SetUnitOrder( pNewOrder );

			// Add the upgrade order
			if( pResultStep->m_eTask == EOSAI::UnitPathwayResultStep::enum_Upgrade )
			{
				CUnitOrder* pNewOrder2 = new CUnitOrder();
				pNewOrder2->SetWorldDesc( pWorldDescServer );
				pNewOrder2->SetParent( pUnit->ObjectId() );
				pNewOrder2->Upgrade( pResultStep->m_pAITarget->GetObjectId() );
				pUnit->AppendOrder( pNewOrder2 );
			}
		}
		else
		{
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}

			//#ifdef _DEBUG
			//pNewOrder->CalculateAStar(); // try it again - for debugging purposes
			//#endif _DEBUG
			//#ifdef _DEBUG
			//pNewOrder->CalculateAStar(); // try it again - for debugging purposes
			//#endif _DEBUG

			pUnit->DeleteOrder( pNewOrder );
			pNewOrder = NULL;
			iNumberOfOrdersFailed++;
			break;
		}
		if( pNewOrder )
		{
			ASSERT( pNewOrder->GetType() != CUnitOrder::enumType_Undefined );
		}

		if( pTacticalProject2 && pNewOrder )
		{
			//if( pPredefinedStep )
			//{
			//	pPredefinedStep->GetEstimatedMovementTime();
			//}
			//float fEndTime1 = pResultStep->GetEndTime();
			pTacticalProject2->AddToResultSteps( pResultStep );
			//float fEndTime = pUnit->GetTimeUntilCompletionOfOrder( pNewOrder );
			//pResultStep->SetUnitOrderEndTime( fEndTime1 );
		}
		//if( pTacticalProject2 && pNewOrder && bSimultaneousArrival )
		//{
		//	float fEndTime = pUnit->GetTimeUntilCompletionOfOrder( pNewOrder );
		//	pTacticalProject2->SetResultStepCompletionTime( pResultStep, fEndTime );
		//}

		LastStepEnd = LocationEnd;
		iCurrentStep++;
	}

	if( iNumberOfOrdersCreated > 0 && iNumberOfOrdersFailed == 0 )
	{
		m_bAllOrdersWereCreated = true;
	}
	return m_bAllOrdersWereCreated;
}
#endif DO_NOT_COMPILE



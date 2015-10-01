
#include "stdafx.h"
//#include "Unit.h"
#include "AIPlayer.h"
//#include "AISpatialMap.h"
//#include "AILogicalObject.h"
//#include "AILogicalAction.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTask.h"
#include "City.h"
#include "PoiObject.h"
#include "EOSAIPoiMobile.h"
#include "EOSAIWorldDistanceTool.h"
//#include "AISuperTask.h"
//#include "WorldDesc.h"

//#include "EOSAILimitedList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::PoiMobile::PoiMobile() : CEOSAIPoiObject()
{
	m_eFieldOrders = EOSAI::enumFieldOrders_None;
	//m_pParentAICity = NULL;
	//m_pServerPoiMobile = NULL;
}
/*
void  EOSAI::PoiMobile::SetServerPoiObject( CPoiObject* pPoiObject )
{
	ASSERT( dynamic_cast< CPoiMobile* >( pPoiObject ) );
	m_pServerPoiMobile = dynamic_cast< CPoiMobile* >( pPoiObject );
	CAIPoiObject::SetServerPoiObject( pPoiObject );
}
*/
/*
void  EOSAI::PoiMobile::SetPlayerPoiObject( CPoiObject* pPoiObject )
{
	if( pPoiObject == NULL ) return;
	//
	ASSERT( dynamic_cast< CPoiMobile* >( pPoiObject ) );
	CAIPoiObject::SetPlayerPoiObject( pPoiObject );
}
*/
/*
CPoiMobile*  EOSAI::PoiMobile::GetPlayerPoiMobile()
{
	return dynamic_cast< CPoiMobile* >( m_pPlayerPoiObject );
}
*/
/*
void  EOSAI::PoiMobile::SetPoiMobile( CPoiMobile* pPoiMobile )
{
	m_pPoiMobile = pPoiMobile;

	SetPoiObject( pPoiMobile );
	//m_InitialState.m_fTime = 0.0f;
	//m_InitialState.SetLocationAndGeo( pPoiMobile->GetLocation() );

	// If this is a GroundUnit, make sure it is on the ground (unless it is inside a container)
	#ifdef _DEBUG
	if( pPoiMobile->GetContainer() == NULL )
	{
		if( pPoiMobile->IsGroundUnit() )
		{
			//ASSERT( m_pAIBrain->GetWorldDesc()->GetLandGeoNumber1( pPoiMobile->GetLocation() ) );
		}
		if( pPoiMobile->IsSeaUnit() )
		{
			//ASSERT( m_pAIBrain->GetWorldDesc()->GetWaterGeoNumber1( pPoiMobile->GetLocation() ) );
		}
		/-*
		CGeo* pGeo = m_pAIBrain->GetWorldDesc()->GetGeo( GetInitialState()->GetGeo() );
		CGeo* pGeo = m_pAIBrain->GetWorldDesc()->GetGeo( GetInitialState()->GetGeo() );
		ASSERT( pGeo );
		if( pPoiMobile->IsGroundUnit() )
		{
			ASSERT( pGeo->GetGeoType() == CGeo::Land );
			long iGridX = pPoiMobile->GetLocation().GetGridX();
			long iGridY = pPoiMobile->GetLocation().GetGridY();
			long iTest1 = 1.0f / 4.0f;
			long iTest2 = 3.0f / 4.0f;
			long iTest3 = 5.0f / 4.0f;
			CGeo* pGeo2 = NULL;
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX+1, iGridY+1 );
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX+1, iGridY   );
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX+1, iGridY-1 );
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX,   iGridY+1 );
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX,   iGridY   );
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX,   iGridY-1 );
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX-1, iGridY+1 );
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX-1, iGridY   );
			pGeo2 = m_pAIBrain->GetWorldDesc()->GetGeo( iGridX-1, iGridY-1 );
			int z=0;
		}
		if( pPoiMobile->IsSeaUnit() )
		{
			ASSERT( pGeo->GetGeoType() == CGeo::Water );
		}
		*-/
	}
	#endif
}
*/

/*
EOSAI::PoiMobile::CAIPoiMobile( CAIPlan* pAIPlan, long iOwner, CPoiMobile* pPoiMobile ) : CAIPoiObject( pAIPlan, iOwner, pPoiMobile->GetLocation() )
{
	//m_pInitialAirbase = NULL;
	m_pParentAICity = NULL;
	m_pPoiMobile = pPoiMobile;

	m_InitialState.m_fTime = 0.0f;
	m_InitialState.SetLocationAndGeo( pPoiMobile->GetLocation() );

	// If this is a GroundUnit, make sure it is on the ground (unless it is inside a container)
	#ifdef _DEBUG
	if( pPoiMobile->GetContainer() == NULL )
	{
		CGeo* pGeo = pAIPlan->GetWorldDesc()->GetGeo( m_InitialState.GetGeo() );
		ASSERT( pGeo );
		if( pPoiMobile->IsGroundUnit() )
		{
			ASSERT( pGeo->GetGeoType() == CGeo::Land );
			long iGridX = pPoiMobile->GetLocation().GetGridX();
			long iGridY = pPoiMobile->GetLocation().GetGridY();
			long iTest1 = 1.0f / 4.0f;
			long iTest2 = 3.0f / 4.0f;
			long iTest3 = 5.0f / 4.0f;
			CGeo* pGeo2 = NULL;
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX+1, iGridY+1 );
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX+1, iGridY   );
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX+1, iGridY-1 );
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX,   iGridY+1 );
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX,   iGridY   );
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX,   iGridY-1 );
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX-1, iGridY+1 );
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX-1, iGridY   );
			pGeo2 = pAIPlan->GetWorldDesc()->GetGeo( iGridX-1, iGridY-1 );
			int z=0;
		}
		if( pPoiMobile->IsSeaUnit() )
		{
			ASSERT( pGeo->GetGeoType() == CGeo::Water );
		}
	}
	#endif
}
*/

/*
EOSAI::PoiMobile::CAIPoiMobile( CAIPlan* pAIPlan, long iOwner, 
							CEOSAICity* pParentAICity, 
							float fTimeOfInitialAvailability ) : CAIPoiObject( pAIPlan, iOwner, pParentAICity->GetInitialLocation() )
{
	m_pPoiMobile = NULL;
	m_pParentAICity = pParentAICity;

	SetInitialContainer( pParentAICity );
	m_InitialState.SetAirbase( pParentAICity );
	m_InitialState.m_fTime = fTimeOfInitialAvailability;
}
*/

EOSAI::PoiMobile::~PoiMobile()
{
	/*
	while( m_UnitTasks.IsEmpty() == FALSE )
	{
		delete m_UnitTasks.RemoveHead();
	}
	*/
}

bool EOSAI::PoiMobile::IsOfType( EOSAIEnumTypeOfUnit eTypeOfUnit )
{
	if( eTypeOfUnit == EOSAIEnumTypeOfUnit_All )
	{
		return true;
	}
	if( eTypeOfUnit == EOSAIEnumTypeOfUnit_CityHunter )
	{
		return CanCaptureCities();
	}
	if( eTypeOfUnit == EOSAIEnumTypeOfUnit_CityHunterTransport )
	{
		if( this->CanContainGroundUnits() )
		{
			// Technically, I need to check whether the transport
			//   can carry units that are CityHunters
			return true;
		}
	}
	return false;
}
/*
long EOSAI::PoiMobile::GetObjectId()
{
	ASSERT( m_pServerPoiMobile );
	if( m_pServerPoiMobile )
	{
		return m_pServerPoiMobile->ObjectId();
	}
	return 0;
}
*/
/*
float EOSAI::PoiMobile::GetAIDestructionValue()
{
	return this->GetProductionCost();
}
*/
/*
void EOSAI::PoiMobile::GetActionRequirements( 
						CAIWeaklyDefinedAction::EnumWeaklyDefinedAction eAction, 
						EOSAI::PoiObjectState* pMyActor, 
						CEOSAILocation TargetLocation, 
						CEOSAIActionRequirement* pRequirement )
{
	
}
*/

bool EOSAI::PoiMobile::CanContain_IgnoreForeignRelations( CEOSAIPoiObject* pAIPoiObject )
{
	EOSAI::PoiMobile* pOtherPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( pAIPoiObject );
	if( pOtherPoiMobile )
	{
		return this->CanContain_IgnoreForeignRelations( pOtherPoiMobile );
	}
	return false;
}

bool EOSAI::PoiMobile::CanContain_IgnoreForeignRelations( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	ASSERT( false ); // this gets recursive
	return false; //this->CanContain_IgnoreForeignRelations( pAIUnitTemplate );
}

/*
void EOSAI::PoiMobile::CreateTypeInformation()
{
	// Create AILogicObject types
	ASSERT( m_pUnitTemplate );
	if( m_pUnitTemplate )
	{
		if( m_pUnitTemplate->IsGroundUnit() )
		{
			this->AddLogicalObjectType( "GroundUnit" );
		}
		if( m_pUnitTemplate->CanCaptureCities() )
		{
			this->AddLogicalObjectType( "CityHunter" );
		}
		if( m_pUnitTemplate->GetMovementRate() > 0.0f )
		{
			this->AddLogicalObjectType( "Explorer" );
		}
		if( m_pUnitTemplate->CanContainUnits() )
		{
			if( m_pUnitTemplate->GetNumberOfGroundUnitsThatICanContain() > 0 )
			{
				this->AddLogicalObjectType( "GroundTransport" );
			}
		}
	}
}
*/

// Information
/*
bool EOSAI::PoiMobile::IAmOfType( CString strLogicObjectName )
{
	POSITION pos = m_IAmOfType.GetHeadPosition();
	while( pos )
	{
		CString strType = m_IAmOfType.GetNext( pos );
		if( strType == strLogicObjectName )
		{
			return true;
		}
	}
	return false;
}
*/
/*
void EOSAI::PoiMobile::SetInitialAirbase( CAIPlan* pAIPlan, long iContainer )
{
	CPoiObject* pAirbase = m_pPoiMobile->GetAirBase();
	if( pAirbase )
	{
		CAIObject* pAIObject = pAIPlan->GetAIObject( pAirbase->ObjectId() );
		CEOSAIPoiObject* pAIPoiObject = dynamic_cast< CEOSAIPoiObject* >( pAIObject );
		if( pAIPoiObject )
		{
			m_pInitialAirbase = pAIPoiObject;
		}
	}
}
*/
void EOSAI::PoiMobile::SetInitialAirbase( CEOSAIPoiObject* pNewAirbase )
{
	if( pNewAirbase )
	{
		GetInitialState()->SetAirbase( pNewAirbase );
	}
}


long EOSAI::PoiMobile::GetNumberOfGroundUnitsThatICanContain()
{
	long iNumber = 0;
	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		iNumber += pAIPoiMobile->GetNumberOfGroundUnitsThatICanContain();
	}
	/*
	POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );
		iNumber += pAIPoiMobile->GetNumberOfGroundUnitsThatICanContain();
	}
	*/
	return iNumber;
}

long EOSAI::PoiMobile::GetNumberOfAirUnitsThatICanContain()
{
	long iNumber = 0;
	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		iNumber += pAIPoiMobile->GetNumberOfAirUnitsThatICanContain();
	}
	/*
	POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );
		iNumber += pAIPoiMobile->GetNumberOfAirUnitsThatICanContain();
	}
	*/
	return iNumber;
}

bool EOSAI::PoiMobile::CanCaptureCities()
{
	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		if( pAIPoiMobile->CanCaptureCities() )
		{
			return true;
		}
	}
	/*
	POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );
		if( pAIPoiMobile->CanCaptureCities() )
		{
			return true;
		}
	}
	*/
	return false;
}

/*
void EOSAI::PoiMobile::CreateAReasonableFirstTask()
{
	//CList< CAILogicAction* > m_MyPotentialActions;

	// Look at the AILogicObject information to figure out my potential tasks
	POSITION pos = g_AIBrain.GetLogicObjects()->GetHeadPosition();
	while( pos )
	{
		CAILogicalObject* pLogicObject = g_AIBrain.GetLogicObjects()->GetNext( pos );
		if( this->IAmOfLogicalObjectType( pLogicObject->GetName() ) )
		{
			// Add actions for this object
			m_MyPotentialActions.AddTail( pLogicObject->GetMyActions() );
		}
	}

	// Look through the possible actions
	float fBestScore = 0.0f;
	CString strBestAction;
	CString strBestTarget;
	//
	pos = m_MyPotentialActions.GetHeadPosition();
	while( pos )
	{
		CAILogicalAction* pLogicAction = m_MyPotentialActions.GetNext( pos );
		CString strAction = pLogicAction->m_strAction;
		//CString strPassiveObject = pLogicAction->m_strPassiveObject;
		//CString strProduct = pLogicAction->m_strProduct;

		// Because different types of units handle things differently
		//  (e.g. airplanes explore in a different pattern than ships or ground units
		//        and they fly further during exploration)
		// I should have the unit decide what some good exploration tasks would be.

		/-*
		if( strAction == "Explore" )
		{
			CEOSAILimitedList< CEOSAIRegion2* > DecentResultsList;
			DecentResultsList.SetMaximumSize( 5 );

			FindReasonableExploreTargets( &DecentResultsList );

			// Turn these results into a "decent tasks" list
			for( long i=0; i<5; i++ )
			{
				CEOSAIRegion2* pAIRegion = DecentResultsList.m_Array[i];
				if( pAIRegion )
				{
					CEOSAITask* pTask = CEOSAITask::CreateTask( this, pLogicAction, pAIRegion, pLogicAction->m_strProduct );
					EOSAI::UnitTask_MoveTo* pUnitTask = dynamic_cast< EOSAI::UnitTask_MoveTo* >( pTask );
					ASSERT( pUnitTask );
					if( pUnitTask )
					{
						pUnitTask->m_fExploreVolume = DecentResultsList.m_ValueArray[i];
						pUnitTask->CalculateValue();
						m_DecentNextTaskList.InsertSorted( pUnitTask );
					}
				}
			}
		}
		if( strAction == "CaptureCity" )
		{
			CEOSAILimitedList< CEOSAICity* > DecentResultsList;
			DecentResultsList.SetMaximumSize( 5 );

			FindReasonableCityTargets( &DecentResultsList );

			// Turn these results into a "decent tasks" list
			for( long i=0; i<5; i++ )
			{
				CEOSAICity* pAIRegion = DecentResultsList.m_Array[i];
				if( pAIRegion )
				{
					CEOSAITask* pTask = CEOSAITask::CreateTask( this, pLogicAction, pAIRegion, pLogicAction->m_strProduct );
					EOSAI::UnitTask_CaptureCity* pUnitTask = dynamic_cast< EOSAI::UnitTask_CaptureCity* >( pTask );
					ASSERT( pUnitTask );
					if( pUnitTask )
					{
						pUnitTask->m_fDistance = DecentResultsList.m_ValueArray[i];
						pUnitTask->CalculateValue();
						m_DecentNextTaskList.InsertSorted( pUnitTask );
					}
				}
			}
		}
		*-/

		// Replace the generic terms with specific ones (e.g. "not-my-city"->"London")
		//   Then score the potential task.

		/*
		// In this first cycle, limit all potential tasks to something that can be
		// accomplished within 5 turns.
		//FindAllObjectsWithinArea
		if( pLogicAction->m_strPassiveObject != "" )
		{
			CList< CAIObject* > AIObjectList;
			m_pAIBrain->FindAllObjectsWithinArea( pLogicAction->m_strPassiveObject, GetInitialLocation(), 100.0f, &AIObjectList );

			// What is the value of this task?
			POSITION pos2 = AIObjectList.GetHeadPosition();
			while( pos2 )
			{
				CAIObject* pPassiveObject = AIObjectList.GetNext( pos2 );
				//this->EvaluateTask( pLogicAction->m_strAction, pPassiveObject, strProduct );
				CEOSAITask* pTask = CEOSAITask::CreateTask( this, pLogicAction, pPassiveObject, strProduct );
			}
		}
		else
		{
			//this->EvaluateTask( pLogicAction->m_strAction, NULL, strProduct );
		}
		*-/
	}

	// What's my decent task list look like?
	pos = m_DecentNextTaskList.GetHeadPosition();
	while( pos )
	{
		EOSAI::UnitTask* pAIUnitTask = m_DecentNextTaskList.GetNext( pos );
		int p=0;
	}

	// Construct an Order based on the task
	if( m_DecentNextTaskList.IsEmpty() == FALSE )
	{
		EOSAI::UnitTask* pAIUnitTask = m_DecentNextTaskList.GetHead();
		pAIUnitTask->CreateOrders();
	}
}
*/
/*
void EOSAI::PoiMobile::FindReasonableCityTargets( CEOSAILimitedList< CEOSAICity* >* pDecentResultsList )
{
	//CEOSAILimitedList< CEOSAIRegion2* >   DecentResultsList;
	//DecentResultsList.SetMaximumSize( 5 );
	//DecentResultsList.IOwnTheObjects( true );
	//DecentResultsList.RetainLargeValues( true );

	float fPixelRange = this->GetMovementRate()*5.0f;

	CList< CAIObject* > RegionList;
	m_pAIBrain->FindAllObjectsWithinArea( "NotMyCity", GetInitialLocation(), this->GetMovementType(), fPixelRange, &RegionList );

	// Which paths are going to give me some good exploration?
	POSITION pos2 = RegionList.GetHeadPosition();
	while( pos2 )
	{
		CAIObject* pObject = RegionList.GetNext( pos2 );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pObject );
		if( pAICity )
		{
			// count the number of unexplored AIRegions I'll fly over
			float fDistance = m_pAIBrain->GetSpatialMap()->GetPixelDistance( this->GetInitialLocation(), pAICity->GetInitialLocation(), this->GetMovementType(), 0 );
			if( fDistance < 100000.0f )
			{
				pDecentResultsList->Add( pAICity, fDistance );
			}
		}
	}
}

void EOSAI::PoiMobile::FindReasonableExploreTargets( CEOSAILimitedList< CEOSAIRegion2* >* pDecentResultsList )
{
	//CEOSAILimitedList< CEOSAIRegion2* >   DecentResultsList;
	//DecentResultsList.SetMaximumSize( 5 );
	//DecentResultsList.IOwnTheObjects( true );
	//DecentResultsList.RetainLargeValues( true );

	float fPixelRange = 0.0f;
	if( this->HasARange() ){ fPixelRange = this->GetGridRange()*4.0f; }
	else{ fPixelRange = this->GetMovementRate()*5.0f; }

	CList< CAIObject* > RegionList;
	m_pAIBrain->FindAllObjectsWithinArea( "UnexploredArea", GetInitialLocation(), this->GetMovementType(), fPixelRange, &RegionList );

	/-*
	if( this->GetUnit()->GetName() == "Infantry" )
	{
		int x=0;
	}
	*-/

	// Which paths are going to give me some good exploration?
	POSITION pos2 = RegionList.GetHeadPosition();
	while( pos2 )
	{
		CAIObject* pObject = RegionList.GetNext( pos2 );
		CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pObject );
		if( pAIRegion )
		{
			// Can I reach this location?
			float fDistance = m_pAIBrain->GetSpatialMap()->GetPixelDistance( this->GetInitialLocation(), pAIRegion->GetInitialLocation(), this->GetUnitTemplate()->GetMovementType(), 0 );
			if( fDistance != -1.0f )
			{
				// count the number of unexplored AIRegions I'll fly over
				float fVolume = m_pAIBrain->GetUnexploredCountOfAllObjectsWithinPath( GetInitialLocation(), this->GetUnitTemplate()->GetPixelViewRange(), pAIRegion->GetInitialLocation() );
				pDecentResultsList->Add( pAIRegion, fVolume );
			}
		}
	}

	/-*
	// Turn these results into a "decent tasks" list
	for( long i=0; i<5; i++ )
	{
		DecentResultsList.m_Array[i];

		CEOSAITask* pTask = CEOSAITask::CreateTask( this, 

		m_DecentTasksList
	}
	*-/
}
*/


float EOSAI::PoiMobile::GetMovementRate()
{
	float fSlowestUnit = 100000.0f;
	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		float fSpeed = pAIPoiMobile->GetMovementRate();
		if( fSpeed < fSlowestUnit )
		{
			fSlowestUnit = fSpeed;
		}
	}
	/*
	POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );
		float fSpeed = pAIPoiMobile->GetMovementRate();
		if( fSpeed < fSlowestUnit )
		{
			fSlowestUnit = fSpeed;
		}
	}
	*/
	return fSlowestUnit;
}

float EOSAI::PoiMobile::GetMovementLimit()
{
	float fShortestRange = 100000.0f;
	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		float fRange = pAIPoiMobile->GetMovementLimit();
		if( fRange < fShortestRange )
		{
			fShortestRange = fRange;
		}
	}
	/*
	POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );
		float fRange = pAIPoiMobile->GetMovementLimit();
		if( fRange < fShortestRange )
		{
			fShortestRange = fRange;
		}
	}
	*/
	return fShortestRange;
}


bool EOSAI::PoiMobile::IsWithinRange( CEOSAILocation loc ) // if I appended move-to loc, can I return?
{
	if( this->HasARange() )
	{
		//float fDistanceTravelledSoFar = this->GetFinalDistanceTravelled();
		//float fDistanceTravelledSoFar = this->m_AIObjectActionProgression.GetMovementDistance();
		float fDistanceTravelledSoFar = this->GetFinalDistanceTravelled();
		float fDistance = g_pWorldDistanceTool->GetDistance( this->GetFinalState()->GetLocation(), loc );
		float fDistanceBackHome = g_pWorldDistanceTool->GetDistance( this->GetFinalState()->GetAirbase()->GetInitialState()->GetLocation(), loc );
		if( fDistanceTravelledSoFar + fDistance + fDistanceBackHome >= this->GetMovementLimit() )
		{
			// Don't go out-of-range to attack an enemy unit
			return false;
		}
	}
	return true;
}
/*
bool EOSAI::PoiMobile::IsRangedUnit()
{
	return ( m_pServerPoiMobile->GetInnerDeathRange() > 0.0f );
}
*/
EOSAIEnumMovementType3 EOSAI::PoiMobile::GetMovementType()
{
	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		return pAIPoiMobile->GetMovementType();
	}
	/*
	POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );
		return pAIPoiMobile->GetMovementType();
	}
	*/
	ASSERT( false );
	return EOSAIEnumMovementType3_Air;
}
/*
CEOSAILocation EOSAI::PoiMobile::GetFinalLocation()
{
	return GetFinalState()->GetLocation();
}

float EOSAI::PoiMobile::GetFinalTime()
{
	return GetFinalState()->GetTime();
}
*/
float EOSAI::PoiMobile::GetFinalDistanceTravelled()
{
	// Get the current distance value
	float fMaxDistance = GetMovementConsumed();

	// If this is a group, find the largest "movement consumed" of the containees.
	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		float fDistance = pAIPoiMobile->GetMovementConsumed();
		if( fMaxDistance < fDistance )
		{
			fMaxDistance = fDistance;
		}
	}
	/*
	//if( m_pUnit )
	{
		POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
		while( pos )
		{
			EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );

			float fDistance = pAIPoiMobile->GetMovementConsumed();
			if( fMaxDistance < fDistance )
			{
				fMaxDistance = fDistance;
			}
		}
	}
	*/

	ASSERT( false ); // What are the planned move distances?

	//fMaxDistance += GetAIObjectActionProgression()->GetMovementDistance();
	//fMaxDistance += this->GetFinalDistanceTravelled();//GetAIObjectActionProgression()->GetMovementDistance();
	/*
	pos = m_AITasks.GetHeadPosition();
	while( pos )
	{
		CEOSAITask* pTask = m_AITasks.GetNext( pos );
		fMaxDistance += pTask->GetMovementDistance();
	}
	*/
	return fMaxDistance;
}

float EOSAI::PoiMobile::GetMovementConsumed()
{
	float fMaxConsumed = 0.0f;

	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		float fConsumed = pAIPoiMobile->GetMovementConsumed();
		if( fConsumed > fMaxConsumed )
		{
			fMaxConsumed = fConsumed;
		}
	}
	/*
	POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );
		float fConsumed = pAIPoiMobile->GetMovementConsumed();
		if( fConsumed > fMaxConsumed )
		{
			fMaxConsumed = fConsumed;
		}
	}
	*/
	return fMaxConsumed;
}

bool EOSAI::PoiMobile::HasARange()
{
	EOSAI::PoiObjectState* pFinalState = this->GetFinalState();
	POSITION pos = pFinalState->GetContaineesList()->GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = pFinalState->GetContaineesList()->GetNext( pos );
		if( pAIPoiMobile->HasARange() ) return true;
	}
	/*
	POSITION pos = m_InitialUnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiMobile* pAIPoiMobile = m_InitialUnitsInsideMe.GetNext( pos );
		if( pAIPoiMobile->HasARange() ) return true;
	}
	*/
	return false;
}
/*
long EOSAI::PoiMobile::GetFinalGeo()
{
	/*
	if( m_UnitTasks.IsEmpty() ){ return GetInitialGeo(); }
	EOSAI::UnitTask* pLastTask = m_UnitTasks.GetTail();
	return pLastTask->GetEndGeo();
	*-/
	return GetInitialGeo();
	/*
	if( m_ScheduledActions.IsEmpty() ){ return GetInitialGeo(); }
	CAIDefinedAction* pLastAction = m_ScheduledActions.GetTail();
	return pLastAction->GetPoiMobileFinalState()->GetGeo();
	*-/
}
*/

/*
CAIPoiObjectStateAndActions* EOSAI::PoiMobile::GetFinalState()
{
	CAIPoiObjectStateAndActions* pLastStateAccordingToActions = GetFinalStateAccordingToActions();
	return pLastStateAccordingToActions;
}

void EOSAI::PoiMobile::ValidateFinalStateOfActionsAndTasks()
{
	#ifdef _DEBUG
	CAIPoiObjectStateAndActions* pLastStateAccordingToActions = GetFinalStateAccordingToActions();
	EOSAI::PoiObjectState* pLastStateAccordingToTasks = GetFinalStateAccordingToTasks();
	ASSERT( pLastStateAccordingToActions->GetTime() == pLastStateAccordingToTasks->GetTime() );
	ASSERT( pLastStateAccordingToActions->GetLocation() == pLastStateAccordingToTasks->GetLocation() );
	if( pLastStateAccordingToActions->GetTime() != pLastStateAccordingToTasks->GetTime() ||
	    pLastStateAccordingToActions->GetLocation() != pLastStateAccordingToTasks->GetLocation() )
	{
		this->UpdateTaskProgression();
	}
	#endif
}
*/

/*
CAIPoiObjectStateAndActions* EOSAI::PoiMobile::GetFinalStateAccordingToActions()
{
	CAIPoiObjectStateAndActions* pLastStateAccordingToActions = GetInitialState();
	if( m_ScheduledActions.IsEmpty() == FALSE )
	{
		CAIDefinedAction* pLastAction = m_ScheduledActions.GetTail();
		pLastStateAccordingToActions = pLastAction->GetPoiMobileFinalState();
	}
	return pLastStateAccordingToActions;
}

EOSAI::PoiObjectState* EOSAI::PoiMobile::GetFinalStateAccordingToTasks()
{
	if( m_UnitTasks.IsEmpty() ){ return GetInitialState(); }
	EOSAI::UnitTask* pLastTask = m_UnitTasks.GetTail();
	return pLastTask->GetFinalState();
}

CAIPoiObjectStateAndActions* EOSAI::PoiMobile::GetEndStateOfPreviousAction( CAIDefinedAction* pAction )
{
	CAIPoiObjectStateAndActions* pPrevState = &m_InitialState;
	POSITION pos = m_ScheduledActions.GetHeadPosition();
	while( pos )
	{
		CAIDefinedAction* pActionInList = m_ScheduledActions.GetNext( pos );
		if( pActionInList == pAction ) return pPrevState;
	}
	return pPrevState;
}
*/

//
// Region Pathway
//
/*
void EOSAI::PoiMobile::UpdateAIRegionPathway()
{
	m_AIRegionPathway.Clear();
	CEOSAIRegion2* pInitialRegion = GetAIBrain()->GetSpatialMap()->GetRegion( GetInitialLocation() );
	m_AIRegionPathway.Add( 0.0f, pInitialRegion );
}
*/

//
/*
void EOSAI::PoiMobile::AppendTask( CEOSAITask* pTask )
{
	EOSAI::UnitTask* pUnitTask = dynamic_cast< EOSAI::UnitTask* >( pTask );
	ASSERT( pUnitTask );
	if( pUnitTask )
	{
		m_UnitTasks.AddTail( pUnitTask );
		//pUnitTask->CalculateTaskDistanceAndDuration();
		UpdateTaskProgression();
	}
}

void EOSAI::PoiMobile::InsertTask( long iPosition, EOSAI::UnitTask* pUnitTask )  // 0 = first, -1 = last
{
	long iCurrTask = 0;
	POSITION pos = m_UnitTasks.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		EOSAI::UnitTask* pExistingTask = m_UnitTasks.GetNext( pos );

		if( iPosition == iCurrTask )
		{
			m_UnitTasks.InsertBefore( prevPos, pUnitTask );

			//pUnitTask->CalculateTaskDistanceAndDuration();
			UpdateTaskProgression();
			return;
		}
		iCurrTask++;
	}
	//
	m_UnitTasks.AddTail( pUnitTask );
	//pUnitTask->CalculateTaskDistanceAndDuration();
	UpdateTaskProgression();
}

void EOSAI::PoiMobile::InsertTaskBefore( EOSAI::UnitTask* pPostTask, EOSAI::UnitTask* pNewTask )
{
	ASSERT( pPostTask );
	if( pPostTask )
	{
		POSITION pos = m_UnitTasks.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			EOSAI::UnitTask* pExistingTask = m_UnitTasks.GetNext( pos );
			ASSERT( pExistingTask != pNewTask );
			if( pExistingTask == pPostTask )
			{
				m_UnitTasks.InsertBefore( prevPos, pNewTask );
				//pNewTask->UpdateTimeBeginAndTimeEnd();
				UpdateTaskProgression();
				return;
			}
		}
	}
	//RecalculateTheEndTimesOfAllProductionTasks();
	//UpdateTheUnitSymbolInitialTimes();
}

void EOSAI::PoiMobile::InsertTaskAfter( EOSAI::UnitTask* pPreTask, EOSAI::UnitTask* pNewTask )
{
	ASSERT( pPreTask );
	if( pPreTask )
	{
		POSITION pos = m_UnitTasks.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			EOSAI::UnitTask* pExistingTask = m_UnitTasks.GetNext( pos );
			ASSERT( pExistingTask != pNewTask );
			if( pExistingTask == pPreTask )
			{
				m_UnitTasks.InsertAfter( prevPos, pNewTask );
				//pNewTask->UpdateTimeBeginAndTimeEnd();
				UpdateTaskProgression();
				return;
			}
		}
	}
	//RecalculateTheEndTimesOfAllProductionTasks();
	//UpdateTheUnitSymbolInitialTimes();
}
*/

/*
void EOSAI::PoiMobile::UpdateTaskProgression()
{
	CAIPoiObjectState CurrentState;
	//CurrentState.SetWorldDesc( m_pPoiMobile->GetWorldDesc() );
	CurrentState.SetAIBrain( m_pAIBrain );
	CurrentState.Copy( GetInitialState() );

	//CEOSAILocation CurrentLocation = this->GetInitialLocation();
	//float     fCurrentTime    = this->GetInitialTime();
	//long      iCurrentGeo     = this->GetInitialGeo();

	// INCOMPLETE: Need to track the container and containee information
	//   Tasks can change this information, so it's important to give the
	//   tasks a say in the state.

	long iNumberOfTasks[100];
	for( long i=0; i<100; i++ ){ iNumberOfTasks[i] = 0; }

	POSITION DebugPos = (POSITION) 1;
	POSITION prevPos = (POSITION) 1;
	//POSITION prevPos2 = (POSITION) 1;
	long bLastTaskRoute = 0;
	long iCurrCount = 0;
	POSITION pos = m_UnitTasks.GetHeadPosition();
	while( pos )
	{
		POSITION HeadPos = m_UnitTasks.GetHeadPosition();
		//prevPos2 = pos;
		EOSAI::UnitTask* pTask = m_UnitTasks.GetNext( pos );

		iNumberOfTasks[iCurrCount] = m_UnitTasks.GetCount();

		iCurrCount++;
		//
		pTask->SetNewStartState( &CurrentState );
		if( pTask->IsValid() )
		{
			//pTask->UpdateFinalState();
			CurrentState.Copy( pTask->GetFinalState() );
			bLastTaskRoute = 1;
		}
		else
		{
			CAIDefinedAction* pActionDefined = pTask->m_pDefinedAction;

			// DEBUG
			#ifdef _DEBUG
			pTask->SetNewStartState( &CurrentState );
			bool b = pTask->IsValid();
			#endif

			// Remove it from the scheduled actions list
			/*
			bool bActionFoundInList = false;
			POSITION pos2 = m_ScheduledActions.GetHeadPosition();
			while( pos2 )
			{
				POSITION prevPos2 = pos2;
				CAIDefinedAction* pActionInList = m_ScheduledActions.GetNext( pos2 );
				if( pActionInList == pActionDefined )
				{
					bActionFoundInList = true;
					//m_ScheduledActions.RemoveAt( prevPos2 );
				}
			}

			if( bActionFoundInList )
			{
				pActionDefined->UnscheduleThisAction();
			}
			*-/
			pActionDefined->UnscheduleThisAction();

			// Go back to the beginning
			CurrentState.Copy( GetInitialState() );
			pos = m_UnitTasks.GetHeadPosition();
			iCurrCount = 0;
			
			DebugPos = pos;
			bLastTaskRoute = 2;
		}
		prevPos = pos;
	}
}
*/

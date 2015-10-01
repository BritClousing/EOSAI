
#include "stdafx.h"
#include "AIUnitGroup.h"
//#include "UnitGroup.h"
#include "AIPlayer2.h"
//#include "AISpatialMap.h"
//#include "AILogicalObject.h"
//#include "AILogicalAction.h"
#include "AIBrain.h"
#include "AIUnitTask.h"
#include "AICity2.h"
#include "AIPoiObject2.h"

//#include "LimitedList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAIUnit2Group::CAIUnit2Group( CAIBrain* pAIPlan ) : CAIPoiMobile( pAIPlan )
{
	m_pServerUnitGroup = NULL;
}

void  CAIUnit2Group::SetServerPoiObject( CPoiObject* pPoiObject )
{
	ASSERT( m_pServerUnitGroup == NULL && m_bHypotheticalPoiObject == false );

	ASSERT( dynamic_cast< CUnitGroup* >( pPoiObject ) );
	m_pServerUnitGroup = dynamic_cast< CUnitGroup* >( pPoiObject );
	CAIPoiMobile2::SetServerPoiObject( pPoiObject );
/*
	//m_pUnit = pUnit;
	m_bHypotheticalPoiObject = false;

	CUnitTemplate* pUnitTemplate = m_pServerUnit->GetUnitTemplate();
	ASSERT( pUnitTemplate );
	SetUnitTemplate( pUnitTemplate );
	m_InitialState.SetHP( m_pServerUnit->GetCurrentHP() );

	//SetPoiMobile( m_pUnit );
	m_Real_TransportSpaceAllocations.SetAIUnit( this );
	m_pAIUnitPathway = CAIUnit2PathwayFinder::CreatePathwayFinder( this );
*/
}
/*
void  CAIUnit2Group::SetPlayerPoiObject( CPoiObject* pPoiObject )
{
	if( pPoiObject == NULL ) return;
	//
	ASSERT( dynamic_cast< CUnitGroup* >( pPoiObject ) );
	CAIPoiMobile2::SetPlayerPoiObject( pPoiObject );
}
*/
//CPoiObject* CAIUnit2Group::GetPoiObject(){ return m_pUnitGroup; }

bool CAIUnit2Group::IsDead()
{
	/*
	if( m_pUnit )
	{
		return m_pUnit->IsDead();
	}
	else // this is a template, it hasn't been built yet
	{
		return false;
	}
	*/
	return false;
}
/*
long  CAIUnit2Group::GetObjectId()
{
	ASSERT( m_pServerUnitGroup );
	if( m_pServerUnitGroup )
	{
		return m_pServerUnitGroup->ObjectId();
	}
	return 0;
}
*/
long CAIUnit2Group::GetNumberOfGroundUnitsThatICanContain()
{
	return m_pServerUnitGroup->GetNumberOfGroundUnitsThatICanContain();
}

long CAIUnit2Group::GetNumberOfAirUnitsThatICanContain()
{
	return m_pServerUnitGroup->GetNumberOfAirUnitsThatICanContain();
}

bool CAIUnit2Group::CanCaptureCities()
{
	return m_pServerUnitGroup->CanCaptureCities();
}

bool CAIUnit2Group::CanCaptureResources()
{
	return m_pServerUnitGroup->CanCaptureResources();
}

//
float  CAIUnit2Group::GetMovementRate()
{
	return m_pServerUnitGroup->GetMovementRate();
}

float  CAIUnit2Group::GetMovementLimit()
{
	return m_pServerUnitGroup->GetMovementLimit();
}

EnumAIMovementType3 CAIUnit2Group::GetMovementType()
{
	return m_pServerUnitGroup->GetMovementType();
}

float CAIUnit2Group::GetMovementConsumed()
{
	if( m_pServerUnitGroup )
	{
		return m_pServerUnitGroup->GetMovementConsumed();
	}
	return 0.0f;
}

bool CAIUnit2Group::HasARange()
{
	return m_pServerUnitGroup->HasARange();
}


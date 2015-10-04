
#include "stdafx.h"
#include "PoiObjectState.h"
//#include "WorldDesc.h"
#include "EOSAIBrain.h"
//#include "AIDesire.h"
#include "EOSAIBrain.h"
#include "EOSAIMultiRegion2.h"
#include "EOSAIGeo.h"
//#include "WorldDescServer.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// need to setup the initial state

EOSAI::PoiObjectState::PoiObjectState()
{
	//m_pAIBrain = NULL;
	m_fHP = 0.0f;
	m_iOwner = 0;
	m_bGroundUnit = false;
	//m_fOddsOfSurvival = 1.0f;
	m_fTime = 0.0f;
	m_pPathfinderPoint = NULL;
	m_pAIRegion = NULL;
	m_pMultiRegion = NULL;
	m_pContainer = NULL;
	m_pAirbase = NULL;
}
/*
CAIPoiObjectState::CAIPoiObjectState( CEOSAIBrain* pAIBrain )
{
	//m_pAIBrain = pAIBrain;

	m_iOwner = 0;
	m_bGroundUnit = false;
	//m_fOddsOfSurvival = 1.0f;
	m_fTime = 0.0f;
	m_pPathfinderPoint = NULL;
	m_pAIRegion = NULL;
	m_pMultiRegion = NULL;
	m_pContainer = NULL;
	m_pAirbase = NULL;
}
*/
/*
CWorldDesc* CAIPoiObjectState::GetWorldDesc() //{ return m_pWorldDesc; }
{
	return m_pAIBrain->GetWorldDescServer();
}
*/

void EOSAI::PoiObjectState::SetLocationAndRegions( CEOSAILocation location )
{
	m_Location = location;
	m_pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( m_Location );
	m_pAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( m_Location );
	m_pPathfinderPoint = m_pAIRegion;
	//m_pAIRegion = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetAIRegionManager()->GetAIRegion( m_Location );
	if( m_bGroundUnit && m_pAIRegion->IsWater() )
	{
		m_pPathfinderPoint = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinderPoint( m_bGroundUnit,m_Location );
	}
}

void EOSAI::PoiObjectState::SetLocationAndRegions( CEOSAIMultiRegion2* pMultiRegion )
{
	m_Location = pMultiRegion->GetLocation();
	m_pAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( m_Location );
	m_pPathfinderPoint = m_pAIRegion;
	m_pMultiRegion = pMultiRegion; 
}

void EOSAI::PoiObjectState::SetLocationAndRegions( CEOSAIRegion2* pAIRegion )
{
	m_Location = pAIRegion->GetLocation();
	m_pAIRegion = pAIRegion;
	m_pPathfinderPoint = m_pAIRegion;
	m_pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( m_Location ); 
}

void EOSAI::PoiObjectState::SetLocationAndRegions( CEOSAILocation location, CEOSAIMultiRegion2* pMultiRegion )
{
	m_Location = location;
	m_pAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( m_Location );
	m_pPathfinderPoint = m_pAIRegion;
	m_pMultiRegion = pMultiRegion;
	if( m_bGroundUnit && m_pAIRegion->IsWater() )
	{
		m_pPathfinderPoint = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinderPoint( m_bGroundUnit,m_Location );
	}
}

void EOSAI::PoiObjectState::SetLocationAndRegions( CEOSAILocation location, CEOSAIRegion2* pAIRegion )
{
	m_Location = location;
	m_pAIRegion = pAIRegion;
	m_pPathfinderPoint = m_pAIRegion;
	m_pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( m_Location ); 
	if( m_bGroundUnit && m_pAIRegion->IsWater() )
	{
		m_pPathfinderPoint = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinderPoint( m_bGroundUnit,m_Location );
	}
}

long EOSAI::PoiObjectState::GetGeo()
{
	//return m_pMultiRegion->GetAIGeo()->GetGeo()->GetGeoNumber();
	return m_pMultiRegion->GetGeoId();
}

bool EOSAI::PoiObjectState::IsInsideATransport()
{
	CEOSAIPoiObject* pCurrObject = GetContainer();
	if( pCurrObject )
	{
		// If it's inside a AIPoiObject, it might be an airfield or city
		// If it's inside a AIPoiMobile, it might be a UnitGroup
		if( dynamic_cast< CEOSAIUnit2* >( pCurrObject ) )
		{
			return true;
		}
		//pCurrObject = pCurrObject->GetContainer();
	}
	return false;
}

bool EOSAI::PoiObjectState::IAmContainedInside( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAIPoiObject* pCurrObject = GetContainer();
	if( pCurrObject )
	{
		if( pCurrObject == pAIPoiObject )
		{
			return true;
		}
		//pCurrObject = pCurrObject->GetContainer();
	}
	return false;
}

bool EOSAI::PoiObjectState::Contains( CEOSAIPoiObject* pAIPoiObject )
{
	POSITION pos = m_Containees.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObjectInList = m_Containees.GetNext( pos );
		if( pAIPoiObjectInList == pAIPoiObject )
		{
			return true;
		}
	}
	return false;
}

long EOSAI::PoiObjectState::GetNumberOfContainees()
{
	return (long) m_Containees.GetCount();
}

/*
EOSAI::PoiMobile* CAIPoiObjectState::GetHighestPoiMobileContainer()
{
	EOSAI::PoiMobile* pHighestPoiMobile = NULL;
	CEOSAIPoiObject* pCurrObject = GetContainer();
	while( pCurrObject )
	{
		if( dynamic_cast< EOSAI::PoiMobile* >( pCurrObject ) )
		{
			pHighestPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( pCurrObject );
		}
		pCurrObject = pCurrObject->GetContainer();
	}
	return pHighestPoiMobile;
}
*/
/*
CEOSAIUnit2*  CAIPoiObjectState::GetHighestUnitContainer()
{
	CEOSAIUnit2* pHighestPoiMobile = NULL;
	CEOSAIPoiObject* pCurrObject = GetContainer();
	while( pCurrObject )
	{
		if( dynamic_cast< CEOSAIUnit2* >( pCurrObject ) )
		{
			pHighestPoiMobile = dynamic_cast< CEOSAIUnit2* >( pCurrObject );
		}
		pCurrObject = pCurrObject->GetContainer();
	}
	return pHighestPoiMobile;
}
*/
CEOSAIUnit2*  EOSAI::PoiObjectState::GetAIUnitContainer()
{
	CEOSAIUnit2* pHighestPoiMobile = NULL;
	CEOSAIPoiObject* pCurrObject = GetContainer();
	if( pCurrObject )
	{
		if( dynamic_cast< CEOSAIUnit2* >( pCurrObject ) )
		{
			pHighestPoiMobile = dynamic_cast< CEOSAIUnit2* >( pCurrObject );
		}
		//pCurrObject = pCurrObject->GetContainer();
	}
	return pHighestPoiMobile;
}

void EOSAI::PoiObjectState::RemoveContainee( EOSAI::PoiMobile* pObj )
{
	POSITION pos = m_Containees.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		EOSAI::PoiMobile* pPoiMobileInList = m_Containees.GetNext( pos );
		m_Containees.RemoveAt( prevPos );
	}
}

void EOSAI::PoiObjectState::Copy( EOSAI::PoiObjectState* pOther )
{
	m_iOwner = pOther->m_iOwner;
	m_Location = pOther->m_Location;
	//m_iGeo = pOther->m_iGeo;
	m_fTime = pOther->m_fTime;
	m_pContainer = pOther->m_pContainer;
	m_pAirbase = pOther->m_pAirbase;

	// See if I need to update the containee information
	//   This is meant as an optimization, but I should actually
	//   check if it speeds things up.
	bool bRecreateList = false;
	POSITION pos1 = pOther->m_Containees.GetHeadPosition();
	POSITION pos2 = this->m_Containees.GetHeadPosition();
	while( pos1 && pos2 )
	{
		EOSAI::PoiMobile* pCon1 = pOther->m_Containees.GetNext( pos1 );
		EOSAI::PoiMobile* pCon2 = this->m_Containees.GetNext( pos2 );
		if( pCon1 != pCon2 ){ bRecreateList = true; break; }
	}
	if( pos1 != NULL || pos2 != NULL )
	{
		bRecreateList = true;
	}
	if( bRecreateList )
	{
		m_Containees.RemoveAll();
		POSITION pos = pOther->m_Containees.GetHeadPosition();
		while( pos )
		{
			EOSAI::PoiMobile* pCon = pOther->m_Containees.GetNext( pos );
			m_Containees.AddTail( pCon );
		}
	}
}

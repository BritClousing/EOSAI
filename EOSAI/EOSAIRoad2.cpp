
#include <stdafx.h>
#include "EOSAIRoad2.h"
#include "EOSAIGenericRegionManager.h"
#include "EOSAIRegion2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIRoad2::CEOSAIRoad2( long iId )
{
	m_iId = iId;
	m_iAIRegion1 = m_iAIRegion2 = 0;
	m_iRoadChunkId = 0;
	m_iTerrainType3 = TERRAIN_GRASS;
	m_iTerrainType9 = TERRAIN_ROAD;
}

CEOSAIRoad2::~CEOSAIRoad2()
{
	while( m_RoadsIConnectTo.IsEmpty() == FALSE )
	{
		delete m_RoadsIConnectTo.RemoveHead();
	}
}

CEOSAIAdjacentAIRoad2Distance* CEOSAIRoad2::GetRoadDistance( CEOSAIRoad2* pAIRoad )
{
	POSITION pos = m_RoadsIConnectTo.GetHeadPosition();
	while( pos )
	{
		CEOSAIAdjacentAIRoad2Distance* pRoadDistance = m_RoadsIConnectTo.GetNext( pos );
		if( pRoadDistance->m_pAIRoad == pAIRoad )
		{
			return pRoadDistance;
		}
	}
	return NULL;
}

CEOSAIRegion2* CEOSAIRoad2::GetAIRegion1()
{
	CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( GetGenericRegionManager()->GetGenericRegion( m_iAIRegion1 ) );
	ASSERT( pAIRegion );
	return pAIRegion;
}

CEOSAIRegion2* CEOSAIRoad2::GetAIRegion2()
{
	CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( GetGenericRegionManager()->GetGenericRegion( m_iAIRegion2 ) );
	ASSERT( pAIRegion );
	return pAIRegion;
}


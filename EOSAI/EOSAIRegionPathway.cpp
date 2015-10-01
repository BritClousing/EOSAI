
#include <stdafx.h>
/*
#include "AIRegionPathway.h"
#include "AIRegion.h"
#include "AIBrain.h"
#include "AIPlayer.h"
#include "SemiGeoManager.h"
#include "WorldDescServer.h"

void CAIRegionPathway::Clear()
{
	while( m_AIRegionTimeList.IsEmpty() == FALSE )
	{
		delete m_AIRegionTimeList.RemoveHead();
	}
}

void  CAIRegionPathway::Add( float fTime, CAIRegion2* pAIRegion )
{
	CAIRegionTime* pAIRegionTime = new CAIRegionTime();
	pAIRegionTime->m_fArrivalTime = fTime;
	pAIRegionTime->m_pAIRegion = pAIRegion;

	//CSemiGeoManager* pSemiGeoManager = pAIRegion->GetAIPlan()->GetAIPlayer()->GetSemiGeoManager();
	/-*
	CSemiGeoManager* pSemiGeoManager = pAIRegion->GetWorldDescServer()->GetSemiGeoManager();
	pAIRegionTime->m_pSemiGeo = pSemiGeoManager->GetSemiGeo( pAIRegion->GetLocation() );
	*-/

	m_AIRegionTimeList.AddTail( pAIRegionTime );
}
*/

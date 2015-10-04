
#include "stdafx.h"
#include "EOSAIRegionPathwayMap.h"
#include "City.h"
#include "EOSAIPathfinderPoint.h"
#include "EOSAICommonData.h"
//#include "WorldDescServer.h"

//

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIRegionPathwayItem::CEOSAIRegionPathwayItem()
{
	m_iDebugId = 0;
}

void CEOSAIRegionPathwayItem::Clear()
{
	m_pPathfinderPoint = NULL;
	m_iPreviousPathwayItem = 0;
	m_fSightings = 0.0f;
	m_fDanger    = 0.0f;
	m_fPresence  = 0.0f;
	m_fTime      = 0.0f;
	m_fDistance  = 0.0f;
	m_iGeoSwitches = 0;
	m_fPathfindingValue = 0.0f;

	m_bPathwayPoiObjectsWithin200PixelsFound = false;
}

void CEOSAIRegionPathwayItem::Copy( CEOSAIRegionPathwayItem* pOther )
{
	m_pPathfinderPoint = pOther->m_pPathfinderPoint;
	m_iPreviousPathwayItem = pOther->m_iPreviousPathwayItem;//m_pPathfinderPoint->m_iId;
	m_fSightings = pOther->m_fSightings;
	m_fDanger    = pOther->m_fDanger;
	m_fPresence  = pOther->m_fPresence;
	m_fTime      = pOther->m_fTime;
	m_fDistance  = pOther->m_fDistance;
	m_iGeoSwitches = pOther->m_iGeoSwitches;
	m_fPathfindingValue = pOther->m_fPathfindingValue;

	m_bPathwayPoiObjectsWithin200PixelsFound = false;
}

bool CEOSAIRegionPathwayItem::PathInvolvesMovementAcrossLand()
{
	if( m_iGeoSwitches == 0 )
	{
		//return m_pRegion->IsLand();
		return m_pPathfinderPoint->IsLand();
	}
	return true; // if there is one geo switch, then it must involve land-movement
}

bool CEOSAIRegionPathwayItem::PathInvolvesMovementAcrossWater()
{
	if( m_iGeoSwitches == 0 )
	{
		//return m_pRegion->IsWater();
		return m_pPathfinderPoint->IsWater();
	}
	return true; // if there is one geo switch, then it must involve land-movement
}

//void CEOSAIRegionPathwayItem::CalculatePathwayPoiObjectsWithin200px( CEOSAIRegionPathwayMap* pRegionPathwayResult )
CList< CEOSAIPoiObject* >* CEOSAIRegionPathwayItem::InvokeCalculatePathwayPoiObjectsWithin200px()
{
	if( m_bPathwayPoiObjectsWithin200PixelsFound ) return &m_PathwayPoiObjectsWithin200Pixels;
	m_bPathwayPoiObjectsWithin200PixelsFound = true;
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	float fRangeDistance = 200.0f;
	m_PathwayPoiObjectsWithin200Pixels.RemoveAll();

	//CEOSAIGenericRegionManager* pMultiRegionManager = pWorldDescServer->GetMultiRegionManager();
	//CEOSAIGenericRegionManager* pRegionManager = m_pRegion->GetGenericRegionManager();
	CEOSAIGenericRegionManager* pRegionManager = m_pPathfinderPoint->GetGenericRegionManager();

	// Iterate over the PoiObjects
	//   Optimization: iterate over the MultiRegions, look at the PoiObjects inside the MultiRegion.
	//   This would be faster because: I can skip entire MultiRegions (taking radius into account)
	//     The MultiRegions already contain limited PoiObjects (filters out Mountains, for example)
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	//POSITION pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		if( pAIPoiObject->IsCityResourceAirfieldOrUnit() )
		{
			// Iterate over the MultiRegions in the pathway
			/*
			long iCurrentRegion = m_pCurrentStep->m_iId;
			CEOSAIRegionPathwayItem* pCurrentRegionResult = this;//pRegionManager->GetMultiRegion( iCurrentMultiRegion );
			while( iCurrentRegion != 0 )
			{
				CEOSAIGenericRegion* pCurrentRegion = pRegionManager->GetGenericRegion( iCurrentRegion );
				CEOSAILocation CurrentRegionLocation = pCurrentRegion->GetLocation();

				float fDistance = pWorldDescServer->GetPixelDistance( pPoiObject->GetLocation(), CurrentRegionLocation );
				if( fDistance <= fRangeDistance )
				{
					m_PathwayPoiObjectsWithin200Pixels.AddTail( pPoiObject );
					break;
				}
				pCurrentRegionResult = pRegionPathwayResult->GetRegionResult( iCurrentRegion );
				iCurrentRegion = pCurrentRegionResult->m_iPreviousRegion;
			}
			*/
			long iDebug_Steps = 0;
			CEOSAIPathfinderPoint* pCurrentStep = m_pPathfinderPoint;
			//CEOSAIRegionPathwayItem* pCurrentRegionResult = this;//pRegionManager->GetMultiRegion( iCurrentMultiRegion );
			while( pCurrentStep != 0 )
			{
				//CEOSAIGenericRegion* pCurrentRegion = pRegionManager->GetGenericRegion( iCurrentRegion );
				//pCurrentStep = pRegionManager->GetGenericRegion( iCurrentRegion );
				CEOSAILocation CurrentRegionLocation = pCurrentStep->GetLocation();

				float fDistance = g_pWorldDistanceTool->GetDistance( pAIPoiObject->GetLocation(), CurrentRegionLocation );
				if( fDistance <= fRangeDistance )
				{
					m_PathwayPoiObjectsWithin200Pixels.AddTail( pAIPoiObject );
					break;
				}
				//pCurrentRegionResult = pRegionPathwayResult->GetRegionResult( iCurrentRegion );
				//iCurrentRegion = pCurrentRegionResult->m_iPreviousRegion;
				//pCurrentStep = pCurrentRegionResult->m_pCurrentStep;
				pCurrentStep = pCurrentStep->m_pTempPreviousStep;
				iDebug_Steps++;
				ASSERT( iDebug_Steps < 1000 ); // there may be a loop if we go over 1000 steps
			}
		}
	}
	return &m_PathwayPoiObjectsWithin200Pixels;
}

long  CEOSAIRegionPathwayItem::GetPathwayStepsCount( CEOSAIRegionPathwayMap* pRegionPathwayResult )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CEOSAIGenericRegionManager* pGenericRegionManager = m_pPathfinderPoint->GetGenericRegionManager();
	//
	long iCount = 0;
	//CEOSAIPathfinderPoint* pCurrentRegion = m_pPathfinderPoint;
	//long iCurrentRegion = m_pCurrentStep->m_iId;
	//CEOSAIRegionPathwayItem* pCurrentRegionResult = this;
	//while( iCurrentRegion != 0 )
	CEOSAIRegionPathwayItem* pCurrentItem = this;
	while( pCurrentItem != NULL )
	{
		//CEOSAIGenericRegion* pCurrentRegion = pGenericRegionManager->GetGenericRegion( iCurrentRegion );
		//CEOSAILocation CurrentRegionLocation = pCurrentRegion->GetLocation();

		//pCurrentRegionResult = pRegionPathwayResult->GetRegionResult( iCurrentRegion );
		//iCurrentRegion = pCurrentRegionResult->m_iPreviousRegion;
		//pCurrentRegion = pCurrentRegion->m_pTempPreviousStep;
		//pCurrentItem = m_pRegionPathwayMap->GetRegionResult( pCurrentItem->m_iPreviousPathwayItem );
		pCurrentItem = pCurrentItem->GetPreviousPathwayItem();

		iCount++;
	}
	return iCount;
}

void CEOSAIRegionPathwayItem::GetDistance( float* pCityDistance, float* pLandDistance, float* pWaterDistance )// CEOSAIRegionPathwayMap* pRegionPathwayResult )
{
	long  iCurrentStep = 0;
	float fCurrentDistance = 0.0f;
	long  iLastTerrain = 0; // 1 = Land, 2 = Water, 3 = City
	*pLandDistance = 0.0f;
	*pWaterDistance = 0.0f;
	*pCityDistance = 0.0f;
	CEOSAIRegionPathwayItem* pCurrentItem = this;
	while( pCurrentItem != NULL )
	{
		if( iCurrentStep > 0 )
		{
			float fGap = fCurrentDistance - pCurrentItem->m_fDistance;

			// If one-half of the movement is city, then use the terrain of the other half
			if( pCurrentItem->m_pPathfinderPoint->IsCity() || iLastTerrain == TERRAIN_CITY  )
			{
				if( pCurrentItem->m_pPathfinderPoint->IsCity() && iLastTerrain == TERRAIN_CITY  )
				{
					*pCityDistance  += fGap/2.0f;
				}
				if( pCurrentItem->m_pPathfinderPoint->IsLand() || iLastTerrain == TERRAIN_GRASS  )
				{
					*pLandDistance  += fGap/2.0f;
				}
				if( pCurrentItem->m_pPathfinderPoint->IsWater() || iLastTerrain == TERRAIN_WATER  )
				{
					*pWaterDistance  += fGap/2.0f;
				}
			}

			if( pCurrentItem->m_pPathfinderPoint->IsLand()  ){ *pLandDistance  += fGap/2.0f; }
			if( pCurrentItem->m_pPathfinderPoint->IsWater() ){ *pWaterDistance += fGap/2.0f; }
			//if( pCurrentItem->m_pPathfinderPoint->IsCity()  ){ *pCityDistance  += fGap/2.0f; }

			if( iLastTerrain == TERRAIN_GRASS ){ *pLandDistance  += fGap/2.0f; }
			if( iLastTerrain == TERRAIN_WATER ){ *pWaterDistance += fGap/2.0f; }
			//if( iLastTerrain == TERRAIN_CITY  ){ *pCityDistance  += fGap/2.0f; }
		}

		if( pCurrentItem->m_pPathfinderPoint->IsLand()  ){ iLastTerrain = TERRAIN_GRASS; }
		if( pCurrentItem->m_pPathfinderPoint->IsWater() ){ iLastTerrain = TERRAIN_WATER; }
		if( pCurrentItem->m_pPathfinderPoint->IsCity()  ){ iLastTerrain = TERRAIN_CITY; }

		fCurrentDistance = pCurrentItem->m_fDistance;
		pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
		iCurrentStep++;
	}
}
/*
float CEOSAIRegionPathwayItem::GetLandDistance()// CEOSAIRegionPathwayMap* pRegionPathwayResult )
{
	bool bLastNodeIsLand = false;
	bool bLastNodeIsWater = false;
	float fCurrentDistance = 0.0f;
	float fTotalLandDistance = 0.0f;
	CEOSAIRegionPathwayItem* pCurrentItem = this;
	while( pCurrentItem != NULL )
	{
		if( pCurrentItem->m_pPathfinderPoint->IsLand() )
		{
			float fGap = 0.0f;
			if( fCurrentDistance > 0.0f ){ fGap = fCurrentDistance - pCurrentItem->m_fDistance; }
			fTotalLandDistance += fGap;
		}
		fCurrentDistance = pCurrentItem->m_fDistance;
		pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
	}
	return fTotalLandDistance;
}

float CEOSAIRegionPathwayItem::GetWaterDistance()// CEOSAIRegionPathwayMap* pRegionPathwayResult )
{
	float fCurrentDistance = 0.0f;
	float fTotalWaterDistance = 0.0f;
	CEOSAIRegionPathwayItem* pCurrentItem = this;
	while( pCurrentItem != NULL )
	{
		if( pCurrentItem->m_pPathfinderPoint->IsLand() )
		{
			float fGap = 0.0f;
			if( fCurrentDistance > 0.0f ){ fGap = fCurrentDistance - pCurrentItem->m_fDistance; }
			fTotalWaterDistance += fGap;
		}
		fCurrentDistance = pCurrentItem->m_fDistance;
		pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
	}
	return fTotalWaterDistance;
}
*/
CEOSAIRegionPathwayItem*  CEOSAIRegionPathwayItem::GetFirstRegionPathwayItem() // keep stepping backwards to the first one
{
	CEOSAIRegionPathwayItem* pCurrentItem = this;
	while( pCurrentItem != NULL )
	{
		if( pCurrentItem->GetPreviousPathwayItem() == NULL ) return pCurrentItem;
		pCurrentItem = pCurrentItem->GetPreviousPathwayItem();
	}
	return NULL;
}

CEOSAIRegionPathwayItem*  CEOSAIRegionPathwayItem::GetPreviousPathwayItem()
{
	if( m_iPreviousPathwayItem > 0 )
	{
		return m_pRegionPathwayMap->GetRegionResult( m_iPreviousPathwayItem );
	}
	return NULL;
}

//////////////////////////////

CEOSAIRegionPathwayMap::CEOSAIRegionPathwayMap()
{
	m_pGenericRegionManager = NULL;
	m_fShortestStartToEndTime = 1000000.0f;
	m_fShortestStartToEndDistance = 1000000.0f;
}

CEOSAIRegionPathwayMap::~CEOSAIRegionPathwayMap()
{
	Clear();
}

void CEOSAIRegionPathwayMap::Clear()
{
	m_pGenericRegionManager = NULL;
	m_fShortestStartToEndTime = 1000000.0f;
	m_fShortestStartToEndDistance = 1000000.0f;
	for( long i=0; i<m_ResultArray.m_iSize; i++ )
	{
		if( m_ResultArray[i] != NULL )
		{
			delete m_ResultArray[i];
			m_ResultArray[i] = NULL;
		}
	}
	m_ResultArray.Reset();
}

void CEOSAIRegionPathwayMap::SetArraySize( long iSize )
{
	ASSERT( iSize > 0 );
	ASSERT( m_ResultArray.m_iSize == 0 || m_ResultArray.m_iSize == iSize );
	m_ResultArray.SetSize( iSize );
	for( long i=0; i<iSize; i++ ){ m_ResultArray[i] = 0; }
}


void CEOSAIRegionPathwayMap::AddRegionPathwayItem( CEOSAIRegionPathwayItem* pResult )
{
	if( m_pGenericRegionManager == NULL )
	{
		m_pGenericRegionManager = pResult->m_pPathfinderPoint->GetGenericRegionManager();
	}
	else
	{
		ASSERT( m_pGenericRegionManager = pResult->m_pPathfinderPoint->GetGenericRegionManager() );
	}
	//m_fShortestStartToEndDistance = min( m_fShortestStartToEndDistance, pResult->m_fDistance );

	//CEOSAIGenericRegion* pRegion = dynamic_cast< CEOSAIGenericRegion* >( pResult->m_pPathfinderPoint );
	//if( pRegion )
	CEOSAIPathfinderPoint* pPoint = dynamic_cast< CEOSAIPathfinderPoint* >( pResult->m_pPathfinderPoint );
	if( pPoint )
	{
		//m_ResultArray[ pResult->m_pCurrentStep->m_iId ] = pResult;
		m_ResultArray[ pPoint->m_iId ] = pResult;
	}
}


void CEOSAIRegionPathwayMap::CalculatePathwayPoiObjectsForEachRegion()
{
	long iSize = m_ResultArray.m_iSize;
	for( long i=0; i<iSize; i++ )
	{
		CEOSAIRegionPathwayItem* pRegionResult = m_ResultArray[i];
		if( pRegionResult == NULL ) continue;
		//pRegionResult->CalculatePathwayPoiObjectsWithin200px( this );
		pRegionResult->InvokeCalculatePathwayPoiObjectsWithin200px();
	}
}

CEOSAIRegionPathwayItem*  CEOSAIRegionPathwayMap::GetRegionResult( CEOSAILocation Location )
{
	//CEOSAIRegionPathwayItem* pResult = m_SortedList.GetHead();
	//CEOSAIGenericRegionManager* pRegionManager = pResult->m_pRegion->GetRegionManager();

	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CEOSAIRegionManager2* pAISpatialMap = pWorldDescServer->GetAIRegionManager();
	//CEOSAIGenericRegionManager* pRegionManager = pWorldDescServer->GetRegionManager();
	//CEOSAIGenericRegionManager* pRegionManager = m_pGenericRegionManager;

	//CEOSAIRegion2* pAIRegion = pAISpatialMap->GetRegion( location );
	//long iRegion = pAIRegion->m_iRegionId;
	//return GetRegion( iRegion );
	//if( m_pRegionsArray->Get(1

	CEOSAIGenericRegion* pRegion = m_pGenericRegionManager->GetGenericRegion( Location );
	return GetRegionResult( pRegion->m_iId );
}

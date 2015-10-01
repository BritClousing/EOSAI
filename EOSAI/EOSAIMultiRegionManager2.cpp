
#include <stdafx.h>
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIRegionManager2.h"
//#include "AISpatialMap.h"
//#include "MovementDescription.h"
#include "EOSAIMultiRegion2.h"
//#include "WorldDesc.h"
//#include "EOSAIBrain.h"
#include "PoiObject.h"
#include "City.h"
#include "EOSAIAirfield.h"
//#include "City.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"
#include "EOSAIStopwatch2.h"
#include "EOSAIWorldDistanceTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Smallest values come first
/*
bool SortedByPathfindingValue( CMultiRegionPathwayItem* pResult1, CMultiRegionPathwayItem* pResult2 )
{
	if( pResult2->m_fPathfindingValue > pResult1->m_fPathfindingValue )
	{
		return true;
	}
	else if( pResult2->m_fPathfindingValue < pResult1->m_fPathfindingValue )
	{
		return false;
	}
	return true;  // always return true when values are equal
}
*/
//
/*
bool CMultiRegionPathwayItem::PathInvolvesMovementAcrossLand()
{
	if( m_iGeoSwitches == 0 )
	{
		return m_pMultiRegion->IsLand();
	}
	return true; // if there is one geo switch, then it must involve land-movement
}

bool CMultiRegionPathwayItem::PathInvolvesMovementAcrossWater()
{
	if( m_iGeoSwitches == 0 )
	{
		return m_pMultiRegion->IsWater();
	}
	return true; // if there is one geo switch, then it must involve land-movement
}

void CMultiRegionPathwayItem::CalculatePathwayPoiObjects( CMultiRegionPathwayMap* pMultiRegionPathwayResult )
{
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	float fRangeDistance = 200.0f;
	m_PathwayPoiObjectsWithin200Pixels.RemoveAll();

	CEOSAIMultiRegionManager2* pMultiRegionManager = pWorldDescServer->GetMultiRegionManager();

	// Iterate over the PoiObjects
	//   Optimization: iterate over the MultiRegions, look at the PoiObjects inside the MultiRegion.
	//   This would be faster because: I can skip entire MultiRegions (taking radius into account)
	//     The MultiRegions already contain limited PoiObjects (filters out Mountains, for example)
	POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		if( dynamic_cast< CResourceSource* >( pPoiObject ) )
		{
			int z=0;
		}
		if( pPoiObject && pPoiObject->IsCityResourceAirfieldOrUnit() )
		{
			// Iterate over the MultiRegions in the pathway
			long iCurrentMultiRegion = m_pMultiRegion->m_iId;
			CMultiRegionPathwayItem* pCurrentMultiRegionResult = this;//pMultiRegionManager->GetMultiRegion( iCurrentMultiRegion );
			while( iCurrentMultiRegion != 0 )
			{
				CMultiRegion* pCurrentMultiRegion = pMultiRegionManager->GetMultiRegion( iCurrentMultiRegion );
				CEOSAILocation CurrentMultiRegionLocation = pCurrentMultiRegion->GetLocation();

				float fDistance = pWorldDescServer->GetPixelDistance( pPoiObject->GetLocation(), CurrentMultiRegionLocation );
				if( fDistance <= fRangeDistance )
				{
					m_PathwayPoiObjectsWithin200Pixels.AddTail( pPoiObject );
					break;
				}
				pCurrentMultiRegionResult = pMultiRegionPathwayResult->GetMultiRegionResult( iCurrentMultiRegion );
				iCurrentMultiRegion = pCurrentMultiRegionResult->m_iPreviousMultiRegion;
			}
		}
	}
	int x=0;
}

long  CMultiRegionPathwayItem::GetPathwayStepsCount( CMultiRegionPathwayMap* pMultiRegionPathwayResult )
{
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CEOSAIMultiRegionManager2* pMultiRegionManager = pWorldDescServer->GetMultiRegionManager();
	//
	long iCount = 0;
	long iCurrentMultiRegion = m_pMultiRegion->m_iId;
	CMultiRegionPathwayItem* pCurrentMultiRegionResult = this;
	while( iCurrentMultiRegion != 0 )
	{
		CMultiRegion* pCurrentMultiRegion = pMultiRegionManager->GetMultiRegion( iCurrentMultiRegion );
		CEOSAILocation CurrentMultiRegionLocation = pCurrentMultiRegion->GetLocation();

		pCurrentMultiRegionResult = pMultiRegionPathwayResult->GetMultiRegionResult( iCurrentMultiRegion );
		iCurrentMultiRegion = pCurrentMultiRegionResult->m_iPreviousMultiRegion;

		iCount++;
	}
	return iCount;
}

//////////////////////////////

void CMultiRegionPathwayMap::Clear()
{
	for( long i=0; i<m_ResultArray.m_iSize; i++ )
	{
		delete m_ResultArray[i];
		m_ResultArray[i] = NULL;
	}
	//m_SortedList.RemoveAll();
}

void CMultiRegionPathwayMap::Initialize( long iSize )
{
	m_ResultArray.SetSize( iSize );
	//m_SortedList.SetSortFunction( SortedByPathfindingValue );
	for( long i=0; i<iSize; i++ )
	{
		m_ResultArray[i] = 0;
	}
}

void CMultiRegionPathwayMap::AddMultiRegionPathwayItem( CMultiRegionPathwayItem* pResult )
{
	m_ResultArray[ pResult->m_pMultiRegion->m_iId ] = pResult;
	//m_SortedList.InsertSorted( pResult );
}

void CMultiRegionPathwayMap::CalculatePathwayPoiObjectsForEachMultiRegion()
{
	long iSize = m_ResultArray.m_iSize;
	for( long i=0; i<iSize; i++ )
	{
		CMultiRegionPathwayItem* pMultiRegionResult = m_ResultArray[i];
		if( pMultiRegionResult == NULL ) continue;
		pMultiRegionResult->CalculatePathwayPoiObjects( this );
	}
}

CMultiRegionPathwayItem*  CMultiRegionPathwayMap::GetMultiRegionResult( CEOSAILocation Location )
{
	//CMultiRegionPathwayItem* pResult = m_SortedList.GetHead();
	//CEOSAIMultiRegionManager2* pMultiRegionManager = pResult->m_pMultiRegion->GetMultiRegionManager();
	CEOSAIMultiRegionManager2* pMultiRegionManager = GetCommonState()->GetWorldDescServer()->GetMultiRegionManager();
	CMultiRegion* pMultiRegion = pMultiRegionManager->GetMultiRegion( Location );
	return GetMultiRegionResult( pMultiRegion->m_iId );
}
*/

//////////////////////////////
/*
CEOSAIRegionPathfinder& CEOSAIMultiRegionManager2::GetPathfinder()
{
	return GetCommonState()->GetWorldDescServer()->GetMultiRegionManager()->GetPathfinder();
}
*/
//

CEOSAIMultiRegionManager2::CEOSAIMultiRegionManager2()
{
	m_iNumberOfMultiRegions = 0;
	m_pAIRegionManager = NULL;
	//m_iCurrentKey = 1;
	m_RegionPathfinder.SetRegionManager( this );
}

CEOSAIMultiRegionManager2::~CEOSAIMultiRegionManager2()
{
	for( long i=0; i<m_MultiRegions.GetSize(); i++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegions.GetAt( i );
		m_MultiRegions.SetAt( i, NULL );
		if( pMultiRegion )
		{
			delete pMultiRegion;
		}
	}
	m_MultiRegionList.RemoveAll();
}
/*
void CEOSAIMultiRegionManager2::ClearAIPoiData()
{
	for( long i=0; i<m_MultiRegions.GetSize(); i++ )
	{
		CMultiRegion* pMultiRegion = m_MultiRegions.GetAt( i );
		if( pMultiRegion )
		{
			pMultiRegion->ClearAIPoiData();
		}
	}
}
*/
void CEOSAIMultiRegionManager2::ClearPoiData()
{
	for( long i=0; i<m_MultiRegions.GetSize(); i++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegions.GetAt( i );
		if( pMultiRegion )
		{
			pMultiRegion->ClearPoiData();
		}
	}
	/*
	Leave the MultiRegions in existence
	for( long i=0; i<m_MultiRegions.GetSize(); i++ )
	{
		CMultiRegion* pMultiRegion = m_MultiRegions.GetAt( i );
		m_MultiRegions.SetAt( i, NULL );
		if( pMultiRegion )
		{
			delete pMultiRegion;
		}
	}
	m_MultiRegionList.RemoveAll();
	*/
	/*
	POSITION pos = m_MultiRegionList.GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pMultiRegion = m_MultiRegionList.GetNext( pos );
		//pMultiRegion->ClearAIData();
	}
	*/
}

void  CEOSAIMultiRegionManager2::SetNumberOfPlayers( long iNumberOfPlayers )
{
	m_RegionPathfinder.SetNumberOfPlayers( iNumberOfPlayers );
}

void  CEOSAIMultiRegionManager2::GenerateMultiRegions( CEOSAIRegionManager2* pAISpatialMap )
{
	//m_pAIBrain = pAIBrain;
	//CAISpatialMap* pAISpatialMap = pAIBrain->GetSpatialMap();
	//long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();

	m_PathfinderPointArray.RemoveAll();
	m_MultiRegions.RemoveAll();
	m_MultiRegionList.RemoveAll();
	m_GenericRegionList.RemoveAll();

	//
	// I want regions that aren't near a border (e.g. I don't want the
	//    center region to be coast).  I want to lump areas into as few
	//    MultiRegions as possible - it's bad to have one large, central area
	//    on an island with a few small Multiregions on the periphery.
	//

	/*
	    I make every City into a separate MultiRegion.  (A single AIRegion)
		This way units can move from the City to the water or land areas.
		I'll have to do a better job of creating AIActionDefinition associated Definitions.
		Note: my random map has 101 MultiRegions and 18 cities.  If I do this, it will increase
			the number of MultiRegions by 18% to 119.
		I should also take a look at the MultiRegions and see about lowering that number.
			Why are there so many?  (It might be that the MultiRegions are picking AIRegions
			on the edge.  If I recenter them more often, I could alleviate some of that.)
	*/
	long iNextMultiRegionId = 1;
	long iSize = pAISpatialMap->GetAIRegionArray()->GetSize();
	for( long i=0; i<iSize; i++ )
	{
		CEOSAIRegion2* pAIRegion = pAISpatialMap->GetAIRegion( i );
		if( pAIRegion && pAIRegion->m_iTerrainType9 == TERRAIN_CITY )
		{
			if( iNextMultiRegionId == 92 || iNextMultiRegionId == 96 )
			{
				int g=0;
			}

			CEOSAIMultiRegion2* pMultiRegion = new CEOSAIMultiRegion2( this, pAIRegion, iNextMultiRegionId );
			iNextMultiRegionId++;
			m_MultiRegions.SetAtGrow( pMultiRegion->m_iId, pMultiRegion );
			m_PathfinderPointArray.SetAtGrow( pMultiRegion->m_iId, pMultiRegion );
			m_MultiRegionList.AddTail( pMultiRegion );
			m_GenericRegionList.AddTail( pMultiRegion );
		}
	}

	// The algorithm will step over the map and attempt to build multiregions
	//CWorldDesc* pWorldDesc = m_pWorldDescServer; //pAISpatialMap->GetWorldDesc();
	//long iMaxX = pWorldDesc->GetPixelWidth();
	//long iMaxY = pWorldDesc->GetPixelHeight();

	long iMaxX = g_pWorldDistanceTool->GetPixelWidth();
	long iMaxY = g_pWorldDistanceTool->GetPixelHeight();
	//for( long x = 120; x < iMaxX; x+=240 )
	//for( long x = 0; x < iMaxX; x+=288 ) // 24*12=288
	for( long x = 0; x < iMaxX; x+=384 ) // 24*16=384, 24*14=336
	{
		//for( long y = 120; y < iMaxY; y+=240 )
		//for( long y = 100; y < iMaxY; y+=200 )
		for( long y = 0; y < iMaxY; y+=168 ) // 14 * 12 = 168
		{
			CEOSAILocation Location;
			CEOSAIRegion2* pAIRegion;
			CEOSAIMultiRegion2* pMultiRegion;
			//
			Location.SetReal( (float)x+75,(float)y+50 );
			if( Location.GetRealX() < g_pWorldDistanceTool->GetPixelWidth() &&
				Location.GetRealY() < g_pWorldDistanceTool->GetPixelHeight() )
			{
				pAIRegion = pAISpatialMap->GetAIRegion( Location );
				ASSERT( pAIRegion );
				if( pAIRegion->m_iMultiRegionId == 0 )
				{
					pMultiRegion = new CEOSAIMultiRegion2( this, pAIRegion, iNextMultiRegionId );
					pMultiRegion->GrowRegion();
					iNextMultiRegionId++;

					// DEBUG
					POSITION pos = pMultiRegion->m_AIRegions.GetHeadPosition();
					while( pos )
					{
						CEOSAIRegion2* pAIRegion = pMultiRegion->m_AIRegions.GetNext( pos );
						ASSERT( pAIRegion->m_iMultiRegionId == pMultiRegion->m_iId );
						int z=0;
					}
					// DEBUG

					m_MultiRegions.SetAtGrow( pMultiRegion->m_iId, pMultiRegion );
					m_PathfinderPointArray.SetAtGrow( pMultiRegion->m_iId, pMultiRegion );
					m_MultiRegionList.AddTail( pMultiRegion );
					m_GenericRegionList.AddTail( pMultiRegion );
				}
			}

			//
			//Location.SetReal( x+220,y+100 );
			Location.SetReal( (float)x+300,(float)y+120 );
			if( Location.GetRealX() < g_pWorldDistanceTool->GetPixelWidth() &&
				Location.GetRealY() < g_pWorldDistanceTool->GetPixelHeight() )
			{
				pAIRegion = pAISpatialMap->GetAIRegion( Location );
				ASSERT( pAIRegion );
				if( pAIRegion->m_iMultiRegionId == 0 )
				{
					pMultiRegion = new CEOSAIMultiRegion2( this, pAIRegion, iNextMultiRegionId );
					pMultiRegion->GrowRegion();
					iNextMultiRegionId++;

					// DEBUG
					POSITION pos = pMultiRegion->m_AIRegions.GetHeadPosition();
					while( pos )
					{
						CEOSAIRegion2* pAIRegion = pMultiRegion->m_AIRegions.GetNext( pos );
						ASSERT( pAIRegion->m_iMultiRegionId == pMultiRegion->m_iId );
						int z=0;
					}
					// DEBUG

					m_MultiRegions.SetAtGrow( pMultiRegion->m_iId, pMultiRegion );
					m_PathfinderPointArray.SetAtGrow( pMultiRegion->m_iId, pMultiRegion );
					m_MultiRegionList.AddTail( pMultiRegion );
					m_GenericRegionList.AddTail( pMultiRegion );
				}
			}
		}
	}
	// DEBUG
	for( long iMultiRegion=1; iMultiRegion<m_MultiRegions.GetSize(); iMultiRegion++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegions.GetAt( iMultiRegion );
		if( pMultiRegion == NULL ) continue;
		POSITION pos = pMultiRegion->m_AIRegions.GetHeadPosition();
		while( pos )
		{
			CEOSAIRegion2* pAIRegion = pMultiRegion->m_AIRegions.GetNext( pos );
			ASSERT( pAIRegion->m_iMultiRegionId == pMultiRegion->m_iId );
			int z=0;
		}
	}
	// DEBUG

	// There will be some holes (due to terrain)
	//   Now go over the map and find the regions that aren't part of a multiregion
	long iMax = (long) pAISpatialMap->m_AIRegionArray.GetSize();
	for( long i=0; i<iMax; i++ )
	{
		CEOSAIRegion2* pAIRegion = pAISpatialMap->m_AIRegionArray.GetAt( i );
		if( pAIRegion )
		{
			if( pAIRegion->m_iMultiRegionId == 0 )
			{
				CEOSAIMultiRegion2* pMultiRegion = new CEOSAIMultiRegion2( this, pAIRegion, iNextMultiRegionId );
				pMultiRegion->GrowRegion();
				iNextMultiRegionId++;

				m_MultiRegions.SetAtGrow( pMultiRegion->m_iId, pMultiRegion );
				m_PathfinderPointArray.SetAtGrow( pMultiRegion->m_iId, pMultiRegion );
				m_MultiRegionList.AddTail( pMultiRegion );
				m_GenericRegionList.AddTail( pMultiRegion );
			}
		}
	}
	// DEBUG
	for( long iMultiRegion=1; iMultiRegion<m_MultiRegions.GetSize(); iMultiRegion++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegions.GetAt( iMultiRegion );
		if( pMultiRegion == NULL ) continue;
		POSITION pos = pMultiRegion->m_AIRegions.GetHeadPosition();
		while( pos )
		{
			CEOSAIRegion2* pAIRegion = pMultiRegion->m_AIRegions.GetNext( pos );
			ASSERT( pAIRegion->m_iMultiRegionId == pMultiRegion->m_iId );
			int z=0;
		}
	}
	// DEBUG


	// DEBUG: How many cities are there?
	/*
	long iCityCount = 0;
	POSITION pos = m_pAIBrain->GetWorldDesc()->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pAIBrain->GetWorldDesc()->GetPoiList()->GetNext( pos )->GetPtr();
		CCity* pCity = dynamic_cast< CCity* >( pPoi );
		if( pCity )
		{
			iCityCount++;
		}
	}
	*/

	// Re-center the multiregions (find the most central AIRegion, use it as the center)
	m_iNumberOfMultiRegions = 0;
	for( long i=0; i<m_MultiRegions.GetSize(); i++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegions.GetAt( i );
		if( pMultiRegion )
		{
			pMultiRegion->ReCenter();
			m_iNumberOfMultiRegions++;
		}
	}
	// DEBUG
	for( long iMultiRegion=1; iMultiRegion<m_MultiRegions.GetSize(); iMultiRegion++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegions.GetAt( iMultiRegion );
		if( pMultiRegion == NULL ) continue;
		POSITION pos = pMultiRegion->m_AIRegions.GetHeadPosition();
		while( pos )
		{
			CEOSAIRegion2* pAIRegion = pMultiRegion->m_AIRegions.GetNext( pos );
			ASSERT( pAIRegion->m_iMultiRegionId == pMultiRegion->m_iId );
			int z=0;
		}
	}
	// DEBUG

	// Setup the adjacent Multiregion information
	for( long i=1; i<m_MultiRegions.GetSize(); i++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegions.GetAt( i );
		if( pMultiRegion == NULL ) continue;
		if( i == 21 )
		{
			int x=0;
		}
		if( i == 114 )
		{
			int x=0;
		}
		//
		ASSERT( pMultiRegion->m_AIRegions.IsEmpty() == FALSE );
		POSITION pos = pMultiRegion->m_AIRegions.GetHeadPosition();
		while( pos )
		{
			CEOSAIRegion2* pAIRegion = pMultiRegion->m_AIRegions.GetNext( pos );

			// Look at the regions adjacent to this region
			POSITION pos2 = pAIRegion->m_AdjacentRegions.GetHeadPosition();
			while( pos2 )
			{
				//CAIAdjacentRegionDistance* pDist = pAIRegion->m_AdjacentRegions.GetNext( pos2 );
				//CEOSAIRegion2* pAdjRegion = pDist->m_pRegion;
				CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pAIRegion->m_AdjacentRegions.GetNext( pos2 );
				CEOSAIRegion2* pAdjRegion = (CEOSAIRegion2*) pAdjRegionDesc->m_pRegion;

				//if( pAIRegion->m_iMultiRegionId != pAdjRegion->m_iMultiRegionId )
				if( pMultiRegion->m_iId != pAdjRegion->m_iMultiRegionId )
				{
					CEOSAILocation JumpOffLocation;
					ASSERT( pAIRegion->GetAdjacentRegionJumpOffLocation( pAdjRegion->m_iId, &JumpOffLocation ) );

					//pMultiRegion->AddAdjacentMultiRegion( pAdjRegion->m_iMultiRegionId );
					CEOSAIMultiRegion2* pAdjMultiRegion = m_MultiRegions.GetAt( pAdjRegion->m_iMultiRegionId );
					pMultiRegion->AddAdjacentRegion( pAdjMultiRegion, JumpOffLocation );
				}

				ASSERT( pAIRegion->IsAdjacentToRegion( pAdjRegion ) );
				ASSERT( pAdjRegion->IsAdjacentToRegion( pAIRegion ) );
				//ASSERT( pAIRegion->IsAdjacentToAIRegion( pAdjRegion->m_iId ) );
				//ASSERT( pAdjRegion->IsAdjacentToAIRegion( pAIRegion->m_iId ) );
				//ASSERT( pAIRegion->IsAdjacentToAIRegion( pAdjRegion ) );
				//ASSERT( pAdjRegion->IsAdjacentToAIRegion( pAIRegion ) );
			}
		}
	}

	// DEBUG
	for( long iMultiRegion=1; iMultiRegion<m_MultiRegions.GetSize(); iMultiRegion++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegions.GetAt( iMultiRegion );
		if( pMultiRegion == NULL ) continue;
		POSITION pos = pMultiRegion->m_AdjacentRegions.GetHeadPosition();
		while( pos )
		{
			CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pMultiRegion->m_AdjacentRegions.GetNext( pos );
			CEOSAIMultiRegion2* pMultiRegion2 = dynamic_cast< CEOSAIMultiRegion2* >( pAdjRegionDesc->m_pRegion );
			ASSERT( pMultiRegion2->IsAdjacentToRegion( pMultiRegion ) );
			//ASSERT( pMultiRegion2->IsAdjacentToMultiRegion( iMultiRegion ) );
		}
	}

	/*
	float fCurrentRadiusCheck = 200.0f;
	POSITION pos2 = m_MultiRegionList.GetHeadPosition();
	while( pos2 )
	{
		CMultiRegion* pMultiRegion = m_MultiRegionList.GetNext( pos2 );
		CEOSAILocation MultiRegionCenterLocation = pMultiRegion->GetLocation();

		POSITION pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
		while( pos )
		{
			CPoi* pPoi = m_pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
			CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
			//
			if( pPoiObject && pPoiObject->IsCityResourceAirfieldOrUnit() )
			{
				float fDistance = m_pWorldDescServer->GetPixelDistance( pPoiObject->GetLocation(), MultiRegionCenterLocation );
				if( fDistance <= fCurrentRadiusCheck )
				{
					pMultiRegion->m_PoiObjectsWithin200Pixels.AddTail( pPoiObject );
				}
			}
		}
	}
	*/
}

CEOSAIMultiRegion2* CEOSAIMultiRegionManager2::GetMultiRegion( CEOSAILocation location )
{
	CEOSAIRegion2* pAIRegion = m_pAIRegionManager->GetAIRegion( location );
	ASSERT( pAIRegion );
	long iMultiRegion = pAIRegion->m_iMultiRegionId;
	ASSERT( iMultiRegion > 0 );
	return GetMultiRegion( iMultiRegion );
}

//

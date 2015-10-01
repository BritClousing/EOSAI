
#include "stdafx.h"
//#include "AISpatialMap.h"
//#include "WorldDesc.h"
#include "EOSAIStopwatch2.h"
//#include "SectorMap.h"
#include "EOSAIBrain.h"
#include "AIPlayer.h"
#include "EOSAIGamePlayer.h"
#include "EOSAIHistoricalSightings.h"
//#include "AIDefinedAction.h"
//#include "AIDefinedAction_Explore.h"
#include "EOSAIMovementDescription.h"
//#include "WorldDescServer.h"
//#include "WorldDescServer.h"
//#include "Mountain.h"
#include "EOSAIRoad2.h"
#include "City.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define _TERRAIN_COAST_IS_SEPARATE_AIREGION_

class CAIRegionSeed
{
	public:
		long m_iRegionId;
		long m_iTerrainType9;
		long m_iPosX;
		long m_iPosY;
};

//

CEOSAIRegionManager2::CEOSAIRegionManager2()
{
	//m_pWorldDesc = NULL;
	//m_pWorldDescServer = NULL;
	m_RegionPathfinder.SetRegionManager( this );
}

CEOSAIRegionManager2::~CEOSAIRegionManager2()
{
	CEOSAIRegionManager2::ClearPoiData();

	long iRegionsDeleted = 0;
	for( long i=0; i<m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AIRegionArray.GetAt( i );
		m_AIRegionArray.SetAt( i, NULL );
		if( pRegion )
		{
			iRegionsDeleted++;
			delete pRegion;
		}
	}
	m_GenericRegionList.RemoveAll();

	long iMax = m_GeoCoastalArray.GetSize();
	for( long i=0; i<iMax; i++ )
	{
		CGeoCoastalData2* pGeoCoastalData = m_GeoCoastalArray.GetAt( i );
		if( pGeoCoastalData )
		{
			delete pGeoCoastalData;
		}
	}

	while( m_AIRoads.IsEmpty() == FALSE ){ delete m_AIRoads.RemoveHead(); }
}

void  CEOSAIRegionManager2::SetNumberOfPlayers( long iNumberOfPlayers )
{
	m_RegionPathfinder.SetNumberOfPlayers( iNumberOfPlayers );
}

CEOSAIPathfinderPoint* CEOSAIRegionManager2::GetPathfinderPoint( bool bGroundUnit, CEOSAILocation location )
{
	if( bGroundUnit == false ){ return GetAIRegion( location ); }

	CEOSAIRegion2* pAIRegion = GetAIRegion( location );
	if( bGroundUnit && pAIRegion->IsWater() )
	{
		//ASSERT( false ); - this can happen if the ground unit is in a transport
		#ifdef THINGS_TO_COMPILE_EVENTUALLY
		// Look for roads in the area
		CRoadNode* pRoadNode = m_pWorldDesc->GetNodeMap()->GetNearbyRoadNode( location.GetGridX(),location.GetGridY() );
		if( pRoadNode &&
			pRoadNode->m_pRoadChunk &&
			pRoadNode->m_pRoadChunk->m_iRoadPathfinderPoint > 0 )
		{
			//CEOSAIPathfinderPoint* pPathfinderPoint = 
			return GetPathfinderPoint( pRoadNode->m_pRoadChunk->m_iRoadPathfinderPoint );
		}
		#endif THINGS_TO_COMPILE_EVENTUALLY
	}
	return pAIRegion;
}


void  CEOSAIRegionManager2::ClearPoiData()
{
	for( long i=0; i<m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pAIRegion = m_AIRegionArray.GetAt( i );
		if( pAIRegion )
		{
			pAIRegion->ClearPoiData();
		}
	}
}

#ifdef DO_NOT_COMPILE
void CEOSAIRegionManager2::ServerFunction_GenerateAIRegions( CWorldDescServer* pWorldDescServer )
{
	ASSERT( pWorldDescServer );

	CEOSAIStopwatch2 watch;
	watch.Start();

	m_pWorldDesc = pWorldDescServer;
	m_pWorldDescServer = pWorldDescServer;

	m_RegionMap.Allocate( m_pWorldDescServer->GetGridWidth(), m_pWorldDescServer->GetGridHeight() );
	m_RegionMap.SetWrap( m_pWorldDescServer->GetWrapRight(), m_pWorldDescServer->GetWrapBottom() );
	m_RegionMap.Clear( 0 );

	// Delete all the old regions
	for( long i=0; i<m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AIRegionArray.GetAt( i );
		m_AIRegionArray.SetAt( i, NULL );
		if( pRegion )
		{
			delete pRegion;
		}
	}
	m_GenericRegionList.RemoveAll();

	long iNextPathfinderPointId = 1;
	CList< CAIRegionSeed* > RegionSeedList;

	long iRegionSeedsCreated = 0;
	long iRegionSeedsDeleted = 0;
	long iRegionsCreated = 0;

	// Create a region seed for each city
	POSITION pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CCity* pCity = dynamic_cast< CCity* >( pPoi );
		if( pCity )
		{
			iRegionSeedsCreated++;
			CreateRegionSeed( iNextPathfinderPointId, pCity->GetLocation().GetGridX(), pCity->GetLocation().GetGridY(), &RegionSeedList );
			//
			iRegionsCreated++;
			CreateRegion( iNextPathfinderPointId, pCity->GetLocation().GetGridX(), pCity->GetLocation().GetGridY(), pCity );
			//
			iNextPathfinderPointId++;
		}
	}
	// Create Regions for Mountains
	pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CMountain* pMountain = dynamic_cast< CMountain* >( pPoi );
		if( pMountain )
		{
			iRegionSeedsCreated++;
			CreateRegionSeed( iNextPathfinderPointId, pMountain->GetLocation().GetGridX(), pMountain->GetLocation().GetGridY(), &RegionSeedList );
			//
			iRegionsCreated++;
			CreateRegion( iNextPathfinderPointId, pMountain->GetLocation().GetGridX(), pMountain->GetLocation().GetGridY() );
			//
			iNextPathfinderPointId++;
		}
	}

	// Grow the seeds
	GrowRegionSeeds( RegionSeedList );

	// Create region seeds
	for( long x=0; x<m_pWorldDescServer->GetGridWidth(); x+=24 )
	{
		for( long y=0; y<m_pWorldDescServer->GetGridHeight(); y+=14 )
		{
			// plant 2 seeds on each pass
			CAIRegionSeed* pSeed = NULL;
			CEOSAIRegion2* pRegion = NULL;

			long iNextX = x+6;
			long iNextY = y+4;
			if( iNextX >= 0 && iNextX < m_pWorldDescServer->GetGridWidth() &&
			    iNextY >= 0 && iNextY < m_pWorldDescServer->GetGridHeight() &&
				m_RegionMap.GridValue( iNextX,iNextY ) == 0 )
			{
				iRegionSeedsCreated++;
				CreateRegionSeed( iNextPathfinderPointId, iNextX, iNextY, &RegionSeedList );
				//
				iRegionsCreated++;
				CreateRegion( iNextPathfinderPointId, iNextX, iNextY );
				//
				iNextPathfinderPointId++;
			}
			//
			iNextX = x+18;
			iNextY = y+14-4;//y+15-2;
			if( iNextX >= 0 && iNextX < m_pWorldDescServer->GetGridWidth() &&
			    iNextY >= 0 && iNextY < m_pWorldDescServer->GetGridHeight() &&
				m_RegionMap.GridValue( iNextX,iNextY ) == 0 )
			{
				iRegionSeedsCreated++;
				CreateRegionSeed( iNextPathfinderPointId, iNextX, iNextY, &RegionSeedList );
				//
				iRegionsCreated++;
				CreateRegion( iNextPathfinderPointId, iNextX, iNextY );
				//
				iNextPathfinderPointId++;
			}
		}
	}
	// Grow the seeds
	GrowRegionSeeds( RegionSeedList );

	// We've gone through all the seeds, but there are probably some regions that
	//   have not been explored (because it's a small geo and none of the seeds landed
	//   on it.
	// Create region seeds
	for( long x=0; x<m_pWorldDescServer->GetGridWidth(); x++ )
	{
		for( long y=0; y<m_pWorldDescServer->GetGridHeight(); y++ )
		{
			if( m_RegionMap.GridValue( x, y ) == 0 )
			{
				// plant 2 seeds on each pass
				CAIRegionSeed* pSeed = NULL;

				CreateRegionSeed( iNextPathfinderPointId, x,y, &RegionSeedList );
				CreateRegion( iNextPathfinderPointId, x,y );
				iNextPathfinderPointId++;

				// Grow the seeds
				GrowRegionSeeds( RegionSeedList );
			}
		}
	}

	// Validate (make sure all map locations are part of a region)
	for( long x=0; x<m_pWorldDescServer->GetGridWidth(); x++ )
	{
		for( long y=0; y<m_pWorldDescServer->GetGridHeight(); y++ )
		{
			ASSERT( m_RegionMap.GridValue( x, y ) != 0 );
		}
	}

	for( long i=1; i<m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AIRegionArray.GetAt( i );
		if( pRegion == NULL ) continue;

		ASSERT( abs( pRegion->m_iMinX - pRegion->m_iMaxX ) < 25 );
		ASSERT( abs( pRegion->m_iMinY - pRegion->m_iMaxY ) < 25 );
	}

	// Re-center all the regions
	for( long i=1; i<m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AIRegionArray.GetAt( i );
		if( pRegion == NULL ) continue;

		long iMidX = pRegion->m_iMinX + (pRegion->m_iMaxX - pRegion->m_iMinX)/2;
		long iMidY = pRegion->m_iMinY + (pRegion->m_iMaxY - pRegion->m_iMinY)/2;

		long iExistingGap = 
			abs( pRegion->GetLocation().GetGridX() - iMidX ) +
			abs( pRegion->GetLocation().GetGridY() - iMidY );
		if( iExistingGap > 2 )
		{
			for( long x=pRegion->m_iMinX; x<=pRegion->m_iMaxX; x++ )
			{
				for( long y=pRegion->m_iMinY; y<=pRegion->m_iMaxY; y++ )
				{
					long iThisGap = abs( x - iMidX ) + abs( y - iMidY );
					if( iThisGap < iExistingGap )
					{
						long iRegionAtLocation = m_RegionMap.GridValue( x,y );
						if( iRegionAtLocation == pRegion->m_iId )
						{
							pRegion->SetLocationAndGeo( CEOSAILocation( Grid, x,y ) );
							iExistingGap = iThisGap;
						}
					}
				}
			}
		}
	}

	// Create AIRegion Adjacent information
	/*
	long iMaxX = m_pWorldDescServer->GetGridWidth();
	long iMaxY = m_pWorldDescServer->GetGridHeight();
	for( long x=0; x<iMaxX; x++ )
	{
		for( long y=0; y<iMaxY-1; y++ )
		{
			long iAIRegion1 = m_RegionMap.GridValue( x,y );
			long iAIRegion2 = m_RegionMap.GridValue( x,y+1 );

			//long iAIRegion2 = 0;
			//if( y+1 < iMaxY ){ iAIRegion2 = m_RegionMap.GridValue( x,y+1 ); }
			//else{              iAIRegion2 = m_RegionMap.GridValue( x,0   ); }
			if( iAIRegion1 == 191 || iAIRegion2 == 191 )
			{
				int g=0;
			}

			if( iAIRegion1 != iAIRegion2 )
			{
				CEOSAIRegion2* pAIRegion1 = m_AIRegionArray.GetAt( iAIRegion1 );
				CEOSAIRegion2* pAIRegion2 = m_AIRegionArray.GetAt( iAIRegion2 );

				ASSERT( pAIRegion1 );
				ASSERT( pAIRegion2 );
				ASSERT( pAIRegion1->IsAdjacentToRegion( pAIRegion2 ) );
				ASSERT( pAIRegion2->IsAdjacentToRegion( pAIRegion1 ) );

				pAIRegion1->AddAdjacentRegion( pAIRegion2 );
				pAIRegion2->AddAdjacentRegion( pAIRegion1 );
			}
		}
	}
	*/

	//
	// INCOMPLETE: What about road connections?
	//    I created a CEOSAIRoad2 class to handle this
	//

	for( long i=1; i<m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AIRegionArray.GetAt( i );
		if( pRegion == NULL ) continue;

		ASSERT( abs( pRegion->m_iMinX - pRegion->m_iMaxX ) < 25 );
		ASSERT( abs( pRegion->m_iMinY - pRegion->m_iMaxY ) < 25 );
	}

	// Delete old CoastalLocations
	/*
	while( m_CoastLocations.IsEmpty() == FALSE ){ delete m_CoastLocations.RemoveTail(); }

	//
	// This should be deleted - I don't need it anymore
	// Create coastal locations procedurally
	for( long x1=0; x1<m_pWorldDescServer->GetGridWidth(); x1+=1 )
	{
		for( long y1=0; y1<m_pWorldDescServer->GetGridHeight(); y1+=2 )
		{
			long x2 = x1; 
			long y2 = y1; 
			if( x1%2 == 0 ){ x2 += 2; }
			else{            y2 += 2; }				
			if( x2 < m_pWorldDescServer->GetGridWidth() &&
				y2 < m_pWorldDescServer->GetGridHeight() )
			{
				CGeo* pGeo1 = m_pWorldDescServer->GetGeo( CEOSAILocation( Grid,x1,y1 ) );
				CGeo* pGeo2 = m_pWorldDescServer->GetGeo( CEOSAILocation( Grid,x2,y2 ) );
				if( pGeo1->GetGeoType() == CGeo::Water &&
					pGeo2->GetGeoType() == CGeo::Land )
				{
					long x3 = x1; x1 = x2; x2 = x3;
					long y3 = y1; y1 = y2; y2 = y3;
					CGeo* pGeo3 = pGeo1; pGeo1 = pGeo2; pGeo2 = pGeo3;
				}
				if( pGeo1->GetGeoType() == CGeo::Land &&
				  ( pGeo2->GetGeoType() == CGeo::Water || pGeo2->GetGeoType() == CGeo::Unknown ) )
				{
					CEOSAILocation CoastLocation = CEOSAILocation( Grid,x2,y2 );
					long       iLandRegion  = m_RegionMap.GridValue( x1,y1 );
					CEOSAIRegion2* pLandRegion  = m_AIRegionArray.GetAt( iLandRegion );
					long       iLandGeo     = pLandRegion->m_iGeo;
					long       iWaterRegion = m_RegionMap.GridValue( x2,y2 );
					CEOSAIRegion2* pWaterRegion = m_AIRegionArray.GetAt( iWaterRegion );
					long       iWaterGeo    = pWaterRegion->m_iGeo;

					if( this->HasNearbyCoastLocation( m_pWorldDesc, CoastLocation, iLandGeo, iWaterGeo ) == false )
					{
						CAIRegionCoastLocation* pNewLocation = new CAIRegionCoastLocation();
						pNewLocation->m_CoastLocationLand  = CEOSAILocation( Grid,x1,y1 );
						pNewLocation->m_CoastLocationWater = CEOSAILocation( Grid,x2,y2 );
						pNewLocation->m_pLandRegion = pLandRegion;
						pNewLocation->m_pWaterRegion = pWaterRegion;
						//
						pLandRegion->m_CoastLocations.AddTail( pNewLocation );
						this->m_CoastLocations.AddTail( pNewLocation );
					}
				}
			}
		}
	}
	*/

	// Debug: All "TERRAIN_CITY" AIRegions should have a CCity
	for( long i=1; i<m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AIRegionArray.GetAt( i );
		if( pRegion == NULL ) continue;

		if( pRegion->m_iTerrainType9 == TERRAIN_CITY )
		{
			ASSERT( pRegion->GetCity() );
		}
	}

	//
	// This should be deleted - I don't need it anymore
	// All my coastal cities should be considered coastal-pickup/dropoff locations
	/*
	pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CCity* pCity = dynamic_cast< CCity* >( pPoi );
		//CAIObject* pAIObject = m_pAIBrain->GetAIObjectList()->GetNext( pos );
		//CEOSAICity* pCitySymbol = dynamic_cast< CEOSAICity* >( pAIObject );
		//if( pCitySymbol && pCitySymbol->IsMine() )
		if( pCity )//&& pCity->GetOwner() == m_pAIBrain->GetAIPlayerNumber() ) //pCity->IsMine() )
		{
			// Only coastal cities serve as coastal locations
			if( pCity->GetWaterGeoNumber7c() != 0 )
			{
				CEOSAILocation LandLocation  = pCity->GetLocation();
				CEOSAILocation WaterLocation = pCity->GetLocation();

				long x1 = LandLocation.GetGridX();
				long y1 = LandLocation.GetGridY();
				long x2 = WaterLocation.GetGridX();
				long y2 = WaterLocation.GetGridY();

				CGeo* pGeo1 = m_pWorldDescServer->GetGeo( CEOSAILocation( Grid,x1,y1 ) );
				CGeo* pGeo2 = m_pWorldDescServer->GetGeo( CEOSAILocation( Grid,x2,y2 ) );
				if( pGeo1->GetGeoType() == CGeo::Water &&
					pGeo2->GetGeoType() == CGeo::Land )
				{
					long x3 = x1; x1 = x2; x2 = x3;
					long y3 = y1; y1 = y2; y2 = y3;
					CGeo* pGeo3 = pGeo1; pGeo1 = pGeo2; pGeo2 = pGeo3;
				}
				if( pGeo1->GetGeoType() == CGeo::Land &&
					pGeo2->GetGeoType() == CGeo::Water )
				{
					CEOSAILocation CoastLocation = CEOSAILocation( Grid,x2,y2 );
					long       iLandRegion  = m_RegionMap.GridValue( x1,y1 );
					CEOSAIRegion2* pLandRegion  = m_AIRegionArray.GetAt( iLandRegion );
					long       iLandGeo     = pLandRegion->m_iGeo;
					long       iWaterRegion = m_RegionMap.GridValue( x2,y2 );
					CEOSAIRegion2* pWaterRegion = m_AIRegionArray.GetAt( iWaterRegion );
					long       iWaterGeo    = pWaterRegion->m_iGeo;

					//if( this->HasNearbyCoastLocation( m_pWorldDesc, CoastLocation, iLandGeo, iWaterGeo ) == false )
					//{
						CAIRegionCoastLocation* pNewLocation = new CAIRegionCoastLocation();
						pNewLocation->m_CoastLocationLand  = CEOSAILocation( Grid,x1,y1 );
						pNewLocation->m_CoastLocationWater = CEOSAILocation( Grid,x2,y2 );
						pNewLocation->m_pLandRegion = pLandRegion;
						pNewLocation->m_pWaterRegion = pWaterRegion;
						//
						pLandRegion->m_CoastLocations.AddTail( pNewLocation );
						this->m_CoastLocations.AddTail( pNewLocation );
					//}
				}
			}
		}
	}
	*/

	// Calculate distances between adjacent regions
	/*
	for( long i=1; i<m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AIRegionArray.GetAt( i );
		ASSERT( pRegion );
		if( pRegion )
		{
			// Add regions to the AIObjectList
			/-*
			if( m_pAIBrain )
			{
				//m_pAIBrain->AddAIObject( pRegion );
				m_pAIBrain->AddAIRegion( pRegion );
			}
			*-/

			// Calculate distances between regions
			/-*
			POSITION pos = pRegion->m_AdjacentRegions.GetHeadPosition();
			while( pos )
			{
				CAIAdjacentRegionDistance* pAdjRegionDist = pRegion->m_AdjacentRegionList.GetNext( pos );
				//CEOSAIRegion2* pAdjRegion = m_AIRegionArray.GetAt( pAdjRegionDist->m_iOtherRegion );
				CEOSAIRegion2* pAdjRegion = pAdjRegionDist->m_pRegion;
				ASSERT( pAdjRegion );
				if( pAdjRegion )
				{
					float fGridDistance = m_pWorldDescServer->GetGridDistance( pRegion->GetLocation().GetGridX(), pRegion->GetLocation().GetGridY(), pAdjRegion->GetLocation().GetGridX(), pAdjRegion->GetLocation().GetGridY() );
					pAdjRegionDist->m_fPixelDistance = fGridDistance*4.0f;
				}
			}
			*-/
		}
	}
	*/

	watch.Stop();
	float fBuildTime1 = watch.GetDeltaSeconds();

	//CEOSAIStopwatch2 watch;
	/*
	CAISpatialSearchResult SpatialSearchResult;
	watch.Start();
	float fPixelDistance = GetPixelDistance( CEOSAILocation(Pixel,50,50), CEOSAILocation(Pixel,850,850), EOSAIEnumMovementType3_Water, 0, &SpatialSearchResult );
	//float fPixelDistance = GetPixelDistance( CEOSAILocation(Pixel,200,50), CEOSAILocation(Pixel,900,900), EOSAIEnumMovementType3_Land, 2 );
	//fPixelDistance = GetDistance( CEOSAILocation(Pixel,50,50), CEOSAILocation(Pixel,50,500) );
	watch.Stop();
	float fTime1 = watch.GetDeltaSeconds();
	*/

	/*
	CMovementDescription MovementDesc;
	MovementDesc.m_fTerrainMovementRateMultiplier[ TERRAIN_WATER ] = 10.0f;
	CList< CEOSAILocation*, CEOSAILocation* >  ResultPath;
	watch.Start();
	m_pWorldDesc->FindPath3( &MovementDesc, CEOSAILocation(Pixel,50,50), CEOSAILocation(Pixel,850,850), &ResultPath );
	watch.Stop();
	float fTime2 = watch.GetDeltaSeconds();
	*/

	// Movement from 50,50 -> 850,850 (note: 50,50 and 850,850 are on water)
	// Movement over unknown map, water movement:
	//   Release: AISpatialMap: 0.001548, FindPath3: 0.672696
	// Movement over known map, water movement:
	//   Debug:   AISpatialMap: 0.006878, FindPath3: 8.832127
	//   Release: AISpatialMap: 0.001036, FindPath3: 0.631510
	//CString strText;
	//strText.Format( "AISpatialMap: %f, FindPath3: %f", fTime1, fTime2 );
	//AfxMessageBox( strText );

	// Create CEOSAIRoad2 objects
	long iRoadArraySize = pWorldDescServer->GetRoadChunksArray()->GetSize();
	for( long iRoadChunk=0; iRoadChunk<iRoadArraySize; iRoadChunk++ )
	{
		CRoadChunk* pRoadChunk = pWorldDescServer->GetRoadChunksArray()->GetAt( iRoadChunk );
		if( pRoadChunk == NULL ) continue;
		CEOSAIRegion2* pRoadChunkAIRegion = GetAIRegion( pRoadChunk->m_Location );

		// Clear the road-chunk data
		pRoadChunk->m_iTempVisited = 0;
		pRoadChunk->m_fTempDistance = 0.0f;

		// Is the adjacent road on another AIRegion?
		long iSize = pRoadChunk->GetAdjacentRoadChunkList()->GetSize();
		POSITION pos = pRoadChunk->GetAdjacentRoadChunkList()->GetHeadPosition();
		while( pos )
		{
			long iAdjacentRoadChunk = pRoadChunk->GetAdjacentRoadChunkList()->GetNext( pos );
			if( iAdjacentRoadChunk >= iSize ) continue;

			// we only want to create one AIRoad
			if( iRoadChunk > iAdjacentRoadChunk ) continue;

			CRoadChunk* pAdjRoadChunk = pWorldDescServer->GetRoadChunksArray()->GetAt( iAdjacentRoadChunk );
			if( pAdjRoadChunk == NULL )
			{
				ASSERT( false ); // this means there is a problem somewhere else
				continue;
			}
			CEOSAIRegion2* pAdjRoadChunkAIRegion = GetAIRegion( pAdjRoadChunk->m_Location );
			if( pRoadChunkAIRegion != pAdjRoadChunkAIRegion ) // They are on different regions
			{
				// Look for this road on the region edge already
				bool bFound = false;
				POSITION pos2 = pRoadChunkAIRegion->m_RoadsOnTheRegionEdge.GetHeadPosition();
				while( pos2 )
				{
					CEOSAIRoad2* pExistingAIRoad = pRoadChunkAIRegion->m_RoadsOnTheRegionEdge.GetNext( pos2 );
					if( pExistingAIRoad->m_iRoadChunkId == pRoadChunk->m_iRoadChunkId &&
						pExistingAIRoad->m_iAIRegion1 == pRoadChunkAIRegion->m_iId &&
						pExistingAIRoad->m_iAIRegion2 == pAdjRoadChunkAIRegion->m_iId )
					{
						bFound = true;
						break;
					}
				}
				if( bFound == false )
				{
					// This is an AIRoad
					CEOSAIRoad2* pAIRoad = new CEOSAIRoad2( iNextPathfinderPointId );
					pAIRoad->m_pRegionManager = this;
					pAIRoad->m_iAIRegion1 = pRoadChunkAIRegion->m_iId;
					pAIRoad->m_iAIRegion2 = pAdjRoadChunkAIRegion->m_iId;
					pAIRoad->m_iRoadChunkId = pRoadChunk->m_iRoadChunkId;
					pAIRoad->m_Location = pRoadChunk->m_Location;

					// Attach the road to the AIRegions
					pRoadChunkAIRegion->AddAIRoad( pAIRoad );
					pAdjRoadChunkAIRegion->AddAIRoad( pAIRoad );
					m_AIRoads.AddTail( pAIRoad ); // this owns the AIRoads

					m_PathfinderPointArray.SetAtGrow( iNextPathfinderPointId, pAIRoad );
					iNextPathfinderPointId++;
				}
			}
		}
	}
	pos = m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIRegion2* pAIRegion = (CEOSAIRegion2*) m_GenericRegionList.GetNext( pos );
		pAIRegion->SetupAIRoadConnections();
	}

	//CSectorMap SectorMap;
	watch.Start();
	//SectorMap.Build( pWorldDesc );
	watch.Stop();
	float fBuildTime2 = watch.GetDeltaSeconds();

	watch.Start();
	//float fPixelDistance2 = SectorMap.GetPixelDistance( CEOSAILocation(Pixel,200,50), CEOSAILocation(Pixel,900,900), EOSAIEnumMovementType3_Water );
	watch.Stop();
	float fTime3 = watch.GetDeltaSeconds();

	long iItemsCreated = 0;
	pos = m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIRegion2* pAIRegion = (CEOSAIRegion2*) m_GenericRegionList.GetNext( pos );
		if( pAIRegion->IsLand() || pAIRegion->IsCity() )
		{
			POSITION pos2 = pAIRegion->m_AdjacentRegions.GetHeadPosition();
			while( pos2 )
			{
				//CEOSAIRegion2* pNextRegion = pAIRegion->m_AdjacentRegions.GetNext( pos2 );
				CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pAIRegion->m_AdjacentRegions.GetNext( pos2 );
				if( pAdjRegionDesc->m_pRegion->IsWater() )
				{
					// Create a GeoSwitch, attach it to the LandGeo
					CGeoCoastalData*  pGeoCoastalData = InvokeGeoCoastalData( pAIRegion->GetGeoId() );
					CGeoSwitchItem* pGeoSwitchItem = new CGeoSwitchItem();
					pGeoSwitchItem->m_iLandAIRegion = pAIRegion->m_iId;
					pGeoSwitchItem->m_iWaterAIRegion = pAdjRegionDesc->m_pRegion->m_iId;
					pGeoCoastalData->m_GeoSwitchList.AddTail( pGeoSwitchItem );
					iItemsCreated++;
				}
			}
		}
	}
	/* DEBUG
	long iMax = m_GeoCoastalArray.GetSize();
	for( long i=0; i<iMax; i++ )
	{
		CGeoCoastalData* pGeoCoastalData = m_GeoCoastalArray.GetAt( i );
		if( pGeoCoastalData )
		{
			POSITION pos = pGeoCoastalData->m_GeoSwitchList.GetHeadPosition();
			while( pos )
			{
				CGeoSwitchItem* pSwitchItem = pGeoCoastalData->m_GeoSwitchList.GetNext( pos );
				int h=0;
			}
			int g=0;
		}
	}
	*/
	int jrwe3=0;
/*
	// Iterate over that region's adjacent regions
	POSITION pos = pAIRegion->m_AdjacentRegions.GetHeadPosition();
	while( pos )
	{
		//CAIAdjacentRegionDistance* pDist = pAIRegion->m_AdjacentRegions.GetNext( pos );
		//CEOSAIRegion2* pNextRegion = pDist->m_pRegion;
		CEOSAIAdjacentRegionDesc* pNextAdjRegionDesc = pAIRegion->m_AdjacentRegions.GetNext( pos );
		CEOSAIRegion2* pNextRegion = dynamic_cast< CEOSAIRegion2* >( pNextAdjRegionDesc->m_pRegion );//pAIRegion->m_AdjacentRegions.GetNext( pos );
*/

	int p=0;
}

void  CEOSAIRegionManager2::CreateRegionSeed( long iRegionId, long x, long y, CList< CAIRegionSeed* >* pRegionSeedList )
{
	CAIRegionSeed* pSeed = new CAIRegionSeed;
	pSeed->m_iRegionId = iRegionId;
	pSeed->m_iPosX = x;
	pSeed->m_iPosY = y;
	pSeed->m_iTerrainType9 = m_pWorldDesc->GetActualTerrainMap().GridValue( x, y );
	/*
	if( pSeed->m_iTerrainType9 == TERRAIN_WATER || 
		pSeed->m_iTerrainType9 == TERRAIN_COAST )
	{
		pSeed->m_iTerrainType3 = TERRAIN_WATER;
	}
	else if( pSeed->m_iTerrainType9 == TERRAIN_CITY )
	{
		pSeed->m_iTerrainType3 = TERRAIN_CITY;
	}
	else if( pSeed->m_iTerrainType9 == TERRAIN_UNKNOWN )
	{
		pSeed->m_iTerrainType3 = TERRAIN_UNKNOWN; ASSERT( false );
	}
	else
	{
		pSeed->m_iTerrainType3 = TERRAIN_GRASS;
	}
	*/

	m_RegionMap.GridValue( x, y ) = iRegionId;
	pRegionSeedList->AddTail( pSeed );
}

void  CEOSAIRegionManager2::CreateRegion( long iRegionId, long x, long y, CCity* pCity )
{
	// The server creates a spatial map + AIRegions which is used as an
	//   AI-cheat.  When the Server does this, it doesn't have an pAIPlan.
	//   So, we have two different code paths.
	if( iRegionId == 13 )
	{
		int h=0;
	}

	CEOSAIRegion2* pRegion = new CAIRegion( this, CEOSAILocation( Grid, x,y ), iRegionId );//, m_pWorldDesc, this, CEOSAILocation( Grid, x, y ) );

	pRegion->m_iGeo = m_pWorldDesc->GetGeoNumber( x, y );

	pRegion->m_iTerrainType9 = m_pWorldDesc->GetActualTerrainMap().GridValue( x, y );
	if( pRegion->m_iTerrainType9 == TERRAIN_WATER || 
		pRegion->m_iTerrainType9 == TERRAIN_COAST )
	{
		pRegion->m_iTerrainType3 = TERRAIN_WATER;
	}
	else if( pRegion->m_iTerrainType9 == TERRAIN_CITY )
	{
		pRegion->m_iTerrainType3 = TERRAIN_CITY;
	}
	else if( pRegion->m_iTerrainType9 == TERRAIN_UNKNOWN )
	{
		pRegion->m_iTerrainType3 = TERRAIN_UNKNOWN; ASSERT( false );
	}
	else
	{
		pRegion->m_iTerrainType3 = TERRAIN_GRASS;
	}

	if( pCity )
	{
		ASSERT( pRegion->m_iTerrainType9 == TERRAIN_CITY );
	}

	if( pRegion->m_iTerrainType9 == TERRAIN_CITY )
	{
		// Find the closest city
		CCity* pClosestCity = NULL;
		float fClosestCityDistance = 1000000.0f;
		POSITION pos = GetWorldDesc()->GetPoiList()->GetHeadPosition();
		while( pos )
		{
			CPoi* pPoi = GetWorldDesc()->GetPoiList()->GetNext( pos )->GetPtr();
			CCity* pCity = dynamic_cast< CCity* >( pPoi );
			if( pCity )
			{
				float fDist = GetWorldDesc()->GetPixelDistance( pCity->GetLocation(), CEOSAILocation( Grid,x,y ) );
				if( fDist < fClosestCityDistance )
				{
					fClosestCityDistance = fDist;
					pClosestCity = pCity;
				}
			}
		}
		ASSERT( pClosestCity );
		pRegion->m_pCity = pClosestCity;

		//ASSERT( pClosestCity->GetLocation() == CEOSAILocation( Grid, x,y ) );
		pRegion->SetLocation( pClosestCity->GetLocation() );
	}

	pRegion->m_iRegionSize = 1;
	pRegion->m_iMinX = x;
	pRegion->m_iMaxX = x;
	pRegion->m_iMinY = y;
	pRegion->m_iMaxY = y;
	/*
	if( m_pAIBrain )
	{
		pRegion->UpdateAreaCitResOwnerBalance();
	}
	*/
	m_AIRegionArray.SetAtGrow( pRegion->m_iId, pRegion );
	m_PathfinderPointArray.SetAtGrow( pRegion->m_iId, pRegion );
	m_GenericRegionList.AddTail( pRegion );
	//m_AIRegionArray.SetAt( pRegion->m_iId, pRegion );
}

void  CEOSAIRegionManager2::GrowRegionSeeds( CList< CAIRegionSeed* >& RegionSeedList )
{
	// Grow the seeds
	long iSeedCount = (long) RegionSeedList.GetCount();
	while( RegionSeedList.IsEmpty() == FALSE )
	{
		CAIRegionSeed* pSeed = RegionSeedList.RemoveHead();

		int  iNextX;
		int  iNextY;
		long iGeo;
		long iTerrainType;
		long iRegion;
		//
		for( long i=0; i<8; i++ )
		{
			if( i==0 )
			{
				iNextX = pSeed->m_iPosX+1;
				iNextY = pSeed->m_iPosY;
			}
			if( i==1 )
			{
				iNextX = pSeed->m_iPosX-1;
				iNextY = pSeed->m_iPosY;
			}
			if( i==2 )
			{
				iNextX = pSeed->m_iPosX;
				iNextY = pSeed->m_iPosY+1;
			}
			if( i==3 )
			{
				iNextX = pSeed->m_iPosX;
				iNextY = pSeed->m_iPosY-1;
			}
			if( i==4 )
			{
				iNextX = pSeed->m_iPosX+1;
				iNextY = pSeed->m_iPosY+1;
			}
			if( i==5 )
			{
				iNextX = pSeed->m_iPosX+1;
				iNextY = pSeed->m_iPosY-1;
			}
			if( i==6 )
			{
				iNextX = pSeed->m_iPosX-1;
				iNextY = pSeed->m_iPosY-1;
			}
			if( i==7 )
			{
				iNextX = pSeed->m_iPosX-1;
				iNextY = pSeed->m_iPosY+1;
			}

			/*
			m_pWorldDesc->GetWorldBuildDesc()->TruncateGridLocation( &iNextX, &iNextY );
			if( m_pWorldDesc->GetWorldBuildDesc()->IsValidGridLocation( iNextX, iNextY ) == false )
			{
				continue;
			}
			*/

			// I want don't want the regions to cross map-wrap boundaries
			//   But, I do want regions on opposite sides of the map-edge to know they are "adjacent"
			if( m_pWorldDesc->GetWorldBuildDesc()->IsValidGridLocation( iNextX, iNextY ) )
			{
				// Make sure I don't stray too far from the Region center
				float fGridDistanceFromCenter = 0.0f;
				CEOSAIRegion2* pParentRegion = m_AIRegionArray.GetAt( pSeed->m_iRegionId );
				ASSERT( pParentRegion );
				if( pParentRegion )
				{
					//pRegion->m_CenterLocation.SetGrid( pSeed->m_iPosX, pSeed->m_iPosY );
					fGridDistanceFromCenter = m_pWorldDesc->GetGridDistance( pSeed->m_iPosX, pSeed->m_iPosY, pParentRegion->GetLocation().GetGridX(), pParentRegion->GetLocation().GetGridY() );
					//fDistanceFromCenter = m_pWorldDesc->GetGridDistance( pSeed->m_iPosX,pSeed->m_iPosY, iNextX,iNextY );
				}

				iRegion = m_RegionMap.GridValue( iNextX, iNextY );

				//if( ( pSeed->m_iTerrainType != TERRAIN_COAST && fDistanceFromCenter < 12.0f ) ||
				//	( pSeed->m_iTerrainType == TERRAIN_COAST && fDistanceFromCenter <  6.0f ) )
				bool bValid = false;
				#ifdef _TERRAIN_COAST_IS_SEPARATE_AIREGION_
				if( pSeed->m_iTerrainType9 == TERRAIN_COAST )
				{
					bValid = ( fGridDistanceFromCenter < 6.0f );
				}
				else if( pSeed->m_iTerrainType9 == TERRAIN_CITY )
				{
					bValid = ( fGridDistanceFromCenter < 24.0f );
				}
				#else
				if( pSeed->m_iTerrainType9 == TERRAIN_CITY )
				{
					bValid = ( fGridDistanceFromCenter < 24.0f );
				}
				#endif _TERRAIN_COAST_IS_SEPARATE_AIREGION_
				else
				{
					bValid = ( fGridDistanceFromCenter < 12.0f );
				}
				//if( ( pSeed->m_iTerrainType9 != TERRAIN_COAST && fDistanceFromCenter < 13.0f ) ||
				//	( pSeed->m_iTerrainType9 == TERRAIN_COAST && fDistanceFromCenter <  6.0f ) )
				//	( pSeed->m_iTerrainType9 == TERRAIN_CITY  && fDistanceFromCenter <  6.0f ) )
				if( bValid )
				{
					iTerrainType = m_pWorldDesc->GetActualTerrainMap().GridValue( iNextX, iNextY );
					iGeo = m_pWorldDesc->GetGeoNumber( iNextX, iNextY );
					//if( iGeo == pSeed->m_iGeo && iRegion == 0 )
					//if( iGeo == pSeed->m_iGeo && iTerrainType == pSeed->m_iTerrainType && iRegion == 0 )
					/*
					bool bSameTerrain = (pSeed->m_iTerrainType == iTerrainType);
					if( ( pSeed->m_iTerrainType == TERRAIN_WATER || pSeed->m_iTerrainType == TERRAIN_COASTAL ) &&
						( iTerrainType == TERRAIN_WATER || iTerrainType == TERRAIN_COASTAL ) )
					{
						bSameTerrain = true;
					}
					*/
					bool bExpandIntoHere = ( iTerrainType == pSeed->m_iTerrainType9 );
					#ifndef _TERRAIN_COAST_IS_SEPARATE_AIREGION_
					if( ( iTerrainType == TERRAIN_WATER || iTerrainType == TERRAIN_COAST ) &&
						( pSeed->m_iTerrainType9 == TERRAIN_WATER || pSeed->m_iTerrainType9 == TERRAIN_COAST ) )
					{
						bExpandIntoHere = true;
					}
					#endif
					//if( iTerrainType == pSeed->m_iTerrainType9 && iRegion == 0 )
					if( bExpandIntoHere && iRegion == 0 )
					{
						//iRegionSeedsCreated++;
						//CreateRegionSeed( pSeed->m_iRegionId, iNextX, iNextY, &RegionSeedList );

						CAIRegionSeed* pSeed2 = new CAIRegionSeed;
						pSeed2->m_iRegionId = pSeed->m_iRegionId;
						pSeed2->m_iPosX = iNextX;
						pSeed2->m_iPosY = iNextY;
						//pSeed2->m_iGeo  = pSeed->m_iGeo;
						pSeed2->m_iTerrainType9 = pSeed->m_iTerrainType9;
						m_RegionMap.GridValue( iNextX, iNextY ) = pSeed->m_iRegionId;
						RegionSeedList.AddTail( pSeed2 );
						//RegionSeedList.AddHead( pSeed2 );

						CEOSAIRegion2* pRegion = m_AIRegionArray.GetAt( pSeed->m_iRegionId );
						ASSERT( pRegion );
						if( pRegion )
						{
							pRegion->m_iRegionSize++;

							if( iNextX < pRegion->m_iMinX ){ pRegion->m_iMinX = iNextX; }
							if( iNextX > pRegion->m_iMaxX ){ pRegion->m_iMaxX = iNextX; }
							if( iNextY < pRegion->m_iMinY ){ pRegion->m_iMinY = iNextY; }
							if( iNextY > pRegion->m_iMaxY ){ pRegion->m_iMaxY = iNextY; }
						}
					}
				}
			}
			// Setup "adjacent" information
			m_pWorldDesc->GetWorldBuildDesc()->TruncateGridLocation( &iNextX, &iNextY );
			if( m_pWorldDesc->GetWorldBuildDesc()->IsValidGridLocation( iNextX, iNextY ) )
			{
				iRegion = m_RegionMap.GridValue( iNextX, iNextY );
				if( iRegion != pSeed->m_iRegionId && iRegion != 0 )
				{
					// Record the "adjacent region" information
					CEOSAIRegion2* pRegion1 = m_AIRegionArray.GetAt( pSeed->m_iRegionId );
					CEOSAIRegion2* pRegion2 = m_AIRegionArray.GetAt( iRegion );
					if( pRegion1 )
					{
						//pRegion1->AddAdjacentRegion( iRegion );
						long iDoubleCheckRegion = m_RegionMap.GridValue( pSeed->m_iPosX,pSeed->m_iPosY );
						ASSERT( pRegion1->m_iId == iDoubleCheckRegion );
						pRegion1->AddAdjacentRegion( pRegion2, CEOSAILocation( Grid, pSeed->m_iPosX,pSeed->m_iPosY ) );
					}
					if( pRegion2 )
					{
						long iDoubleCheckRegion = m_RegionMap.GridValue( iNextX,iNextY );
						ASSERT( pRegion2->m_iId == iDoubleCheckRegion );
						pRegion2->AddAdjacentRegion( pRegion1, CEOSAILocation( Grid, iNextX,iNextY ) );
					}
				}
			}
		}
		//iRegionSeedsDeleted++;
		delete pSeed;
	}
}
#endif DO_NOT_COMPILE


CEOSAIRegion2* CEOSAIRegionManager2::GetAIRegion( CEOSAILocation location )
{
	short id = m_RegionMap.GridValue( location.GetGridX(), location.GetGridY() );
	return m_AIRegionArray.GetAt( id );
}

CGeoCoastalData2*  CEOSAIRegionManager2::InvokeGeoCoastalData( long iGeo )
{
	if( m_GeoCoastalArray.GetSize() <= iGeo )
	{
		m_GeoCoastalArray.SetSize( iGeo+50, 50 );
	}
	CGeoCoastalData2* pGeoCoastalData = m_GeoCoastalArray.GetAt( iGeo );
	if( pGeoCoastalData )
	{
		return pGeoCoastalData;
	}
	pGeoCoastalData = new CGeoCoastalData2;
	pGeoCoastalData->m_iGeo = iGeo;
	m_GeoCoastalArray.SetAt( iGeo, pGeoCoastalData );
	return pGeoCoastalData;
}


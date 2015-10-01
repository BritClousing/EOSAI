
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAISimpleMap.h"
#include "EOSAILocation.h"
#include "EOSAIEnumMovementType.h"
#include "EOSAIListSort.h"
//#include "EOSAIObject.h"
#include "EOSAIGeo.h"
#include "EOSAIRegion2.h"
#include "EOSAIBCArray.h"
#include "EOSAIGenericRegionManager.h"
#include "EOSAIRegionPathfinder.h"
class CEOSAIMovementDescription;
class CAIRegionSeed;
class CEOSAIRoad2;
//class CWorldDescServer;
//class CPoiObject;
//class CWorldDesc;
//class CEOSAIRegion2;

// Used by the AI when trying to find pickup/dropoff locations
class CGeoSwitchItem2
{
	public:
		CGeoSwitchItem2(){ m_iLandAIRegion = 0; m_iWaterAIRegion = 0; }
		long  m_iLandAIRegion;
		long  m_iWaterAIRegion;
};

class CGeoCoastalData2
{
	public:
		CGeoCoastalData2(){ m_iGeo = 0; }
		long  m_iGeo;
		CList< CGeoSwitchItem2* >  m_GeoSwitchList;
};

class DLLIMPEXP CEOSAIRegionManager2 : public CEOSAIGenericRegionManager
{
	public:

		CEOSAIRegionManager2();
		~CEOSAIRegionManager2();

		//CWorldDesc*  GetWorldDesc(){ return m_pWorldDesc; }
		void SetNumberOfPlayers( long iNumberOfPlayers );

		virtual CEOSAIGenericRegion*  GetGenericRegion( CEOSAILocation location ){ return GetAIRegion( location ); }
		virtual CEOSAIGenericRegion*  GetGenericRegion( long iId ){ return GetAIRegion( iId ); }
		CEOSAIRegion2* GetAIRegion( CEOSAILocation location );
		CEOSAIRegion2* GetAIRegion( long id ){ return m_AIRegionArray.GetAt( id ); }

		//void GetCoastalRegions( CEOSAILongSet* pCoastalRegions );

		CEOSAIPathfinderPoint* GetPathfinderPoint( bool bGroundObject, CEOSAILocation location );
		CEOSAIPathfinderPoint* GetPathfinderPoint( long id ){ return m_PathfinderPointArray.GetAt( id ); }

		CArray< CEOSAIRegion2* >*        GetAIRegionArray(){ return &m_AIRegionArray; }
		CGeoCoastalData2*            GetGeoCoastalData( long iGeo ){ return m_GeoCoastalArray.GetAt( iGeo ); }
		CArray< CGeoCoastalData2* >* GetGeoCoastalArray(){ return &m_GeoCoastalArray; }

		void  ClearPoiData(); // deleted AIRegions, CoastalLocations, GeoGeoCache data
//		void  ServerFunction_GenerateAIRegions( CWorldDescServer* pWorldDescServer );

	private:
//		void  CreateRegionSeed( long iRegionId, long x, long y, CList< CAIRegionSeed* >* pRegionSeedList );
//		void  CreateRegion( long iRegionId, long x, long y, CEOSAICity* pAICity = NULL );
//		void  GrowRegionSeeds( CList< CAIRegionSeed* >& RegionSeedList );

	public:

		// Pathfinding
		CEOSAIRegionPathfinder&  GetPathfinder(){ return m_RegionPathfinder; }

		CGeoCoastalData2*  InvokeGeoCoastalData( long iGeo );
		CArray< CGeoCoastalData2* >  m_GeoCoastalArray;

	//
	// Pathfinding
	//
		// Pathfinding
		CEOSAIRegionPathfinder  m_RegionPathfinder;

		//CWorldDesc*           m_pWorldDesc;
		//.CWorldDescServer*     m_pWorldDescServer;
		CArray< CEOSAIRegion2* >  m_AIRegionArray;
		CEOSAISimpleMap< short > m_RegionMap;  // tells which AIRegion is at each GridX,GridY location

		CList< CEOSAIRoad2* >     m_AIRoads;

		// This is cached information describing the distance between two geos
		//   It may not be relevant anymore - MultiRegionManager should be able to quickly calculate it
		//CList< CPixelDistanceGeoGeoMoveCache* > m_PixelDistanceGeoGeoMoveList;

		//CList< CAIRegionCoastLocation* >   m_CoastLocations; // Owned
};



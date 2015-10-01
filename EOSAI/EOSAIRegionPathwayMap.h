
#pragma once
#include "EOSAIBCDumbArray1D.h"
#include "EOSAILocation.h"
class CEOSAIRegionPathwayItem;
class CEOSAIRegionPathwayMap;
class CEOSAIGenericRegionManager;
class CEOSAIPathfinderPoint;
class CEOSAIPoiObject;

class CEOSAIRegionPathwayItem // Distance, Danger, GeoSwitches, Sightings
{
	public:
		CEOSAIRegionPathwayItem();
		void   Clear();
		void   Copy( CEOSAIRegionPathwayItem* pItem );

		bool   PathInvolvesMovementAcrossLand();
		bool   PathInvolvesMovementAcrossWater();

		CList< CEOSAIPoiObject* >*   InvokeCalculatePathwayPoiObjectsWithin200px();// CEOSAIRegionPathwayMap* pRegionPathwayResult );
		//void   CalculatePathwayPoiObjectsWithin200px( CEOSAIRegionPathwayMap* pRegionPathwayResult );
		long   GetPathwayStepsCount( CEOSAIRegionPathwayMap* pRegionPathwayResult );
		void   GetDistance( float* pCityDistance, float* pLandDistance, float* pWaterDistance );
		//
		//void   CalculatePoiObjectsAlongPathway( CList< CPoiObject* >* pPoiObjects );

		CEOSAIRegionPathwayItem*  GetFirstRegionPathwayItem(); // keep stepping backwards to the first one
		CEOSAIRegionPathwayItem*  GetPreviousPathwayItem();

		//CMultiRegion* m_pMultiRegion;
		//CEOSAIGenericRegion* m_pRegion;
		CEOSAIRegionPathwayMap*  m_pRegionPathwayMap;
		CEOSAIPathfinderPoint*   m_pPathfinderPoint;

		long   m_iDebugId;

		// Cummulative values based on tracing back the m_iPreviousMultiRegion's to a start
		float  m_fPathfindingValue; // PathfindingValue is influenced by Pathfinding settings: DangerMultiplier, SwitchMultiplier, SightingMultiplier
		float  m_fDistance;
		float  m_fTime;
		float  m_fDanger;
		float  m_fPresence;
		long   m_iGeoSwitches;
		float  m_fSightings;
		//long   m_iPreviousRegion; // allows me to trace a pathway
		//CEOSAIRegionPathwayItem*  m_pPreviousStep;
		long   m_iPreviousPathwayItem; // allows me to trace a pathway

	private:
		bool  m_bPathwayPoiObjectsWithin200PixelsFound;
		CList< CEOSAIPoiObject* >  m_PathwayPoiObjectsWithin200Pixels; // result of "CalculatePathwayPoiObjects"
};

// Stores data about the Distance, GeoSwitches, Danger, etc from one or more Regions
class CEOSAIRegionPathwayMap
{
	public:
		CEOSAIRegionPathwayMap();
		~CEOSAIRegionPathwayMap();
		void Clear();
		bool IsEmpty(){ return m_pGenericRegionManager == NULL; }

		void                           SetRegionManager( CEOSAIGenericRegionManager* p ){ m_pGenericRegionManager = p; }
		CEOSAIGenericRegionManager*  GetRegionManager(){ return m_pGenericRegionManager; }

		//void SetGenericRegionManager( CEOSAIGenericRegionManager* pGenericRegionManager ){ m_pGenericRegionManager = pGenericRegionManager; }
		//void AllocatePathfinderPointArray( long iSize );
		void AddRegionPathwayItem( CEOSAIRegionPathwayItem* pResult );
		void AddShortestStartToEndDistance( float fDistance ){ m_fShortestStartToEndDistance = min( m_fShortestStartToEndDistance, fDistance ); }
		void AddShortestStartToEndTime( float fTime ){ m_fShortestStartToEndTime = min( m_fShortestStartToEndTime, fTime ); }

		void SetArraySize( long iSize );
		long GetArraySize(){ return m_ResultArray.m_iSize; }

		void CalculatePathwayPoiObjectsForEachRegion();

		// ask for a iMultiRegion, and this will return the pathway there
		CEOSAIRegionPathwayItem*  GetRegionResult( CEOSAILocation Location );
		CEOSAIRegionPathwayItem*  GetRegionResult( long iRegion ){ return m_ResultArray[ iRegion ]; }
		float  GetShortestStartToEndDistance(){ return m_fShortestStartToEndDistance; }
		float  GetShortestStartToEndTime(){ return m_fShortestStartToEndTime; }

	private:
		CEOSAIGenericRegionManager*  m_pGenericRegionManager;
		float  m_fShortestStartToEndDistance;
		float  m_fShortestStartToEndTime;
		CEOSAIBCDumbArray1D< CEOSAIRegionPathwayItem* > m_ResultArray; // Owned data
};

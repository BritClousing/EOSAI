
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIBCArray.h"
#include "EOSAILocation.h"
class CEOSAIMultiRegion2;
class CEOSAIRegionManager2;
class CEOSAIBrain;
//#include "BCHashSortedList.h"
#include "EOSAIBCArray2.h"
#include "EOSAIListSort.h"
#include "EOSAIGenericRegionManager.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAIMultiRegion2.h"
#include "EOSAIGenericRegion.h"
#include "EOSAIRegionPathfinder.h"

class CWorldDescServer;
class CEOSAIMultiRegionManager2;
//class CMultiRegionPathwayItem;
class CPoiObject;
class CMultiRegionPathwayMap;
//class CEOSAIRegionManager2;

// These are used in CEOSAIMultiRegionManager2::AddToStartRegions
#define MULTIREGIONS_CITIES      0x1
#define MULTIREGIONS_AIRFIELDS   0x2
#define MULTIREGIONS_CARRIERS    0x4
#define MULTIREGIONS_TRANSPORTS  0x8
#define MULTIREGIONS_COMBATSHIPS 0x10 // not used yet
#define MULTIREGIONS_SEARESHUNTERS 0x20
#define MULTIREGIONS_CITYHUNTERS 0x20
#define MULTIREGIONS_AIRUNITS    0x40


/*
class CMultiRegionDDSS // Distance, Danger, GeoSwitches, Sightings
{
	public:
		CMultiRegion* m_pMultiRegion;
		float  m_fPathfindingValue; // This is the pathfinding potential (influenced by the original values put into DangerMultiplier, SwitchMultiplier, SightingMultiplier)
		float  m_fDistance;
		float  m_fDanger;
		long   m_iGeoSwitches;
		float  m_fSightings;
		CMultiRegion* m_pPreviousMultiRegion;  // useful for finding out previous Danger, Sighting, GeoSwitch information
};
*/

// I don't use this yet.  This is part of an experiment.  I would like to be able to calculate
//   my best pathways, what kind of danger I'll encounter in certain areas/pathways, and be able to
//   figure out what other players might also do.
// I would also like to cache this information (the cache cleared each turn when the situation changes)
/*
class CMultiRegionPathwayKey
{
	public:
		CMultiRegionPathwayItem*  GetPolitePathway( long iPlayer, long iStartMultiRegion, long iEndMultiRegion );
		CMultiRegionPathwayItem*  GetAttackPathway( long iPlayer, long iStartMultiRegion, long iEndMultiRegion );
		CMultiRegionPathwayItem*  GetAttackPathway( long iPlayer );

		void SetStartMultiRegion( long iMultiRegion ){}
		void SetStartMultiRegionsToPlayerCitRes( long iPlayer ){}
		void SetStartMultiRegionsToPlayerCitResUnit( long iPlayer ){}

		// This information is used if I want to cache the data
		long m_iStartMultiRegion;
		long m_iStartMultiRegionsToPlayerCitRes;
		long m_iStartMultiRegionsToPlayerCitResUnit;
		CList< CMultiRegion* >  m_StartRegions;

		float  m_fGeoSwitchMultiplier;
		long   m_iMaxGeoSwitches;

		CEOSAIBCDumbArray1D< float > m_PlayerPresenceMultiplier;
		CEOSAIBCDumbArray1D< float > m_PlayerDangerMultiplier;
		CEOSAIBCDumbArray1D< float > m_PlayerSightingMultiplier;

		CEOSAIBCDumbArray1D< CMultiRegionPathwayItem* >  m_Results;
};
*/
//class CMultiRegionResult // Distance, Danger, GeoSwitches, Sightings
/*
class CMultiRegionPathwayItem // Distance, Danger, GeoSwitches, Sightings
{
	public:
		bool   PathInvolvesMovementAcrossLand();
		bool   PathInvolvesMovementAcrossWater();

		void   CalculatePathwayPoiObjects( CMultiRegionPathwayMap* pMultiRegionPathwayResult );
		long   GetPathwayStepsCount( CMultiRegionPathwayMap* pMultiRegionPathwayResult );

		CMultiRegion* m_pMultiRegion;

		// Cummulative values based on tracing back the m_iPreviousMultiRegion's to a start
		float  m_fPathfindingValue; // PathfindingValue is influenced by Pathfinding settings: DangerMultiplier, SwitchMultiplier, SightingMultiplier
		float  m_fDistance;
		float  m_fDanger;
		float  m_fPresence;
		long   m_iGeoSwitches;
		float  m_fSightings;
		long   m_iPreviousMultiRegion; // allows me to trace a pathway

		CList< CPoiObject* >  m_PathwayPoiObjectsWithin200Pixels;
};
*/
// Stores data about the Distance, GeoSwitches, Danger, etc from one or more MultiRegions
//class CMultiRegionPathwayResult
/*
class CMultiRegionPathwayMap
{
	public:
		~CMultiRegionPathwayMap(){ Clear(); }
		void Clear();
		void Initialize( long iSize );
		void AddMultiRegionPathwayItem( CMultiRegionPathwayItem* pResult );

		long GetArraySize(){ return m_ResultArray.m_iSize; }

		void CalculatePathwayPoiObjectsForEachMultiRegion();

		// ask for a iMultiRegion, and this will return the pathway there
		CMultiRegionPathwayItem*  GetMultiRegionResult( CEOSAILocation Location );
		CMultiRegionPathwayItem*  GetMultiRegionResult( long iMultiRegion ){ return m_ResultArray[ iMultiRegion ]; }

	private:
		CEOSAIBCDumbArray1D< CMultiRegionPathwayItem* > m_ResultArray; // Owned data
};
*/

class DLLIMPEXP CEOSAIMultiRegionManager2 : public CEOSAIGenericRegionManager
{
	public:
		CEOSAIMultiRegionManager2();
		virtual ~CEOSAIMultiRegionManager2();

		//void              SetWorldDescServer( CWorldDescServer* pWorldDescServer ){ m_pWorldDescServer = pWorldDescServer; }
		//CWorldDescServer* GetWorldDescServer(){ return m_pWorldDescServer; }
		void SetNumberOfPlayers( long iNumberOfPlayers );

		void              SetAIRegionManager( CEOSAIRegionManager2* p ){ m_pAIRegionManager = p; }
		CEOSAIRegionManager2* GetAIRegionManager(){ return m_pAIRegionManager; }

		//void  ClearAIPoiData(); // done by each AIPlayer
		void  ClearPoiData();   // done each turn

		long  GetNumberOfMultiRegions(){ return m_iNumberOfMultiRegions; }
		void  GenerateMultiRegions( CEOSAIRegionManager2* pAISpatialMap );
		void  AddAIPoiObjectsToMultiRegions( CEOSAIBrain* pAIBrain );

		virtual CEOSAIGenericRegion*  GetGenericRegion( CEOSAILocation location ){ return GetMultiRegion( location ); }
		virtual CEOSAIGenericRegion*  GetGenericRegion( long iId ){ return GetMultiRegion( iId ); }
		CEOSAIMultiRegion2* GetMultiRegion( CEOSAILocation location );
		CEOSAIMultiRegion2* GetMultiRegion( long id ){ return m_MultiRegions.GetAt( id ); }

		CArray< CEOSAIMultiRegion2* >*  GetMultiRegionArray(){ return &m_MultiRegions; }
		CList< CEOSAIMultiRegion2* >*   GetMultiRegionList(){ return &m_MultiRegionList; }

/*
	Tactics:
		Capture/Consolidate unowned CitRes in a MultiRegion
		Invade Owned MultiRegion
			Two methods:
				Want secrecy; keep sightings and danger down (when nations are somewhat equal)
				Blatant steamroller (when the other nation is weaker), bring more combat units than enemy
		Confront enemy force
			Two methods:
				Want secrecy; keep sightings and danger down (when enemy might retreat)
				Blatant steamroller (when the other force is weaker and has nowhere to go, 
					or expects me to counterattack his invasion), bring combat units
*/
		// Pathfinding
		CEOSAIRegionPathfinder&  GetPathfinder(){ return m_RegionPathfinder; }

	private:

		//
		//CWorldDescServer* m_pWorldDescServer;
		CEOSAIRegionManager2* m_pAIRegionManager;

		// Pathfinding
		CEOSAIRegionPathfinder  m_RegionPathfinder;

		//CAISpatialMap*    m_pAISpatialMap;
		CArray< CEOSAIMultiRegion2* >  m_MultiRegions; // owned
		CList< CEOSAIMultiRegion2* >   m_MultiRegionList; // unowned
		long  m_iNumberOfMultiRegions;
};


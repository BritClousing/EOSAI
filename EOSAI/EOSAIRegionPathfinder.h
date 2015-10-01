
#pragma once

//#include "EOSAIMultiRegion2.h"
#include "EOSAIGenericRegion.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAITerrainBasedValue.h"
#include "EOSAIRegionPathwayMap.h"
#include "EOSAIEnumForeignRelations.h"
#include "EOSAIBCArray.h"
class CEOSAIRegionPathwayMap;
//class CWorldDescServer;
//namespace EOSAI
//{
	class CEOSAIGenericRegionManager;
	class CEOSAIPathfinderPoint;
//};

// These are used in CEOSAIMultiRegionManager2::AddToStartRegions
#define MULTIREGIONS_CITIES      0x1
#define MULTIREGIONS_AIRFIELDS   0x2
#define MULTIREGIONS_CARRIERS    0x4
#define MULTIREGIONS_TRANSPORTS  0x8
#define MULTIREGIONS_COMBATSHIPS 0x10 // not used yet
#define MULTIREGIONS_SEARESHUNTERS 0x20
#define MULTIREGIONS_CITYHUNTERS 0x20
#define MULTIREGIONS_AIRUNITS    0x40

//
class CEOSAIRegionPathwayItem; // Distance, Danger, GeoSwitches, Sightings

//class CMultiRegionPathwayItem // Distance, Danger, GeoSwitches, Sightings

/*
// Stores data about the Distance, GeoSwitches, Danger, etc from one or more Regions
class CEOSAIRegionPathwayMap
{
	public:
		CEOSAIRegionPathwayMap();
		~CEOSAIRegionPathwayMap();
		void Clear();

		void                    SetRegionManager( CEOSAIGenericRegionManager* p ){ m_pGenericRegionManager = p; }
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
*/
//

//namespace EOSAI
//{

class CEOSAIRegionPathfinderPlayerWeights
{
	public:
		CEOSAIRegionPathfinderPlayerWeights()
		{
			Clear();
		}
		void Clear()
		{
			// These values are used to adjust the best pathway
			m_fPlayerPresencePathfindingMultiplier = 0.0f;
			m_fPlayerDangerPathfindingMultiplier = 0.0f;
			m_fPlayerSightingPathfindingMultiplier = 0.0f;
			// The Total Presence/Danger/Sighting values are based on these values (cummulative over the path)
			m_fPlayerPresenceResultMultiplier = 0.0f;
			m_fPlayerDangerResultMultiplier = 0.0f;
			m_fPlayerSightingResultMultiplier = 0.0f;
		}

		// These values are used to adjust the best pathway
		float  m_fPlayerPresencePathfindingMultiplier;
		float  m_fPlayerDangerPathfindingMultiplier;
		float  m_fPlayerSightingPathfindingMultiplier;
		//float  m_fPlayerPresenceAdjustedMultiplier;
		//float  m_fPlayerDangerAdjustedMultiplier;
		//float  m_fPlayerSightingAdjustedMultiplier;
		// The Total Presence/Danger/Sighting values are based on these values (cummulative over the path)
		float  m_fPlayerPresenceResultMultiplier;
		float  m_fPlayerDangerResultMultiplier;
		float  m_fPlayerSightingResultMultiplier;
		//float  m_fPlayerPresenceResultMultiplier;
		//float  m_fPlayerDangerResultMultiplier;
		//float  m_fPlayerSightingResultMultiplier;
};

class CEOSAIRegionPathfinder
{
	public:

		//
		static void AddBorderRegions( CEOSAIRegionPathwayMap* pOriginalMap, CEOSAIRegionPathwayMap* pBorderMap, float fMovementRate );

		// Compile the overlapping [A][B] regions
		static void GetOverlappedRegions( CEOSAIRegionPathwayMap* pMap1, CEOSAIRegionPathwayMap* pMap2, CEOSAIRegionPathwayMap* pOverlappedMap );
		// Compile the overlapping [A][B] regions + the regions in [A] that border on [B] (add the B region)
		//static void GetOverlappedAndBorderRegions( CEOSAIRegionPathwayMap* pMap1, CEOSAIRegionPathwayMap* pMap2, CEOSAIRegionPathwayMap* pOverlappedMap );

		//
		enum EnumNeutralsPathwayType
		{
			IgnoreNeutrals, // Useful for finding the best possible route, or steamrolling
			SemiPoliteToNeutrals,
			PoliteToNeutrals,
		};
		enum EnumEnemiesPathwayType
		{
			IgnoreEnemies,
			AllowSomeEnemyContact,
			AvoidEnemyContact, // Sneak-Attack or very weak unit
		};

	public:
		CEOSAIRegionPathfinder();
		~CEOSAIRegionPathfinder();
		//
		void SetNumberOfPlayers( long iNumberOfPlayers );

		// Which player is moving?  (This is used for movement through cities)
		//
			void  SetPlayer( long iPlayer );//{ m_iPlayer = iPlayer; }
			long  GetPlayer(){ return m_iPlayer; }
			void  SetForeignRelations( CEOSAIBCDumbArray1D< EOSAIEnumForeignRelations >* pForeignRelations );

		// Pathfinding Key
		//
			long  CreateNewKey(){ m_iCurrentKey++; if( m_iCurrentKey > 2000000000 ){ m_iCurrentKey = 1; } return m_iCurrentKey; }

		// Pathfinding System
		//
			void ClearPathfindingSettings();
		//
			void SetRegionManager( CEOSAIGenericRegionManager* pRegionManager ){ m_pRegionManager = pRegionManager; }
		//
		// Danger / Pathfinder Danger Avoidance Strategy
		//
			void ClearPlayerWeights();
			//
			void ApplyStrategy( EnumNeutralsPathwayType eNeutralStrategy, EnumEnemiesPathwayType eEnemyStrategy, float fAIPower );
			//
			void SetToPolitePathway( long iPlayer );
			void SetToIgnoreOtherPlayersPathway( long iPlayer );
			void SetToDangerAvoidancePathway( long iPlayer );
			//
			//void AddBlockingUnits( bool b ){ m_bAddBlockingUnits = b; } // blocked by land/sea units (Solve3)
		//
		// Movement Rate - preferential terrain movement, time-to-target calculations
		//
			//void SetLandMovementRate( float fRate );
			//void SetSeaMovementRate( float fRate );
			void SetMovementRate( CEOSAIUnitTemplate* pAIUnitTemplate );
			void SetMovementRate( CEOSAITerrainBasedValue* pMovementRates ){ m_TerrainBasedMovementRates = *pMovementRates; }
			void SetMovementRate( long iTerrain, float fMovementRate ){ m_TerrainBasedMovementRates.m_fTerrainValue[iTerrain] = fMovementRate; }
		//
		// Distance 
		//
			// Used to favor land or water (a LandDistanceMultiplier of 2.0 makes land distances seem 2x farther)
			void SetLandDistanceMultiplier( float fMult ){ m_fLandDistanceMultiplier = fMult; }
			void SetSeaDistanceMultiplier( float fMult ){ m_fSeaDistanceMultiplier = fMult; }
			void SetDistanceMultiplier( float fMult ){ m_fLandDistanceMultiplier = m_fSeaDistanceMultiplier = fMult; }
			//
			void SetMaximumDistance( float f ){ m_fMaximumDistance = f; }
		//
		// Geo-Switches
		//
			void SetGeoSwitches_TreatInitialCitiesAsLand( bool b ){ m_bTreatInitialCitiesAsLand = b; }
			void SetGeoSwitches_TreatInitialCitiesAsWater( bool b ){ m_bTreatInitialCitiesAsWater = b; }
			void SetGeoSwitchMultiplier( float fMult ){ m_fGeoSwitchMultiplier = fMult; }
			void SetMaximumGeoSwitches( long iMax ){ m_iMaxGeoSwitches = iMax; }
		//
		// Start Locations
		//
			void ClearStartRegions(){ m_StartingPathfindingRegions.RemoveAll(); }
			//
			void AddStartLocation( CEOSAILocation Location );
			void AddStartingRegion( CEOSAIGenericRegion* pRegion );
			void AddStartingRegion( long iRegion );
			void AddStartingPathfinderPoint( long iRegion );
			void AddStartingRegionPathwayItem( CEOSAIPathfinderPoint* pPathfinderPoint );
			void AddStartingRegionPathwayItemWithTimeAndPathfindingValue( CEOSAIRegionPathwayItem* pStartItem );
			void AddStartingGeo( long iGeo );
			//
			void AddToStartRegions( long iPlayer, long iRegionDefines );
			void AddMyCitiesAsStartRegions( long iPlayer ){ AddToStartRegions( iPlayer, MULTIREGIONS_CITIES ); }
			void AddMyCitiesAndTransportsAsStartRegions( long iPlayer ){ AddToStartRegions( iPlayer, MULTIREGIONS_CITIES | MULTIREGIONS_TRANSPORTS ); }
			void AddMyCitiesAndCityHuntersAsStartRegions( long iPlayer ){ AddToStartRegions( iPlayer, MULTIREGIONS_CITIES | MULTIREGIONS_CITYHUNTERS ); }
			void AddMyCitiesAirfieldsCarriersAsStartRegions( long iPlayer ){ AddToStartRegions( iPlayer, MULTIREGIONS_CITIES | MULTIREGIONS_AIRFIELDS | MULTIREGIONS_CARRIERS ); }
			//
			void AddPlayerCitResUnitRegionsAsStartRegions( long iPlayer );
			void AddPlayerCitResRegionsAsStartRegions( long iPlayer );
			void AddPlayerCitGroundUnitRegionsAsStartRegions( long iPlayer );
		//
		// End Location
		//
			void AddEndLocation( CEOSAILocation Location );
			void AddEndingRegion( CEOSAIGenericRegion* pRegion );
			void AddEndingRegion( long iRegion );
			void AddEndingGeo( long iGeo );
		//
		// Solve
		//
			void Solve( long iPlayer, bool bStartWithLandMovementFromCities );
			void Solve2( long iPlayer, bool bStartWithLandMovementFromCities );
			void Solve3();
			void GetResult( bool bGetAllResults, CEOSAIRegionPathwayMap* pResultData );

	//private:

		// Presence01: If the player has any CitResUnits in the region, then Presence is 1.0. Otherwise 0.0.
		// Sighting01: Includes sighting information from the surrounding area
		// Danger: Based on the unit power in the region.
		//
			void SetEnemyDangerPathfindingMultiplier(   float fWeight );
			void SetEnemyPresencePathfindingMultiplier( float fWeight );
			void SetEnemySightingPathfindingMultiplier( float fWeight );
			void SetEnemyDangerResultMultiplier(   float fWeight );
			void SetEnemyPresenceResultMultiplier( float fWeight );
			void SetEnemySightingResultMultiplier( float fWeight );
		//
			void SetNeutralDangerPathfindingMultiplier(   float fWeight );
			void SetNeutralPresencePathfindingMultiplier( float fWeight );
			void SetNeutralSightingPathfindingMultiplier( float fWeight );
			void SetNeutralDangerResultMultiplier(   float fWeight );
			void SetNeutralPresenceResultMultiplier( float fWeight );
			void SetNeutralSightingResultMultiplier( float fWeight );
		//
			void SetPathfindingAllyNeutralEnemyPresenceMultiplier( long iMovingPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult );
			void SetPathfindingAllyNeutralEnemyDangerMultiplier( long iMovingPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult );
			void SetPathfindingAllyNeutralEnemySightingMultiplier( long iMovingPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult );
			//
			void SetResultAllyNeutralEnemyPresenceMultiplier( long iMovingPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult );
			void SetResultAllyNeutralEnemyDangerMultiplier( long iMovingPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult );
			void SetResultAllyNeutralEnemySightingMultiplier( long iMovingPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult );
		//
			void SetPlayerPresencePathfindingMultiplier( long iPlayer, float fMult ){ m_PlayerWeights[iPlayer]->m_fPlayerPresencePathfindingMultiplier = fMult; }
			void SetPlayerDangerPathfindingMultiplier( long iPlayer, float fMult ){ m_PlayerWeights[iPlayer]->m_fPlayerDangerPathfindingMultiplier = fMult; }
			void SetPlayerSightingPathfindingMultiplier( long iPlayer, float fMult ){ m_PlayerWeights[iPlayer]->m_fPlayerSightingPathfindingMultiplier = fMult; }

	private:

		//CWorldDescServer*         m_pWorldDescServer;
		CEOSAIBCDumbArray1D< EOSAIEnumForeignRelations >  m_OwnerForeignRelations; // Team, Alliance, Neutral, War, etc
		//CGlobalForeignRelations*  m_pGlobalForeignRelations;
		CEOSAIGenericRegionManager*    m_pRegionManager; // not owned
		//
		CList< CEOSAIGenericRegion* >      m_StartingPathfindingRegions;
		//CList< CEOSAIPathfinderPoint* >    m_StartingPathfindingRegions;
		CList< CEOSAIRegionPathwayItem* >  m_StartingPathwayItems;//pStartValues 
		//

		// Pathfinding
		bool  m_bDebug_IsBeingSetup; // debug

		long  m_iCurrentKey;

		long  m_iPlayer;

	//	EnumNeutralsPathwayType  m_eNeutralStrategy;
	//	EnumEnemiesPathwayType   m_eEnemyStrategy;

		//bool  m_bAddBlockingUnits;

		CEOSAITerrainBasedValue  m_TerrainBasedMovementRates;
		float m_fLandDistanceMultiplier;
		float m_fSeaDistanceMultiplier;
		float m_fMaximumDistance;
		//
		bool  m_bTreatInitialCitiesAsLand;
		bool  m_bTreatInitialCitiesAsWater;
		float m_fGeoSwitchMultiplier;
		long  m_iMaxGeoSwitches;

		// Adjusted Values are used in calculating the PathfindingValue
		float GetAdjustedDangerValueOfRegion( CEOSAIGenericRegion* pRegion );
		float GetAdjustedPresenceValueOfRegion( CEOSAIGenericRegion* pRegion );
		float GetAdjustedSightingValueOfRegion( CEOSAIGenericRegion* pRegion );

		// Real Values are used after the fact.  They allow me to calculate paths while ignoring
		//   the danger inherent in the pathway, and then give me the danger value at the end.
		// The adjusted/real split allows me to do things like calculate several paths from
		//   A to B, and calculate the actual danger/sighting/presence values of each of them.
		float GetRealDangerValueOfRegion( CEOSAIGenericRegion* pRegion );
		float GetRealPresenceValueOfRegion( CEOSAIGenericRegion* pRegion );
		float GetRealSightingValueOfRegion( CEOSAIGenericRegion* pRegion );

		// Pathfinding Values
		//CList< CEOSAIGenericRegion* >  m_CurrentPathfindingRegions;
		CList< CEOSAIPathfinderPoint* >  m_CurrentPathfindingRegions;

		// Player sighting/danger weight
		CEOSAIBCDumbArray1D< CEOSAIRegionPathfinderPlayerWeights* >  m_PlayerWeights;
};

//};

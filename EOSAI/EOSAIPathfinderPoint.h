
#pragma once

#include "EOSAILocation.h"
class CEOSAIGenericRegionManager;
#define TERRAIN_WATER 1
#define TERRAIN_GRASS 5
//#define TERRAIN_ROAD  8
//#define TERRAIN_CITY  9
#define TERRAIN_ROAD  7
#define TERRAIN_CITY  8

class CEOSAIPathfinderPoint
{
	public:
		CEOSAIPathfinderPoint()
		{
			m_iTerrainType3 = 0;
			m_iTerrainType9 = 0;
			//
			m_iId = 0;
			//
			m_fTempPathfindingValue = 0.0f;
			m_fTempTotalDistance = 0.0f;
			m_fTempTotalTime = 0.0f;
			m_fTempTotalDanger = 0.0f;
			m_fTempTotalPresence = 0.0f;
			m_fTempTotalSighting = 0.0f;
			m_iTempVisitedKey = 0;
			m_iTempEndKey = 0;
			m_bTempIsInTheList = false;
			m_iTempNumberOfGeoSwitches = 0;
			m_iTempIncomingTerrain = 0;
			m_pTempPreviousStep = NULL;

			//m_iBlockedKey = 0;
		}
		virtual ~CEOSAIPathfinderPoint(){}

		//virtual void DoNothing(){}
		CEOSAIGenericRegionManager* GetGenericRegionManager(){ return m_pRegionManager; }

		void    SetId( long iId ){ m_iId = iId; }
		long    GetId(){ return m_iId; }

		void       SetLocation( CEOSAILocation Location ){ m_Location = Location; }
		CEOSAILocation  GetLocation(){ return m_Location; }

		//void  IsBlocked( long iCurrentKey ){ m_iBlockedKey = iCurrentKey; }

		// Classifications
		bool  IsLand(){ return m_iTerrainType3 == TERRAIN_GRASS; }
		bool  IsCity(){ return m_iTerrainType3 == TERRAIN_CITY; }
		bool  IsWater(){ return m_iTerrainType3 == TERRAIN_WATER; }
		long  GetTerrainType3(){ return m_iTerrainType3; }

		//
		CEOSAIGenericRegionManager*  m_pRegionManager;
		//
		long   m_iId;
		//
		CEOSAILocation  m_Location;
		long  m_iTerrainType3; // TERRAIN_WATER, TERRAIN_GRASS, TERRAIN_CITY
		long  m_iTerrainType9; // Same as "m_iTerrainType3" in MultiRegions

		//long  m_iBlockedKey;
/*
		CList< CEOSAIPathfinderPoint* >  m_AdjacentPathfinderSteps;
*/
	//
	// Temporary information (used by pathfinding)
	//
		float m_fTempPathfindingValue; // Weighted by Distance, Danger, GeoSwitches, etc
		float m_fTempTotalDistance;
		float m_fTempTotalTime;
		float m_fTempTotalDanger;
		float m_fTempTotalPresence;
		float m_fTempTotalSighting;
		long  m_iTempVisitedKey;
		long  m_iTempEndKey;
		bool  m_bTempIsInTheList;
		long  m_iTempNumberOfGeoSwitches;
		long  m_iTempIncomingTerrain;
		CEOSAIPathfinderPoint* m_pTempPreviousStep;
};


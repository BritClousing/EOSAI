
#pragma once

//
// Stores data on Nationwide pathways (i.e. pathways from the nation - as a whole - to other places)
//

#include "EOSAIMultiRegion2.h"
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAIRegionPathfinder.h"
#include "EOSAIMultiRegionAttackAccess.h"
class CWorldDesc;

class CEOSAIMultiRegionNationwidePathways
{
	public:
		CEOSAIMultiRegionNationwidePathways();
		~CEOSAIMultiRegionNationwidePathways();

		void  SetPlayer( long iPlayer ){ m_iPlayer = iPlayer; }
		long  GetPlayer(){ return m_iPlayer; }
		//void  SetWorldDesc( CWorldDesc* pWorldDesc ){ m_pWorldDesc = pWorldDesc; }
		void  SetMultiRegionManager( CEOSAIMultiRegionManager2* pMultiRegionManager ){ m_pMultiRegionManager = pMultiRegionManager; }

		// Calculations
		void  Calculate();
		//{
		//	CalculateSightingAndPathwayInformation();
		//	CalculatePathwayPoiObjectsForEachMultiRegion();
		//}

		// Results
		CEOSAIRegionPathwayMap*    GetPolitePathFromMyCitResUnitsToRestOfWorld(){ return &m_PolitePathFromMyCitResUnitsToRestOfWorld; }
		//
		CEOSAIRegionPathwayItem*   GetPolitePathfindingValueFromMyCitResUnits( long i ){ return m_PolitePathFromMyCitResUnitsToRestOfWorld.GetRegionResult( i ); }
		//CMultiRegionPathwayItem*  GetDistGeoPathfindingValueFromMyCitResUnits( long iMultiRegion ){ return m_DistGeoPathFromMyCitResUnitsToRestOfWorld.GetMultiRegionResult( iMultiRegion ); }
		CEOSAIRegionPathwayItem*   GetDistGeoPathfindingValueFromMyCitResUnits( long iMultiRegion ){ return m_DistGeoPathFromMyCitResUnitsToRestOfWorld.GetRegionResult( iMultiRegion ); }
		CEOSAIRegionPathwayItem*   GetDistGeoPathfindingValueFromMyCitRes( long iMultiRegion ){ return m_DistGeoPathFromMyCitResToRestOfWorld.GetRegionResult( iMultiRegion ); }
		CEOSAIRegionPathwayItem*   GetMinimumGeoSwitchPathfindingValueFromMyCitResUnits( long iMultiRegion ){ return m_MinimumGeoSwitchPathwayFromMyCitResUnitsToRestOfWorld.GetRegionResult( iMultiRegion ); }

		// AttackPathway is based on:
		//   CEOSAIRegionPathfinder::SemiPoliteToNeutrals, CEOSAIRegionPathfinder::AllowSomeEnemyContact, 10.0f
		CEOSAIRegionPathwayItem*   GetPlayerAttackPathwayBasedOnCitResUnits( long iMultiRegion ){ return m_PlayerAttackPathwayArrayBasedOnCitResUnits.GetRegionResult( iMultiRegion ); }
		CEOSAIRegionPathwayItem*   GetPlayerAttackPathwayBasedOnCitRes( long iMultiRegion ){ return m_PlayerAttackPathwayArrayBasedOnCitRes.GetRegionResult( iMultiRegion ); }

		//float  GetMultiRegion_GroundSeaAttackAccess01( long iMultiRegion ){ return m_MultiRegion_GroundSeaAttackAccess01[iMultiRegion]; }
		//float  GetMultiRegion_AirAttackAccess01( long iMultiRegion ){ return m_MultiRegion_AirAttackAccess01[iMultiRegion]; }
		CEOSAIMultiRegionAttackAccess*  GetMultiRegionAttackAccess( long iMultiRegion ){ return m_MultiRegionAttackAccess[iMultiRegion]; }

	private:

		void  CalculateSightingAndPathwayInformation();
		void  CalculatePathwayPoiObjectsForEachMultiRegion();
		//void  CalculateLocalAndPathwayAIPoiObjects();

		//
		long  m_iPlayer;
		//CWorldDesc* m_pWorldDesc;
		CEOSAIMultiRegionManager2* m_pMultiRegionManager;

		//CList< CMultiRegion* >  m_FreelyAccessibleMultiRegions;
		//CList< CMultiRegion* >  m_FrontMultiRegionList;

		//CMultiRegionPathwayMap  m_PolitePathFromMyCitResUnitsToRestOfWorld;
		CEOSAIRegionPathwayMap   m_PolitePathFromMyCitResUnitsToRestOfWorld;
		CEOSAIRegionPathwayMap   m_DistGeoPathFromMyCitResUnitsToRestOfWorld;
		//CMultiRegionPathwayMap  m_DistGeoPathFromMyCitResUnitsToRestOfWorld;
		CEOSAIRegionPathwayMap   m_DistGeoPathFromMyCitResToRestOfWorld;
		CEOSAIRegionPathwayMap   m_MinimumGeoSwitchPathwayFromMyCitResUnitsToRestOfWorld;
		//CEOSAIRegionPathwayMap   m_LandPathFromMyCitGroundUnitToRestOfWorld;
		CEOSAIRegionPathwayMap   m_SeaPathFromMyCitiesToRestOfWorld; // ignore all resistence

		CEOSAIRegionPathwayMap   m_PlayerAttackPathwayArrayBasedOnCitResUnits;
		CEOSAIRegionPathwayMap   m_PlayerAttackPathwayArrayBasedOnCitRes;

		// ExpectedAttackTarget is based on the CitRes+CitResUnit AttackPathway
		CEOSAIBCDumbArray1D< CEOSAIMultiRegionAttackAccess* >  m_MultiRegionAttackAccess; // 1 = complete access

		// Includes distance, geo, sighting, presence information
		//   I can use this information to figure out how deep in my/enemy territory a multiregion is
		//CEOSAIBCDumbArray1D< CMultiRegionPathwayMap* >  m_PlayerAttackPathwayArrayBasedOnCitResUnits;
		//CEOSAIBCDumbArray1D< CMultiRegionPathwayMap* >  m_PlayerAttackPathwayArrayBasedOnCitRes;
};

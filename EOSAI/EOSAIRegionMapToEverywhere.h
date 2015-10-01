
#pragma once

#include "EOSAIBCDumbArray1D.h"
#include "EOSAIRegionPathwayMap.h"
class CEOSAIUnitTemplate;

// This class exists because I was having some speed/memory issues with the existing system
//   Under the old system, every AIUnit would calculate it's own path to everywhere structure
//   But, in a lot of cases, identical units were in the same location, meaning that I had
//   lots of duplicate calculations (slow) and memory (causing massive memory consumption)
// This structure stores pathway information, but matches calculations to AIRegions to avoid duplication

namespace EOSAI
{
class CAICachedMap
{
	public:
		CAICachedMap(){ m_pActorAIUnitTemplate = NULL; }
		CEOSAIRegionPathwayMap* InvokeDirectAIRegionMapToEverywhere();
		CEOSAIRegionPathwayMap* InvokeDirectAIRegionMapToCoasts();

		//long               m_iAIRegion;
		long               m_iPathfinderPointId;
		CEOSAIUnitTemplate*   m_pActorAIUnitTemplate;
		CEOSAIRegionPathwayMap  m_AIRegionMapDirectToEverywhere;
		CEOSAIRegionPathwayMap  m_AIRegionMapDirectToCoasts;
};
};

//class CAIRegionCachedMap
class CEOSAIPathfinderPointCachedMap
{
	public:
		CEOSAIPathfinderPointCachedMap(){}
		~CEOSAIPathfinderPointCachedMap();
		CEOSAIRegionPathwayMap* InvokeDirectAIRegionMapToEverywhere( CEOSAIUnitTemplate* pActorUnitTemplate );
		CEOSAIRegionPathwayMap* InvokeDirectAIRegionMapToCoasts( CEOSAIUnitTemplate* pActorUnitTemplate );

		//long  m_iAIRegion;
		long  m_iPathfinderPointId;
		CList< EOSAI::CAICachedMap* >  m_AICachedMaps;
};

class CEOSAIRegionMapToEverywhere
{
	public:
		CEOSAIRegionMapToEverywhere(){}
		~CEOSAIRegionMapToEverywhere();
		void  Clear();

		CEOSAIRegionPathwayMap* InvokeDirectAIRegionMapToEverywhere( long iStartingAIRegion, CEOSAIUnitTemplate* pActorUnitTemplate );
		CEOSAIRegionPathwayMap* InvokeDirectAIRegionMapToCoasts( long iStartingAIRegion, CEOSAIUnitTemplate* pActorUnitTemplate );

		//CEOSAIBCDumbArray1D< CAIRegionCachedMap* >  m_StartingAIRegions;
		CEOSAIBCDumbArray1D< CEOSAIPathfinderPointCachedMap* >  m_StartingPathfinderPoints;
};



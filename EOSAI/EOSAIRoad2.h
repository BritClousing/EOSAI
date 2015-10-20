
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIPathfinderPoint.h"
#include "EOSAILocation.h"
class CEOSAIRoad2;
//namespace EOSAI
//{
	class CEOSAIGenericRegion;
//}
class CEOSAIRegion2;

// Describes a section of road for the AI
//   The CEOSAIRoad2 object points to a bit of road that occurs on the edge of AIRegions
//   It connects to other CEOSAIRoad2 objects which occur on the edges of other AIRegions
//      and records the distance between these bits.
//   It is used in figuring out the fastest paths in GenericPathfinder.

class CEOSAIAdjacentAIRoad2Distance
{
	public:
		CEOSAIAdjacentAIRoad2Distance()
		{
			m_pIntermediateRegion = NULL;
			m_pAIRoad = NULL;
			m_fPixelDistance = 0.0f;
		}

		CEOSAIGenericRegion*  m_pIntermediateRegion;
		CEOSAIRoad2*   m_pAIRoad;
		float      m_fPixelDistance;
};

class DLLIMPEXP CEOSAIRoad2 : public CEOSAIPathfinderPoint
{
	//
	// The AIRoad Location is one-step inside the edge of two AIRegion boundaries.
	//    The AIRoad exists within a single AIRegion (stored in m_iAIRegion1).
	//    The AIRoad eventually runs into another AIRegion (stored in m_iAIRegion2).
	//
	// Both AIRegions (m_iAIRegion1 + m_iAIRegion2) contain pointers to this AIRoad (via AddAIRoad()).
	//
	public:
		CEOSAIRoad2( long iPathfinderPointId );
		virtual ~CEOSAIRoad2();

		CEOSAIAdjacentAIRoad2Distance* GetRoadDistance( CEOSAIRoad2* pAIRoad );
		CEOSAIRegion2* GetAIRegion1();
		CEOSAIRegion2* GetAIRegion2();

		long   m_iAIRegion1;
		long   m_iAIRegion2;

		long   m_iRoadChunkId;

		CList< CEOSAIAdjacentAIRoad2Distance* >  m_RoadsIConnectTo;
};

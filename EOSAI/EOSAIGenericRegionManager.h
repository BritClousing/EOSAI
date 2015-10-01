
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAILocation.h"

class CEOSAIGenericRegion;
class CEOSAIPathfinderPoint;

class DLLIMPEXP CEOSAIGenericRegionManager
{
	public:
		CEOSAIGenericRegionManager(){}

		long    GetNumberOfGenericRegions(){ return (long) m_GenericRegionList.GetCount(); }
		//long    GetNumberOfPathfinderPoints(){ return (long) m_PathfinderPointArray.GetUpperBound()-1; }

		virtual CEOSAIGenericRegion*  GetGenericRegion( CEOSAILocation location ){ ASSERT( false ); return NULL; }
		virtual CEOSAIGenericRegion*  GetGenericRegion( long iId ){ ASSERT( false ); return NULL; }
		CEOSAIPathfinderPoint*  GetPathfinderPoint( long iId ){ return m_PathfinderPointArray.GetAt( iId ); }

		//void    AddPathfinderPoint( CEOSAIPathfinderPoint* pPathfinderPoint );

		CArray< CEOSAIPathfinderPoint* >  m_PathfinderPointArray;
		CList< CEOSAIGenericRegion* >     m_GenericRegionList;
};


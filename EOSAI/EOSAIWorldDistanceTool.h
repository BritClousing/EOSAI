
#pragma once
#include "EOSAILocation.h"

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

// This is used to calculate the air-distance between locations on the map.
//   It needs to be overridden by the game (i.e. derive a new WorldDistanceTool from EOSAI::CWorldDistanceTool and then pass it into the EOSAIInterface).

namespace EOSAI
{
class DLLIMPEXP CWorldDistanceTool
{
	public:
		CWorldDistanceTool(){ m_iPixelWidth = 0; m_iPixelHeight = 0; }

		virtual float GetDistance( CEOSAILocation Location1, CEOSAILocation Location2 ){ return 0.0f; }
		virtual void  TruncateLocation( float* fRealX, float* fRealY ){ ASSERT( false ); }

		virtual CEOSAILocation GetAverageLocation( CList< CEOSAILocation >* pLocations );//{ return CEOSAILocation( Pixel,0,0 ); }

		// This is used by the MultiRegionManager Generate method.  It should be depreciated.
		long GetPixelWidth(){  ASSERT( m_iPixelWidth  > 0 ); return m_iPixelWidth; }
		long GetPixelHeight(){ ASSERT( m_iPixelHeight > 0 ); return m_iPixelHeight; }

		long m_iPixelWidth;
		long m_iPixelHeight;
};
};

extern EOSAI::CWorldDistanceTool* g_pWorldDistanceTool;

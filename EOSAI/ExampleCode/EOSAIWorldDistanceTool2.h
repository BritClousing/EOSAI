
#pragma once
#include "Location.h"
#include "EOSAI\Include\EOSAIWorldDistanceTool.h"
class CWorldBuildDesc;

class CEOSAIWorldDistanceTool2 : public CEOSAIWorldDistanceTool
{
	public:
		CEOSAIWorldDistanceTool2(){ m_pWorldBuildDesc = NULL; }
		void SetWorldBuildDesc( CWorldBuildDesc* pWorldBuildDesc ){ m_pWorldBuildDesc = pWorldBuildDesc; }

		virtual float GetDistance( CEOSAILocation Location1, CEOSAILocation Location2 );
		virtual void  TruncateLocation( float* fRealX, float* fRealY ){ ASSERT( false ); }

		virtual CEOSAILocation GetAverageLocation( CList< CEOSAILocation >* pLocations );

		// This is used by the MultiRegionManager Generate method.  It should be depreciated.
		long GetPixelWidth(){ ASSERT( false ); return 1000; }
		long GetPixelHeight(){ ASSERT( false ); return 1000; }

		CWorldBuildDesc* m_pWorldBuildDesc;
		//CWorldDescServer* m_pWorldDescServer;
};

//extern CAIWorldDistanceTool* g_pAIWorldDistanceTool;

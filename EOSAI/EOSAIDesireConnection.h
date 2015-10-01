
#pragma once

//
// This links a SpatialDesire to another SpatialDesire.
//   I may use it to connect a MultiRegion to spatial desires as well.
//

#include "EOSAILocation.h"
class CEOSAIDesireSpatial;

class CEOSAIDesireConnection
{
	public:
		CEOSAIDesireConnection()
		{
			m_iSynergy = 0;
			m_fAirDistance = 0.0f;
			m_bStartsOnLand = false;
			m_fSingleGeoDistance = 0.0f;
			m_iMinimumGeoSwitches = 0;
			m_pConnectedAISpatialDesire = NULL;
		}
		void   Calculate( long iPlayer, CEOSAIDesireSpatial* pStartDesire, CEOSAILocation Location2 );
		//void   Calculate( long iPlayer, CEOSAILocation Location1, CEOSAILocation Location2 );
		float  GetConnectionValue01(); // Based on values below

		long   m_iSynergy;
		//
		float  m_fAirDistance;
		bool   m_bStartsOnLand;
		float  m_fSingleGeoDistance;
		long   m_iMinimumGeoSwitches;

		CEOSAIDesireSpatial* m_pConnectedAISpatialDesire;
};


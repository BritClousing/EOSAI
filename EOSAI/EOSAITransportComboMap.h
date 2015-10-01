
#pragma once

#include "EOSAIRegionPathwayMap.h"
class CEOSAIUnitTemplate;

class CEOSAITransportCombinedMap
{
	public:
		CEOSAITransportCombinedMap()
		{
			m_pAITransport = NULL;
		}
		void  SetTransportUnitTemplate( CEOSAIUnitTemplate* pAITransport ){ m_pAITransport = pAITransport; }

		CEOSAIUnitTemplate*   m_pAITransport;
		CEOSAIRegionPathwayMap  m_PathwayMap;
		//CEOSAIRegionPathwayMap  m_MultiRegionPathwayMap;
};


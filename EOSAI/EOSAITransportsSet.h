
#pragma once

// Is used to store the Cities, Airfield, and Carriers.
//   It should include the cities and airfields of allies (because my aircraft can use them).

#include "EOSAILocation.h"
class CEOSAIThoughtDatabase;
class CEOSAIUnitTemplate;
class CEOSAIPoiObject;
class CEOSAIUnit;
class CEOSAICity;

class CEOSAITransportsSet
{
	public:
		CEOSAITransportsSet( CEOSAIThoughtDatabase* pThoughtDatabase, CEOSAIUnitTemplate* pUnitToTransport );
		//void   SetGroundUnitToTransport( CUnitTemplate* pUnitTemplate ){ m_pUnitToTransport = pUnitTemplate; }
		//void   Clear(){ m_ExistingTransports.RemoveAll(); m_BuildableTransports.RemoveAll(); m_Cities.RemoveAll(); }

		float  GetEarliestArrivalTimeToLocation_UseCrowsFlight( CEOSAILocation Location );
		float  GetFastestMovementRate();

		//
		CEOSAIThoughtDatabase* m_pThoughtDatabase;
		long                m_iPlayer;
		CEOSAIUnitTemplate*    m_pUnitToTransport;

		// Rather than collect all the information for later processing, I'll just access the 
		//   ThoughtDatabase when needed to lookup the information.
		//
		CList< CEOSAIUnit* >   m_ExistingTransports;

		// Information about buildable transports
		CList< CEOSAICity* >          m_Cities;
		CList< CEOSAIUnitTemplate* >  m_BuildableTransports;
};


#pragma once

// Is used to store the Cities, Airfield, and Carriers.
//   It should include the cities and airfields of allies (because my aircraft can use them).

#include "EOSAILocation.h"
class CEOSAIPoiObject;

class CEOSAIAirbasesSet
{
	public:
		CEOSAIAirbasesSet(){}

		float  GetClosestAirbaseDistance( CEOSAILocation Location );
		float  GetClosestAirbaseDistanceX5Turns( CEOSAILocation Location ); // assuming carriers move towards point

		float  GetClosestAirbaseDistance_IgnoreOneAirbase( CEOSAIPoiObject* pAirbase, CEOSAILocation Location );

		void   GetAirbasesWithRange( CEOSAILocation Location, float fRange, CEOSAIAirbasesSet* pNewAirbasesSet );

		CEOSAIPoiObject* GetClosestAirbase( CEOSAILocation Location );

		CList< CEOSAIPoiObject* >  m_Airbases;
};

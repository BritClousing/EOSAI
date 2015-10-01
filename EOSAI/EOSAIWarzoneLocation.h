
#pragma once

#include "EOSAILocation.h"

class CEOSAIWarzoneLocation
{
	public:
		CEOSAIWarzoneLocation()
		{
			m_fWarzoneSignificance01 = 0.0f;
			m_fSATUnitPower = 0.0f;
			m_fSATCitResValue = 0.0f;
			m_fNEUnitPower = 0.0f;
			m_fNECitResValue = 0.0f;
		}

		CEOSAILocation  m_Location;
		float      m_fWarzoneSignificance01;

		CList< long >  m_AIRegions;

		float  m_fSATUnitPower;
		float  m_fSATCitResValue;
		float  m_fNEUnitPower;
		float  m_fNECitResValue;
};


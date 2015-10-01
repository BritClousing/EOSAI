
#pragma once

class CEOSAIMultiRegionAttackAccess
{
	public:
		CEOSAIMultiRegionAttackAccess()
		{
			m_fGroundSeaUnitAccess01 = 0.0f; // Includes "Resistence"
			m_fSubmarineAccess01 = 0.0f; // Ignores all resistence, water only
			m_fAirUnitAccess01 = 0.0f;
		}
		void   SetGroundSeaUnitAccess01( float f ){ m_fGroundSeaUnitAccess01 = f; }
		float  GetGroundSeaUnitAccess01(){ return m_fGroundSeaUnitAccess01; }
		void   SetSubmarineAccess01( float f ){ m_fSubmarineAccess01 = f; }
		float  GetSubmarineAccess01(){ return m_fSubmarineAccess01; }
		void   SetAirUnitAccess01( float f ){ m_fAirUnitAccess01 = f; }
		float  GetAirUnitAccess01(){ return m_fAirUnitAccess01; }

	private:
		float  m_fGroundSeaUnitAccess01;
		float  m_fSubmarineAccess01;
		float  m_fAirUnitAccess01; // based on airbases + aircraft ranges
};



#pragma once

class CEOSAICitResValueMultiplier
{
	public:
		CEOSAICitResValueMultiplier()
		{
			m_fCityProductionValueMultiplier = 1.0f;
			m_fIronValueMultiplier = 1.0f;
			m_fOilValueMultiplier = 1.0f;
			m_fFoodValueMultiplier = 1.0f;
			m_fGoldValueMultiplier = 1.0f;
		}
		float GetResourceValueMultiplier( CString strResourceType );
		float GetIronValueMultiplier(){ return m_fIronValueMultiplier; }

		float  m_fCityProductionValueMultiplier;
		float  m_fIronValueMultiplier;
		float  m_fOilValueMultiplier;
		float  m_fFoodValueMultiplier;
		float  m_fGoldValueMultiplier;
};



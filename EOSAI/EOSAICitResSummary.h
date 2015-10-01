
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAICitResValueMultiplier.h"
class CEOSAIPoiObject;

class DLLIMPEXP CEOSAICitResSummary
{
	public:
		CEOSAICitResSummary()
		{
			Clear();
		}

		void  Clear();

		void  operator+=( CEOSAICitResSummary& Other );
		void  AddToSummary( CEOSAIPoiObject* pAIPoiObject );

		float GetCityValue(){ return m_fCityProduction; }
		float GetCityValue( CEOSAICitResValueMultiplier* pMult ){ return m_fCityProduction * pMult->m_fCityProductionValueMultiplier; }

		float GetResValue(){ return m_fGoldProduction + m_fIronProduction + m_fFoodProduction + m_fOilProduction; }

		float GetCitResValue(){ return m_fCityProduction + m_fGoldProduction + m_fIronProduction + m_fFoodProduction + m_fOilProduction; }
		float GetCitResValue( CEOSAICitResValueMultiplier* pMult )
		{
			return m_fCityProduction * pMult->m_fCityProductionValueMultiplier + 
				   m_fGoldProduction * pMult->m_fGoldValueMultiplier + 
				   m_fIronProduction * pMult->m_fIronValueMultiplier + 
				   m_fFoodProduction * pMult->m_fFoodValueMultiplier + 
				   m_fOilProduction  * pMult->m_fOilValueMultiplier;
		}

		long   m_iCitResCount;
		float  m_fCityProduction;
		float  m_fGoldProduction;
		float  m_fIronProduction;
		float  m_fFoodProduction;
		float  m_fOilProduction;
};



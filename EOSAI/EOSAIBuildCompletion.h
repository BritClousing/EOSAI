
#pragma once

// The BuildingCompletion class is used to describe how far along a Build-Item is to completion.
//   For example, if a city is building a battleship, and it has completed 15 turns of production, this class describes that.
//   This information is useful to the AI so that it knows to continue building items which are partially completed.

class CEOSAIBuildCompletion
{
	public:
		CEOSAIBuildCompletion()
		{
			m_fPercentComplete01 = 0.0f;
			m_fTimeUntilCompletion = 0.0f;
		}
		float GetPercentComplete(){ return m_fPercentComplete01; } // Ranges from 0.0 to 1.0
		float TimeUntilCompletion(){ return m_fTimeUntilCompletion; }
		CEOSAIBuildOption*  GetAIBuildOption(){ return m_pAIBuildOption; }

		//void          AddToProductionInvested( float fProd ){ m_fProductionInvested += fProd; }
		void   SetProductionInvested( float fProd ){ m_fProductionInvested = fProd; }
		float  GetProductionInvested(){ return m_fProductionInvested; }

		CEOSAIBuildOption*  m_pAIBuildOption;
		//CString  m_strItemInProduction;
		float    m_fPercentComplete01;
		float    m_fTimeUntilCompletion;
		float    m_fProductionInvested;
};



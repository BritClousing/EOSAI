
#pragma once

class CEOSAIDesireSatisfactionSummary
{
	public:
		CEOSAIDesireSatisfactionSummary()
		{
			m_bNeededATransport = false;
			m_bTransportIsNotWellPositionedForThisAction = false;
			m_bUnsureHowToReachTheDropoffCoast = false;
			//
			m_fNeedCityHunter = 0.0f;
			m_fNeedTransport = 0.0f;
			m_fNeedExplorer = 0.0f;
		}

		bool  m_bNeededATransport;
		bool  m_bTransportIsNotWellPositionedForThisAction;
		bool  m_bUnsureHowToReachTheDropoffCoast;

	private:
		float m_fNeedCityHunter;
		float m_fNeedTransport;
		float m_fNeedExplorer;

};


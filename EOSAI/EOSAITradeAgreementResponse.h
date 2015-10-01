
#pragma once

class CEOSAITradeAgreementResponse
{
	// Used by the AI to track responses
	public:
		CEOSAITradeAgreementResponse(){ m_iFromPlayer = 0; }
		CEOSAITradeAgreementResponse( 
			CString  strTradeAgreementId,
			long     iPlayer,
			EOSAIEnumTradeAgreementResponse  eResponse,
			EOSAIEnumTradeAgreementState  eNewState )
		{
			m_strTradeAgreementId = strTradeAgreementId;
			m_iFromPlayer   = iPlayer;
			m_eResponse = eResponse;
		}

		CString  m_strTradeAgreementId;
		long  m_iFromPlayer;
		EOSAIEnumTradeAgreementResponse  m_eResponse;
};


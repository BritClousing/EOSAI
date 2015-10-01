#pragma once

#include "MessageFromAI.h"

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

// Tells the player whether the AI accepts or rejects the trade offer.

#include "EOSAIEnumTradeAgreementResponse.h"

namespace EOSAI
{
	class DLLIMPEXP MessageFromAI_TradeOfferResponse : public MessageFromAI
	{
	public:
		MessageFromAI_TradeOfferResponse()
		{
			m_iSendToPlayer = 0;
			m_iFromAIPlayer = 0;

			//m_fTradeValue11 = 0.0f; // -1.0 (AI doesn't want it) to 0.0 (okay trade) to +1.0 (gift)
			//m_fLumpSumValueToAI = 0.0f;
			//m_fLumpSumValueToPlayer = 0.0f;
			//m_fForeignRelationsBonus = 0.0f;
			//m_bAIDoesntWantToSellThatMuch = false;
		}

		int    m_iSendToPlayer = 0; // Player sending the TradeOffer
		int    m_iFromAIPlayer = 0;

		CString m_strTradeAgreementId;
		EOSAIEnumTradeAgreementResponse m_eResponse = EOSAIEnumTradeAgreementResponse_NoResponse;

		//float  m_fTradeValue11; // -1.0 (AI doesn't want it) to 0.0 (okay trade) to +1.0 (gift to the AI)
		//float  m_fLumpSumValueToAI;
		//float  m_fLumpSumValueToPlayer;
		//float  m_fForeignRelationsBonus;
		//bool   m_bAIDoesntWantToSellThatMuch;
	};
}


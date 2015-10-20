#pragma once

#include "MessageFromAI.h"
#include <map>

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

// Equivalent to EOS' CAITradeEvaluationResult

namespace EOSAI
{
	class DLLIMPEXP MessageFromAI_TradeOfferOpinion : public MessageFromAI
	{
	public:
		MessageFromAI_TradeOfferOpinion()
		{
			m_iSendToPlayer = 0;
			m_iFromAIPlayer = 0;

			m_fTradeValue11 = 0.0f; // -1.0 (AI doesn't want it) to 0.0 (okay trade) to +1.0 (gift)
			m_fLumpSumValueToAI = 0.0f;
			m_fLumpSumValueToPlayer = 0.0f;
			m_fForeignRelationsBonus = 0.0f;
			m_bAIDoesntWantToSellThatMuch = false;
		}

		int    m_iSendToPlayer; // Player sending the TradeOffer
		int    m_iFromAIPlayer;

		float  m_fTradeValue11; // -1.0 (AI doesn't want it) to 0.0 (okay trade) to +1.0 (gift to the AI)
		float  m_fLumpSumValueToAI;
		float  m_fLumpSumValueToPlayer;
		float  m_fForeignRelationsBonus;
		bool   m_bAIDoesntWantToSellThatMuch;
	};
}


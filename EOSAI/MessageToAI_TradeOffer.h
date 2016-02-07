
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "Message.h"
#include "EOSAITradeAgreement.h"

namespace EOSAI
{
class DLLIMPEXP MessageToAI_TradeOffer : public Message
{
	public:
		MessageToAI_TradeOffer(){}

		virtual void PolymorphicType(){}
		
		CEOSAITradeAgreement  m_TradeOffer;
};
}


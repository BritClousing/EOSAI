
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "MessageToAI.h"

namespace EOSAI
{
class DLLIMPEXP MessageToAI_TradeOfferResponse : public MessageToAI
{
	public:
		MessageToAI_TradeOfferResponse(){}

		virtual void PolymorphicType(){}
};
}



#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "MessageToAI.h"

namespace EOSAI
{
class DLLIMPEXP MessageToAI_TradeOffer : public MessageToAI
{
	public:
		MessageToAI_TradeOffer(){}

		virtual void PolymorphicType(){}
};
}


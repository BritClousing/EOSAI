
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "MessageToAI.h"

namespace EOSAI
{
class DLLIMPEXP MessageToAI_RequestTradeDesires : public MessageToAI
{
	public:
		MessageToAI_RequestTradeDesires(){}

		virtual void PolymorphicType(){}
};
}


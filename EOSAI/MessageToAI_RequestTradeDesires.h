
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "Message.h"

namespace EOSAI
{
class DLLIMPEXP MessageToAI_RequestTradeDesires : public Message
{
	public:
		MessageToAI_RequestTradeDesires(){}

		virtual void PolymorphicType(){}
};
}


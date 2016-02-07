
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "Message.h"

namespace EOSAI
{
class DLLIMPEXP MessageToAI_TeamFormed : public Message
{
	public:
		MessageToAI_TeamFormed(){}

		virtual void PolymorphicType(){}
};
}


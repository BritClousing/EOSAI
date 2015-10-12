
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "MessageToAI.h"

namespace EOSAI
{
class DLLIMPEXP MessageToAI_TeamFormed : public MessageToAI
{
	public:
		MessageToAI_TeamFormed(){}

		virtual void PolymorphicType(){}
};
}



#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

// Messages to AI might be stuff like trade offers.
/*
#include "MessageToAI.h"
namespace EOSAI
{
class DLLIMPEXP MessageToAI_MessageResponse : public MessageToAI
{
	public:
		MessageToAI_MessageResponse(){}

		virtual void PolymorphicType(){}
};
}
*/

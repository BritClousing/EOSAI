
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

// Messages to AI might be stuff like trade offers.

namespace AI
{
class DLLIMPEXP MessageToAI_IMailResponse
{
	public:
		MessageToAI_IMailResponse(){}

		virtual void PolymorphicType(){}
};
}



#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "MessageToAI.h"

// This is a global message, no need to send it to one particular AI.
/*
namespace EOSAI
{
	class DLLIMPEXP MessageToAI_WarWasDeclared : public EOSAI::MessageToAI
	{
	public:
		MessageToAI_WarWasDeclared(int iActor, int iTarget)
		{
			m_iActor = iActor;
			m_iTarget = iTarget;
		}
		virtual bool SendToAllAIs() { return true; } // send to all AIs?

		int m_iActor;
		int m_iTarget;
	};
}
*/

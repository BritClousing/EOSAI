
#pragma once

#include "Message.h"

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
class DLLIMPEXP MessageFromAI_DeclareWar : public Message
{
	public:
		int m_iAttackingPlayer;
		int m_iTargetPlayer;
};
}


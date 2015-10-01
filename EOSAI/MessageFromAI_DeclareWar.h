
#pragma once

#include "MessageFromAI.h"

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
class DLLIMPEXP MessageFromAI_DeclareWar : public MessageFromAI
{
	public:
		int m_iAttackingPlayer;
		int m_iTargetPlayer;
};
}


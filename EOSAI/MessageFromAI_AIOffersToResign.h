
#pragma once

#include "MessageFromAI.h"

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
class DLLIMPEXP MessageFromAI_AIOffersToResign : public MessageFromAI
{
	public:
		//MessageFromAI_AIOffersToResign() {}

		int m_iFromAIPlayer;
		int m_iToPlayer;
		long m_AIMessageUID;

		//std::map<CString, float> m_ResourcesToBuy;
		//std::map<CString, float> m_ResourcesToSell;

		//float GetResourceToSell(CString szResourceName) { return m_ResourcesToBuy[szResourceName]; }
		//float GetResourceToBuy(CString szResourceName) { return m_ResourcesToSell[szResourceName]; }
		//float GetResourceToBuy2(){ return 0.0f; }

		//std::map<std::string,float> m_ResourcesToSell;
		//std::map<std::string,float> m_ResourcesToBuy;
};
}



#pragma once

#include "MessageFromAI.h"
//#include <string>
#include <map>

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
class DLLIMPEXP MessageFromAI_TradeOffer : public MessageFromAI
{
	public:
		int m_iFromPlayer;
		int m_iToPlayer;

		//int m_iItemsToTrade; // TODO
		std::map<CString,float> m_ResourcesToBuy;
		std::map<CString,float> m_ResourcesToSell;

		float GetResourceToSell(CString szResourceName){ return m_ResourcesToBuy[szResourceName]; }
		float GetResourceToBuy(CString szResourceName){ return m_ResourcesToSell[szResourceName]; }
		//float GetResourceToBuy2(){ return 0.0f; }

		//std::map<std::string,float> m_ResourcesToSell;
		//std::map<std::string,float> m_ResourcesToBuy;
};
}


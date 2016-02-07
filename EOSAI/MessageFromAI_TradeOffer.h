
#pragma once

#include "Message.h"
#include "EOSAITradeAgreement.h"
#include <map>

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
class DLLIMPEXP MessageFromAI_TradeOffer : public Message
{
	public:
		int m_iFromPlayer;
		int m_iToPlayer;

		CEOSAITradeAgreement  m_TradeAgreement;

		//int m_iItemsToTrade; // TODO
		//std::map<CString,float> m_ResourcesToBuy;
		//std::map<CString,float> m_ResourcesToSell;

		//float GetResourceToSell(CString szResourceName){ return m_ResourcesToSell[szResourceName]; }
		//float GetResourceToBuy(CString szResourceName){ return m_ResourcesToBuy[szResourceName]; }
		//float GetResourceToBuy2(){ return 0.0f; }

		//std::map<std::string,float> m_ResourcesToSell;
		//std::map<std::string,float> m_ResourcesToBuy;
};
}


#pragma once

#include "MessageFromAI.h"
#include <map>

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
	class DLLIMPEXP MessageFromAI_TradeDesires : public MessageFromAI
	{
	public:
		int m_iFromAIPlayer;
		//int m_iToPlayer;

		//int m_iItemsToTrade; // TODO
		//std::map<CString, float> m_ResourcesToBuy;
		//std::map<CString, float> m_ResourcesToSell;

		//long  m_iAIPlayer;
		long  m_iSendToPlayer;
		bool  m_bTheAIDidNotProvideAnyDataBecauseYouAreAtWar;
		// Resources
		long  m_iWantsToBuyAndSellOil;    // +10 = wants to buy 10 oil,  -10 = wants to sell 10 oil
		float m_fPriceOfOil;              // Buy or Sell Price
		long  m_iWantsToBuyAndSellIron;   // +10 = wants to buy 10 iron, -10 = wants to sell 10 iron
		float m_fPriceOfIron;             // Buy or Sell Price
		long  m_iWantsToBuyAndSellFood;   // +10 = wants to buy 10 food, -10 = wants to sell 10 food
		float m_fPriceOfFood;             // Buy or Sell Price
										  // Technology
										  //float m_fWantsToBuyTechnology01;  // 1.0 = wants to buy technology (higher when behind in tech)
										  // Peace, Alliances
		float m_fWantsPeace01;    // 0.0 = wants to continue war, 1.0 = overwhelmed, wants peace

		//float GetResourceToSell(CString szResourceName) { return m_ResourcesToBuy[szResourceName]; }
		//float GetResourceToBuy(CString szResourceName) { return m_ResourcesToSell[szResourceName]; }
		//float GetResourceToBuy2(){ return 0.0f; }

		//std::map<std::string,float> m_ResourcesToSell;
		//std::map<std::string,float> m_ResourcesToBuy;
	};
}


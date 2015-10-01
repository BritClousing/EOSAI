
#pragma once

class CEOSAISerial;
class CEOSAICitResValueMultiplier;
//#define OPENMARKET_COST_TO_BUY_RESOURCE 3
//#define OPENMARKET_MONEY_FOR_SELLING_RESOURCE 1

// 
class CEOSAITradeAgreementMultiplier
{
	public:
		CEOSAITradeAgreementMultiplier();

		//void Serialize( CEOSAISerial* pSerial );
		//void Deserialize( CEOSAISerial* pSerial );

		void  operator=( CEOSAITradeAgreementMultiplier& other );
		void  SetToOpenMarketPrices();

		// Skew of 0.0 means treat the player like an enemy (overcharge them)
		// Skew of 0.5 means use an objective/fair-value trade with other players + no special res needs
		// Skew of 1.0 means try to help them out (because they're a teammate)
		//void  SetFriendlinessWithOtherPlayer( float fFriendliness ){ m_fFriendlinessWithOtherPlayer = fFriendliness; }
		//
		// 0.0 = excess oil/want to sell, 1.0 = need oil badly
		void  SetOilNeed01(  float fNeed ){ m_fOilNeed01  = fNeed; }
		void  SetIronNeed01( float fNeed ){ m_fIronNeed01 = fNeed; }
		void  SetFoodNeed01( float fNeed ){ m_fFoodNeed01 = fNeed; }
		//
		void  CalculateBuyAndSellPrices( 
				float fFriendlinessWithOtherPlayer, 
				CEOSAICitResValueMultiplier* pAIPlayerMultipliers );

		//
		//float  m_fFriendlinessWithOtherPlayer;
		float  m_fOilNeed01;  // above 0.5 = needs to buy oil
		float  m_fIronNeed01;
		float  m_fFoodNeed01;
		//
		float  m_fBuyMoney;
		float  m_fBuyOilPrice;
		float  m_fBuyIronPrice;
		float  m_fBuyFoodPrice;
		//
		float  m_fSellMoney;
		float  m_fSellOilPrice;
		float  m_fSellIronPrice;
		float  m_fSellFoodPrice;
};


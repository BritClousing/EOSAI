
#include <stdafx.h>
#include "EOSAITradeAgreementMultiplier.h"
#include "EOSAICitResValueMultiplier.h"
#include "EOSAICommonData2.h"
#include "EOSAISerial.h"

CEOSAITradeAgreementMultiplier::CEOSAITradeAgreementMultiplier()
{
	float fAverageValue = 
		( (float) g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() + 
		  (float) g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() ) / 2.0f;

	//m_fFriendlinessWithOtherPlayer = 0.5f;
	m_fOilNeed01 = 0.5f;
	m_fIronNeed01 = 0.5f;
	m_fFoodNeed01 = 0.5f;

	// m_fBuyOilMult = the value of oil according to the AIPlayer.  If the AI has too 
	//   much oil, the number goes down.  If the AI wants more oil, the number goes up.
	//   This number is the amount of money the AI is willing to pay for 1 oil.
	m_fBuyMoney = 1.0f;
	m_fBuyOilPrice = fAverageValue;
	m_fBuyIronPrice = fAverageValue;
	m_fBuyFoodPrice = fAverageValue;
	//
	// m_fSellOilMult = the value of oil according to the AIPlayer.  If the AI has too 
	//   much oil, the number goes down.  If the AI wants more oil, the number goes up.
	m_fSellMoney = 1.0f;
	m_fSellOilPrice = fAverageValue;
	m_fSellIronPrice = fAverageValue;
	m_fSellFoodPrice = fAverageValue;
}
/*
void CEOSAITradeAgreementMultiplier::Serialize( CEOSAISerial* pSerial )
{
	char iVersion = 1;
	pSerial->Serialize( iVersion );

	pSerial->Serialize( m_fOilNeed01 );
	pSerial->Serialize( m_fIronNeed01 );
	pSerial->Serialize( m_fFoodNeed01 );
	//
	pSerial->Serialize( m_fBuyMoney );
	pSerial->Serialize( m_fBuyOilPrice );
	pSerial->Serialize( m_fBuyIronPrice );
	pSerial->Serialize( m_fBuyFoodPrice );
	//
	pSerial->Serialize( m_fSellMoney );
	pSerial->Serialize( m_fSellOilPrice );
	pSerial->Serialize( m_fSellIronPrice );
	pSerial->Serialize( m_fSellFoodPrice );
}

void CEOSAITradeAgreementMultiplier::Deserialize( CEOSAISerial* pSerial )
{
	char iVersion = 1;
	pSerial->Deserialize( iVersion );

	pSerial->Deserialize( m_fOilNeed01 );
	pSerial->Deserialize( m_fIronNeed01 );
	pSerial->Deserialize( m_fFoodNeed01 );
	//
	pSerial->Deserialize( m_fBuyMoney );
	pSerial->Deserialize( m_fBuyOilPrice );
	pSerial->Deserialize( m_fBuyIronPrice );
	pSerial->Deserialize( m_fBuyFoodPrice );
	//
	pSerial->Deserialize( m_fSellMoney );
	pSerial->Deserialize( m_fSellOilPrice );
	pSerial->Deserialize( m_fSellIronPrice );
	pSerial->Deserialize( m_fSellFoodPrice );
}
*/

void CEOSAITradeAgreementMultiplier::operator=( CEOSAITradeAgreementMultiplier& other )
{
	m_fOilNeed01 = other.m_fOilNeed01;
	m_fIronNeed01 = other.m_fIronNeed01;
	m_fFoodNeed01 = other.m_fFoodNeed01;
	//
	m_fBuyMoney = other.m_fBuyMoney;
	m_fBuyOilPrice = other.m_fBuyOilPrice;
	m_fBuyIronPrice = other.m_fBuyIronPrice;
	m_fBuyFoodPrice = other.m_fBuyFoodPrice;
	//
	m_fSellMoney = other.m_fSellMoney;
	m_fSellOilPrice = other.m_fSellOilPrice;
	m_fSellIronPrice = other.m_fSellIronPrice;
	m_fSellFoodPrice = other.m_fSellFoodPrice;
}

void CEOSAITradeAgreementMultiplier::SetToOpenMarketPrices()
{
	//   This number is the amount of money the AI is willing to pay for 1 oil.
	m_fBuyOilPrice  = g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X();
	m_fBuyIronPrice = g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X();
	m_fBuyFoodPrice = g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X();
	//
	//   This number is the amount of money the AI is willing to sell 1 oil for.
	m_fSellOilPrice  = g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X();
	m_fSellIronPrice = g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X();
	m_fSellFoodPrice = g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X();
}

void CEOSAITradeAgreementMultiplier::CalculateBuyAndSellPrices( float fFriendlinessWithOtherPlayer, CEOSAICitResValueMultiplier* pAIPlayerMultipliers )
{
	float fOpenMarketCostToBuyResource = (float) g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X();
	float fOpenMarketMoneyForSellingResource = (float) g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X();
	float fCenterPrice = ( fOpenMarketCostToBuyResource + fOpenMarketMoneyForSellingResource ) / 2.0f;
	float fGapBetweenCenterPriceAndUpperPrice = fOpenMarketCostToBuyResource - fCenterPrice;
	float fGapBetweenOpenMarketBuyAndSell = fOpenMarketCostToBuyResource - fOpenMarketMoneyForSellingResource;

	float fPriceGap = 1.0f - fFriendlinessWithOtherPlayer;

	float fSellResourcePrice = 0.0f;
	float fBuyResourcePrice = 0.0f;

	m_fOilNeed01  = 0.5f * pAIPlayerMultipliers->m_fOilValueMultiplier;
	m_fIronNeed01 = 0.5f * pAIPlayerMultipliers->m_fIronValueMultiplier;
	m_fFoodNeed01 = 0.5f * pAIPlayerMultipliers->m_fFoodValueMultiplier;

	if( m_fOilNeed01  > 1.0f ) m_fOilNeed01 = 1.0f;
	if( m_fIronNeed01 > 1.0f ) m_fIronNeed01 = 1.0f;
	if( m_fFoodNeed01 > 1.0f ) m_fFoodNeed01 = 1.0f;

	for( long i=0; i<3; i++ )
	{
		float fResourceNeed01 = 0.0f;
		if( i == 0 ){ fResourceNeed01 = m_fOilNeed01; }
		if( i == 1 ){ fResourceNeed01 = m_fIronNeed01; }
		if( i == 2 ){ fResourceNeed01 = m_fFoodNeed01; }
		float fPricePoint = fCenterPrice;

		#ifdef _DEBUG
		if( i == 0 )
		{
			// Debug: 24987124
			fResourceNeed01 = 1.0f;
		}
		#endif _DEBUG

		float fMaximumBuyPrice = fOpenMarketCostToBuyResource;
		float fMinimumBuyPrice = fOpenMarketMoneyForSellingResource;
		float fMaximumSellPrice = fOpenMarketCostToBuyResource;
		float fMinimumSellPrice = fOpenMarketMoneyForSellingResource;

		bool bWillingToBuyResource = true;
		bool bWillingToSellResource = true;

		// If the AIPlayer needs a resource, put the sell price up to 3.0 (don't sell it), buy is elevated
		if( fResourceNeed01 >= 0.5f )
		{
			fSellResourcePrice = fOpenMarketCostToBuyResource;
			if( fResourceNeed01 > 0.60f ) // If I really need the oil, raise the PricePoint, don't sell
			{
				float fPricePointIncrease01 = ( fResourceNeed01-0.60f )/0.35f; // increase PricePoint with AI need
				fPricePointIncrease01 *= ( 1.0f - fFriendlinessWithOtherPlayer ); // unless we're friends
				fPricePoint += fPricePointIncrease01 * fGapBetweenCenterPriceAndUpperPrice;
			}
			bWillingToBuyResource = true;
			bWillingToSellResource = false;
		}
		// If the AIPlayer has excess resource, lower the price point (for selling), don't buy
		if( fResourceNeed01 < 0.5f )
		{
			if( fResourceNeed01 < 0.35f )
			{
				float fResourceExcess01 = ( 0.35f-fResourceNeed01 )/0.35f;
				float fPricePointDecrease01 = fResourceExcess01; // decrease PricePoint with AI excess
				//fPricePointDecrease01 *= ( 1.0f - fFriendlinessWithOtherPlayer ); // unless we're friends
				fPricePoint -= fPricePointDecrease01 * fGapBetweenCenterPriceAndUpperPrice;
			}
			bWillingToBuyResource = false;
			bWillingToSellResource = true;
		}

		if( bWillingToBuyResource )
		{
			// don't spend more than 2.75 buying this resource
			//fMaximumBuyPrice = (7.0f/8.0f)*fOpenMarketCostToBuyResource + (1.0f/8.0f)*fOpenMarketMoneyForSellingResource;
			//fMaximumBuyPrice = (7.0f/8.0f)*fOpenMarketCostToBuyResource + (1.0f/8.0f)*fOpenMarketMoneyForSellingResource;
			fMaximumBuyPrice = 0.95f*fGapBetweenOpenMarketBuyAndSell + fOpenMarketMoneyForSellingResource;
		}
		else
		{
			fMaximumBuyPrice = fOpenMarketCostToBuyResource;
		}

		if( bWillingToSellResource )
		{
			// don't sell for less than 1.25
			//fMinimumSellPrice = (1.0f/8.0f)*fOpenMarketCostToBuyResource + (7.0f/8.0f)*fOpenMarketMoneyForSellingResource;
			fMaximumBuyPrice = 0.05f*fGapBetweenOpenMarketBuyAndSell + fOpenMarketMoneyForSellingResource;
		}
		else
		{
			fMinimumSellPrice = fOpenMarketMoneyForSellingResource;
		}

		// This number is the amount of money the AI is willing to pay for 1 oil.
		fBuyResourcePrice  = fPricePoint - fPriceGap;
		// This number is the amount of money the AI is willing to sell 1 oil.
		fSellResourcePrice = fPricePoint + fPriceGap;

		// Limit the buy/sell price ( 1.0-3.0 )
		fSellResourcePrice = min( fSellResourcePrice, fMaximumSellPrice );
		fSellResourcePrice = max( fSellResourcePrice, fMinimumSellPrice );
		fBuyResourcePrice  = min( fBuyResourcePrice,  fMaximumBuyPrice );
		fBuyResourcePrice  = max( fBuyResourcePrice,  fMinimumBuyPrice );

		if( bWillingToBuyResource == false ){ fBuyResourcePrice = fOpenMarketMoneyForSellingResource; }
		if( bWillingToSellResource == false ){ fSellResourcePrice = fOpenMarketCostToBuyResource; }

		if( i == 0 )
		{
			m_fSellOilPrice = fSellResourcePrice;
			m_fBuyOilPrice  = fBuyResourcePrice;
		}
		if( i == 1 )
		{
			m_fSellIronPrice = fSellResourcePrice;
			m_fBuyIronPrice  = fBuyResourcePrice;
		}
		if( i == 2 )
		{
			m_fSellFoodPrice = fSellResourcePrice;
			m_fBuyFoodPrice  = fBuyResourcePrice;
		}
	}

	/*
	float fOilPricePoint = fCenterPrice;
	if( m_fOilNeed > 0.65f ) // If I really need the oil, raise the PricePoint
	{
		float fPricePointIncrease01 = ( m_fOilNeed-0.65f )/0.35f; // increase PricePoint with AI need
		fPricePointIncrease01 *= ( 1.0f - fFriendlinessWithOtherPlayer ); // unless we're friends
		fOilPricePoint += fPricePointIncrease01 * fGapBetweenCenterPriceAndUpperPrice;
	}
	float fIronPricePoint = fCenterPrice;
	float fFoodPricePoint = fCenterPrice;

	// m_fBuyOilMult = the value of oil according to the AIPlayer.  If the AI has too 
	//   much oil, the number goes down.  If the AI wants more oil, the number goes up.
	//   This number is the amount of money the AI is willing to pay for 1 oil.
	m_fBuyOil  = fOilPricePoint - fPriceGap;
	m_fBuyIron = fIronPricePoint - fPriceGap;
	m_fBuyFood = fFoodPricePoint - fPriceGap;
	//
	// m_fSellOilMult = the value of oil according to the AIPlayer.  If the AI has too 
	//   much oil, the number goes down.  If the AI wants more oil, the number goes up.
	m_fSellOil  = fOilPricePoint + fPriceGap;
	m_fSellIron = fIronPricePoint + fPriceGap;
	m_fSellFood = fFoodPricePoint + fPriceGap;

	// If the AIPlayer needs a resource, put the sell price up to 3.0 (don't sell it)
	if( m_fOilNeed  > 0.5f ){ m_fSellOil  = fOpenMarketCostToBuyResource; }
	if( m_fIronNeed > 0.5f ){ m_fSellIron = fOpenMarketCostToBuyResource; }
	if( m_fFoodNeed > 0.5f ){ m_fSellFood = fOpenMarketCostToBuyResource; }

	// If the AIPlayer has excess resource, it is willing to sell for 1.5
	if( m_fOilNeed  < 0.3f ){ m_fBuyOil  = fOpenMarketMoneyForSellingResource + 0.5f; }
	if( m_fIronNeed < 0.3f ){ m_fBuyIron = fOpenMarketMoneyForSellingResource + 0.5f; }
	if( m_fFoodNeed < 0.3f ){ m_fBuyFood = fOpenMarketMoneyForSellingResource + 0.5f; }

	// Prices are limited by OPENMARKET_COST_TO_BUY_RESOURCE, OPENMARKET_MONEY_FOR_SELLING_RESOURCE
	m_fSellOil  = min( m_fSellOil,  fOpenMarketCostToBuyResource );
	m_fSellIron = min( m_fSellIron, fOpenMarketCostToBuyResource );
	m_fSellFood = min( m_fSellFood, fOpenMarketCostToBuyResource );
	m_fSellOil  = max( m_fSellOil,  fOpenMarketMoneyForSellingResource );
	m_fSellIron = max( m_fSellIron, fOpenMarketMoneyForSellingResource );
	m_fSellFood = max( m_fSellFood, fOpenMarketMoneyForSellingResource );

	m_fBuyOil   = min( m_fBuyOil,  fOpenMarketCostToBuyResource );
	m_fBuyIron  = min( m_fBuyIron, fOpenMarketCostToBuyResource );
	m_fBuyFood  = min( m_fBuyFood, fOpenMarketCostToBuyResource );
	m_fBuyOil   = max( m_fBuyOil,  fOpenMarketMoneyForSellingResource );
	m_fBuyIron  = max( m_fBuyIron, fOpenMarketMoneyForSellingResource );
	m_fBuyFood  = max( m_fBuyFood, fOpenMarketMoneyForSellingResource );

	/*
	Buy/Sell price depends on:
		How much the AI likes the other player (enemies should never get a good deal)
		When selling,
			low price if the AI produces much more oil than it needs
			don't sell all the oil, just the excess that would go to OpenMarket
		When buying,
			low price if the AI doesn't need it badly, high price if the AI needs it badly
			don't overbuy the quantity of oil

	if the AIPlayer doesn't like the other player,
		When buying, he should offer as little money as possible - $1.1
			(the oil helps the AIPlayer, but money helps his enemy, too)
		When selling, he should require as much money as possible - $2.9
			(the AI wants to give the other player as bad of a deal as possible)
	if the AIPlayer is neutral towards the other player, he should request slightly higher than fair
		When buying, he should offer $1.9 per oil
		When selling, he should request $2.2
	if the AIPlayer likes the other player (e.g. a teammate)
		When buying, he should offer a decent deal
			(about $2.0 per oil if the AI needs it - which helps both players equally)
		When selling, he should offer good prices
			(about $2 per oil if the AI has an excess)

	if the AIPlayer needs oil, offer higher buying/selling prices (don't sell)
	if the other player needs oil, offer higher buying/selling prices (don't buy)

	if the AI needs oil, the oil "buy" price should go up towards 3.0 (or slightly higher)
	if the AI needs oil, the oil "sell" price should go up to 3.0 (or slightly higher)
	*/
}

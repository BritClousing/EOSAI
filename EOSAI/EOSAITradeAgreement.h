
#pragma once

#include "EOSAIEnumTradeAgreementResponse.h"
#include "EOSAIEnumTradeAgreementState.h"
#include "EOSAIEnumPoiObjectType.h"
#include "PoiObjectIdAndName.h"
#include "EOSAILongSet.h"
#include "AIPlayer.h"
namespace EOSAI{ class StringAndFloatSet; }
//class CEOSAIPoiObjectIdAndName;
class CEOSAISerial;
/*
class CEOSAIPoiObjectIdAndName
{
	public:
		CEOSAIPoiObjectIdAndName()
		{
			m_iOwner = 0;
			m_ePoiObjectType = EnumAIPoiObjectType::enum_Undefined;
			m_iObjectId = 0;
			m_iProduction = 0;
			m_bLandResource = true;
		}

		CEOSAIPoiObjectIdAndName* Duplicate()
		{
			CEOSAIPoiObjectIdAndName* pNew = new CEOSAIPoiObjectIdAndName();
			*pNew = *this;
			/-*
			pNew->m_iOwner = m_iOwner;
			pNew->m_ePoiObjectType = m_ePoiObjectType;
			pNew->m_GameTime = m_GameTime;
			pNew->m_iObjectId = m_iObjectId;
			pNew->m_strName = m_strName;
			pNew->m_iProduction = m_iProduction;
			pNew->m_strType = m_strType;
			*-/
			return pNew;
		}
		void operator=( CEOSAIPoiObjectIdAndName& other )
		{
			m_iOwner = other.m_iOwner;
			m_ePoiObjectType = other.m_ePoiObjectType;
			m_GameTime = other.m_GameTime;
			m_iObjectId = other.m_iObjectId;
			m_strName = other.m_strName;
			m_iProduction = other.m_iProduction;
			m_bLandResource = other.m_bLandResource;
			m_strType = other.m_strType;
		}

		void Serialize( CEOSAISerial* pSerial );
		void Deserialize( CEOSAISerial* pSerial );

		long    m_iOwner;
		EnumAIPoiObjectType  m_ePoiObjectType;
		//
		float   m_GameTime; // used only when it's stored in the WorldDesc
		//
		long    m_iObjectId;
		CString m_strName;
		long    m_iProduction; // Used by cities and resources
		bool    m_bLandResource;
		CString m_strType;     // UnitTemplate or ResourceType
};
*/
class DLLIMPEXP CEOSAITradeAgreement
{
	public:
		CEOSAITradeAgreement();

		//void InitializeWithPlayerNumbers( long iMyPlayerNumber, long iOtherPlayerNumber, long iTradeAgreementId );
		void InitializeWithPlayerNumbers( long iMyPlayerNumber, long iOtherPlayerNumber, CString strTradeAgreementId );

		void Serialize( CEOSAISerial* pSerial );
		void Deserialize( CEOSAISerial* pSerial );
		//
		void Copy( CEOSAITradeAgreement* pOther );
		//
		void SetOfferedOnTurn( long iTurn ){ m_iOfferedOnTurn = iTurn; }
		long GetOfferedOnTurn(){ return m_iOfferedOnTurn; }
		//
		EOSAIEnumTradeAgreementResponse  GetPlayerResponse( long iPlayerNumber );
		EOSAIEnumTradeAgreementResponse  GetOtherPlayerResponse( long iPlayerNumber );
		void SetResponse( long iPlayerNumber, EOSAIEnumTradeAgreementResponse response );
		bool SetPlayerResponseAndChangeState( long iPlayerNumber, EOSAIEnumTradeAgreementResponse response );
		//void SetState( EOSAIEnumTradeAgreementState State ){ m_TradeAgreementState = State; }
		//void UpdateUI( CWorldDesc* pWorldDesc, EOSAIEnumTradeAgreementState eNewState );
		//
		//float CalculateTradeValue( long iPlayerPerspective, CTradeAgreementMultiplier* pMultiplier ); // -1.0 to +1.0
		//
		bool IsModifiable(){ return m_TradeAgreementState == EOSAIEnumTradeAgreementState_BrandNew; }
		//bool CanBeAccepted(){ return m_TradeAgreementState == EOSAIEnumTradeAgreementState_IncomingOffer; }
		//
		//CString GetDescription( long iMaximumPixelWidth );
		//
		bool PlayersInvolved( long iPlayer1, long iPlayer2 ){ return ( m_iPlayer[0]==iPlayer1 && m_iPlayer[1]==iPlayer2 ) || ( m_iPlayer[0]==iPlayer2 && m_iPlayer[1]==iPlayer1 ); }
		long GetOtherPlayerNumber( long iMyPlayerNumber );
		long GetOtherPlayerColumn( long iMyPlayerNumber );
		long GetMyPlayerColumn( long iMyPlayerNumber );
		void SetPlayerWhoMadeTheOffer( long i ){ m_iPlayerWhoMadeTheOffer = i; }
		long GetPlayerWhoMadeTheOffer(){ return m_iPlayer[0]; }
		//
		bool IsEmpty();  // If there is no trade
		//
		bool IsAnOngoingTradeAgreement_AccordingToResourceTrade();
		bool IsAnOngoingTradeAgreement();
		//void AddOngoingMoneyAndResourceDeltas( long iPlayer, float* pfDeltaMoney, float* pfDeltaIron, float* pfDeltaOil, float* pfDeltaFood );
		void AddOngoingMoneyAndResourceDeltas( long iPlayer, EOSAI::StringAndFloatSet* pResourceTrade );
		//
		void ClearNonOpenMarketItems();
		//
		void AutoEqualize();  // Used when resources sell for a predetermined price
		void Clear();

		bool IsEqualized();
		//bool IsLimitedToPlayerState( CWorldDesc* pWorldDesc, bool bLumpSumAndOngoingTrade ); // do the players have enough money, technology, etc to make this trade
		//bool IsLimitedToPlayerState( CWorldDesc* pWorldDesc, bool bCheckLumpSumTrade, bool bCheckOngoingTrade, float fTimeDelta );
		bool DoesTheAIHaveEnoughResourcesToCompleteTheTrade( EOSAI::AIPlayer* pAIPlayer, bool bCheckLumpSumTrade, bool bCheckOngoingTrade, float fTimeDelta );
		//
		bool IsTheSame( CEOSAITradeAgreement* pTradeAgreement );
		//
		//
		//
		//long m_iTradeAgreementId;
		CString  m_strTradeAgreementId; // used by the server to uniquely identify trade offers
		//
		EOSAIEnumTradeAgreementState  m_TradeAgreementState;
		//
		long m_iOfferedOnTurn;
		bool m_bThisTradeOfferMustBeExplicitlyExpired;
		bool m_bAllowPlayerToCounteroffer;
		//
		long m_iPlayer[2];  // Player Number

		long m_iAICode; // Used by the AI

		long m_iPlayerMoneyLumpSum[2];
		long m_iPlayerMoneyPerTurn[2];
		long m_iPlayerIronLumpSum[2];
		long m_iPlayerIronPerTurn[2];
		long m_iPlayerOilLumpSum[2];
		long m_iPlayerOilPerTurn[2];
		long m_iPlayerFoodLumpSum[2];
		long m_iPlayerFoodPerTurn[2];

		CList< EOSAI::PoiObjectIdAndName* >  m_PlayerUnitObjectId[2];
		CList< EOSAI::PoiObjectIdAndName* >  m_PlayerCitiesObjectId[2];
		CList< EOSAI::PoiObjectIdAndName* >  m_PlayerResourcesObjectId[2];

		bool m_bPlayerMapPeek[2];
		bool m_bPlayerMapViewOngoing[2];

		CList< CString > m_PlayerTechnologyList[2];

		//
		//bool m_bAllowCounterOffers;
		// If true, the options are "Accept Offer", "Decline Offer", "Create Counteroffer"
		// If false, the options are "Sounds good", "Not right now", "No way"
		//bool m_bAllowUserToAcceptRejectOrModifyTradeAgreement;

		// Happens to both players
		bool m_bPeaceTreaty;
		bool m_bNonAggressionPact;
		long m_iNonAggressionPactEndTurn;

		bool m_bAlliance;
		//bool m_bPermanentAlliance;
		//long m_iAgreementToAttackPlayer; // 0 (if invalid) or a player number
		CEOSAILongSet  m_AgreementToAttackPlayer[2]; // A set of attack targets for each player

		//
		long m_iPlayerWhoMadeTheOffer;
		EOSAIEnumTradeAgreementResponse  m_ePlayerResponse[2];

		//
		/*
		bool m_bTradeOfferHasBeenCancelled;
		bool m_bOngoingTradeIsActive;
		bool m_bTradeIsCompleted;
		bool m_bTradeHasExpired;
		*/
};

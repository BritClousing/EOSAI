
#include "stdafx.h"
#include "EOSAITradeAgreement.h"
#include "EOSAISerial.h"
#include "EOSAISettings.h"
#include "EOSAIEnumPoiObjectType.h"
#include "AIPlayer.h"
#include "EOSAIGamePlayer.h"
#include "EOSAICommonData.h"
#include "EOSAINationalSummary3.h"
#include <math.h>

//#define RESOURCE_TO_MONEY_CONVERSION 3
//#define OPENMARKET_COST_TO_BUY_RESOURCE 3
//#define OPENMARKET_MONEY_FOR_SELLING_RESOURCE 1
/*
void CEOSAIPoiObjectIdAndName::Serialize( CEOSAISerial* pSerial )
{
	long iVersion = 1000;
	pSerial->Serialize( iVersion );
	pSerial->Serialize( m_iOwner );

	long iPoiObjectType = (long) m_ePoiObjectType.m_v;
	pSerial->Serialize( iPoiObjectType );

	//m_GameTime.Serialize( pSerial );
	pSerial->Serialize( m_GameTime );

	pSerial->Serialize( m_iObjectId );
	pSerial->SerializeANSI8( m_strName );
	pSerial->Serialize( m_iProduction );
	pSerial->Serialize( m_bLandResource );
	pSerial->SerializeANSI8( m_strType );
/-*
	Old data
	pSerial->Serialize( m_iOwner );

	long iPoiObjectType = (long) m_ePoiObjectType.m_v;
	pSerial->Serialize( iPoiObjectType );

	m_GameTime.Serialize( pSerial );

	pSerial->Serialize( m_iObjectId );
	pSerial->SerializeANSI8( m_strName );
	pSerial->Serialize( m_iProduction );
	pSerial->SerializeANSI8( m_strType );
*-/
}

void CEOSAIPoiObjectIdAndName::Deserialize( CEOSAISerial* pSerial )
{
	long iVersion = 0;
	pSerial->Deserialize( iVersion );
	if( iVersion >= 1000 )
	{
		pSerial->Deserialize( m_iOwner );

		long iPoiObjectType = 0;
		pSerial->Deserialize( iPoiObjectType );
		m_ePoiObjectType.m_v = (EnumAIPoiObjectType::Value) iPoiObjectType;

		//m_GameTime.Deserialize( pSerial );
		pSerial->Deserialize( m_GameTime );

		pSerial->Deserialize( m_iObjectId );
		pSerial->DeserializeANSI8( m_strName );
		pSerial->Deserialize( m_iProduction );
		pSerial->Deserialize( m_bLandResource );
		pSerial->DeserializeANSI8( m_strType );
	}
	else
	{
		// Old Version
		m_iOwner = iVersion;

		long iPoiObjectType = 0;
		pSerial->Deserialize( iPoiObjectType );
		m_ePoiObjectType.m_v = (EnumAIPoiObjectType::Value) iPoiObjectType;

		//m_GameTime.Deserialize( pSerial );
		pSerial->Deserialize( m_GameTime );

		pSerial->Deserialize( m_iObjectId );
		pSerial->DeserializeANSI8( m_strName );
		pSerial->Deserialize( m_iProduction );
		pSerial->DeserializeANSI8( m_strType );
	}
}
*/
//

CEOSAITradeAgreement::CEOSAITradeAgreement()
{
	Clear();
	m_iOfferedOnTurn = 0;

	m_TradeAgreementState = EOSAIEnumTradeAgreementState_BrandNew;
	//
	m_iPlayerWhoMadeTheOffer = 0;
	m_ePlayerResponse[0] = EOSAIEnumTradeAgreementResponse_NoResponse;
	m_ePlayerResponse[1] = EOSAIEnumTradeAgreementResponse_NoResponse;

	m_bAllowPlayerToCounteroffer = true;
	m_iAICode = 0;
	m_bThisTradeOfferMustBeExplicitlyExpired = false;
	//m_bAllowCounterOffers = true;

	//m_bTradeOfferHasBeenCancelled = false;
	//m_bOngoingTradeIsActive = false;
	//m_bTradeIsCompleted = false;
	//m_bTradeHasExpired = false;
}

//void CTradeAgreement::InitializeWithPlayerNumbers( long iMyPlayerNumber, long iOtherPlayerNumber, long iTradeAgreementId )
void CEOSAITradeAgreement::InitializeWithPlayerNumbers( long iMyPlayerNumber, long iOtherPlayerNumber, CString strTradeAgreementId )
{
	m_iPlayer[0] = iMyPlayerNumber;
	m_iPlayer[1] = iOtherPlayerNumber;
	m_iPlayerWhoMadeTheOffer = iMyPlayerNumber;

	//m_iTradeAgreementId = iTradeAgreementId;
	m_strTradeAgreementId = m_strTradeAgreementId;
	m_ePlayerResponse[0] = EOSAIEnumTradeAgreementResponse_Accept;
}

void CEOSAITradeAgreement::Serialize( CEOSAISerial* pSerial )
{
	// First, the class name
	//pSerial->Serialize( CTradeAgreement_string );

	// A version number
	pSerial->Serialize( (long) 2 );

	//pSerial->Serialize( m_iTradeAgreementId );
	pSerial->SerializeANSI8( m_strTradeAgreementId );

	long iEnum = m_TradeAgreementState;
	pSerial->Serialize( iEnum );

	pSerial->Serialize( m_iOfferedOnTurn );
	pSerial->Serialize( m_bThisTradeOfferMustBeExplicitlyExpired );

	pSerial->Serialize( m_iPlayer[0] );
	pSerial->Serialize( m_iPlayer[1] );
	pSerial->Serialize( m_iAICode );
	//pSerial->Serialize( m_strText );
	pSerial->Serialize( m_iPlayerMoneyLumpSum[0] );
	pSerial->Serialize( m_iPlayerMoneyLumpSum[1] );
	pSerial->Serialize( m_iPlayerMoneyPerTurn[0] );
	pSerial->Serialize( m_iPlayerMoneyPerTurn[1] );
	pSerial->Serialize( m_iPlayerIronLumpSum[0] );
	pSerial->Serialize( m_iPlayerIronLumpSum[1] );
	pSerial->Serialize( m_iPlayerIronPerTurn[0] );
	pSerial->Serialize( m_iPlayerIronPerTurn[1] );
	pSerial->Serialize( m_iPlayerOilLumpSum[0] );
	pSerial->Serialize( m_iPlayerOilLumpSum[1] );
	pSerial->Serialize( m_iPlayerOilPerTurn[0] );
	pSerial->Serialize( m_iPlayerOilPerTurn[1] );
	pSerial->Serialize( m_iPlayerFoodLumpSum[0] );
	pSerial->Serialize( m_iPlayerFoodLumpSum[1] );
	pSerial->Serialize( m_iPlayerFoodPerTurn[0] );
	pSerial->Serialize( m_iPlayerFoodPerTurn[1] );

	// Units
	for( long i=0; i<2; i++ )
	{
		long iCount;
		iCount = m_PlayerUnitObjectId[i].GetCount();
		pSerial->Serialize( iCount );
		POSITION pos = m_PlayerUnitObjectId[i].GetHeadPosition();
		while( pos )
		{
			EOSAI::PoiObjectIdAndName* pPoiObjectIdAndName = m_PlayerUnitObjectId[i].GetNext( pos );
			pPoiObjectIdAndName->Serialize( pSerial );
			//long id = m_PlayerUnitObjectId[i].GetNext( pos );
			//pSerial->Serialize( id );
		}
	}

	// Cities
	for( long i=0; i<2; i++ )
	{
		long iCount = m_PlayerCitiesObjectId[i].GetCount();
		pSerial->Serialize( iCount );
		POSITION pos = m_PlayerCitiesObjectId[i].GetHeadPosition();
		while( pos )
		{
			EOSAI::PoiObjectIdAndName* pPoiObjectIdAndName = m_PlayerCitiesObjectId[i].GetNext( pos );
			pPoiObjectIdAndName->Serialize( pSerial );
			//long id = m_PlayerCitiesObjectId[0].GetNext( pos );
			//pSerial->Serialize( id );
		}
	}

	// Resources
	for( long i=0; i<2; i++ )
	{
		long iCount = m_PlayerResourcesObjectId[i].GetCount();
		pSerial->Serialize( iCount );
		POSITION pos = m_PlayerResourcesObjectId[i].GetHeadPosition();
		while( pos )
		{
			EOSAI::PoiObjectIdAndName* pPoiObjectIdAndName = m_PlayerResourcesObjectId[i].GetNext( pos );
			pPoiObjectIdAndName->Serialize( pSerial );
			//long id = m_PlayerResourcesObjectId[i].GetNext( pos );
			//pSerial->Serialize( id );
		}
	}

	// Technology
	for( long i=0; i<2; i++ )
	{
		long iCount = m_PlayerTechnologyList[i].GetCount();
		pSerial->Serialize( iCount );
		POSITION pos = m_PlayerTechnologyList[i].GetHeadPosition();
		while( pos )
		{
			CString strName = m_PlayerTechnologyList[i].GetNext( pos );
			pSerial->SerializeANSI8( strName );
		}
	}

	pSerial->Serialize( m_bPlayerMapPeek[0] );
	pSerial->Serialize( m_bPlayerMapPeek[1] );
	pSerial->Serialize( m_bPlayerMapViewOngoing[0] );
	pSerial->Serialize( m_bPlayerMapViewOngoing[1] );

	pSerial->Serialize( m_bAllowPlayerToCounteroffer );

	pSerial->Serialize( m_bPeaceTreaty );
	pSerial->Serialize( m_bNonAggressionPact );
	pSerial->Serialize( m_iNonAggressionPactEndTurn ); // Version 2
	pSerial->Serialize( m_bAlliance );
	//pSerial->Serialize( m_bPermanentAlliance );
	m_AgreementToAttackPlayer[0].Serialize( pSerial );
	m_AgreementToAttackPlayer[1].Serialize( pSerial );

	pSerial->Serialize( m_iPlayerWhoMadeTheOffer );

	long iResponse;
	//iResponse = (long) m_eResponse;
	//pSerial->Serialize( iResponse );
	iResponse = (long) m_ePlayerResponse[0];
	pSerial->Serialize( iResponse );
	iResponse = (long) m_ePlayerResponse[1];
	pSerial->Serialize( iResponse );

	//pSerial->Serialize( m_bTradeOfferHasBeenCancelled );
	//pSerial->Serialize( m_bOngoingTradeIsActive );
	//pSerial->Serialize( m_bTradeIsCompleted );
	//pSerial->Serialize( m_bTradeHasExpired );
}

void CEOSAITradeAgreement::Deserialize( CEOSAISerial* pSerial )
{
	// First, the class name
	//CStringANSI8 strClassName = pSerial->ExtractStringANSI8();
	//ASSERT( strClassName == CTradeAgreement_string );

	// A version number
	long iVersion = pSerial->ExtractLong();

	//pSerial->Deserialize( m_iTradeAgreementId );
	pSerial->DeserializeANSI8( m_strTradeAgreementId );

	long iEnum = 0;
	pSerial->Deserialize( iEnum );
	m_TradeAgreementState = (EOSAIEnumTradeAgreementState) iEnum;

	pSerial->Deserialize( m_iOfferedOnTurn );
	pSerial->Deserialize( m_bThisTradeOfferMustBeExplicitlyExpired );

	pSerial->Deserialize( m_iPlayer[0] );
	pSerial->Deserialize( m_iPlayer[1] );
	pSerial->Deserialize( m_iAICode );
	//pSerial->Deserialize( m_strText );
	pSerial->Deserialize( m_iPlayerMoneyLumpSum[0] );
	pSerial->Deserialize( m_iPlayerMoneyLumpSum[1] );
	pSerial->Deserialize( m_iPlayerMoneyPerTurn[0] );
	pSerial->Deserialize( m_iPlayerMoneyPerTurn[1] );
	pSerial->Deserialize( m_iPlayerIronLumpSum[0] );
	pSerial->Deserialize( m_iPlayerIronLumpSum[1] );
	pSerial->Deserialize( m_iPlayerIronPerTurn[0] );
	pSerial->Deserialize( m_iPlayerIronPerTurn[1] );
	pSerial->Deserialize( m_iPlayerOilLumpSum[0] );
	pSerial->Deserialize( m_iPlayerOilLumpSum[1] );
	pSerial->Deserialize( m_iPlayerOilPerTurn[0] );
	pSerial->Deserialize( m_iPlayerOilPerTurn[1] );
	pSerial->Deserialize( m_iPlayerFoodLumpSum[0] );
	pSerial->Deserialize( m_iPlayerFoodLumpSum[1] );
	pSerial->Deserialize( m_iPlayerFoodPerTurn[0] );
	pSerial->Deserialize( m_iPlayerFoodPerTurn[1] );

	// Units
	for( long j=0; j<2; j++ )
	{
		long iCount;
		pSerial->Deserialize( iCount );
		for( long i=0; i<iCount; i++ )
		{
			//long id = 0;
			//pSerial->Deserialize( id );
			//m_PlayerUnitObjectId[j].AddTail( id );
			EOSAI::PoiObjectIdAndName* pPoiObjectIdAndName = new EOSAI::PoiObjectIdAndName();
			pPoiObjectIdAndName->Deserialize( pSerial );
			m_PlayerUnitObjectId[j].AddTail( pPoiObjectIdAndName );
		}
	}

	// Cities
	for( long j=0; j<2; j++ )
	{
		long iCount;
		pSerial->Deserialize( iCount );
		for( long i=0; i<iCount; i++ )
		{
			//long id = 0;
			//pSerial->Deserialize( id );
			//m_PlayerCitiesObjectId[0].AddTail( id );
			EOSAI::PoiObjectIdAndName* pPoiObjectIdAndName = new EOSAI::PoiObjectIdAndName();
			pPoiObjectIdAndName->Deserialize( pSerial );
			m_PlayerCitiesObjectId[j].AddTail( pPoiObjectIdAndName );
		}
	}

	// Resources
	for( long j=0; j<2; j++ )
	{
		long iCount;
		pSerial->Deserialize( iCount );
		for( long i=0; i<iCount; i++ )
		{
			//long id = 0;
			//pSerial->Deserialize( id );
			//m_PlayerResourcesObjectId[0].AddTail( id );
			EOSAI::PoiObjectIdAndName* pPoiObjectIdAndName = new EOSAI::PoiObjectIdAndName();
			pPoiObjectIdAndName->Deserialize( pSerial );
			m_PlayerResourcesObjectId[j].AddTail( pPoiObjectIdAndName );
		}
	}

	// Technology
	for( long j=0; j<2; j++ )
	{
		long iCount;
		pSerial->Deserialize( iCount );
		for( long i=0; i<iCount; i++ )
		{
			CString strName;
			pSerial->DeserializeANSI8( strName );
			m_PlayerTechnologyList[j].AddTail( strName );
		}
	}

	pSerial->Deserialize( m_bPlayerMapPeek[0] );
	pSerial->Deserialize( m_bPlayerMapPeek[1] );
	pSerial->Deserialize( m_bPlayerMapViewOngoing[0] );
	pSerial->Deserialize( m_bPlayerMapViewOngoing[1] );

	pSerial->Deserialize( m_bAllowPlayerToCounteroffer );

	pSerial->Deserialize( m_bPeaceTreaty );
	pSerial->Deserialize( m_bNonAggressionPact );
	if( iVersion >= 2 )
	{
		pSerial->Deserialize( m_iNonAggressionPactEndTurn ); // Version 2
	}
	pSerial->Deserialize( m_bAlliance );
	//pSerial->Deserialize( m_bPermanentAlliance );
	m_AgreementToAttackPlayer[0].Deserialize( pSerial );
	m_AgreementToAttackPlayer[1].Deserialize( pSerial );

	pSerial->Deserialize( m_iPlayerWhoMadeTheOffer );

	long iResponse;
	//pSerial->Deserialize( iResponse );
	//m_eResponse = (CTradeAgreement::EOSAIEnumTradeAgreementResponse) iResponse;
	pSerial->Deserialize( iResponse );
	m_ePlayerResponse[0] = (EOSAIEnumTradeAgreementResponse) iResponse;
	pSerial->Deserialize( iResponse );
	m_ePlayerResponse[1] = (EOSAIEnumTradeAgreementResponse) iResponse;

	//pSerial->Deserialize( m_bTradeOfferHasBeenCancelled );
	//pSerial->Deserialize( m_bOngoingTradeIsActive );
	//pSerial->Deserialize( m_bTradeIsCompleted );
	//pSerial->Deserialize( m_bTradeHasExpired );
}

//

void CEOSAITradeAgreement::Copy( CEOSAITradeAgreement* pOther )
{
	//m_iTradeAgreementId = pOther->m_iTradeAgreementId;
	m_strTradeAgreementId = pOther->m_strTradeAgreementId;
	//
	m_TradeAgreementState = pOther->m_TradeAgreementState;
	//
	m_iOfferedOnTurn = pOther->m_iOfferedOnTurn;
	m_bThisTradeOfferMustBeExplicitlyExpired = pOther->m_bThisTradeOfferMustBeExplicitlyExpired;
	m_bAllowPlayerToCounteroffer = pOther->m_bAllowPlayerToCounteroffer;
	//
	m_iPlayer[0] = pOther->m_iPlayer[0];
	m_iPlayer[1] = pOther->m_iPlayer[1];
	//
	m_iAICode = pOther->m_iAICode;
	//m_strText = pOther->m_strText;
	//
	m_iPlayerMoneyLumpSum[0] = pOther->m_iPlayerMoneyLumpSum[0];
	m_iPlayerMoneyLumpSum[1] = pOther->m_iPlayerMoneyLumpSum[1];
	m_iPlayerMoneyPerTurn[0] = pOther->m_iPlayerMoneyPerTurn[0];
	m_iPlayerMoneyPerTurn[1] = pOther->m_iPlayerMoneyPerTurn[1];
	m_iPlayerIronLumpSum[0] = pOther->m_iPlayerIronLumpSum[0];
	m_iPlayerIronLumpSum[1] = pOther->m_iPlayerIronLumpSum[1];
	m_iPlayerIronPerTurn[0] = pOther->m_iPlayerIronPerTurn[0];
	m_iPlayerIronPerTurn[1] = pOther->m_iPlayerIronPerTurn[1];
	m_iPlayerOilLumpSum[0] = pOther->m_iPlayerOilLumpSum[0];
	m_iPlayerOilLumpSum[1] = pOther->m_iPlayerOilLumpSum[1];
	m_iPlayerOilPerTurn[0] = pOther->m_iPlayerOilPerTurn[0];
	m_iPlayerOilPerTurn[1] = pOther->m_iPlayerOilPerTurn[1];
	m_iPlayerFoodLumpSum[0] = pOther->m_iPlayerFoodLumpSum[0];
	m_iPlayerFoodLumpSum[1] = pOther->m_iPlayerFoodLumpSum[1];
	m_iPlayerFoodPerTurn[0] = pOther->m_iPlayerFoodPerTurn[0];
	m_iPlayerFoodPerTurn[1] = pOther->m_iPlayerFoodPerTurn[1];

	POSITION pos;
	while( m_PlayerUnitObjectId[0].IsEmpty() == FALSE ){ delete m_PlayerUnitObjectId[0].RemoveHead(); }
	pos = pOther->m_PlayerUnitObjectId[0].GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiObjectIdAndName* pOtherItem = pOther->m_PlayerUnitObjectId[0].GetNext( pos );
		m_PlayerUnitObjectId[0].AddTail( pOtherItem->Duplicate() );
	}
	while( m_PlayerUnitObjectId[1].IsEmpty() == FALSE ){ delete m_PlayerUnitObjectId[1].RemoveHead(); }
	pos = pOther->m_PlayerUnitObjectId[1].GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiObjectIdAndName* pOtherItem = pOther->m_PlayerUnitObjectId[1].GetNext( pos );
		m_PlayerUnitObjectId[1].AddTail( pOtherItem->Duplicate() );
	}

	while( m_PlayerCitiesObjectId[0].IsEmpty() == FALSE ){ delete m_PlayerCitiesObjectId[0].RemoveHead(); }
	pos = pOther->m_PlayerCitiesObjectId[0].GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiObjectIdAndName* pOtherItem = pOther->m_PlayerCitiesObjectId[0].GetNext( pos );
		m_PlayerCitiesObjectId[0].AddTail( pOtherItem->Duplicate() );
	}
	while( m_PlayerCitiesObjectId[1].IsEmpty() == FALSE ){ delete m_PlayerCitiesObjectId[1].RemoveHead(); }
	pos = pOther->m_PlayerCitiesObjectId[1].GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiObjectIdAndName* pOtherItem = pOther->m_PlayerCitiesObjectId[1].GetNext( pos );
		m_PlayerCitiesObjectId[1].AddTail( pOtherItem->Duplicate() );
	}

	while( m_PlayerResourcesObjectId[0].IsEmpty() == FALSE ){ delete m_PlayerResourcesObjectId[0].RemoveHead(); }
	pos = pOther->m_PlayerResourcesObjectId[0].GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiObjectIdAndName* pOtherItem = pOther->m_PlayerResourcesObjectId[0].GetNext( pos );
		m_PlayerResourcesObjectId[0].AddTail( pOtherItem->Duplicate() );
	}
	while( m_PlayerResourcesObjectId[1].IsEmpty() == FALSE ){ delete m_PlayerResourcesObjectId[1].RemoveHead(); }
	pos = pOther->m_PlayerResourcesObjectId[1].GetHeadPosition();
	while( pos )
	{
		EOSAI::PoiObjectIdAndName* pOtherItem = pOther->m_PlayerResourcesObjectId[1].GetNext( pos );
		m_PlayerResourcesObjectId[1].AddTail( pOtherItem->Duplicate() );
	}

	m_bPlayerMapPeek[0] = pOther->m_bPlayerMapPeek[0];
	m_bPlayerMapPeek[1] = pOther->m_bPlayerMapPeek[1];
	m_bPlayerMapViewOngoing[0] = pOther->m_bPlayerMapViewOngoing[0];
	m_bPlayerMapViewOngoing[1] = pOther->m_bPlayerMapViewOngoing[1];

	while( m_PlayerTechnologyList[0].IsEmpty() == FALSE ){ m_PlayerTechnologyList[0].RemoveHead(); }
	pos = pOther->m_PlayerTechnologyList[0].GetHeadPosition();
	while( pos )
	{
		CString strTech = pOther->m_PlayerTechnologyList[0].GetNext( pos );
		m_PlayerTechnologyList[0].AddTail( strTech );
	}
	while( m_PlayerTechnologyList[1].IsEmpty() == FALSE ){ m_PlayerTechnologyList[1].RemoveHead(); }
	pos = pOther->m_PlayerTechnologyList[1].GetHeadPosition();
	while( pos )
	{
		CString strTech = pOther->m_PlayerTechnologyList[1].GetNext( pos );
		m_PlayerTechnologyList[1].AddTail( strTech );
	}

	//
	//

	// Happens to both players
	m_bPeaceTreaty = pOther->m_bPeaceTreaty;
	m_bNonAggressionPact = pOther->m_bNonAggressionPact;
	m_iNonAggressionPactEndTurn = pOther->m_iNonAggressionPactEndTurn;
	m_bAlliance = pOther->m_bAlliance;
	//m_bPermanentAlliance = pOther->m_bPermanentAlliance;
	//m_iAgreementToAttackPlayer = pOther->m_iAgreementToAttackPlayer;
	m_AgreementToAttackPlayer[0] = pOther->m_AgreementToAttackPlayer[0];
	m_AgreementToAttackPlayer[1] = pOther->m_AgreementToAttackPlayer[1];

	m_iPlayerWhoMadeTheOffer = pOther->m_iPlayerWhoMadeTheOffer; // used by the server, overwritten by the server}
	m_ePlayerResponse[0] = pOther->m_ePlayerResponse[0];
	m_ePlayerResponse[1] = pOther->m_ePlayerResponse[1];
	//

	//m_bTradeOfferHasBeenCancelled = pOther->m_bTradeOfferHasBeenCancelled;
	//m_bOngoingTradeIsActive = pOther->m_bOngoingTradeIsActive;
	//m_bTradeIsCompleted = pOther->m_bTradeIsCompleted;
	//m_bTradeHasExpired = pOther->m_bTradeHasExpired;
}

EOSAIEnumTradeAgreementResponse CEOSAITradeAgreement::GetPlayerResponse( long iPlayerNumber )
{
	if( m_iPlayer[0] == iPlayerNumber ){ return m_ePlayerResponse[0]; }
	if( m_iPlayer[1] == iPlayerNumber ){ return m_ePlayerResponse[1]; }
	return EOSAIEnumTradeAgreementResponse_NoResponse;
}

EOSAIEnumTradeAgreementResponse CEOSAITradeAgreement::GetOtherPlayerResponse( long iPlayerNumber )
{
	if( m_iPlayer[0] == iPlayerNumber ){ return m_ePlayerResponse[1]; }
	if( m_iPlayer[1] == iPlayerNumber ){ return m_ePlayerResponse[0]; }
	return EOSAIEnumTradeAgreementResponse_NoResponse;
}

void CEOSAITradeAgreement::SetResponse( long iPlayerNumber, EOSAIEnumTradeAgreementResponse response )
{
	if( m_iPlayer[0] == iPlayerNumber )
	{
		m_ePlayerResponse[0] = response;
	}
	if( m_iPlayer[1] == iPlayerNumber )
	{
		m_ePlayerResponse[1] = response;
	}
}

bool CEOSAITradeAgreement::SetPlayerResponseAndChangeState( long iPlayerNumber, EOSAIEnumTradeAgreementResponse eNewResponse )
{
	bool bStateChanged = false;
	EOSAIEnumTradeAgreementState  eNextState = EOSAIEnumTradeAgreementState_Undefined;
	if( this->m_TradeAgreementState == EOSAIEnumTradeAgreementState_WaitingForApproval )
	{
		EOSAIEnumTradeAgreementResponse  eCurrentResponse = GetPlayerResponse( iPlayerNumber );
		EOSAIEnumTradeAgreementResponse  eCurrentOtherResponse = GetOtherPlayerResponse( iPlayerNumber );

		if( eCurrentResponse      == EOSAIEnumTradeAgreementResponse_NoResponse &&
			eNewResponse          == EOSAIEnumTradeAgreementResponse_Accept &&
			eCurrentOtherResponse == EOSAIEnumTradeAgreementResponse_Accept )
		{
			if( this->IsAnOngoingTradeAgreement() )
			{
				bStateChanged = true;
				eNextState = EOSAIEnumTradeAgreementState_ActiveOngoingAgreement;
			}
			else
			{
				bStateChanged = true;
				//SetResponse( iPlayerNumber, eNewResponse );
				eNextState = EOSAIEnumTradeAgreementState_CompletedAgreement;
			}
		}
		if( eCurrentResponse      == EOSAIEnumTradeAgreementResponse_NoResponse &&
		  ( eNewResponse          == EOSAIEnumTradeAgreementResponse_Decline || eNewResponse == EOSAIEnumTradeAgreementResponse_Reject ) &&
			eCurrentOtherResponse == EOSAIEnumTradeAgreementResponse_Accept )
		{
			bStateChanged = true;
			eNextState = EOSAIEnumTradeAgreementState_RejectedOffer;
		}
		if( eCurrentResponse      == EOSAIEnumTradeAgreementResponse_Accept &&
			eNewResponse          == EOSAIEnumTradeAgreementResponse_CancelOffer &&
			eCurrentOtherResponse == EOSAIEnumTradeAgreementResponse_NoResponse )
		{
			bStateChanged = true;
			eNextState = EOSAIEnumTradeAgreementState_CancelledOffer;
		}
	}
	if( this->m_TradeAgreementState == EOSAIEnumTradeAgreementState_ActiveOngoingAgreement )
	{
		if( eNewResponse == EOSAIEnumTradeAgreementResponse_EndOngoingTrade )
		{
			bStateChanged = true;
			eNextState = EOSAIEnumTradeAgreementState_EndedOngoingTrade;
		}
	}
	//
	if( bStateChanged )
	{
		SetResponse( iPlayerNumber, eNewResponse );
		m_TradeAgreementState = eNextState;
	}
	return bStateChanged;
}

/*
void CTradeAgreement::UpdateUI( CWorldDesc* pWorldDesc, EOSAIEnumTradeAgreementState eNewState )
{
	bool bTransitionIsOkay = true;
	/-*
	bool bTransitionIsOkay = false;
	if( m_TradeAgreementState == EOSAIEnumTradeAgreementState_WaitingForApproval )
	{
		if( eNewState == EOSAIEnumTradeAgreementState_ExpiredOffer ||
			eNewState == EOSAIEnumTradeAgreementState_CancelledOffer ||
			eNewState == EOSAIEnumTradeAgreementState_CompletedAgreement ||
			eNewState == EOSAIEnumTradeAgreementState_ActiveOngoingAgreement ||
			eNewState == EOSAIEnumTradeAgreementState_RejectedOffer )
		{
			bTransitionIsOkay = true;
		}
	}
	if( m_TradeAgreementState == EOSAIEnumTradeAgreementState_ActiveOngoingAgreement )
	{
		if( eNewState == EOSAIEnumTradeAgreementState_EndedOngoingTrade )
		{
			bTransitionIsOkay = true;
		}
	}
	*-/

	if( bTransitionIsOkay )
	{
		//EOSAIEnumTradeAgreementState eOldState = m_TradeAgreementState;
		//m_TradeAgreementState = eNewState;

		// Notify the AI of the change
		/-*
		CAIPlayer* pAIPlayer = GetCommonState()->GetAIPlayer( pWorldDesc->GetLocalPlayerNumber() );
		if( pAIPlayer )
		{
			pAIPlayer->Incoming_TradeAgreementStateChange( this, eOldState, eNewState );
		}
		*-/

		// Update the interface
		if( g_pGameAppState &&
			g_pGameAppState->GetWorldDescPlayer() == pWorldDesc )
		{
			if( g_pCommunicationDialog &&
				g_pCommunicationDialog->GetInternalMailWindow() &&
				g_pCommunicationDialog->GetInternalMailWindow()->m_ProposalWindow )
			{
				if( g_pCommunicationDialog->GetInternalMailWindow()->m_ProposalWindow.GetTradeAgreement() &&
					//g_pCommunicationDialog->GetInternalMailWindow()->m_ProposalWindow.GetTradeAgreement()->m_iTradeAgreementId == m_iTradeAgreementId )
					g_pCommunicationDialog->GetInternalMailWindow()->m_ProposalWindow.GetTradeAgreement()->m_strTradeAgreementId == m_strTradeAgreementId )
				{
					CTradeAgreement* pTradeAgreement = g_pCommunicationDialog->GetInternalMailWindow()->m_ProposalWindow.GetTradeAgreement();
					g_pCommunicationDialog->GetInternalMailWindow()->m_ProposalWindow.SetTradeAgreement( pTradeAgreement );
				}
			}
			if( g_pSendMailDialog &&
				g_pSendMailDialog->GetInternalMailWindow() &&
				g_pSendMailDialog->GetInternalMailWindow()->m_ProposalWindow )
			{
				if( g_pSendMailDialog->GetInternalMailWindow()->m_ProposalWindow.GetTradeAgreement() &&
					//g_pSendMailDialog->GetInternalMailWindow()->m_ProposalWindow.GetTradeAgreement()->m_iTradeAgreementId == m_iTradeAgreementId )
					g_pSendMailDialog->GetInternalMailWindow()->m_ProposalWindow.GetTradeAgreement()->m_strTradeAgreementId == m_strTradeAgreementId )
				{
					CTradeAgreement* pTradeAgreement = g_pSendMailDialog->GetInternalMailWindow()->m_ProposalWindow.GetTradeAgreement();
					g_pSendMailDialog->GetInternalMailWindow()->m_ProposalWindow.SetTradeAgreement( pTradeAgreement );
				}
			}
		}
	}
}
*/

//
/*
float CTradeAgreement::CalculateObjectiveTradeBalance( long iPlayerPerspective ) // -1.0 to +1.0
{
	ASSERT( m_iPlayer[0] == iPlayerPerspective ||
			m_iPlayer[1] == iPlayerPerspective );

	float fImmediatePlayerValue[2], f20TurnPlayerValue[2];
	fImmediatePlayerValue[0] = 0.0f;
	fImmediatePlayerValue[1] = 0.0f;
	//f20TurnPlayerValue[0] = 0.0f;
	//f20TurnPlayerValue[1] = 0.0f;
	for( long i=0; i<2; i++ )
	{
		fImmediatePlayerValue[i] += m_iPlayerMoneyLumpSum[i];
		fImmediatePlayerValue[i] += 2.0f*m_iPlayerIronLumpSum[i];
		fImmediatePlayerValue[i] += 2.0f*m_iPlayerOilLumpSum[i];
		fImmediatePlayerValue[i] += 2.0f*m_iPlayerFoodLumpSum[i];

		//fImmediatePlayerValue[i] += 2.0f*m_iPlayerFoodLumpSum[i];
	}
	if( fImmediatePlayerValue[0] + fImmediatePlayerValue[1] == 0.0f ) return 0.0f;

	float fRatio01 = 0.0f;
	if( m_iPlayer[0] == iPlayerPerspective )
	{
		fRatio01 = fImmediatePlayerValue[0] / ( fImmediatePlayerValue[0] + fImmediatePlayerValue[1] );
	}
	if( m_iPlayer[1] == iPlayerPerspective )
	{
		fRatio01 = fImmediatePlayerValue[1] / ( fImmediatePlayerValue[0] + fImmediatePlayerValue[1] );
	}
	return ( fRatio01 * 2.0 ) - 1.0f; // -1.0 to +1.0
}
*/
/*
float CTradeAgreement::CalculateTradeValue( long iPlayerPerspective, CTradeAgreementMultiplier* pMultiplier )
{
	ASSERT( m_iPlayer[0] == iPlayerPerspective || m_iPlayer[1] == iPlayerPerspective );
	bool bAutomaticallyRejectOffer = false;

	float fLumpSumValueToThisPlayer = 0.0f;
	float fLumpSumValueToOtherPlayer = 0.0f;
	float fOngoingValueToThisPlayer = 0.0f;
	float fOngoingValueToOtherPlayer = 0.0f;
	//float fImmediatePlayerValue[2];//, f20TurnPlayerValue[2];
	//fImmediatePlayerValue[0] = 0.0f;
	//fImmediatePlayerValue[1] = 0.0f;

	long iThisPlayerIndex = 0;
	long iOtherPlayerIndex = 0;
	if( m_iPlayer[0] == iPlayerPerspective ){ iThisPlayerIndex = 0; iOtherPlayerIndex = 1; }
	if( m_iPlayer[1] == iPlayerPerspective ){ iThisPlayerIndex = 1; iOtherPlayerIndex = 0; }
	if( iThisPlayerIndex == 0 && iOtherPlayerIndex == 0 ){ ASSERT( false ); return -1.0f; }

	CPlayer* pThisPlayer = GetCommonState()->GetPlayer( m_iPlayer[iThisPlayerIndex] );
	CPlayer* pOtherPlayer = GetCommonState()->GetPlayer( m_iPlayer[iOtherPlayerIndex] );

	/-*
	if( iThisPlayerIndex > 0 || iOtherPlayerIndex > 0 )
	{
		fValueToThisPlayer +=                         1.0f * m_iPlayerMoneyLumpSum[iOtherPlayerIndex];
		fValueToThisPlayer += pMultiplier->m_fBuyIronPrice * m_iPlayerIronLumpSum[iOtherPlayerIndex];
		fValueToThisPlayer += pMultiplier->m_fBuyOilPrice  * m_iPlayerOilLumpSum[iOtherPlayerIndex];
		fValueToThisPlayer += pMultiplier->m_fBuyFoodPrice * m_iPlayerFoodLumpSum[iOtherPlayerIndex];

		fValueToThisPlayer -=                          1.0f * m_iPlayerMoneyLumpSum[iThisPlayerIndex];
		fValueToThisPlayer -= pMultiplier->m_fSellIronPrice * m_iPlayerIronLumpSum[iThisPlayerIndex];
		fValueToThisPlayer -= pMultiplier->m_fSellOilPrice  * m_iPlayerOilLumpSum[iThisPlayerIndex];
		fValueToThisPlayer -= pMultiplier->m_fSellFoodPrice * m_iPlayerFoodLumpSum[iThisPlayerIndex];
	}
	*-/

	// Calculate the LumpSum value of the Money + Resources
	fLumpSumValueToThisPlayer  +=                         1.0f * m_iPlayerMoneyLumpSum[iOtherPlayerIndex];
	fLumpSumValueToThisPlayer  += pMultiplier->m_fBuyIronPrice * m_iPlayerIronLumpSum[iOtherPlayerIndex];
	fLumpSumValueToThisPlayer  += pMultiplier->m_fBuyOilPrice  * m_iPlayerOilLumpSum[iOtherPlayerIndex];
	fLumpSumValueToThisPlayer  += pMultiplier->m_fBuyFoodPrice * m_iPlayerFoodLumpSum[iOtherPlayerIndex];
	fLumpSumValueToOtherPlayer +=                          1.0f * m_iPlayerMoneyLumpSum[iThisPlayerIndex];
	fLumpSumValueToOtherPlayer += pMultiplier->m_fSellIronPrice * m_iPlayerIronLumpSum[iThisPlayerIndex];
	fLumpSumValueToOtherPlayer += pMultiplier->m_fSellOilPrice  * m_iPlayerOilLumpSum[iThisPlayerIndex];
	fLumpSumValueToOtherPlayer += pMultiplier->m_fSellFoodPrice * m_iPlayerFoodLumpSum[iThisPlayerIndex];

	// Calculate the Ongoing value of the Money + Resources
	fOngoingValueToThisPlayer  +=                         1.0f * m_iPlayerMoneyPerTurn[iOtherPlayerIndex];
	fOngoingValueToThisPlayer  += pMultiplier->m_fBuyIronPrice * m_iPlayerIronPerTurn[iOtherPlayerIndex];
	fOngoingValueToThisPlayer  += pMultiplier->m_fBuyOilPrice  * m_iPlayerOilPerTurn[iOtherPlayerIndex];
	fOngoingValueToThisPlayer  += pMultiplier->m_fBuyFoodPrice * m_iPlayerFoodPerTurn[iOtherPlayerIndex];
	fOngoingValueToOtherPlayer +=                          1.0f * m_iPlayerMoneyPerTurn[iThisPlayerIndex];
	fOngoingValueToOtherPlayer += pMultiplier->m_fSellIronPrice * m_iPlayerIronPerTurn[iThisPlayerIndex];
	fOngoingValueToOtherPlayer += pMultiplier->m_fSellOilPrice  * m_iPlayerOilPerTurn[iThisPlayerIndex];
	fOngoingValueToOtherPlayer += pMultiplier->m_fSellFoodPrice * m_iPlayerFoodPerTurn[iThisPlayerIndex];

	if( fOngoingValueToOtherPlayer > 0.0f ) bAutomaticallyRejectOffer = true;

	// Calculate the value of Units, Resources, and Cities
	if( m_PlayerUnitObjectId[iThisPlayerIndex].IsEmpty() == FALSE ) bAutomaticallyRejectOffer = true;
	if( m_PlayerCitiesObjectId[iThisPlayerIndex].IsEmpty() == FALSE ) bAutomaticallyRejectOffer = true;
	if( m_PlayerResourcesObjectId[iThisPlayerIndex].IsEmpty() == FALSE ) bAutomaticallyRejectOffer = true;
	// CList< CEOSAIPoiObjectIdAndName* >  m_PlayerUnitObjectId[2];
	// CList< CEOSAIPoiObjectIdAndName* >  m_PlayerCitiesObjectId[2];
	// CList< CEOSAIPoiObjectIdAndName* >  m_PlayerResourcesObjectId[2];

	// Calculate the value of Map-Views
	if( m_bPlayerMapPeek[iThisPlayerIndex] ) bAutomaticallyRejectOffer = true;
	if( m_bPlayerMapViewOngoing[iThisPlayerIndex] ) bAutomaticallyRejectOffer = true;
	// bool m_bPlayerMapPeek[2];
	// bool m_bPlayerMapViewOngoing[2];

	// Calculate the value of the technology
	CWorldDescPlayer* pWorldDescPlayer = GetCommonState()->GetWorldDescPlayerByPlayerNumber( iPlayerPerspective );//g_pGameAppState->GetWorldDescPlayer();//GetCommonState()->GetLo
	CPlayerTechnologySet* pPlayerTechnologySet = pWorldDescPlayer->GetPlayerTechnologySet();
	CUnitset* pActiveUnitset = GetCommonState()->GetActiveUnitset();
	CString strThisPlayerUnitsubset = pThisPlayer->GetUnitsubset();
	CString strOtherPlayerUnitsubset = pOtherPlayer->GetUnitsubset();
	for( long iPlayerIndex=0; iPlayerIndex<2; iPlayerIndex++ )
	{
		long iCurrentPlayer = m_iPlayer[iPlayerIndex];
		long iOtherPlayer = m_iPlayer[1-iPlayerIndex];
		POSITION pos = m_PlayerTechnologyList[iPlayerIndex].GetHeadPosition();
		while( pos )
		{
			CString strTechnology = m_PlayerTechnologyList[iPlayerIndex].GetNext( pos );

			float fResearchCost = 0.0f;
			if( iOtherPlayer == pWorldDescPlayer->GetLocalPlayerNumber() )
			{
				fResearchCost += pWorldDescPlayer->GetResearchPointsUntilCompletion( strTechnology );
			}
			else
			{
				CTechnologyDesc* pTechnologyDesc = pActiveUnitset->GetTechnologyDesc( strTechnology );
				CPlayerTechnologyObject* pPlayerTechnologyObject = pPlayerTechnologySet->GetPlayerTechnologyObject( iOtherPlayer );
				if( pPlayerTechnologyObject->TechnologyExists( strTechnology ) == false )
				{
					fResearchCost += pTechnologyDesc->GetCost();
				}
			}
			if( iCurrentPlayer == iPlayerPerspective ) fLumpSumValueToOtherPlayer += fResearchCost;
			if( iCurrentPlayer != iPlayerPerspective ) fLumpSumValueToThisPlayer += fResearchCost;
		}
	}

	if( m_bPeaceTreaty )
	{
		bAutomaticallyRejectOffer = true;
	}
	if( m_bNonAggressionPact ) bAutomaticallyRejectOffer = true;
	if( m_bAlliance ) bAutomaticallyRejectOffer = true;
	if( m_AgreementToAttackPlayer[iThisPlayerIndex].GetSize() > 0 ) bAutomaticallyRejectOffer = true;

	if( bAutomaticallyRejectOffer ) return -1.0f;
	if( fLumpSumValueToThisPlayer + fLumpSumValueToOtherPlayer == 0.0f ) return -0.1f; // avoid divide by 0

	float fRatio01 = fLumpSumValueToThisPlayer / ( fLumpSumValueToThisPlayer + fLumpSumValueToOtherPlayer );
	return ( fRatio01 * 2.0 ) - 1.0f; // -1.0 to +1.0
	//return fValueToThisPlayer;
}
*/
/*
CString CEOSAITradeAgreement::GetDescription( long iMaximumPixelWidth )
{
	CString strText;
	CString strTemp;

	strTemp.Format( Lang( 1381, _T("Turn %d, ") ), m_iOfferedOnTurn );
	strText += strTemp;

	for( long i=0; i<2; i++ )
	{
		if( m_iPlayerMoneyLumpSum[i] != 0 )
		{
			strTemp.Format( Lang( 1382, _T("$%d, ") ), m_iPlayerMoneyLumpSum[i] );
			strText += strTemp;
		}
		if( m_iPlayerMoneyPerTurn[i] != 0 )
		{
			strTemp.Format( Lang( 1383, _T("$%d/turn, ") ), m_iPlayerMoneyPerTurn[i] );
			strText += strTemp;
		}
		if( m_iPlayerIronLumpSum[i] != 0 )
		{
			strTemp.Format( Lang( 1384, _T("%d Iron, ") ), m_iPlayerIronLumpSum[i] );
			strText += strTemp;
		}
		if( m_iPlayerIronPerTurn[i] != 0 )
		{
			strTemp.Format( Lang( 1385, _T("%d Iron/turn, ") ), m_iPlayerIronPerTurn[i] );
			strText += strTemp;
		}
		if( m_iPlayerOilLumpSum[i] != 0 )
		{
			strTemp.Format( Lang( 1386, _T("%d Oil, ") ), m_iPlayerOilLumpSum[i] );
			strText += strTemp;
		}
		if( m_iPlayerOilPerTurn[i] != 0 )
		{
			strTemp.Format( Lang( 1387, _T("%d Oil/turn, ") ), m_iPlayerOilPerTurn[i] );
			strText += strTemp;
		}
		if( m_iPlayerFoodLumpSum[i] != 0.0f )
		{
			strTemp.Format( Lang( 1388, _T("%d Food, ") ), m_iPlayerFoodLumpSum[i] );
			strText += strTemp;
		}
		if( m_iPlayerFoodPerTurn[i] != 0.0f )
		{
			strTemp.Format( Lang( 1389, _T("%d Food/turn, ") ), m_iPlayerFoodPerTurn[i] );
			strText += strTemp;
		}
	}
	// Remove the last two characters (to remove the comma and space)
	strText = strText.Left( strText.GetLength()-2 );
	//
	bool bAbbreviated = false;
	CDC* pDC = g_pAppDialog->GetDC();
	CFont* pOldFont = pDC->SelectObject( &g_pApp->m_DefaultFont );
	if( pDC )
	{
		CRect rect;
		pDC->DrawText( strText, &rect, DT_CALCRECT );
		while( rect.Width() > iMaximumPixelWidth )
		{
			strText = strText.Left( strText.GetLength()-3 );
			pDC->DrawText( strText, &rect, DT_CALCRECT );
			bAbbreviated = true;
		}
		if( bAbbreviated )
		{
			strText += Lang( 1344, _T("...") );
		}
	}
	pDC->SelectObject( pOldFont );
	g_pAppDialog->ReleaseDC( pDC );

	return strText;
}
*/
/*
void CTradeAgreement::AssignATradeId()
{
	m_iTradeAgreementId = iNextTradeId;
	iNextTradeId++;
}
*/
//

long CEOSAITradeAgreement::GetOtherPlayerNumber( long iMyPlayerNumber )
{
	if( m_iPlayer[0] != iMyPlayerNumber ) return m_iPlayer[0];
	return m_iPlayer[1];
}

long CEOSAITradeAgreement::GetOtherPlayerColumn( long iMyPlayerNumber )
{
	if( m_iPlayer[0] != iMyPlayerNumber ) return 0;
	return 1;
}

long CEOSAITradeAgreement::GetMyPlayerColumn( long iMyPlayerNumber )
{
	if( m_iPlayer[0] == iMyPlayerNumber ) return 0;
	return 1;
}

bool CEOSAITradeAgreement::IsEmpty()
{
	//if( m_iAICode != 0 ) return false;
	//if( m_strText == "" ) return false;

	if( m_iPlayerMoneyLumpSum[0] != 0 ) return false;
	if( m_iPlayerMoneyLumpSum[1] != 0 ) return false;
	if( m_iPlayerMoneyPerTurn[0] != 0 ) return false;
	if( m_iPlayerMoneyPerTurn[1] != 0 ) return false;
	if( m_iPlayerIronLumpSum[0] != 0 ) return false;
	if( m_iPlayerIronLumpSum[1] != 0 ) return false;
	if( m_iPlayerIronPerTurn[0] != 0 ) return false;
	if( m_iPlayerIronPerTurn[1] != 0 ) return false;
	if( m_iPlayerOilLumpSum[0] != 0 ) return false;
	if( m_iPlayerOilLumpSum[1] != 0 ) return false;
	if( m_iPlayerOilPerTurn[0] != 0 ) return false;
	if( m_iPlayerOilPerTurn[1] != 0 ) return false;
	if( m_iPlayerFoodLumpSum[0] != 0 ) return false;
	if( m_iPlayerFoodLumpSum[1] != 0 ) return false;
	if( m_iPlayerFoodPerTurn[0] != 0 ) return false;
	if( m_iPlayerFoodPerTurn[1] != 0 ) return false;

	if( m_iPlayer[0] != 0 )
	{
		if( m_PlayerUnitObjectId[0].IsEmpty() == FALSE ) return false;
		if( m_PlayerCitiesObjectId[0].IsEmpty() == FALSE ) return false;
		if( m_PlayerResourcesObjectId[0].IsEmpty() == FALSE ) return false;
		if( m_PlayerTechnologyList[0].IsEmpty() == FALSE ) return false;
		if( m_bPlayerMapPeek[0] != 0.0f ) return false;
		if( m_bPlayerMapViewOngoing[0] != 0.0f ) return false;
		if( m_AgreementToAttackPlayer[0].GetSize() > 0 ) return false;
	}
	if( m_iPlayer[1] != 0 )
	{
		if( m_PlayerUnitObjectId[1].IsEmpty() == FALSE ) return false;
		if( m_PlayerCitiesObjectId[1].IsEmpty() == FALSE ) return false;
		if( m_PlayerResourcesObjectId[1].IsEmpty() == FALSE ) return false;
		if( m_PlayerTechnologyList[1].IsEmpty() == FALSE ) return false;
		if( m_bPlayerMapPeek[1] != 0.0f ) return false;
		if( m_bPlayerMapViewOngoing[1] != 0.0f ) return false;
		if( m_AgreementToAttackPlayer[1].GetSize() > 0 ) return false;
	}

	if( m_iPlayer[0] != 0 && m_iPlayer[1] != 0 )
	{
		if( m_bPeaceTreaty ) return false;
		if( m_bNonAggressionPact ) return false;
		if( m_bAlliance ) return false;
		//if( m_bPermanentAlliance ) return false;
		//if( m_iAgreementToAttackPlayer ) return false;
	}

	return true;
}

//

bool CEOSAITradeAgreement::IsAnOngoingTradeAgreement_AccordingToResourceTrade()
{
	if( m_iPlayerMoneyPerTurn[0] != 0 ) return true;
	if( m_iPlayerMoneyPerTurn[1] != 0 ) return true;
	if( m_iPlayerIronPerTurn[0] != 0 ) return true;
	if( m_iPlayerIronPerTurn[1] != 0 ) return true;
	if( m_iPlayerOilPerTurn[0] != 0 ) return true;
	if( m_iPlayerOilPerTurn[1] != 0 ) return true;
	if( m_iPlayerFoodPerTurn[0] != 0 ) return true;
	if( m_iPlayerFoodPerTurn[1] != 0 ) return true;

	//if( m_bPeaceTreaty ) return true;
	//if( m_bNonAggressionPact ) return true;
	//if( m_bAlliance ) return true;

	return false;
}

bool CEOSAITradeAgreement::IsAnOngoingTradeAgreement()
{
	if( m_iPlayerMoneyPerTurn[0] != 0 ) return true;
	if( m_iPlayerMoneyPerTurn[1] != 0 ) return true;
	if( m_iPlayerIronPerTurn[0] != 0 ) return true;
	if( m_iPlayerIronPerTurn[1] != 0 ) return true;
	if( m_iPlayerOilPerTurn[0] != 0 ) return true;
	if( m_iPlayerOilPerTurn[1] != 0 ) return true;
	if( m_iPlayerFoodPerTurn[0] != 0 ) return true;
	if( m_iPlayerFoodPerTurn[1] != 0 ) return true;

	if( m_iPlayer[0] != 0 )
	{
		if( m_bPlayerMapViewOngoing[0] ) return true;
	}
	if( m_iPlayer[1] != 0 )
	{
		if( m_bPlayerMapViewOngoing[1] ) return true;
	}

	//if( m_bPeaceTreaty ) return true;
	if( m_bNonAggressionPact ) return true;
	if( m_bAlliance ) return true;

	return false;
}
/*
void CTradeAgreement::AddOngoingMoneyAndResourceDeltas( long iPlayer, 
			float* pfDeltaMoney, float* pfDeltaIron, float* pfDeltaOil, float* pfDeltaFood )
{
	if( m_TradeAgreementState != EOSAIEnumTradeAgreementState_ActiveOngoingAgreement ) return;
	if( m_iPlayer[0] == iPlayer || m_iPlayer[1] == iPlayer )
	{
		long iMult = +1;
		if( m_iPlayer[0] == iPlayer ){ iMult = -1; }
		(*pfDeltaMoney) += iMult * m_iPlayerMoneyPerTurn[0];
		(*pfDeltaIron)  += iMult * m_iPlayerIronPerTurn[0];
		(*pfDeltaOil)   += iMult * m_iPlayerOilPerTurn[0];
		(*pfDeltaFood)  += iMult * m_iPlayerFoodPerTurn[0];

		iMult = +1;
		if( m_iPlayer[1] == iPlayer ){ iMult = -1; }
		(*pfDeltaMoney) += iMult * m_iPlayerMoneyPerTurn[1];
		(*pfDeltaIron)  += iMult * m_iPlayerIronPerTurn[1];
		(*pfDeltaOil)   += iMult * m_iPlayerOilPerTurn[1];
		(*pfDeltaFood)  += iMult * m_iPlayerFoodPerTurn[1];
	}
}
*/
void CEOSAITradeAgreement::AddOngoingMoneyAndResourceDeltas( long iPlayer, EOSAI::StringAndFloatSet* pResourceTrade )
{
	if( m_TradeAgreementState != EOSAIEnumTradeAgreementState_ActiveOngoingAgreement ) return;
	if( m_iPlayer[0] == iPlayer || m_iPlayer[1] == iPlayer )
	{
		long iMult = +1;
		if( m_iPlayer[0] == iPlayer ){ iMult = -1; }
		pResourceTrade->Add( _T("Money"), (float)( iMult * m_iPlayerMoneyPerTurn[0] ) );
		pResourceTrade->Add( _T("Iron"),  (float)( iMult * m_iPlayerIronPerTurn[0] ) );
		pResourceTrade->Add( _T("Oil"),   (float)( iMult * m_iPlayerOilPerTurn[0] ) );
		pResourceTrade->Add( _T("Food"),  (float)( iMult * m_iPlayerFoodPerTurn[0] ) );
		//(*pfDeltaMoney) += iMult * m_iPlayerMoneyPerTurn[0];
		//(*pfDeltaIron)  += iMult * m_iPlayerIronPerTurn[0];
		//(*pfDeltaOil)   += iMult * m_iPlayerOilPerTurn[0];
		//(*pfDeltaFood)  += iMult * m_iPlayerFoodPerTurn[0];

		iMult = +1;
		if( m_iPlayer[1] == iPlayer ){ iMult = -1; }
		pResourceTrade->Add( _T("Money"), (float)( iMult * m_iPlayerMoneyPerTurn[1] ) );
		pResourceTrade->Add( _T("Iron"),  (float)( iMult * m_iPlayerIronPerTurn[1] ) );
		pResourceTrade->Add( _T("Oil"),   (float)( iMult * m_iPlayerOilPerTurn[1] ) );
		pResourceTrade->Add( _T("Food"),  (float)( iMult * m_iPlayerFoodPerTurn[1] ) );
		//(*pfDeltaMoney) += iMult * m_iPlayerMoneyPerTurn[1];
		//(*pfDeltaIron)  += iMult * m_iPlayerIronPerTurn[1];
		//(*pfDeltaOil)   += iMult * m_iPlayerOilPerTurn[1];
		//(*pfDeltaFood)  += iMult * m_iPlayerFoodPerTurn[1];
	}
}

//
void CEOSAITradeAgreement::ClearNonOpenMarketItems()
{
	for( long iIndex=0; iIndex<2; iIndex++ )
	{
		//m_iPlayerMoneyPerTurn[iIndex] = 0;
		//m_iPlayerIronPerTurn[iIndex] = 0;
		//m_iPlayerOilPerTurn[iIndex] = 0;
		//m_iPlayerFoodPerTurn[iIndex] = 0;

		m_PlayerUnitObjectId[iIndex].RemoveAll();
		m_PlayerCitiesObjectId[iIndex].RemoveAll();
		m_PlayerResourcesObjectId[iIndex].RemoveAll();

		m_bPlayerMapPeek[iIndex] = false; 
		m_bPlayerMapViewOngoing[iIndex] = false;

		m_PlayerTechnologyList[iIndex].RemoveAll();

		// Happens to both players
		m_bPeaceTreaty = false;
		m_bNonAggressionPact = false;
		m_iNonAggressionPactEndTurn = 0;

		m_bAlliance = false;

		m_AgreementToAttackPlayer[iIndex].Clear(); // A set of attack targets for each player
	}
}

void CEOSAITradeAgreement::AutoEqualize()  // Used when resources sell for a predetermined price
{
	// Add-up the iron, oil, food the player is offering
	long iMoneyValueLumpSum0 = 0;
	iMoneyValueLumpSum0 += (long)( this->m_iPlayerFoodLumpSum[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );
	iMoneyValueLumpSum0 += (long)( this->m_iPlayerIronLumpSum[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );
	iMoneyValueLumpSum0 += (long)( this->m_iPlayerOilLumpSum[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );

	long iMoneyValuePerTurn0 = 0;
	iMoneyValuePerTurn0 += (long)( this->m_iPlayerFoodPerTurn[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );
	iMoneyValuePerTurn0 += (long)( this->m_iPlayerIronPerTurn[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );
	iMoneyValuePerTurn0 += (long)( this->m_iPlayerOilPerTurn[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );

	// Add-up the iron, gold, food the OpenMarket is offering
	long iMoneyValueLumpSum1 = 0;
	iMoneyValueLumpSum1 += (long)( this->m_iPlayerFoodLumpSum[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );
	iMoneyValueLumpSum1 += (long)( this->m_iPlayerIronLumpSum[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );
	iMoneyValueLumpSum1 += (long)( this->m_iPlayerOilLumpSum[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );

	long iMoneyValuePerTurn1 = 0;
	iMoneyValuePerTurn1 += (long)( this->m_iPlayerFoodPerTurn[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );
	iMoneyValuePerTurn1 += (long)( this->m_iPlayerIronPerTurn[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );
	iMoneyValuePerTurn1 += (long)( this->m_iPlayerOilPerTurn[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );

	// Equalize
	long iDeltaLumpSum = iMoneyValueLumpSum0 - iMoneyValueLumpSum1;
	if( iDeltaLumpSum > 0 )
	{
		this->m_iPlayerMoneyLumpSum[0] = 0;
		this->m_iPlayerMoneyLumpSum[1] = iDeltaLumpSum;
	}
	else
	{
		this->m_iPlayerMoneyLumpSum[0] = -iDeltaLumpSum;
		this->m_iPlayerMoneyLumpSum[1] = 0;
	}

	long iDeltaPerTurn = iMoneyValuePerTurn0 - iMoneyValuePerTurn1;
	if( iDeltaPerTurn > 0 )
	{
		this->m_iPlayerMoneyPerTurn[0] = 0;
		this->m_iPlayerMoneyPerTurn[1] = iDeltaPerTurn;
	}
	else
	{
		this->m_iPlayerMoneyPerTurn[0] = -iDeltaPerTurn;
		this->m_iPlayerMoneyPerTurn[1] = 0;
	}
}

void CEOSAITradeAgreement::Clear()
{
	//m_iTradeAgreementId = 0;
	m_strTradeAgreementId = _T("");

	m_iAICode = 0;
	//m_strText = "";

	m_iPlayer[0] = 0;  // Player Number
	m_iPlayerMoneyLumpSum[0] = 0;
	m_iPlayerMoneyPerTurn[0] = 0;
	m_iPlayerIronLumpSum[0] = 0;
	m_iPlayerIronPerTurn[0] = 0;
	m_iPlayerOilLumpSum[0] = 0;
	m_iPlayerOilPerTurn[0] = 0;
	m_iPlayerFoodLumpSum[0] = 0;
	m_iPlayerFoodPerTurn[0] = 0;
	m_PlayerUnitObjectId[0].RemoveAll();
	m_PlayerCitiesObjectId[0].RemoveAll();
	m_PlayerResourcesObjectId[0].RemoveAll();
	m_PlayerTechnologyList[0].RemoveAll();
	m_bPlayerMapPeek[0] = false;
	m_bPlayerMapViewOngoing[0] = false;

	m_iPlayer[1] = 0;  // Player Number
	m_iPlayerMoneyLumpSum[1] = 0;
	m_iPlayerMoneyPerTurn[1] = 0;
	m_iPlayerIronLumpSum[1] = 0;
	m_iPlayerIronPerTurn[1] = 0;
	m_iPlayerOilLumpSum[1] = 0;
	m_iPlayerOilPerTurn[1] = 0;
	m_iPlayerFoodLumpSum[1] = 0;
	m_iPlayerFoodPerTurn[1] = 0;
	m_PlayerUnitObjectId[1].RemoveAll();
	m_PlayerCitiesObjectId[1].RemoveAll();
	m_PlayerResourcesObjectId[1].RemoveAll();
	m_PlayerTechnologyList[1].RemoveAll();
	m_bPlayerMapPeek[1] = false;
	m_bPlayerMapViewOngoing[1] = false;

	//
	m_bAllowPlayerToCounteroffer = true;

	// Happens to both players
	m_bPeaceTreaty = false;
	m_bNonAggressionPact = false;
	m_iNonAggressionPactEndTurn = 0;
	m_bAlliance = false;
	//m_bPermanentAlliance = false;
	//m_iAgreementToAttackPlayer = false;

	m_AgreementToAttackPlayer[0].Clear();
	m_AgreementToAttackPlayer[1].Clear();

	//m_eResponse = CTradeAgreement::enum_NoResponse;
	m_ePlayerResponse[0] = EOSAIEnumTradeAgreementResponse_NoResponse;
	m_ePlayerResponse[1] = EOSAIEnumTradeAgreementResponse_NoResponse;
	//m_iPlayerWhoMadeTheOffer = 0;
}

bool CEOSAITradeAgreement::IsEqualized()
{
	long iPlayer1LumpSumValue = 0;
	long iPlayer1PerTurnValue = 0;
	long iPlayer2LumpSumValue = 0;
	long iPlayer2PerTurnValue = 0;

	iPlayer1LumpSumValue += m_iPlayerMoneyLumpSum[0];
	iPlayer1LumpSumValue += (long)( m_iPlayerIronLumpSum[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );
	iPlayer1LumpSumValue += (long)( m_iPlayerOilLumpSum[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );
	iPlayer1LumpSumValue += (long)( m_iPlayerFoodLumpSum[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );

	iPlayer1PerTurnValue += m_iPlayerMoneyPerTurn[0];
	iPlayer1PerTurnValue += (long)( m_iPlayerIronPerTurn[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );
	iPlayer1PerTurnValue += (long)( m_iPlayerOilPerTurn[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );
	iPlayer1PerTurnValue += (long)( m_iPlayerFoodPerTurn[0]*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X() );

	iPlayer2LumpSumValue += m_iPlayerMoneyLumpSum[1];
	iPlayer2LumpSumValue += (long)( m_iPlayerIronLumpSum[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );
	iPlayer2LumpSumValue += (long)( m_iPlayerOilLumpSum[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );
	iPlayer2LumpSumValue += (long)( m_iPlayerFoodLumpSum[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );

	iPlayer2PerTurnValue += m_iPlayerMoneyPerTurn[1];
	iPlayer2PerTurnValue += (long)( m_iPlayerIronPerTurn[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );
	iPlayer2PerTurnValue += (long)( m_iPlayerOilPerTurn[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );
	iPlayer2PerTurnValue += (long)( m_iPlayerFoodPerTurn[1]*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X() );

	//if( fabs( iPlayer1LumpSumValue - iPlayer2LumpSumValue ) < 0.0001f &&
	//    fabs( iPlayer1PerTurnValue - fPlayer2PerTurnValue ) < 0.0001f )
	if( iPlayer1LumpSumValue == iPlayer2LumpSumValue &&
	    iPlayer1PerTurnValue == iPlayer2PerTurnValue )
	{
		return true;
	}
	return false;
}

//bool CTradeAgreement::IsLimitedToPlayerState( CWorldDesc* pWorldDesc, bool bLumpSumAndOngoingTrade )
//bool CEOSAITradeAgreement::IsLimitedToPlayerState(EOSAI::AIPlayer* pAIPlayer, bool bCheckLumpSumTrade, bool bCheckOngoingTrade, float fTimeDelta)
bool CEOSAITradeAgreement::DoesTheAIHaveEnoughResourcesToCompleteTheTrade(EOSAI::AIPlayer* pAIPlayer, bool bCheckLumpSumTrade, bool bCheckOngoingTrade, float fTimeDelta)
{
	//CWorldDescServer* pWorldDescServer = dynamic_cast< CWorldDescServer* >(pWorldDesc);
	//if (pWorldDescServer)
	{
		for (long i = 0; i<2; i++)
		{
			long iPlayer = m_iPlayer[i];
			if (iPlayer == 0) continue;

			int iAIPlayer = pAIPlayer->GetPlayerNumber();
			if (iAIPlayer != iPlayer)continue; // We only check our own resources

			//CWorldDescPlayerProxy* pPlayerProxy = pWorldDescServer->GetWorldDescPlayerProxy(iPlayer);
			if (bCheckOngoingTrade)
			{
				//if (pPlayerProxy)
				{
					//float f = g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X();
					//g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalMoney();
					
					// Make sure the player has enough money for LumpSum and just one "Per Turn" trade.
					if (fTimeDelta*m_iPlayerMoneyPerTurn[i] > g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalMoney()) { return false; }
					if (fTimeDelta*m_iPlayerIronPerTurn[i]  > g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalIron()) { return false; }
					if (fTimeDelta*m_iPlayerOilPerTurn[i]   > g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalOil()) { return false; }
					if (fTimeDelta*m_iPlayerFoodPerTurn[i]  > g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalFood()) { return false; }
				}
			}
			if (bCheckLumpSumTrade)
			{
				//if (pPlayerProxy)
				{
					// Make sure the player has enough money for LumpSum and just one "Per Turn" trade.
					if (m_iPlayerMoneyLumpSum[i] > g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalMoney()) { return false; }
					if (m_iPlayerIronLumpSum[i]  > g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalIron()) { return false; }
					if (m_iPlayerOilLumpSum[i]   > g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalOil()) { return false; }
					if (m_iPlayerFoodLumpSum[i]  > g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetTotalFood()) { return false; }
				}
				// Make sure the players actually owns the items they're trying to sell
				//   This shouldn't ever happen, and if it does, the Server should stop it.
				/*
				POSITION pos;
				pos = m_PlayerUnitObjectId[i].GetHeadPosition();
				while (pos)
				{
					EOSAI::PoiObjectIdAndName* pItem = m_PlayerUnitObjectId[i].GetNext(pos);
					CPoiObject* pPoiObject = pWorldDescServer->GetPoiObject(pItem->m_iObjectId);
					CUnit* pUnit = dynamic_cast< CUnit* >(pPoiObject);
					if (pUnit == NULL) return false;
					if (pUnit->GetOwner() != iPlayer) return false;
				}
				pos = m_PlayerCitiesObjectId[i].GetHeadPosition();
				while (pos)
				{
					EOSAI::PoiObjectIdAndName* pItem = m_PlayerCitiesObjectId[i].GetNext(pos);
					CPoiObject* pPoiObject = pWorldDescServer->GetPoiObject(pItem->m_iObjectId);
					CCity* pCity = dynamic_cast< CCity* >(pPoiObject);
					if (pCity == NULL) return false;
					if (pCity->GetOwner() != iPlayer) return false;
				}
				pos = m_PlayerResourcesObjectId[i].GetHeadPosition();
				while (pos)
				{
					EOSAI::PoiObjectIdAndName* pItem = m_PlayerResourcesObjectId[i].GetNext(pos);
					CPoiObject* pPoiObject = pWorldDescServer->GetPoiObject(pItem->m_iObjectId);
					CResourceSource* pRes = dynamic_cast< CResourceSource* >(pPoiObject);
					if (pRes == NULL) return false;
					if (pRes->GetOwner() != iPlayer) return false;
				}
				*/
			}
		}
		return true;
	}
	return true;

	/*
	//
	CWorldDescServer* pWorldDescServer = dynamic_cast< CWorldDescServer* >( pWorldDesc );
	if( pWorldDescServer )
	{
		for( long i=0; i<2; i++ )
		{
			long iPlayer = m_iPlayer[i];
			if( iPlayer == 0 ) continue;

			CWorldDescPlayerProxy* pPlayerProxy = pWorldDescServer->GetWorldDescPlayerProxy( iPlayer );
			if( bCheckOngoingTrade )
			{
				if( pPlayerProxy )
				{
					// Make sure the player has enough money for LumpSum and just one "Per Turn" trade.
					if( fTimeDelta*m_iPlayerMoneyPerTurn[i] > pPlayerProxy->GetTotalMoney() ){ return false; }
					if( fTimeDelta*m_iPlayerIronPerTurn[i]  > pPlayerProxy->GetTotalIron()  ){ return false; }
					if( fTimeDelta*m_iPlayerOilPerTurn[i]   > pPlayerProxy->GetTotalOil()   ){ return false; }
					if( fTimeDelta*m_iPlayerFoodPerTurn[i]  > pPlayerProxy->GetTotalFood()  ){ return false; }
				}
			}
			if( bCheckLumpSumTrade )
			{
				if( pPlayerProxy )
				{
					// Make sure the player has enough money for LumpSum and just one "Per Turn" trade.
					if( m_iPlayerMoneyLumpSum[i] > pPlayerProxy->GetTotalMoney() ){ return false; }
					if( m_iPlayerIronLumpSum[i]  > pPlayerProxy->GetTotalIron()  ){ return false; }
					if( m_iPlayerOilLumpSum[i]   > pPlayerProxy->GetTotalOil()   ){ return false; }
					if( m_iPlayerFoodLumpSum[i]  > pPlayerProxy->GetTotalFood()  ){ return false; }
				}
				//
				POSITION pos;
				pos = m_PlayerUnitObjectId[i].GetHeadPosition();
				while( pos )
				{
					CEOSAIPoiObjectIdAndName* pItem = m_PlayerUnitObjectId[i].GetNext( pos );
					CPoiObject* pPoiObject = pWorldDescServer->GetPoiObject( pItem->m_iObjectId );
					CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
					if( pUnit == NULL ) return false;
					if( pUnit->GetOwner() != iPlayer ) return false;
				}
				pos = m_PlayerCitiesObjectId[i].GetHeadPosition();
				while( pos )
				{
					CEOSAIPoiObjectIdAndName* pItem = m_PlayerCitiesObjectId[i].GetNext( pos );
					CPoiObject* pPoiObject = pWorldDescServer->GetPoiObject( pItem->m_iObjectId );
					CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
					if( pCity == NULL ) return false;
					if( pCity->GetOwner() != iPlayer ) return false;
				}
				pos = m_PlayerResourcesObjectId[i].GetHeadPosition();
				while( pos )
				{
					CEOSAIPoiObjectIdAndName* pItem = m_PlayerResourcesObjectId[i].GetNext( pos );
					CPoiObject* pPoiObject = pWorldDescServer->GetPoiObject( pItem->m_iObjectId );
					CResourceSource* pRes = dynamic_cast< CResourceSource* >( pPoiObject );
					if( pRes == NULL ) return false;
					if( pRes->GetOwner() != iPlayer ) return false;
				}
			}
		}
		return true;
	}
	CWorldDescPlayer* pWorldDescPlayer = dynamic_cast< CWorldDescPlayer* >( pWorldDesc );
	if( pWorldDescPlayer )
	{
		for( long i=0; i<2; i++ )
		{
			long iPlayer = m_iPlayer[i];
			if( iPlayer != pWorldDescPlayer->GetLocalPlayerNumber() ) continue;

			if( bCheckOngoingTrade )
			{
				// Make sure the player has enough money for LumpSum and just one "Per Turn" trade.
				if( fTimeDelta*m_iPlayerMoneyPerTurn[i] > pWorldDescPlayer->GetTotalMoney() ){ return false; }
				if( fTimeDelta*m_iPlayerIronPerTurn[i]  > pWorldDescPlayer->GetTotalIron()  ){ return false; }
				if( fTimeDelta*m_iPlayerOilPerTurn[i]   > pWorldDescPlayer->GetTotalOil()   ){ return false; }
				if( fTimeDelta*m_iPlayerFoodPerTurn[i]  > pWorldDescPlayer->GetTotalFood()  ){ return false; }
			}
			if( bCheckLumpSumTrade )
			{
				// Make sure the player has enough money for LumpSum and just one "Per Turn" trade.
				if( m_iPlayerMoneyLumpSum[i] > pWorldDescPlayer->GetTotalMoney() ){ return false; }
				if( m_iPlayerIronLumpSum[i]  > pWorldDescPlayer->GetTotalIron()  ){ return false; }
				if( m_iPlayerOilLumpSum[i]   > pWorldDescPlayer->GetTotalOil()   ){ return false; }
				if( m_iPlayerFoodLumpSum[i]  > pWorldDescPlayer->GetTotalFood()  ){ return false; }

				//
				POSITION pos;
				pos = m_PlayerUnitObjectId[i].GetHeadPosition();
				while( pos )
				{
					CEOSAIPoiObjectIdAndName* pItem = m_PlayerUnitObjectId[i].GetNext( pos );
					CPoiObject* pPoiObject = pWorldDescPlayer->GetPoiObject( pItem->m_iObjectId );
					CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
					if( pUnit == NULL ) return false;
					if( pUnit->GetOwner() != iPlayer ) return false;
				}
				pos = m_PlayerCitiesObjectId[i].GetHeadPosition();
				while( pos )
				{
					CEOSAIPoiObjectIdAndName* pItem = m_PlayerCitiesObjectId[i].GetNext( pos );
					CPoiObject* pPoiObject = pWorldDescPlayer->GetPoiObject( pItem->m_iObjectId );
					CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
					if( pCity == NULL ) return false;
					if( pCity->GetOwner() != iPlayer ) return false;
				}
				pos = m_PlayerResourcesObjectId[i].GetHeadPosition();
				while( pos )
				{
					CEOSAIPoiObjectIdAndName* pItem = m_PlayerResourcesObjectId[i].GetNext( pos );
					CPoiObject* pPoiObject = pWorldDescPlayer->GetPoiObject( pItem->m_iObjectId );
					CResourceSource* pRes = dynamic_cast< CResourceSource* >( pPoiObject );
					if( pRes == NULL ) return false;
					if( pRes->GetOwner() != iPlayer ) return false;
				}
			}
		}
		return true;
	}
	return true;
	*/
}

bool CEOSAITradeAgreement::IsTheSame( CEOSAITradeAgreement* pOther )
{
	if( this->m_iPlayer[0] != pOther->m_iPlayer[0] ) return false;
	if( this->m_iPlayer[1] != pOther->m_iPlayer[1] ) return false;

	if( this->m_iOfferedOnTurn != pOther->m_iOfferedOnTurn ) return false;

	if( this->m_iAICode != pOther->m_iAICode ) return false;
	//if( this->m_strText != pOther->m_strText ) return false;

	if( this->m_iPlayerMoneyLumpSum[0] != pOther->m_iPlayerMoneyLumpSum[0] ) return false;
	if( this->m_iPlayerMoneyLumpSum[1] != pOther->m_iPlayerMoneyLumpSum[1] ) return false;
	if( this->m_iPlayerMoneyPerTurn[0] != pOther->m_iPlayerMoneyPerTurn[0] ) return false;
	if( this->m_iPlayerMoneyPerTurn[1] != pOther->m_iPlayerMoneyPerTurn[1] ) return false;
	if( this->m_iPlayerIronLumpSum[0] != pOther->m_iPlayerIronLumpSum[0] ) return false;
	if( this->m_iPlayerIronLumpSum[1] != pOther->m_iPlayerIronLumpSum[1] ) return false;
	if( this->m_iPlayerIronPerTurn[0] != pOther->m_iPlayerIronPerTurn[0] ) return false;
	if( this->m_iPlayerIronPerTurn[1] != pOther->m_iPlayerIronPerTurn[1] ) return false;
	if( this->m_iPlayerOilLumpSum[0] != pOther->m_iPlayerOilLumpSum[0] ) return false;
	if( this->m_iPlayerOilLumpSum[1] != pOther->m_iPlayerOilLumpSum[1] ) return false;
	if( this->m_iPlayerOilPerTurn[0] != pOther->m_iPlayerOilPerTurn[0] ) return false;
	if( this->m_iPlayerOilPerTurn[1] != pOther->m_iPlayerOilPerTurn[1] ) return false;
	if( this->m_iPlayerFoodLumpSum[0] != pOther->m_iPlayerFoodLumpSum[0] ) return false;
	if( this->m_iPlayerFoodLumpSum[1] != pOther->m_iPlayerFoodLumpSum[1] ) return false;
	if( this->m_iPlayerFoodPerTurn[0] != pOther->m_iPlayerFoodPerTurn[0] ) return false;
	if( this->m_iPlayerFoodPerTurn[1] != pOther->m_iPlayerFoodPerTurn[1] ) return false;

	for( long i=0; i<2; i++ )
	{
		if( this->m_PlayerUnitObjectId[i].GetCount() != pOther->m_PlayerUnitObjectId[i].GetCount() ) return false;
		POSITION pos1 = this->m_PlayerUnitObjectId[i].GetHeadPosition();
		POSITION pos2 = pOther->m_PlayerUnitObjectId[i].GetHeadPosition();
		while( pos1 && pos2 )
		{
			long iObjectId1 = this->m_PlayerUnitObjectId[i].GetNext( pos1 )->m_iObjectId;
			long iObjectId2 = pOther->m_PlayerUnitObjectId[i].GetNext( pos2 )->m_iObjectId;

			if( iObjectId1 != iObjectId2 ) return false;
		}
	}
	for( long i=0; i<2; i++ )
	{
		if( this->m_PlayerCitiesObjectId[i].GetCount() != pOther->m_PlayerCitiesObjectId[i].GetCount() ) return false;
		POSITION pos1 = this->m_PlayerCitiesObjectId[i].GetHeadPosition();
		POSITION pos2 = pOther->m_PlayerCitiesObjectId[i].GetHeadPosition();
		while( pos1 && pos2 )
		{
			long iObjectId1 = this->m_PlayerCitiesObjectId[i].GetNext( pos1 )->m_iObjectId;
			long iObjectId2 = pOther->m_PlayerCitiesObjectId[i].GetNext( pos2 )->m_iObjectId;

			if( iObjectId1 != iObjectId2 ) return false;
		}
	}
	for( long i=0; i<2; i++ )
	{
		if( this->m_PlayerResourcesObjectId[i].GetCount() != pOther->m_PlayerResourcesObjectId[i].GetCount() ) return false;
		POSITION pos1 = this->m_PlayerResourcesObjectId[i].GetHeadPosition();
		POSITION pos2 = pOther->m_PlayerResourcesObjectId[i].GetHeadPosition();
		while( pos1 && pos2 )
		{
			long iObjectId1 = this->m_PlayerResourcesObjectId[i].GetNext( pos1 )->m_iObjectId;
			long iObjectId2 = pOther->m_PlayerResourcesObjectId[i].GetNext( pos2 )->m_iObjectId;

			if( iObjectId1 != iObjectId2 ) return false;
		}
	}
	for( long i=0; i<2; i++ )
	{
		if( this->m_PlayerTechnologyList[i].GetCount() != pOther->m_PlayerTechnologyList[i].GetCount() ) return false;
		POSITION pos1 = this->m_PlayerTechnologyList[i].GetHeadPosition();
		POSITION pos2 = pOther->m_PlayerTechnologyList[i].GetHeadPosition();
		while( pos1 && pos2 )
		{
			CString strTechName1 = this->m_PlayerTechnologyList[i].GetNext( pos1 );
			CString strTechName2 = pOther->m_PlayerTechnologyList[i].GetNext( pos2 );

			if( strTechName1 != strTechName2 ) return false;
		}
	}

	if( this->m_bAllowPlayerToCounteroffer != pOther->m_bAllowPlayerToCounteroffer ) return false;

	if( this->m_bPlayerMapPeek[0] != pOther->m_bPlayerMapPeek[0] ) return false;
	if( this->m_bPlayerMapPeek[1] != pOther->m_bPlayerMapPeek[1] ) return false;
	if( this->m_bPlayerMapViewOngoing[0] != pOther->m_bPlayerMapViewOngoing[0] ) return false;
	if( this->m_bPlayerMapViewOngoing[1] != pOther->m_bPlayerMapViewOngoing[1] ) return false;
	if( this->m_bPeaceTreaty != pOther->m_bPeaceTreaty ) return false;
	if( this->m_bNonAggressionPact != pOther->m_bNonAggressionPact ) return false;
	if( this->m_bNonAggressionPact && pOther->m_bNonAggressionPact )
	{
		if( this->m_iNonAggressionPactEndTurn != pOther->m_iNonAggressionPactEndTurn ) return false;
	}
	if( this->m_bAlliance != pOther->m_bAlliance ) return false;
	//if( this->m_bPermanentAlliance != pOther->m_bPermanentAlliance ) return false;
	//if( this->m_iAgreementToAttackPlayer != pOther->m_iAgreementToAttackPlayer ) return false;

	if( this->m_AgreementToAttackPlayer[0] != pOther->m_AgreementToAttackPlayer[0] ) return false;
	if( this->m_AgreementToAttackPlayer[1] != pOther->m_AgreementToAttackPlayer[1] ) return false;

	return true;
}
/*
void CTradeAgreement::SetOfferIsOnlyAvailableOnTurnX( long iTurn )
{
	m_iOfferIsOnlyAvailableOnTurnX = iTurn;
}

long CTradeAgreement::GetOfferIsOnlyAvailableOnTurnX()
{
	return m_iOfferIsOnlyAvailableOnTurnX;
}
*/
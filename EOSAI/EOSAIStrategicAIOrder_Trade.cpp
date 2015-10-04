
#include <stdafx.h>
#include "EOSAIInterface.h"
#include "EOSAIStrategicAIOrder_Trade.h"
//#include "Player.h"
#include "AIPlayer.h"
#include "EOSAIPlayerManager.h"
#include "EOSAITradeAgreement.h"
#include "EOSAITradeAgreementResponse.h"
#include "EOSAICommonData2.h"
#include "MessageFromAI_TradeOffer.h"
//#include "GameEvent_IMail.h"
//#include "TradeAgreement.h"
//#include "WorldDescServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIStrategicAIOrder_Trade::CEOSAIStrategicAIOrder_Trade( CEOSAIStrategicAI* pStrategicAI ) 
	: CEOSAIStrategicAIOrder( pStrategicAI )
{
	m_iToPlayer = 0;
	m_fTradeBalance = 0.0f;
	m_eResponse = EOSAIEnumTradeAgreementResponse_NoResponse;//CEOSAITradeAgreement::enum_NoResponse;
}

void CEOSAIStrategicAIOrder_Trade::Execute( long iCurrentTurn )
{
	m_iExecutionTurn = iCurrentTurn;
 	m_eCurrentState = enumState_Pending;

	ASSERT( m_iToPlayer >= 0 );

	// Send an IMail message
	long iAIPlayer = GetPlayerNumber();
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iAIPlayer );
	ASSERT( pAIPlayer );
	//m_TradeIMail.m_iAIMessageUID = pAIPlayer->GetNextAIMailId();

	EOSAI::MessageFromAI_TradeOffer* pTradeOfferMessage = new EOSAI::MessageFromAI_TradeOffer();
	pTradeOfferMessage->m_iFromPlayer = iAIPlayer;
	pTradeOfferMessage->m_iToPlayer = m_iToPlayer;
	pTradeOfferMessage->m_ResourcesToBuy[_T("Money")] = m_ResourcesToBuy[_T("Money")];
	pTradeOfferMessage->m_ResourcesToBuy[_T("Food")] = m_ResourcesToBuy[_T("Food")];
	pTradeOfferMessage->m_ResourcesToBuy[_T("Iron")] = m_ResourcesToBuy[_T("Iron")];
	pTradeOfferMessage->m_ResourcesToBuy[_T("Oil")] = m_ResourcesToBuy[_T("Oil")];
	pTradeOfferMessage->m_ResourcesToSell[_T("Money")] = m_ResourcesToSell[_T("Money")];
	pTradeOfferMessage->m_ResourcesToSell[_T("Food")] = m_ResourcesToSell[_T("Food")];
	pTradeOfferMessage->m_ResourcesToSell[_T("Iron")] = m_ResourcesToSell[_T("Iron")];
	pTradeOfferMessage->m_ResourcesToSell[_T("Oil")] = m_ResourcesToSell[_T("Oil")];
	/*
	pTradeOfferMessage->m_ResourcesToBuy[_T("Money")] = m_ResourcesToBuy.Get(_T("Money"));
	pTradeOfferMessage->m_ResourcesToBuy[_T("Food")] = m_ResourcesToBuy.Get(_T("Food"));
	pTradeOfferMessage->m_ResourcesToBuy[_T("Iron")] = m_ResourcesToBuy.Get(_T("Iron"));
	pTradeOfferMessage->m_ResourcesToBuy[_T("Oil")] = m_ResourcesToBuy.Get(_T("Oil"));
	pTradeOfferMessage->m_ResourcesToSell[_T("Money")] = m_ResourcesToSell.Get(_T("Money"));
	pTradeOfferMessage->m_ResourcesToSell[_T("Food")] = m_ResourcesToSell.Get(_T("Food"));
	pTradeOfferMessage->m_ResourcesToSell[_T("Iron")] = m_ResourcesToSell.Get(_T("Iron"));
	pTradeOfferMessage->m_ResourcesToSell[_T("Oil")] = m_ResourcesToSell.Get(_T("Oil"));
	*/
	//	std::map<CString,float> m_ResourcesToBuy;
	//	std::map<CString,float> m_ResourcesToSell;

	g_pEOSAIInterface->SendMessageFromAI( pTradeOfferMessage );

	//ASSERT( false );
#ifdef GAME_CODE
/*
	CGameEvent_IMail* pIMail = new CGameEvent_IMail();
	pIMail->SetSender( iAIPlayer );
	pIMail->SetSendTo( CEOSAIIntSet( m_iTradePartner ) );
	pIMail->SetSubject( Lang( 1232, _T("Trade Offer") ) );
	pIMail->SetMessage( Lang( 1342, _T("Would you like to trade?") ) );
	pIMail->SetAIMessageType( CGameEvent_IMail::enumType_Trade );
	//pIMail->SetAIMailId( m_TradeIMail.m_iAIMessageUID );
	//pIMail->AttachReplyOption( EnumIMailResponse_Approve, "Yes, that sounds like a good idea." );
	//pIMail->AttachReplyOption( EnumIMailResponse_Decline, "No thanks, maybe later." );
	//pIMail->AttachReplyOption( EnumIMailResponse_Reject,  "Peace will rule when you are defeated." );
	//pIMail->SetResponseTimerDuration( 60 );
*/
	CEOSAITradeAgreement* pTradeAgreement = new CEOSAITradeAgreement();
	pTradeAgreement->m_iPlayer[0] = GetPlayerNumber();
	pTradeAgreement->m_iPlayer[1] = m_iTradePartner;
	pTradeAgreement->m_iPlayerWhoMadeTheOffer = GetPlayerNumber();
	//pTradeAgreement->m_iTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();
	pTradeAgreement->m_strTradeAgreementId = GetWorldDescServer()->GetNextTradeAgreementId();
	//pTradeAgreement->m_ePlayerResponse[0] = CTradeAgreement::enum_Accept;
	pTradeAgreement->m_iOfferedOnTurn = g_pEOSAICommonData->GetCurrentTurn();
	/*
	pTradeAgreement->InitializeWithPlayerNumbers( 
		GetPlayer()->GetPlayerNumber(),
		m_iTargetPlayer,
		GetWorldDescPlayer()->GetNextTradeAgreementId() );
	*/
	//pTradeAgreement->m_iPlayerWhoMadeTheOffer = 1;
	//pTradeAgreement->m_iPlayer[0] = GetPlayer()->GetPlayerNumber(); // player number
	//pTradeAgreement->m_iPlayer[1] = iSendTo; // player number
	//pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_BrandNew;
	//pTradeAgreement->m_iTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();
	pTradeAgreement->m_iPlayerMoneyLumpSum[0] = 3;
	pTradeAgreement->m_iPlayerFoodLumpSum[1] = 3;
	//pTradeAgreement->m_iPlayerMoneyPerTurn[0] = 3;
	//pTradeAgreement->m_bAlliance = true;
	//pTradeAgreement->m_iTurn = GetWorldDescPlayer()->GetCurrentTurn();
	//pTradeAgreement->m_ePlayerResponse[0] = CTradeAgreement::enum_Accept;
	pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_WaitingForApproval;
	//pTradeAgreement->m_iTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();
	//m_iTradeAgreement = pTradeAgreement->m_iTradeAgreementId;
	m_strTradeAgreement = pTradeAgreement->m_strTradeAgreementId;

	//
	m_pStrategicAI->AddAIAction( pTradeAgreement );
#endif GAME_CODE

//	pIMail->SetTradeAgreement( pTradeAgreement );
//	pIMail->SendFromPlayerToServer();
}

//void  CEOSAIStrategicAIOrder_Trade::SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse )
/*
void  CEOSAIStrategicAIOrder_Trade::SetResponse( CIMailResponse* pIMailResponse )
{
	if( m_TradeIMail.m_iAIMessageUID == pIMailResponse->m_iAIMessageUID &&
		m_TradeIMail.m_PlayersIWantResponsesFrom.Exists( pIMailResponse->m_iPlayer ) &&
		m_TradeIMail.GetResponse( pIMailResponse->m_iPlayer ) == EnumIMailResponse_Undefined )
	{
		m_TradeIMail.SetResponse( pIMailResponse->m_iPlayer, pIMailResponse->m_eResponse );
		if( m_TradeIMail.AllResponsesReceived() )
		{
			//
		}
		return;
	}
	ASSERT( false );
}
*/
void  CEOSAIStrategicAIOrder_Trade::IncomingResponse( CEOSAITradeAgreementResponse* pResponse )
{
	//if( pResponse->m_iTradeAgreement == m_iTradeAgreement &&
	if( pResponse->m_strTradeAgreementId == m_strTradeAgreement &&
		pResponse->m_iFromPlayer == this->m_iToPlayer )
	{
		m_eResponse = pResponse->m_eResponse;
		if( pResponse->m_eResponse == EOSAIEnumTradeAgreementResponse_Accept )//CTradeAgreement::enum_Accept )
		{
		}
	}
}

//void  CEOSAIStrategicAIOrder_Trade::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void  CEOSAIStrategicAIOrder_Trade::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings )
{
	if( m_iExecutionTurn == 0 ){ return; }

	long iCurrentPlayer = this->GetPlayerNumber();
	//long iCurrentPlayer = GetPlayer()->GetPlayerNumber();
	long iTurnGap = iCurrentTurn - m_iExecutionTurn;

	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Accept )//CTradeAgreement::enum_Accept )
	{
	}
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Reject )//CTradeAgreement::enum_Reject )
	{
	}

	//
	// What if the player accepts the IMail, but doesn't approve the "Team" later?
	/*
	POSITION pos = m_TradeIMail.m_Responses.GetHeadPosition();
	while( pos )
	{
		CIMailResponse* pResponse = m_TradeIMail.m_Responses.GetNext( pos );
		if( pResponse->m_eResponse == EnumIMailResponse_Approve )
		{
			pState->AddFeelings( iCurrentPlayer, pResponse->m_iPlayer, 0.1f );
		}
		if( pResponse->m_eResponse == EnumIMailResponse_Reject )
		{
			pState->AddFeelings( iCurrentPlayer, pResponse->m_iPlayer, -0.1f );
		}
	}
	*/
}


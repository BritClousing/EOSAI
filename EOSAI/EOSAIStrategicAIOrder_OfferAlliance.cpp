
#include <stdafx.h>
#include "EOSAIStrategicAIOrder_OfferAlliance.h"
//#include "Player.h"
#include "AIPlayer.h"
#include "EOSAIPlayerManager.h"
#include "EOSAITradeAgreement.h"
#include "EOSAIMailResponse.h"
//#include "GameEvent_IMail.h"
//#include "TradeAgreement.h"
//#include "WorldDescServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIStrategicAIOrder_OfferAlliance::Execute( long iCurrentTurn )
{
	m_iExecutionTurn = iCurrentTurn;
 	m_eCurrentState = enumState_Pending;

	ASSERT( m_iTargetPlayer > 0 );

	ASSERT( false );
#ifdef GAME_CODE
	// Send an IMail message
	long iAIPlayer = GetPlayerNumber();
	//CAIPlayer* pAIPlayer = GetCommonState()->GetAIPlayer( iAIPlayer );
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iAIPlayer );
	ASSERT( pAIPlayer );
	m_AllianceIMail.m_iAIMessageUID = pAIPlayer->GetNextAIMailId();

	CGameEvent_IMail* pIMail = new CGameEvent_IMail();
	pIMail->SetSender( iAIPlayer );
	pIMail->SetSendTo( CEOSAIIntSet( m_iTargetPlayer ) );
	pIMail->SetSubject( Lang( 1329, _T("I suggest an alliance") ) );
	pIMail->SetMessage( Lang( 1330, _T("I suggest we form an alliance.") ) );
	pIMail->SetAIMessageType( CGameEvent_IMail::enumType_Alliance );
	pIMail->SetAIMailId( m_AllianceIMail.m_iAIMessageUID );
	//pIMail->AttachReplyOption( EnumIMailResponse_Approve, "Yes, that sounds like a good idea." );
	//pIMail->AttachReplyOption( EnumIMailResponse_Decline, "No thanks, maybe later." );
	//pIMail->AttachReplyOption( EnumIMailResponse_Reject,  "Peace will rule when you are defeated." );
	//pIMail->SetResponseTimerDuration( 60 );

	CEOSAITradeAgreement* pTradeAgreement = new CEOSAITradeAgreement();
	pTradeAgreement->m_iPlayer[0] = GetPlayerNumber();
	pTradeAgreement->m_iPlayer[1] = m_iTargetPlayer;
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
	//pTradeAgreement->m_iPlayerMoneyLumpSum[0] = 3;
	//pTradeAgreement->m_iPlayerMoneyPerTurn[0] = 3;
	pTradeAgreement->m_bAlliance = true;
	//pTradeAgreement->m_iTurn = GetWorldDescPlayer()->GetCurrentTurn();
	//pTradeAgreement->m_ePlayerResponse[0] = CTradeAgreement::enum_Accept;
	pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_WaitingForApproval;
	//pTradeAgreement->m_iTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();

	pIMail->SetTradeAgreement( pTradeAgreement );

	pIMail->SendFromPlayerToServer();
#endif GAME_CODE
}

//void  CEOSAIStrategicAIOrder_OfferAlliance::SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse )
void  CEOSAIStrategicAIOrder_OfferAlliance::IncomingResponse( CEOSAIMailResponse* pIMailResponse )
{
	if( m_AllianceIMail.m_iAIMessageUID == pIMailResponse->m_iAIMessageUID &&
		m_AllianceIMail.m_PlayersIWantResponsesFrom.Exists( pIMailResponse->m_iFromPlayer ) &&
		m_AllianceIMail.GetResponse( pIMailResponse->m_iFromPlayer ) == EOSAI::EnumAIMailResponse_Undefined )
	{
		m_AllianceIMail.SetResponse( pIMailResponse->m_iFromPlayer, pIMailResponse->m_eResponse );
		if( m_AllianceIMail.AllResponsesReceived() )
		{
			//
		}
		/*
		if( pObserver->AllResponsesReceived() )
		{
			NotifyEvent_AllResponsesHaveBeenReceived();
		}
		else
		{
			NotifyEvent_SomeResponsesHaveBeenReceived();
		}
		*/
		return;
	}
	ASSERT( false );
}


//void  CEOSAIStrategicAIOrder_OfferAlliance::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void  CEOSAIStrategicAIOrder_OfferAlliance::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings )
{
	if( m_iExecutionTurn == 0 ){ return; }

	long iCurrentPlayer = GetPlayerNumber();
	long iTurnGap = iCurrentTurn - m_iExecutionTurn;

	//
	// What if the player accepts the IMail, but doesn't approve the "Team" later?
	POSITION pos = m_AllianceIMail.m_Responses.GetHeadPosition();
	while( pos )
	{
		CEOSAIMailResponse* pResponse = m_AllianceIMail.m_Responses.GetNext( pos );
		if( pResponse->m_eResponse == EOSAI::EnumAIMailResponse_Approve )
		{
			pFeelings->Value( iCurrentPlayer, pResponse->m_iFromPlayer ) += 0.1f;
		}
		if( pResponse->m_eResponse == EOSAI::EnumAIMailResponse_Reject )
		{
			pFeelings->Value( iCurrentPlayer, pResponse->m_iFromPlayer ) -= 0.1f;
		}
	}
}


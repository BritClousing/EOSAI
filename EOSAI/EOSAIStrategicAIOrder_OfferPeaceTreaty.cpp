
#include <stdafx.h>
#include "EOSAIStrategicAIOrder_OfferPeaceTreaty.h"
//#include "Player.h"
#include "AIPlayer.h"
#include "EOSAIPlayerManager.h"
#include "EOSAITradeAgreement.h"
#include "EOSAICommonData.h"
//#include "GameEvent_IMail.h"
//#include "TradeAgreement.h"
//#include "WorldDescServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIStrategicAIOrder_OfferPeaceTreaty::SetTargetPlayer( long iTargetPlayer )
{
	m_iTargetPlayer = iTargetPlayer;
	m_PeaceTreatyIMail.m_PlayersIWantResponsesFrom.Add( iTargetPlayer );
}

void CEOSAIStrategicAIOrder_OfferPeaceTreaty::Execute( long iCurrentTurn )
{
	m_iExecutionTurn = iCurrentTurn;
 	m_eCurrentState = enumState_Pending;

	ASSERT( m_iTargetPlayer > 0 );

	// Send an IMail message
	long iAIPlayer = GetPlayerNumber();
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iAIPlayer );
	ASSERT( pAIPlayer );
	m_PeaceTreatyIMail.m_iAIMessageUID = pAIPlayer->GetNextAIMessageUID();
	ASSERT(false);
#ifdef GAME_CODE
	//CGameEvent_IMail* pIMail = new CGameEvent_IMail();
	CEOSAIMail* pIMail = new CEOSAIMail();
	pIMail->SetSender( iAIPlayer );
	pIMail->SetSendTo( CEOSAIIntSet( m_iTargetPlayer ) );
	pIMail->SetSubject( Lang( 1331, _T("I suggest peace") ) );
	pIMail->SetMessage( m_strMessage );
	//pIMail->SetMessage( "We should sign a peace agreement.  This war has gone on far too long." );
	pIMail->SetAIMessageType( CGameEvent_IMail::enumType_PeaceAgreement );
	pIMail->SetAIMailId( m_PeaceTreatyIMail.m_iAIMessageUID );
	//pIMail->AttachReplyOption( EnumIMailResponse_Approve, "Yes, that sounds like a good idea." );
	//pIMail->AttachReplyOption( EnumIMailResponse_Decline, "No thanks, maybe later." );
	//pIMail->AttachReplyOption( EnumIMailResponse_Reject,  "Peace will rule when you are defeated." );
	pIMail->SetResponseTimerDuration( 60 );

	CEOSAITradeAgreement* pTradeAgreement = new CEOSAITradeAgreement();
	pTradeAgreement->m_iPlayer[0] = GetPlayerNumber();
	pTradeAgreement->m_iPlayer[1] = m_iTargetPlayer;
	pTradeAgreement->m_iPlayerWhoMadeTheOffer = GetPlayerNumber();
	//pTradeAgreement->m_iTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();
	pTradeAgreement->m_strTradeAgreementId = GetWorldDescServer()->GetNextTradeAgreementId();
	pTradeAgreement->m_bPeaceTreaty = true;
	pTradeAgreement->m_iOfferedOnTurn = g_pEOSAICommonData->GetCurrentTurn();
	pTradeAgreement->m_ePlayerResponse[0] = CTradeAgreement::enum_Accept;
	pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_WaitingForApproval;
	//pTradeAgreement->m_iTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();

	// This offer is only available on this turn
	//pTradeAgreement->SetOfferIsOnlyAvailableOnTurnX( GetWorldDescPlayer()->GetCurrentTurn() );

	pIMail->SetTradeAgreement( pTradeAgreement );
	//m_iTradeAgreement = pTradeAgreement->m_iTradeAgreementId;
	m_strTradeAgreement = pTradeAgreement->m_strTradeAgreementId;

	pIMail->SendFromPlayerToServer();
#endif GAME_CODE
}
/*
void  CEOSAIStrategicAIOrder_OfferPeaceTreaty::SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse )
{
	if( m_PeaceTreatyIMail.m_iAIMessageUID == iAIMailId &&
		m_PeaceTreatyIMail.m_PlayersIWantResponsesFrom.Exists( iPlayer ) &&
		m_PeaceTreatyIMail.GetResponse( iPlayer ) == EnumIMailResponse_Undefined )
	{
		m_PeaceTreatyIMail.SetResponse( iPlayer, eResponse );
		return;
	}
	ASSERT( false );
}
*/

void  CEOSAIStrategicAIOrder_OfferPeaceTreaty::IncomingResponse( CEOSAIMailResponse* pIMailResponse )
{
	if( m_PeaceTreatyIMail.m_iAIMessageUID == pIMailResponse->m_iAIMessageUID &&
		m_PeaceTreatyIMail.m_PlayersIWantResponsesFrom.Exists( pIMailResponse->m_iFromPlayer ) &&
		m_PeaceTreatyIMail.GetResponse( pIMailResponse->m_iFromPlayer ) == EOSAI::EnumAIMailResponse_Undefined )
	{
		m_PeaceTreatyIMail.SetResponse( pIMailResponse->m_iFromPlayer, pIMailResponse->m_eResponse );
		return;
	}
	ASSERT( false );
}

//void  CEOSAIStrategicAIOrder_OfferPeaceTreaty::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void  CEOSAIStrategicAIOrder_OfferPeaceTreaty::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings )
{
	if( m_iExecutionTurn == 0 ){ return; }

	long iCurrentPlayer = GetPlayerNumber();
	long iTurnGap = iCurrentTurn - m_iExecutionTurn;

	//
	// What if the player accepts the IMail, but doesn't approve the "Team" later?
	POSITION pos = m_PeaceTreatyIMail.m_Responses.GetHeadPosition();
	while( pos )
	{
		CEOSAIMailResponse* pResponse = m_PeaceTreatyIMail.m_Responses.GetNext( pos );
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



#include <stdafx.h>
#include "EOSAIStrategicAIOrder_ConcedeGame.h"
#include "MessageFromAI_ConcedeGame.h"
//#include "Player.h"
#include "AIPlayer.h"
#include "EOSAIMailResponse.h"
#include "EOSAIPlayerManager.h"
#include "EOSAIStrategicAIOrder_Resign.h"
#include "EOSAIInterface.h"
//#include "GameEvent_IMail.h"
//#include "TradeAgreement.h"
//#include "WorldDescPlayer.h"
//#include "MessageManager.h"
//#include "MessageTargetManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long  CEOSAIStrategicAIOrder_ConcedeGame::HowManyTurnsAgoDidILastSendThisMessage( CEOSAIStrategicAI* pStrategicAI )
{
	long iTurn = -100;
	POSITION pos = pStrategicAI->GetStrategicAIOrders()->GetHeadPosition();
	while( pos )
	{
		CEOSAIStrategicAIOrder*  pAIOrder = pStrategicAI->GetStrategicAIOrders()->GetNext( pos );
		CEOSAIStrategicAIOrder_ConcedeGame* pOfferSurrender = dynamic_cast< CEOSAIStrategicAIOrder_ConcedeGame* >( pAIOrder );
		if( pOfferSurrender )
		{
			iTurn = max( iTurn, pOfferSurrender->m_iExecutionTurn );
		}
	}
	return pStrategicAI->GetCurrentTurn() - iTurn;
}

void CEOSAIStrategicAIOrder_ConcedeGame::Execute( long iCurrentTurn )
{
	m_iExecutionTurn = iCurrentTurn;
 	m_eCurrentState = enumState_Pending;

	ASSERT( m_iSurrenderToPlayer > 0 );
	//ASSERT( GetCommonState()->GetNumberOfActivePlayers() <= 2 );

	long iAIPlayer = GetPlayerNumber();
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer(iAIPlayer);
	ASSERT(pAIPlayer);

	long iAIMessageUID = pAIPlayer->GetNextAIMessageUID();

	// Setup a response observer
	m_ResponseObserver.m_iAIMessageUID = iAIMessageUID;
	m_ResponseObserver.m_PlayersIWantResponsesFrom.Add( m_iSurrenderToPlayer );

	// Send a message
	EOSAI::MessageFromAI_ConcedeGame* pConcedeGame = new EOSAI::MessageFromAI_ConcedeGame();
	pConcedeGame->m_iFromAIPlayer = iAIPlayer;
	pConcedeGame->m_iToPlayer = m_iSurrenderToPlayer;
	pConcedeGame->m_AIMessageUID = iAIMessageUID;
	g_pEOSAIInterface->AddNewMessageFromAI(pConcedeGame);

#ifdef GAME_CODE
	CGameEvent_IMail* pIMail = new CGameEvent_IMail();
	pIMail->SetSender( iAIPlayer );
	pIMail->SetSendTo( CEOSAIIntSet( m_iSurrenderToPlayer ) );
	pIMail->SetSubject( Lang( 1325, _T("I Concede") ) );//My Terms of Surrender" );
	//pIMail->SetMessage( "I think we know who is going to win this war.  I would like to discuss the terms of my surrender.  Would you accept my offer?" );
	pIMail->SetMessage( Lang( 1326, _T("I think we know who is going to win this war.  I am willing to concede.") ) );
	pIMail->SetAIMessageType( CGameEvent_IMail::enumType_OfferSurrender );
	pIMail->SetAIMailId( m_SurrenderIMail.m_iAIMessageUID );
	pIMail->AttachReplyOption( EOSAI::EnumAIMailResponse_Approve, Lang( 1327, _T("Yes, I accept your surrender.") ) );
	pIMail->AttachReplyOption( EOSAI::EnumAIMailResponse_Decline, Lang( 1328, _T("No, I do not accept.") ) );
	pIMail->SetResponseTimerDuration( 60 );


/*
	CTradeAgreement* pTradeAgreement = new CTradeAgreement();
	pTradeAgreement->m_iPlayer[0] = GetPlayer()->GetPlayerNumber();
	pTradeAgreement->m_iPlayer[1] = m_iSurrenderToPlayer;
	pTradeAgreement->m_iPlayerWhoMadeTheOffer = GetPlayer()->GetPlayerNumber();
	//pTradeAgreement->m_iTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();
	pTradeAgreement->m_strTradeAgreementId = GetWorldDescPlayer()->GetNextTradeAgreementId();
	//pTradeAgreement->m_ePlayerResponse[0] = CTradeAgreement::enum_Accept;
	/-*
	pTradeAgreement->InitializeWithPlayerNumbers( 
		GetPlayer()->GetPlayerNumber(),
		m_iTargetPlayer,
		GetWorldDescPlayer()->GetNextTradeAgreementId() );
	*-/
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
*/
	pIMail->SendFromPlayerToServer();
#endif
}

//void  CEOSAIStrategicAIOrder_ConcedeGame::SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse )
void  CEOSAIStrategicAIOrder_ConcedeGame::IncomingResponse( CEOSAIMailResponse* pIMailResponse )
{
	ASSERT(m_ResponseObserver.m_iAIMessageUID == pIMailResponse->m_iAIMessageUID);
	if (m_ResponseObserver.m_iAIMessageUID == pIMailResponse->m_iAIMessageUID &&
		m_ResponseObserver.m_PlayersIWantResponsesFrom.Exists(pIMailResponse->m_iFromPlayer) &&
		m_ResponseObserver.GetResponse(pIMailResponse->m_iFromPlayer) == EOSAI::EnumAIMailResponse_Undefined)
	{
		long iCurrentTurn = m_pStrategicAI->GetCurrentTurn();
		if (iCurrentTurn == m_iExecutionTurn)
		{
			m_ResponseObserver.SetResponse(pIMailResponse->m_iFromPlayer, pIMailResponse->m_eResponse);
			if (m_ResponseObserver.AllResponsesReceived())
			{
				if (m_ResponseObserver.AllResponsesReceivedAndHaveApproved())
				{
					CEOSAIStrategicAIOrder_Resign* pResign = new CEOSAIStrategicAIOrder_Resign(m_pStrategicAI);
					m_pStrategicAI->AddStrategicAIOrder(pResign);
					pResign->Execute(iCurrentTurn);

					/*
					// Surrender
					g_pMessageManager->SendMessage_PlayerResigned(
						GetPlayer()->GetMessageTargetId(), //m_pGameAppState->GetCurrentPlayer()->GetMessageTargetId(),
						g_pMessageTargetManager->GetMessageTarget_ServerId(),
						GetPlayer()->GetPlayerNumber() ); //m_pGameAppState->GetCurrentPlayer()->GetPlayerNumber() );
					*/
				}
			}
			return;
		}
		return;
	}
	ASSERT( false );
}

void  CEOSAIStrategicAIOrder_ConcedeGame::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings )
{
	/*
	if( m_iExecutionTurn == 0 ){ return; }

	long iCurrentPlayer = GetPlayerNumber();
	long iTurnGap = iCurrentTurn - m_iExecutionTurn;

	//
	// What if the player accepts the IMail, but doesn't approve the "Team" later?
	POSITION pos = m_ResponseObserver.m_Responses.GetHeadPosition();
	while( pos )
	{
		CEOSAIMailResponse* pResponse = m_ResponseObserver.m_Responses.GetNext( pos );
		if( pResponse->m_eResponse == EOSAI::EnumAIMailResponse_Approve )
		{
			pFeelings->Value( iCurrentPlayer, pResponse->m_iFromPlayer ) += 0.1f;
		}
		if( pResponse->m_eResponse == EOSAI::EnumAIMailResponse_Reject )
		{
			pFeelings->Value( iCurrentPlayer, pResponse->m_iFromPlayer ) -= 0.1f;
		}
	}
	*/
}




#include <stdafx.h>
#include "EOSAIStrategicAI.h"
#include "EOSAIStrategicAIOrder_SuggestTeam.h"
#include "EOSAIPlayerInteraction.h"
#include "EOSAIMailResponse.h"
//#include "InternalMail.h"
//#include "GameEvent_IMail.h"
//#include "TradeAgreement.h"
//#include "TWDx.h"
//#include "MessageManager.h"
//#include "WorldDescPlayer.h"
//#include "Message2_GenericXML.h"
//#include "Player.h"
#include "AIPlayer.h"
//#include "MessageTargetManager.h"
//#include "EOSAIStopwatch2.h"
//#include "PlayerTeam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIStrategicAIOrder_SuggestTeam::Execute( long iCurrentTurn )
{
	m_iExecutionTurn = iCurrentTurn;
	m_eCurrentState = enumState_Pending;

	long iAIPlayer = GetPlayerNumber();
	EOSAI::AIPlayer* pAIPlayer = GetAIPlayer();
	ASSERT( pAIPlayer );

	ASSERT( m_InitialTeamSuggestion.m_PlayersIWantResponsesFrom.GetSize() == 0 );
	m_InitialTeamSuggestion.m_PlayersIWantResponsesFrom = m_TeamPlayers;
	m_InitialTeamSuggestion.m_PlayersIWantResponsesFrom.Remove( iAIPlayer );
	m_InitialTeamSuggestion.m_iAIMessageUID = pAIPlayer->GetNextAIMessageUID();

	ASSERT( false );

#ifdef GAME_CODE
	CGameEvent_IMail* pIMail = new CGameEvent_IMail();
	pIMail->SetSender( iAIPlayer );
	pIMail->SetSendTo( &m_InitialTeamSuggestion.m_PlayersIWantResponsesFrom );
	pIMail->SetSubject( Lang( 1338, _T("I suggest that we form a team") ) );
	pIMail->SetMessage( Lang( 1339, _T("We should form a team.") ) );
	pIMail->SetAIMessageType( CGameEvent_IMail::enumType_SuggestTeam );
	pIMail->SetAIMailId( m_InitialTeamSuggestion.m_iAIMessageUID );
	pIMail->AttachReplyOption( EnumIMailResponse_Approve, Lang( 1334, _T("Yes, that sounds like a good idea.") ) );
	pIMail->AttachReplyOption( EnumIMailResponse_Decline, Lang( 1340, _T("No thanks, maybe later.") ) );
	pIMail->AttachReplyOption( EnumIMailResponse_Reject,  Lang( 1341, _T("Ha.  You'd like to ride my coat-tails wouldn't you?") ) );
	pIMail->SetResponseTimerDuration( 60 );

	// Problem: the team details aren't showing up, the "Create Team" function does nothing.
	pIMail->SendFromPlayerToServer();
#endif GAME_CODE

	// Add this to the player interactions
	/*
	CEOSAIPlayerInteraction_StrategicAIOrder* pPlayerInteraction = new CEOSAIPlayerInteraction_StrategicAIOrder();
	pPlayerInteraction->m_pStrategicAIOrder = this;
	m_pStrategicAI->AddPlayerInteraction( pPlayerInteraction );
	*/
}
/*
void  CEOSAIStrategicAIOrder_SuggestTeam::SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse )
{
	if( m_InitialTeamSuggestion.m_iAIMessageUID == iAIMailId &&
		m_InitialTeamSuggestion.m_PlayersIWantResponsesFrom.Exists( iPlayer ) &&
		m_InitialTeamSuggestion.GetResponse( iPlayer ) == EnumIMailResponse_Undefined )
	{
		m_InitialTeamSuggestion.SetResponse( iPlayer, eResponse );
		if( m_InitialTeamSuggestion.AllResponsesReceived() &&
			m_InitialTeamSuggestion.AllResponsesReceivedAndHaveApproved() )
		{
			CreateTeamIMail();
		}
		m_pStrategicAI->StrategicAIOrderResponseReceived( true );
		return;
	}
	if( m_TeamCreation.m_iAIMessageUID == iAIMailId &&
		m_TeamCreation.m_PlayersIWantResponsesFrom.Exists( iPlayer ) &&
		m_TeamCreation.GetResponse( iPlayer ) == EnumIMailResponse_Undefined )
	{
		m_TeamCreation.SetResponse( iPlayer, eResponse );
		return;
	}
	ASSERT( false );
}
*/
void  CEOSAIStrategicAIOrder_SuggestTeam::IncomingResponse( CEOSAIMailResponse* pIMailResponse )
{
	if( m_InitialTeamSuggestion.m_iAIMessageUID == pIMailResponse->m_iAIMessageUID &&
		m_InitialTeamSuggestion.m_PlayersIWantResponsesFrom.Exists( pIMailResponse->m_iFromPlayer ) &&
		m_InitialTeamSuggestion.GetResponse( pIMailResponse->m_iFromPlayer ) == EOSAI::EnumAIMailResponse_Undefined )
	{
		m_InitialTeamSuggestion.SetResponse( pIMailResponse->m_iFromPlayer, pIMailResponse->m_eResponse );
		if( m_InitialTeamSuggestion.AllResponsesReceived() &&
			m_InitialTeamSuggestion.AllResponsesReceivedAndHaveApproved() )
		{
			CreateTeamIMail();
		}
		//m_pStrategicAI->StrategicAIOrderResponseReceived( true );
		return;
	}
	if( m_TeamCreation.m_iAIMessageUID == pIMailResponse->m_iAIMessageUID &&
		m_TeamCreation.m_PlayersIWantResponsesFrom.Exists( pIMailResponse->m_iFromPlayer ) &&
		m_TeamCreation.GetResponse( pIMailResponse->m_iFromPlayer ) == EOSAI::EnumAIMailResponse_Undefined )
	{
		m_TeamCreation.SetResponse( pIMailResponse->m_iFromPlayer, pIMailResponse->m_eResponse );
		return;
	}
	ASSERT( false );
}

void  CEOSAIStrategicAIOrder_SuggestTeam::CreateTeamIMail()
{
	ASSERT( m_InitialTeamSuggestion.AllResponsesReceived() );

ASSERT( false );
#ifdef GAME_CODE
	//double dTime = CEOSAIStopwatch2::GetTime();
	//CString strTeamId; strTeamId.Format( _T("%d-%f"), rand(), dTime );
	CString strTeamId = CPlayerTeam::GetRandomTeamId();

	CString strData;
	strData += _T("<MGroup>Team</MGroup>");
	strData += _T("<MType>SuggestedTeam</MType>");
	strData += _T("<TeamId>") + strTeamId + _T("</TeamId>");
	strData += _T("<Name>") + m_strTeam + _T("</Name>");
	strData += _T("<Players>") + m_TeamPlayers.WriteCommaDelimitedText() + _T("</Players>");

	CMessage2_GenericXML  ProposedTeam;
	ProposedTeam.SetSenderId( GetPlayer()->GetMessageTargetId() );
	ProposedTeam.AddSendTo( g_pMessageTargetManager->GetMessageTarget_ServerId() );
	ProposedTeam.SetData( strData );
	g_pMessageManager->Send( ProposedTeam );
#endif GAME_CODE

	/*
	CPlayer* pPlayer = m_pStrategicAI->GetPlayer();
	if( pPlayer )
	{
		g_pMessageManager->SendMessage_PlayerResigned( pPlayer->GetMessageTargetId(), g_pMessageTargetManager->GetMessageTarget_ServerId(), pPlayer->GetPlayerNumber() );
	}
	*/
}
/*
void  CEOSAIStrategicAIOrder_SuggestTeam::NotifyEvent_SomeResponsesHaveBeenReceived()
{
	m_pStrategicAI->StrategicAIOrderResponseReceived( true );
}

void  CEOSAIStrategicAIOrder_SuggestTeam::NotifyEvent_AllResponsesHaveBeenReceived()
{
	if( m_InitialTeamSuggestion.AllResponsesHaveBeenReceived() )
	{
		double dTime = CEOSAIStopwatch2::GetTime();
		CString strTeamId; strTeamId.Format( "%d-%f", rand(), dTime );

		CString strData;
		strData += "<MGroup>Team</MGroup>";
		strData += "<MType>SuggestedTeam</MType>";
		strData += "<TeamId>" + strTeamId + "</TeamId>";
		strData += "<Name>" + m_strTeam + "</Name>";
		strData += "<Players>" + m_TeamPlayers.WriteCommaDelimitedText() + "</Players>";

		//CMessage2_GenericXML  ProposedTeam;
		ProposedTeam.SetSenderId( GetPlayer()->GetMessageTargetId() );
		ProposedTeam.AddSendTo( g_pMessageTargetManager->GetMessageTarget_ServerId() );
		ProposedTeam.SetData( strData );
		g_pMessageManager->Send( ProposedTeam );

		/-*
		CPlayer* pPlayer = m_pStrategicAI->GetPlayer();
		if( pPlayer )
		{
			g_pMessageManager->SendMessage_PlayerResigned( pPlayer->GetMessageTargetId(), g_pMessageTargetManager->GetMessageTarget_ServerId(), pPlayer->GetPlayerNumber() );
		}
		*-/
	}
	m_pStrategicAI->StrategicAIOrderResponseReceived( true );
}
*/
void  CEOSAIStrategicAIOrder_SuggestTeam::NotifyEvent_TimeExpired()
{
	// If all the players have agreed, send out a team suggestion
	//m_pStrategicAI->StrategicAIOrderResponseReceived( true );
}

//void  CEOSAIStrategicAIOrder_SuggestTeam::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void  CEOSAIStrategicAIOrder_SuggestTeam::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings )
{
	if( m_iExecutionTurn == 0 ){ return; }

	long iCurrentPlayer = GetPlayerNumber();
	long iTurnGap = iCurrentTurn - m_iExecutionTurn;

	//
	// What if the player accepts the IMail, but doesn't approve the "Team" later?
	POSITION pos = m_InitialTeamSuggestion.m_Responses.GetHeadPosition();
	while( pos )
	{
		CEOSAIMailResponse* pResponse = m_InitialTeamSuggestion.m_Responses.GetNext( pos );
		if( pResponse->m_eResponse == EOSAI::EnumAIMailResponse_Approve )
		{
			pFeelings->Value( iCurrentPlayer, pResponse->m_iFromPlayer ) += 0.1f;
		}
		if( pResponse->m_eResponse == EOSAI::EnumAIMailResponse_Reject )
		{
			pFeelings->Value( iCurrentPlayer, pResponse->m_iFromPlayer ) -= 0.1f;
		}
	}
	pos = m_TeamCreation.m_Responses.GetHeadPosition();
	while( pos )
	{
		CEOSAIMailResponse* pResponse = m_TeamCreation.m_Responses.GetNext( pos );
		if( pResponse->m_eResponse == EOSAI::EnumAIMailResponse_Approve )
		{
			pFeelings->Value( iCurrentPlayer, pResponse->m_iFromPlayer ) += 0.1f;
		}
	}
}

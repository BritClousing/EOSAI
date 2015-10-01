
#include <stdafx.h>
#include "EOSAIStrategicAIOrder_SuggestMultilateralWar.h"
//#include "GameEvent_IMail.h"
#include "EOSAIMailResponse.h"
#include "AIPlayer.h"
//#include "Player.h"
//#include "CommonState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIStrategicAIOrder_SuggestMultilateralWar::Execute( long iCurrentTurn )
{
	m_iExecutionTurn = iCurrentTurn;
 	m_eCurrentState = enumState_Pending;

	// Send a message to potential allies in the war - get feedback
	long iAIPlayer = GetPlayerNumber();
	EOSAI::AIPlayer* pAIPlayer = GetAIPlayer();
	ASSERT( pAIPlayer );

	ASSERT( m_TargetPlayers.Intersects( m_PotentialAllies ) == false );
	ASSERT( m_TargetPlayers.Exists( iAIPlayer ) == false );
	ASSERT( m_PotentialAllies.Exists( iAIPlayer ) == false );
	ASSERT( m_TargetPlayers.GetSize() > 0 );
	ASSERT( m_PotentialAllies.GetSize() > 0 );
	ASSERT( AllPlayersExist( &m_PotentialAllies ) );
	ASSERT( AllPlayersExist( &m_TargetPlayers ) );

	CString strText;

	m_SuggestionIMail.m_iAIMessageUID = pAIPlayer->GetNextAIMessageUID();
	m_SuggestionIMail.m_PlayersIWantResponsesFrom = m_PotentialAllies;

	ASSERT( false );
#ifdef GAME_CODE
	CGameEvent_IMail* pIMail = new CGameEvent_IMail();
	pIMail->SetSender( iAIPlayer );
	pIMail->SetSendTo( &m_PotentialAllies );
	pIMail->SetSubject( Lang( 1332, _T("Join forces for an attack?") ) );

	strText.Format( Lang( 1333, _T("I suggest we join forces and go to war against %s.") ), GetTargetPlayersString( &m_TargetPlayers ) );
	//strText  = "I suggest we join forces and go to war against ";
	//strText += GetTargetPlayersString( &m_TargetPlayers );
	//strText += ".";

	pIMail->SetMessage( strText );
	pIMail->SetAIMessageType( CGameEvent_IMail::enumType_MultilateralWar );
	pIMail->SetAIMailId( m_SuggestionIMail.m_iAIMessageUID );
	pIMail->AttachReplyOption( EnumIMailResponse_Approve, Lang( 1334, _T("Yes, that sounds like a good idea.") ) );
	pIMail->AttachReplyOption( EnumIMailResponse_Decline, Lang( 1335, _T("No, thanks.") ) );
	pIMail->AttachReplyOption( EnumIMailResponse_Reject,  Lang( 1336, _T("No way.  I'm on their side.") ) );

	// Problem: the team details aren't showing up, the "Create Team" function does nothing.
	//pIMail->SetSuggestedTeam( "The A-Team", &m_TeamPlayers );
	pIMail->SendFromPlayerToServer();
#endif GAME_CODE
}

/*
void  CEOSAIStrategicAIOrder_SuggestMultilateralWar::SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse )
{
	if( m_SuggestionIMail.m_iAIMessageUID == iAIMailId &&
		m_SuggestionIMail.m_PlayersIWantResponsesFrom.Exists( iPlayer ) &&
		m_SuggestionIMail.GetResponse( iPlayer ) == EnumIMailResponse_Undefined )
	{
		m_SuggestionIMail.SetResponse( iPlayer, eResponse );
		if( m_SuggestionIMail.AllResponsesReceivedAndHaveApproved() )
		{
			SendConfirmationMessage();
		}
		return;
	}
	ASSERT( false );
}
*/
void  CEOSAIStrategicAIOrder_SuggestMultilateralWar::IncomingResponse( CEOSAIMailResponse* pIMailResponse )
{
	if( m_SuggestionIMail.m_iAIMessageUID == pIMailResponse->m_iAIMessageUID &&
		m_SuggestionIMail.m_PlayersIWantResponsesFrom.Exists( pIMailResponse->m_iFromPlayer ) &&
		m_SuggestionIMail.GetResponse( pIMailResponse->m_iFromPlayer ) == EOSAI::EnumAIMailResponse_Undefined )
	{
		m_SuggestionIMail.SetResponse( pIMailResponse->m_iFromPlayer, pIMailResponse->m_eResponse );
		if( m_SuggestionIMail.AllResponsesReceivedAndHaveApproved() )
		{
			SendConfirmationMessage();
		}
		return;
	}
	ASSERT( false );
}

void  CEOSAIStrategicAIOrder_SuggestMultilateralWar::SendConfirmationMessage()
{
	ASSERT( m_SuggestionIMail.AllResponsesReceivedAndHaveApproved() );

	// Send a message to potential allies in the war - get feedback
	long iAIPlayer = GetPlayerNumber();
	//CAIPlayer* pAIPlayer = GetCommonState()->GetAIPlayer( iAIPlayer );
	//ASSERT( pAIPlayer );

	ASSERT( false );
#ifdef GAME_CODE
	CGameEvent_IMail* pIMail = new CGameEvent_IMail();
	pIMail->SetSender( iAIPlayer );
	pIMail->SetSendTo( &m_PotentialAllies );
	pIMail->SetSubject( Lang( 291, _T("Declare War") ) );

	CString strText;
	strText.Format( Lang( 1337, _T("It is agreed, then: we will go to war against %s.  Declare war against %s before the end of this turn.") ), 
		GetTargetPlayersString( &m_TargetPlayers ), GetTargetPlayersString( &m_TargetPlayers ) );
	//strText += GetTargetPlayersString( &m_TargetPlayers );
	//strText += ".  ";
	//strText += "Declare war against ";
	//strText += GetTargetPlayersString( &m_TargetPlayers );
	//strText += " before the end of this turn.";

	pIMail->SetMessage( strText );
	pIMail->SetAIMessageType( CGameEvent_IMail::enumType_MultilateralWarConfirmationMessage );
	//pIMail->SetAIMailId( m_SuggestionIMail.m_iAIMessageUID );
	//pIMail->AttachReplyOption( EnumIMailResponse_Approve, "Declare War" );
	//pIMail->AttachReplyOption( EnumIMailResponse_Decline, "On second thought, maybe not." );
	//pIMail->AttachReplyOption( EnumIMailResponse_Reject,  "No way.  I'm on their side." );

	// Problem: the team details aren't showing up, the "Create Team" function does nothing.
	//pIMail->SetSuggestedTeam( "The A-Team", &m_TeamPlayers );
	pIMail->SendFromPlayerToServer();
#endif GAME_CODE
}


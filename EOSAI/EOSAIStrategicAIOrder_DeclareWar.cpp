
#include <stdafx.h>
#include "EOSAIStrategicAIOrder_DeclareWar.h"
#include "AIPlayer.h"
//#include "AIAction_DeclareWar.h"
//#include "Player.h"
//#include "Message2_DeclareWarMessage.h"
//#include "MessageManager.h"
//#include "CommonState.h"
//#include "TWDx.h"
#include "MessageFromAI_DeclareWar.h"
#include "EOSAIInterface.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIStrategicAIOrder_DeclareWar::Execute( long iCurrentTurn )
{
	m_iExecutionTurn = iCurrentTurn;
 	m_eCurrentState = enumState_Completed;

	long iAIPlayer = GetPlayerNumber();
	EOSAI::AIPlayer* pAIPlayer = GetAIPlayer();
	ASSERT( pAIPlayer );

	// The AI needs to send an outgoing message to the player or server.
	EOSAI::MessageFromAI_DeclareWar* pDeclareWar = new EOSAI::MessageFromAI_DeclareWar();
	pDeclareWar->m_iAttackingPlayer = iAIPlayer;
	pDeclareWar->m_iTargetPlayer = m_iTargetPlayer;
	g_pEOSAIInterface->SendMessageFromAI( pDeclareWar );

	//ASSERT( false );
	/*
	// Send a declaration message
	CMessage2_DeclareWarMessage  DeclarationMessage;
	//DeclarationMessage.SendToAll();
	DeclarationMessage.SendToServer();//AddSendTo( g_pMessageTargetManager->GetMessageTarget_ServerId() );
	DeclarationMessage.SetSenderId( pAIPlayer->GetPlayer()->GetMessageTargetId() );
	//DeclarationMessage.m_eDeclarationType = CGameEvent_PlayerDeclaration::enum_DeclarationOfWar;
	DeclarationMessage.m_iTargetPlayer = m_iTargetPlayer;
	g_pMessageManager->Send( DeclarationMessage );
	*/
}

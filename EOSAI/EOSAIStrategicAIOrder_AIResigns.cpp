
#include "stdafx.h"
#include "EOSAIStrategicAIOrder_AIResigns.h"
#include "MessageFromAI_ResignGame.h"
#include "EOSAIInterface.h"

void CEOSAIStrategicAIOrder_AIResigns::Execute(long iCurrentTurn)
{
	m_iExecutionTurn = iCurrentTurn;
	//m_eCurrentState = enumState_Pending;

	//ASSERT(m_iSurrenderToPlayer > 0);
	//ASSERT( GetCommonState()->GetNumberOfActivePlayers() <= 2 );

	long iAIPlayer = GetPlayerNumber();
	//EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer(iAIPlayer);
	//ASSERT(pAIPlayer);

	//long iAIMessageUID = pAIPlayer->GetNextAIMessageUID();

	// Setup a response observer
	//m_ResponseObserver.m_iAIMessageUID = iAIMessageUID;
	//m_ResponseObserver.m_PlayersIWantResponsesFrom.Add(m_iSurrenderToPlayer);

	// Send a message
	EOSAI::MessageFromAI_ResignGame* pResignGame = new EOSAI::MessageFromAI_ResignGame();
	pResignGame->m_iFromAIPlayer = iAIPlayer;
	//pConcedeGame->m_iToPlayer = m_iSurrenderToPlayer;
	//pConcedeGame->m_AIMessageUID = iAIMessageUID;
	g_pEOSAIInterface->SendMessageFromAI(pResignGame);
}

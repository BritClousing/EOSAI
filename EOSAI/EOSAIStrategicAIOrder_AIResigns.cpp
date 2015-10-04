
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

	// Send a message
	EOSAI::MessageFromAI_ResignGame* pResignGame = new EOSAI::MessageFromAI_ResignGame();
	pResignGame->m_iFromAIPlayer = iAIPlayer;
	//pConcedeGame->m_iToPlayer = m_iSurrenderToPlayer;
	//pConcedeGame->m_AIMessageUID = iAIMessageUID;
	g_pEOSAIInterface->SendMessageFromAI(pResignGame);
}

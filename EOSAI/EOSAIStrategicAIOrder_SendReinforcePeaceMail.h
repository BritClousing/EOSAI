
#pragma once
#include "EOSAIStrategicAIOrder.h"

//
// Send an email to reinforce peace between an existing neutral or allied player
//   This is done to try to prevent players from going to war against them, which is particularly useful
//   when the player is already embroiled in a war.
//

class CEOSAIStrategicAIOrder_SendReinforcePeaceMail : public CEOSAIStrategicAIOrder
{
	public:
		CEOSAIStrategicAIOrder_SendReinforcePeaceMail( CEOSAIStrategicAI* pStrategicAI ) : CEOSAIStrategicAIOrder( pStrategicAI ){ m_iSendTo = 0; }
		void  SendTo( long iPlayer ){ m_iSendTo = iPlayer; }

		long m_iSendTo;
};


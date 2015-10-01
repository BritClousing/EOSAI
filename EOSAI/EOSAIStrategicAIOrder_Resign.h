
#pragma once

#include "EOSAIStrategicAIOrder.h"

//
// This tells the game that the AI player is resigning.
//   In general, the 'ConcedeGame' action happens before this order is given.
//

class CEOSAIStrategicAIOrder_Resign : public CEOSAIStrategicAIOrder
{
	public:
		CEOSAIStrategicAIOrder_Resign( CEOSAIStrategicAI* pStrategicAI ) : CEOSAIStrategicAIOrder( pStrategicAI ){}

		virtual void  Execute(long iCurrentTurn); // Send the offer
};


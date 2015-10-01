
#pragma once

#include "EOSAIStrategicAIOrder.h"

class CEOSAIStrategicAIOrder_DeclareWar : public CEOSAIStrategicAIOrder
{
	public:
		CEOSAIStrategicAIOrder_DeclareWar( CEOSAIStrategicAI* pStrategicAI ) : CEOSAIStrategicAIOrder( pStrategicAI ){ m_iTargetPlayer = 0; }
		void SetTarget( int iTargetPlayer ){ m_iTargetPlayer = iTargetPlayer; }

		virtual void  Execute( long iCurrentTurn ); // Send the declaration

	private:
		int  m_iTargetPlayer;
};


#pragma once

#include "EOSAIIntSet.h"
#include "EOSAIStrategicAIOrder.h"

class CEOSAIStrategicAIOrder_SuggestMultilateralWar : public CEOSAIStrategicAIOrder
{
	public:
		CEOSAIStrategicAIOrder_SuggestMultilateralWar( CEOSAIStrategicAI* pStrategicAI ) : CEOSAIStrategicAIOrder( pStrategicAI ){}
		void  SetAllies( CEOSAIIntSet& PotentialAllies ){ m_PotentialAllies = PotentialAllies; }
		void  SetTargets( CEOSAIIntSet& TargetPlayers ){ m_TargetPlayers = TargetPlayers; }

		virtual void  Execute( long iCurrentTurn );
		//
		virtual bool  ContainsAIMailId( long iAIMessageUID){ return m_SuggestionIMail.m_iAIMessageUID == iAIMessageUID; }
		//virtual void  SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse );
		virtual void  IncomingResponse( CEOSAIMailResponse* pIMailResponse );
		//
		void SendConfirmationMessage();
		//
		CEOSAIStrategicOrderResponseObserver  m_SuggestionIMail;

		CEOSAIIntSet  m_TargetPlayers;
		CEOSAIIntSet  m_PotentialAllies; // send a message to these players
};


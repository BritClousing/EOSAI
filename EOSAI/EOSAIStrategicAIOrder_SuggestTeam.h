
#pragma once

#include "EOSAIIntSet.h"
#include "EOSAIStrategicAIOrder.h"

class CEOSAIStrategicAIOrder_SuggestTeam : public CEOSAIStrategicAIOrder
{
	public:
		CEOSAIStrategicAIOrder_SuggestTeam( CEOSAIStrategicAI* pStrategicAI ) : CEOSAIStrategicAIOrder( pStrategicAI ){}
		void  SetTeam( CString strTeam, CEOSAIIntSet& TeamPlayers ){ m_strTeam = strTeam; m_TeamPlayers = TeamPlayers; }

		virtual void  Execute( long iCurrentTurn ); // Send the offer
		//
		virtual bool  ContainsAIMailId( long iAIMessageUID){ return m_InitialTeamSuggestion.m_iAIMessageUID == iAIMessageUID || m_TeamCreation.m_iAIMessageUID == iAIMessageUID; }
		//virtual void  SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse );
		virtual void  IncomingResponse( CEOSAIMailResponse* pIMailResponse );
		//
		void  CreateTeamIMail();
		//virtual void  NotifyEvent_SomeResponsesHaveBeenReceived(); // Override
		//virtual void  NotifyEvent_AllResponsesHaveBeenReceived(); // Override
		virtual void  NotifyEvent_TimeExpired(); // Override
		//
		//virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		CString  m_strTeam;
		CEOSAIIntSet  m_TeamPlayers;
		CEOSAIStrategicOrderResponseObserver  m_InitialTeamSuggestion;
		CEOSAIStrategicOrderResponseObserver  m_TeamCreation;
};



#pragma once

#include "EOSAIStrategicAIOrder.h"

/*
ConcedeGame happens when the AIPlayer decides to give up the game. He requests if he can surrender to the player. If the player agrees, then
the AI will send a ResignGame message.
*/

class CEOSAIStrategicAIOrder_ConcedeGame : public CEOSAIStrategicAIOrder
{
	public:
		static long  HowManyTurnsAgoDidILastSendThisMessage( CEOSAIStrategicAI* pStrategicAI );

	public:
		CEOSAIStrategicAIOrder_ConcedeGame( CEOSAIStrategicAI* pStrategicAI ) : CEOSAIStrategicAIOrder( pStrategicAI ){ m_iSurrenderToPlayer = 0; }
		void  SurrenderToPlayer( long iStrongPlayer ){ m_iSurrenderToPlayer = iStrongPlayer; }

		virtual void  Execute( long iCurrentTurn ); // Send the offer
		//
		virtual bool  ContainsAIMailId( long iAIMessageUID ){ return m_ResponseObserver.m_iAIMessageUID == iAIMessageUID; }
		//virtual void  SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse );
		virtual void  IncomingResponse( CEOSAIMailResponse* pIMailResponse );
		//
		//void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		long    m_iSurrenderToPlayer;
		CEOSAIStrategicOrderResponseObserver m_ResponseObserver;// m_SurrenderIMail;
};


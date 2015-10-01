
#pragma once

#include "EOSAIStrategicAIOrder.h"

class CEOSAIStrategicAIOrder_OfferPeaceTreaty : public CEOSAIStrategicAIOrder
{
	public:
		CEOSAIStrategicAIOrder_OfferPeaceTreaty( CEOSAIStrategicAI* pStrategicAI ) : CEOSAIStrategicAIOrder( pStrategicAI )
		{
			m_iTargetPlayer = 0;
			m_strMessage = "We should sign a peace agreement.  This war has gone on far too long.";
		}
		void    SetTargetPlayer( long iTargetPlayer );
		void    SetIMailMessage( CString strMessage ){ m_strMessage = strMessage; }

		virtual float GetOddsOfSuccess(){ return 1.0f; } // this should be based on: whether the player rejected/declined a past offer recently, the balance of power, the direction of the war, etc
		virtual void  Execute( long iCurrentTurn ); // Send the offer
		//
		virtual bool  ContainsAIMailId( long iAIMessageUID){ return m_PeaceTreatyIMail.m_iAIMessageUID == iAIMessageUID; }
		//virtual bool  ContainsTradeAgreement( long iTradeAgreement ){ return m_iTradeAgreement == iTradeAgreement; }
		virtual bool  ContainsTradeAgreement( CString strTradeAgreement ){ return m_strTradeAgreement == strTradeAgreement; }
		//virtual void  SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse );
		virtual void  IncomingResponse( CEOSAIMailResponse* pIMailResponse );
		//
		//virtual void  NotifyEvent_SomeResponsesHaveBeenReceived(){} // Override
		//virtual void  NotifyEvent_AllResponsesHaveBeenReceived(){} // Override
		virtual void  NotifyEvent_TimeExpired(){} // Override
		//
		//virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		long     m_iTargetPlayer;
		CString  m_strMessage;
		//long     m_iTradeAgreement;
		CString  m_strTradeAgreement;

		CEOSAIStrategicOrderResponseObserver  m_PeaceTreatyIMail;
};


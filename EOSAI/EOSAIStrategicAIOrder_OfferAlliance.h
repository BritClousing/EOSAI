
#pragma once

#include "EOSAIStrategicAIOrder.h"

class CEOSAIStrategicAIOrder_OfferAlliance : public CEOSAIStrategicAIOrder
{
	public:
		CEOSAIStrategicAIOrder_OfferAlliance( CEOSAIStrategicAI* pStrategicAI ) : CEOSAIStrategicAIOrder( pStrategicAI ){ m_iTargetPlayer = 0; }
		void  SetPotentialAlly( long iTargetPlayer ){ m_iTargetPlayer = iTargetPlayer; }

		virtual void  Execute( long iCurrentTurn ); // Send the offer
		//
		virtual bool  ContainsAIMailId( long iAIMessageUID){ return m_AllianceIMail.m_iAIMessageUID == iAIMessageUID; }
		//virtual void  SetResponse( long iAIMailId, long iPlayer, EnumIMailResponse eResponse );
		virtual void  IncomingResponse( CEOSAIMailResponse* pIMailResponse );
		//
		//void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		long    m_iTargetPlayer;
		//long    m_iResponse;
		CEOSAIStrategicOrderResponseObserver  m_AllianceIMail;
};


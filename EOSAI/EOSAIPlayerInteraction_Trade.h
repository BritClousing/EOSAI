
#pragma once

#include "EOSAIPlayerInteraction.h"

class CEOSAIPlayerInteraction_Trade : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_Trade()
		{
			m_iRequestingPlayer = 0;
			m_iSentTo = 0;
			//m_iTradeAgreement = 0;
			//m_fTradeBalance = 0.0f;
			m_eResponse = EOSAIEnumTradeAgreementResponse_NoResponse;
			m_iImprovedForeignRelationsAIPlayer = 0;
			m_iImprovedForeignRelationsHumanPlayer = 0;
			m_fImprovedForeignRelationsValue01 = 0.0f; // AI player's opinion of playerX improves
		}

		void Set( long iTurn, long iRequestingPlayer, long iSentToPlayer, 
				  CString m_strTradeAgreement,
				  EOSAIEnumTradeAgreementResponse  eResponse,
				  long iAIPlayer, long iHumanPlayer, float fImprovedRelationsValue01 );

		//
		CEOSAISerial_INFORMATION_LONG( 203, CEOSAIPlayerInteraction_Trade );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );

		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		//
		long     m_iRequestingPlayer;
		long     m_iSentTo;
		//long     m_iTradeAgreement;
		CString  m_strTradeAgreement;
		//float    m_fTradeBalance;
		EOSAIEnumTradeAgreementResponse  m_eResponse;
		//
		long     m_iImprovedForeignRelationsAIPlayer;
		long     m_iImprovedForeignRelationsHumanPlayer;
		float    m_fImprovedForeignRelationsValue01; // AI player's opinion of playerX improves
};

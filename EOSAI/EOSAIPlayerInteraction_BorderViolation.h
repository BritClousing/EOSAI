
#pragma once

#include "EOSAIPlayerInteraction.h"

class CEOSAIPlayerInteraction_BorderViolation : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_BorderViolation()
		{
			m_iViolatorPlayer = 0;
			m_iViolateePlayer = 0;
			//m_eResponse = CTradeAgreement::enum_NoResponse;
			//m_iImprovedForeignRelationsAIPlayer = 0;
			//m_iImprovedForeignRelationsHumanPlayer = 0;
			//m_fImprovedForeignRelationsValue01 = 0.0f; // AI player's opinion of playerX improves
		}

		void Set( long iTurn, long iViolatorPlayer, long iViolateePlayer,  float fReducedForeignRelationsValue100 );
		virtual bool ValidateValues();

		//
		CEOSAISerial_INFORMATION_LONG( 209, CEOSAIPlayerInteraction_BorderViolation );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );

		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		//
		long     m_iViolatorPlayer;
		long     m_iViolateePlayer;
		//long     m_iTradeAgreement;
		//CString  m_strTradeAgreement;
		//float    m_fTradeBalance;
		//CTradeAgreement::EOSAIEnumTradeAgreementResponse  m_eResponse;
		//
		//long     m_iImprovedForeignRelationsAIPlayer;
		//long     m_iImprovedForeignRelationsHumanPlayer;
		float    m_fDecreasedForeignRelationsValue100; // AI player's opinion of playerX is reduced
};

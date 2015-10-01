
#include "stdafx.h"
#include "EOSAIPlayerInteraction_Trade.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIPlayerInteraction_Trade::Set( long iTurn,
		  long iRequestingPlayer, long iSentToPlayer, 
		  CString m_strTradeAgreement,
		  EOSAIEnumTradeAgreementResponse  eResponse,
		  long iAIPlayer, long iHumanPlayer, float fImprovedRelationsValue01 )
{
	m_iEventTurn = iTurn;
	m_iRequestingPlayer = iRequestingPlayer;
	m_iSentTo = iSentToPlayer;
	m_strTradeAgreement = m_strTradeAgreement;
	m_eResponse = eResponse;
	//
	m_iImprovedForeignRelationsAIPlayer = iAIPlayer;
	m_iImprovedForeignRelationsHumanPlayer = iHumanPlayer;
	m_fImprovedForeignRelationsValue01 = fImprovedRelationsValue01;
}

void CEOSAIPlayerInteraction_Trade::Serialize( CEOSAISerial* pSerial )
{
	pSerial->SerializeClassId( GetCEOSAISerialClassId() ); // handles the 256 conversion for long

	/*
	pSerial->Serialize( (char) 1 ); // version
	pSerial->Serialize( m_iEventTurn );

	pSerial->Serialize( m_iRequestingPlayer );
	pSerial->Serialize( m_iSentTo );
	pSerial->Serialize( m_iTradeAgreement );
	pSerial->Serialize( m_fTradeBalance );
	long iResponse = (long) m_eResponse;
	pSerial->Serialize( iResponse );
	*/
	pSerial->Serialize( (char) 2 ); // version
	pSerial->Serialize( m_iEventTurn );

	pSerial->Serialize( m_iRequestingPlayer );
	pSerial->Serialize( m_iSentTo );
	pSerial->SerializeANSI8( m_strTradeAgreement );
	//pSerial->Serialize( m_fTradeBalance );
	long iResponse = (long) m_eResponse;
	pSerial->Serialize( iResponse );
	//
	pSerial->Serialize( m_iImprovedForeignRelationsAIPlayer );
	pSerial->Serialize( m_iImprovedForeignRelationsHumanPlayer );
	pSerial->Serialize( m_fImprovedForeignRelationsValue01 ); // AI player's opinion of human improves
}

void CEOSAIPlayerInteraction_Trade::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	pSerial->ExtractAndVerifyClassId( GetCEOSAISerialClassId() );

	char i = pSerial->ExtractChar();
	if( i == 1 )
	{
		pSerial->Deserialize( m_iEventTurn );

		pSerial->Deserialize( m_iRequestingPlayer );
		pSerial->Deserialize( m_iSentTo );
		long iTradeAgreement;
		pSerial->Deserialize( iTradeAgreement );
		float fTradeBalance;
		pSerial->Deserialize( fTradeBalance );
		long iResponse = (long) m_eResponse;
		pSerial->Deserialize( iResponse );
		m_eResponse = (EOSAIEnumTradeAgreementResponse) iResponse;
	}
	if( i == 2 )
	{
		pSerial->Deserialize( m_iEventTurn );

		pSerial->Deserialize( m_iRequestingPlayer );
		pSerial->Deserialize( m_iSentTo );
		pSerial->DeserializeANSI8( m_strTradeAgreement );

		long iResponse = (long) m_eResponse;
		pSerial->Deserialize( iResponse );
		m_eResponse = (EOSAIEnumTradeAgreementResponse) iResponse;
		//
		pSerial->Deserialize( m_iImprovedForeignRelationsAIPlayer );
		pSerial->Deserialize( m_iImprovedForeignRelationsHumanPlayer );
		pSerial->Deserialize( m_fImprovedForeignRelationsValue01 ); // AI player's opinion of human improves
	}
}

/*
void CEOSAIPlayerInteraction_CreatedTeam::UpdateForeignRelationsState(
			long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
{
	// Fade the weight with time
	//   INCOMPLETE: Making a peace agreement should dull this effect
	POSITION pos1 = m_Players.GetList()->GetHeadPosition();
	while( pos1 )
	{
		int iPlayerA = m_Players.GetList()->GetNext( pos1 );
		//
		POSITION pos2 = m_Players.GetList()->GetHeadPosition();
		while( pos2 )
		{
			int iPlayerB = m_Players.GetList()->GetNext( pos2 );
			if( iPlayerA != iPlayerB )
			{
				pFeelings->Value( iPlayerA, iPlayerB ) = 1000.0f;
				pForeignRelations->Value( iPlayerA, iPlayerB ) = EOSAIEnumForeignRelations::enum_Teammate;
			}
		}
	}
}
*/

//void CEOSAIPlayerInteraction_Trade::UpdateForeignRelationsState( long iCurrentTurn,
void CEOSAIPlayerInteraction_Trade::UpdateForeignRelationsFeelings( long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
{
	/*
	float fDelta = 0.0f;
	if( m_eResponse == CTradeAgreement::enum_Accept  ){ fDelta  = +0.1f; }
	if( m_eResponse == CTradeAgreement::enum_Decline ){ fDelta  =  0.0f; }
	if( m_eResponse == CTradeAgreement::enum_Reject  ){ fDelta  = -0.05f; }

	// Fade the weight with time
	fDelta = AdjustDeltaAccordingToTime( iCurrentTurn - m_iEventTurn, fDelta, NORMAL_DECAY_TIME );
	pFeelings->Value( m_iRequestingPlayer, m_iSentTo ) += fDelta;
	*/
	// Fade the weight with time
	float fDelta = m_fImprovedForeignRelationsValue01/100.0f;
	fDelta = AdjustDeltaAccordingToTime( iCurrentTurn - m_iEventTurn, fDelta, NORMAL_DECAY_TIME );
	pFeelings->Value( m_iImprovedForeignRelationsAIPlayer, m_iImprovedForeignRelationsHumanPlayer ) += fDelta;
}

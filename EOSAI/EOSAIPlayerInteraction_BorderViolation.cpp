
#include "stdafx.h"
#include "EOSAIPlayerInteraction_BorderViolation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIPlayerInteraction_BorderViolation::Set( long iTurn, long iViolatorPlayer, long iViolateePlayer,  float fReducedForeignRelationsValue100 )
//		  long iTurn,
//		  long iRequestingPlayer, long iSentToPlayer, 
//		  CString m_strTradeAgreement,
//		  CTradeAgreement::EOSAIEnumTradeAgreementResponse  eResponse,
//		  long iAIPlayer, long iHumanPlayer, float fImprovedRelationsValue01 )
{
	m_iEventTurn = iTurn;
	m_iViolatorPlayer = iViolatorPlayer;
	m_iViolateePlayer = iViolateePlayer;
	m_fDecreasedForeignRelationsValue100 = fReducedForeignRelationsValue100;
}

void CEOSAIPlayerInteraction_BorderViolation::Serialize( CEOSAISerial* pSerial )
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
	char iVersion = 1;
	pSerial->Serialize( iVersion ); // version
	pSerial->Serialize( m_iEventTurn );
	pSerial->Serialize( m_iViolatorPlayer );
	pSerial->Serialize( m_iViolateePlayer );
	pSerial->Serialize( m_fDecreasedForeignRelationsValue100 ); // AI player's opinion of human improves
}

void CEOSAIPlayerInteraction_BorderViolation::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	pSerial->ExtractAndVerifyClassId( GetCEOSAISerialClassId() );

	char iVersion = 1;
	pSerial->Deserialize( iVersion );
	pSerial->Deserialize( m_iEventTurn );
	pSerial->Deserialize( m_iViolatorPlayer );
	pSerial->Deserialize( m_iViolateePlayer );
	pSerial->Deserialize( m_fDecreasedForeignRelationsValue100 ); // AI player's opinion of human improves
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

//void CEOSAIPlayerInteraction_BorderViolation::UpdateForeignRelationsState( long iCurrentTurn,
void CEOSAIPlayerInteraction_BorderViolation::UpdateForeignRelationsFeelings( long iCurrentTurn,
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
	float fDelta = -m_fDecreasedForeignRelationsValue100/100.0f;
	fDelta = AdjustDeltaAccordingToTime( iCurrentTurn - m_iEventTurn, fDelta, NORMAL_DECAY_TIME );
	pFeelings->Value( m_iViolateePlayer, m_iViolatorPlayer ) += fDelta;
}

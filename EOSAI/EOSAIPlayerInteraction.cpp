
#include "stdafx.h"
#include "EOSAIPlayerInteraction.h"
//#include "CommonState.h"
#include "EOSAIForeignRelationsSituation.h"
#include "EOSAIForeignRelationsState.h"
#include "EOSAIStrategicAIOrder.h"
//#include "WorldDescServer.h"
#include "EOSAIMathFunction.h"
//#include "GlobalForeignRelations.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIPlayerInteraction::Serialize( CEOSAISerial* pSerial )
{
	ASSERT( false );
	//pSerial->Serialize( m_iEventTurn );
}

void CEOSAIPlayerInteraction::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	ASSERT( false );
	//pSerial->Deserialize( m_iEventTurn );
}

float CEOSAIPlayerInteraction::AdjustDeltaAccordingToTime( long iTurnsSinceEvent, float fDelta, float fDecayTime )
{
	ASSERT( iTurnsSinceEvent >= 0 );
	float fTurnsSinceEvent = (float) iTurnsSinceEvent;
	float f = 0.0f;
	/*
	if( fTurnsSinceEvent < fDecayTime )
	{
		f = 1.0f - ( fTurnsSinceEvent / ( fDecayTime*2.0f ) );
	}
	else
	{
		f = 1.0f - ( fTurnsSinceEvent / ( fTurnsSinceEvent + fDecayTime ) );
	}
	*/
	if( fTurnsSinceEvent < fDecayTime ){ f = 1.0f; }
	else
	{
		fTurnsSinceEvent -= fDecayTime;
		if( fTurnsSinceEvent < fDecayTime )
		{
			f = 1.0f - ( fTurnsSinceEvent / ( fDecayTime*2.0f ) );
		}
		else
		{
			f = 1.0f - ( fTurnsSinceEvent / ( fTurnsSinceEvent + fDecayTime ) );
		}
	}

	return f * fDelta;
}

//

//void CEOSAIPlayerInteraction_StrategicAIOrder::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
/*
void CEOSAIPlayerInteraction_StrategicAIOrder::UpdateForeignRelationsState(
			long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >*                pFeelings )
{
	m_pStrategicAIOrder->UpdateForeignRelationsState( iCurrentTurn, pForeignRelations, pFeelings );
}
*/
//

void CEOSAIPlayerInteraction_CreatedAlliance::Serialize( CEOSAISerial* pSerial )
{
	pSerial->SerializeClassId( GetCEOSAISerialClassId() ); // handles the 256 conversion for long

	pSerial->Serialize( (char) 2 ); // version
	pSerial->Serialize( m_iPlayer1 );
	pSerial->Serialize( m_iPlayer2 );

	pSerial->Serialize( m_iEventTurn ); // Version 2
}

void CEOSAIPlayerInteraction_CreatedAlliance::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	pSerial->ExtractAndVerifyClassId( GetCEOSAISerialClassId() );

	char iVersion = pSerial->ExtractChar();
	pSerial->Deserialize( m_iPlayer1 );
	pSerial->Deserialize( m_iPlayer2 );

	if( iVersion >= 2 )
	{
		pSerial->Deserialize( m_iEventTurn ); // Version 2
	}
}

//void CEOSAIPlayerInteraction_CreatedAlliance::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void CEOSAIPlayerInteraction_CreatedAlliance::UpdateForeignRelationsFeelings(
			long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
			//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
			//float                Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] )
			//CGlobalForeignRelations* pForeignRelations )
{
	//ASSERT( pForeignRelations->Value( m_iPlayer1, m_iPlayer2 ) != EOSAIEnumForeignRelations::enum_Alliance );
	//ASSERT( pForeignRelations->Value( m_iPlayer2, m_iPlayer1 ) != EOSAIEnumForeignRelations::enum_Alliance );
	//ASSERT( pForeignRelations->Value( m_iPlayer1, m_iPlayer2 ) != EOSAIEnumForeignRelations::enum_Teammate );
	//ASSERT( pForeignRelations->Value( m_iPlayer2, m_iPlayer1 ) != EOSAIEnumForeignRelations::enum_Teammate );

	// Fade the weight with time?
	pFeelings->Value( m_iPlayer1, m_iPlayer2 ) += 0.2f;
	pFeelings->Value( m_iPlayer2, m_iPlayer1 ) += 0.2f;

	//pForeignRelations->Value( m_iPlayer1, m_iPlayer2 ) = EOSAIEnumForeignRelations::enum_Alliance;
	//pForeignRelations->Value( m_iPlayer2, m_iPlayer1 ) = EOSAIEnumForeignRelations::enum_Alliance;
}


void CEOSAIPlayerInteraction_AskedToJoinInWar::Serialize( CEOSAISerial* pSerial )
{
	pSerial->SerializeClassId( GetCEOSAISerialClassId() ); // handles the 256 conversion for long

	pSerial->Serialize( (char) 1 ); // version
	pSerial->Serialize( m_iRequestingPlayer );
	pSerial->Serialize( m_iSentTo );
	long iResponse = (long) m_eResponse;
	pSerial->Serialize( iResponse );
}

void CEOSAIPlayerInteraction_AskedToJoinInWar::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	pSerial->ExtractAndVerifyClassId( GetCEOSAISerialClassId() );

	char iVersion = pSerial->ExtractChar();
	pSerial->Deserialize( m_iRequestingPlayer );
	pSerial->Deserialize( m_iSentTo );
	long iResponse = 0;
	pSerial->Deserialize( iResponse );
	m_eResponse = (EOSAIEnumTradeAgreementResponse) iResponse;
}

//void CEOSAIPlayerInteraction_AskedToJoinInWar::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void CEOSAIPlayerInteraction_AskedToJoinInWar::UpdateForeignRelationsFeelings(
			long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
			//CGlobalForeignRelations* pForeignRelations )
{
	float fDelta = 0.0;
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Accept  ){ fDelta = +0.20f; }
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Decline ){ fDelta = -0.05f; }
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Reject  ){ fDelta = -0.20f; }

	// Fade the weight with time
	fDelta = AdjustDeltaAccordingToTime( iCurrentTurn - m_iEventTurn, fDelta, NORMAL_DECAY_TIME );
	pFeelings->Value( m_iRequestingPlayer, m_iSentTo ) += fDelta;
}



//void CEOSAIPlayerInteraction_AskedToMakePeace::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void CEOSAIPlayerInteraction_AskedToMakePeace::UpdateForeignRelationsFeelings( long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
			//CGlobalForeignRelations* pForeignRelations )
{
	float fDelta = 0.0f;
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Accept  ){ fDelta = +0.06f; }
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Decline ){ fDelta = -0.03f; }
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Reject  ){ fDelta = -0.06f; }

	// Fade the weight with time
	fDelta = AdjustDeltaAccordingToTime( iCurrentTurn - m_iEventTurn, fDelta, NORMAL_DECAY_TIME );
	pFeelings->Value( m_iRequestingPlayer, m_iSentTo ) += fDelta;

	/*
	if( m_eResponse == CTradeAgreement::enum_Accept )
	{
		pState->m_ForeignRelations.Value( m_iRequestingPlayer, m_iSentTo ) = EOSAIEnumForeignRelations::enum_Neutral;
		pState->m_ForeignRelations.Value( m_iSentTo, m_iRequestingPlayer ) = EOSAIEnumForeignRelations::enum_Neutral;
	}
	*/
}

//void CEOSAIPlayerInteraction_MadePeace::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void CEOSAIPlayerInteraction_MadePeace::UpdateForeignRelationsFeelings( long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
			//CGlobalForeignRelations* pForeignRelations )
{
	//ASSERT( pForeignRelations->Value( m_iPlayer1, m_iPlayer2 ) == EOSAIEnumForeignRelations::enum_War );
	//ASSERT( pForeignRelations->Value( m_iPlayer2, m_iPlayer1 ) == EOSAIEnumForeignRelations::enum_War );

	pFeelings->Value( m_iPlayer1, m_iPlayer2 ) += 0.1f;
	pFeelings->Value( m_iPlayer2, m_iPlayer1 ) += 0.1f;

	//pForeignRelations->Value( m_iPlayer1, m_iPlayer2 ) = EOSAIEnumForeignRelations::enum_Neutral;
	//pForeignRelations->Value( m_iPlayer2, m_iPlayer1 ) = EOSAIEnumForeignRelations::enum_Neutral;
}

//void CEOSAIPlayerInteraction_AskedForNonAgressionPact::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void CEOSAIPlayerInteraction_AskedForNonAgressionPact::UpdateForeignRelationsFeelings( long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
			//CGlobalForeignRelations* pForeignRelations )
{
	float fDelta = 0.0f;
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Accept  ){ fDelta = +0.1f; }
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Decline ){ fDelta =  0.0f; }
	if( m_eResponse == EOSAIEnumTradeAgreementResponse_Reject  ){ fDelta = -0.1f; }

	// Fade the weight with time
	fDelta = AdjustDeltaAccordingToTime( iCurrentTurn - m_iEventTurn, fDelta, NORMAL_DECAY_TIME );
	pFeelings->Value( m_iRequestingPlayer, m_iSentTo ) += fDelta;
}



#include "stdafx.h"
#include "EOSAIPlayerInteraction_CreatedAlliance.h"


void CEOSAIPlayerInteraction_CreatedAlliance::Serialize(CEOSAISerial* pSerial)
{
	pSerial->SerializeClassId(GetCEOSAISerialClassId()); // handles the 256 conversion for long

	pSerial->Serialize((char)2); // version
	pSerial->Serialize(m_iPlayer1);
	pSerial->Serialize(m_iPlayer2);

	pSerial->Serialize(m_iEventTurn); // Version 2
}

void CEOSAIPlayerInteraction_CreatedAlliance::Deserialize(CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc)
{
	pSerial->ExtractAndVerifyClassId(GetCEOSAISerialClassId());

	char iVersion = pSerial->ExtractChar();
	pSerial->Deserialize(m_iPlayer1);
	pSerial->Deserialize(m_iPlayer2);

	if (iVersion >= 2)
	{
		pSerial->Deserialize(m_iEventTurn); // Version 2
	}
}

//void CEOSAIPlayerInteraction_CreatedAlliance::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void CEOSAIPlayerInteraction_CreatedAlliance::UpdateForeignRelationsState(
	long iCurrentTurn,
	CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
	CEOSAIBCDumbArray2D< float >* pFeelings)
	//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
	//float                Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] )
	//CGlobalForeignRelations* pForeignRelations )
{
	pForeignRelations->Value(m_iPlayer1, m_iPlayer2) = EOSAIEnumForeignRelations::enum_Alliance;
	pForeignRelations->Value(m_iPlayer2, m_iPlayer1) = EOSAIEnumForeignRelations::enum_Alliance;

	//ASSERT( pForeignRelations->Value( m_iPlayer1, m_iPlayer2 ) != EOSAIEnumForeignRelations::enum_Alliance );
	//ASSERT( pForeignRelations->Value( m_iPlayer2, m_iPlayer1 ) != EOSAIEnumForeignRelations::enum_Alliance );
	//ASSERT( pForeignRelations->Value( m_iPlayer1, m_iPlayer2 ) != EOSAIEnumForeignRelations::enum_Teammate );
	//ASSERT( pForeignRelations->Value( m_iPlayer2, m_iPlayer1 ) != EOSAIEnumForeignRelations::enum_Teammate );

	// Fade the weight with time?
	pFeelings->Value(m_iPlayer1, m_iPlayer2) += 0.2f;
	pFeelings->Value(m_iPlayer2, m_iPlayer1) += 0.2f;

	//pForeignRelations->Value( m_iPlayer1, m_iPlayer2 ) = EOSAIEnumForeignRelations::enum_Alliance;
	//pForeignRelations->Value( m_iPlayer2, m_iPlayer1 ) = EOSAIEnumForeignRelations::enum_Alliance;
}


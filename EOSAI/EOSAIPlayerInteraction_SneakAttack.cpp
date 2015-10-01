
#include "stdafx.h"
#include "EOSAIPlayerInteraction_SneakAttack.h"
#include "EOSAISerial.h"
#include "EOSAIMathFunction.h"

void CEOSAIPlayerInteraction_SneakAttack::Serialize( CEOSAISerial* pSerial )
{
	pSerial->SerializeClassId( GetCEOSAISerialClassId() ); // handles the 256 conversion for long

	pSerial->Serialize( (char) 2 ); // version
	pSerial->Serialize( m_iEventTurn );
	pSerial->Serialize( m_iActor );
	pSerial->Serialize( m_iTarget );
}

void CEOSAIPlayerInteraction_SneakAttack::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	pSerial->ExtractAndVerifyClassId( GetCEOSAISerialClassId() );

	char iVersion = pSerial->ExtractChar();
	if( iVersion == 1 )
	{
		m_iEventTurn = 1;
	}
	if( iVersion == 2 )
	{
		pSerial->Deserialize( m_iEventTurn );
	}
	pSerial->Deserialize( m_iActor );
	pSerial->Deserialize( m_iTarget );
}

void CEOSAIPlayerInteraction_SneakAttack::UpdateForeignRelationsFeelings(
			long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
{
	// Fade the weight with time
	//fDelta = AdjustDeltaAccordingToTime( iCurrentTurn - m_iEventTurn, fDelta, NORMAL_DECAY_TIME );
	//pFeelings->Value( m_iRequestingPlayer, m_iSentTo ) += fDelta;

	//ASSERT( pForeignRelations->Value( m_iDefendingPlayer, m_iAttackingPlayer ) != EOSAIEnumForeignRelations::enum_War );
	//ASSERT( pForeignRelations->Value( m_iAttackingPlayer, m_iDefendingPlayer ) != EOSAIEnumForeignRelations::enum_War );

	//CEOSAIMathFunction  FeelingsIO;
	CEOSAIMathFunction  NewFeelingsFromOldFeelingsFunc;
	NewFeelingsFromOldFeelingsFunc.SetInputOutput( 0.0f,-0.5f );
	NewFeelingsFromOldFeelingsFunc.SetInputOutput( 0.5f,-0.3f );
	NewFeelingsFromOldFeelingsFunc.SetInputOutput( 1.0f,-0.5f );

	float fFeeling;
	fFeeling = pFeelings->Value( m_iTarget, m_iActor );
	pFeelings->Value( m_iTarget, m_iActor ) = NewFeelingsFromOldFeelingsFunc.GetOutput( fFeeling );
	/*
	float fFeeling;
	fFeeling = pFeelings->Value( m_iDefendingPlayer, m_iAttackingPlayer );
	fFeeling -= 0.1f; // this gets cancelled when peace is declared
	if( fFeeling > 0.5f ){ fFeeling = 0.5f; }
	if( fFeeling > 0.0f ){ fFeeling = 0.2f * fFeeling; }
	pFeelings->Value( m_iTarget, m_iActor ) = fFeeling; // maximum value is 0.25
	*/

	fFeeling = pFeelings->Value( m_iActor, m_iTarget );
	fFeeling -= 0.2f; // this gets cancelled when peace is declared
	if( fFeeling > 0.5f ){ fFeeling = 0.5f; }
	if( fFeeling > 0.0f ){ fFeeling = 0.5f * fFeeling; }
	pFeelings->Value( m_iActor, m_iTarget ) = fFeeling; // maximum value is 0.25

	//pForeignRelations->Value( m_iTarget, m_iActor ) = EOSAIEnumForeignRelations::enum_War;
	//pForeignRelations->Value( m_iActor, m_iTarget ) = EOSAIEnumForeignRelations::enum_War;
}


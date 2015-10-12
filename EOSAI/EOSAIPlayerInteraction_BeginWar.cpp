
#include "stdafx.h"
#include "EOSAIPlayerInteraction_BeginWar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIPlayerInteraction_BeginWar::Serialize( CEOSAISerial* pSerial )
{
	pSerial->SerializeClassId( GetCEOSAISerialClassId() ); // handles the 256 conversion for long

	pSerial->Serialize( (char) 1 ); // version
	pSerial->Serialize( m_iEventTurn );
	pSerial->Serialize( m_iActor );
	pSerial->Serialize( m_iTarget );
}

void CEOSAIPlayerInteraction_BeginWar::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	pSerial->ExtractAndVerifyClassId( GetCEOSAISerialClassId() );

	char i = pSerial->ExtractChar();
	if( i == 1 )
	{
		pSerial->Deserialize( m_iEventTurn );
		pSerial->Deserialize( m_iActor );
		pSerial->Deserialize( m_iTarget );
	}
}

//void CEOSAIPlayerInteraction_DeclaredWar::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void CEOSAIPlayerInteraction_BeginWar::UpdateForeignRelationsState(
			long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
{
	pForeignRelations->Value(m_iTarget, m_iActor) = EOSAIEnumForeignRelations::enum_War;
	pForeignRelations->Value(m_iActor, m_iTarget) = EOSAIEnumForeignRelations::enum_War;

	//ASSERT( pForeignRelations->Value( m_iTarget, m_iActor ) != EOSAIEnumForeignRelations::enum_War );
	//ASSERT( pForeignRelations->Value( m_iActor, m_iTarget ) != EOSAIEnumForeignRelations::enum_War );

	float fFeeling;
	fFeeling = pFeelings->Value( m_iTarget, m_iActor );
	fFeeling -= 0.1f; // this gets cancelled when peace is declared
	if( fFeeling > 0.5f ){ fFeeling = 0.5f; }
	if( fFeeling > 0.0f ){ fFeeling = 0.5f * fFeeling; }
	pFeelings->Value( m_iTarget, m_iActor ) = fFeeling; // maximum value is 0.25

	fFeeling = pFeelings->Value( m_iActor, m_iTarget );
	fFeeling -= 0.1f; // this gets cancelled when peace is declared
	if( fFeeling > 0.5f ){ fFeeling = 0.5f; }
	if( fFeeling > 0.0f ){ fFeeling = 0.5f * fFeeling; }
	pFeelings->Value( m_iActor, m_iTarget ) = fFeeling; // maximum value is 0.25

	//pForeignRelations->Value( m_iTarget, m_iActor ) = EOSAIEnumForeignRelations::enum_War;
	//pForeignRelations->Value( m_iActor, m_iTarget ) = EOSAIEnumForeignRelations::enum_War;
}



#include "stdafx.h"
#include "EOSAIPlayerInteraction_Combat.h"
#include "EOSAISerial.h"

void CEOSAIPlayerInteraction_Combat::Serialize( CEOSAISerial* pSerial )
{
	pSerial->SerializeClassId( GetCEOSAISerialClassId() ); // handles the 256 conversion for long

	pSerial->Serialize( (char) 1 ); // version
	pSerial->Serialize( m_iAttacker );
	pSerial->Serialize( m_iTarget );
	pSerial->Serialize( m_iCitResLoss );
	pSerial->Serialize( m_iDestroyedProductionValue );
}

void CEOSAIPlayerInteraction_Combat::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	pSerial->ExtractAndVerifyClassId( GetCEOSAISerialClassId() );

	char iVersion = pSerial->ExtractChar();
	pSerial->Deserialize( m_iAttacker );
	pSerial->Deserialize( m_iTarget );
	pSerial->Deserialize( m_iCitResLoss );
	pSerial->Deserialize( m_iDestroyedProductionValue );
}

//void CEOSAIPlayerInteraction_Combat::UpdateForeignRelationFeelings( CEOSAIForeignRelationsSituation* pSituation )
//void CEOSAIPlayerInteraction_Combat::UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState )
void CEOSAIPlayerInteraction_Combat::UpdateForeignRelationsState( long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings )
{
	float fDelta = -( m_iCitResLoss/20.0f + m_iDestroyedProductionValue/200.0f );

	// Fade the weight with time
	fDelta = AdjustDeltaAccordingToTime( iCurrentTurn - m_iEventTurn, fDelta, NORMAL_DECAY_TIME );
	pFeelings->Value( m_iTarget, m_iAttacker ) += fDelta;
}


#include "stdafx.h"
#include "EOSAIPlayerInteraction_CreatedTeam.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void CEOSAIPlayerInteraction_CreatedTeam::Serialize( CEOSAISerial* pSerial )
{
	pSerial->SerializeClassId( GetCEOSAISerialClassId() ); // handles the 256 conversion for long

	pSerial->Serialize( (char) 1 ); // version
	pSerial->Serialize( m_iEventTurn );
	m_Players.Serialize( pSerial );
}

void CEOSAIPlayerInteraction_CreatedTeam::Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc )
{
	pSerial->ExtractAndVerifyClassId( GetCEOSAISerialClassId() );

	char i = pSerial->ExtractChar();
	if( i == 1 )
	{
		pSerial->Deserialize( m_iEventTurn );
		m_Players.Deserialize( pSerial );
	}
}

void CEOSAIPlayerInteraction_CreatedTeam::UpdateForeignRelationsFeelings(
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
				//pForeignRelations->Value( iPlayerA, iPlayerB ) = EOSAIEnumForeignRelations::enum_Teammate;
			}
		}
	}
}


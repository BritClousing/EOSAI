#pragma once

#include "EOSAIPublic.h"
#include "EOSAIPlayerInteraction.h"

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIPlayerInteraction_CreatedAlliance : public CEOSAIPlayerInteraction
{
public:
	CEOSAIPlayerInteraction_CreatedAlliance() { m_iPlayer1 = 0; m_iPlayer2 = 0; }
	CEOSAIPlayerInteraction_CreatedAlliance(long iPlayer1, long iPlayer2) { m_iPlayer1 = iPlayer1; m_iPlayer2 = iPlayer2; }

	//
	CEOSAISerial_INFORMATION_LONG(205, CEOSAIPlayerInteraction_CreatedAlliance);
	virtual void Serialize(CEOSAISerial* pSerial);
	virtual void Deserialize(CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc);

	virtual bool ValidateValues() { return(m_iPlayer1 > 0 && m_iPlayer2 > 0 && m_iEventTurn != -1 && m_bEveryoneKnowsAboutThisInteraction == true); };

	//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
	virtual void UpdateForeignRelationsState(
		long iCurrentTurn,
		CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
		CEOSAIBCDumbArray2D< float >* pFeelings);
	//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
	//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
	//CGlobalForeignRelations* pForeignRelations );

	long  m_iPlayer1;
	long  m_iPlayer2;
};


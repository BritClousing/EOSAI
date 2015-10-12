
#pragma once

#include "EOSAIPlayerInteraction.h"

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIPlayerInteraction_DeclaredWar : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_DeclaredWar(){ m_iActor = 0; m_iTarget = 0; }
		CEOSAIPlayerInteraction_DeclaredWar( long iActor, long iTarget ){ m_iActor = iActor; m_iTarget = iTarget; }
		//
		CEOSAISerial_INFORMATION_LONG( 200, CEOSAIPlayerInteraction_DeclaredWar );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );
		//
		// Make sure all the necessary values are set
		virtual bool ValidateValues() { return(m_iActor > 0 && m_iTarget > 0 && m_iEventTurn != -1 && m_bEveryoneKnowsAboutThisInteraction == true); };
		//
		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );
		//virtual void UpdateForeignRelationsState( long iCurrentTurn, 
		//				EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1], 
		//				float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
		long  m_iActor = 0;
		long  m_iTarget = 0;
};

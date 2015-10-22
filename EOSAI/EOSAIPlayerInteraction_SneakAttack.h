
#pragma once

#include "EOSAIPlayerInteraction.h"
#include "EOSAIBCDumbArray2D.h"
class CEOSAISerial;

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIPlayerInteraction_SneakAttack : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_SneakAttack()
		{
			m_iActor = 0;
			m_iTarget = 0;
			//m_iAttackingPlayer = 0;
			//m_iDefendingPlayer = 0;
		}

		//
		CEOSAISerial_INFORMATION_LONG( 207, CEOSAIPlayerInteraction_SneakAttack );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );

		virtual bool ValidateValues() { return(m_iActor > 0 && m_iTarget > 0 && m_iEventTurn != -1 && m_bEveryoneKnowsAboutThisInteraction == true); };

		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, 
						CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, 
						CEOSAIBCDumbArray2D< float >* pFeelings );
						//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
						//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
						//CGlobalForeignRelations* pForeignRelations );

		long  m_iActor;
		long  m_iTarget;
		//long  m_iAttackingPlayer;
		//long  m_iDefendingPlayer;
};

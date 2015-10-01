
#pragma once
#include "EOSAIPlayerInteraction.h"


class CEOSAIPlayerInteraction_Combat : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_Combat()
		{
			m_iAttacker = 0;
			m_iTarget = 0;
			m_iCitResLoss = 0;
			m_iDestroyedProductionValue = 0; // Units, Buildings, Airfields, ...
		}

		//
		CEOSAISerial_INFORMATION_LONG( 208, CEOSAIPlayerInteraction_Combat );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );

		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsFeelings( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		long  m_iAttacker;
		long  m_iTarget;
		long  m_iCitResLoss;  // Cities, Resources captured by iPlayer[1]
		long  m_iDestroyedProductionValue; // Destroyed Units, Buildings, Airfields, ...
};
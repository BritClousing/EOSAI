
#pragma once

#include "EOSAIPlayerInteraction.h"

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

//
// Similar to "DeclaredWar" except that it doesn't assign blame.
//   This is used in scenarios when nations are setup as "at war".
//

class DLLIMPEXP CEOSAIPlayerInteraction_BeginWar : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_BeginWar(){ m_iActor = 0; m_iTarget = 0; }
		CEOSAIPlayerInteraction_BeginWar( long iActor, long iTarget ){ m_iActor = iActor; m_iTarget = iTarget; }
		//
		CEOSAISerial_INFORMATION_LONG( 201, CEOSAIPlayerInteraction_BeginWar );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );
		//
		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );
		long  m_iActor;
		long  m_iTarget;
};

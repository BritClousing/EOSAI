
#pragma once

#include "EOSAIPlayerInteraction.h"

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIPlayerInteraction_CreatedTeam : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_CreatedTeam(){}
		CEOSAIPlayerInteraction_CreatedTeam( long iPlayer1, long iPlayer2 ){ m_Players.Add( iPlayer1 ); m_Players.Add( iPlayer2 ); }
		CEOSAIPlayerInteraction_CreatedTeam( long iPlayer1, long iPlayer2, long iPlayer3 ){ m_Players.Add( iPlayer1 ); m_Players.Add( iPlayer2 ); m_Players.Add( iPlayer3 ); }
		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		//
		CEOSAISerial_INFORMATION_LONG( 202, CEOSAIPlayerInteraction_CreatedTeam );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );
		//
		virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );
		CEOSAIIntSet  m_Players;
};

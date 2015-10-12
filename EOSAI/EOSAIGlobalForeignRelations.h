
#ifndef EOSAI_GLOBAL_FOREIGN_RELATIONS_H
#define EOSAI_GLOBAL_FOREIGN_RELATIONS_H

//
// This class is used to describe the foreign relations between players
//
#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIEnumForeignRelations.h"
#include "EOSAIBCDumbArray2D.h"
#include "EOSAISettings.h"
class CEOSAIPlayerInteractionEvent;
class CPlayerTeam;
class CEOSAIIntSet;

class DLLIMPEXP CEOSAIGlobalForeignRelations
{
	public:
		CEOSAIGlobalForeignRelations();
		CEOSAIGlobalForeignRelations(CEOSAIGlobalForeignRelations& rel);
		~CEOSAIGlobalForeignRelations();
		//
		//void  Serialize( CEOSAISerial* pSerial );
		//void  Deserialize( CEOSAISerial* pSerial );

		CString  OutputDebugString();

		///void  SetWorldDesc( CWorldDesc* pWorldDesc ){ m_pWorldDesc = pWorldDesc; }
		//void  SetNumberOfPlayers( long iNumberOfPlayers );
		void  Copy( CEOSAIGlobalForeignRelations* pGlobalForeignRelations );
		//void  CopyFeelings( CEOSAIBCDumbArray2D< float >* pFeelings );
		//void  CopyRelations( CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pRelations );

		//
		//CPlayerForeignRelations*  GetPlayerForeignRelations( long iPlayer );
		CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* GetForeignRelations(){ return &m_ForeignRelations; } // Team, Alliance, Neutral, War, etc
		void                    SetForeignRelations( CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pRelationsArray );
		void                    SetForeignRelations( long iActor, long iTarget, EOSAIEnumForeignRelations eRel );
		EOSAIEnumForeignRelations  GetForeignRelations( long iActor, long iTarget );

		void   ResetFeelings();  // reset everyone back to 0.5
		void   SetAllPlayersToWar();
		CEOSAIBCDumbArray2D< float >*  GetFeelings(){ return &m_Feelings; }
		void   SetFeelings( CEOSAIBCDumbArray2D< float >* pFeelingsArray );//{ m_Feelings.Value( iActor, iTarget ) = f; }
		void   SetFeelings( long iActor, long iTarget, float f );//{ m_Feelings.Value( iActor, iTarget ) = f; }
		float  GetRawFeelings( long iActor, long iTarget );//{ return m_Feelings.Value( iActor, iTarget ); }
		float  GetFeelings01( long iActor, long iTarget );
/*
		//
		void          AddTeam( CPlayerTeam* pTeam );
		//void          AddApprovedTeam( CString strTeamId, CString strTeam, CEOSAIIntSet* pPlayers );
		CPlayerTeam*  GetTeam( CString strTeamName );
		CPlayerTeam*  GetTeamByTeamId( CString strTeamId );
		//CPlayerTeam*  GetTeam( CString strTeam );
		void          UpdateForeignRelationsBasedOnTeams();
		void          UpdateForeignRelationsBasedOnTeam( CString strTeamId );
		void          UpdateForeignRelationsBasedOnTeam( CPlayerTeam* pTeam );
		bool          PlayersAreInATeamTogether( long iPlayer1, long iPlayer2 );
		//
		CPlayerTeam*  AddSuggestedTeam( CString strTeamId, CString strTeam, long iSuggestedOnTurn, CEOSAIIntSet* pPlayers, long iApprovedPlayer );
		void          PlayerHasApprovedTeam( long iPlayer, CString strTeamId, CString strTeamValidation );
		void          PlayerHasDisapprovedTeam( long iPlayer, CString strTeamId, CString strTeamValidation );
		//
		long     GetNumberOfApprovedTeams();
		long     GetNumberOfTeamsPlayerIsPartOf( long iPlayer );
		CString  GetTeamsPlayerIsPartOf( long iPlayer );

		//void          DeleteTeam( CString strTeam );

		// Players
		//CWorldDesc*  m_pWorldDesc;
		//long         m_iNumberOfPlayers;
		//CPlayerForeignRelations*  m_Players[MAX_NUMBER_OF_PLAYERS+1];
		//
		CList< CPlayerTeam* >     m_Teams;
		//CList< CPlayerTeam* >     m_SuggestedTeams;

		//EOSAIEnumForeignRelations  m_ForeignRelations[ MAX_NUMBER_OF_PLAYERS+1 ][ MAX_NUMBER_OF_PLAYERS+1 ];
		//float                 m_Feelings[ MAX_NUMBER_OF_PLAYERS+1 ][ MAX_NUMBER_OF_PLAYERS+1 ];
*/
		//
		bool  IsSendingOngoingSendMap( long iPlayer1, long iPlayer2 );
		//bool  HasNonAggressionPact( long iPlayer1, long iPlayer2 );
		long  GetEndOfNonAggressionPact( long iPlayer1, long iPlayer2 ); // 0 = no agreement
		bool  GetHasOngoingTrade( long iPlayer1, long iPlayer2 );
		bool  GetHasOngoingTrade_ResourceTrade( long iPlayer1, long iPlayer2 );

	private:
		CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >  m_ForeignRelations; // Team, Alliance, Neutral, War, etc
		CEOSAIBCDumbArray2D< float >                      m_Feelings; //

		// Event information - server-only (used by the AI for feelings calculations)
		//
			/*
			// Trade
			void  AddEvent_Trade( long iActor, long iTargetPlayer, CGameTime Time, float fSignificance );
			// War
			void  AddEvent_DeclarationOfWar( long iActor, long iTargetPlayer, CGameTime Time );
			void  AddEvent_SneakAttack( long iActor, long iTargetPlayer, CGameTime Time );
			void  AddEvent_CombatEvent( long iActor, long iTargetPlayer, CGameTime Time, float fSignificance );
			// Peace
			void  AddEvent_PeaceTreaty( long iActor, long iTargetPlayer, CGameTime Time );
			void  AddEvent_NonAggressionPact( long iActor, long iTargetPlayer, CGameTime Time, long iDuration = 1000000 );
			void  AddEvent_TemporaryAlliance( long iActor, long iTargetPlayer, CGameTime Time, long iDuration = 1000000 );
			void  AddEvent_PermanentAlliance( long iActor, long iTargetPlayer, CGameTime Time );

			bool  WarStateExists( long iActor, long iTargetPlayer );
			void  EndWarState( long iActor, long iTargetPlayer, CGameTime Time );
			bool  PermanentAllianceStateExists( long iActor, long iTargetPlayer, CGameTime Time );
			*/

			//void  ProcessHistoricalEvents( CGameTime CurrentTime );

			//float GetPlayerFeelings( long iPlayer, long iFeelingsTowardsPlayer );

			// History
			//CGameTime  m_CurrentTime; // Used when inserting new events
			//CList< CEOSAIPlayerInteractionEvent* >  m_PlayerInteractionHistory;
};

#endif EOSAI_GLOBAL_FOREIGN_RELATIONS_H

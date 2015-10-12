
#pragma once
#include "EOSAIEnumForeignRelations.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAIBCDumbArray2D.h"
class CEOSAIStrategicAI;
class CEOSAIIntSet;

//
// The AIForeignRelationsHypotheticalState describes the state of the foreign relations
//    for the minmax algorithm.  It is a node on a tree describing all possible foreign relations.
//

class CEOSAIForeignRelationsState
{
	public:

		enum EnumRelationsPlan
		{
			enum_None,
			enum_ContinueWar,
			enum_StartWar, // Declare or sneak attack
			enum_PlanWar,
			enum_Neutral,
			enum_MakePeace, // if I'm currently at war
			enum_MaintainPeace,
		};

		//

		CEOSAIForeignRelationsState();
		void  SetStrategicAI( CEOSAIStrategicAI* pStrategicAI ){ m_pStrategicAI = pStrategicAI; }
		void  Copy( CEOSAIForeignRelationsState* pOther );
		void  SetNumberOfPlayers( long iNumberOfPlayers );

		void     ClearValues();
		CString  OutputDebugString();

		//void  RunCalculations();

		void  SetFeelings( long iPlayer1, long iPlayer2, float fValue );
		void  AddFeelings( long iPlayer1, long iPlayer2, float fDelta );
		float GetFeelings( long iPlayer1, long iPlayer2 );
		void  LimitFeelings01();
		float GetPlayerAccessibility( long iPlayer1, long iPlayer2 );
		//

		void  GetPlayerTeammates( long iPlayer, CEOSAIIntSet* pPlayerTeammates );
		void  GetPlayerAllies( long iPlayer, CEOSAIIntSet* pPlayerAllies );
		void  GetPlayerEnemies( long iPlayer, CEOSAIIntSet* pPlayerEnemies );

		//float GetPowerOfPlayerAndTeammates( long iPlayer );
		//float GetPowerOfPlayerTeammatesAllies( long iPlayer );
		//float GetPowerOfPlayerEnemies( long iPlayer );
		// SATE = SelfAllyTeamEnemy
		float GetSATEPowerBalanceBasedOnMatrix( long iPlayer ); // -infintity to +infinity
		float GetSATEPowerBalanceBasedOnMatrix01( long iPlayer ); // 0 to 1.  1 = no enemies, 0.5 = equal power with enemies, 0 = overwhelmed
		//
		float GetSATPower01( long iPlayer );
		float GetGlobalPower();
		//
		long  GetNumberOfDirectEnemies( long iPlayer );
		long  GetNumberOfDirectAndIndirectEnemies( long iPlayer );
		//
		//void  GetFeeling_PlayerIsGettingTooPowerful( long iActor, CEOSAIBCDumbArray1D< float >* pPlayerIsGettingTooPowerful );
		//void  GetDesireToWeakenPlayer( long iActor, CEOSAIBCDumbArray1D< float >* pDesireToOppose ); // Important for forming alliances against powerful players
		//void  GetDesireToTeamWithPlayer( long iActor, CEOSAIBCDumbArray1D< float >* pDesireToOppose );
		//
		// The Score of a particular player based on the current alliance/war state
		//   This helps to guide the player down certain transitions
		float GetPlayerExpansion( long iPlayer );
		float GetPositionalScore( long iPlayer ); 
		//
		long  GetPlayerIWouldMostLikeToAttackBasedOnRawWarDesire( long iPlayer );

	//private:

		//
		void  ApplyHistoricalEventsToFeelings();
		void  CalculateExpectedUnownedExpansion();
		void  CalculatePlayerRelationsMatrix();
		void  CalculateSATGlobalPower();
		float GetGeneralDesireForWar01( long iPlayer ){ return m_GeneralWarDesire.Value( iPlayer ); }
		void  CalculateRawWarDesire();
		//
		void  CalculateGeneralWarDesire();
		void  NormalizeWarDesires();
		void  CalculateFinalWarDesiresUsingPlayerMatrix();
		//
		void  GetTopTarget( long iPlayer, long* piTopTarget, float* pfValue );
		//
		void  CalculateRawPursueFriendlyRelations01();
		void  CalculateCurrentStances();
		void  CalculateDesireToConcedeGame();

		float GetConcernThatPlayerIsTooPowerful( long iPlayer );

		float GetUnownedCitResAccessibility01( long iPlayer );
		float GetCombatSignificance( long iPlayer );
		float GetPower( long iPlayer );
		float GetUnitMaintenanceCostsPercentage( long iPlayer );

		CEOSAIStrategicAI*  m_pStrategicAI;
		long  m_iNumberOfPlayers;

	// Initial Values
	//
		bool  m_bExpectedUnownedExpansion01;
		float m_fDesireToConcedeGame; // 0.99 or higher = wants to resign
		bool  GetDesireToConcedeGame(){ return m_fDesireToConcedeGame > 0.99f; }

		CEOSAIBCDumbArray2D< float >  m_Feelings01;      // 0.0 to 1.0 (0 = dislike, 1 = like)
		CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >  m_ForeignRelations; // Team, Alliance, Neutral, War, etc
		CEOSAIBCDumbArray2D< float >  m_PlayerRelationsMatrix; // Foreign relations (first/second degree) in numbers
		//
		CEOSAIBCDumbArray1D< float >  m_GeneralWarDesire;
	//
	// Result
	//
		CEOSAIBCDumbArray2D< float >  m_PlayerPowerAllocationMatrix;
		CEOSAIBCDumbArray2D< float >  m_RawWarDesire; // PlayerX's desire for war with PlayerY (pretending no wars are going on)
		CEOSAIBCDumbArray2D< float >  m_NormalizedWarDesire; //
		CEOSAIBCDumbArray2D< float >  m_FinalWarDesire;
		//CEOSAIBCDumbArray1D< long  >  m_TopWarTarget;
		//CEOSAIBCDumbArray1D< float >  m_TopWarTargetValue;
		//
		CEOSAIBCDumbArray2D< float >  m_PursueFriendlyRelations;

		// ExpectedConquest11: based on enemies, their accessibility, power-balance, etc
		// -0.9 = enemy is 10x as powerful as me
		// -0.5 = enemy is 2x as powerful as me
		// -0.1 = enemy is 1.1x as powerful as me
		//  0.0 = no enemies, or equal power to enemies
		//  0.5 =  2x as powerful as enemies
		//  0.9 = 10x as powerful as enemies
		CEOSAIBCDumbArray1D< float >  m_ExpectedConquest11;
		CEOSAIBCDumbArray1D< float >  m_ExpectedUnownedExpansion01;
		CEOSAIBCDumbArray1D< float >  m_SATGlobalPower01; // Power of the player, including teammates and allies

		CEOSAIBCDumbArray2D< EnumRelationsPlan >  m_RelationsPlan;
};

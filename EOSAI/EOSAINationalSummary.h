
#pragma once

#include "EOSAILongSet.h"
#include "EOSAICitResSummary.h"
#include "EOSAIUnitSummary.h"
#include "EOSAIEnumForeignRelations.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAIListSort.h"
//class CPlayer;
namespace EOSAI
{
	class CAINationalAlliedAndEnemyPower;
};
//class CWorldDescServer;
class CEOSAINationalSummary;
class CEOSAIForeignRelationsSituation;
#define EOSAI_MAX_NUMBER_OF_PLAYERS 10

/*
	This is the AI's perception of a player's current and future military power.
	This includes a perception of the AI's own power - used for comparison and
	  calculations.
*/
namespace EOSAI
{
class CPositionTowardsOtherPlayer
{
	public:
		CPositionTowardsOtherPlayer()
		{
			m_pNationalSummary = NULL;
			m_iTargetPlayer = 0;

			// Based on history
			ResetFeelings();

			// Level 1 thoughts/values
			m_fAccessibility = 0.0f;
			m_fPlayerIsGettingTooPowerful = 0.0f;
			m_fPlayerIsEasyPrey = 0.0f;
			m_fSoloPowerRatio = 0.0f;
			m_fLevel1DesireToAttackPlayer = 0.0f;
			m_fThisPlayersL1DesireToAttackMe = 0.0f;

			// Allied/Enemy values
			m_fTargetPlayersCurrentPowerBalanceExcludingMyContribution = 0.0f;
			m_fLevel3DesireToAttackPlayer = 0.0f;
			m_fValueAsAnAlly = 0.0f;
		}

		void ResetFeelings(){ m_fFeelings = 0.5f; m_fFeelingsWeight = 1.0f; }
		void AdjustFeelings( float fValue, float fWeight );

		//
		CEOSAINationalSummary* m_pNationalSummary;
		long  m_iTargetPlayer;

		// Initial values (based on history)
		float m_fFeelings; // Based on past experience.  0 = bad, 1 = good
		float m_fFeelingsWeight;  // Used while adjusting feelings

		// m_fAccessibility of 1.0  if the player is 0-200 pixels away
		// m_fAccessibility of 0.67 if the player is   400 pixels away
		// m_fAccessibility of 0.5  if the player is   600 pixels away
		// m_fAccessibility of 0.4  if the player is   800 pixels away
		// m_fAccessibility of 0.33 if the player is  1000 pixels away
		float m_fAccessibility; // 0 = bad, 1 = good

		// Level 1 thoughts/values - this ignores my current wars, allies, and power balance
		float m_fPlayerAccessibilityToWorld;  // 0 (no accessibility) to NumberOfPlayers
		float m_fPlayerIsGettingTooPowerful;
		float m_fPlayerIsEasyPrey;
		float m_fSoloPowerRatio; // 0.0-1.0.  Ratios near 0.0 mean I am weaker than he is
		float m_fLevel1DesireToAttackPlayer;
		float m_fThisPlayersL1DesireToAttackMe;

		// Level2 desires - includes other player's views, other player's desire to attack me back
		//float m_fOtherPlayersL1DesireToAttackPlayer;
		float GetLevel2DesireToAttackPlayer();//{ return m_fLevel1DesireToAttackPlayer + m_fThisPlayersL1DesireToAttackMe + 0.5f*m_fOtherPlayersL1DesireToAttackPlayer; }

		// Allied/Enemy values
		//float m_fTargetPlayerAndAlliesPowerExcludingMyContribution;
		//float m_fTargetEnemyPowerExcludingMyContribution;
		float m_fTargetPlayersCurrentPowerBalanceExcludingMyContribution;
		//float m_fMyPowerBalanceIfIAddedThisPlayerAsAnEnemy;
		//float m_fTargetPlayersPowerBalanceIfIAddedThisPlayerAsAnEnemy;
		float m_fLevel3DesireToAttackPlayer; // includes Level2 + alliance, war information

		float m_fValueAsAnAlly; // 0.0 = top potential enemy, 1.0 = lowest potential enemy score
};
};

class CEOSAINationalSummary
{
	public:
		//CAINationalSummary( CWorldDescServer* pWorldDescServer, CEOSAIForeignRelationsSituation* pAIForeignRelationsSituation, long iPlayer );
		CEOSAINationalSummary( CEOSAIForeignRelationsSituation* pAIForeignRelationsSituation, long iPlayer );
		~CEOSAINationalSummary();
		//
		EOSAI::CAINationalAlliedAndEnemyPower*  AllocateAINationalAlliedAndEnemyPowerObject();

		EOSAI::CPositionTowardsOtherPlayer*  GetPositionTowardsPlayer( long iPlayer );

		// 0 = would like peace/cease-fire, otherwise it returns the player number
		long  GetPotentialTarget( long i ); // if i = 1, get the top potential target
		CEOSAIListSort< EOSAI::CPositionTowardsOtherPlayer* >*  GetSortedListOfPotentialTargets();

		float GetPlayerProduction(){ return m_CitResSummary.m_fCityProduction; }

		CEOSAIForeignRelationsSituation* m_pAIForeignRelationsSituation;
		long  m_iPlayer;

		CEOSAICitResSummary  m_CitResSummary;
		CEOSAIUnitSummary    m_UnitSummary;

		EOSAIEnumForeignRelations  GetForeignRelations( long iOtherPlayer );
		EOSAIEnumForeignRelations  m_ForeignRelations[ EOSAI_MAX_NUMBER_OF_PLAYERS+1 ];

		private:

			void CalculateAccessibility( long iPlayer );

			void  SortPotentialTargets();
			CEOSAIListSort< EOSAI::CPositionTowardsOtherPlayer* >  m_SortedListOfPotentialTargets;

		public:
		//
		//
			// Player attributes
			float m_fPlayerCombatPower; // 0-infinity
			float m_fPlayerPower; // 0.0-1.0, value based on combat power, production, and adjusted world-production
			float m_fPlayerVsWorldPowerBalance; // 0.0-1.0 (percentage of world power)
			float m_fPlayerVsAveragePlayerPowerBalance; // value of 1.0 is the average player
			float m_fPlayerVsStrongestPlayerPower; // value of 1.0 is the strongest player
			long  m_iPlayerPowerPosition;  // 1 = single most powerful player, 2 = second most powerful, ...

			float m_fCurrentWarPowerSum;     // PlayerPower+AllyPower-EnemyPower (I could take accessibility into account)
			float m_fCurrentWarPowerBalance; // Player+Allies/Player+Allies+Enemies

			// Includes past alliances, wars, backstabbing, trade, ... (affects feelings and trust)
			CList< long >  m_ForeignRelationshipEvents;

			EOSAI::CAINationalAlliedAndEnemyPower*  m_pPower;

			// Desire for war is based on several factors:
			//   Player's military is getting worn down (this can be especially bad if there are lots of players)
			//   There are plenty of accessible unowned CitRes
			//   If I my resources are having a hard time supporting my military, reduce desire for peace
			//   If there are very few accessible unowned CitRes, increase desire for war
			//   If my combat power (percent) is larger than my production power (percent) then lean towards war
			//   If my combat power (percent) is smaller than my production power (percent) then lean towards peace
			float  m_fDesireForPeace;

			// Relationship attributes
			//CPositionTowardsOtherPlayer*  m_aPositionTowardsPlayer; // An array
			CEOSAIBCDumbArray1D< EOSAI::CPositionTowardsOtherPlayer >  m_aPositionTowardsPlayer;
			//CEOSAILongSet  m_PotentialAlliesSet;

			//
			float m_fOtherPlayersCombinedL1DesireToAttackMe;
			float m_fOtherPlayersCombinedL3DesireToAttackMe;

			//float m_fFeelingsTowardsPlayer[ MAX_NUMBER_OF_PLAYERS+1 ]; // 0 = bad, 1 = good
			//float m_fMyTrustOfPlayer[ MAX_NUMBER_OF_PLAYERS+1 ]; // How much do I trust this player?
			//float m_fAccessibilityToPlayer[ MAX_NUMBER_OF_PLAYERS+1 ]; // 0 = bad, 1 = good

			//float m_fDesireToAttackPlayer[ MAX_NUMBER_OF_PLAYERS+1 ]; // 0 = weak, 1 = strong
			//float m_fDesireForPeaceWithPlayer[ MAX_NUMBER_OF_PLAYERS+1 ]; // 0 = weak, 1 = strong
			//float m_fDesireForAllianceWithPlayer[ MAX_NUMBER_OF_PLAYERS+1 ]; // 0 = weak, 1 = strong

			/*
			// Allies/Enemies attributes
			float m_fCombatPowerOfAllies;
			float m_fCombatPowerOfEnemies;

			float m_fProductionOfAllies;
			float m_fProductionOfEnemies;

			// 1.0 = no enemies
			// 0.9 = enemies, but they are very weak compared to player and his allies
			// 0.5 = player and allies are roughly balanced with his enemies
			// 0.1 = player is much weaker than his enemies
			float m_fAlliedAndEnemyCombatPowerBalance; // 0.0-1.0
			float m_fAlliedAndEnemyCombatPowerSum; // Sum of MyPower + AlliesPower - EnemyPower

			float m_fAlliedAndEnemyProductionBalance; // 0.0-1.0
			float m_fAlliedAndEnemyProductionSum; // Sum of MyProduction + AlliesProduction - EnemyProduction

			float m_fPlayerAndAlliesVsWorldPowerBalance; // 0.0-1.0

			// DangerIndex is a scale for determining the danger of this player of winning the game
			//   Players who score high on this index are getting powerful.  The AI should
			//   look for ways to weaken them (through alliances, stealing their allies, etc)
			// The danger index is meant to be a single measure of danger (no need to compare
			//   danger indicies across nations, it already includes that).
			float m_fDangerIndex;
*/
		// Affected by past warfare and interactions (I need to track the individual events)
		//float m_fFeelingsTowardPlayer; // -1.0 = hostile, 1.0 = friendly

		// Combines my current war standing ("am I in over my head?") with my production power vs his
		// 0 = make allies, 0.5 = neutral, 1.0 = make war
		//float m_fProductionBasedDesireToFightThisPlayer; 

		// -1.0 = at bitter war, 0.0 = neutral, 1.0 = permanent alliance
		//float m_fHowMuchCanITrustThisPlayer;

		//float  m_fCurrentMilitaryPower;
		//float  m_fFutureMilitaryPower;  // say 100 turns from now
};


// This is used for thinking about shifting alliances
namespace EOSAI
{
class CAINationalAlliedAndEnemyPower
{
	public:
		CAINationalAlliedAndEnemyPower()
		{
			m_pNationalSummary = NULL;
			for( long i=0; i<EOSAI_MAX_NUMBER_OF_PLAYERS+1; i++ )
			{
				m_Relationship[i] = EOSAIEnumForeignRelations::enum_Undefined;
			}

			m_fThinkingPlayersContributionToAlliedAndEnemyPower = 0.0f;

			// Allies/Enemies attributes
			m_fAlliesCombatPower = 0.0f;
			m_fAlliesProduction = 0.0f;
			m_fAlliesPower = 0.0f;

			m_fEnemiesCombatPower = 0.0f;
			m_fEnemiesProduction = 0.0f;
			m_fEnemiesPower = 0.0f;

			//
			m_fPlayerAlliesAndEnemiesCombatPowerBalance = 0.0f; // 0.0-1.0 (1.0 means me+allies are powerful)
			m_fPlayerAlliesAndEnemiesCombatPowerSum = 0.0f; // Sum of MyPower + AlliesPower - EnemyPower

			m_fPlayerAlliesAndEnemiesProductionBalance = 0.0f; // 0.0-1.0 (1.0 means me+allies are powerful)
			m_fPlayerAlliesAndEnemiesProductionSum = 0.0f; // Sum of MyProduction + AlliesProduction - EnemyProduction

			m_fPlayerAndAlliesVsWorldPowerBalance = 0.0f; // 0.0-1.0 (me+allies as a percentage of world power)
			m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution = 0.0f;

			m_fDangerIndex = 0.0f;
		}

		// Reset the Foreign Relations values to their actual values
		//   (This function exists because we alter the foreign relations values for planning.)
		void SetForeignRelationsToActualValues(); 

		//
		CEOSAINationalSummary*  m_pNationalSummary;
		EOSAIEnumForeignRelations m_Relationship[ EOSAI_MAX_NUMBER_OF_PLAYERS+1 ];

		float GetPlayerAlliesAndEnemiesPowerBalance() // 0.0 - 1.0
		{
			return ( m_fPlayerAlliesAndEnemiesCombatPowerBalance + m_fPlayerAlliesAndEnemiesCombatPowerBalance ) / 2.0f;
		}

	//private:

		float m_fThinkingPlayersContributionToAlliedAndEnemyPower;

		// Allies attributes
		float m_fAlliesCombatPower;
		float m_fAlliesProduction;
		float m_fAlliesPower;

		// Allies/Enemies attributes
		float m_fEnemiesCombatPower;
		float m_fEnemiesProduction;
		float m_fEnemiesPower;

		// 1.0 = no enemies
		// 0.9 = enemies, but they are very weak compared to player and his allies
		// 0.5 = player and allies are roughly balanced with his enemies
		// 0.1 = player is much weaker than his enemies
		float m_fPlayerAlliesAndEnemiesCombatPowerBalance; // 0.0-1.0
		float m_fPlayerAlliesAndEnemiesCombatPowerSum; // Sum of MyPower + AlliesPower - EnemyPower

		float m_fPlayerAlliesAndEnemiesProductionBalance; // 0.0-1.0
		float m_fPlayerAlliesAndEnemiesProductionSum; // Sum of MyProduction + AlliesProduction - EnemyProduction

		float m_fPlayerAndAlliesVsWorldPowerBalance; // 0.0-1.0
		float m_fPlayerAndAlliesVsWorldPowerBalanceIgnoreAIPlayerContribution; // 0.0-1.0

		// DangerIndex is a scale for determining the danger of this player of winning the game
		//   Players who score high on this index are getting powerful.  The AI should
		//   look for ways to weaken them (through alliances, stealing their allies, etc)
		// The danger index is meant to be a single measure of danger (no need to compare
		//   danger indicies across nations, it already includes that).
		float m_fDangerIndex;
};
};

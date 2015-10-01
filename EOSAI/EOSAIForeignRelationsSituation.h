
#pragma once

#include "EOSAINationalSummary.h"
#include "EOSAIListSort.h"
#include <list>
namespace EOSAI
{
	class AIPlayer;
};
class CEOSAIBrain;

/*
	ForeignRelationsSituation does several tasks:

		Creates NationalSummaries for each nation
		Creates NationalAlliedAndEnemyPower objects for each nation
		Comes up with some ideas for ForeignRelations changes (alliances, wars, gang-up, etc)

	Overall situation:
		Lost - One player has overwhelming strength (70%+).  Probably just a matter of time.
		DangerousPlayer - One player has 40-70% of world power.  Other players should align.
		DuelingPlayers - Two powerful players fighting for the win.  Other players should align.
		DangerousGroup - One group has a disproportionate amount of world power.  
			Non-group players should align against them, with them,
			and attempt to steal away in-group players.


	//
	// Algorithm:
	//
	//   LEVEL 1: Calculate the foreign relations thoughts for all players about all players.
	//   This doesn't take into account the feasibility of attack because we don't know
	//     yet what player's will join us, form peace with us, whether they want to attack
	//     the player, too.
	//     + Player is getting too strong (consider attacking, and forming a coalition against them)
	//     + Player is accessible (accessible players make better targets)
	//     + Don't like player (want to attack players who I don't like)
	//     Normalize my attack desires then multiply by my combat capability
	//       I may be looking for someone to attack
	//     = Single desire to attack value
	//
	//   LEVEL 2: Consider other's players thoughts on these matters, think about adding allies
	//     based on common desires.  Increase desire to attack players who want to attack me
	//     (I'd rather attack the guy who doesn't like me / has no one else to attack than attack
	//     a third player and then have this guy attack me).
	//


	// Want to strengthen myself, weaken the other powerful players
	//   Maintain some constant relationship (alliance or war) to players,
	//   Avoid disloyal allies (whether they are disloyal to me or other players)
	//   Seek to retaliate against players disloyal to me
	// Accessibility:
	//   PlayerX has no accessibility to PlayerY:
	//     Alliance has little value to either player, except as a deterent to other players (double-team)
	//     War against between them isn't very dangerous
	//   PlayerX has high accessibility to PlayerY:
	//     Alliance has more value (worry less about invasion), can more easily double-team an enemy
	//     War between them is more dangerous

	//   PlayerX getting PlayerY to attack PlayerZ
	//     If PlayerX is on the defensive,
	//       Is valuable to PlayerX because it allows for a relief from attack
	//       PlayerX is on the defensive anyway, so he wasn't going to capture any new territory anyway
	//     If PlayerX is on the offensive,
	//       Can be valuable for weakening PlayerZ further
	//       Can be valuable to avoid overextending PlayerX (making him weak for other players)
	//       But, PlayerX might lose some territory to PlayerY that he might've captured for himself

	// What affects the desire to fight someone versus ally with them?
	//   - If there are plenty of unowned CitRes, this cuts down on the desire to fight
	//     - Although, it might be useful to do an early rush
	//   - Proximity to other player (does this favor being an ally or enemy?)
	//     - Closer promitity increases the chances of double-cross (both me double-crossing him and vice-versa)
	//       So distant allies are "safer", but offer less opportunity for me to double-cross him.
	//     - Enemies who are far away aren't useful enemies because you can't capture their territory
	//   - Trade (if I need to get oil and they have extra oil, I can setup a trade relationship)
	//   - Feelings (which includes past warfare)
	//   - My Production/Military power versus Other Player's Production/Military power
	//   - My current combat situation (am I already in over my head?)
	//   - If I have reached the limits of my expansion, increase combat desire and look for an enemy
	//     - This is especially true if the other player might not have reached their expansion limit
	//         It's better to start a war now than wait until they capture the easy unowned territory.
	//   - If the player is already at war, it might be good to jump in and grab a chunk of territory
	//   - The shape of the proximity affects things too 
	//       (e.g. if i barely touch one player, but share a decent border with a second player, I might
	//        want to go after the second player so I don't end up with a stretched empire, which is hard
	//        to defend.  The AI should prefer rounder empires, preferably in a map corner or edge.)
	//   - Try to avoid allying with players much stronger than me (this can be ignored if I need
	//       to form an alliance to oppose a third, even stronger player.
	//  Note: The AI should start to pre-plan unit positions and production based on who I am going 
	//     to attack.  I might want to move ground units and air to an island to serve as a defended
	//     forward base.

	//
	// There are several levels of player power:
	//   Weak: (might become allies with him, but he won't be very valuable, and don't let
	//     him suck you into a war - he'll probably draw wars to him).  May want to backstab him,
	//     may be able to keep him around as essentially a vassal state until all the other
	//     players have been eliminated.  (No reason to waste your units and his capturing
	//     his country when he's perfectly willing to do what you want and fight a common enemy.)
	//   Moderate: a decent ally, a respectable enemy
	//   Strong: a strong ally, dangerous enemy.  Won't want to be enemies with him unless
	//     you have backup.  As an ally, you have to be careful not to allow him to gain
	//     too much more power.  Want to increase power more quickly than he does.
	//   V.Strong: Makes a bad ally because he's going to run over you once you help him
	//     eliminate the other players.  Best strategy is to form an alliance with the other
	//     players against him.  Hope that other players take the brunt of his attacks.
	//   Overwhelming: so powerful that he already overpowers all the other players combined.
	//     The game is essentially over.  Might as well form an alliance with everyone else
	//     and make a go at it, maybe you'll get lucky.
	//

	// The way this algorithm works:
	//   Figure out how much I want to attack each player.
	//     This is based on the player's strength, player's alliance, geographical accessibility, ..
	//   (What about forming alliances with players? What about asking another player to gang
	//    up on a player? Need to calculate how much the other players want to fight the other
	//    players, too.)  Also, players can't just end wars - it must negotiate peace.  This is
	//    relevant to having the player break-off a war to fight a third player.


	// Alter the relationships, aim for a good score
	//   The definition of a "good score" is: 
	//     Strengthens me (by giving me allies),
	//     Weakens the strongest players,
	//     Gives me enemies that I can handle and at least one that is accessible
	//       (Enemies that I can handle includes weaker enemies and enemies at war with others)
	//     Maintains stable relationships,
	//     Avoids forming alliances with unreliable players
	//   There are some potential conflicts here: forming an alliance with a powerful
	//     player "strengthens me" AND "strengthens the strongest players".  It can be 
	//     useful if I'm on the defensive, but I risk being overrun by my powerful ally.
	//   Also, if I don't have accessibility to PlayerZ, it doesn't make much sense to
	//     talk players X and Y into attacking PlayerZ since I can't get the spoils (unless
	//     I'm hoping to weaken the other players for an attack).
	//   And if PlayerX doesn't have access to PlayerZ, it doesn't make much sense to
	//     talk him into attacking PlayerZ - he can't do anything anyway.
	//   Also, I don't want to get into a situation where PlayerZ is attacking me, and I get
	//     PlayerX to attack him from behind.  This will weaken PlayerZ (which is good), but
	//     allow PlayerX to gain some easy territory (since PlayerZ's units are near me).
	//     This might be okay if PlayerX is small and weak because it won't substantially 
	//     increase PlayerX's power.

	// What about attacking enemies that are nearby for the purpose of gaining territory + CitRes?
	// Note: in the case that one player is too powerful, players should set-aside their wars
	// Calculate the desire of the other players to attack the other players (especially me).

*/

class CEOSAIForeignRelationsSituation
{
	public:
		CEOSAIForeignRelationsSituation();
		~CEOSAIForeignRelationsSituation(){ DeleteExistingObjects(); }
		void  DeleteExistingObjects();

		long  GetAIPlayerNumber();
		float GetPercentageOfCitResThatAreUnowned();

		void  ResetForeignRelationsFeelings();

		// Used by the historical objects to adjust foreign feelings
		void  AdjustForeignRelationFeelings( long iPlayer, long iTarget, float fValue, float fWeight );

		// Used when agreements are signed
		//void  SignedAPeaceAgreement( long iPlayer1, long iPlayer2 );

		// Create the Player and Relationships values, and some strategy ideas
		void  CreateNationalSummaries( EOSAI::AIPlayer* pAIPlayer );
		void  Calculate();
		//long  GetPlanToAttackPlayer(){ return m_iPlanToAttackPlayer; }
		CEOSAILongSet*  GetMyEnemiesOrSoonToBeEnemies(){ return &m_MyEnemiesOrSoonToBeEnemies; }

		CEOSAINationalSummary*  GetNationalSummary( long iPlayer );
		//CAINationalAlliedAndEnemyPower*  GetAINationalAlliedAndEnemyPower( long iPlayer );

		// Level1 desire of all players
		float  GetCombinedLevel1DesireToAttackPlayer( long iPotentialTargetPlayer );
		float  GetLevel1DesireToAttackPlayer( long iPlayer, long iPotentialTargetPlayer );

		// Ranges from negative values (for allies) to positive (1.0+)
		float  GetDesireToAttackPlayer( long iPotentialTargetPlayer );

	private:

		bool  m_bHasBeenInitialized;

		CEOSAIBrain*   m_pAIBrain;
		EOSAI::AIPlayer* m_pAIPlayer;

		// Input: pPowerList with the Relationships and Player values filled in
		// Results:
		//     The pPowerList objects with Allied + Enemy values filled in
		//     m_NationsSortedByDangerIndex is filled in
		//void   CalculateAlliedAndEnemyBalances( CList< CAINationalAlliedAndEnemyPower* >* pPowerList );
		void   CalculateAlliedAndEnemyPowerForEachPlayer();
		//
		void   CalculatePositionTowardsOtherPlayer_Level1Thoughts();
		void   CalculateLevel1DesiresTowardsPlayer( long iPlayer, long iPotentialTargetPlayer );
		void   CalculatePositionTowardsOtherPlayer_Level2Thoughts();
		void   CalculatePositionTowardsOtherPlayer_WarsAndAlliancesEffects();
		//void   CreateSomeHypotheticalForeignRelationScenarios();
		//void   CreateStrategicOptionsForThePlayer();
		void   CreateForeignRelationsPlanForThePlayer();
		void   TalkToOtherPlayersAboutCooperating();

		// Player Totals
		float  m_fTotalCombatPowerOfAllPlayers;
		float  m_fTotalProductionOfAllPlayers;
		// Whole-Map Totals
	//	float  m_fTotalProductionOnMap;
	//	float  m_fTotalUnownedCitResOnMap;
	//	float  m_fTotalCitResOnMap;
		//float  GetAdjustedTotalProduction(){ return ( m_fTotalProductionOfAllPlayers + m_fTotalProductionOnMap ) / 2.0f; }

		CList< CEOSAINationalSummary* >  m_NationalSummaries;

		// Foreign Relation situation
		CEOSAIListSort< CEOSAINationalSummary* >  m_SingleNationsSortedByPower;
		CEOSAIListSort< EOSAI::CAINationalAlliedAndEnemyPower* >  m_NationsSortedByDangerIndex;

		// Overall situation
		float  m_fUnownedCitResStrategy; // 0.0-1.0  Based on the percentage of unowned CitRes
		float  m_fCombatStrategy; // 0.0-1.0

		// Combat planning
		//   I may need the assistance of a human player to carry this out - that requires
		//   sending a message to the player and getting an agreement
		long     m_iWouldLikeToAttackPlayer; // doesn't tell me if I actually have enough support
		float    m_fMyPowerBalanceIfISimplyDeclareWar;
		CEOSAILongSet m_PlayersWhoMightBeWillingToMakePeace;
		float    m_fMyPowerBalanceIfIMakePeaceWithExistingEnemies;
		CEOSAILongSet m_PlayersWhoMightBeWillingToJoinMeInWar;
		float    m_fMyPowerBalanceIfIMakePeaceAndGetAllOtherPlayersToDeclareWar;
		CEOSAILongSet m_PotentialAlliesSet;

		// When I decide to go to war, that doesn't mean that I declare war
		//   It's probably more useful to plan a sneak attack on that player
		// How do I handle secret agreements between players against a third player?
		long     m_iPlanToAttackPlayer; // 0 or a player number, if this is a player number, it means that I have enough other-player support

		// The set of players I am currently at war with, or will be at war with 
		//   (if I am planning an attack)
		CEOSAILongSet  m_MyEnemiesOrSoonToBeEnemies;

		//CEOSAIListSort< CForeignRelationIssue* >  m_ForeignRelationIssues;
};

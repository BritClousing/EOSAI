#pragma once

class CEOSAIUnitTemplate;
/*
enum EnumMobility
{
	enum_HighMobility,   // This unit is much faster than the target
	enum_MediumMobility, // Is almost as fast as the target
	enum_LowMobility,
};
*/
// Now that all the values exist, I can start to evaluate the usefulness of each of my units
//   This value is based on several things:
//   (1) Will this unit have an attack or defense role against [UnitX]?
//       (a) This is based on movement rate 
//       (b) And the unit's Ground/Sea/Air type + Ground/Sea/Air type of the other unit
//       If the unit plays a defensive role, then it either needs a particular role (Anti-Air)
//          or it need a well-rounded attrition against all units.
//   (2) What is my attrition value against [UnitX]?
//   (3) What is my attrition value against [UnitX] relative to my other units?
//       If all my units have bad attrition, then use my best one - don't always avoid contact
//       Although, I should avoid bad-attrition combat if I have a better attrition combat
//       On the other hand, if he is doing an invasion, then I have to deal with it
// These values will vary depending on the player

//	I should include an "how much of UnitX can I take-out?"
//		This would allow me to do a quick check to figure out when I have enough power to defeat
//		a group of units.
//	Example: 3 enemy tanks - I can add Infantry to the task force until I have enough power to
//		take them out (in this case, one Infantry is assumed to take-out 0.7 Tank).
//		This might get more complicated with mixes of units.

class CEOSAIUnit2VsUnitValue
{
	public:
		CEOSAIUnit2VsUnitValue( CEOSAIUnitTemplate* pAttacker, CEOSAIUnitTemplate* pDefender );

		CEOSAIUnitTemplate* GetAttackerUnitTemplate(){ return m_pAttackerUnitTemplate; }
		CEOSAIUnitTemplate* GetTargetUnitTemplate(){ return m_pTargetUnitTemplate; }

		//
		//void  SetOutgoingProdDamagePerTurn( float f ){ m_fOutgoingProdDamagePerTurn = f; }
		//void  SetIncomingProdDamagePerTurn( float f ){ m_fIncomingProdDamagePerTurn = f; }
		//void  SetMeleeAttrition01( float f ){ m_fMeleeAttrition01 = f; }
		float GetMeleeAttrition01(){ return m_fMeleeAttrition01; }
		//void  SetRangedAttrition01( float f ){ m_fRangedAttrition01 = f; }
		float GetRangedAttrition01(){ return m_fRangedAttrition01; }
		//void  SetRangeX2Attrition01( float f ){ m_fRangeX2Attrition01 = f; }
		float GetRangeX2Attrition01(){ return m_fRangeX2Attrition01; }
		float GetAverageAttrition01(){ return (m_fMeleeAttrition01 + m_fRangedAttrition01)/2.0f; }
		//void  SetMyRelativeMeleeAttritionValue01( float f ){ m_fMyRelativeMeleeAttritionValue01 = f; }
		//float GetMyRelativeMeleeAttritionValue01(){ return m_fMyRelativeMeleeAttritionValue01; }
		//void  SetMyRelativeRangedAttritionValue01( float f ){ m_fMyRelativeRangedAttritionValue01 = f; }
		//float GetMyRelativeRangedAttritionValue01(){ return m_fMyRelativeRangedAttritionValue01; }
		//void  SetMyRelativeRangeX2AttritionValue01( float f ){ m_fMyRelativeRangeX2AttritionValue01 = f; }
		//float GetMyRelativeRangeX2AttritionValue01(){ return m_fMyRelativeRangeX2AttritionValue01; }
		//float GetMyRelativeAttritionValue01(){ return (m_fMyRelativeMeleeAttritionValue01 + m_fMyRelativeRangeX2AttritionValue01) / 2.0f; }
		float GetMyRelativeMeleeAttritionValue01( long iMyPlayerNumber );
		float GetMyRelativeRangedAttritionValue01( long iMyPlayerNumber );
		float GetMyRelativeRangeX2AttritionValue01( long iMyPlayerNumber );
		//float GetMyRelativeAttritionValue01( long iMyPlayerNumber );

		// Attition can be ignored if the TimeToDestroy/BeDestroyed is very small
		//   For example, an attack satelite is a pain and has a 1.0 attrition, but isn't a strong deterent
		float GetTimeToDestroyTarget();
		float GetTimeToBeDestroyed();

		// The usefulness of [Unit1] against [Unit2] is based not only on attrition, but on a number of
		//   factors.  Assuming I own [Unit1], these are:
		// (1) I am using Unit1 in a stationary position (e.g. a city) while Unit2 attacks.
		//       Range becomes very important in this case, because I don't want an infantry getting 
		//       attacked by artillery (or some ranged unit it can't fight back against).
		// (2) I am invading using Unit1 to invade a territory.  If Unit2 is remaining stationary, then
		//       ranged units and good melee attrition will be good.  Unit2 can be allowed to 
		//       escape - that's not a problem.  I don't necessarily need good mobility in this case.
		// (3) I want to destroy Unit2.  This requires that Unit1 has better mobility than Unit2.
		// Note: How does AntiAircraft fit in with this?  (AntiAircraft units might be used in all cases.)
//	//	The task force class must be much smarter to deal with these complications.  It needs to
//	//		consider groups of units.  For example, mixtures of artillery and infantry rather than
//	//		assigning a single number to each unit.  A unit group might also include some AA guns to fend
//	//		off air-attacks against my invading force.
		//float GetCombatUsefulness01();
		float GetCombatSignificance();

		// The "iForPlayer" value is because the AI will use a weak unit in combat if 
		//    it's the best that it's got
		bool  IsUsefulInCombat();

		// Freedom of attack
		//   If a unit is very slow, it's not going to 'rule the seas' because everyone can avoid combat
		//   But, a stationary unit defending a city is perfectly fine
		//
		// Calculate Mobility (sort-of a "freedom of attack")
		//   Offensive roles depend more on MovementRate, AirMovement and somewhat on AttackRange
		//   Defensive roles depend less on MovementRate, and heavier on AttackRange
		// Return values range from -1 to 1
		float  GetOffenseFreedomOfAttack();
		float  GetDefenseFreedomOfAttack();

	private:
		// Units involved
		//
			CEOSAIUnitTemplate* m_pAttackerUnitTemplate;
			CEOSAIUnitTemplate* m_pTargetUnitTemplate;

			//float  m_fFreedomOfMovementBalance01; // 1.0 = high mobility against target, 0.0 = no mobility
			//EnumMobility  m_eMobility;
			//float  m_fMobilityBalance01; // 1.0 = high mobility, 0.0 = no mobility
			//float  m_fOffenseFreedomOfAttack01; // 1.0 = high mobility, 0.0 = no mobility
			//float  m_fDefenseFreedomOfAttack01; // 1.0 = high mobility, 0.0 = no mobility
			float  m_fMovementRateBalance;
			float  m_fAttackRangeAdvantage;
			float  m_fTerrainMovementAdvantage;

		// Damage
		//
			float  m_fOutgoingProdDamagePerTurn;
			float  m_fIncomingProdDamagePerTurn;

		// Attrition Values
		//
			float  m_fMeleeAttrition01;

			// Attacker moves to defender until it can attack
			//   If the Attacker has a longer range than the Defender, then defender moves to attacker
			//   If the Attacker has the same range as Defender, then attrition will be same as Melee
			//   If the Attacker has a shorter range than Defender, then attacker moves to defender, attrition will be less than Melee (free counterattacks)
			float  m_fRangedAttrition01;
			float  m_fRangeX2Attrition01;

		// This is player specific
		//   This is based on a comparison to the player's other units (1.0=best attrition, 0.0=Zero Attrition)
		// Right now, I'm storing global values (ie. based on all the units in the game, what's the relative attrition)
		// However, not all players have access to all units.  This means that the best unit PlayerN has
		//   for attacking UnitX will have a RelativeAttrition below 1.0
		// An alternative way of handling this is to store 'best' attrition values for each player
		//   For memory conservation, I could store that inside each UnitTemplate
		//   Or, a slightly slower version would involve looking up all the player's units and calculating it on the fly
		//	float  m_fMyRelativeMeleeAttritionValue01; 
		//	float  m_fMyRelativeRangedAttritionValue01;
		//	float  m_fMyRelativeRangeX2AttritionValue01;
};

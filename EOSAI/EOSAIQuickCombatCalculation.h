
#pragma once

// Used by the AITacticalProjects class
//   It needs to be very fast because I'm going to do a LOT of these calculations.
//   It treats all the units as one group in one location.

// Information I need out of the quick calculations:
//   Does adding UnitX to my combat-group improve my combat balance?
//     This has to take into account situations where I already take care of some of his units.
//     For example, if he has destroyers and submarines, and I've already got enough force to beat
//       his destroyers, it's the anti-submarine units I need more.  Or, if he has ships and tanks...
//   Do I destroy all his significant combat units?
//

#include "EOSAIStringAndFloatSet.h"
#include "EOSAIQuickCombatResults.h"
class CEOSAIUnit2;
class CEOSAIPoiObject;
namespace EOSAI
{
	class PoiMobile;
}
class CEOSAIUnitTemplate;
class CEOSAIUnit2TemplatesAndFloat;

// Used to do some quick combat calculations
class CEOSAIQuickCombatUnit
{
	public:
		CEOSAIQuickCombatUnit( CEOSAIUnit2* pUnit );
		CEOSAIQuickCombatUnit( CEOSAIUnitTemplate* pUnitTemplate );

		CEOSAIUnit2* m_pAIUnit; // sometimes valid
		CEOSAIUnitTemplate* m_pAIUnitTemplate; // always valid
		//
		CEOSAIQuickCombatUnit* m_pCurrentBestTarget;
		//
		//bool   m_bIsDead;
		float  m_fInitialHP;
		float  m_fCurrentHP;
		//
		bool   m_bAggressive; // militia are not aggressive, but they will fight back
		//
		//float  m_fAbilityToDamageToEnemyForce;
};


// Timing Tests (using UnitTemplates):
//   1 Infantry vs 1 Infantry:
//       Debugger - 0.33 milliseconds  ( 3000 tests/second)
//       Release  - 0.03 milliseconds  (30000 tests/second)
//   2 Infantry vs 2 Infantry:
//       Debugger - 0.67  milliseconds ( 1500 tests/second)
//       Debug    - 0.53  milliseconds ( 1900 tests/second)
//       Release  - 0.096 milliseconds (10000 tests/second)
//   3 Infantry vs 3 Infantry:
//       Release  - 0.22 milliseconds  ( 5000 tests/second)
class CEOSAIQuickCombatCalculation
{
	public:
		// You can setup units or portions of units can have them fight against each other
		//   The CombatResults tell what percentage of each EnemyUnit gets destroyed.
		//   A value of "Infantry" 1.0 means all Infantry units have been destroyed (and no excess power)
		// Timing Test: Using 5 UnitCategories x 5 UnitCategories
		//   Debugger - 0.30 milliseconds (3000 tests/second)
		static void  QuickCombatResults( CEOSAIUnit2TemplatesAndFloat& MyUnits, CEOSAIUnit2TemplatesAndFloat& EnemyUnits, CEOSAIUnit2TemplatesAndFloat& CombatResults );
		static void  QuickCombatDamageSqrt( CEOSAIUnit2TemplatesAndFloat& EnemyUnits, CEOSAIUnit2TemplatesAndFloat& CombatResults, float* pfTotalDamageToEnemyProduction, float* pfTotalDamageToEnemyCombatCapability );

		// Attrition Calculations
		// Attrition 1.0 = Attacker advantage, 0.5 = Equal, 0.0 = Defender advantage
		static float GetMeleeAttrition01( CEOSAIUnitTemplate* pAttacker, CEOSAIUnitTemplate* pDefender ){ return CalculateAttrition01( pAttacker, pDefender, 0.0f ); }
		static float GetRangedAttrition01( CEOSAIUnitTemplate* pAttacker, CEOSAIUnitTemplate* pDefender ){ return CalculateAttrition01( pAttacker, pDefender, 1.0f ); }
		static float GetRangeX2Attrition01( CEOSAIUnitTemplate* pAttacker, CEOSAIUnitTemplate* pDefender ){ return CalculateAttrition01( pAttacker, pDefender, 2.0f ); }
		static float CalculateAttrition01( CEOSAIUnitTemplate* pAttacker, CEOSAIUnitTemplate* pDefender, float fRangeMultiplier, float* pTimeResult = NULL );

		static float GetRangedAttrition01( EOSAI::PoiMobile* pAttacker, EOSAI::PoiMobile* pDefender ){ return CalculateAttrition01( pAttacker, pDefender, 1.0f ); }
		static float CalculateAttrition01( EOSAI::PoiMobile* pAttacker, EOSAI::PoiMobile* pDefender, float fRangeMultiplier, float* pTimeResult = NULL );

		// CombatWin Calculations - 1.0=Attacker Wins, 0.5 = evenly matched, 0.0=Defender Wins
		// Assume both units are within combat range
		static float CalculateCombatWin01( EOSAI::PoiMobile* pAttacker, EOSAI::PoiMobile* pDefender );
		static float CalculateCombatWin01( CEOSAIUnit2TemplatesAndFloat& AttackingUnits, CEOSAIUnit2TemplatesAndFloat& DefendingUnits );

		// Positive values mean the attacker has a range advantage, negative means defender has an advantage
		static float GetCombatRangeAdvantage( EOSAI::PoiMobile* pAttacker, EOSAI::PoiMobile* pDefender );

	public:
		CEOSAIQuickCombatCalculation();
		~CEOSAIQuickCombatCalculation();

		void  ClearAll();
		void  ClearResults();

		// Initialization
		//
			void  LimitCombatByLandAndWaterBarriers( bool b ){ m_bLimitCombatByLandAndWaterBarriers = b; }
			//
			void  SetProDefenderBias( float fBias ); // Bias of 1.0 is equal. 2.0 = ProDefender by 2x. 0.5 = ProAttacker
			//void  SetBiasAgainstAttacker( bool b ){ m_bBiasAgainstAttacker = b; if( b ) m_bBiasAgainstDefender = false; }
			//void  SetBiasAgainstDefender( bool b ){ m_bBiasAgainstDefender = b; if( b ) m_bBiasAgainstAttacker = false; }
			//
			//void  AddToAttackers( CUnit* pUnit );
			void  AddToAttackers( CEOSAIPoiObject* pPoiObject );
			void  AddToAttackers( CEOSAIUnitTemplate* pUnit );
			void  RemoveFromAttackers( CEOSAIPoiObject* pPoiObject );
			void  RemoveFromAttackers( CEOSAIUnitTemplate* pUnit );
			CList<CEOSAIQuickCombatUnit*>*  GetAttackersList(){ return &m_AttackerUnits; }
			//void  AddToDefenders( CUnit* pUnit );
			void  AddToDefenders( CEOSAIPoiObject* pPoiObject );
			void  AddToDefenders( CEOSAIUnitTemplate* pUnit );
			CList<CEOSAIQuickCombatUnit*>*  GetDefendersList(){ return &m_DefenderUnits; }
			//
			void  HandleDefenderCounterAttack( bool b ){ m_bHandleDefenderCounterAttack = b; }

		// I could impliment different targetting methods:
		//   Go after the combat-capable units, degrade enemy combat capability as quickly as possible
		//   Go after weak units
		//   Go after units that will damage my weak units (transports, etc)
		// I do not implement a system that watches geo-barriers.
		//   This means artillery can freely attack ships and vice-versa, though they might not be capable of 
		//   fighting in reality
		void  RunCalculation();

		// Results
		//
			void  SaveState( CEOSAIQuickCombatResults* pCombatResults );

			float GetTotalDamage(){ return m_fProductionDamageToAttacker + m_fProductionDamageToDefender; }

			// Win Balance
			//
				bool  InvolvesEnemyGroundUnits();
				float GetGroundCombatWinBalance01(); // 0.0 = Favors Defender, 1.0 = Favors Attacker
				bool  InvolvesEnemySeaUnits();
				float GetSeaCombatWinBalance01(); // 0.0 = Favors Defender, 1.0 = Favors Attacker
				float GetCombatWinBalance01();

				// If the Enemy has ground units, I need to destroy all of the significant ones and have Ground/Air power left over
				// If the Enemy has sea units, I need to destroy all of the significant ones and have Sea/Air power left over
				// If the Enemy has air units, I need to destroy all of the significant ones and have Air units left over
				//float GetCombatWinBalanceUsingCombatSignificance01();
				// Return: 0.0     Defender was unharmed (no production damage)
				// Return: 0.0-1.0 Defender was harmed but not eliminated
				// Return: 1.0     All CombatSignificant defenders were eliminated
				// Return: 1.0+    All CombatSignificant defenders were eliminated, have some excess attackers
				//float GetAttackerSuccess02();//AttackerSuccessfullyEliminatedDefender();
				//float GetAttackValue01();
				float GetAttackerSuccessValue02();

			//
			bool  AllSignificantAttackersWereDestroyed(); // All Attackers dead, or only has combat-insignificant units left (e.g. satellites)
			bool  AllSignificantDefendersWereDestroyed(); // All Defenders dead, or only has combat-insignificant units left (e.g. satellites)

			// Damage per turn
			//
				float GetOutgoingProdDamagePerTurn();
				float GetIncomingProdDamagePerTurn();

			// Attrition Balance (damage to attackers vs. damage to defenders)
			//
				float GetAttritionBalance01(){ return m_fAttritionBalance01; }
				//float GetMeleeAttrition01();
				//static float GetMeleeAttrition01( CUnitTemplate* pAttacker, CUnitTemplate* pDefender );
				//static float GetRangedAttrition01( CUnitTemplate* pAttacker, CUnitTemplate* pDefender );
				//static float GetRangeX2Attrition01();

			// Survival
			//
				float GetAttackerSurvivalAsCombatSignificance01();
				float GetDefenderSurvivalAsCombatSignificance01();
				float GetAttackerSurvivalAsProductionCost01();
				float GetDefenderSurvivalAsProductionCost01();
				float GetDefenderSurvivalAsCombatSignificance();
				//
				float GetAttackerDamageAsProduction(); // Damage done to Attacker
				float GetDefenderDamageAsProduction(); // Damage done to Defender
				float GetDefenderDamageAsCombatSignificance(); // Damage done to Defender
				// Ground Results
				float GetAttackerGroundUnitDamageAsProduction();
				float GetAttackerGroundUnitSurvivalAsCombatSignificance01();
				float GetAttackerGroundUnitSurvivalAsCombatSignificance();
				float GetDefenderGroundUnitSurvivalAsCombatSignificance01();
				float GetDefenderGroundUnitSurvivalAsCombatSignificance();
				//float GetAttackerGroundUnitSurvival01();
				//float GetDefenderGroundUnitSurvival01();
				// Sea Results
				float GetAttackerSeaUnitDamageAsProduction();
				float GetAttackerSeaUnitSurvivalAsCombatSignificance01();
				float GetAttackerSeaUnitSurvivalAsCombatSignificance();
				float GetDefenderSeaUnitSurvivalAsCombatSignificance01();
				float GetDefenderSeaUnitSurvivalAsCombatSignificance();
				// Air Results
				float GetAttackerAirUnitDamageAsProduction();
				float GetAttackerAirUnitSurvivalAsCombatSignificance01();
				float GetAttackerAirUnitSurvivalAsCombatSignificance();
				float GetDefenderAirUnitSurvivalAsCombatSignificance01();
				float GetDefenderAirUnitSurvivalAsCombatSignificance();

			// Time
			//
				float GetCompletionTime(){ return m_fCompletionTime; }

			// Damage
			//
				float GetProductionDamageToAttacker(){ return m_fProductionDamageToAttacker; }
				float GetProductionDamageToAttacker01(){ return m_fProductionDamageToAttacker / m_fProductionCostOfAttacker; }
				float GetProductionDamageToDefender(){ return m_fProductionDamageToDefender; }
				float GetProductionDamageToDefender01(){ return m_fProductionDamageToDefender / m_fProductionCostOfDefender; }

	private:

		void                CalculateAbilityToDamageForAllUnits();
		CEOSAIQuickCombatUnit* FindBestTarget( CEOSAIQuickCombatUnit* pUnit1, CList< CEOSAIQuickCombatUnit* >* pList2, float* pfOutHPDamage );
		float               GetApplyableDamage( CEOSAIQuickCombatUnit* pTarget, float fHPDamage );
		void                ApplyDamage( CEOSAIQuickCombatUnit* pTarget, float fHPDamage );

		// If [true], then prevent ground units from freely attacking sea units and vice-versa.
		bool   m_bLimitCombatByLandAndWaterBarriers;
		float  m_fProDefenderBias;
		//bool   m_bBiasAgainstAttacker;
		//bool   m_bBiasAgainstDefender;
		bool   m_bHandleDefenderCounterAttack;

		CList< CEOSAIQuickCombatUnit* >  m_AttackerUnits;
		CList< CEOSAIQuickCombatUnit* >  m_DefenderUnits;

		// Results
		float m_fCompletionTime;
		float m_fAttritionBalance01; // 0.0 = Favors Defender, 0.5 = Equal Power, 1.0 = Favors Attacker
		float m_fProductionCostOfAttacker;
		float m_fProductionDamageToAttacker;
		float m_fProductionCostOfDefender;
		float m_fProductionDamageToDefender;
};



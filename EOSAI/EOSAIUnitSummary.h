
#pragma once

class CEOSAIUnitTemplate;
class CEOSAIPoiObject;
#include "EOSAIStringAndFloatSet.h"

class CEOSAIUnitSummary
{
	public:
		CEOSAIUnitSummary(){ Clear(); }
		CEOSAIUnitSummary( CEOSAIUnitSummary& Other ){ *this = Other; }

		void  Clear();

		float GetProductionValue(){ return m_fProductionValue; }
		float GetCombatSignificance(){ return m_fCombatSignificance; }
		float GetTotalCombatPower(){ return SumOfCombatValues(); }

		void  AddToSummary( CEOSAIPoiObject* pAIPoiObject );
		//void  AddToSummary( CUnitTemplate* pUnitTemplate );
		void  AddToSummary( CEOSAIUnitTemplate* pAIUnitTemplate );
		void  RemoveFromSummary( CEOSAIPoiObject* pAIPoiObject );
		//void  RemoveFromSummary( CUnitTemplate* pUnitTemplate );
		void  RemoveFromSummary( CEOSAIUnitTemplate* pUnitTemplate );

		// CombatCalculations can be done with CAICombatPlanner, but 
		// iBattleResult 0 = Stalemate, 1 = I Win, 2 = Other Wins
		//void  CalculateCombat( CEOSAIUnitSummary* pOtherSummary,
		//		long* iBattleResult, float* pfProductionDamageToMe, float* pfProductionDamageToOther,
		//		float* pfTimeToEndOfBattle );

		//void  Calculate( CAIPlayer* pPlayer );  // Calculate aggregate capability of entire nation
		//void  CalculateInProduction( CAIPlayer* pPlayer );  // Calculate capabilities of units in production
		void  Set( CEOSAIUnitTemplate* pTemplate );  // Calculate abilities of single unit
		void  SetAndDivideByCost( CEOSAIUnitTemplate* pTemplate, float fCost );  // Calculate abilities of single unit

		float GetGroundCombatPresence(){ return m_fGroundAttackUnits + m_fGroundDefenseUnits; }

		//
		void  operator=( CEOSAIUnitSummary& Other );
		void  operator+=( CEOSAIUnitSummary& Other );
		void  operator-=( CEOSAIUnitSummary& Other );
		CEOSAIUnitSummary operator+( CEOSAIUnitSummary& Other1 ){ CEOSAIUnitSummary ret; ret = *this; ret += Other1; return ret; }
		CEOSAIUnitSummary operator-( CEOSAIUnitSummary& Other1 ){ CEOSAIUnitSummary ret; ret = *this; ret -= Other1; return ret; }
		CEOSAIUnitSummary operator*( float fMult ){ CEOSAIUnitSummary ret; ret = *this; ret.Multiply( fMult ); return ret; }
		//

		//
		// Math functions
		//
			// Unary functions
			void  Normalize();             // Scale all values to the Sum is 1.0
			void  NothingBelowZero();      // Any negative values for categories are set to 0
			void  Multiply( float fVal );  // Multiply all categories by [fVal]
			float Sum();               // Simple sum of all values
			float SumOfCombatValues(); // Simple sum of ( UnitStrength + CombatVs ) values
			//float SumSquared();
			//float CalculateSatisfactionScore( CEOSAIUnitSummary& Other );

		// [this] should contain the Desired Capability
		// Used to calculate the value of a Template
			//float GetSumSquaredValue( CBuildOptionEvaluation& BuildOptionEvaluation );
			//float GetSumSquaredValue( CAIProductionOption& AIProductionOption );

	//private:

			CList< CEOSAIPoiObject* >     m_AIPoiObjects;
			CList< CEOSAIUnitTemplate* >  m_AIUnitTemplates;

		//
		// I may want to track some other capabilities, for example:
		//    I could divide Exploration into "Short range" (aircraft), "Long range" (ships), and 
		//    (possibly) "Long Range Stealth" (submarines).
		// I might also want to add-in an "importance" value.  Right now, everything is considered
		//    to be equally important, but there may be cases where some capabilities are more
		//    more fine-grained (and thus, less important).
		//

		float  m_fProductionValue;
		float  m_fCombatSignificance;

		// Exploration, Sighting
		float  m_fExploration;
		float  m_fSighting;

		// Aircraft
		float  m_fStrategicBombing;
		float  m_fAirEscort;
		float  m_fAntiAir;  // anti-aircaft guns, fighters, SAM sites

		// Submarines
		float  m_fSubmarineCapability;  // strength, speed, stealth of submarines
		float  m_fAntiSubmarine;

		// Ground
		float  m_fLandCitResHunter;
		float  m_fBombardment;        // good as a standoff ground weapon, hitting ships
		float  m_fGroundAttackUnits;  // units that can attack ground units (ground units, aircraft, ..)
		float  m_fGroundDefenseUnits;
		float  m_fInvasionDefense;    // includes aircraft, coastal ships to attack transports, ...

		// Sea
		float  m_fSeaResourceHunter;
		float  m_fSeaSupremacy;  // ship-to-ship combat
		float  m_fShoreBombardment;
		//
		float  m_fGroundTransportCapability; // movement of ground units over water (via a ship or aircraft)
		//
		float  m_fTrade;

	// Combat
	//
		// UnitStrength is Defense * Current HP
		EOSAI::StringAndFloatSet  m_UnitHPDefense_UnitTypeSet;

		// CombatVs is the AttackVs value (to-hit*damage) * Current HP * NormalDefense
		EOSAI::StringAndFloatSet  m_CombatPowerVsUnitTypes;
		float  m_fCombatVsCity;

	private:
		void  AddToSummaryValue( CEOSAIUnitTemplate* pUnitTemplate ); // Calculate abilities of single unit
};

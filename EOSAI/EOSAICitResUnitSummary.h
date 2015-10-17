
#pragma once

#include "EOSAICitResSummary.h"
#include "EOSAIUnitSummary.h"
//class pAIPoiObject;

class CEOSAICitResUnitSummary
{
	public:
		CEOSAICitResUnitSummary(){}

		void Clear()
		{
			m_CitRes.Clear();
			m_Units.Clear();
		}

		void AddToSummary( CEOSAIPoiObject* pAIPoiObject )
		{
			m_CitRes.AddToSummary( pAIPoiObject );
			m_Units.AddToSummary( pAIPoiObject );
		}
		void  operator+=( CEOSAICitResUnitSummary& Other );

		long   GetCitResCount(){ return m_CitRes.m_iCitResCount; }
		float  GetUnitPower()
		{
			return
				m_Units.m_UnitHPDefense_UnitTypeSet.GetCummulativeValue() +
				//m_Units.m_fUnitStrengthSoft + 
				//m_Units.m_fUnitStrengthArmor +
				//m_Units.m_fUnitStrengthAircraft + m_Units.m_fUnitStrengthMissile +
				//m_Units.m_fUnitStrengthShip + m_Units.m_fUnitStrengthSubmarine +
				m_Units.m_CombatPowerVsUnitTypes.GetCummulativeValue() +
				//m_Units.m_fCombatVsSoft + 
				//m_Units.m_fCombatVsArmor +
				//m_Units.m_fCombatVsAircraft + m_Units.m_fCombatVsMissile +
				//m_Units.m_fCombatVsShip + m_Units.m_fCombatVsSubmarine;
				0.0f;
		}
		float  GetCitResUnitPresence()
		{
			return 
				m_CitRes.GetCitResValue() +
				m_Units.m_fGroundTransportCapability +
				m_Units.m_UnitHPDefense_UnitTypeSet.GetCummulativeValue() +
				//m_Units.m_fUnitStrengthSoft + 
				//m_Units.m_fUnitStrengthArmor +
				//m_Units.m_fUnitStrengthAircraft + m_Units.m_fUnitStrengthMissile +
				//m_Units.m_fUnitStrengthShip + m_Units.m_fUnitStrengthSubmarine +
				m_Units.m_CombatPowerVsUnitTypes.GetCummulativeValue() +
				//m_Units.m_fCombatVsSoft + 
				//m_Units.m_fCombatVsArmor +
				//m_Units.m_fCombatVsAircraft + m_Units.m_fCombatVsMissile +
				//m_Units.m_fCombatVsShip + m_Units.m_fCombatVsSubmarine;
				0.0f;
		}
		float  GetAIDangerValue()
		{
			return
				m_CitRes.GetCitResValue()/2.0f + // only because a player is likely to attack if we get too close
				m_Units.m_UnitHPDefense_UnitTypeSet.GetCummulativeValue() +
				//m_Units.m_fUnitStrengthSoft + 
				//m_Units.m_fUnitStrengthArmor +
				//m_Units.m_fUnitStrengthAircraft + m_Units.m_fUnitStrengthMissile +
				//m_Units.m_fUnitStrengthShip + m_Units.m_fUnitStrengthSubmarine +
				m_Units.m_CombatPowerVsUnitTypes.GetCummulativeValue() +
				//m_Units.m_fCombatVsSoft + 
				//m_Units.m_fCombatVsArmor +
				//m_Units.m_fCombatVsAircraft + m_Units.m_fCombatVsMissile +
				//m_Units.m_fCombatVsShip + m_Units.m_fCombatVsSubmarine;
				0.0f;
		}

		CEOSAICitResSummary  m_CitRes;
		CEOSAIUnitSummary    m_Units;
};


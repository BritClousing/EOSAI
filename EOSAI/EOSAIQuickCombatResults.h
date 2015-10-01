
#pragma once

class CEOSAIQuickCombatResults
{
	public:
		CEOSAIQuickCombatResults()
		{
			Clear();
		}
		void  Clear()
		{
			m_fCombatSuccessValue02 = 0.0f;
			m_fAttritionBalance01 = 0.0f;
			//
			m_fAttackerDamageAsProduction = 0.0f;
			//
			m_fDefenderDamageAsProduction = 0.0f;
			m_fDefenderDamageAsCombatSignificance = 0.0f;
			m_fDefenderSurvivalAsCombatSignificance = 0.0f;
		}

		float GetDelta_AttackerDamageAsProduction( CEOSAIQuickCombatResults* p ){ return m_fAttackerDamageAsProduction - p->m_fAttackerDamageAsProduction; }
		float GetDelta_DefenderDamageAsProduction( CEOSAIQuickCombatResults* p ){ return m_fDefenderDamageAsProduction - p->m_fDefenderDamageAsProduction; }
		float GetDelta_DefenderDamageAsCombatSignificance( CEOSAIQuickCombatResults* p ){ return m_fDefenderDamageAsCombatSignificance - p->m_fDefenderDamageAsCombatSignificance; }
		float GetDelta_DefenderSurvivalAsCombatSignificance( CEOSAIQuickCombatResults* p ){ return m_fDefenderSurvivalAsCombatSignificance - p->m_fDefenderSurvivalAsCombatSignificance; }

		float m_fCombatSuccessValue02;
		float m_fAttritionBalance01;
		// Attacker Results
		float m_fAttackerDamageAsProduction;
		// Defender Results
		float m_fDefenderDamageAsProduction;
		float m_fDefenderDamageAsCombatSignificance;
		float m_fDefenderSurvivalAsCombatSignificance;
};


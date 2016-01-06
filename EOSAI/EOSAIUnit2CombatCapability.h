#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIBCDumbArray1D.h"
//#include "AIUnitTemplateCombatAndAttritionValues.h"
class CEOSAIUnitVsUnitValue;
class CEOSAIUnitTemplate;
class CEOSAIUnitTemplateSet;

// Describes a UnitTemplate's combat abilities against other units

class DLLIMPEXP CEOSAIUnitCombatCapability
{
	public:
		CEOSAIUnitCombatCapability();
		~CEOSAIUnitCombatCapability();

		// Initialization
		void  SetMyAIUnitTemplate( CEOSAIUnitTemplate* pAIUnitTemplate ){ m_pMyAIUnitTemplate = pAIUnitTemplate; }

		// Visible Range (used by the AI to figure out submarine units)
		void  CalculateAverageVisibleRange( CEOSAIUnitTemplateSet* pEnemyUnitSet );
		float GetAverageVisibleRange();

		// Used to calculate Combat Significance
		void  CalculateRelativeValue( CEOSAIUnitTemplateSet* pMyUnitSet, CEOSAIUnitTemplateSet* pEnemyUnitSet );
		bool  IsACombatUnit(){ return m_fCombatAttritionSignificance01 > 0.4f; }
		float GetCombatSignificance(); // Unit Cost * m_fCombatAttritionSignificance
		float GetCombatAttritionSignificance01(){ return m_fCombatAttritionSignificance01; }

		// Used to calculate unit-vs-unit attrition balance
		CEOSAIUnitVsUnitValue*  InvokeCombatValue( CEOSAIUnitTemplate* pAIUnitTemplate );
		CEOSAIBCDumbArray1D< CEOSAIUnitVsUnitValue* >*  GetCombatValueArray(){ return &m_CombatValueArray; }

	private:

		CEOSAIUnitTemplate* m_pMyAIUnitTemplate;

		float  m_fAverageVisibleRange; // Submarines will have a lower average visible range, based on all units in the game
		float  m_fCombatAttritionSignificance01;

		void   InvokeArray();
		CEOSAIBCDumbArray1D< CEOSAIUnitVsUnitValue* >  m_CombatValueArray;
};


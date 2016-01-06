
#pragma once
#include "EOSAIUnitTemplateAndFloat.h"

class CEOSAIUnitTemplatesAndFloat
{
	public:
		~CEOSAIUnitTemplatesAndFloat();

		void  Clear(); // deletes all m_List objects
		void  SetAllValuesToZero(); // sets all m_List objects to 0

		void  Add( CEOSAIUnitTemplate* pUnitTemplate, float f );
		void  Set( CEOSAIUnitTemplate* pUnitTemplate, float f );
		float GetValue( CEOSAIUnitTemplate* pUnitTemplate );

		// This can't tell you if [MyUnits] can defeat [EnemyUnits]
		//   It can tell you if there is an imbalance in my combat capability against particular EnemyUnits 
		//void  CalculatePowerLevelsAgainst( CEOSAIUnitTemplatesAndFloat& MyUnits, CEOSAIUnitTemplatesAndFloat& EnemyUnits );

		CList< CEOSAIUnitTemplateAndFloat* >  m_List;
};

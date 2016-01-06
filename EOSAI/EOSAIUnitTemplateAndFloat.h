
#pragma once

class CEOSAIUnitTemplate;

class CEOSAIUnitTemplateAndFloat
{
	public:
		CEOSAIUnitTemplateAndFloat(){ m_pAIUnitTemplate = NULL; m_fValue = 0.0f; }
		CEOSAIUnitTemplateAndFloat( CEOSAIUnitTemplate* pAIUnitTemplate, float f ){ m_pAIUnitTemplate = pAIUnitTemplate; m_fValue = f; }

		CEOSAIUnitTemplate* m_pAIUnitTemplate;
		float m_fValue;
};


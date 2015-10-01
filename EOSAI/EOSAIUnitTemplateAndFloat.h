
#pragma once

class CEOSAIUnitTemplate;

class CEOSAIUnit2TemplateAndFloat
{
	public:
		CEOSAIUnit2TemplateAndFloat(){ m_pAIUnitTemplate = NULL; m_fValue = 0.0f; }
		CEOSAIUnit2TemplateAndFloat( CEOSAIUnitTemplate* pAIUnitTemplate, float f ){ m_pAIUnitTemplate = pAIUnitTemplate; m_fValue = f; }

		CEOSAIUnitTemplate* m_pAIUnitTemplate;
		float m_fValue;
};


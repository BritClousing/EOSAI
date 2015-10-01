
#pragma once
#include "EOSAIBuildOption.h"
class CEOSAIBuildOption;

class CEOSAIBuildOrder
{
	public:
		CEOSAIBuildOrder(){ m_pAIBuildOption = NULL; }

		CEOSAIBuildOption*  GetAIBuildOption(){ return m_pAIBuildOption; }

		bool   IsMoney(){ return m_pAIBuildOption->IsMoney(); }// == "<Money>"; }
		bool   IsAUnit(){ return m_pAIBuildOption->IsAUnit(); }//m_strBuildTarget.Find( "<" ) == -1; }
		bool   IsABuilding(){ return m_pAIBuildOption->IsABuilding(); }

		CString  GetBuildTarget(){ return m_pAIBuildOption->GetInternalName(); }

	protected:
		CEOSAIBuildOption*  m_pAIBuildOption;
};



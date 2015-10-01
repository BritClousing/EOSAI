
#include "stdafx.h"
#include "EOSAICitResValueMultiplier.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

float CEOSAICitResValueMultiplier::GetResourceValueMultiplier( CString strResourceType )
{
	if( strResourceType == _T("Gold") )
	{
		return m_fGoldValueMultiplier;
	}
	else if( strResourceType == _T("Oil") )
	{
		return m_fOilValueMultiplier;
	}
	else if( strResourceType == _T("Iron") )
	{
		return m_fIronValueMultiplier;
	}
	else if( strResourceType == _T("Food") )
	{
		return m_fFoodValueMultiplier;
	}
	ASSERT( false );
	return 0.0f;
}

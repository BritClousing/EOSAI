
#include "stdafx.h"
#include "EOSAICitResUnitSummary.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void  CEOSAICitResUnitSummary::operator+=( CEOSAICitResUnitSummary& Other )
{
	m_CitRes += Other.m_CitRes;
	m_Units += Other.m_Units;
}



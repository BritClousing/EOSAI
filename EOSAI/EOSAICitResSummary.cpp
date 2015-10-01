
#include "stdafx.h"
#include "EOSAICitResSummary.h"
#include "City.h"
//#include "EOSAIResource.h"
#include "EOSAIResource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAICitResSummary::Clear()
{
	m_iCitResCount = 0;
	m_fCityProduction = 0.0f;
	m_fGoldProduction = 0.0f;
	m_fIronProduction = 0.0f;
	m_fFoodProduction = 0.0f;
	m_fOilProduction  = 0.0f;
}

void  CEOSAICitResSummary::operator+=( CEOSAICitResSummary& Other )
{
	m_iCitResCount += Other.m_iCitResCount;
	m_fCityProduction += Other.m_fCityProduction;
	m_fGoldProduction += Other.m_fGoldProduction;
	m_fIronProduction += Other.m_fIronProduction;
	m_fFoodProduction += Other.m_fFoodProduction;
	m_fOilProduction  += Other.m_fOilProduction;
}

void  CEOSAICitResSummary::AddToSummary( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
	if( pAICity )
	{
		m_iCitResCount += 1;
		m_fCityProduction += pAICity->GetProduction();//GetTotalProduction();// false,false );
	}
	CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
	if( pAIResource )
	{
		CString strResType = pAIResource->GetResourceType();
		float fProduction = (float) pAIResource->GetResourcePerTurn();

		m_iCitResCount += 1;
		if(      strResType == _T("Gold") ){ m_fGoldProduction += fProduction; }
		else if( strResType == _T("Iron") ){ m_fIronProduction += fProduction; }
		else if( strResType == _T("Oil") ){  m_fOilProduction  += fProduction; }
		else if( strResType == _T("Food") ){ m_fFoodProduction += fProduction; }
		else { ASSERT( false ); }
	}
}

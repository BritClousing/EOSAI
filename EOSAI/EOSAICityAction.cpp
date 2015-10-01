
#include "stdafx.h"
#include "EOSAICityAction.h"
#include "EOSAIUnitActionIdea.h"
#include "City.h"
//#include "EOSAIProductionOption.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
/*
CEOSAICityAction_Production::CEOSAICityAction_Production( 
	CEOSAIUnit2ActionIdea* pAIUnitActionIdea ) :
	CEOSAIAction( pAIUnitActionIdea )
{
	/-*
	CEOSAIPoiObject* pAIPoiObject = pAIUnitActionValue->m_pActor;
	CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
	ASSERT( pAICity );
	if( pAICity )
	{
		m_pAICity = pAICity;
	}
	*-/
	m_pAICity = pAIUnitActionIdea->GetAICityActor();
	m_pAIProduction = NULL;
}

void CEOSAICityAction_Production::Update()
{
	float fTimeToBuild = m_pAIProduction->GetSimpleTimeToComplete( m_pAICity->GetServerCity() );

	// This is actually wrong, it should take the "start time" into account
	//m_FinalState.SetTime( fTimeToBuild );
}
*/
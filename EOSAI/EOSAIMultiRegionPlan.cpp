
#include "stdafx.h"
#include "EOSAIMultiRegionPlan.h"
#include "EOSAIMultiRegion2.h"
#include "EOSAICommonData2.h"
#include "EOSAIBrain.h"
#include "EOSAIInterface.h"
/*
float CEOSAIMultiRegionPlan::GetGroundSeaProximity()
{
	CEOSAIMultiRegionNationwidePathways* pNationwidePathways = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );
	CEOSAIMultiRegionAttackAccess* pAttackAccess = pNationwidePathways->GetMultiRegionAttackAccess( m_iMultiRegion );

	//m_pMultiRegion->
}

float CEOSAIMultiRegionPlan::GetAirProximity()
{

}
*/
CEOSAIMultiRegionPlan::CEOSAIMultiRegionPlan( CEOSAIBrain* pAIBrain )
{
	m_pAIBrain = pAIBrain;
	//
	m_iMultiRegion = 0;
	m_pMultiRegion = NULL;
	m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Undefined;
	//
	m_fOtherPlayerGroundSeaUnitAccess01x = 0.0f;
	m_fOtherPlayerAirUnitAccess01x = 0.0f;
	//
	m_fDistanceFromMyCitRes = 0.0f; // 1 = I have units inside the MultiRegion
	m_fDistanceFromMyCitResUnits = 0.0f;
	m_fValue01 = 0.0f; // CitRes value + value of destroying enemy units (particularly if they are invading)
	m_fNearbyEnemyCombatUnits = 0.0f;
	m_fCombatUnits = 0.0f;
	//
	m_fDangerOfEnemyInvasion01 = 0.0f;
}

CEOSAIGeo* CEOSAIMultiRegionPlan::GetAIGeo()
{
	long iGeo = m_pMultiRegion->GetGeoId();
	ASSERT( iGeo > 0 );
	if( iGeo == 0 ) return NULL;
	//CEOSAIGeo* pAIGeo = m_pAIBrain->GetAIGeo( iGeo );
	CEOSAIGeo* pAIGeo = g_pEOSAIInterface->GetAICommonData()->GetAIGeo( iGeo );
	ASSERT( pAIGeo );
	return pAIGeo;
}

bool CEOSAIMultiRegionPlan::PlanIsToIgnore()
{
	if( m_eMultiRegionPlan == EOSAIEnumMultiRegionPlan_Undefined ||
		m_eMultiRegionPlan == EOSAIEnumMultiRegionPlan_NoPlan ||
		m_eMultiRegionPlan == EOSAIEnumMultiRegionPlan_Ignore ||
		m_eMultiRegionPlan == EOSAIEnumMultiRegionPlan_Water_NoPlanYet ||
		m_eMultiRegionPlan == EOSAIEnumMultiRegionPlan_Land_NoPlanYet )
	{
		return true;
	}
	return false;
}


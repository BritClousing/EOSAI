
#pragma once
#include "EOSAIEnumMultiRegionPlan.h"
class CEOSAIMultiRegion2;
class CEOSAIBrain;
class CEOSAIGeo;

class CEOSAIMultiRegionPlan
{
	public:
		CEOSAIMultiRegionPlan( CEOSAIBrain* pAIBrain );

		CEOSAIGeo* GetAIGeo();
		EOSAIEnumMultiRegionPlan  GetPlan(){ return m_eMultiRegionPlan; }
		bool PlanIsToIgnore();

		//float GetGroundSeaProximity();
		//float GetAirProximity();

		CEOSAIBrain* m_pAIBrain;
		long      m_iMultiRegion;
		CEOSAIMultiRegion2*       m_pMultiRegion;
		EOSAIEnumMultiRegionPlan  m_eMultiRegionPlan;

		float  m_fOtherPlayerGroundSeaUnitAccess01x;
		float  m_fOtherPlayerAirUnitAccess01x;

		//
		float  m_fDistanceFromMyCitRes;
		float  m_fDistanceFromMyCitResUnits;
		float  m_fValue01;
		float  m_fNearbyEnemyCombatUnits; // combat value of units nearby (if an enemy can bring over forces)
		float  m_fCombatUnits; // combat value of units inside the multiregion

		// If I own this MultiRegion
		//
			float m_fDangerOfEnemyInvasion01; // Not used yet, will be used for building-up forces
};



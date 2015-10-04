
#pragma once
#include "EOSAIStringAndFloatSet.h"
#include "EOSAIStringAndLongSet.h"
class CWorldDesc;

namespace EOSAI
{
class PlayerResourceSummary
{
	public:
		PlayerResourceSummary();
		void Clear();
		//void Calculate( CWorldDesc* pWorldDesc, long iPlayer, EOSAI::StringAndFloatSet* pResourceTotals );

		//long    GetAvailabilityNumber( CString strResource );
		//CString GetAvailabilityText( CString strResource );

		//std::map<CString, float>   m_TotalResources; // Current Resources
		EOSAI::StringAndFloatSet  m_TotalResources; // Current Resources

		EOSAI::StringAndFloatSet  m_ResourceProduction;
		EOSAI::StringAndFloatSet  m_ResourceConsumption;
		EOSAI::StringAndFloatSet  m_ResourceTrade;
		EOSAI::StringAndFloatSet  m_ResourceDelta;

		EOSAI::StringAndFloatSet  m_ResourceConsumptionRate; // Adjusts the consumption of resources
		EOSAI::StringAndFloatSet  m_ResourceExcessAmount; // Where excess benefits begin
		//EOSAI::StringAndLongSet   m_ResourceAvailabilityDisplay; // 1,2,3,4,5 = Critical, Shortage, Adequate, ...
/*
		// Population Growth
		float m_fCityPopulationGrowth; // 1.0 = No Population Growth
		// Production Multiplier
		float m_fMoneyBasedProductionMultiplier; // 1.0 = Normal
		float m_fOilBasedProductionMultiplier; // 1.0 = Normal
		float m_fFoodBasedProductionMultiplier; // 1.0 = Normal
		//
		float m_fIronBasedIronProductionMultiplier; // Only applies to iron-based production
		// Movement Rates
		float m_fOilBasedRangedUnitLaunchPercentage; // 1.0 = Normal
		float m_fOilBasedGroundSeaUnitMovementRateMultiplier; // 1.0 = Normal
*/
};
}

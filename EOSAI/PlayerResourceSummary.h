
#pragma once
#include "EOSAIStringAndFloatSet.h"
#include "EOSAIStringAndLongSet.h"
#include "ResourceAmounts.h"
#include <map>
//class CWorldDesc;

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
		//EOSAI::StringAndFloatSet  m_TotalResources; // Current Resources
		EOSAI::ResourceAmounts    m_TotalResources; // Current Resources (should always be 0 or higher)

		EOSAI::ResourceAmounts    m_ResourceProductionAssumingNoOrders;
		EOSAI::ResourceAmounts    m_ResourceProductionByCurrentOrders;
		EOSAI::ResourceAmounts    m_ResourceConsumptionAssumingNoOrders;
		EOSAI::ResourceAmounts    m_ResourceConsumptionByCurrentOrders;
		//EOSAI::StringAndFloatSet  m_ResourceProduction;
		//EOSAI::StringAndFloatSet  m_ResourceConsumption;
		EOSAI::ResourceAmounts    m_ResourceTradeDelta;
		//EOSAI::StringAndFloatSet  m_ResourceTrade;
		//EOSAI::StringAndFloatSet  m_ResourceDelta;
		EOSAI::ResourceAmounts    GetResourceDelta() {
										return m_ResourceProductionAssumingNoOrders
											+ m_ResourceProductionByCurrentOrders
											- m_ResourceConsumptionAssumingNoOrders
											- m_ResourceConsumptionByCurrentOrders
											+ m_ResourceTradeDelta; }

//		EOSAI::StringAndFloatSet  m_ResourceConsumptionRate; // Adjusts the consumption of resources - I should probably add this back in
//		EOSAI::StringAndFloatSet  m_ResourceExcessAmount; // Where excess benefits begin
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

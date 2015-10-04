
#pragma once
#include "EOSAIStringAndFloatSet.h"
#include "EOSAIStringAndLongSet.h"
namespace EOSAI
{
	class AIPlayer;
}
//class CWorldDesc;

class CEOSAIResourceEffectsSummary
{
	public:
		CEOSAIResourceEffectsSummary();
		void Clear();
		//void Calculate(CWorldDesc* pWorldDesc, long iPlayer, EOSAI::StringAndFloatSet* pResourceTotals);
		void Calculate(EOSAI::AIPlayer* pAIPlayer, long iPlayer, EOSAI::StringAndFloatSet* pResourceTotals);

		long    GetAvailabilityNumber( CString strResource );
		//CString GetAvailabilityText( CString strResource );

		EOSAI::StringAndFloatSet  m_ResourceProduction;
		EOSAI::StringAndFloatSet  m_ResourceConsumption;
		EOSAI::StringAndFloatSet  m_ResourceTrade;
		EOSAI::StringAndFloatSet  m_ResourceDelta;

		EOSAI::StringAndFloatSet  m_ResourceConsumptionRate; // Adjusts the consumption of resources
		EOSAI::StringAndFloatSet  m_ResourceExcessAmount; // Where excess benefits begin
		EOSAI::StringAndLongSet   m_ResourceAvailabilityDisplay; // 1,2,3,4,5 = Critical, Shortage, Adequate, ...

		// Population Growth
		/*
		// All of this is very game-specific. I should come up with some other way of codifying it.
		//   Also, most games don't do this at all.
		//
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


#include <stdafx.h>
#include "PlayerResourceSummary.h"

using namespace EOSAI;

PlayerResourceSummary::PlayerResourceSummary()
{
	Clear();
}

void PlayerResourceSummary::Clear()
{
	m_ResourceProduction.Clear();
	m_ResourceConsumption.Clear();
	m_ResourceTrade.Clear();
	m_ResourceDelta.Clear();

	m_ResourceConsumptionRate.Clear();
	m_ResourceExcessAmount.Clear();
	m_ResourceAvailabilityDisplay.Clear();
	//
	/*
	m_fCityPopulationGrowth = 1.0f;
	//
	m_fMoneyBasedProductionMultiplier = 1.0f;
	m_fOilBasedProductionMultiplier = 1.0f;
	m_fFoodBasedProductionMultiplier = 1.0f;
	m_fIronBasedIronProductionMultiplier = 1.0f;
	//
	m_fOilBasedRangedUnitLaunchPercentage = 1.0f;
	m_fOilBasedGroundSeaUnitMovementRateMultiplier = 1.0f;
	*/
}

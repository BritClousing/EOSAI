
#include "stdafx.h"
#include "EOSAIBuildingDescription.h"
#include "EOSAIInterface.h"


CEOSAIBuildingValue::CEOSAIBuildingValue( CEOSAICity* pCity, CEOSAIBuildingDescription* pBuildingDesc )
{
	m_pAICity = pCity;
	m_pBuildingDescription = pBuildingDesc;
	//
	m_strInternalName = pBuildingDesc->m_strInternalName;

	g_pEOSAIInterface->GetBuildingValue( this );
	//g_pEOSAIInterface->GetBuildingValue( &BuildingValue );
}

CEOSAIBuildingDescription::CEOSAIBuildingDescription( EOSAI::CGameRules* pGameRules )
{
	m_pGameRules = pGameRules;

	m_bCanOnlyBeBuiltInCitiesAdjacentToWater = false;
	m_iMaximumNumberOfCopiesOfThisBuildingInACity = 1;
	m_iNumberAllowedInNation = 1000000;

	m_bAutomaticallyDestroyedWhenCapturedOrTransferred = false;

	m_fMinimumPopulationRequired = 0.0f;

	// Cost
	//
	m_fProductionCost = 0.0f;
	//m_fIronCost = 0.0f;

	// Benefits
	//
	// Production
	m_fBonusProduction = 0.0f;
	m_fProductionBonusXPopulation = 0.0f;
	m_fGroundUnitProductionBonusXPopulation = 0.0f;//m_fBonusGroundUnitProduction = 0.0f;
	m_fSeaUnitProductionBonusXPopulation = 0.0f;//m_fBonusSeaUnitProduction = 0.0f;
	m_fAirUnitProductionBonusXPopulation = 0.0f;//m_fBonusAirUnitProduction = 0.0f;

	// Repair
	m_fRepairGroundUnitRate = 0.0f;
	m_fRepairAircraftRate = 0.0f;
	m_fRepairSeaUnitRate = 0.0f;

	// Food
	m_fLandFoodProductionUptoDouble = 0.0f;//m_fAdditionalFoodProduction = 0.0f;
	m_fFoodProvidedToUnitsInCity = 0.0f;//m_fFoodForCityUnits = 0.0f;

	// Oil
	m_fOilProductionUptoDouble = 0.0f;//m_fAdditionalOilProduction = 0.0f;
	m_fCityOilConsumptionReduction = 0.0f;//m_fReducedOilConsumption = 0.0f;

	// View Range
	m_fViewRange = 0.0f;//m_fCurrentViewRange = 0.0f;
	//m_fNewViewRange = 0.0f;
	m_fRadarRange = 0.0f;
}

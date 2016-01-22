
#pragma once
class CEOSAIUnitTemplate;
#include "EOSAIUnitsubsetPermission.h"
#include "EOSAIStringAndFloatSet.h"
#include "ResourceAmounts.h"
class CEOSAICity;
class CEOSAIBuildingDescription;

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
	class CGameRules;
};

// Calculate the effects of a BuildingDesc on a particular city (it takes population into account)
class DLLIMPEXP CEOSAIBuildingValue
{
	public:
		CEOSAIBuildingValue( CEOSAICity* pCity, CEOSAIBuildingDescription* pBuildingDesc );

		CEOSAICity*  m_pAICity;
		CEOSAIBuildingDescription*  m_pBuildingDescription;

		CString m_strInternalName;

		// Cost
		//
			float m_fProductionCost;
			float m_fIronCost;

		// Benefits
		//
			// Production
			float m_fBonusProduction;
			float m_fBonusGroundUnitProduction;
			float m_fBonusSeaUnitProduction;
			float m_fBonusAirUnitProduction;

			// Repair
			float m_fRepairGroundUnitRate;
			float m_fRepairAircraftRate;
			float m_fRepairSeaUnitRate;

			// Food
			float m_fAdditionalFoodProduction;
			float m_fFoodForCityUnits;

			// Oil
			float m_fAdditionalOilProduction;
			float m_fReducedOilConsumption;

			// View Range
			float m_fCurrentViewRange;
			float m_fNewViewRange;
			float m_fRadarRange;

			// Enables Units
			CList< CEOSAIUnitTemplate* >  m_EnablesUnits; // TODO: I should probably change this to a CString and use a map.
};

class DLLIMPEXP CEOSAIBuildingDescription
{
	public:
		CEOSAIBuildingDescription( EOSAI::CGameRules* pGameRules );

		void     SetInternalName( CString str ){ m_strInternalName = str; }
		CString  GetInternalName(){ return m_strInternalName; }

		void   SetProductionCost( float f ){ m_fProductionCost = f; }
		float  GetProductionCost(){ return m_fProductionCost; }
		//void   SetIronCost( float f ){ m_fIronCost = f; }
		//float  GetIronCost(){ return m_fIronCost; }
		//float  GetProductionAndIronCost1() { return m_fProductionCost + m_fIronCost; }
		//float  GetProductionAndIronCost1() { return m_fProductionCost + m_ResourceCostList[_T("Iron")]; }
		float  GetProductionAndIronCost1() { return m_fProductionCost + 3.0f*m_ResourceCostList.CombinedResourceValues(); } // TODO: need resource multipliers (instead of the 3.0)

		float  GetProductionBonus( float fCityPopulation ){ ASSERT( false ); return 0.0f; }
		void   CanOnlyBeBuiltInCitiesAdjacentToWater( bool b ){ m_bCanOnlyBeBuiltInCitiesAdjacentToWater = b; }
		bool   CanOnlyBeBuiltInCitiesAdjacentToWater(){ return m_bCanOnlyBeBuiltInCitiesAdjacentToWater; }

		long  GetMaximumNumberOfCopiesOfThisBuildingInACity(){ ASSERT( m_iMaximumNumberOfCopiesOfThisBuildingInACity>0 ); return m_iMaximumNumberOfCopiesOfThisBuildingInACity; }

		//
		//
			EOSAI::CGameRules* m_pGameRules;

			CEOSAIUnitsubsetPermission  m_AllowUnitsubset;

			CString m_strInternalName;
			bool    m_bCanOnlyBeBuiltInCitiesAdjacentToWater;
			long    m_iMaximumNumberOfCopiesOfThisBuildingInACity;
			long    m_iNumberAllowedInNation;

			bool    m_bAutomaticallyDestroyedWhenCapturedOrTransferred;

			CString m_strUpgradeLine;
			long    m_iUpgradeLevel;

			float   m_fMinimumPopulationRequired;

		// Cost
		//
			float m_fProductionCost;
			//float m_fIronCost;
			EOSAI::ResourceAmounts  m_ResourceCostList; // e.g. The Iron cost of creating this
			EOSAI::ResourceAmounts  m_ProductionAndConsumptionPerTurnList; // e.g. A resource bonus (e.g. Capitol = +2 Money/turn) after you build this.
			//EOSAI::StringAndFloatSet  m_ResourceCostList; // e.g. The Iron cost of creating this
			//EOSAI::StringAndFloatSet  m_ProductionAndConsumptionPerTurnList; // e.g. A resource bonus (e.g. Capitol = +2 Money/turn) after you build this.
			//CList< CStringAndFloat* >  m_CostList;
			//CList< CStringAndFloat* >  m_ProductionAndConsumptionPerTurnList;

		// Benefits
		//
			// TODO: this is all very game-specific. Need to change how I handle this.

			// Production
			float m_fBonusProduction;
			float m_fProductionBonusXPopulation;
			float m_fGroundUnitProductionBonusXPopulation;//m_fBonusGroundUnitProduction;
			float m_fSeaUnitProductionBonusXPopulation;//m_fBonusSeaUnitProduction;
			float m_fAirUnitProductionBonusXPopulation;//m_fBonusAirUnitProduction;

			// Repair
			float m_fRepairGroundUnitRate;
			float m_fRepairAircraftRate;
			float m_fRepairSeaUnitRate;

			// Food
			float m_fLandFoodProductionUptoDouble; //float m_fAdditionalFoodProduction;
			float m_fFoodProvidedToUnitsInCity;//m_fFoodForCityUnits;

			// Oil
			float m_fOilProductionUptoDouble;//m_fAdditionalOilProduction;
			float m_fCityOilConsumptionReduction;//m_fReducedOilConsumption;

			// View Range
			float m_fViewRange;//m_fCurrentViewRange;
			//float m_fNewViewRange;
			float m_fRadarRange;

			// Enables Units
			CList< CEOSAIUnitTemplate* >  m_EnablesUnits;
};


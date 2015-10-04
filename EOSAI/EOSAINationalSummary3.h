
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAICitResSummary.h"
#include "EOSAIUnit2Summary.h"
#include "EOSAIBCDumbArray1D.h"
#include "PlayerResourceSummary.h"
//#include "EOSAIResourceEffectsSummary.h"

class DLLIMPEXP CEOSAINationalSummary3
{
	public:
		CEOSAINationalSummary3()
		{
			m_iPlayer = 0;
			Clear();
		}
		void SetPlayer( long iPlayer ){ m_iPlayer = iPlayer; }
		void Clear();
		/*
		{
			m_CitResSummary.Clear();
			m_UnitSummary.Clear();
			m_fMaintenanceCosts = 0.0f;
			//
			m_fResearchPointsProducedThisTurn = 0.0f;
			m_ResearchCompletedList.RemoveAll();
		}
		*/

		void  CalculatePlayerPower();
		void  CalculatePlayerToPlayerAccessibility();
		float GetPlayerAccessibilty( long iOtherPlayer ){ return m_PlayerAccessibility.Value( iOtherPlayer ); }

		// INCOMPLETE: this is correlated with military power, but not identical
		float GetPower(){ return m_CitResSummary.GetCityValue() + m_CitResSummary.GetResValue()/2.0f + m_UnitSummary.GetCombatSignificance()/20.0f; }
		//float GetMilitaryPower(){ return m_UnitSummary.GetProductionValue()/40.0f; }
		float GetCombatSignificanceDiv20(){ return m_UnitSummary.GetCombatSignificance()/20.0f; }
		//

	//
		long  m_iPlayer;

		// Resources
		void  CalculateResourceDeltas(); // void  AICalculateResourceConsumptionAndDeltas( bool bIncludeCityNoOrdersProduceWealth );
		//CEOSAIResourceEffectsSummary*  GetAIResourceEffectsSummary() { return &m_AIResourceEffectsSummary; }
		EOSAI::PlayerResourceSummary*  GetResourceSummary() { return &m_ResourceSummary; }
		/*
		float GetTotalMoney(){ ASSERT( false ); return 0.0f; }
		float GetTotalFood(){ ASSERT( false ); return 0.0f; }
		float GetTotalIron(){ ASSERT( false ); return 0.0f; }
		float GetTotalOil(){ ASSERT( false ); return 0.0f; }
		*/
		#ifdef THINGS_TO_COMPILE_EVENTUALLY
		I don't think I'm ever setting the resource totals. I need to do this, otherwise they will always be "0".
		#endif THINGS_TO_COMPILE_EVENTUALLY
		/*
		float GetTotalMoney(){ return m_TotalResources.Get( _T("Money") ); }
		float GetTotalFood(){ return m_TotalResources.Get( _T("Food") ); }
		float GetTotalIron(){ return m_TotalResources.Get( _T("Iron") ); }
		float GetTotalOil(){ return m_TotalResources.Get( _T("Oil") ); }
		*/
		float GetTotalMoney() { return m_ResourceSummary.m_TotalResources.Get(_T("Money")); }
		float GetTotalFood() { return m_ResourceSummary.m_TotalResources.Get(_T("Food")); }
		float GetTotalIron() { return m_ResourceSummary.m_TotalResources.Get(_T("Iron")); }
		float GetTotalOil() { return m_ResourceSummary.m_TotalResources.Get(_T("Oil")); }

		// Technology
		bool  AllTechnologiesHaveBeenDiscovered();//{ ASSERT( false ); return false; }
		float GetDiscoveredTechnologyResearchPoints();//{ ASSERT( false ); return 0.0f; }
		float GetResearchPointsProducedThisTurn(){ return m_fResearchPointsProducedThisTurn; }
		//float ResearchPointsProducedThisTurn(){ return m_fResearchPointsProducedThisTurn; } //{ ASSERT( false ); return 0.0f; }
		bool  GetTechnologyHasBeenDiscovered( CString strTechnology );//{ ASSERT( false ); return false; }

		CStringList* GetResearchCompletedList(){ return &m_ResearchCompletedList; }

		// City/Resource/Unit summaries
		CEOSAICitResSummary  m_CitResSummary;
		CEOSAIUnit2Summary   m_UnitSummary;
		float m_fMaintenanceCosts;

		//CEOSAIResourceState m_ResourceState; // Total resources, resource deltas (with and without orders)
		//EOSAI::StringAndFloatSet m_TotalResources;

		//CResourceEffectsSummary  m_AIResourceEffectsSummary;
		EOSAI::PlayerResourceSummary  m_ResourceSummary;
		//CEOSAIResourceEffectsSummary  m_AIResourceEffectsSummary; // TODO: Do I need this? Can I switch to EOSAI::PlayerResourceSummary? Get rid of TotalResources?

		// Unowned CitRes Accessibility 
		CEOSAIBCDumbArray1D< float >  m_PlayerAccessibility;

		float        m_fResearchPointsProducedThisTurn;
		CStringList  m_ResearchCompletedList;
};

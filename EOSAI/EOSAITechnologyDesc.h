
#pragma once

#include "EOSAIUnitsubsetPermission.h"
namespace EOSAI
{
	class GameRules;
}

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAITechnologyDesc
{
	public:
		CEOSAITechnologyDesc()
		{
			m_pGameRules = NULL;
			m_bIsBasicResearch = false;
			m_iCost = 0;
			m_fNationalLandFoodBonusMultiplier = 0.0f;
			m_fNationalSeaFoodBonusMultiplier  = 0.0f;
		}

		CString GetInternalName(){ return m_strInternalName; }

		long    GetCost(){ return m_iCost; }
		bool    IsBasicResearch(){ return m_bIsBasicResearch; }

		void    GetPrereqTechList( CStringList* pPrereqTechList );//{ ASSERT( false ); } // This is recurvively generated based on dependencies
		long    GetPrereqTechCost();

		bool    ResearchIsAllowed( CString strUnitsubset );

		// Units
		//void  AddEnableUnit( CString strInternalName ){ m_EnableUnitList.AddTail( strInternalName ); }
		//void  RemoveEnableUnit( CString strInternalName );
		bool    EnablesUnit( CString strInternalUnitName );
		//void  GetDisablesUnitList( CStringList* pList );
		bool    EnablesBuilding( CString strInternalUnitName );

		bool    EnablesBuildingOrUnitOrNationalImprovement();


		CStringList*  GetEnableBuildingList(){ return &m_EnableBuildingList; }
		CStringList*  GetEnableUnitList(){ return &m_EnableUnitList; }

		float GetNationalLandFoodBonusMultiplier(){ return m_fNationalLandFoodBonusMultiplier; }//ASSERT( false ); return 1.0f; }
		float GetNationalSeaFoodBonusMultiplier(){ return m_fNationalSeaFoodBonusMultiplier; }//ASSERT( false ); return 1.0f; }

		//
		EOSAI::CGameRules* m_pGameRules;

		CEOSAIUnitsubsetPermission  m_AllowUnitsubset;

		CString      m_strInternalName;
		CString      m_strExternalName;
		long         m_iCost;
		bool         m_bIsBasicResearch;

		float m_fNationalLandFoodBonusMultiplier;
		float m_fNationalSeaFoodBonusMultiplier;

		CStringList  m_PrereqTechList;
		//
		CStringList  m_EnableBuildingList;
		CStringList  m_DisableBuildingList;
		CStringList  m_EnableAirfieldImprovementList;
		CStringList  m_DisableAirfieldImprovementList;
		//
		CStringList  m_EnableUnitList;

};



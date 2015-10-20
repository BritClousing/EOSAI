#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

// This is the AI's equivalent to the CUnitset class

class CEOSAIMovementUnitType;
class CEOSAICombatUnitType;
class CEOSAIBuildOption;
class CEOSAIUnitTemplate;
class CEOSAIBuildingDescription;
class CEOSAITechnologyDesc;
class CEOSAITechTreeNode;

namespace EOSAI
{

class CUnitsubsetValue
{
	public:
		CUnitsubsetValue(){}
		CUnitsubsetValue( CString strUnitsubset, CString strValue ){ m_strUnitsubset = strUnitsubset; m_strValue = strValue; }

		CString m_strUnitsubset;
		CString m_strValue;
};

class DLLIMPEXP CGameRules
{
	public:
		CGameRules(){}

		// Build Options
		//
			bool  CanBuild( long iPlayer, CEOSAIBuildOption* pBuildOption, bool bAllowOldTechnologies );

		// Unitsubsets
		//
			bool           UnitsubsetExists( CString strUnitsubset );
			void           AddUnitsubset( CString strUnitsubset ){ m_UnitsubsetList.AddTail( strUnitsubset ); }
			void           DuplicateUnitsubset( CString strUnitsubsetToDuplicate, CString strUnitsubset );
			void           RemoveUnitsubset( CString strUnitsubset );
			long           GetNumberOfUnitsubsets(){ return (long) m_UnitsubsetList.GetCount(); }
			long           GetUnitsubsetIndex( CString strUnitsubset );
			CStringList*   GetUnitsubsetList(){ return &m_UnitsubsetList; }

			//void           SetInitialCanBuildUnit( EOSAI::CUnitsubsetValue* pValue );
			void           SetInitialCanBuildUnit( CString strUnitsubset, CString strInternalUnitName );
			void           SetInitialCanBuildBuilding( CString strUnitsubset, CString strInternalBuildingName );

		// MovementUnitTypes
		//
			void                           AddMovementUnitType( CEOSAIMovementUnitType* pMovementUnitType );//{ m_MovementUnitTypes.AddTail( pMovementUnitType ); }
			CEOSAIMovementUnitType*           GetMovementUnitType( long iMovementUnitType );
			CEOSAIMovementUnitType*           GetMovementUnitTypeByExternalName( CString strName );
			CList< CEOSAIMovementUnitType* >* GetMovementUnitTypes(){ return &m_MovementUnitTypes; }
			void                           RemoveMovementUnitType( long iMovementUnitType );

		// CombatUnitTypes
		//
			void                           AddCombatUnitType( CEOSAICombatUnitType* pCombatUnitType );
			CEOSAICombatUnitType*             GetCombatUnitType( long iCombatUnitType );
			CEOSAICombatUnitType*             GetCombatUnitTypeByExternalName( CString strExternalName );
			CList< CEOSAICombatUnitType* >*   GetCombatUnitTypes(){ return &m_CombatUnitTypes; }
			void                           RemoveCombatUnitType( long iCombatUnitType );
			void                           RemoveCombatUnitTypeByExternalName( CString strCombatUnitType );

		// Technology
		//
			long  GetSpendingRequiredForXResearchPoints( float fResearchPoints );
			void                   AddTechnologyDesc( CEOSAITechnologyDesc* pTech ){ m_TechnologyDescs.AddTail( pTech ); }
			CEOSAITechnologyDesc*  GetTechnologyDesc( CString str );
			CList<CEOSAITechnologyDesc*>* GetTechnologyDescsList(){ return &m_TechnologyDescs; }

			void  AddTechTreeNode( CEOSAITechTreeNode* pTechTreeNode ){ m_TechTreeNodes.AddTail( pTechTreeNode ); }
			CEOSAITechTreeNode* GetTechTreeNode( CString strTechDesc );
			void  DeleteTechTreeNodes( CString strTechDesc );
			void  RemoveFromTechTreeNodeList( CEOSAITechTreeNode* pTechTreeNode );
			CList< CEOSAITechTreeNode* >*    GetTechTreeNodes(){ return &m_TechTreeNodes; }
			CEOSAITechTreeNode*  GetFirstTechTreeNode( CString strTechDesc );

		// AIUnitTemplates
		//
			void                  AddAIUnitTemplate( CEOSAIUnitTemplate* pAIUnitTemplate );
			CEOSAIUnitTemplate*  GetAIUnitTemplate( CString strAIUnitTemplate );
			CList< CEOSAIUnitTemplate* >*  GetAIUnitTemplates(){ return &m_AIUnitTemplates; }
			void                GetAIUnitTemplates( CString strInTag, CList< CEOSAIUnitTemplate* >* pOutUnitTemplateList );

		// AICityImprovements
		//
			bool GetUnitsubsetAllowsBuilding( CString strUnitsubset, CString strBuilding );
			bool CanBuildBuilding( long iPlayer, CString strBuilding );
			void                        AddBuildingDescription( CEOSAIBuildingDescription* pBuildingDesc );
			CEOSAIBuildingDescription*  GetAIBuildingDescription( CString strAIBuildingDescription );
			CEOSAIBuildingDescription*  GetAIBuildingDescriptionFromUpgradeLineAndLevel( CString strUpgradeLine, long iUpgradeLevel );
			CList< CEOSAIBuildingDescription* >* GetBuildingDescriptionList(){ return &m_BuildingDescriptionList; }

		// Units
		//
			bool GetAllowUnit( CString strUnitsubset, CString strUnit );
			bool CanBuildUnit( long iPlayer, CString strUnit );


	private:
		CStringList  m_UnitsubsetList;

		CList< CEOSAIMovementUnitType* >  m_MovementUnitTypes;
		CList< CEOSAICombatUnitType* >    m_CombatUnitTypes;

		CList< CEOSAIUnitTemplate* >      m_AIUnitTemplates;
		CList< CUnitsubsetValue* >        m_InitialCanBuildUnitList;
		CList< CUnitsubsetValue* >        m_InitialCanBuildBuildingList;

		CList< CEOSAITechnologyDesc* >    m_TechnologyDescs;
		CList< CEOSAITechTreeNode* >      m_TechTreeNodes;

		CList< CEOSAIBuildingDescription* >  m_BuildingDescriptionList;
};
};




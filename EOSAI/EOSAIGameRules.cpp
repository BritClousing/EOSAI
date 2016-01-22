
#include "stdafx.h"
#include "EOSAIGameRules.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIBuildingDescription.h"
#include "EOSAIGamePlayer.h"
#include "EOSAICommonData.h"
#include "EOSAITechnologyDesc.h"
#include "EOSAITechTreeNode.h"
#include "EOSAINationalSummary3.h"
#include "EOSAIBuildOption.h"
#include "EOSAIMain.h"

using namespace EOSAI;
#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

bool  CGameRules::CanBuild( long iPlayer, CEOSAIBuildOption* pBuildOption, bool bAllowOldTechnologies )
{
	EOSAI::CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
	if( pGamePlayer == NULL ) return true;

	CString strUnitsubset = pGamePlayer->GetUnitsubset();

	//if( pBuildOption->IsIncProd() ) return true;
	if( pBuildOption->IsMoney() ) return true;

	if( pBuildOption->IsABuilding() )
	{
		CString strBuilding = pBuildOption->GetBuildingDescription()->GetInternalName();
		return CanBuildBuilding( iPlayer, strBuilding );
	}
	if( pBuildOption->IsAUnit() )
	{
		CString strUnit = pBuildOption->GetAIUnitTemplate()->GetInternalName();
		CString strUpgradedUnit = pBuildOption->GetAIUnitTemplate()->GetUnitsubsetUpgradeTo( strUnitsubset );

		bool bCanBuildThisUnit = CanBuildUnit(iPlayer, strUnit);
		if (bAllowOldTechnologies || strUpgradedUnit == _T(""))
		{
			return bCanBuildThisUnit; // CanBuildUnit(iPlayer, strUnit);
		}
		else
		{
			if (bCanBuildThisUnit == false) return false;

			bool bCanBuildupgradedUnit = CanBuildUnit(iPlayer, strUpgradedUnit);
			return bCanBuildThisUnit && !bCanBuildupgradedUnit;
		}
	}
	ASSERT( false );
	return false;
}

bool  CGameRules::UnitsubsetExists( CString strUnitsubset )
{
	POSITION pos = m_UnitsubsetList.GetHeadPosition();
	while( pos )
	{
		CString strUnitsubsetInList = m_UnitsubsetList.GetNext( pos );
		if( strUnitsubset == strUnitsubsetInList ) return true;
	}
	return false;
}

void  CGameRules::RemoveUnitsubset( CString strUnitsubset )
{
	POSITION pos = m_UnitsubsetList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CString strUnitsubsetInList = m_UnitsubsetList.GetNext( pos );
		if( strUnitsubsetInList == strUnitsubset )
		{
			m_UnitsubsetList.RemoveAt( prevPos );
		}
	}
}

long  CGameRules::GetUnitsubsetIndex( CString strUnitsubset )
{
	long iIndex = 1;
	POSITION pos = m_UnitsubsetList.GetHeadPosition();
	while( pos )
	{
		CString strUnitsubsetInList = m_UnitsubsetList.GetNext( pos );
		if( strUnitsubsetInList == strUnitsubset )
		{
			return iIndex;
		}
		iIndex++;
	}
	return 0;
}

//

void CGameRules::SetInitialCanBuildUnit( CString strUnitsubset, CString strInternalUnitName )// EOSAI::CUnitsubsetValue* pValue )
{
	// Debug: look for that value in the list already
	POSITION pos = m_InitialCanBuildUnitList.GetHeadPosition();
	while( pos )
	{
		EOSAI::CUnitsubsetValue* p = m_InitialCanBuildUnitList.GetNext( pos );
		if( p->m_strUnitsubset == strUnitsubset && p->m_strValue == strInternalUnitName )
		{
			return; // it's already in the list
		}
	}

	EOSAI::CUnitsubsetValue* pValue = new EOSAI::CUnitsubsetValue;
	pValue->m_strUnitsubset = strUnitsubset;
	pValue->m_strValue = strInternalUnitName;
	m_InitialCanBuildUnitList.AddTail( pValue );
}

void CGameRules::SetInitialCanBuildBuilding( CString strUnitsubset, CString strInternalBuildingName )
{
	// Debug: look for that value in the list already
	POSITION pos = m_InitialCanBuildBuildingList.GetHeadPosition();
	while( pos )
	{
		EOSAI::CUnitsubsetValue* p = m_InitialCanBuildBuildingList.GetNext( pos );
		if( p->m_strUnitsubset == strUnitsubset && p->m_strValue == strInternalBuildingName )
		{
			return; // it's already in the list
		}
	}

	EOSAI::CUnitsubsetValue* pValue = new EOSAI::CUnitsubsetValue;
	pValue->m_strUnitsubset = strUnitsubset;
	pValue->m_strValue = strInternalBuildingName;
	m_InitialCanBuildBuildingList.AddTail( pValue );
}


// MovementUnitType
//

void CGameRules::AddMovementUnitType( CEOSAIMovementUnitType* pMovementUnitType )
{
	m_MovementUnitTypes.AddTail( pMovementUnitType );
}

CEOSAIMovementUnitType* CGameRules::GetMovementUnitType( long iMovementUnitType )
{
	POSITION pos = m_MovementUnitTypes.GetHeadPosition();
	while( pos )
	{
		CEOSAIMovementUnitType* pMovementUnitType = m_MovementUnitTypes.GetNext( pos );
		if( pMovementUnitType->m_iMovementUnitType == iMovementUnitType ) return pMovementUnitType;
	}
	return NULL;
}

CEOSAIMovementUnitType*  CGameRules::GetMovementUnitTypeByExternalName( CString strExternalName )
{
	POSITION pos = m_MovementUnitTypes.GetHeadPosition();
	while( pos )
	{
		CEOSAIMovementUnitType* pMovementUnitType = m_MovementUnitTypes.GetNext( pos );
		if( pMovementUnitType->m_strExternalName == strExternalName ) return pMovementUnitType;
	}
	return NULL;
}

//
//
void CGameRules::AddCombatUnitType( CEOSAICombatUnitType* pCombatUnitType )
{
	m_CombatUnitTypes.AddTail( pCombatUnitType );
}

CEOSAICombatUnitType* CGameRules::GetCombatUnitType( long iCombatUnitType )
{
	POSITION pos = m_CombatUnitTypes.GetHeadPosition();
	while( pos )
	{
		CEOSAICombatUnitType* pCombatUnitType = m_CombatUnitTypes.GetNext( pos );
		if( pCombatUnitType->m_iCombatUnitType == iCombatUnitType ) return pCombatUnitType;
	}
	return NULL;
}

CEOSAICombatUnitType*  CGameRules::GetCombatUnitTypeByExternalName( CString strExternalName )
{
	POSITION pos = m_CombatUnitTypes.GetHeadPosition();
	while( pos )
	{
		CEOSAICombatUnitType* pCombatUnitType = m_CombatUnitTypes.GetNext( pos );
		if( pCombatUnitType->m_strExternalName == strExternalName ) return pCombatUnitType;
	}
	return NULL;
}

// Technology
//
long  CGameRules::GetSpendingRequiredForXResearchPoints( float fResearchPoints )
{
	#ifdef THINGS_TO_COMPILE_EVENTUALLY
	ASSERT( false );
	#endif THINGS_TO_COMPILE_EVENTUALLY
	/*
	for( long iDollars=0; iDollars<100; iDollars++ )
	{
		float fVal = GetResearchPoints( iDollars );
		if( fVal >= fResearchPoints ) return iDollars;
	}
	*/
	return 100;
}
/*
void  CGameRules::AddTechnologyDesc( CEOSAITechnologyDesc* pTechDesc )
{
	m_TechnologyDescs.AddTail( pTechDesc );
}
*/
CEOSAITechnologyDesc*  CGameRules::GetTechnologyDesc( CString strInternalName )
{
	POSITION pos = m_TechnologyDescs.GetHeadPosition();
	while( pos )
	{
		CEOSAITechnologyDesc* pTechNode = m_TechnologyDescs.GetNext( pos );
		if( pTechNode->GetInternalName() == strInternalName ) return pTechNode;
	}
	return NULL;
}
/*
void  CGameRules::DeleteTechnologyDesc( CEOSAITechnologyDesc* pTechDesc )
{
	POSITION pos = m_TechnologyDescs.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAITechnologyDesc* pTechDescInList = m_TechnologyDescs.GetNext( pos );
		if( pTechDescInList == pTechDesc )
		{
			m_TechnologyDescs.RemoveAt( prevPos );
		}
	}

	// Delete TechTreeNodes that reference this TechDesc
	DeleteTechTreeNodes( pTechDesc->GetInternalName() );

	delete pTechDesc;
}
*/
CEOSAITechTreeNode* CGameRules::GetTechTreeNode( CString strTechDesc )
{
	POSITION pos = m_TechTreeNodes.GetHeadPosition();
	while( pos )
	{
		CEOSAITechTreeNode* pTechTreeNode = m_TechTreeNodes.GetNext( pos );
		if( pTechTreeNode->m_strTechName == strTechDesc ){ return pTechTreeNode; }
	}
	return NULL;
}

void  CGameRules::DeleteTechTreeNodes( CString strTechDesc )
{
	POSITION pos = m_TechTreeNodes.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAITechTreeNode* pTechTreeNodeInList = m_TechTreeNodes.GetNext( pos );
		if( pTechTreeNodeInList->m_strTechName == strTechDesc )
		{
			m_TechTreeNodes.RemoveAt( prevPos );

			// Re-wire the TreeTechNode
			CEOSAITechTreeNode* pCurrentNode = pTechTreeNodeInList;
			CEOSAITechTreeNode* pCurrentNodeParent = pTechTreeNodeInList->m_pParentNode;

			if( pCurrentNodeParent )
			{
				pCurrentNodeParent->RemoveChild( pCurrentNode );
			}
			// Move my children to my parent
			while( pCurrentNode->m_ChildNodes.IsEmpty() == FALSE )
			{
				CEOSAITechTreeNode* pChild = pCurrentNode->m_ChildNodes.RemoveHead();
				pChild->m_pParentNode = pCurrentNodeParent;
				if( pCurrentNodeParent )
				{
					pCurrentNodeParent->m_ChildNodes.AddTail( pChild );
				}
			}
			//
			delete pCurrentNode;
		}
	}
}

void  CGameRules::RemoveFromTechTreeNodeList( CEOSAITechTreeNode* pTechTreeNode )
{
	POSITION pos = m_TechTreeNodes.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAITechTreeNode* pTechTreeNodeInList = m_TechTreeNodes.GetNext( pos );
		if( pTechTreeNodeInList == pTechTreeNode )
		{
			m_TechTreeNodes.RemoveAt( prevPos );
		}
	}
}

CEOSAITechTreeNode*  CGameRules::GetFirstTechTreeNode( CString strTechDesc )
{
	POSITION pos = m_TechTreeNodes.GetHeadPosition();
	while( pos )
	{
		CEOSAITechTreeNode* pTechTreeNodeInList = m_TechTreeNodes.GetNext( pos );
		if( pTechTreeNodeInList->m_strTechName == strTechDesc )
		{
			return pTechTreeNodeInList;
		}
	}
	return NULL;
}

// AIUnitTemplates
//

void  CGameRules::AddAIUnitTemplate( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	m_AIUnitTemplates.AddTail( pAIUnitTemplate );
}

CEOSAIUnitTemplate*  CGameRules::GetAIUnitTemplate( CString strAIUnitTemplate )
{
	POSITION pos = m_AIUnitTemplates.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_AIUnitTemplates.GetNext( pos );
		if( pAIUnitTemplate->GetInternalName() == strAIUnitTemplate )
		{
			return pAIUnitTemplate;
		}
	}
	return NULL;
}

void  CGameRules::GetAIUnitTemplates( CString strInTag, CList< CEOSAIUnitTemplate* >* pOutUnitTemplateList )
{
	POSITION pos = m_AIUnitTemplates.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_AIUnitTemplates.GetNext( pos );
		if( pAIUnitTemplate->UnitTagExists( strInTag ) )
		{
			pOutUnitTemplateList->AddTail( pAIUnitTemplate );
		}
	}
}

// Can I *ever* build the unit?
bool CGameRules::GetUnitsubsetAllowsBuilding( CString strUnitsubset, CString strBuilding )
{
	POSITION pos = m_BuildingDescriptionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuildingDesc = m_BuildingDescriptionList.GetNext( pos );
		if( pBuildingDesc->GetInternalName() == strBuilding )
		{
			return pBuildingDesc->m_AllowUnitsubset.UnitsubsetIsPositive( strUnitsubset );
			//ffcgf
			//return pBuildingDesc->GetAllowUnitsubsetObj()->UnitsubsetIsPositive( strUnitsubset );
		}
	}
	return false;
}

bool CGameRules::CanBuildBuilding( long iPlayer, CString strBuilding )
{
	EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
	if( pPlayer == NULL ){ ASSERT( false ); return false; }
/*
	#ifdef _DEBUG
	if( strBuilding == _T("Capitol") )
	{
		int g=0;
	}
	#endif
 */
	CString strUnitsubset = pPlayer->GetUnitsubset();

	// Can I *ever* build the unit?
	//ASSERT( false );//if( GetAllowBuilding( strUnitsubset, strBuilding ) == false ) return false;
	if( GetUnitsubsetAllowsBuilding( strUnitsubset, strBuilding ) == false ) return false;

	// Can I *initially* build the unit?
	bool bCanBuild = false;
	POSITION pos = m_InitialCanBuildBuildingList.GetHeadPosition();
	while( pos )
	{
		//CEOSAIUnitsubsetValue* pUnitsubsetValue = m_InitialCanBuildBuildingList.GetNext( pos );
		CUnitsubsetValue* pUnitsubsetValue = m_InitialCanBuildBuildingList.GetNext( pos );
		if( pUnitsubsetValue->m_strUnitsubset == strUnitsubset )
		{
			if( pUnitsubsetValue->m_strValue == strBuilding )
			{
				return true;
			}
		}
	}

	CEOSAINationalSummary3* pNationalSummary = g_pEOSAIMain->GetAICommonData()->GetAINationalSummary3( iPlayer );
	pos = m_TechnologyDescs.GetHeadPosition();
	while( pos )
	{
		CEOSAITechnologyDesc* pTechDesc = m_TechnologyDescs.GetNext( pos );
		bool bEnablesBuilding = pTechDesc->EnablesBuilding( strBuilding );
		if( bEnablesBuilding )
		{
			//if( pWorldDescPlayerProxy->GetTechnologyHasBeenDiscovered( pTechNode->GetInternalName() ) )
			if( pNationalSummary->GetTechnologyHasBeenDiscovered( pTechDesc->GetInternalName() ) )
			{
				return true;
			}
		}
	}

	return false;
}

void CGameRules::AddBuildingDescription( CEOSAIBuildingDescription* pBuildingDesc )
{
#ifdef _DEBUG
	// Make sure it doesn't exist
	POSITION pos = m_BuildingDescriptionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuildingDescInList = m_BuildingDescriptionList.GetNext( pos );
		if( pBuildingDescInList->GetInternalName() == pBuildingDesc->GetInternalName() )
		{
			ASSERT( false );
			//return pBuilding;
		}
	}
#endif _DEBUG
	m_BuildingDescriptionList.AddTail( pBuildingDesc );
}

CEOSAIBuildingDescription* CGameRules::GetAIBuildingDescription( CString strInternalName )
{
	POSITION pos = m_BuildingDescriptionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildingDescription* pBuilding = m_BuildingDescriptionList.GetNext( pos );
		if( pBuilding->GetInternalName() == strInternalName )
		{
			return pBuilding;
		}
	}
	return NULL;
}

CEOSAIBuildingDescription* CGameRules::GetAIBuildingDescriptionFromUpgradeLineAndLevel( CString strUpgradeLine, long iUpgradeLevel )
{
	POSITION pos = m_BuildingDescriptionList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIBuildingDescription* pBuilding = m_BuildingDescriptionList.GetNext( pos );
		if( pBuilding->m_strUpgradeLine == strUpgradeLine &&
			pBuilding->m_iUpgradeLevel == iUpgradeLevel )
		{
			return pBuilding;
		}
	}
	return NULL;
}


bool  CGameRules::GetAllowUnit( CString strUnitsubset, CString strUnit )
{
	POSITION pos = m_AIUnitTemplates.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = m_AIUnitTemplates.GetNext( pos );
		if( pUnitTemplate->GetInternalName() == strUnit )
		{
			return pUnitTemplate->GetAllowUnitsubsetObj()->UnitsubsetIsPositive( strUnitsubset );
		}
	}
	return false;
}

bool  CGameRules::CanBuildUnit( long iPlayer, CString strUnit )
{
	CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
	if( pGamePlayer == NULL ){ ASSERT( false ); return false; }

	CString strUnitsubset = pGamePlayer->GetUnitsubset();

	// Can I *ever* build the unit?
	if( GetAllowUnit( strUnitsubset, strUnit ) == false ) return false;

	// Can I *initially* build the unit?
	bool bCanBuild = false;
	ASSERT(m_InitialCanBuildUnitList.IsEmpty() == false);
	POSITION pos = m_InitialCanBuildUnitList.GetHeadPosition();
	while( pos )
	{
		CUnitsubsetValue* pUnitsubsetValue = m_InitialCanBuildUnitList.GetNext( pos );
		if( pUnitsubsetValue->m_strUnitsubset == strUnitsubset )
		{
			if( pUnitsubsetValue->m_strValue == strUnit )
			{
				return true;
			}
		}
	}

	// Check the technology to see if the player can build it
	/*
	CPlayerTechnologySet* pTechnologySet = NULL;
	if( GetCommonState()->GetWorldDescServer() )
	{
		GetCommonState()->GetWorldDescServer()->GetTechnologyHasBeenDiscovered( iPlayer, 
	}
	else
	{
		ASSERT( g_pGameAppState );
		CWorldDescPlayer* pWorldDescPlayer = g_pGameAppState->GetWorldDescPlayer();
		CPlayerTechnologyObject* pObject = pWorldDescPlayer->GetPlayerTechnologySet()->GetPlayerTechnologyObject( iPlayer );
		pObject->TechnologyExists( 
	}
	*/
	//POSITION pos = m_TechnologyDescs.GetHeadPosition();
	pos = m_TechnologyDescs.GetHeadPosition();
	while( pos )
	{
		CEOSAITechnologyDesc* pTechNode = m_TechnologyDescs.GetNext( pos );
		bool bEnablesUnit = pTechNode->EnablesUnit( strUnit );
		if( bEnablesUnit )
		{
			CEOSAINationalSummary3* pNationalSummary = g_pEOSAIMain->GetAICommonData()->m_AINationalSummaries[iPlayer];
			if( pNationalSummary->GetTechnologyHasBeenDiscovered( pTechNode->GetInternalName() ) )
			{
				return true;
			}
/*
			if( GetCommonState()->GetWorldDescServer() )
			{
				if( GetCommonState()->GetWorldDescServer()->GetTechnologyHasBeenDiscovered( iPlayer, pTechNode->GetInternalName() ) )
				{
					return true;
				}
			}
			else
			{
				//ASSERT( g_pGameAppState );
				ASSERT( GetCommonState() );
				ASSERT( GetCommonState()->GetLocalPlayerWorldDescList()->IsEmpty() == FALSE );

				//CWorldDescPlayer* pWorldDescPlayer = g_pCommonState->GetWorldDescPlayerByPlayerNumber( iPlayer );
				//if( pWorldDescPlayer == NULL )
				//{
				//	pWorldDescPlayer = GetCommonState()->GetLocalHumanPlayerWorldDesc();
				//}
				//if( pWorldDescPlayer == NULL )
				//{
				//	pWorldDescPlayer = GetCommonState()->GetLocalPlayerWorldDescList()->GetHead();
				//}
				CWorldDescPlayer* pWorldDescPlayer = g_pCommonState->GetLocalPlayerWorldDescList()->GetHead();
				ASSERT( pWorldDescPlayer );
				//CWorldDescPlayer* pWorldDescPlayer = GetCommonState()->GetLocalPlayerWorldDescList()->GetHead();
				//CWorldDescPlayer* pWorldDescPlayer = g_pGameAppState->GetWorldDescPlayer();
				CPlayerTechnologyObject* pObject = pWorldDescPlayer->GetPlayerTechnologySet()->GetPlayerTechnologyObject( iPlayer );
				if( pObject && pObject->TechnologyExists( pTechNode->GetInternalName() ) )
				{
					return true;
				}
				/-*
				if( pWorldDescPlayer->GetTechnologyHasBeenDiscovered( pTechNode->GetName() ) )
				{
					return true;
				}
				*-/
			}
*/
		}
	}

	/*
	CWorldDescPlayer* pWorldDescPlayer = GetCommonState()->GetWorldDescPlayerByPlayerNumber( iPlayer );
	if( pWorldDescPlayer == NULL ){ ASSERT( false ); return false; }

	pos = m_TechnologyList.GetHeadPosition();
	while( pos )
	{
		CTechnologyDesc* pTechNode = m_TechnologyList.GetNext( pos );
		bool bEnablesUnit = pTechNode->EnablesUnit( strUnit );
		//bool bDisablesUnit = pTechNode->DisablesUnit( strUnit );
		if( bEnablesUnit )
		{
			if( GetCommonState()->GetWorldDescServer() )
			{
				if( GetCommonState()->GetWorldDescServer()->GetTechnologyHasBeenDiscovered( iPlayer, pTechNode->GetName() ) )
				{
					return true;
				}
			}
			else
			{
				if( pWorldDescPlayer->GetTechnologyHasBeenDiscovered( pTechNode->GetName() ) )
				{
					return true;
				}
			}
		}
	}
	*/

	return false;
}

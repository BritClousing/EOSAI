
#include <stdafx.h>
#include "EOSAITechnologyDesc.h"
#include "EOSAITechTreeNode.h"
#include "EOSAIGameRules.h"

bool ExistsInList( CString strString, CStringList* pList )
{
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		CString strStringInList = pList->GetNext( pos );
		if( strString == strStringInList )
		{
			return true;
		}
	}
	return false;
}

void  CEOSAITechnologyDesc::GetPrereqTechList( CStringList* pPrereqTechList )
{
	if( m_bIsBasicResearch )
	{
		POSITION pos = m_PrereqTechList.GetHeadPosition();
		while( pos )
		{
			CString strPrereqTech = m_PrereqTechList.GetNext( pos );

			// See if it's already in the list
			if( ExistsInList( strPrereqTech, pPrereqTechList ) == false )
			{
				CEOSAITechnologyDesc* pChild = m_pGameRules->GetTechnologyDesc( strPrereqTech );
				if( pChild )
				{
					pPrereqTechList->AddTail( strPrereqTech );
					pChild->GetPrereqTechList( pPrereqTechList );
				}
			}
		}
	}
	else
	{
		/*
		CTechTreeNode* pCurrentTechTreeNode = m_pUnitset->GetFirstTechTreeNode( m_strInternalName );
		while( pCurrentTechTreeNode )
		{
			pCurrentTechTreeNode = pCurrentTechTreeNode->m_pParentNode;
			if( pCurrentTechTreeNode )
			{
				CString strPrereqTech = pCurrentTechTreeNode->m_strTechName;

				//pPrereqTechList->AddTail( pCurrentTechTreeNode->m_strTechName );
				// See if it's already in the list
				if( CBCList::ExistsInList( strPrereqTech, pPrereqTechList ) == false )
				{
					CTechnologyDesc* pChild = m_pUnitset->GetTechnologyDesc( strPrereqTech );
					if( pChild )
					{
						pPrereqTechList->AddTail( strPrereqTech );
					}
				}
			}
		}
		// Iterate over the list and add any prereqs
		POSITION pos = pPrereqTechList->GetHeadPosition();
		while( pos )
		{
			// Add the pre-reqs prereqs (if they aren't already in the list)
			CString strPrereqTech = pPrereqTechList->GetNext( pos );
			CTechnologyDesc* pTechDesc = m_pUnitset->GetTechnologyDesc( strPrereqTech );
			if( pTechDesc && pTechDesc->IsBasicResearch() )
			{
				pTechDesc->GetPrereqTechList( pPrereqTechList );
			}
		}
		*/

		CEOSAITechTreeNode* pCurrentTechTreeNode = m_pGameRules->GetFirstTechTreeNode( m_strInternalName );
		while( pCurrentTechTreeNode )
		{
			ASSERT( pCurrentTechTreeNode != pCurrentTechTreeNode->m_pParentNode );
			pCurrentTechTreeNode = pCurrentTechTreeNode->m_pParentNode;
			if( pCurrentTechTreeNode )
			{
				CString strPrereqTech = pCurrentTechTreeNode->m_strTechName;

				//pPrereqTechList->AddTail( pCurrentTechTreeNode->m_strTechName );
				// See if it's already in the list
				if( ExistsInList( strPrereqTech, pPrereqTechList ) == false )
				{
					CEOSAITechnologyDesc* pChild = m_pGameRules->GetTechnologyDesc( strPrereqTech );
					if( pChild )
					{
						pPrereqTechList->AddTail( strPrereqTech );
						pChild->GetPrereqTechList( pPrereqTechList );
					}
				}
			}
		}
	}
}

long  CEOSAITechnologyDesc::GetPrereqTechCost()
{
	long iCost = GetCost();

	CStringList StringList;
	GetPrereqTechList( &StringList );

	POSITION pos = StringList.GetHeadPosition();
	while( pos )
	{
		CString strTech = StringList.GetNext( pos );
		CEOSAITechnologyDesc* pTechDesc = m_pGameRules->GetTechnologyDesc( strTech );
		iCost += pTechDesc->GetCost();
	}
	return iCost;
}

bool  CEOSAITechnologyDesc::ResearchIsAllowed( CString strUnitsubset )
{
	return m_AllowUnitsubset.UnitsubsetIsPositive( strUnitsubset );
	//return GetAllowUnitsubsetObj()->UnitsubsetIsPositive( strUnitsubset );
	//return m_pGameRules->GetAllowTechnology( strUnitsubset, m_strInternalName );
}

bool  CEOSAITechnologyDesc::EnablesUnit( CString strInternalUnitName )
{
	POSITION pos = m_EnableUnitList.GetHeadPosition();
	while( pos )
	{
		CString strUnit = m_EnableUnitList.GetNext( pos );
		if( strUnit == strInternalUnitName ) return true;
	}
	return false;
}

bool CEOSAITechnologyDesc::EnablesBuilding( CString strInternalBuildingName )
{
	POSITION pos = m_EnableBuildingList.GetHeadPosition();
	while( pos )
	{
		CString strBuilding = m_EnableBuildingList.GetNext( pos );
		if( strBuilding == strInternalBuildingName ) return true;
	}
	return false;
}


bool  CEOSAITechnologyDesc::EnablesBuildingOrUnitOrNationalImprovement()
{
	//return m_EnableUnitList.IsEmpty() == false || m_EnableBuildingList.IsEmpty() == false;
	if( m_EnableUnitList.IsEmpty() == false ) return true;
	if( m_EnableBuildingList.IsEmpty() == false ) return true;
	if( m_EnableAirfieldImprovementList.IsEmpty() == false ) return true;
	if( m_fNationalLandFoodBonusMultiplier > 0.0f ) return true;
	if( m_fNationalSeaFoodBonusMultiplier > 0.0f ) return true;
	return false;
}

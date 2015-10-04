
#include "stdafx.h"
#include "EOSAIUnit2CombatCapability.h"
#include "EOSAIUnitTemplate.h"
//#include "CommonState.h"
#include "EOSAIUnitVsUnitValue.h"
//#include "AICombatPlanner.h"
#include "EOSAIQuickCombatCalculation.h"
#include "EOSAICommonData.h"
#include <math.h>

#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIUnit2CombatCapability::CEOSAIUnit2CombatCapability()
{
	m_pMyAIUnitTemplate = NULL;
	m_fAverageVisibleRange = -1.0f;
	m_fCombatAttritionSignificance01 = -1.0f;
}

CEOSAIUnit2CombatCapability::~CEOSAIUnit2CombatCapability()
{
	for( long i=0; i<m_CombatValueArray.m_iSize; i++ )
	{
		delete m_CombatValueArray.Value( i );
	}
}

CEOSAIUnit2VsUnitValue*  CEOSAIUnit2CombatCapability::InvokeCombatValue( CEOSAIUnitTemplate* pTargetTemplate )
{
	// Make sure the array is allocated
	InvokeArray();
	CEOSAIUnit2VsUnitValue* pValue = m_CombatValueArray.Value( pTargetTemplate->GetIndex() );
	if( pValue == NULL )
	{
		pValue = new CEOSAIUnit2VsUnitValue( m_pMyAIUnitTemplate, pTargetTemplate );
		m_CombatValueArray.Value( pTargetTemplate->GetIndex() ) = pValue;
	}

	return pValue;
}
/*
bool CEOSAIUnit2CombatCapability::IsACombatUnitAgainst( CUnitTemplate* pTarget )
{
	CEOSAIUnit2VsUnitValue* pUnitVsUnitValue = GetCombatValue( pTarget );
	if( pUnitVsUnitValue )
	{
		if( m_pMyUnitTemplate->IsGroundUnit() && pTarget->IsSeaUnit() ) return false;
		if( m_pMyUnitTemplate->IsSeaUnit() && pTarget->IsGroundUnit() ) return false;

		if( pUnitVsUnitValue->GetMeleeAttrition01() > 0.4f ) return true;
		if( pUnitVsUnitValue->GetRangeX2Attrition01() > 0.4f ) return true;

		if( pUnitVsUnitValue->GetMyRelativeMeleeAttritionValue01() > 0.7f ) return true;
		if( pUnitVsUnitValue->GetMyRelativeRangeX2AttritionValue01() > 0.7f ) return true;
	}
	return false;
}
*/
void CEOSAIUnit2CombatCapability::CalculateAverageVisibleRange( CEOSAIUnitTemplateSet* pEnemyUnitSet )
{
	float fCummulativeViewRange = 0.0f;
	long  iCummulativeViewRangeCount = 0;

	CEOSAIUnitTemplate* pMyUnit = m_pMyAIUnitTemplate;
	POSITION pos = pEnemyUnitSet->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pEnemyUnit = pEnemyUnitSet->m_List.GetNext( pos );
		// Combat
		//pMyUnit->GetAIUnitCombatCapability()->InvokeCombatValue( pEnemyUnit );
		// View Range
		fCummulativeViewRange += pEnemyUnit->GetViewRange( pMyUnit );
		iCummulativeViewRangeCount++;
		//pEnemyUnit->GetUnitCombatCapability()->GetCombatValue( pMyUnit );
	}
	m_fAverageVisibleRange = 0.0f;
	if( iCummulativeViewRangeCount > 0 )
	{
		m_fAverageVisibleRange = fCummulativeViewRange / iCummulativeViewRangeCount;
	}
}

float CEOSAIUnit2CombatCapability::GetAverageVisibleRange()
{
	ASSERT( m_fAverageVisibleRange >= 0.0f );
	return m_fAverageVisibleRange;
}


float CEOSAIUnit2CombatCapability::GetCombatSignificance()
{
	ASSERT( m_fCombatAttritionSignificance01 >= 0.0f );
	return m_pMyAIUnitTemplate->GetProductionAndIronCost1() * m_fCombatAttritionSignificance01;
}

void CEOSAIUnit2CombatCapability::CalculateRelativeValue( CEOSAIUnitTemplateSet* pMyUnitSet, CEOSAIUnitTemplateSet* pEnemyUnitSet )
{
	// Calculate the value of this unit relative to all my other units
	//   This is useful in calculating if this unit is useful in combat against unit X.
	//   (If the attrition is bad, but it's the best unit I've got, then I'll use it.)
	POSITION pos = pEnemyUnitSet->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pEnemyUnitTemplate = pEnemyUnitSet->m_List.GetNext( pos );
		CEOSAIUnit2VsUnitValue* pValue = InvokeCombatValue( pEnemyUnitTemplate );
	}
	/*
	POSITION pos = pEnemyUnitSet->m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pEnemyUnitTemplate = pEnemyUnitSet->m_List.GetNext( pos );

		// What is my best unit for attacking this enemy unit?
		float fBestMeleeAttritionValue01 = 0.001f;
		float fBestRangedAttritionValue01 = 0.001f;
		float fBestRangeX2AttritionValue01 = 0.001f;
		POSITION pos2 = pMyUnitSet->m_List.GetHeadPosition();
		while( pos2 )
		{
			CUnitTemplate* pMyOtherTemplate = pMyUnitSet->m_List.GetNext( pos2 );
			//
			CEOSAIUnit2VsUnitValue* pValue = pMyOtherTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( pEnemyUnitTemplate );
			fBestMeleeAttritionValue01 = max( fBestMeleeAttritionValue01, pValue->GetMeleeAttrition01() );
			fBestRangedAttritionValue01 = max( fBestRangedAttritionValue01, pValue->GetRangedAttrition01() );
			fBestRangeX2AttritionValue01 = max( fBestRangeX2AttritionValue01, pValue->GetRangeX2Attrition01() );
		}

		CEOSAIUnit2VsUnitValue* pValue = InvokeCombatValue( pEnemyUnitTemplate );
		pValue->SetMyRelativeMeleeAttritionValue01( pValue->GetMeleeAttrition01() / fBestMeleeAttritionValue01 );
		pValue->SetMyRelativeRangedAttritionValue01( pValue->GetRangedAttrition01() / fBestRangedAttritionValue01 );
		pValue->SetMyRelativeRangeX2AttritionValue01( pValue->GetRangeX2Attrition01() / fBestRangeX2AttritionValue01 );

		// DEBUG
		bool bDebugIsUseful = pValue->IsUsefulInCombat();

		int h=0;
	}
	*/

	// If a unit is significant against one type of unit, and useless against others, how does it get rated?
	// How do I differentiate that from units destroying weak units (transports)?
	float fTotalValue = 0.0f;
	float fTotalWeight = 0.0f;
	for( long i=0; i<m_CombatValueArray.m_iSize; i++ )
	{
		if( m_CombatValueArray[i] == NULL ) continue;

		float fWeight = 1.0f;
		CEOSAIUnit2VsUnitValue* pUVUValue = m_CombatValueArray[i];
		if( pUVUValue->GetAttackerUnitTemplate()->IsGroundUnit() && pUVUValue->GetTargetUnitTemplate()->IsGroundUnit() )
		{
			fWeight = 3.0f;
		}
		if( pUVUValue->GetAttackerUnitTemplate()->IsSeaUnit() && pUVUValue->GetTargetUnitTemplate()->IsSeaUnit() )
		{
			fWeight = 3.0f;
		}

		float fAtt1 = 0.25f*pUVUValue->GetMeleeAttrition01() + 0.75f*pUVUValue->GetRangeX2Attrition01();
		float fAtt2 = 0.75f*pUVUValue->GetMeleeAttrition01() + 0.25f*pUVUValue->GetRangeX2Attrition01();
		//float fAtt1 = 0.00f*pUVUValue->GetMeleeAttrition01() + 0.75f*pUVUValue->GetRangeX2Attrition01();
		//float fAtt2 = 0.50f*pUVUValue->GetMeleeAttrition01() + 0.25f*pUVUValue->GetRangeX2Attrition01();
		//float fRelAtt1 = 0.75f*pUVUValue->GetMyRelativeMeleeAttritionValue01();
		//float fRelAtt2 = 0.75f*pUVUValue->GetMyRelativeRangeX2AttritionValue01();
		float fTimeToDestroyTarget = pUVUValue->GetTimeToDestroyTarget();
		float fValue = max( fAtt1, fAtt2 );
		//float fValue = max( fAtt1, fAtt2 ) + max( fRelAtt1, fRelAtt2 );
		//fValue -= 0.2f;
		// If it takes a long time to destroy the target, start marking down the value
		if( fTimeToDestroyTarget > 2.5f ){ fValue -= 0.05f*(fTimeToDestroyTarget-2.5f); }

		if( fValue < 0.0f ) fValue = 0.0f;
		if( fValue > 1.0f ) fValue = 1.0f;

		// Weigh the higher attrition values heavier
		//   This is because some units are specialized.  
		//   AntiAircraft guns are significant, but are worthless against 80% of all units.
		fWeight += fWeight*2.0f*sqrt(fValue);

		fTotalValue += fWeight*fValue;
		fTotalWeight += fWeight;
	}
	m_fCombatAttritionSignificance01 = 0.0f;
	if( fTotalWeight > 0.0f )
	{
		// Because this is based on attrition, add-in production cost
		//m_fCombatAttritionSignificance = m_pMyUnitTemplate->GetProductionAndIronCost1() * fTotalAttrition/fTotalWeight;
		m_fCombatAttritionSignificance01 = fTotalValue/fTotalWeight;
	}
	int g=0;
}

void CEOSAIUnit2CombatCapability::InvokeArray()
{
	if( m_CombatValueArray.m_iSize == 0 )
	{
		//long iUnitTemplates = GetCommonState()->GetActiveUnitset()->GetUnitTemplateList()->GetCount();
		long iUnitTemplates = g_pEOSAIInterface->GetAIGameRules()->GetAIUnitTemplates()->GetCount();
		m_CombatValueArray.SetSize( iUnitTemplates+1 );

		for( long i=0; i<iUnitTemplates+1; i++ ){ m_CombatValueArray[i] = NULL; }
	}
}

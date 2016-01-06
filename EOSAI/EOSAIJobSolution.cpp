
#include "stdafx.h"
#include "EOSAIJobSolution.h"
#include "EOSAIJobCapability.h"
#include "EOSAIMathFunction.h"
#include "EOSAIQuickCombatCalculation.h"
#include "EOSAIUnitTemplate.h"
//#include "UnitTemplate.h"
//#include "CommonState.h"
#include <math.h>

class CFloatAndFloat
{
	public:
		CFloatAndFloat(){ m_f1 = 0.0f; m_f2 = 0.0f; }
		CFloatAndFloat( float f1, float f2 ){ m_f1 = f1; m_f2 = f2; }
		CFloatAndFloat( long i1, long i2 ){ m_f1 = (float) i1; m_f2 = (float) i2; }
		float m_f1;
		float m_f2;
};

void CEOSAIJobSolution::Clear()
{
	m_fTransportNeed = 0.0f; // How many ground units do I need to move? (One transport = 2,3,...)
	m_fCarrierNeed = 0.0f;

	m_fExpandJobSignificance = 1.0f;
	m_fCombatJobSignificance = 1.0f;
}

void CEOSAIJobSolution::MultiplyExpandJobSignificance( float f )
{
	m_fExpandJobSignificance = f;
	m_pAIJobsToDo->MultiplyExpandJobSignificance( f );
	m_fTransportNeed *= f;

	// Multiply the Militia unit values by [f]
	/*
	POSITION pos = m_pAIJobsToDo->m_UnitTemplatesAndFloat.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplateAndFloat* pUnitTemplateAndFloat = m_pAIJobsToDo->m_UnitTemplatesAndFloat.m_List.GetNext( pos );
		CEOSAIUnitTemplate* pAIUnitTemplate = pUnitTemplateAndFloat->m_pAIUnitTemplate;

		CStringList* pMilitiaUnits = g_pEOSAICommonData->GetActiveUnitset()->GetInitialMilitiaUnitList();
		bool bIsMilitia = false;
		POSITION pos2 = pMilitiaUnits->GetHeadPosition();
		while( pos2 )
		{
			CString strMilitia = pMilitiaUnits->GetNext( pos2 );
			if( pUnitTemplate->GetInternalName() == strMilitia ){ bIsMilitia = true; }
		}
		if( bIsMilitia )
		{
			pUnitTemplateAndFloat->m_fValue *= f;
		}
	}
	*/
}

void CEOSAIJobSolution::MultiplyConquestJobSignificance( float f )
{
	m_fCombatJobSignificance = f;
	m_pAIJobsToDo->MultiplyConquestJobSignificance( f );

	// Multiply NON Militia unit values by [f]
	/*
	POSITION pos = m_pAIJobsToDo->m_UnitTemplatesAndFloat.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplateAndFloat* pUnitTemplateAndFloat = m_pAIJobsToDo->m_UnitTemplatesAndFloat.m_List.GetNext( pos );
		CEOSAIUnitTemplate* pUnitTemplate = pUnitTemplateAndFloat->m_pAIUnitTemplate;

		CStringList* pMilitiaUnits = g_pEOSAICommonData->GetActiveUnitset()->GetInitialMilitiaUnitList();
		bool bIsMilitia = false;
		POSITION pos2 = pMilitiaUnits->GetHeadPosition();
		while( pos2 )
		{
			CString strMilitia = pMilitiaUnits->GetNext( pos2 );
			if( pUnitTemplate->GetInternalName() == strMilitia ){ bIsMilitia = true; }
		}
		if( bIsMilitia == false )
		{
			pUnitTemplateAndFloat->m_fValue *= f;
		}
	}
	*/
}

bool CEOSAIJobSolution::CanFulfillAJob( CEOSAIUnitTemplate* pActor )
{
	CEOSAIJobCapability JobCapability;
	JobCapability.Set( pActor );

	if( m_fTransportNeed > 0.0f && JobCapability.m_fTransportCapability > 0.0f ) return true;
	if( m_fCarrierNeed > 0.0f && JobCapability.m_fCarrierCapability > 0.0f ) return true;

	return m_pAIJobsToDo->CanFulfillAGeneralJob( &JobCapability );
}

float CEOSAIJobSolution::GetCurrentCapabilityNeed()
{
	ASSERT( m_pAIJobsToDo );
	ASSERT( m_pAIJobCapability );
	float fTotalNeed = 0.0f;

	CList< CFloatAndFloat >  NeedAndCapabilityList;
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fAirEscort,           m_pAIJobCapability->m_fAirEscort        ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fAntiAir,             m_pAIJobCapability->m_fAntiAir          ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fAntiSubmarine,       m_pAIJobCapability->m_fAntiSubmarine    ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fDestroyBuildings,    m_pAIJobCapability->m_fDestroyBuildings ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fInProductionDamage,  m_pAIJobCapability->m_fInProductionDamage ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fExploration,         m_pAIJobCapability->m_fExploration      ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fGroundAttackUnits,   m_pAIJobCapability->m_fGroundAttackUnits ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fGroundDefenseUnits,  m_pAIJobCapability->m_fGroundDefenseUnits ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fInvasionDefense,     m_pAIJobCapability->m_fInvasionDefense  ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fKillPopulation,      m_pAIJobCapability->m_fKillPopulation   ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fSeaSupremacy,        m_pAIJobCapability->m_fSeaSupremacy     ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fShoreBombardment,    m_pAIJobCapability->m_fShoreBombardment ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fSighting,            m_pAIJobCapability->m_fSighting         ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fSubmarineCapability, m_pAIJobCapability->m_fSubmarineCapability ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_iAirfieldBuilder,     m_pAIJobCapability->m_iAirfieldBuilder  ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fGroundCitResHunter,  m_pAIJobCapability->m_fGroundCitResHunter ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fSeaResHunter,        m_pAIJobCapability->m_fSeaResHunter     ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_fTransportNeed, m_pAIJobCapability->m_fTransportCapability ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_fCarrierNeed,   m_pAIJobCapability->m_fCarrierCapability ) );

	POSITION pos = NeedAndCapabilityList.GetHeadPosition();
	while( pos )
	{
		CFloatAndFloat NeedAndCapability = NeedAndCapabilityList.GetNext( pos );
		fTotalNeed += NeedAndCapability.m_f1;
	}

	return fTotalNeed;
}

float CEOSAIJobSolution::GetCurrentCapabilityValue()
{
	ASSERT( m_pAIJobsToDo );
	ASSERT( m_pAIJobCapability );
	float fCapability = 0.0f;
	float fTotalCapability = 0.0f;
	float fValue = 0.0f;
	float fNeed = 0.0f;

	CEOSAIMathFunction CapabilityNeedRatioValue;
	CapabilityNeedRatioValue.SetInputOutput( 0.0f, 0.0f );
	CapabilityNeedRatioValue.SetInputOutput( 0.2f, 0.5f );
	CapabilityNeedRatioValue.SetInputOutput( 0.5f, 0.7f );
	CapabilityNeedRatioValue.SetInputOutput( 1.0f, 1.0f );
	CapabilityNeedRatioValue.SetInputOutput( 2.0f, 1.5f );
	CapabilityNeedRatioValue.SetInputOutput( 3.0f, 1.8f );
	CapabilityNeedRatioValue.SetInputOutput( 4.0f, 1.9f );
	CapabilityNeedRatioValue.SetInputOutput( 5.0f, 1.95f);
	CapabilityNeedRatioValue.SetInputOutput( 6.0f, 1.98f);
	CapabilityNeedRatioValue.SetInputOutput( 9.0f, 2.00f);

	CList< CFloatAndFloat >  NeedAndCapabilityList;
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fAirEscort,           m_pAIJobCapability->m_fAirEscort        ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fAntiAir,             m_pAIJobCapability->m_fAntiAir          ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fAntiSubmarine,       m_pAIJobCapability->m_fAntiSubmarine    ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fDestroyBuildings,    m_pAIJobCapability->m_fDestroyBuildings ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fInProductionDamage,  m_pAIJobCapability->m_fInProductionDamage ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fExploration,         m_pAIJobCapability->m_fExploration      ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fGroundAttackUnits,   m_pAIJobCapability->m_fGroundAttackUnits ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fGroundDefenseUnits,  m_pAIJobCapability->m_fGroundDefenseUnits ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fInvasionDefense,     m_pAIJobCapability->m_fInvasionDefense  ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fKillPopulation,      m_pAIJobCapability->m_fKillPopulation   ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fSeaSupremacy,        m_pAIJobCapability->m_fSeaSupremacy     ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fShoreBombardment,    m_pAIJobCapability->m_fShoreBombardment ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fSighting,            m_pAIJobCapability->m_fSighting         ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fSubmarineCapability, m_pAIJobCapability->m_fSubmarineCapability ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_iAirfieldBuilder,     m_pAIJobCapability->m_iAirfieldBuilder  ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fGroundCitResHunter,  m_pAIJobCapability->m_fGroundCitResHunter ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_pAIJobsToDo->m_fSeaResHunter,        m_pAIJobCapability->m_fSeaResHunter     ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_fTransportNeed, m_pAIJobCapability->m_fTransportCapability ) );
	NeedAndCapabilityList.AddTail( CFloatAndFloat( m_fCarrierNeed,   m_pAIJobCapability->m_fCarrierCapability ) );

	POSITION pos = NeedAndCapabilityList.GetHeadPosition();
	while( pos )
	{
		CFloatAndFloat NeedAndCapability = NeedAndCapabilityList.GetNext( pos );

		fNeed = NeedAndCapability.m_f1;
		fCapability = NeedAndCapability.m_f2;
		if( fCapability > 0.0f && fNeed > 0.0f )
		{
			float fCapabilityToNeedRatio = fCapability / fNeed;
			//fTotalCapability += sqrt( fCapabilityToNeedRatio ) * fNeed;
			fTotalCapability += CapabilityNeedRatioValue.GetOutput( fCapabilityToNeedRatio ) * sqrt( fNeed );
		}
	}

	CEOSAIUnitTemplatesAndFloat& MyUnits = m_pAIJobCapability->m_UnitTemplatesAndFloat;
	CEOSAIUnitTemplatesAndFloat& EnemyUnits = m_pAIJobsToDo->m_UnitTemplatesAndFloat;
	float fDamageToEnemyProduction = 0.0f;
	float fDamageToEnemyCombatCapability = 0.0f;
	CEOSAIUnitTemplatesAndFloat  NationalCombatResults;
	CEOSAIQuickCombatCalculation::QuickCombatResults( MyUnits, EnemyUnits, NationalCombatResults );
	CEOSAIQuickCombatCalculation::QuickCombatDamageSqrt( EnemyUnits, NationalCombatResults, &fDamageToEnemyProduction, &fDamageToEnemyCombatCapability );

	float fTotalEnemyCost = 0.0f;
	pos = EnemyUnits.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplateAndFloat* pAIUnitTemplateAndFloat = EnemyUnits.m_List.GetNext( pos );
		CEOSAIUnitTemplate* pAIUnitTemplate = pAIUnitTemplateAndFloat->m_pAIUnitTemplate;

		fTotalEnemyCost += pAIUnitTemplateAndFloat->m_fValue * pAIUnitTemplateAndFloat->m_pAIUnitTemplate->GetProductionCost();
	}

	//m_pAIJobCapability->m_UnitTemplatesAndFloat.Add( pUnitTemplate, -1.0f );
	//
	//float fDeltaNationalProductionDamage = fNewDamageToEnemyProduction - fCurrentDamageToEnemyProduction;
	//float fDeltaNationalCombatDamage     = fNewDamageToEnemyCombatCapability - fCurrentDamageToEnemyCombatCapability;
	//fCombatValue = 
	//	0.3f * (( fThisUnitDamageToEnemyProduction + fThisUnitDamageToEnemyCombatCapability ) / pUnitTemplate->GetProductionAndIronCost1()) +
	//	0.7f * (( fDeltaNationalProductionDamage + fDeltaNationalCombatDamage ) / pUnitTemplate->GetProductionAndIronCost1()) +
	//	0.3f * pUnitTemplate->GetUnitCombatCapability()->GetCombatAttritionSignificance01();
	float fCombatValue = 
		sqrt( 0.2f*fTotalEnemyCost ) *
		( 0.5f*fDamageToEnemyProduction + 0.5f*fDamageToEnemyCombatCapability ) / (0.1f + fTotalEnemyCost);
	fTotalCapability += fCombatValue;

	return fTotalCapability;
}

float CEOSAIJobSolution::GetValueOfAddingToCapability( CEOSAIUnitTemplate* pActor )
{
	float fOriginalValue = GetCurrentCapabilityValue();
	m_pAIJobCapability->Add( pActor );
	float fNewValue = GetCurrentCapabilityValue();
	m_pAIJobCapability->Remove( pActor );
	return fNewValue - fOriginalValue;
}

/*
bool CEOSAIJobSolution::CanFulfillACombatJob( CUnitTemplate* pActor )
{
	ASSERT( m_pAIUnitNeed );
	if( pActor->GetUnitCombatCapability()->IsACombatUnit() == false ) return false;

	POSITION pos = m_pAIUnitNeed->m_Enemies.GetHeadPosition();
	while( pos )
	{
		CUnit* pEnemy = m_pAIUnitNeed->m_Enemies.GetNext( pos );
		if( pActor->GetUnitCombatCapability()->GetCombatValue( pEnemy->GetUnitTemplate() )->IsUsefulInCombat() )
		{
			return true;
		}
	}
	return false;
}

bool CEOSAIJobSolution::CanFulfillANonCombatJob( CUnitTemplate* pActor )
{
	// Primary Jobs
	if( m_iGroundCitResHunter > 0 && pCapability->m_iGroundCitResHunter > 0 ) return true;
	if( m_iSeaResHunter       > 0 && pCapability->m_iSeaResHunter > 0 ) return true;
	if( m_iDegradeCity        > 0 && pCapability->m_iDegradeCity > 0 ) return true;
	if( m_iAirfieldBuilder    > 0 && pCapability->m_iAirfieldBuilder > 0 ) return true;
	return false;
}

bool CEOSAIJobSolution::CanFulfillANonCombatJob( CEOSAIUnitCapability* pAIUnitCapability )
{
	// Primary Jobs
	if( m_iGroundCitResHunter > 0 && pCapability->m_iGroundCitResHunter > 0 ) return true;
	if( m_iSeaResHunter       > 0 && pCapability->m_iSeaResHunter > 0 ) return true;
	if( m_iDegradeCity        > 0 && pCapability->m_iDegradeCity > 0 ) return true;
	if( m_iAirfieldBuilder    > 0 && pCapability->m_iAirfieldBuilder > 0 ) return true;
	return false;
}
*/
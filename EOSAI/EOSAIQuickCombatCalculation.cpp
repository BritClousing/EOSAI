
#include "stdafx.h"
#include "EOSAIQuickCombatCalculation.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIUnit2.h"
#include "PoiObject.h"
#include "EOSAIUnitTemplatesAndFloat.h"
#include "EOSAIUnitVsUnitValue.h"
#include <Math.h>
#include "EOSAIMathFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIQuickCombatUnit::CEOSAIQuickCombatUnit( CEOSAIUnit2* pAIUnit )
{
	m_pAIUnit = pAIUnit;
	m_pAIUnitTemplate = pAIUnit->GetAIUnitTemplate();
	m_pCurrentBestTarget = NULL;
	//m_bIsDead = false;
	m_fInitialHP = (float)pAIUnit->GetCurrentHP();
	m_fCurrentHP = (float)pAIUnit->GetCurrentHP();
	m_bAggressive = false;
	if( pAIUnit->GetOwner() > 0 ) m_bAggressive = true; // Players will counterattack, militia will not
	//m_fAbilityToDamageToEnemyForce = 0.0f;
}

CEOSAIQuickCombatUnit::CEOSAIQuickCombatUnit( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	m_pAIUnit = NULL;
	m_pAIUnitTemplate = pAIUnitTemplate;
	m_pCurrentBestTarget = NULL;
	//m_bIsDead = false;
	m_fInitialHP = (float) pAIUnitTemplate->GetMaxHP();
	m_fCurrentHP = (float) pAIUnitTemplate->GetMaxHP();
	m_bAggressive = true; // UnitTemplates are typically built units (which means they can be aggressive)
	//m_fAbilityToDamageToEnemyForce = 0.0f;
}

///////////////////////////////////////////////////////////


void  CEOSAIQuickCombatCalculation::QuickCombatResults( 
		CEOSAIUnit2TemplatesAndFloat& MyUnits, CEOSAIUnit2TemplatesAndFloat& EnemyUnits, 
		CEOSAIUnit2TemplatesAndFloat& CombatResults )
{
	//ASSERT( pCombatResults );
	CombatResults.Clear();
	CEOSAIUnit2TemplatesAndFloat  CombatPowerAppliedAgainstEnemyUnits;

	POSITION pos = EnemyUnits.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pEnemy = EnemyUnits.m_List.GetNext( pos );

		// What power can I throw against this enemy unit?
		float fTotalCombatPower = 0.0f;
		POSITION pos2 = MyUnits.m_List.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnit2TemplateAndFloat* pMyUnit = MyUnits.m_List.GetNext( pos2 );

			// Skip the Ground->Sea and Sea->Ground attacks
			if( pMyUnit->m_pAIUnitTemplate->IsGroundUnit() && pEnemy->m_pAIUnitTemplate->IsSeaUnit() ) continue;
			if( pMyUnit->m_pAIUnitTemplate->IsSeaUnit()    && pEnemy->m_pAIUnitTemplate->IsGroundUnit() ) continue;
			// Skip power calculations for any of my units that aren't combat units
			//if( pMyUnit->m_pUnitTemplate->GetUnitCombatCapability()->IsACombatUnit() == false ) continue;
			if( pMyUnit->m_pAIUnitTemplate->IsACombatUnit() == false ) continue;

			fTotalCombatPower +=
				pMyUnit->m_fValue *
				pMyUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( pEnemy->m_pAIUnitTemplate )->GetCombatSignificance();
		}
		CombatPowerAppliedAgainstEnemyUnits.Set( pEnemy->m_pAIUnitTemplate, fTotalCombatPower ); // Record the Total Combat Significance
	}

	// Right now, there's a quirk in the way PowerLevels work
	// Specifically, each of MyUnits gets to apply power against each type of Enemy unit
	//   If EnemyUnits = Infantry + Militia, I have 20 power against *each* of those units
	//   If EnemyUnits = Infantry + Infantry, I have 20 power against Infantry alone
	//   It appears as if I am much more powerful against the Infantry+Militia combo, when I'm actually not
	// It's necessary to normalize these values so that I don't get to apply power against each individually
	// How do I do this?
	//   I want each of MyUnits to contribute one unit's worth of power (not one unit's worth against each enemy category)
	//   Compile all my units power (each of my units contributes fully to each enemy category)
	//   Then, figure out how much power I spend on each category (if I'm doing overkill, then remember that)
	//   Now, spread each of MyUnit's power to all categories, but divide this power so that it adds up to 
	//     one unit's worth of power, and use the power-excesses to also limit the unit's power distribution

	// Calculate the Normalized power levels
	CEOSAIUnit2TemplatesAndFloat  NormalizedPowerAppliedAgainstEnemyUnits;
	pos = CombatPowerAppliedAgainstEnemyUnits.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pCombatPowerAgainstEnemyUnit = CombatPowerAppliedAgainstEnemyUnits.m_List.GetNext( pos );

		float fCurrentNumberOfEnemyUnits = EnemyUnits.GetValue( pCombatPowerAgainstEnemyUnit->m_pAIUnitTemplate );
		float fEnemyUnitProductionCost = pCombatPowerAgainstEnemyUnit->m_pAIUnitTemplate->GetProductionCost();
		float fTotalProductionCost = fCurrentNumberOfEnemyUnits * fEnemyUnitProductionCost;

		float fNormalizedPower = pCombatPowerAgainstEnemyUnit->m_fValue / fTotalProductionCost;
		if( fNormalizedPower > 0.0f )
		{
			NormalizedPowerAppliedAgainstEnemyUnits.Set( pCombatPowerAgainstEnemyUnit->m_pAIUnitTemplate, fNormalizedPower );
		}
	}

	// Clear my values
	//CombatResults.SetAllValuesToZero();

	// Now, iterate over the list, distrubute combat power over targets, taking NormalizedPower into account
	pos = MyUnits.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pMyUnit = MyUnits.m_List.GetNext( pos );

		// Skip power calculations for any of my units that aren't combat units
		if( pMyUnit->m_pAIUnitTemplate->IsACombatUnit() == false ) continue;

		// How do I apply this unit's power over the Enemies?
		// First, add-up all the values
		float fUnitPowerMultiplesSpent = 0.0f;
		POSITION pos2 = EnemyUnits.m_List.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnit2TemplateAndFloat* pEnemy = EnemyUnits.m_List.GetNext( pos2 );

			// Skip the Ground->Sea and Sea->Ground attacks
			if( pMyUnit->m_pAIUnitTemplate->IsGroundUnit() && pEnemy->m_pAIUnitTemplate->IsSeaUnit() ) continue;
			if( pMyUnit->m_pAIUnitTemplate->IsSeaUnit()    && pEnemy->m_pAIUnitTemplate->IsGroundUnit() ) continue;

			float fNormalizedCombatAgainst = NormalizedPowerAppliedAgainstEnemyUnits.GetValue( pEnemy->m_pAIUnitTemplate );
			if( pMyUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( pEnemy->m_pAIUnitTemplate )->GetCombatSignificance() > 0.0f )
			{
				fUnitPowerMultiplesSpent += min( 1.0f, 1.0f/fNormalizedCombatAgainst );
			}
		}
		// Now, apply all the values
		pos2 = EnemyUnits.m_List.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnit2TemplateAndFloat* pEnemy = EnemyUnits.m_List.GetNext( pos2 );
			float fNormalizedPowerAgainstUnit = NormalizedPowerAppliedAgainstEnemyUnits.GetValue( pEnemy->m_pAIUnitTemplate );

			// Skip the Ground->Sea and Sea->Ground attacks
			if( pMyUnit->m_pAIUnitTemplate->IsGroundUnit() && pEnemy->m_pAIUnitTemplate->IsSeaUnit() ) continue;
			if( pMyUnit->m_pAIUnitTemplate->IsSeaUnit()    && pEnemy->m_pAIUnitTemplate->IsGroundUnit() ) continue;

			if( pMyUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( pEnemy->m_pAIUnitTemplate )->GetCombatSignificance() > 0.0f )
			{
				// If I kill this unit 10x over, just spend 1/10th
				float fPowerToSpend1 = 1.0f / max( 1.0f, fNormalizedPowerAgainstUnit );
				float fPowerToSpend2 = 1.0f / max( 1.0f, fUnitPowerMultiplesSpent );
				float fPowerToSpend  = min( fPowerToSpend1, fPowerToSpend2 );

				// Measured in production
				float fAppliedPower =
					pMyUnit->m_fValue *
					pMyUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( pEnemy->m_pAIUnitTemplate )->GetCombatSignificance()
					* fPowerToSpend;
				/*
				float fAppliedPower =
					pMyUnit->m_fValue *
					pMyUnit->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatValue( pEnemy->m_pUnitTemplate )->GetCombatSignificance()
					/ max( 1.0f, fNormalizedPowerAgainstUnit );
					/ max( 1.0f, fUnitPowerMultiplesSpent );
				*/

				CombatResults.Add( pEnemy->m_pAIUnitTemplate, fAppliedPower );
			}
		}
	}
	int g=0;

	// Now, normalize the values so that I can see if I have sufficient force to overcome each enemy
	pos = CombatResults.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pCombatPowerAgainstUnit = CombatResults.m_List.GetNext( pos );

		float fNumberOfEnemyUnits = EnemyUnits.GetValue( pCombatPowerAgainstUnit->m_pAIUnitTemplate );
		float fEnemyUnitCost = pCombatPowerAgainstUnit->m_pAIUnitTemplate->GetProductionCost();
		float fExistingEnemyPower = fNumberOfEnemyUnits * fEnemyUnitCost;

		pCombatPowerAgainstUnit->m_fValue /= fExistingEnemyPower;
		if( pCombatPowerAgainstUnit->m_fValue < 0.0f ) pCombatPowerAgainstUnit->m_fValue = 0.0f;
		if( pCombatPowerAgainstUnit->m_fValue > 1.0f ) pCombatPowerAgainstUnit->m_fValue = 1.0f;

		int h=0;
	}
}

/*
void  CEOSAIQuickCombatCalculation::QuickCombatDamageNormal( CEOSAIUnit2TemplatesAndFloat* pCombatResults, float* pfTotalDamageToEnemyProduction, float* pfTotalDamageToEnemyCombatCapability )
{
	ASSERT( &pfTotalDamageToEnemyProduction );
	ASSERT( &pfTotalDamageToEnemyCombatCapability );

	float fTotalDamageToEnemyProduction = 0.0f;
	float fTotalDamageToEnemyCombatCapability = 0.0f;
	pos = pCombatResults->m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplateAndFloat* pDestruction = pCombatResults->m_List.GetNext( pos );
		//
		fTotalDamageToEnemyProduction +=
			pDestruction->m_fValue *
			pDestruction->m_pUnitTemplate->GetProductionAndIronCost1() *
			EnemyUnits.GetValue( pDestruction->m_pUnitTemplate );
		fTotalDamageToEnemyCombatCapability +=
			pDestruction->m_fValue *
			pDestruction->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatSignificance() *
			EnemyUnits.GetValue( pDestruction->m_pUnitTemplate );
	}
	if( pfTotalDamageToEnemyProduction ){ *pfTotalDamageToEnemyProduction = fTotalDamageToEnemyProduction; }
	if( pfTotalDamageToEnemyCombatCapability ){ *pfTotalDamageToEnemyCombatCapability = fTotalDamageToEnemyCombatCapability; }
}
*/
void  CEOSAIQuickCombatCalculation::QuickCombatDamageSqrt( CEOSAIUnit2TemplatesAndFloat& EnemyUnits, CEOSAIUnit2TemplatesAndFloat& CombatResults, float* pfTotalDamageToEnemyProduction, float* pfTotalDamageToEnemyCombatCapability )
{
	ASSERT( &pfTotalDamageToEnemyProduction );
	ASSERT( &pfTotalDamageToEnemyCombatCapability );

	float fTotalDamageToEnemyProduction = 0.0f;
	float fTotalDamageToEnemyCombatCapability = 0.0f;
	POSITION pos = CombatResults.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pDestruction = CombatResults.m_List.GetNext( pos );
		//
		fTotalDamageToEnemyProduction +=
			sqrt( pDestruction->m_fValue ) *
			pDestruction->m_pAIUnitTemplate->GetProductionAndIronCost1() *
			EnemyUnits.GetValue( pDestruction->m_pAIUnitTemplate );
		fTotalDamageToEnemyCombatCapability +=
			sqrt( pDestruction->m_fValue ) *
			pDestruction->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance() *
			EnemyUnits.GetValue( pDestruction->m_pAIUnitTemplate );
	}
	if( pfTotalDamageToEnemyProduction ){ *pfTotalDamageToEnemyProduction = fTotalDamageToEnemyProduction; }
	if( pfTotalDamageToEnemyCombatCapability ){ *pfTotalDamageToEnemyCombatCapability = fTotalDamageToEnemyCombatCapability; }
}
/*
float CEOSAIQuickCombatCalculation::GetCombatWinCalc01( CUnit* pAttacker, CUnit* pDefender )
{
	float fTimeToDestroyDefender = 0.0f;
	float fHPDamageToAttacker = pDefender->GetUnitTemplate()->GetExpectedHPDamage( pAttacker->GetUnitTemplate() );
	float fHPDamageToDefender = pAttacker->GetUnitTemplate()->GetExpectedHPDamage( pDefender->GetUnitTemplate() );
	if( fHPDamageToAttacker == 0.0f && fHPDamageToDefender == 0.0f ){ return 0.0f; }
	if( fHPDamageToAttacker >  0.0f && fHPDamageToDefender == 0.0f ){ return 0.0f; }
	if( fHPDamageToAttacker == 0.0f && fHPDamageToDefender >  0.0f ){ return 1.0f; }
	if( fHPDamageToAttacker >  0.0f && fHPDamageToDefender >  0.0f )
	{
		float fTimeToKillAttacker = (float) pAttacker->GetCurrentHP() / fHPDamageToAttacker;
		float fTimeToKillDefender = (float) pDefender->GetCurrentHP() / fHPDamageToDefender;
		return fTimeToKillAttacker / ( fTimeToKillAttacker + fTimeToKillDefender );
	}
	ASSERT( false );
	return 0.0f;
}
*/
float CEOSAIQuickCombatCalculation::GetCombatRangeAdvantage( EOSAI::PoiMobile* pAttacker, EOSAI::PoiMobile* pDefender )
{
	float fAttackerRange = pAttacker->GetAttackRange( pDefender, pDefender->IsLandedOrDocked() );
	float fDefenderRange = pDefender->GetAttackRange( pAttacker, false );
	return fAttackerRange - fDefenderRange;
}
/*
float CEOSAIQuickCombatCalculation::GetCombatRangeAdvantage( CUnit* pAttacker, CUnit* pDefender )
{
	float fAttackerRange = pAttacker->GetAttackRange( pDefender, pDefender->IsLandedOrDocked() );
	float fDefenderRange = pDefender->GetAttackRange( pAttacker, false );
	return fAttackerRange - fDefenderRange;
}
*/
///////////////////////////////////////////////////////////

CEOSAIQuickCombatCalculation::CEOSAIQuickCombatCalculation()
{
	m_bLimitCombatByLandAndWaterBarriers = true;
	ClearResults();

	// Clear inputs
	m_fProDefenderBias = 1.0f;
	//m_bBiasAgainstAttacker = false;
	//m_bBiasAgainstDefender = false;
	m_bHandleDefenderCounterAttack = true;
}

CEOSAIQuickCombatCalculation::~CEOSAIQuickCombatCalculation()
{
	while( m_AttackerUnits.IsEmpty() == FALSE ){ delete m_AttackerUnits.RemoveHead(); }
	while( m_DefenderUnits.IsEmpty() == FALSE ){ delete m_DefenderUnits.RemoveHead(); }
}

void CEOSAIQuickCombatCalculation::ClearAll()
{
	while( m_AttackerUnits.IsEmpty() == FALSE ){ delete m_AttackerUnits.RemoveHead(); }
	while( m_DefenderUnits.IsEmpty() == FALSE ){ delete m_DefenderUnits.RemoveHead(); }
	ClearResults();
}

void CEOSAIQuickCombatCalculation::ClearResults()
{
	// Combat Result
	//m_fCombatWinBalance01 = 0.5f; // 0.0 = Favors Player1, 0.5 = Equal Power, 1.0 = Favors Player2
	m_fAttritionBalance01 = 0.0f;
	//
	m_fCompletionTime = 0.0f;
	// Attrition
	m_fProductionCostOfAttacker = 0.0f;
	m_fProductionDamageToAttacker = 0.0f;
	m_fProductionCostOfDefender = 0.0f;
	m_fProductionDamageToDefender = 0.0f;
}

void CEOSAIQuickCombatCalculation::SetProDefenderBias( float fBias )
{
	m_fProDefenderBias = fBias;
}

// Initialization
void CEOSAIQuickCombatCalculation::AddToAttackers( CEOSAIPoiObject* pPoiObject )
{
	CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pPoiObject );
	if( pUnit )
	{
		CEOSAIQuickCombatUnit* pQUnit = new CEOSAIQuickCombatUnit(pUnit);
		m_AttackerUnits.AddTail( pQUnit );
	}
	else
	{
		//ASSERT( false );
		//POSITION pos = pPoiObject->GetUnitsInsideMe()->GetHeadPosition();
		POSITION pos = pPoiObject->GetInitialState()->GetContaineesList()->GetHeadPosition();
		while( pos )
		{
			//CEOSAIPoiObject* pChild = pPoiObject->GetUnitsInsideMe()->GetNext( pos );
			EOSAI::PoiMobile* pChild = pPoiObject->GetInitialState()->GetContaineesList()->GetNext( pos );
			AddToAttackers( pChild );
		}
	}
}

void CEOSAIQuickCombatCalculation::AddToAttackers( CEOSAIUnitTemplate* pUnitTemplate )
{
	ASSERT( pUnitTemplate );
	m_AttackerUnits.AddTail( new CEOSAIQuickCombatUnit(pUnitTemplate) );
}

void CEOSAIQuickCombatCalculation::RemoveFromAttackers( CEOSAIUnitTemplate* pUnitTemplate )
{
	ASSERT( pUnitTemplate );
	POSITION pos = m_AttackerUnits.GetTailPosition();
	while( pos )
	{
		POSITION PrevPos = pos;
		CEOSAIQuickCombatUnit* pAIQuickCombatUnit = m_AttackerUnits.GetPrev( pos );
		if( pAIQuickCombatUnit->m_pAIUnitTemplate == pUnitTemplate )
		{
			m_AttackerUnits.RemoveAt( PrevPos );
		}
	}
}

void CEOSAIQuickCombatCalculation::RemoveFromAttackers( CEOSAIPoiObject* pPoiObject )
{
	CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pPoiObject );
	if( pUnit )
	{
		//CEOSAIQuickCombatUnit* pQUnit = new CEOSAIQuickCombatUnit(pUnit);
		//m_DefenderUnits.AddTail( pQUnit );
		POSITION pos = m_AttackerUnits.GetTailPosition();
		while( pos )
		{
			POSITION PrevPos = pos;
			CEOSAIQuickCombatUnit* pAIQuickCombatUnit = m_AttackerUnits.GetPrev( pos );
			if( pAIQuickCombatUnit->m_pAIUnit == pUnit )
			{
				m_AttackerUnits.RemoveAt( PrevPos );
			}
		}
	}
	else
	{
		ASSERT( false );
		/*
		POSITION pos = pPoiObject->GetUnitsInsideMe()->GetHeadPosition();
		while( pos )
		{
			CPoiObject* pChild = pPoiObject->GetUnitsInsideMe()->GetNext( pos )->GetPtr();
			AddToDefenders( pChild );
		}
		*/
	}
}

void CEOSAIQuickCombatCalculation::AddToDefenders( CEOSAIPoiObject* pPoiObject )
{
	CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pPoiObject );
	if( pUnit )
	{
		CEOSAIQuickCombatUnit* pQUnit = new CEOSAIQuickCombatUnit(pUnit);
		m_DefenderUnits.AddTail( pQUnit );
	}
	else
	{
		//POSITION pos = pPoiObject->GetUnitsInsideMe()->GetHeadPosition();
		POSITION pos = pPoiObject->GetInitialState()->GetContaineesList()->GetHeadPosition();
		while( pos )
		{
			//CEOSAIPoiObject* pChild = pPoiObject->GetUnitsInsideMe()->GetNext( pos );
			EOSAI::PoiMobile* pChild = pPoiObject->GetInitialState()->GetContaineesList()->GetNext( pos );
			AddToDefenders( pChild );
		}
	}
}

/*
void CEOSAIQuickCombatCalculation::AddToDefenders( CUnit* pUnit )
{
	ASSERT( pUnit );
	CEOSAIQuickCombatUnit* pQUnit = new CEOSAIQuickCombatUnit(pUnit);
	m_DefenderUnits.AddTail( pQUnit );
}
*/
void CEOSAIQuickCombatCalculation::AddToDefenders( CEOSAIUnitTemplate* pUnit )
{
	ASSERT( pUnit );
	CEOSAIQuickCombatUnit* pQUnit = new CEOSAIQuickCombatUnit(pUnit);
	m_DefenderUnits.AddTail( pQUnit );
}

void CEOSAIQuickCombatCalculation::RunCalculation()
{
	if( m_AttackerUnits.IsEmpty() ) return;
	if( m_DefenderUnits.IsEmpty() ) return;

	// Reset the CurrentHP to the initial value (in case I've run this calculation before)
	POSITION pos;
	pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pQUnit = m_AttackerUnits.GetNext( pos );
		pQUnit->m_pCurrentBestTarget = NULL;
		pQUnit->m_fCurrentHP = pQUnit->m_fInitialHP;
	}
	pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pQUnit = m_DefenderUnits.GetNext( pos );
		pQUnit->m_pCurrentBestTarget = NULL;
		pQUnit->m_fCurrentHP = pQUnit->m_fInitialHP;
	}

	// Modify the defense values to add a bias towards one player
	float fUnitCount = (float) ( m_AttackerUnits.GetCount() + m_DefenderUnits.GetCount() );

	// A bias of 2 means I need twice as many attackers as defenders to equal parity.
	//   However, if there are 20 attackers vs 10 defenders, I will overwhelm him.
	//   But, if there are 2 attackers and 1 defender, I will simply be playing it safe.
	//   Lower numbers of units means I need a bigger bias.
	float fTotalDefenderHP = 0.0f;
	pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		fTotalDefenderHP += pUnit->m_fInitialHP;
	}
	CEOSAIMathFunction  DefenderHPBiasMultiplierFunc;
	DefenderHPBiasMultiplierFunc.SetInputOutput(  0.0f,1.3f );
	DefenderHPBiasMultiplierFunc.SetInputOutput(  5.0f,1.3f );
	DefenderHPBiasMultiplierFunc.SetInputOutput( 20.0f,1.2f );
	DefenderHPBiasMultiplierFunc.SetInputOutput( 50.0f,1.0f );

	float fHitAttackerHPDamageMult = DefenderHPBiasMultiplierFunc.GetOutput(fTotalDefenderHP) * m_fProDefenderBias;
	float fHitDefenderHPDamageMult = 1.0f;//1.0f/m_fProDefenderBias; //1.0f;
	//if( m_bBiasAgainstAttacker ){ fHitAttackerHPDamageMult = 1.0f + 1.2f/fUnitCount; fHitDefenderHPDamageMult = 1.0f - 0.6f/fUnitCount; }
	//if( m_bBiasAgainstDefender ){ fHitAttackerHPDamageMult = 1.0f - 0.6f/fUnitCount; fHitDefenderHPDamageMult = 1.0f + 1.2f/fUnitCount; }

	// Calculate each QuickCombatUnit's power against the other force
	//CalculateAbilityToDamageForAllUnits();

	// Start Processing Combat
	//   End Conditions:
	//   (1) One side is all dead
	//   (2) Units can't fight (example: two satellites are left)
	//   (3) Damage has been reduced to a low-level (example: Player1 AttackSatellite vs Player2 Fleet)
	float fCurrentTime = 0.0f;
	float fTimeIncrement = 0.4f;
	bool bCombatIsFinished = false;
	while( bCombatIsFinished == false )
	{
		fCurrentTime += fTimeIncrement;

		float fDamageThisPass = 0.0f;
		for( long iCycle=0; iCycle<2; iCycle++ )
		{
			CList< CEOSAIQuickCombatUnit* >* pList1;
			CList< CEOSAIQuickCombatUnit* >* pList2;
			if( iCycle==0 ){ pList1 = &m_AttackerUnits; pList2 = &m_DefenderUnits; }
			if( iCycle==1 ){ pList1 = &m_DefenderUnits; pList2 = &m_AttackerUnits; }
			if( iCycle==1 && m_bHandleDefenderCounterAttack == false ) continue;

			pos = pList1->GetHeadPosition();
			while( pos )
			{
				CEOSAIQuickCombatUnit* pUnit1 = pList1->GetNext( pos );
				if( iCycle==1 && pUnit1->m_bAggressive == false ) continue; // non-agressive units don't fight
				if( pUnit1->m_fCurrentHP <= 0.0f ) continue; // dead units don't fight
				//if( pUnit1->m_bIsDead ) continue; // dead units don't fight

				float fHPDamage1 = 0.0f;
				float fTime = fTimeIncrement;//1.0f;
				while( fTime > 0.0f )
				{
					if( pUnit1->m_pCurrentBestTarget )
					{
						if( pUnit1->m_pCurrentBestTarget->m_fCurrentHP == 0.0f ) pUnit1->m_pCurrentBestTarget = NULL;
					}

					CEOSAIQuickCombatUnit* pBestTarget = pUnit1->m_pCurrentBestTarget;
					if( pUnit1->m_pCurrentBestTarget == NULL )
					{
						// Find a new target, calculate the HPDamage
						pUnit1->m_pCurrentBestTarget = FindBestTarget( pUnit1, pList2, &fHPDamage1 );
						pBestTarget = pUnit1->m_pCurrentBestTarget;
					}
					else
					{
						// Calculate the HPDamage
						if( pBestTarget->m_pAIUnit ){ fHPDamage1 = pUnit1->m_pAIUnitTemplate->GetExpectedHPDamage( pBestTarget->m_pAIUnit, true ); }
						else{                         fHPDamage1 = pUnit1->m_pAIUnitTemplate->GetExpectedHPDamage( pBestTarget->m_pAIUnitTemplate ); }
					}

					// Do Damage to the [BestTarget]
					if( pBestTarget )
					{
						// Unit2 gets to fight back (important when Unit2 is non-aggressive)
						// Note: ExpectedHPDamage includes terrain, entrenched, etc multipliers
						//   But, if the defense is bad (docked), then use the normal terrain because 
						//   ships/aircraft will move rather than stay in their vulnerable state.
						float fHPDamage2 = 0.0f;
						if( pUnit1->m_pAIUnit ){ fHPDamage2 = pBestTarget->m_pAIUnitTemplate->GetExpectedHPDamage( pUnit1->m_pAIUnit, true ); }
						else{                    fHPDamage2 = pBestTarget->m_pAIUnitTemplate->GetExpectedHPDamage( pUnit1->m_pAIUnitTemplate ); }

						fHPDamage1 *= fTime;
						fHPDamage2 *= fTime;

						if( iCycle == 0 )
						{
							fHPDamage1 *= fHitDefenderHPDamageMult;
							fHPDamage2 *= fHitAttackerHPDamageMult;
						}
						else
						{
							fHPDamage1 *= fHitAttackerHPDamageMult; // Attacker in slot 1
							fHPDamage2 *= fHitDefenderHPDamageMult;
						}

						float fPercentApplyableDamage1 = GetApplyableDamage( pBestTarget, fHPDamage1 );
						float fPercentApplyableDamage2 = GetApplyableDamage( pUnit1, fHPDamage2 );
						float fPercentApplyableDamage = min( fPercentApplyableDamage1,fPercentApplyableDamage2 );

						if( fPercentApplyableDamage == 0.0f )
						{
							fTime = 0.0f;
						}
						else
						{
							fDamageThisPass +=
								fPercentApplyableDamage*fHPDamage1 + fPercentApplyableDamage*fHPDamage2;

							ApplyDamage( pBestTarget, fPercentApplyableDamage*fHPDamage1 );
							ApplyDamage( pUnit1, fPercentApplyableDamage*fHPDamage2 );
							fTime *= (1.0f - fPercentApplyableDamage);
						}
					}
					else
					{
						// If this happens, Unit1 cannot attack anyone in the enemy group
						//   Either they simply can't fight (e.g. infantry+satellite, infantry+submarine)
						//   Or, one side has been completely destroyed
						fTime = 0.0f;
					}
				}
			}
		}

		// Reduce the HPDamage Multipler
		/*
		if( fHitAttackerHPDamageMult != 1.0f )
		{
			fHitAttackerHPDamageMult = 1.0f + (1.0f-(fTimeIncrement*0.4f))*(fHitAttackerHPDamageMult-1.0f);
		}
		if( fHitDefenderHPDamageMult != 1.0f )
		{
			fHitDefenderHPDamageMult = 1.0f + (1.0f-(fTimeIncrement*0.4f))*(fHitDefenderHPDamageMult-1.0f);
		}
		*/

		// Mark all 'zero hp' units as dead
		long iNumberOfLivingAttackers = 0;
		pos = m_AttackerUnits.GetHeadPosition();
		while( pos )
		{
			CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
			//if( pUnit->m_fCurrentHP <= 0.0f ) pUnit->m_bIsDead = true;
			//if( pUnit->m_bIsDead == false ) iNumberOfLivingAttackers++;
			if( pUnit->m_fCurrentHP > 0.0f ) iNumberOfLivingAttackers++;
		}
		long iNumberOfLivingDefenders = 0;
		pos = m_DefenderUnits.GetHeadPosition();
		while( pos )
		{
			CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
			//if( pUnit->m_fCurrentHP <= 0.0f ) pUnit->m_bIsDead = true;
			//if( pUnit->m_bIsDead == false ) iNumberOfLivingDefenders++;
			if( pUnit->m_fCurrentHP > 0.0f ) iNumberOfLivingDefenders++;
		}

		if( iNumberOfLivingAttackers == 0 || iNumberOfLivingDefenders == 0 )
		{
			bCombatIsFinished = true;
		}
		if( fDamageThisPass == 0.0f ) // Reached a stalemate
		{
			fCurrentTime -= fTimeIncrement; // Nothing happened this time increment, so subtract it
			bCombatIsFinished = true;
		}
		if( fCurrentTime > 9.99f ) // End after 10 turns (there might be slow damage going on)
		{
			bCombatIsFinished = true;
		}
	}
	ClearResults();

	m_fCompletionTime = fCurrentTime;

	// Mark all 'zero hp' units as dead
	float fTotalHP = 0;
	pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );

		float fOriginalHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		m_fProductionCostOfAttacker += fOriginalHP01*pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();

		float fHPDamage = (pUnit->m_fInitialHP - pUnit->m_fCurrentHP) / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		m_fProductionDamageToAttacker += fHPDamage*pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );

		float fOriginalHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		m_fProductionCostOfDefender += fOriginalHP01*pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();

		float fHPDamage = (pUnit->m_fInitialHP - pUnit->m_fCurrentHP) / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		m_fProductionDamageToDefender += fHPDamage*pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}

	if( m_fProductionDamageToDefender+m_fProductionDamageToAttacker == 0.0f )
	{
		m_fAttritionBalance01 = 0.5f; // no damage
	}
	else
	{
		m_fAttritionBalance01 = m_fProductionDamageToDefender / (m_fProductionDamageToDefender+m_fProductionDamageToAttacker);
	}

}
/*
void CEOSAIQuickCombatCalculation::CalculateAbilityToDamageForAllUnits()
{
	// Calculate each QuickCombatUnit's power against the other force
	POSITION pos;
	for( long i=0; i<2; i++ )
	{
		CList< CEOSAIQuickCombatUnit* >* pList1;
		CList< CEOSAIQuickCombatUnit* >* pList2;
		if( i==0 ){ pList1 = &m_AttackerUnits; pList2 = &m_DefenderUnits; }
		if( i==1 ){ pList1 = &m_DefenderUnits; pList2 = &m_AttackerUnits; }

		pos = pList1->GetHeadPosition();
		while( pos )
		{
			CEOSAIQuickCombatUnit* pUnit1 = pList1->GetNext( pos );

			// The attacker should maximize degrading enemy power, so find out how much damage each unit can do
			//
			float fMaximumDamageToPower = 0.0f;
			POSITION pos2 = pList2->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIQuickCombatUnit* pUnit2 = pList2->GetNext( pos2 );
			//	float fHPDamage = 0.0f;
			//	if( pUnit2->m_pUnit ){ fHPDamage = pUnit1->m_pUnitTemplate->GetExpectedHPDamage( pUnit2->m_pUnit, true ); }
			//	else{                  fHPDamage = pUnit1->m_pUnitTemplate->GetExpectedHPDamage( pUnit2->m_pUnitTemplate ); }
			//	float fDamageToPower = (fHPDamage / pUnit2->m_pUnitTemplate->GetMaxHP()) * pUnit2->m_pUnitTemplate->GetAIPowerValue();
			//	fMaximumDamageToPower = max( fDamageToPower,fMaximumDamageToPower );

				float fUnit1CombatSignificanceAgainstUnit2 = pUnit1->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatValue( pUnit2->m_pUnitTemplate )->GetCombatSignificance();
				float fDamageToPower = 
					( fUnit1CombatSignificanceAgainstUnit2 / pUnit2->m_pUnitTemplate->GetProductionAndIronCost1() ) *
					  pUnit2->m_pUnitTemplate->GetCombatSignificance();
				fMaximumDamageToPower = max( fDamageToPower,fMaximumDamageToPower );
			}
			//pUnit1->m_fAbilityToDamageToEnemyForce = fMaximumDamageToPower;
		}
	}
}
*/
CEOSAIQuickCombatUnit* CEOSAIQuickCombatCalculation::FindBestTarget( CEOSAIQuickCombatUnit* pUnit1, CList< CEOSAIQuickCombatUnit* >* pList2, float* pfOutHPDamage )
{
	// Find the best target (I can cache this later)
	float fBestTargetDamageToPower = 0.0f;
	CEOSAIQuickCombatUnit* pBestTarget = NULL;
	POSITION pos2 = pList2->GetHeadPosition();
	while( pos2 )
	{
		CEOSAIQuickCombatUnit* pUnit2 = pList2->GetNext( pos2 );
		if( pUnit2->m_fCurrentHP <= 0.0f ) continue; // don't hit dead units
		if( pUnit1->m_pAIUnitTemplate->GetViewRange( pUnit2->m_pAIUnitTemplate ) <= 0.0f ) continue; // can't hit units it can't see

		if( m_bLimitCombatByLandAndWaterBarriers )
		{
			if( pUnit1->m_pAIUnitTemplate->IsGroundUnit() && pUnit2->m_pAIUnitTemplate->IsSeaUnit() ) continue;
			if( pUnit1->m_pAIUnitTemplate->IsSeaUnit() && pUnit2->m_pAIUnitTemplate->IsGroundUnit() ) continue;
		}

		// Note: ExpectedHPDamage includes terrain, entrenched, etc multipliers
		//   But, if the defense is bad (docked), then use the normal terrain because ships/aircraft
		//   will move rather than stay in their vulnerable state.
		float fHPDamage = 0.0f;
		if( pUnit2->m_pAIUnit ){ fHPDamage = pUnit1->m_pAIUnitTemplate->GetExpectedHPDamage( pUnit2->m_pAIUnit, true ); }
		else{                    fHPDamage = pUnit1->m_pAIUnitTemplate->GetExpectedHPDamage( pUnit2->m_pAIUnitTemplate ); }
		float fDamageToPower =  (fHPDamage / pUnit2->m_pAIUnitTemplate->GetMaxHP()) * pUnit2->m_pAIUnitTemplate->GetAICombatSignificance();
	/*
		float fDamageToPower = (fHPDamage / pUnit2->m_pUnitTemplate->GetMaxHP()) * pUnit2->m_pUnitTemplate->GetAIPowerValue();
		
		float fHPDamage = pUnit1->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatValue( pUnit2->m_pUnitTemplate )->GetCombatSignificance();
		float fUnit1CombatSignificanceAgainstUnit2 = pUnit1->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatValue( pUnit2->m_pUnitTemplate )->GetCombatSignificance();
		float fDamageToPower = 
			( fUnit1CombatSignificanceAgainstUnit2 / pUnit2->m_pUnitTemplate->GetProductionAndIronCost1() ) *
			  pUnit2->m_pUnitTemplate->GetCombatSignificance();
	*/
		//float fDamageToPower = 
		//	pUnit1->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatValue( pUnit2->m_pUnitTemplate )->GetCombatSignificance() *
		//	pUnit2->m_pUnitTemplate->GetCombatSignificance();
		if( fDamageToPower > fBestTargetDamageToPower )
		{
			*pfOutHPDamage = fHPDamage;
			fBestTargetDamageToPower = fDamageToPower;
			pBestTarget = pUnit2;
		}
	}
	return pBestTarget;
}

float CEOSAIQuickCombatCalculation::GetApplyableDamage( CEOSAIQuickCombatUnit* pTarget, float fHPDamage )
{
	if( fHPDamage <= pTarget->m_fCurrentHP ){ return 1.0f; }
	else{                                     return pTarget->m_fCurrentHP / fHPDamage; }
}

void CEOSAIQuickCombatCalculation::ApplyDamage( CEOSAIQuickCombatUnit* pTarget, float fHPDamage )
{
	if( fHPDamage < pTarget->m_fCurrentHP )
	{
		pTarget->m_fCurrentHP -= fHPDamage;
		if( pTarget->m_fCurrentHP < 0.00001f ) pTarget->m_fCurrentHP = 0.0f;
	}
	else
	{
		pTarget->m_fCurrentHP  = 0.0f;
	}
}

void CEOSAIQuickCombatCalculation::SaveState( CEOSAIQuickCombatResults* pCombatResults )
{
	pCombatResults->m_fCombatSuccessValue02 = GetAttackerSuccessValue02();
	pCombatResults->m_fAttritionBalance01 = GetAttritionBalance01();
	//
	pCombatResults->m_fAttackerDamageAsProduction = GetAttackerDamageAsProduction();
	//
	pCombatResults->m_fDefenderDamageAsProduction = GetDefenderDamageAsProduction();
	pCombatResults->m_fDefenderDamageAsCombatSignificance = GetDefenderDamageAsCombatSignificance();
	pCombatResults->m_fDefenderSurvivalAsCombatSignificance = GetDefenderSurvivalAsCombatSignificance();
}

bool CEOSAIQuickCombatCalculation::InvolvesEnemyGroundUnits()
{
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pQUnit = m_DefenderUnits.GetNext( pos );
		if( pQUnit->m_pAIUnitTemplate->IsGroundUnit() ) return true;
	}
	return false;
}

float CEOSAIQuickCombatCalculation::GetGroundCombatWinBalance01() // 0.0 = Favors Defender, 1.0 = Favors Attacker
{
	float fAttackerSurvival01 = GetAttackerGroundUnitSurvivalAsCombatSignificance01();
	float fDefenderSurvival01 = GetDefenderGroundUnitSurvivalAsCombatSignificance01();
	//
	float fCombatWinBalance01 = 0.5f;
	if( fAttackerSurvival01 == 0.0f && fDefenderSurvival01 == 0.0f )
	{
		fCombatWinBalance01 = 0.5f; // everybody dies
	}
	else if( fAttackerSurvival01 > 0.0f && fDefenderSurvival01 > 0.0f )
	{
		fCombatWinBalance01 = fAttackerSurvival01 / (fAttackerSurvival01+fDefenderSurvival01);
	}
	else if( fAttackerSurvival01 > 0.0f && fDefenderSurvival01 == 0.0f )
	{
		fCombatWinBalance01 = 0.5f + 0.5f*fAttackerSurvival01;
	}
	else if( fAttackerSurvival01 == 0.0f && fDefenderSurvival01 > 0.0f )
	{
		fCombatWinBalance01 = 0.5f - 0.5f*fDefenderSurvival01;
	}
	return fCombatWinBalance01;
}

bool CEOSAIQuickCombatCalculation::InvolvesEnemySeaUnits()
{
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pQUnit = m_DefenderUnits.GetNext( pos );
		if( pQUnit->m_pAIUnitTemplate->IsSeaUnit() ) return true;
	}
	return false;
}

float CEOSAIQuickCombatCalculation::GetSeaCombatWinBalance01() // 0.0 = Favors Defender, 1.0 = Favors Attacker
{
	float fAttackerSurvival01 = GetAttackerSeaUnitSurvivalAsCombatSignificance01();
	float fDefenderSurvival01 = GetDefenderSeaUnitSurvivalAsCombatSignificance01();
	//
	float fCombatWinBalance01 = 0.5f;
	if( fAttackerSurvival01 == 0.0f && fDefenderSurvival01 == 0.0f )
	{
		fCombatWinBalance01 = 0.5f; // everybody dies
	}
	else if( fAttackerSurvival01 > 0.0f && fDefenderSurvival01 > 0.0f )
	{
		fCombatWinBalance01 = fAttackerSurvival01 / (fAttackerSurvival01+fDefenderSurvival01);
	}
	else if( fAttackerSurvival01 > 0.0f && fDefenderSurvival01 == 0.0f )
	{
		fCombatWinBalance01 = 0.5f + 0.5f*fAttackerSurvival01;
	}
	else if( fAttackerSurvival01 == 0.0f && fDefenderSurvival01 > 0.0f )
	{
		fCombatWinBalance01 = 0.5f - 0.5f*fDefenderSurvival01;
	}
	return fCombatWinBalance01;
}

float CEOSAIQuickCombatCalculation::GetCombatWinBalance01()
{
	if( m_fProductionCostOfAttacker == 0.0f ) return 0.0f;
	if( m_fProductionCostOfDefender == 0.0f ) return 1.0f;

	//float fAttackerSurvival01 = GetAttackerUnitSurvival01();
	//float fDefenderSurvival01 = GetDefenderUnitSurvival01();
	float fAttackerSurvival01 = (m_fProductionCostOfAttacker-m_fProductionDamageToAttacker) / m_fProductionCostOfAttacker;
	float fDefenderSurvival01 = (m_fProductionCostOfDefender-m_fProductionDamageToDefender) / m_fProductionCostOfDefender;

	//
	float fCombatWinBalance01 = 0.5f;
	if( fAttackerSurvival01 == 0.0f && fDefenderSurvival01 == 0.0f )
	{
		fCombatWinBalance01 = 0.5f; // everybody dies
	}
	else if( fAttackerSurvival01 > 0.0f && fDefenderSurvival01 > 0.0f )
	{
		fCombatWinBalance01 = fAttackerSurvival01 / (fAttackerSurvival01+fDefenderSurvival01);
	}
	else if( fAttackerSurvival01 > 0.0f && fDefenderSurvival01 == 0.0f )
	{
		fCombatWinBalance01 = 0.5f + 0.5f*(fAttackerSurvival01*fAttackerSurvival01);
	}
	else if( fAttackerSurvival01 == 0.0f && fDefenderSurvival01 > 0.0f )
	{
		fCombatWinBalance01 = 0.5f - 0.5f*(fDefenderSurvival01*fDefenderSurvival01);
	}
	return fCombatWinBalance01;
}

/*
float CEOSAIQuickCombatCalculation::GetCombatWinBalance01()
{
	float fAttackerSurvival01 = min( GetAttackerUnitSurvival01();
	float fDefenderSurvival01 = GetDefenderUnitSurvival01();
}
*/

// If the Enemy has ground units, I need to destroy all of the significant ones and have Ground/Air power left over
// If the Enemy has sea units, I need to destroy all of the significant ones and have Sea/Air power left over
// If the Enemy has air units, I need to destroy all of the significant ones and have Air units left over
//float CEOSAIQuickCombatCalculation::GetCombatWinBalanceUsingCombatSignificance01()
//float CEOSAIQuickCombatCalculation::GetAttackValue01()
float CEOSAIQuickCombatCalculation::GetAttackerSuccessValue02()
{
	float fInitialAttackerGroundUnitSignificance = 0.0f;
	float fFinalAttackerGroundUnitSignificance = 0.0f;
	float fInitialAttackerSeaUnitSignificance = 0.0f;
	float fFinalAttackerSeaUnitSignificance = 0.0f;
	float fInitialAttackerAirUnitSignificance = 0.0f;
	float fFinalAttackerAirUnitSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		float fCombatSignificance01 = pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatAttritionSignificance01();
		if( fCombatSignificance01 > 0.2f )
		{
			//if( pUnit->m_bIsDead == false )
			if( pUnit->m_fCurrentHP > 0.0f )
			{
				float fCurrentHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
				float fCombatSignificance = fCurrentHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
				if( pUnit->m_pAIUnitTemplate->IsGroundUnit() ){ fFinalAttackerGroundUnitSignificance += fCombatSignificance; }
				if( pUnit->m_pAIUnitTemplate->IsSeaUnit()    ){ fFinalAttackerSeaUnitSignificance    += fCombatSignificance; }
				if( pUnit->m_pAIUnitTemplate->IsAirUnit()    ){ fFinalAttackerAirUnitSignificance    += fCombatSignificance; }
			}
			float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
			float fCombatSignificance = fInitialHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
			if( pUnit->m_pAIUnitTemplate->IsGroundUnit() ){ fInitialAttackerGroundUnitSignificance += fCombatSignificance; }
			if( pUnit->m_pAIUnitTemplate->IsSeaUnit()    ){ fInitialAttackerSeaUnitSignificance    += fCombatSignificance; }
			if( pUnit->m_pAIUnitTemplate->IsAirUnit()    ){ fInitialAttackerAirUnitSignificance    += fCombatSignificance; }
		}
	}

	float fInitialDefenderGroundUnitSignificance = 0.0f;
	float fFinalDefenderGroundUnitSignificance = 0.0f;
	float fInitialDefenderSeaUnitSignificance = 0.0f;
	float fFinalDefenderSeaUnitSignificance = 0.0f;
	float fInitialDefenderAirUnitSignificance = 0.0f;
	float fFinalDefenderAirUnitSignificance = 0.0f;
	float fInitialCombatInsignificantProductionValue = 0.0f;
	float fFinalCombatInsignificantProductionValue = 0.0f;
	//float fInitialProductionValueOfDefender = 0.0f;
	//float fInitialProductionValueOfDefender = 0.0f;
	float fProductionDamageDoneToDefender = 0.0f;
	pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );

		float fHPDamage01 = (float)( pUnit->m_fInitialHP - pUnit->m_fCurrentHP ) / (float)pUnit->m_pAIUnitTemplate->GetMaxHP();
		fProductionDamageDoneToDefender += fHPDamage01 * pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();

		float fCombatSignificance01 = pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatAttritionSignificance01();
		if( fCombatSignificance01 > 0.2f )
		{
			//if( pUnit->m_bIsDead == false )
			if( pUnit->m_fCurrentHP > 0.0f )
			{
				float fCurrentHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
				float fCombatSignificance = fCurrentHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
				if( pUnit->m_pAIUnitTemplate->IsGroundUnit() ){ fFinalDefenderGroundUnitSignificance += fCombatSignificance; }
				if( pUnit->m_pAIUnitTemplate->IsSeaUnit()    ){ fFinalDefenderSeaUnitSignificance    += fCombatSignificance; }
				if( pUnit->m_pAIUnitTemplate->IsAirUnit()    ){ fFinalDefenderAirUnitSignificance    += fCombatSignificance; }
			}
			float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
			float fCombatSignificance = fInitialHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
			if( pUnit->m_pAIUnitTemplate->IsGroundUnit() ){ fInitialDefenderGroundUnitSignificance += fCombatSignificance; }
			if( pUnit->m_pAIUnitTemplate->IsSeaUnit()    ){ fInitialDefenderSeaUnitSignificance    += fCombatSignificance; }
			if( pUnit->m_pAIUnitTemplate->IsAirUnit()    ){ fInitialDefenderAirUnitSignificance    += fCombatSignificance; }
		}
		else
		{
			float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
			float fDeltaHP01 = ( pUnit->m_fInitialHP - pUnit->m_fCurrentHP ) / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();

			fInitialCombatInsignificantProductionValue = fInitialHP01 * pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
			fFinalCombatInsignificantProductionValue = fDeltaHP01 * pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
		}
	}

	//
	float fInitialAttackerSignificance = fInitialAttackerGroundUnitSignificance + fInitialAttackerSeaUnitSignificance + fInitialAttackerAirUnitSignificance;
	float fFinalAttackerSignificance = fFinalAttackerGroundUnitSignificance + fFinalAttackerSeaUnitSignificance + fFinalAttackerAirUnitSignificance;
	float fInitialDefenderSignificance = fInitialDefenderGroundUnitSignificance + fInitialDefenderSeaUnitSignificance + fInitialDefenderAirUnitSignificance;
	float fFinalDefenderSignificance = fFinalDefenderGroundUnitSignificance + fFinalDefenderSeaUnitSignificance + fFinalDefenderAirUnitSignificance;

	// There were no significant defenders
	if( fInitialDefenderSignificance == 0.0f )
	{
		if( fFinalAttackerSignificance > 0.0f ) return 1.0f; // No significant enemies, Excess Attackers
		return 0.5f; // There are no significant enemies, but no significant attackers
		//return 0.8f; // There are no significant enemies, but no significant attackers
	}
	// Not all the significant defenders have been eliminated
	if( fFinalDefenderSignificance > 0.0f )
	{
		// If fFinalDefenderSignificance == fInitialDefenderSignificance, return 0.0
		// As fFinalDefenderSignificance approaches 0.0, return 0.8
		return 0.5f - 0.5f*( fFinalDefenderSignificance / fInitialDefenderSignificance );
	}
	// All the significant defenders have been eliminated, but what's my excess, what damage has been done?
	else //if( fFinalDefenderSignificance == 0.0f )
	{
		// If fFinalDefenderSignificance == fInitialDefenderSignificance, return 0.0
		// As fFinalDefenderSignificance approaches 0.0, return 0.8
		//return 0.8 - 0.8f*( fFinalDefenderSignificance / fInitialDefenderSignificance )
		float fInsigProdValue = 0.0f;
		if( fInitialCombatInsignificantProductionValue > 0.0f )
		{
			fInsigProdValue = 0.1f * (1.0f - (fFinalCombatInsignificantProductionValue / (fInitialCombatInsignificantProductionValue+0.001f)));
			ASSERT( fInsigProdValue >= 0.0f && fInsigProdValue <= 0.1f );
		}

		// Would like to maintain a capacity to fight potential enemies and poor luck fighting existing ones
		// How much can each unit contribute to fighting these enemies?

		float fExcessGround = 1.0f;
		if( fInitialDefenderGroundUnitSignificance > 0.0f )
		{
			// How much extra GroundUnit + AirUnit Significance do I have?
			fExcessGround =
				( fFinalAttackerGroundUnitSignificance + fFinalAttackerAirUnitSignificance ) /
				( fInitialAttackerGroundUnitSignificance + fInitialAttackerAirUnitSignificance + 5.0f );
		}
		float fExcessSea = 1.0f;
		if( fInitialDefenderSeaUnitSignificance > 0.0f )
		{
			fExcessSea =
				( fFinalAttackerSeaUnitSignificance + fFinalAttackerAirUnitSignificance ) /
				( fInitialAttackerSeaUnitSignificance + fInitialAttackerAirUnitSignificance + 5.0f );
		}
		float fExcessAir = 1.0f;
		if( fInitialDefenderAirUnitSignificance > 0.0f )
		{
			fExcessAir =
				fFinalAttackerAirUnitSignificance / ( fInitialAttackerAirUnitSignificance + 5.0f );
		}
		// An excess of 100% in all areas will yield an fExcess of 0.4 (maxed-out)
		float fExcess = min( fExcessAir, min( fExcessGround, fExcessSea ));
		fExcess *= 0.4f;
		if( fExcess > 0.4f ) fExcess = 0.4f;
		ASSERT( fExcess >= 0.0f && fExcess <= 0.4f );

		return 1.0f + fInsigProdValue + fExcess;
		//return 1.0f + fInsigProdValue + fExcess;
	}
}

float CEOSAIQuickCombatCalculation::GetOutgoingProdDamagePerTurn()
{
	if( GetCompletionTime() == 0.0f ) return 0.0f;
	return GetProductionDamageToDefender() / GetCompletionTime();
}

float CEOSAIQuickCombatCalculation::GetIncomingProdDamagePerTurn()
{
	if( GetCompletionTime() == 0.0f ) return 0.0f;
	return GetProductionDamageToAttacker() / GetCompletionTime();
}
/*
// Attrition
float CEOSAIQuickCombatCalculation::GetMeleeAttrition01()
{
	if( m_fCompletionTime == 0.0f ){ return 0.0f; }

	float fOutgoingDamagePerTurn = GetProductionDamageToDefender() / GetCompletionTime();
	float fIncomingDamagePerTurn = GetProductionDamageToAttacker() / GetCompletionTime();
	return fOutgoingDamagePerTurn / ( fOutgoingDamagePerTurn + fIncomingDamagePerTurn );
}

float CEOSAIQuickCombatCalculation::GetMeleeAttrition01( CUnitTemplate* pAttacker, CUnitTemplate* pDefender )
{
	//if( m_fCompletionTime == 0.0f ){ return 0.0f; }

	float fOutgoingDamagePerTurn = pAttacker->GetExpectedHPDamage( pDefender );
	float fIncomingDamagePerTurn = pDefender->GetExpectedHPDamage( pAttacker );

	// Attacker has a longer range than defender, assume a moving defender
	//float fTimeUnderFire = fRangeGap / (pDefender->GetMovementRate()+0.001f);
	float fAttackerProd = pAttacker->GetProductionAndIronCost1();
	float fDefenderProd = pDefender->GetProductionAndIronCost1();
	if( fDefenderProd <= 0.0f ) return 1.0f; // Defender is dead before it can attack

	float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
	float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

	float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
	fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
	fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

	float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
	float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

	return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
}

float CEOSAIQuickCombatCalculation::GetRangedAttrition01( CUnitTemplate* pAttacker, CUnitTemplate* pDefender )
{
	//if( m_fCompletionTime == 0.0f ){ return 0.0f; }

	// Only allowed when there is one attacker, one defender
	//ASSERT( m_AttackerUnits.GetCount() == 1 );
	//ASSERT( m_DefenderUnits.GetCount() == 1 );
	//ASSERT( m_bHandleDefenderCounterAttack == false );

	//CUnitTemplate* pAttacker = m_AttackerUnits.GetHead()->m_pUnitTemplate;
	//CUnitTemplate* pDefender = m_DefenderUnits.GetHead()->m_pUnitTemplate;

	//float fOutgoingDamagePerTurn = GetProductionDamageToDefender() / GetCompletionTime();
	//float fIncomingDamagePerTurn = GetProductionDamageToAttacker() / GetCompletionTime();

	CEOSAIAttackVs2* pAttackVs1 = pAttacker->GetAttackVs( EnumPoiObjectType::enum_Unit, pDefender, false );
	float fRange1 = 0.0f; if( pAttackVs1 ){ fRange1 = pAttackVs1->GetAttackRange(); }
	float fOutgoingDamagePerTurn = pAttacker->GetExpectedHPDamage( pDefender );

	CEOSAIAttackVs2* pAttackVs2 = pDefender->GetAttackVs( EnumPoiObjectType::enum_Unit, pAttacker, false );
	float fRange2 = 0.0f; if( pAttackVs2 ){ fRange2 = pAttackVs2->GetAttackRange(); }
	float fIncomingDamagePerTurn = pDefender->GetExpectedHPDamage( pAttacker );

	float fRangeGap = fRange1 - fRange2;

	if( fRangeGap > 0.0f )
	{
		// Attacker has a longer range than defender, assume a moving defender
		float fTimeUnderFire = fRangeGap / (pDefender->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1();
		float fDefenderProd = pDefender->GetProductionAndIronCost1() - fTimeUnderFire*fOutgoingDamagePerTurn;
		if( fDefenderProd <= 0.0f ) return 1.0f; // Defender is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	eif( fRangeGap == 0.0f )
	{
		return GetMeleeAttrition01( pAttacker, pDefender );
	}
	else // fRangeGap < 0.0f
	{
		float fTimeUnderFire = (-fRangeGap) / (pAttacker->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1() - fTimeUnderFire*fIncomingDamagePerTurn;
		float fDefenderProd = pDefender->GetProductionAndIronCost1();
		if( fAttackerProd <= 0.0f ) return 0.0f; // Attacker is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	ASSERT( false );
	return 0.0f;
}

float CEOSAIQuickCombatCalculation::GetRangeX2Attrition01()
{
	//if( m_fCompletionTime == 0.0f ){ return 0.0f; }

	// Only allowed when there is one attacker, one defender
	ASSERT( m_AttackerUnits.GetCount() == 1 );
	ASSERT( m_DefenderUnits.GetCount() == 1 );
	ASSERT( m_bHandleDefenderCounterAttack == false );

	CUnitTemplate* pAttacker = m_AttackerUnits.GetHead()->m_pUnitTemplate;
	CUnitTemplate* pDefender = m_DefenderUnits.GetHead()->m_pUnitTemplate;

	float fOutgoingDamagePerTurn = GetProductionDamageToDefender() / GetCompletionTime();
	float fIncomingDamagePerTurn = GetProductionDamageToAttacker() / GetCompletionTime();

	CEOSAIAttackVs2* pAttackVs1 = pAttacker->GetAttackVs( EnumPoiObjectType::enum_Unit, pDefender, false );
	CEOSAIAttackVs2* pAttackVs2 = pDefender->GetAttackVs( EnumPoiObjectType::enum_Unit, pAttacker, false );
	float fRange1 = 0.0f; if( pAttackVs1 ){ fRange1 = pAttackVs1->GetAttackRange(); }
	float fRange2 = 0.0f; if( pAttackVs2 ){ fRange2 = pAttackVs2->GetAttackRange(); }
	float fRangeGap = 2.0f * (fRange1 - fRange2);

	if( fRangeGap > 0.0f )
	{
		// Attacker has a longer range than defender, assume a moving defender
		float fTimeUnderFire = fRangeGap / (pDefender->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1();
		float fDefenderProd = pDefender->GetProductionAndIronCost1() - fTimeUnderFire*fOutgoingDamagePerTurn;
		if( fDefenderProd <= 0.0f ) return 1.0f; // Defender is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	else if( fRangeGap == 0.0f )
	{
		return GetMeleeAttrition01();
	}
	else // fRangeGap < 0.0f
	{
		float fTimeUnderFire = (-fRangeGap) / (pAttacker->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1() - fTimeUnderFire*fIncomingDamagePerTurn;
		float fDefenderProd = pDefender->GetProductionAndIronCost1();
		if( fAttackerProd <= 0.0f ) return 0.0f; // Attacker is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	ASSERT( false );
	return 0.0f;
}
*/
float CEOSAIQuickCombatCalculation::CalculateAttrition01( CEOSAIUnitTemplate* pAttacker, CEOSAIUnitTemplate* pDefender, float fRangeMultiplier, float* pTimeResult )
{
	CEOSAIAttackVs2* pAttackVs1 = pAttacker->GetAttackVs( EnumAIPoiObjectType::enum_Unit, pDefender, false );
	float fRange1 = 0.0f; if( pAttackVs1 ){ fRange1 = pAttackVs1->GetAttackRange(); }
	float fOutgoingDamagePerTurn = pAttacker->GetExpectedHPDamage( pDefender );

	CEOSAIAttackVs2* pAttackVs2 = pDefender->GetAttackVs( EnumAIPoiObjectType::enum_Unit, pAttacker, false );
	float fRange2 = 0.0f; if( pAttackVs2 ){ fRange2 = pAttackVs2->GetAttackRange(); }
	float fIncomingDamagePerTurn = pDefender->GetExpectedHPDamage( pAttacker );

	if( fOutgoingDamagePerTurn == 0.0f && fIncomingDamagePerTurn == 0.0f ){ return 0.0f; }

	float fRangeGap = fRange1 - fRange2;
	fRangeGap *= fRangeMultiplier;

	if( pTimeResult )
	{
		*pTimeResult = pAttacker->GetMaxHP() / (fOutgoingDamagePerTurn+0.00001f);
	}

	if( fRangeGap > 0.0f )
	{
		// Attacker has a longer range than defender, assume a moving defender
		float fTimeUnderFire = fRangeGap / (pDefender->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1();
		float fDefenderProd = pDefender->GetProductionAndIronCost1() - fTimeUnderFire*fOutgoingDamagePerTurn;
		if( fDefenderProd <= 0.0f ) return 1.0f; // Defender is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	else if( fRangeGap == 0.0f )
	{
		//float fOutgoingDamagePerTurn = pAttacker->GetExpectedHPDamage( pDefender );
		//float fIncomingDamagePerTurn = pDefender->GetExpectedHPDamage( pAttacker );

		// Attacker has a longer range than defender, assume a moving defender
		//float fTimeUnderFire = fRangeGap / (pDefender->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1();
		float fDefenderProd = pDefender->GetProductionAndIronCost1();
		if( fDefenderProd <= 0.0f ) return 1.0f; // Defender is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	else // fRangeGap < 0.0f
	{
		float fTimeUnderFire = (-fRangeGap) / (pAttacker->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1() - fTimeUnderFire*fIncomingDamagePerTurn;
		float fDefenderProd = pDefender->GetProductionAndIronCost1();
		if( fAttackerProd <= 0.0f ) return 0.0f; // Attacker is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	ASSERT( false );
	return 0.0f;
}

float CEOSAIQuickCombatCalculation::CalculateAttrition01( EOSAI::PoiMobile* pAttacker, EOSAI::PoiMobile* pDefender, float fRangeMultiplier, float* pTimeResult )
{
	CEOSAIUnit2* pAttackerUnit = dynamic_cast< CEOSAIUnit2* >( pAttacker );
	CEOSAIUnit2* pDefenderUnit = dynamic_cast< CEOSAIUnit2* >( pDefender );
	float f = 0.0f;
	if( pAttackerUnit && pDefenderUnit )
	{
		return CalculateAttrition01( pAttackerUnit->GetAIUnitTemplate(), pDefenderUnit->GetAIUnitTemplate(), fRangeMultiplier, pTimeResult );
	}

	//
	CList< CEOSAIPoiObject* >  AttackersList;
	CList< CEOSAIPoiObject* >  DefendersList;
	pAttacker->AddToCombatGroup( &AttackersList );
	pDefender->AddToCombatGroup( &DefendersList );

	CEOSAIUnit2TemplatesAndFloat  Attackers;
	CEOSAIUnit2TemplatesAndFloat  Defenders;
	POSITION pos;
	pos = AttackersList.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAttackerPoiObject = AttackersList.GetNext( pos );
		CEOSAIUnit2* pAttackerUnit = dynamic_cast< CEOSAIUnit2* >( pAttackerPoiObject );
		Attackers.Add( pAttackerUnit->GetAIUnitTemplate(), pAttackerUnit->GetCurrentHP01() );
	}
	pos = DefendersList.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pDefenderPoiObject = DefendersList.GetNext( pos );
		CEOSAIUnit2* pDefenderUnit = dynamic_cast< CEOSAIUnit2* >( pDefenderPoiObject );
		Defenders.Add( pDefenderUnit->GetAIUnitTemplate(), pDefenderUnit->GetCurrentHP01() );
	}
	//
	float fTotalAttrition01 = 0.0f;
	float fTotalWeight = 0.0f;
	pos = Attackers.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pAttackerUnitTemplateAndFloat = Attackers.m_List.GetNext( pos );
		CEOSAIUnitTemplate* pAttacker = pAttackerUnitTemplateAndFloat->m_pAIUnitTemplate;
		float fAttackerCost = pAttacker->GetProductionAndIronCost1() * pAttackerUnitTemplateAndFloat->m_fValue;

		POSITION pos2 = Defenders.m_List.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnit2TemplateAndFloat* pDefenderUnitTemplateAndFloat = Defenders.m_List.GetNext( pos2 );
			CEOSAIUnitTemplate* pDefender = pDefenderUnitTemplateAndFloat->m_pAIUnitTemplate;
			float fDefenderCost = pDefender->GetProductionAndIronCost1() * pDefenderUnitTemplateAndFloat->m_fValue;

			float fAttrition01 = CEOSAIQuickCombatCalculation::CalculateAttrition01( pAttacker, pDefender, fRangeMultiplier );
			fTotalAttrition01 += fAttrition01 * (fAttackerCost + fDefenderCost);
			fTotalWeight += fAttackerCost + fDefenderCost;
		}
	}
	fTotalAttrition01 /= fTotalWeight;
	return fTotalAttrition01;

	/*
	CEOSAIUnit2TemplatesAndFloat CombatResultsAgainstDefenders;
	CEOSAIQuickCombatCalculation::QuickCombatResults( Attackers, Defenders, CombatResultsAgainstDefenders );
	CEOSAIUnit2TemplatesAndFloat CombatResultsAgainstAttackers;
	CEOSAIQuickCombatCalculation::QuickCombatResults( Defenders, Attackers, CombatResultsAgainstAttackers );

	// Count up the damage to the defender
	float fProductionDamageToDefender = 0.0f;
	pos = CombatResultsAgainstDefenders.m_List.AddHead();
	while( pos )
	{
		CUnitTemplateAndFloat* pUnitTemplateAndFloat = CombatResultsAgainstDefenders.m_List.GetNext( pos );
		fProductionDamageToDefender += pUnitTemplateAndFloat->m_fValue;
	}
	float fProductionDamageToAttacker = 0.0f;
	pos = CombatResultsAgainstAttackers.m_List.AddHead();
	while( pos )
	{
		CUnitTemplateAndFloat* pUnitTemplateAndFloat = CombatResultsAgainstAttackers.m_List.GetNext( pos );
		fProductionDamageToAttacker += pUnitTemplateAndFloat->m_fValue;
	}
	return 
	*/

	/*
	CEOSAIAttackVs2* pAttackVs1 = pAttacker->GetAttackVs( EnumPoiObjectType::enum_Unit, pDefender, false );
	float fRange1 = 0.0f; if( pAttackVs1 ){ fRange1 = pAttackVs1->GetAttackRange(); }
	float fOutgoingDamagePerTurn = pAttacker->GetExpectedHPDamage( pDefender );

	CEOSAIAttackVs2* pAttackVs2 = pDefender->GetAttackVs( EnumPoiObjectType::enum_Unit, pAttacker, false );
	float fRange2 = 0.0f; if( pAttackVs2 ){ fRange2 = pAttackVs2->GetAttackRange(); }
	float fIncomingDamagePerTurn = pDefender->GetExpectedHPDamage( pAttacker );

	if( fOutgoingDamagePerTurn == 0.0f && fIncomingDamagePerTurn == 0.0f ){ return 0.0f; }

	float fRangeGap = fRange1 - fRange2;
	fRangeGap *= fRangeMultiplier;

	if( pTimeResult )
	{
		*pTimeResult = pAttacker->GetMaxHP() / (fOutgoingDamagePerTurn+0.00001f);
	}

	if( fRangeGap > 0.0f )
	{
		// Attacker has a longer range than defender, assume a moving defender
		float fTimeUnderFire = fRangeGap / (pDefender->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1();
		float fDefenderProd = pDefender->GetProductionAndIronCost1() - fTimeUnderFire*fOutgoingDamagePerTurn;
		if( fDefenderProd <= 0.0f ) return 1.0f; // Defender is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	else if( fRangeGap == 0.0f )
	{
		//float fOutgoingDamagePerTurn = pAttacker->GetExpectedHPDamage( pDefender );
		//float fIncomingDamagePerTurn = pDefender->GetExpectedHPDamage( pAttacker );

		// Attacker has a longer range than defender, assume a moving defender
		//float fTimeUnderFire = fRangeGap / (pDefender->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1();
		float fDefenderProd = pDefender->GetProductionAndIronCost1();
		if( fDefenderProd <= 0.0f ) return 1.0f; // Defender is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	else // fRangeGap < 0.0f
	{
		float fTimeUnderFire = (-fRangeGap) / (pAttacker->GetMovementRate()+0.001f);
		float fAttackerProd = pAttacker->GetProductionAndIronCost1() - fTimeUnderFire*fIncomingDamagePerTurn;
		float fDefenderProd = pDefender->GetProductionAndIronCost1();
		if( fAttackerProd <= 0.0f ) return 0.0f; // Attacker is dead before it can attack

		float fAttackerSurvivalTime = fAttackerProd / fIncomingDamagePerTurn;
		float fDefenderSurvivalTime = fDefenderProd / fOutgoingDamagePerTurn;

		float fCombatTime = min( fAttackerSurvivalTime, fDefenderSurvivalTime );
		fAttackerProd -= fCombatTime*fIncomingDamagePerTurn;
		fDefenderProd -= fCombatTime*fOutgoingDamagePerTurn;

		float fDamageToAttacker = pAttacker->GetProductionAndIronCost1() - fAttackerProd;
		float fDamageToDefender = pDefender->GetProductionAndIronCost1() - fDefenderProd;

		return fDamageToDefender / ( fDamageToDefender + fDamageToAttacker );
	}
	ASSERT( false );
	return 0.0f;
	*/
}

float CEOSAIQuickCombatCalculation::CalculateCombatWin01( EOSAI::PoiMobile* pAttacker, EOSAI::PoiMobile* pDefender )
{
	CList< CEOSAIPoiObject* >  AttackersList;
	CList< CEOSAIPoiObject* >  DefendersList;
	pAttacker->AddToCombatGroup( &AttackersList );
	pDefender->AddToCombatGroup( &DefendersList );

	CEOSAIUnit2TemplatesAndFloat  Attackers;
	CEOSAIUnit2TemplatesAndFloat  Defenders;
	POSITION pos;
	pos = AttackersList.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAttackerPoiObject = AttackersList.GetNext( pos );
		CEOSAIUnit2* pAttackerUnit = dynamic_cast< CEOSAIUnit2* >( pAttackerPoiObject );
		Attackers.Add( pAttackerUnit->GetAIUnitTemplate(), pAttackerUnit->GetCurrentHP01() );
	}
	pos = DefendersList.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pDefenderPoiObject = DefendersList.GetNext( pos );
		CEOSAIUnit2* pDefenderUnit = dynamic_cast< CEOSAIUnit2* >( pDefenderPoiObject );
		Defenders.Add( pDefenderUnit->GetAIUnitTemplate(), pDefenderUnit->GetCurrentHP01() );
	}
	//
	//CEOSAIUnit2TemplatesAndFloat PowerAgainstDefenders;
	//QuickCombatResults( Attackers, Defenders, PowerAgainstDefenders );
	//CEOSAIUnit2TemplatesAndFloat PowerAgainstAttackers;
	//QuickCombatResults( Defenders, Attackers, PowerAgainstAttackers );
	return CalculateCombatWin01( Attackers, Defenders );
	/*
	float fDamageToAttacker = 0.0f;
	float fDamageToDefender = 0.0f;
	//float fTotalAttrition01 = 0.0f;
	//float fTotalWeight = 0.0f;
	pos = Attackers.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplateAndFloat* pAttackerUnitTemplateAndFloat = Attackers.m_List.GetNext( pos );
		CUnitTemplate* pAttacker = pAttackerUnitTemplateAndFloat->m_pUnitTemplate;
		float fAttackerCost = pAttacker->GetProductionAndIronCost1() * pAttackerUnitTemplateAndFloat->m_fValue;

		POSITION pos2 = Defenders.m_List.GetHeadPosition();
		while( pos2 )
		{
			CUnitTemplateAndFloat* pDefenderUnitTemplateAndFloat = Defenders.m_List.GetNext( pos2 );
			CUnitTemplate* pDefender = pDefenderUnitTemplateAndFloat->m_pUnitTemplate;
			float fDefenderCost = pDefender->GetProductionAndIronCost1() * pDefenderUnitTemplateAndFloat->m_fValue;

			float fAttrition01 = CEOSAIQuickCombatCalculation::CalculateCombatWin01( pAttacker, pDefender, fRangeMultiplier );
			fTotalAttrition01 += fAttrition01 * (fAttackerCost + fDefenderCost);
			fTotalWeight += fAttackerCost + fDefenderCost;
		}
	}
	fTotalAttrition01 /= fTotalWeight;
	return fTotalAttrition01;
	QuickCombatResults( CEOSAIUnit2TemplatesAndFloat& MyUnits, CEOSAIUnit2TemplatesAndFloat& EnemyUnits, CEOSAIUnit2TemplatesAndFloat& CombatResults );
	*/
}

float CEOSAIQuickCombatCalculation::CalculateCombatWin01( CEOSAIUnit2TemplatesAndFloat& Attackers, CEOSAIUnit2TemplatesAndFloat& Defenders )
{
	CEOSAIUnit2TemplatesAndFloat PowerAgainstDefenders;
	QuickCombatResults( Attackers, Defenders, PowerAgainstDefenders );
	CEOSAIUnit2TemplatesAndFloat PowerAgainstAttackers;
	QuickCombatResults( Defenders, Attackers, PowerAgainstAttackers );

	POSITION pos;
	float fDefenderProduction = 0.0f;
	float fDefenderProductionDamage = 0.0f;
	pos = Defenders.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pUnitTemplateAndFloat = Defenders.m_List.GetNext( pos );
		fDefenderProduction +=
			pUnitTemplateAndFloat->m_fValue *
			pUnitTemplateAndFloat->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	pos = PowerAgainstDefenders.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pUnitTemplateAndFloat = PowerAgainstDefenders.m_List.GetNext( pos );
		fDefenderProductionDamage +=
			Defenders.GetValue( pUnitTemplateAndFloat->m_pAIUnitTemplate ) *
			pUnitTemplateAndFloat->m_fValue *
			pUnitTemplateAndFloat->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	float fAttackerProduction = 0.0f;
	float fAttackerProductionDamage = 0.0f;
	pos = Attackers.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pUnitTemplateAndFloat = Attackers.m_List.GetNext( pos );
		fAttackerProduction +=
			pUnitTemplateAndFloat->m_fValue *
			pUnitTemplateAndFloat->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	pos = PowerAgainstAttackers.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pUnitTemplateAndFloat = PowerAgainstAttackers.m_List.GetNext( pos );
		fAttackerProductionDamage +=
			Attackers.GetValue( pUnitTemplateAndFloat->m_pAIUnitTemplate ) *
			pUnitTemplateAndFloat->m_fValue *
			pUnitTemplateAndFloat->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}

	float fDefenderSurvival01 = (fDefenderProduction-fDefenderProductionDamage) / fDefenderProduction;
	float fAttackerSurvival01 = (fAttackerProduction-fAttackerProductionDamage) / fAttackerProduction;
	if( fDefenderSurvival01 == 0.0f && fAttackerSurvival01 == 0.0f ) return 0.5f;
	if( fDefenderSurvival01 == 0.0f && fAttackerSurvival01 > 0.0f )
	{
		return 0.5f + 0.5f*fAttackerSurvival01; // AttackerSurvival 1.0 = return 1.0
	}
	if( fDefenderSurvival01 > 0.0f && fAttackerSurvival01 == 0.0f )
	{
		return 0.5f - 0.5f*fDefenderSurvival01; // DefenderSurvival 1.0 = return 0.0
	}
	return fAttackerSurvival01 / (fAttackerSurvival01 + fDefenderSurvival01);
}

bool CEOSAIQuickCombatCalculation::AllSignificantAttackersWereDestroyed()
{
	// Either: all defenders are dead, 
	//   or they are causing little/no damage (the attackers can survive 10 turns)
	bool bAllUnitsAreDead = true;
	//float fTotalExpectedDamage = 0.0f;
	float fTotalCombatSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_fCurrentHP <= 0.0f ) continue;
		//if( pUnit->m_bIsDead ) continue;
		float fCombatSignificance01 = pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatAttritionSignificance01();
		if( fCombatSignificance01 > 0.2f ) return false;

		//fTotalCombatSignificance += pUnit->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatSignificance();
		/*
		bAllDefendersAreDead = false;
		float fExpectedDamage = 0.0f;
		CEOSAIQuickCombatUnit* pBestTarget = FindBestTarget( pUnit, &m_AttackerUnits, &fExpectedDamage );
		fTotalExpectedDamage += fExpectedDamage;
		*/
	}
	//if( bAllUnitsAreDead ) return true;
	//if( fTotalExpectedDamage == 0.0f ) return true;

	/*
	// Add-up the HP of all the attackers
	float fTotalAttackerHP = 0.0f;
	pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		fTotalAttackerHP += pUnit->m_fCurrentHP;
	}
	return ( fTotalAttackerHP / fTotalExpectedDamage > 10.0f );
	*/
	return true;
}

bool CEOSAIQuickCombatCalculation::AllSignificantDefendersWereDestroyed()
{
	// Either: all defenders are dead, 
	//   or they are causing little/no damage (the attackers can survive 10 turns)
	float fTotalExpectedDamage = 0.0f;
	bool bAllUnitsAreDead = true;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_fCurrentHP <= 0.0f ) continue;
		//if( pUnit->m_bIsDead ) continue;
		float fCombatSignificance01 = pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatAttritionSignificance01();
		if( fCombatSignificance01 > 0.2f ) return false;
		/*
		bAllUnitsAreDead = false;
		float fExpectedDamage = 0.0f;
		CEOSAIQuickCombatUnit* pBestTarget = FindBestTarget( pUnit, &m_AttackerUnits, &fExpectedDamage );
		fTotalExpectedDamage += fExpectedDamage;
		*/
	}
	/*
	if( bAllUnitsAreDead ) return true;
	if( fTotalExpectedDamage == 0.0f ) return true;

	// Add-up the HP of all the attackers
	float fTotalAttackerHP = 0.0f;
	pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		fTotalAttackerHP += pUnit->m_fCurrentHP;
	}
	return ( fTotalAttackerHP / fTotalExpectedDamage > 10.0f );
	*/
	return true;
}

float CEOSAIQuickCombatCalculation::GetAttackerSurvivalAsCombatSignificance01()
{
	float fInitialSignificance = 0.0f;
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );

		float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialSignificance += fInitialHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		float fSurvivingHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingSignificance += fSurvivingHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fSurvivingSignificance / ( fInitialSignificance + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetDefenderSurvivalAsCombatSignificance01()
{
	float fInitialSignificance = 0.0f;
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );

		float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialSignificance += fInitialHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		float fSurvivingHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingSignificance += fSurvivingHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fSurvivingSignificance / ( fInitialSignificance + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetAttackerSurvivalAsProductionCost01()
{
	float fInitialProduction = 0.0f;
	float fSurvivingProduction = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );

		float fHPPercent = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialProduction += fHPPercent*pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
		float fSurvivalHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingProduction += fSurvivalHP01*pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	return fSurvivingProduction / ( fInitialProduction + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetDefenderSurvivalAsProductionCost01()
{
	float fInitialProduction = 0.0f;
	float fSurvivingProduction = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );

		float fHPPercent = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialProduction += fHPPercent*pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
		float fSurvivalHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingProduction += fSurvivalHP01*pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
		//float fHPPercent = pUnit->m_fInitialHP / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		//fTotalProduction += fHPPercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();
		//float fHPDamagePercent = (pUnit->m_fInitialHP - pUnit->m_fCurrentHP) / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		//fProductionDamage += fHPDamagePercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();
		//
		//fSurvival01 = (fTotalProduction-fProductionDamage) / fTotalProduction;
	}
	return fSurvivingProduction / ( fInitialProduction + 0.0001f );
	//return fSurvival01;
}

float CEOSAIQuickCombatCalculation::GetDefenderSurvivalAsCombatSignificance()
{
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_fCurrentHP > 0.0f )
		{
			float fHP01 = pUnit->m_fCurrentHP/pUnit->m_pAIUnitTemplate->GetMaxHP();
			fSurvivingSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		}
	}
	return fSurvivingSignificance;
}

float CEOSAIQuickCombatCalculation::GetAttackerDamageAsProduction()
{
	float fDamageSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		float fHP01 = (pUnit->m_fInitialHP-pUnit->m_fCurrentHP)/pUnit->m_pAIUnitTemplate->GetMaxHP();
		fDamageSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	return fDamageSignificance;
}

float CEOSAIQuickCombatCalculation::GetDefenderDamageAsProduction()
{
	float fDamageSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		float fHP01 = (pUnit->m_fInitialHP-pUnit->m_fCurrentHP)/pUnit->m_pAIUnitTemplate->GetMaxHP();
		fDamageSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	return fDamageSignificance;
}

float CEOSAIQuickCombatCalculation::GetDefenderDamageAsCombatSignificance()
{
	float fDamageSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		float fHP01 = (pUnit->m_fInitialHP-pUnit->m_fCurrentHP)/pUnit->m_pAIUnitTemplate->GetMaxHP();
		fDamageSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fDamageSignificance;
}

float CEOSAIQuickCombatCalculation::GetAttackerGroundUnitDamageAsProduction()
{
	float fDamageSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsGroundUnit() == false ) continue;

		float fHP01 = (pUnit->m_fInitialHP-pUnit->m_fCurrentHP)/pUnit->m_pAIUnitTemplate->GetMaxHP();
		fDamageSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	return fDamageSignificance;
}

float CEOSAIQuickCombatCalculation::GetAttackerGroundUnitSurvivalAsCombatSignificance01()
{
	float fInitialSignificance = 0.0f;
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsGroundUnit() == false ) continue;

		float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialSignificance += fInitialHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		float fSurvivingHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingSignificance += fSurvivingHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fSurvivingSignificance / ( fInitialSignificance + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetAttackerGroundUnitSurvivalAsCombatSignificance()
{
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsGroundUnit() == false ) continue;
		if( pUnit->m_fCurrentHP > 0.0f )
		{
			float fHP01 = pUnit->m_fCurrentHP/pUnit->m_pAIUnitTemplate->GetMaxHP();
			fSurvivingSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		}
	}
	return fSurvivingSignificance;
}

float CEOSAIQuickCombatCalculation::GetDefenderGroundUnitSurvivalAsCombatSignificance01()
{
	float fInitialSignificance = 0.0f;
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsGroundUnit() == false ) continue;

		float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialSignificance += fInitialHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		float fSurvivingHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingSignificance += fSurvivingHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fSurvivingSignificance / ( fInitialSignificance + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetDefenderGroundUnitSurvivalAsCombatSignificance()
{
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsGroundUnit() == false ) continue;
		if( pUnit->m_fCurrentHP > 0.0f )
		{
			float fHP01 = pUnit->m_fCurrentHP/pUnit->m_pAIUnitTemplate->GetMaxHP();
			fSurvivingSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		}
	}
	return fSurvivingSignificance;
}

/*
float CEOSAIQuickCombatCalculation::GetGroundCombatErrorSize01()
{
	float fTotalHP = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pUnitTemplate->IsGroundUnit() == false ) continue;
		fTotalHP += pUnit->m_fInitialHP;
	}
	pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pUnitTemplate->IsGroundUnit() == false ) continue;
		fTotalHP += pUnit->m_fInitialHP;
	}
	return 1.0f / (fTotalHP+1.0f); // 2HP = 0.33, 3HP = 0.25, 4HP = 0.20, .. 9HP = 0.10
}
*/
/*
float CEOSAIQuickCombatCalculation::GetAttackerSeaUnitSurvival01()
{
	float fSurvival01 = 0.0f;
	float fTotalProduction = 0.0f;
	float fProductionDamage = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pUnitTemplate->IsSeaUnit() == false ) continue;

		float fHPPercent = pUnit->m_fInitialHP / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		fTotalProduction += fHPPercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();
		float fHPDamagePercent = (pUnit->m_fInitialHP - pUnit->m_fCurrentHP) / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		fProductionDamage += fHPDamagePercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();

		fSurvival01 = (fTotalProduction-fProductionDamage) / fTotalProduction;
	}
	return fSurvival01;
}

float CEOSAIQuickCombatCalculation::GetDefenderSeaUnitSurvival01()
{
	float fSurvival01 = 0.0f;
	float fTotalProduction = 0.0f;
	float fProductionDamage = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pUnitTemplate->IsSeaUnit() == false ) continue;

		float fHPPercent = pUnit->m_fInitialHP / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		fTotalProduction += fHPPercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();
		float fHPDamagePercent = (pUnit->m_fInitialHP - pUnit->m_fCurrentHP) / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		fProductionDamage += fHPDamagePercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();

		fSurvival01 = (fTotalProduction-fProductionDamage) / fTotalProduction;
	}
	return fSurvival01;
}
*/

float CEOSAIQuickCombatCalculation::GetAttackerSeaUnitDamageAsProduction()
{
	float fDamageSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsSeaUnit() == false ) continue;

		{
			float fHP01 = (pUnit->m_fInitialHP-pUnit->m_fCurrentHP)/pUnit->m_pAIUnitTemplate->GetMaxHP();
			fDamageSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
		}
	}
	return fDamageSignificance;
}

float CEOSAIQuickCombatCalculation::GetAttackerSeaUnitSurvivalAsCombatSignificance01()
{
	float fInitialSignificance = 0.0f;
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsSeaUnit() == false ) continue;

		float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialSignificance += fInitialHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		float fSurvivingHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingSignificance += fSurvivingHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fSurvivingSignificance / ( fInitialSignificance + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetAttackerSeaUnitSurvivalAsCombatSignificance()
{
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsSeaUnit() == false ) continue;
		//fSurvivingSignificance += pUnit->m_fCurrentHP * pUnit->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatSignificance();
		if( pUnit->m_fCurrentHP > 0.0f )
		{
			float fHP01 = pUnit->m_fCurrentHP/pUnit->m_pAIUnitTemplate->GetMaxHP();
			fSurvivingSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		}
	}
	return fSurvivingSignificance;
}

float CEOSAIQuickCombatCalculation::GetDefenderSeaUnitSurvivalAsCombatSignificance01()
{
	float fInitialSignificance = 0.0f;
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsSeaUnit() == false ) continue;

		float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialSignificance += fInitialHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		float fSurvivingHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingSignificance += fSurvivingHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fSurvivingSignificance / ( fInitialSignificance + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetDefenderSeaUnitSurvivalAsCombatSignificance()
{
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsSeaUnit() == false ) continue;
		//fSurvivingSignificance += pUnit->m_fCurrentHP * pUnit->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatSignificance();
		if( pUnit->m_fCurrentHP > 0.0f )
		{
			float fHP01 = pUnit->m_fCurrentHP/pUnit->m_pAIUnitTemplate->GetMaxHP();
			fSurvivingSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		}
	}
	return fSurvivingSignificance;
}

/*
float CEOSAIQuickCombatCalculation::GetAttackerAirUnitSurvival01()
{
	float fSurvival01 = 0.0f;
	float fTotalProduction = 0.0f;
	float fProductionDamage = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pUnitTemplate->IsAirUnit() == false ) continue;

		float fHPPercent = pUnit->m_fInitialHP / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		fTotalProduction += fHPPercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();
		float fHPDamagePercent = (pUnit->m_fInitialHP - pUnit->m_fCurrentHP) / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		fProductionDamage += fHPDamagePercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();

		fSurvival01 = (fTotalProduction-fProductionDamage) / fTotalProduction;
	}
	return fSurvival01;
}

float CEOSAIQuickCombatCalculation::GetDefenderAirUnitSurvival01()
{
	float fSurvival01 = 0.0f;
	float fTotalProduction = 0.0f;
	float fProductionDamage = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pUnitTemplate->IsAirUnit() == false ) continue;

		float fHPPercent = pUnit->m_fInitialHP / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		fTotalProduction += fHPPercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();
		float fHPDamagePercent = (pUnit->m_fInitialHP - pUnit->m_fCurrentHP) / (float) pUnit->m_pUnitTemplate->GetMaxHP();
		fProductionDamage += fHPDamagePercent*pUnit->m_pUnitTemplate->GetProductionAndIronCost1();

		fSurvival01 = (fTotalProduction-fProductionDamage) / fTotalProduction;
	}
	return fSurvival01;
}
*/

float CEOSAIQuickCombatCalculation::GetAttackerAirUnitDamageAsProduction()
{
	float fDamageSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsAirUnit() == false ) continue;

		float fHP01 = (pUnit->m_fInitialHP-pUnit->m_fCurrentHP)/pUnit->m_pAIUnitTemplate->GetMaxHP();
		fDamageSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetProductionAndIronCost1();
	}
	return fDamageSignificance;
}

float CEOSAIQuickCombatCalculation::GetAttackerAirUnitSurvivalAsCombatSignificance01()
{
	float fInitialSignificance = 0.0f;
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsAirUnit() == false ) continue;

		float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialSignificance += fInitialHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		float fSurvivingHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingSignificance += fSurvivingHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fSurvivingSignificance / ( fInitialSignificance + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetAttackerAirUnitSurvivalAsCombatSignificance()
{
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsAirUnit() == false ) continue;
		//fSurvivingSignificance += pUnit->m_fCurrentHP * pUnit->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatSignificance();
		if( pUnit->m_fCurrentHP > 0.0f )
		{
			float fHP01 = pUnit->m_fCurrentHP/pUnit->m_pAIUnitTemplate->GetMaxHP();
			fSurvivingSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		}
	}
	return fSurvivingSignificance;
}

float CEOSAIQuickCombatCalculation::GetDefenderAirUnitSurvivalAsCombatSignificance01()
{
	float fInitialSignificance = 0.0f;
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsAirUnit() == false ) continue;

		float fInitialHP01 = pUnit->m_fInitialHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fInitialSignificance += fInitialHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		float fSurvivingHP01 = pUnit->m_fCurrentHP / (float) pUnit->m_pAIUnitTemplate->GetMaxHP();
		fSurvivingSignificance += fSurvivingHP01*pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	return fSurvivingSignificance / ( fInitialSignificance + 0.0001f );
}

float CEOSAIQuickCombatCalculation::GetDefenderAirUnitSurvivalAsCombatSignificance()
{
	float fSurvivingSignificance = 0.0f;
	POSITION pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{ 
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pAIUnitTemplate->IsAirUnit() == false ) continue;
		//fSurvivingSignificance += pUnit->m_fCurrentHP * pUnit->m_pUnitTemplate->GetUnitCombatCapability()->GetCombatSignificance();
		if( pUnit->m_fCurrentHP > 0.0f )
		{
			float fHP01 = pUnit->m_fCurrentHP/pUnit->m_pAIUnitTemplate->GetMaxHP();
			fSurvivingSignificance += fHP01 * pUnit->m_pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		}
	}
	return fSurvivingSignificance;
}

/*
float CEOSAIQuickCombatCalculation::GetSeaCombatErrorSize01()
{
	float fTotalHP = 0.0f;
	POSITION pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_AttackerUnits.GetNext( pos );
		if( pUnit->m_pUnitTemplate->IsSeaUnit() ||
			) continue;
		fTotalHP += pUnit->m_fInitialHP;
	}
	pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIQuickCombatUnit* pUnit = m_DefenderUnits.GetNext( pos );
		if( pUnit->m_pUnitTemplate->IsSeaUnit() == false ) continue;
		fTotalHP += pUnit->m_fInitialHP;
	}
	return 1.0f / (fTotalHP+1.0f); // 1HP = 0.5, 2HP = 0.33, 3HP = 0.25, 4HP = 0.20, .. 9HP = 0.10
}
*/

/*
void CEOSAIQuickCombatCalculation::RunCalculation_old()
{
	m_Attacker_UnitHPDefense_UnitTypeSet.Clear();
	m_Attacker_CombatPowerVsUnitTypes.Clear();
	m_Defender_UnitHPDefense_UnitTypeSet.Clear();
	m_Defender_CombatPowerVsUnitTypes.Clear();

	// Setup the StringLists
	POSITION pos, pos2;
	pos = m_AttackerUnits.GetHeadPosition();
	while( pos )
	{
		CUnit* pUnit = m_AttackerUnits.GetNext( pos );
		CUnitTemplate* pUnitTemplate = pUnit->GetUnitTemplate();

		// Units that self-destruct don't get added to the HPDefense Value
		if( pUnit->SelfDestructsOnAttack() )
		{
			CUnitTemplate* pUnitTemplate = pUnit->GetUnitTemplate();
			pos2 = pUnitTemplate->GetAttackVsList()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIAttackVs2* pAttackVs = pUnitTemplate->GetAttackVsList()->GetNext( pos2 );
				float fEstimatedDamage = pAttackVs->GetToHit()*pAttackVs->GetDamage();
				m_Attacker_MissileCombatPowerVsUnitTypes.Add( pAttackVs->GetCombatUnitType(), fEstimatedDamage );
			}
		}
		else
		{
			float fHPDefenseValue = pUnit->GetCurrentHP() * pUnit->GetDefenseValueForNormalTerrain();
			m_Attacker_UnitHPDefense_UnitTypeSet.Add( pUnit->GetCombatUnitTypeString(), fHPDefenseValue );

			pos2 = pUnitTemplate->GetAttackVsList()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIAttackVs2* pAttackVs = pUnitTemplate->GetAttackVsList()->GetNext( pos2 );
				float fEstimatedDamage = pAttackVs->GetToHit()*pAttackVs->GetDamage();
				m_Attacker_CombatPowerVsUnitTypes.Add( pAttackVs->GetCombatUnitType(), fEstimatedDamage );
			}
		}
	}
	pos = m_DefenderUnits.GetHeadPosition();
	while( pos )
	{
		CUnit* pUnit = m_DefenderUnits.GetNext( pos );
		CUnitTemplate* pUnitTemplate = pUnit->GetUnitTemplate();

		// Units that self-destruct don't get added to the HPDefense Value
		if( pUnit->SelfDestructsOnAttack() )
		{
			CUnitTemplate* pUnitTemplate = pUnit->GetUnitTemplate();
			pos2 = pUnitTemplate->GetAttackVsList()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIAttackVs2* pAttackVs = pUnitTemplate->GetAttackVsList()->GetNext( pos2 );
				float fEstimatedDamage = pAttackVs->GetToHit()*pAttackVs->GetDamage();
				m_Defender_MissileCombatPowerVsUnitTypes.Add( pAttackVs->GetCombatUnitType(), fEstimatedDamage );
			}
		}
		else
		{
			float fHPDefenseValue = pUnit->GetCurrentHP() * pUnit->GetDefenseValueForNormalTerrain();
			m_Defender_UnitHPDefense_UnitTypeSet.Add( pUnit->GetCombatUnitTypeString(), fHPDefenseValue );

			pos2 = pUnitTemplate->GetAttackVsList()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIAttackVs2* pAttackVs = pUnitTemplate->GetAttackVsList()->GetNext( pos2 );
				float fEstimatedDamage = pAttackVs->GetToHit()*pAttackVs->GetDamage();
				m_Defender_CombatPowerVsUnitTypes.Add( pAttackVs->GetCombatUnitType(), fEstimatedDamage );
			}
		}
	}

	// Apply Missile Effects
	//pos = m_Defender_UnitHPDefense_UnitTypeSet.m_List.

	// Calculate Time to consume each category
	This algorithm makes the assumption that power is spread uniformly across the units
		However, that's not actually true.  An infantry + satellite produces a problem.
		The combat power is assumed to be spread over both units, and that means destroying the
		infantry only halves the HPDefense value.  It's assumed that the combat power is still
		around in the untouchable satellite.
	I can break this apart into a 2d-grid to make it more accurate.

//	CombatPower    |  VsSoft  VsArmor  VsAir
//	-----------------------------------------------------------------------------
//	Soft: 10       |    5        3       3
//	Armor: 20      |    4        4       4
//	Air: 30        |    0        0       0

	// Run combat
	for( long i=0; i<100; i++ )
	{
		// Count-up the HPDefense defender types
		float fTotalDefenderHPDefender = 0.0f;
		pos = m_Defender_UnitHPDefense_UnitTypeSet.m_List.GetHeadPosition();
		while( pos )
		{
			EOSAI::StringAndFloat* pStringAndFloat = m_Defender_UnitHPDefense_UnitTypeSet.m_List.GetNext( pos );
			fTotalDefenderHPDefender += pStringAndFloat->m_fValue;
		}
		float fTotalAttackerHPDefender = 0.0f;
		pos = m_Attacker_UnitHPDefense_UnitTypeSet.m_List.GetHeadPosition();
		while( pos )
		{
			EOSAI::StringAndFloat* pStringAndFloat = m_Attacker_UnitHPDefense_UnitTypeSet.m_List.GetNext( pos );
			fTotalAttackerHPDefender += pStringAndFloat->m_fValue;
		}

		//
		pos = m_Attacker_CombatPowerVsUnitTypes.m_List.GetHeadPosition();
		while( pos )
		{
			EOSAI::StringAndFloat* pAttackerCombatFloat = m_Attacker_CombatPowerVsUnitTypes.GetNext( pos );
			m_Defender_UnitHPDefense_UnitTypeSet.Add( 
				pAttackerCombatFloat->m_strValue, -pAttackerCombatFloat->m_fValue );
		}
	}
}
*/
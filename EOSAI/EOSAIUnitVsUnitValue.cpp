#include "stdafx.h"
#include "EOSAIUnitVsUnitValue.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIQuickCombatCalculation.h"
#include "EOSAICommonData2.h"
#include "AIPlayer.h"
#include "EOSAIPlayerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIUnit2VsUnitValue::CEOSAIUnit2VsUnitValue( CEOSAIUnitTemplate* pAttacker, CEOSAIUnitTemplate* pDefender )
{
	m_pAttackerUnitTemplate = pAttacker;
	m_pTargetUnitTemplate = pDefender;
/*
	#ifdef _DEBUG
	if( m_pAttackerUnitTemplate->GetInternalName() == _T("Cruiser (Class 1)") &&
		m_pTargetUnitTemplate->GetInternalName() == _T("Submarine (Class 2)") )
	{
		//ASSERT( false );
		int g=0;
	}
	#endif
*/
	// Calculate Mobility (sort-of a "freedom of attack")
	//   Offensive roles depend more on MovementRate, AirMovement and somewhat on AttackRange
	//   Defensive roles depend less on MovementRate, and heavier on AttackRange
	m_fMovementRateBalance = ( pAttacker->GetMovementRate()+0.001f ) / ( pAttacker->GetMovementRate() + pDefender->GetMovementRate() + 0.001f );
	m_fMovementRateBalance -= 0.5f;

	// Terrain-movement freedom
	m_fTerrainMovementAdvantage = 0.0f;
	if( pAttacker->IsAirUnit() ){ m_fTerrainMovementAdvantage += 0.3f; }
	if( pDefender->IsAirUnit() ){ m_fTerrainMovementAdvantage -= 0.3f; }
	if( pAttacker->IsGroundUnit() && pDefender->IsSeaUnit() ){ m_fTerrainMovementAdvantage -= 0.5f; }
	if( pAttacker->IsSeaUnit() && pDefender->IsGroundUnit() ){ m_fTerrainMovementAdvantage -= 0.4f; }

	// Attack Range
	float fAttackRange1 = pAttacker->GetAttackRangeVs( pDefender );
	float fAttackRange2 = pDefender->GetAttackRangeVs( pAttacker );
	float fAttackRangeGap = fAttackRange1 - fAttackRange2;
	m_fAttackRangeAdvantage = fAttackRangeGap/400.0f;

	float fHPDamageToDefender = m_pAttackerUnitTemplate->GetExpectedHPDamage( m_pTargetUnitTemplate );
	float fHPDamageToAttacker = m_pTargetUnitTemplate->GetExpectedHPDamage( m_pAttackerUnitTemplate );

	m_fOutgoingProdDamagePerTurn = 
		m_pTargetUnitTemplate->GetProductionAndIronCost1() *
		fHPDamageToDefender / m_pTargetUnitTemplate->GetMaxHP();
		//m_pAttackerUnitTemplate->GetExpectedHPDamage( m_pTargetUnitTemplate ) / m_pTargetUnitTemplate->GetMaxHP();
	m_fIncomingProdDamagePerTurn = 
		m_pAttackerUnitTemplate->GetProductionAndIronCost1() *
		fHPDamageToAttacker / m_pAttackerUnitTemplate->GetMaxHP();
		//m_pTargetUnitTemplate->GetExpectedHPDamage( m_pAttackerUnitTemplate ) / m_pAttackerUnitTemplate->GetMaxHP();

	//
	if( m_pAttackerUnitTemplate->GetViewRange( m_pTargetUnitTemplate ) == 0.0f )
	{
		m_fMeleeAttrition01 = 0.0f;
		m_fRangedAttrition01 = 0.0f;
		m_fRangeX2Attrition01 = 0.0f;
	}
	else
	{
		m_fMeleeAttrition01 = CEOSAIQuickCombatCalculation::GetMeleeAttrition01( m_pAttackerUnitTemplate,m_pTargetUnitTemplate );
		ASSERT( m_fMeleeAttrition01 <= 1.0f );
		m_fRangedAttrition01 = CEOSAIQuickCombatCalculation::GetRangedAttrition01( m_pAttackerUnitTemplate,m_pTargetUnitTemplate );
		ASSERT( m_fRangedAttrition01 <= 1.0f );
		m_fRangeX2Attrition01 = CEOSAIQuickCombatCalculation::GetRangeX2Attrition01( m_pAttackerUnitTemplate,m_pTargetUnitTemplate );
		ASSERT( m_fRangeX2Attrition01 <= 1.0f );

		if( m_pAttackerUnitTemplate->GetViewRange( m_pTargetUnitTemplate ) < 10.0f )
		{
			m_fMeleeAttrition01 *= 0.5f;
			m_fRangedAttrition01 *= 0.5f;
			m_fRangeX2Attrition01 *= 0.5f;
		}
	}

//	m_fMyRelativeMeleeAttritionValue01 = 0.0f;
//	m_fMyRelativeRangedAttritionValue01 = 0.0f;
//	m_fMyRelativeRangeX2AttritionValue01 = 0.0f;

	// Check the Offense/Defense freedom of movement
	//float fOffense = pValue->GetOffenseFreedomOfAttack();
	//float fDefense = pValue->GetDefenseFreedomOfAttack();
}

// CombatSignificance = My Unit's Attrition Value against Target * MyUnits Cost
float CEOSAIUnit2VsUnitValue::GetCombatSignificance()
{
	if( m_fOutgoingProdDamagePerTurn == 0.0f ) return 0.0f;

	// Attacking across land/water boundaries is hard
	float fValue = 1.0f;
	if( m_pAttackerUnitTemplate->IsGroundUnit() && m_pTargetUnitTemplate->IsSeaUnit() ){ fValue *= 0.3f; }
	if( m_pAttackerUnitTemplate->IsSeaUnit() && m_pTargetUnitTemplate->IsGroundUnit() ){ fValue *= 0.3f; }

	float fAttrition =
			0.50f * ( GetMeleeAttrition01() + GetRangedAttrition01() + GetRangeX2Attrition01() + max( GetMeleeAttrition01(), GetRangedAttrition01() ) );
	fValue *= fAttrition;
	fValue *= m_pAttackerUnitTemplate->GetProductionCost();
	return fValue;
}

bool CEOSAIUnit2VsUnitValue::IsUsefulInCombat()
{
	long iMyPlayerNumber = g_pAIPlayerManager->CurrentlyProcessingAIPlayer();

	// An infantry vs. Infantry will have an AverageAttrition01 of 0.5
	//   The AverageAttrition01 tends not to get too much over 0.5 against enemy combat units
	// However, I should always have one unit that has a relative attrition of 1.0
	// That's why these are weighted like this - because I want to use my best unit even when attrition is bad
	// And I'll be allowed to use good-attrition units even if the relative value is bad (e.g. if my tech is a lot better than his)

	// I need to take mobility into account
	//   I don't want a very good attrition (e.g. Attack Satellite->Infantry, Battleship->Infantry) 
	//   throwing off my relative attrition rating because they have severe restrictions.

	if( m_pAttackerUnitTemplate->IsGroundUnit() && m_pTargetUnitTemplate->IsSeaUnit() ) return false;
	if( m_pAttackerUnitTemplate->IsSeaUnit() && m_pTargetUnitTemplate->IsGroundUnit() ) return false;

	if( m_fOutgoingProdDamagePerTurn == 0.0f ) return false;

	// This unit is useful if my unit has a range advantage
	//   Maybe the target won't move (or I have a defensive line)
	if( m_fAttackRangeAdvantage > 0.0f && m_fOutgoingProdDamagePerTurn > 0.0f ) return true;

	// My best units are always considered 'useful'
	//if( GetMyRelativeAttritionValue01() > 0.9f ) return true;

	if( GetMeleeAttrition01() > 0.35f ) return true;
	if( GetRangeX2Attrition01() > 0.35f ) return true;

	if( GetMyRelativeMeleeAttritionValue01( iMyPlayerNumber ) > 0.7f ) return true;
	if( GetMyRelativeRangeX2AttritionValue01( iMyPlayerNumber ) > 0.7f ) return true;

	// Cases:                                                                                    50/50 70/30
	// Infantry (c4) vs. His Infantry (c1) (Attrition 0.7), Relative Attrition 0.2 (poor)    = 0.45  0.55
	// Infantry (c1) vs. His Infantry (c1) (Attrition 0.5), Relative Attrition 1.0 (my best) = 0.75  0.65
	// Infantry (c1) vs. His Infantry (c4) (Attrition 0.2), Relative Attrition 1.0 (my best) = 0.6   0.44
	//float fAverageAttrition01 = (m_fMeleeAttrition01 + m_fRangedAttrition01)/2.0f;
	/*
	float fUsefulness1 = 0.5f*m_fMeleeAttrition01 + 0.5f*m_fMyRelativeMeleeAttritionValue01;
	float fUsefulness2 = 0.7f*m_fMeleeAttrition01 + 0.3f*m_fMyRelativeMeleeAttritionValue01;
	float fUsefulness01 = max( fUsefulness1, fUsefulness2 );
	return ( fUsefulness01 > 0.5f );
	*/
	//float fUsefulness01 = 0.8f*m_fMeleeAttrition01 + 0.2f*m_fMyRelativeMeleeAttritionValue01;
	//float fUsefulness01 = 0.8f*GetAverageAttrition01() + 0.2f*GetMyRelativeAttritionValue01();
	//return ( fUsefulness01 > 0.5f );
	return false;
}
/*
float CEOSAIUnit2VsUnitValue::GetCombatUsefulness01()
{
	// An infantry vs. Infantry will have an AverageAttrition01 of 0.5
	//   The AverageAttrition01 tends not to get too much over 0.5 against enemy combat units
	// However, I should always have one unit that has a relative attrition of 1.0
	// That's why these are weighted like this - because I want to use my best unit even when attrition is bad
	// And I'll be allowed to use good-attrition units even if the relative value is bad (e.g. if my tech is a lot better than his)

	// I need to take mobility into account
	//   I don't want a very good attrition (e.g. Attack Satellite->Infantry, Battleship->Infantry) 
	//   throwing off my relative attrition rating because they have severe restrictions.

	// Cases:
	//   My Infantry vs. His Infantry (Attrition 0.5), Relative Attrition of 1.0 (my best) = 0.75
	float fAverageAttrition01 = (m_fMeleeAttrition01 + m_fRangedAttrition01)/2.0f;
	float fUsefulness1 = 0.5f*fAverageAttrition01 + 0.5f*m_fMyRelativeAttritionValue01;
	float fUsefulness2 = 0.8f*fAverageAttrition01 + 0.2f*m_fMyRelativeAttritionValue01;
	return max( fUsefulness1, fUsefulness2 );
}
*/
float CEOSAIUnit2VsUnitValue::GetOffenseFreedomOfAttack()
{
	return m_fMovementRateBalance*1.2f + m_fTerrainMovementAdvantage*1.2f + m_fAttackRangeAdvantage*0.5f;
}

float CEOSAIUnit2VsUnitValue::GetDefenseFreedomOfAttack()
{
	return m_fMovementRateBalance*0.5f + m_fTerrainMovementAdvantage*0.5f + m_fAttackRangeAdvantage*1.5f;
}

float CEOSAIUnit2VsUnitValue::GetMyRelativeMeleeAttritionValue01( long iMyPlayerNumber )
{
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iMyPlayerNumber );
	if( pAIPlayer &&
		pAIPlayer->GetAIBrain() &&
		pAIPlayer->GetAIBrain()->GetAIThoughtDatabase() )
	{
		float fBestValue = 0.0f;
		CEOSAIUnitTemplateSet* pUnitTemplateSet = pAIPlayer->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsIHave();
		POSITION pos = pUnitTemplateSet->m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pMyUnitTemplate = pUnitTemplateSet->m_List.GetNext( pos );
			float fValue = pMyUnitTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( m_pTargetUnitTemplate )->GetMeleeAttrition01();
			fBestValue = max( fBestValue, fValue );
		}
		if( fBestValue == 0.0f ){ fBestValue = 1.0f; }
		return this->GetMeleeAttrition01() / fBestValue;
	}
	ASSERT( false );
	return 1.0f;
}

float CEOSAIUnit2VsUnitValue::GetMyRelativeRangedAttritionValue01( long iMyPlayerNumber )
{
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iMyPlayerNumber );
	if( pAIPlayer &&
		pAIPlayer->GetAIBrain() &&
		pAIPlayer->GetAIBrain()->GetAIThoughtDatabase() )
	{
		float fBestValue = 0.0f;
		CEOSAIUnitTemplateSet* pUnitTemplateSet = pAIPlayer->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsIHave();
		POSITION pos = pUnitTemplateSet->m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pMyUnitTemplate = pUnitTemplateSet->m_List.GetNext( pos );
			float fValue = pMyUnitTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( m_pTargetUnitTemplate )->GetRangedAttrition01();
			fBestValue = max( fBestValue, fValue );
		}
		if( fBestValue == 0.0f ){ fBestValue = 1.0f; }
		return this->GetRangedAttrition01() / fBestValue;
	}
	ASSERT( false );
	return 1.0f;
}

float CEOSAIUnit2VsUnitValue::GetMyRelativeRangeX2AttritionValue01( long iMyPlayerNumber )
{
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iMyPlayerNumber );
	if( pAIPlayer &&
		pAIPlayer->GetAIBrain() &&
		pAIPlayer->GetAIBrain()->GetAIThoughtDatabase() )
	{
		float fBestValue = 0.0f;
		CEOSAIUnitTemplateSet* pUnitTemplateSet = pAIPlayer->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsIHave();
		POSITION pos = pUnitTemplateSet->m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pMyUnitTemplate = pUnitTemplateSet->m_List.GetNext( pos );
			float fValue = pMyUnitTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( m_pTargetUnitTemplate )->GetRangeX2Attrition01();
			fBestValue = max( fBestValue, fValue );
		}
		if( fBestValue == 0.0f ){ fBestValue = 1.0f; }
		return this->GetRangeX2Attrition01() / fBestValue;
	}
	ASSERT( false );
	return 1.0f;
}
/*
float CEOSAIUnit2VsUnitValue::GetMyRelativeAttritionValue01( long iMyPlayerNumber )
{
}
*/

float CEOSAIUnit2VsUnitValue::GetTimeToDestroyTarget()
{
	if( m_fOutgoingProdDamagePerTurn == 0.0f ) return 1000000.0f;
	return m_pTargetUnitTemplate->GetProductionAndIronCost1() / m_fOutgoingProdDamagePerTurn;
	//return m_pTargetUnitTemplate->GetMaxHP() / m_fOutgoingHPDamagePerTurn;
}

float CEOSAIUnit2VsUnitValue::GetTimeToBeDestroyed()
{
	if( m_fIncomingProdDamagePerTurn == 0.0f ) return 1000000.0f;
	return m_pAttackerUnitTemplate->GetProductionAndIronCost1() / m_fIncomingProdDamagePerTurn;
	//return m_pAttackerUnitTemplate->GetMaxHP() / m_fIncomingHPDamagePerTurn;
}


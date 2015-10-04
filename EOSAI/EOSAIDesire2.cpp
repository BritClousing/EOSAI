
#include "stdafx.h"
#include "EOSAIDesire2.h"
#include "EOSAIUnitTemplate.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"
//#include "WorldDescPlayerProxy.h"
#include "EOSAIBrain.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIDesire2::CEOSAIDesire2()
{
	m_pAIBrain = NULL;
	//m_pTacticalProject = NULL;

	SetTimeValueFunction_PlanLongterm();

	m_iAggressiveTowardsPlayer = 0;

	// Scoring
	m_eScoringLevel = EnumUndefined;
	m_fScore = 0.0f;
}

CEOSAIThoughtDatabase* CEOSAIDesire2::GetAIThoughtDatabase()
{
	return m_pAIBrain->GetAIThoughtDatabase();
}

bool CEOSAIDesire2::IsAPrimaryGoalOfTacticalProject( CAITacticalProject* p )
{
	POSITION pos = m_TacticalProjectsIAmAPrimaryMemberOf.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacProject = m_TacticalProjectsIAmAPrimaryMemberOf.GetNext( pos );
		if( pTacProject == p ) return true;
	}
	return false;
}

bool CEOSAIDesire2::IsAPrimaryGoalOfOneOrMoreTacticalProjects()
{
	return ( m_TacticalProjectsIAmAPrimaryMemberOf.IsEmpty() == FALSE );
}

void CEOSAIDesire2::AddAsPrimaryGoalOfTacticalProject( CAITacticalProject* p )
{
	ASSERT( IsAPrimaryGoalOfTacticalProject( p ) == false );
	m_TacticalProjectsIAmAPrimaryMemberOf.AddTail( p );
}
/*
void CEOSAIDesire2::RemoveAsPrimaryGoalOfTacticalProject( CAITacticalProject* p )
{
	ASSERT( IsAPrimaryGoalOfTacticalProject( p ) );
	//
	POSITION pos = m_TacticalProjectsIAmAPrimaryMemberOf.GetHeadPosition();
	while( pos )
	{
		POSITION PrevPos = pos;
		CAITacticalProject* pTacProjectInList = m_TacticalProjectsIAmAPrimaryMemberOf.GetNext( PrevPos );
		if( pTacProjectInList == p )
		{
			m_TacticalProjectsIAmAPrimaryMemberOf.RemoveAt( PrevPos );
			break;
		}
	}
}
*/

bool CEOSAIDesire2::IsAPrimaryGoalOfTacticalProject2( CEOSAITacticalProject2* p )
{
	POSITION pos = m_TacticalProjectsIAmAPrimaryMemberOf.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacProject = m_TacticalProjects2IAmAPrimaryMemberOf.GetNext( pos );
		if( pTacProject == p ) return true;
	}
	return false;
}

bool CEOSAIDesire2::IsAPrimaryGoalOfOneOrMoreTacticalProjects2()
{
	return ( m_TacticalProjects2IAmAPrimaryMemberOf.IsEmpty() == FALSE );
}

void CEOSAIDesire2::AddAsPrimaryGoalOfTacticalProject2( CEOSAITacticalProject2* p )
{
	ASSERT( IsAPrimaryGoalOfTacticalProject2( p ) == false );
	m_TacticalProjects2IAmAPrimaryMemberOf.AddTail( p );
}

//
// Time-Value functions
//

bool  CEOSAIDesire2::CurrentForeignRelationsPreventsPersuingThisDesire()
{
	if( m_iAggressiveTowardsPlayer == 0 ) return false;
	//EOSAIEnumForeignRelations eRel = m_pAIBrain->GetWorldDescPlayer()->GetForeignRelations( m_iAggressiveTowardsPlayer );
	//if( eRel.IsEnemy() ) return false;

	//CWorldDescServer* pWorldDescServer = GetAIBrain()->GetWorldDescServer();
	//CWorldDescPlayerProxy* pWorldDescPlayerProxy = GetAIBrain()->GetWorldDescPlayerProxy();
	if( g_pEOSAICommonData->GetForeignRelations( m_pAIBrain->GetAIPlayerNumber(), m_iAggressiveTowardsPlayer ).IsEnemy() ) return false;
	//if( pWorldDescPlayerProxy->GetSneakAttack( m_iAggressiveTowardsPlayer ) ) return false;
	if( g_pEOSAICommonData->HasSetSneakAttack( m_pAIBrain->GetAIPlayerNumber(), m_iAggressiveTowardsPlayer ) ) return false;

	return true;
}

//
// Time-Value functions
//
void  CEOSAIDesire2::SetTimeValueFunction_OnlyAvailableNow()
{
	m_TimeValueFunction.Clear();
	m_TimeValueFunction.SetInputOutput( 0.0f, 1.0f );
	m_TimeValueFunction.SetInputOutput( 1.0f, 0.9f );
	m_TimeValueFunction.SetInputOutput( 2.0f, 0.5f );
	m_TimeValueFunction.SetInputOutput( 3.0f, 0.2f );
	m_TimeValueFunction.SetInputOutput( 4.0f, 0.0f );
}

void  CEOSAIDesire2::SetTimeValueFunction_PlanForNext10Turns()
{
	m_TimeValueFunction.Clear();
	m_TimeValueFunction.SetInputOutput( 0.0f, 1.0f );
	m_TimeValueFunction.SetInputOutput( 4.0f, 0.9f );
	m_TimeValueFunction.SetInputOutput( 6.0f, 0.8f );
	m_TimeValueFunction.SetInputOutput( 8.0f, 0.5f );
	m_TimeValueFunction.SetInputOutput(10.0f, 0.0f );
}

void  CEOSAIDesire2::SetTimeValueFunction_PlanLongterm()
{
	m_TimeValueFunction.Clear();
	m_TimeValueFunction.SetInputOutput(  0.0f, 1.0f );
	m_TimeValueFunction.SetInputOutput( 50.0f, 0.5f );
}

bool  CEOSAIDesire2::ActionInvolvesCapturingAndOccupyingGroundTerritory()
{
	return m_JobsToDo.m_fGroundCitResHunter > 0.0f;
}

// Score
void CEOSAIDesire2::CalculateScore( EnumScoringLevel eScoringLevel )
{
	if( eScoringLevel == EnumInitialScoring )
	{
		m_eScoringLevel = eScoringLevel;
		//m_fScore = GetAccessibility01() * GetAreaSimpleInterest();
		m_fScore = GetAccessibility01() * (0.7f*GetSimpleInterest() + 0.3f*GetAreaSimpleInterest());
		int g=0;
		/*
		if( fValue > fBestValue )
		{
			fBestValue = fValue;
			pBestDesire = pDesire;
		}
		*/
	}
	else
	{
		ASSERT( false );
	}
}

/*
//float CEOSAIDesire2::GetBestMovementRateSimpleTaskForce()
float CEOSAIDesire2::GetBestMovementRateForUnitsCompletingAllTheJobs()
{
	return m_Jobs.GetBestMovementRateForUnitsCompletingAllTheJobs( 
		GetAIThoughtDatabase(), GetAIThoughtDatabase()->GetUnitsICanBuildOrHave() );
}
*/

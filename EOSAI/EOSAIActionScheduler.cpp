
#include <stdafx.h>
#include "EOSAIThoughtDatabase.h"
#include "EOSAIActionScheduler.h"
//#include "AITacticalProject.h"
#include "EOSAITacticalProject2.h"
//#include "AITacticalProject_CaptureArea.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireDestroyUnit.h"
#include "EOSAITaskForce3.h"
#include "EOSAIUnitAction.h"
#include "City.h"
//#include "Unit.h"
#include "EOSAIDesireConnection.h"
//#include "AITacticalProjectStrategy.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAIStopwatch2.h"
#include "AIPlayer.h"
//#include "WorldDescServer.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAIWarzoneLocation.h"
#include "EOSAIAirbasesSet.h"
#include "EOSAIUnitVsUnitValue.h"
#include "EOSAITacticalProject2Strategy.h"
//#include "TWDx.h"
#include "EOSAICommonData.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIStringAndLongSet.h"
//#include "..\..\BCToolkit\LogFile.h"
#include "EOSAILogFile.h"
#include "EOSAIPublic.h"
extern CList< long >  g_HighlightAIRegions;

#ifdef _DEBUG
#include "EOSAIInterface.h"
#endif

/*
#ifdef _DEBUG
bool bUseNormalTacticalProjectSystem = true; // Can turn this off to calculate IdleUnit tasks
#else _DEBUG
bool bUseNormalTacticalProjectSystem = true; // Always true
#endif _DEBUG
*/
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace EOSAI;

bool SortTacticalProjectsBySimpleInterest( CAITacticalProject* pTacticalProject1, CAITacticalProject* pTacticalProject2 );
bool SortTacticalProjectsByInitialEvaluationScore( CAITacticalProject* pTacticalProject1, CAITacticalProject* pTacticalProject2 );
bool SortTacticalProjectsByInitialEvaluationScore( CEOSAITacticalProject2* pTacticalProject1, CEOSAITacticalProject2* pTacticalProject2 );
bool SortByExpectedResistance( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	if( p1->GetResistanceCombatSignificance() > p2->GetResistanceCombatSignificance() ){ return true; }
	else if( p1->GetResistanceCombatSignificance() < p2->GetResistanceCombatSignificance() ){ return false; }
	return true;  // always return true when values are equal
}

bool SortByOrderOfProcessing( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	if( p1->GetOrderOfProcessing() > p2->GetOrderOfProcessing() ){ return true; }
	else if( p1->GetOrderOfProcessing() < p2->GetOrderOfProcessing() ){ return false; }
	return true;  // always return true when values are equal
}

/*
bool SortTacticalProjectsBySimpleInterest( CAITacticalProject* pTacticalProject1, CAITacticalProject* pTacticalProject2 )
{
	if( pTacticalProject1->GetSimpleInterest() > pTacticalProject2->GetSimpleInterest() ){ return true; }
	else if( pTacticalProject1->GetSimpleInterest() < pTacticalProject2->GetSimpleInterest() ){ return false; }
	return true;  // always return true when values are equal
}
*/
/*
bool SortTacticalProjectsByInitialEvaluationScore( CAITacticalProject* pTacticalProject1, CAITacticalProject* pTacticalProject2 )
{
	if( pTacticalProject1->GetInitialEvaluationScore() > pTacticalProject2->GetInitialEvaluationScore() ){ return true; }
	else if( pTacticalProject1->GetInitialEvaluationScore() < pTacticalProject2->GetInitialEvaluationScore() ){ return false; }
	return true;  // always return true when values are equal
}
*/
CActionScheduler::CActionScheduler()
{
	m_pAIBrain = NULL;
	m_fAttritionValue = 1.0f; // 0.5 = don't worry too much about my attrition, 1.5 worry about it.
	m_fTimeValue = 1.0f;      // 0.5 = don't worry about doing things quickly
	//
	//m_iNumberOfMyUnitsAndCities = 0;
	//m_iUnitsAndCitiesAllocatedToTacticalProjects = 0;
	//m_iNumberOfAllocatedTacticalProjects = 0;
	//m_fInsufficientTacticalProjectsUnderCurrentForeignRelations01 = 0.0f;
}

CEOSAIThoughtDatabase*  CActionScheduler::GetAIThoughtDatabase()
{
	ASSERT( m_pAIBrain );
	return m_pAIBrain->GetAIThoughtDatabase();
}

CEOSAIListSort< CEOSAIDesireSpatial* >*  CActionScheduler::GetDesiresSortedByScore()
{
	return m_pAIBrain->GetAIThoughtDatabase()->GetDesiresSortedByScore();
}

void CActionScheduler::CreateTacticalProjects2()
{
	long iNextId = 1;
	CEOSAIStopwatch2 watch1, watch2;

	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	//Checkpoint::Write( _T("CActionScheduler::CreateTacticalProjects2() - Begin") );

	//CEOSAIListSort< CAITacticalProject* >  SortedTacticalProjects;
	//SortedTacticalProjects.SetSortFunction( SortTacticalProjectsByInitialEvaluationScore );
	watch1.Start();
	long iCount = 0;
	CEOSAIThoughtDatabase* pAIThoughtDatabase = m_pAIBrain->GetAIThoughtDatabase();
	int iNumberOfDesires = GetDesiresSortedByScore()->GetCount();
	POSITION pos = GetDesiresSortedByScore()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = GetDesiresSortedByScore()->GetNext( pos );
		iCount++;
		if( pDesire->GetAIPoiObjectTarget() )
		{
			long iTargetObjectId = pDesire->GetTargetObjectId();
			if( pDesire->GetTargetObjectId() == 118 ||
				pDesire->GetTargetObjectId() == 104 )
			{
				int h=0;
			}
			if( pDesire->GetTargetObjectId() == 12 )
			{
				int h=0;
			}
		}

		//if( pDesire->CurrentForeignRelationsPreventsPersuingThisDesire() ) continue;
		if( pDesire->IsAPrimaryGoalOfOneOrMoreTacticalProjects2() ) continue;

		// If the desire is already allocated, skip it
		//if( pDesire->GetTacticalProject() ) continue;

		// If I can't get there in any reasonable amount of time (according to TimeValue), skip it
		float fTimeToTarget = pDesire->GetNationwideEstimatedTimeToTarget();
		if( pDesire->GetTimeValueFunction( fTimeToTarget ) == 0.0f ) continue;

		// Group Desires into TacticalProjects
		//   (This is based on the type of Desire and the proximity of Desires to each other)

		// Submarine warfare?
		// 
		// "Destroy units" project?  (Destroy incoming fleet/army)
		if( pDesire->GetType() == EnumDestroyUnit &&
			pDesire->IsAggressiveAgainstPlayer() > 0 ) // Aggression against militia isn't valuable
		{
			CEOSAITacticalProject2* pTacticalProject2 = new CEOSAITacticalProject2( m_pAIBrain );
			pTacticalProject2->SetId( iNextId ); iNextId++;
			pTacticalProject2->ActionScheduler_SetPrimaryAction( ::CEOSAITacticalProject2::enum_DestroyUnits );
			pTacticalProject2->ActionScheduler_AddPrimaryGoal( pDesire );
			pTacticalProject2->CalculateInitialEvaluation(); // PathwayResistance, Distance, SimpleInterest
			float f = pTacticalProject2->GetDistanceFromMyCitResToPrimaryGoals();
			//
			pAIThoughtDatabase->AddTacticalProject2( pTacticalProject2 );
		}
		// "Degrade cities" project?  (Bomb cities with aircraft/missiles, bombard with ships/artillery)
		// This is a one-desire project
		if( pDesire->GetType() == EnumDegradeCity &&
			pDesire->IsAggressiveAgainstPlayer() > 0 )
		{
			CEOSAITacticalProject2* pTacticalProject2 = new CEOSAITacticalProject2( m_pAIBrain );
			pTacticalProject2->SetId( iNextId ); iNextId++;
			pTacticalProject2->ActionScheduler_SetPrimaryAction( CEOSAITacticalProject2::enum_DegradeCity );
			pTacticalProject2->ActionScheduler_AddPrimaryGoal( pDesire );
			pTacticalProject2->CalculateInitialEvaluation(); // PathwayResistance, Distance, SimpleInterest
			float f = pTacticalProject2->GetDistanceFromMyCitResToPrimaryGoals();
			//
			pAIThoughtDatabase->AddTacticalProject2( pTacticalProject2 );
		}
		// Create a "capture" project?
		// Add nearby capture desires
		if( pDesire->GetType() == EnumCaptureCity ||
			pDesire->GetType() == EnumCaptureResource ||
			pDesire->GetType() == EnumCaptureAirfield )
		{
			CEOSAITacticalProject2* pTacticalProject2 = new CEOSAITacticalProject2( m_pAIBrain );
			pTacticalProject2->SetId( iNextId ); iNextId++;
			//if( pTacticalProject2->GetId() == 3 )
			//{
			//	int g=0;
			//}
			pTacticalProject2->ActionScheduler_SetPrimaryAction( CEOSAITacticalProject2::enum_CaptureArea );
			pTacticalProject2->ActionScheduler_AddPrimaryGoal( pDesire );
			pTacticalProject2->ActionScheduler_AddNearbyRelatedDesires( pDesire );
			pTacticalProject2->CalculateInitialEvaluation(); // PathwayResistance, Distance, SimpleInterest
			float f = pTacticalProject2->GetDistanceFromMyCitResToPrimaryGoals();

			pAIThoughtDatabase->AddTacticalProject2( pTacticalProject2 );
		}
	}
	// DEBUG
	pos = pAIThoughtDatabase->GetTacticalProjects2()->GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = pAIThoughtDatabase->GetTacticalProjects2()->GetNext( pos );
		int y=0;
	}
	watch1.Stop();

	// Create some submarine TacticalProjects
	/*
	watch2.Start();
	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();
	pos = pMultiRegionManager->GetMultiRegionList()->GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pMultiRegion = pMultiRegionManager->GetMultiRegionList()->GetNext( pos );
		float fDanger01 = pMultiRegion->GetDanger01( iAIPlayer );
		if( pMultiRegion->IsWater() && pMultiRegion->GetSize() > 20 &&
			fDanger01 > 0.0f && fDanger01 < 0.5f )
		{
			CEOSAITacticalProject2* pTacticalProject2 = new CEOSAITacticalProject2( m_pAIBrain );
			pTacticalProject2->SetId( iNextId ); iNextId++;
			pTacticalProject2->ActionScheduler_SetPrimaryAction( CEOSAITacticalProject2::enum_SubmarineWarzone );
			pTacticalProject2->AddLocation( pMultiRegion->GetLocation() );
			pTacticalProject2->SetSimpleInterest( 3.0f );
		//	m_fSimpleInterest
		//	pTacticalProject2->SetActionIsHostileAgainstPlayers( );
		//	pTacticalProject2->ActionScheduler_AddPrimaryGoal( pDesire );
		//	pTacticalProject2->ActionScheduler_AddNearbyRelatedDesires( pDesire );
			pTacticalProject2->CalculateInitialEvaluation(); // PathwayResistance, Distance, SimpleInterest
			//float f = pTacticalProject2->GetDistanceFromMyCitResToPrimaryGoals();
			pAIThoughtDatabase->AddTacticalProject2( pTacticalProject2 );
		}
	}
	*/

	// Setup Protect-Area and Submarine-Warfare targets
	//long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CEOSAIMultiRegionNationwidePathways* pNationwidePathways = pWorldDescServer->GetAICommonData()->GetNationwidePathways( iAIPlayer );
	CEOSAIMultiRegionNationwidePathways* pNationwidePathways = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );
	// Look for multiregions that should be blocked or watched, could be good submarine locations

	pAIThoughtDatabase->SetTacticalProjects2ScoreIndex();
	watch2.Stop();

	// Data from a 1000x1000 map, 2 AI players, Turn 125
	//                                                 Player1     Player2
	float fWatch1 = (float) watch1.GetDeltaMilliseconds(); // 1166.4264   769.34412
	float fWatch2 = (float) watch2.GetDeltaMilliseconds(); //  158.66229  161.00729
	int h=0;
}

void CActionScheduler::AllocateTacticalProjects2()
{
	CEOSAIStopwatch2 watch1,  watch2,  watch3,   watch4, watch5,  watch6,  watch7,  watch8,  watch9;
	CEOSAIStopwatch2 watch10, watch11, watch12, watch13, watch14, watch15;

	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	CEOSAIThoughtDatabase* pAIThoughtDatabase = m_pAIBrain->GetAIThoughtDatabase();
	long iIdleUnits1 = this->GetAIThoughtDatabase()->GetNumberOfMyIdleUnits();
	long iIdleCities1 = this->GetAIThoughtDatabase()->GetNumberOfMyIdleCities();
	int h=0;

	int iTacticalProjects = pAIThoughtDatabase->GetTacticalProjects2()->GetCount();
	ASSERT( iTacticalProjects < 30 );
	int iDesires = pAIThoughtDatabase->GetDesires()->GetCount();
	ASSERT( iDesires < 100 );
	//int iCurrentTurn = g_pEOSAIInterface->GetCurrentTurn();

	//
	// Order the TacticalProjects by "OrderOfProcessing"
	//
		watch1.Start();
		CEOSAIListSort< CEOSAITacticalProject2* >  TacticalProjectsToEvaluate;
		TacticalProjectsToEvaluate.SetSortFunction( SortByOrderOfProcessing );
		POSITION pos = pAIThoughtDatabase->GetTacticalProjects2()->GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2* pTacticalProject2 = pAIThoughtDatabase->GetTacticalProjects2()->GetNext( pos );
			if( iAIPlayer == 2 && pTacticalProject2->ProjectIncludesObjectIdAsPrimaryTask( 12 ) )
			{
				int h=0;
			}
			if( iAIPlayer == 2 && pTacticalProject2->ProjectIncludesObjectIdAsPrimaryTask( 7 ) )
			{
				int h=0;
			}
			if( pTacticalProject2->GetShouldBeInAllocateTacticalProjectList( 5 ) )
			{
				pTacticalProject2->IsInsideTacticalProjectsList( true );
				ASSERT( pTacticalProject2->GetOrderOfProcessing() >  0.0f );
				TacticalProjectsToEvaluate.InsertSorted( pTacticalProject2 );
			}
			long iNearbyScoreIndex = pTacticalProject2->GetInitialEvaluation_NearbyScoreIndex();
			long iValueScoreIndex = pTacticalProject2->GetInitialEvaluation_ValueScoreIndex();
			long iBasicScoreIndex = pTacticalProject2->GetInitialEvaluation_BasicScoreIndex();
			long iEasyTargetScoreIndex = pTacticalProject2->GetInitialEvaluation_EasyTargetScoreIndex();
			long iNoResistanceScoreIndex = pTacticalProject2->GetInitialEvaluation_NoResistanceScoreIndex();
			int h=0;
		}
		long iMaximumIndex = 6;
		while( TacticalProjectsToEvaluate.GetCount() < 10 && iMaximumIndex < 9 )
		{
			POSITION pos = pAIThoughtDatabase->GetTacticalProjects2()->GetHeadPosition();
			while( pos )
			{
				CEOSAITacticalProject2* pTacticalProject2 = pAIThoughtDatabase->GetTacticalProjects2()->GetNext( pos );
				if( pTacticalProject2->GetShouldBeInAllocateTacticalProjectList( iMaximumIndex ) &&
					pTacticalProject2->IsInsideTacticalProjectsList() == false )
				{
					pTacticalProject2->IsInsideTacticalProjectsList( true );
					ASSERT( pTacticalProject2->GetOrderOfProcessing() >  0.0f );
					TacticalProjectsToEvaluate.InsertSorted( pTacticalProject2 );
				}
			}
			iMaximumIndex++;
		}

		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		watch1.Stop();

	//
	// Turn TacticalProjects into Strategies and UnitActionIdeas
	//
		if( iAIPlayer == 1 )
		{
			int h=0;
		}

		watch2.Start();
		//m_pAIBrain->m_CreateStrategiesAndUnitActionIdeasTime.Start();
		pos = TacticalProjectsToEvaluate.GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2* pTacticalProject = TacticalProjectsToEvaluate.GetNext( pos );

			float fResistance = pTacticalProject->GetResistanceCombatSignificance();
			if( iAIPlayer == 2 && pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 12 ) )
			{
				int h=0;
			}
			if( iAIPlayer == 2 && pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 7 ) )
			{
				int h=0;
			}
			pTacticalProject->CreateStrategiesAndUnitActionIdeas();
			if( iAIPlayer == 1 && pTacticalProject->GetId() == 137 )
			{
				int h=0;
			}
			if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		}
		//m_pAIBrain->m_CreateStrategiesAndUnitActionIdeasTime.Stop();
		watch2.Stop();

	//
	// Create Best-Case TaskForces (but only 40 of them)
	//
		watch3.Start();
		//m_pAIBrain->m_CreateBestCaseActionIdeasTime.Start();
		long iTacticalProjectsEvaluated = 0;
		pos = TacticalProjectsToEvaluate.GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2* pTacticalProject = TacticalProjectsToEvaluate.GetNext( pos );

			if( iAIPlayer == 2 && pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 12 ) )
			{
				int h=0;
			}
			if( iAIPlayer == 1 && pTacticalProject->GetId() == 137 )
			{
				int h=0;
			}

			if( pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 7 ) )
			{
				int h=0;
			}
			if( pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 9 ) )
			{
				int h=0;
			}
			if( pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 12 ) )
			{
				int h=0;
			}
			if( pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 15 ) )
			{
				int h=0;
			}

			//pTacticalProject2->CreateStrategiesAndUnitActionIdeas();
			pTacticalProject->CreateBestCaseTaskForce();
			if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }

			bool bEnterFunc = false;
			if( bEnterFunc ) // && iAIPlayer == 1 && pTacticalProject2->GetId() == 137 ) // redo
			{
				pTacticalProject->CreateBestCaseTaskForce();
			}

			iTacticalProjectsEvaluated++;
			if( iTacticalProjectsEvaluated >= 40 ) break;
		}
		//m_pAIBrain->m_CreateBestCaseActionIdeasTime.Stop();
		watch3.Stop();

	//
	// Add the BestCase TackForces to a sorted list
	//
		watch4.Start();
		//m_pAIBrain->m_CreateTaskForcesAndPredefinedStepsTime.Start();
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		pos = TacticalProjectsToEvaluate.GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2* pTacticalProject = TacticalProjectsToEvaluate.GetNext( pos );
			/*
			if( iCurrentTurn == 11 )
			{
				if( pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 7 ) ||
					pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 9 ) ||
					pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 12 ) ||
					pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 15 ) )
				{
					int h=0;
				}
			}
			*/

			CList< CEOSAITacticalProject2Strategy* > StrategyList;
			pTacticalProject->GetAITacticalProjectStrategies( &StrategyList );
			POSITION pos2 = StrategyList.GetHeadPosition();
			while( pos2 )
			{
				CEOSAITacticalProject2Strategy* pStrategy = StrategyList.GetNext( pos2 );
				float fBestCaseTaskForceValue = pStrategy->GetBestCaseTaskForceValue();
				if( pStrategy->GetBestCaseTaskForceValue() > 0.0f )
				{
					GetAIThoughtDatabase()->AddToTacticalProjectStrategiesSortedByBestCaseValue( pStrategy );
				}
				else
				{
					int h=0;
				}
			}
		}
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		watch4.Stop();

	// Sort tactical projects into categories.
	//   I want a variety of different projects.
	//   Categories include:
	//     (1) Attacks on other (enemy/non-enemy) players (often involves lots of planning)
	//     (2) Attacks on Player0 CitRes
	//   I may want to add a third category: attacks on no-risk Player0 resources
	long iUnitsAllocatedToAttackPlayerNProjects = 0;
	long iUnitsAllocatedToAttackPlayer0Projects = 0;

	// Read over the (sorted) BestCase TacticalProject Strategies
	//   Change the 'Plan' state of the highest-rated ones (this will cause some interference because
	//   different strategies for the same project will be in the list).
	//long iTotalAllocatedActorsAndCities = 0;
	//long iUnitsAndCitiesAllocatedToTacticalProjects = 0;
	long iUnitsAndCitiesNeededByPersuedTacticalProjects = 0;
	long iUnitsAndCitiesNeededBySkippedTacticalProjects = 0;
	long iNumberOfPursuedTacticalProjects = 0;
	long iNumberOfSkippedTacticalProjects = 0;
	//long iMaxUnitsAndCitiesToSpend = (1.5f*(float)iNumberOfMyUnitsAndCities)+3;
	long iMaxUnitsAndCitiesToSpend = (long) ( 1.5f * GetAIThoughtDatabase()->GetNumberOfMyIdleUnitsAndCities() );
//	m_fInsufficientTacticalProjectsUnderCurrentForeignRelations01 = 0.0f;


	//
	// If a unit needs to be upgraded, isn't inside a unit container, and is within 100 pixels of 
	//    an upgrade location, then give it orders to move there and upgrade
	//
		watch6.Start();
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		AddUpgradePredefinedStepToAllUnitsWhoNeedAnUpgradeAndWithin100Pixels();
		watch6.Stop();

		#ifdef OLD_SYSTEM
	//
	// Iterate over the TacticalProject Strategies (sorted by BestCaseValue)
	//   If foreign relations prevent pursuing a project, mark it as "SkipThisActionBecauseOfForeignRelations"
	//   Othewise, set their plan to "Actively Pursue" until I have them overallocated (based on number of actors)
	//   Then, switch them to "Consider This Action" (which gets used in build-plans)
	//
		long iIndex = 1;
		watch5.Start();
		pos = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2Strategy* pStrategy = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetNext( pos );
			pStrategy->SetBestCaseValueIndex( iIndex ); iIndex++;

			CEOSAITacticalProject2* pTacticalProject = pStrategy->GetTacticalProject2();
			if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }

			float fDebug_StrategyValue = pStrategy->GetBestCaseTaskForceValue();
			bool bAlreadyOverallocatingUnits = false;
			if( pTacticalProject->PrimaryGoalsAreHostileActionAgainstAnotherPlayer() )
			{
				if( iUnitsAllocatedToAttackPlayerNProjects > iMaxUnitsAndCitiesToSpend )
				{
					bAlreadyOverallocatingUnits = true;
				}
			}
			else
			{
				if( iUnitsAllocatedToAttackPlayer0Projects > iMaxUnitsAndCitiesToSpend )
				{
					bAlreadyOverallocatingUnits = true;
				}
			}

			bool bDoThisProject = false;
			if( pTacticalProject->GetPlan() == CEOSAITacticalProject2::enum_Undefined )
			{
				if( pTacticalProject->CurrentForeignRelationsPreventsPersuingThisTacticalProject() )
				{
					// I can't do this tactical project because of current Foreign Relations
					pTacticalProject->SetPlan( CEOSAITacticalProject2::enum_SkipThisActionBecauseOfForeignRelations );
					pStrategy->SetPlan( CEOSAITacticalProject2Strategy::enum_SkipThisActionBecauseOfForeignRelations );

					iUnitsAndCitiesNeededBySkippedTacticalProjects += pStrategy->GetBestCaseTaskForce()->GetNumberOfAllocatedUnitsAndCities();
					iNumberOfSkippedTacticalProjects++;
				}
				else
				{
					if( bAlreadyOverallocatingUnits )
					{
						pTacticalProject->SetPlan( CEOSAITacticalProject2::enum_ConsiderThisAction );
						pStrategy->SetPlan( CEOSAITacticalProject2Strategy::enum_ConsiderThisAction );
					}
					else
					{
						bDoThisProject = true;
						pTacticalProject->SetPlan( CEOSAITacticalProject2::enum_ActivelyPursueThisAction );
						pStrategy->SetPlan( CEOSAITacticalProject2Strategy::enum_ActivelyPursueThisAction );

						long iDEBUG_AllocatedUnitsAndCities = pStrategy->GetBestCaseTaskForce()->GetNumberOfAllocatedUnitsAndCities();
						iUnitsAndCitiesNeededByPersuedTacticalProjects += pStrategy->GetBestCaseTaskForce()->GetNumberOfAllocatedUnitsAndCities();
						iNumberOfPursuedTacticalProjects++;
					}
				}
			}

			if( bDoThisProject )
			{
				if( pTacticalProject->PrimaryGoalsAreHostileActionAgainstAnotherPlayer() )
				{
					iUnitsAllocatedToAttackPlayerNProjects += pStrategy->GetBestCaseTaskForce()->GetNumberOfAllocatedUnitsAndCities();
				}
				else
				{
					iUnitsAllocatedToAttackPlayer0Projects += pStrategy->GetBestCaseTaskForce()->GetNumberOfAllocatedUnitsAndCities();
				}
			}
			/*
			if( iUnitsAndCitiesNeededByPersuedTacticalProjects > iMaxUnitsAndCitiesToSpend &&
				iNumberOfPursuedTacticalProjects >= 3 )
			{
				break; // That's all the units I can allocate
			}
			*/
		}
		watch5.Stop();

	//
	// Are there insufficient TacticalProjects?
	//   Is that because I'm skipping war-based TacticalProjects?
	//
		/* This data might be useful, but I wasn't using it so I commented it out
		if( iUnitsAndCitiesNeededByPersuedTacticalProjects < iMaxUnitsAndCitiesToSpend )
		{
			// Yes, there are insufficient Tactical Projects to persue - maybe I need to think about war
			m_fInsufficientTacticalProjectsUnderCurrentForeignRelations01 =
				1.0f - ( (float)iUnitsAndCitiesNeededByPersuedTacticalProjects / (float)iMaxUnitsAndCitiesToSpend );
		}
		*/

	//
	// Iterate over the TacticalProject Strategies (ordered by BestCase Value),
	//   if the plan is "Actively Pursue", then start setup a negotiated TaskForce
	//
		watch7.Start();
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		long iTacticalProjectStrategiesCalculated = 0;
		pos = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2Strategy* pStrategy = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetNext( pos );

			// If I'm planning to accomplish this TacticalProject, and it hasn't already been allocated
			//   (i.e. there are no strategies allocated to accomplish it), then allocate it.
			if( pStrategy->GetTacticalProject2()->GetPlan() == CEOSAITacticalProject2::enum_ActivelyPursueThisAction &&
				pStrategy->GetTacticalProject2()->NegotiatedTaskForceHasBeenAllocated() == false )
			{
				iTacticalProjectStrategiesCalculated++;

				/*
				I would like to setup a calculation where I can say, "use unit X for project Y" and then check 
				the fulfillment of other projects. (Oops, the unit is required for accomplishing project Z).
				It could also include some alternatives - for example, if I steal-away this transport, can I 
				fulfill the same project by using a destroyer?  If I steal-away this Tank, can I use a 
				different GroundUnit?
				
				Because the whole situation is so dynamic, I probably shouldn't cache anything.
				Also, I need to include multiple transports in the BestCase setup.  (Right now, I allow 
				infinite container size.)

				Can I allocate these units?  Can I find alternatives?  If the alternative is almost the same 
				  score (for example, if I use a different Infantry from the exact same location), then I 
				  don't need to negotiate.
				*/

				// Are the desired units available?  Or are they already allocated by someone?

				// Step 1. Remember the current RealScore (I'll need it for comparison later)
				// Step 2. Try to allocate a TaskForce with the restrictions that:
				//   (a) The TaskForce can only use unallocated units.
				//   (b) The TaskForce must actually plan things out including container sizes (unlike BestCase)
				//   This means it must do a 'BestCase' with available units and more rigor.
				//   Then, get the score.  How does this score compare to the BestCase score?
				//     If the score is worse than the BestCase score, then attempt to negotiate for
				//     better/more units.
				pStrategy->GetNegotiatedTaskForce()->SetUnitNegotiation( CEOSAITaskForce3::AllocateOnlyUnallocatedUnits );
				pStrategy->GetNegotiatedTaskForce()->SetTransportStrategy( CEOSAITaskForce3::GroundUnitArrivalASAP );
				pStrategy->GetNegotiatedTaskForce()->ConstructTaskForce();
				float fRealScore = pStrategy->GetNegotiatedTaskForce()->GetRealScore();

				// If the RealScore is worse than the BestCase scores, then negotiate for better units
				// Step 3. Negotiation for better units
				//   Construct a BestCase TaskForce (include accurate Transport-Container sizes)
				//   Then, iterate over the hypothetically-allocated units
				//   For each unit that is wanted, attempt to 'steal' each unit for the TaskForce.
				//     How does stealing work?
				//     (1) Mark the unit as "unavailable" for my TaskForce, try allocating units until
				//         I reach success again.  Check the score.
				//     (2) Find the TaskForce allocating the unit, mark the unit as "unavailable" for
				//         that task force, try allocating units until it reaches success again.  Check score.
				//     Multiply those scores by the TacticalProject's Value
				//       (more valuable projects get more weight in allocating units)
				//     How do the scores compare?  If the score is better with stealing, then steal it.
				//   If I fail to steal the unit, then mark it as "unavailable" and attempt to add
				//     another unit to the TaskForce.
			//	is there another way?

				//pStrategy->GetNegotiatedTaskForce()->ReleaseStackAllocations();
				//pStrategy->ConstructNegotiatedTaskForce();
				int h=0;
				//
			}
		}
		watch7.Stop();
		#else
		watch7.Start();

		long iLargeProjectsTried = 0;
		long iSmallProjectsTried = 0;
		long iTinyProjectsTried = 0;
		long iConsiderProjectsTried = 0;
		long iTotalUnitsAndCities = pAIThoughtDatabase->GetNumberOfMyCities() + pAIThoughtDatabase->GetNumberOfMyUnits();
		pos = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2Strategy* pStrategy = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetNext( pos );
			CEOSAITacticalProject2* pTacticalProject = pStrategy->GetTacticalProject2();
			if( iAIPlayer == 2 && pTacticalProject->m_Id == 1 )
			{
				int h=0;
			}
			if( iAIPlayer == 1 && pTacticalProject->GetId() == 137 )
			{
				int h=0;
			}
			if( iAIPlayer == 2 && pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 7 ) )
			{
				int h=0;
			}
			if( iAIPlayer == 2 && pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 9 ) )
			{
				int h=0;
			}
			if( iAIPlayer == 2 && pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 12 ) )
			{
				int h=0;
			}
			if( iAIPlayer == 2 && pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 15 ) )
			{
				int h=0;
			}
			if( pTacticalProject->GetId() == 24 ||
				pTacticalProject->GetId() == 26 )
			{
				int h=0;
			}

			// If a different strategy on the same project has been allocated, then don't do this strategy.
			if( pTacticalProject->NegotiatedTaskForceHasBeenAllocated() )
			{
				continue;
			}

			// I can't do this tactical project because of current Foreign Relations
			if( pTacticalProject->CurrentForeignRelationsPreventsPersuingThisTacticalProject() )
			{
				pTacticalProject->SetPlan( CEOSAITacticalProject2::enum_SkipThisActionBecauseOfForeignRelations );
				pStrategy->SetPlan( CEOSAITacticalProject2Strategy::enum_SkipThisActionBecauseOfForeignRelations );
				continue;
			}

			long iIdleUnitsAndCities = pAIThoughtDatabase->GetNumberOfMyIdleUnitsAndCities();
			float fIdleUnits01 = (float)iIdleUnitsAndCities / (float)iTotalUnitsAndCities;

			long iNumberOfActors = pStrategy->GetBestCaseTaskForce()->GetNumberOfAllocatedUnitsAndCities();

			bool bTryAllocatingThisStrategy = false;
			if( iLargeProjectsTried < 15 && fIdleUnits01 > 0.3f )
			{
				// Look for Large Projects
				pTacticalProject->SetPlan( CEOSAITacticalProject2::enum_ActivelyPursueThisAction );
				pStrategy->SetPlan( CEOSAITacticalProject2Strategy::enum_ActivelyPursueThisAction );
				iLargeProjectsTried++;
			}
			eif( iSmallProjectsTried < 10 && iNumberOfActors >= 1 && iNumberOfActors <= 3 )
			{
				// Look for Small Projects
				pTacticalProject->SetPlan( CEOSAITacticalProject2::enum_ActivelyPursueThisAction );
				pStrategy->SetPlan( CEOSAITacticalProject2Strategy::enum_ActivelyPursueThisAction );
				iSmallProjectsTried++;
			}
			eif( iTinyProjectsTried < 10 && iNumberOfActors == 1 )
			{
				// Look for Small Projects
				pTacticalProject->SetPlan( CEOSAITacticalProject2::enum_ActivelyPursueThisAction );
				pStrategy->SetPlan( CEOSAITacticalProject2Strategy::enum_ActivelyPursueThisAction );
				iTinyProjectsTried++;
			}
			eif( iConsiderProjectsTried < 10 )
			{
				pTacticalProject->SetPlan( CEOSAITacticalProject2::enum_ConsiderThisAction );
				pStrategy->SetPlan( CEOSAITacticalProject2Strategy::enum_ConsiderThisAction );
				iConsiderProjectsTried++;
			}

			if( pStrategy->GetTacticalProject2()->GetPlan() == CEOSAITacticalProject2::enum_ActivelyPursueThisAction )
			{
				//if( iAIPlayer == 2 && pTacticalProject->m_Id == 1 )
				if( iAIPlayer == 1 )
				{
					int h=0;
				}
				if( pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 7 ) ||
					pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 9 ) ||
					pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 12 ) ||
					pTacticalProject->ProjectIncludesObjectIdAsPrimaryTask( 15 ) )
				{
					/*
					if( g_pEOSAIInterface->GetCurrentTurn() == 11 )
					{
						int h=0;
					}
					*/
				}
				//pStrategy->GetNegotiatedTaskForce()->SetUnitNegotiation( CEOSAITaskForce3::AllocateOnlyUnallocatedUnits );
				//pStrategy->GetNegotiatedTaskForce()->SetTransportStrategy( CEOSAITaskForce3::GroundUnitArrivalASAP );
				pStrategy->GetNegotiatedTaskForce()->ConstructTaskForce();
				long iDebug_ActorCount = pStrategy->GetNegotiatedTaskForce()->GetNumberOfAllocatedUnitsAndCities();
				float fRealScore = pStrategy->GetNegotiatedTaskForce()->GetRealScore();
				int h=0;
			}
			if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		}

		watch7.Stop();
		#endif
	//
	// If I have tactical bombers around, I should check about adding them to invasion forces
	//
		watch8.Start();
		CList< CEOSAIUnit* >* pAirUnits = m_pAIBrain->GetAIThoughtDatabase()->GetMyAirUnits();
		if( pAirUnits->GetCount() > 0 )
		{
			CEOSAIAirbasesSet  Airbases;
			GetAIThoughtDatabase()->GetAirbases( m_pAIBrain->GetAIPlayerNumber(), &Airbases );

			pos = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetHeadPosition();
			while( pos )
			{
				CEOSAITacticalProject2Strategy* pStrategy = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetNext( pos );
				if( pStrategy->GetTacticalProject2()->NegotiatedTaskForceHasBeenAllocated() )
				{
					if( pStrategy->GetTacticalProject2()->GetResistanceCombatSignificance() > 20.0f &&
						pStrategy->GetTacticalProject2()->GetNationalPathwayTimeToTarget() < 2.0f )
					{
						CList< CEOSAIDesireSpatial* >  EnemyUnits;
						pStrategy->GetTacticalProject2()->GetUnitTargets( &EnemyUnits );

						CList< CEOSAIUnit* >  AirUnitsWithinAttackRange;
						float fEnemyDistanceFromAirbases = 1000000.0f;
						POSITION posX = EnemyUnits.GetHeadPosition();
						while( posX )
						{
							CEOSAIDesireSpatial* pEnemyUnitDesire = EnemyUnits.GetNext( posX );
							float fDistance = Airbases.GetClosestAirbaseDistance( pEnemyUnitDesire->GetLocation() );
							fEnemyDistanceFromAirbases = min( fEnemyDistanceFromAirbases, fDistance );
						}
						POSITION posY = pAirUnits->GetHeadPosition();
						while( posY )
						{
							CEOSAIUnit* pAIAirUnit = pAirUnits->GetNext( posY );
							// Don't use damaged aircraft
							if( pAIAirUnit->GetNeedForRepair01() > 0.0f ) continue;
							float fAttackRange = pAIAirUnit->GetAIUnitTemplate()->GetMovementRangeForAttack();
							if( fAttackRange > fEnemyDistanceFromAirbases )
							{
								AirUnitsWithinAttackRange.AddTail( pAIAirUnit );
							}
						}
						// Find the closest enemy
						//CWorldBuildDesc* pWorldBuildDesc = m_pAIBrain->GetWorldDescServer()->GetWorldBuildDesc();
						//CWorldBuildDesc* pWorldBuildDesc = g_pEOSAICommonData->GetWorldBuildDesc()->GetWorldBuildDesc();
						//CAIWorldDistanceTool* pAIWorldDistanceTool = g_pEOSAICommonData->GetWorldDistanceTool();
						posX = AirUnitsWithinAttackRange.GetHeadPosition();
						while( posX )
						{
							CEOSAIUnit* pAIAirUnit = AirUnitsWithinAttackRange.GetNext( posX );
							CEOSAIUnitPathwayFinder* pPathway = pAIAirUnit->GetAIUnitPathwayFinder();

							if( pPathway->GetPreDefinedPath()->IsEmpty() == FALSE ) continue;

							POSITION posY = EnemyUnits.GetHeadPosition();
							while( posY )
							{
								CEOSAIDesireSpatial* pEnemyUnitDesire = EnemyUnits.GetNext( posY );
								float fDistance = g_pWorldDistanceTool->GetDistance( 
									pAIAirUnit->GetInitialState()->GetLocation(), pEnemyUnitDesire->GetLocation() );

								//if( pAIAirUnit->GetUnitTemplate()->GetMovementRangeForAttack() > fDistance )
								if( pAIAirUnit->GetMovementRangeForAttack() > fDistance )
								{
									CEOSAIPoiObject* pAIEnemyPoiObject = pEnemyUnitDesire->GetAIPoiObjectTarget();
									if( pAIEnemyPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
									{
										CEOSAIUnit* pAIEnemyUnit = (CEOSAIUnit*) pAIEnemyPoiObject;
										CEOSAIUnitVsUnitValue* pUUValue = pAIAirUnit->GetAIUnitCombatCapability()->InvokeCombatValue( pAIEnemyUnit->GetAIUnitTemplate() );
										if( pUUValue->IsUsefulInCombat() )
										{
											pAIAirUnit->AddPotentialTarget( pAIEnemyUnit );
										}
										/*
										CUnit* pEnemyUnit = (CUnit*) pAIEnemyPoiObject->GetServerPoiObject();
										CEOSAIUnitVsUnitValue* pUUValue = pAIAirUnit->GetUnitTemplate()->GetAIUnitCombatCapability()->InvokeCombatValue( pEnemyUnit->GetUnitTemplate() );
										if( pUUValue->IsUsefulInCombat() )
										{
											pAIAirUnit->AddPotentialTarget( pAIEnemyUnit );
										}
										*/
									}
								}
							}
						}
					}
				}
			}
		}
		watch8.Stop();

	//
	// If any of my units don't have orders, try to find a TactialProject for them to accomplish
	//
		watch9.Start();
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		//if( bUseNormalTacticalProjectSystem )
		{
			// try some low-cost tactical projects
			pos = pAIThoughtDatabase->GetTacticalProject2StrategiesByBestCaseValue()->GetHeadPosition();
			while( pos )
			{
				CEOSAITacticalProject2Strategy* pStrategy = pAIThoughtDatabase->GetTacticalProject2StrategiesByBestCaseValue()->GetNext( pos );
				CEOSAITacticalProject2* pTacticalProject2 = pStrategy->GetTacticalProject2();
				if( pTacticalProject2->m_Id == 1 )
				{
					int h=0;
				}
				if( pStrategy->GetTacticalProject2()->NegotiatedTaskForceHasBeenAllocated() )
				{
					continue;
				}
				if( iAIPlayer == 2 && pStrategy->GetTacticalProject2()->m_Id == 39 )
				{
					int h=0;
				}

				// If I'm planning to accomplish this TacticalProject, and it hasn't already been allocated
				//   (i.e. there are no strategies allocated to accomplish it), then allocate it.
				if( pStrategy->GetTacticalProject2()->GetPlan() == CEOSAITacticalProject2::enum_ConsiderThisAction &&
					pStrategy->GetTacticalProject2()->NegotiatedTaskForceHasBeenAllocated() == false &&
					pStrategy->GetBestCaseTaskForce()->GetNumberOfAllocatedUnitsAndCities() <= 2 &&
					pStrategy->GetTacticalProject2()->GetResistanceCombatSignificance() < 40.0f )
				{
					/*
					I would like to setup a calculation where I can say, "use unit X for project Y" and then check 
					the fulfillment of other projects. (Oops, the unit is required for accomplishing project Z).
					It could also include some alternatives - for example, if I steal-away this transport, can I 
					fulfill the same project by using a destroyer?  If I steal-away this Tank, can I use a 
					different GroundUnit?
					
					Because the whole situation is so dynamic, I probably shouldn't cache anything.
					Also, I need to include multiple transports in the BestCase setup.  (Right now, I allow 
					infinite container size.)

					Can I allocate these units?  Can I find alternatives?  If the alternative is almost the same 
					  score (for example, if I use a different Infantry from the exact same location), then I 
					  don't need to negotiate.
					*/

					// Are the desired units available?  Or are they already allocated by someone?

					// Step 1. Remember the current RealScore (I'll need it for comparison later)
					// Step 2. Try to allocate a TaskForce with the restrictions that:
					//   (a) The TaskForce can only use unallocated units.
					//   (b) The TaskForce must actually plan things out including container sizes (unlike BestCase)
					//   This means it must do a 'BestCase' with available units and more rigor.
					//   Then, get the score.  How does this score compare to the BestCase score?
					//     If the score is worse than the BestCase score, then attempt to negotiate for
					//     better/more units.
					//pStrategy->GetNegotiatedTaskForce()->SetUnitNegotiation( CEOSAITaskForce3::AllocateOnlyUnallocatedUnits );
					//pStrategy->GetNegotiatedTaskForce()->SetTransportStrategy( CEOSAITaskForce3::GroundUnitArrivalASAP );
					pStrategy->GetNegotiatedTaskForce()->ConstructTaskForce();
					float fRealScore = pStrategy->GetNegotiatedTaskForce()->GetRealScore();

					// If the RealScore is worse than the BestCase scores, then negotiate for better units
					// Step 3. Negotiation for better units
					//   Construct a BestCase TaskForce (include accurate Transport-Container sizes)
					//   Then, iterate over the hypothetically-allocated units
					//   For each unit that is wanted, attempt to 'steal' each unit for the TaskForce.
					//     How does stealing work?
					//     (1) Mark the unit as "unavailable" for my TaskForce, try allocating units until
					//         I reach success again.  Check the score.
					//     (2) Find the TaskForce allocating the unit, mark the unit as "unavailable" for
					//         that task force, try allocating units until it reaches success again.  Check score.
					//     Multiply those scores by the TacticalProject's Value
					//       (more valuable projects get more weight in allocating units)
					//     How do the scores compare?  If the score is better with stealing, then steal it.
					//   If I fail to steal the unit, then mark it as "unavailable" and attempt to add
					//     another unit to the TaskForce.
				//	is there another way?

					//pStrategy->GetNegotiatedTaskForce()->ReleaseStackAllocations();
					//pStrategy->ConstructNegotiatedTaskForce();
					int h=0;
					//
				}
			}
			// Direct attacks (ground and sea)
			pos = pAIThoughtDatabase->GetTacticalProject2StrategiesByBestCaseValue()->GetHeadPosition();
			while( pos )
			{
				CEOSAITacticalProject2Strategy* pStrategy = pAIThoughtDatabase->GetTacticalProject2StrategiesByBestCaseValue()->GetNext( pos );

				// If I'm planning to accomplish this TacticalProject, and it hasn't already been allocated
				//   (i.e. there are no strategies allocated to accomplish it), then allocate it.
				if( pStrategy->GetTacticalProject2()->GetPlan() == CEOSAITacticalProject2::enum_ConsiderThisAction &&
					pStrategy->GetTacticalProject2()->NegotiatedTaskForceHasBeenAllocated() == false &&
					pStrategy->GetTacticalProject2()->RequiresATransport() == false &&
					pStrategy->GetBestCaseTaskForce()->GetNumberOfAllocatedUnitsAndCities() <= 2 )
				{
					// Now try to use-up any units that don't have orders
					//pStrategy->GetNegotiatedTaskForce()->SetUnitNegotiation( CEOSAITaskForce3::AllocateOnlyUnallocatedUnits );
					//pStrategy->GetNegotiatedTaskForce()->SetTransportStrategy( CEOSAITaskForce3::GroundUnitArrivalASAP );
					pStrategy->GetNegotiatedTaskForce()->ConstructTaskForce();
					if( pStrategy->NegotiatedTaskForceHasBeenAllocated() )
					{
						int g=0;
					}
				}
			}
		}
		watch9.Stop();

	/*
		pos = GetAIThoughtDatabase()->GetSortedBestCaseTacticalProjectStrategies()->GetHeadPosition();
		while( pos )
		{
			CAITacticalProjectStrategy* pStrategy = GetAIThoughtDatabase()->GetSortedBestCaseTacticalProjectStrategies()->GetNext( pos );
			if( pStrategy->GetTacticalProject()->IsAllocated() == false )
			{
				// INCOMPLETE:
				// How do I figure out the best strategy?  I'm using 'best case' stuff, but
				//   if I have one transport aircraft, then all invasions are going to involve
				//   the transport plane.  Instead, I need to be able to switch to a different strategy.
				pStrategy->ConstructNegotiatedTaskForce();
			}
		}
	*/

	//
	// Turn Negotiated TaskForces into Predefined Steps
	//
		watch10.Start();
		pos = pAIThoughtDatabase->GetTacticalProject2StrategiesByBestCaseValue()->GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2Strategy* pStrategy = pAIThoughtDatabase->GetTacticalProject2StrategiesByBestCaseValue()->GetNext( pos );
			if( pStrategy->NegotiatedTaskForceHasBeenAllocated() )
			{
				if( pStrategy->GetTacticalProject2()->ProjectIncludesObjectIdAsPrimaryTask( 12 ) )
				{
					int h=0;
				}
				if( pStrategy->GetTacticalProject2()->ProjectIncludesObjectIdAsPrimaryTask( 89 ) )
				{
					int h=0;
				}
				pStrategy->CreatePredefinedSteps();
			}
		}
		pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				pAIUnit->CalculateAIRegionPathFromPredefinedSteps();
			}
		}
		pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				pAIUnit->UpdateRedPath();
			}
		}
		pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				pAIUnit->ConstructFullRedPath();
			}
		}
		// Now that I have the red path setup, adjust the timing of the units.
		pos = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2Strategy* pStrategy = GetAIThoughtDatabase()->GetTacticalProject2StrategiesByBestCaseValue()->GetNext( pos );
			if( pStrategy->NegotiatedTaskForceHasBeenAllocated() )
			{
				if( pStrategy->GetTacticalProject2()->IsASecretInvasion() )
				//if( true ||
				//	pStrategy->GetTacticalProject2()->GetTaskForceStrategy() == 
				//		CEOSAITacticalProject2::EnumTaskForceStrategy_SecretInvasion )
				{
					pStrategy->GetNegotiatedTaskForce()->SynchronizeInvasionUsingRedPath();
				}
			}
		}

		/*
		// If this is a secret invasion, and there are transports, calculate the arrival
		if( GetTacticalProject2()->IsASecretInvasion() )
		{
			lkklnnlk
			m_fShoreArrivalTime
		}
		*/

		watch10.Stop();

	//
	// Iterate over AirUnits, if the unit has no orders, then look through PotentialTargets to attack
	//
		watch11.Start();
		//CList< CEOSAIUnit* >* pAirUnits = m_pAIBrain->GetAIThoughtDatabase()->GetMyAirUnits();
		pos = pAirUnits->GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit* pAIAirUnit = pAirUnits->GetNext( pos );
			CEOSAIUnitPathwayFinder* pPathway = pAIAirUnit->GetAIUnitPathwayFinder();
			if( pAIAirUnit->HasPotentialTargets() &&
				pPathway->GetPreDefinedPath()->IsEmpty() )
			{
				ASSERT( pPathway->GetPreDefinedPath()->IsEmpty() );

				CEOSAIUnit* pAIEnemyUnit = pAIAirUnit->GetRandomPotentialTarget();
				ASSERT( pAIEnemyUnit );
				CEOSAIUnitPathwayPredefinedStep* pStep = pPathway->CreateAttackUnit( pAIEnemyUnit );
				pPathway->AppendStep( pStep );
				ASSERT( pPathway->GetPreDefinedPath()->GetCount() == 1 );
			}
			pAIAirUnit->ClearPotentialTargets();
		}
		watch11.Stop();

	//
	// Add some "on the way" tasks
	//
		watch12.Start();
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		pos = GetAIThoughtDatabase()->GetMyActors()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = GetAIThoughtDatabase()->GetMyActors()->GetNext( pos );
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit && pAIUnit->IsAirUnit() == false )
			{
				// Situations:
				//   The unit is not being used for anything (look for nearby desires)
				//   The unit is on the way somewhere, pickup some easy desires
				//   The unit is waiting for a transport, pickup some easy desires
				pAIPoiObject->AddOnTheWayAndWaitTasksAsPredefinedSteps();
			}
		}
		watch12.Stop();
		//m_pAIBrain->m_CreateTaskForcesAndPredefinedStepsTime.Stop();

	//
	// Repair and Warzone tasks for idle units
	//
		watch13.Start();
		CreateRepairNearbyTaskOrWarzoneForIdleUnits();
		watch13.Start();

	//GetAIThoughtDatabase()->CalculateTasksIdleUnits();//CalculatePreferedWarzoneForIdleUnits();
	//GetAIThoughtDatabase()->CreatePredefinedStepsToSendIdleUnitsToWarzones();

	//CEOSAIPoiObject* pAIPoiObject117 = m_pAIBrain->GetAIPoiObject( 117 );
	//CEOSAIUnit* pAIUnit117 = dynamic_cast< CEOSAIUnit* >( pAIPoiObject117 );
	//int h=0;
	/*
	pos = GetAIThoughtDatabase()->GetMyActors()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = GetAIThoughtDatabase()->GetMyActors()->GetNext( pos );
		pAIPoiObject->TurnAllocatedUnitActionIdeaIntoPredefinedStep();
	}
	*/
	//
	// Turn PredefinedTasks into ResultPaths
	//   Setup non-transport units first (*ground units using a transport will cause the transport to make a path)
	//   Then, setup transport paths
	//   I don't quite remember why I do it in this order
	//
		watch14.Start();
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		pos = GetAIThoughtDatabase()->GetMyActors()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = GetAIThoughtDatabase()->GetMyActors()->GetNext( pos );
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit && pAIUnit->IsTransport() == false )
			{
				// Units using Transports will force the transport-assisted path to be calculated
				pAIPoiObject->TurnPredefinedPathIntoResultPath();
			}
		}
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		pos = GetAIThoughtDatabase()->GetMyActors()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = GetAIThoughtDatabase()->GetMyActors()->GetNext( pos );
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit && pAIUnit->IsTransport() == true )
			{
				pAIPoiObject->TurnPredefinedPathIntoResultPath();
			}
		}
		if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		watch14.Stop();


	//#ifdef _DEBUG
	//g_HighlightAIRegions.RemoveAll();
	//#endif _DEBUG

	//
	// Turn ResultPath into Orders
	//
		watch15.Start();
		pos = GetAIThoughtDatabase()->GetMyActors()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = GetAIThoughtDatabase()->GetMyActors()->GetNext( pos );
			pAIPoiObject->TurnResultPathIntoOrders();
			if( m_pAIBrain->GetAIPlayer()->GetFlag_ShutdownAIPlayer() ){ return; }
		}
		watch15.Stop();

	//
	// Adjust timing of the orders
	//
		pos = GetAIThoughtDatabase()->GetTacticalProjects2()->GetHeadPosition();
		while( pos )
		{
			CEOSAITacticalProject2* pTacticalProject2 = GetAIThoughtDatabase()->GetTacticalProjects2()->GetNext( pos );
			if( pTacticalProject2->IsASecretInvasion() &&
				pTacticalProject2->GetResultSteps()->IsEmpty() == FALSE )
			{
				pTacticalProject2->AdjustWaitTimeForSimultaneousAttack();
			}
		}


//	#ifdef _DEBUG
//	CString strMapName = g_pCommonState->GetWorldBuildDesc()->GetScenarioName();
//	if( strMapName == _T("TacticalBomberTest") )
//	{
//		pos = GetAIThoughtDatabase()->GetMyActors()->GetHeadPosition();
//		while( pos )
//		{
//			CEOSAIPoiObject* pAIPoiObject = GetAIThoughtDatabase()->GetMyActors()->GetNext( pos );
//			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
//			if( pAIUnit &&
//				pAIUnit->GetServerPoiMobile()->HasOrdersToAttack() == false )
//			{
//				ASSERT( false );
//			}
//		}
//	}
//	#endif _DEBUG

	//Stopwatch.Stop();
	// Data from a 1000x1000 map, 2 AI players, Turn 125
	long  iAIPlayerX = m_pAIBrain->GetAIPlayerNumber();
	                                                 //    Player1        Player2
	float fWatch1 = (float) watch1.GetDeltaMilliseconds();   //    0.13540000     0.10190000
	float fWatch2 = (float) watch2.GetDeltaMilliseconds();   // 1679.5743      2084.4998
	float fWatch3 = (float) watch3.GetDeltaMilliseconds();   // 3051.6831      2164.8159
	float fWatch4 = (float) watch4.GetDeltaMilliseconds();   //    0.11670000     0.10220000
	float fWatch5 = (float) watch5.GetDeltaMilliseconds();   //    0.52179998     0.62739998
	float fWatch6 = (float) watch6.GetDeltaMilliseconds();   //   18.667400      46.439899
	float fWatch7 = (float) watch7.GetDeltaMilliseconds();   //  301.17651      290.69049
	float fWatch8 = (float) watch8.GetDeltaMilliseconds();   //    1.9513000      1.7803000
	float fWatch9 = (float) watch9.GetDeltaMilliseconds();   //    0.075400002    0.014500000
	float fWatch10 = (float) watch10.GetDeltaMilliseconds(); //    0.12200000     0.0068999999
	float fWatch11 = (float) watch11.GetDeltaMilliseconds(); //    0.0019000000   0.012800000
	float fWatch12 = (float) watch12.GetDeltaMilliseconds(); //   12.353500      12.448200
	float fWatch13 = (float) watch13.GetDeltaMilliseconds(); //    0.00000000     0.00000000
	float fWatch14 = (float) watch14.GetDeltaMilliseconds(); //  162.18030      166.06039
	float fWatch15 = (float) watch15.GetDeltaMilliseconds(); //  325.54160      204.99361

	//long iTacticalProjectStrategiesCalculated2 = iTacticalProjectStrategiesCalculated;
	long iIdleUnits = iIdleUnits1;
	long iIdleCities = iIdleCities1;
	long iIdleUnits2 = GetAIThoughtDatabase()->GetNumberOfMyIdleUnits();
	long iIdleCities2 = GetAIThoughtDatabase()->GetNumberOfMyIdleCities();
	CList< CEOSAIPoiObject* >  IdleUnitsAndCities;
	GetAIThoughtDatabase()->GetMyIdleUnitsAndCities( &IdleUnitsAndCities );
	CString strIdlePoi;
	EOSAI::StringAndLongSet  IdleUnitsAndCitiesSet;
	pos = IdleUnitsAndCities.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = IdleUnitsAndCities.GetNext( pos );
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			CString str = pAIUnit->GetAIUnitTemplate()->GetInternalName();
			IdleUnitsAndCitiesSet.Add( str, +1 );
		}
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			//pAICity->GetServerCity()->GetName();
			pAICity->GetName();
			IdleUnitsAndCitiesSet.Add( _T("City"), +1 );
		}
	}
	pos = IdleUnitsAndCitiesSet.m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndLong* p = IdleUnitsAndCitiesSet.m_List.GetNext( pos );
		CString str;
		str.Format( _T("%dx %s/\n"), p->m_iValue, p->m_str );
		strIdlePoi += str;
	}

	int h5656=0;

	//Checkpoint::Write( _T("CActionScheduler::AllocateTacticalProjects() - End") );
}


void  CActionScheduler::CreateRepairNearbyTaskOrWarzoneForIdleUnits()
{
	//if( m_WarzoneLocations.IsEmpty() ) return; // There are no warzones
	CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIThoughtDatabase();
	CList< CEOSAIPoiObject* >*  pMyActors = pAIThoughtDatabase->GetMyActors();

	// Compile a list of idle units
	CList< CEOSAIUnit* >  MyIdleUnits;
	POSITION pos = pMyActors->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pMyActors->GetNext( pos );
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );

		if( pAIUnit &&
			pAIUnit->IsHypotheticalPoiObject() == false &&
			pAIUnit->GetAIUnitPathwayFinder()->GetPreDefinedPath()->IsEmpty() )
		{
			MyIdleUnits.AddTail( pAIUnit );
		}
	}

	//CWorldBuildDesc* pWorldBuildDesc = m_pAIBrain->GetWorldDescServer()->GetWorldBuildDesc();

	// If this is a combat unit, look for some nearby desires the unit can accomplish
	// A Minor Task can be capture something nearby, go get repaired or unpgraded, etc.
	pos = MyIdleUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pAIUnit = MyIdleUnits.GetNext( pos );
		int g=0;
		pAIUnit->AllocateRepairNearbyTaskOrWarzoneToThisIdleUnit();
	}
}

void  CActionScheduler::AddUpgradePredefinedStepToAllUnitsWhoNeedAnUpgradeAndWithin100Pixels()
{
	CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIThoughtDatabase();
	POSITION pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			//if( pAIUnit->GetServerUnit() && 
			//	pAIUnit->GetServerUnit()->GetNeedsToBeUpgraded() )
			if( pAIUnit->GetNeedsToBeUpgraded() )
			{
				//if( pAIUnit->GetServerUnit()->IsInsideAUnitContainer() == false ||
				//	pAIUnit->GetServerUnit()->CanBeUpgradedHere() )
				if( pAIUnit->IsInsideAUnitContainer() == false ||
					pAIUnit->CanBeUpgradedHere() )
				{
					float fClosestUpgradeDistance;
					CEOSAIPoiObject* pClosestUpgradeAIPoiObject = NULL;
					GetAIThoughtDatabase()->GetClosestUpgradeLocation( pAIUnit, &pClosestUpgradeAIPoiObject, &fClosestUpgradeDistance );

					if( pClosestUpgradeAIPoiObject && fClosestUpgradeDistance < 100.0f )
					{
						CEOSAIUnitPathwayFinder* pPathwayFinder = pAIUnit->GetAIUnitPathwayFinder();

						CEOSAIUnitPathwayPredefinedStep* pPredefinedStep = new CEOSAIUnitPathwayPredefinedStep( pPathwayFinder );
						pPredefinedStep->SetAIUnitActionIdea( NULL );
						pPredefinedStep->UpgradeAt( pClosestUpgradeAIPoiObject );
						pPathwayFinder->AppendStep( pPredefinedStep );
					}
				}
			}
		}
	}
}

/*
void  CActionScheduler::AllocateTaskForceUnits( CEOSAITaskForce3* pTaskForce )
{
	POSITION pos = pTaskForce->GetUnitActionStack()->m_Items.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitActionStackItem* pStackItem = pTaskForce->GetUnitActionStack()->m_Items.GetNext( pos );
		if( pStackItem->IsAllocated() )
		{
			CEOSAIUnitActionIdea* pIdea = pStackItem->m_pUnitActionIdea;
			pIdea->AllocateActorToTacticalProject();
		}
	}
}
*/
/*
void  CActionScheduler::ScheduleTaskForce( CEOSAITaskForce3* pTaskForce )
{
	ASSERT( false );
	POSITION pos = pTaskForce->GetUnitActionStack()->m_Items.GetHeadPosition();
	while( pos )
	{
		//CEOSAIUnitActionIdea* pUnitActionIdea = pTaskForce->GetUnitActionIdeas()->GetNext( pos );
		//CEOSAIUnit* pAIUnit = pUnitActionIdea->GetAIUnitActor();
		//pAIUnit->AllocateForActionDefinition( this );
		CEOSAIUnitActionStackItem* pStackItem = pTaskForce->GetUnitActionStack()->m_Items.GetNext( pos );
		//if( pStackItem->IsReallyAllocated() )
		//if( pStackItem->ActionIdeaIsAllocated() )
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			CEOSAIUnitActionIdea* pUnitActionIdea = pStackItem->m_pUnitActionIdea;
			ASSERT( pUnitActionIdea->IsScheduled() == false );

			if( pUnitActionIdea->GetAIUnitActor()->IsHypotheticalPoiObject() )
			{
				CEOSAICity* pAICity = pUnitActionIdea->GetAIUnitActor()->GetBuiltByCity();
				ASSERT( pAICity );
				pAICity->ActionScheduler_CreateBuildOrder( pUnitActionIdea->GetUnitTemplate() );
			}
			else
			{
				CEOSAIUnit* pAIUnit = pUnitActionIdea->GetAIUnitActor();
				CEOSAIUnitAction* pAIUnitAction = pUnitActionIdea->ActionScheduler_CreateAction();
				pAIUnitAction->ActionScheduler_TurnActionsIntoTasks();
				pAIUnit->ActionScheduler_TurnTasksIntoOrders();
			}
		}
	}
}
*/

/*
void  CActionScheduler::AddDesireToTacticalProject( CAITacticalProject* pTacticalProject, CEOSAIDesireSpatial* pDesire )
{
	pTacticalProject->ActionScheduler_AddDesire( pDesire );
}
*/
/*
void  CActionScheduler::AddNearbyRelatedDesiresToTacticalProject( CAITacticalProject* pTacticalProject )
{
	// If this is a 'capture' desire, then
	//   I want to capture the nearby targets and destroy defending/nearby units
	// If this is a 'degrade' desire (bombing), then
	//   I don't need to capture or degrade nearby targets, and don't need to destroy defending/nearby units
	CAITacticalProject::EnumPrimaryAction ePrimaryAction = pTacticalProject->GetPrimaryAction();

	ASSERT( pTacticalProject->GetDesires()->IsEmpty() == FALSE );
	CEOSAIDesireSpatial* pPrimaryDesire = pTacticalProject->GetDesires()->GetHead();
	CList< CEOSAIDesireSpatial* >  RelatedDesires;

	POSITION pos = pPrimaryDesire->GetConnectedDesires()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireConnection* pConnection = pPrimaryDesire->GetConnectedDesires()->GetNext( pos );
		CEOSAIDesireSpatial* pOtherDesire = pConnection->m_pConnectedAISpatialDesire;
		ASSERT( pPrimaryDesire != pOtherDesire );

		// TEMPORARY - for simplicity, I'm going to use "zero geo switches"
		//    Depending on the situation, I could handle a geo switch, but it depends on the situation
		/*
		float fDistance = pConnection->m_fAirDistance;
		fDistance += ( 50.0f * pConnection->m_iMinimumGeoSwitches );
		if( fDistance > 200.0f ) continue;
		*-/
		if( pConnection->m_iMinimumGeoSwitches > 0 ) continue;
		//if( pConnection->m_fAirDistance > 180.0f ) continue;

		if( ePrimaryAction == CAITacticalProject::enum_CaptureArea )
		{
			if( pOtherDesire->GetType() == CEOSAIDesireSpatial::EnumCaptureCity ||
				pOtherDesire->GetType() == CEOSAIDesireSpatial::EnumCaptureResource ||
				pOtherDesire->GetType() == CEOSAIDesireSpatial::EnumDestroyUnit )
			{
				RelatedDesires.AddTail( pOtherDesire );
			}
			eif( pOtherDesire->GetType() == CEOSAIDesireSpatial::EnumDegradeCity ||
				 pOtherDesire->GetType() == CEOSAIDesireSpatial::EnumBuildAirfield ||
				 pOtherDesire->GetType() == CEOSAIDesireSpatial::EnumDestroyAirfield )
			{
				//
			}
			else
			{
				ASSERT( false );
			}
		}
	}

	//CList< CEOSAIDesireSpatial* >  RelatedDesires;
	//pDesire->GetSingleGeoConnectedDesires( &RelatedDesires );
	//GetSingleGeoConnectedDesires( pTacticalProject->  &RelatedDesires );
	//CAITacticalProject::enum_CaptureArea

	//pTacticalProject->ActionScheduler_AddDesire( pDesire );
	pos = RelatedDesires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pRelatedDesire = RelatedDesires.GetNext( pos );
		pTacticalProject->ActionScheduler_AddDesire( pRelatedDesire );
	}
}
*/

/*
void  CActionScheduler::AddDesireAndRelatedDesiresToTacticalProject( CAITacticalProject* pTacticalProject, CEOSAIDesireSpatial* pDesire )
{
	// If this is a 'capture' desire, then
	//   I want to capture the nearby targets and destroy defending/nearby units
	// If this is a 'degrade' desire (bombing), then
	//   I don't need to capture or degrade nearby targets, and don't need to destroy defending/nearby units
	/-*
	CList< CEOSAIDesireSpatial* >  RelatedDesires;
	//pDesire->GetSingleGeoConnectedDesires( &RelatedDesires );
	GetSingleGeoConnectedDesires( pTacticalProject->  &RelatedDesires );
	CAITacticalProject::enum_CaptureArea

	pTacticalProject->ActionScheduler_AddDesire( pDesire );
	POSITION pos = RelatedDesires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pRelatedDesire = RelatedDesires.GetNext( pos );
		pTacticalProject->ActionScheduler_AddDesire( pRelatedDesire );
	}
	*-/
}
*/
/*
void  CActionScheduler::CreateHypotheticalTaskForceForAllTacticalProjects()
{
	POSITION pos = GetAIThoughtDatabase()->GetTacticalProjects2()->GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = GetAIThoughtDatabase()->GetTacticalProjects2()->GetNext( pos );
		//pTacticalProject->CreateBestCaseTaskForce();
		ASSERT( false );
	}
}
*/
/*
float CActionScheduler::GetCurrentScore()
{
	float fScore = 0.0f;
	POSITION pos = GetAIThoughtDatabase()->GetTacticalProjects2()->GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = GetAIThoughtDatabase()->GetTacticalProjects2()->GetNext( pos );
		//pActionDefinition->CreateHypotheticalTaskForce();
		if( pTacticalProject->IsScheduled() )
		{
			fScore += 1.0f;
		}
	}
	return fScore;
}
*/
/*
float CActionScheduler::GetAllocatedUnitsAndCities01()
{
	float fAllocated01 = (float) m_iUnitsAndCitiesAllocatedToTacticalProjects / (1.5f*m_iNumberOfMyUnitsAndCities + 3);
	if( fAllocated01 > 1.0f ){ fAllocated01 = 1.0f; }
	return fAllocated01;
	//return m_iUnitsAndCitiesAllocatedToTacticalProjects / (1.5f*m_iNumberOfMyUnitsAndCities + 3);
}
*/

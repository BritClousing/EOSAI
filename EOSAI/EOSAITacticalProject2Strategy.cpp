
#include "stdafx.h"
#include "EOSAITacticalProject2Strategy.h"
#include "EOSAITacticalProject2.h"
#include "EOSAIUnit2.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireSpatial.h"
//#include "ResourceSource.h"
//#include "Unit.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAIUnitTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAITacticalProject2Strategy::CEOSAITacticalProject2Strategy( CEOSAITacticalProject2* pProject, EnumStrategy eStrategy )
{
	m_pTacticalProject2 = pProject;
	ASSERT( m_pTacticalProject2 );

	m_eStrategy = eStrategy;
	m_eCurrentPlan = enum_Undefined;
	m_iBestCaseValueIndex = 0;

	ASSERT( //m_eStrategy == enum_SeedIsland ||
			//m_eStrategy == enum_SeedIslandWithAirDrop  ||
			m_eStrategy == enum_Invasion ||
			m_eStrategy == enum_InvasionWithAirDrop ||
			m_eStrategy == enum_AirStrike ||
			m_eStrategy == enum_Bombardment ||
			m_eStrategy == enum_AirSearchAndDestroyStrategy ||
			m_eStrategy == enum_SubmarineStrategy );

	m_BestCaseTaskForce.SetTacticalProject2( m_pTacticalProject2 );
	m_BestCaseTaskForce.SetTacticalProject2Strategy( this );
	m_BestCaseTaskForce.SetBestCaseOrRealTaskForce( CEOSAITaskForce3::BestCaseTaskForce );

	m_NegotiatedTaskForce.SetTacticalProject2( m_pTacticalProject2 );
	m_NegotiatedTaskForce.SetTacticalProject2Strategy( this );
	m_NegotiatedTaskForce.SetBestCaseOrRealTaskForce( CEOSAITaskForce3::RealTaskForce );

	//m_fBestCaseTaskForceValue = 0.0f;
	//m_fNegotiatedTaskForceValue = 0.0f;

	m_fSufficientForcesTime = 1000000.0f;
	m_fHisExpectedLosses = 0.0f;
	m_fMyExpectedLosses = 0.0f;
}

CEOSAITacticalProject2Strategy::~CEOSAITacticalProject2Strategy()
{
	int g=0;
}


// Debug
//
void  CEOSAITacticalProject2Strategy::Dump( CString& strData )
{
	CString str;

	str.Format( _T("TacticalProjectSimpleInterest: %0.2f\n"), GetTacticalProject2()->GetSimpleInterest() );
	strData += str;
	str.Format( _T("TacticalProject2InitialEvalScore: %0.2f/%0.2f/%0.2f\n"), 
		GetTacticalProject2()->GetInitialEvaluation_BasicScore(),
		GetTacticalProject2()->GetInitialEvaluation_EasyTargetScore(),
		GetTacticalProject2()->GetInitialEvaluation_NoResistanceScore() );
	strData += str;
	//str.Format( _T("TacticalProjectResistancePower: %0.2f\n"), GetTacticalProject2()->GetResistanceCombatSignificance() );
	//strData += str;

	str.Format( _T("BestCaseTaskForce.GrowthScore: %0.2f\n"), m_BestCaseTaskForce.GetGrowthScore() );
	strData += str;
	str.Format( _T("BestCaseTaskForce.RealScore: %0.2f\n"), m_BestCaseTaskForce.GetRealScore() );
	strData += str;

	// Desires
	strData += _T(" Desires:\n");
/*
	for( long i=0; i<4; i++ )
	{
		CList< CEOSAIDesireSpatial* >* pDesires;
		if( i==0 )
		{
			pDesires = m_pTacticalProject2->GetPrimaryGoals();

			float fEnemyUnitCost = 0.0f;
			float fAIPower = 0.0f;
			POSITION pos = pDesires->GetHeadPosition();
			while( pos )
			{
				CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );
				CPoiObject* pPoiObject = pDesire->GetPoiObjectTarget();
				CUnit* pTargetUnit = dynamic_cast< CUnit* >( pPoiObject );
				if( pTargetUnit )
				{
					fEnemyUnitCost += pTargetUnit->GetProductionCost();
					fAIPower += pTargetUnit->GetAICombatSignificance();//GetAIPowerValue();
				}
			}
			//strData += "   Primary Goals:\n";
			str.Format( _T("   Primary Goals  (ProdCost: %1.0f, AIPower: %1.0f)\n"), fEnemyUnitCost, fAIPower );
			strData += str;
		}
		if( i==1 )
		{
			pDesires = m_pTacticalProject2->GetLocalResistance();//&m_LocalResistance;

			float fEnemyUnitCost = 0.0f;
			float fAIPower = 0.0f;
			POSITION pos = pDesires->GetHeadPosition();
			while( pos )
			{
				CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );
				CPoiObject* pPoiObject = pDesire->GetPoiObjectTarget();
				CUnit* pTargetUnit = dynamic_cast< CUnit* >( pPoiObject );
				if( pTargetUnit )
				{
					fEnemyUnitCost += pTargetUnit->GetProductionCost();
					//fAIPower += pTargetUnit->GetUnitTemplate()->GetAIPowerValue();
					fAIPower += pTargetUnit->GetAICombatSignificance();//>GetUnitTemplate()->GetAIPowerValue();
				}
			}
			str.Format( _T("   Local Resistance:  (ProdCost: %1.0f, AIPower: %1.0f)\n"), fEnemyUnitCost, fAIPower );
			strData += str;
		}
		if( i==2 )
		{
			pDesires = m_pTacticalProject2->GetPathwayResistance();//&m_PathwayResistance;

			float fEnemyUnitCost = 0.0f;
			float fAIPower = 0.0f;
			POSITION pos = pDesires->GetHeadPosition();
			while( pos )
			{
				CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );
				CPoiObject* pPoiObject = pDesire->GetPoiObjectTarget();
				CUnit* pTargetUnit = dynamic_cast< CUnit* >( pPoiObject );
				if( pTargetUnit )
				{
					fEnemyUnitCost += pTargetUnit->GetProductionCost();
					fAIPower += pTargetUnit->GetAICombatSignificance();//GetAIPowerValue();
				}
			}
			//ASSERT( pDesires->IsEmpty() == FALSE );

			str.Format( _T("   Pathway Resistance:  (ProdCost: %1.0f, AIPower: %1.0f)\n"), fEnemyUnitCost, fAIPower );
			strData += str;
		}
		if( i==3 )
		{
			pDesires = m_pTacticalProject2->GetOnTheWayOpportunities();//&m_OnTheWayOpportunities;
			strData += _T("   On The Way Opportunities:\n");
		}

		POSITION pos = pDesires->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );

			strData += _T("     ");

			if( pDesire->GetType() == EnumCaptureCity ){ strData += _T("CaptureCity | "); }
			if( pDesire->GetType() == EnumDegradeCity ){ strData += _T("DegradeCity | "); }
			if( pDesire->GetType() == EnumCaptureResource ){ strData += _T("CaptureRes | "); }
			if( pDesire->GetType() == EnumDestroyUnit ){ strData += _T("DestroyUnit | "); }
			if( pDesire->GetType() == EnumBuildAirfield ){ strData += _T("BuildAirfield | "); }
			if( pDesire->GetType() == EnumDestroyAirfield ){ strData += _T("DestroyAirfield | "); }

			/-*
			if( dynamic_cast< CCity* >( pDesire->GetPoiObjectTarget() ) ){ strData += "City | "; }
			if( dynamic_cast< CResourceSource* >( pDesire->GetPoiObjectTarget() ) ){ strData += "Res | "; }
			if( dynamic_cast< CAirfield* >( pDesire->GetPoiObjectTarget() ) ){ strData += "Airfield | "; }
			if( dynamic_cast< CUnit* >( pDesire->GetPoiObjectTarget() ) ){ strData += "Unit | "; }
			*-/
			if( dynamic_cast< CEOSAICity* >( pDesire->GetAIPoiObjectTarget() ) )
			{
				//strData += pDesire->GetPoiObjectTarget()->GetName();
				CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pDesire->GetAIPoiObjectTarget() );
				CString strTemp;
				strTemp.Format( _T("%s Prod:%d"), pAICity->GetServerCity()->GetName(), pAICity->GetTotalProduction() );
				strData += strTemp;
			}
			if( dynamic_cast< CEOSAIResource* >( pDesire->GetAIPoiObjectTarget() ) )
			{
				CEOSAIResource* pAIRes = dynamic_cast< CEOSAIResource* >( pDesire->GetAIPoiObjectTarget() );
				CString strTemp;
				strTemp.Format( _T("%s Prod:%d"), pAIRes->GetResource()->GetResourceType(), pAIRes->GetResource()->GetResourcePerTurn() );
				strData += strTemp;
			}
			if( dynamic_cast< CEOSAIUnit* >( pDesire->GetAIPoiObjectTarget() ) )
			{
				CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pDesire->GetAIPoiObjectTarget() );
				CString strTemp;
				strTemp.Format( _T("%s "), pAIUnit->GetUnitTemplate()->GetInternalName() );
				strData += strTemp;
			}
			str.Format( _T(" (ObjectId %d)\n"), pDesire->GetPoiObjectTarget()->GetObjectId() );
			strData += str;

			str.Format( _T("       SimpleInterest: %0.3f\n"), pDesire->GetSimpleInterest() );
			strData += str;
		}
	}
	*/

	// Units
	strData += _T(" BaseCaseTaskForce Allocated Units:\n");
	POSITION pos = GetBestCaseTaskForce()->GetUnitActionStack()->m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitActionStackItem* pStackItem = GetBestCaseTaskForce()->GetUnitActionStack()->m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			if( pStackItem->m_pUnitActionIdea->GetAIUnitActor() &&
				pStackItem->m_pUnitActionIdea->GetAIUnitActor()->GetObjectId() > 0 )
			{
				str.Format( _T("  %s (ObjectId %d)\n"), 
					pStackItem->m_pUnitActionIdea->GetAIUnitActor()->GetAIUnitTemplate()->GetInternalName(),
					pStackItem->m_pUnitActionIdea->GetAIUnitActor()->GetObjectId() );
			}
			eif( pStackItem->m_pUnitActionIdea->GetAICityActor() )
			{
				str.Format( _T("  %s (UnitTemplate)\n"), 
					pStackItem->m_pUnitActionIdea->GetAIUnitTemplate()->GetInternalName() );
					//pStackItem->m_pUnitActionIdea->GetAIUnitActor()->GetUnitTemplate()->GetInternalName() );
			}
			strData += str;
		}
	}
	strData += _T("\n");
}

CEOSAITacticalProject2Strategy::EnumTaskPursuit  CEOSAITacticalProject2Strategy::HandleLocalResistance()
{
	//if( m_eStrategy == enum_SeedIsland                  ){ return enum_HandleTasks; }
	//if( m_eStrategy == enum_SeedIslandWithAirDrop       ){ return enum_HandleTasks; }
	if( m_eStrategy == enum_Invasion                    ){ return enum_HandleTasks; }
	if( m_eStrategy == enum_InvasionWithAirDrop         ){ return enum_HandleTasks; }
	if( m_eStrategy == enum_AirStrike                   ){ return enum_AvoidEnemies; }
	if( m_eStrategy == enum_Bombardment                 ){ return enum_HandleTasks; }
	if( m_eStrategy == enum_AirSearchAndDestroyStrategy ){ return enum_AvoidEnemies; }
	if( m_eStrategy == enum_SubmarineStrategy           ){ return enum_AvoidEnemies; }
	ASSERT( false );
	return enum_IgnoreTasks;
}

CEOSAITacticalProject2Strategy::EnumTaskPursuit  CEOSAITacticalProject2Strategy::HandlePathwayResistance()
{
	//if( m_eStrategy == enum_SeedIsland                  ){ return enum_HandleTasks; }
	//if( m_eStrategy == enum_SeedIslandWithAirDrop       ){ return enum_OptionalTasks; }
	if( m_eStrategy == enum_Invasion                    ){ return enum_HandleTasks; }
	if( m_eStrategy == enum_InvasionWithAirDrop         ){ return enum_OptionalTasks; }
	if( m_eStrategy == enum_AirStrike                   ){ return enum_AvoidEnemies; }
	if( m_eStrategy == enum_Bombardment                 ){ return enum_HandleTasks; }
	if( m_eStrategy == enum_AirSearchAndDestroyStrategy ){ return enum_AvoidEnemies; }
	if( m_eStrategy == enum_SubmarineStrategy           ){ return enum_AvoidEnemies; }
	ASSERT( false );
	return enum_IgnoreTasks;
}

CEOSAITacticalProject2Strategy::EnumTaskPursuit  CEOSAITacticalProject2Strategy::HandleOnTheWayOpportunities()
{
	//if( m_eStrategy == enum_SeedIsland                  ){ return enum_OptionalTasks; }
	//if( m_eStrategy == enum_SeedIslandWithAirDrop       ){ return enum_IgnoreTasks; }
	if( m_eStrategy == enum_Invasion                    ){ return enum_OptionalTasks; }
	if( m_eStrategy == enum_InvasionWithAirDrop         ){ return enum_IgnoreTasks; }
	if( m_eStrategy == enum_AirStrike                   ){ return enum_IgnoreTasks; }
	if( m_eStrategy == enum_Bombardment                 ){ return enum_HandleTasks; }
	if( m_eStrategy == enum_AirSearchAndDestroyStrategy ){ return enum_IgnoreTasks; }
	if( m_eStrategy == enum_SubmarineStrategy           ){ return enum_OptionalTasks; }
	ASSERT( false );
	return enum_IgnoreTasks;
}

bool CEOSAITacticalProject2Strategy::UseOnlyAirUnits()
{
	//if( m_eStrategy == enum_SeedIsland                  ){ return false; }
	//if( m_eStrategy == enum_SeedIslandWithAirDrop       ){ return false; }
	if( m_eStrategy == enum_Invasion                    ){ return false; }
	if( m_eStrategy == enum_InvasionWithAirDrop         ){ return false; }
	if( m_eStrategy == enum_AirStrike                   ){ return true; }
	if( m_eStrategy == enum_Bombardment                 ){ return false; }
	if( m_eStrategy == enum_AirSearchAndDestroyStrategy ){ return true; }
	if( m_eStrategy == enum_SubmarineStrategy           ){ return false; }
	ASSERT( false );
	return enum_IgnoreTasks;
}

bool CEOSAITacticalProject2Strategy::UseAirTransports()
{
	//if( m_eStrategy == enum_SeedIsland                  ){ return false; }
	//if( m_eStrategy == enum_SeedIslandWithAirDrop       ){ return true; }
	if( m_eStrategy == enum_Invasion                    ){ return false; }
	if( m_eStrategy == enum_InvasionWithAirDrop         ){ return true; }
	if( m_eStrategy == enum_AirStrike                   ){ return false; }
	if( m_eStrategy == enum_Bombardment                 ){ return false; }
	if( m_eStrategy == enum_AirSearchAndDestroyStrategy ){ return false; }
	if( m_eStrategy == enum_SubmarineStrategy           ){ return false; }
	ASSERT( false );
	return enum_IgnoreTasks;
}

void  CEOSAITacticalProject2Strategy::GetDesires( CList< CEOSAIDesireSpatial* >* pList )
{
	//CList< CEOSAIDesireSpatial* >* GetPrimaryGoals(){ return &m_PrimaryGoals; }
	//CList< CEOSAIDesireSpatial* >* GetLocalResistance(){ return &m_LocalResistance; }
	//CList< CEOSAIDesireSpatial* >* GetPathwayResistance(){ return &m_PathwayResistance; }

	POSITION pos = m_pTacticalProject2->GetPrimaryGoals()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_pTacticalProject2->GetPrimaryGoals()->GetNext( pos );
		pList->AddTail( pDesire );
	}
	if( HandleLocalResistance() )
	{
		pos = m_pTacticalProject2->GetLocalResistance()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = m_pTacticalProject2->GetLocalResistance()->GetNext( pos );
			pList->AddTail( pDesire );
		}
	}
	if( HandlePathwayResistance() )
	{
		pos = m_pTacticalProject2->GetPathwayResistance()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = m_pTacticalProject2->GetPathwayResistance()->GetNext( pos );
			pList->AddTail( pDesire );
		}
	}
	if( HandleOnTheWayOpportunities() )
	{
		pos = m_pTacticalProject2->GetOnTheWayOpportunities()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = m_pTacticalProject2->GetOnTheWayOpportunities()->GetNext( pos );
			pList->AddTail( pDesire );
		}
	}
}

void CEOSAITacticalProject2Strategy::ConstructBestCaseTaskForce()
{
	m_BestCaseTaskForce.ConstructTaskForce();
	//UpdateTaskForceGoalsAndJobs( &m_BestCaseTaskForce );
	//ConstructTaskForce( &m_BestCaseTaskForce );
}

void CEOSAITacticalProject2Strategy::ConstructNegotiatedTaskForce()
{
	m_NegotiatedTaskForce.ConstructTaskForce();
	//UpdateTaskForceGoalsAndJobs( &m_NegotiatedTaskForce );
	//ConstructTaskForce( &m_NegotiatedTaskForce );
}

void CEOSAITacticalProject2Strategy::UpdateTaskForceGoalsAndJobs( CEOSAITaskForce3* pTaskForce )
{
	ASSERT( m_pTacticalProject2 );
	CEOSAIJobsToDo* pPrimaryGoalsJobs = m_pTacticalProject2->GetPrimaryGoals_JobsToDo();
	CEOSAIJobsToDo* pLocalResistanceJobs = m_pTacticalProject2->GetLocalResistance_JobsToDo();
	CEOSAIJobsToDo* pPathwayResistanceJobs = m_pTacticalProject2->GetPathwayResistance_JobsToDo();
	//CEOSAIJobsToDo* pOnTheWayOpportunitiesJobs = m_pTacticalProject->GetOnTheWayOpportunities_JobsToDo();


	// Roles:
	//   MainTasks: PrimaryGoals / LocalResistance / PathwayResistance / OnTheWay
	//   Support: Transport
	// Steps:
	//   1. Calculate what units I'll use to solve the MainTasks.
	//   2. Figure out if I need to use transports (based on the reserved units).

	if( m_eStrategy == enum_Invasion )
	{
		// Could do a 'seed' strategy, or a full invasion strategy
		//   The choice affects the Units that I put into the JobsToDo list.
		//   (Seed strategy means putting one militia into the list.)
		//   (Invasion means putting all the units into the list.)
		//   There may be a middle ground between the two strategies

		// Units which must be fought:
		//   + All enemy units
		//   + All units which will be my enemy if this is an aggressive action against someone
		//   + At least one militia unit (seed strategy), and maybe all militia (quick invasion)
	}
	if( m_eStrategy == enum_InvasionWithAirDrop )
	{
		// Do I have any air transports?  (Probably not.)  If not, then immediately fail.
		pTaskForce->UseAirTransports( true );

		float fFastestAirTransport = m_pTacticalProject2->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetFastestAirTransportMovementRate();
		if( fFastestAirTransport == 0.0f )
		{
			return; // I have no air-transports
		}
		//ASSERT( false );  // this isn't bad.  I just want to see it happen sometime.
	}
	if( m_eStrategy == enum_AirStrike )
	{
		// Do I have (or can build) any air units?  (Probably.)  If not, then immediately fail.
		pTaskForce->UseOnlyAirUnits( true );
	}

	pTaskForce->GetJobsToDo()->Clear();
	pTaskForce->GetJobsToDo()->Add( pPrimaryGoalsJobs );
	if( HandleLocalResistance() ){ pTaskForce->GetJobsToDo()->Add( pLocalResistanceJobs ); }
	if( HandlePathwayResistance() ){ pTaskForce->GetJobsToDo()->Add( pPathwayResistanceJobs ); }
}
/*
//void CAITacticalProjectStrategy::CalculateValueOfTaskForce( CEOSAITaskForce3* pTaskForce )
void CAITacticalProjectStrategy::ConstructTaskForce( CEOSAITaskForce3* pTaskForce )
{
/-*
	ASSERT( m_pTacticalProject );
	CEOSAIJobsToDo* pPrimaryGoalsJobs = m_pTacticalProject->GetPrimaryGoals_JobsToDo();
	CEOSAIJobsToDo* pLocalResistanceJobs = m_pTacticalProject->GetLocalResistance_JobsToDo();
	CEOSAIJobsToDo* pPathwayResistanceJobs = m_pTacticalProject->GetPathwayResistance_JobsToDo();
	//CEOSAIJobsToDo* pOnTheWayOpportunitiesJobs = m_pTacticalProject->GetOnTheWayOpportunities_JobsToDo();

	// Clear the 'reserved' flag
	/-*
	POSITION pos;
	pos = m_pTacticalProject->GetAIUnitDistances()->GetHeadPosition();
	while( pos )
	{
		CAIPoiObjectDistance* pAIPoiObjectDistance = m_pTacticalProject->GetAIUnitDistances()->GetNext( pos );
		pAIPoiObjectDistance->m_bReserved = false;
	}
	pos = m_pTacticalProject->GetAICityDistances()->GetHeadPosition();
	while( pos )
	{
		CAIPoiObjectDistance* pAIPoiObjectDistance = m_pTacticalProject->GetAICityDistances()->GetNext( pos );
		pAIPoiObjectDistance->m_bReserved = false;
	}
	*-/

	// Roles:
	//   MainTasks: PrimaryGoals / LocalResistance / PathwayResistance / OnTheWay
	//   Support: Transport
	// Steps:
	//   1. Calculate what units I'll use to solve the MainTasks.
	//   2. Figure out if I need to use transports (based on the reserved units).

	if( m_eStrategy == enum_Invasion )
	{
		// Could do a 'seed' strategy, or a full invasion strategy
		//   The choice affects the Units that I put into the JobsToDo list.
		//   (Seed strategy means putting one militia into the list.)
		//   (Invasion means putting all the units into the list.)
		//   There may be a middle ground between the two strategies

		// Units which must be fought:
		//   + All enemy units
		//   + All units which will be my enemy if this is an aggressive action against someone
		//   + At least one militia unit (seed strategy), and maybe all militia (quick invasion)
	}
	if( m_eStrategy == enum_InvasionWithAirDrop )
	{
		// Do I have any air transports?  (Probably not.)  If not, then immediately fail.
		pTaskForce->UseAirTransports( true );

		float fFastestAirTransport = m_pTacticalProject->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetFastestAirTransportMovementRate();
		if( fFastestAirTransport == 0.0f )
		{
			return; // I have no air-transports
		}
		ASSERT( false );  // this isn't bad.  I just want to see it happen sometime.
	}
	if( m_eStrategy == enum_AirStrike )
	{
		// Do I have (or can build) any air units?  (Probably.)  If not, then immediately fail.
		pTaskForce->UseOnlyAirUnits( true );
	}

	pTaskForce->GetJobsToDo()->Clear();
	pTaskForce->GetJobsToDo()->Add( pPrimaryGoalsJobs );
	if( HandleLocalResistance() ){ pTaskForce->GetJobsToDo()->Add( pLocalResistanceJobs ); }
	if( HandlePathwayResistance() ){ pTaskForce->GetJobsToDo()->Add( pPathwayResistanceJobs ); }
	//if( HandleOnTheWayOpportunities() ){ JobsToDo.Add( &OnTheWayOpportunitiesJobs ); }
*-/
	//m_BestCaseTaskForce.CreateActionIdeas(); // based on AIJobsToDo
	pTaskForce->ConstructTaskForce();

	//float f1 = pTaskForce->GetGrowthScore();
	//float f2 = pTaskForce->GetRealScore();
	//int h=0;
	//m_fValue = 0.0f;
	//if( this->m_BestCaseTaskForce.HasSufficientForces() )
	if( pTaskForce == &m_BestCaseTaskForce ){ m_fBestCaseTaskForceValue = pTaskForce->GetRealScore(); }
	if( pTaskForce == &m_NegotiatedTaskForce ){ m_fNegotiatedTaskForceValue = pTaskForce->GetRealScore(); }
/-*
	//
	ReserveAIUnitsToHandleMainTasks();

	// Step 1. Lookup units to accomplish the MainTask
	bool bUseUnit = false;
	pos = m_pTacticalProject->GetAIUnitDistances()->GetHeadPosition();
	while( pos )
	{
		CAIPoiObjectDistance* pAIPoiObjectDistance = m_pTacticalProject->GetAIUnitDistances()->GetNext( pos );
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObjectDistance->m_pAIPoiObject );
		ASSERT( pAIUnit );
		bUseUnit = false;
		if( ( pAIUnit->IsGroundUnit() || pAIUnit->IsSeaUnit() ) && m_bMainTasks_GroundAndSeaUnitsAreAllowed )
		{
			bUseUnit = true;
		}
		if( pAIUnit->IsAirUnit() )
		{
			bUseUnit = true;
		}

		// Consider using this unit
		
	}
	pos = m_pTacticalProject->GetAICityDistances()->GetHeadPosition();
	while( pos )
	{
		CAIPoiObjectDistance* pAIPoiObjectDistance = m_pTacticalProject->GetAICityDistances()->GetNext( pos );
	}

	// Step 2. If the action requires transports, lookup some transports
	float fEarliestTransportTimeToTarget = 0.0f;
	if( m_bSupportTask_RequiresAirTransports )
	{
		AddEarliestAirTransports();
	}
	if( m_bSupportTask_RequiresAirOrSeaTransports )
	{
		AddEarliestAirOrSeaTransports();
	}
	*-/
}
*/
//bool CAITacticalProjectStrategy::IsAllocated()
bool CEOSAITacticalProject2Strategy::NegotiatedTaskForceHasBeenAllocated()
{
	if( m_NegotiatedTaskForce.GetRealScore() > 0.0f ) return true;
	return false;
}

void CEOSAITacticalProject2Strategy::CreatePredefinedSteps()
{
	// Note: I need to schedule the NegotiatedTaskForce
	//m_pTacticalProject->GetAIBrain()->GetAIActionScheduler()->ScheduleTaskForce( &m_BestCaseTaskForce );
	//m_BestCaseTaskForce.Schedule();
	//m_NegotiatedTaskForce.Schedule();
	m_NegotiatedTaskForce.CreatePredefinedSteps();
}
/*
void CEOSAITacticalProject2Strategy::CalculateTimingDataFromPredefinedSteps()
{
	m_NegotiatedTaskForce.CalculateTimingDataFromPredefinedSteps();
}
*/
//void CAITacticalProjectStrategy::ScheduleTaskForce()
/*
void CAITacticalProjectStrategy::CreateActions()
{
	// Note: I need to schedule the NegotiatedTaskForce
	//m_pTacticalProject->GetAIBrain()->GetAIActionScheduler()->ScheduleTaskForce( &m_BestCaseTaskForce );
	//m_BestCaseTaskForce.Schedule();
	//m_NegotiatedTaskForce.Schedule();
	m_NegotiatedTaskForce.CreateActions();
	int g=0;
}
*/

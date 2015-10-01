
#include "stdafx.h"
#include "EOSAIBuildManager.h"
#include "EOSAIThoughtDatabase.h"
#include "AIPlayer.h"
#include "EOSAIBuildNeed.h"
#include "City.h"
#include "EOSAIUnit2.h"
//#include "WorldDescServer.h"
#include "EOSAIUnitTemplatesAndFloat.h"
//#include "AITacticalProject.h"
#include "EOSAITacticalProject2.h"
#include "EOSAIJobCapability.h"
#include "EOSAICommonData2.h"
//#include "Player.h"
//#include "BuildingDescription.h"
#include "EOSAIMultiRegionNationwidePathways.h"
#include "EOSAIQuickCombatCalculation.h"
#include "EOSAIJobSolution.h"
#include "EOSAIStringAndFloat.h"
//#include "WorldDescPlayer.h"
#include "EOSAIDesireSpatial.h"
//#include "BuildOption.h"
//#include "BuildCompletion.h"
#include "EOSAIGlobalForeignRelations.h"
#include "EOSAIPlayerManager.h"
#include "EOSAICommonData2.h"
#include "EOSAIBuildOrder.h"
#include "EOSAIBuildCompletion.h"
#include "EOSAIBuildOption.h"
#include "EOSAIStringAndLongSet.h"
#ifdef _DEBUG
#include "EOSAIDesireCaptureCitRes.h"
#endif
//#include "BCMathFloat01.h"
//#include "TWDx.h"
#include "EOSAIInterface.h"

using namespace EOSAI;
extern CInterface* g_pEOSAIInterface;

// Highest sort values first
bool SortAICitiesBySortValue( CEOSAICity* pAICity1, CEOSAICity* pAICity2 )
{
	if( pAICity2->GetSortValue() > pAICity1->GetSortValue() ){ return false; }
	if( pAICity2->GetSortValue() < pAICity1->GetSortValue() ){ return true; }
	return true;  // always return true when values are equal
}

// Highest values first
bool SortByFloat( EOSAI::StringAndFloat* pStringAndFloat1, EOSAI::StringAndFloat* pStringAndFloat2 )
{
	if( pStringAndFloat2->m_fValue > pStringAndFloat1->m_fValue ){ return false; }
	if( pStringAndFloat2->m_fValue < pStringAndFloat1->m_fValue ){ return true; }
	return true;  // always return true when values are equal
}

CAIBuildManager::CAIBuildManager()
{
	m_pAIPlayer = NULL;
	m_iNumberOfAvailableCities = 0;
}

CAIBuildManager::~CAIBuildManager()
{
//	while( m_BuildNeeds.IsEmpty() == FALSE ){ delete m_BuildNeeds.RemoveHead(); }
}

void CAIBuildManager::SetAIPlayer( EOSAI::AIPlayer* pAIPlayer )
{
	m_pAIPlayer = pAIPlayer;
}

void CAIBuildManager::Clear()
{
//	while( m_BuildNeeds.IsEmpty() == FALSE ){ delete m_BuildNeeds.RemoveHead(); }
	//while( m_AIBuildOptions.IsEmpty() == FALSE ){ delete m_AIBuildOptions.RemoveHead(); }
}


/*
void CAIBuildManager::AddBuildNeed( float fPriority01, CEOSAIUnitTemplate* pAIUnitTemplate, CEOSAILocation PreferedLocation )
{
	CAIBuildNeed* pAIBuildNeed = new CAIBuildNeed( fPriority01, pAIUnitTemplate, PreferedLocation );
	m_BuildNeeds.AddTail( pAIBuildNeed );
}
*/
void CAIBuildManager::CreateBuildOrders_FirstPass()
{
	// Setup initial build-orders:
	//   Anything more than half-done gets set
	//   Cities in imminent danger should build ground units to defend themselves (GroundUnits)
	ASSERT( m_pAIPlayer );
	CEOSAIThoughtDatabase* pAIThoughtDatabase = m_pAIPlayer->GetAIBrain()->GetAIThoughtDatabase();
	POSITION pos = pAIThoughtDatabase->GetMyCities()->GetHeadPosition();
	while( pos )
	{
		// Setup build preferences
		CEOSAICity* pAICity = pAIThoughtDatabase->GetMyCities()->GetNext( pos );

		// If the city is in imminent danger, then direct the city to building quick defensive units
		float fGroundSeaDanger01 = pAICity->GetGroundSeaDanger01();
		if( fGroundSeaDanger01 > 0.1f )
		{
			pAICity->SetPreferenceForLowCostUnits01( min( 1.0f, fGroundSeaDanger01+0.3f ) );
			pAICity->SetPreferenceForGroundUnits01( min( 1.0f, fGroundSeaDanger01+0.3f ) );
		}

		if( pAICity->GetNumberOfGroundUnitsInside() >= 5 )
		{
			pAICity->SetPreferenceForGroundUnits01( 0.0f );
		}

		// If a unit is partially completed, then create an AIUnit, and set the BuildOrder
		float fMostCompletedTurnsUntilCompletion = 0.0f;
		float fMostCompletedBuildOrder01 = 0.0f;
		CEOSAIBuildOption* pMostCompletedBuildOption = NULL;
		//POSITION pos2 = pAICity->GetServerCity()->GetBuildCompletionList()->GetHeadPosition();
		POSITION pos2 = pAICity->GetAIBuildCompletionList()->GetHeadPosition();
		while( pos2 )
		{
			//CBuildCompletion* pBuildCompletion = pAICity->GetServerCity()->GetBuildCompletionList()->GetNext( pos2 );
			CEOSAIBuildCompletion* pAIBuildCompletion = pAICity->GetAIBuildCompletionList()->GetNext( pos2 );
			float fPercentComplete = pAIBuildCompletion->GetPercentComplete();
			if( fPercentComplete > fMostCompletedBuildOrder01 )
			{
				//fMostCompletedTurnsUntilCompletion = pAIBuildCompletion->TimeUntilCompletion( pAICity->GetServerCity() );
				fMostCompletedTurnsUntilCompletion = pAIBuildCompletion->TimeUntilCompletion();
				fMostCompletedBuildOrder01 = fPercentComplete;
				pMostCompletedBuildOption = pAIBuildCompletion->GetAIBuildOption();
			}
		}
		// No matter what the danger it, if I'm need less than 1 turn to complete a unit, finish it
		if( pMostCompletedBuildOption && 
			pMostCompletedBuildOption->IsAUnit() &&
			fMostCompletedTurnsUntilCompletion < 1.0f )
		{
			ASSERT( pAICity->GetNumberOfBuildOrders() == 0 );
			pAICity->ActionScheduler_CreateBuildOrder( pMostCompletedBuildOption );

			CEOSAIUnitTemplate* pAIUnitTemplate = pMostCompletedBuildOption->GetAIUnitTemplate();
			if( pAIUnitTemplate )
			{
				// Create an AIUnit
				CEOSAIUnit2* pAIUnit = new CEOSAIUnit2();// m_pAIPlayer->GetAIBrain() );
				pAIUnit->CreateHypotheticalUnit( pAICity, pAIUnitTemplate, fMostCompletedTurnsUntilCompletion );
				//m_pAIPlayer->GetAIBrain()->AddAIPoiObject( pAIUnit );
				g_pEOSAICommonData->AddHypotheticalAIPoiObject( pAIUnit );
			}
			return;
		}
		// If danger is lower, and I'm at least 50% done with a build, finish it
		if( fGroundSeaDanger01 <= 0.4f && 
			pMostCompletedBuildOption &&
			fMostCompletedBuildOrder01 > 0.5f )
		{
			ASSERT( pAICity->GetNumberOfBuildOrders() == 0 );
			pAICity->ActionScheduler_CreateBuildOrder( pMostCompletedBuildOption );

			// Create an AIUnit
			CEOSAIUnitTemplate* pAIUnitTemplate = pMostCompletedBuildOption->GetAIUnitTemplate();
			if( pAIUnitTemplate )
			{
				// Create an AIUnit
				CEOSAIUnit2* pAIUnit = new CEOSAIUnit2();// m_pAIPlayer->GetAIBrain() );
				pAIUnit->CreateHypotheticalUnit( pAICity, pAIUnitTemplate, fMostCompletedTurnsUntilCompletion );
				//m_pAIPlayer->GetAIBrain()->AddAIPoiObject( pAIUnit );
				g_pEOSAICommonData->AddHypotheticalAIPoiObject( pAIUnit );
			}
			return;
		}
		if( fGroundSeaDanger01 > 0.3f ) // Don't count these cities in the 'available cities'
		{
			return;
		}
		m_iNumberOfAvailableCities++;
	}
}


/*
void  CAIFutureBuildIdeas::AddTacticalProjectStrategy( CAITacticalProjectStrategy* pStrategy )
{
	m_JobsToDo.Add( pStrategy->GetBestCaseTaskForce()->GetJobsToDo() );
}
*/

// Algorithm:
// - Figure out what the balance of units should be (e.g. 10% transports, 20% Ground Units, etc)
//   - This depends on the situation the AI is in (if you're taking-over Asia, you don't need ships)
// - Calculate the gap between the 'desired' unit balance and the current(+build) units
void CAIBuildManager::CreateBuildOrders_Final()
{
	ASSERT( m_pAIPlayer );
	EOSAI::AIPlayer* pAIPlayer = m_pAIPlayer;
	CEOSAIBrain* pAIBrain = m_pAIPlayer->GetAIBrain();
	CEOSAIThoughtDatabase* pAIThoughtDatabase = pAIBrain->GetAIThoughtDatabase();
//	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
//	CGlobalForeignRelations* pGlobalForeignRelations = pWorldDescServer->GetGlobalForeignRelations();
//	CEOSAIGlobalForeignRelationsState* pGlobalForeignRelations = g_pAIPlayerManager->GetGlobalForeignRelations();
	CEOSAIGlobalForeignRelations* pGlobalForeignRelations = g_pEOSAICommonData->GetGlobalForeignRelations();
	long iAIPlayer = pAIBrain->GetAIPlayer()->GetPlayerNumber();

	//CEOSAIUnit2TemplatesAndFloat  MyUnits;
	//CEOSAIUnit2TemplatesAndFloat  EnemyUnits;
	CEOSAIJobCapability  AIJobCapability;
	CEOSAIJobsToDo       AIJobsToDo;
	CEOSAIJobSolution    AIJobSolution( &AIJobsToDo, &AIJobCapability );
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoi = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CUnit* pUnit = dynamic_cast< CUnit* >( pPoi );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pPoi );
		if( pAIUnit )//&& pAIUnit->IsAlive() )
		{
			EOSAIEnumForeignRelations eRel = pAIUnit->GetForeignRelationsTo( iAIPlayer );
			float fFeelingsAboutPlayer01 = pGlobalForeignRelations->GetFeelings01( iAIPlayer, pAIUnit->GetOwner() );

			if( eRel == EOSAIEnumForeignRelations::enum_Self )
			{
				ASSERT( fFeelingsAboutPlayer01 == 1.0f );
				AIJobCapability.Add( pAIUnit->GetAIUnitTemplate()->GetAIJobCapability() );
			}
			else
			{
				if( eRel <= EOSAIEnumForeignRelations::enum_Neutral ||
				  ( eRel == EOSAIEnumForeignRelations::enum_Alliance && fFeelingsAboutPlayer01 < 0.7f ) )
				{
					AIJobsToDo.m_EnemyUnits.AddTail( pAIUnit );
					AIJobsToDo.m_UnitTemplatesAndFloat.Add( pAIUnit->GetAIUnitTemplate(), 1.0f );
					//AIJobsToDo.AddEnemyUnit( pUnit );
					//EnemyUnits.Add( pUnit->GetUnitTemplate(), pUnit->GetCurrentHP01() );
				}
			}
		}
	}
	int g=0;

	//
	// DEBUG - Some tests
	//
	/*
	{
		CEOSAIUnit2TemplatesAndFloat  MyUnits2;
		MyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 1.0f );
		CEOSAIUnit2TemplatesAndFloat  EnemyUnits2;
		EnemyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 1.0f );
		CEOSAIUnit2TemplatesAndFloat  PowerLevels;
		PowerLevels.CalculatePowerLevelsAgainst( MyUnits2, EnemyUnits2 );
		int g=0;
	}
	{
		CEOSAIUnit2TemplatesAndFloat  MyUnits2;
		MyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 1.0f );
		CEOSAIUnit2TemplatesAndFloat  EnemyUnits2;
		EnemyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 2.0f );
		CEOSAIUnit2TemplatesAndFloat  PowerLevels;
		PowerLevels.CalculatePowerLevelsAgainst( MyUnits2, EnemyUnits2 );
		int g=0;
	}
	{
		CEOSAIUnit2TemplatesAndFloat  MyUnits2;
		MyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 2.0f );
		CEOSAIUnit2TemplatesAndFloat  EnemyUnits2;
		EnemyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 1.0f );
		EnemyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Militia" ), 1.0f );
		CEOSAIUnit2TemplatesAndFloat  PowerLevels;
		PowerLevels.CalculatePowerLevelsAgainst( MyUnits2, EnemyUnits2 );
		int g=0;
	}
	{
		CEOSAIUnit2TemplatesAndFloat  MyUnits2;
		MyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 1.0f );
		CEOSAIUnit2TemplatesAndFloat  EnemyUnits2;
		EnemyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 1.0f );
		EnemyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Militia" ), 1.0f );
		CEOSAIUnit2TemplatesAndFloat  PowerLevels;
		PowerLevels.CalculatePowerLevelsAgainst( MyUnits2, EnemyUnits2 );
		int g=0;
	}
	{
		CEOSAIUnit2TemplatesAndFloat  MyUnits2;
		MyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 2.0f );
		CEOSAIUnit2TemplatesAndFloat  EnemyUnits2;
		EnemyUnits2.Add( GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" ), 1.0f );
		CEOSAIUnit2TemplatesAndFloat  PowerLevels;
		PowerLevels.CalculatePowerLevelsAgainst( MyUnits2, EnemyUnits2 );
		int g=0;
	}
	*/
	//
	// DEBUG - Some tests
	//

	// Add-up the current capability
	/*
	CEOSAIUnit2TemplatesAndFloat  MyUnits;
	CEOSAIUnit2TemplatesAndFloat  EnemyUnits;

	pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
		if( pAIUnit )
		{
			MyUnits.Add( pAIUnit->GetUnitTemplate(), pAIUnit->GetServerUnit()->GetCurrentHP01() );
		}
	}
	*/

	// Add-up Jobs to do

	//
	// Instead of grabbing the ActivelyPursueThisAction + PlanToDoThisAction, I could
	//   just grab a whole bunch of top-rated strategies.
	// This would involve adding-up the cities/city production and looking at the unit needs of
	//   each strategy.
	//
	long iTotalCities = pAIThoughtDatabase->GetMyCities()->GetCount();

	//long iTransportJobs = 0;
	float fTotalRequiredUnits = 0;
	//CEOSAIJobsToDo  JobsToDo;
/*
	CEOSAIListSort< CAITacticalProjectStrategy* >* pStrategyList = pAIThoughtDatabase->GetSortedBestCaseTacticalProjectStrategies();
	pos = pStrategyList->GetHeadPosition();
	while( pos )
	{
		CAITacticalProjectStrategy* pStrategy = pStrategyList->GetNext( pos );
		//if( pStrategy->GetPlan() == CAITacticalProjectStrategy::enum_ActivelyPursueThisAction ||
		//	pStrategy->GetPlan() == CAITacticalProjectStrategy::enum_PlanToDoThisAction )
		//{

		float fRequiredUnits = pStrategy->GetBestCaseTaskForce()->GetEstimatedNumberOfUnitsNeeded();
		if( fRequiredUnits < 1.0f ) fRequiredUnits = 1.0f;
		fTotalRequiredUnits += fRequiredUnits;

		AIJobsToDo.Add( pStrategy->GetBestCaseTaskForce()->GetJobsToDo() );
		if( pStrategy->GetTacticalProject()->GetNumberOfCitiesOnTheSameGeo() == 0 )
		{
			AIJobSolution.AddTransportNeed( max( 1, fRequiredUnits/3.0f ) );
		}

		//if( pStrategy->GetTacticalProject()->RequiresATransport() ){ iTransportJobs++;  }
		//if( pStrategy->GetTacticalProject()->GetNumberOfCitiesOnTheSameGeo() == 0 ){ iTransportJobs += max( 1, fRequiredUnits/3.0f ); }
		//if( pStrategy->GetTacticalProject()->RequiresATransport() ){ fRequiredUnits += max( 1.0f, fRequiredUnits/2.0f ); }

		if( fTotalRequiredUnits > (iTotalCities+3)*2 ) break;
	}
*/
	//CEOSAIListSort< CAITacticalProjectStrategy* >* pStrategyList = pAIThoughtDatabase->GetSortedBestCaseTacticalProjectStrategies();
	long fRequiresSeaBasedMovement = 0;
	long fRequiresNoSeaBasedMovement = 0;
	float fSeaBasedMovementRatio01 = 0.0f;
	CEOSAIListSort< CEOSAITacticalProject2* >  TacticalProjectList;
	pAIThoughtDatabase->GetTacticalProjectsSortedByInitialEvaluationScore( TacticalProjectList );
	CEOSAIListSort< CEOSAITacticalProject2* >* pTacticalProjectList = &TacticalProjectList;
	long iCount = 0;
	float fAdditionalTransportNeed = 0.0f;
	pos = pTacticalProjectList->GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = pTacticalProjectList->GetNext( pos );
		//CAITacticalProjectStrategy* pStrategy = pStrategyList->GetNext( pos );
		//if( pStrategy->GetPlan() == CAITacticalProjectStrategy::enum_ActivelyPursueThisAction ||
		//	pStrategy->GetPlan() == CAITacticalProjectStrategy::enum_PlanToDoThisAction )
		//{

		CEOSAIJobsToDo* pPrimaryGoalsJobs = pTacticalProject->GetPrimaryGoals_JobsToDo();
		CEOSAIJobsToDo* pLocalResistanceJobs = pTacticalProject->GetLocalResistance_JobsToDo();

		// DEBUG
		#ifdef _DEBUG
		POSITION posDesires = pTacticalProject->GetPrimaryGoals()->GetHeadPosition();
		while( posDesires )
		{
			CEOSAIDesireSpatial* pDesireSpatial = pTacticalProject->GetPrimaryGoals()->GetNext( posDesires );
			ASSERT( pDesireSpatial->GetLocation().GetRealX() != 0.0f );

			CEOSAIDesireCaptureCitRes* pDesireCaptureCitRes = dynamic_cast<CEOSAIDesireCaptureCitRes*>( pDesireSpatial );
			if( pDesireCaptureCitRes )
			{
				CEOSAIPoiObject* pPoiObject = pDesireCaptureCitRes->GetAIPoiObjectTarget();
				ASSERT( pPoiObject->GetLocation().GetRealX() != 0.0f );
			}
		}
		#endif

		CEOSAIJobsToDo  ThisTacticalProjectAIJobsToDo;
		ThisTacticalProjectAIJobsToDo.Add( pPrimaryGoalsJobs );
		ThisTacticalProjectAIJobsToDo.Add( pLocalResistanceJobs );

		//float fRequiredUnits = pStrategy->GetBestCaseTaskForce()->GetEstimatedNumberOfUnitsNeeded();
		float fRequiredUnits = ThisTacticalProjectAIJobsToDo.GetEstimatedNumberOfUnitsNeeded();
		if( fRequiredUnits < 1.0f ) fRequiredUnits = 1.0f;
		fTotalRequiredUnits += fRequiredUnits;

		if( pTacticalProject->GetNumberOfCitiesOnTheSameGeo() == 0 )
		{
			fAdditionalTransportNeed += max( 1.0f, fRequiredUnits/3.0f );
			fRequiresSeaBasedMovement += (long) fRequiredUnits;
		}
		else
		{
			fRequiresNoSeaBasedMovement += (long) fRequiredUnits;
		}
		AIJobSolution.GetAIJobsToDo()->Add( &ThisTacticalProjectAIJobsToDo );

		if( fTotalRequiredUnits > (iTotalCities+3)*2 ) break;
		iCount++;
		if( iCount > 8 ) break; // Only add-up the next eight tactical projects
	}
	CEOSAIMathFunction  TransportNeedFunc;
	TransportNeedFunc.SetInputOutput( 0.0f, 0.0f );
	TransportNeedFunc.SetInputOutput( 2.0f, 2.0f );
	TransportNeedFunc.SetInputOutput( 4.0f, 3.0f );
	TransportNeedFunc.SetInputOutput( 8.0f, 4.0f );
	TransportNeedFunc.SetInputOutput(12.0f, 5.0f );
	fAdditionalTransportNeed = TransportNeedFunc.GetOutput( fAdditionalTransportNeed );
	//if( fAdditionalTransportNeed > 4.0f ){ fAdditionalTransportNeed = 4.0f + sqrt( fAdditionalTransportNeed-4.0f ); }
	//if( fAdditionalTransportNeed > 6.0f ){ fAdditionalTransportNeed = 6.0f; }
	AIJobSolution.AddTransportNeed( fAdditionalTransportNeed );
	fSeaBasedMovementRatio01 = fRequiresSeaBasedMovement / ( fRequiresSeaBasedMovement + fRequiresNoSeaBasedMovement + 1.0f );

	/*
	pos = JobsToDo.m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CUnit* pEnemyUnit = JobsToDo.m_EnemyUnits.GetNext( pos );
		EnemyUnits.Add( pEnemyUnit->GetUnitTemplate(), pEnemyUnit->GetCurrentHP01() );
	}
	*/

	//
	// Add some weight from all units owned by other players (where feelings < 0.6)
	/*
	pos = pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pAIBrain->GetAIPoiObjectList()->GetNext( pos );
		float fFeelingsAboutPlayer01 = pGlobalForeignRelations->GetFeelings01( iAIPlayer, pAIPoiObject->GetOwner() );
		if( fFeelingsAboutPlayer01 < 0.7f )
		{
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
			if( pAIUnit )
			{
				float fCurrentHP01 = 1.0f;
				if( pAIUnit->GetServerUnit() ){ fCurrentHP01 = pAIUnit->GetServerUnit()->GetCurrentHP01(); }

				CEOSAIMathFunction  FeelingsMathFunc;
				FeelingsMathFunc.SetInputOutput( 0.0f, 0.15f );
				FeelingsMathFunc.SetInputOutput( 0.3f, 0.15f );
				FeelingsMathFunc.SetInputOutput( 0.6f, 0.10f );
				FeelingsMathFunc.SetInputOutput( 0.7f, 0.0f );
				EnemyUnits.Add( 
					pAIUnit->GetUnitTemplate(),
					FeelingsMathFunc.GetOutput( fFeelingsAboutPlayer01 ) * fCurrentHP01 );
			}
		}
	}
	*/

	// What is already in Production?  Compile a list.
	//   I should actually track how fast these things are being built.  
	//   A city with production 4 building a Battleship isn't worth much, and shouldn't really count.
	//CList< CBuildOrder* >  InProductionList;
	pos = pAIThoughtDatabase->GetMyCities()->GetHeadPosition();
	while( pos )
	{
		CEOSAICity* pAICity = pAIThoughtDatabase->GetMyCities()->GetNext( pos );

		long iBuildOrderCount = 0;
		//POSITION pos2 = pAICity->GetServerCity()->GetBuildOrders()->GetHeadPosition();
		POSITION pos2 = pAICity->GetAIBuildOrders()->GetHeadPosition();
		while( pos2 )
		{
			CEOSAIBuildOrder* pBuildOrder = pAICity->GetAIBuildOrders()->GetNext( pos2 );
			iBuildOrderCount++;
			ASSERT( iBuildOrderCount <= 2 );

			//InProductionList.AddTail( pBuildOrder );
			if( pBuildOrder->GetAIBuildOption()->GetAIUnitTemplate() )
			{
				AIJobCapability.Add( pBuildOrder->GetAIBuildOption()->GetAIUnitTemplate() );
				//MyUnits.Add( pBuildOrder->GetBuildOption()->GetUnitTemplate(), 1.0f );
			}
		}
	}

	// Now that I have an "In Production" list, I need to figure out what to build
	// Factors:
	//  (1) Low-Production Cities shouldn't build things that take a long time
	//  (2) Units far from the front (especially on remote islands) shouldn't produce GroundUnits
	//      (except for self-defense).  Instead, they should produce ships or (fast moving) aircraft.
	//  (3) Units on the front should produce GroundUnits (defense, offense, useful for transport pickup)
	//  (4) High Production-Cost units should be built further from the border (away from Ground/Air attacks)
	//  (5) Cities with partially-completed units should favor completing those units
	//  (6) Coastal Cities might need to favor sea-units (depending on many sea-units I want and where
	//      my cities are at).  This is because coastal cities are the only ones that can build sea units
	// I can use the Nationwide Pathway information to figure out where my fronts (and potential fronts) are
	// Go through and give each city a preference towards different types of units
	CEOSAIListSort< CEOSAICity* >  m_SortedCities;
	m_SortedCities.SetSortFunction( SortAICitiesBySortValue );
	pos = pAIThoughtDatabase->GetMyCities()->GetHeadPosition();
	while( pos )
	{
		CEOSAICity* pAICity = pAIThoughtDatabase->GetMyCities()->GetNext( pos );
		if( pAICity->GetTimeUntilCompletionOfBuildQueue() > 1.0f ) continue;

		//CMultiRegion* pMultiRegion = pWorldDescServer->GetAICommonData()->GetMultiRegionManager()->GetMultiRegion( pAICity->GetServerCity()->GetLocation() );
		CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( pAICity->GetLocation() );
		long iCityMultiRegion = pMultiRegion->GetId();

		// 1.0 = on the front
		float fFrontDanger01 = 0.0f;//pWorldDescServer->GetNationwidePathways();
		//long iNumberOfPlayers = g_pCommonState->GetWorldBuildDesc()->GetNumberOfPlayers();
		long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		//for( long iOtherPlayer=1; iOtherPlayer<pGlobalForeignRelations->m_iNumberOfPlayers+1; iOtherPlayer++ )
		for( long iOtherPlayer=1; iOtherPlayer<iNumberOfPlayers+1; iOtherPlayer++ )
		{
			if( iAIPlayer == iOtherPlayer ) continue;
			//CPlayer* pOtherPlayer = GetCommonState()->GetPlayer( iOtherPlayer );
			EOSAI::CGamePlayer* pOtherPlayer = g_pEOSAIInterface->GetGamePlayer( iOtherPlayer );
			if( pOtherPlayer->IsAlive() == false ) continue;

			float fFeelingsAboutPlayer01 = pGlobalForeignRelations->GetFeelings01( iAIPlayer, iOtherPlayer );
			if( fFeelingsAboutPlayer01 > 0.9f ) continue;

			CEOSAIMultiRegionNationwidePathways* pMultiRegionNationwidePathways =
				//pWorldDescServer->GetAICommonData()->GetNationwidePathways( iOtherPlayer );
				g_pEOSAICommonData->GetNationwidePathways( iOtherPlayer );
			if( pMultiRegionNationwidePathways == NULL ){ continue; }

			if( pMultiRegionNationwidePathways->GetPolitePathFromMyCitResUnitsToRestOfWorld()->GetArraySize() == 0 )
			{
				continue;
			}

			CEOSAIRegionPathwayItem* pRegionPathwayItem =
				g_pEOSAICommonData->GetNationwidePathways( iOtherPlayer )->GetDistGeoPathfindingValueFromMyCitResUnits( iCityMultiRegion );

			CEOSAIMathFunction  DistanceMathFunc;
			DistanceMathFunc.SetInputOutput( 200.0f, 1.0f );
			DistanceMathFunc.SetInputOutput( 600.0f, 0.0f );
			CEOSAIMathFunction  FeelingsMathFunc;
			FeelingsMathFunc.SetInputOutput( 0.0f, 1.0f );
			FeelingsMathFunc.SetInputOutput( 0.4f, 1.0f );
			FeelingsMathFunc.SetInputOutput( 0.9f, 0.0f );

			float fFrontDangerFromPlayer01 = 
				FeelingsMathFunc.GetOutput( fFeelingsAboutPlayer01 ) *
				DistanceMathFunc.GetOutput( pRegionPathwayItem->m_fDistance );
			fFrontDanger01 = max( fFrontDanger01, fFrontDangerFromPlayer01 );
		}

		float fSortValue = 0.0f;

		// PreferenceForLowCostUnits is based on:
		//   City Production (low production = favor cheap units)
		//   Distance from Front (close to front = favor cheap units)
		CEOSAIMathFunction  LowCostUnitPreference;
		LowCostUnitPreference.SetInputOutput( 5.0f,  1.0f );
		LowCostUnitPreference.SetInputOutput( 10.0f, 0.0f );
		float fProductionBasedLowCostUnitPreference = LowCostUnitPreference.GetOutput( pAICity->GetTotalProduction() );
		pAICity->SetPreferenceForLowCostUnits01( 
			0.4f * fFrontDanger01 +
			0.4f * fProductionBasedLowCostUnitPreference +
			0.2f * max( fFrontDanger01, fProductionBasedLowCostUnitPreference ) );

		// Higher SortValue because this city is more constrained
		//fSortValue += pAICity->GetPreferenceForLowCostUnits01();
		fSortValue += 0.3f * fFrontDanger01;

		// Grab HighProduction cities first (the most-needed units should be built faster)
		fSortValue += 0.1f * pAICity->GetTotalProduction();
		fSortValue += 0.01f * (rand()%100); // DEBUG
		fSortValue += 2.0f * pAICity->GetPartiallyCompletedItems01(); // favor units with some build completion

		// PreferenceForGroundUnits is based on:
		//   Distance from Front (close to front = favor GroundUnits, far from front = avoid GroundUnits)
		pAICity->SetPreferenceForGroundUnits01( fFrontDanger01 );
		pAICity->SetPreferenceForAirUnits01( 0.5f );
		pAICity->SetPreferenceForSeaUnits01( 0.5f );
		long iAdjacentWaterSize = 0;
		//

		long iGeo = g_pEOSAIInterface->GetAICommonData()->GetGeoId( CEOSAILocation( CEOSAILocation::Pixel,10,10 ) );
		int g=0;

		CEOSAILongSet WaterGeos;
/*
		//pAICity->GetServerCity()->GetAdjacentWaterGeos( &WaterGeos );
		No AdjacentWaterGeos are being returned

		The original code uses this. I have no WaterGeo sets, though.
void CCity::GetAdjacentWaterGeos( CLongSet* pGeoSet )
{
	pGeoSet->Clear();
	m_pWorldDesc->GetWaterGeoSet( GetLocation(), 5,3, pGeoSet );
}
*/
		pAICity->GetAdjacentWaterGeos( &WaterGeos );
		ASSERT( WaterGeos.GetCount() > 0 ); // temp - some cities have no water geos
		POSITION pos = WaterGeos.GetList()->GetHeadPosition();
		while( pos )
		{
			long iWaterGeo = WaterGeos.GetList()->GetNext( pos );
			//CGeo* pGeo = pAICity->GetServerCity()->GetWorldDesc()->GetGeo( iWaterGeo );
			CEOSAIGeo* pGeo = g_pEOSAICommonData->GetAIGeo( iWaterGeo );
			ASSERT( pGeo );
			if( pGeo )
			{
				ASSERT( pGeo->GetSize() > 0 );
				iAdjacentWaterSize += pGeo->GetSize();
			}
		}
		if( iAdjacentWaterSize < 6000 ) // 310x310 pixels  //pAICity->GetServerCity()->IsOnAMajorBodyOfWater() == false )
		{
			fSortValue += 1.0f;  // Higher SortValue because this city is more constrained
			pAICity->SetPreferenceForSeaUnits01( 0.0f );
		}
		pAICity->SetSizeOfAdjacentWater( iAdjacentWaterSize );

		// So that cities will preserve their sort value
		//fSortValue += (pAICity->GetServerCity()->GetObjectId()/100000.0f);
		fSortValue += (pAICity->GetObjectId()/100000.0f);

		/*
		say I need an Infantry (badly) and a Transport
		I have a coastal city and a landlocked city
		If I calculate the coastal city first, it will build the infantry and the landlocked can't
			build a transport
		I could calculate the more constrained city first (landlocked cities are more constrained)
		Or maybe I could rearrange build orders between cities
		*/

		// Determines which cities are given orders first
		pAICity->SetSortValue( fSortValue );
		m_SortedCities.InsertSorted( pAICity );
	}

	// Normalize the values?

	// Build Options?
	//CreateEOSAIBuildOptions();

	// Setup Build Orders - Cities with high SortValue are first
	CList< CEOSAIBuildOption* >  MyBuildOptions;
	//CList< CEOSAIBuildOption* >* pBuildOptionsList = GetCommonState()->GetBuildOptionList();
	ASSERT( g_pEOSAICommonData->GetAIBuildOptionList()->GetCount() > 0 );
	//ASSERT( this->GetAIBuildOptionList()->GetCount() > 0 );
	CList< CEOSAIBuildOption* >* pBuildOptionsList = g_pEOSAICommonData->GetAIBuildOptionList();
	//CList< CEOSAIBuildOption* >* pBuildOptionsList = this->GetAIBuildOptionList();
	pos = pBuildOptionsList->GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildOption* pAIBuildOption = pBuildOptionsList->GetNext( pos );
		//if( g_pEOSAICommonData->GetAIGameRules()->CanBuild( iAIPlayer, pAIBuildOption, false ) )
		if( g_pEOSAIInterface->GetAIGameRules()->CanBuild( iAIPlayer, pAIBuildOption, false ) )
		{
			MyBuildOptions.AddTail( pAIBuildOption );
		}
	}

	float fMinimumSeaUnitPrice = 100000.0f;
	float fMaximumSeaUnitPrice = 0.0f;
	pos = MyBuildOptions.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildOption* pAIBuildOption = MyBuildOptions.GetNext( pos );
		if( pAIBuildOption->GetAIUnitTemplate() )
		{
			if( pAIBuildOption->GetAIUnitTemplate()->IsSeaUnit() )
			{
				fMaximumSeaUnitPrice = max( fMaximumSeaUnitPrice, pAIBuildOption->GetAIUnitTemplate()->GetProductionAndIronCost1() );
				fMinimumSeaUnitPrice = min( fMinimumSeaUnitPrice, pAIBuildOption->GetAIUnitTemplate()->GetProductionAndIronCost1() );
			}
		}
	}
	fMaximumSeaUnitPrice = max( fMaximumSeaUnitPrice, fMinimumSeaUnitPrice+0.1f );

	// Calculate which sea-units are harder to see
	// I should move this into the AICombatCapabilities structure
	float fCummulativeViewRange = 0.0f;
	long iCummulativeViewRangeCount = 0;
	pos = MyBuildOptions.GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildOption* pAIBuildOption = MyBuildOptions.GetNext( pos );
		if( pAIBuildOption->GetAIUnitTemplate() &&
			pAIBuildOption->GetAIUnitTemplate()->IsSeaUnit() )
		{
			float fAverageVisibleRange = pAIBuildOption->GetAIUnitTemplate()->GetAIUnitCombatCapability()->GetAverageVisibleRange();

			fCummulativeViewRange += pAIBuildOption->GetAIUnitTemplate()->GetAIUnitCombatCapability()->GetAverageVisibleRange();
			iCummulativeViewRangeCount++;
			/*
			float fCummulativeViewRange2 = 0.0f;
			long iCummulativeViewRangeCount2 = 0;
			CUnitTemplate* pSeaUnit = pBuildOption->GetUnitTemplate();
			POSITION pos2 = MyBuildOptions.GetHeadPosition();
			while( pos )
			{
				CBuildOption* pBuildOption = MyBuildOptions.GetNext( pos );
				if( pBuildOption->GetUnitTemplate() )
				{
					CUnitTemplate* pViewer = pBuildOption->GetUnitTemplate();
					float fViewRange = pViewer->GetViewRange( pSeaUnit );
					fCummulativeViewRange2 += fViewRange;
					iCummulativeViewRangeCount2++;
				}
			}
			if( iCummulativeViewRangeCount2 > 0 )
			{
				float fViewRange = fCummulativeViewRange2 / iCummulativeViewRangeCount2;
				fCummulativeViewRange1 += fViewRange;
				iCummulativeViewRangeCount1++;
			}
			*/
		}
	}
	float fAverageSeaUnitIsVisibleRange = 1.0f;
	if( iCummulativeViewRangeCount > 0 ){ fAverageSeaUnitIsVisibleRange = fCummulativeViewRange / iCummulativeViewRangeCount; }
/*
this is actually a bad way to handle things
  Right now, if I need an oil refinery, the first city with a "this is my best build option" will
  build it.  This is a problem because there might be a better city for building it.
  Probably what I should do is calculate the best build option for each city, then find the highest-rated
  build option/city combo, and build it.  Then I iterate over all my cities again, finding the best
  option again.

  Also, maybe I should adjust the MyUnits values so that Low-Production cities count for less.
*/
	CEOSAIStrategicAI* pStrategicAI = &m_pAIPlayer->m_StrategicAI;
	AIJobSolution.MultiplyExpandJobSignificance( 1.0f + 0.8f*pStrategicAI->GetStrategy_CaptureUnownedCitRes01() );
	AIJobSolution.MultiplyConquestJobSignificance( 0.3f + 1.3f*pStrategicAI->GetStrategy_CombatOrPlanForCombat01() );
	//	float GetStrategy_CaptureUnownedCitRes01(){ return m_fStrategy_CaptureUnownedCitRes01; }
	//	float GetStrategy_CombatOrPlanForCombat01(){ return m_fStrategy_CombatOrPlanForCombat01; }

	CStringList Debug_AllBuildItems;
//	CWorldBuildDesc* pWorldBuildDesc = m_pAIPlayer->GetWorldDescServer()->GetWorldBuildDesc();

	m_pAIPlayer->CalculateExperiencingResourceShortage01();

	float fSortValue = 100000.0f;
	pos = m_SortedCities.GetHeadPosition();
	while( pos )
	{
		CEOSAICity* pAICity = m_SortedCities.GetNext( pos );

		ASSERT( pAICity->GetSortValue() <= fSortValue );
		fSortValue = pAICity->GetSortValue();

		// Skip build orders if the city has orders already
		//if( pAICity->GetCity()->GetTimeUntilCompletionOfBuildQueue() > 5.0f ) continue;
		//if( pAICity->GetServerCity()->GetBuildOrders()->IsEmpty() == FALSE ) continue;
		if( pAICity->GetAIBuildOrders()->IsEmpty() == FALSE ) continue;

		// What is my current damage to enemy units?
		/*
		float fCurrentDamageToEnemyProduction = 0.0f;
		float fCurrentDamageToEnemyCombatCapability = 0.0f;
		CEOSAIUnit2TemplatesAndFloat  CurrentCombatResults;
		CEOSAIStopwatch2  Stopwatch;
		Stopwatch.Start();
		//CEOSAIQuickCombatCalculation::QuickCombatResults( MyUnits, EnemyUnits, CurrentCombatResults );
		//CEOSAIQuickCombatCalculation::QuickCombatDamageSqrt( EnemyUnits, CurrentCombatResults, &fCurrentDamageToEnemyProduction, &fCurrentDamageToEnemyCombatCapability );
		Stopwatch.Stop();
		double d = Stopwatch.GetDeltaMilliseconds();
		*/

		float fMyTotalUnitProductionValue = 0.0f;
		POSITION pos2 = AIJobCapability.m_UnitTemplatesAndFloat.m_List.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnit2TemplateAndFloat* pUnitTemplateAndFloat = AIJobCapability.m_UnitTemplatesAndFloat.m_List.GetNext( pos2 );
			fMyTotalUnitProductionValue += 
				pUnitTemplateAndFloat->m_pAIUnitTemplate->GetProductionCost() *
				pUnitTemplateAndFloat->m_fValue;
		}
		/*
		POSITION pos2 = AIJobCapability.m_UnitTemplatesForCombat.GetHeadPosition();
		while( pos2 )
		{
			CUnitTemplate* pUnitTemplate = AIJobCapability.m_UnitTemplatesForCombat.GetNext( pos2 );
			iMaxUnitProductionValue += pUnitTemplate->GetProductionCost();
		}
		*/

		#ifdef DEBUG
		CEOSAIListSort< EOSAI::StringAndFloat* >  SortedBuildOptions;
		SortedBuildOptions.SetSortFunction( SortByFloat );
		#endif DEBUG

		if( pAICity->GetObjectId() == 13 )
		{
			int g=0;
		}

		// Find the Jobs in the immediate area, add it to the JobsToDo
		CEOSAIJobsToDo       LocalAIJobsToDo;
		CEOSAIJobCapability  LocalAIJobCapability;
		CEOSAIJobSolution  LocalAIJobSolution( &LocalAIJobsToDo, &LocalAIJobCapability );
		CEOSAIThoughtDatabase* pAIThoughtDatabase = m_pAIPlayer->GetAIBrain()->GetAIThoughtDatabase();
		POSITION pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
			if( pAIUnit )
			{
				float fDistance = g_pWorldDistanceTool->GetDistance( pAIUnit->GetInitialState()->GetLocation(), pAICity->GetInitialState()->GetLocation() );
				if( fDistance < 100.0f )
				{
					LocalAIJobCapability.Add( pAIUnit->GetAIUnitTemplate() );
				}
			}
		}
		float fLocalDesireWeight = 0.0f;
		pos = pAIThoughtDatabase->GetDesires()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesire2* pDesire = pAIThoughtDatabase->GetDesires()->GetNext( pos );
			CEOSAIDesireSpatial* pDesireSpatial = dynamic_cast< CEOSAIDesireSpatial* >( pDesire );
			if( pDesireSpatial && 
				pDesireSpatial->CurrentForeignRelationsPreventsPersuingThisDesire() == false )
			{
				//float fDistance = pWorldBuildDesc->GetPixelDistance( pDesireSpatial->GetLocation(), pAICity->GetInitialState()->GetLocation() );
				float fDistance = g_pWorldDistanceTool->GetDistance( pDesireSpatial->GetLocation(), pAICity->GetInitialState()->GetLocation() );
				if( fDistance < 200.0f )
				{
					LocalAIJobsToDo.Add( pDesire->GetJobsToDo() );
					fLocalDesireWeight += 0.2f;
					if( fLocalDesireWeight > 1.0f ){ fLocalDesireWeight = 1.0f; }
				}
			}
		}

		// Debug
		//EOSAI::StringAndFloatSet Debug_BuildOptionsValue;

		// What should I Build?
		//   Compare the JobsToDo with the Capabilities
		//CEOSAIMathFloat01  Value01;
		float fBestBuildOptionValue = 0.0f;
		CEOSAIBuildOption* pBestBuildOption = NULL;
		CStringList TextualReasonsForFailure;
		//bool bRequiresBetterTechnology;
		ASSERT( MyBuildOptions.GetCount() > 0 );
		pos2 = MyBuildOptions.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIBuildOption* pAIBuildOption = MyBuildOptions.GetNext( pos2 );
			//if( pAICity->GetServerCity()->CanBuild( pBuildOption, CCity::eGoal_AddAnotherBuildItem, false ) == false ) continue;
			if( pAICity->CanBuild( pAIBuildOption, CEOSAICity::eGoal_AddAnotherBuildItem, false ) == false ) continue;
			//if( pAICity->GetServerCity()->CanBuild( pBuildOption, false, &TextualReasonsForFailure, &bRequiresBetterTechnology ) == false ) continue;
			float fValue = 0.0f;

			if( pAIBuildOption->IsMoney() )
			{
				float fResShortage01 = m_pAIPlayer->GetExperiencingResourceShortage01();
				fValue = 3.0f*fResShortage01;
			}
			if( pAIBuildOption->IsABuilding() )
			{
				// How valuable is the building?  How long until I get 'paid back'?
				// Am I in danger of bombers?  Have my enemies discovered this city?
				// Has this city been bombed in the past?  Have my cities been getting bombed?
				// Is this city in danger of being invaded?  Is it on the front?
				// If I'm overwhelmed with unit-maintenance costs, buildings are a better investment
				CEOSAIBuildingDescription* pBuildingDesc = pAIBuildOption->GetBuildingDescription();
				fValue = pAICity->GetBuildingValue( pBuildingDesc );
				fValue *= 1.0f + ( 0.5f*m_pAIPlayer->GetPersonality_Builder01() );
			}
			if( pAIBuildOption->IsAUnit() )
			{
				CEOSAIUnitTemplate* pAIUnitTemplate = pAIBuildOption->GetAIUnitTemplate();

				#ifdef _DEBUG
				if( pAIUnitTemplate->IsTransport() )
				{
					int h=0;
				}
				#endif

			//
			// Increase value if this BuildOption is already partially-completed
			//
				float fPercentCompletion = 0.0f;
				CEOSAIBuildCompletion* pAIBuildCompletion = pAICity->GetPartiallyCompletedItem( pAIBuildOption );
				if( pAIBuildCompletion )
				{
					fPercentCompletion = pAIBuildCompletion->GetPercentComplete();
				}
				fValue = 0.0f;
				if( fPercentCompletion > 0.0f )
				{
					fValue = 4.0f * fPercentCompletion;
					if( fPercentCompletion > 0.5f ){ fValue += 1.0f; }
				}

				if( pAIUnitTemplate->IsGroundUnit() &&
					pAICity->GetNumberOfGroundUnitsInside() >= 5 )
				{
					fValue -= 2.0f;
				}

			//
			// Add value for diversity, reduce value if I'm producing another unit of the same type.
			//    (The value is adjusted by the AI's desire for diversity.)
			//
				// Calculate what percentage:
				//   AIJobCapability.ProductionCost( this unit ) / AIJobCapability.ProductionCost( all my units )
				float fCurrentUnitProductionValue = 0.0f;
				/*
				POSITION pos3 = AIJobCapability.m_UnitTemplatesForCombat.GetHeadPosition();
				while( pos3 )
				{
					CUnitTemplate* pUnitTemplateInList = AIJobCapability.m_UnitTemplatesForCombat.GetNext( pos3 );
					if( pUnitTemplateInList == pBuildOption->GetUnitTemplate() )
					{
						iCurrentUnitProductionValue += pUnitTemplateInList->GetProductionCost();
					}
				}
				*/
				float fCount = AIJobCapability.m_UnitTemplatesAndFloat.GetValue( pAIUnitTemplate );
				fCurrentUnitProductionValue += fCount * pAIUnitTemplate->GetProductionCost();

				float fPercentage01 = (float) fCurrentUnitProductionValue / (float) (300.0f+fMyTotalUnitProductionValue);
				CEOSAIMathFunction DiversityValueMathFunc;
				DiversityValueMathFunc.SetInputOutput( 0.0f, 1.0f );
				DiversityValueMathFunc.SetInputOutput( 0.6f,-2.0f );
				DiversityValueMathFunc.SetInputOutput( 1.0f,-3.0f );
				fValue += DiversityValueMathFunc.GetOutput( fPercentage01 ) * pAIPlayer->GetPersonality_ProUnitDiversity01();

			//
			// Add value depending on the unit's value to the AIJobSolution
			//
				// Add a slight bias against more expensive units
				float fModUnitProduction = (pAIUnitTemplate->GetProductionCost() + 300) / 300.0f;

				// What's the value of this UnitTemplate (compared to the CurrentCapabilities)
				float fNationalValue = AIJobSolution.GetValueOfAddingToCapability( pAIUnitTemplate ) / fModUnitProduction;
				fValue += fNationalValue;// / AIJobSolution.GetCurrentCapabilityNeed();

				if( fLocalDesireWeight > 0.0f )
				{
					float fLocalValue = LocalAIJobSolution.GetValueOfAddingToCapability( pAIUnitTemplate ) / fModUnitProduction;
					fValue += fLocalDesireWeight * fLocalValue;
				}

			//
			// Adjust value depending on the city's preference for:
			//     Ground/Air/Sea units
			//     Low cost units
			//
				if( pAIUnitTemplate->IsGroundUnit() )
				{
					if( pAICity->GetPreferenceForGroundUnits01() > 0.0f ) 
					{
						fValue += pAICity->GetPreferenceForGroundUnits01();
					}
				}
				if( pAIUnitTemplate->IsAirUnit() )
				{
					if( pAICity->GetPreferenceForAirUnits01() > 0.0f )
					{
						fValue += pAICity->GetPreferenceForAirUnits01();
					}
				}
				if( pAIUnitTemplate->IsSeaUnit() )
				{
					if( pAICity->GetPreferenceForSeaUnits01() > 0.0f )
					{
						fValue += pAICity->GetPreferenceForSeaUnits01();
					}
				}

				//
				if( pAICity->GetPreferenceForLowCostUnits01() > 0.0f )
				{
					// Favor units that don't take much time to build
					float fTime = pAIUnitTemplate->GetProductionAndIronCost1() / pAICity->GetTotalProduction();

					// I don't use negative values because I don't want fValue to ever be negative
					CEOSAIMathFunction LowCostFunc;
					LowCostFunc.SetInputOutput(  0.0f,  2.0f );
					LowCostFunc.SetInputOutput(  8.0f,  2.0f );
					LowCostFunc.SetInputOutput( 20.0f,  0.0f );
					LowCostFunc.SetInputOutput( 80.0f, -1.0f );
					float fCityPreferenceForLowCostUnits01 = pAICity->GetPreferenceForLowCostUnits01();
					float fLowCostFunc = LowCostFunc.GetOutput( fTime );
					//fValue += pAICity->GetPreferenceForLowCostUnits01() * LowCostFunc.GetOutput( fTime );
					fValue += fCityPreferenceForLowCostUnits01 * fLowCostFunc;
				}

			//
			// Adjust value depending on the AI's preference for:
			//    Pro-Aircraft, Pro-Missile, 
			//    Pro-GroundSea units, Pro-artillery, 
			//    Pro-Big/Little ships, Pro-submarine,
			//     
				if( pAIUnitTemplate->IsAirUnit() )
				{
					//fValue *= ( 1.0f + 0.5f*pAIPlayer->GetPersonality_ProAircraftPos1_ProGroundSeaNeg1() );
					fValue *= ( 1.0f + 0.5f*pAIPlayer->GetPersonality_ProAirUnit11() );
					if( pAIUnitTemplate->SelfDestructsOnAttack() )
					{
						fValue *= 1.0f + 0.5f*pAIPlayer->GetPersonality_ProMissile01();
					}
					//if( pUnitTemplate->GetUnitCombatCapability()->
					//GetPersonality_ProStrategicBombing01()
				}
				eif( pAIUnitTemplate->IsGroundUnit() )
				{
					fValue *= ( 1.0f + 0.5f*pAIPlayer->GetPersonality_ProGroundUnit11() );
				}
				else
				{
					ASSERT( pAIUnitTemplate->IsSeaUnit() );
					fValue *= ( 1.0f + 0.5f*pAIPlayer->GetPersonality_ProSeaUnit11() );
				}
				/*
				else
				{
					fValue *= ( 1.0f - 0.5f*pAIPlayer->GetPersonality_ProAircraftPos1_ProGroundSeaNeg1() );
					//fValue *= ( 1.5f - pAIPlayer->GetPersonality_ProAircraft01() );
				}
				*/
				//
				if( pAIUnitTemplate->IsSeaUnit() )
				{
					float fThisIsABigShip01 = 
						( pAIUnitTemplate->GetProductionAndIronCost1() - fMinimumSeaUnitPrice ) /
						( fMaximumSeaUnitPrice - fMinimumSeaUnitPrice );
					float fThisIsABigShipPos1_LittleShipNeg1 = ( 2.0f * fThisIsABigShip01 ) - 1.0f;
					float fPersonalityProExpensiveShipsPos1CheapShipsNeg1 = pAIPlayer->GetPersonality_ProExpensiveShipsPos1_CheapShipsNeg1();
					fValue *= 1.0f + 0.5f*fThisIsABigShipPos1_LittleShipNeg1*pAIPlayer->GetPersonality_ProExpensiveShipsPos1_CheapShipsNeg1();

					// Submarine?  (I'm looking for stealth here)
					float fIsVisibleRange = pAIUnitTemplate->GetAIUnitCombatCapability()->GetAverageVisibleRange();
					if( 0.75f*fAverageSeaUnitIsVisibleRange > fIsVisibleRange &&
						pAIPlayer->GetPersonality_ProSubmarine01() > 0.0f )
					{
						fValue *= 
							1.0f + 
							//0.5f*pAIPlayer->GetPersonality_ProSubmarine01() +
							(1.0f-(fIsVisibleRange/fAverageSeaUnitIsVisibleRange))*0.8f*pAIPlayer->GetPersonality_ProSubmarine01();
					}
				}
				//
				if( pAIUnitTemplate->IsGroundUnit() )
				{
					if( pAIUnitTemplate->GetAttackRangeMaximum() > 10.0f )
					{
						fValue *= 1.0f + pAIPlayer->GetPersonality_ProArtillery01();
					}
				}
			//
			// Never build ships in cities on small water
			//
				if( pAIUnitTemplate->IsSeaUnit() && pAICity->GetSizeOfAdjacentWater() < 6000 ) // 310x310 pixels
				{
					fValue = 0.0f;
				}
				if( pAICity->GetObjectId() == 12 )
				{
					int g=0;
				}
			}
			//Debug_BuildOptionsValue.Add( pAIBuildOption->GetInternalName(), fValue );
			if( fValue > fBestBuildOptionValue )
			{
				fBestBuildOptionValue = fValue;
				pBestBuildOption = pAIBuildOption;
			}

			// DEBUG
			#ifdef _DEBUG
			EOSAI::StringAndFloat* pBuildAndFloat = new EOSAI::StringAndFloat( pAIBuildOption->GetInternalName(), fValue );
			SortedBuildOptions.InsertSorted( pBuildAndFloat );
			#endif DEBUG
			//*/
		}

		#ifdef DEBUG
		CString strDebugBuildOptions;
		pos = SortedBuildOptions.GetHeadPosition();
		while( pos )
		{
			EOSAI::StringAndFloat* pBuildAndFloat = SortedBuildOptions.GetNext( pos );
			CString str;
			str.Format( _T("%s=%1.2f, \n"), pBuildAndFloat->m_strValue, pBuildAndFloat->m_fValue );
			strDebugBuildOptions += str;
		}
/*
		Why are no buildings showing up in by build list?
		Why am I coming up with "money" as the highest-ranking build item?  
			In the regular AI, money was 0.0.
			I probably need to set the current money, oil, food values
		The regular AI's values:
			Artillery (Class 1) = 4.25,
			Shipyard = 3.58,
			Infantry (Class 1) = 3.51,
			Barracks = 3.46,
			Destroyer (Class 1) = 2.22,
			Capitol = 2.16,
			Transport (Class 1) = 1.93,
			Zeppelin = 0.57,
			<Money> = 0.00,
*/
		int g=0;
		while( SortedBuildOptions.IsEmpty() == FALSE ){ delete SortedBuildOptions.RemoveHead(); }
		// Personality information
		float fBuilder11 = pAIPlayer->GetPersonality_Builder01();
		if( pAICity->GetObjectId() == 13 )
		{
			int g=0;
		}
		#endif DEBUG

		ASSERT( pBestBuildOption );
		if( pBestBuildOption )
		{
			Debug_AllBuildItems.AddTail( pBestBuildOption->GetInternalName() );

			pAICity->ActionScheduler_CreateBuildOrder( pBestBuildOption );
			if( pBestBuildOption->GetAIUnitTemplate() )
			{
				CEOSAIUnitTemplate* pBestAIUnitTemplate = pBestBuildOption->GetAIUnitTemplate();
				AIJobCapability.Add( pBestAIUnitTemplate );
			}
			m_pAIPlayer->CalculateExperiencingResourceShortage01();
			//m_pAIPlayer->AddedBuildItemToResourceConsumption( pBestBuildOption );
		}
	}

	EOSAI::StringAndLongSet  AllBuildItems;
	pos = Debug_AllBuildItems.GetHeadPosition();
	while( pos )
	{
		CString strBuild = Debug_AllBuildItems.GetNext( pos );
		AllBuildItems.Add( strBuild, +1 );
	}
	int j=0;
}



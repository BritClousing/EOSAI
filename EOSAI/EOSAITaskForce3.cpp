
#include "stdafx.h"
#include "EOSAITaskForce3.h"
#include "EOSAIJobCapability.h"
//#include "UnitTemplate.h"
#include "EOSAIJobsToDo.h"
#include "PoiObject.h"
#include "EOSAIUnit2.h"
#include "City.h"
//#include "AITacticalProject.h"
#include "EOSAITacticalProject2.h"
#include "EOSAIUnitActionStack.h"
//#include "AICityActionIdeaNode.h"
#include "City.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAIQuickCombatCalculation.h"
#include "EOSAIUnit2.h"
//#include "WorldDescPlayer.h"
#include "EOSAIUnitAction.h"
#include "EOSAIUnitActionIdea.h"
//#include "WorldDescServer.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAICommonData2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
/*
bool SortByTimeToTarget( CEOSAIUnit2ActionIdea* pUnitActionIdea1, CEOSAIUnit2ActionIdea* pUnitActionIdea2 )
{
	if( pUnitActionIdea1->GetBuildAndTransportAndMoveToTargetTime() > pUnitActionIdea2->GetBuildAndTransportAndMoveToTargetTime() ){ return true; }
	else if( pUnitActionIdea1->GetBuildAndTransportAndMoveToTargetTime() < pUnitActionIdea2->GetBuildAndTransportAndMoveToTargetTime() ){ return false; }
	return true;  // always return true when values are equal
}
*/

CEOSAITaskForce3::CEOSAITaskForce3()
{
	m_bIsBeingDeleted = false;

	m_pTacticalProject2 = NULL;
	m_pTacticalProject2Strategy = NULL;

	// Limits on Units that I can use
	m_bUseOnlyAirUnits = false;  // Air-strikes, Air SearchAndDestroy
	m_bUseAirTransports = false; // AirTransport strategy

	m_eBestCaseOrRealTaskForce = EnumBestCaseOrRealTaskForce_Undefined;
	for( long iPlayer=0; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS+1; iPlayer++ )
	{
		m_PlayersToTreatAsEnemies[iPlayer] = false;
	}
	m_iNumberOfPlayer0Units = 0; // Calculated
	m_iNumberOfPlayerNUnits = 0; // Calculated

	m_UnitActionStack.m_pParentTaskForce = this;

	// Time-Value function
	//   TEMPORARY - These values should be customizable
	//   In some cases, I need to do things quickly.
	//   In other cases, I can plan long-term.
	m_TimeValueFunction.Clear();
	m_TimeValueFunction.SetInputOutput(  0.0f, 1.00f );
	m_TimeValueFunction.SetInputOutput(  5.0f, 0.40f );
	m_TimeValueFunction.SetInputOutput( 10.0f, 0.15f );
	m_TimeValueFunction.SetInputOutput( 20.0f, 0.10f );
	m_TimeValueFunction.SetInputOutput( 50.0f, 0.05f );
	m_TimeValueFunction.SetInputOutput(100.0f, 0.00f );
	//

	m_FulfillmentToScoreFunc.SetInputOutput( 0.0f, 0.0f );
	m_FulfillmentToScoreFunc.SetInputOutput( 1.0f, 1.0f );
	m_FulfillmentToScoreFunc.SetInputOutput( 1.3f, 1.2f );
	m_FulfillmentToScoreFunc.SetInputOutput( 1.6f, 1.3f );

	m_bNeedToUpdateScores = false;

	m_fCompletionTime = 1000000.0f;

	m_fGrowthScore = 0.0f;
	m_fRealScore = 0.0f;
	m_fBestCaseRealScore = 0.0f;

	m_CombatCalculation.SetProDefenderBias( 1.4f );

	m_CombatResultsOfAllocatedUnits.Clear();
	m_fMinimumNonCombatFulfillment02 = 0.0f;
	m_fAverageNonCombatFulfillment01 = 0.0f;

	m_fPrevGrowthScore = 0.0f; // Gives points for half-fulfilled tasks
	m_fPrevRealScore = 0.0f;   // Gives no points for half-fulfilled tasks
}

CEOSAITaskForce3::~CEOSAITaskForce3()
{
	m_bIsBeingDeleted = true;
	//ReleaseStackAllocations();
	//while( m_TimeAndRealScores.IsEmpty() == FALSE ){ delete m_TimeAndRealScores.RemoveHead(); }

	//m_NeedsTransportList.RemoveAll();
	//while( m_UnitActionIdeas.IsEmpty() == FALSE ){ delete m_UnitActionIdeas.RemoveHead(); }
	//while( m_CityActionIdeas.IsEmpty() == FALSE ){ delete m_CityActionIdeas.RemoveHead(); }

	/*
	POSITION pos = m_CityActionIdeas.GetHeadPosition();
	while( pos )
	{
		CEOSAICityActionIdea* pIdea = m_CityActionIdeas.GetNext( pos );
		pIdea->ReleaseAllocations();
		pIdea->DeleteHypotheticalUnits();
	}
	*/
}

void CEOSAITaskForce3::SetBestCaseOrRealTaskForce( EnumBestCaseOrRealTaskForce eTaskForceType )
{
	// Can't change the TaskForceType after it is set
	ASSERT( m_eBestCaseOrRealTaskForce == EnumBestCaseOrRealTaskForce_Undefined );
	//if( m_eBestCaseOrRealTaskForce == EnumBestCaseOrRealTaskForce_Undefined ){ m_eBestCaseOrRealTaskForce = eTaskForceType; }
	m_eBestCaseOrRealTaskForce = eTaskForceType;
}

// Information
CEOSAIBrain* CEOSAITaskForce3::GetAIBrain()
{
	if( m_pTacticalProject2 ){ return m_pTacticalProject2->GetAIBrain(); }
	ASSERT( false );
	return NULL;
}

float CEOSAITaskForce3::GetEstimatedNumberOfUnitsNeeded()
{
	return m_JobsToDo.GetEstimatedNumberOfUnitsNeeded();
}

void CEOSAITaskForce3::CopyUnitActionIdeasIntoStack()
{
	ASSERT( m_UnitActionStack.m_ItemsSortedByArrivalTime.IsEmpty() );
	if( m_UnitActionStack.m_ItemsSortedByArrivalTime.IsEmpty() )
	{
		CList< CEOSAIUnit2ActionIdea* >* pUnitActionIdeas;
		if( m_pTacticalProject2 ){ pUnitActionIdeas = m_pTacticalProject2->GetUnitActionIdeas(); }

		// Create the UnitActionStack from UnitActionIdeas
		m_UnitActionStack.Delete();
		POSITION pos = pUnitActionIdeas->GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit2ActionIdea* pUnitActionIdea = pUnitActionIdeas->GetNext( pos );

			if( pUnitActionIdea->GetAIUnitActor() &&
				pUnitActionIdea->GetAIUnitActor()->GetNeedForRepair01BasedOnDistanceAndDamage() >= 1.0f )
			{
				// Don't use units that have a high desire for repair
				continue;
			}

			m_UnitActionStack.Insert( pUnitActionIdea );
			//CEOSAIUnit2ActionStackItem* pItem = m_UnitActionStack.Insert( pUnitActionIdea );

			// If a unit is already inside a transport, then mark the space as 'allocated'
			/*
			if( pUnitActionIdea->GetAIUnitActor() &&
				pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer() )
			{
				CEOSAIUnit2* pAITransportUnit = pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer();
				pItem->SetSuggestedTransportMoveDesc( pAITransportUnit );
				pItem->AllocateSuggestedTransportMoveDesc();
				pItem->AllocateSuggestedTransportSpace();
			}
			*/
			/*
			if( pUnitActionIdea->GetAIUnitActor() &&
				pUnitActionIdea->GetAIUnitActor()->GetUnit() )
			{
				if( pUnitActionIdea->GetAIUnitActor()->GetUnit()->GetContainerUnit() )
				{
					long iTransportObjectId = pUnitActionIdea->GetAIUnitActor()->GetUnit()->GetContainerUnit()->GetObjectId();
					CEOSAIUnit2* pAITransportUnit = GetAIBrain()->GetAIUnit( iTransportObjectId );
					ASSERT( pAITransportUnit );

					pItem->SetSuggestedTransportMoveDesc( pAITransportUnit );
					pItem->AllocateSuggestedTransportMoveDesc();
					pItem->AllocateSuggestedTransportSpace();
				}
			}
			*/
		}
	}
}

long CEOSAITaskForce3::GetNumberOfAllocatedUnitsAndCities()
{
	CList< CEOSAIPoiObject* >  Transports;

	long iNumberOfAllocatedUnitsAndCities = 0;
	POSITION pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			iNumberOfAllocatedUnitsAndCities++;

			// Add each transport as an actor
			if( pStackItem->GetSuggestedTransportMoveDesc() )
			{
				bool bExistsInList = false;
				POSITION pos2 = Transports.GetHeadPosition();
				while( pos2 )
				{
					CEOSAIPoiObject* pAIPoiObjectInList = Transports.GetNext( pos2 );
					if( pAIPoiObjectInList == pStackItem->GetSuggestedTransportMoveDesc()->GetAIPoiActor() )
					{
						bExistsInList = true;
					}
				}
				if( bExistsInList == false )
				{
					Transports.AddTail( pStackItem->GetSuggestedTransportMoveDesc()->GetAIPoiActor() );
					iNumberOfAllocatedUnitsAndCities++;
				}
			}
		}
	}
	return iNumberOfAllocatedUnitsAndCities;
}

void CEOSAITaskForce3::ConstructTaskForce()
{
	CEOSAIStopwatch2 watch0;
	watch0.Start();

	ReleaseStackAllocations();

	ASSERT( m_pTacticalProject2Strategy );
	if( m_pTacticalProject2Strategy ){ m_pTacticalProject2Strategy->UpdateTaskForceGoalsAndJobs( this ); }

	// Setup the enemies list
	for( long iOtherPlayer=0; iOtherPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS+1; iOtherPlayer++ ) m_PlayersToTreatAsEnemies[ iOtherPlayer ] = false;
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	//CWorldDescServer* pWorldDescServer = GetAIBrain()->GetWorldDescServer();
	//ASSERT( pWorldDescServer );
	//if( pWorldDescServer )
	{
		for( long iOtherPlayer=1; iOtherPlayer<=iNumberOfPlayers; iOtherPlayer++ )
		{
			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( GetAIBrain()->GetAIPlayerNumber(), iOtherPlayer );
			if( eRel.IsEnemy() ) m_PlayersToTreatAsEnemies[ iOtherPlayer ] = true;
		}
		CEOSAIIntSet HostileActionAgainstPlayers;
		m_pTacticalProject2->GetActionIsHostileAgainstPlayers( &HostileActionAgainstPlayers );
		POSITION pos = HostileActionAgainstPlayers.GetList()->GetHeadPosition();
		while( pos )
		{
			int iOtherPlayer = HostileActionAgainstPlayers.GetList()->GetNext( pos );
			m_PlayersToTreatAsEnemies[ iOtherPlayer ] = true;
		}
	}

	// Relies on m_PlayersToTreatAsEnemies and UpdateTaskForceGoalsAndJobs
	CalculateEnemyUnitCount();

	// Clear state information
//	m_fInvasionStrategy_CombatSuccess02 = 0.0f;
//	m_fInvasionStrategy_AttritionBalance01 = 0.0f;
//	m_fSeedStrategy_CombatSuccess02 = 0.0f;
//	m_fSeedStrategy_AttritionBalance01 = 0.0f;
	m_CombatResultsOfAllocatedUnits.Clear();
	//m_SeedCombatResults.Clear();

	CEOSAIStopwatch2 watch1;
	watch1.Start();
	CopyUnitActionIdeasIntoStack();
	watch1.Stop();

	m_fBestCaseRealScore = 0.0f;

	// Step 1. Allocate all UnitActionIdeas (for Prim/Sec roles + combat) until the job is satisfied
	//   This will give the earliest possible completion (ignoring city production) although with
	//   potentially bad attrition.
	CEOSAIStopwatch2 watch2;
	watch2.Start();
	AllocateUnitsInStack();
	watch2.Stop();

	CEOSAIStopwatch2 watch3;
	watch3.Start();
	//UpdateScoresIfNecessary();
	CalculateScores();
	watch3.Stop();

	CEOSAIStopwatch2 watch4;
	watch4.Start();
	TryInsertingBuildActionsIntoStack();
	watch4.Stop();

	CEOSAIStopwatch2 watch5;
	watch5.Start();
	CalculateScores();
	watch5.Stop();

	CEOSAIStopwatch2 watch6;
	watch6.Start();
	float fRealScore = GetRealScore();
	if( fRealScore == 0.0f )
	{
		// Deallocate the UnitActionIdeas, we couldn't build-up enough units to accomplish this job
		ReleaseStackAllocations();
	}
	else
	{
		// If I am going ahead with this, then create the AIUnitActionIdea for the Transport
		if( m_eBestCaseOrRealTaskForce == RealTaskForce )
		{
			CreateTransportUnitActionIdeaFromSuggestedTransportMoveDescs();
			//this->AllocateSuggestedTransportMoveDesc();
		}
	}
	watch6.Stop();

	m_fGrowthScore = GetGrowthScore();
	m_fRealScore = GetRealScore();

	//if( m_eBestCaseOrRealTaskForce == BestCaseTaskForce )
	//{
	//	m_bHaveConstructedBestCaseTaskForce = true;
	//}
	watch0.Stop();

	float fWatch0 = (float) watch0.GetDeltaSeconds();
	float fWatch1 = (float) watch1.GetDeltaSeconds();
	float fWatch2 = (float) watch2.GetDeltaSeconds(); // 0.31068891 - time value from the first try, save Turn181
	float fWatch3 = (float) watch3.GetDeltaSeconds();
	float fWatch4 = (float) watch4.GetDeltaSeconds(); // 0.09898459 - time value from the first try, save Turn181
	float fWatch5 = (float) watch5.GetDeltaSeconds();
	int h=0;

	/*
	Scoring:
	Do I have a good arrival time?
	Do I have a good attrition balance?  (Relative to what?  What I could build?)
	Do I have sufficient units for the non-combat tasks?
	Do I have sufficient units to win combat?
	*/
}

bool CEOSAITaskForce3::TryInsertingOneBuildActionIntoStack()
{
	m_bNeedToUpdateScores = true;

	//CEOSAICityActionIdeaNode* pBestAICityActionIdeaNode = NULL;
	CEOSAICity*           pBestAICity = NULL;
	CEOSAIUnit2ActionIdea* pBestUnitActionIdea = NULL;
	CEOSAIUnitTemplate*   pBestUnitTemplate = NULL;
	float              fBestScore = 0.0f;

	// If there are no CityActionIdeas, then fail
	//if( m_CityActionIdeas.IsEmpty() ){ return false; }

	// Count up the allocations for each city (using this ActionIdeaStack only)
	//POSITION pos = m_pTacticalProject->GetCityActionIdeaNodes()->GetHeadPosition();
	CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIBrain()->GetAIThoughtDatabase();
	//POSITION pos = m_pTacticalProject->GetCityActionIdeaNodes()->GetHeadPosition();
	POSITION pos = pAIThoughtDatabase->GetMyCities()->GetHeadPosition();
	while( pos )
	{
		//CEOSAICityActionIdeaNode* pCityActionIdeaNode = m_pTacticalProject->GetCityActionIdeaNodes()->GetNext( pos );
		CEOSAICity* pAICity = pAIThoughtDatabase->GetMyCities()->GetNext( pos );
		pAICity->m_iTempItemsInTaskForceStack = 0;
		//pCityActionIdeaNode->m_iTempAllocationsCount = 0;
	}
	pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->m_pUnitActionIdea->m_pAICityActor )
		{
			pStackItem->m_pUnitActionIdea->m_pAICityActor->m_iTempItemsInTaskForceStack++;
		}
	}

	CList< CEOSAIUnitTemplate* >* pUsefulUnitTemplates;
	/*
	if( m_pTacticalProject )
	{
		pUsefulUnitTemplates = m_pTacticalProject->GetUsefulBuildableUnitTemplates();
	}
	*/
	if( m_pTacticalProject2 )
	{
		pUsefulUnitTemplates = m_pTacticalProject2->GetUsefulBuildableUnitTemplates();
	}
	pos = pUsefulUnitTemplates->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = pUsefulUnitTemplates->GetNext( pos );
 
		// I should test the usefulness of this unit before running off to see how long until it arrives

		// Lookup how quickly I can build and get the new unit to the target.
		//   Return the city and arrival time for this.  Do this for each build option.
		//   This will give me a "best case scenario" which will help the AI decide whether to persue it.
		//CEOSAICityActionIdeaNode* pCityActionIdeaNode = NULL;
		CEOSAICity* pAICity = NULL;
		float fArrivalTime = 1000000.0f;
		//GetBestCityBuildArrivalTime( pUnitTemplate, &pCityActionIdea, &fArrivalTime );
		CList< CEOSAICityAndTime >  CityAndTimeList;
		GetCityBuildArrivalTimes( pAIUnitTemplate, &CityAndTimeList );
		// Lookup the best time
		POSITION pos2 = CityAndTimeList.GetHeadPosition();
		while( pos2 )
		{
			CEOSAICityAndTime CityAndTime = CityAndTimeList.GetNext( pos2 );
			if( CityAndTime.m_fBuildAndMoveTime < fArrivalTime &&
				CityAndTime.m_pAICity->m_iTempItemsInTaskForceStack < 2 )
			{
				if( m_eBestCaseOrRealTaskForce == BestCaseTaskForce )
				{
					pAICity = CityAndTime.m_pAICity;
					fArrivalTime = CityAndTime.m_fBuildAndMoveTime;
				}
				if( m_eBestCaseOrRealTaskForce == RealTaskForce &&
					//pCityActionIdea->m_pAICity->IsAllocated() == false )
					//CityAndTime.m_pCityActionNode->m_pAICity->IsAllocated() == false )
					CityAndTime.m_pAICity->IsAllocated() == false )
				{
					pAICity = CityAndTime.m_pAICity;
					fArrivalTime = CityAndTime.m_fBuildAndMoveTime;
				}
			}
		}

		// Is this valuable at all according to the TimeValueFunction?
		float fTimeValue = m_TimeValueFunction.GetOutput( fArrivalTime );
		//if( pCityActionIdeaNode && fTimeValue < 1000000.0f )
		if( pAICity && fTimeValue < 1000000.0f )
		{
			// Yes, it has a TimeValue.  Try adding it to the stack to see how it affects things

			// Create a hypothetical unit
			//long iOwner = m_pTacticalProject->GetAIBrain()->GetAIPlayerNumber();
			//float fTimeToBuild = pCityActionIdea->m_pAICity->GetCity()->GetTimeToBuild( pUnitTemplate, true );
			//CEOSAILocation InitialLocation = pCityActionIdea->m_pAICity->GetCity()->GetLocation();

		//	Don't allocate a city more than twice

			//CEOSAIUnit2ActionIdea* pUnitActionIdea = pCityActionIdeaNode->CreateAIUnitActionIdea( pUnitTemplate );
			CEOSAIUnit2ActionIdea* pUnitActionIdea;
			/*
			if( m_pTacticalProject )
			{
				pUnitActionIdea = pAICity->CreateAIUnitActionIdea( m_pTacticalProject, pUnitTemplate );
			}
			*/
			if( m_pTacticalProject2 )
			{
				pUnitActionIdea = pAICity->CreateAIUnitActionIdea( m_pTacticalProject2, pAIUnitTemplate );
			}
			CEOSAIUnit2ActionStackItem* pUnitActionStackItem = m_UnitActionStack.Insert( pUnitActionIdea );
			ASSERT( pUnitActionStackItem->m_fArrivalTime >= 0.0f && pUnitActionStackItem->m_fArrivalTime <= 1000000.0f );
			ASSERT( m_UnitActionStack.m_ItemsSortedByArrivalTime.GetCount() < 500 );
			long iItemsAllocated = m_UnitActionStack.GetNumberOfAllocatedItems();

			// Is this addition valuable?  Run calculations to see
			RecordCurrentScores();

			float fOldScore = GetGrowthScore();
			pUnitActionStackItem->Allocate( true );
			CalculateScores();
			float fNewScore = GetGrowthScore(); // The NewScore might be lower - if it takes extra time

			// Restore the old state
			m_UnitActionStack.Remove( pUnitActionStackItem );
			pUnitActionStackItem->Allocate( false );
			delete pUnitActionStackItem;
			RestoreOldScores();

			bool bNewUnitActionIdeaAddedAsBestUnitActionIdea = false;
			if( fNewScore > fOldScore )
			{
				if( fNewScore > fBestScore )
				{
					//pBestAICityActionIdeaNode = pCityActionIdeaNode;
					pBestAICity = pAICity;
					pBestUnitTemplate = pAIUnitTemplate;
					fBestScore  = fNewScore;
					if( pBestUnitActionIdea ) delete pBestUnitActionIdea;
					pBestUnitActionIdea = pUnitActionIdea;
					bNewUnitActionIdeaAddedAsBestUnitActionIdea = true;
				}
			}
			if( bNewUnitActionIdeaAddedAsBestUnitActionIdea == false ){ delete pUnitActionIdea; }
		}
	}

	// Add the build idea to the Stack?
	//if( pBestAICityActionIdeaNode && pBestUnitActionIdea && pBestUnitTemplate )
	if( pBestAICity && pBestUnitActionIdea && pBestUnitTemplate )
	{
		// Insert the UnitActionIdea into the Stack
		CEOSAIUnit2ActionStackItem* pUnitActionStackItem = m_UnitActionStack.Insert( pBestUnitActionIdea );
		AllocateUnitsInStack();
		return true; // item inserted
	}
	return false;
}

void CEOSAITaskForce3::CreateTransportUnitActionIdeaFromSuggestedTransportMoveDescs()
{
	POSITION pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->ActorIsAllocatedByThisTaskForce() &&
			pStackItem->GetSuggestedTransportMoveDesc() )
		{
			pStackItem->CreateTransportUnitActionIdeaFromSuggestedTransportMoveDesc();
			//this->AllocateSuggestedTransportMoveDesc();
			//pStackItem->m_pUnitActionIdea->m_pAICityActor->m_iTempItemsInTaskForceStack++;
		}
	}
}

void CEOSAITaskForce3::RecordCurrentScores()
{
	m_PrevCombatResults = m_CombatResultsOfAllocatedUnits;
	//m_PrevSeedCombatResults = m_SeedCombatResults;
	m_fPrevGrowthScore = m_fGrowthScore;
	m_fPrevRealScore = m_fRealScore;
	/*
	m_ScoresBackup.AddTail( m_fInvasionStrategy_CombatSuccess02 );
	m_ScoresBackup.AddTail( m_fInvasionStrategy_AttritionBalance01 );
	m_ScoresBackup.AddTail( m_fSeedStrategy_CombatSuccess02 );
	m_ScoresBackup.AddTail( m_fSeedStrategy_AttritionBalance01 );
	m_ScoresBackup.AddTail( m_fGrowthScore );
	m_ScoresBackup.AddTail( m_fRealScore );
	*/
}

void CEOSAITaskForce3::RestoreOldScores()
{
	m_CombatResultsOfAllocatedUnits = m_PrevCombatResults;
	//m_SeedCombatResults = m_PrevSeedCombatResults;
	m_fGrowthScore = m_fPrevGrowthScore;
	m_fRealScore = m_fPrevRealScore;
	/*
	m_fRealScore = m_ScoresBackup.RemoveTail();
	m_fGrowthScore = m_ScoresBackup.RemoveTail();
	m_fSeedStrategy_AttritionBalance01 = m_ScoresBackup.RemoveTail();
	m_fSeedStrategy_CombatSuccess02 = m_ScoresBackup.RemoveTail();
	m_fInvasionStrategy_AttritionBalance01 = m_ScoresBackup.RemoveTail();
	m_fInvasionStrategy_CombatSuccess02 = m_ScoresBackup.RemoveTail();
	*/
}

//void CEOSAITaskForce3::Schedule()
void CEOSAITaskForce3::CreatePredefinedSteps()
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	m_pTacticalProject2->SetTaskForceWithPredefinedSteps( this );//m_fCompletionTime );

	// Create AIUnitAction objects / Build Orders
	//
	// In this section, I need to allocate AIUnits to targets.
	//   There are probably multiple units and multiple targets.  How do I match them up?
	//
	// Step 1. Create a list of available Targets for each unit
	// Step 2. Allocate units to Targets based on their AvailableTargets
	//         This includes avoiding 'doubling up' on targets that only need one unit (e.g. capture res)

	// At this point, I have a TacticalProject, a bunch of desires, and a bunch of actors.
	//   Now, I need to setup the actors to best accomplish all the desires.
	//   This will setup the 'approach' (come in from the front, side, capture who?)
	//   Each actor has an AIUnitAction.  Now, assign a [TargetDesire] to each AIUnitAction.
	// Some factors in setting up these TargetDesires:
	//   - What units do I have for this TacticalProject?
	//   - What Desires are closest, and most accessible?
	//   - Am I trying to be secretive?  (If so, attack border areas first, simultaneous arrival.)
	//   - Can this unit accomplish this desire?  (e.g. Aircraft can't capture cities)
	// Things to determine: Is this a sneak attack?  (against ally/neutral/enemy)?
	//   If I have his terribly outnumbered, then I might not need to worry about stealth.
	//   If this is against Player0, then no stealth is needed at all.
	// Right now, I'll just attack the closest valid target for each unit.  No coordination, no stealth

	POSITION pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			pStackItem->m_pUnitActionIdea->CompileDesireValues();//CompileTargetDesires();
		}
		/*
		#ifdef _DEBUG
		if( pStackItem->m_pUnitActionIdea->GetAIUnitActor() )
		{
			if( pStackItem->m_pUnitActionIdea->GetAIUnitActor()->GetServerUnit()->GetObjectId() == 4960 )
			{
				int h3424=0;
			}
		}
		#endif
		*/
	}

	//
	// Allocate Units to TargetDesires
	//
	// Originally, I was thinking that any units with a single desire should go fulfill that desire
	//   (because they can't possibly do anything else in the TacticalProject).
	//   However, I might want to wait until other things change (don't walk past powerful enemy units)
	//
	// Step 1. Is this a non-combat/player0-combat TacticalProject?
	//   If so, then my job is easier - less planning required because there are no combat complications.
	bool bAgainstPlayerN = false;
	/*
	if( GetTacticalProject() )
	{
		bAgainstPlayerN = GetTacticalProject()->PrimaryGoalsAreHostileActionAgainstAnotherPlayer();
	}
	*/
	if( GetTacticalProject2() )
	{
		bAgainstPlayerN = GetTacticalProject2()->PrimaryGoalsAreHostileActionAgainstAnotherPlayer();
	}
	if( bAgainstPlayerN )
	{
		// This involved an attack on another player - this gets a little more complicated
		//   because of unit complexities, counterattacks, etc.

		// Step 1. I need to figure out paths to targets.  This way I can figure out if I can move safely.
		// I sort of need a scheduling system like CEOSAITaskForce
		// Maybe I should move everything together at the same time - no, aircraft move fast

		//ASSERT( false ); // need to handle this
	}

	//else
	{
		// This is against neutral or player0 desires - easy case.
		//   There might be some militia involved.

		// Some factors:
		//  1) I might want to establish units on the island quickly.  If so, then capture cities.
		//     Do this if the island already has enemy units, or is near the enemy nation
		//  2) Try to get the highest-value targets at the lowest cost 
		//     High-value resource > low-value city (with militia)
		//     High-value city > low-value city (same combat, better outcome)
		//     City with no militia > City with militia
		//  3) Prefer close targets to distant targets

		// Iterate over each Actor.
		//   Attach a value to each TargetDesire
		//   The value is based on all of the above factors, and it unique for each unit due to:
		//     Different locations of units, Movement speed, Attrition values, ...
		//bool bAllUnitsHaveBeenSubAllocated = false;
		//while( bAllUnitsHaveBeenSubAllocated == false )
		bool bAllUnitsHaveBeenSubAllocated = false;
		while( bAllUnitsHaveBeenSubAllocated == false )
		{
			bAllUnitsHaveBeenSubAllocated = true;

			pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
			while( pos )
			{
				CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
				if( pStackItem->ActorIsAllocatedByThisTaskForce() )
				{
					CEOSAIUnit2ActionIdea* pAIUnitActionIdea = pStackItem->m_pUnitActionIdea;
					if( pAIUnitActionIdea->GetNumberOfDesireAllocations() > 0 ) continue; // already allocated

					if( pAIUnitActionIdea->GetAIPoiObjectActor()->GetObjectId() == 120 )
					{
						int h=0;
					}
					if( pAIUnitActionIdea->GetAIPoiObjectActor()->GetObjectId() == 121 )
					{
						int h=0;
					}

					/*
					#ifdef _DEBUG
					if( pAIUnitActionIdea->GetAIUnitActor() )
					{
						if( pAIUnitActionIdea->GetAIUnitActor()->GetServerUnit()->GetObjectId() == 4960 )
						{
							int h3424=0;
						}
					}
					#endif
					*/
					float fBestUnitActionIdeaTargetValue = 0.0f;
					CEOSAIUnit2ActionIdeaTargetValue* pBestUnitActionIdeaTargetValue = NULL;

					CList< CEOSAIUnit2ActionIdeaTargetValue* >* pTargets = &pAIUnitActionIdea->m_DesireValues;
					POSITION pos2 = pTargets->GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnit2ActionIdeaTargetValue* pTargetValue = pTargets->GetNext( pos2 );
						//if( GetTacticalProject()->IsPrimaryGoal( pTargetValue->m_pDesire ) == false ) continue;

						float fUnfulfillment = 1.0f - (0.5f*pTargetValue->m_pDesire->GetAllocatedFulfillment());
						if( pTargetValue->m_fTargetValue*fUnfulfillment > fBestUnitActionIdeaTargetValue )
						{
							pBestUnitActionIdeaTargetValue = pTargetValue;
							fBestUnitActionIdeaTargetValue = pTargetValue->m_fTargetValue*fUnfulfillment;
						}
					}
					if( pBestUnitActionIdeaTargetValue )
					{
						bAllUnitsHaveBeenSubAllocated = false;

						CEOSAIDesireSpatial* pDesire = pBestUnitActionIdeaTargetValue->m_pDesire;
						CEOSAIUnit2ActionIdea* pAIUnitActionIdea = pBestUnitActionIdeaTargetValue->m_pAIUnitActionIdea;

						pDesire->AddAllocation( pAIUnitActionIdea );
						pAIUnitActionIdea->AddDesireAllocation( pDesire );
					}
				}
			}
			/*
			float fBestUnitActionIdeaTargetValue = 0.0f;
			CEOSAIUnit2ActionIdeaTargetValue* pBestUnitActionIdeaTargetValue = NULL;
			pos = m_UnitActionStack.m_Items.GetHeadPosition();
			while( pos )
			{
				CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_Items.GetNext( pos );
				if( pStackItem->ActorIsAllocatedByThisTaskForce() )
				{
					CEOSAIUnit2ActionIdea* pAIUnitActionIdea = pStackItem->m_pUnitActionIdea;
					if( pAIUnitActionIdea->GetNumberOfDesireAllocations() > 0 ) continue; // already allocated

					CList< CEOSAIUnit2ActionIdeaTargetValue* >* pTargets = &pAIUnitActionIdea->m_DesireValues;
					POSITION pos2 = pTargets->GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnit2ActionIdeaTargetValue* pTargetValue = pTargets->GetNext( pos2 );
						//if( GetTacticalProject()->IsPrimaryGoal( pTargetValue->m_pDesire ) == false ) continue;

						float fUnfulfillment = 0.1f + (0.9f - pTargetValue->m_pDesire->GetAllocatedFulfillment());
						if( pTargetValue->m_fValue*fUnfulfillment > fBestUnitActionIdeaTargetValue )
						{
							pBestUnitActionIdeaTargetValue = pTargetValue;
							fBestUnitActionIdeaTargetValue = pTargetValue->m_fValue*fUnfulfillment;
						}
					}
				}
			}
			if( pBestUnitActionIdeaTargetValue )
			{
				bAllUnitsHaveBeenSubAllocated = false;

				CEOSAIDesireSpatial* pDesire = pBestUnitActionIdeaTargetValue->m_pDesire;
				CEOSAIUnit2ActionIdea* pAIUnitActionIdea = pBestUnitActionIdeaTargetValue->m_pAIUnitActionIdea;

				pDesire->AddAllocation( pAIUnitActionIdea );
				pAIUnitActionIdea->AddDesireAllocation( pDesire );
			}
			*/
		}

		// Iterate over each Actor, find the best target (which doesn't have enough units allocated).
		//   Allocate the best actor to his best target.
		//   Repeat
		/*
		float fLowestFulfillment = 0.0f;
		CEOSAIDesireSpatial* pLowestFulfillmentDesire = NULL;
		pos = m_UnitActionStack.m_Items.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_Items.GetNext( pos );
			if( pStackItem->ActorIsAllocated() )
			{
				CEOSAIUnit2ActionIdea* pAIUnitActionIdea = pStackItem->m_pUnitActionIdea;
				//pAIUnitActionIdea->GetNumberOfDesireValues();

				CList< CEOSAIUnit2ActionIdeaTargetValue* >* pTargets = &pAIUnitActionIdea->m_DesireValues;
				POSITION pos2 = pTargets->GetHeadPosition();
				while( pos2 )
				{
					CEOSAIUnit2ActionIdeaTargetValue* pTargetValue = pTargets->GetNext( pos2 );
					CEOSAIDesireSpatial* pDesire = pTargetValue->m_pDesire;
					float fFulfillment = pDesire->GetAllocatedFulfillment();
					if( fFulfillment <= fLowestFulfillment )
					{
						fLowestFulfillment = fFulfillment;
						pLowestFulfillmentDesire = pDesire;
					}
				}

				if( pLowestFulfillmentDesire )
				{
					pLowestFulfillmentDesire->AddAllocation( pAIUnitActionIdea );
					pAIUnitActionIdea->AddDesireAllocation( pLowestFulfillmentDesire );
				}
			}
		}
		*/
	}

	// Create City Build Orders and an AIUnit
	pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );

		#ifdef _DEBUG
		if( pStackItem->m_pUnitActionIdea->GetAIUnitActor() )
		{
			if( pStackItem->m_pUnitActionIdea->GetAIUnitActor()->GetObjectId() == 4960 )
			{
				int h3424=0;
			}
		}
		#endif

		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			CEOSAIUnit2ActionIdea* pUnitActionIdea = pStackItem->m_pUnitActionIdea;
			//ASSERT( pUnitActionIdea->GetAIUnitAction() == NULL );
			if( pUnitActionIdea->GetNumberOfDesireAllocations() == 0 ) continue;

			if( pUnitActionIdea->GetAITransportUnitActionIdea() )
			{
				CEOSAIUnit2ActionIdea* pTransportUnitActionIdea = pUnitActionIdea->GetAITransportUnitActionIdea();
				CEOSAICity* pAICity = pTransportUnitActionIdea->GetAIUnitActor()->GetBuiltByCity();
				if( pAICity && pAICity->GetNumberOfBuildOrders() == 0 )
				{
					pAICity->ActionScheduler_CreateBuildOrder( pTransportUnitActionIdea->GetAIUnitTemplate() );
				}
			}
			if( pUnitActionIdea->GetAICityActor() )//>GetAIUnitActor()->IsHypotheticalPoiObject() )
			{
				//CEOSAICity* pAICity = pUnitActionIdea->GetAIUnitActor()->GetBuiltByCity();
				CEOSAICity* pAICity = pUnitActionIdea->GetAICityActor();
				ASSERT( pAICity );
				pAICity->ActionScheduler_CreateBuildOrder( pUnitActionIdea->GetAIUnitTemplate() );

				// Create an AIUnit - which can be used for tasks 
				// (this code is not currently setup)!  I don't need to create the AIUnit if I don't use it
//				CEOSAIUnit2* pAIUnit = new CEOSAIUnit2( m_pAIBrain );
//				pAIUnit->
			}
		}
	}
	// Create Unit Predefined Steps
	pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			CEOSAIUnit2ActionIdea* pUnitActionIdea = pStackItem->m_pUnitActionIdea;
			if( pUnitActionIdea->GetNumberOfDesireAllocations() == 0 ) continue;

			if( pUnitActionIdea->GetAIUnitActor() )
			{
				pUnitActionIdea->CreatePredefinedStep();
			}
		}
	}
}

void  CEOSAITaskForce3::SynchronizeInvasionUsingRedPath()
{
	//ASSERT( m_pTacticalProject2->GetTaskForceStrategy() == CEOSAITacticalProject2::EnumTaskForceStrategy_SecretInvasion );
	long iTargetGeo = this->GetTacticalProject2()->GetTargetGeo();

	CList< CEOSAIUnit2* >  AllUnitsInvolvedInTaskForce;
	POSITION pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pStackItem->ActorIsAllocatedByThisTaskForce() )
		{
			CEOSAIUnit2ActionIdea* pUnitActionIdea = pStackItem->m_pUnitActionIdea;
			if( pUnitActionIdea->GetNumberOfDesireAllocations() == 0 )
			{
				int g=0;
				continue;
			}

			CEOSAIUnit2* pAIUnit = pUnitActionIdea->GetAIUnitActor();
			if( pAIUnit )
			{
				AllUnitsInvolvedInTaskForce.AddTail( pAIUnit );
			}
		}
	}

	// When do the ground/sea units arrive at the geo?
	//   If the ground units are coming in on transports, then find out the first
	//   shore-arrival for each GroundUnit on each transport.
	long iUnitsOnTheTargetGeo = 0;
	//float fLastArrivalTimeAtTargetGeo = 0.0f;
	CMap< long,long, float,float >  UnitIdGeoTriggerTime;
	pos = AllUnitsInvolvedInTaskForce.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pAIUnit = AllUnitsInvolvedInTaskForce.GetNext( pos );
		if( pAIUnit->IsGroundUnit() || pAIUnit->IsSeaUnit() )
		{
			if( pAIUnit->GetInitialState()->GetGeo() == iTargetGeo )
			{
				iUnitsOnTheTargetGeo++;
				//UnitIdGeoArrivalTimes.SetAt( pAIUnit->GetObjectId(), 0.0f );
				// If the unit is on the geo, then set the trigger time to be the attack time
				float fAttackTime = pAIUnit->GetAttackTimeOfTacticalProject( GetTacticalProject2() );
				UnitIdGeoTriggerTime.SetAt( pAIUnit->GetObjectId(), fAttackTime );
			}
			else
			{
				// This unit is not on the target geo
				CEOSAIUnit2* pAITransportUnit = pAIUnit->GetTransportUsedInTacticalProject( GetTacticalProject2() );
				if( pAITransportUnit )
				{
					float fTime = 0.0f;
					float fArrivalTimeAtTargetGeo = pAIUnit->CalculateRedPathArrivalTimeAtGeo( iTargetGeo );
					if( fArrivalTimeAtTargetGeo >= 0.0f )
					{
						if( UnitIdGeoTriggerTime.Lookup( pAITransportUnit->GetObjectId(), fTime ) )
						{
							if( fArrivalTimeAtTargetGeo < fTime )
							{
								UnitIdGeoTriggerTime.SetAt( pAIUnit->GetObjectId(), fArrivalTimeAtTargetGeo );
							}
						}
						else
						{
							UnitIdGeoTriggerTime.SetAt( pAIUnit->GetObjectId(), fArrivalTimeAtTargetGeo );
						}
					}
				}
				else
				{
					// This can happen if a combat ship is attacking an enemy ship, but the
					//   target geo is land
					// I'm adjusting the attack 
					//float fAttackTime = pAIUnit->GetAttackTimeOfTacticalProject( GetTacticalProject2() );
					//UnitIdGeoTriggerTime.SetAt( pAIUnit->GetObjectId(), fAttackTime+3.0f );
					//UnitIdGeoArrivalTimes.SetAt( pAIUnit->GetObjectId(), fArrivalTimeAtTargetGeo );
				}
				//fLastArrivalTimeAtTargetGeo = max( fArrivalTimeAtTargetGeo, fLastArrivalTimeAtTargetGeo );
			}
		}
	}

	// Look through the trigger times to figure out when the synchronized trigger should happen
	long iUnitObjectId;
	float fUnitTriggerTime = 0.0f;
	float fGroupTriggerTime = 0.0f;
	pos = UnitIdGeoTriggerTime.GetStartPosition();
	while( pos )
	{
		UnitIdGeoTriggerTime.GetNextAssoc( pos, iUnitObjectId,fUnitTriggerTime );
		fGroupTriggerTime = max( fGroupTriggerTime, fUnitTriggerTime );
	}

	// Adjust the timings so everyone arrives at the same time
	pos = AllUnitsInvolvedInTaskForce.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pAIUnit = AllUnitsInvolvedInTaskForce.GetNext( pos );
		if( pAIUnit->IsGroundUnit() || pAIUnit->IsSeaUnit() )
		{
			if( pAIUnit->GetInitialState()->GetGeo() != iTargetGeo )
			{
				if( UnitIdGeoTriggerTime.Lookup( pAIUnit->GetObjectId(), fUnitTriggerTime ) )
				{
					float fWaitTime = fGroupTriggerTime - fUnitTriggerTime;
					fWaitTime -= 0.8f;
					if( fWaitTime > 0.0f ){ pAIUnit->SetInitialWaitTime( fWaitTime ); }
				}
			}
			/*
			if( pAIUnit->GetInitialState()->GetGeo() == iTargetGeo )
			{
				iUnitsOnTheTargetGeo++;
				pAIUnit->GetAIUnitPathwayFinder()->SetCacheRedPathGeoArrivalTime( 0.0f );
			}
			else
			{
				// This unit is not on the target geo
				float fArrivalTimeAtTargetGeo = pAIUnit->GetRedPathArrivalTimeAtGeo( iTargetGeo );
				pAIUnit->GetAIUnitPathwayFinder()->SetCacheRedPathGeoArrivalTime( fArrivalTimeAtTargetGeo );

				fLastArrivalTimeAtTargetGeo = max( fArrivalTimeAtTargetGeo, fLastArrivalTimeAtTargetGeo );
			}
			*/
		}
	}
}

void  CEOSAITaskForce3::AllocateUnitsInStack()
{
	m_bNeedToUpdateScores = true;

	//if( m_pTacticalProject2->GetAIPlayer()->GetPlayerNumber() == 1 && 
	//	m_pTacticalProject2->GetId() == 137 )
	//{
		// iAIPlayer == 1 && pTacticalProject2->GetId() == 137
	//	int h=0;
	//}
	if( m_pTacticalProject2->GetId() == 1 &&
		m_eBestCaseOrRealTaskForce == RealTaskForce )
	{
		int h=0;
	}

	// Clear out the old CTimeAndRealScore data
	//while( m_TimeAndRealScores.IsEmpty() == FALSE ){ delete m_TimeAndRealScores.RemoveHead(); }

	ASSERT( m_eBestCaseOrRealTaskForce == BestCaseTaskForce ||
			m_eBestCaseOrRealTaskForce == RealTaskForce );
	//ASSERT( m_eUnitNegotiation != NegotiateForUnits );
	//ASSERT( m_eUnitNegotiation == BestCaseUnits ||
	//		m_eUnitNegotiation == AllocateOnlyUnallocatedUnits );
	EnumUnitNegotiation eUnitNegotiation = BestCaseUnits;
	if( m_eBestCaseOrRealTaskForce == BestCaseTaskForce )
	{
		eUnitNegotiation = BestCaseUnits;
	}
	if( m_eBestCaseOrRealTaskForce == RealTaskForce )
	{
		eUnitNegotiation = AllocateOnlyUnallocatedUnits;//c;
	}

	if( m_eBestCaseOrRealTaskForce == RealTaskForce )
	{
		// Lookup the BestCase end time
		//   Add 30% to the BestCase end time, and use it as the 'target time' for the negotiated units?
		int g=0;
	}

	bool  bFirstCompletionTimeFound = false;
	float fFirstCompletionTime = 1000000.0f;
	//m_fInvasionStrategy_CombatSuccess02 = 0.0f;
	//m_fSeedStrategy_CombatSuccess02 = 0.0f;
	//m_SeedCombatResults.Clear();

	// Clear the 'allocated' flag
	ReleaseStackAllocations();

	// One by one, add each unit to the capability until the JobsToDo is satisfied.
	//   When it is finally satisfied, this is the minimum completion time.
	//   Then I can see about removing earlier units 
	//     Some of these earlier units: might not help things, might be overkill, or might have bad attrition
	//   Also, I can see about having cities fulfill some roles
	//float fSufficientForcesTime = 0.0f;
	//CEOSAIJobCapability  AIJobCapability;
	m_JobCapabilityOfAllocatedUnits.Clear();
	m_CombatResultsOfAllocatedUnits.Clear();

	long iIterationCount = 0;
	POSITION pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pUnitStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pUnitStackItem->ActorIsAllocatedByThisTaskForce() ) continue; // Can occur if I reposition the item
		if( pUnitStackItem->ActorIsAllocatedByOtherTaskForce() ) continue; // Can occur if I reposition the item
		iIterationCount++;

		CEOSAIUnit2ActionIdea* pUnitActionIdea = pUnitStackItem->m_pUnitActionIdea;
		CEOSAIUnitTemplate* pAIUnitTemplate = pUnitStackItem->GetAIUnitTemplate();

		if( m_pTacticalProject2Strategy && m_pTacticalProject2Strategy->UseOnlyAirUnits() )
		{
			if( pAIUnitTemplate->IsAirUnit() == false ) continue;
		}

		// TEMPORARY HACK: Don't negotiate for units.  Simply allocate them (if they aren't allocated)
		if( eUnitNegotiation == AllocateOnlyUnallocatedUnits &&
			pUnitStackItem->ActorIsAllocatedByOtherTaskForce() ) continue;

		if( pUnitActionIdea->GetDirectMoveToTargetTime() == 1000000.0f &&
			pUnitActionIdea->GetComboTransportMoveToTargetTime() == 1000000.0f )
		{
			// There is no known pathway
			// Note: This might not be the way to handle this.
			//   If a player has some ships inside a coastal city, and I'd like to go after them
			//   with my own ship, there will be no known pathway.
			continue; 
		}

		// What's the non-combat value?
		// What's the combat value of this unit (first, on the total enemy; then based on capability)?
		//   I could cache this data, but invalidate it whenever the group changes
	    // Why aren't the destroyers coming in to attack the sub?

		// Ignoring the current capability, is this unit useful?
		//    Use this unit if:
		//    (A) It performs some non-combat role
		//    (B) There is combat to be done, it performs a combat role, and with decent attrition
		// I might not want this unit if there are other units in the stack that can perform the
		//   same task with better numbers.  This is partially dependent on time-pressures.
		bool bCanFulfillAGeneralJob = false;
		if( m_JobsToDo.CanFulfillAGeneralJob( pAIUnitTemplate ) )
		{
			if( eUnitNegotiation == BestCaseUnits )
			{
				bCanFulfillAGeneralJob = true;
			}
			if( eUnitNegotiation == AllocateOnlyUnallocatedUnits )
			{
				// Look at the current capability vs needs.  Is this unit a useful addition to the group?
				float fValue1 = m_JobsToDo.GetAverageNonCombatFulfillment01( &m_JobCapabilityOfAllocatedUnits );
				m_JobCapabilityOfAllocatedUnits.Add( pAIUnitTemplate );
				float fValue2 = m_JobsToDo.GetAverageNonCombatFulfillment01( &m_JobCapabilityOfAllocatedUnits );
				m_JobCapabilityOfAllocatedUnits.Remove( pAIUnitTemplate );
				if( fValue2 > fValue1 ){ bCanFulfillAGeneralJob = true; }
			}
		}
		bool bCanFulfillACombatJob = false;
		if( pAIUnitTemplate->IsACombatUnit() && m_JobsToDo.CanFulfillASpecificCombatJob( pAIUnitTemplate ) )
		{
			if( eUnitNegotiation == BestCaseUnits )
			{
				bCanFulfillACombatJob = true;
			}
			if( eUnitNegotiation == AllocateOnlyUnallocatedUnits )
			{
				// Look at the current combat capability vs needs.  Is this unit a useful addition?
				CEOSAIQuickCombatResults  NewResults;
				m_CombatCalculation.AddToAttackers( pAIUnitTemplate );
				m_CombatCalculation.RunCalculation();
				m_CombatCalculation.SaveState( &NewResults );
				if( NewResults.m_fCombatSuccessValue02 > m_CombatResultsOfAllocatedUnits.m_fCombatSuccessValue02 )
				{
					bCanFulfillACombatJob = true;
				}
				m_CombatCalculation.RemoveFromAttackers( pAIUnitTemplate );
			}
		}
		if( bCanFulfillAGeneralJob == false && bCanFulfillACombatJob == false ) continue;
		/*
		bool bCanFulfilANonCombatJob = m_JobsToDo.CanFulfillAGeneralJob( pUnitTemplate );
		bool bCanFulfilACombatJob = m_JobsToDo.CanFulfillASpecificCombatJob( pUnitTemplate );
		if( m_JobsToDo.CanFulfillAGeneralJob( pUnitTemplate ) == false &&
			m_JobsToDo.CanFulfillASpecificCombatJob( pUnitTemplate ) == false )
		{
			continue;
		}
		*/


		bool bAllocateThisUnit = true;
		/*
		if( //m_eUnitNegotiation == NegotiateForUnits ||
			m_eUnitNegotiation == AllocateOnlyUnallocatedUnits )
		{
			// See if I can allocate this unit (Is anyone else using it?  Can I steal it from them?)
			// value of this ActionIdea, value of alternative (replacement) ActionIdea?
			/*
			how do I want to setup unit allocation and negotiation?
			  First, the task force tries to allocates everything it wants.
			  If the AIUnit is already taken, then try to negotiate for it
			  Negotiation involves:
			    TaskForceX wants to steal AIUnit from TaskForceY
				How much does TaskForceX need it?  (What are the alternatives?  What are the value of those alternatives?  Just as good? Worse? None?)
				How much does TaskForceY need it?  (What are the alternatives?)
			*-/
			// TEMPORARY
			if( pUnitStackItem->ActorIsAllocatedByOtherTaskForce() ) bAllocateThisUnit = false;
		}
		*/
		if( bAllocateThisUnit == false ) continue;

		if( pUnitStackItem->GetSuggestedTransportMoveDesc() ){ pUnitStackItem->ClearSuggestedTransportMoveDesc(); }
		// DEBUG
		/*
		ASSERT( pUnitStackItem->m_pSuggestedTransportMoveDesc == NULL );
		if( pUnitStackItem->GetSuggestedTransportMoveDesc() &&
			pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport )
		{
			bool b = pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport->CanInsertTransportIdea( this, pUnitStackItem->m_pUnitActionIdea );
			ASSERT( b );
		}
		*/
		// DEBUG

		// DEBUG
		/*
		if( pUnitStackItem->GetSuggestedTransportMoveDesc() &&
			pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport )
		{
			bool b = pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport->CanInsertTransportIdea( this, pUnitStackItem->m_pUnitActionIdea );
			ASSERT( b );
		}
		*/
		// DEBUG

		//
		// Find a transport that I *could* use (I want to know how fast I can get it there)
		//   I want to know how quickly it can get there before I negotiate for this unit
		//
		// Set a suggested transport?
		if( pUnitActionIdea->RequiresTransport() )
			//pUnitStackItem->GetSuggestedTransportMoveDesc() == NULL )
		{
			//pUnitActionIdea->InvokeTransportMovementDescs();

			SimpleCalc_SetSuggestedTransport( pUnitStackItem );
			ASSERT( pUnitActionIdea->GetAIUnitTemplate()->IsGroundUnit() );
			ASSERT( pUnitActionIdea->GetDirectMoveToTargetTime() > 999999.0f );

			// Did I find a transport?  If not, then skip this unit
			if( pUnitStackItem->GetSuggestedTransportMoveDesc() == NULL ) continue;
		}

		// DEBUG
		if( pUnitStackItem->GetSuggestedTransportMoveDesc() &&
			pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport )
		{
			bool b = pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport->CanInsertTransportIdea( this, pUnitStackItem->m_pUnitActionIdea );
			ASSERT( b );
		}
		// DEBUG

		/*
		if( pUnitActionIdea->RequiresTransport() )
		{
			CEOSAIUnit2TransportMovementDesc* pBestPossibleMovementDesc = NULL;
			CEOSAIUnit2TransportMovementDesc* pBestAvailableMovementDesc = NULL;
			pUnitActionIdea->GetTransportIdeas( &pBestPossibleMovementDesc, &pBestAvailableMovementDesc );

			// Use the best possible transport (ignore competition for this transport)
			if( m_eBestCaseOrRealTaskForce == BestCaseTaskForce )
			{
				if( pBestPossibleMovementDesc == NULL ) continue;
				if( pUnitStackItem->GetSuggestedTransportMoveDesc() != pBestPossibleMovementDesc )
				{
					pUnitStackItem->SetSuggestedTransportMoveDesc( pBestPossibleMovementDesc );
				}
			}
			// Use the best available transport
			//   (I could negotiate for a better transport, but that is not yet implimented)
			if( m_eBestCaseOrRealTaskForce == RealTaskForce )
			{
				if( pBestAvailableMovementDesc == NULL ) continue;
				if( pUnitStackItem->GetSuggestedTransportMoveDesc() != pBestAvailableMovementDesc )
				{
					pUnitStackItem->SetSuggestedTransportMoveDesc( pBestAvailableMovementDesc );
					// I really need to restart the stack-iteration
				}
			}
		}
		if( pUnitActionIdea->RequiresTransport() && pUnitStackItem->GetSuggestedTransportMoveDesc() == NULL ) continue;
		*/

		//if( m_eBestCaseOrRealTaskForce == RealTaskForce ){}

		if( bAllocateThisUnit == false )
		{
			if( pUnitStackItem->GetSuggestedTransportMoveDesc() )
			{
				pUnitStackItem->ClearSuggestedTransportMoveDesc();
				//pUnitStackItem->SetSuggestedTransportMoveDesc( NULL );
			}
			continue;
		}

		/*
		Some different ways to look at transport-based TacticalProjects-
			Think about it in terms of transport movement - the transports move from A to B, and
			  there is a pathway of GroundUnits 'on the way' that can be picked up.
			Right now, I'm either using one transport for each GroundUnit, or I'm allocating a
			  Transport to a TacticalProject and using it to pickup GroundUnits.
			  The problem is that the GroundUnits might be widely separated, even though they
			  are the closest GroundUnits to the target.
			If I need a whole bunch of units, then finding the closest one and allocating it 
			  isn't very useful.
			Perhaps I need to think about it more wholistically.
		*/

		// Change the "Suggested Transport" into an "Allocated Transport"
		//if( m_eBestCaseOrRealTaskForce == BestCaseTaskForce &&
		/*
		if( pUnitActionIdea->RequiresTransport() &&
			pUnitStackItem->GetSuggestedTransportMoveDesc() )
		{
			/-*
			if( pUnitActionIdea->GetAIUnitActor() &&
				pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer() )
			{
				ASSERT( pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport ==
						pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer() );
			}
			else
			{
				pUnitStackItem->AllocateSuggestedTransportSpace();
				pUnitStackItem->AllocateSuggestedTransportMoveDesc();
			}
			*-/
			pUnitStackItem->AllocateSuggestedTransportSpace();
			pUnitStackItem->AllocateSuggestedTransportMoveDesc();

			/-*
			CEOSAIUnit2TransportMovementDesc* pSuggestedTransportMovementDesc = pUnitStackItem->GetSuggestedTransportMoveDesc();

			// Look-up the transport
			if( m_eBestCaseOrRealTaskForce == BestCaseTaskForce )
			{
				CEOSAIUnit2ActionIdea* pAITransportActionIdea = pSuggestedTransportMovementDesc->CreateAIUnitActionIdea( m_pTacticalProject );
				pUnitActionIdea->SetAITransportActionIdea( pAITransportActionIdea );
			}
			if( m_eBestCaseOrRealTaskForce == RealTaskForce )
			{
				CEOSAIUnit2ActionIdea* pAITransportActionIdea = pSuggestedTransportMovementDesc->CreateAIUnitActionIdea( m_pTacticalProject );

				// Allocate the Transport to this UnitActionIdea
				pAITransportActionIdea->GetAIPoiObjectActor()->SetAllocation( pAITransportActionIdea );
				pSuggestedTransportMovementDesc->GetAIPoiActor()->TransportAllocatedToTacticalProject( m_pTacticalProject );
				// Set the ActionIdea to point to the Transport
				pUnitActionIdea->SetAITransportActionIdea( pAITransportActionIdea );
			}
			*-/
		}
		*/

		if( bAllocateThisUnit )
		{
			pUnitActionIdea->GetJobRole()->m_bCombat = m_JobsToDo.CanFulfillASpecificCombatJob( pAIUnitTemplate );

			// DEBUG
			long iAllocatedSpace = -1;
			long iPreAllocatedSpace = -1;
			if( pUnitStackItem->GetSuggestedTransportMoveDesc() &&
				pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport )
			{
				bool b = pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport->CanInsertTransportIdea( this, pUnitStackItem->m_pUnitActionIdea );
				ASSERT( b );

				CEOSAITransportSpaceAllocation* pAITransportSpaceAllocation =
					pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport->GetTransportSpaceAllocationObj( this );
				iAllocatedSpace = pAITransportSpaceAllocation->m_iAllocatedSpace;
				iPreAllocatedSpace = pAITransportSpaceAllocation->m_iPreAllocatedSpace;
			}
			// DEBUG

			if( m_eBestCaseOrRealTaskForce == RealTaskForce &&
				pUnitStackItem->GetAIUnitActor() &&
				pUnitStackItem->GetAIUnitActor()->GetObjectId() == 143 )
			{
				// Lookup the BestCase end time
				//   Add 30% to the BestCase end time, and use it as the 'target time' for the negotiated units?
				int g=0;
				//why does Infantry-143 think it needs a transport for this?
			}

			pUnitStackItem->Allocate( true );

			bool bGoIntoFunction = false;
			if( bGoIntoFunction )
			{
				// DEBUG
				if( pUnitStackItem->GetSuggestedTransportMoveDesc() &&
					pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport )
				{
					bool b = pUnitStackItem->GetSuggestedTransportMoveDesc()->m_pAITransport->CanInsertTransportIdea( this, pUnitStackItem->m_pUnitActionIdea );
					ASSERT( b );
				}
				// DEBUG

				SimpleCalc_SetSuggestedTransport( pUnitStackItem );
			}
			//OptimizeTransportAndGroundUnitMatchup();

			// If NonCombat Jobs are Fulfilled, and Combat Jobs are Fulfilled, then start recording Scores
			//    I'd like to know the best combination of units, and it probably won't happen
			//    immediately after the jobs are fulfilled.  Rather, it might be a few units later.
			float fMinimumNonCombatJobFulfillment02 = m_JobsToDo.GetMinimumNonCombatFulfillment02( &m_JobCapabilityOfAllocatedUnits );
			if( fMinimumNonCombatJobFulfillment02 >= 1.0f )
			{
				float fCurrentTime = pUnitStackItem->m_fArrivalTime;
				if( bFirstCompletionTimeFound == false )
				{
					bFirstCompletionTimeFound = true;
					fFirstCompletionTime = pUnitStackItem->m_fArrivalTime;
				}
				// Check Combat Job Fulfillment

				// The NewScore might be lower - if it takes too much extra time
				CalculateScores();
				float fNewScore = GetRealScore();
				if( fNewScore >= 0.0f )
				{
					if( m_fBestCaseRealScore < fNewScore ){ m_fBestCaseRealScore = fNewScore; }

					// We have a viable task force
					// Store the score (we might want to return to this state)
					//m_TimeAndRealScores.AddTail( new CTimeAndRealScore( fCurrentTime,fNewScore ) );
				}

				// Once a task is overfulfilled, stop trying to fulfill it
				//   Note: this logic only applies to tasks with no combat (it should be chnaged)
				if( GetTaskInvolvesCombat() == false &&
					fMinimumNonCombatJobFulfillment02 >= 2.0 )
				{
					return;
				}
				if( GetTaskInvolvesCombat() == true &&
					fMinimumNonCombatJobFulfillment02   >= 2.0f &&
					m_CombatResultsOfAllocatedUnits.m_fCombatSuccessValue02 >= 1.0f )
					//m_CombatResults.m_fInvasionStrategy_CombatSuccess02 >= 1.0f )
				{
					return;
				}

				#ifdef _DEBUG
				CalculateScores(); // DEBUG

				float fCombatSuccess1 = m_CombatResultsOfAllocatedUnits.m_fCombatSuccessValue02; //m_fInvasionStrategy_CombatSuccess02;
				//float fCombatSuccess2 = m_SeedCombatResults.m_fCombatSuccessValue02; //m_fSeedStrategy_CombatSuccess02;
				if( fCombatSuccess1 == 1.0f ) // Maxed out (doesn't always happen)
				{
					int h=0;
				}
				float fGrowthScore = GetGrowthScore();
				float fRealScore = GetRealScore();
				#endif _DEBUG

				int g=0;

				//m_fCompletionTime = pUnitStackItem->m_fArrivalTime;

				// What's the score?  

				// Once the job fulfillment happens, it doesn't mean I should stop adding units to the list
				// Rather, I want to find the best score - which means adding later units
				// Also, the GrowthScore doesn't require that the JobFulfillment = 1.0

				// I also need to setup the 'role' correctly.
				// Fortunately, I can use the CombatSignificance information here (above 0.35 or 0.4?)

				//return;
			}
		}
	}
}

void CEOSAITaskForce3::TryInsertingBuildActionsIntoStack()
{
	// Try adding CityBuild items to the stack to satisfy or improve the TaskForce
	long iCount = 0;
	bool bTryCreatingBuildAction = true;
	while( bTryCreatingBuildAction )
	{
		bTryCreatingBuildAction = TryInsertingOneBuildActionIntoStack(); // return true if something useful was added
		iCount++;
		if( iCount > 5 ) break; // this is to prevent infinite looping
	}
}


void CEOSAITaskForce3::GetCityBuildArrivalTimes( CEOSAIUnitTemplate* pUnitTemplate, CList< CEOSAICityAndTime >* pCityAndTimeList )
{
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();

	// Iterate over the cities, figure out how long it will take for a built pUnitTemplate to arrive
	// Create an imaginary unit (so we can do all the terrain calculations)
	//CEOSAIUnit2  UnitActor( m_pTacticalProject->GetAIBrain() );
	//UnitActor.CreateHypotheticalUnit( pUnitTemplate, iAIPlayer, CEOSAILocation( Pixel,0,0 ), 0.0f );
	CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIBrain()->GetAIThoughtDatabase();
	//POSITION pos = m_pTacticalProject->GetCityActionIdeaNodes()->GetHeadPosition();
	POSITION pos = pAIThoughtDatabase->GetMyCities()->GetHeadPosition();
	while( pos )
	{
		CEOSAICity* pAICity = pAIThoughtDatabase->GetMyCities()->GetNext( pos );
		//CEOSAICityActionIdeaNode* pCityActionIdea = m_pTacticalProject->GetCityActionIdeaNodes()->GetNext( pos );
		//CEOSAICity* pAICity = pCityActionIdea->m_pAICity;

		//float fBuildAndMoveTime = pCityActionIdea->GetBuildTimeAndTimeToTarget( pUnitTemplate );
		float fBuildAndMoveTime;
		/*
		if( m_pTacticalProject )
		{
			fBuildAndMoveTime = pAICity->GetBuildTimeAndTimeToTarget( m_pTacticalProject, pUnitTemplate );
		}
		*/
		if( m_pTacticalProject2 )
		{
			fBuildAndMoveTime = pAICity->GetBuildTimeAndTimeToTarget( m_pTacticalProject2, pUnitTemplate );
		}

		// Add the information
		//CCityUnitTemplateTime* pUnitTemplateTime = new CCityUnitTemplateTime();
		CEOSAICityAndTime CityAndTime;
		//CityAndTime.m_pCityActionNode = pCityActionIdea;
		CityAndTime.m_pAICity = pAICity;
		CityAndTime.m_fBuildAndMoveTime = fBuildAndMoveTime;
		pCityAndTimeList->AddTail( CityAndTime );
	}
}

void CEOSAITaskForce3::SimpleCalc_SetSuggestedTransport( CEOSAIUnit2ActionStackItem* pUnitStackItem )
{
	CEOSAIUnit2ActionIdea* pUnitActionIdea = pUnitStackItem->m_pUnitActionIdea;

	// Use the best possible transport (ignore competition for this transport)
	if( m_eBestCaseOrRealTaskForce == BestCaseTaskForce )
	{
		CEOSAIUnit2TransportMovementDesc* pBestMovementDesc = NULL;
		//pUnitActionIdea->GetTransportIdeas( CEOSAIUnit2ActionIdea::EnumMovementDescFilter_BestPossible, this, &pBestMovementDesc );
		pUnitActionIdea->GetTransportIdeas2( CEOSAIUnit2ActionIdea::EnumMovementDescFilter_BestAvailable, this, &pBestMovementDesc );

		if( pBestMovementDesc == NULL ) return;
		if( pUnitStackItem->GetSuggestedTransportMoveDesc() != pBestMovementDesc )
		{
			pUnitStackItem->SetSuggestedTransportMoveDesc( pBestMovementDesc );
		}
	}
	// Use the best available transport
	//   (I could negotiate for a better transport, but that is not yet implimented)
	if( m_eBestCaseOrRealTaskForce == RealTaskForce )
	{
		CEOSAIUnit2TransportMovementDesc* pBestMovementDesc = NULL;
		pUnitActionIdea->GetTransportIdeas2( CEOSAIUnit2ActionIdea::EnumMovementDescFilter_BestAvailable, this, &pBestMovementDesc );

		if( pBestMovementDesc == NULL ) return;
		if( pUnitStackItem->GetSuggestedTransportMoveDesc() != pBestMovementDesc )
		{
			pUnitStackItem->SetSuggestedTransportMoveDesc( pBestMovementDesc );
			// I really need to restart the stack-iteration
		}
	}
}

void CEOSAITaskForce3::OptimizeTransportAndGroundUnitMatchup()
{
	ASSERT( false ); // Not being used
	EnumTransportStrategy  eTransportStrategy = GroundUnitArrivalASAP;
	if( eTransportStrategy == GroundUnitArrivalASAP ) // Don't coordinate transport arrival times
	{
		POSITION pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit2ActionStackItem* pUnitStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
			CEOSAIUnit2ActionIdea* pUnitActionItem = pUnitStackItem->m_pUnitActionIdea;

			CEOSAIUnit2TransportMovementDesc* pMoveDesc = pUnitStackItem->GetSuggestedTransportMoveDesc(); // disconnect all suggested transport information
			if( pMoveDesc && pMoveDesc->m_pAITransport )
			{
				pUnitStackItem->ClearSuggestedTransportMoveDesc();
				//pUnitStackItem->SetSuggestedTransportMoveDesc( NULL );
				pMoveDesc->m_pAITransport->DeAllocateTransportSpace( this,pUnitActionItem );
			}
		}

		// Iterate over all UnitsActionIdeas needing a Transport, 
		// Find the fastest GroundUnit+Transport combo (using only transports that are available)
		// Allocate it
		// Repeat
		bool bKeepMatchingTransportsAndGroundUnits = true;
		while( bKeepMatchingTransportsAndGroundUnits )
		{
			long iGroundUnitsWithoutTransports = 0;
			CEOSAIUnit2ActionStackItem* pBestUnitStackItem = NULL;
			CEOSAIUnit2TransportMovementDesc* pBestAvailable1 = NULL;
			pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
			while( pos )
			{
				CEOSAIUnit2ActionStackItem* pUnitStackItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
				CEOSAIUnit2ActionIdea* pUnitActionItem = pUnitStackItem->m_pUnitActionIdea;

				if( pUnitStackItem->ActorIsAllocatedByThisTaskForce() &&
					pUnitActionItem->RequiresTransport() &&
					pUnitStackItem->GetSuggestedTransportMoveDesc() == NULL )
				{
					iGroundUnitsWithoutTransports++;

					CEOSAIUnit2ActionIdea* pUnitActionIdea = pUnitStackItem->m_pUnitActionIdea;

					CEOSAIUnit2TransportMovementDesc* pBestAvailable2 = NULL;
					pUnitActionIdea->GetTransportIdeas2( CEOSAIUnit2ActionIdea::EnumMovementDescFilter_BestAvailable, this, &pBestAvailable2 );
					if( pBestAvailable2 )
					{
						if( pBestAvailable1 == NULL ||
						  ( pBestAvailable1 && pBestAvailable1->m_fTotalTime < pBestAvailable2->m_fTotalTime ) )
						{
							pBestUnitStackItem = pUnitStackItem;
							pBestAvailable1 = pBestAvailable2;
						}
					}
					/*
					CEOSAIUnit2TransportMovementDesc* pBestPossible2 = NULL;
					CEOSAIUnit2TransportMovementDesc* pBestAvailable2 = NULL;
					pUnitActionIdea->GetTransportIdeas( this, &pBestPossible2, &pBestAvailable2 );

					if( pBestAvailable2 )
					{
						if( pBestAvailable1 == NULL ||
						  ( pBestAvailable1 && pBestAvailable1->m_fTotalTime < pBestAvailable2->m_fTotalTime ) )
						{
							pBestUnitStackItem = pUnitStackItem;
							pBestAvailable1 = pBestAvailable2;
						}
					}
					*/
				}
			}
			// Setup the Suggested Transport
			if( pBestAvailable1 && pBestUnitStackItem )
			{
				pBestUnitStackItem->SetSuggestedTransportMoveDesc( pBestAvailable1 );
				if( pBestAvailable1->m_pAITransport )
				{
					pBestAvailable1->m_pAITransport->AllocateTransportSpace( this,pBestUnitStackItem->m_pUnitActionIdea );
				}
				iGroundUnitsWithoutTransports--;
			}
			if( pBestAvailable1 == NULL || iGroundUnitsWithoutTransports == 0 )
			{
				bKeepMatchingTransportsAndGroundUnits = false;
			}
		}
	}
	/*
	if( m_eTransportStrategy == GroundUnitSimultaneousArrival ) // Coordinate transport arrival times
	{
		ASSERT( false );  // Not yet implimented
	}
	*/
}
/*
CTimeAndRealScore*  CEOSAITaskForce3::GetBestTimeAndRealScore()
{
	CTimeAndRealScore* pBest = NULL;
	POSITION pos = m_TimeAndRealScores.GetHeadPosition();
	while( pos )
	{
		CTimeAndRealScore* pCurrent = m_TimeAndRealScores.GetNext( pos );
		if( pBest == NULL )
		{
			pBest = pCurrent;
		}
		else if( pBest->m_fRealScore < pCurrent->m_fRealScore )
		{
			pBest = pCurrent;
		}
	}
	return pBest;
}
*/

void CEOSAITaskForce3::UpdateScoresIfNecessary()
{
	if( m_bNeedToUpdateScores ){ CalculateScores(); }
}

// Results
// The best scores accomplish these tasks:
//   (1) All jobs must be satisfied
//   (2) Get jobs done quickly (sooner is better than later) (At N turns, value might drop to 0)
//   (3) Low attrition
//   (4) Perferably bring a little extra combat force (just to make sure I win)
//   (5) If I build something, it would be better if I can reuse later for other tasks
//
void CEOSAITaskForce3::CalculateScores()
{
	ASSERT( m_pTacticalProject2 );

	// Scoring Tactical Project Strategies (Growth + Real Scores):
	// - TacticalProject Simple Interest
	// - Do I have enough units to complete the task?
	// - When can I get my units there?
	//   - Filter this through the time-score function
	// - Am I using appropriate units (attrition, using transports for transport tasks)

	// What's the value of the TacticalProject?
	float fTacticalProjectSimpleInterest = GetTacticalProject2()->GetSimpleInterest();//>GetInitialEvaluationScore();
	float fTacticalProjectInitialEvalBasicScore = GetTacticalProject2()->GetInitialEvaluation_BasicScore();//>GetInitialEvaluationScore();
	float fTacticalProjectResistancePower = GetTacticalProject2()->GetResistanceCombatSignificance();

	// Calculate a score based on Attrition, TimeValue, Reinforcements, ChanceOfSuccess (Combat), etc
	//
		// Completion Time
		m_fCompletionTime = m_UnitActionStack.GetLastAllocatedItemTime();
		ASSERT( m_fCompletionTime >= 0.0f && m_fCompletionTime <= 1000000.0f );
		float fTimeValue = m_TimeValueFunction.GetOutput( m_fCompletionTime );

		// Shore Arrival Time (if I'm using transports, and this is a secret invasion)
		/*
		if( m_UnitActionStack.InvolvesTransports() )
		{
			m_fShoreArrivalTime = m_UnitActionStack.GetLastShoreArrivalTime();
		}
		*/

		// NonCombat Fulfillment
		m_fMinimumNonCombatFulfillment02 = m_JobsToDo.GetMinimumNonCombatFulfillment02( &m_JobCapabilityOfAllocatedUnits );
		m_fAverageNonCombatFulfillment01 = m_JobsToDo.GetAverageNonCombatFulfillment01( &m_JobCapabilityOfAllocatedUnits );

		CalculateCombatSuccess();
		float fGrowthCombatScore = GetGrowthCombatScore();
		float fRealCombatScore = GetRealCombatScore();

	// 1.0 = all the appropriate units
	//   This is based on: attrition values, using transports as transports
	float fAppropriateUnitScore01 = 1.0f;
	POSITION pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pItem->ActorIsAllocatedByThisTaskForce() )
		{
			float fAttritionScore01 = pItem->GetAIUnitTemplate()->GetAIUnitCombatCapability()->GetCombatAttritionSignificance01();
			fAppropriateUnitScore01 *= 0.9f + 0.1f*fAttritionScore01;

			if( pItem->GetAIUnitTemplate()->IsTransport() )
			{
				fAppropriateUnitScore01 *= 0.01f;
			}
			if( pItem->m_pUnitActionIdea &&
				pItem->m_pUnitActionIdea->GetAIUnitActor() )
			{
				CEOSAIUnit2* pAIUnitActor = pItem->m_pUnitActionIdea->GetAIUnitActor();
				fAppropriateUnitScore01 -= 0.6f * pAIUnitActor->GetNeedForRepair01BasedOnDistanceAndDamage();
				fAppropriateUnitScore01 -= 0.4f * pAIUnitActor->GetDesireForUpgrade01();
			}
		}
	}

	m_fGrowthScore =
		((2.0f+fTacticalProjectSimpleInterest)/2.0f) *
		((20.0f+fTacticalProjectInitialEvalBasicScore)/20.0f) *
		(40.0f/(fTacticalProjectResistancePower+40.0f)) *
		fTimeValue *
		m_fAverageNonCombatFulfillment01 *
		fGrowthCombatScore *
		fAppropriateUnitScore01;
	m_fRealScore =
		((2.0f+fTacticalProjectSimpleInterest)/2.0f) *
		((20.0f+fTacticalProjectInitialEvalBasicScore)/20.0f) *
		(40.0f/(fTacticalProjectResistancePower+40.0f)) *
		fTimeValue *
		min( 1.0f, m_fMinimumNonCombatFulfillment02 ) *
		fRealCombatScore *
		fAppropriateUnitScore01;
	m_bNeedToUpdateScores = false;
}

void CEOSAITaskForce3::CalculateEnemyUnitCount()
{
	m_iNumberOfPlayer0Units = 0;
	m_iNumberOfPlayerNUnits = 0;
	m_TargetUnitsList.RemoveAll();
	//m_CompleteTargetUnitsList.RemoveAll();

	POSITION pos = m_JobsToDo.m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pAIUnit = m_JobsToDo.m_EnemyUnits.GetNext( pos );
		//m_CompleteTargetUnitsList.AddTail( pUnit );

		if( pAIUnit->GetOwner() == 0 )
		{
			if( m_iNumberOfPlayer0Units == 0 ){ m_TargetUnitsList.AddTail( pAIUnit ); }
			m_iNumberOfPlayer0Units++;
		}
		if( pAIUnit->GetOwner() > 0 && m_PlayersToTreatAsEnemies[ pAIUnit->GetOwner() ] )
		{
			m_TargetUnitsList.AddTail( pAIUnit );
			m_iNumberOfPlayerNUnits++;
		}
	}
}

// CombatsSuccess is based on several things:
//   1) Don't include my non-combat units in the "Attackers" list
//   2) Calculate a "Seed" strategy (only the minimum 'must kill' list) and Invasion strategy (kill all)
void CEOSAITaskForce3::CalculateCombatSuccess()
{
	POSITION pos;
	/*
	// Split all enemy units into Minimum-Target-List and Complete Target List
	CList< CUnit* >  MinimumTargetUnitsList;
	CList< CUnit* >  CompleteTargetUnitsList;

	// Seed Strategy: Read over the cities, do any/all of them have militia?
	//   Possibilities:
	//   (1) When I destroy PlayerN units, I'll have control of a city
	//   (2) There's a Player0/PlayerN city with no units
	// I could figure out if I really need to fight one militia to capture a city, but
	//   for simplicity, I'll just say 'yes'
	POSITION pos = m_JobsToDo.m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CUnit* pUnit = m_JobsToDo.m_EnemyUnits.GetNext( pos );
		CompleteTargetUnitsList.AddTail( pUnit );

		if( pUnit->GetOwner() == 0 )
		{
			if( MinimumTargetUnitsList.IsEmpty() ){ MinimumTargetUnitsList.AddTail( pUnit ); }
			m_iNumberOfPlayer0Units++;
		}
		if( pUnit->GetOwner() > 0 && m_PlayersToTreatAsEnemies[ pUnit->GetOwner() ] )
		{
			MinimumTargetUnitsList.AddTail( pUnit );
			m_iNumberOfPlayerNUnits++;
		}
	}
	*/

	if( m_iNumberOfPlayer0Units == 0 && m_iNumberOfPlayerNUnits == 0 )
	{
		// There is no combat needed (maybe it's a "Capture Resource" task)
		//m_fInvasionStrategy_CombatSuccess02 = 2.0f;
		//m_fInvasionStrategy_AttritionBalance01 = 1.0f;
		//m_fSeedStrategy_CombatSuccess02 = 2.0f;
		//m_fSeedStrategy_AttritionBalance01 = 1.0f;

		m_CombatResultsOfAllocatedUnits.m_fCombatSuccessValue02 = 2.0f;
		m_CombatResultsOfAllocatedUnits.m_fAttritionBalance01 = 1.0f;
		//m_SeedCombatResults.m_fCombatSuccessValue02 = 2.0f;
		//m_SeedCombatResults.m_fAttritionBalance01 = 1.0f;

		return;
	}

	// Calculate Combat Balance
	//bool bInvasionAndSeedStrategyAreTheSame = true;
	//if( m_iNumberOfPlayer0Units > 1 && m_iNumberOfPlayerNUnits == 0 )
	//if( m_MinimalTargetUnitsList.GetCount() == m_CompleteTargetUnitsList.GetCount() )
	//{
	//	bInvasionAndSeedStrategyAreTheSame = false;
	//}

	// Invasion (fight-off all enemies), or seed (capture one city, expand over non-owned cities)
	//   Because all enemy players can counterattack, they must be counted as defenders.
	//   But Player0 units aren't aggressive - but do eventually have to be fought (maybe after capture+build)
	// (1) Compile a list of all the enemy units (not including Player0 units) - these must be fought
	// (2) Are there any cities I can capture without militia?  Which ones?
	// (3) If all cities have militia, then add one city (+ its militia) to the required targets.


	// Calculate minimal-combat
	//CEOSAIQuickCombatCalculation  InvasionCombatCalc;
	m_CombatCalculation.ClearAll();
	pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionStackItem* pItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
		if( pItem->ActorIsAllocatedByThisTaskForce() == false ) continue;
		if( pItem->m_pUnitActionIdea->GetJobRole()->m_bCombat == false ) continue;
		if( pItem->GetAIUnitActor() )
		{
			m_CombatCalculation.AddToAttackers( pItem->GetAIUnitActor() );
		}
		else
		{
			m_CombatCalculation.AddToAttackers( pItem->GetAIUnitTemplate() );
		}
	}
	pos = m_TargetUnitsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pAIUnit = m_TargetUnitsList.GetNext( pos );
		m_CombatCalculation.AddToDefenders( pAIUnit );
	}
	//
	m_CombatCalculation.RunCalculation();
	m_CombatCalculation.SaveState( &m_CombatResultsOfAllocatedUnits );
	//m_fInvasionStrategy_CombatSuccess02 = InvasionCombatCalc.GetAttackerSuccessValue02();
	//m_fInvasionStrategy_AttritionBalance01 = InvasionCombatCalc.GetAttritionBalance01();

	/*
	if( bInvasionAndSeedStrategyAreTheSame )
	{
		// Invasion and Seed fight the exact same units, so they will be identical
		//m_fSeedStrategy_CombatSuccess02 = m_fInvasionStrategy_CombatSuccess02;
		//m_fSeedStrategy_AttritionBalance01 = m_fInvasionStrategy_AttritionBalance01;
		InvasionCombatCalc.SaveState( &m_SeedCombatResults );
		return;
	}
	//if( m_fInvasionStrategy_CombatSuccess02 >= 1.0f )
	if( m_CombatResults.m_fCombatSuccessValue02 >= 1.0f )//m_fInvasionStrategy_CombatSuccess02 >= 1.0f )
	{
		// The invasion is killing all the enemy units
		// Don't calculate the seed strategy
		//m_fSeedStrategy_CombatSuccess02 = m_fInvasionStrategy_CombatSuccess02;
		//m_fSeedStrategy_AttritionBalance01 = m_fInvasionStrategy_AttritionBalance01;
		InvasionCombatCalc.SaveState( &m_SeedCombatResults );
		return;
	}

	// Seed and Invasion aren't the same thing.
	// If the Invasion strategy is not defeating all the units, calculate the seed strategy
	// The seed strategy includes only one Player0 unit, and all PlayerN units
	//if( m_fInvasionStrategy_CombatSuccess02 < 1.0f ) 
	if( m_CombatResults.m_fCombatSuccessValue02 < 1.0f )
	{
		//CEOSAIQuickCombatCalculation  MinimumCombatCalc;
		pos = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit2ActionStackItem* pItem = m_UnitActionStack.m_ItemsSortedByArrivalTime.GetNext( pos );
			if( pItem->ActorIsAllocatedByThisTaskForce() == false ) continue;
			if( pItem->m_pUnitActionIdea->GetJobRole()->m_bCombat == false ) continue;
			if( pItem->GetUnitActor() )
			{
				MinimumCombatCalc.AddToAttackers( pItem->GetUnitActor() );
			}
			else
			{
				MinimumCombatCalc.AddToAttackers( pItem->GetUnitTemplate() );
			}
		}
		pos = m_MinimalTargetUnitsList.GetHeadPosition();
		while( pos )
		{
			CUnit* pUnit = m_MinimalTargetUnitsList.GetNext( pos );
			MinimumCombatCalc.AddToDefenders( pUnit );
		}
		//
		MinimumCombatCalc.RunCalculation();
		MinimumCombatCalc.SaveState( &m_SeedCombatResults );
		//m_fSeedStrategy_CombatSuccess02 = MinimumCombatCalc.GetAttackerSuccessValue02();
		//m_fSeedStrategy_AttritionBalance01 = MinimumCombatCalc.GetAttritionBalance01();
		int j0=0;
	}
	*/
}

//float CEOSAITaskForce3::GetCombatBasedScoreMultiplier()
float CEOSAITaskForce3::GetGrowthCombatScore()
{
	//float fInvasionStrategy = m_fInvasionStrategy_CombatSuccess02 * (2.0f * m_fInvasionStrategy_AttritionBalance01);
	//float fSeedStrategy = m_fSeedStrategy_CombatSuccess02 * (2.0f * m_fSeedStrategy_AttritionBalance01);
	float fInvasionStrategy = 
		m_CombatResultsOfAllocatedUnits.m_fCombatSuccessValue02 * 
		(2.0f * m_CombatResultsOfAllocatedUnits.m_fAttritionBalance01);
	//float fSeedStrategy = m_SeedCombatResults.m_fCombatSuccessValue02 * (2.0f * m_SeedCombatResults.m_fAttritionBalance01);
	//return ( 0.4f * fInvasionStrategy ) + ( 0.6f * fSeedStrategy );
	return fInvasionStrategy;
}

float CEOSAITaskForce3::GetRealCombatScore()
{
	//if( m_fInvasionStrategy_CombatSuccess02 >= 1.0f )
	if( m_CombatResultsOfAllocatedUnits.m_fCombatSuccessValue02 >= 1.0f )
	{
		//return m_fInvasionStrategy_CombatSuccess02 * (2.0f * m_fInvasionStrategy_AttritionBalance01);
		return m_CombatResultsOfAllocatedUnits.m_fCombatSuccessValue02 * 
			(2.0f * m_CombatResultsOfAllocatedUnits.m_fAttritionBalance01);
	}
	/*
	//if( m_fSeedStrategy_CombatSuccess02 >= 1.0f )
	if( m_SeedCombatResults.m_fCombatSuccessValue02 >= 1.0f )
	{
		//float fInvasionStrategy = m_fInvasionStrategy_CombatSuccess02 * (2.0f * m_fInvasionStrategy_AttritionBalance01);
		//float fSeedStrategy = m_fSeedStrategy_CombatSuccess02 * (2.0f * m_fSeedStrategy_AttritionBalance01);
		float fInvasionStrategy = m_CombatResults.m_fCombatSuccessValue02 * (2.0f * m_CombatResults.m_fAttritionBalance01);
		//float fSeedStrategy = m_SeedCombatResults.m_fCombatSuccessValue02 * (2.0f * m_SeedCombatResults.m_fAttritionBalance01);

		// Give some extra points as we get closer to a viable invasion strategy
		return ( 0.1f * fInvasionStrategy ) + ( 0.9f * fSeedStrategy );
	}
	*/
	return 0.0f;
}

void  CEOSAITaskForce3::GetValueOfAddingAttackerToCombatForce( CEOSAIUnitTemplate* pAttackerAIUnitTemplate, CEOSAIQuickCombatResults* pNewCombatResults )
{
	m_CombatCalculation.AddToAttackers( pAttackerAIUnitTemplate );
	m_CombatCalculation.RunCalculation();
	m_CombatCalculation.SaveState( pNewCombatResults );

	m_CombatCalculation.RemoveFromAttackers( pAttackerAIUnitTemplate );
}




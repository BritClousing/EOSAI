
#include <stdafx.h>
#include "EOSAIThoughtDatabase.h"
//#include "AIThought_WhatCityHuntersCanIBuild.h"
#include "EOSAIBrain.h"
//#include "WorldDesc.h"
//#include "WorldDescPlayer.h"
#include "AIPlayer.h"
#include "EOSAIGamePlayer.h"
#include "EOSAIBuildOption.h"
#include "EOSAIUnitTemplate.h"
//#include "AIThought.h"
#include "EOSAIMultiRegion2.h"
#include "City.h"
#include "EOSAIAirfield.h"
//#include "City.h"
#include "EOSAIUnit.h"
//#include "WorldDescServer.h"
//#include "AIThought_AIPoiObjectFilter.h"
//#include "AIThought_WhatPlacesWouldILikeToExplore.h"
//#include "9MovementPathwaysFromEverywhereToMRX.h"
//#include "AIThought_ShouldSendUnitsHere.h"
#include "EOSAIActionRequirement.h"
//#include "AIThought_WhatUnitsCanIGetToMultiRegion.h"
//#include "AITacticalProject.h"
#include "EOSAITacticalProject2.h"
//#include "AIActionDefinition_CaptureCitRes.h"
//#include "AITransportCoordinator.h"
//#include "AICityBuildAction.h"
#include "EOSAIMultiRegionNationwidePathways.h"
//
#include "EOSAIDesireCaptureCitRes.h"
#include "EOSAIDesireCaptureAirfield.h"
#include "EOSAIDesireDamageCity.h"
#include "EOSAIDesireDestroyUnit.h"
#include "EOSAIStopwatch2.h"
//#include "Checkpoint.h"
#include "EOSAIAirbasesSet.h"
//#include "TWDx.h"
#include "EOSAIBCDumbArray2D.h"
#include "EOSAIUnitVsUnitValue.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAIGeo.h"
#include "EOSAICommonData.h"
#include "EOSAIWarzoneLocation.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAIMultiRegionPlan.h"
#include "EOSAIMain.h"
class CBuildCompletion;
#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
bool SortByOrderOfProcessing( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 );
bool SortDesiresDescByScore( CEOSAIDesireSpatial* pDesire1, CEOSAIDesireSpatial* pDesire2 )
{
	if( pDesire1->GetScore() > pDesire2->GetScore() ){ return true; }
	else if( pDesire1->GetScore() < pDesire2->GetScore() ){ return false; }
	return true;  // always return true when values are equal
}

bool SortDesiresDescBySimpleInterest( CEOSAIDesireSpatial* pDesire1, CEOSAIDesireSpatial* pDesire2 )
{
	if( pDesire1->GetSimpleInterest() > pDesire2->GetSimpleInterest() ){ return true; }
	else if( pDesire1->GetSimpleInterest() < pDesire2->GetSimpleInterest() ){ return false; }
	return true;  // always return true when values are equal
}

// Large values first
bool SortTacticalProjectsBySimpleInterest( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	if( p1->GetSimpleInterest() > p2->GetSimpleInterest() ){ return true; }
	else if( p1->GetSimpleInterest() < p2->GetSimpleInterest() ){ return false; }
	return true;  // always return true when values are equal
}
/*
bool SortTacticalProjectsBySimpleInterestAndAIPoiObjectRelativeScore( CEOSAIUnitActionIdea* p1, CEOSAIUnitActionIdea* p2 )
{
	if( p1->m_fTTTScoreAndAIPoiObjectRelativeScore > p2->m_fTTTScoreAndAIPoiObjectRelativeScore ){ return true; }
	else if( p1->m_fTTTScoreAndAIPoiObjectRelativeScore < p2->m_fTTTScoreAndAIPoiObjectRelativeScore ){ return false; }
	return true;  // always return true when values are equal
}
*/
/*
bool SortTacticalProjectsByInitialEvaluationScore( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	// Highest values first
	if( p1->GetInitialEvaluationScore() > p2->GetInitialEvaluationScore() ){ return true; }
	else if( p1->GetInitialEvaluationScore() < p2->GetInitialEvaluationScore() ){ return false; }
	return true;  // always return true when values are equal
}
*/

bool SortTacticalProjectsByInitialEvaluationNearbyScore( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	// Highest values first
	if( p1->GetInitialEvaluation_NearbyScore() > p2->GetInitialEvaluation_NearbyScore() ){ return true; }
	else if( p1->GetInitialEvaluation_NearbyScore() < p2->GetInitialEvaluation_NearbyScore() ){ return false; }
	return true;  // always return true when values are equal
}

bool SortTacticalProjectsByInitialEvaluationValueScore( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	// Highest values first
	if( p1->GetInitialEvaluation_ValueScore() > p2->GetInitialEvaluation_ValueScore() ){ return true; }
	else if( p1->GetInitialEvaluation_ValueScore() < p2->GetInitialEvaluation_ValueScore() ){ return false; }
	return true;  // always return true when values are equal
}

bool SortTacticalProjectsByInitialEvaluationBasicScore( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	// Highest values first
	if( p1->GetInitialEvaluation_BasicScore() > p2->GetInitialEvaluation_BasicScore() ){ return true; }
	else if( p1->GetInitialEvaluation_BasicScore() < p2->GetInitialEvaluation_BasicScore() ){ return false; }
	return true;  // always return true when values are equal
}

bool SortTacticalProjectsByInitialEvaluationEasyTargetScore( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	// Highest values first
	if( p1->GetInitialEvaluation_EasyTargetScore() > p2->GetInitialEvaluation_EasyTargetScore() ){ return true; }
	else if( p1->GetInitialEvaluation_EasyTargetScore() < p2->GetInitialEvaluation_EasyTargetScore() ){ return false; }
	return true;  // always return true when values are equal
}

bool SortTacticalProjectsByInitialEvaluationNoResistanceScore( CEOSAITacticalProject2* p1, CEOSAITacticalProject2* p2 )
{
	// Highest values first
	if( p1->GetInitialEvaluation_NoResistanceScore() > p2->GetInitialEvaluation_NoResistanceScore() ){ return true; }
	else if( p1->GetInitialEvaluation_NoResistanceScore() < p2->GetInitialEvaluation_NoResistanceScore() ){ return false; }
	return true;  // always return true when values are equal
}

/*
bool SortTacticalProjectsByBestCaseTaskForceScore( CAITacticalProject* p1, CAITacticalProject* p2 )
{
	// Highest values first
	if( p1->GetBestCaseTaskForceScore() > p2->GetBestCaseTaskForceScore() ){ return true; }
	else if( p1->GetBestCaseTaskForceScore() < p2->GetBestCaseTaskForceScore() ){ return false; }
	return true;  // always return true when values are equal
}
*/
/*
bool SortTacticalProjectStrategiesByBestCaseValue( CAITacticalProjectStrategy* p1, CAITacticalProjectStrategy* p2 )
{
	// Highest values first
	if( p1->GetBestCaseTaskForceValue() > p2->GetBestCaseTaskForceValue() ){ return true; }
	else if( p1->GetBestCaseTaskForceValue() < p2->GetBestCaseTaskForceValue() ){ return false; }
	return true;  // always return true when values are equal
}
*/
bool SortTacticalProject2StrategiesByBestCaseValue( CEOSAITacticalProject2Strategy* p1, CEOSAITacticalProject2Strategy* p2 )
{
	// Highest values first
	if( p1->GetBestCaseTaskForceValue() > p2->GetBestCaseTaskForceValue() ){ return true; }
	else if( p1->GetBestCaseTaskForceValue() < p2->GetBestCaseTaskForceValue() ){ return false; }
	return true;  // always return true when values are equal
}

////////////////////////////////////////

CEOSAIThoughtDatabase::CEOSAIThoughtDatabase()
{
	m_pAIBrain = NULL;

	//m_AIFutureBuildIdeas.SetAIThoughtDatabase( this );

	m_DesiresSortedByScore.SetSortFunction( SortDesiresDescByScore );

	m_TacticalProjects2ByInitialEvaluation_NearbyScore.SetSortFunction( SortTacticalProjectsByInitialEvaluationNearbyScore );
	m_TacticalProjects2ByInitialEvaluation_ValueScore.SetSortFunction( SortTacticalProjectsByInitialEvaluationValueScore );
	m_TacticalProjects2ByInitialEvaluation_BasicScore.SetSortFunction( SortTacticalProjectsByInitialEvaluationBasicScore );
	m_TacticalProjects2ByInitialEvaluation_EasyTargets.SetSortFunction( SortTacticalProjectsByInitialEvaluationEasyTargetScore );
	m_TacticalProjects2ByInitialEvaluation_NoResistance.SetSortFunction( SortTacticalProjectsByInitialEvaluationNoResistanceScore );

	//m_SortedBestCaseTacticalProjectStrategies.SetSortFunction( SortTacticalProjectStrategiesByBestCaseValue );
	//m_TacticalProject2StrategiesSortedByBestCaseValue.SetSortFunction( SortTacticalProject2StrategiesByBestCaseValue );
	m_TacticalProject2StrategiesSortedByBestCaseValue.SetSortFunction( SortTacticalProject2StrategiesByBestCaseValue );
}

CEOSAIThoughtDatabase::~CEOSAIThoughtDatabase()
{
	Clear();
}

void CEOSAIThoughtDatabase::Clear()
{
	// Units
	//
	m_AllUnitsInTheGameOrBuildableByMe.Clear();
	m_PotentialEnemyUnits.Clear();
	m_UnitsICanBuild.Clear();
	m_UnitsIHave.Clear();
	m_UnitsICanBuildOrHave.Clear();

	m_MyActors.RemoveAll(); // My units, cities (not-owned)
	m_MyCities.RemoveAll(); // My units, cities (not-owned)
	m_MySeaTransports.RemoveAll(); // (not-owned)
	m_MyAirTransports.RemoveAll(); // (not-owned)
	m_MyAirUnits.RemoveAll();
	m_MyAircraftCarriers.RemoveAll(); // (not-owned)

	// AI Desires, TacticalProjects, Strategies
	//
	m_DesiresSortedByScore.RemoveAll();
	while (m_Desires.IsEmpty() == FALSE){ delete m_Desires.RemoveHead(); }
	//while( m_TacticalProjects.IsEmpty() == FALSE ){ delete m_TacticalProjects.RemoveHead(); }
	while (m_TacticalProjects2.IsEmpty() == FALSE){ delete m_TacticalProjects2.RemoveHead(); }

	m_TacticalProjects2ByInitialEvaluation_NearbyScore.RemoveAll();
	m_TacticalProjects2ByInitialEvaluation_ValueScore.RemoveAll();
	m_TacticalProjects2ByInitialEvaluation_BasicScore.RemoveAll();
	m_TacticalProjects2ByInitialEvaluation_EasyTargets.RemoveAll();
	m_TacticalProjects2ByInitialEvaluation_NoResistance.RemoveAll();
	m_TacticalProject2StrategiesSortedByBestCaseValue.RemoveAll();

	while (m_WarzoneLocations.IsEmpty() == FALSE){ delete m_WarzoneLocations.RemoveHead(); }
	//CList< CEOSAITransportCombinedMap* >  m_TransportCombinedMultiRegionMaps_old;


	long iSize = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegionArray()->GetSize();
	for( long i=0; i<iSize; i++ )
	{
		//CEOSAIRegion2* pAIRegion = m_pAIBrain->GetAIRegionManager()->GetAIRegion( i );
		CEOSAIRegion2* pAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( i );
		if( pAIRegion )
		{
			pAIRegion->ClearGroundUnitAndSeaTransportPathToEverywhere();
		}
	}
}

/*
CEOSAIGeo* CEOSAIThoughtDatabase::GetAIGeo( long iGeoId )
{
	POSITION pos = m_AIGeoList.GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pAIGeo = m_AIGeoList.GetNext( pos );
		if( pAIGeo->GetGeo()->GetGeoNumber() == iGeoId )
		{
			return pAIGeo;
		}
	}
	return NULL;
}
*/

// Desires
//
void CEOSAIThoughtDatabase::CreateDesires()
{
	ASSERT( m_Desires.IsEmpty() );

	CEOSAIStopwatch2 watchEntireMethod;
	watchEntireMethod.Start();

	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();

	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();

	// Create Desires
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );

		pAIPoiObject->ClearAIDesires(); // Clear all attached AIDesires
		pAIPoiObject->ReleaseAllocatedUnitActionIdea();

		// Setup personal desires
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			pAIUnit->CreatePersonalDesires();
		}

		long iMultiRegion = pAIPoiObject->GetInitialState()->GetMultiRegion()->GetId();
		CEOSAIMultiRegionNationwidePathways* pPlayerPathway = g_pEOSAICommonData->GetNationwidePathways( GetAIBrain()->GetAIPlayerNumber() );
		ASSERT( pPlayerPathway );
		if( pPlayerPathway == NULL ) continue;
		CEOSAIRegionPathwayItem* pPathwayItem = pPlayerPathway->GetDistGeoPathfindingValueFromMyCitRes( iMultiRegion );
		ASSERT( pPathwayItem );
		if( pPathwayItem == NULL ) continue;
		float fDistance = pPathwayItem->m_fDistance;
		if( fDistance > 1200.0 ) continue;

		// Lookup the AIGeo values
		CEOSAIMultiRegionPlan* pMultiRegionPlan = m_pAIBrain->GetMultiRegionPlan( iMultiRegion );
		if( pMultiRegionPlan->PlanIsToIgnore() )
		{
			continue;
		}

		//
		if( pAIPoiObject &&
			pAIPoiObject->IsCityResourceAirfieldFortOrUnit() )
		{
			bool bPoiObjectIsMine = ( pAIPoiObject->GetOwner() == iAIPlayer );
			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iAIPlayer,pAIPoiObject->GetOwner() );
			bool bPoiObjectIsNeutralOrAlly = eRel.IsNeutral() || eRel.IsAlly();
			bool bPoiObjectIsEnemyOrUnowned = pAIPoiObject->GetOwner() == 0 || eRel.IsEnemy();

			//if( pServerPoiObject->GetObjectId() == 80 )
			//{
			//	int h=0;
			//}

			//long iGeo = pServerPoiObject->GetGeo();
			//CEOSAIGeo* pAIGeo = m_pAIBrain->GetAIGeo( iGeo );
			//if( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Ignore ||
			//	pAIGeo->GetPlan() == EOSAIEnumGeoPlan_NoPlan ) continue;
			CEOSAIMultiRegion2* pMultiRegion = pMultiRegionManager->GetMultiRegion( pAIPoiObject->GetLocation() );
			ASSERT( pMultiRegion );
			CEOSAIMultiRegionPlan* pMultiRegionPlan = m_pAIBrain->GetMultiRegionPlan( iMultiRegion );
			if( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Ignore ||
				pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_NoPlan ) continue;
			ASSERT( pMultiRegionPlan->GetPlan() != EOSAIEnumMultiRegionPlan_Undefined );

			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity )
			{
				if( bPoiObjectIsMine )
				{
					// Protect from capture
					// Protect from damage
				}
				if( bPoiObjectIsEnemyOrUnowned || bPoiObjectIsNeutralOrAlly )
				{
					// Capture City?
					CEOSAIDesireCaptureCitRes* pCaptureDesire = new CEOSAIDesireCaptureCitRes();
					pCaptureDesire->SetAIBrain( m_pAIBrain );
					pCaptureDesire->SetTarget( pAIPoiObject );//pPoiObject );
					//pCaptureDesire->IsAggressiveAgainstPlayer( pPoiObject->GetOwner() );
					//pCaptureDesire->AttachToMultiRegion();
					float f1 = pCaptureDesire->GetSimpleInterest(); // DEBUG
					m_Desires.AddTail( pCaptureDesire );

					// Damage City?
					CAIDesireDamageCity* pDamageDesire = new CAIDesireDamageCity();
					pDamageDesire->SetAIBrain( m_pAIBrain );
					pDamageDesire->SetTarget( pAIPoiObject );//pPoiObject );
					//pDamageDesire->AttachToMultiRegion();
					float f2 = pDamageDesire->GetSimpleInterest(); // DEBUG
					m_Desires.AddTail( pDamageDesire );

					// Bomb/Bombard the buildings?
					//CList< CBuildingDescription* >* pList = pCity->GetBuildingList();
					POSITION pos = pAICity->GetBuildingList()->GetHeadPosition();
					while( pos )
					{
						CEOSAIBuildingDescription* pBuildingDesc = pAICity->GetBuildingList()->GetNext( pos );
						//CAITacticalProject_DegradeCity* pDegradeCity = new CAITacticalProject_DegradeCity( m_pAIBrain, this, pBuildingDesc->GetInternalName() );
						//pAIThoughtDatabase->AddActionDefinition( pDegradeCity );
					}

					//pDamageDesire->m_ConflictingDesires.AddTail( pCaptureDesire );
					//pCaptureDesire->m_ConflictingDesires.AddTail( pDamageDesire );
				}
			}
			CEOSAIResource* pResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
			if( pResource )
			{
				if( bPoiObjectIsMine )
				{
					// Protect from capture
				}
				if( bPoiObjectIsEnemyOrUnowned || bPoiObjectIsNeutralOrAlly )
				{
					// Capture Resource?
					CEOSAIDesireCaptureCitRes* pDesire = new CEOSAIDesireCaptureCitRes();
					pDesire->SetAIBrain( m_pAIBrain );
					pDesire->SetTarget( pAIPoiObject );//pPoiObject );
					//pDesire->IsAggressiveAgainstPlayer( pPoiObject->GetOwner() );
					//pDesire->AttachToMultiRegion();
					float f = pDesire->GetSimpleInterest(); // DEBUG
					m_Desires.AddTail( pDesire );
				}
			}
			CEOSAIAirfield* pAirfield = dynamic_cast< CEOSAIAirfield* >( pAIPoiObject );
			if( pAirfield )
			{
				if( bPoiObjectIsMine )
				{
					// Protect from capture
				}
				if( bPoiObjectIsEnemyOrUnowned || bPoiObjectIsNeutralOrAlly )
				{
					// Capture Resource?
					CEOSAIDesireCaptureAirfield* pDesire = new CEOSAIDesireCaptureAirfield();
					pDesire->SetAIBrain( m_pAIBrain );
					pDesire->SetTarget( pAIPoiObject );//pPoiObject );
					//pDesire->IsAggressiveAgainstPlayer( pPoiObject->GetOwner() );
					//pDesire->AttachToMultiRegion();
					float f = pDesire->GetSimpleInterest(); // DEBUG
					m_Desires.AddTail( pDesire );
				}
			}
			CEOSAIUnit* pUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pUnit )
			{
				if( bPoiObjectIsEnemyOrUnowned || bPoiObjectIsNeutralOrAlly )
				{
					// Destroy Unit?
					bool bCanAttackUnitDirectly = true;
					if( pUnit->IsGroundUnit() && 
						pUnit->IsInsideAUnitContainer() )
					{
						CEOSAIRegion2* pAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( pUnit->GetLocation() );
						if( pAIRegion->IsWater() )
						{
							// This is a ground unit, and it is out-at-sea on a transport
							//   This means I can't target it directly
							bCanAttackUnitDirectly = false;
						}
					}
					if( bCanAttackUnitDirectly )
					{
						CAIDesireDestroyUnit* pDesire = new CAIDesireDestroyUnit();
						pDesire->SetAIBrain( m_pAIBrain );
						pDesire->SetTarget( pAIPoiObject );//pPoiObject );
						//pDesire->IsAggressiveAgainstPlayer( pPoiObject->GetOwner() );
						//pDesire->AttachToMultiRegion();
						float f = pDesire->GetSimpleInterest(); // DEBUG
						m_Desires.AddTail( pDesire );
					}
				}
			}

		}
	}

	//Checkpoint::Write( _T("CEOSAIThoughtDatabase::CreateDesires() - 2") );
	/*
	long iAIDesires = 0;
	pos = pWorldDescPlayer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = pWorldDescPlayer->GetPoiList()->GetNext( pos )->GetPtr();
		CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );

		CResourceSource* pResource = dynamic_cast< CResourceSource* >( pPoi );
		if( pResource )
		{
			if( pResource->GetAIDesire( EnumCaptureResource ) )
			{
				iAIDesires++;
			}
			int g=0;
		}
	}
	*/
	int h=0;

	// Create Connections
	// Create Pathway Resistance list
	long iCurrentDesire_Debug = 0;
	CEOSAIStopwatch2 Timer1;
	CEOSAIStopwatch2 Timer2;
	CEOSAIStopwatch2 Timer3;
	CEOSAIStopwatch2 Timer4;
	CEOSAIStopwatch2 Timer5;
	CEOSAIStopwatch2 Timer6;
	Timer1.Start();
	//Checkpoint::Write( _T("CEOSAIThoughtDatabase::CreateDesires() - 3") );
	pos = m_Desires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesire2* pDesire = m_Desires.GetNext( pos );
		iCurrentDesire_Debug++;

		// Connections?
		CEOSAIDesireSpatial* pSpatialDesire = dynamic_cast< CEOSAIDesireSpatial* >( pDesire );
		ASSERT( pSpatialDesire );
		if( pSpatialDesire )
		{
			// Accessibility?
			//Checkpoint::Write( "3a" );
			Timer2.Continue();
			pSpatialDesire->CalculateAccessibility01();
			float fAccessibility = pSpatialDesire->GetAccessibility01();
			Timer2.Stop();

			// Resistance?
			//Checkpoint::Write( "3b" );
			Timer3.Continue();
			pSpatialDesire->CalculateExpectedPathwayResistance();
			Timer3.Stop();

			Timer4.Continue();
			//Checkpoint::Write( "3c" );
			CList< CEOSAIDesireSpatial* >  NearbyDesires;
			GetAllDesiresWithinRadius( pSpatialDesire->GetLocation(), 250.0f, &NearbyDesires );
			//float fDesireRange = pSpatialDesire->GetRadius();
			//GetAllDesiresWithinRadius( pSpatialDesire, 400.0f+fDesireRange, &NearbyDesires );
			Timer4.Stop();

			Timer5.Continue();
			//Checkpoint::Write( "3d" );
			POSITION pos2 = NearbyDesires.GetHeadPosition();
			while( pos2 )
			{
				CEOSAIDesireSpatial* pSpatialDesire2 = NearbyDesires.GetNext( pos2 );
				if( pSpatialDesire == pSpatialDesire2 ) continue;
				//
				pSpatialDesire->CreateConnectedDesire( iAIPlayer, pSpatialDesire2 );
				//pSpatialDesire2->CreateConnectedDesire( iAIPlayer, pSpatialDesire );
			}
			int x=0;
			//Checkpoint::Write( "3e" );
			Timer5.Stop();

			Timer6.Continue();
			float fRawScore = pSpatialDesire->GetSimpleInterest();
			float fAreaRawScore = pSpatialDesire->GetAreaSimpleInterest();
			int g=0;
			Timer6.Stop();
		}
		int x=0;
	}
	Timer1.Stop();
	float fTimer1Seconds = (float) Timer1.GetDeltaSeconds();
	float fTimer2Seconds = (float) Timer2.GetDeltaSeconds();
	float fTimer3Seconds = (float) Timer3.GetDeltaSeconds();
	float fTimer4Seconds = (float) Timer4.GetDeltaSeconds();
	float fTimer5Seconds = (float) Timer5.GetDeltaSeconds();
	float fTimer6Seconds = (float) Timer6.GetDeltaSeconds();

	//Checkpoint::Write( _T("CEOSAIThoughtDatabase::CreateDesires() - 4") );
	if( m_Desires.IsEmpty() )
	{
		// Debug: If "desires" is empty it doesn't mean the AI controls everything.
		//   The AI doesn't create desires for PoiObjects owned by non-enemy players.
		//   This means that when an AI is not at war, and when there are no unowned PoiObjects on the map,
		//     then there will be no desires (at least not attack desires, there may be 'protect' desires).
		//   When this situation happens, the AI should be influenced to:
		//     attack a player, build up a military for future attack, or something like that
		//ASSERT( false );
	}

	// Sort the list of spatial desires
	//ASSERT( GetAIThoughtDatabase()->GetDesires()->IsEmpty() == FALSE );
	//CList< CEOSAIDesire2* >* pDesires = GetAIThoughtDatabase()->GetDesires();
	pos = m_Desires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesire2* pDesire = m_Desires.GetNext( pos );
		CEOSAIDesireSpatial* pSpatialDesire = dynamic_cast< CEOSAIDesireSpatial* >( pDesire );
		if( pSpatialDesire )
		{
			pSpatialDesire->CalculateScore( CEOSAIDesire2::EnumInitialScoring );
			//ASSERT( pSpatialDesire->GetSimpleInterest() > 0.0f );
			GetDesiresSortedByScore()->InsertSorted( pSpatialDesire );
		}
	}

	watchEntireMethod.Stop();
	//CString strText;
	//strText.Format( _T("CEOSAIThoughtDatabase::CreateDesires - EntireMethod %f"), 	watchEntireMethod.GetDeltaSeconds() );
	//Checkpoint::Write( strText );
	//Checkpoint::Write( _T("CEOSAIThoughtDatabase::CreateDesires() - End") );
}

void CEOSAIThoughtDatabase::GetAllDesiresWithinRadius( CEOSAILocation Location, float fRadius, CList< CEOSAIDesireSpatial* >* pOutputList )
{
	POSITION pos2 = m_Desires.GetHeadPosition();
	while( pos2 )
	{
		CEOSAIDesire2* pAIDesire = m_Desires.GetNext( pos2 );
		CEOSAIDesireSpatial* pSpatialDesire = dynamic_cast< CEOSAIDesireSpatial* >( pAIDesire );
		if( pSpatialDesire )
		{
			float fDesireRadius = pSpatialDesire->GetRadius();
			if( pSpatialDesire->GetDistance( Location )-fDesireRadius < fRadius )
			{
				pOutputList->AddTail( pSpatialDesire );
			}
		}
	}
}

void  CEOSAIThoughtDatabase::WriteDesiresSortedByScoreItems( CList< CEOSAIDebugData* >* pDataList )
{
	while( pDataList->IsEmpty() == FALSE ){ delete pDataList->RemoveHead(); }

	CString str;// = "Desires\r\n";
	CString strTemp;
	CEOSAIListSort< CEOSAIDesireSpatial* >* pDesires = GetDesiresSortedByScore();
	POSITION pos = pDesires->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );

		CEOSAIDebugData* pDebugData = new CEOSAIDebugData();
		pDebugData->m_strText = pDesire->Dump();
		pDebugData->AddToAnnotations( pDesire->GetLocation(), 50.0f, 1.0,1.0,0.0 );
		pDataList->AddTail( pDebugData );
	}
}

void  CEOSAIThoughtDatabase::WriteDesiresSortedBySimpleInterestItems( CList< CEOSAIDebugData* >* pDataList )
{
	while( pDataList->IsEmpty() == FALSE ){ delete pDataList->RemoveHead(); }

	CString str;
	CString strTemp;
	CEOSAIListSort< CEOSAIDesireSpatial* >  DesiresSortedBySimpleInterest;
	DesiresSortedBySimpleInterest.SetSortFunction( SortDesiresDescBySimpleInterest );

	POSITION pos = GetDesiresSortedByScore()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = GetDesiresSortedByScore()->GetNext( pos );
		DesiresSortedBySimpleInterest.InsertSorted( pDesire );
	}

	//CEOSAIListSort< CEOSAIDesireSpatial* >* pDesires = GetDesiresSortedBySimpleInterest();
	pos = DesiresSortedBySimpleInterest.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = DesiresSortedBySimpleInterest.GetNext( pos );

		CEOSAIDebugData* pDebugData = new CEOSAIDebugData();
		pDebugData->m_strText = pDesire->Dump();
		//pDebugData->m_Annotations.AddTail( new CEOSAIAnnotation( pDesire->GetLocation(), 50.0f ) );
		pDebugData->AddToAnnotations( pDesire->GetLocation(), 50.0f, 1.0,1.0,0.0 );
		pDataList->AddTail( pDebugData );
	}
}

void  CEOSAIThoughtDatabase::WriteTacticalProjectsSortedBySimpleInterest( CList< CEOSAIDebugData* >* pDataList )
{
	while( pDataList->IsEmpty() == FALSE ){ delete pDataList->RemoveHead(); }

	CString str;
	CString strTemp;
	CEOSAIListSort< CEOSAITacticalProject2* >  TacticalProjectsSortedBySimpleInterest;
	TacticalProjectsSortedBySimpleInterest.SetSortFunction( SortTacticalProjectsBySimpleInterest );

	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = m_TacticalProjects2.GetNext( pos );
		TacticalProjectsSortedBySimpleInterest.InsertSorted( pTacticalProject );
	}
	pos = TacticalProjectsSortedBySimpleInterest.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = TacticalProjectsSortedBySimpleInterest.GetNext( pos );

		CEOSAIDebugData* pDebugData = new CEOSAIDebugData();
		//pDebugData->m_strText = pTacticalProject->Dump();
		pTacticalProject->Dump( pDebugData->m_strText );

		POSITION pos = pTacticalProject->GetPrimaryGoals()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject->GetPrimaryGoals()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 50.0f, 1.0f,1.0f,0.0f );
		}
		pos = pTacticalProject->GetLocalResistance()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject->GetLocalResistance()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 1.0f,0.0f,0.0f );
		}
		pos = pTacticalProject->GetPathwayResistance()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject->GetPathwayResistance()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 0.7f,0.0f,0.0f );
		}
		pos = pTacticalProject->GetOnTheWayOpportunities()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject->GetOnTheWayOpportunities()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 0.7f,0.7f,0.0f );
		}
		pDataList->AddTail( pDebugData );
	}
}

void  CEOSAIThoughtDatabase::WriteTacticalProjectsSortedByInitialEvaluation( CList< CEOSAIDebugData* >* pDataList )
{
	while( pDataList->IsEmpty() == FALSE ){ delete pDataList->RemoveHead(); }

	CString str;
	CString strTemp;
	CEOSAIListSort< CEOSAITacticalProject2* >  TacticalProjectsSortedByInitialEvaluation;
	TacticalProjectsSortedByInitialEvaluation.SetSortFunction( SortTacticalProjectsByInitialEvaluationBasicScore );

	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = m_TacticalProjects2.GetNext( pos );
		TacticalProjectsSortedByInitialEvaluation.InsertSorted( pTacticalProject );
	}
	pos = TacticalProjectsSortedByInitialEvaluation.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = TacticalProjectsSortedByInitialEvaluation.GetNext( pos );

		CEOSAIDebugData* pDebugData = new CEOSAIDebugData();
		//pDebugData->m_strText = pTacticalProject->Dump();
		pTacticalProject->Dump( pDebugData->m_strText );

		POSITION pos = pTacticalProject->GetPrimaryGoals()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject->GetPrimaryGoals()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 50.0f, 1.0f,1.0f,0.0f );
		}
		pos = pTacticalProject->GetLocalResistance()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject->GetLocalResistance()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 1.0f,0.0f,0.0f );
		}
		pos = pTacticalProject->GetPathwayResistance()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject->GetPathwayResistance()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 0.7f,0.0f,0.0f );
		}
		pos = pTacticalProject->GetOnTheWayOpportunities()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject->GetOnTheWayOpportunities()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 0.7f,0.7f,0.0f );
		}
		pDataList->AddTail( pDebugData );
	}
}

void  CEOSAIThoughtDatabase::WriteTacticalProjects2( CList< CEOSAIDebugData* >* pDataList )
{
	while( pDataList->IsEmpty() == FALSE ){ delete pDataList->RemoveHead(); }

	CString str;
	CString strTemp;
	/*
	CEOSAIListSort< CAITacticalProject* >  TacticalProjectsSortedByInitialEvaluation;
	TacticalProjectsSortedByInitialEvaluation.SetSortFunction( SortTacticalProjectsByInitialEvaluationScore );

	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacticalProject = m_TacticalProjects.GetNext( pos );
		TacticalProjectsSortedByInitialEvaluation.InsertSorted( pTacticalProject );
	}
	*/
	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = m_TacticalProjects2.GetNext( pos );

		CEOSAIDebugData* pDebugData = new CEOSAIDebugData();
		//pDebugData->m_strText = pTacticalProject->Dump();
		pTacticalProject2->Dump( pDebugData->m_strText );

		POSITION pos = pTacticalProject2->GetPrimaryGoals()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject2->GetPrimaryGoals()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 50.0f, 1.0,1.0,0.0 );
		}
		pos = pTacticalProject2->GetLocalResistance()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject2->GetLocalResistance()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 1.0,0.0,0.0 );
		}
		pos = pTacticalProject2->GetPathwayResistance()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject2->GetPathwayResistance()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 0.7f,0.0f,0.0f );
		}
		pos = pTacticalProject2->GetOnTheWayOpportunities()->GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = pTacticalProject2->GetOnTheWayOpportunities()->GetNext( pos );
			pDebugData->AddToAnnotations( pDesire->GetLocation(), 40.0f, 0.7f,0.7f,0.0f );
		}
		//
		// Setup the path data
		pos = pTacticalProject2->GetNationwideMultiRegionPathway()->GetHeadPosition();
		while( pos )
		{
			CEOSAIMultiRegion2* pMultiRegion = pTacticalProject2->GetNationwideMultiRegionPathway()->GetNext( pos );
			float fRadius = pMultiRegion->m_fRegionRadius;
			pDebugData->AddToAnnotations( pMultiRegion->GetLocation(), fRadius, 0.0f,0.7f,0.0f );
		}

		pDataList->AddTail( pDebugData );
	}
}

CString  CEOSAIThoughtDatabase::GetSortedDesiresText()
{
	CString str;// = "Desires\r\n";
	CString strTemp;
	CEOSAIListSort< CEOSAIDesireSpatial* >* pDesires = GetDesiresSortedByScore();
	POSITION pos = pDesires->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );
		//strTemp.Format( "ObjectId %d\r\n", pDesire->GetAIPoiObjectTarget()->GetObjectId() );
		//str += strTemp;
		str += pDesire->Dump();
	}

	return str;//"SortedDesiresText";
}

//
// Warzones
//
void  CEOSAIThoughtDatabase::ClearWarzoneCalculations()
{
	while( m_WarzoneLocations.IsEmpty() == FALSE ){ delete m_WarzoneLocations.RemoveHead(); }
}
/*
I need to define areas of the map.  There are land/sea pathways and also air access.
- Area 1: Deep in my territory = less need for protection (though, I may need Antiair and anti-paratroop)
- Area 2: Moderately in my territory = some troops, some protection
- Area 3: Border areas in my territory = more need for troops, anti-air, repair because of potential invasions
- Area 4: Just outside my territory = slightly increased danger
- Area 5: No mans land
- Area 6: Just outside other players territory (player number)
- Area 7: Border areas in other players territory (player number)
- Area 8: Moderately in other players territory (player number)
- Area 9: Deep in other players territory (player number)
These areas will be used for placing units.  If the AI is weaker than the other player, they should
withdraw a little bit.  If they are stronger, they should get more gutsy about moving into other players
territory.
There should be a danger map as well - which helps inform the AI about whether or not to escort.
*/
/*
void  CEOSAIThoughtDatabase::CreateMultiRegionOwnershipLevels()
{
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();

	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();
	long iNumberOfMultiRegions = pMultiRegionManager->GetNumberOfMultiRegions();
	for( long iMultiRegion=1; iMultiRegion<iNumberOfMultiRegions; iMultiRegion++ )
	{
		CMultiRegion* pMultiRegion = pMultiRegionManager->GetMultiRegion( iMultiRegion );
		if( pMultiRegion == NULL ) continue;

		// Compile the adjacent multiregions
		CList< CMultiRegion* >  MultiRegions;
		MultiRegions.AddTail( pMultiRegion );
		POSITION pos = pMultiRegion->GetAdjacentRegions()->GetHeadPosition();
		while( pos )
		{
			CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pMultiRegion->GetAdjacentRegions()->GetNext( pos );
			CEOSAIGenericRegion* pGenericRegion = pAdjRegionDesc->m_pRegion;
			CMultiRegion* pMultiRegion2 = dynamic_cast< CMultiRegion* >( pGenericRegion );
			if( pMultiRegion2 )
			{
				MultiRegions.AddTail( pMultiRegion2 );
			}
		}
		long iAreaType = 0;
		long iMyAIRegions = 0;
		long iPlayer0AIRegions = 0;
		long iOtherPlayerAIRegions = 0;
		pos = MultiRegions.GetHeadPosition();
		while( pos )
		{
			CMultiRegion* pMultiRegion2 = MultiRegions.GetNext( pos );
			POSITION pos2 = pMultiRegion2->m_AIRegions.GetHeadPosition();
			while( pos2 )
			{
				CEOSAIRegion2* pAIRegion = pMultiRegion2->m_AIRegions.GetNext( pos2 );
				if( pAIRegion->m_iOwner == 0 )
				{
					iPlayer0AIRegions++;
				}
				eif( pAIRegion->m_iOwner == iAIPlayer )
				{
					iMyAIRegions++;
				}
				else
				{
					iOtherPlayerAIRegions++;
				}
			}
		}
		if( iMyAIRegions > 0 )
		{
			iAreaType = 3;
		}
		loijhlj
		//pMultiRegion->m_i
	}
}
*/
void  CEOSAIThoughtDatabase::FindWarzones()
{
	CList< CEOSAIRegion2* > AIRegions;
	//CWorldBuildDesc* pWorldBuildDesc = m_pAIBrain->GetWorldDescServer()->GetWorldBuildDesc();

	// Look at every AIRegion.  If the AI has a high control value, and another player (neutral or enemy)
	//   has a high control value, then the TempWarzoneValue goes up.
	// Lookup my enemies, lookup AIRegions
	long iLocalPlayer = m_pAIBrain->GetAIPlayerNumber();
	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
	for( long iAIRegion=0; iAIRegion<pAIRegionManager->m_AIRegionArray.GetSize(); iAIRegion++ )
	{
		CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( iAIRegion );
		if( pAIRegion == NULL ) continue;
		float fValue01 = 0.0f;
		long iPlayers = pAIRegion->m_PlayerCitResControlValue01.m_iSize;
		for( long iOtherPlayer=1; iOtherPlayer<iPlayers; iOtherPlayer++ )
		{
			if( iLocalPlayer == iOtherPlayer ) continue;
			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iLocalPlayer, iOtherPlayer );
			if( eRel == EOSAIEnumForeignRelations::enum_Teammate ) continue;
			if( eRel == EOSAIEnumForeignRelations::enum_Alliance ) continue;
			fValue01 = max( fValue01, pAIRegion->GetWarzoneValue01( iLocalPlayer,iOtherPlayer ) );
		}
		pAIRegion->m_fTempWarzoneValue01 = fValue01;
		if( fValue01 > 0.0f )
		{
			AIRegions.AddTail( pAIRegion );
		}
	}
	// Find the highest AIRegion
	bool bFoundWarzone = true;
	long iNumberOfWarzones = 0;
	while( bFoundWarzone )
	{
		CEOSAIRegion2* pHighestAIRegion = NULL;
		POSITION pos = AIRegions.GetHeadPosition();
		while( pos )
		{
			CEOSAIRegion2* pAIRegion = AIRegions.GetNext( pos );
			if( pHighestAIRegion == NULL ||
				pHighestAIRegion->m_fTempWarzoneValue01 < pAIRegion->m_fTempWarzoneValue01 )
			{
				pHighestAIRegion = pAIRegion;
			}
		}
		// First   warzone = 0.1+
		// Second  warzone = 0.2+
		// Third   warzone = 0.3+
		// Fourth  warzone = 0.4+
		// Fifth   warzone = 0.5+
		// Sixth   warzone = 0.6+
		// Seventh warzone = 0.7+
		float fMinimumRequired = 0.1f + 0.10f*iNumberOfWarzones;
		if( pHighestAIRegion && pHighestAIRegion->m_fTempWarzoneValue01 > fMinimumRequired )
		{
			iNumberOfWarzones++;

			CEOSAIWarzoneLocation* pAIWarzoneLocation = new CEOSAIWarzoneLocation();
			pAIWarzoneLocation->m_Location = pHighestAIRegion->GetLocation();
			pAIWarzoneLocation->m_fWarzoneSignificance01 = pHighestAIRegion->m_fTempWarzoneValue01;
			m_WarzoneLocations.AddTail( pAIWarzoneLocation );

			// Knock-down the surrounding area's WarzoneValue
			for( long iAIRegion=0; iAIRegion<pAIRegionManager->m_AIRegionArray.GetSize(); iAIRegion++ )
			{
				CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( iAIRegion );
				if( pAIRegion == NULL ) continue;
				//float fPixelDistance = pWorldBuildDesc->GetPixelDistance( pAIRegion->GetLocation(), pHighestAIRegion->GetLocation() );
				float fPixelDistance = g_pWorldDistanceTool->GetDistance( pAIRegion->GetLocation(), pHighestAIRegion->GetLocation() );
				if( fPixelDistance < 180.0f )
				{
					pAIRegion->m_fTempWarzoneValue01 = 0.0f;
					pAIWarzoneLocation->m_AIRegions.AddTail( pAIRegion->GetId() );
				}
			}
			// Add-up the nearby power values
			//POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
			POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
			while( pos )
			{
				CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
				if( pAIPoiObject->IsHypotheticalPoiObject() ) continue; // it doesn't exist

				//float fPixelDistance = pWorldBuildDesc->GetPixelDistance( pAIPoiObject->GetInitialState()->GetLocation(), pHighestAIRegion->GetLocation() );
				float fPixelDistance = g_pWorldDistanceTool->GetDistance( pAIPoiObject->GetInitialState()->GetLocation(), pHighestAIRegion->GetLocation() );
				if( fPixelDistance < 300.0f )
				{
					float fSignificance01 = 1.0f;
					if( fPixelDistance > 150.0f ){ fSignificance01 = (300.0f-fPixelDistance)/150.0f; }
					long iOwner = pAIPoiObject->GetInitialState()->GetOwner();
					//EOSAIEnumForeignRelations eRel = m_pAIBrain->GetWorldDescServer()->GetForeignRelations( iLocalPlayer,iOwner );
					EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iLocalPlayer,iOwner );
					if( eRel >= EOSAIEnumForeignRelations::enum_Alliance )
					{
						CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
						if( pAIUnit )
						{
							pAIWarzoneLocation->m_fSATUnitPower += fSignificance01 * pAIUnit->GetAIPowerValue();
						}
						CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
						if( pAICity )
						{
							pAIWarzoneLocation->m_fSATCitResValue += fSignificance01 * pAICity->GetTotalProduction();
						}
						CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
						if( pAIResource )
						{
							pAIWarzoneLocation->m_fSATCitResValue += fSignificance01 * pAIResource->GetResourcePerTurn();
						}
					}
					if( eRel < EOSAIEnumForeignRelations::enum_Neutral )
					{
						CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
						if( pAIUnit )
						{
							pAIWarzoneLocation->m_fNEUnitPower += fSignificance01 * pAIUnit->GetAIPowerValue();
						}
						CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
						if( pAICity )
						{
							pAIWarzoneLocation->m_fNECitResValue += fSignificance01 * pAICity->GetTotalProduction();
						}
						CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
						if( pAIResource )
						{
							pAIWarzoneLocation->m_fNECitResValue += fSignificance01 * pAIResource->GetResourcePerTurn();
						}
					}
					int g=0;
					//pAIWarzoneLocation->m_f
				}
			}
		}
		else
		{
			bFoundWarzone = false;
		}
	}
}

//
//
/*
void  CEOSAIThoughtDatabase::AddTacticalProject( CAITacticalProject* pTacticalProject )
{
	m_TacticalProjects.AddTail( pTacticalProject );
}
*/
void  CEOSAIThoughtDatabase::AddTacticalProject2( CEOSAITacticalProject2* pTacticalProject2 )
{
	m_TacticalProjects2.AddTail( pTacticalProject2 );

	//ASSERT( pTacticalProject2->GetInitialEvaluation_ValueScore() > 0.0f );
	if( pTacticalProject2->GetInitialEvaluation_NearbyScore() > 0.0f )
	{
		m_TacticalProjects2ByInitialEvaluation_NearbyScore.InsertSorted( pTacticalProject2 );
	}
	if( pTacticalProject2->GetInitialEvaluation_ValueScore() > 0.0f )
	{
		m_TacticalProjects2ByInitialEvaluation_ValueScore.InsertSorted( pTacticalProject2 );
	}
	//ASSERT( pTacticalProject2->GetInitialEvaluation_BasicScore() > 0.0f );
	if( pTacticalProject2->GetInitialEvaluation_BasicScore() > 0.0f )
	{
		m_TacticalProjects2ByInitialEvaluation_BasicScore.InsertSorted( pTacticalProject2 );
	}
	if( pTacticalProject2->GetInitialEvaluation_EasyTargetScore() > 0.0f )
	{
		m_TacticalProjects2ByInitialEvaluation_EasyTargets.InsertSorted( pTacticalProject2 );
	}
	if( pTacticalProject2->GetInitialEvaluation_NoResistanceScore() > 0.0f )
	{
		m_TacticalProjects2ByInitialEvaluation_NoResistance.InsertSorted( pTacticalProject2 );
	}
}
/*
void  CEOSAIThoughtDatabase::AddToSortedBestCaseTacticalProjectStrategies( CAITacticalProject* pTacticalProject )
{
	CList< CEOSAITacticalProject2Strategy* > StrategyList;
	pTacticalProject->GetAITacticalProjectStrategies( &StrategyList );
	POSITION pos = StrategyList.GetHeadPosition();
	while( pos )
	{
		CAITacticalProjectStrategy* pStrategy = StrategyList.GetNext( pos );
		if( pStrategy->GetBestCaseTaskForceValue() > 0.0f )
		{
			m_SortedBestCaseTacticalProjectStrategies.InsertSorted( pStrategy );
		}
	}
}

void  CEOSAIThoughtDatabase::AddToSortedBestCaseTacticalProjectStrategies( CAITacticalProjectStrategy* pStrategy )
{
	m_SortedBestCaseTacticalProjectStrategies.InsertSorted( pStrategy );
}
*/
void  CEOSAIThoughtDatabase::AddToTacticalProjectStrategiesSortedByBestCaseValue( CEOSAITacticalProject2Strategy* pStrategy )
{
	m_TacticalProject2StrategiesSortedByBestCaseValue.InsertSorted( pStrategy );
}

void  CEOSAIThoughtDatabase::SetTacticalProjects2ScoreIndex()
{
	POSITION pos;
	long iIndex = 1;
	pos = m_TacticalProjects2ByInitialEvaluation_NearbyScore.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = m_TacticalProjects2ByInitialEvaluation_NearbyScore.GetNext( pos );
		if( pTacticalProject2->CurrentForeignRelationsPreventsPersuingThisTacticalProject() ) continue;
		pTacticalProject2->SetInitialEvaluation_NearbyScoreIndex( iIndex );
		ASSERT( pTacticalProject2->GetInitialEvaluation_ValueScore() > 0.0f );
		//if( pTacticalProject2->GetInitialEvaluation_ValueScore() == 0.0f ){ pTacticalProject2->SetInitialEvaluation_ValueScoreIndex( 0 ); }
		iIndex++;
	}
	iIndex = 1;
	pos = m_TacticalProjects2ByInitialEvaluation_ValueScore.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = m_TacticalProjects2ByInitialEvaluation_ValueScore.GetNext( pos );
		if( pTacticalProject2->CurrentForeignRelationsPreventsPersuingThisTacticalProject() ) continue;
		pTacticalProject2->SetInitialEvaluation_ValueScoreIndex( iIndex );
		ASSERT( pTacticalProject2->GetInitialEvaluation_ValueScore() > 0.0f );
		//if( pTacticalProject2->GetInitialEvaluation_ValueScore() == 0.0f ){ pTacticalProject2->SetInitialEvaluation_ValueScoreIndex( 0 ); }
		iIndex++;
	}
	iIndex = 1;
	pos = m_TacticalProjects2ByInitialEvaluation_BasicScore.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = m_TacticalProjects2ByInitialEvaluation_BasicScore.GetNext( pos );
		if( pTacticalProject2->CurrentForeignRelationsPreventsPersuingThisTacticalProject() ) continue;
		pTacticalProject2->SetInitialEvaluation_BasicScoreIndex( iIndex );
		ASSERT( pTacticalProject2->GetInitialEvaluation_BasicScore() > 0.0f );
		//if( pTacticalProject2->GetInitialEvaluation_BasicScore() == 0.0f ){ pTacticalProject2->SetInitialEvaluation_BasicScoreIndex( 0 ); }
		iIndex++;
	}
	iIndex = 1;
	pos = m_TacticalProjects2ByInitialEvaluation_EasyTargets.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = m_TacticalProjects2ByInitialEvaluation_EasyTargets.GetNext( pos );
		if( pTacticalProject2->CurrentForeignRelationsPreventsPersuingThisTacticalProject() ) continue;
		pTacticalProject2->SetInitialEvaluation_EasyTargetScoreIndex( iIndex );
		ASSERT( pTacticalProject2->GetInitialEvaluation_EasyTargetScore() > 0.0f );
		//if( pTacticalProject2->GetInitialEvaluation_EasyTargetScore() == 0.0f ){ pTacticalProject2->SetInitialEvaluation_EasyTargetScoreIndex( 0 ); }
		iIndex++;
	}
	iIndex = 1;
	pos = m_TacticalProjects2ByInitialEvaluation_NoResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = m_TacticalProjects2ByInitialEvaluation_NoResistance.GetNext( pos );
		if( pTacticalProject2->CurrentForeignRelationsPreventsPersuingThisTacticalProject() ) continue;
		pTacticalProject2->SetInitialEvaluation_NoResistanceScoreIndex( iIndex );
		ASSERT( pTacticalProject2->GetInitialEvaluation_NoResistanceScore() > 0.0f );
		//if( pTacticalProject2->GetInitialEvaluation_NoResistanceScore() == 0.0f ){ pTacticalProject2->SetInitialEvaluation_NoResistanceScoreIndex( 0 ); }
		iIndex++;
	}

	// Set the 'order of processing' data
	//   This isn't used for anything except debug
	#ifdef _DEBUG
	CEOSAIListSort< CEOSAITacticalProject2* >  TacticalProjectsToEvaluate;
	TacticalProjectsToEvaluate.SetSortFunction( SortByOrderOfProcessing );
	pos = GetTacticalProjects2()->GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = GetTacticalProjects2()->GetNext( pos );
		if( pTacticalProject2->GetShouldBeInAllocateTacticalProjectList( 5 ) )
		{
			TacticalProjectsToEvaluate.InsertSorted( pTacticalProject2 );
		}
	}
	iIndex = 1;
	pos = TacticalProjectsToEvaluate.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject2 = TacticalProjectsToEvaluate.GetNext( pos );
		pTacticalProject2->SetOrderOfProcessingIndex( iIndex );
		iIndex++;
	}
	#endif _DEBUG
}

/*
void  CEOSAIThoughtDatabase::RemoveUnitActionValue( CEOSAIUnitActionIdea* pUnitActionValue )
{
	POSITION pos = m_AIUnitActionValues.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnitActionIdea* pUnitActionValueInList = m_AIUnitActionValues.GetNext( pos );
		if( pUnitActionValueInList == pUnitActionValue )
		{
			m_AIUnitActionValues.RemoveAt( prevPos );
		}
	}
}
*/

/*
void CEOSAIThoughtDatabase::CreateTacticalProjects()
{
	//
	// DEBUG
	//
	/-*
	CEOSAIUnitTemplate* pInfantryAIUnitTemplate = GetAIUnitTemplate( "Infantry" );
	CAITacticalProjectUnitNeeds2      Needs;
	CAITacticalProjectUnitCapability  Capability;
	bool b = Needs.NeedsAreSatisfiedByCapability( &Capability );

	//Needs.m_fNeed_AntiArmorCombat = 2.0f;
	//b = Needs.NeedsAreSatisfiedByCapability( &Capability );
	Needs.AddToNeedsAndExtras( pInfantryAIUnitTemplate );

	//CEOSAIUnitTemplate* pInfantryAIUnitTemplate = GetAIUnitTemplate( "Infantry" );
	float f1 = Needs.GetNeedsValue( pInfantryAIUnitTemplate, NULL );
	float f2 = Needs.GetValue( pInfantryAIUnitTemplate, NULL );

	Capability.Add( pInfantryAIUnitTemplate );
	f1 = Needs.GetNeedsValue( pInfantryAIUnitTemplate, &Capability );
	f2 = Needs.GetValue( pInfantryAIUnitTemplate, &Capability );
	int z=0;
	*-/

	//
	// This stuff (below) is old, but I want to transfer it to what I'm currently doing.
	//

	// Create some thoughts
	POSITION pos = m_pAIBrain->GetAIObjectList()->GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_pAIBrain->GetAIObjectList()->GetNext( pos );

		// DEBUG - just want to create one ActionDefinition
		//   ObjectId 20 = nearby land-route City
		//   ObjectId 34 = nearby water-route City
		if( pAIObject->GetObjectId() == 20 )
		{
			pAIObject->CreateSomeThoughtsAboutThisObject();
		}
		//pAIObject->CreateSomeThoughtsAboutThisObject();
	}

	// Think about the regions around me and what thoughts I have about them
	//   (Defense of MultiRegion, etc)
	for( long i=1; i<m_pAIBrain->GetMultiRegionManager()->GetMultiRegionArray()->GetSize(); i++ )
	{
		CMultiRegion* pMultiRegion = m_pAIBrain->GetMultiRegionManager()->GetMultiRegionArray()->GetAt( i );
		if( pMultiRegion )
		{
			//pMultiRegion->CreateSomeThoughtsAboutThisObject( m_pAIBrain );
		}
	}

	//
	// New stuff --
	//   Start creating some AIActionDefinitions and fulfilling desires
	//   The AI needs to keep-in-mind global desires, such as:
	//     "conservative play"/"aggressive play"
	//     "remain in building phase for now"
	//     etc

	m_pAIBrain->GetAIActionScheduler()->CreateTacticalProjects();

	/-*
	// Create a list of spatial desires
	CList< CEOSAIDesireSpatial* >  SpatialDesires;
	pos = m_Desires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesire2* pDesire = m_Desires.GetNext( pos );
		CEOSAIDesireSpatial* pSpatialDesire = dynamic_cast< CEOSAIDesireSpatial* >( pDesire );
		if( pSpatialDesire )
		{
			SpatialDesires.AddTail( pSpatialDesire );
			//CAITacticalProject* pActionDef = new CAITacticalProject( m_pAIBrain );
			//pActionDef->AddDesire( pSpatialDesire );
		}
	}

	CAITacticalProject* pActionDefinition = new CAITacticalProject( m_pAIBrain );
	pActionDefinition->Create( &SpatialDesires );
	pActionDefinition
	*-/
}
*/

void CEOSAIThoughtDatabase::Debug_OutputTacticalProjectsToFile( EnumDebugDataOutput eOutput, CString strFile )
{
	if( EOSAISettings::m_bDoingTesting )
	{
		CString strData = GetSortedTacticalProjectsText( eOutput );
		/*
		CString strData;
		CString strTemp;

		CEOSAIListSort< CAITacticalProject* >  SortedTacticalProjects;
		if( eOutput == EnumDebugDataOutput_TacticalProjectsSortedBySimpleInterest )//0 )
		{
			strData.Format( "TacticalProjects (Count: %d)\n", m_TacticalProjects.GetCount() );
			GetTacticalProjectsSortedBySimpleInterest( SortedTacticalProjects );
			strTemp.Format( "Ordered By: SimpleInterest\n\n" );
			strData += strTemp;

			long iCount = 1;
			POSITION pos = SortedTacticalProjects.GetHeadPosition();
			while( pos )
			{
				strTemp.Format( "%03d ", iCount );
				strData += strTemp;
				iCount++;
				CAITacticalProject* pTacticalProject = SortedTacticalProjects.GetNext( pos ); // owned
				pTacticalProject->Dump( strData );
			}
		}
		else if( eOutput == EnumDebugDataOutput_TacticalProjectsSortedByInitialEvaluationScore )//1 )
		{
			strData.Format( "TacticalProjects (Count: %d)\n", m_TacticalProjects.GetCount() );
			GetTacticalProjectsSortedByInitialEvaluationScore( SortedTacticalProjects );
			strTemp.Format( "Ordered By: InitialEvaluationScore\n\n" );
			strData += strTemp;

			long iCount = 1;
			POSITION pos = SortedTacticalProjects.GetHeadPosition();
			while( pos )
			{
				strTemp.Format( "%03d ", iCount );
				strData += strTemp;
				iCount++;
				CAITacticalProject* pTacticalProject = SortedTacticalProjects.GetNext( pos ); // owned
				pTacticalProject->Dump( strData );
			}
		}
		else if( eOutput == EnumDebugDataOutput_TacticalProjectsSortedByHypotheticalTaskForceScore )//2 )
		{
			strData.Format( "TacticalProjects (Count: %d)\n", m_TacticalProjects.GetCount() );
			GetTacticalProjectsSortedByHypotheticalTaskForceScore( SortedTacticalProjects );
			strTemp.Format( "Ordered By: HypotheticalTaskForceScore\n\n" );
			strData += strTemp;

			long iCount = 1;
			POSITION pos = SortedTacticalProjects.GetHeadPosition();
			while( pos )
			{
				strTemp.Format( "%03d ", iCount );
				strData += strTemp;
				iCount++;
				CAITacticalProject* pTacticalProject = SortedTacticalProjects.GetNext( pos ); // owned
				pTacticalProject->Dump( strData );
			}
		}
		else if( eOutput == EnumDebugDataOutput_TacticalProjectsStrategies )//2 )
		{
			strData.Format( "TacticalProjectsStrategy (Count: %d)\n", m_SortedBestCaseTacticalProjectStrategies.GetCount() );

			long iCount = 1;
			POSITION pos = m_SortedBestCaseTacticalProjectStrategies.GetHeadPosition();
			while( pos )
			{
				strTemp.Format( "%03d ", iCount );
				strData += strTemp;
				iCount++;
				CAITacticalProjectStrategy* pTacticalProjectStrategy = m_SortedBestCaseTacticalProjectStrategies.GetNext( pos ); // owned
				pTacticalProjectStrategy->Dump( strData );
			}
			/-*
			GetTacticalProjectsStrategiesSorted( SortedTacticalProjects );
			strTemp.Format( "Ordered By: Strategies\n\n" );
			strData += strTemp;
			*-/
		}
		else
		{
			ASSERT( false );
		}
		*/
		//

		CStdioFile  File;
		if( File.Open( strFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeText ) )
		{
			File.WriteString( strData );
		}
	}
}

CString CEOSAIThoughtDatabase::GetSortedTacticalProjectsText( EnumDebugDataOutput eOutput )
{
	CString strData;
	CString strTemp;

	CEOSAIListSort< CEOSAITacticalProject2* >  SortedTacticalProjects;
	if( eOutput == EnumDebugDataOutput_TacticalProjectsSortedBySimpleInterest )//0 )
	{
		strData.Format( _T("TacticalProjects (Count: %d)\n"), m_TacticalProjects2.GetCount() );
		GetTacticalProjectsSortedBySimpleInterest( SortedTacticalProjects );
		strTemp.Format( _T("Ordered By: SimpleInterest\n\n") );
		strData += strTemp;

		long iCount = 1;
		POSITION pos = SortedTacticalProjects.GetHeadPosition();
		while( pos )
		{
			strTemp.Format( _T("%03d "), iCount );
			strData += strTemp;
			iCount++;
			CEOSAITacticalProject2* pTacticalProject = SortedTacticalProjects.GetNext( pos ); // owned
			pTacticalProject->Dump( strData );
		}
	}
	else if( eOutput == EnumDebugDataOutput_TacticalProjectsSortedByInitialEvaluationScore )//1 )
	{
		strData.Format( _T("TacticalProjects (Count: %d)\n"), m_TacticalProjects2.GetCount() );
		GetTacticalProjectsSortedByInitialEvaluationScore( SortedTacticalProjects );
		strTemp.Format( _T("Ordered By: InitialEvaluationScore\n\n") );
		strData += strTemp;

		long iCount = 1;
		POSITION pos = SortedTacticalProjects.GetHeadPosition();
		while( pos )
		{
			strTemp.Format( _T("%03d "), iCount );
			strData += strTemp;
			iCount++;
			CEOSAITacticalProject2* pTacticalProject = SortedTacticalProjects.GetNext( pos ); // owned
			pTacticalProject->Dump( strData );
		}
	}
	else if( eOutput == EnumDebugDataOutput_TacticalProjectsSortedByHypotheticalTaskForceScore )//2 )
	{
		/*
		strData.Format( "TacticalProjects (Count: %d)\n", m_TacticalProjects.GetCount() );
		GetTacticalProjectsSortedByHypotheticalTaskForceScore( SortedTacticalProjects );
		strTemp.Format( "Ordered By: HypotheticalTaskForceScore\n\n" );
		strData += strTemp;

		long iCount = 1;
		POSITION pos = SortedTacticalProjects.GetHeadPosition();
		while( pos )
		{
			strTemp.Format( "%03d ", iCount );
			strData += strTemp;
			iCount++;
			CAITacticalProject* pTacticalProject = SortedTacticalProjects.GetNext( pos ); // owned
			pTacticalProject->Dump( strData );
		}
		*/
	}
	else if( eOutput == EnumDebugDataOutput_TacticalProjectsStrategies )//2 )
	{
		strData.Format( _T("TacticalProjectsStrategy (Count: %d)\n"), m_TacticalProject2StrategiesSortedByBestCaseValue.GetCount() );

		long iCount = 1;
		POSITION pos = m_TacticalProject2StrategiesSortedByBestCaseValue.GetHeadPosition();
		while( pos )
		{
			strTemp.Format( _T("%03d "), iCount );
			strData += strTemp;
			iCount++;
			CEOSAITacticalProject2Strategy* pTacticalProjectStrategy = m_TacticalProject2StrategiesSortedByBestCaseValue.GetNext( pos ); // owned
			pTacticalProjectStrategy->Dump( strData );
		}
		/*
		GetTacticalProjectsStrategiesSorted( SortedTacticalProjects );
		strTemp.Format( "Ordered By: Strategies\n\n" );
		strData += strTemp;
		*/
	}
	else
	{
		ASSERT( false );
	}
	return strData;
}

/*
void  CEOSAIThoughtDatabase::InvokeMyCityAndTransportPathwayData()
{
	// Create Transport pathway information for:
	//    All sea transports
	//    All potential transports from my cities
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( pAIPoiObject->IsMine() )
		{
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity )
			{
				//pAICity->GetInitialState()->GetMultiRegion()->InvokeSeaPathways_EverywhereMRX();
				pAICity->GetInitialState()->GetMultiRegion()->InvokePathwaysToMultiRegion();
			}
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				if( pAIUnit->IsSeaUnit() && pAIUnit->GetNumberOfGroundUnitsThatICanContain() > 0 )
				{
					//pAIUnit->GetInitialState()->GetMultiRegion()->InvokeSeaPathways_EverywhereMRX();
					pAIUnit->GetInitialState()->GetMultiRegion()->InvokePathwaysToMultiRegion();
				}
			}
		}
	}
}
*/
/*
void  CEOSAIThoughtDatabase::CreateUnitActionIdeas()
{
	CEOSAIListSort< CAITacticalProject* > SortedTacticalProjects;
	//GetActionDefinitionsSortedBySimpleInterest( SortedActionDefinitions );
	GetTacticalProjectsSortedByInitialEvaluationScore( SortedTacticalProjects );

	// Count the number of actors
	long iTotalActors = 0;
	POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_pAIBrain->GetAIPoiObjectList()->GetNext( pos );
		if( pAIPoiObject->IsMine() )
		{
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAIUnit || pAICity ){ iTotalActors++; }
		}
	}

	//? Go through the ActionDefinitions, come up with some pathways from the other multiregions.
	//?   These pathways include danger, sighting, presence data.  By finding pathways from
	//?   the MultiRegions, we can quickly find land/sea/air routes for units in those multiregions.

	// Create UnitActionIdeas
	//   If an ActionDefinition has a high SimpleScore, create more UnitActionIdeas
	//   If an ActionDefinition has a low SimpleScore, create UnitActionIdeas for nearby Units/Cities
	long iCurrentTacticalProjectCount = 0;
	pos = SortedTacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacticalProject = SortedTacticalProjects.GetNext( pos );

		float fRange = 1.0f;
		if( iCurrentTacticalProjectCount > iTotalActors/2 )
		{
			float fCurrentCount = iCurrentTacticalProjectCount;
			float fTotalActors = iTotalActors;
			// should this start at 1.0?
			//fRange = 1.0 - ( ( fCurrentCount - (fTotalActors/2.0f) ) / fTotalActors );
			// 
			// Range changes from 0.5 to 0.0
			fRange = 0.75 - ( fCurrentCount / (fTotalActors*2.0f) );
		}
		// On a 1000x1000 map, a range of 0.1 is 200 pixels
		if( fRange < 0.1f ){ fRange = 0.1f; }

		pTacticalProject->CreateStrategiesAndBestCaseActionIdeas();// fRange );

		//
		iCurrentTacticalProjectCount++;
		if( iCurrentTacticalProjectCount >= iTotalActors*2 ) break;
	}

	/*
	Do I look over the AIActionDefinitions, look at the attached UnitActionValues and decide
	  whether or not to move forward with trying to schedule the AIActionDefinition?
	Do I iterate over each unit's UnitActionValues, and try to schedule things?
	Also, should I iterate over all the ActionDefinitions and create a few UnitActionValues?
	  Because if a unit is right next to an ActionDefinition, I should probably fulfill it even
	  though it's not in the top 5 (which means it doesn't get evaluated at all).
	  Should I evaluate all the AIActionDefinitions?  (More time, but it's more through.)
	  Should AIActionDefinitions be evaluated according to some 0-1 value, 1 means look at
	    all the Units for possible fulfillment, and less than 1 means look at a subset of the
		close units for fulfillment?  
		Question: How many UnitActionIdeas am I going to create?  Am I going to create too many
		  UnitActionIdeas for the amount of memory/processing that I have?  Calculation:
		  (CitRes count + MultiRegion count + EnemyUnit count + NeutralUnit count) * (MyCity count + MyUnit count)
		  e.g. (40 + 60 + 50 + 50) * (20 + 40) = (200)*(60) = 12,000 UnitActionValues
	*-/

	int z=0;
}
*/
//void  CEOSAIThoughtDatabase::DetermineWhetherAndWhenActionDefinitionsCanBeFulfilled()
/*
void  CEOSAIThoughtDatabase::EstimateWhenTacticalProjectsCanBeCompletedByUnitActionIdeas()
{
	// Sort them by simple score, estimate their completion times
	CEOSAIListSort< CAITacticalProject* >  SortedTacticalProjects;
	SortedTacticalProjects.SetSortFunction( SortTacticalProjectsBySimpleInterest );
	POSITION pos = m_TacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacticalProject = m_TacticalProjects.GetNext( pos );
		SortedTacticalProjects.InsertSorted( pTacticalProject );
	}

	pos = SortedTacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pAITacticalProject = SortedTacticalProjects.GetNext( pos );
		pAITacticalProject->EstimateWhenTacticalProjectCanBeCompletedByUnitActionIdeas();
	}
}
*/
/*
void CEOSAIThoughtDatabase::CalculateSimpleTimeToTargetForActionDefinitionsNeededUnits()
{
	POSITION pos = m_AIActionDefinitions.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pAIActionDefinition = m_AIActionDefinitions.GetNext( pos );
		pAIActionDefinition->CalculateSimpleTimeToTargetForAllNeededUnits();
	}
}
*/
/*
void CEOSAIThoughtDatabase::CalculateRelativeTTTScoresForUnitActionIdeas()
{
	// Create Level3 UnitActionValue Scores
	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = m_TacticalProjects2.GetNext( pos );
		if( pTacticalProject->GetValue() == 0.0f ) continue;

		// Read the relative UnitActionValue for the TacticalProject and the AIUnit
		//   Create a relative action score
		//pTacticalProject->CreateRelativeActionScoresAccordingToAIActionDefinition();
	}
	pos = m_pAIBrain->GetAIObjectList()->GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_pAIBrain->GetAIObjectList()->GetNext( pos );
		CEOSAIPoiObject* pAIPoiObject = dynamic_cast< CEOSAIPoiObject* >( pAIObject );
		if( pAIPoiObject )
		{
			//pAIPoiObject->CreateRelativeActionScoresAccordingToAIPoiObject();
		}
	}
}
*/
void CEOSAIThoughtDatabase::GetTacticalProjectsSortedBySimpleInterest( CEOSAIListSort< CEOSAITacticalProject2* >& SortedList )
{
	SortedList.RemoveAll();
	SortedList.SetSortFunction( SortTacticalProjectsBySimpleInterest );
	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = m_TacticalProjects2.GetNext( pos );
		SortedList.InsertSorted( pTacticalProject );
	}
}
/*
void CEOSAIThoughtDatabase::GetTacticalProjectsSortedByBasicFeasibility( CEOSAIListSort< CAITacticalProject* >& SortedList )
{
	SortedList.RemoveAll();
	SortedList.SetSortFunction( SortTacticalProjectsByBasicFeasibility );
	POSITION pos = m_TacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacticalProject = m_TacticalProjects.GetNext( pos );
		SortedList.InsertSorted( pTacticalProject );
	}
}
*/
/*
void CEOSAIThoughtDatabase::GetTacticalProjectsSortedByInitialEvaluationScore( CEOSAIListSort< CEOSAITacticalProject2* >& SortedList )
{
	SortedList.RemoveAll();
	SortedList.SetSortFunction( SortTacticalProjectsByInitialEvaluationBasicScore );
	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = m_TacticalProjects2.GetNext( pos );
		SortedList.InsertSorted( pTacticalProject );
	}
}
*/
void CEOSAIThoughtDatabase::GetTacticalProjectsSortedByInitialEvaluationScore( CEOSAIListSort< CEOSAITacticalProject2* >& SortedList )
{
	SortedList.RemoveAll();
	SortedList.SetSortFunction( SortTacticalProjectsByInitialEvaluationBasicScore );
	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = m_TacticalProjects2.GetNext( pos );
		SortedList.InsertSorted( pTacticalProject );
	}
}

/*
void CEOSAIThoughtDatabase::GetTacticalProjectsSortedByHypotheticalTaskForceScore( CEOSAIListSort< CAITacticalProject* >& SortedList )
{
	SortedList.RemoveAll();
	SortedList.SetSortFunction( SortTacticalProjectsByBestCaseTaskForceScore );
	POSITION pos = m_TacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacticalProject = m_TacticalProjects.GetNext( pos );
		SortedList.InsertSorted( pTacticalProject );
	}
}
*/
/*
void CEOSAIThoughtDatabase::InvokeMultiRegionPathwayDataForEachOfMyCitiesAndGroundAndSeaUnits()
{
	/-*
	long iPlayer = m_pAIBrain->GetAIPlayerNumber();
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( pAIPoiObject->IsMine() )
		{
			// I don't know if this is quite what I want.
			//   I could do calculations for each unit (based on it's particular movement values)
			//     This would give me more accurate information - ground units prefering ground pathways
			//     Ships using water pathways exclusively 
			//   I can't do that with cities or airfields because they can have many units
			//   Air units move directly and have a range, so no need to calculate them or airfields
			if( dynamic_cast< CEOSAICity* >( pAIPoiObject ) )
			{
				pAIPoiObject->GetInitialState()->GetMultiRegion()->InvokePathwaysToMultiRegion( iPlayer );
			}
			if( dynamic_cast< CEOSAIUnit* >( pAIPoiObject ) )
			{
				CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
				if( pAIUnit->IsAirUnit() ) continue;
				pAIPoiObject->GetInitialState()->GetMultiRegion()->InvokePathwaysToMultiRegion( iPlayer );
			}
		}
	}
	*-/
}
*/
/*
void CEOSAIThoughtDatabase::InvokeCityTransportAndCarrierPathwayData()
{
	long iPlayer = m_pAIBrain->GetAIPlayerNumber();
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( pAIPoiObject->IsMine() )
		{
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity )
			{
				pAICity->GetInitialState()->GetMultiRegion()->InvokePathwaysToMultiRegion( iPlayer );
			}
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				if( pAIUnit->GetNumberOfGroundUnitsThatICanContain() > 0 ||
					pAIUnit->GetNumberOfAirUnitsThatICanContain() > 0 )
				{
					//pAIUnit->GetInitialState()->GetMultiRegion()->InvokeSeaPathways_EverywhereMRX();
					pAIUnit->GetInitialState()->GetMultiRegion()->InvokePathwaysToMultiRegion( iPlayer );
				}
			}
		}
	}
	/-*
	// I probably need to know the transport proximities, so calculate it
	//   Construct pathways for the transports
	CList< CEOSAIUnit* > ExistingTransportsAndCarriers;
	POSITION pos = m_MySeaTransports.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pTransport = m_MySeaTransports.GetNext( pos );
		ExistingTransportsAndCarriers.AddTail( pTransport );
	}
	pos = m_ExistingAirTransports.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pTransport = m_ExistingAirTransports.GetNext( pos );
		ExistingTransportsAndCarriers.AddTail( pTransport );
	}
	pos = m_ExistingAircraftCarriers.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pCarrier = m_ExistingAircraftCarriers.GetNext( pos );
		ExistingTransportsAndCarriers.AddTail( pCarrier );
	}
	pos = ExistingTransportsAndCarriers.GetHeadPosition();
	while( pos )
	{
		// I want to find-out the proximity of all the transports and carriers to the rest
		//   of the world.  I will use this information to figure out what transports are most
		//   useful in picking up which units, and in getting to the ActionDefinition target.
		CEOSAIUnit* pTransportOrCarrier = ExistingTransportsAndCarriers.GetNext( pos );
		CMultiRegion* pMultiRegion = pTransportOrCarrier->GetInitialState()->GetMultiRegion();
		ASSERT( pMultiRegion );
		pMultiRegion->InvokePathwaysToMultiRegion();
	}
	*-/
}

//void CEOSAIThoughtDatabase::InvokeTransportAndImaginaryTransportPathwayData()
void CEOSAIThoughtDatabase::InvokeTransportPathwayData()
{
	long iPlayer = m_pAIBrain->GetAIPlayerNumber();
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( pAIPoiObject->IsMine() )
		{
			/-*
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity )
			{
				pAICity->GetInitialState()->GetMultiRegion()->InvokePathwaysToMultiRegion();
			}
			*-/
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				if( pAIUnit->GetNumberOfGroundUnitsThatICanContain() > 0 )
				{
					// Do a transport-to-everywhere calculation
					// Create a UnitProximity object for each MultiRegion

					//CEOSAIUnitProximity

					//pAIUnit->GetInitialState()->GetMultiRegion()->InvokeSeaPathways_EverywhereMRX();
					pAIUnit->GetInitialState()->GetMultiRegion()->InvokePathwaysToMultiRegion( iPlayer );
				}
			}
		}
	}
}
*/

/*
void CEOSAIThoughtDatabase::EvaluateSchedulingActionDefinitions()
{
	POSITION pos = m_AIActionDefinitions.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pActionDefinition = m_AIActionDefinitions.GetNext( pos );
		pActionDefinition->EvaluateSchedulingActionDefinition();
	}
}
*/
/*
void CEOSAIThoughtDatabase::CalculateUnitActionIdeaScores()
{
	POSITION pos = m_TacticalProjects2.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pTacticalProject = m_TacticalProjects2.GetNext( pos );
		pTacticalProject->CalculateUnitActionIdeaScores();
	}
}
*/
//void CEOSAIThoughtDatabase::CreateATaskForceForEachActionDefinition()
/*
void CEOSAIThoughtDatabase::CreateHypotheticalTaskForce()
{
	CEOSAIListSort< CAITacticalProject* >  SortedList;
	GetActionDefinitionsSortedByBasicFeasibility( SortedList );
	//
	POSITION pos = SortedList.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pActionDefinition = SortedList.GetNext( pos );
		float fScore = pActionDefinition->GetBasicFeasibilityScore();
		if( fScore > 0.0f )
		{
			pActionDefinition->CreateBestCaseTaskForce();
		}
	}
}
*/
/*
void CEOSAIThoughtDatabase::AllocateTaskForce()
{
	CEOSAIListSort< CAITacticalProject* >  SortedList;
	GetTacticalProjectsSortedByHypotheticalTaskForceScore( SortedList );
	//
	POSITION pos = SortedList.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacticalProject = SortedList.GetNext( pos );
		float fScore = pTacticalProject->GetBestCaseTaskForceScore();
		if( fScore > 0.0f )
		{
			pTacticalProject->AllocateTaskForce();
		}
	}
}
*/
/*
void CEOSAIThoughtDatabase::CalculateBasicFeasibilityOfTacticalProjects()
{
	// Evaluate ActionDefinitions
	POSITION pos = m_TacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacticalProject = m_TacticalProjects.GetNext( pos );
		//pActionDefinition->EvaluateDoingActionDefinition();
		pTacticalProject->CalculateBasicFeasibility();
	}
}

void CEOSAIThoughtDatabase::DecideWhichTacticalProjectsToSchedule()
{
}
*/
/*
void CEOSAIThoughtDatabase::EvaluateSchedulingActionDefinitions()
{
	POSITION pos = m_AIActionDefinitions.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pAIActionDefinition = m_AIActionDefinitions.GetNext( pos );
		pAIActionDefinition->EvaluateSchedulingActionDefinition();
	}
}
*/

#ifdef DO_NOT_COMPILE
void CEOSAIThoughtDatabase::ScheduleBuildOrders()
{
	return;

	long iPlayer = m_pAIBrain->GetAIPlayerNumber();

	POSITION pos;
	long iCount = 0;
	//CEOSAIActionRequirements  UnitReq;
	//CEOSAIActionRequirements  UnitExtras;

	// DEBUG
	/*
	iCount = 0;
	pos = m_AIActionDefinitions.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pAIActionDefinition = m_AIActionDefinitions.GetNext( pos );
		if( dynamic_cast< CAITacticalProject_CaptureCitRes* >( pAIActionDefinition ) && iCount < 5 )
		{
			pAIActionDefinition->SetCurrentPosition( CAITacticalProject::PlanToDoThisAction );
			iCount++;
		}
	}
	*/

	// "Level1" means that I need to plan for these ActionDefinitions
	// "Level2" means that these are long-term plans.  I shouldn't plan for them directly, but
	//    they do influence the production values (i may want to reuse the units later).
	CList< CAITacticalProject* >  TacticalProjectsToPlan;
	pos = m_TacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pTacticalProject = m_TacticalProjects.GetNext( pos );
		if( pTacticalProject->GetPlan() == CAITacticalProject::enum_PlanToDoThisAction )
		{
			TacticalProjectsToPlan.AddTail( pTacticalProject );
			//UnitReq.Add( pAIActionDefinition->GetUnmetUnitReqs() );
			//UnitExtras.Add( pAIActionDefinition->GetUnmetUnitExtras() );
		}
	}

	// Now try to allocate these to cities

	// INCOMPLETE: ideally, I would allocate the tasks to the best cities
	//   "Best city" depends on:
	//     Current partially-completed production,
	//     city production value, 
	//     proximity to target,
	//   "Best Unit" depends on:
	//     scheduling the most important build targets first, 
	//     distance from city to target based on unit movement rate,
	//     check the air-range needed compared to the air-range of the air-unit,
	//   "Best city, best unit" combo:
	//     the speed of the unit has an effect on which unit i build (for future use in other tasks)
	//     the "disposition" of the MultiRegion (border, interior) affects unit score

	// City Actors
	long iNumberOfCities = 0;
	pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity ){ iNumberOfCities++; }
	}
	CEOSAIBCDumbArray1D< CEOSAICity* >  CityActors;
	CityActors.SetSize( iNumberOfCities );
	iCount = 0;
	pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			CityActors.Value( iCount ) = pAICity;
			iCount++;
		}
	}

	// Build Options: Infantry, Tank, Transport, Fighter, ...
	long iNumberOfBuildOptions = (long) m_UnitsICanBuild.m_List.GetCount();
	CEOSAIBCDumbArray1D< CUnitTemplate* >  UnitTemplateBuildOptions;
	UnitTemplateBuildOptions.SetSize( iNumberOfBuildOptions );
	iCount = 0;
	pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		UnitTemplateBuildOptions.Value( iCount ) = pUnitTemplate;
		iCount++;
	}
/*
	// BuildValues is a 2d table of [City] x [UnitTemplate] scores
	CEOSAIBCDumbArray2D< float >  BuildValues;
	BuildValues.SetSize( iNumberOfCities, iNumberOfBuildOptions );
	for( long i=0; i<iNumberOfCities; i++ )
	{
		for( long j=0; j<iNumberOfBuildOptions; j++ )
		{
			BuildValues.Value( x,y ) = 0.0f;
		}
	}
*/
	//
	long iCycle = 0;
	bool bDone = false;
	while( bDone == false )
	{
		iCycle++;

		float fBestScore = 0.0f;
		CEOSAICity* pChosenAICity = NULL;
		CUnitTemplate* pChosenUnitTemplate = NULL;
		CAITacticalProject* pChosenAITacticalProject = NULL;
		//
		// Do all my cities have build orders?
		bool bAllMyCitiesHaveBuildOrdersThroughTheNextTurn = true;
		for( long iCity=0; iCity<CityActors.m_iSize; iCity++ )
		{
			CEOSAICity* pAICity = CityActors.Value( iCity );
			if( pAICity->GetServerCity()->GetNumberOfBuildOrders() < 2 )
			{
				bAllMyCitiesHaveBuildOrdersThroughTheNextTurn = false;
				break;
			}
			/*
			if( pAICity->GetCity()->BuildQueueWillBeEmptyThisTurn() )
			{
				bAllMyCitiesHaveBuildOrdersThroughTheNextTurn = false;
				break;
			}
			*/
		}
		if( bAllMyCitiesHaveBuildOrdersThroughTheNextTurn )
		{
			bDone = true;
			continue;
		}

		//
		long  iCount = 0;
		float fOrderValue = 0.0f;
		pos = TacticalProjectsToPlan.GetHeadPosition();
		while( pos )
		{
			CAITacticalProject* pTacticalProject = TacticalProjectsToPlan.GetNext( pos );
			if( pTacticalProject->GetPlan() != CAITacticalProject::enum_PlanToDoThisAction )
			{
				continue;
			}

			float fTacticalProjectScore = 1.0f; //pActionDefinition->GetESScore();
		}

		// This is actually allowed, but if it happens I should use national desires to 
		//   guide city production (otherwise I'll have cities without build orders).
		ASSERT( fBestScore > 0.0f );
		ASSERT( pChosenAICity );
		ASSERT( pChosenUnitTemplate );
		ASSERT( pChosenAITacticalProject );
		//
		if( fBestScore > 0.0f && pChosenAICity && pChosenUnitTemplate && pChosenAITacticalProject )
		{
			//pChosenAICity->AddToBuildQueue( pChosenAIUnitTemplate->GetUnitTemplate()->GetBuildOption() );

			// Attach the BuildAction to the TacticalProject so the Unmet scores get adjusted
			CEOSAICityBuildAction* pAICityBuildAction = new CEOSAICityBuildAction();
			pAICityBuildAction->SetCity( pChosenAICity );
			pAICityBuildAction->SetBuild( pChosenUnitTemplate->GetBuildOption() );
			pAICityBuildAction->SetUnitTemplate( pChosenUnitTemplate );
			pAICityBuildAction->SetTacticalProject( pChosenAITacticalProject );
			pChosenAICity->AppendBuildAction( pAICityBuildAction );

			pChosenAITacticalProject->AddCityBuildAction( pAICityBuildAction );
		}
		else
		{
			// If I have no results (perhaps because there are no pathways) exit out so
			//   the AI doesn't hang.
			bDone = true;
		}
	}

	int z=0;

	// Should I setup a table of Cities x UnitTemplate scores
	/*
	pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			// Look over the potential build targets, chose the best one based on the UnitReq,UnitExtras
			CEOSAIUnitTemplate* pBestAIUnitTemplate = NULL;
			float fBestValue = 0.0f;
			POSITION pos2 = m_UnitsICanBuild.m_List.GetHeadPosition();
			while( pos2 )
			{
				CEOSAIUnitTemplate* pAIUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos2 );
				float fValue = 0.0f;
				fValue += UnitReq.GetValue( pAIUnitTemplate );
				fValue += 0.5f * UnitExtras.GetValue( pAIUnitTemplate );

				if( fValue > fBestValue )
				{
					pBestAIUnitTemplate = pAIUnitTemplate;
					fBestValue = fValue;
				}
			}

			if( pBestAIUnitTemplate )
			{
				//CBuildOption* pNewBuildOption = GetCommonState()->GetBuildOptions()->GetBuildOption( pBestAIUnitTemplate->GetUnitTemplate()->GetInternalName() );
				//ASSERT( pNewBuildOption );
				//CBuildOption* pNewBuildOption = pBestAIUnitTemplate->GetUnitTemplate()->GetBuildOption();
				float fBuildCompletion = pAICity->GetCompletionTimeForNewBuildOption( pBuildOption );
				if( fBuildCompletion < 100.0f )
				{
					pAICity->AddToBuildQueue( pBuildOption );
				}

				UnitReq.Subtract( pBestAIUnitTemplate );
				UnitExtras.Subtract( pBestAIUnitTemplate );
			}
		}
	}
	*/
}
#endif DO_NOT_COMPILE

/*
void CEOSAIThoughtDatabase::ScheduleAITacticalProjects()
{
	// Scheduling is based on the Allocated UnitActionIdeas
	{
		/-*
		POSITION pos = m_MyActors.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pActor = m_MyActors.GetNext( pos );

			CEOSAIUnitActionIdea* pUnitActionIdea = pActor->GetAllocatedUnitActionIdea();
			if( pUnitActionIdea )
			{
				CAITacticalProject* pTacticalProject = pUnitActionIdea->GetTaskForce()->GetTacticalProject();
				ASSERT( pTacticalProject );
				//pActor->ScheduleAllocatedUnitActionIdea();
				pTacticalProject->ScheduleUnitActionIdea( pUnitActionIdea );
			}
		}
		*-/

		return;
	}

	// Make sure I create some decent AIActionOutlines and take transports into account
	// (If there are no nearby transports, the AIUnitActionIdea gets a lower score)

	// Sort the AIActionDefinitions by their ESScore
	//   Begin scheduling them
	CEOSAIListSort< CAITacticalProject* >  SortedTacticalProjects;
	GetTacticalProjectsSortedByBasicFeasibility( SortedTacticalProjects );
	/-*
	SortedActionDefinitions.SetSortFunction( SortActionDefinitionsByBasicFeasibility );
	POSITION pos = m_AIActionDefinitions.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pAIActionDefinition = m_AIActionDefinitions.GetNext( pos );
		SortedActionDefinitions.InsertSorted( pAIActionDefinition );
	}
	*-/

	// Go through and schedule ActionDefinitions until I can't anymore
	long iCount = 0;
	POSITION pos = SortedTacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pAITacticalProject = SortedTacticalProjects.GetNext( pos );
		iCount++;
		/-*
		if( dynamic_cast< CAITacticalProject_CaptureCitRes* >( pAITacticalProject ) )
		{
			pAIActionDefinition->ScheduleAction();
		}
		*-/
		if( iCount == 1 ) break;
	}

	//
	/-*
	bool bScheduleSomeMoreActions = true;
	while( bScheduleSomeMoreActions )
	{
		// Find the highest-scoring AIActionDefinition
		CAITacticalProject* pAIActionDefinition = NULL;
		float fBestScore = 0.0f;
		POSITION pos = m_AIActionDefinitions.GetHeadPosition();
		while( pos )
		{
			CAITacticalProject* pAIActionDefinitionInList = m_AIActionDefinitions.GetNext( pos );
			if( pAIActionDefinitionInList->GetESScore() > fBestScore )
			{
				pAIActionDefinition = pAIActionDefinitionInList;
				fBestScore = pAIActionDefinition->GetESScore();
			}
		}
		if( dynamic_cast< CAITacticalProject_CaptureCitRes* >( pAIActionDefinition ) )
		{
			pAIActionDefinition->ScheduleAction();
		}
		//

		bScheduleSomeMoreActions = false;
	}
	*-/

	// What ActionDefinitions do I want to plan?
	//   I should get an estimate of the amount of unmet needs for each AIActionDefinition
	//   I should "plan" to do all the ActionDefinitions up to a certain cost.
	iCount = 0;
	pos = SortedTacticalProjects.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pAITacticalProject = SortedTacticalProjects.GetNext( pos );
		/-*
		if( dynamic_cast< CAITacticalProject_CaptureCitRes* >( pAIActionDefinition ) && iCount < 5 )
		{
			if( pAIActionDefinition->GetCurrentPosition() == CAITacticalProject::enum_LackSufficientForces )
			{
				pAIActionDefinition->SetCurrentPosition( CAITacticalProject::enum_PlanToDoThisAction );
				iCount++;
			}
		}
		*-/
	}

	// TESTING: Schedule the first one that doesn't require a transport.  That's all.
	/*
	// Sort the AIActionDefinitions by their best "UnitActionValue.SimpleInterestAndAIPoiObjectRelativeScore"
	CEOSAIListSort< CAITacticalProject* >  SortedActionDefinitions;
	SortedActionDefinitions.SetSortFunction( SortActionDefinitionsByBestSimpleInterestAndAIPoiObjectRelativeScore );

	POSITION pos = m_AIActionDefinitions.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pActionDefinition = m_AIActionDefinitions.GetNext( pos );
		SortedActionDefinitions.InsertSorted( pActionDefinition );
	}

	pos = SortedActionDefinitions.GetHeadPosition();
	while( pos )
	{
		CAITacticalProject* pActionDefinition = SortedActionDefinitions.GetNext( pos );

		if( dynamic_cast< CAITacticalProject_CaptureCitRes* >( pActionDefinition ) == NULL )
		{
			continue;
		}

		// Do I need a transport to accomplish this Action?
		if( pActionDefinition->GetRequiresATransport() == false )
		{
			POSITION pos2 = pActionDefinition->GetAllMyUnitActionIdeas()->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIUnitActionIdea* pAIUnitActionIdea = pActionDefinition->GetAllMyUnitActionIdeas()->GetNext( pos2 );
				if( pAIUnitActionIdea->GetRequiresATransport() == false )
				{
					pActionDefinition->ScheduleAction( pAIUnitActionIdea );
					return;
				}
			}
		}
	}
	*-/
}
*/

/*
void CEOSAIThoughtDatabase::Process_MultiRegions_ShouldSendUnitsHere()
{
	// Sort the objects by "m_fSimpleInterestInGoingHere"
	CEOSAIListSort< CAIThought_ShouldSendUnitsHere* >  m_SortedList;
	m_SortedList.SetSortFunction( SortShouldSendUnitsHereBySimpleInterest );

	POSITION pos = m_AIThoughts_ShouldSendUnitsHere.GetHeadPosition();
	while( pos )
	{
		CAIThought_ShouldSendUnitsHere* pShouldSendUnitsThought = m_AIThoughts_ShouldSendUnitsHere.GetNext( pos );
		m_SortedList.InsertSorted( pShouldSendUnitsThought );
	}

	// Go through the sorted list, come up with some pathways from the other multiregions.
	//   These pathways include danger, sighting, presence data.  By finding pathways from
	//   the MultiRegions, we can quickly find land/sea/air routes for units in those multiregions.
	long iCount = 0;
	pos = m_SortedList.GetHeadPosition();
	while( pos )
	{
		CAIThought_ShouldSendUnitsHere* pShouldSendUnitsHere = m_SortedList.GetNext( pos );
		//pShouldSendUnitsThought->m_pMultiRegion->InvokePathwaysToMultiRegion();
		//pShouldSendUnitsThought->m_pMultiRegion->InvokeThought_WhatUnitsCanIGetToMultiRegion( CEOSAIActionRequirement::enum_GroundCitResHunter );
		//
		//CAIThought_WhatUnitsCanIGetToMultiRegion* pThought =
		//	new CAIThought_WhatUnitsCanIGetToMultiRegion( pShouldSendUnitsHere->m_pMultiRegion, CEOSAIActionRequirement::enum_GroundCitResHunter );

		//
		iCount++;
		if( iCount > 5 ) break;
	}
}
*/
/*
long CEOSAIThoughtDatabase::GetScoreCategory( float fScore )
{
	if( fScore < 0.0f ){ return 0; }
	if( fScore < 5.0f ){ return 1; }
	if( fScore < 10.0f ){ return 2; }
	if( fScore < 20.0f ){ return 3; }
	if( fScore < 40.0f ){ return 4; }
	if( fScore < 80.0f ){ return 5; }
	if( fScore < 160.0f ){ return 6; }
	if( fScore < 320.0f ){ return 7; }
	if( fScore < 640.0f ){ return 8; }
	return 9;
}
*/
/*
EnumAIThoughtScoreCategory  CEOSAIThoughtDatabase::GetScoreCategory( float fScore )
{
	if( fScore <=  0.0f ){ return EnumAIThoughtScoreCategory_Bad; }
	if( fScore <= 10.0f ){ return EnumAIThoughtScoreCategory_Inconvenient; }
	if( fScore <= 20.0f ){ return EnumAIThoughtScoreCategory_Decent; }
	if( fScore <= 30.0f ){ return EnumAIThoughtScoreCategory_Good; }
	if( fScore <= 40.0f ){ return EnumAIThoughtScoreCategory_Excellent; }
	if( fScore <= 50.0f ){ return EnumAIThoughtScoreCategory_Necessary; }
	ASSERT( false );
	// If this happens on a release build, we want it to work, so
	//   categorize it as "necessary".
	return EnumAIThoughtScoreCategory_Necessary;
}
*/
/*
void  CEOSAIThoughtDatabase::AddToThoughtScoreTable( CAIThought* pThought )
{
	long iThoughtDepth = pThought->GetThoughtDepth();
	ASSERT( iThoughtDepth > 0 && iThoughtDepth <= 10 );

	long iScoreCategory = GetScoreCategory( pThought->GetScore() );
	ASSERT( iScoreCategory > 0 && iScoreCategory <= 9 );

	if( iThoughtDepth > 0  && iThoughtDepth <= 10 &&
		iScoreCategory > 0 && iScoreCategory <= 9 )
	{
		m_ThoughtScoreTable[ iThoughtDepth ][ iScoreCategory ].AddTail( pThought );
	}
}

void  CEOSAIThoughtDatabase::RemoveFromThoughtScoreTable( CAIThought* pThought )
{
	long iThoughtDepth = pThought->GetThoughtDepth();
	ASSERT( iThoughtDepth > 0 && iThoughtDepth <= 10 );

	long iScoreCategory = GetScoreCategory( pThought->GetScore() );
	ASSERT( iScoreCategory > 0 && iScoreCategory <= 9 );

	if( iThoughtDepth > 0  && iThoughtDepth <= 10 &&
		iScoreCategory > 0 && iScoreCategory <= 9 )
	{
		POSITION pos = m_ThoughtScoreTable[ iThoughtDepth ][ iScoreCategory ].GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CAIThought* pThoughtInList = m_ThoughtScoreTable[ iThoughtDepth ][ iScoreCategory ].GetNext( pos );
			if( pThought == pThoughtInList )
			{
				m_ThoughtScoreTable[ iThoughtDepth ][ iScoreCategory ].RemoveAt( prevPos );
				return;
			}
		}
		ASSERT( false );  // didn't find the Thought in the list
	}
	/*
	long  iThoughtDepth = pThought->GetThoughtDepth();
	float fScore = pThought->GetScore();
	ASSERT( iThoughtDepth > 0 && iThoughtDepth <= 10 );
	if( iThoughtDepth > 0 && iThoughtDepth <= 10 )
	{
		m_ThoughtScoreTable[ iThoughtDepth ].Insert( fScore, pThought );
	}
	*-/
}
*/

/*
void  CEOSAIThoughtDatabase::UpdatePositionInThoughtScoreTable( CAIThought* pThought )
{
	if( pThought->GetThoughtScoreTable_CurrentScoreCategory() != -1 &&
		pThought->GetThoughtScoreTable_CurrentThoughtDepth() != -1 )
	{
		long iThoughtDepth = pThought->GetThoughtScoreTable_CurrentThoughtDepth();
		ASSERT( iThoughtDepth > 0 && iThoughtDepth <= 10 );

		long iScoreCategory = pThought->GetThoughtScoreTable_CurrentScoreCategory();
		ASSERT( iScoreCategory > 0 && iScoreCategory <= 9 );

		if( iThoughtDepth > 0  && iThoughtDepth <= 10 &&
			iScoreCategory > 0 && iScoreCategory <= 9 )
		{
			POSITION pos = m_ThoughtScoreTable[ iThoughtDepth ][ iScoreCategory ].GetHeadPosition();
			while( pos )
			{
				POSITION prevPos = pos;
				CAIThought* pThoughtInList = m_ThoughtScoreTable[ iThoughtDepth ][ iScoreCategory ].GetNext( pos );
				if( pThought == pThoughtInList )
				{
					m_ThoughtScoreTable[ iThoughtDepth ][ iScoreCategory ].RemoveAt( prevPos );
					return;
				}
			}
			ASSERT( false );  // didn't find the Thought in the list
		}
	}
	// Put it (back) into the ThoughtScoreTable
	long iThoughtDepth = pThought->GetThoughtDepth();
	ASSERT( iThoughtDepth > 0 && iThoughtDepth <= 10 );

	long iScoreCategory = GetScoreCategory( pThought->GetScore() );
	ASSERT( iScoreCategory > 0 && iScoreCategory <= 9 );

	if( iThoughtDepth > 0  && iThoughtDepth <= 10 &&
		iScoreCategory > 0 && iScoreCategory <= 9 )
	{
		m_ThoughtScoreTable[ iThoughtDepth ][ iScoreCategory ].AddTail( pThought );
		pThought->SetThoughtScoreTable_CurrentScoreCategory( iScoreCategory );
		pThought->SetThoughtScoreTable_CurrentThoughtDepth( iThoughtDepth );
	}
}
*/

/*
void  CEOSAIThoughtDatabase::RemoveFromThoughtCategory( EnumAIThoughtScoreCategory eCategory, CAIThought* pThought )
{
	if( eCategory == EnumAIThoughtScoreCategory_Undefined    ){ return; }

	CList< CAIThought* >* pCategoryList;
	if( eCategory == EnumAIThoughtScoreCategory_Necessary    ){ pCategoryList = &m_NecessaryThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Excellent    ){ pCategoryList = &m_ExcellentThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Good         ){ pCategoryList = &m_GoodThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Decent       ){ pCategoryList = &m_DecentThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Inconvenient ){ pCategoryList = &m_InconvenientThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Bad          ){ pCategoryList = &m_BadThoughtList; }
	ASSERT( pCategoryList );

	POSITION prevPos;
	POSITION pos = pCategoryList->GetHeadPosition();
	while( pos )
	{
		prevPos = pos;
		CAIThought* pThoughtInList = pCategoryList->GetNext( pos );
		if( pThoughtInList == pThought )
		{
			pCategoryList->RemoveAt( prevPos );
		}
	}
}

void  CEOSAIThoughtDatabase::AddToThoughtCategory( EnumAIThoughtScoreCategory eCategory, CAIThought* pThought )
{
	if( eCategory == EnumAIThoughtScoreCategory_Undefined    ){ return; }

	CList< CAIThought* >* pCategoryList;
	if( eCategory == EnumAIThoughtScoreCategory_Necessary    ){ pCategoryList = &m_NecessaryThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Excellent    ){ pCategoryList = &m_ExcellentThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Good         ){ pCategoryList = &m_GoodThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Decent       ){ pCategoryList = &m_DecentThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Inconvenient ){ pCategoryList = &m_InconvenientThoughtList; }
	if( eCategory == EnumAIThoughtScoreCategory_Bad          ){ pCategoryList = &m_BadThoughtList; }
	ASSERT( pCategoryList );

	pCategoryList->AddTail( pThought );
}
*/
/*
CEOSAIUnitTemplate* CEOSAIThoughtDatabase::GetAIUnitTemplate( CString strUnitTemplateName )
{
	POSITION pos = m_AIUnitTemplates.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_AIUnitTemplates.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->GetInternalName() == strUnitTemplateName )
		{
			return pAIUnitTemplate;
		}
	}
	return NULL;
}
*/

/*
void CEOSAIThoughtDatabase::CreateUnitActionValues_old()
{
	// These answers will help to guide which ActionDefinitions->UnitActionValues I'm going to focus on.

	//
	//ProcessActionDefinitions_CreateUnitActionValues();
	CreateUnitActionValues();

	//CList< CMultiRegion* >  SortedMultiRegionList;
	//m_pAIBrain->GetMultiRegionManager()->GetMultiRegionListSortedByDistanceFromMyPresence( &SortedMultiRegionList );

	// Think about the places I'd like to explore
	//m_pAIThought_WhatPlacesWouldILikeToExplore = new CAIThought_WhatPlacesWouldILikeToExplore( m_pAIBrain );
	//m_pAIThought_WhatPlacesWouldILikeToExplore->ThinkAboutIt();

	// At this point, I should have a bunch of thoughts in the database.
	//   I can classify these thoughts into groups (good, fair, bad)
	//   and work through them.  Look at the "good" thoughts, evaluate them
	//   a little more, perhaps downgrade them.  Then look through them
	//   again and find ways to accomplish them.

	// If there is a shortage of good-thoughts, I can start looking at ways
	//   to accomplish the "fair" thoughts.

	// I can also use this opportunity to evaluate ways to improve the
	//   scores of thoughts, if, for example, they need transports or aircraft


	// Create a thought - capture area 51 (contains a city, requires a transport)
	

	/*
	for( long iScoreCategory=SCORE_CATEGORY_COUNT-1; iScoreCategory>0; iScoreCategory-- )
	{
		for( long iDepthOfThought=0; iDepthOfThought<DEPTH_OF_THOUGHT_COUNT; iDepthOfThought++ )
		{
			CList< CAIThought* >* pThoughtCategoryList = &m_ThoughtScoreTable[ iDepthOfThought ][ iScoreCategory ];

			// Start looking at the excellent thoughts.  They seem promising, but
			//   I need to refine them a bit - they might score lower when I
			//   flesh them out a bit.  When they score lower, I might drop it's
			//   score a bit, not impliment it, and look at some other possibilities.
			//   Those other possibilities might actually be a little better.

			// When a AIPoiObject is assigned to carry-out some task, the values
			//   of performing other nearby tasks increases.  For example, an aircraft
			//   that is scheduled to explore a region will alter it's scores for
			//   exploring other regions nearby the task.  When a unit is assigned
			//   a task, it needs to update the scores of its other tasks.

			// Think about the ideas some more
			//   If all my units are busy doing stuff, I can stop thinking
			//   about new actions.
			// I should also include Thoughts that are centered around units -
			//   "What do I want to do with this unit?"  So that I am fully utilizing
			//   my units.
			POSITION pos = pThoughtCategoryList->GetHeadPosition();
			while( pos )
			{
				CAIThought* pThought = pThoughtCategoryList->GetNext( pos );
				if( pThought->IsDoneThinking() == false )
				{
					//pThought->ThinkAboutIt();
				}
			}
			//
		}
	}
	*/

/*
	CList< CAIThought* > ThoughtCategoryList;
	for( long iCategory=0; iCategory<m_ThoughtScoreTable[0].m_iCategories; iCategory++ )
	{
		for( long iDepthOfThought=0; iDepthOfThought<11; iDepthOfThought++ )
		{
			POSITION pos = m_ThoughtScoreTable[iDepthOfThought].m_Array[iCategory].GetHeadPosition();
			while( pos )
			{
				CAIThought* pAIThought = m_ThoughtScoreTable[iDepthOfThought].m_Array[iCategory].GetNext( pos );
				ThoughtCategoryList.AddTail( pAIThought );
			}
		}
	}

	// Start looking at the excellent thoughts.  They seem promising, but
	//   I need to refine them a bit - they might score lower when I
	//   flesh them out a bit.  When they score lower, I might drop it's
	//   score a bit, not impliment it, and look at some other possibilities.
	//   Those other possibilities might actually be a little better.

	// When a AIPoiObject is assigned to carry-out some task, the values
	//   of performing other nearby tasks increases.  For example, an aircraft
	//   that is scheduled to explore a region will alter it's scores for
	//   exploring other regions nearby the task.  When a unit is assigned
	//   a task, it needs to update the scores of its other tasks.

	// Think about the ideas some more
	//   If all my units are busy doing stuff, I can stop thinking
	//   about new actions.
	// I should also include Thoughts that are centered around units -
	//   "What do I want to do with this unit?"  So that I am fully utilizing
	//   my units.
	POSITION pos = pThoughtCategoryList->GetHeadPosition();
	while( pos )
	{
		CAIThought* pThought = pThoughtCategoryList->GetNext( pos );
		if( pThought->IsDoneThinking() == false )
		{
			//pThought->ThinkAboutIt();
		}
	}
*/
/*
	CList< CAIThought* >* pThoughtCategoryList;
	for( long i=0; i<3; i++ )
	{
		if( i == 0 ){ pThoughtCategoryList = &m_NecessaryThoughtList; }
		if( i == 1 ){ pThoughtCategoryList = &m_ExcellentThoughtList; }
		if( i == 2 ){ pThoughtCategoryList = &m_GoodThoughtList; }
		if( i == 3 ){ pThoughtCategoryList = &m_DecentThoughtList; }
		if( i == 4 ){ pThoughtCategoryList = &m_InconvenientThoughtList; }

		// Start looking at the excellent thoughts.  They seem promising, but
		//   I need to refine them a bit - they might score lower when I
		//   flesh them out a bit.  When they score lower, I might drop it's
		//   score a bit, not impliment it, and look at some other possibilities.
		//   Those other possibilities might actually be a little better.

		// When a AIPoiObject is assigned to carry-out some task, the values
		//   of performing other nearby tasks increases.  For example, an aircraft
		//   that is scheduled to explore a region will alter it's scores for
		//   exploring other regions nearby the task.  When a unit is assigned
		//   a task, it needs to update the scores of its other tasks.

		// Think about the ideas some more
		//   If all my units are busy doing stuff, I can stop thinking
		//   about new actions.
		// I should also include Thoughts that are centered around units -
		//   "What do I want to do with this unit?"  So that I am fully utilizing
		//   my units.
		POSITION pos = pThoughtCategoryList->GetHeadPosition();
		while( pos )
		{
			CAIThought* pThought = pThoughtCategoryList->GetNext( pos );
			if( pThought->IsDoneThinking() == false )
			{
				//pThought->ThinkAboutIt();
			}
		}
	}
*-/
	// Create the actions to perform the thought
	POSITION pos = m_ThoughtList.GetHeadPosition();
	while( pos )
	{
		CAIThought* pThought = m_ThoughtList.GetNext( pos );
		if( pThought->IWillDoThis() )
		{
			pThought->ImplementThis();
		}
	}
}
*/
/*
bool  CEOSAIThoughtDatabase::DoIHaveAnyCityHuntersOrCitiesOnTheGeo( CEOSAILocation Location )
{
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	ASSERT( pWorldDescServer );
	if( pWorldDescServer )
	{
		long iTargetGeo = pWorldDescServer->AICheat_GetGeoNumber( Location );

		POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = m_pAIBrain->GetAIPoiObjectList()->GetNext( pos );

			if( pAIPoiObject->IsMine() )
			{
				CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
				if( pAIUnit )
				{
					if( pAIUnit->IsOfType( EOSAIEnumTypeOfUnit_CityHunter ) )
					{
						long iMyUnitGeo = pWorldDescServer->AICheat_GetGeoNumber( pAIUnit->GetInitialState()->GetLocation() );
						if( iTargetGeo == iMyUnitGeo ) return true;
					}
				}
				CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
				if( pAICity )
				{
					long iMyCityGeo = pWorldDescServer->AICheat_GetGeoNumber( pAICity->GetInitialState()->GetLocation() );
					if( iTargetGeo == iMyCityGeo ) return true;
				}
			}
		}
	}
	return false;
}
*/
// private
/*
void CEOSAIThoughtDatabase::CreateAIUnitTemplatesForAllUnitTemplates()
{
	// Delete the old information
	while( m_AIUnitTemplates.IsEmpty() == FALSE )
	{
		delete m_AIUnitTemplates.RemoveHead();
	}

	CList< CBuildOption* >* pBuildOptions = GetCommonState()->GetActiveUnitset()->GetBuildOptionsList();
	POSITION pos = pBuildOptions->GetHeadPosition();
	while( pos )
	{
		CBuildOption* pBuildOption = pBuildOptions->GetNext( pos );
		if( pBuildOption->IsAUnit() )
		{
			CEOSAIUnitTemplate* pAIUnitTemplate = new CEOSAIUnitTemplate( pBuildOption->GetUnitTemplate() );
			m_AIUnitTemplates.AddTail( pAIUnitTemplate );
		}
	}
}
*/
void CEOSAIThoughtDatabase::CalculateUnitsICanBuildOrHave()
{
	// Delete the old information
	//m_AllUnitsInTheGame.Clear();
	m_AllUnitsInTheGameOrBuildableByMe.Clear();
	m_UnitsICanBuild.Clear();
	m_UnitsICanBuildOrHave.Clear();
	m_UnitsIHave.Clear();
	//m_SubmarineUnitsInTheGame.Clear();

	// Find all units currently in the game (or buildable)
	//CUnitset* pUnitset = GetCommonState()->GetActiveUnitset();
	//long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	CList< CEOSAIBuildOption* >* pBuildOptions = g_pEOSAICommonData->GetAIBuildOptionList();

	//
	// Units I can Build
	//
	POSITION pos = pBuildOptions->GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildOption* pBuildOption = pBuildOptions->GetNext( pos );
		if( pBuildOption->IsAUnit() )
		{
			//if( pUnitset->CanBuild( strMyUnitsubset, pBuildOption, false ) )
			if( g_pEOSAIMain->GetAIGameRules()->CanBuild( m_pAIBrain->GetAIPlayerNumber(), pBuildOption, false ) )
			{
				CEOSAIUnitTemplate* pUnitTemplate = pBuildOption->GetAIUnitTemplate();
				ASSERT( pUnitTemplate );
				//
				m_AllUnitsInTheGameOrBuildableByMe.AddUnitTemplate( pUnitTemplate );
				//CEOSAIUnitTemplate* pAIUnitTemplate = new CEOSAIUnitTemplate( pBuildOption->GetUnitTemplate() );
				//m_UnitsICanBuild.AddTail( pAIUnitTemplate );
				//m_UnitsICanBuildOrHave.AddTail( pAIUnitTemplate );
				m_UnitsICanBuild.AddUnitTemplate_Unsafe( pUnitTemplate );
				m_UnitsICanBuildOrHave.AddUnitTemplate_Unsafe( pUnitTemplate );
			}
		}
	}

	//
	//pos = m_pAIBrain->GetWorldDescServer()->GetPoiList()->GetHeadPosition();
	pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoi = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAIUnit* pUnit = dynamic_cast< CEOSAIUnit* >( pPoi );
		if( pUnit == NULL ) continue;
		//if( pUnit->IsDead() ) continue;
		if( pUnit->IsAlive() == false ) continue;

		CEOSAIUnitTemplate* pUnitTemplate = pUnit->GetAIUnitTemplate();
		ASSERT( pUnitTemplate );

		m_AllUnitsInTheGameOrBuildableByMe.AddUnitTemplate( pUnitTemplate );

		if( pUnit->GetOwner() == m_pAIBrain->GetAIPlayerNumber() )
		{
			m_UnitsIHave.AddUnitTemplate( pUnitTemplate );
			m_UnitsICanBuildOrHave.AddUnitTemplate( pUnitTemplate );
		}
	}
/*
	//
	// Identify which units of mine are "submarines"
	//
	float fTotalSeaUnitViewDistance = 0.0f;
	long iCount = 0;
	pos = m_AllUnitsInTheGameOrBuildableByMe.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_AllUnitsInTheGameOrBuildableByMe.m_List.GetNext( pos );
		if( pUnitTemplate && pUnitTemplate->IsSeaUnit() )
		{
			fTotalSeaUnitViewDistance += pUnitTemplate->GetUnitCombatCapability()->GetAverageVisibleRange();
			iCount++;
		}
	}
	float fAverageSeaUnitViewDistance = 0.0f;
	if( iCount > 0 )
	{
		fAverageSeaUnitViewDistance = fTotalSeaUnitViewDistance / iCount;

		pos = m_AllUnitsInTheGameOrBuildableByMe.m_List.GetHeadPosition();
		while( pos )
		{
			CUnitTemplate* pUnitTemplate = m_AllUnitsInTheGameOrBuildableByMe.m_List.GetNext( pos );
			if( pUnitTemplate && pUnitTemplate->IsSeaUnit() )
			{
				float fViewDistance = pUnitTemplate->GetUnitCombatCapability()->GetAverageVisibleRange();
				if( fViewDistance < fAverageSeaUnitViewDistance*0.50f )
				{
					// This is a submarine
					m_SubmarineUnitsInTheGame.AddUnitTemplate( pUnitTemplate );
				}
			}
		}
	}
	//CUnitTemplate* pSub = g_pCommonState->GetActiveUnitset()->GetUnitTemplate( _T("Submarine (Class 1)") );
	//float fDist = pSub->GetUnitCombatCapability()->GetAverageVisibleRange();
	int h=0;
*/

	// Build national unit capabilities
	//
	/*
	m_NationalCapabilities.Clear();
	pos = m_UnitsIHave.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_UnitsIHave.m_List.GetNext( pos );
		m_NationalCapabilities.Add( pUnitTemplate );

		//float f = 0.0f;
		//m_NationalCapabilities.m_PowerAgainstCombatTypes.GetValue( "Soft", &f );
		//int z=0;
	}
	*/
	long x=0;
	// Based on my Production capability, I should add a little bit to the NationalCapabilities
	//   (Otherwise, early in the game, it will look like I have no National Capabilities and
	//    I use the National Capabilities information to consider various attack ideas.)
	/*
	m_NationalCapabilities.m_fAntiAircraftCombat += 3.0f;
	m_NationalCapabilities.m_fAntiArmorCombat += 3.0f;
	//m_NationalCapabilities.m_fAntiMissileCombat += 3.0f;
	m_NationalCapabilities.m_fAntiShipCombat += 3.0f;
	m_NationalCapabilities.m_fAntiSoftCombat += 3.0f;
	m_NationalCapabilities.m_fAntiSubmarineCombat += 3.0f;
	*/
	/*
	m_NationalCapabilities.m_fStrategicBombing += 3.0f;
	m_NationalCapabilities.m_iAirfieldBuilder += 1.0f;
	m_NationalCapabilities.m_iCarrierCapacity += 1.0f;
	m_NationalCapabilities.m_iGroundCitResHunter += 3.0f;
	m_NationalCapabilities.m_iSeaResHunter += 3.0f;
	*/
}

void CEOSAIThoughtDatabase::CalculatePotentialEnemyUnits()
{
	m_PotentialEnemyUnits.Clear();

	//
	// Units other players have (include 'allies' in this list - just in case they turn on me)
	//
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAIUnit* pUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pUnit && pUnit->GetForeignRelationsTo( iAIPlayer ) <= EOSAIEnumForeignRelations::enum_Alliance )
		{
			CEOSAIUnitTemplate* pUnitTemplate = pUnit->GetAIUnitTemplate();
			ASSERT( pUnitTemplate );

			m_PotentialEnemyUnits.AddUnitTemplate( pUnitTemplate );
		}
	}
	int z=0;
}
/*
void CEOSAIThoughtDatabase::CalculateCombatAndAttritionValues()
{
	ASSERT( m_AllUnitsInTheGame.m_List.IsEmpty() == FALSE );

	// Invoke all the CombatAttrition values between MyUnits and PotentialEnemy units
	ASSERT( m_AllUnitsInTheGame.m_List.IsEmpty() == FALSE );
	POSITION pos = m_AllUnitsInTheGame.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pMyUnit = m_AllUnitsInTheGame.m_List.GetNext( pos );
		pMyUnit->GetUnitCombatCapability()->CalculateValue( &m_AllUnitsInTheGame );
		int g=0;
	}
	pos = m_AllUnitsInTheGame.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pMyUnit = m_AllUnitsInTheGame.m_List.GetNext( pos );
		//pMyUnit->GetUnitCombatCapability()->CalculateRelativeValue( &m_AllUnitsInTheGame, &m_AllUnitsInTheGame );
		int g=0;
	}

	// DEBUG - What's the relative value?
	pos = m_AllUnitsInTheGame.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pMyUnit = m_AllUnitsInTheGame.m_List.GetNext( pos );

		long iArraySize = pMyUnit->GetUnitCombatCapability()->GetArray()->m_iSize;
		for( long i=0; i<iArraySize; i++ )
		{
			CEOSAIUnitVsUnitValue* pVal = pMyUnit->GetUnitCombatCapability()->GetArray()->Value(i);
			if( pVal == NULL ) continue;
			CUnitTemplate* pTargetUnitTemplate = pVal->GetTargetUnitTemplate();
			float f1 = pVal->GetMeleeAttrition01();
			float f2 = pVal->GetRangedAttrition01();
			float f3 = pVal->GetRangeX2Attrition01();
			//
			//bool b = pVal->IsUsefulInCombat();
			int g=0;
		}

		//bool   IsACombatUnit(){ return m_fCombatAttritionSignificance01 > 0.3f; }
		//float  GetCombatSignificance(); // Unit Cost * m_fCombatAttritionSignificance
		//float  GetCombatAttritionSignificance01(){ return m_fCombatAttritionSignificance01; }
		bool  bIsACombatUnit = pMyUnit->GetUnitCombatCapability()->IsACombatUnit();
		float fCombatSignificance = pMyUnit->GetUnitCombatCapability()->GetCombatSignificance();
		float fCombatAttritionSignificance = pMyUnit->GetUnitCombatCapability()->GetCombatAttritionSignificance01();
		int g=0;
	}
	int g=0;
}
*/
void CEOSAIThoughtDatabase::CompileMyActorsIntoCategories()
{
	// Remove any old information
	m_MyActors.RemoveAll();
	m_MySeaTransports.RemoveAll();
	m_MyAirTransports.RemoveAll();
	m_MyAirUnits.RemoveAll();
	m_MyAircraftCarriers.RemoveAll();
	m_MyCities.RemoveAll();

	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();

	//ASSERT( m_pAIBrain->GetAIPoiObjectList()->IsEmpty() == FALSE );
	ASSERT( g_pEOSAICommonData->GetAIPoiObjects()->IsEmpty() == FALSE );
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		if( pAIPoiObject->GetOwner() == iAIPlayer )
		{
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity )
			{
				m_MyActors.AddTail( pAICity );
				m_MyCities.AddTail( pAICity );
			}
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				m_MyActors.AddTail( pAIUnit );
				if( pAIUnit->GetNumberOfGroundUnitsThatICanContain() > 0 )
				{
					if( pAIUnit->IsSeaUnit() )
					{
						m_MySeaTransports.AddTail( pAIUnit );
					}
					if( pAIUnit->IsAirUnit() )
					{
						m_MyAirTransports.AddTail( pAIUnit );
					}
				}
				if( pAIUnit->GetNumberOfAirUnitsThatICanContain() > 0 )
				{
					m_MyAircraftCarriers.AddTail( pAIUnit );
				}
				if( pAIUnit->IsAirUnit() )
				{
					m_MyAirUnits.AddTail( pAIUnit );
				}
			}
		}
	}

	int z=0;
}

/*
float CEOSAIThoughtDatabase::GetFastestMovementRate( CAIThought_AIPoiObjectFilter* pFilter )
{
	float fFastestMovementRate = 0.0f;

	POSITION pos = m_UnitsICanBuild.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_UnitsICanBuild.GetNext( pos );
		if( pFilter->PassesFilter( pAIUnitTemplate ) )
		{
			float fMovementRate = pAIUnitTemplate->GetUnitTemplate()->GetMovementDescription()->GetHighestMovementRate();
			if( fMovementRate > fFastestMovementRate )
			{
				fFastestMovementRate = fMovementRate;
			}
		}
	}
	return fFastestMovementRate;
}
*/

//void CEOSAIThoughtDatabase::GetTransportsList( CEOSAIUnitTemplate* pTransportee, CList< CEOSAIUnitTemplate* >* pAIUnitTemplateList )
void CEOSAIThoughtDatabase::GetTransportTemplatesICanBuild( CEOSAIUnitTemplate* pTransportee, CList< CEOSAIUnitTemplate* >* pUnitTemplateList )
{
	// Iterate over the transports
	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->CanContainUnits() )
		{
			if( pUnitTemplate->CanContain( pTransportee ) )
			{
				pUnitTemplateList->AddTail( pUnitTemplate );
			}
		}
	}
}
/*
bool CEOSAIThoughtDatabase::IsASubmarineUnitInTheGame( CUnitTemplate* pUnitTemplate )
{
	POSITION pos = m_SubmarineUnitsInTheGame.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pSubmarineUnitTemplate = m_SubmarineUnitsInTheGame.m_List.GetNext( pos );
		if( pSubmarineUnitTemplate == pUnitTemplate )
		{
			return true;
		}
	}
	return false;
}
*/
// Transports
//
/*
void  CEOSAIThoughtDatabase::GetTransportsSet( CUnitTemplate* pInGroundUnitTemplate, CEOSAITransportsSet* pOutTransportsSet )
{
	pOutTransportsSet->Clear();
	pOutTransportsSet->m_pUnitToTransport = pInGroundUnitTemplate;

	// Collect the list of buildable transports
	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pBuildableUnit = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pBuildableUnit->CanContainUnits() && pBuildableUnit->CanContain( pInGroundUnitTemplate ) )
		{
			pOutTransportsSet->m_BuildableTransports.AddTail( pBuildableUnit );
		}
	}

	// Collect the list of cities (which could build a transport)
	
	CList< CEOSAICity* >        m_Cities;
}
*/

// Cities
//

long CEOSAIThoughtDatabase::GetNumberOfCitiesWithImprovement( CString strCityImprovement )
{
	long iCitiesWithImprovement = 0;
	POSITION pos = m_MyCities.GetHeadPosition();
	while( pos )
	{
		CEOSAICity* pAICity = m_MyCities.GetNext( pos );
		if( pAICity )
		{
			//CCity* pCity = (CCity*) pAICity->GetServerPoiObject();
			if( pAICity->GetBuildingCountIncludingPartiallyCompleted( strCityImprovement ) > 0 )
			{
				iCitiesWithImprovement++;
			}
		}
	}
	return iCitiesWithImprovement;
}

// Resources

float CEOSAIThoughtDatabase::GetMyLandFoodResources()
{
	float fAmount = 0.0f;
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while (pos)
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext(pos);
		CEOSAIResource* pRes = dynamic_cast< CEOSAIResource* >(pAIPoiObject);
		if (pRes &&
			pRes->GetOwner() == iAIPlayer &&
			pRes->GetResourceType() == _T("Food") &&
			pRes->ResourceSource_IsOnLand())
		{
			fAmount += pRes->GetResourcePerTurn();
			//pRes->RemoveAllItemsFromBuildQueue();
		}
	}
	return fAmount;
}

float CEOSAIThoughtDatabase::GetMySeaFoodResources()
{
	float fAmount = 0.0f;
	long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while (pos)
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext(pos);
		CEOSAIResource* pRes = dynamic_cast< CEOSAIResource* >(pAIPoiObject);
		if (pRes &&
			pRes->GetOwner() == iAIPlayer &&
			pRes->GetResourceType() == _T("Food") &&
			pRes->ResourceSource_IsOnLand() == false)
		{
			fAmount += pRes->GetResourcePerTurn();
			//pRes->RemoveAllItemsFromBuildQueue();
		}
	}
	return fAmount;
}


// Airbases
//
void  CEOSAIThoughtDatabase::GetAirbases( long iPlayer, CEOSAIAirbasesSet* pAirbasesSet )
{
	ASSERT( pAirbasesSet );
	pAirbasesSet->m_Airbases.RemoveAll();

	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CWorldDescPlayer* pWorldDescPlayer = m_pAIBrain->GetWorldDescPlayer();//GetCommonState()->GetWorldDescServer();

	//CSingleLock  AccessLock( &pWorldDescServer->m_AccessWorldData );
	//AccessLock.Lock();
	//ASSERT( AccessLock.IsLocked() );

	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		if( pPoiObject )
		{
			bool bIsOwnedByPlayer = pPoiObject->GetOwner() == iPlayer;
			bool bIsOwnedByAlly = 
				pPoiObject->GetForeignRelationsTo( iPlayer ).IsAlly() || 
				pPoiObject->GetForeignRelationsTo( iPlayer ).IsTeammate();

			if( bIsOwnedByPlayer || bIsOwnedByAlly )
			{
				if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
					pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Airfield )
				{
					pAirbasesSet->m_Airbases.AddTail( pPoiObject );
				}
				//CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
				//CAirfield* pAirfield = dynamic_cast< CAirfield* >( pPoiObject );
				//if( pCity || pAirfield ){ pAirbasesSet->m_Airbases.AddTail( pPoiObject ); }
			}
			if( bIsOwnedByPlayer && !bIsOwnedByAlly )
			{
				if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
				//CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
				//if( pUnit )
				{
					CEOSAIUnit* pUnit = ( CEOSAIUnit* ) pPoiObject;
					if( pUnit->CanContainUnits() && pUnit->CanLaunchAirUnits() )
					{
						pAirbasesSet->m_Airbases.AddTail( pPoiObject );
					}
				}
			}
		}
	}
}

void  CEOSAIThoughtDatabase::GetAirbases( long iPlayer, CEOSAIUnitTemplate* pAirUnit, CEOSAIAirbasesSet* pAirbasesSet )
{
	ASSERT( pAirUnit );
	ASSERT( pAirUnit->IsAirUnit() );
	ASSERT( pAirbasesSet );
	pAirbasesSet->m_Airbases.RemoveAll();

	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		//if( pPoiObject == NULL   ) continue;
		//if( pPoiObject->IsDead() ) continue;
		if( pPoiObject->IsAlive() == false ) continue;

		if( pPoiObject )
		{
			bool bIsOwnedByPlayer = pPoiObject->GetOwner() == iPlayer;
			bool bIsOwnedByAlly = 
				pPoiObject->GetForeignRelationsTo( iPlayer ).IsAlly() || 
				pPoiObject->GetForeignRelationsTo( iPlayer ).IsTeammate();

			if( bIsOwnedByPlayer || bIsOwnedByAlly )
			{
				if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
					pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Airfield )
				{
					pAirbasesSet->m_Airbases.AddTail( pPoiObject );
				}
				//CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
				//CAirfield* pAirfield = dynamic_cast< CAirfield* >( pPoiObject );
				//if( pCity || pAirfield ){ pAirbasesSet->m_Airbases.AddTail( pPoiObject ); }
			}
			if( bIsOwnedByPlayer && !bIsOwnedByAlly )
			{
				if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
				//CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
				//if( pUnit )
				{
					CEOSAIUnit* pUnit = ( CEOSAIUnit* ) pPoiObject;
					if( pUnit->CanContainUnits() && 
						pUnit->IsLandingPadForAirUnits() &&
						pUnit->IsLaunchingPadForAirUnits() &&
						//pUnit->CanLaunchAirUnits() &&
						pUnit->CanContain_IgnoreForeignRelations( pAirUnit ) )
					{
						pAirbasesSet->m_Airbases.AddTail( pPoiObject );
					}
				}
			}
		}
	}
}

void  CEOSAIThoughtDatabase::GetAirbases( long iPlayer, CEOSAIUnitTemplate* pAirUnit, CEOSAILocation AirUnitLocation, float fLimitToDistance, CEOSAIAirbasesSet* pAirbasesSet )
{
	ASSERT( pAirUnit );
	ASSERT( pAirUnit->IsAirUnit() );
	ASSERT( pAirbasesSet );
	pAirbasesSet->m_Airbases.RemoveAll();

	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		CEOSAIPoiObject* pPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );

		if( pPoiObject )
		{
			//float fDistance = pWorldDescServer->GetPixelDistance( AirUnitLocation, pPoiObject->GetLocation() );
			float fDistance = g_pWorldDistanceTool->GetDistance( AirUnitLocation, pPoiObject->GetLocation() );
			if( fDistance < fLimitToDistance )
			{
				bool bIsOwnedByPlayer = pPoiObject->GetOwner() == iPlayer;
				bool bIsOwnedByAlly = 
					pPoiObject->GetForeignRelationsTo( iPlayer ).IsAlly() || 
					pPoiObject->GetForeignRelationsTo( iPlayer ).IsTeammate();

				if( bIsOwnedByPlayer || bIsOwnedByAlly )
				{
					if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
						pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Airfield )
					{
						pAirbasesSet->m_Airbases.AddTail( pPoiObject );
					}
					//CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
					//CAirfield* pAirfield = dynamic_cast< CAirfield* >( pPoiObject );
					//if( pCity || pAirfield ){ pAirbasesSet->m_Airbases.AddTail( pPoiObject ); }
				}
				if( bIsOwnedByPlayer && !bIsOwnedByAlly )
				{
					if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
					//CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
					//if( pUnit )
					{
						CEOSAIUnit* pUnit = ( CEOSAIUnit* ) pPoiObject;
						if( pUnit->CanContainUnits() && 
							pUnit->CanLaunchAirUnits() &&
							pUnit->CanContain_IgnoreForeignRelations( pAirUnit ) )
						{
							pAirbasesSet->m_Airbases.AddTail( pPoiObject );
						}
					}
				}
			}
		}
	}
}

// Upgrade Locations
//
void CEOSAIThoughtDatabase::GetClosestUpgradeLocation( CEOSAIUnit* pAIUnit, CEOSAIPoiObject** pClosestUpgradeLocation, float* pfDistance )
{
	//CUnit* pUnit = pAIUnit->GetServerUnit();
	ASSERT( pAIUnit );
	ASSERT( pAIUnit->GetNeedsToBeUpgraded() );

	*pClosestUpgradeLocation = NULL;
	*pfDistance = 0.0f;

	CEOSAIRegionPathwayMap* pDirectPathToEverywhere = pAIUnit->InvokeDirectAIRegionMapToEverywhere();

	// Repair?
	float         fBestUpgradeDistance = 1000000.0f;
	CEOSAIPoiObject* pBestUpgradeAIPoiObject = NULL;
	if( pAIUnit->GetNeedsToBeUpgraded() )
	{
		//POSITION pos = GetAIBrain()->GetAIPoiObjectList()->GetHeadPosition();
		POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
		while( pos )
		{
			//CEOSAIPoiObject* pAIContainerPoiObject = GetAIBrain()->GetAIPoiObjectList()->GetNext( pos );
			CEOSAIPoiObject* pAIContainerPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
			//CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			//if( pAICity &&
			//	pAICity->GetOwner() == pAIUnit->GetOwner() )
			if( pAIContainerPoiObject->GetOwner() == pAIUnit->GetOwner() &&
				pAIUnit->CanBeUpgradedHere( pAIContainerPoiObject ) )
			{
				//float fHPRepairPerTurn = pAIContainerPoiObject->GetHPRepairPerTurn( pAIUnit->GetUnitTemplate() );
				//if( fHPRepairPerTurn == 0.0f ) continue;
				/*
				can i repair at this city?
				should i influence it to build a shipyard/barracks?
				are there any buildings that can repair units in this unitset?
				*/
				CEOSAILocation CityLocation = pAIContainerPoiObject->GetInitialState()->GetLocation();
				CEOSAIRegionPathwayItem* pRegionPathwayItem = pDirectPathToEverywhere->GetRegionResult( CityLocation );
				if( pRegionPathwayItem )
				{
					float fDistanceAndDanger = pRegionPathwayItem->m_fDistance + pRegionPathwayItem->m_fDanger;
					if( pRegionPathwayItem->m_fDistance < 10.0f ) fDistanceAndDanger = 0.0f;
					if( fDistanceAndDanger < fBestUpgradeDistance )
					{
						fBestUpgradeDistance = fDistanceAndDanger;
						pBestUpgradeAIPoiObject = pAIContainerPoiObject;

						*pClosestUpgradeLocation = pBestUpgradeAIPoiObject;
						*pfDistance = fBestUpgradeDistance;
					}
				}
			}
		}
	}
}

void CEOSAIThoughtDatabase::GetClosestRepairLocation( 
		CEOSAIUnit* pAIDamagedUnit, CEOSAIPoiObject** pClosestRepairLocation, float* pfDistance )
{
	*pClosestRepairLocation = NULL;
	*pfDistance = 0.0f;

	CEOSAIRegionPathwayMap* pDirectPathToEverywhere = pAIDamagedUnit->InvokeDirectAIRegionMapToEverywhere();
	//CUnit* pDamagedUnit = pAIDamagedUnit->GetServerUnit();

	// Repair?
	float         fBestRepairDistance = 1000000.0f;
	CEOSAIPoiObject* pBestRepairAIPoiObject = NULL;
	ASSERT( pAIDamagedUnit->GetNeedForRepair01() > 0.0f );
	if( pAIDamagedUnit->GetNeedForRepair01() > 0.0f )
	{
		//POSITION pos = GetAIBrain()->GetAIPoiObjectList()->GetHeadPosition();
		POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
		while( pos )
		{
			//CEOSAIPoiObject* pAIContainerPoiObject = GetAIBrain()->GetAIPoiObjectList()->GetNext( pos );
			CEOSAIPoiObject* pAIContainerPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
			//CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			//if( pAICity &&
			//	pAICity->GetOwner() == pAIDamagedUnit->GetOwner() )
			if( pAIContainerPoiObject->GetOwner() == pAIDamagedUnit->GetOwner() &&
				pAIDamagedUnit->CanBeRepairedHere( pAIContainerPoiObject ) )
			{
				float fHPRepairPerTurn = pAIContainerPoiObject->GetHPRepairPerTurn( pAIDamagedUnit->GetAIUnitTemplate() );
				if( fHPRepairPerTurn == 0.0f ) continue;
				/*
				can i repair at this city?
				should i influence it to build a shipyard/barracks?
				are there any buildings that can repair units in this unitset?
				*/
				CEOSAILocation CityLocation = pAIContainerPoiObject->GetInitialState()->GetLocation();
				CEOSAIRegionPathwayItem* pRegionPathwayItem = pDirectPathToEverywhere->GetRegionResult( CityLocation );
				if( pRegionPathwayItem )
				{
					float fDistanceAndDanger = pRegionPathwayItem->m_fDistance + pRegionPathwayItem->m_fDanger;
					if( pRegionPathwayItem->m_fDistance < 10.0f ) fDistanceAndDanger = 0.0f;
					if( fDistanceAndDanger < fBestRepairDistance )
					{
						fBestRepairDistance = fDistanceAndDanger;
						pBestRepairAIPoiObject = pAIContainerPoiObject;

						*pClosestRepairLocation = pBestRepairAIPoiObject;
						*pfDistance = fBestRepairDistance;
					}
				}
			}
		}
	}
}

long CEOSAIThoughtDatabase::GetNumberOfMyUnits()
{
	long iUnits = 0;
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( dynamic_cast< CEOSAIUnit* >( pAIPoiObject ) ){ iUnits++; }
	}
	return iUnits;
}

long CEOSAIThoughtDatabase::GetNumberOfMyCities()
{
	long iCities = 0;
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( dynamic_cast< CEOSAICity* >( pAIPoiObject ) ){ iCities++; }
	}
	return iCities;
}

// Idle Units
//
long  CEOSAIThoughtDatabase::GetNumberOfMyIdleUnitsAndCities()
{
	long iCount=0;
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( pAIPoiObject->GetAllocatedUnitActionIdea() == NULL )
		{
			iCount++;
		}
		/*
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			if( pAICity->HasPredefinedBuildOrders() == false ){ iCount++; }
			continue;
		}
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			if( pAIUnit->HasPredefinedPath() == false ){ iCount++; }
			continue;
		}
		*/
	}
	return iCount;
}

long  CEOSAIThoughtDatabase::GetNumberOfMyIdleCities()
{
	long iCount=0;
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( pAIPoiObject->GetAllocatedUnitActionIdea() == NULL )
		{
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity )
			{
				iCount++;
			}
		}
	}
	return iCount;
}
long  CEOSAIThoughtDatabase::GetNumberOfMyIdleUnits()
{
	long iCount=0;
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( pAIPoiObject->GetAllocatedUnitActionIdea() == NULL )
		{
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				iCount++;
			}
		}
	}
	return iCount;
}

void  CEOSAIThoughtDatabase::GetMyIdleUnitsAndCities( CList< CEOSAIPoiObject* >* pIdleUnitsAndCities )
{
	pIdleUnitsAndCities->RemoveAll();
	POSITION pos = m_MyActors.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_MyActors.GetNext( pos );
		if( pAIPoiObject->GetAllocatedUnitActionIdea() == NULL )
		{
			pIdleUnitsAndCities->AddTail( pAIPoiObject );
		}
		/*
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			if( pAICity->HasPredefinedBuildOrders() == false ){ pIdleUnitsAndCities->AddTail( pAICity ); }
			continue;
		}
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			if( pAIUnit->HasPredefinedPath() == false ){ pIdleUnitsAndCities->AddTail( pAIUnit ); }
			continue;
		}
		*/
	}
}

float CEOSAIThoughtDatabase::GetFastestTransportMovementRate( CList< CEOSAIUnitTemplate* >* pTransporteeList )
{
	float fFastestMovementRate = 0.0f;

	// Iterate over the transports
	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->GetNumberOfAirUnitsThatICanContain() > 0 ||
			pUnitTemplate->GetNumberOfGroundUnitsThatICanContain() > 0 )
		{
			POSITION pos2 = pTransporteeList->GetHeadPosition();
			while( pos2 )
			{
				CEOSAIUnitTemplate* pTransportee = pTransporteeList->GetNext( pos2 );
				if( pUnitTemplate->CanContain( pTransportee ) )
				{
					float fMovementRate = pUnitTemplate->GetMovementRate();
					if( fFastestMovementRate < fMovementRate )
					{
						fFastestMovementRate = fMovementRate;
						break;
					}
				}
			}
		}
	}

	return fFastestMovementRate;
}
/*
float CEOSAIThoughtDatabase::GetFastestTransportMovementRate( CAIThought_AIPoiObjectFilter* pFilter )
{
	float fFastestMovementRate = 0.0f;

	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pFilter->m_bMyCityHunters )
		{
			if( pUnitTemplate->CanContainGroundUnits() )
			{
				float fMovementRate = pUnitTemplate->GetMovementDescription()->GetHighestMovementRate();
				if( fMovementRate > fFastestMovementRate )
				{
					fFastestMovementRate = fMovementRate;
				}
			}
		}
	}
	return fFastestMovementRate;
}
*/
/*
bool  CEOSAIThoughtDatabase::GetTypicalAirMovementDescription( CAIThought_AIPoiObjectFilter* pFilter, CMovementDescription* pMovementDescription )
{
	bool bUnitTemplateFound = false;
	pMovementDescription->SetToZero();

	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->IsAirUnit() )
		{
			if( pFilter->PassesFilter( pUnitTemplate ) )
			{
				bUnitTemplateFound = true;
				pMovementDescription->CopyFastestComponents( pUnitTemplate->GetMovementDescription() );
			}
		}
	}
	return bUnitTemplateFound;
}
*/
bool  CEOSAIThoughtDatabase::GetTypicalGroundUnitMovementDescription( CEOSAIMovementDescription* pMovementDescription )
{
	long iUnitTemplatesFound = 0;
	pMovementDescription->SetToZero();

	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->IsGroundUnit() )
		{
			iUnitTemplatesFound++;
			pMovementDescription->m_fMovementRate += pUnitTemplate->GetMovementDescription()->m_fMovementRate;
			for( long iTerrain=0; iTerrain<TERRAIN_COUNT; iTerrain++ )
			{
				pMovementDescription->m_MovementRateMultiplier.m_fTerrainValue[iTerrain] += 
					pUnitTemplate->GetMovementDescription()->m_MovementRateMultiplier.m_fTerrainValue[ iTerrain ];
			}
		}
	}
	if( iUnitTemplatesFound >= 2 )
	{
		pMovementDescription->m_fMovementRate /= iUnitTemplatesFound;
		for( long iTerrain=0; iTerrain<TERRAIN_COUNT; iTerrain++ )
		{
			pMovementDescription->m_MovementRateMultiplier.m_fTerrainValue[iTerrain] /= iUnitTemplatesFound;
		}
	}
	return iUnitTemplatesFound > 0;
}
/*
bool  CEOSAIThoughtDatabase::GetTypicalGroundMovementDescription( CAIThought_AIPoiObjectFilter* pFilter, CMovementDescription* pMovementDescription )
{
	bool bUnitTemplateFound = false;
	pMovementDescription->SetToZero();

	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->IsGroundUnit() )
		{
			if( pFilter->PassesFilter( pUnitTemplate ) )
			{
				bUnitTemplateFound = true;
				pMovementDescription->CopyFastestComponents( pUnitTemplate->GetMovementDescription() );
			}
		}
	}
	return bUnitTemplateFound;
}
*/
/*
bool  CEOSAIThoughtDatabase::GetTypicalSeaMovementDescription( CAIThought_AIPoiObjectFilter* pFilter, CMovementDescription* pMovementDescription )
{
	bool bUnitTemplateFound = false;
	pMovementDescription->SetToZero();

	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->IsSeaUnit() )
		{
			if( pFilter->PassesFilter( pUnitTemplate ) )
			{
				bUnitTemplateFound = true;
				pMovementDescription->CopyFastestComponents( pUnitTemplate->GetMovementDescription() );
			}
		}
	}
	return bUnitTemplateFound;
}
*/
bool  CEOSAIThoughtDatabase::GetTypicalSeaTransportMovementDescription( CEOSAIMovementDescription* pMovementDescription )
{
	long iUnitTemplatesFound = 0;
	pMovementDescription->SetToZero();

	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->IsSeaUnit() && pUnitTemplate->IsTransport() )
		{
			iUnitTemplatesFound++;
			pMovementDescription->m_fMovementRate += pUnitTemplate->GetMovementDescription()->m_fMovementRate;
			for( long iTerrain=0; iTerrain<TERRAIN_COUNT; iTerrain++ )
			{
				pMovementDescription->m_MovementRateMultiplier.m_fTerrainValue[iTerrain] += 
					pUnitTemplate->GetMovementDescription()->m_MovementRateMultiplier.m_fTerrainValue[ iTerrain ];
			}
		}
	}
	if( iUnitTemplatesFound >= 2 )
	{
		pMovementDescription->m_fMovementRate /= iUnitTemplatesFound;
		for( long iTerrain=0; iTerrain<TERRAIN_COUNT; iTerrain++ )
		{
			pMovementDescription->m_MovementRateMultiplier.m_fTerrainValue[iTerrain] /= iUnitTemplatesFound;
		}
	}
	return iUnitTemplatesFound > 0;
}

/*
float CEOSAIThoughtDatabase::GetLargestRangeOfCurrentAirUnitTemplates()
{
	float fFurthestRange = 0.0f;
	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->IsAirUnit() )
		{
			bool bHasARange = pUnitTemplate->HasARange();
			if( !bHasARange )
			{
				fFurthestRange = 1000000.0f;
			}
			else
			{
				float fRange = pUnitTemplate->GetMovementLimit();
				if( fRange > fFurthestRange )
				{
					fFurthestRange = fRange;
				}
			}
		}
	}
	return fFurthestRange;
}
*/
float CEOSAIThoughtDatabase::GetMovementLimit_AirUnits( CEOSAIUnitTemplateSet* pUnits )
{
	float fFurthestRange = 0.0f;
	POSITION pos = pUnits->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = pUnits->m_List.GetNext( pos );
		if( pUnitTemplate->IsAirUnit() )
		{
			bool bHasARange = pUnitTemplate->HasARange();
			if( !bHasARange )
			{
				fFurthestRange = 1000000.0f;
			}
			else
			{
				fFurthestRange = max( fFurthestRange, pUnitTemplate->GetMovementLimit() );
			}
		}
	}
	return fFurthestRange;
}

/*
float CEOSAIThoughtDatabase::GetMovementRate_FastestCityHunterMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_UnitsICanBuild.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = m_UnitsICanBuild.m_List.GetNext( pos );
		if( pUnitTemplate->IsCityHunter() )
		{
			float fMovementRate = pUnitTemplate->GetMovementDescription()->GetHighestMovementRate();
			if( fMovementRate > fFastestMovementRate )
			{
				fFastestMovementRate = fMovementRate;
			}
		}
	}
	return fFastestMovementRate;
}
*/
float CEOSAIThoughtDatabase::GetMovementRate_FastestAirfieldBuilderOrTransport( CEOSAIUnitTemplateSet* pUnits )
{
	CEOSAIUnitTemplateSet AirfieldBuilders;
	CEOSAIUnitTemplateSet Transports;
	pUnits->OutputSets( CEOSAIUnitTemplateSet::AirfieldBuildersAndTheirTransports, &AirfieldBuilders, &Transports );

	float fFastest = 0.0f;
	POSITION pos;
	pos = AirfieldBuilders.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = AirfieldBuilders.m_List.GetNext( pos );
		fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
	}
	pos = Transports.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = Transports.m_List.GetNext( pos );
		fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
	}
	return fFastest;
}

float CEOSAIThoughtDatabase::GetMovementRate_FastestGroundCitResHunterOrTransport( CEOSAIUnitTemplateSet* pUnits )
{
	CEOSAIUnitTemplateSet CitResHunters;
	CEOSAIUnitTemplateSet Transports;
	pUnits->OutputSets( CEOSAIUnitTemplateSet::GroundCitResHuntersAndTheirTransports, &CitResHunters, &Transports );

	float fFastest = 0.0f;
	POSITION pos;
	pos = CitResHunters.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = CitResHunters.m_List.GetNext( pos );
		fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
	}
	pos = Transports.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = Transports.m_List.GetNext( pos );
		fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
	}
	return fFastest;
}


float CEOSAIThoughtDatabase::GetMovementRate_FastestSeaResHunters( CEOSAIUnitTemplateSet* pUnits )
{
	CEOSAIUnitTemplateSet SeaResHunters;
	pUnits->Add( CEOSAIUnitTemplateSet::SeaResHunters, &SeaResHunters );

	float fFastest = 0.0f;
	POSITION pos;
	pos = SeaResHunters.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = SeaResHunters.m_List.GetNext( pos );
		fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
	}
	return fFastest;
}


float CEOSAIThoughtDatabase::GetMovementRate_FastestCityDegraders( CEOSAIUnitTemplateSet* pUnits )
{
	CEOSAIUnitTemplateSet CityDegraders;
	CEOSAIUnitTemplateSet Transports;
	pUnits->OutputSets( CEOSAIUnitTemplateSet::StrategicBombingAndTheirTransports, &CityDegraders, &Transports );

	float fFastest = 0.0f;
	POSITION pos;
	pos = CityDegraders.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = CityDegraders.m_List.GetNext( pos );
		fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
	}
	pos = Transports.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = Transports.m_List.GetNext( pos );
		fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
	}
	return fFastest;
}

float CEOSAIThoughtDatabase::GetMovementRate_FastestTransport( CEOSAIUnitTemplate* pUnitToTransport, CEOSAIUnitTemplateSet* pUnits )
{
	float fFastest = 0.0f;
	POSITION pos;
	pos = pUnits->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = pUnits->m_List.GetNext( pos );
		if( pUnitTemplate->CanContainUnits() && pUnitTemplate->CanContain( pUnitToTransport ) )
		{
			fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
		}
	}
	return fFastest;
}

float CEOSAIThoughtDatabase::GetMovementRate_FastestSeaTransport( CEOSAIUnitTemplate* pUnitToTransport, CEOSAIUnitTemplateSet* pUnits )
{
	float fFastest = 0.0f;
	POSITION pos;
	pos = pUnits->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplate = pUnits->m_List.GetNext( pos );
		if( pUnitTemplate->IsTransport() &&
			pUnitTemplate->IsSeaUnit() &&
			pUnitTemplate->CanContainUnits() && 
			pUnitTemplate->CanContain( pUnitToTransport ) )
		{
			fFastest = max( fFastest, pUnitTemplate->GetMovementRate() );
		}
	}
	return fFastest;
}

// 
/*
float CEOSAIThoughtDatabase::GetTimeToTarget(
			CEOSAILocation UnitStart,
			CEOSAILocation UnitEnd,
			EnumMovementMotive eMovementMotive,
			CUnitTemplate* pTraveller,
			CMultiRegionPathwayResult& PathwayResult,
			CEOSAIUnitTemplateSet* pAssistUnits )
{
	CWorldDesc* pWorldDesc = m_pAIBrain->GetWorldDesc();

	CMultiRegion* pUnitStartMultiRegion = m_pAIBrain->GetMultiRegionManager()->GetMultiRegion( UnitStart );
	CMultiRegionResult* pUnitPathResult = PathwayResult.GetMultiRegionResult( pUnitStartMultiRegion->m_iId );

	// If this is an air unit, check the air-distance
	float fDistance = pUnitPathResult->m_fDistance;
	float fTravellerMovementRate = pTraveller->GetMovementRate();
	if( fTravellerMovementRate <= 0.0f ){ fTravellerMovementRate = 0.001f; } // to prevent divide by zero
	if( pTraveller->IsAirUnit() )
	{
		fDistance = pWorldDesc->GetPixelDistance( UnitStart, UnitEnd );
		if( fDistance <= 0.0f ) return 0.0f;

		if( eMovementMotive == MovementMotive_Attack )
		{
			if( fDistance < pTraveller->GetMovementRangeForAttack() ){ return fDistance / fTravellerMovementRate; }
			else // This air unit need to land somewhere first
			{
				return 2.0f + fDistance / fTravellerMovementRate;
			}
		}
		if( eMovementMotive == MovementMotive_Recon )
		{
			if( fDistance < pTraveller->GetMovementLimit()/2.0f ){ return fDistance / fTravellerMovementRate; }
			else // This air unit need to land somewhere first
			{
				return 2.0f + fDistance / fTravellerMovementRate;
			}
		}
	}
	else // This is a ground/sea unit
	{
		if( fDistance <= 0.0f ) return 0.0f;
		float fTravellerMovementRate = pTraveller->GetMovementRate();
		float fTimeToTarget = 1000000.0f;
		if( pTraveller->GetMovementRate() > 0.0f ){ fTimeToTarget = fDistance / fTravellerMovementRate; }

		CEOSAIUnitTemplateSet Transports;
		pAssistUnits->GetTransports( pTraveller, &Transports );
		POSITION pos = Transports.m_List.GetHeadPosition();
		while( pos )
		{
			CUnitTemplate* pTransport = Transports.m_List.GetNext( pos );
			if( pTransport->GetMovementRate() > 0.0f &&
				pTransport->GetMovementRate() > fTravellerMovementRate )
			{
				if( pTransport->IsAirUnit() )
				{
					float fDistance2 = pWorldDesc->GetPixelDistance( UnitStart, UnitEnd );
					if( fDistance2 <= 0.0f ) return 0.0f;
					if( fDistance2 < pTransport->GetMovementLimit()/2.0f )
					{
						fTimeToTarget = fDistance2 / pTransport->GetMovementRate();
					}
					else // This air unit need to land somewhere first
					{
						fTimeToTarget = 2.0f + fDistance2 / pTransport->GetMovementRate();
					}
				}
				else
				{
					fTimeToTarget = fDistance / pTransport->GetMovementRate();
				}
			}
		}
		return fTimeToTarget;

		/-*
		// What about if the traveller took a transport there?
		//   INCOMPLETE: if I'm dealing with air-units, I should take MovementLimit into account
		//   Ait-units can be either the Traveller or the Transport
		//   INCOMPLETE: I should take into account the existing cities and airfields (and allies)
		float fFastestTransport = GetMovementRate_FastestTransport( pTraveller, pAssistUnits );
		float fMovementRate = max( pTraveller->GetMovementRate(), fFastestTransport );
		if( fMovementRate <= 0.0f ) return 0.0f;

		return fDistance / fMovementRate;
		*-/
	}
	return 1000000.0f;
}
*/

/*
void CEOSAIThoughtDatabase::CalculateFastestCityHunterMovementRate()
{
	ASSERT( m_UnitsICanBuild.IsEmpty() == FALSE );

	m_fFastestCityHunterMovementRate = 0.0f;

	POSITION pos = m_UnitsICanBuild.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_UnitsICanBuild.GetNext( pos );
		if( pAIUnitTemplate->IsOfType( EOSAIEnumTypeOfUnit_CityHunter ) )
		{
			float fMovementRate = pAIUnitTemplate->GetUnitTemplate()->GetMovementDescription()->GetHighestMovementRate();

			if( fMovementRate > m_fFastestCityHunterMovementRate )
			{
				m_fFastestCityHunterMovementRate = fMovementRate;
			}
		}
	}
}

void CEOSAIThoughtDatabase::CalculateFastestCityHunterTransportMovementRate()
{
	ASSERT( m_UnitsICanBuild.IsEmpty() == FALSE );

	m_fFastestCityHunterTransportMovementRate = 0.0f;

	POSITION pos = m_UnitsICanBuild.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_UnitsICanBuild.GetNext( pos );
		//if( pAIUnitTemplate->IsOfType( EOSAIEnumTypeOfUnit_CityHunter ) )
		if( pAIUnitTemplate->IsOfType( EOSAIEnumTypeOfUnit_CityHunterTransport ) )
		{
			float fMovementRate = pAIUnitTemplate->GetUnitTemplate()->GetMovementDescription()->GetHighestMovementRate();

			if( fMovementRate > m_fFastestCityHunterTransportMovementRate )
			{
				m_fFastestCityHunterTransportMovementRate = fMovementRate;
			}
		}
	}
}
*/

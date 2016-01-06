
#include "stdafx.h"
#include "EOSAITacticalProject2.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAICommonData.h"
//#include "WorldDescServer.h"
#include "EOSAIMultiRegionAttackAccess.h"
#include "EOSAIMultiRegionNationwidePathways.h"
#include "EOSAINationalSummary3.h"
#include "EOSAITacticalProject2Strategy.h"
#include "EOSAIGeo.h"
#include "City.h"
#include "AIPlayer.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAIDesireConnection.h"
#include "EOSAIUnitPathwayResultStep.h"
#include "EOSAIInterface.h"

CEOSAITacticalProject2::CEOSAITacticalProject2( CEOSAIBrain* pAIBrain )
{
	m_pAIBrain = pAIBrain;
	m_bDebug_HasBeenDeleted = false;
	m_Id = 0;
	m_ePrimaryAction = enum_PrimaryActionUndefined;
	m_eCurrentPlan = enum_Undefined;
	m_bIsASecretInvasion = false;
	//m_eTaskForceStrategy = EnumTaskForceStrategy_UnitsASAP;
	//m_eEvaluationLevel = enum_NoCalculations;

	//m_fSimpleInterestInThisProject = 0.0f;
	//m_fEnemyCombatSignificance = 0.0f;

	//m_bNationalPathwayHasBeenCalculated = false;
	m_iTargetGeo = 0;
	m_fTargetGroundSeaAttackAccess01 = 0.0f;
	m_fTargetAirAttackAccess01 = 0.0f;
	m_fDistanceFromMyCitResToPrimaryGoals = 0.0f;
	m_fDistanceFromMyCitResUnitsToPrimaryGoals = 0.0f;
	m_fDistanceFromNeutralPlayerCitResToPrimaryGoals = 0.0f;
	m_fDistanceFromEnemyPlayerCitResToPrimaryGoals = 0.0f;
	m_fResistanceCombatSignificance = 0.0f;
	m_fNationalPathwayTimeToTarget = 0.0f;

	m_pInvasionStrategy = NULL;  // Fight all expected resistance
	m_pAirDropStrategy = NULL;   // Fight local resistance
	m_pAirStrikeStrategy = NULL; // Dodge expected resistance, hit target
	m_pBombardmentStrategy = NULL; // Dodge expected resistance, hit target
	m_pAirSearchAndDestroyStrategy = NULL; // Dodge expected resistance, hit target
	m_pSubmarineStrategy = NULL; // Dodge expected resistance, hit target

	// Stuff I want to get rid of:
	//
	m_fSimpleInterest = 0.0f;
	m_fUrgency01 = 0.0f;
	m_fOrderOfProcessing = 0.0f;
	m_iOrderOfProcessingIndex = 0;

	m_iNumberOfMyCitiesOnTheSameGeo = 0;
	m_bSameGeoAsMyResources = false;
	m_bSameGeoAsMyGroundUnits = false;

	//m_fExpectedResistanceUnitCost = 0.0f;
	//m_fTotalCostOfUnitNeeds = 0.0f;

	m_bInitialEvaluationDone = false;
	//m_fInitialEvaluationScoreIgnoreResistance = 0.0f;
	//m_fInitialEvaluationScore = 0.0f;
	m_fInitialEvaluation_NearbyScore = 0.0f;
	m_fInitialEvaluation_ValueScore = 0.0f;
	m_fInitialEvaluation_BasicScore = 0.0f;
	m_fInitialEvaluation_EasyTargetScore = 0.0f;
	m_fInitialEvaluation_NoResistanceScore = 0.0f;

	m_iInitialEvaluation_NearbyScoreIndex = 0;
	m_iInitialEvaluation_ValueScoreIndex = 0;
	m_iInitialEvaluation_BasicScoreIndex = 0;
	m_iInitialEvaluation_EasyTargetScoreIndex = 0;
	m_iInitialEvaluation_NoResistanceScoreIndex = 0;

	m_bIsInsideTacticalProjectsList = false;
	m_pTaskForceWithPredefinedSteps = NULL;
}

EOSAI::AIPlayer*  CEOSAITacticalProject2::GetAIPlayer()
{
	return m_pAIBrain->GetAIPlayer();
}

long  CEOSAITacticalProject2::GetAIPlayerNumber()
{
	return m_pAIBrain->GetAIPlayer()->GetPlayerNumber();
}

CEOSAILocation  CEOSAITacticalProject2::CalculateCenterLocation()
{
	/*
	if( m_PrimaryGoals.IsEmpty() )
	{
		return m_Location;
	}
	*/
	CEOSAILocation CenterLocation = g_pWorldDistanceTool->GetAverageLocation( &m_Locations );
	return CenterLocation;
/*
	CEOSAILocation CenterLocation;
	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;
	long iCount = 0;
	POSITION pos = m_Locations.GetHeadPosition();
	while( pos )
	{
		//CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		CEOSAILocation Location = m_Locations.GetNext( pos );
		iCount++;
		if( iCount == 1 )
		{
			CenterLocation = Location;
		}
		else
		{
			float fX,fY;
			GetAIBrain()->GetWorldDescServer()->GetPixelDirection( 
				CenterLocation.GetRealX(), CenterLocation.GetRealY(),
				Location.GetRealX(), Location.GetRealY(),
				&fX, &fY );
			fOffsetX += fX;
			fOffsetY += fY;
		}
	}
	if( iCount > 1 )
	{
		fOffsetX /= iCount;
		fOffsetY /= iCount;
	}
	CenterLocation.SetReal( CenterLocation.GetRealX()-fOffsetX, CenterLocation.GetRealY()-fOffsetY );
	GetAIBrain()->GetWorldDescServer()->GetWorldBuildDesc()->TruncateLocation( &CenterLocation );
	return CenterLocation;
*/
}

void CEOSAITacticalProject2::Dump( CString& strData )
{
	CString str;

	str.Format( _T("%d. "), m_Id );
	strData += str;

	// Primary Action
	if( m_ePrimaryAction == enum_PrimaryActionUndefined ){ strData += _T("PrimaryAction: Undefined\n"); }
	if( m_ePrimaryAction == enum_CaptureArea ){ strData += _T("PrimaryAction: CaptureArea\n"); }
	if( m_ePrimaryAction == enum_DegradeCity ){ strData += _T("PrimaryAction: DegradeCity\n"); }
	if( m_ePrimaryAction == enum_SubmarineWarzone ){ strData += _T("PrimaryAction: SubmarineWarzone\n"); }
	if( m_ePrimaryAction == enum_ShipWarzone ){ strData += _T("PrimaryAction: ShipWarzone\n"); }
	//if( m_ePrimaryAction == enum_StrategicBomber ){ strData += _T("PrimaryAction: StrategicBomber\n"); }
	if( m_ePrimaryAction == enum_ProtectArea ){ strData += _T("PrimaryAction: ProtectArea\n"); }
	if( m_ePrimaryAction == enum_DestroyUnits ){ strData += _T("PrimaryAction: DestroyUnits\n"); }

	//bool bShouldBeInAllocateTacticalProjectList = GetShouldBeInAllocateTacticalProjectList( 5 );
	//if( bShouldBeInAllocateTacticalProjectList ){ strData += _T(" * Evaluation List *\n"); }
	if( this->IsInsideTacticalProjectsList() ){ strData += _T(" * Evaluation List *\n"); }

	// Score
	str.Format( _T(" SimpleInterest: %0.3f\n"), m_fSimpleInterest );
	strData += str;
	str.Format( _T(" OrderOfProcessing: (%d) %0.3f\n"), m_iOrderOfProcessingIndex, m_fOrderOfProcessing );
	strData += str;
	//str.Format( _T(" EnemyCombatSignificance: %0.3f\n"), m_fEnemyCombatSignificance );
	//strData += str;
	str.Format( _T(" InitialEvaluationScore: %0.3f/%0.3f/%0.3f/%0.3f/%0.3f\n"), 
		GetInitialEvaluation_NearbyScore(), GetInitialEvaluation_ValueScore(), GetInitialEvaluation_BasicScore(), GetInitialEvaluation_EasyTargetScore(), GetInitialEvaluation_NoResistanceScore() );
	strData += str;
	str.Format( _T(" NationalPathwayTimeToTarget: %0.3f\n"), GetNationalPathwayTimeToTarget() );
	strData += str;
	//str.Format( " BestCaseTaskForceRealScore: %0.3f\n", GetBestCaseTaskForceScore() );
	//strData += str;
	str.Format( _T(" DistanceFromMyCitResToPrimaryGoals: %0.3f (w/GeoSwitch cost)\n"), GetDistanceFromMyCitResToPrimaryGoals() );
	strData += str;
	str.Format( _T(" DistanceFromMyCitResUnitsToPrimaryGoals: %0.3f (w/GeoSwitch cost)\n"), GetDistanceFromMyCitResUnitsToPrimaryGoals() );
	strData += str;
	str.Format( _T(" DistanceFromNeutralPlayerCitResToPrimaryGoals: %0.3f\n"), m_fDistanceFromNeutralPlayerCitResToPrimaryGoals );
	strData += str;
	str.Format( _T(" DistanceFromEnemyPlayerCitResToPrimaryGoals: %0.3f\n"), m_fDistanceFromEnemyPlayerCitResToPrimaryGoals );
	strData += str;
	
	//str.Format( " EnemyUnitCost: %0.3f\n", GetEnemyUnitCost() );
	//strData += str;
	//str.Format( " HostileAgainst: %d\n", m_iIsHostileActionAgainstPlayer );
	//strData += str;
	CEOSAIIntSet HostileAgainst;
	GetActionIsHostileAgainstPlayers( &HostileAgainst );
	str.Format( _T(" HostileAgainstPlayers: %s\n"), HostileAgainst.WriteCommaDelimitedText() );
	strData += str;

	// Sea or Land?
	long iGeo = g_pEOSAICommonData->GetGeoId( m_Locations.GetHead() );
	//long iGeo = m_pAIBrain->GetWorldDescServer()->GetGeoNumber( m_Locations.GetHead() );
	//long iGeo = GetFirstDesire()->GetPoiObjectTarget()->GetGeo();
	if( iGeo == 0 )
	{
		ASSERT( false );
		//CList< CGeo* >* pGeoList = m_pAIBrain->GetWorldDesc()->GetGeoList();
		//CPoiObject* pPoiObject = GetFirstDesire()->GetPoiObjectTarget();
		//pPoiObject->GetGeo();
	}
	CEOSAIGeo* pGeo = g_pEOSAICommonData->GetAIGeo( iGeo );
	if( pGeo->GetGeoType() == CEOSAIGeo::Land ){  strData += _T(" Terrain: Land\n"); }
	if( pGeo->GetGeoType() == CEOSAIGeo::Water ){ strData += _T(" Terrain: Water\n"); }

	// Desires
	strData += _T(" Desires:\n");

	/*
	CList< CEOSAIDesireSpatial* > PrimaryDesires;
	CList< CEOSAIDesireSpatial* > SecondaryDesires;
	POSITION pos = m_Desires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_Desires.GetNext( pos );
		if( pDesire->IsAPrimaryMemberOfTacticalProject( this ) ){ PrimaryDesires.AddTail( pDesire ); }
		else{                                                     SecondaryDesires.AddTail( pDesire ); }
	}
	*/

	for( long i=0; i<4; i++ )
	{
		CList< CEOSAIDesireSpatial* >* pDesires;
		if( i==0 )
		{
			pDesires = &m_PrimaryGoals;

			float fEnemyUnitCost = 0.0f;
			float fAIPower = 0.0f;
			POSITION pos = pDesires->GetHeadPosition();
			while( pos )
			{
				CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );
				CEOSAIPoiObject* pPoiObject = pDesire->GetAIPoiObjectTarget();
				CEOSAIUnit* pAITargetUnit = dynamic_cast< CEOSAIUnit* >( pPoiObject );
				if( pAITargetUnit )
				{
					fEnemyUnitCost += pAITargetUnit->GetProductionCost();
					fAIPower += pAITargetUnit->GetAICombatSignificance();//GetAIPowerValue();
				}
			}
			//strData += "   Primary Goals:\n";
			str.Format( _T("   Primary Goals  (ProdCost: %1.0f, AIPower: %1.0f)\n"), fEnemyUnitCost, fAIPower );
			strData += str;
		}
		if( i==1 )
		{
			pDesires = &m_LocalResistance;

			float fEnemyUnitCost = 0.0f;
			float fAIPower = 0.0f;
			POSITION pos = pDesires->GetHeadPosition();
			while( pos )
			{
				CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );
				CEOSAIPoiObject* pAIPoiObject = pDesire->GetAIPoiObjectTarget();
				CEOSAIUnit* pAITargetUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
				if( pAITargetUnit )
				{
					fEnemyUnitCost += pAITargetUnit->GetProductionCost();
					fAIPower += pAITargetUnit->GetAICombatSignificance();//GetAIPowerValue();
				}
			}
			str.Format( _T("   Local Resistance:  (ProdCost: %1.0f, AIPower: %1.0f)\n"), fEnemyUnitCost, fAIPower );
			strData += str;
		}
		if( i==2 )
		{
			pDesires = &m_PathwayResistance;

			float fEnemyUnitCost = 0.0f;
			float fAIPower = 0.0f;
			POSITION pos = pDesires->GetHeadPosition();
			while( pos )
			{
				CEOSAIDesireSpatial* pDesire = pDesires->GetNext( pos );
				CEOSAIPoiObject* pAIPoiObject = pDesire->GetAIPoiObjectTarget();
				CEOSAIUnit* pAITargetUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
				if( pAITargetUnit )
				{
					fEnemyUnitCost += pAITargetUnit->GetProductionCost();
					fAIPower += pAITargetUnit->GetAICombatSignificance();//GetAIPowerValue();
				}
			}
			//ASSERT( pDesires->IsEmpty() == FALSE );

			str.Format( _T("   Pathway Resistance:  (ProdCost: %1.0f, AIPower: %1.0f)\n"), fEnemyUnitCost, fAIPower );
			strData += str;
		}
		if( i==3 )
		{
			pDesires = &m_OnTheWayOpportunities;
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

			/*
			if( dynamic_cast< CCity* >( pDesire->GetPoiObjectTarget() ) ){ strData += "City | "; }
			if( dynamic_cast< CResourceSource* >( pDesire->GetPoiObjectTarget() ) ){ strData += "Res | "; }
			if( dynamic_cast< CAirfield* >( pDesire->GetPoiObjectTarget() ) ){ strData += "Airfield | "; }
			if( dynamic_cast< CUnit* >( pDesire->GetPoiObjectTarget() ) ){ strData += "Unit | "; }
			*/
			/*
			strData += pDesire->GetPoiObjectTarget()->GetName();
			*/
			if( dynamic_cast< CEOSAIUnit* >( pDesire->GetAIPoiObjectTarget() ) )
			{
				CEOSAIUnit* pAIUnitTarget = dynamic_cast< CEOSAIUnit* >( pDesire->GetAIPoiObjectTarget() );
				strData += pAIUnitTarget->GetAIUnitTemplate()->GetInternalName();
			}
			eif( dynamic_cast< CEOSAIResource* >( pDesire->GetAIPoiObjectTarget() ) )
			{
				CEOSAIResource* pResTarget = dynamic_cast< CEOSAIResource* >( pDesire->GetAIPoiObjectTarget() );
				CString strTemp;
				strTemp.Format( _T("%s x%d"), pResTarget->GetResourceExternalText(), pResTarget->GetResourcePerTurn() );
				strData += strTemp;
				//strData += pResTarget->GetResourceExternalText() ->Get->GetUnitTemplate()->GetInternalName();
			}
			else
			{
				strData += pDesire->GetAIPoiObjectTarget()->GetName();
			}
			str.Format( _T(" (ObjectId %d)\n"), pDesire->GetAIPoiObjectTarget()->GetObjectId() );
			strData += str;

			str.Format( _T("       SimpleInterest: %0.3f\n"), pDesire->GetSimpleInterest() );
			strData += str;
		}
	}
	strData += _T("\n");
}

void CEOSAITacticalProject2::ActionScheduler_SetPrimaryAction( EnumPrimaryAction eType )
{
	m_ePrimaryAction = eType;
}

void CEOSAITacticalProject2::ActionScheduler_AddPrimaryGoal( CEOSAIDesireSpatial* pDesire )
{
	m_Locations.AddTail( pDesire->GetLocation() );
	/*
	if( m_Location.GetRealX() == 0.0f && m_Location.GetRealY() == 0.0f )
	{
		m_Location = pDesire->GetLocation();
	}
	*/

	ASSERT( IsPrimaryGoal( pDesire ) == false );
	ASSERT( IsLocalResistance( pDesire ) == false );
	ASSERT( IsPathwayResistance( pDesire ) == false );
	ASSERT( IsOnTheWayOpportunity( pDesire ) == false );

	pDesire->AddAsPrimaryGoalOfTacticalProject2( this );
	m_PrimaryGoals.AddTail( pDesire );
	m_PrimaryGoals_JobsToDo.Add( pDesire->GetJobsToDo() );
}

void CEOSAITacticalProject2::ActionScheduler_AddAsLocalResistance( CEOSAIDesireSpatial* pDesire )
{
	ASSERT( IsPrimaryGoal( pDesire ) == false );
	ASSERT( IsLocalResistance( pDesire ) == false );
	ASSERT( IsPathwayResistance( pDesire ) == false );
	ASSERT( IsOnTheWayOpportunity( pDesire ) == false );
	m_LocalResistance.AddTail( pDesire );
	m_LocalResistance_JobsToDo.Add( pDesire->GetJobsToDo() );
}

void CEOSAITacticalProject2::ActionScheduler_AddAsPathwayResistance( CEOSAIDesireSpatial* pDesire )
{
	ASSERT( IsPrimaryGoal( pDesire ) == false );
	ASSERT( IsLocalResistance( pDesire ) == false );
	ASSERT( IsPathwayResistance( pDesire ) == false );
	ASSERT( IsOnTheWayOpportunity( pDesire ) == false );
	m_PathwayResistance.AddTail( pDesire );
	m_PathwayResistance_JobsToDo.Add( pDesire->GetJobsToDo() );
}

void CEOSAITacticalProject2::ActionScheduler_AddAsOnTheWayOpportunity( CEOSAIDesireSpatial* pDesire )
{
	ASSERT( IsPrimaryGoal( pDesire ) == false );
	ASSERT( IsLocalResistance( pDesire ) == false );
	ASSERT( IsPathwayResistance( pDesire ) == false );
	ASSERT( IsOnTheWayOpportunity( pDesire ) == false );
	m_OnTheWayOpportunities.AddTail( pDesire );
	m_OnTheWayOpportunities_JobsToDo.Add( pDesire->GetJobsToDo() );
}

void  CEOSAITacticalProject2::ActionScheduler_AddNearbyRelatedDesires( CEOSAIDesireSpatial* pMainDesire )
{
	// If this is a 'capture' desire, then
	//   I want to capture the nearby targets and destroy defending/nearby units
	// If this is a 'degrade' desire (bombing), then
	//   I don't need to capture or degrade nearby targets, and don't need to destroy defending/nearby units
	CEOSAITacticalProject2::EnumPrimaryAction ePrimaryAction = GetPrimaryAction();

	//ASSERT( pTacticalProject->GetDesires()->IsEmpty() == FALSE );
	//CEOSAIDesireSpatial* pPrimaryDesire = pTacticalProject->GetDesires()->GetHead();
	//CList< CEOSAIDesireSpatial* >  RelatedDesires;
	long iMainDesireGeo = pMainDesire->GetAIPoiObjectTarget()->GetGeoId();

	POSITION pos = pMainDesire->GetNearbyDesires()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireConnection* pConnection = pMainDesire->GetNearbyDesires()->GetNext( pos );
		CEOSAIDesireSpatial* pOtherDesire = pConnection->m_pConnectedAISpatialDesire;
		ASSERT( pMainDesire != pOtherDesire );

		ASSERT( pOtherDesire->GetType() == EnumCaptureCity ||
				pOtherDesire->GetType() == EnumCaptureAirfield ||
				pOtherDesire->GetType() == EnumCaptureResource ||
				pOtherDesire->GetType() == EnumDestroyUnit ||
				pOtherDesire->GetType() == EnumDegradeCity ||
				pOtherDesire->GetType() == EnumBuildAirfield ||
				pOtherDesire->GetType() == EnumDestroyAirfield );

		/*
		float fDistance = pConnection->m_fAirDistance;
		fDistance += ( 50.0f * pConnection->m_iMinimumGeoSwitches );
		if( fDistance > 200.0f ) continue;
		*/
		//if( pConnection->m_fAirDistance > 180.0f ) continue;

		if( ePrimaryAction == CEOSAITacticalProject2::enum_CaptureArea )
		{
			if( pOtherDesire->GetType() == EnumCaptureCity ||
				pOtherDesire->GetType() == EnumCaptureResource ||
				pOtherDesire->GetType() == EnumDestroyAirfield )
			{
				if( pOtherDesire->IsAPrimaryGoalOfOneOrMoreTacticalProjects2() == false &&
					pConnection->m_iMinimumGeoSwitches == 0 )
				{
					// Primary Desires need to be on the same Geo
					long iOtherDesireGeo = pOtherDesire->GetAIPoiObjectTarget()->GetGeoId();
					ASSERT( iMainDesireGeo == iOtherDesireGeo );
					ActionScheduler_AddPrimaryGoal( pOtherDesire );
				}
			}
		}
	}
	// Add expected resistance
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	pos = pMainDesire->GetNearbyDesires()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireConnection* pConnection = pMainDesire->GetNearbyDesires()->GetNext( pos );
		CEOSAIDesireSpatial* pOtherDesire = pConnection->m_pConnectedAISpatialDesire;
		ASSERT( pMainDesire != pOtherDesire );

		//if( pOtherDesire->IsAPrimaryGoalOfTacticalProject2( this ) == false &&
		//	pOtherDesire->GetType() == EnumDestroyUnit )
		if( pOtherDesire->GetType() == EnumDestroyUnit )
		{
			// Only include Player0 units if he's inside a primary goal
			if( pOtherDesire->GetAIPoiObjectTarget()->GetOwner() == 0 )
			{
				float fDist = GetClosestPrimaryTargetDistance( pOtherDesire->GetAIPoiObjectTarget()->GetLocation() );
				if( fDist < 20.0f )
				{
					ActionScheduler_AddAsLocalResistance( pOtherDesire );
				}
			}
			// Add PlayerN units if:
			//   - This is a GroundUnit and no geo switches
			//   - This is a SeaUnit or AirUnit
			else
			{
				if( m_Id == 3 &&
					pOtherDesire->GetAIPoiObjectTarget()->GetObjectId() == 117 )
				{
					int h=0;
				}

				if( ( pOtherDesire->GetAIPoiObjectTarget()->IsGroundUnit() && pConnection->m_iMinimumGeoSwitches == 0 ) ||
					  pOtherDesire->GetAIPoiObjectTarget()->IsAirUnit() ||
					  pOtherDesire->GetAIPoiObjectTarget()->IsSeaUnit() )
				{
					float fDist = GetClosestPrimaryTargetDistance( pOtherDesire->GetAIPoiObjectTarget()->GetLocation() );
					if( fDist < 100.0f )
					{
						long iTargetOwner = pOtherDesire->GetAIPoiObjectTarget()->GetOwner();
						//EOSAIEnumForeignRelations eRel = m_pAIBrain->GetWorldDescServer()->GetForeignRelations( 
						EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iTargetOwner, m_pAIBrain->GetAIPlayerNumber() );
						if( eRel.IsEnemy() || PrimaryGoalsAreHostileActionAgainstPlayerN( iTargetOwner ) )
						{
							ActionScheduler_AddAsLocalResistance( pOtherDesire );
						}
					}
				}
			}
		}
	}
	int h34234=0;
}

void CEOSAITacticalProject2::GetActionIsHostileAgainstPlayers( CEOSAIIntSet* pSet )
{
	pSet->Clear();
	POSITION pos;
	pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		pSet->Add( pDesire->GetAIPoiObjectTarget()->GetOwner() );
	}
	pos = m_LocalResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_LocalResistance.GetNext( pos );
		pSet->Add( pDesire->GetAIPoiObjectTarget()->GetOwner() );
	}
	pos = m_PathwayResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PathwayResistance.GetNext( pos );
		pSet->Add( pDesire->GetAIPoiObjectTarget()->GetOwner() );
	}
}

bool  CEOSAITacticalProject2::PrimaryGoalsAreHostileActionAgainstAnotherPlayer()
{
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		if( pDesire->IsAggressiveAgainstPlayer() > 0 ) return true;
	}
	return false;
}

bool  CEOSAITacticalProject2::PrimaryGoalsAreHostileActionAgainstPlayerN( long iOtherPlayer )
{
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		if( pDesire->IsAggressiveAgainstPlayer() == iOtherPlayer ) return true;
	}
	return false;
}

bool  CEOSAITacticalProject2::CurrentForeignRelationsPreventsPersuingThisTacticalProject()
{
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		if( pDesire->CurrentForeignRelationsPreventsPersuingThisDesire() ) return true;
	}
	return false;
}

bool CEOSAITacticalProject2::GetShouldBeInAllocateTacticalProjectList( long iMaximumValueIndex )
{
	long iNearbyScoreIndex = GetInitialEvaluation_NearbyScoreIndex();
	long iValueScoreIndex = GetInitialEvaluation_ValueScoreIndex();
	long iBasicScoreIndex = GetInitialEvaluation_BasicScoreIndex();
	long iEasyTargetScoreIndex = GetInitialEvaluation_EasyTargetScoreIndex();
	long iNoResistanceScoreIndex = GetInitialEvaluation_NoResistanceScoreIndex();
	if( ( iNearbyScoreIndex > 0       && iNearbyScoreIndex <= iMaximumValueIndex ) ||
		( iValueScoreIndex > 0        && iValueScoreIndex <= iMaximumValueIndex ) ||
		( iBasicScoreIndex > 0        && iBasicScoreIndex <= iMaximumValueIndex ) ||
		( iEasyTargetScoreIndex > 0   && iEasyTargetScoreIndex <= iMaximumValueIndex ) ||
		( iNoResistanceScoreIndex > 0 && iNoResistanceScoreIndex <= iMaximumValueIndex ) )
	{
		return true;
	}
	return false;
}

void CEOSAITacticalProject2::GetUnitTargets( CList< CEOSAIDesireSpatial* >* pList )
{
	pList->RemoveAll();
	//
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		if( pDesire->GetType() == EnumDestroyUnit )
		{
			pList->AddTail( pDesire );
		}
	}
	pos = m_LocalResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_LocalResistance.GetNext( pos );
		if( pDesire->GetType() == EnumDestroyUnit )
		{
			pList->AddTail( pDesire );
		}
	}
	pos = m_PathwayResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PathwayResistance.GetNext( pos );
		if( pDesire->GetType() == EnumDestroyUnit )
		{
			pList->AddTail( pDesire );
		}
	}
}

void CEOSAITacticalProject2::GetDesires( CList< CEOSAIDesireSpatial* >* pList )
{
	pList->RemoveAll();
	//
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		pList->AddTail( pDesire );
	}
	pos = m_LocalResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_LocalResistance.GetNext( pos );
		pList->AddTail( pDesire );
	}
	pos = m_PathwayResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PathwayResistance.GetNext( pos );
		pList->AddTail( pDesire );
	}
}

bool  CEOSAITacticalProject2::IsPrimaryGoal( CEOSAIDesireSpatial* pDesire )
{
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesireInList = m_PrimaryGoals.GetNext( pos );
		if( pDesire == pDesireInList ) return true;
	}
	return false;
}

bool  CEOSAITacticalProject2::IsLocalResistance( CEOSAIDesireSpatial* pDesire )
{
	POSITION pos = m_LocalResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesireInList = m_LocalResistance.GetNext( pos );
		if( pDesire == pDesireInList ) return true;
	}
	return false;
}

bool  CEOSAITacticalProject2::IsPathwayResistance( CEOSAIDesireSpatial* pDesire )
{
	POSITION pos = m_PathwayResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesireInList = m_PathwayResistance.GetNext( pos );
		if( pDesire == pDesireInList ) return true;
	}
	return false;
}

bool  CEOSAITacticalProject2::IsOnTheWayOpportunity( CEOSAIDesireSpatial* pDesire )
{
	POSITION pos = m_OnTheWayOpportunities.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesireInList = m_OnTheWayOpportunities.GetNext( pos );
		if( pDesire == pDesireInList ) return true;
	}
	return false;
}

float CEOSAITacticalProject2::GetClosestPrimaryGoal( CEOSAILocation Location, CEOSAIDesireSpatial** pOutClosestDesire )
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	float fMinDistance = 1000000.0f;
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pPrimaryGoal = m_PrimaryGoals.GetNext( pos );
		//float fDist = pWorldBuildDesc->GetPixelDistance( pPrimaryGoal->GetPoiObjectTarget()->GetLocation(), Location );
		float fDist = g_pWorldDistanceTool->GetDistance( pPrimaryGoal->GetAIPoiObjectTarget()->GetLocation(), Location );
		if( fDist < fMinDistance )
		{
			fMinDistance = fDist;
			if( pOutClosestDesire ){ *pOutClosestDesire = pPrimaryGoal; }
		}
		//fMinDistance = min( fMinDistance, fDist );
	}
	return fMinDistance;
}

CEOSAILocation CEOSAITacticalProject2::GetClosestPrimaryTargetLocation( CEOSAILocation Location )
{
	float fClosestPrimaryLocation = 1000000.0f;
	CEOSAILocation ClosestPrimaryGoalLocation;

	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	POSITION pos = m_Locations.GetHeadPosition();
	while( pos )
	{
		//CEOSAIDesireSpatial* pPrimaryGoal = m_PrimaryGoals.GetNext( pos );
		CEOSAILocation LocationInList = m_Locations.GetNext( pos );
		//float fDistance = pWorldBuildDesc->GetPixelDistance( LocationInList, Location );
		float fDistance = g_pWorldDistanceTool->GetDistance( LocationInList, Location );
		if( fDistance < fClosestPrimaryLocation )
		{
			fClosestPrimaryLocation = fDistance;
			ClosestPrimaryGoalLocation = LocationInList;
			//if( pOutClosestDesire ){ *pOutClosestDesire = pPrimaryGoal; }
		}
	}
	return ClosestPrimaryGoalLocation;
}

float CEOSAITacticalProject2::GetClosestPrimaryTargetDistance( CEOSAILocation Location )
{
	float fClosestPrimaryLocation = 1000000.0f;
	CEOSAILocation ClosestPrimaryGoalLocation;

	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	POSITION pos = m_Locations.GetHeadPosition();
	while( pos )
	{
		//CEOSAIDesireSpatial* pPrimaryGoal = m_PrimaryGoals.GetNext( pos );
		CEOSAILocation LocationInList = m_Locations.GetNext( pos );
		float fDistance = g_pWorldDistanceTool->GetDistance( LocationInList, Location );
		if( fDistance < fClosestPrimaryLocation )
		{
			fClosestPrimaryLocation = fDistance;
			ClosestPrimaryGoalLocation = LocationInList;
			//if( pOutClosestDesire ){ *pOutClosestDesire = pPrimaryGoal; }
		}
	}
	return fClosestPrimaryLocation;
}

void  CEOSAITacticalProject2::CalculateNationalPathway()
{
	//m_bNationalPathwayHasBeenCalculated = true;
	//float fGroundSeaAttackAccess01 = 0.0f;
	//float fAirAttackAccess01 = 0.0f;

	// Calculate SimpleInterest ( this isn't very useful - it doesn't include Resistance, distance, etc )
	//long iDesireGeo = 0;
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CEOSAIMultiRegionNationwidePathways* pNationwidePathways = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );
	//m_bSameGeoAsMyCities = false;
	m_iNumberOfMyCitiesOnTheSameGeo = 0;
	m_bSameGeoAsMyResources = false;
	m_bSameGeoAsMyGroundUnits = false;

	//m_fSimpleInterest = 0.0f;
	//m_fEnemyCombatSignificance = 0.0f;
	//m_fUrgency01 = 0.0f;

	//
	//CEOSAIMultiRegionNationwidePathways* pMyNationalPathway = pWorldDescServer->GetAICommonData()->GetNationwidePathways( iAIPlayer );
	CEOSAIMultiRegionNationwidePathways* pMyNationalPathway = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );
	m_fDistanceFromMyCitResToPrimaryGoals = 1000000.0f;
	m_fDistanceFromMyCitResUnitsToPrimaryGoals = 1000000.0f;
	CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( m_Locations.GetHead() );
	ASSERT( pMultiRegion );
	if( pMultiRegion )
	{
		long iMultiRegion = pMultiRegion->GetId();//g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( m_Location );
		m_iTargetGeo = pMultiRegion->GetGeoId();

		CEOSAIMultiRegionAttackAccess* pAttackAccess = pNationwidePathways->GetMultiRegionAttackAccess( pMultiRegion->GetId() );
		if( pAttackAccess )
		{
			m_fTargetGroundSeaAttackAccess01 = max( m_fTargetGroundSeaAttackAccess01, pAttackAccess->GetGroundSeaUnitAccess01() );
			m_fTargetAirAttackAccess01 = max( m_fTargetAirAttackAccess01, pAttackAccess->GetAirUnitAccess01() );
		}

		//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
		POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
		while( pos )
		{
			//CPoi* pPoi = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos )->GetPtr();
			//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
			CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );

			if( pAIPoiObject &&
				pAIPoiObject->GetOwner() == iAIPlayer &&
				pAIPoiObject->GetGeoId() == m_iTargetGeo &&
			  ( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
				pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Resource ||
				pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit ) )
			{
				CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
				CEOSAIResource* pAIRes = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
				CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );

				if( pAICity )
				{
					m_iNumberOfMyCitiesOnTheSameGeo++;
				}
				if( pAIRes )
				{
					m_bSameGeoAsMyResources = true;
				}
				if( pAIUnit && pAIUnit->IsGroundUnit() )
				{
					m_bSameGeoAsMyGroundUnits = true;
				}
			}
		}
		CEOSAIRegionPathwayItem* pRegionPathwayItem1 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitRes( iMultiRegion );
		CEOSAIRegionPathwayItem* pRegionPathwayItem2 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitResUnits( iMultiRegion );
		m_fDistanceFromMyCitResToPrimaryGoals = min( m_fDistanceFromMyCitResToPrimaryGoals, pRegionPathwayItem1->m_fDistance );
		m_fDistanceFromMyCitResUnitsToPrimaryGoals = min( m_fDistanceFromMyCitResUnitsToPrimaryGoals, pRegionPathwayItem2->m_fDistance );
	}

	//
	// National pathway+distance, value, danger, etc
	//
	m_fDistanceFromNeutralPlayerCitResToPrimaryGoals = 1000000.0f;
	m_fDistanceFromEnemyPlayerCitResToPrimaryGoals = 1000000.0f;
	pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( m_Locations.GetHead() );
	ASSERT( pMultiRegion );
	if( pMultiRegion )
	{
		//CEOSAIDesireSpatial* pPrimaryGoal = m_PrimaryGoals.GetNext( pos );
		//long iMultiRegion = pPrimaryGoal->GetMultiRegion()->GetId();
		long iMultiRegion = pMultiRegion->GetId();//g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( m_Location );

		//
		//long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		for( long iPlayer=1; iPlayer<iNumberOfPlayers; iPlayer++ )
		{
			//EOSAIEnumForeignRelations eRel = pWorldDescServer->GetForeignRelations( iAIPlayer,iPlayer );
			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iAIPlayer,iPlayer );
			if( eRel.IsEnemy() || eRel.IsNeutral() )
			{
				//CEOSAIMultiRegionNationwidePathways* pNationalPathway = pWorldDescServer->GetAICommonData()->GetNationwidePathways( iPlayer );
				CEOSAIMultiRegionNationwidePathways* pNationalPathway = g_pEOSAICommonData->GetNationwidePathways( iPlayer );
				// I don't always calculate NationalPathways for all players.
				// If the player has been eliminated, or has no poi, there's not NationalPathway to calculate.
				if( pNationalPathway->GetPolitePathFromMyCitResUnitsToRestOfWorld()->GetRegionManager() == NULL ) continue;

				CEOSAIRegionPathwayItem* pCitResPathwayItem = pNationalPathway->GetDistGeoPathfindingValueFromMyCitRes( iMultiRegion );
				CEOSAIRegionPathwayItem* pCitResUnitPathwayItem = pNationalPathway->GetDistGeoPathfindingValueFromMyCitResUnits( iMultiRegion );
				float fAverage = ( pCitResPathwayItem->m_fDistance + pCitResUnitPathwayItem->m_fDistance ) / 2.0f;

				// Just in case I have no CitRes, I want to limit the "average" value
				fAverage = min( fAverage, 2.0f*pCitResPathwayItem->m_fDistance );
				fAverage = min( fAverage, 2.0f*pCitResUnitPathwayItem->m_fDistance );

				// Add-in some more cost for geo-switches
				fAverage += 100.0f * pCitResUnitPathwayItem->m_iGeoSwitches;

				if( eRel.IsNeutral() )
				{
					m_fDistanceFromNeutralPlayerCitResToPrimaryGoals = min( m_fDistanceFromNeutralPlayerCitResToPrimaryGoals, fAverage );
				}
				if( eRel.IsEnemy() )
				{
					m_fDistanceFromEnemyPlayerCitResToPrimaryGoals = min( m_fDistanceFromEnemyPlayerCitResToPrimaryGoals, fAverage );
				}
			}
		}
		//CEOSAIRegionPathwayItem* pRegionPathwayItem1 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitRes( iMultiRegion );
		//CEOSAIRegionPathwayItem* pRegionPathwayItem2 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitResUnits( iMultiRegion );
		//m_fDistanceFromMyCitResToPrimaryGoals = min( m_fDistanceFromMyCitResToPrimaryGoals, pRegionPathwayItem1->m_fDistance );
		//m_fDistanceFromMyCitResUnitsToPrimaryGoals = min( m_fDistanceFromMyCitResUnitsToPrimaryGoals, pRegionPathwayItem2->m_fDistance );
	}

	// How quickly can I get units there (assume NationalPathway)
	CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
	bool bIsWithinAirRange = pUnitsICanBuildOrHave->GetLargestAirUnitMovementRange() > m_fDistanceFromMyCitResToPrimaryGoals;
	float fSlowestMovementRateOfAllNecessaryUnits = max( pUnitsICanBuildOrHave->GetFastestGroundUnitMovementRate(), pUnitsICanBuildOrHave->GetFastestSeaUnitMovementRate() );
	if( bIsWithinAirRange )
	{
		fSlowestMovementRateOfAllNecessaryUnits = max( fSlowestMovementRateOfAllNecessaryUnits, pUnitsICanBuildOrHave->GetFastestAirUnitMovementRate() );
	}
	if( m_PrimaryGoals_JobsToDo.m_iAirfieldBuilder > 0.0f )
	{
		fSlowestMovementRateOfAllNecessaryUnits = min( fSlowestMovementRateOfAllNecessaryUnits, pUnitsICanBuildOrHave->GetFastestAirfieldBuilderAndTransportMovementRate() );
	}
	//if( m_PrimaryGoals_JobsToDo.m_iDegradeCity ){} // Already covered by the general movement rate earlier
	if( m_PrimaryGoals_JobsToDo.m_fGroundCitResHunter > 0.0f )
	{
		fSlowestMovementRateOfAllNecessaryUnits = min( fSlowestMovementRateOfAllNecessaryUnits, pUnitsICanBuildOrHave->GetFastestGroundCitResHunterAndTransportMovementRate() );
	}
	if( m_PrimaryGoals_JobsToDo.m_fSeaResHunter > 0.0f )
	{
		fSlowestMovementRateOfAllNecessaryUnits = min( fSlowestMovementRateOfAllNecessaryUnits, pUnitsICanBuildOrHave->GetFastestSeaResHunterMovementRate() );
	}
	if( fSlowestMovementRateOfAllNecessaryUnits > 10.0f )
	{
		fSlowestMovementRateOfAllNecessaryUnits = (float) ( 0.8 * fSlowestMovementRateOfAllNecessaryUnits + 0.2 * 50.0f );
	}

	m_fNationalPathwayTimeToTarget = 
		0.3f * ( m_fDistanceFromMyCitResToPrimaryGoals      / (fSlowestMovementRateOfAllNecessaryUnits+0.1f) ) +
		0.7f * ( m_fDistanceFromMyCitResUnitsToPrimaryGoals / (fSlowestMovementRateOfAllNecessaryUnits+0.1f) );
}

void  CEOSAITacticalProject2::CalculateInitialEvaluation()
{
	ASSERT( m_bInitialEvaluationDone == false );
	ASSERT( m_fInitialEvaluation_NearbyScore == 0.0f );
	ASSERT( m_fInitialEvaluation_BasicScore == 0.0f );
	ASSERT( m_fInitialEvaluation_ValueScore == 0.0f );
	ASSERT( m_fInitialEvaluation_EasyTargetScore == 0.0f );
	ASSERT( m_fInitialEvaluation_NoResistanceScore == 0.0f );
	ASSERT( m_fResistanceCombatSignificance == 0.0f );

	m_bInitialEvaluationDone = true;
	// There are no desires.  This shouldn't happen, but in case it does, exit-out
	if( m_Locations.IsEmpty() ){ ASSERT( false ); return; }

	// Add-up the simple-interest
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		m_fSimpleInterest += pDesire->GetSimpleInterest();
	}
	pos = m_LocalResistance.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_LocalResistance.GetNext( pos );
		m_fSimpleInterest += pDesire->GetSimpleInterest();
	}
	ASSERT( m_fSimpleInterest >= 0.0f );

	// Calculate the national pathway
	ASSERT( m_Locations.IsEmpty() == FALSE );
	CalculateNationalPathway(); // Find pathway and distance from CitResUnits (mine, neutrals, enemies)

	// Calculate whether this is a secret invasion
	//CEOSAIGeo* pAIGeo = m_pAIBrain->GetAIGeo( m_iTargetGeo );
	CEOSAIGeo* pAIGeo = g_pEOSAIInterface->GetAICommonData()->GetAIGeo( m_iTargetGeo );
	if( pAIGeo &&
		pAIGeo->GetGeoType() == CEOSAIGeo::Land )
	{
		EOSAIEnumGeoPlan eGeoPlan = pAIGeo->GetView( GetAIPlayerNumber() )->GetPlan();
		if( eGeoPlan == EOSAIEnumGeoPlan_Land_InvadeGeo ||
			eGeoPlan == EOSAIEnumGeoPlan_Land_InvadeSection )
		{
			IsASecretInvasion( true );
		}
	}

	// Calculate danger/urgency
	m_fUrgency01 = 0.0f;
	if( m_ePrimaryAction == enum_DestroyUnits )
	{
		CEOSAIMathFunction DistanceFromMyBorders;
		DistanceFromMyBorders.SetInputOutput(   0.0f, 2.0f );
		DistanceFromMyBorders.SetInputOutput( 200.0f, 2.0f );
		DistanceFromMyBorders.SetInputOutput( 400.0f, 1.0f );
		float fSimpleInterestMult = DistanceFromMyBorders.GetOutput( GetDistanceFromMyCitResToPrimaryGoals() );
		m_fSimpleInterest *= fSimpleInterestMult;

		float fTransportUrgency = 0.0f;
		POSITION pos = m_PrimaryGoals.GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
			if( pDesire->GetAIPoiObjectTarget() )
			{
				CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pDesire->GetAIPoiObjectTarget() );
				if( pAIUnit && pAIUnit->GetAIUnitTemplate()->IsTransport() )
				{
					fTransportUrgency = 0.2f;
				}
			}
		}

		CEOSAIMathFunction DistanceFromMyBorders2;
		DistanceFromMyBorders2.SetInputOutput(   0.0f, 0.8f + fTransportUrgency );
		DistanceFromMyBorders2.SetInputOutput( 150.0f, 0.6f + fTransportUrgency );
		DistanceFromMyBorders2.SetInputOutput( 300.0f, 0.0f );

		m_fUrgency01 = DistanceFromMyBorders2.GetOutput( GetDistanceFromMyCitResToPrimaryGoals() );
	}

	//CalculatePathwayDistances_UsingNationalPathway();
	CalculateExpectedPathwayResistance_UsingNationalPathway();

	// Resistance Combat Significance
	ASSERT( m_fResistanceCombatSignificance == 0.0f );
	pos = m_PrimaryGoals_JobsToDo.m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pUnit = m_PrimaryGoals_JobsToDo.m_EnemyUnits.GetNext( pos );
		m_fResistanceCombatSignificance += pUnit->GetAICombatSignificance();
	}
	pos = m_LocalResistance_JobsToDo.m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pUnit = m_LocalResistance_JobsToDo.m_EnemyUnits.GetNext( pos );
		m_fResistanceCombatSignificance += pUnit->GetAICombatSignificance();
	}
	pos = m_PathwayResistance_JobsToDo.m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pUnit = m_PathwayResistance_JobsToDo.m_EnemyUnits.GetNext( pos );
		m_fResistanceCombatSignificance += pUnit->GetAICombatSignificance();
	}

	//CAICommonData* pAICommonData = GetCommonState()->GetWorldDescServer()->GetAICommonData();
	CEOSAINationalSummary3* pNationalSummary = g_pEOSAICommonData->GetAINationalSummary3( GetAIPlayer()->GetPlayerNumber() );
	float fMyNationalCombatSignificance = pNationalSummary->m_UnitSummary.GetCombatSignificance();
	fMyNationalCombatSignificance *= 0.5f;

	float fLandWaterGeoMultiplier = 1.0f;
	float fSameGeoMultiplier = 1.0f;
	bool bIsLandGeo = false;
	pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		CEOSAIGeo* pGeo = pDesire->GetMultiRegion()->GetGeo();
		if( pGeo->GetGeoType() == CEOSAIGeo::Land ){ bIsLandGeo = true; }
	}
	if( bIsLandGeo )
	{
		if( m_iNumberOfMyCitiesOnTheSameGeo > 0 || m_bSameGeoAsMyGroundUnits ) fSameGeoMultiplier += 0.2f;
		if( m_iNumberOfMyCitiesOnTheSameGeo > 0 ) fSameGeoMultiplier += 0.2f;
		if( m_bSameGeoAsMyResources ) fSameGeoMultiplier += 0.1f;
		if( m_bSameGeoAsMyGroundUnits ) fSameGeoMultiplier += 0.1f;
		fSameGeoMultiplier += m_iNumberOfMyCitiesOnTheSameGeo * 0.1f;
	}
	else
	{
		fLandWaterGeoMultiplier = 0.6f; // Sea-resources have lower value
	}

	CEOSAIMathFunction TimeToTargetFunc;
	TimeToTargetFunc.SetInputOutput(  0.0f, 3.0f );//2.0  );
	TimeToTargetFunc.SetInputOutput(  1.0f, 2.0f );//1.0  );
	TimeToTargetFunc.SetInputOutput(  2.0f, 1.5f );//0.66 );
	TimeToTargetFunc.SetInputOutput(  3.0f, 1.0f );//0.5  );
	TimeToTargetFunc.SetInputOutput(  4.0f, 0.8f );//0.4  );
	TimeToTargetFunc.SetInputOutput(  5.0f, 0.7f );//0.33 );
	TimeToTargetFunc.SetInputOutput(  6.0f, 0.6f );//0.28 );
	TimeToTargetFunc.SetInputOutput(  7.0f, 0.5f );//0.25 );
	TimeToTargetFunc.SetInputOutput(  8.0f, 0.4f );//0.22 );
	TimeToTargetFunc.SetInputOutput(  9.0f, 0.3f );//0.2  );
	TimeToTargetFunc.SetInputOutput( 10.0f, 0.2f );//0.18 );
	TimeToTargetFunc.SetInputOutput( 20.0f, 0.1f );//0.1 );
	TimeToTargetFunc.SetInputOutput( 30.0f, 0.05f );//0.06

	float fGeoValue = 1.0f;
	//CEOSAIGeo* pAIGeo = m_pAIBrain->GetAIGeo( m_iTargetGeo );
	ASSERT( pAIGeo );
	if( pAIGeo )
	{
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Land )
		{
			float fSubjectiveCitResValue = pAIGeo->GetView( GetAIPlayerNumber() )->GetSubjectiveCitResValue();
			fGeoValue = pAIGeo->GetView( GetAIPlayerNumber() )->GetSubjectiveCitResValue();
		}
		eif( pAIGeo->GetGeoType() == CEOSAIGeo::Water )
		{
			float fSubjectiveCitResValue = pAIGeo->GetView( GetAIPlayerNumber() )->GetSubjectiveCitResValue();
			fGeoValue = min( 4.0f, pAIGeo->GetView( GetAIPlayerNumber() )->GetSubjectiveCitResValue() );
		}
		else
		{
			ASSERT( false );
		}
	}

	/*
	m_fInitialEvaluationScoreIgnoreResistance =
		(2.0f*fGeoValue) *
		SimpleInterestFunc.GetOutput( m_fSimpleInterest ) * //m_fSimpleInterest *
		TimeToTargetFunc.GetOutput( m_fNationalPathwayTimeToTarget ) * //( 2.0f / (fTimeToTarget+1.0f)) *
		fLandWaterGeoMultiplier *
		fSameGeoMultiplier *
		max( 0.1f, max( m_fTargetGroundSeaAttackAccess01, m_fTargetAirAttackAccess01 ) );
	*/

	/*
	m_fInitialEvaluationScore = 
		(2.0f*fGeoValue) *
		SimpleInterestFunc.GetOutput( m_fSimpleInterest ) * //m_fSimpleInterest *
		TimeToTargetFunc.GetOutput( m_fNationalPathwayTimeToTarget ) * //( 2.0f / (fTimeToTarget+1.0f)) *
		fLandWaterGeoMultiplier *
		fSameGeoMultiplier *
		max( 0.1f, max( m_fTargetGroundSeaAttackAccess01, m_fTargetAirAttackAccess01 ) );// *
		//CombatResistanceFunc.GetOutput( m_fResistanceCombatSignificance / (0.1f+fMyNationalCombatSignificance) );
	*/

	float fNationalResistanceCombatRatio = m_fResistanceCombatSignificance / (0.1f+fMyNationalCombatSignificance);

	{
		CEOSAIMathFunction SimpleInterestFunc;
		SimpleInterestFunc.SetInputOutput(    0.0f,    0.0        );
		SimpleInterestFunc.SetInputOutput(    5.0f,    5.0 * 1.00 );
		SimpleInterestFunc.SetInputOutput(   10.0f,   10.0 * 1.00 );
		SimpleInterestFunc.SetInputOutput(   20.0f,   20.0 * 0.80 );
		SimpleInterestFunc.SetInputOutput(   40.0f,   40.0 * 0.60 );
		SimpleInterestFunc.SetInputOutput(   80.0f,   80.0 * 0.40 );
		SimpleInterestFunc.SetInputOutput(  160.0f,  160.0 * 0.30 );
		SimpleInterestFunc.SetInputOutput(  320.0f,  320.0 * 0.20 );
		SimpleInterestFunc.SetInputOutput(  640.0f,  640.0 * 0.10 );

		CEOSAIMathFunction ResistanceToValueFunc;
		ResistanceToValueFunc.SetInputOutput(    0.0f, 1.0f );
		ResistanceToValueFunc.SetInputOutput(   50.0f, 0.7f );
		ResistanceToValueFunc.SetInputOutput(  100.0f, 0.5f );
		ResistanceToValueFunc.SetInputOutput(  200.0f, 0.25f );
		ResistanceToValueFunc.SetInputOutput(  400.0f, 0.0f );

		CEOSAIMathFunction TimeToTargetFunc2;
		TimeToTargetFunc2.SetInputOutput(  0.0f, 3.0 );//2.0  );
		TimeToTargetFunc2.SetInputOutput(  1.0f, 2.5 );//1.0  );
		TimeToTargetFunc2.SetInputOutput(  2.0f, 2.0 );//0.66 );
		TimeToTargetFunc2.SetInputOutput(  3.0f, 1.5 );//0.5  );
		TimeToTargetFunc2.SetInputOutput(  4.0f, 0.0 );//0.4  );

		// Score targets based on SimpleInterest, Access.  Reduce score if there's a lot of resistance.
		//   This has the problem that small, valuable targets with no resistance get low scores.
		m_fInitialEvaluation_NearbyScore =
			(2.0f*fGeoValue) *
			SimpleInterestFunc.GetOutput( m_fSimpleInterest ) * //m_fSimpleInterest *
			TimeToTargetFunc2.GetOutput( m_fNationalPathwayTimeToTarget ) * //( 2.0f / (fTimeToTarget+1.0f)) *
			fLandWaterGeoMultiplier *
			fSameGeoMultiplier *
			max( 0.1f, max( m_fTargetGroundSeaAttackAccess01, m_fTargetAirAttackAccess01 ) ) *
			ResistanceToValueFunc.GetOutput( m_fResistanceCombatSignificance);
	}

	{
		CEOSAIMathFunction SimpleInterestFunc;
		SimpleInterestFunc.SetInputOutput(    0.0f,    0.0        );
		SimpleInterestFunc.SetInputOutput(   10.0f,   10.0 * 1.00 );
		SimpleInterestFunc.SetInputOutput(   20.0f,   20.0 * 0.95 );
		SimpleInterestFunc.SetInputOutput(   40.0f,   40.0 * 0.90 );
		SimpleInterestFunc.SetInputOutput(   80.0f,   80.0 * 0.85 );
		SimpleInterestFunc.SetInputOutput(  160.0f,  160.0 * 0.80 );
		SimpleInterestFunc.SetInputOutput(  320.0f,  320.0 * 0.70 );
		SimpleInterestFunc.SetInputOutput(  640.0f,  640.0 * 0.60 );
		SimpleInterestFunc.SetInputOutput( 1280.0f, 1280.0 * 0.40 );

		CEOSAIMathFunction ResistanceToValueFunc;
		ResistanceToValueFunc.SetInputOutput(    0.0f, 1.0f );
		ResistanceToValueFunc.SetInputOutput(   50.0f, 0.6f );
		ResistanceToValueFunc.SetInputOutput(  100.0f, 0.3f );
		ResistanceToValueFunc.SetInputOutput(  150.0f, 0.2f );
		ResistanceToValueFunc.SetInputOutput(  200.0f, 0.15f );
		ResistanceToValueFunc.SetInputOutput(  400.0f, 0.075f );
		ResistanceToValueFunc.SetInputOutput(  800.0f, 0.037f );
		ResistanceToValueFunc.SetInputOutput( 1600.0f, 0.020f );
		ResistanceToValueFunc.SetInputOutput( 3200.0f, 0.010f );

		// Score targets based on SimpleInterest, Access.  Reduce score if there's a lot of resistance.
		//   This has the problem that small, valuable targets with no resistance get low scores.
		m_fInitialEvaluation_ValueScore =
			(2.0f*fGeoValue) *
			SimpleInterestFunc.GetOutput( m_fSimpleInterest ) * //m_fSimpleInterest *
			TimeToTargetFunc.GetOutput( m_fNationalPathwayTimeToTarget ) * //( 2.0f / (fTimeToTarget+1.0f)) *
			fLandWaterGeoMultiplier *
			fSameGeoMultiplier *
			max( 0.1f, max( m_fTargetGroundSeaAttackAccess01, m_fTargetAirAttackAccess01 ) ) *
			ResistanceToValueFunc.GetOutput( m_fResistanceCombatSignificance);
	}

	{
		CEOSAIMathFunction SimpleInterestFunc;
		SimpleInterestFunc.SetInputOutput(    0.0f,    0.0        );
		SimpleInterestFunc.SetInputOutput(   10.0f,   10.0 * 1.00 );
		SimpleInterestFunc.SetInputOutput(   20.0f,   20.0 * 0.95 );
		SimpleInterestFunc.SetInputOutput(   40.0f,   40.0 * 0.90 );
		SimpleInterestFunc.SetInputOutput(   80.0f,   80.0 * 0.85 );
		SimpleInterestFunc.SetInputOutput(  160.0f,  160.0 * 0.80 );
		SimpleInterestFunc.SetInputOutput(  320.0f,  320.0 * 0.70 );
		SimpleInterestFunc.SetInputOutput(  640.0f,  640.0 * 0.60 );
		SimpleInterestFunc.SetInputOutput( 1280.0f, 1280.0 * 0.40 );

		CEOSAIMathFunction NationalCombatResistanceFunc;
		NationalCombatResistanceFunc.SetInputOutput( 0.0f, 1.0f );
		NationalCombatResistanceFunc.SetInputOutput( 0.3f, 1.0f );
		NationalCombatResistanceFunc.SetInputOutput( 0.8f, 0.1f );
		NationalCombatResistanceFunc.SetInputOutput( 1.1f, 0.0001f );

		// Score targets based on SimpleInterest, Access.  Reduce score if there's a lot of resistance.
		//   This has the problem that small, valuable targets with no resistance get low scores.
		m_fInitialEvaluation_BasicScore =
			(2.0f*fGeoValue) *
			SimpleInterestFunc.GetOutput( m_fSimpleInterest ) * //m_fSimpleInterest *
			TimeToTargetFunc.GetOutput( m_fNationalPathwayTimeToTarget ) * //( 2.0f / (fTimeToTarget+1.0f)) *
			fLandWaterGeoMultiplier *
			fSameGeoMultiplier *
			max( 0.1f, max( m_fTargetGroundSeaAttackAccess01, m_fTargetAirAttackAccess01 ) ) *
			NationalCombatResistanceFunc.GetOutput( fNationalResistanceCombatRatio );
	}
	m_fInitialEvaluation_EasyTargetScore = 0.0f;
	if( m_fResistanceCombatSignificance < 50.0f || fNationalResistanceCombatRatio < 0.1f )
	{
		CEOSAIMathFunction SimpleInterestFunc;
		SimpleInterestFunc.SetInputOutput(    0.0f,    0.0f );//    0.0        );
		SimpleInterestFunc.SetInputOutput(    5.0f,   10.0f );//
		SimpleInterestFunc.SetInputOutput(   10.0f,   12.0f );// 10.0 * 1.00 );
		SimpleInterestFunc.SetInputOutput(   20.0f,   14.0f );// 20.0 * 0.95 );
		SimpleInterestFunc.SetInputOutput(   40.0f,   15.0f );// 40.0 * 0.90 );
		SimpleInterestFunc.SetInputOutput(   80.0f,   16.0f );// 80.0 * 0.85 );
		SimpleInterestFunc.SetInputOutput(  160.0f,   17.0f );//160.0 * 0.80 );

		float fMaxResistanceCombatSignificance = max( 50.0f, 0.1f*fMyNationalCombatSignificance );
		CEOSAIMathFunction NationalCombatResistanceFunc;
		NationalCombatResistanceFunc.SetInputOutput( 0.0f, 1.0f );
		NationalCombatResistanceFunc.SetInputOutput( fMaxResistanceCombatSignificance, 0.9f );
		NationalCombatResistanceFunc.SetInputOutput( fMaxResistanceCombatSignificance+1.0f,0.0f );

		m_fInitialEvaluation_EasyTargetScore =
			(2.0f*fGeoValue) *
			SimpleInterestFunc.GetOutput( m_fSimpleInterest ) * //m_fSimpleInterest *
			TimeToTargetFunc.GetOutput( m_fNationalPathwayTimeToTarget ) * //( 2.0f / (fTimeToTarget+1.0f)) *
			fLandWaterGeoMultiplier *
			fSameGeoMultiplier *
			max( 0.1f, max( m_fTargetGroundSeaAttackAccess01, m_fTargetAirAttackAccess01 ) ) *
			NationalCombatResistanceFunc.GetOutput( m_fResistanceCombatSignificance );
	}
	m_fInitialEvaluation_NoResistanceScore = 0.0f;
	if( m_fResistanceCombatSignificance == 0.0f )
	{
		CEOSAIMathFunction SimpleInterestFunc;
		SimpleInterestFunc.SetInputOutput(    0.0f,    0.0f );//    0.0        );
		SimpleInterestFunc.SetInputOutput(    5.0f,   10.0f );//
		SimpleInterestFunc.SetInputOutput(   10.0f,   12.0f );// 10.0 * 1.00 );
		SimpleInterestFunc.SetInputOutput(   20.0f,   14.0f );// 20.0 * 0.95 );
		SimpleInterestFunc.SetInputOutput(   40.0f,   15.0f );// 40.0 * 0.90 );
		SimpleInterestFunc.SetInputOutput(   80.0f,   16.0f );// 80.0 * 0.85 );
		SimpleInterestFunc.SetInputOutput(  160.0f,   17.0f );//160.0 * 0.80 );

		//CEOSAIMathFunction NationalCombatResistanceFunc;
		//NationalCombatResistanceFunc.SetInputOutput( 0.0f, 2.0f );
		//NationalCombatResistanceFunc.SetInputOutput( 0.1f, 1.5f );
		//NationalCombatResistanceFunc.SetInputOutput( 0.11f,0.0f );

		m_fInitialEvaluation_NoResistanceScore =
			(2.0f*fGeoValue) *
			SimpleInterestFunc.GetOutput( m_fSimpleInterest ) * //m_fSimpleInterest *
			TimeToTargetFunc.GetOutput( m_fNationalPathwayTimeToTarget ) * //( 2.0f / (fTimeToTarget+1.0f)) *
			fLandWaterGeoMultiplier *
			fSameGeoMultiplier *
			max( 0.1f, max( m_fTargetGroundSeaAttackAccess01, m_fTargetAirAttackAccess01 ) );
	}

	//
	// Compile the JobToDo information
	//   Some strategies (invasion) involve all different jobs (Fight LocalResistance, PathwayResistance, ...)
	//   Other strategies involve flying over enemies and hitting the PrimaryGoals
	//   So, I've broken out the jobs into separate categories
	if( m_ePrimaryAction == enum_SubmarineWarzone )
	{
		//m_PrimaryGoals_JobsToDo.m_fSeaSupremacy = 1.0f;
		m_PrimaryGoals_JobsToDo.m_fSubmarineCapability = 1.0f;
	}
	else
	{
		ASSERT( m_PrimaryGoals_JobsToDo.IsEmpty() == false );
	}

	// Find all useful available (buildable or existing) units
	CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIBrain()->GetAIThoughtDatabase();
	m_UsefulAvailableUnitTemplates.RemoveAll();
	CEOSAIUnitTemplateSet*  pGetUnitsICanBuildOrHave = pAIThoughtDatabase->GetUnitsICanBuildOrHave();
	pos = pGetUnitsICanBuildOrHave->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = pGetUnitsICanBuildOrHave->m_List.GetNext( pos );
		if( m_PrimaryGoals_JobsToDo.CanFulfillAJob( pAIUnitTemplate ) )
		{
			m_UsefulAvailableUnitTemplates.AddTail( pAIUnitTemplate );
		}
	}
	// Find all useful BUILDABLE units
	m_UsefulBuildableUnitTemplates.RemoveAll();
	CEOSAIUnitTemplateSet*  pGetUnitsICanBuild = pAIThoughtDatabase->GetUnitsICanBuild();
	pos = pGetUnitsICanBuild->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pBuildableActor = pGetUnitsICanBuild->m_List.GetNext( pos );
		if( m_PrimaryGoals_JobsToDo.CanFulfillAJob( pBuildableActor ) )
		{
			m_UsefulBuildableUnitTemplates.AddTail( pBuildableActor );
		}
	}

	// Calculate Order Of Processing
	{
		CEOSAIMathFunction BasicScoreToProcessingValue;
		BasicScoreToProcessingValue.SetInputOutput(     0.0f, 0.0001f );
		BasicScoreToProcessingValue.SetInputOutput(    10.0f, 4.0f );
		BasicScoreToProcessingValue.SetInputOutput(   100.0f, 8.0f );
		BasicScoreToProcessingValue.SetInputOutput(  1000.0f, 9.0f );
		BasicScoreToProcessingValue.SetInputOutput( 10000.0f, 10.0f );

		CEOSAIMathFunction UrgencyToProcessingValue;
		UrgencyToProcessingValue.SetInputOutput( 0.0f,  0.2f );
		UrgencyToProcessingValue.SetInputOutput( 0.5f,  2.0f );
		UrgencyToProcessingValue.SetInputOutput( 1.0f, 10.0f );

		// High Resistance requires lots of units, so I need to process it earlier
		CEOSAIMathFunction ResistanceToProcessingValue;
		ResistanceToProcessingValue.SetInputOutput(    0.0f,  0.5f );
		ResistanceToProcessingValue.SetInputOutput(   50.0f,  2.0f );
		ResistanceToProcessingValue.SetInputOutput(  500.0f,  5.0f );
		ResistanceToProcessingValue.SetInputOutput( 5000.0f, 10.0f );

		m_fOrderOfProcessing = 
			BasicScoreToProcessingValue.GetOutput( m_fInitialEvaluation_BasicScore ) *
			UrgencyToProcessingValue.GetOutput( m_fUrgency01 ) *
			ResistanceToProcessingValue.GetOutput( m_fResistanceCombatSignificance );

		ASSERT( m_fOrderOfProcessing >  0.0f );
	}
}


void  CEOSAITacticalProject2::CalculateExpectedPathwayResistance_UsingNationalPathway()
{
	POSITION pos;
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();
	CMap< CEOSAIGenericRegion*, CEOSAIGenericRegion*, float,float >  ProcessedMultiRegions;
	CMap< CEOSAIPoiObject*,CEOSAIPoiObject*, CEOSAIPoiObjectProximity*,CEOSAIPoiObjectProximity* >  PoiObjectsNearPathway;

	CEOSAIIntSet GeosPathway;
	long iAIPlayer = GetAIPlayer()->GetPlayerNumber();
	//CEOSAIMultiRegionNationwidePathways* pMyNationalPathway = pWorldDescServer->GetAICommonData()->GetNationwidePathways( iAIPlayer );
	CEOSAIMultiRegionNationwidePathways* pMyNationalPathway = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );

	CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( m_Locations.GetHead() );
	ASSERT( pMultiRegion );
	if( pMultiRegion )
	{
		long iMultiRegion = pMultiRegion->GetId();
		//CEOSAIDesireSpatial* pPrimaryGoal = m_PrimaryGoals.GetNext( pos );
		//long iMultiRegion = pPrimaryGoal->GetMultiRegion()->GetId();

		//CEOSAIRegionPathwayItem* pRegionPathwayItem1 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitResUnits( iMultiRegion );
		CEOSAIRegionPathwayItem* pRegionPathwayItem1 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitRes( iMultiRegion );
		//CEOSAIRegionPathwayItem* pRegionPathwayItem2 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitResUnits( iMultiRegion );
		//float fAverage = ( pRegionPathwayItem1->m_fDistance + pRegionPathwayItem2->m_fDistance ) / 2.0f;
		//m_fDistanceFromMyCitResToPrimaryGoals = min( m_fDistanceFromMyCitResToPrimaryGoals, fAverage );
		//m_fTimeFromMyCitResToPrimaryGoals = m_fDistanceFromMyCitResToPrimaryGoals/10.0f;

		m_NationwideMultiRegionPathway.RemoveAll();

		// Add Danger
		while( pRegionPathwayItem1 )
		{
			CEOSAIGenericRegion* pGenericRegion = dynamic_cast< CEOSAIGenericRegion* >( pRegionPathwayItem1->m_pPathfinderPoint );
			ASSERT( pGenericRegion );
			CEOSAIMultiRegion2* pMultiRegion = dynamic_cast< CEOSAIMultiRegion2* >( pGenericRegion );
			ASSERT( pMultiRegion );

			m_NationwideMultiRegionPathway.AddHead( pMultiRegion );
			GeosPathway.Add( pMultiRegion->GetGeoId() );

			//
			float fDummy=0.0f;
			if( ProcessedMultiRegions.Lookup( pGenericRegion, fDummy ) == FALSE )
			{
				ProcessedMultiRegions.SetAt( pGenericRegion,1.0f );

				// Find all the PoiObjects contained in this Region's PoiObjectProximity List
				POSITION pos2 = pGenericRegion->GetPoiObjectProximities()->GetHeadPosition();
				while( pos2 )
				{
					CEOSAIPoiObjectProximity* pRegionProximityObject = pGenericRegion->GetPoiObjectProximities()->GetNext( pos2 );
					ASSERT( pRegionProximityObject->m_pAIPoiObject->IsAlive() );

					if( m_Id == 1 && pRegionProximityObject->m_pAIPoiObject->GetObjectId() == 117 )
					{
						int h=0;
					}

					//float fDistanceAccordingToList=1000000.0f;
					//if( PoiObjectsNearPathway.Lookup( pRegionProximityObject->m_pPoiObject, fDistanceAccordingToList ) )
					CEOSAIPoiObjectProximity* pExistingProximityData = NULL;
					if( PoiObjectsNearPathway.Lookup( pRegionProximityObject->m_pAIPoiObject, pExistingProximityData ) )
					{
						if( pExistingProximityData->m_fDistanceFromRegionEdge > pRegionProximityObject->m_fDistanceFromRegionEdge )
						{
							PoiObjectsNearPathway.SetAt( pRegionProximityObject->m_pAIPoiObject,pRegionProximityObject );
						}
					}
					else
					{
						PoiObjectsNearPathway.SetAt( pRegionProximityObject->m_pAIPoiObject,pRegionProximityObject );
					}
				}
			}
			// Step to the previous PathwayItem
			//pRegionPathwayItem1 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitResUnits( pRegionPathwayItem1->m_iPreviousPathwayItem );
			pRegionPathwayItem1 = pMyNationalPathway->GetDistGeoPathfindingValueFromMyCitRes( pRegionPathwayItem1->m_iPreviousPathwayItem );
		}
	}

	// Record the PoiObjects near pathway
	//while( m_PoiObjectsNearPathway.IsEmpty() == FALSE ){ delete m_PoiObjectsNearPathway.RemoveHead(); }
	pos = PoiObjectsNearPathway.GetStartPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = NULL;
		//float fPoiObjectDistanceFromPath = 0.0f;
		//PoiObjectsNearPathway.GetNextAssoc( pos, pPoiObject, fPoiObjectDistanceFromPath );
		CEOSAIPoiObjectProximity* pProximityObject = NULL;
		PoiObjectsNearPathway.GetNextAssoc( pos, pAIPoiObject, pProximityObject );

		//if( m_Id == 1 && 
		//  ( pPoiObject->GetObjectId() == 117 || pPoiObject->GetObjectId() == 117 ) )

		if( pProximityObject->m_fDistanceFromRegionEdge < 100.0f ) // "Distance" is from the edge of the multiregion
		{
			ASSERT( pAIPoiObject->IsDead() == false ); // ignore dead PoiObjects
			/*
			CPoiObjectDistance* pPoiObjectDistance = new CPoiObjectDistance();
			pPoiObjectDistance->m_pPoiObject = pPoiObject;
			pPoiObjectDistance->m_fDistance = pProximityObject->m_fDistance;//fPoiObjectDistanceFromPath;
			m_PoiObjectsNearPathway.AddTail( pPoiObjectDistance );
			*/
			m_PoiObjectsNearNationwidePathway.AddTail( pProximityObject );
		}
	}

	//
	pos = m_PoiObjectsNearNationwidePathway.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObjectProximity* pPoiObjectDistance = m_PoiObjectsNearNationwidePathway.GetNext( pos );
		CEOSAIPoiObject* pAIPoiObject = pPoiObjectDistance->m_pAIPoiObject;
		float fPoiObjectDistanceFromPath = pPoiObjectDistance->m_fDistanceFromRegionEdge;

		// Lookup the Spatial desire and add it to the ExpectedResistance (if it's not mine, not Player0,...)
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			//CPoiObject* pPlayerPoiObject = pWorldDescPlayer->GetPoiObject( pServerPoiObject->GetObjectId() );
			//ASSERT( pPlayerPoiObject );

			//CPoiObject* pPoiObjectCopy = pRes->AIFunc_GetWorldDescPlayerCopyOfThisPoiObject();
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit ) // AIPoiObject can be NULL if unit is dead
			{
				long iPoiObjectGeo = pAIPoiObject->GetGeoId();
				bool bIsOnTheSameGeo = GeosPathway.Exists( iPoiObjectGeo );
				if( pAIUnit->IsAirUnit() || bIsOnTheSameGeo )
				{
					CEOSAIDesireSpatial* pDesire = pAIUnit->GetAIDesire( EnumDestroyUnit );
					if( pDesire &&
						pDesire->GetAIPoiObjectTarget()->GetOwner() != 0 && // Player0 units won't attack
						IsPrimaryGoal( pDesire ) == false && 
						IsLocalResistance( pDesire ) == false &&
						IsPathwayResistance( pDesire ) == false )
					{
						long iTargetOwner = pDesire->GetAIPoiObjectTarget()->GetOwner();
						EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iTargetOwner, m_pAIBrain->GetAIPlayerNumber() );

						if( eRel.IsEnemy() || PrimaryGoalsAreHostileActionAgainstPlayerN( iTargetOwner ) )
						{
							ActionScheduler_AddAsPathwayResistance( pDesire );
						}
					}
				}
			}
		}
		if( GetFlag_AddOnTheWayOpportunities() )
		{
			// Add some ideas to the "on the way" list
			CEOSAIResource* pAIRes = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
			if( pAIRes )
			{
				//CPoiObject* pPlayerPoiObject = pWorldDescPlayer->GetPoiObject( pServerPoiObject->GetObjectId() );
				//ASSERT( pPlayerPoiObject );

				// On-The-Way Opportunities must be *IN* the MultiRegion pathway, not just "near" it
				//   Without this check, I sometimes pickup land resources while on a water-route
				bool bIsInTheList = false;
				CEOSAIMultiRegion2* pResourceMultiRegion = pMultiRegionManager->GetMultiRegion( pAIPoiObject->GetLocation() );
				POSITION pos = m_NationwideMultiRegionPathway.GetHeadPosition();
				while( pos )
				{
					CEOSAIMultiRegion2* pPathStepMultiRegion = m_NationwideMultiRegionPathway.GetNext( pos );
					if( pResourceMultiRegion == pPathStepMultiRegion )
					{
						bIsInTheList = true;
						break;
					}
				}
				if( bIsInTheList )
				{
					//CResourceSource* pPlayerRes = dynamic_cast< CResourceSource* >( pPlayerPoiObject );
					//ASSERT( pPlayerRes );
					CEOSAIDesireSpatial* pDesire = pAIRes->GetAIDesire( EnumCaptureResource );
					// ASSERT( pDesire ); - If this player owns this Resource, there will be no desire associated with it
					if( pDesire &&
						IsPrimaryGoal( pDesire ) == false && 
						IsLocalResistance( pDesire ) == false &&
						IsPathwayResistance( pDesire ) == false )
					{
						//ActionScheduler_AddAsExpectedResistance( pDesire );
						ActionScheduler_AddAsOnTheWayOpportunity( pDesire );
					}
				}
			}
		}
	}

	//
	CalculateProductionCostOfUnitNeeds();
}


bool CEOSAITacticalProject2::InvolvesGroundTargets()
{
	bool bInvolvesGroundTargets = false;
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		if( pDesire->InvolvesGroundTargets() ) return true;
	}
	return false;
}

void  CEOSAITacticalProject2::CreateStrategiesAndUnitActionIdeas()
{
	POSITION pos;
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	CEOSAIThoughtDatabase* pAIThoughtDatabase = m_pAIBrain->GetAIThoughtDatabase();

	ASSERT( m_pInvasionStrategy == NULL );
	ASSERT( m_pAirDropStrategy == NULL );
	ASSERT( m_pAirStrikeStrategy == NULL );
	ASSERT( m_pBombardmentStrategy == NULL );
	ASSERT( m_pAirSearchAndDestroyStrategy == NULL );
	ASSERT( m_pSubmarineStrategy == NULL );

	// Check some strategies
	if( m_ePrimaryAction == enum_CaptureArea )
	{
		float fFastestAirTransport = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetFastestAirTransportMovementRate();

		m_pInvasionStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_Invasion );
		m_pAirDropStrategy = NULL;
		if( InvolvesGroundTargets() && fFastestAirTransport > 0.0f )
		{
			m_pAirDropStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_InvasionWithAirDrop );
		}
		m_pAirStrikeStrategy = NULL;
		m_pBombardmentStrategy = NULL;
		m_pAirSearchAndDestroyStrategy = NULL;
		m_pSubmarineStrategy = NULL;
	}
	eif( m_ePrimaryAction == enum_DegradeCity )
	{
		float fFastestAirTransport = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetFastestAirTransportMovementRate();

		m_pInvasionStrategy = NULL;
		if( InvolvesGroundTargets() && fFastestAirTransport > 0.0f )
		{
			m_pAirDropStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_InvasionWithAirDrop );
		}
		m_pAirStrikeStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_AirStrike );
		m_pBombardmentStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_Bombardment );
		m_pAirSearchAndDestroyStrategy = NULL;
		m_pSubmarineStrategy = NULL;
	}
	eif( m_ePrimaryAction == enum_DestroyUnits )
	{
		float fFastestAirTransport = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetFastestAirTransportMovementRate();

		m_pInvasionStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_Invasion );
		if( InvolvesGroundTargets() && fFastestAirTransport > 0.0f )
		{
			m_pAirDropStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_InvasionWithAirDrop );
		}
		m_pAirStrikeStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_AirStrike );
		m_pBombardmentStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_Bombardment );
		m_pAirSearchAndDestroyStrategy = NULL;
		m_pSubmarineStrategy = NULL;
	}
	/*
	eif( m_ePrimaryAction == enum_EasyTargetHunter )
	{
		ASSERT( false );
		m_pInvasionStrategy = NULL;
		m_pAirDropStrategy = NULL;
		m_pAirStrikeStrategy = NULL;
		m_pBombardmentStrategy = NULL;
		m_pAirSearchAndDestroyStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_AirSearchAndDestroyStrategy );
		m_pSubmarineStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_SubmarineStrategy );
	}
	*/
	eif( m_ePrimaryAction == enum_ShipWarzone )
	{
		m_pInvasionStrategy = NULL;
		m_pAirDropStrategy = NULL;
		m_pAirStrikeStrategy = NULL;
		m_pBombardmentStrategy = NULL;
		m_pAirSearchAndDestroyStrategy = NULL;
		m_pSubmarineStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_ShipStrategy );
	}
	eif( m_ePrimaryAction == enum_SubmarineWarzone )
	{
		m_pInvasionStrategy = NULL;
		m_pAirDropStrategy = NULL;
		m_pAirStrikeStrategy = NULL;
		m_pBombardmentStrategy = NULL;
		m_pAirSearchAndDestroyStrategy = NULL;
		m_pSubmarineStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_SubmarineStrategy );
	}
	eif( m_ePrimaryAction == enum_ProtectArea )
	{
		m_pInvasionStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_Invasion );
		m_pAirDropStrategy = NULL;
		m_pAirStrikeStrategy = NULL;
		m_pBombardmentStrategy = NULL;
		m_pAirSearchAndDestroyStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_AirSearchAndDestroyStrategy );
		m_pSubmarineStrategy = new CEOSAITacticalProject2Strategy( this, CEOSAITacticalProject2Strategy::enum_SubmarineStrategy );
	}
	else
	{
		ASSERT( false );
	}
	//Checkpoint::Write( "CreateStrategiesAndBestCaseActionIdeas() - 1" );


	// This should probably go into the Tactical Project
	//   No need to recreate these objects for each TaskForce (i.e. every TacticalProjectStrategy)
	//   Or, maybe I could cache the information somewhere and reuse it?

	#ifdef _DEBUG
	// Get a count on the number of units and cities
	long iUnitCount = 0;
	long iCityCount = 0;
	pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			iCityCount++;
		}
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			iUnitCount++;
		}
	}
	int g=0;
	#endif _DEBUG

	long iCurrentUnitCity = 0;
	pos = pAIThoughtDatabase->GetMyActors()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = pAIThoughtDatabase->GetMyActors()->GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			//Checkpoint::Write( "c1" );
			// Add all cities - create an AICityActionIdea
		//	CEOSAICityActionIdeaNode* pAICityActionIdeaNode = new CEOSAICityActionIdeaNode( this, pAICity );
		//	pAICityActionIdeaNode->CalculateAirSeaGroundPathways();
			//pAICityActionIdea->CalculatePathwayAndTimeToTarget();
			//pAICityActionIdea->CalculatePathwayAndTimeToTarget();
		//	m_CityActionIdeaNodes.AddTail( pAICityActionIdeaNode );
			m_pAIBrain->m_MyCities_InvokePathwaysToEverywhere.Continue();
			pAICity->InvokeMultiRegionPathwaysToEverywhere();
			m_pAIBrain->m_MyCities_InvokePathwaysToEverywhere.Stop();
			//Checkpoint::Write( "c2" );
		}
		//
		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			ASSERT( pAIUnit->IsHypotheticalPoiObject() == false );//>m_pBuiltByCity == NULL );
			//Checkpoint::Write( "u1" );

			// Add it to the UnitActionIdeas if:
			//   (1) The unit can fulfill a job
			//   (2) It's a transport/carrier (support role might be useful)
			//if( m_JobsToDo.CanFulfillAJob( pAIUnit->GetUnitTemplate() ) )
			if( m_PrimaryGoals_JobsToDo.CanFulfillAJob( pAIUnit->GetAIUnitTemplate() ) ||
				m_LocalResistance_JobsToDo.CanFulfillAJob( pAIUnit->GetAIUnitTemplate() ) ||
				m_PathwayResistance_JobsToDo.CanFulfillAJob( pAIUnit->GetAIUnitTemplate() ) ||
				m_OnTheWayOpportunities_JobsToDo.CanFulfillAJob( pAIUnit->GetAIUnitTemplate() ) )//||
				//pAIUnit->GetUnitTemplate()->CanContainGroundUnits() || 
				//pAIUnit->GetUnitTemplate()->CanContainAirUnits() )
			{
				m_pAIBrain->m_MyUnits_CreateUnitActionIdeas.Continue();
				// Create an AIUnitActionIdea
				CEOSAIUnitActionIdea* pAIUnitActionIdea = new CEOSAIUnitActionIdea( this, pAIUnit );
				//pAIUnitActionIdea->SetActionType( CEOSAIUnitActionIdea::EnumActionType_Capture ); // TEMP
				//pAIUnitActionIdea->Capture(); // TEMP

				if( this->ProjectIncludesObjectIdAsPrimaryTask( 12 ) )
				{
					// why is the pAIUnitActionIdea->CalculateTimeToTarget() setting m_fTransportComboTime = 100000.0?
					int h=0;
				}

				m_pAIBrain->m_MyUnits_CalculateTimeToTarget.Continue();
				pAIUnitActionIdea->CalculateTimeToTarget();
				m_pAIBrain->m_MyUnits_CalculateTimeToTarget.Stop();
				m_pAIBrain->m_MyUnits_InvokeTransportMovementDescs.Continue();
				pAIUnitActionIdea->InvokeMultiRegionTransportMovementDescs();
				m_pAIBrain->m_MyUnits_InvokeTransportMovementDescs.Stop();

				m_UnitActionIdeas.AddTail( pAIUnitActionIdea );
				//Checkpoint::Write( "u3" );
				m_pAIBrain->m_MyUnits_CreateUnitActionIdeas.Stop();
			}

			#ifdef _DEBUG
			bool b1 = m_PrimaryGoals_JobsToDo.CanFulfillAJob( pAIUnit->GetAIUnitTemplate() );
			bool b2 = m_LocalResistance_JobsToDo.CanFulfillAJob( pAIUnit->GetAIUnitTemplate() );
			bool b3 = m_PathwayResistance_JobsToDo.CanFulfillAJob( pAIUnit->GetAIUnitTemplate() );
			bool b4 = m_OnTheWayOpportunities_JobsToDo.CanFulfillAJob( pAIUnit->GetAIUnitTemplate() );
			#endif
			//Checkpoint::Write( "u4" );
		}
		ASSERT( pAICity || pAIUnit );
		iCurrentUnitCity++;

		if( iCurrentUnitCity > 10 )
		{
			int h244=0;
		}
		int hsdf=0;
	}
	int h=0;
	//Checkpoint::Write( "1b" );
}

void  CEOSAITacticalProject2::CreateBestCaseTaskForce()
{
	// Should I evaluate each section (PrimaryGoals, LocalResistance, PathwayResistance, etc) separately?
	CList< CEOSAITacticalProject2Strategy* >  StrategyList;
	GetAITacticalProjectStrategies( &StrategyList );
	POSITION pos = StrategyList.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2Strategy* pTacticalProjectStrategy = StrategyList.GetNext( pos );
		pTacticalProjectStrategy->ConstructBestCaseTaskForce();
		float f = pTacticalProjectStrategy->GetBestCaseTaskForceValue();
		int g=0;
	}
}
/*
void CEOSAITacticalProject2::AddUnitActionIdea( CEOSAIUnitActionIdea* pUnitActionValue )
{
	m_UnitActionIdeas.AddTail( pUnitActionValue );
}

void CEOSAITacticalProject2::RemoveUnitActionIdea( CEOSAIUnitActionIdea* pUnitActionValue )
{
	POSITION pos = m_UnitActionIdeas.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnitActionIdea* pUnitActionValueInList = m_UnitActionIdeas.GetNext( pos );
		if( pUnitActionValueInList == pUnitActionValue )
		{
			m_UnitActionIdeas.RemoveAt( prevPos );
		}
	}
}
*/

CEOSAITacticalProject2Strategy*  CEOSAITacticalProject2::GetAllocatedStrategy()
{
	CList< CEOSAITacticalProject2Strategy* >  StrategyList;
	GetAITacticalProjectStrategies( &StrategyList );

	POSITION pos = StrategyList.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2Strategy* pStrategy = StrategyList.GetNext( pos );
		if( pStrategy->NegotiatedTaskForceHasBeenAllocated() ) return pStrategy;
	}
	return NULL;
}

bool  CEOSAITacticalProject2::NegotiatedTaskForceHasBeenAllocated()
{
	CList< CEOSAITacticalProject2Strategy* >  StrategyList;
	GetAITacticalProjectStrategies( &StrategyList );

	POSITION pos = StrategyList.GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2Strategy* pStrategy = StrategyList.GetNext( pos );
		if( pStrategy->NegotiatedTaskForceHasBeenAllocated() ) return true;
	}
	return false;
}

void CEOSAITacticalProject2::GetAITacticalProjectStrategies( CList< CEOSAITacticalProject2Strategy* >* pStrategyList )
{
	pStrategyList->RemoveAll();
	if( m_pInvasionStrategy ){ pStrategyList->AddTail( m_pInvasionStrategy ); }
	if( m_pAirDropStrategy ){ pStrategyList->AddTail( m_pAirDropStrategy ); }
	if( m_pAirStrikeStrategy ){ pStrategyList->AddTail( m_pAirStrikeStrategy ); }
	if( m_pBombardmentStrategy ){ pStrategyList->AddTail( m_pBombardmentStrategy ); }
	if( m_pAirSearchAndDestroyStrategy ){ pStrategyList->AddTail( m_pAirSearchAndDestroyStrategy ); }
	if( m_pSubmarineStrategy ){ pStrategyList->AddTail( m_pSubmarineStrategy ); }
}

//
// Arrival Time information (used for simultaneous arrival)
//
void  CEOSAITacticalProject2::AdjustWaitTimeForSimultaneousAttack()
{
	//ASSERT( false );
	/*
	ASSERT( m_bIsASecretInvasion );
	if( m_bIsASecretInvasion )
	{
		float fLastArrivalTime = 0.0f;
		POSITION pos = m_ResultSteps.GetHeadPosition();
		while( pos )
		{
			EOSAI::UnitPathwayResultStep* pResult = m_ResultSteps.GetNext( pos );
			fLastArrivalTime = max( fLastArrivalTime, pResult->GetUnitOrderEndTime() );
		}
		// Now that we have the last arrival time, setup some wait times
		float fTargetArrivalTime = fLastArrivalTime - 1.0f;
		pos = m_ResultSteps.GetHeadPosition();
		while( pos )
		{
			EOSAI::UnitPathwayResultStep* pResult = m_ResultSteps.GetNext( pos );
			if( pResult->GetUnitOrderEndTime() < fTargetArrivalTime )
			{
				float fWaitTime = fTargetArrivalTime - pResult->GetUnitOrderEndTime();
				if( pResult->m_SupportOrders.IsEmpty() == FALSE )
				{
					CUnitOrder* pUnitOrder = pResult->m_SupportOrders.GetHead();
					pUnitOrder->SetPreOrdersWaitTime( fWaitTime );
				}
				else
				{
					CUnitOrder* pUnitOrder = pResult->m_pUnitOrder;
					pUnitOrder->SetPreOrdersWaitTime( fWaitTime );
				}
			}
		}
	}
	*/
}


//
// Debug
//
bool CEOSAITacticalProject2::ProjectIncludesObjectIdAsPrimaryTask( long iObjectId )
{
	POSITION pos = m_PrimaryGoals.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		if( pDesire && 
			pDesire->GetAIPoiObjectTarget() &&
			pDesire->GetAIPoiObjectTarget()->GetObjectId() == iObjectId )
		{
			return true;
		}
	}
	return false;
}

void CEOSAITacticalProject2::CalculateProductionCostOfUnitNeeds()
{
	/*
	m_fExpectedResistanceUnitCost = 0.0f;
	CList< CEOSAIDesireSpatial* > Desires; GetDesires( &Desires );
	POSITION pos = Desires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = Desires.GetNext( pos );
		CPoiObject* pPoiObject = pDesire->GetPoiObjectTarget();
		CUnit* pTargetUnit = dynamic_cast< CUnit* >( pPoiObject );
		if( pTargetUnit )
		{
			m_fExpectedResistanceUnitCost += pTargetUnit->GetProductionCost();
		}
	}
	*/

	CEOSAIUnitTemplateSet* pExistingOrCanBuildSet = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();

	// The missile cannon-fodder calculation is a little bit off
	// First of all, the missile values should be calculated based on each unit-category cost ratio
	// And it need to be based on to-hit, to-damage values
	//   right now, it is also based on missile hp and defense values

	// Add up the value of the units in terms of production and count

	// I set this tiered system up like this for one reason:
	//   If I need combat, CitResHunter, and airfield builder, I can do it with one unit.
	//   To avoid double-counting, I setup a tiered system.

	// I commented out this section because I'm eliminating the "Soft", "Armor", etc. references
	float fGroundCombatCost = 0.0f;
	float fSeaCombatCost = 0.0f;
	float fAirCombatCost = 0.0f;
	/*
	float fGroundCombatCost =
		m_UnitNeeds.m_fNeed_AntiSoftCombat      * pExistingOrCanBuildSet->GetLowestCostDivAntiSoftCombat() +
		m_UnitNeeds.m_fNeed_AntiArmorCombat     * pExistingOrCanBuildSet->GetLowestCostDivAntiArmorCombat();

	float fSeaCombatCost =
		m_UnitNeeds.m_fNeed_AntiShipCombat      * pExistingOrCanBuildSet->GetLowestCostDivAntiShipCombat() +
		m_UnitNeeds.m_fNeed_AntiSubmarineCombat * pExistingOrCanBuildSet->GetLowestCostDivAntiSubmarineCombat();

	float fAirCombatCost =
		m_UnitNeeds.m_fNeed_AntiAircraftCombat * pExistingOrCanBuildSet->GetLowestCostDivAntiAircraftCombat() +
		min( m_UnitNeeds.m_fNeed_AntiMissileCombat * pExistingOrCanBuildSet->GetLowestCostDivAntiMissileCombat(),
			 m_UnitNeeds.m_fNeed_AntiMissileCombat * pExistingOrCanBuildSet->GetHighestHPDefenseDivCost_UnitTypeSet()->GetHighestValue() );
			 //m_UnitNeeds.m_fNeed_AntiMissileCombat * pExistingOrCanBuildSet->GetHighestCostDivUnit() );
	 */

	//ASSERT( false ); // Do I need this, do I use this?
	//m_fTotalCostOfUnitNeeds = fGroundCombatCost + fSeaCombatCost + fAirCombatCost;
}

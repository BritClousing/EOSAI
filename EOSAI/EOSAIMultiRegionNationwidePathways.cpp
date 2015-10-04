
#include <stdafx.h>
#include "EOSAIMultiRegionNationwidePathways.h"
#include "EOSAIMultiRegionManager2.h"
//#include "CommonState.h"
//#include "WorldDesc.h"
//#include "WorldDescServer.h"
#include "EOSAIRegionPathfinder.h"
#include "EOSAIStopwatch2.h"
#include "EOSAICommonData.h"
#include "City.h"
#include "EOSAIAirfield.h"
#include "EOSAIUnit2.h"
//#include "Checkpoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum EnumPath
{
	EnumPath_CitRes,
	EnumPath_CitRes_Land,
	EnumPath_CitRes_Water,
	EnumPath_CitRes_Land_Water,
	EnumPath_CitRes_Water_Land,
	EnumPath_CitRes_Land_Water_Land,
	EnumPath_CitRes_Water_Land_Water,
	EnumPath_CitRes_Land_Water_Land_Water,
	EnumPath_CitRes_Water_Land_Water_Land
};

CEOSAIMultiRegionNationwidePathways::CEOSAIMultiRegionNationwidePathways()
{
	m_iPlayer = 0;
	//m_pWorldDesc = NULL;
	m_pMultiRegionManager = NULL;
}

CEOSAIMultiRegionNationwidePathways::~CEOSAIMultiRegionNationwidePathways()
{
	long iSize = m_MultiRegionAttackAccess.m_iSize;
	for( long i=0; i<iSize; i++ )
	{
		delete m_MultiRegionAttackAccess[i];
		m_MultiRegionAttackAccess[i] = NULL;
	}
}

void  CEOSAIMultiRegionNationwidePathways::Calculate()
{
	CalculateSightingAndPathwayInformation();
	CalculatePathwayPoiObjectsForEachMultiRegion();
}

void CEOSAIMultiRegionNationwidePathways::CalculateSightingAndPathwayInformation()
{
	//CWorldDesc* pWorldDesc = m_pWorldDesc;//m_pAIBrain->GetWorldDesc();
	//CWorldDescServer* pWorldDescServer = m_pMultiRegionManager->GetWorldDescServer();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();

	CEOSAIStopwatch2  Watch1;
	CEOSAIStopwatch2  Watch2;
	CEOSAIStopwatch2  Watch3;
	CEOSAIStopwatch2  Watch3b;
	CEOSAIStopwatch2  Watch4;
	CEOSAIStopwatch2  Watch5;
	CEOSAIStopwatch2  WatchTotal;
	WatchTotal.Start();

	Watch1.Start();
	// Delete the old information
	m_PolitePathFromMyCitResUnitsToRestOfWorld.Clear();
	m_DistGeoPathFromMyCitResUnitsToRestOfWorld.Clear();
	m_DistGeoPathFromMyCitResToRestOfWorld.Clear();
	m_MinimumGeoSwitchPathwayFromMyCitResUnitsToRestOfWorld.Clear();
	//m_LandPathFromMyCitGroundUnitToRestOfWorld.Clear();
	m_SeaPathFromMyCitiesToRestOfWorld.Clear();

	m_PlayerAttackPathwayArrayBasedOnCitResUnits.Clear();
	m_PlayerAttackPathwayArrayBasedOnCitRes.Clear();
	/*
	for( long iPlayer = 1; iPlayer<m_PlayerAttackPathwayArrayBasedOnCitResUnits.m_iSize; iPlayer++ )
	{
		delete m_PlayerAttackPathwayArrayBasedOnCitResUnits[iPlayer];
		m_PlayerAttackPathwayArrayBasedOnCitResUnits[iPlayer] = NULL;
	}
	for( long iPlayer = 1; iPlayer<m_PlayerAttackPathwayArrayBasedOnCitRes.m_iSize; iPlayer++ )
	{
		delete m_PlayerAttackPathwayArrayBasedOnCitRes[iPlayer];
		m_PlayerAttackPathwayArrayBasedOnCitRes[iPlayer] = NULL;
	}
	*/

	// Calculate the Sighting Presence for each MultiRegion
	//for( long i=0; i<m_pMultiRegionManager->m_MultiRegions.m_iArraySize; i++ )
	POSITION pos = m_pMultiRegionManager->GetMultiRegionList()->GetHeadPosition();
	{
		//CMultiRegion* pMultiRegion = m_pMultiRegionManager->m_MultiRegions.Get( i );
		CEOSAIMultiRegion2* pMultiRegion = m_pMultiRegionManager->GetMultiRegionList()->GetNext( pos );
		//if( pMultiRegion )
		{
			pMultiRegion->ClearSightingAndPowerValueForEachPlayer();
			//pMultiRegion->ClearSightingPresenceForEachPlayer();
			//pMultiRegion->ClearPowerValueForEachPlayer();
		}
	}
	//for( long i=0; i<m_pMultiRegionManager->m_MultiRegions.m_iArraySize; i++ )
	/*
	pos = m_pMultiRegionManager->GetMultiRegionList()->GetHeadPosition();
	{
		//CMultiRegion* pMultiRegion = m_pMultiRegionManager->m_MultiRegions.Get( i );
		//if( pMultiRegion )
		CMultiRegion* pMultiRegion = m_pMultiRegionManager->GetMultiRegionList()->GetNext( pos );
		{
			pMultiRegion->CalculateSightingPresenceForEachPlayer();
		}
	}
	*/
	// Calculate traversible regions
	Watch1.Stop();
	Watch2.Start();

	// Create Invasion Potential Tags
	//   Start with all the player's owned MultiRegions
	//   Then spread out from there, looking for MultiRegions that I don't own
	// Iterate over each of the Other Players and figure out how I would invade them

	// Do a quick rundown of the MultiRegion distances, ignore danger
	//   This comes up with distances based on the most direct movement pathways
	//m_DDSSFromMyCitResUnits.SetSortFunction( SortedByPathfindingValue );

	CEOSAIRegionPathfinder& RegionPathfinder = m_pMultiRegionManager->GetPathfinder();
	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();
	//
	RegionPathfinder.ClearPathfindingSettings();
	//RegionPathfinder.SetDistanceMultiplier( 1.0f );
	RegionPathfinder.SetGeoSwitchMultiplier( 300.0f );
	RegionPathfinder.SetToPolitePathway( m_iPlayer );
	RegionPathfinder.AddPlayerCitResUnitRegionsAsStartRegions( m_iPlayer );
	RegionPathfinder.Solve( m_iPlayer, true );
	RegionPathfinder.GetResult( true, &m_PolitePathFromMyCitResUnitsToRestOfWorld );
/*
	//CalculateDangerDistancesFromMyCitResUnitMultiRegionsToEntireMap( 
	//	m_pAIBrain->GetAIPlayerNumber(), 300.0f, 10.0f, 800.0f, &m_DDSSFromMyCitResUnits );
	m_pMultiRegionManager->ClearPathfindingSettings();
	m_pMultiRegionManager->SetDistanceMultiplier( 1.0f );
	m_pMultiRegionManager->SetGeoSwitchMultiplier( 300.0f );
	m_pMultiRegionManager->SetToPolitePathway( m_iPlayer );
	m_pMultiRegionManager->AddPlayerCitResUnitRegionsAsStartRegions( m_iPlayer );
	m_pMultiRegionManager->Solve( m_iPlayer, true, &m_PolitePathFromMyCitResUnitsToRestOfWorld );
*/
	RegionPathfinder.ClearPathfindingSettings();
	//RegionPathfinder.UseMultiRegions();
	//RegionPathfinder.SetDistanceMultiplier( 1.0f );
	RegionPathfinder.SetGeoSwitchMultiplier( 100.0f );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iPlayer );
	RegionPathfinder.AddPlayerCitResUnitRegionsAsStartRegions( m_iPlayer );
	RegionPathfinder.Solve( m_iPlayer, true );
	RegionPathfinder.GetResult( true, &m_DistGeoPathFromMyCitResUnitsToRestOfWorld );
/*
	m_pMultiRegionManager->ClearPathfindingSettings();
	m_pMultiRegionManager->SetDistanceMultiplier( 1.0f );
	m_pMultiRegionManager->SetGeoSwitchMultiplier( 100.0f );
	m_pMultiRegionManager->SetToIgnoreOtherPlayersPathway( m_iPlayer );
	//AddAllMyCitResUnitRegionsAsStartRegions();
	m_pMultiRegionManager->AddPlayerCitResUnitRegionsAsStartRegions( m_iPlayer );
	m_pMultiRegionManager->Solve( m_iPlayer, true, &m_DistGeoPathFromMyCitResUnitsToRestOfWorld );
*/
//	The pathfinder should take danger into account - 
//		at least to avoid moving through dangerous (or sighted) areas when there are safer routes.
//	This has to be done carefully, though, to avoid making it too weighted
//	Count up the power

	RegionPathfinder.ClearPathfindingSettings();
	//RegionPathfinder.UseMultiRegions();
	//RegionPathfinder.SetDistanceMultiplier( 1.0f );
	RegionPathfinder.SetPlayer( m_iPlayer );
	RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
	RegionPathfinder.SetGeoSwitchMultiplier( 100.0f );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iPlayer );
	RegionPathfinder.SetPathfindingAllyNeutralEnemySightingMultiplier( m_iPlayer, 0.0f,40.0f,40.0f );
	RegionPathfinder.SetPathfindingAllyNeutralEnemyPresenceMultiplier( m_iPlayer, 0.0f,40.0f,40.0f );
	RegionPathfinder.AddPlayerCitResRegionsAsStartRegions( m_iPlayer );
	//RegionPathfinder.Solve( m_iPlayer, true );
	RegionPathfinder.Solve3();
	RegionPathfinder.GetResult( true, &m_DistGeoPathFromMyCitResToRestOfWorld );
/*
	m_pMultiRegionManager->ClearPathfindingSettings();
	m_pMultiRegionManager->SetDistanceMultiplier( 1.0f );
	m_pMultiRegionManager->SetGeoSwitchMultiplier( 100.0f );
	m_pMultiRegionManager->SetToIgnoreOtherPlayersPathway( m_iPlayer );
	m_pMultiRegionManager->AddPlayerCitResRegionsAsStartRegions( m_iPlayer );
	m_pMultiRegionManager->Solve( m_iPlayer, true, &m_DistGeoPathFromMyCitResToRestOfWorld );
*/
	RegionPathfinder.ClearPathfindingSettings();
	//RegionPathfinder.UseMultiRegions();
	//RegionPathfinder.SetDistanceMultiplier( 1.0f );
	RegionPathfinder.SetPlayer( m_iPlayer );
	RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
	RegionPathfinder.SetGeoSwitchMultiplier( 10000.0f );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iPlayer );
	RegionPathfinder.SetPathfindingAllyNeutralEnemySightingMultiplier( m_iPlayer, 0.0f,40.0f,40.0f );
	RegionPathfinder.SetPathfindingAllyNeutralEnemyPresenceMultiplier( m_iPlayer, 0.0f,40.0f,40.0f );
	RegionPathfinder.AddPlayerCitResUnitRegionsAsStartRegions( m_iPlayer );
	//RegionPathfinder.Solve( m_iPlayer, true );
	RegionPathfinder.Solve3();
	RegionPathfinder.GetResult( true, &m_MinimumGeoSwitchPathwayFromMyCitResUnitsToRestOfWorld );
/*
	m_pMultiRegionManager->ClearPathfindingSettings();
	m_pMultiRegionManager->SetDistanceMultiplier( 1.0f );
	m_pMultiRegionManager->SetGeoSwitchMultiplier( 1000.0f );
	m_pMultiRegionManager->SetToIgnoreOtherPlayersPathway( m_iPlayer );
	//m_pMultiRegionManager->AddAllMyCitResUnitRegionsAsStartRegions();
	m_pMultiRegionManager->AddPlayerCitResUnitRegionsAsStartRegions( m_iPlayer );
	m_pMultiRegionManager->Solve( m_iPlayer, true, &m_MinimumGeoSwitchPathwayFromMyCitResUnitsToRestOfWorld );
*/

/*
	RegionPathfinder.ClearPathfindingSettings();
	//RegionPathfinder.UseMultiRegions();
	//RegionPathfinder.SetDistanceMultiplier( 1.0f );
	RegionPathfinder.SetGeoSwitchMultiplier( 10000.0f );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iPlayer );
	RegionPathfinder.SetMaximumGeoSwitches( 0 );
	RegionPathfinder.AddPlayerCitGroundUnitRegionsAsStartRegions( m_iPlayer );
	RegionPathfinder.Solve( m_iPlayer, true );
	RegionPathfinder.GetResult( true, &m_LandPathFromMyCitGroundUnitToRestOfWorld );
*/
	RegionPathfinder.ClearPathfindingSettings();
	//RegionPathfinder.UseMultiRegions();
	RegionPathfinder.SetPlayer( m_iPlayer );
	RegionPathfinder.SetDistanceMultiplier( 1.0f );
	//RegionPathfinder.SetGeoSwitchMultiplier( 10000.0f );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( m_iPlayer );
	RegionPathfinder.SetMaximumGeoSwitches( 0 );
	RegionPathfinder.AddMyCitiesAsStartRegions( m_iPlayer );
	RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsWater( true );
	//RegionPathfinder.Solve( m_iPlayer, false );
	RegionPathfinder.Solve3();
	RegionPathfinder.GetResult( true, &m_SeaPathFromMyCitiesToRestOfWorld );

/*
	m_pMultiRegionManager->ClearPathfindingSettings();
	m_pMultiRegionManager->SetDistanceMultiplier( 1.0f );
	m_pMultiRegionManager->SetGeoSwitchMultiplier( 1000.0f );
	m_pMultiRegionManager->SetToIgnoreOtherPlayersPathway( m_iPlayer );
	m_pMultiRegionManager->SetMaxGeoSwitches( 0 );
	m_pMultiRegionManager->AddPlayerCitGroundUnitRegionsAsStartRegions( m_iPlayer );
	m_pMultiRegionManager->Solve( m_iPlayer, true, &m_LandPathFromMyCitGroundUnitToRestOfWorld );
*/
	/*
	for( long i=1; i<120; i++ )
	{
		CMultiRegionPathwayItem* pResult = m_MinimumGeoSwitchPathwayFromMyCitResUnitsToRestOfWorld.GetMultiRegionResult( i );
		if( pResult )
		{
			int z=0;
		}
	}
	*/

	//long iMyPlayerNumber = GetAIBrain()->GetAIPlayerNumber();
	//m_PlayerAttackPathwayArrayBasedOnCitResUnits.SetSize( iNumberOfPlayers+1 );
	//m_PlayerAttackPathwayArrayBasedOnCitResUnits[0] = NULL;
	//for( long iPlayer1 = 1; iPlayer1<iNumberOfPlayers+1; iPlayer1++ )
	{
		//if( iPlayer1 == iMyPlayerNumber ) continue;
		/*
		EOSAIEnumForeignRelations eMyRelToPlayer = EOSAIEnumForeignRelations::enum_PermanentAlliance;
		if( iMyPlayerNumber != iPlayer1 )
		{
			eMyRelToPlayer = pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer1 );
		}
		if( eMyRelToPlayer.IsPAlly() == false )
		*/
		{
			//m_PlayerAttackPathwayArrayBasedOnCitResUnits[iPlayer1] = new CMultiRegionPathwayResult();
			RegionPathfinder.ClearPathfindingSettings();
			RegionPathfinder.AddPlayerCitResUnitRegionsAsStartRegions( m_iPlayer );
			int g=0;
			RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
			RegionPathfinder.SetGeoSwitchMultiplier( 400.0f );
			RegionPathfinder.SetPlayer( m_iPlayer );
			RegionPathfinder.ApplyStrategy( CEOSAIRegionPathfinder::IgnoreNeutrals, CEOSAIRegionPathfinder::AllowSomeEnemyContact, 10.0f );
			//RegionPathfinder.ApplyStrategy( CEOSAIRegionPathfinder::SemiPoliteToNeutrals, CEOSAIRegionPathfinder::AllowSomeEnemyContact, 10.0f );
			RegionPathfinder.Solve3();
			//RegionPathfinder.Solve( m_iPlayer, true );
			RegionPathfinder.GetResult( true, &m_PlayerAttackPathwayArrayBasedOnCitResUnits );

			/*
			#ifdef _DEBUG
			long iNumberOfMultiRegions = pMultiRegionManager->GetNumberOfMultiRegions();
			for( long iMultiRegion=1; iMultiRegion<=iNumberOfMultiRegions; iMultiRegion++ )
			{
				ASSERT( m_PlayerAttackPathwayArrayBasedOnCitResUnits.GetRegionResult( iMultiRegion ) );
			}
			//CMultiRegion* pMultiRegion = pMultiRegionManager->GetMultiRegion( 96 );
			#endif _DEBUG
			*/
			int h=0;
		}
	}
	//m_PlayerAttackPathwayArrayBasedOnCitRes.SetSize( iNumberOfPlayers+1 );
	//m_PlayerAttackPathwayArrayBasedOnCitRes[0] = NULL;
	//for( long iPlayer1 = 1; iPlayer1<iNumberOfPlayers+1; iPlayer1++ )
	{
		/*
		EOSAIEnumForeignRelations eMyRelToPlayer = EOSAIEnumForeignRelations::enum_PermanentAlliance;
		if( iMyPlayerNumber != iPlayer1 )
		{
			eMyRelToPlayer = pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer1 );
		}
		if( eMyRelToPlayer.IsPAlly() == false )
		*/
		{
			//m_PlayerAttackPathwayArrayBasedOnCitRes[iPlayer1] = new CMultiRegionPathwayResult();
			RegionPathfinder.ClearPathfindingSettings();
			RegionPathfinder.AddPlayerCitResRegionsAsStartRegions( m_iPlayer );
			//RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
			RegionPathfinder.SetGeoSwitchMultiplier( 400.0f );
			RegionPathfinder.SetPlayer( m_iPlayer );
			RegionPathfinder.ApplyStrategy( CEOSAIRegionPathfinder::IgnoreNeutrals, CEOSAIRegionPathfinder::AllowSomeEnemyContact, 10.0f );
			//RegionPathfinder.ApplyStrategy( CEOSAIRegionPathfinder::SemiPoliteToNeutrals, CEOSAIRegionPathfinder::AllowSomeEnemyContact, 10.0f );
			//RegionPathfinder.Solve( m_iPlayer, true );
			RegionPathfinder.Solve3();
			RegionPathfinder.GetResult( true, &m_PlayerAttackPathwayArrayBasedOnCitRes );
		}
	}
	Watch2.Stop();
	Watch3.Start();

	CEOSAIMathFunction GroundSeaDistanceFunc;
	GroundSeaDistanceFunc.SetInputOutput(    0.0f, 1.0f );
	GroundSeaDistanceFunc.SetInputOutput(  100.0f, 1.0f );
	GroundSeaDistanceFunc.SetInputOutput( 1000.0f, 0.0f );

	// Setup the m_MultiRegion_GroundSeaAttackAccess01 array
	long iNumberOfMultiRegions = m_pMultiRegionManager->GetNumberOfMultiRegions();
	ASSERT( m_MultiRegionAttackAccess.m_iSize == 0 );
	m_MultiRegionAttackAccess.SetSize( iNumberOfMultiRegions+1 );
	for( long iMultiRegion=0; iMultiRegion<iNumberOfMultiRegions+1; iMultiRegion++ )
	{
		m_MultiRegionAttackAccess[iMultiRegion] = new CEOSAIMultiRegionAttackAccess();
		m_MultiRegionAttackAccess[iMultiRegion]->SetAirUnitAccess01( 0.0f );
		m_MultiRegionAttackAccess[iMultiRegion]->SetGroundSeaUnitAccess01( -1.0f );
		m_MultiRegionAttackAccess[iMultiRegion]->SetSubmarineAccess01( -1.0f );
		/*
		//m_MultiRegionAttackAccess[iMultiRegion]->m_fResistanceCombatSignificance = 0.0f;

		CEOSAIRegionPathwayItem* pPathwayItem = m_PlayerAttackPathwayArrayBasedOnCitRes.GetRegionResult( iMultiRegion );
		if( pPathwayItem )
		{
			//pPathwayItem->InvokeCalculatePathwayPoiObjectsWithin200px();
			CMultiRegion* pMultiRegion = dynamic_cast< CMultiRegion* >( pPathwayItem->m_pPathfinderPoint );
			POSITION pos = pMultiRegion->GetPoiObjectsInsideRegion()->GetHeadPosition();
			while( pos )
			{
				CPoiObject* pPoiObject = pMultiRegion->GetPoiObjectsInsideRegion()->GetNext( pos );
				CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
				if( pUnit && pPoiObject->GetForeignRelationsTo( m_iPlayer ) <= EOSAIEnumForeignRelations::enum_Neutral )
				{
					m_MultiRegionAttackAccess[iMultiRegion]->m_fResistanceCombatSignificance += pUnit->GetCombatSignificance();
				}
			}
		}
		*/
	}
	Watch3.Stop();
	Watch3b.Start();
	// Calculate the Air Access
	// What's my maximum air-range?
	float fMaxAirRange = 0.0f;
	//pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	//pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
	pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
		if( pAIUnit && pAIUnit->GetOwner() == m_iPlayer && pAIUnit->IsAirUnit() )
		{
			fMaxAirRange = max( fMaxAirRange, pAIUnit->GetMovementRangeForAttack() );
		}
	}
	CList< CEOSAIPoiObject* >  Airbases;
	pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		if( pAIPoiObject && pAIPoiObject->GetOwner() == m_iPlayer )
		{
			CEOSAICity* pCity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			CEOSAIAirfield* pAirfield = dynamic_cast< CEOSAIAirfield* >( pAIPoiObject );
			if( ( pCity || pAirfield )  )
			{
				Airbases.AddTail( pAIPoiObject );
			}
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
			if( pAIUnit && pAIUnit->GetNumberOfAirUnitsThatICanContain() && pAIUnit->CanLaunchAirUnits() )
			{
				Airbases.AddTail( pAIPoiObject );
			}
		}
	}
	for( long iMultiRegion1=1; iMultiRegion1<iNumberOfMultiRegions+1; iMultiRegion1++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_pMultiRegionManager->GetMultiRegion( iMultiRegion1 );
		ASSERT( Airbases.GetCount() > 0 );
		POSITION pos = Airbases.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = Airbases.GetNext( pos );
			//float fDistance = pWorldDescServer->GetPixelDistance( pPoiObject->GetLocation(),pMultiRegion->GetLocation() );
			float fDistance = g_pWorldDistanceTool->GetDistance( pAIPoiObject->GetLocation(),pMultiRegion->GetLocation() );
			fDistance -= pMultiRegion->m_fRegionRadius;
			if( fDistance < fMaxAirRange )
			{
				m_MultiRegionAttackAccess[iMultiRegion1]->SetAirUnitAccess01( 1.0f );
			}
		}

		/*
		POSITION pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
		while( pos )
		{
			CPoiObject* pPoiObject = pWorldDescServer->GetActorsList()->GetNext( pos );
			if( pPoiObject && pPoiObject->GetOwner() == m_iPlayer )
			{
				CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
				CAirfield* pAirfield = dynamic_cast< CAirfield* >( pPoiObject );
				if( ( pCity || pAirfield )  )
				{
					float fDistance = pWorldDescServer->GetPixelDistance( pPoiObject->GetLocation(),pMultiRegion->GetLocation() );
					fDistance -= pMultiRegion->m_fRegionRadius;
					if( fDistance < fMaxAirRange )
					{
						m_MultiRegionAttackAccess[iMultiRegion1]->SetAirUnitAccess01( 1.0f );
					}
				}
				CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
				if( pUnit && pUnit->GetNumberOfAirUnitsThatICanContain() && pUnit->CanLaunchAirUnits() )
				{
					float fDistance = pWorldDescServer->GetPixelDistance( pPoiObject->GetLocation(),pMultiRegion->GetLocation() );
					fDistance -= pMultiRegion->m_fRegionRadius;
					if( fDistance < fMaxAirRange )
					{
						m_MultiRegionAttackAccess[iMultiRegion1]->SetAirUnitAccess01( 1.0f );
					}
				}
			}
		}
		*/
	}
	Watch3b.Stop();
	Watch4.Start();
	// Calculate the GroundSea Access
	for( long iMultiRegion1=1; iMultiRegion1<iNumberOfMultiRegions+1; iMultiRegion1++ )
	{
		CEOSAIRegionPathwayItem* pPathwayItem = m_PlayerAttackPathwayArrayBasedOnCitRes.GetRegionResult( iMultiRegion1 );
		if( pPathwayItem == NULL ) continue;
		CEOSAIMultiRegion2* pMultiRegion = dynamic_cast< CEOSAIMultiRegion2* >( pPathwayItem->m_pPathfinderPoint );
		pMultiRegion->m_iTempVisitedKey = 0;
	}
	for( long iMultiRegion1=1; iMultiRegion1<iNumberOfMultiRegions+1; iMultiRegion1++ )
	{
		if( m_iPlayer == 2 && iMultiRegion1 == 72 )
		{
			int g=0;
		}

		CEOSAIRegionPathwayItem* pPathwayItem = m_PlayerAttackPathwayArrayBasedOnCitRes.GetRegionResult( iMultiRegion1 );
		if( pPathwayItem == NULL ) continue;

		// Trace the path backwards to it's starting point.
		CList< long >  Path;
		Path.AddHead( pPathwayItem->m_pPathfinderPoint->GetId() );
		while( pPathwayItem->m_iPreviousPathwayItem )
		{
			pPathwayItem = m_PlayerAttackPathwayArrayBasedOnCitRes.GetRegionResult( pPathwayItem->m_iPreviousPathwayItem );
			Path.AddHead( pPathwayItem->m_pPathfinderPoint->GetId() );
		}
		// Now, step forward
		long  iCurrentStep = 1;
		long  iNumberOfSteps = Path.GetCount();
		//long  iPreviousTerrain = 0;
		float fAccumulatedResistance = 0.0f;
		float fGroundSeaAccess01 = 1.0f;
		//bool  bStartsOnCityOrLand = false;
		EnumPath enumPath = EnumPath_CitRes;
		POSITION pos = Path.GetHeadPosition();
		while( pos )
		{
			long iMultiRegion2 = Path.GetNext( pos );
			CEOSAIRegionPathwayItem* pPathwayItem = m_PlayerAttackPathwayArrayBasedOnCitRes.GetRegionResult( iMultiRegion2 );

			// Because of the way I process
			//if( m_MultiRegionAttackAccess[iMultiRegion2]->m_fGroundSeaUnitAccess01 >= 0.0f ){ continue; }

			long iCurrentTerrain = 0;
			if( pPathwayItem->m_pPathfinderPoint->IsCity() ||
				pPathwayItem->m_pPathfinderPoint->IsLand()  ) iCurrentTerrain = TERRAIN_GRASS;
			if( pPathwayItem->m_pPathfinderPoint->IsWater() ) iCurrentTerrain = TERRAIN_WATER;
			ASSERT( iCurrentTerrain != 0 );

			if( iCurrentStep == 1 )
			{
				enumPath = EnumPath_CitRes;
			}
			if( iCurrentStep > 1 )
			{
				if(  enumPath == EnumPath_CitRes )
				{
					if( iCurrentTerrain == TERRAIN_GRASS ){ enumPath = EnumPath_CitRes_Land; }
					if( iCurrentTerrain == TERRAIN_WATER ){ enumPath = EnumPath_CitRes_Water; }
				}
				eif( enumPath == EnumPath_CitRes_Land )
				{
					if( iCurrentTerrain == TERRAIN_GRASS ){ enumPath = EnumPath_CitRes_Land; }
					if( iCurrentTerrain == TERRAIN_WATER ){ enumPath = EnumPath_CitRes_Land_Water; }
				}
				eif( enumPath == EnumPath_CitRes_Water )
				{
					if( iCurrentTerrain == TERRAIN_GRASS ){ enumPath = EnumPath_CitRes_Water_Land; }
					if( iCurrentTerrain == TERRAIN_WATER ){ enumPath = EnumPath_CitRes_Water; }
				}
				eif( enumPath == EnumPath_CitRes_Land_Water )
				{
					if( iCurrentTerrain == TERRAIN_GRASS ){ enumPath = EnumPath_CitRes_Land_Water_Land; }
					if( iCurrentTerrain == TERRAIN_WATER ){ enumPath = EnumPath_CitRes_Land_Water; }
				}
				eif( enumPath == EnumPath_CitRes_Water_Land )
				{
					if( iCurrentTerrain == TERRAIN_GRASS ){ enumPath = EnumPath_CitRes_Water_Land; }
					if( iCurrentTerrain == TERRAIN_WATER ){ enumPath = EnumPath_CitRes_Water_Land_Water; }
				}
				eif( enumPath == EnumPath_CitRes_Land_Water_Land )
				{
					if( iCurrentTerrain == TERRAIN_GRASS ){ enumPath = EnumPath_CitRes_Land_Water_Land; }
					if( iCurrentTerrain == TERRAIN_WATER ){ enumPath = EnumPath_CitRes_Land_Water_Land_Water; }
				}
				eif( enumPath == EnumPath_CitRes_Water_Land_Water )
				{
					if( iCurrentTerrain == TERRAIN_GRASS ){ enumPath = EnumPath_CitRes_Water_Land_Water_Land; }
					if( iCurrentTerrain == TERRAIN_WATER ){ enumPath = EnumPath_CitRes_Water_Land_Water; }
				}
			}

			if(  enumPath == EnumPath_CitRes ) fGroundSeaAccess01 = 1.0f;
			eif( enumPath == EnumPath_CitRes_Land  ) fGroundSeaAccess01 = 1.0f;
			eif( enumPath == EnumPath_CitRes_Water ) fGroundSeaAccess01 = 1.0f;
			eif( enumPath == EnumPath_CitRes_Land_Water ) fGroundSeaAccess01 = 0.5f;//0.5f;
			eif( enumPath == EnumPath_CitRes_Water_Land ) fGroundSeaAccess01 = 1.0f;
			eif( enumPath == EnumPath_CitRes_Land_Water_Land  ) fGroundSeaAccess01 = 0.5f;//0.2f;
			eif( enumPath == EnumPath_CitRes_Water_Land_Water ) fGroundSeaAccess01 = 0.0f;
			eif( enumPath == EnumPath_CitRes_Land_Water_Land_Water ) fGroundSeaAccess01 = 0.0f;
			eif( enumPath == EnumPath_CitRes_Water_Land_Water_Land ) fGroundSeaAccess01 = 0.0f;
			else{ ASSERT( false ); }

			fGroundSeaAccess01 *= GroundSeaDistanceFunc.GetOutput( pPathwayItem->m_fDistance );
			fGroundSeaAccess01 -= ( fAccumulatedResistance / 800.0f );
			//fGroundSeaAccess01 -= ( fAccumulatedVisibility / 500.0f );
			if( fGroundSeaAccess01 < 0.0f ) fGroundSeaAccess01 = 0.0f;

			if( iMultiRegion1 == iMultiRegion2 )
			{
				m_MultiRegionAttackAccess[iMultiRegion2]->SetGroundSeaUnitAccess01( fGroundSeaAccess01 );
			}

			if( iCurrentStep < iNumberOfSteps-1 ) // Ignore the resistence in the last two steps
			{
				float fThisMultiRegionResistance = 0.0f; // Combat Significance
				//if( ( iSteps == 1 && bStartsOnCityOrLand == false ) ||
				//	( iSteps >= 2 ) )
				{
					// Look for units in the area
					CEOSAIMultiRegion2* pMultiRegion = dynamic_cast< CEOSAIMultiRegion2* >( pPathwayItem->m_pPathfinderPoint );
					if( pMultiRegion->m_iTempVisitedKey != iMultiRegion1 )
					{
						pMultiRegion->m_iTempVisitedKey = iMultiRegion1;
						POSITION pos = pMultiRegion->GetPoiObjectsInsideRegion()->GetHeadPosition();
						while( pos )
						{
							CEOSAIPoiObject* pPoiObject = pMultiRegion->GetPoiObjectsInsideRegion()->GetNext( pos );
							if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
								pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Airfield ||
								pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Resource ||
								pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
							{
								long iOwner = pPoiObject->GetOwner();
								EOSAIEnumForeignRelations eRel = pPoiObject->GetForeignRelationsTo( m_iPlayer );
								if( iOwner == 0 || eRel <= EOSAIEnumForeignRelations::enum_Neutral )
								{
									if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
										pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Airfield ||
										pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Resource )
									{
										fThisMultiRegionResistance += 50.0f;
									}
									if( pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
									{
										CEOSAIUnit2* pAIUnit = ( CEOSAIUnit2* ) pPoiObject;
										fThisMultiRegionResistance += pAIUnit->GetAICombatSignificance();
									}
								}
							}
						}
					}
					if( iCurrentStep < iNumberOfSteps-1 )
					{
						POSITION pos3 = pMultiRegion->GetAdjacentRegions()->GetHeadPosition();
						while( pos3 )
						{
							CEOSAIAdjacentRegionDesc* pDesc = pMultiRegion->GetAdjacentRegions()->GetNext( pos3 );
							CEOSAIMultiRegion2* pAdjMultiRegion = (CEOSAIMultiRegion2*) pDesc->m_pRegion;

							if( pAdjMultiRegion->m_iTempVisitedKey != iMultiRegion1 )
							{
								pAdjMultiRegion->m_iTempVisitedKey = iMultiRegion1;
								POSITION pos = pAdjMultiRegion->GetPoiObjectsInsideRegion()->GetHeadPosition();
								while( pos )
								{
									CEOSAIPoiObject* pAIPoiObject = pAdjMultiRegion->GetPoiObjectsInsideRegion()->GetNext( pos );
									if( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
										pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Airfield ||
										pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Resource ||
										pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
									{
										long iOwner = pAIPoiObject->GetOwner();
										EOSAIEnumForeignRelations eRel = pAIPoiObject->GetForeignRelationsTo( m_iPlayer );
										if( iOwner == 0 || eRel <= EOSAIEnumForeignRelations::enum_Neutral )
										{
											if( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City ||
												pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Airfield ||
												pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Resource )
											{
												fThisMultiRegionResistance += 50.0f;
											}
											if( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
											{
												CEOSAIUnit2* pAIUnit = ( CEOSAIUnit2* ) pAIPoiObject;
												fThisMultiRegionResistance += pAIUnit->GetAICombatSignificance();
											}
										}
									}
								}
							}
						}
					}

					//for( long iPlayer=1; iPlayer
					//fAccumulatedVisibility += pMultiRegion->GetPlayerVisibility01();
					/*
					POSITION pos2 = pMultiRegion->m_AdjacentRegions.GetHeadPosition();
					while( pos2 )
					{
						CEOSAIAdjacentRegionDesc* pDesc = pMultiRegion->m_AdjacentRegions.GetNext( pos2 );
						CMultiRegion* pAdjMultiRegion = dynamic_cast< CMultiRegion* >( pDesc->m_pRegion );
						if( pMultiRegion->GetGeoId() != pAdjMultiRegion->GetGeoId() ) continue;

						POSITION pos = pAdjMultiRegion->GetPoiObjectsInsideRegion()->GetHeadPosition();
						while( pos )
						{
							CPoiObject* pPoiObject = pAdjMultiRegion->GetPoiObjectsInsideRegion()->GetNext( pos );
							CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
							if( pUnit )
							{
								if( pUnit->GetOwner() >= 0 &&
									pUnit->GetForeignRelationsTo( m_iPlayer ) <= EOSAIEnumForeignRelations::enum_Neutral )
								{
									fThisMultiRegionResistance += pUnit->GetCombatSignificance();
								}
							}
						}
					}
					*/
				}
				fAccumulatedResistance += fThisMultiRegionResistance;
			}
			iCurrentStep++;
		}
	}
	Watch4.Stop();
	Watch5.Start();
	// m_fSubmarineAccess01
	CEOSAIMathFunction SubmarineDistanceFunc;
	SubmarineDistanceFunc.SetInputOutput(    0.0f, 1.0f );
	SubmarineDistanceFunc.SetInputOutput(  300.0f, 1.0f );
	SubmarineDistanceFunc.SetInputOutput(  600.0f, 0.8f );
	SubmarineDistanceFunc.SetInputOutput( 1000.0f, 0.4f );
	SubmarineDistanceFunc.SetInputOutput( 2000.0f, 0.05f );
	SubmarineDistanceFunc.SetInputOutput( 4000.0f, 0.00f );
	for( long iMultiRegion1=1; iMultiRegion1<iNumberOfMultiRegions+1; iMultiRegion1++ )
	{
		CEOSAIRegionPathwayItem* pPathwayItem = m_SeaPathFromMyCitiesToRestOfWorld.GetRegionResult( iMultiRegion1 );
		if( pPathwayItem )
		{
			// Distance    0 = 1.0
			// Distance  200 = 1.0
			// Distance  400 = 0.66
			// Distance  600 = 0.5
			// Distance  800 = 0.4
			// Distance 1000 = 0.33
			m_MultiRegionAttackAccess[iMultiRegion1]->SetSubmarineAccess01( 
				SubmarineDistanceFunc.GetOutput( pPathwayItem->m_fDistance ) );
			//m_MultiRegionAttackAccess[iMultiRegion1]->m_fSubmarineAccess01 = 
			//	SubmarineDistanceFunc.GetOutput( pPathwayItem->m_fPathfindingValue );
		}
		else
		{
			m_MultiRegionAttackAccess[iMultiRegion1]->SetSubmarineAccess01( 0.0f );
		}
		/*
		// Trace the path backwards to it's starting point.
		CList< long >  Path;
		Path.AddHead( pPathwayItem->m_pPathfinderPoint->GetId() );
		while( pPathwayItem->m_iPreviousPathwayItem )
		{
			pPathwayItem = m_SeaPathFromMyCitiesToRestOfWorld.GetRegionResult( pPathwayItem->m_iPreviousPathwayItem );
			Path.AddHead( pPathwayItem->m_pPathfinderPoint->GetId() );
		}
		// Now, step forward
		long  iSteps = 0;
		//long  iPreviousTerrain = 0;
		float fAccumulatedResistance = 0.0f;
		float fGroundSeaAccess01 = 1.0f;
		//bool  bStartsOnCityOrLand = false;
		EnumPath enumPath = EnumPath_CitRes;
		POSITION pos = Path.GetHeadPosition();
		while( pos )
		{
			long iMultiRegion2 = Path.GetNext( pos );
			CEOSAIRegionPathwayItem* pPathwayItem = m_PlayerAttackPathwayArrayBasedOnCitRes.GetRegionResult( iMultiRegion2 );
			//if( m_MultiRegionAttackAccess[iMultiRegion2]->m_fGroundSeaUnitAccess01 >= 0.0f ){ continue; }

			long iCurrentTerrain = 0;
			if( pPathwayItem->m_pPathfinderPoint->IsCity() ||
				pPathwayItem->m_pPathfinderPoint->IsLand()  ) iCurrentTerrain = TERRAIN_GRASS;
			if( pPathwayItem->m_pPathfinderPoint->IsWater() ) iCurrentTerrain = TERRAIN_WATER;
			ASSERT( iCurrentTerrain != 0 );

			fGroundSeaAccess01 *= GroundSeaDistanceFunc.GetOutput( pPathwayItem->m_fDistance );
			//fGroundSeaAccess01 -= ( fAccumulatedResistance / 200.0f );
			if( fGroundSeaAccess01 < 0.0f ) fGroundSeaAccess01 = 0.0f;

			m_MultiRegionAttackAccess[iMultiRegion2]->m_fGroundSeaUnitAccess01 = fGroundSeaAccess01;
		}
		*/
	}
	Watch5.Stop();

	int h=0;
	//

	// DEBUG
	#ifdef _DEBUG
	//long iNumberOfMultiRegions = m_pMultiRegionManager->GetNumberOfMultiRegions();
	//for( long iPlayer = 1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		//long iArraySize = m_PlayerAttackPathwayArrayBasedOnCitRes[iPlayer]->GetArraySize();
		//for( long iMultiRegion = 1; iMultiRegion<iArraySize; iMultiRegion++ )
		for( long iMultiRegion = 1; iMultiRegion<m_pMultiRegionManager->GetNumberOfMultiRegions(); iMultiRegion++ )
		{
			CEOSAIMultiRegion2* pMultiRegion = m_pMultiRegionManager->GetMultiRegion( iMultiRegion );

			CEOSAIRegionPathwayItem* pMultiRegionResult1 =
				m_PlayerAttackPathwayArrayBasedOnCitRes.GetRegionResult( iMultiRegion );

			CEOSAIRegionPathwayItem* pMultiRegionResult2 =
				m_PlayerAttackPathwayArrayBasedOnCitResUnits.GetRegionResult( iMultiRegion );

			ASSERT( pMultiRegion );

			// This situation can happen in one case: if there is a pond inside a city, then the
			//   [pMultiRegionResult1] and [pMultiRegionResult2] values can be NULL.  This is because
			//   the players can't move through cities owned by other players (e.g. panama canal).
			//ASSERT( pMultiRegionResult1 );
			//ASSERT( pMultiRegionResult2 );
		}
	}
	#endif _DEBUG

	// Setup the PolitePathfinding Array (don't know that I really need this except to quickly find the DDSS)
	//m_PolitePathFromMyCitResUnitsToRestOfWorld.Reset();
	//m_PolitePathFromMyCitResUnitsToRestOfWorld.Initialize( m_PolitePathfindingValuesFromMyCitResUnitsToRestOfWorld.GetCount() + 1 );
	/*
	pos = m_PolitePathfindingValuesFromMyCitResUnitsToRestOfWorld.GetHeadPosition();
	while( pos )
	{
		CMultiRegionPathwayItem* pResult = m_PolitePathfindingValuesFromMyCitResUnitsToRestOfWorld.GetNext( pos );
		m_PolitePathfindingValuesFromMyCitResUnitsToRestOfWorldArray[pResult->m_pMultiRegion->m_iId] = pResult;
	}
	*/
	WatchTotal.Stop();
	float fWatch1     = (float) Watch1.GetDeltaSeconds();
	float fWatch2     = (float) Watch2.GetDeltaSeconds();
	float fWatch3     = (float) Watch3.GetDeltaSeconds();
	float fWatch3b    = (float) Watch3b.GetDeltaSeconds();
	float fWatch4     = (float) Watch4.GetDeltaSeconds();
	float fWatch5     = (float) Watch5.GetDeltaSeconds();
	float fWatchTotal = (float) WatchTotal.GetDeltaSeconds();

	int y=0;
}

/*
void CEOSAIMultiRegionNationwidePathways::CalculateLocalAndPathwayAIPoiObjectsForEachMultiRegion()
{
	/-*
	POSITION pos = m_pMultiRegionManager->m_MultiRegionList.GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pMultiRegion = m_pMultiRegionManager->m_MultiRegionList.GetNext( pos );
		pMultiRegion->CalculateLocalAndPathwayAIPoiObjects();
	}
	*-/
}
*/
void CEOSAIMultiRegionNationwidePathways::CalculatePathwayPoiObjectsForEachMultiRegion()
{
//	CEOSAIStopwatch2 watch;
//	watch.Start();

	m_PolitePathFromMyCitResUnitsToRestOfWorld.CalculatePathwayPoiObjectsForEachRegion();
	m_DistGeoPathFromMyCitResUnitsToRestOfWorld.CalculatePathwayPoiObjectsForEachRegion();
	m_DistGeoPathFromMyCitResToRestOfWorld.CalculatePathwayPoiObjectsForEachRegion();
	m_MinimumGeoSwitchPathwayFromMyCitResUnitsToRestOfWorld.CalculatePathwayPoiObjectsForEachRegion();
	//m_LandPathFromMyCitGroundUnitToRestOfWorld.CalculatePathwayPoiObjectsForEachRegion();
	m_SeaPathFromMyCitiesToRestOfWorld.CalculatePathwayPoiObjectsForEachRegion();

	m_PlayerAttackPathwayArrayBasedOnCitResUnits.CalculatePathwayPoiObjectsForEachRegion();
	m_PlayerAttackPathwayArrayBasedOnCitRes.CalculatePathwayPoiObjectsForEachRegion();

//	watch.Stop();
//	CString strText; strText.Format( _T("CEOSAIMultiRegionNationwidePathways::CalculatePathwayPoiObjectsForEachMultiRegion - time: %f"), watch.GetDeltaSeconds() );
//	Checkpoint::Write( strText );
/*
	CWorldDescServer* pWorldDescServer = m_pMultiRegionManager->GetWorldDescServer();

	POSITION pos = m_pMultiRegionManager->GetMultiRegionList()->GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pMultiRegion = m_pMultiRegionManager->GetMultiRegionList()->GetNext( pos );

		//pMultiRegion->m_PoiObjectsWithin200Pixels.RemoveAll();
		pMultiRegion->m_PathwayPoiObjects.RemoveAll();

		CEOSAILocation MultiRegionCenterLocation = pMultiRegion->GetLocation();

		// Add all AIPoi in the MultiRegion
		/-*
		POSITION pos = pMultiRegion->m_PoiObjects.GetHeadPosition();
		while( pos )
		{
			CPoiObject* pPoiObject = pMultiRegion->m_PoiObjects.GetNext( pos );
			pMultiRegion->m_PoiObjectsWithin200Pixels.AddTail( pPoiObject );
		}

		// Assume that units within 150 pixels or RadiusOfRegions+50 of the Location will act 
		//   as local resistence
		//
		//CWorldDescPlayer* pWorldDescPlayer = dynamic_cast< CWorldDescPlayer* >( m_pWorldDesc );
		float fCurrentRadiusCheck = 200.0f;
		//float fRadius = max( 150.0f, m_fRadiusOfRegions + 50.0f );
		//CEOSAIBrain* pAIBrain = m_pMultiRegionManager->GetAIBrain();
		pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
		while( pos )
		{
			//CEOSAIPoiObject* pAIPoiObject = pAIBrain->GetAIPoiObjectList()->GetNext( pos );
			CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
			CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
			//
			if( pPoiObject && pPoiObject->IsCityResourceAirfieldOrUnit() )
			{
				float fDistance = pWorldDescServer->GetPixelDistance( pPoiObject->GetLocation(), MultiRegionCenterLocation );
				if( fDistance < fCurrentRadiusCheck )
				{
					bool bExistsInTheLocalList = false;
					POSITION pos2 = pMultiRegion->m_PoiObjectsWithin200Pixels.GetHeadPosition();
					while( pos2 )
					{
						CPoiObject* pExistingTarget = pMultiRegion->m_PoiObjectsWithin200Pixels.GetNext( pos2 );
						if( pPoiObject == pExistingTarget )
						{
							bExistsInTheLocalList = true;
							break;
						}
					}
					if( bExistsInTheLocalList == false )
					{
						// I'm using this method because I should catch all the children this way anyway
						pMultiRegion->m_PoiObjectsWithin200Pixels.AddTail( pPoiObject );
					}
				}
			}
		}
		*-/

		// Look at the pathway, add-up the expected resistence
		//
		float fCurrentRadiusCheck = 200.0f;
		//long iMyPlayer = pAIBrain->GetAIPlayerNumber();
		//CEOSAIMultiRegionManager2* pMultiRegionManager = pAIBrain->GetMultiRegionManager();
		//CMultiRegionPathwayResult* pMultiRegionPathwayResult = GetPlayerAttackPathwayBasedOnCitRes( m_iPlayer );
		//CMultiRegion* pMultiRegion = this;
		//if( pMultiRegionPathwayResult && pMultiRegion )
		{
			long iCurrentMultiRegion = pMultiRegion->m_iId;
			//CMultiRegionPathwayItem* pMultiRegionResult = pMultiRegionPathwayResult->GetMultiRegionResult( iCurrentMultiRegion );
			CMultiRegionPathwayItem* pMultiRegionResult = GetPlayerAttackPathwayBasedOnCitRes( iCurrentMultiRegion );
			iCurrentMultiRegion = pMultiRegionResult->m_iPreviousMultiRegion;
			while( iCurrentMultiRegion != 0 )
			{
				CMultiRegionPathwayItem* pMultiRegionResult =
					GetPlayerAttackPathwayBasedOnCitRes( iCurrentMultiRegion );
				ASSERT( pMultiRegionResult );
				//
				CMultiRegion* pCurrentMultiRegion = pMultiRegionResult->m_pMultiRegion;
				CEOSAILocation CurrentMultiRegionLocation = pCurrentMultiRegion->GetLocation();
				//
				//POSITION pos = pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
				POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
				while( pos )
				{
					//CEOSAIPoiObject* pAIPoiObject = pAIBrain->GetAIPoiObjectList()->GetNext( pos );
					//
					CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
					CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
					if( pPoiObject && pPoiObject->IsCityResourceAirfieldOrUnit() )
					{
						float fDistance = pWorldDescServer->GetPixelDistance( pPoiObject->GetLocation(), CurrentMultiRegionLocation );
						if( fDistance < fCurrentRadiusCheck )
						{
							bool bExistsInTheList = false;
							POSITION pos2 = pMultiRegion->m_PoiObjectsWithin200Pixels.GetHeadPosition();
							while( pos2 )
							{
								CPoiObject* pExistingTarget = pMultiRegion->m_PoiObjectsWithin200Pixels.GetNext( pos2 );
								if( pPoiObject == pExistingTarget )
								{
									bExistsInTheList = true;
									break;
								}
							}
							if( bExistsInTheList == false )
							{
								pos2 = pMultiRegion->m_PathwayPoiObjects.GetHeadPosition();
								while( pos2 )
								{
									CPoiObject* pExistingTarget = pMultiRegion->m_PathwayPoiObjects.GetNext( pos2 );
									if( pPoiObject == pExistingTarget )
									{
										bExistsInTheList = true;
										break;
									}
								}
							}
							if( bExistsInTheList == false )
							{
								// I'm using this method because I should catch all the children this way anyway
								//pMultiRegion->m_PathwayPoiObjects.AddTail( pAIPoiObject->GetPoiObject() );
								pMultiRegion->m_PathwayPoiObjects.AddTail( pPoiObject );
							}
						}
					}
				}

				iCurrentMultiRegion = pMultiRegionResult->m_iPreviousMultiRegion;

				fCurrentRadiusCheck += 25.0f;
				if( fCurrentRadiusCheck > 250.0f ){ fCurrentRadiusCheck = 250.0f; }
			}
		}
	}
	*/
}

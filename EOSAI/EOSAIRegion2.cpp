
#include "stdafx.h"
#include "EOSAIRegion2.h"
#include "EOSAIBrain.h"
//#include "AIDefinedAction_Explore.h"
#include "EOSAIMovementDescription.h"
#include "AIPlayer.h"
//#include "WorldDescServer.h"
//#include "WorldDescPlayer.h"
#include "EOSAIGamePlayer.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIRegion2::CEOSAIRegion2( CEOSAIGenericRegionManager* pRegionManager, CEOSAILocation Location, long iGeo, long iId )
	: CEOSAIGenericRegion( pRegionManager, iId )
{
	m_fRegionRadius = 50.0f;
	SetLocation( Location );
	//SetLocationAndGeo( Location );

	m_pAICity = NULL;
	m_iRegionSize = 0;
	m_iMultiRegionId = 0;
}

CEOSAIRegion2::~CEOSAIRegion2()
{
	CEOSAIRegion2::ClearPoiData();

	m_pAICity = NULL;
	m_iRegionSize = 0;
	m_iMultiRegionId = 0;

	// Clear-out the AIRoads
	m_RoadsOnTheRegionEdge.RemoveAll();
}


void CEOSAIRegion2::ClearGroundUnitAndSeaTransportPathToEverywhere()
{
	m_GroundUnitAndSeaTransportPathToEverywhere.Clear();
}

// Use a typical ground unit and typical sea-transport for this calculation
CEOSAIRegionPathwayMap* CEOSAIRegion2::InvokeGroundUnitAndSeaTransportPathToEverywhere( CEOSAIBrain* pAIBrain )
{
	if( m_GroundUnitAndSeaTransportPathToEverywhere.GetArraySize() == 0 )
	{
		pAIBrain->m_GetTransportAssistedMovementPath_Count++;

		CEOSAIMovementDescription  GroundUnitMovementDesc;
		CEOSAIMovementDescription  SeaTransportMovementDesc;

		pAIBrain->GetAIThoughtDatabase()->GetTypicalGroundUnitMovementDescription( &GroundUnitMovementDesc );
		pAIBrain->GetAIThoughtDatabase()->GetTypicalSeaTransportMovementDescription( &SeaTransportMovementDesc );

		CEOSAITerrainBasedValue TerrainBasedMovement;
		for( long iTerrain=0; iTerrain<TERRAIN_COUNT; iTerrain++ )
		{
			TerrainBasedMovement.m_fTerrainValue[iTerrain] = 
				GroundUnitMovementDesc.m_fMovementRate * 
				GroundUnitMovementDesc.m_MovementRateMultiplier.m_fTerrainValue[iTerrain];
		}

		/*
		//float fTransportMovementRate = pTransportUnitTemplate->GetMovementRate();
		//CTerrainBasedValue MovementDesc;
		//MovementDesc.Set( m_pUnitTemplate );
		TerrainBasedMovement.m_fTerrainValue[TERRAIN_COAST] = 
			SeaTransportMovementDesc.m_MovementRateMultiplier[TERRAIN_COAST];
		//MovementDesc.m_fTerrainValue[TERRAIN_WATER] = fTransportMovementRate;
		TerrainBasedMovement.m_fTerrainValue[TERRAIN_WATER] = 
			SeaTransportMovementDesc.m_MovementRateMultiplier[TERRAIN_WATER];
		*/
		TerrainBasedMovement.m_fTerrainValue[TERRAIN_COAST] = 
			SeaTransportMovementDesc.m_fMovementRate * 
			SeaTransportMovementDesc.m_MovementRateMultiplier.m_fTerrainValue[TERRAIN_COAST];
		TerrainBasedMovement.m_fTerrainValue[TERRAIN_WATER] = 
			SeaTransportMovementDesc.m_fMovementRate * 
			SeaTransportMovementDesc.m_MovementRateMultiplier.m_fTerrainValue[TERRAIN_WATER];

		long iPlayer = pAIBrain->GetAIPlayer()->GetPlayerNumber();

		//CEOSAIRegionPathwayMap  TargetToEverywhereMap;
		CEOSAIStopwatch2 Timer1;
		Timer1.Start();
		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iPlayer );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetMaximumGeoSwitches( 1 );
		RegionPathfinder.SetMovementRate( &TerrainBasedMovement );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iPlayer );
		//RegionPathfinder.AddStartLocation( TargetLocation );
		RegionPathfinder.AddStartLocation( this->GetLocation() );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_GroundUnitAndSeaTransportPathToEverywhere );
		Timer1.Stop();
		int g=0;

		/*
		// Multiregions
		CEOSAIRegionPathfinder& RegionPathfinder = pAIBrain->GetMultiRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iPlayer );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetMaximumGeoSwitches( 1 );
		RegionPathfinder.SetMovementRate( &TerrainBasedMovement );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iPlayer );
		//RegionPathfinder.AddStartLocation( TargetLocation );
		RegionPathfinder.AddStartLocation( this->GetLocation() );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, m_MultiRegion_GroundUnitAndSeaTransportPathToEverywhere );
		float f2 = Timer2.GetDeltaSeconds();
		int g2=0;
		*/
	}
	return &m_GroundUnitAndSeaTransportPathToEverywhere;
}

//void CEOSAIRegion2::SetLocationAndGeo( CEOSAILocation location )
void CEOSAIRegion2::SetLocation( CEOSAILocation location )
{
	m_Location = location;
	//m_iGeo = GetCommonState()->GetWorldDescServer()->GetGeoNumber( m_Location );
	//m_iGeo = g_pEOSAICommonData->GetGeoId( m_Location );
	//ASSERT( m_iGeo > 0 );
}


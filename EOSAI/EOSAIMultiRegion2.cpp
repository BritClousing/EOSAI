
#include <stdafx.h>
#include "EOSAIMultiRegion2.h"
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIMovementDescription.h"
#include "EOSAIRegion2.h"
#include "EOSAIBrain.h"
//#include "WorldDesc.h"
#include "PoiObject.h"
#include "EOSAIUnit2.h"
#include "City.h"
#include "EOSAIResource.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"
#include "EOSAIGamePlayer.h"
//#include "AIThought_ShouldSendUnitsToCaptureTheLandCitRes.h"
//#include "AIThought_WhatUnitsCanIGetToMultiRegion.h"
//#include "AIThought_ShouldSendUnitsHere.h"
#include "EOSAIResource.h"
//#include "9MovementPathwaysFromEverywhereToMRX.h"
#include "EOSAIRegionPathfinder.h"
//#include "AIUnitProximity.h"
#include "EOSAICommonData.h"
#include "EOSAIMathFunction.h"
#include "EOSAIInterface.h"

//#include "AISeaPathways_EverywhereMRX.h"
//#include "AIMultiRegionPathwayScoresForTransports.h"

#define MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER 125.0f

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CEOSAIMultiRegion2::CEOSAIMultiRegion2( CEOSAIMultiRegionManager2* pMultiRegionManager, CEOSAIRegion2* pCenterAIRegion, long iId )
	: CEOSAIGenericRegion( pMultiRegionManager, iId )
{
	//
	m_pMultiRegionManager = pMultiRegionManager;
	m_pCenterAIRegion = pCenterAIRegion;
	m_iGeo = pCenterAIRegion->m_iGeo;
	if( pCenterAIRegion->m_iGeo == 0 ){ ASSERT(pCenterAIRegion->GetRegionSize() == 1); }

	m_Location = pCenterAIRegion->GetLocation();
	m_iCityObjectId = pCenterAIRegion->m_iCityObjectId;//pCenterRegion->GetEOSAICity();
	//m_pAICity = NULL;//pCenterRegion->GetEOSAICity();
	m_iTerrainType3 = TERRAIN_UNKNOWN;
	m_iTerrainType9 = TERRAIN_UNKNOWN;
	//m_fMaximumRadius = 125.0f;
	m_fRegionRadius = MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER;

	// Add it to the MultiRegion list
	m_pCenterAIRegion->m_iMultiRegionId = iId;
	m_AIRegions.AddTail( pCenterAIRegion );

	//
	if( m_pCenterAIRegion->m_iTerrainType9 == TERRAIN_WATER || 
		m_pCenterAIRegion->m_iTerrainType9 == TERRAIN_COAST )
	{
		m_iTerrainType3 = TERRAIN_WATER;
		m_iTerrainType9 = TERRAIN_WATER;
		//m_iTerrainType_LandWaterCity = 2;
	}
	else if( m_pCenterAIRegion->m_iTerrainType9 == TERRAIN_CITY )
	{
		m_iTerrainType3 = TERRAIN_CITY;
		m_iTerrainType9 = TERRAIN_CITY;
		//m_iTerrainType_LandWaterCity = 3;
	}
	else if( m_pCenterAIRegion->m_iTerrainType9 == TERRAIN_UNKNOWN )
	{
		m_iTerrainType3 = TERRAIN_UNKNOWN;
		m_iTerrainType9 = TERRAIN_UNKNOWN;
		ASSERT( false );
	}
	else
	{
		m_iTerrainType3 = TERRAIN_GRASS;
		m_iTerrainType9 = TERRAIN_GRASS;
		//m_iTerrainType_LandWaterCity = 1;
	}

	for( long iPlayer=0; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS+1; iPlayer++ )
	{
		m_OwnerControl01[ iPlayer ] = 0.0f;
	}

	//m_iOwner = 0;
	//m_fOwnerControl = 0.0f; // 0.0 - 1.0

	//long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	//m_PlayerCitResUnitSummaries.SetSize( iNumberOfPlayers+1 );
	//m_OverflowVisibility.SetSize( iNumberOfPlayers+1 ); // 0-1
	//m_CrowsDistanceFromPlayerCitResUnits.SetSize( iNumberOfPlayers+1 );
	/*
	m_CrowsDistanceFromPlayerCities.SetSize( iNumberOfPlayers+1 );
	m_CrowsDistanceFromPlayerResources.SetSize( iNumberOfPlayers+1 );
	m_CrowsDistanceFromPlayerGroundUnits.SetSize( iNumberOfPlayers+1 );
	m_CrowsDistanceFromPlayerSeaUnits.SetSize( iNumberOfPlayers+1 );
	m_CrowsDistanceFromPlayerAircraft.SetSize( iNumberOfPlayers+1 );
	m_CrowsDistanceFromPlayerMissiles.SetSize( iNumberOfPlayers+1 );
	*/
	//m_PlayerHasGroundUnitsOnThisGeo.SetSize( iNumberOfPlayers+1 );
	//m_PlayerHasCitiesOnThisGeo.SetSize( iNumberOfPlayers+1 );


	//m_pPathwayToEverywhereResult = NULL;

	//
		//m_pPathwaysToMultiRegion = NULL;
		//m_pWhatUnitsCanIGetToMultiRegion = NULL;

	// Thoughts
	//
		//m_pAIThought_ShouldSendUnitsToCaptureTheLandCitRes = NULL;
		//m_pAIThought_ShouldSendUnitsToDegradePlayerCities = NULL;
		//m_pAIThought_ShouldSendUnitsToAttackHisUnits = NULL;
		//m_pAIThought_ShouldSendUnitsHere = NULL;

	//
		m_iTempVisitedKey = 0;
		m_iTempEndKey = 0;
}

CEOSAIMultiRegion2::~CEOSAIMultiRegion2()
{
	//while( m_TransportProximityList.IsEmpty() == FALSE ){ delete m_TransportProximityList.RemoveHead(); }
	//delete m_pPathwaysToMultiRegion;
	/*
	if( m_pPathwayToEverywhereResult )
	{
		delete m_pPathwayToEverywhereResult;
		m_pPathwayToEverywhereResult = NULL;
	}
	*/
}

/*
CEOSAILocation CMultiRegion::GetLocation()
{
	return m_pCenterRegion->GetLocation();
}
*/
void  CEOSAIMultiRegion2::GrowRegion()
{
	long iNumberOfPlayers = g_pEOSAIInterface->GetNumberOfGamePlayers();

	// I use the spatial map for some distance calculations
	CEOSAIMovementDescription  MovementDesc;
	if( m_iTerrainType3 == TERRAIN_UNKNOWN )
	{
		ASSERT( false );
		MovementDesc.m_eMovementType = EOSAIEnumMovementType3_Land;
		MovementDesc.m_fMovementRate = 1.0f;
		MovementDesc.m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_UNKNOWN ] = 1.0f;
	}
	if( m_iTerrainType3 == TERRAIN_GRASS )
	{
		MovementDesc.SetToGenericGroundUnit();
	}
	if( m_iTerrainType3 == TERRAIN_WATER )
	{
		MovementDesc.SetToGenericSeaUnit();
	}

	//CWorldDesc* pWorldDesc = GetCommonState()->GetWorldDescServer();
	CEOSAIRegionManager2* pAISpatialMap = m_pMultiRegionManager->GetAIRegionManager();

	CList< CEOSAIRegion2* >  AdjacentRegionsWithinRange;
	AdjacentRegionsWithinRange.AddTail( m_pCenterAIRegion );

	// Add AIRegions to the Multiregions - this is based on distance
	while( AdjacentRegionsWithinRange.IsEmpty() == FALSE )
	{
		CEOSAIRegion2* pAIRegion = AdjacentRegionsWithinRange.RemoveHead();

		// Iterate over that region's adjacent regions
		POSITION pos = pAIRegion->m_AdjacentRegions.GetHeadPosition();
		while( pos )
		{
			//CAIAdjacentRegionDistance* pDist = pAIRegion->m_AdjacentRegions.GetNext( pos );
			//CEOSAIRegion2* pNextRegion = pDist->m_pRegion;
			CEOSAIAdjacentRegionDesc* pNextAdjRegionDesc = pAIRegion->m_AdjacentRegions.GetNext( pos );
			CEOSAIRegion2* pNextRegion = dynamic_cast< CEOSAIRegion2* >( pNextAdjRegionDesc->m_pRegion );//pAIRegion->m_AdjacentRegions.GetNext( pos );

			// Is the MultiRegion on the same geo as the AIRegion?
			if( pNextRegion->m_iGeo != this->m_iGeo ) continue;

			// Is the region already in the MultiRegion list?
			if( pNextRegion->m_iMultiRegionId == m_iId ) continue; // yes, skip it

			// Rather than Crows-distance, I might want to use spatial distance
			//   Spatial distance doesn't wrap around islands so much
			// Is the region within range?
			//float fDistance = pWorldDesc->GetPixelDistance( pNextRegion->GetLocation(), m_pCenterRegion->GetLocation() );
			float fDistance = g_pWorldDistanceTool->GetDistance( pNextRegion->GetLocation(), m_pCenterAIRegion->GetLocation() );
			if( fDistance > MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER )
			{
				continue; // nope
			}

			// Is the region the correct terrain?
			long iNextRegionTerrainType3 = TERRAIN_UNKNOWN;
			if( pNextRegion->m_iTerrainType9 == TERRAIN_WATER || 
				pNextRegion->m_iTerrainType9 == TERRAIN_COAST )
			{
				iNextRegionTerrainType3 = TERRAIN_WATER;
			}
			else if( pNextRegion->m_iTerrainType9 == TERRAIN_UNKNOWN )
			{
				iNextRegionTerrainType3 = TERRAIN_UNKNOWN;
			}
			else
			{
				iNextRegionTerrainType3 = TERRAIN_GRASS;
			}

			//
			if( iNextRegionTerrainType3 != m_iTerrainType3 )
			{
				continue;  // wrong terrain type
			}

			// If this region already belongs to a MultiRegion, consider stealing it
			/*
			if( pNextRegion->m_iMultiRegionId > 0 &&
				pNextRegion->m_iMultiRegionId != m_pCenterRegion->m_iMultiRegionId )
			{
				CMultiRegion* pOtherMultiRegion = m_pMultiRegionManager->GetMultiRegion( pNextRegion->m_iMultiRegionId );
				ASSERT( pOtherMultiRegion );

				float fDistanceFromOtherMultiRegion = pWorldDesc->GetPixelDistance( pNextRegion->GetInitialLocation(), pOtherMultiRegion->GetLocation() );
				if( fDistanceFromOtherMultiRegion < fDistance )
				{
					continue; // nope, the other Multiregion keeps it
				}
				else
				{
					// steal it
					pOtherMultiRegion->RemoveRegion( pNextRegion );
				}
			}
			*/
			if( pNextRegion->m_iMultiRegionId > 0 &&
				pNextRegion->m_iMultiRegionId != m_pCenterAIRegion->m_iMultiRegionId )
			{
				CEOSAIMultiRegion2* pOtherMultiRegion = m_pMultiRegionManager->GetMultiRegion( pNextRegion->m_iMultiRegionId );
				ASSERT( pOtherMultiRegion );

				if( pOtherMultiRegion->m_pCenterAIRegion == pNextRegion )
				{
					continue; // can't steal the center AIRegion from another MultiRegion
				}

				//CEOSAIRegionPathfinder& RegionPathfinder = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetAIRegionManager()->GetPathfinder();
				CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
				RegionPathfinder.SetNumberOfPlayers( iNumberOfPlayers ); // Brit-12/30
				CEOSAIRegionPathwayMap  RegionPathwayMap;
				//
				RegionPathfinder.ClearPathfindingSettings();
				RegionPathfinder.AddStartLocation( this->GetLocation() );
				RegionPathfinder.AddEndLocation( pNextRegion->GetLocation() );
				RegionPathfinder.SetMaximumGeoSwitches( 0 );
				RegionPathfinder.SetMaximumDistance( MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER+20 );
				RegionPathfinder.Solve( 0,false );
				RegionPathfinder.GetResult( true, &RegionPathwayMap );
				float fThisSpatialDistance = RegionPathwayMap.GetShortestStartToEndDistance();
				//
				RegionPathfinder.ClearPathfindingSettings();
				RegionPathfinder.AddStartLocation( pOtherMultiRegion->GetLocation() );
				RegionPathfinder.AddEndLocation( pNextRegion->GetLocation() );
				RegionPathfinder.SetMaximumGeoSwitches( 0 );
				RegionPathfinder.SetMaximumDistance( MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER+20 );
				RegionPathfinder.Solve( 0,false );
				RegionPathfinder.GetResult( true, &RegionPathwayMap );
				float fOtherSpatialDistance = RegionPathwayMap.GetShortestStartToEndDistance();

				/*
				float fThisSpatialDistance = pAISpatialMap->GetTimeToLocation(
					this->GetLocation(), pNextRegion->GetLocation(), 0.0f,
					&MovementDesc, NULL );
				float fOtherSpatialDistance = pAISpatialMap->GetTimeToLocation(
					pOtherMultiRegion->GetLocation(), pNextRegion->GetLocation(), 0.0f,
					&MovementDesc, NULL );
				*/

				if( fThisSpatialDistance < fOtherSpatialDistance )
				{
					// steal it
					pOtherMultiRegion->RemoveRegion( pNextRegion );
				}
				else
				{
					continue; // nope, the other Multiregion keeps it
				}
				/*
				float fDistanceFromOtherMultiRegion = pWorldDesc->GetPixelDistance( pNextRegion->GetInitialLocation(), pOtherMultiRegion->GetLocation() );
				if( fDistanceFromOtherMultiRegion < fDistance )
				{
					continue; // nope, the other Multiregion keeps it
				}
				else
				{
					// steal it
					pOtherMultiRegion->RemoveRegion( pNextRegion );
				}
				*/
			}

			// Passed all the tests, add it
			pNextRegion->m_iMultiRegionId = m_iId;
			m_AIRegions.AddTail( pNextRegion );
			AdjacentRegionsWithinRange.AddTail( pNextRegion );
		}
	}
}

void CEOSAIMultiRegion2::ReCenter()
{
	CEOSAIRegion2* pMostCentralRegion = NULL;
	float fLargestDistance = 10000.0f;

	float fOldDistance = 0.0f;

	//CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();
	//CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetWorldDescServer();

	// Read over the MultiRegions, find the most central one
	POSITION pos1 = m_AIRegions.GetHeadPosition();
	while( pos1 )
	{
		CEOSAIRegion2* pAIRegion1 = m_AIRegions.GetNext( pos1 );

		float fDistanceFromRegion1ToAllOtherRegions = 0.0f;
		POSITION pos2 = m_AIRegions.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIRegion2* pAIRegion2 = m_AIRegions.GetNext( pos2 );

			if( pAIRegion1 == pAIRegion2 ) continue;

			//float fDistance = pWorldDesc->GetPixelDistance( pAIRegion1->GetLocation(), pAIRegion2->GetLocation() );
			float fDistance = g_pWorldDistanceTool->GetDistance( pAIRegion1->GetLocation(), pAIRegion2->GetLocation() );
			if( fDistance > fDistanceFromRegion1ToAllOtherRegions )
			{
				fDistanceFromRegion1ToAllOtherRegions = fDistance;
			}
		}

		if( pAIRegion1 == m_pCenterAIRegion )
		{
			fOldDistance = fDistanceFromRegion1ToAllOtherRegions;
		}

		if( fLargestDistance > fDistanceFromRegion1ToAllOtherRegions )
		{
			pMostCentralRegion = pAIRegion1;
			fLargestDistance = fDistanceFromRegion1ToAllOtherRegions;
		}
	}
	if( pMostCentralRegion != m_pCenterAIRegion )
	{
		int x=0; // for debugging
	}
	m_fRegionRadius = fLargestDistance + (12.0f*4.0f);
	m_pCenterAIRegion = pMostCentralRegion;
	m_Location = m_pCenterAIRegion->GetLocation();
}

void CEOSAIMultiRegion2::RemoveRegion( CEOSAIRegion2* pAIRegion )
{
	POSITION pos = m_AIRegions.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIRegion2* pAIRegionInList = m_AIRegions.GetNext( pos );
		if( pAIRegionInList == pAIRegion )
		{
			m_AIRegions.RemoveAt( prevPos );
			return;
		}
	}
}

long CEOSAIMultiRegion2::GetSize()
{
	long iSize = 0;
	POSITION pos = m_AIRegions.GetHeadPosition();
	while( pos )
	{
		CEOSAIRegion2* pAIRegion = m_AIRegions.GetNext( pos );
		iSize += pAIRegion->GetRegionSize();
	}
	return iSize;
}

/*
void CMultiRegion::ClearAIPoiData()
{
	while( m_TransportProximityList.IsEmpty() == FALSE ){ delete m_TransportProximityList.RemoveHead(); }
}
*/
/*
void CMultiRegion::ClearPoiData()
{
	//while( m_TransportProximityList.IsEmpty() == FALSE ){ delete m_TransportProximityList.RemoveHead(); }
	//for( long i=0; i<m_CrowsDistanceFromPlayerCitResUnits.m_iSize; i++ )
	//{
		//m_CrowsDistanceFromPlayerCitResUnits[i] = 10e6;
		//m_PlayerHasGroundUnitsOnThisGeo[i] = false;
		//m_PlayerHasCitiesOnThisGeo[i] = false;
	//}
	//m_PoiObjectsWithin200Pixels.RemoveAll();
	CEOSAIGenericRegion::ClearPoiData();
}
*/

float CEOSAIMultiRegion2::GetDanger01( long iPlayerViewpoint )
{
	//CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
	//ASSERT( pWorldDescServer );
	//if( pWorldDescServer )
	{
		float fOwnership01 = 0.0f;
		//long iNumberOfPlayers = pWorldDescServer->GetWorldBuildDesc()->GetNumberOfPlayers();
		long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayerViewpoint, iPlayer );
			if( eRel == EOSAIEnumForeignRelations::enum_Neutral ||
				eRel == EOSAIEnumForeignRelations::enum_War )
			{
				float fPlayerNOwnership = m_OwnerControl01[iPlayer];
				fOwnership01 += fPlayerNOwnership;
			}
		}
		if( fOwnership01 > 0.0f )
		{
			ASSERT( fOwnership01 >= 0.0f && fOwnership01 <= 1.0f );
			fOwnership01 = max( 0.0f, fOwnership01 );
			fOwnership01 = min( 1.0f, fOwnership01 );

			CEOSAIMathFunction MathFunc;
			MathFunc.SetInputOutput( 0.0f,0.0f );
			MathFunc.SetInputOutput( 0.5f,1.0f );
			return MathFunc.GetOutput( fOwnership01 );
		}
		else
		{
			return 0.0f;
		}
	}
	ASSERT( false );
	return 0.0f;
}

// 1.0=Deep in my territory, 0.5=No man's land, 0.0=Deep in enemy territory
float CEOSAIMultiRegion2::GetSATOwnershipLevel01( long iPlayerViewpoint )
{
	//CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
	//ASSERT( pWorldDescServer );
	//if( pWorldDescServer )
	{
		// Lookup the foreign relations
		float fOwnership01 = 0.0f;
		float fOwnershipWeight = 0.0f;
		long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		for( long iPlayer=0; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			//float fMultiplier = 1.0f;
			if( iPlayer == 0 )
			{
				float fPlayer0Ownership = m_OwnerControl01[0];
				fOwnership01 += 0.5f*fPlayer0Ownership;
				fOwnershipWeight += fPlayer0Ownership;
			}
			else
			{
				EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayerViewpoint, iPlayer );
				if( eRel == EOSAIEnumForeignRelations::enum_Self ||
					eRel == EOSAIEnumForeignRelations::enum_Teammate || 
					eRel == EOSAIEnumForeignRelations::enum_Alliance )
				{
					float fPlayerNOwnership = m_OwnerControl01[iPlayer];
					fOwnership01 += 1.0f*fPlayerNOwnership;//0.5f - 0.5f*fPlayerNOwnership;
					fOwnershipWeight += fPlayerNOwnership;
				}
				else
				{
					ASSERT( eRel == EOSAIEnumForeignRelations::enum_Neutral ||
							eRel == EOSAIEnumForeignRelations::enum_War );
					float fPlayerNOwnership = m_OwnerControl01[iPlayer];
					fOwnership01 += 0.0f*fPlayerNOwnership;//0.5f + 0.5f*fPlayerNOwnership;
					fOwnershipWeight += fPlayerNOwnership;
				}
			}
		}
		if( fOwnershipWeight > 0.0f )
		{
			return fOwnership01 / fOwnershipWeight;
		}
		else
		{
			return 0.5f;
		}
	}
	ASSERT( false );
	return 0.0f;
}

CEOSAIRegionPathwayMap* CEOSAIMultiRegion2::InvokePathToEverywhere()
{
	if( m_PathwayToEverywhereResult.GetArraySize() == 0 )
	//if( m_PathwayToEverywhereResult.GetArraySize() == 0 )
	{
		//m_pPathwayToEverywhereResult = new CEOSAIRegionPathwayMap();

		//CWorldDesc* pWorldDesc = m_pAIBrain->GetWorldDesc();
		//CEOSAIMultiRegionManager2* pMultiRegionManager = m_pAIBrain->GetMultiRegionManager();
		//CEOSAIMultiRegionManager2* pMultiRegionManager = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager();
		//long iPlayer = m_pAIBrain->GetAIPlayerNumber();
		//CMultiRegion* pStartMultiRegion = m_pMultiRegionManager->GetMultiRegion( GetLocation() );

		// Timer test
		//CEOSAIStopwatch2  Stopwatch1a, Stopwatch1b, Stopwatch2;
		//Stopwatch1a.Start();

		// Make the "target" the start location (I'll check the end locations later)
		CEOSAIRegionPathfinder& RegionPathfinder = m_pMultiRegionManager->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( 0 );
		RegionPathfinder.SetDistanceMultiplier( 1.0f );
		RegionPathfinder.SetGeoSwitchMultiplier( 200.0f );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( 0 );
		RegionPathfinder.AddStartingRegion( this );//pStartMultiRegion );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		//Stopwatch1a.Stop();
		//Stopwatch1b.Start();
		RegionPathfinder.Solve3();//.Solve( iPlayer, true );
		RegionPathfinder.GetResult( true, &m_PathwayToEverywhereResult );
		//Stopwatch1b.Stop();
	}
	return &m_PathwayToEverywhereResult;
}


// Use a typical ground unit and typical sea-transport for this calculation
CEOSAIRegionPathwayMap* CEOSAIMultiRegion2::InvokeGroundUnitAndSeaTransportPathToEverywhere( CEOSAIBrain* pAIBrain )
{
	if( m_GroundUnitAndSeaTransportPathToEverywhere.GetArraySize() == 0 )
	{
		//pAIBrain->m_GetTransportAssistedMovementPath_Count++;

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

		//long iPlayer = pAIBrain->GetAIPlayer()->GetPlayerNumber();

		/*
		//CEOSAIRegionPathwayMap  TargetToEverywhereMap;
		CEOSAIStopwatch2 Timer1;
		Timer1.Start();
		CEOSAIRegionPathfinder& RegionPathfinder = pAIBrain->GetAIRegionManager()->GetPathfinder();
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
		*/

		// Multiregions
		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetMultiRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( 0 );//iPlayer );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetMaximumGeoSwitches( 1 );
		RegionPathfinder.SetMovementRate( &TerrainBasedMovement );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( 0 );// iPlayer );
		//RegionPathfinder.AddStartLocation( TargetLocation );
		RegionPathfinder.AddStartLocation( this->GetLocation() );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_GroundUnitAndSeaTransportPathToEverywhere );
		//float f2 = Timer2.GetDeltaSeconds();
		int g2=0;
	}
	return &m_GroundUnitAndSeaTransportPathToEverywhere;
}

/*
bool  CMultiRegion::IsAdjacentToMultiRegion( long iId )
{
	ASSERT( m_iId != iId );

	// See if it already exists in the list
	if( iId == 0 ) return false;
	POSITION pos = m_AdjacentMultiRegions.GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pMultiRegion = m_AdjacentMultiRegions.GetNext( pos );
		if( pMultiRegion->m_iId == iId ){ return true; }
	}

	return false;
}

void  CMultiRegion::AddAdjacentMultiRegion( long iId )
{
	/-*
	ASSERT( m_iId != iId );

	// See if it already exists in the list
	POSITION pos = m_AdjacentMultiRegions.GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pMultiRegion = m_AdjacentMultiRegions.GetNext( pos );
		if( pMultiRegion->m_iId == iId ){ return; }
	}
	*-/
	if( iId == 0 ) return;
	if( IsAdjacentToMultiRegion( iId ) == false )
	{
		// Didn't find it
		CMultiRegion* pMultiRegion = m_pMultiRegionManager->GetMultiRegion( iId );
		ASSERT( pMultiRegion );
		m_AdjacentMultiRegions.AddTail( pMultiRegion );
	}
}
*/

/*
void  CMultiRegion::AddPoiObjectToSummary( CPoiObject* pPoiObject )
{
	m_PoiObjects.AddTail( pPoiObject );
	//m_ObjectIds.AddTail( pPoiObject->GetObjectId() );

	//m_AIPoiObjects.AddTail( pAIPoiObject );
	//pAIPoiObject->GetInitialState()->SetLocationAndMultiRegion( pAIPoiObject->GetInitialState()->GetLocation(), this );

	m_TotalCitResSummary.AddToSummary( pPoiObject );

	// Update the CitResUnitSummary object
	CEOSAICitResUnitSummary* pSummary = &m_PlayerCitResUnitSummaries[ pPoiObject->GetOwner() ];
	ASSERT( pSummary );
	if( pSummary )
	{
		pSummary->AddToSummary( pPoiObject );

		CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
		if( pUnit )
		{
			pSummary->m_Units.m_fGroundTransportCapability += pUnit->GetAIGroundTransportCapability();

			//
			// This doesn't seem to track the unit ownership.  Why not?
			//
			pSummary->m_Units.m_UnitHPDefense_UnitTypeSet.Add( pUnit->GetCombatUnitTypeString(), pUnit->GetHPDefenseValue() );
			//pSummary->m_Units.m_fUnitStrengthSoft += pAIUnit->GetUnitStrengthSoft();
			//pSummary->m_Units.m_fUnitStrengthArmor += pAIUnit->GetUnitStrengthArmor();
			//pSummary->m_Units.m_fUnitStrengthAircraft += pAIUnit->GetUnitStrengthAircraft();
			//pSummary->m_Units.m_fUnitStrengthMissile += pAIUnit->GetUnitStrengthMissile();
			//pSummary->m_Units.m_fUnitStrengthShip += pAIUnit->GetUnitStrengthShip();
			//pSummary->m_Units.m_fUnitStrengthSubmarine += pAIUnit->GetUnitStrengthSubmarine();

			pSummary->m_Units.m_CombatPowerVsUnitTypes += pUnit->GetCombatPowerVsUnitTypes();
			//pSummary->m_Units.m_fCombatVsSoft += pAIUnit->GetAICombatVsSoft();
			//pSummary->m_Units.m_fCombatVsArmor += pAIUnit->GetAICombatVsArmor();
			//pSummary->m_Units.m_fCombatVsAircraft += pAIUnit->GetAICombatVsAircraft();
			//pSummary->m_Units.m_fCombatVsMissile += pAIUnit->GetAICombatVsMissile();
			//pSummary->m_Units.m_fCombatVsShip += pAIUnit->GetAICombatVsShip();
			//pSummary->m_Units.m_fCombatVsSubmarine += pAIUnit->GetAICombatVsSubmarine();
		}
	}
}
*/
/*
void  CMultiRegion::AddAIPoiObject( CEOSAIPoiObject* pAIPoiObject )
{
	sadasd
	m_AIPoiObjects.AddTail( pAIPoiObject );
	pAIPoiObject->GetInitialState()->SetLocationAndMultiRegion( pAIPoiObject->GetInitialState()->GetLocation(), this );

	m_TotalCitResSummary.AddToSummary( pAIPoiObject );

	// Update the CitResUnitSummary object
	CEOSAICitResUnitSummary* pSummary = &m_PlayerCitResUnitSummaries[ pAIPoiObject->GetOwner() ];
	ASSERT( pSummary );
	if( pSummary )
	{
		pSummary->AddToSummary( pAIPoiObject );

		CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
		if( pAIUnit )
		{
			pSummary->m_Units.m_fGroundTransportCapability += pAIUnit->GetAIGroundTransportCapability();

			//
			// This doesn't seem to track the unit ownership.  Why not?
			//
			pSummary->m_Units.m_UnitHPDefense_UnitTypeSet.Add( pAIUnit->GetCombatUnitTypeString(), pAIUnit->GetHPDefenseValue() );
			//pSummary->m_Units.m_fUnitStrengthSoft += pAIUnit->GetUnitStrengthSoft();
			//pSummary->m_Units.m_fUnitStrengthArmor += pAIUnit->GetUnitStrengthArmor();
			//pSummary->m_Units.m_fUnitStrengthAircraft += pAIUnit->GetUnitStrengthAircraft();
			//pSummary->m_Units.m_fUnitStrengthMissile += pAIUnit->GetUnitStrengthMissile();
			//pSummary->m_Units.m_fUnitStrengthShip += pAIUnit->GetUnitStrengthShip();
			//pSummary->m_Units.m_fUnitStrengthSubmarine += pAIUnit->GetUnitStrengthSubmarine();

			pSummary->m_Units.m_CombatPowerVsUnitTypes += pAIUnit->GetCombatPowerVsUnitTypes();
			//pSummary->m_Units.m_fCombatVsSoft += pAIUnit->GetAICombatVsSoft();
			//pSummary->m_Units.m_fCombatVsArmor += pAIUnit->GetAICombatVsArmor();
			//pSummary->m_Units.m_fCombatVsAircraft += pAIUnit->GetAICombatVsAircraft();
			//pSummary->m_Units.m_fCombatVsMissile += pAIUnit->GetAICombatVsMissile();
			//pSummary->m_Units.m_fCombatVsShip += pAIUnit->GetAICombatVsShip();
			//pSummary->m_Units.m_fCombatVsSubmarine += pAIUnit->GetAICombatVsSubmarine();
		}
	}
}
*/

//void  CMultiRegion::UpdateDistancesBasedOnPoiObjects( CPoiObject* pPoiObject )
//{
	//CEOSAIBrain* pAIPlan = pAIPoiObject->GetAIBrain();
	//CWorldDesc* pWorldDesc = pAIPoiObject->GetAIBrain()->GetWorldDescPlayer();
/*
	CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetWorldDescServer();
	float fDistance = pWorldDesc->GetPixelDistance( pPoiObject->GetLocation(), m_pCenterRegion->GetLocation() );
	if( fDistance < MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER )
	{
		// the PoiObject might be in the MultiRegion
		CEOSAIRegion2* pAIRegion = m_pMultiRegionManager->GetAIRegionManager()->GetAIRegion( pPoiObject->GetLocation() );
		if( pAIRegion && pAIRegion->m_iMultiRegionId == m_iId )
		{
			fDistance = 0.0f;
		}
	}
	fDistance -= MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER/2.0f;
	if( fDistance < 0.0f ){ fDistance = 0.0f; }
*/
/*
	if( dynamic_cast< CCity* >( pPoiObject ) ||
		dynamic_cast< CResourceSource* >( pPoiObject ) ||
		dynamic_cast< CUnit* >( pPoiObject ) )
	{
		if( m_CrowsDistanceFromPlayerCitResUnits[ pPoiObject->GetOwner() ] > fDistance )	
		{
			m_CrowsDistanceFromPlayerCitResUnits[ pPoiObject->GetOwner() ] = fDistance;
		}
	}
*/
	/*
	CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
	if( pCity )
	{
		if( pCity->GetGeo() == m_pCenterRegion->GetGeoId() )
		{
			m_PlayerHasCitiesOnThisGeo[ pPoiObject->GetOwner() ] = true;
		}
	}
	CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
	if( pUnit )
	{
		if( pUnit->GetGeo() == m_pCenterRegion->GetGeoId() )
		{
			m_PlayerHasGroundUnitsOnThisGeo[ pPoiObject->GetOwner() ] = true;
		}
	}
	*/
//}
/*
void  CMultiRegion::UpdateDistancesBasedOnAIPoiObject( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAIBrain* pAIPlan = pAIPoiObject->GetAIBrain();
	CWorldDesc* pWorldDesc = pAIPoiObject->GetAIBrain()->GetWorldDescPlayer();

	float fDistance = pWorldDesc->GetPixelDistance( pAIPoiObject->GetInitialState()->GetLocation(), m_pCenterRegion->GetLocation() );
	if( fDistance < MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER )
	{
		// the PoiObject might be in the MultiRegion
		//CEOSAIRegion2* pAIRegion = this->m_pMultiRegionManager->GetAISpatialMap()->pAIPoiObject->GetAIBrain()->GetSpatialMap()->GetRegion( pAIPoiObject->GetInitialState()->GetLocation() );
		CEOSAIRegion2* pAIRegion = this->m_pMultiRegionManager->GetAISpatialMap()->GetRegion( pAIPoiObject->GetInitialState()->GetLocation() );
		if( pAIRegion && pAIRegion->m_iMultiRegionId == m_iId )
		{
			fDistance = 0.0f;
		}
	}
	fDistance -= MAXIMUM_DISTANCE_OF_REGION_FROM_CENTER/2.0f;
	if( fDistance < 0.0f ){ fDistance = 0.0f; }


	if( dynamic_cast< CEOSAICity* >( pAIPoiObject ) ||
		dynamic_cast< CEOSAIResource* >( pAIPoiObject ) ||
		dynamic_cast< CEOSAIUnit* >( pAIPoiObject ) )
	{
		if( m_CrowsDistanceFromPlayerCitResUnits[ pAIPoiObject->GetOwner() ] > fDistance )	
		{
			m_CrowsDistanceFromPlayerCitResUnits[ pAIPoiObject->GetOwner() ] = fDistance;
		}
	}

	//
	CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
	if( pAICity )
	{
		/-*
		if( m_CrowsDistanceFromPlayerCities[ pAIPoiObject->GetOwner() ] > fDistance )	
		{
			m_CrowsDistanceFromPlayerCities[ pAIPoiObject->GetOwner() ] = fDistance;
		}
		*-/
		if( pAICity->GetInitialState()->GetGeo() == m_pCenterRegion->GetGeo() )
		{
			m_PlayerHasCitiesOnThisGeo[ pAIPoiObject->GetOwner() ] = true;
		}
	}
	/-*
	CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
	if( pAIResource )
	{
		if( m_CrowsDistanceFromPlayerResources[ pAIPoiObject->GetOwner() ] > fDistance )	
		{
			m_CrowsDistanceFromPlayerResources[ pAIPoiObject->GetOwner() ] = fDistance;
		}
	}
	*-/
	CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
	if( pAIUnit )
	{
		/-*
		if( pAIUnit->IsGroundUnit() )
		{
			if( m_CrowsDistanceFromPlayerGroundUnits[ pAIPoiObject->GetOwner() ] > fDistance )	
			{
				m_CrowsDistanceFromPlayerGroundUnits[ pAIPoiObject->GetOwner() ] = fDistance;
			}
		}
		if( pAIUnit->IsSeaUnit() )
		{
			if( m_CrowsDistanceFromPlayerSeaUnits[ pAIPoiObject->GetOwner() ] > fDistance )	
			{
				m_CrowsDistanceFromPlayerSeaUnits[ pAIPoiObject->GetOwner() ] = fDistance;
			}
		}
		if( pAIUnit->IsAircraft() )
		{
			if( m_CrowsDistanceFromPlayerAircraft[ pAIPoiObject->GetOwner() ] > fDistance )	
			{
				m_CrowsDistanceFromPlayerAircraft[ pAIPoiObject->GetOwner() ] = fDistance;
			}
		}
		if( pAIUnit->IsMissile() )
		{
			if( m_CrowsDistanceFromPlayerMissiles[ pAIPoiObject->GetOwner() ] > fDistance )	
			{
				m_CrowsDistanceFromPlayerMissiles[ pAIPoiObject->GetOwner() ] = fDistance;
			}
		}
		*-/
		if( pAIUnit->GetInitialState()->GetGeo() == m_pCenterRegion->GetGeo() )
		{
			m_PlayerHasGroundUnitsOnThisGeo[ pAIPoiObject->GetOwner() ] = true;
		}
	}
}
*/
/*
void CMultiRegion::ClearSightingPresenceForEachPlayer()
{
	for( long iPlayer=0; iPlayer<m_OverflowVisibility.m_iSize; iPlayer++ )
	{
		m_OverflowVisibility[iPlayer] = 0.0f;
	}
}
*/
/*
void CMultiRegion::CalculateSightingPresenceForEachPlayer()
{
	// Update the Sighting Presence for each Player

	CEOSAIBCDumbArray1D< long >  PlayerUnitCount;
	PlayerUnitCount.SetSize( m_OverflowVisibility.m_iSize );
	for( long iPlayer=1; iPlayer<m_OverflowVisibility.m_iSize; iPlayer++ )
	{
		PlayerUnitCount[ iPlayer ] = GetCitResUnitCount( iPlayer );
		//m_OverflowVisibility[iPlayer] = 1.0f - ( 1.0f / ( PlayerUnitCount[ iPlayer ] + 1.0f ) );
	}

	// Adjacent Regions have half value (at minimum)
	POSITION pos = m_AdjacentRegions.GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pOtherRegion = m_AdjacentRegions.GetNext( pos );
		float fMult = 0.5f;
		for( long iPlayer=1; iPlayer<m_OverflowVisibility.m_iSize; iPlayer++ )
		{
			if( PlayerUnitCount[ iPlayer ] == 0 ) continue;
			float fPresence = 1.0f - ( 1.0f / ( PlayerUnitCount[ iPlayer ] + 1.0f ) );

			float fVal = fMult * fPresence;
			if( fVal > pOtherRegion->m_OverflowVisibility[iPlayer] )
			{
				pOtherRegion->m_OverflowVisibility[iPlayer] = fVal;
			}
		}
	}

	// Update the surrounding areas with a Secondary Sighting Presence
	//CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();
	CWorldDesc* pWorldDesc = GetCommonState()->GetWorldDescServer();
	CEOSAIBCArray< CMultiRegion* >* pArray = m_pMultiRegionManager->GetMultiRegionArray();
	for( long iMultiRegion=0; iMultiRegion<pArray->m_iArraySize; iMultiRegion++ )
	{
		CMultiRegion* pOtherRegion = pArray->Get( iMultiRegion );
		if( pOtherRegion )
		{
			float fDistance = pWorldDesc->GetPixelDistance( GetLocation(), pOtherRegion->GetLocation() );

			if( fDistance < 200.0f )
			{
				float fMult = ( 200.0f - fDistance ) / 200.0f;
				if( fMult > 1.0f ){ fMult = 1.0f; }
				for( long iPlayer=1; iPlayer<m_OverflowVisibility.m_iSize; iPlayer++ )
				{
					if( PlayerUnitCount[ iPlayer ] == 0 ) continue;
					float fPresence = 1.0f - ( 1.0f / ( PlayerUnitCount[ iPlayer ] + 1.0f ) );

					float fVal = fMult * fPresence;
					if( fVal > pOtherRegion->m_OverflowVisibility[iPlayer] )
					{
						pOtherRegion->m_OverflowVisibility[iPlayer] = fVal;
					}
				}
			}
		}
	}
}
*/
/*
float CMultiRegion::GetCrowsDistanceFromMyCitRes() //{ return min( m_fCrowsDistanceFromMyCities, m_fCrowsDistanceFromMyResources ); }
{
	long iPlayer = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	return min( m_CrowsDistanceFromPlayerCities[iPlayer], m_CrowsDistanceFromPlayerResources[iPlayer] );
}
*/
/*
float CMultiRegion::GetCrowsDistanceFromPlayerCitResUnits( long iPlayer ) //{ return m_fCrowsDistanceFromMyCitResUnits; }
{
	//long iPlayer = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	return m_CrowsDistanceFromPlayerCitResUnits[iPlayer];
}
*/
/*
float  CMultiRegion::GetPlayerAndSATPresence( long iPlayer )
{
	long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	//long iMyPlayerNumber = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	//CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	float fPresence = 0.0f;
	for( long i = 1; i<iNumberOfPlayers+1; i++ )
	{
		if( i == iPlayer )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( i );
			fPresence += pSummary->GetCitResUnitPresence();
		}
		else
		{
			EOSAIEnumForeignRelations eRel = pWorldDescServer->GetForeignRelations( iPlayer, i );
			if( eRel.IsAlly() || eRel.IsTeammate() )
			{
				CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( i );
				fPresence += pSummary->GetCitResUnitPresence();
			}
		}
	}
	return fPresence;
}
*/
/*
float  CMultiRegion::GetMyCitResValue()
{
	long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	long iMyPlayerNumber = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();

	float fCitResValue = 0.0f;
	for( long iPlayer = 1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		if( iPlayer == iMyPlayerNumber )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( iPlayer );
			fCitResValue += pSummary->m_CitRes.GetCitResValue( m_pMultiRegionManager->GetAIBrain()->GetCitResValueMultiplier() );
		}
	}
	return fCitResValue;
}

float  CMultiRegion::GetPAllyCitResValue()
{
	long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	long iMyPlayerNumber = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();

	float fCitResValue = 0.0f;
	for( long iPlayer = 1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		if( iPlayer == iMyPlayerNumber ) continue;
		//if( pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer ).IsPAlly() )
		if( pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer ).IsAlly() )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( iPlayer );
			fCitResValue += pSummary->m_CitRes.GetCitResValue( m_pMultiRegionManager->GetAIBrain()->GetCitResValueMultiplier() );
		}
	}
	return fCitResValue;
}

float  CMultiRegion::GetMyAndPAllyCitResValue()
{
	long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	long iMyPlayerNumber = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();

	float fFriendlyCitResValue = 0.0f;
	for( long iPlayer = 1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		if( iPlayer == iMyPlayerNumber )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( iPlayer );
			fFriendlyCitResValue += pSummary->m_CitRes.GetCitResValue( m_pMultiRegionManager->GetAIBrain()->GetCitResValueMultiplier() );
		}
		//else if( pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer ).IsPAlly() )
		else if( pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer ).IsAlly() )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( iPlayer );
			fFriendlyCitResValue += pSummary->m_CitRes.GetCitResValue( m_pMultiRegionManager->GetAIBrain()->GetCitResValueMultiplier() );
		}
	}
	return fFriendlyCitResValue;
}

float  CMultiRegion::GetOtherPlayerPresence()
{
	long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	long iMyPlayerNumber = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();

	float fPresence = 0.0f;
	for( long iPlayer = 1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		if( iPlayer == iMyPlayerNumber ){ continue; }
		//else if( pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer ).IsPAlly() == false )
		else if( pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer ).IsAlly() == false )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( iPlayer );
			fPresence += pSummary->GetCitResUnitPresence();
		}
	}
	return fPresence;
}

float CMultiRegion::GetEnemyPlayerCityValue()
{
	long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	long iMyPlayerNumber = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();

	float fCityValue = 0.0f;
	for( long iPlayer = 1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		if( iPlayer == iMyPlayerNumber ){ continue; }
		else if( pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer ).IsEnemy() )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( iPlayer );
			fCityValue += pSummary->m_CitRes.GetCityValue( m_pMultiRegionManager->GetAIBrain()->GetCitResValueMultiplier() );
		}
	}
	return fCityValue;
}

float CMultiRegion::GetTotalCitResValue()
{
	return m_TotalCitResSummary.GetCitResValue( m_pMultiRegionManager->GetAIBrain()->GetCitResValueMultiplier() );
}
*/

/*
void  CMultiRegion::CalculateLocalAndPathwayAIPoiObjects()
{
	m_LocalPoiObjects.RemoveAll();
	m_PathwayPoiObjects.RemoveAll();

	CEOSAILocation MultiRegionCenterLocation = this->GetLocation();

	// Add all AIPoi in the MultiRegion
	POSITION pos = m_PoiObjects.GetHeadPosition();
	while( pos )
	{
		CPoiObject* pPoiObject = m_PoiObjects.GetNext( pos );
		m_LocalPoiObjects.AddTail( pPoiObject );
	}

	// Assume that units within 150 pixels or RadiusOfRegions+50 of the Location will act 
	//   as local resistence
	//
	CWorldDescServer* pWorldDescServer = m_pMultiRegionManager->GetWorldDescServer();
	float fCurrentRadiusCheck = 175.0f;
	//float fRadius = max( 150.0f, m_fRadiusOfRegions + 50.0f );
	CEOSAIBrain* pAIBrain = m_pMultiRegionManager->GetAIBrain();
	pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		//CEOSAIPoiObject* pAIPoiObject = pAIBrain->GetAIPoiObjectList()->GetNext( pos );
		CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		//
		if( pPoiObject )
		{
			float fDistance = pAIBrain->GetWorldDesc()->GetPixelDistance( pPoiObject->GetLocation(), MultiRegionCenterLocation );
			if( fDistance < fCurrentRadiusCheck )
			{
				bool bExistsInTheLocalList = false;
				POSITION pos2 = m_LocalPoiObjects.GetHeadPosition();
				while( pos2 )
				{
					CPoiObject* pExistingTarget = m_LocalPoiObjects.GetNext( pos2 );
					if( pPoiObject == pExistingTarget )
					{
						bExistsInTheLocalList = true;
						break;
					}
				}
				if( bExistsInTheLocalList == false )
				{
					// I'm using this method because I should catch all the children this way anyway
					m_LocalPoiObjects.AddTail( pPoiObject );
				}
			}
		}
	}

	// Look at the pathway, add-up the expected resistence
	//
	fCurrentRadiusCheck = 200.0f;
	long iMyPlayer = pAIBrain->GetAIPlayerNumber();
	CEOSAIMultiRegionManager2* pMultiRegionManager = pAIBrain->GetMultiRegionManager();
	CMultiRegionPathwayResult* pMultiRegionPathwayResult =
		pMultiRegionManager->GetPlayerAttackPathwayBasedOnCitRes( iMyPlayer );
	CMultiRegion* pMultiRegion = this;
	if( pMultiRegionPathwayResult && pMultiRegion )
	{
		long iCurrentMultiRegion = pMultiRegion->m_iId;
		CMultiRegionResult* pMultiRegionResult =
			pMultiRegionPathwayResult->GetMultiRegionResult( iCurrentMultiRegion );
		iCurrentMultiRegion = pMultiRegionResult->m_iPreviousMultiRegion;
		while( iCurrentMultiRegion != 0 )
		{
			CMultiRegionResult* pMultiRegionResult =
				pMultiRegionPathwayResult->GetMultiRegionResult( iCurrentMultiRegion );
			ASSERT( pMultiRegionResult );
			//
			CMultiRegion* pCurrentMultiRegion = pMultiRegionResult->m_pMultiRegion;
			CEOSAILocation CurrentMultiRegionLocation = pCurrentMultiRegion->GetLocation();
			//
			POSITION pos = pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
			while( pos )
			{
				CEOSAIPoiObject* pAIPoiObject = pAIBrain->GetAIPoiObjectList()->GetNext( pos );
				//
				float fDistance = pAIBrain->GetWorldDesc()->GetPixelDistance( pAIPoiObject->GetInitialState()->GetLocation(), CurrentMultiRegionLocation );
				if( fDistance < fCurrentRadiusCheck )
				{
					bool bExistsInTheList = false;
					POSITION pos2 = m_LocalPoiObjects.GetHeadPosition();
					while( pos2 )
					{
						CPoiObject* pExistingTarget = m_LocalPoiObjects.GetNext( pos2 );
						if( pAIPoiObject->GetPoiObject() == pExistingTarget )
						{
							bExistsInTheList = true;
							break;
						}
					}
					if( bExistsInTheList == false )
					{
						pos2 = m_PathwayPoiObjects.GetHeadPosition();
						while( pos2 )
						{
							CPoiObject* pExistingTarget = m_PathwayPoiObjects.GetNext( pos2 );
							if( pAIPoiObject->GetPoiObject() == pExistingTarget )
							{
								bExistsInTheList = true;
								break;
							}
						}
					}
					if( bExistsInTheList == false )
					{
						// I'm using this method because I should catch all the children this way anyway
						m_PathwayPoiObjects.AddTail( pAIPoiObject->GetPoiObject() );
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

// Thoughts
/*
void  CMultiRegion::CreateSomeThoughtsAboutThisObject( CEOSAIBrain* pAIBrain )
{
	//CEOSAIBrain* pAIBrain = m_pMultiRegionManager->GetAIBrain();

	// Thoughts:
	//   - Want to capture the CitRes for myself
	//   - Want to defeat enemy units 
	//       Either because they threaten my assets, could threaten, they are in an important
	//       waterway that I need for invasion, or I simply want to clean-up
	//   - Want to degrade production capacity (strategic bombing)

	//   - The Multiregion might be connected to other Multiregions that have associated-thoughts.
	//       E.g. If enemy units are in an adjacent MultiRegion, will want to destroy them
	//       to protect this MultiRegion.  Or, if an enemy owns an adjacent MultiRegion and
	//       I want to capture unowned CitRes here, I need to send reinforcements.
	//     Also, an island of two adjacent multiregions with unowned CitRes are more valuable 
	//       than two disconnected islands with the same CitRes.  The value of a MultiRegion
	//       should inherit some value of the adjacent multiregions.

	//m_pAIThought_ShouldSendUnitsToCaptureTheLandCitRes =
	//	new CAIThought_ShouldSendUnitsToCaptureTheLandCitRes( pAIBrain, this );

	//m_pAIThought_ShouldSendUnitsHere =
	//	new CAIThought_ShouldSendUnitsHere( pAIBrain, this );
}
*/
/*
void  CMultiRegion::InvokeThought_WhatIsMyCitResUnitProximityToMultiRegion()
{
	// This information is pre-computed
}
*/
/*
float CMultiRegion::GetScore_WhatIsMyCitResUnitProximityToMultiRegion() // 0 - 1
{
	// I'm using CitResUnit because I don't know what the task will be yet.
	//   It might involve attacking an enemy fleet when my fleet is near them.
	//   In that case, the Unit distance is important to remember.
	//   In other cases, like capturing CitRes, I probably care more about
	//     my CitRes proximity to the target, but maybe I have a transport
	//     with ground unit on the way.  I want the AI to keep moving the
	//     transport towards it's target even if another target appears closer
	//     to my landmass.

	// At distance    0, return 100.0
	// At distance  200, return 100.0
	// At distance  300, return  75.0
	// At distance  400, return  60.0
	// At distance  600, return  42.9
	// At distance  800, return  33.3
	// At distance 1000, return  27.3
	// At distance 1200, return  23.1
	long iPlayer = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	float fVal = m_CrowsDistanceFromPlayerCitResUnits[ iPlayer ];
	//float fVal = m_fCrowsDistanceFromMyCitResUnits;
	fVal -= 200.0f;
	if( fVal < 0.0f ) fVal = 0.0f;

	fVal = 300.0f / (fVal + 300.0f);
	return fVal;
}
*/
/*
C9MovementPathwaysFromEverywhereToMRX* CMultiRegion::InvokePathwaysToMultiRegion( long iPlayer )
{
	if( m_pPathwaysToMultiRegion == NULL )
	{
		m_pPathwaysToMultiRegion = new C9MovementPathwaysFromEverywhereToMRX( iPlayer, this );
	}
	return m_pPathwaysToMultiRegion;
}
*/
/*
CAISeaPathways_EverywhereMRX* CMultiRegion::InvokeSeaPathways_EverywhereMRX()
{
	if( m_pSeaPathways_EverywhereMRX == NULL )
	{
		m_pSeaPathways_EverywhereMRX = new CAISeaPathways_EverywhereMRX( this );
	}
	return m_pSeaPathways_EverywhereMRX;
}
*/
/*
CAIMultiRegionPathwayScoresForTransports* CMultiRegion::InvokeTransportPathwaysToMultiRegion()
{
	if( m_pPathwaysForTransportsToThisMultiRegion == NULL )
	{
		m_pPathwaysForTransportsToThisMultiRegion = new CAIMultiRegionPathwayScoresForTransports();
	}
	return m_pPathwaysForTransportsToThisMultiRegion;
}
*/
/*
void  CMultiRegion::Calculate_WhatUnitsCanIGetToMultiRegion( CEOSAIActionRequirement::EnumUnitAction eAction )
{
	CAIThought_WhatUnitsCanIGetToMultiRegion* pThought =
		new CAIThought_WhatUnitsCanIGetToMultiRegion( this, eAction );
}
*/

//void  CMultiRegion::FillPotentialActionList( CEOSAIActionRequirement::EnumUnitAction eAction, CEOSAIActionList* pAIActionList )
//void  CMultiRegion::CreateUnitPotentialMovementList( CEOSAIActionRequirement::EnumUnitAction eAction, CEOSAIActionList* pAIActionList )
//{
	// Come-up with some decent potential Unit/ImaginaryUnit movements
	//pAIActionList->AddAIAction( );

	// Limit the list of units to 
//}
/*
CAIThought_WhatUnitsCanIGetToMultiRegion*  CMultiRegion::CreateUnitPotentialMovementList( CEOSAIActionRequirement::EnumUnitAction eAction )
{
	return NULL;
}
*/
/*
CAIThought_WhatUnitsCanIGetToMultiRegion*  InvokeThought_WhatUnitsCanIGetToMultiRegion( CEOSAIActionRequirement::EnumUnitAction eAction )
{
	CAIThought_WhatUnitsCanIGetToMultiRegion* pThought =
		new CAIThought_WhatUnitsCanIGetToMultiRegion( this, eAction );
}
*/

/*
void  CMultiRegion::InvokeThought_WhatUnitsCanIGetToMultiRegion( CEOSAIActionRequirement::EnumUnitAction eAction )
{
	return new CAIThought_WhatUnitsCanIGetToMultiRegion( this, eAction );
}
*/
/*
void  CMultiRegion::InvokeThought_WhatPathwaysCanITakeToTheMultiRegion()
{
}
*/
/*
float CMultiRegion::GetEnemyUnitPower()
{
	long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
	long iMyPlayerNumber = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();

	float fEnemyCombatPower = 0.0f;
	for( long iPlayer = 1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		if( pWorldDesc->GetForeignRelations( iMyPlayerNumber, iPlayer ).IsEnemy() )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( iPlayer );
			fEnemyCombatPower += pSummary->m_Units.GetTotalCombatPower();
		}
	}
	return fEnemyCombatPower;
}
*/

/*
float CMultiRegion::GetUnitPower( long iPlayer )
{
	CEOSAICitResUnitSummary* pSummary = &m_PlayerCitResUnitSummaries[iPlayer];
	return pSummary->GetUnitPower();
}

long CMultiRegion::GetCitResUnitCount( long iPlayer )
{
	long iCount = 0;
	POSITION pos = m_PoiObjects.GetHeadPosition();
	while( pos )
	{
		CPoiObject* pPoiObject = m_PoiObjects.GetNext( pos );
		if( pPoiObject->GetOwner() == iPlayer )
		{
			iCount++;
		}
	}
	return iCount;
}

long CMultiRegion::GetCitGroundUnitCount( long iPlayer )
{
	long iCount = 0;
	POSITION pos = m_PoiObjects.GetHeadPosition();
	while( pos )
	{
		CPoiObject* pPoiObject = m_PoiObjects.GetNext( pos );
		if( pPoiObject->GetOwner() == iPlayer )
		{
			if( dynamic_cast< CCity* >( pPoiObject ) )
			{
				iCount++;
			}
			if( dynamic_cast< CUnit* >( pPoiObject ) )
			{
				CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
				if( pUnit->IsGroundUnit() )
				{
					iCount++;
				}
			}
		}
	}
	return iCount;
}

long CMultiRegion::GetCitResCount( long iPlayer )
{
	long iCount = 0;
	POSITION pos = m_PoiObjects.GetHeadPosition();
	while( pos )
	{
		CPoiObject* pPoiObject = m_PoiObjects.GetNext( pos );
		if( pPoiObject->GetOwner() == iPlayer )
		{
			if( dynamic_cast< CCity* >( pPoiObject ) )
			{
				iCount++;
			}
			if( dynamic_cast< CResourceSource* >( pPoiObject ) )
			{
				iCount++;
			}
		}
	}
	return iCount;
}

float CMultiRegion::GetPresence01( long iPlayer )
{
	return 1.0f - ( 1.0f / ( GetCitResUnitCount( iPlayer ) + 1.0f ) );
}
*/


#include "stdafx.h"
#include "EOSAIRegionMapToEverywhere.h"
#include "EOSAICommonData2.h"
//#include "UnitTemplate.h"

using namespace EOSAI;

// Cache the data into an AIRegionMapToEverywhere structure
CEOSAIRegionPathwayMap* CAICachedMap::InvokeDirectAIRegionMapToEverywhere()
{
	if( m_AIRegionMapDirectToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = 0;//m_InitialState.GetOwner();
		//CEOSAILocation StartLocation = m_InitialState.GetLocation();//GetPoiObject()->GetLocation();
		CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();

		CEOSAIRegionPathfinder& RegionPathfinder = pAIRegionManager->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetMovementRate( m_pActorAIUnitTemplate );
		RegionPathfinder.SetDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetGeoSwitchMultiplier( 300.0f );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartingPathfinderPoint( m_iPathfinderPointId );//.AddStartLocation( StartLocation );
		//RegionPathfinder.AddEndLocation( pPoint->m_Location );//pMultiRegion );
		//RegionPathfinder.Solve2( iOwner, true );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_AIRegionMapDirectToEverywhere );

		// Enlarge the Pathway map - find coastal locations
	}
	//
	return &m_AIRegionMapDirectToEverywhere;
}

CEOSAIRegionPathwayMap* CAICachedMap::InvokeDirectAIRegionMapToCoasts()
{
	if( m_AIRegionMapDirectToCoasts.GetArraySize() == 0 )
	{
		float fMovementRate = 1.0f;//m_pActorUnitTemplate->GetMovementRate();
		CEOSAIRegionPathwayMap* pPathwayMap = InvokeDirectAIRegionMapToEverywhere();
		CEOSAIRegionPathfinder::AddBorderRegions( pPathwayMap, &m_AIRegionMapDirectToCoasts, fMovementRate );
	}
	//
	return &m_AIRegionMapDirectToCoasts;
}

CEOSAIPathfinderPointCachedMap::~CEOSAIPathfinderPointCachedMap()
{
	while( m_AICachedMaps.IsEmpty() == FALSE ){ delete m_AICachedMaps.RemoveHead(); }
}

CEOSAIRegionPathwayMap* CEOSAIPathfinderPointCachedMap::InvokeDirectAIRegionMapToEverywhere( CEOSAIUnitTemplate* pActorAIUnitTemplate )
{
	POSITION pos = m_AICachedMaps.GetHeadPosition();
	while( pos )
	{
		CAICachedMap* pAICachedMap = m_AICachedMaps.GetNext( pos );
		if( pAICachedMap->m_pActorAIUnitTemplate == pActorAIUnitTemplate )
		{
			// Found a cached copy
			//Beep( 1600,200 );
			return pAICachedMap->InvokeDirectAIRegionMapToEverywhere();
		}
	}
	// This path not found.  Create it.
	CAICachedMap* pAICachedMap = new CAICachedMap();
	pAICachedMap->m_iPathfinderPointId = m_iPathfinderPointId;
	pAICachedMap->m_pActorAIUnitTemplate = pActorAIUnitTemplate;
	m_AICachedMaps.AddTail( pAICachedMap );

	return pAICachedMap->InvokeDirectAIRegionMapToEverywhere();
}

CEOSAIRegionPathwayMap* CEOSAIPathfinderPointCachedMap::InvokeDirectAIRegionMapToCoasts( CEOSAIUnitTemplate* pActorAIUnitTemplate )
{
	POSITION pos = m_AICachedMaps.GetHeadPosition();
	while( pos )
	{
		CAICachedMap* pAICachedMap = m_AICachedMaps.GetNext( pos );
		if( pAICachedMap->m_pActorAIUnitTemplate == pActorAIUnitTemplate )
		{
			return pAICachedMap->InvokeDirectAIRegionMapToCoasts();
		}
	}
	// This path not found.  Create it.
	CAICachedMap* pAICachedMap = new CAICachedMap();
	pAICachedMap->m_iPathfinderPointId = m_iPathfinderPointId;
	pAICachedMap->m_pActorAIUnitTemplate = pActorAIUnitTemplate;
	m_AICachedMaps.AddTail( pAICachedMap );

	return pAICachedMap->InvokeDirectAIRegionMapToCoasts();
}


CEOSAIRegionMapToEverywhere::~CEOSAIRegionMapToEverywhere()
{
	//for( long i=0; i<m_StartingAIRegions.m_iSize; i++ )
	//{
	//	if( m_StartingAIRegions[i] ){ delete m_StartingAIRegions[i]; m_StartingAIRegions[i] = NULL; }
	//}
	for( long i=0; i<m_StartingPathfinderPoints.m_iSize; i++ )
	{
		if( m_StartingPathfinderPoints[i] ){ delete m_StartingPathfinderPoints[i]; m_StartingPathfinderPoints[i] = NULL; }
	}
}

void  CEOSAIRegionMapToEverywhere::Clear()
{
	//for( long i=0; i<m_StartingAIRegions.m_iSize; i++ )
	//{
	//	if( m_StartingAIRegions[i] ){ delete m_StartingAIRegions[i]; m_StartingAIRegions[i] = NULL; }
	//}
	for( long i=0; i<m_StartingPathfinderPoints.m_iSize; i++ )
	{
		if( m_StartingPathfinderPoints[i] ){ delete m_StartingPathfinderPoints[i]; m_StartingPathfinderPoints[i] = NULL; }
	}
}

CEOSAIRegionPathwayMap* CEOSAIRegionMapToEverywhere::InvokeDirectAIRegionMapToEverywhere( long iStartingAIRegion, CEOSAIUnitTemplate* pActorAIUnitTemplate )
{
	CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
	if( m_StartingPathfinderPoints.m_iSize == 0 )
	{
		//long iSize = pAIRegionManager->GetAIRegionArray()->GetSize();
		long iSize = pAIRegionManager->m_PathfinderPointArray.GetSize();
		m_StartingPathfinderPoints.SetSize( iSize );

		//for( long i=0; i<iSize; i++ ){ m_StartingAIRegions[i] = NULL; }
		for( long i=0; i<iSize; i++ ){ m_StartingPathfinderPoints[i] = NULL; }
	}

	if( iStartingAIRegion < 0 ){ ASSERT( false ); return NULL; }
	//if( iStartingAIRegion >= m_StartingAIRegions.m_iSize ){ ASSERT( false ); return NULL; }
	if( iStartingAIRegion >= m_StartingPathfinderPoints.m_iSize ){ ASSERT( false ); return NULL; }
	//CAIRegionCachedMap* pAICachedMap = m_StartingAIRegions[iStartingAIRegion];
	CEOSAIPathfinderPointCachedMap* pAICachedMap = m_StartingPathfinderPoints[iStartingAIRegion];
	if( pAICachedMap == NULL )
	{
		pAICachedMap = new CEOSAIPathfinderPointCachedMap();
		pAICachedMap->m_iPathfinderPointId = iStartingAIRegion;
		m_StartingPathfinderPoints[iStartingAIRegion] = pAICachedMap;
	}
	ASSERT( pAICachedMap );

	return pAICachedMap->InvokeDirectAIRegionMapToEverywhere( pActorAIUnitTemplate );
}

CEOSAIRegionPathwayMap* CEOSAIRegionMapToEverywhere::InvokeDirectAIRegionMapToCoasts( long iStartingAIRegion, CEOSAIUnitTemplate* pActorAIUnitTemplate )
{
	CEOSAIRegionPathwayMap* pDirectMapToEverywhere = InvokeDirectAIRegionMapToEverywhere( iStartingAIRegion, pActorAIUnitTemplate );
	if( pDirectMapToEverywhere == NULL ) return NULL;
	//CAIRegionCachedMap* pAICachedMap = m_StartingAIRegions[iStartingAIRegion];
	CEOSAIPathfinderPointCachedMap* pAICachedMap = m_StartingPathfinderPoints[iStartingAIRegion];

	return pAICachedMap->InvokeDirectAIRegionMapToCoasts( pActorAIUnitTemplate );
}

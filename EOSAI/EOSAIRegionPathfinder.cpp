
#include "stdafx.h"
#include "EOSAIRegionPathfinder.h"
#include "EOSAIRegion2.h"
#include "EOSAIMultiRegionManager2.h" // class CEOSAIRegionPathwayItem, CEOSAIRegionPathwayMap;
#include "City.h"
#include "EOSAIAirfield.h"
#include "EOSAIUnit.h"
#include "EOSAIRoad2.h"
//#include "WorldDescServer.h"
#include "EOSAIStopwatch2.h"
#include "EOSAIGenericRegion.h"
//#include "PathfindingPoint.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//using namespace EOSAI;

//
// Copy all the data from [OriginalMap] and add adjacent regions
void CEOSAIRegionPathfinder::AddBorderRegions( CEOSAIRegionPathwayMap* pOriginalMap, CEOSAIRegionPathwayMap* pBorderMap, float fMovementRate )
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	ASSERT( pOriginalMap->GetRegionManager() );
	ASSERT( pOriginalMap->GetArraySize() > 0 );

	pBorderMap->Clear();
	pBorderMap->SetRegionManager( pOriginalMap->GetRegionManager() );
	pBorderMap->SetArraySize( pOriginalMap->GetArraySize() );

	// Copy all the data first
	long iSize = pOriginalMap->GetArraySize();
	for( long iRegion=1; iRegion<iSize; iRegion++ )
	{
		CEOSAIRegionPathwayItem* pOriginalItem = pOriginalMap->GetRegionResult( iRegion );
		if( pOriginalItem )
		{
			// Create a new Object
			if( pBorderMap->GetRegionResult( iRegion ) == NULL )
			{
				CEOSAIRegionPathwayItem* pNewBorderItem = new CEOSAIRegionPathwayItem();
				pNewBorderItem->Copy( pOriginalItem );
				pNewBorderItem->m_pRegionPathwayMap = pBorderMap;
				pBorderMap->AddRegionPathwayItem( pNewBorderItem );
			}
		}
	}
	// Copy the border regions
	for( long iRegion=1; iRegion<iSize; iRegion++ )
	{
		CEOSAIRegionPathwayItem* pOriginalItem = pOriginalMap->GetRegionResult( iRegion );
		if( pOriginalItem )
		{
			CEOSAIGenericRegion* pOriginalRegion = dynamic_cast< CEOSAIGenericRegion* >( pOriginalItem->m_pPathfinderPoint );
			if( pOriginalRegion )
			{
				POSITION pos = pOriginalRegion->m_AdjacentRegions.GetHeadPosition();
				while( pos )
				{
					CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pOriginalRegion->m_AdjacentRegions.GetNext( pos );
					CEOSAIGenericRegion* pAdjRegion = pAdjRegionDesc->m_pRegion;
					if( pOriginalMap->GetRegionResult( pAdjRegion->m_iId ) == NULL )
					{
						CEOSAIRegionPathwayItem* pNewBorderItem = pBorderMap->GetRegionResult( pAdjRegion->m_iId );
						if( pNewBorderItem == NULL )
						{
							// Create a new Object
							pNewBorderItem = new CEOSAIRegionPathwayItem();
							pNewBorderItem->Copy( pOriginalItem );
							pNewBorderItem->m_pRegionPathwayMap = pBorderMap;//pOriginalItem->m_pRegionPathwayMap;
							pNewBorderItem->m_pPathfinderPoint = pAdjRegion;
							//
							float fDistance = g_pWorldDistanceTool->GetDistance( pAdjRegion->GetLocation(), pOriginalItem->m_pPathfinderPoint->GetLocation() );
							pNewBorderItem->m_fDistance += fDistance;
							pNewBorderItem->m_fPathfindingValue += fDistance;
							pNewBorderItem->m_fTime += fDistance/fMovementRate;
							//
							pNewBorderItem->m_iPreviousPathwayItem = pOriginalItem->m_pPathfinderPoint->m_iId;
							pBorderMap->AddRegionPathwayItem( pNewBorderItem );
						}
						else if( pNewBorderItem->m_fPathfindingValue > pOriginalItem->m_fPathfindingValue )
						{
							pNewBorderItem->Copy( pOriginalItem );
							pNewBorderItem->m_pPathfinderPoint = pAdjRegion;
							//
							float fDistance = g_pWorldDistanceTool->GetDistance( pAdjRegion->GetLocation(), pOriginalItem->m_pPathfinderPoint->GetLocation() );
							pNewBorderItem->m_fDistance += fDistance;
							pNewBorderItem->m_fPathfindingValue += fDistance;
							pNewBorderItem->m_fTime += fDistance/fMovementRate;
							//
							pNewBorderItem->m_iPreviousPathwayItem = pOriginalItem->m_pPathfinderPoint->m_iId;
						}
					}
				}
			}
		}
	}
}

//
// Compile the overlapping [A][B] regions
void CEOSAIRegionPathfinder::GetOverlappedRegions( CEOSAIRegionPathwayMap* pMap1, CEOSAIRegionPathwayMap* pMap2, CEOSAIRegionPathwayMap* pOverlappedMap )
{
	ASSERT( pMap1->GetRegionManager() == pMap2->GetRegionManager() );
	ASSERT( pMap1->GetArraySize()     == pMap2->GetArraySize() );

	pOverlappedMap->Clear();
	pOverlappedMap->SetRegionManager( pMap1->GetRegionManager() );
	pOverlappedMap->SetArraySize( pMap1->GetArraySize() );

	long iSize = pMap1->GetArraySize();
	for( long iRegion=1; iRegion<iSize; iRegion++ )
	{
		CEOSAIRegionPathwayItem* pItem1 = pMap1->GetRegionResult( iRegion );
		CEOSAIRegionPathwayItem* pItem2 = pMap2->GetRegionResult( iRegion );
		if( pItem1 != NULL && pItem2 != NULL )
		{
			// Create a new Object
			CEOSAIRegionPathwayItem* pNewItem = new CEOSAIRegionPathwayItem();
			pNewItem->Copy( pItem1 );
			pNewItem->m_fDanger = max( pItem1->m_fDanger, pItem2->m_fDanger );
			pNewItem->m_fTime   = max( pItem1->m_fTime, pItem2->m_fTime );
			pNewItem->m_fPathfindingValue = max( pItem1->m_fPathfindingValue, pItem2->m_fPathfindingValue );
			pOverlappedMap->AddRegionPathwayItem( pNewItem );
		}
	}
}


//
// Compile the overlapping [A][B] regions + the regions in [A] that border on [B] (add the B region)
/*
void CEOSAIRegionPathfinder::GetOverlappedAndBorderRegions( CEOSAIRegionPathwayMap* pMap1, CEOSAIRegionPathwayMap* pMap2, CEOSAIRegionPathwayMap* pOverlappedMap )
{
	ASSERT( pMap1->GetRegionManager() == pMap2->GetRegionManager() );
	ASSERT( pMap1->GetArraySize()     == pMap2->GetArraySize() );

	pOverlappedMap->Clear();
	pOverlappedMap->SetRegionManager( pMap1->GetRegionManager() );

	long iSize = pMap1->GetArraySize();
	for( long iRegion=1; iRegion<iSize; iRegion++ )
	{
		if( pMap1->GetRegionResult( iRegion ) != NULL &&
			pMap1->GetRegionResult( iRegion ) == pMap2->GetRegionResult( iRegion ) )
		{
			CEOSAIRegionPathwayItem* pItem1 = pMap1->GetRegionResult( iRegion );
			CEOSAIRegionPathwayItem* pItem2 = pMap2->GetRegionResult( iRegion );

			// Create a new Object
			CEOSAIRegionPathwayItem* pNewItem = new CEOSAIRegionPathwayItem();
			pNewItem->m_fDanger = max( pItem1->m_fDanger, pItem2->m_fDanger );
			pNewItem->m_fTime   = max( pItem1->m_fTime, pItem2->m_fTime );
			pNewItem->m_fPathfindingValue = max( pItem1->m_fPathfindingValue, pItem2->m_fPathfindingValue );
			pOverlappedMap->AddRegionPathwayItem( pNewItem );
		}
	}
	for( long iRegion=1; iRegion<iSize; iRegion++ )
	{
		// Add Border Regions
		CEOSAIRegionPathwayItem* pItem1 = pMap1->GetRegionResult( iRegion );
		if( pItem1 != NULL )
		{
			CEOSAIGenericRegion* pRegion1 = pMap1->GetRegionManager()->GetGenericRegion( iRegion );
			POSITION pos = pRegion1->m_AdjacentRegions.GetHeadPosition();
			while( pos )
			{
				CEOSAIGenericRegion* pAdjRegion = pRegion1->m_AdjacentRegions.GetNext( pos );
				if( pMap1->GetRegionResult( pAdjRegion->m_iId ) == NULL &&
					pMap2->GetRegionResult( pAdjRegion->m_iId ) != NULL )
				{
					CEOSAIRegionPathwayItem* pItem2 = pMap2->GetRegionResult( pAdjRegion->m_iId );

					// Add it
					CEOSAIRegionPathwayItem* pNewItem = pOverlappedMap->GetRegionResult( pAdjRegion->m_iId );
					if( pNewItem == NULL )
					{
						pNewItem = CEOSAIRegionPathwayItem();
						pNewItem->m_fDanger = max( pItem1->m_fDanger, pItem2->m_fDanger );
						pNewItem->m_fTime   = max( pItem1->m_fTime, pItem2->m_fTime );
						pNewItem->m_fPathfindingValue = max( pItem1->m_fPathfindingValue, pItem2->m_fPathfindingValue );
						pOverlappedMap->AddRegionPathwayItem( pNewItem );
					}
					/-*
					else
					{
						pNewItem->m_fDanger = max( pItem1->m_fDanger, pNewItem->m_fDanger );
						pNewItem->m_fTime   = max( pItem1->m_fTime, pNewItem->m_fTime );
						pNewItem->m_fPathfindingValue = max( pItem1->m_fPathfindingValue, pNewItem->m_fPathfindingValue );
					}
					//
					pOverlappedMap->AddRegionPathwayItem( pNewItem );
					*-/
				}
			}
			/-*
			CEOSAIRegionPathwayItem* pItem1 = pMap1->GetRegionResult( iRegion );
			CEOSAIRegionPathwayItem* pItem2 = pMap2->GetRegionResult( iRegion );

			// Create a new Object
			CEOSAIRegionPathwayItem* pNewItem = CEOSAIRegionPathwayItem();
			pNewItem->m_fDanger = max( pItem1->m_fDanger, pItem2->m_fDanger );
			pNewItem->m_fTime   = max( pItem1->m_fTime, pItem2->m_fTime );
			pNewItem->m_fPathfindingValue = max( pItem1->m_fPathfindingValue, pItem2->m_fPathfindingValue );
			pOverlappedMap->AddRegionPathwayItem( pNewItem );
			*-/
		}
	}
}
*/
CEOSAIRegionPathfinder::CEOSAIRegionPathfinder()
{
	//m_pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//ASSERT( m_pWorldDescServer );
	m_bDebug_IsBeingSetup = false;

	m_iCurrentKey = 1;
	m_pRegionManager = NULL;
	//
	m_iPlayer = 0;

//	m_eNeutralStrategy = IgnoreNeutrals;
//	m_eEnemyStrategy = IgnoreEnemies;
	//m_bAddBlockingUnits = false;
/*
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();

	m_PlayerWeights.SetSize( iNumberOfPlayers+1 );
	m_PlayerWeights[0] = NULL;
	for( long i=1; i<iNumberOfPlayers+1; i++ )
	{
		m_PlayerWeights[i] = new CEOSAIRegionPathfinderPlayerWeights();
		m_PlayerWeights[i]->Clear();
	}

	m_OwnerForeignRelations.SetSize( iNumberOfPlayers+1 );
	for( long i=0; i<iNumberOfPlayers; i++ )
	{
		m_OwnerForeignRelations[ i ] = EOSAIEnumForeignRelations::enum_Neutral;
	}
*/
	/*
	long iNumberOfPlayers = m_pWorldDescServer->GetWorldBuildDesc()->GetNumberOfPlayers();
	//
	m_PlayerWeights.SetSize( iNumberOfPlayers+1 );
	for( long i=1; i<iNumberOfPlayers+1; i++ )
	{
		m_PlayerWeights[i] = new CEOSAIRegionPathfinderPlayerWeights();
	}
	*/
	/*
	m_PlayerPresencePathfindingMultiplier.SetSize( iNumberOfPlayers+1 );
	m_PlayerDangerPathfindingMultiplier.SetSize( iNumberOfPlayers+1 );
	m_PlayerSightingPathfindingMultiplier.SetSize( iNumberOfPlayers+1 );
	m_PlayerPresenceResultMultiplier.SetSize( iNumberOfPlayers+1 );
	m_PlayerDangerResultMultiplier.SetSize( iNumberOfPlayers+1 );
	m_PlayerSightingResultMultiplier.SetSize( iNumberOfPlayers+1 );
	*/
}

//
CEOSAIRegionPathfinder::~CEOSAIRegionPathfinder()
{
	for( long i=1; i<m_PlayerWeights.m_iSize; i++ )
	{
		delete m_PlayerWeights[i];
		m_PlayerWeights[i] = NULL;
	}
}

void CEOSAIRegionPathfinder::SetNumberOfPlayers( long iNumberOfPlayers )
{
	//long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();

	m_PlayerWeights.SetSize( iNumberOfPlayers+1 );
	m_PlayerWeights[0] = NULL;
	for( long i=1; i<iNumberOfPlayers+1; i++ )
	{
		m_PlayerWeights[i] = new CEOSAIRegionPathfinderPlayerWeights();
		m_PlayerWeights[i]->Clear();
	}

	m_OwnerForeignRelations.SetSize( iNumberOfPlayers+1 );
	for( long i=0; i<iNumberOfPlayers; i++ )
	{
		m_OwnerForeignRelations[ i ] = EOSAIEnumForeignRelations::enum_Neutral;
	}
}

/*
void CEOSAIRegionPathfinder::SetNumberOfPlayers( long iNumberOfPlayers )
{
}
*/

void CEOSAIRegionPathfinder::SetPlayer( long iPlayer )
{
	m_iPlayer = iPlayer;
/*
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();

	ASSERT( m_PlayerWeights.m_iSize == 0 );
	{
		m_OwnerForeignRelations.SetSize( iNumberOfPlayers+1 );
		m_PlayerWeights.SetSize( iNumberOfPlayers+1 );
		for( long i=1; i<iNumberOfPlayers+1; i++ )
		{
			m_PlayerWeights[i] = new CEOSAIRegionPathfinderPlayerWeights();
		}
	}
*/
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	for( long i=0; i<m_OwnerForeignRelations.m_iSize; i++ )
	{
		m_OwnerForeignRelations[i] = g_pEOSAICommonData->GetForeignRelations( iPlayer, i );
	}
}

void  CEOSAIRegionPathfinder::SetForeignRelations( CEOSAIBCDumbArray1D< EOSAIEnumForeignRelations >* pForeignRelations ) //m_OwnerForeignRelations;
{
	ASSERT( m_OwnerForeignRelations.m_iSize == pForeignRelations->m_iSize );
	for( long i=0; i<m_OwnerForeignRelations.m_iSize; i++ )
	{
		m_OwnerForeignRelations[i] = pForeignRelations->Value(i);
	}
}

//

void CEOSAIRegionPathfinder::ClearPathfindingSettings()
{
	ASSERT( m_bDebug_IsBeingSetup == false );
	m_bDebug_IsBeingSetup = true;

	//m_pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//ASSERT( m_pWorldDescServer );

	m_iPlayer = 0;

//	m_eNeutralStrategy = IgnoreNeutrals;
//	m_eEnemyStrategy = IgnoreEnemies;
	//m_bAddBlockingUnits = false;

	/*
	if( m_PlayerWeights.m_iSize == 0 )
	{
		m_pWorldDescServer = GetCommonState()->GetWorldDescServer();
		long iNumberOfPlayers = m_pWorldDescServer->GetWorldBuildDesc()->GetNumberOfPlayers();
	}
	*/
	for( long i=1; i<m_PlayerWeights.m_iSize; i++ )
	{
		m_PlayerWeights[i]->Clear();
	}
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	for( long i=0; i<m_OwnerForeignRelations.m_iSize; i++ )
	{
		m_OwnerForeignRelations[i] = g_pEOSAICommonData->GetForeignRelations( m_iPlayer, i );
	}

	m_TerrainBasedMovementRates.SetAllValuesToOne();
	//
	m_fMaximumDistance = 1000000.0f;
	m_fLandDistanceMultiplier = 1.0f;
	m_fSeaDistanceMultiplier = 1.0f;
	//
	m_bTreatInitialCitiesAsLand = false;
	m_bTreatInitialCitiesAsWater = false;
	m_fGeoSwitchMultiplier = 0.0f;
	m_iMaxGeoSwitches = -1;
	//
	m_CurrentPathfindingRegions.RemoveAll();

	CreateNewKey();
}


void CEOSAIRegionPathfinder::ClearPlayerWeights()
{
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		m_PlayerWeights[iOtherPlayer]->Clear();
	}
}

void CEOSAIRegionPathfinder::ApplyStrategy( EnumNeutralsPathwayType eNeutralStrategy, EnumEnemiesPathwayType eEnemyStrategy, float fAIPower )
{
	fAIPower = max( fAIPower, 0.1f );
	//pRegionPathfinder->ApplyStrategy( m_eNeutralsPathwayStategy, m_eEnemiesPathwayStategy, 1.0f );

//	m_eNeutralStrategy = eNeutralStrategy;
//	m_eEnemyStrategy = eEnemyStrategy;

	ASSERT( GetPlayer() > 0 );
	//
	ClearPlayerWeights();
	SetEnemyDangerResultMultiplier(   1.0f );
	SetEnemyPresenceResultMultiplier( 1.0f );
	SetEnemySightingResultMultiplier( 1.0f );

	if( eNeutralStrategy == IgnoreNeutrals )
	{
		SetNeutralDangerPathfindingMultiplier(   0.0f );
		SetNeutralPresencePathfindingMultiplier( 0.0f );
		SetNeutralSightingPathfindingMultiplier( 0.0f );
	}
	if( eNeutralStrategy == SemiPoliteToNeutrals )
	{
		SetNeutralDangerPathfindingMultiplier(   0.0f );
		SetNeutralPresencePathfindingMultiplier( 100.0f );
		SetNeutralSightingPathfindingMultiplier( 0.0f );
	}
	if( eNeutralStrategy == PoliteToNeutrals )
	{
		SetNeutralDangerPathfindingMultiplier(   0.0f );
		SetNeutralPresencePathfindingMultiplier( 100.0f );
		SetNeutralSightingPathfindingMultiplier( 0.0f );
	}

	if( eEnemyStrategy == IgnoreEnemies )
	{
		SetEnemyDangerPathfindingMultiplier(   0.0f );
		SetEnemyPresencePathfindingMultiplier( 0.0f );
		SetEnemySightingPathfindingMultiplier( 0.0f );
	}
	if( eEnemyStrategy == AllowSomeEnemyContact )
	{
		SetEnemyDangerPathfindingMultiplier(   200.0f/fAIPower );
		SetEnemyPresencePathfindingMultiplier( 200.0f );
		SetEnemySightingPathfindingMultiplier( 0.0f );
	}
	if( eEnemyStrategy == AvoidEnemyContact )
	{
		SetEnemyDangerPathfindingMultiplier(   200.0f/fAIPower );
		SetEnemyPresencePathfindingMultiplier( 200.0f );
		SetEnemySightingPathfindingMultiplier( 200.0f );
	}
}

void CEOSAIRegionPathfinder::SetToPolitePathway( long iPlayer )
{
//	m_eNeutralStrategy = PoliteToNeutrals;
//	m_eEnemyStrategy = AvoidEnemyContact;

	SetPathfindingAllyNeutralEnemyPresenceMultiplier( iPlayer, 25.0f, 800.0f, 800.0f ); // 0-1
	SetPathfindingAllyNeutralEnemyDangerMultiplier(   iPlayer,  0.0f,   0.0f,  50.0f ); // 0-infinity
	SetPathfindingAllyNeutralEnemySightingMultiplier( iPlayer,  0.0f, 100.0f, 800.0f ); // 0-1

	SetResultAllyNeutralEnemyPresenceMultiplier( iPlayer, 0.0f, 0.0f, 1.0f ); // 0-1
	SetResultAllyNeutralEnemyDangerMultiplier(   iPlayer, 0.0f, 0.0f, 1.0f ); // 0-infinity
	SetResultAllyNeutralEnemySightingMultiplier( iPlayer, 0.0f, 0.0f, 1.0f ); // 0-1
}

void CEOSAIRegionPathfinder::SetToIgnoreOtherPlayersPathway( long iPlayer )
{
//	m_eNeutralStrategy = IgnoreNeutrals;
//	m_eEnemyStrategy = IgnoreEnemies;

	SetPathfindingAllyNeutralEnemyPresenceMultiplier( iPlayer, 0.0f, 0.0f, 0.0f ); // 0-1
	SetPathfindingAllyNeutralEnemyDangerMultiplier(   iPlayer, 0.0f, 0.0f, 0.0f ); // 0-infinity
	SetPathfindingAllyNeutralEnemySightingMultiplier( iPlayer, 0.0f, 0.0f, 0.0f ); // 0-1

	SetResultAllyNeutralEnemyPresenceMultiplier( iPlayer, 0.0f, 0.0f, 1.0f ); // 0-1
	SetResultAllyNeutralEnemyDangerMultiplier(   iPlayer, 0.0f, 0.0f, 1.0f ); // 0-infinity
	SetResultAllyNeutralEnemySightingMultiplier( iPlayer, 0.0f, 0.0f, 1.0f ); // 0-1
}

void CEOSAIRegionPathfinder::SetToDangerAvoidancePathway( long iPlayer )
{
//	m_eNeutralStrategy = IgnoreNeutrals;
//	m_eEnemyStrategy = AvoidEnemyContact;

	SetPathfindingAllyNeutralEnemyPresenceMultiplier( iPlayer, 0.0f, 0.0f, 100.0f ); // 0-1
	SetPathfindingAllyNeutralEnemyDangerMultiplier(   iPlayer, 0.0f, 0.0f, 100.0f ); // 0-infinity
	SetPathfindingAllyNeutralEnemySightingMultiplier( iPlayer, 0.0f, 0.0f, 800.0f ); // 0-1

	SetResultAllyNeutralEnemyPresenceMultiplier( iPlayer, 0.0f, 0.0f, 1.0f ); // 0-1
	SetResultAllyNeutralEnemyDangerMultiplier(   iPlayer, 0.0f, 0.0f, 1.0f ); // 0-infinity
	SetResultAllyNeutralEnemySightingMultiplier( iPlayer, 0.0f, 0.0f, 1.0f ); // 0-1
}

//
void CEOSAIRegionPathfinder::SetEnemyDangerPathfindingMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsEnemy() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerPathfindingMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetEnemyPresencePathfindingMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsEnemy() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerPresencePathfindingMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetEnemySightingPathfindingMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsEnemy() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingPathfindingMultiplier = fWeight; }
	}
}

//
void CEOSAIRegionPathfinder::SetEnemyDangerResultMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsEnemy() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerResultMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetEnemyPresenceResultMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsEnemy() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerPresenceResultMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetEnemySightingResultMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsEnemy() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingResultMultiplier = fWeight; }
	}
}

//
void CEOSAIRegionPathfinder::SetNeutralDangerPathfindingMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsNeutral() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerPathfindingMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetNeutralPresencePathfindingMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsNeutral() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerPresencePathfindingMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetNeutralSightingPathfindingMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsNeutral() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingPathfindingMultiplier = fWeight; }
	}
}

//
void CEOSAIRegionPathfinder::SetNeutralDangerResultMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsNeutral() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerResultMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetNeutralPresenceResultMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsNeutral() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerPresenceResultMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetNeutralSightingResultMultiplier( float fWeight )
{
	ASSERT( m_iPlayer > 0 );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iMovingPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations.IsNeutral() ){ m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingResultMultiplier = fWeight; }
	}
}

void CEOSAIRegionPathfinder::SetPathfindingAllyNeutralEnemyPresenceMultiplier( long iPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult )
{
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		if( iPlayer == iOtherPlayer )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresencePathfindingMultiplier = 0.0f;
			continue;
		}

		// What's my foreign relations towards him?
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations >= EOSAIEnumForeignRelations::enum_Alliance )//eRelations.IsAlly() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresencePathfindingMultiplier = fAllyMult;
		}
		if( eRelations.IsNeutral() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresencePathfindingMultiplier = fNeutralMult;
		}
		if( eRelations.IsEnemy() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresencePathfindingMultiplier = fEnemyMult;
		}
	}
}

void CEOSAIRegionPathfinder::SetPathfindingAllyNeutralEnemyDangerMultiplier( long iPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult )
{
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		if( iPlayer == iOtherPlayer )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresencePathfindingMultiplier = 0.0f;
			continue;
		}

		// What's my foreign relations towards him?
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations >= EOSAIEnumForeignRelations::enum_Alliance )//eRelations.IsAlly() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerPathfindingMultiplier = fAllyMult;
		}
		if( eRelations.IsNeutral() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerPathfindingMultiplier = fNeutralMult;
		}
		if( eRelations.IsEnemy() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerPathfindingMultiplier = fEnemyMult;
		}
	}
}


void CEOSAIRegionPathfinder::SetPathfindingAllyNeutralEnemySightingMultiplier( long iPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult )
{
	ASSERT( m_CurrentPathfindingRegions.IsEmpty() );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		if( iPlayer == iOtherPlayer )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresencePathfindingMultiplier = 0.0f;
			continue;
		}

		// What's my foreign relations towards him?
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations >= EOSAIEnumForeignRelations::enum_Alliance )//eRelations.IsAlly() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingPathfindingMultiplier = fAllyMult;
		}
		if( eRelations.IsNeutral() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingPathfindingMultiplier = fNeutralMult;
		}
		if( eRelations.IsEnemy() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingPathfindingMultiplier = fEnemyMult;
		}
	}
}

//

void CEOSAIRegionPathfinder::SetResultAllyNeutralEnemyPresenceMultiplier( long iPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult )
{
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		if( iPlayer == iOtherPlayer )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresenceResultMultiplier = 0.0f;
			continue;
		}

		// What's my foreign relations towards him?
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations >= EOSAIEnumForeignRelations::enum_Alliance )//eRelations.IsAlly() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresenceResultMultiplier = fAllyMult;
		}
		if( eRelations.IsNeutral() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresenceResultMultiplier = fNeutralMult;
		}
		if( eRelations.IsEnemy() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresenceResultMultiplier = fEnemyMult;
		}
	}
}

void CEOSAIRegionPathfinder::SetResultAllyNeutralEnemyDangerMultiplier( long iPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult )
{
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		if( iPlayer == iOtherPlayer )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresenceResultMultiplier = 0.0f;
			continue;
		}

		// What's my foreign relations towards him?
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations >= EOSAIEnumForeignRelations::enum_Alliance )//eRelations.IsAlly() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerResultMultiplier = fAllyMult;
		}
		if( eRelations.IsNeutral() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerResultMultiplier = fNeutralMult;
		}
		if( eRelations.IsEnemy() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerDangerResultMultiplier = fEnemyMult;
		}
	}
}

void CEOSAIRegionPathfinder::SetResultAllyNeutralEnemySightingMultiplier( long iPlayer, float fAllyMult, float fNeutralMult, float fEnemyMult )
{
	ASSERT( m_CurrentPathfindingRegions.IsEmpty() );
	for( long iOtherPlayer=1; iOtherPlayer<m_PlayerWeights.m_iSize; iOtherPlayer++ )
	{
		if( iPlayer == iOtherPlayer )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerPresenceResultMultiplier = 0.0f;
			continue;
		}

		// What's my foreign relations towards him?
		//EOSAIEnumForeignRelations eRelations = m_pWorldDescServer->GetForeignRelations( iPlayer, iOtherPlayer );
		EOSAIEnumForeignRelations eRelations = m_OwnerForeignRelations[iOtherPlayer];
		if( eRelations >= EOSAIEnumForeignRelations::enum_Alliance )//eRelations.IsAlly() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingResultMultiplier = fAllyMult;
		}
		if( eRelations.IsNeutral() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingResultMultiplier = fNeutralMult;
		}
		if( eRelations.IsEnemy() )
		{
			m_PlayerWeights[iOtherPlayer]->m_fPlayerSightingResultMultiplier = fEnemyMult;
		}
	}
}

void CEOSAIRegionPathfinder::SetMovementRate( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	ASSERT( pAIUnitTemplate );
	float fMovementRate = pAIUnitTemplate->GetMovementRate();
	m_TerrainBasedMovementRates = pAIUnitTemplate->GetMovementDescription()->m_MovementRateMultiplier;
	m_TerrainBasedMovementRates.MultiplyAllValuesBy( fMovementRate );
}

void CEOSAIRegionPathfinder::AddStartLocation( CEOSAILocation Location )
{
	ASSERT( m_pRegionManager );
	CEOSAIGenericRegion* pRegion = m_pRegionManager->GetGenericRegion( Location );
	ASSERT( pRegion );
	AddStartingRegion( pRegion );
}

void CEOSAIRegionPathfinder::AddStartingRegion( CEOSAIGenericRegion* pRegion )
{
	#ifdef _DEBUG
	POSITION pos = m_StartingPathfindingRegions.GetHeadPosition();
	while( pos )
	{
		//CEOSAIGenericRegion* pRegionInList = m_StartingPathfindingRegions.GetNext( pos );
		CEOSAIPathfinderPoint* pPathfindingPointInList = m_StartingPathfindingRegions.GetNext( pos );
		if( pPathfindingPointInList == pRegion ){ ASSERT( false ); return; }
	}
	#endif

	m_StartingPathfindingRegions.AddTail( pRegion );
}

void CEOSAIRegionPathfinder::AddStartingRegion( long iRegion )
{
	CEOSAIGenericRegion* pRegion = m_pRegionManager->GetGenericRegion( iRegion );
	//CEOSAIPathfinderPoint* pPoint = m_pRegionManager->GetPathfinderPoint( iRegion );

	#ifdef _DEBUG
	POSITION pos = m_StartingPathfindingRegions.GetHeadPosition();
	while( pos )
	{
		CEOSAIGenericRegion* pRegionInList = m_StartingPathfindingRegions.GetNext( pos );
		//CEOSAIPathfinderPoint* pPathfinderPointInList = m_StartingPathfindingRegions.GetNext( pos );
		if( pRegionInList == pRegion ){ ASSERT( false ); return; }
	}
	#endif

	//m_StartingPathfindingRegions.AddTail( pPoint );
	m_StartingPathfindingRegions.AddTail( pRegion );
}

void CEOSAIRegionPathfinder::AddStartingPathfinderPoint( long iPoint )
{
	CEOSAIPathfinderPoint* pPoint = m_pRegionManager->GetPathfinderPoint( iPoint );
	ASSERT( pPoint );
	if( pPoint )
	{
		CEOSAIGenericRegion* pGenericRegion = dynamic_cast< CEOSAIGenericRegion* >( pPoint );
		if( pGenericRegion )
		{
			AddStartingRegion( pGenericRegion );
		}
		else
		{
			AddStartingRegionPathwayItem( pPoint );
		}
	}
}

void CEOSAIRegionPathfinder::AddStartingRegionPathwayItem( CEOSAIPathfinderPoint* pPathfinderPoint )
{
	CEOSAIRegionPathwayItem* pStartItem = new CEOSAIRegionPathwayItem();
	pStartItem->Clear();
	pStartItem->m_pPathfinderPoint = pPathfinderPoint;
	m_StartingPathwayItems.AddTail( pStartItem );
}

void CEOSAIRegionPathfinder::AddStartingRegionPathwayItemWithTimeAndPathfindingValue( CEOSAIRegionPathwayItem* pStartItem )
{
	m_StartingPathwayItems.AddTail( pStartItem );
}

void CEOSAIRegionPathfinder::AddStartingGeo( long iGeo )
{
	POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIGenericRegion* pRegion = m_pRegionManager->m_GenericRegionList.GetNext( pos );
		if( pRegion->m_iGeo == iGeo )
		{
			AddStartingRegion( pRegion );
		}
	}
}

void CEOSAIRegionPathfinder::AddToStartRegions( long iPlayer, long iRegionDefines )
{
	// EnumStartRegions_MyCities
	//long iPlayer = m_pAIBrain->GetAIPlayerNumber();
	//for( long i=0; i<m_pRegions->GetSize(); i++ )
	//CArray< CEOSAIGenericRegion* >* pRegions = m_pRegionManager->GetGenericRegionArray();
	//for( long i=0; i<pRegions->GetSize(); i++ )
	POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		//CEOSAIGenericRegion* pMultiRegion = m_pRegions->GetAt(i);
		//CEOSAIGenericRegion* pMultiRegion = pRegions->GetAt(i);
		CEOSAIGenericRegion* pRegion = m_pRegionManager->m_GenericRegionList.GetNext( pos );
		if( pRegion )
		{
			if( pRegion->GetCitResUnitCount( iPlayer ) > 0 )
			{
				POSITION pos = pRegion->GetPoiObjectsInsideRegion()->GetHeadPosition();
				while( pos )
				{
					//bool bThisMultiRegionWasAdded = false;
					CEOSAIPoiObject* pAIPoiObject = pRegion->GetPoiObjectsInsideRegion()->GetNext( pos );
					if( pAIPoiObject->GetOwner() == iPlayer )
					{
						if( iRegionDefines & MULTIREGIONS_CITIES )
						{
							CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
							if( pAICity )
							{
								AddStartingRegion( pRegion );
								break;
							}
						}
						if( iRegionDefines & MULTIREGIONS_AIRFIELDS )
						{
							CEOSAIAirfield* pAIAirfield = dynamic_cast< CEOSAIAirfield* >( pAIPoiObject );
							if( pAIAirfield )
							{
								AddStartingRegion( pRegion );
								break;
							}
						}
						CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
						if( pAIUnit )
						{
							// Ships
							if( iRegionDefines & MULTIREGIONS_CARRIERS && pAIUnit->GetAIUnitTemplate()->IsAirUnitCarrier() )
							{
								AddStartingRegion( pRegion );
								break;
							}
							if( iRegionDefines & MULTIREGIONS_TRANSPORTS && pAIUnit->GetAIUnitTemplate()->IsTransport() )
							{
								AddStartingRegion( pRegion );
								break;
							}
							if( iRegionDefines & MULTIREGIONS_SEARESHUNTERS && pAIUnit->GetAIUnitTemplate()->IsSeaResHunter() )
							{
								AddStartingRegion( pRegion );
								break;
							}
							// Ground Units
							if( iRegionDefines & MULTIREGIONS_CITYHUNTERS && pAIUnit->GetAIUnitTemplate()->IsCityHunter() )
							{
								AddStartingRegion( pRegion );
								break;
							}
							// Air Units
							if( iRegionDefines & MULTIREGIONS_AIRUNITS && pAIUnit->GetAIUnitTemplate()->IsAirUnit() )
							{
								AddStartingRegion( pRegion );
								break;
							}
						}
					}
				}
			}
		}
	}
}


void CEOSAIRegionPathfinder::AddPlayerCitResUnitRegionsAsStartRegions( long iPlayer )
{
	bool bSomeRegionWasAdded = false;
	POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
	while( pos )
	//CArray< CEOSAIGenericRegion* >* pRegions = m_pRegionManager->GetGenericRegionArray();
	//for( long i=0; i<pRegions->GetSize(); i++ )
	{
		//CEOSAIGenericRegion* pMultiRegion = pRegions->GetAt(i);
		CEOSAIGenericRegion* pRegion = m_pRegionManager->m_GenericRegionList.GetNext( pos );
		if( pRegion )
		{
			if( pRegion->GetCitResUnitCount( iPlayer ) > 0 )
			{
				bSomeRegionWasAdded = true;
				AddStartingRegion( pRegion );
			}
		}
	}
	ASSERT( bSomeRegionWasAdded );
}

void CEOSAIRegionPathfinder::AddPlayerCitResRegionsAsStartRegions( long iPlayer )
{
	bool bSomeRegionWasAdded = false;
	//CArray< CEOSAIGenericRegion* >* pRegions = m_pRegionManager->GetGenericRegionArray();
	POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
	//for( long i=0; i<pRegions->GetSize(); i++ )
	while( pos )
	{
		//CEOSAIGenericRegion* pMultiRegion = pRegions->GetAt(i);
		CEOSAIGenericRegion* pRegion = m_pRegionManager->m_GenericRegionList.GetNext( pos );
		if( pRegion )
		{
			//if( pRegion->GetCitResUnitSummary( iPlayer )->GetCitResCount() > 0 )
			if( pRegion->GetCitResCount( iPlayer ) > 0 )
			{
				bSomeRegionWasAdded = true;
				AddStartingRegion( pRegion );
			}
		}
	}
	ASSERT( bSomeRegionWasAdded );
}

void CEOSAIRegionPathfinder::AddPlayerCitGroundUnitRegionsAsStartRegions( long iPlayer )
{
	//long iPlayer = GetAIBrain()->GetAIPlayerNumber();
	//CArray< CEOSAIGenericRegion* >* pRegions = m_pRegionManager->GetGenericRegionArray();
	//for( long i=0; i<pRegions->GetSize(); i++ )
	POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		//CEOSAIGenericRegion* pMultiRegion = pRegions->GetAt(i);
		CEOSAIGenericRegion* pRegion = m_pRegionManager->m_GenericRegionList.GetNext( pos );
		if( pRegion )
		{
			if( pRegion->GetCitGroundUnitCount( iPlayer ) > 0 )
			{
				AddStartingRegion( pRegion );
			}
		}
	}
}


void CEOSAIRegionPathfinder::AddEndLocation( CEOSAILocation Location )
{
	ASSERT( m_pRegionManager );
	CEOSAIGenericRegion* pRegion = m_pRegionManager->GetGenericRegion( Location );
	ASSERT( pRegion );
	//AddEndingRegion( pRegion );
	pRegion->m_iTempEndKey = m_iCurrentKey;
}

void CEOSAIRegionPathfinder::AddEndingRegion( long iRegion )
{
	CEOSAIGenericRegion* pRegion = m_pRegionManager->GetGenericRegion( iRegion );
	ASSERT( pRegion );
	AddEndingRegion( pRegion );
}

void CEOSAIRegionPathfinder::AddEndingRegion( CEOSAIGenericRegion* pRegion )
{
	pRegion->m_iTempEndKey = m_iCurrentKey;
}

void CEOSAIRegionPathfinder::AddEndingGeo( long iGeo )
{
	POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIGenericRegion* pRegion = m_pRegionManager->m_GenericRegionList.GetNext( pos );
		if( pRegion->m_iGeo == iGeo )
		{
			pRegion->m_iTempEndKey = m_iCurrentKey;
		}
	}
}


void CEOSAIRegionPathfinder::Solve( long iPlayer, bool bStartWithLandMovementFromCities )
{
	ASSERT( m_bDebug_IsBeingSetup == true );
	m_bDebug_IsBeingSetup = false;

	//CEOSAIStopwatch2  Stopwatch1a;
	//Stopwatch1a.Start();

	m_iPlayer = iPlayer;

	ASSERT( m_StartingPathfindingRegions.IsEmpty() == FALSE );
	if( m_StartingPathfindingRegions.IsEmpty() ){ return; }
	while( m_StartingPathfindingRegions.IsEmpty() == FALSE )
	{
		CEOSAIPathfinderPoint* pPathfinderPoint = m_StartingPathfindingRegions.RemoveTail();
		CEOSAIGenericRegion* pRegion = dynamic_cast< CEOSAIGenericRegion* >( pPathfinderPoint );
		ASSERT( pRegion );
		if( pRegion )
		{
			/*
			if( m_iPlayer == 0 )
			{
				pRegion->m_fTempPathfindingValue = 0.0;

				pRegion->m_fTempTotalDistance = 0.0f;
				pRegion->m_fTempTotalDanger = 0.0f;
				pRegion->m_fTempTotalPresence = 0.0f;
				pRegion->m_fTempTotalSighting = 0.0f;
			}
			else
			*/
			{
				float f1 = GetAdjustedDangerValueOfRegion( pRegion );
				float f2 = GetAdjustedPresenceValueOfRegion( pRegion );
				float f3 = GetAdjustedSightingValueOfRegion( pRegion );
				pRegion->m_fTempPathfindingValue = 
					GetAdjustedDangerValueOfRegion( pRegion ) +
					GetAdjustedPresenceValueOfRegion( pRegion ) +
					GetAdjustedSightingValueOfRegion( pRegion );

				pRegion->m_fTempTotalDistance = 0.0f;
				pRegion->m_fTempTotalDanger = GetRealDangerValueOfRegion( pRegion );
				pRegion->m_fTempTotalPresence = GetRealPresenceValueOfRegion( pRegion );
				pRegion->m_fTempTotalSighting = GetRealSightingValueOfRegion( pRegion );
			}
			pRegion->m_iTempNumberOfGeoSwitches = 0;
			pRegion->m_iTempIncomingTerrain = pRegion->m_iTerrainType3;
			if( pRegion->m_iTerrainType3 == TERRAIN_CITY )
			{
				CEOSAICity* pAICity = pRegion->GetEOSAICity();
				ASSERT( pAICity );
				if( pAICity )
				{
					if( pAICity->IsOwnedByOrAlliedTo( iPlayer ) == false )
					{
						pRegion->m_iTempIncomingTerrain = TERRAIN_GRASS;
					}
					else if( bStartWithLandMovementFromCities )
					{
						pRegion->m_iTempIncomingTerrain = TERRAIN_GRASS;
					}
					else
					{
						pRegion->m_iTempIncomingTerrain = TERRAIN_WATER;
					}
				}
			}
			pRegion->m_iTempVisitedKey = m_iCurrentKey;
			pRegion->m_bTempIsInTheList = true;
			pRegion->m_pTempPreviousStep = NULL;

			ASSERT( pRegion->m_fTempPathfindingValue >= 0.0f );
		}
		/*
		else
		{
			pPathfinderPoint->m_fTempPathfindingValue = 0.0f;
			pPathfinderPoint->m_fTempTotalDistance = 0.0f;
			pPathfinderPoint->m_fTempTotalDanger = 0.0f;
			pPathfinderPoint->m_fTempTotalPresence = 0.0f;
			pPathfinderPoint->m_fTempTotalSighting = 0.0f;
			pPathfinderPoint->m_iTempNumberOfGeoSwitches = 0;
			pPathfinderPoint->m_iTempIncomingTerrain = pPathfinderPoint->m_iTerrainType3;

			pPathfinderPoint->m_iTempVisitedKey = m_iCurrentKey;
			pPathfinderPoint->m_bTempIsInTheList = true;
			pPathfinderPoint->m_pTempPreviousStep = NULL;

			ASSERT( pPathfinderPoint->m_fTempPathfindingValue >= 0.0f );
		}
		*/

		m_CurrentPathfindingRegions.AddTail( pPathfinderPoint );
	}

	CEOSAIGenericRegion* pEndRegion = NULL;

	// Now "grow" those MultiRegions until I run-out MultiRegions
	while( m_CurrentPathfindingRegions.IsEmpty() == FALSE )
	{
		CEOSAIPathfinderPoint* pPathfinderPoint = m_CurrentPathfindingRegions.RemoveHead();
		CEOSAIGenericRegion* pRegion = dynamic_cast< CEOSAIGenericRegion* >( pPathfinderPoint );
		ASSERT( pRegion );
		//CEOSAIGenericRegion* pRegion = m_CurrentPathfindingRegions.RemoveHead();
		pRegion->m_bTempIsInTheList = false;

		long iOriginalLandCount = 0;
		long iCurrentLandCount = 0;
		if( pRegion->IsLand() || pRegion->IsCity() ){ iOriginalLandCount=1; }

		// Check the AIRoads
		/*
		POSITION pos = pRegion->m_RoadsOnTheRegionEdge.GetHeadPosition();
		while( pos )
		{
			CEOSAIRoad2* pAdj = pRegion->m_RoadsOnTheRegionEdge.GetNext( pos );

			// Distance
			float fAdditionalDistance = m_pWorldDescServer->GetPixelDistance( pRegion->GetLocation(), pAdj->GetLocation() );
		}
		*/
		// Check the Adjacent Regions
		POSITION pos = pRegion->m_AdjacentRegions.GetHeadPosition();
		while( pos )
		{
			CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pRegion->m_AdjacentRegions.GetNext( pos );
			CEOSAIGenericRegion* pAdj = pAdjRegionDesc->m_pRegion;
			//CEOSAIGenericRegion* pAdj = pRegion->m_AdjacentRegions.GetNext( pos );
			ASSERT( pRegion->m_fTempTotalDistance < 900000.0f );

			iCurrentLandCount = iOriginalLandCount;
			if( pAdj->IsLand() || pAdj->IsCity() ){ iCurrentLandCount++; }

			// Distance
			float fAdditionalDistance = g_pWorldDistanceTool->GetDistance( pRegion->GetLocation(), pAdj->GetLocation() );

			// Danger
			float fAdjustedAdditionalDanger = GetAdjustedDangerValueOfRegion( pAdj );

			// Presence (don't like to move through neutral-player territory, doesn't matter their power)
			float fAdjustedAdditionalPresence = GetAdjustedPresenceValueOfRegion( pAdj );

			// Sightings
			float fAdjustedAdditionalSightings = GetAdjustedSightingValueOfRegion( pAdj );

			// GeoSwitches
			long iAdditionalSwitches = 0;
			if( pRegion->m_iTerrainType3 != pAdj->m_iTerrainType3 )
			{
				// Cities that I can't pass through are considered to be land
				long iAdjTerrain = pAdj->m_iTerrainType3;
				if( iAdjTerrain == TERRAIN_CITY )
				{
					ASSERT( pAdj->GetCityObjectId() );
					if( iPlayer == 0 ||
						pAdj->GetEOSAICity()->IsOwnedByOrAlliedTo( iPlayer ) )
					{
						iAdjTerrain = TERRAIN_GRASS;
					}
				}

				if( pRegion->m_iTerrainType3 == TERRAIN_WATER &&
					iAdjTerrain == TERRAIN_GRASS )
				{
					iAdditionalSwitches=1;
				}
				if( pRegion->m_iTerrainType3 == TERRAIN_GRASS &&
					iAdjTerrain == TERRAIN_WATER )
				{
					iAdditionalSwitches=1;
				}
				if( pRegion->m_iTerrainType3 == TERRAIN_CITY &&
					pRegion->m_iTempIncomingTerrain == TERRAIN_GRASS &&
					iAdjTerrain == TERRAIN_WATER )
				{
					iAdditionalSwitches=1;
				}
				if( pRegion->m_iTerrainType3 == TERRAIN_CITY &&
					pRegion->m_iTempIncomingTerrain == TERRAIN_WATER &&
					iAdjTerrain == TERRAIN_GRASS )
				{
					iAdditionalSwitches=1;
				}
			}

			float fDistanceCost = 0.0f;
			if( iCurrentLandCount == 0 ){ fDistanceCost = m_fSeaDistanceMultiplier * fAdditionalDistance; }
			else if( iCurrentLandCount == 1 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance/2.0f + m_fSeaDistanceMultiplier * fAdditionalDistance/2.0f; }
			else if( iCurrentLandCount == 2 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance; }
			else{ ASSERT( false ); }

			float fNewPathfindingValue = 
				pRegion->m_fTempPathfindingValue +
				fDistanceCost +
				//(m_fDistanceMultiplier * fAdditionalDistance) +
				fAdjustedAdditionalDanger +
				fAdjustedAdditionalPresence +
				fAdjustedAdditionalSightings +
				(m_fGeoSwitchMultiplier * iAdditionalSwitches);

			if( m_iMaxGeoSwitches >= 0 )
			{
				if( pRegion->m_iTempNumberOfGeoSwitches + iAdditionalSwitches > m_iMaxGeoSwitches )
				{
					continue; // too many geo switches
				}
			}

			if( pAdj->m_iTempVisitedKey != m_iCurrentKey ||
				pAdj->m_fTempPathfindingValue > fNewPathfindingValue )
			{
				if( pRegion->m_fTempTotalDistance + fAdditionalDistance < m_fMaximumDistance )
				{
					bool bWasPreviouslyVisited = ( pAdj->m_iTempVisitedKey == m_iCurrentKey );
					if( bWasPreviouslyVisited == false )
					{
						pAdj->m_iTempVisitedKey = m_iCurrentKey;
						pAdj->m_bTempIsInTheList = false;
					}
					//
					pAdj->m_fTempPathfindingValue = fNewPathfindingValue;
					pAdj->m_fTempTotalDistance    = pRegion->m_fTempTotalDistance + fAdditionalDistance;
					pAdj->m_fTempTotalDanger      = pRegion->m_fTempTotalDanger + GetRealDangerValueOfRegion( pAdj );
					pAdj->m_fTempTotalPresence    = pRegion->m_fTempTotalPresence + GetRealPresenceValueOfRegion( pAdj );
					pAdj->m_fTempTotalSighting    = pRegion->m_fTempTotalSighting + GetRealSightingValueOfRegion( pAdj );
					pAdj->m_iTempNumberOfGeoSwitches = pRegion->m_iTempNumberOfGeoSwitches + iAdditionalSwitches;
					pAdj->m_pTempPreviousStep     = pRegion;
					pAdj->m_iTempIncomingTerrain  = pRegion->m_iTerrainType3;
					if( pAdj->m_iTempIncomingTerrain == TERRAIN_CITY )
					{
						pAdj->m_iTempIncomingTerrain = pRegion->m_iTempIncomingTerrain;
					}

					if( pAdj->m_iTempEndKey == m_iCurrentKey )
					{
						if( pEndRegion == NULL )
						{
							pEndRegion = pAdj;
						}
						else
						{
							if( pEndRegion->m_fTempPathfindingValue > pAdj->m_fTempPathfindingValue )
							{
								pEndRegion = pAdj;
							}
						}
					}

					ASSERT( pAdj->m_fTempPathfindingValue >= 0.0f );

					// Push it back into the CurrentList if there aren't targets
					m_CurrentPathfindingRegions.AddTail( pAdj );
					pAdj->m_bTempIsInTheList = true;
				}
			}
		}
	}

	//Stopwatch1a.Stop();
	//float fTime1a = Stopwatch1a.GetDeltaSeconds();
	int x=0;
}


//void CEOSAIRegionPathfinder::Solve2( bool bStartWithLandMovementFromCities )
void CEOSAIRegionPathfinder::Solve2( long iPlayer, bool bStartWithLandMovementFromCities )
{
	ASSERT( m_bDebug_IsBeingSetup == true );
	m_bDebug_IsBeingSetup = false;

	//CEOSAIStopwatch2  Stopwatch1a;
	//Stopwatch1a.Start();

	m_iPlayer = iPlayer;

	CList< CEOSAIPathfinderPoint* >  CurrentPathfindingRegions;

	ASSERT( m_StartingPathfindingRegions.IsEmpty() == FALSE );
	if( m_StartingPathfindingRegions.IsEmpty() ){ return; }
	while( m_StartingPathfindingRegions.IsEmpty() == FALSE )
	{
		CEOSAIPathfinderPoint* pPoint = m_StartingPathfindingRegions.RemoveTail();
		CEOSAIGenericRegion* pRegion = dynamic_cast< CEOSAIGenericRegion* >( pPoint );
		//CEOSAIGenericRegion* pRegion = m_StartingPathfindingRegions.RemoveTail();

		pRegion->m_fTempPathfindingValue = 
			GetAdjustedDangerValueOfRegion( pRegion ) +
			GetAdjustedPresenceValueOfRegion( pRegion ) +
			GetAdjustedSightingValueOfRegion( pRegion );
		pRegion->m_fTempTotalDistance = 0.0f;
		pRegion->m_fTempTotalTime = 0.0f;
		pRegion->m_fTempTotalDanger = GetRealDangerValueOfRegion( pRegion );
		pRegion->m_fTempTotalPresence = GetRealPresenceValueOfRegion( pRegion );
		pRegion->m_fTempTotalSighting = GetRealSightingValueOfRegion( pRegion );
		pRegion->m_iTempNumberOfGeoSwitches = 0;
		pRegion->m_iTempIncomingTerrain = pRegion->m_iTerrainType3;
		if( pRegion->m_iTerrainType3 == TERRAIN_CITY )
		{
			CEOSAICity* pAICity = pRegion->GetEOSAICity();
			ASSERT( pAICity );
			if( pAICity )
			{
				if( pAICity->IsOwnedByOrAlliedTo( m_iPlayer ) == false )
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_GRASS;
				}
				else if( bStartWithLandMovementFromCities )
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_GRASS;
				}
				else
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_WATER;
				}
			}
		}
		pRegion->m_iTempVisitedKey = m_iCurrentKey;
		pRegion->m_bTempIsInTheList = true;
		pRegion->m_pTempPreviousStep = NULL;

		CurrentPathfindingRegions.AddTail( pRegion );
	}

	POSITION pos;
	CEOSAIGenericRegion* pEndRegion = NULL;
	long iPathfinderChecks = 0;

	// Now "grow" those MultiRegions until I run-out MultiRegions
	while( CurrentPathfindingRegions.IsEmpty() == FALSE )
	{
		CEOSAIPathfinderPoint* pPathfinderStep = CurrentPathfindingRegions.RemoveHead();
		iPathfinderChecks++;

		long iOriginalLandCount = 0;
		long iCurrentLandCount = 0;
		pPathfinderStep->m_bTempIsInTheList = false;

		ASSERT( pPathfinderStep->m_fTempTotalDistance < 900000.0f );

		CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pPathfinderStep );
		if( pAIRoad )
		{
			iOriginalLandCount=1;

			// Collect the PathwaySteps
			CList< CEOSAIPathfinderPoint* >  PathwaySteps;
			pos = pAIRoad->m_RoadsIConnectTo.GetHeadPosition();
			while( pos )
			{
				CEOSAIAdjacentAIRoad2Distance* pRoadDistance = pAIRoad->m_RoadsIConnectTo.GetNext( pos );
				CEOSAIRoad2* pAdjRoad = pRoadDistance->m_pAIRoad;
				PathwaySteps.AddTail( pAdjRoad );
			}
			CEOSAIGenericRegion* pAIRegion1 = m_pRegionManager->GetGenericRegion( pAIRoad->m_iAIRegion1 );
			PathwaySteps.AddTail( pAIRegion1 );
			CEOSAIGenericRegion* pAIRegion2 = m_pRegionManager->GetGenericRegion( pAIRoad->m_iAIRegion2 );
			PathwaySteps.AddTail( pAIRegion2 );

			// AIRoad -> Region
			pos = PathwaySteps.GetHeadPosition();
			while( pos )
			{
				CEOSAIPathfinderPoint* pAdjStep = PathwaySteps.GetNext( pos );

				float fMovementRate = m_TerrainBasedMovementRates.m_fTerrainValue[ pAdjStep->m_iTerrainType9 ];
				if( fMovementRate <= 0.0f ) continue; // impassible

				float fAdditionalDistance = 0.0f;
				float fAdditionalTime = 0.0f;
				float fAdjustedAdditionalDanger = 0.0f;
				float fAdjustedAdditionalPresence = 0.0f;
				float fAdjustedAdditionalSightings = 0.0f;
				float fRealAdditionalDanger = 0.0f;
				float fRealAdditionalPresence = 0.0f;
				float fRealAdditionalSighting = 0.0f;

				long  iAdditionalSwitches = 0;
				long  iAdjTerrain3 = TERRAIN_GRASS;
				long  iAdjTerrain9 = TERRAIN_GRASS;
				float fDistanceCost = 0.0f;

				// AIRoad -> AIRoad
				CEOSAIRoad2* pAdjRoad = dynamic_cast< CEOSAIRoad2* >( pAdjStep );
				if( pAdjRoad )
				{
					CEOSAIAdjacentAIRoad2Distance* pRoadDistance = pAIRoad->GetRoadDistance( pAdjRoad );
					ASSERT( pRoadDistance );
					CEOSAIGenericRegion* pAdjRegion = pRoadDistance->m_pIntermediateRegion;

					// Distance/Time
					fAdditionalDistance = pRoadDistance->m_fPixelDistance;
					fAdditionalTime = fAdditionalDistance / fMovementRate;
					// Danger/Presence/Sightings
					fAdjustedAdditionalDanger = GetAdjustedDangerValueOfRegion( pAdjRegion );
					fAdjustedAdditionalPresence = GetAdjustedPresenceValueOfRegion( pAdjRegion );
					fAdjustedAdditionalSightings = GetAdjustedSightingValueOfRegion( pAdjRegion );
					fRealAdditionalDanger = GetRealDangerValueOfRegion( pAdjRegion );
					fRealAdditionalPresence = GetRealPresenceValueOfRegion( pAdjRegion );
					fRealAdditionalSighting = GetRealSightingValueOfRegion( pAdjRegion );
					// Adj Terrain
					iAdjTerrain3 = TERRAIN_GRASS;
					iAdjTerrain9 = TERRAIN_ROAD;

					// Distance Cost is only used in calculating distance
					//   It is not used in multiplying the Danger/Presence/Sightings values
					fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance;
				}
				// AIRoad -> Region
				CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
				if( pAdjRegion )
				{
					iCurrentLandCount = iOriginalLandCount;
					if( pAdjRegion->IsLand() || pAdjRegion->IsCity() ){ iCurrentLandCount++; }

					// Distance, Time
					fAdditionalDistance = g_pWorldDistanceTool->GetDistance( pAIRoad->GetLocation(), pAdjRegion->GetLocation() );
					fAdditionalTime = fAdditionalDistance / fMovementRate;
					// Danger/Presence/Sightings
					fAdjustedAdditionalDanger = GetAdjustedDangerValueOfRegion( pAdjRegion );
					fAdjustedAdditionalPresence = GetAdjustedPresenceValueOfRegion( pAdjRegion );
					fAdjustedAdditionalSightings = GetAdjustedSightingValueOfRegion( pAdjRegion );
					fRealAdditionalDanger = GetRealDangerValueOfRegion( pAdjRegion );
					fRealAdditionalPresence = GetRealPresenceValueOfRegion( pAdjRegion );
					fRealAdditionalSighting = GetRealSightingValueOfRegion( pAdjRegion );

					// Adj Terrain
					iAdjTerrain3 = pAdjRegion->m_iTerrainType3;
					iAdjTerrain9 = pAdjRegion->m_iTerrainType9;

					// Distance Cost is only used in calculating distance
					//   It is not used in multiplying the Danger/Presence/Sightings values
					if( iCurrentLandCount == 1 ){ fDistanceCost = m_fSeaDistanceMultiplier * fAdditionalDistance; }
					else if( iCurrentLandCount == 2 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance; }
					else{ ASSERT( false ); }

					// Cities that I can't pass through are considered to be land
					long iAdjTerrain3 = pAdjRegion->m_iTerrainType3;
					if( iAdjTerrain3 == TERRAIN_CITY )
					{
						ASSERT( pAdjRegion->GetEOSAICity() );
						if( m_iPlayer == 0 ||
							pAdjRegion->GetEOSAICity()->IsOwnedByOrAlliedTo( m_iPlayer ) )
						{
							iAdjTerrain3 = TERRAIN_GRASS;
						}
					}
				}

				// GeoSwitches
				//long iAdditionalSwitches = 0;
				if( pPathfinderStep->m_iTerrainType3 != iAdjTerrain3 )
				{
					/*
					// Cities that I can't pass through are considered to be land
					//long iAdjTerrain = pAdj->m_iTerrainType3;
					if( iAdjTerrain == TERRAIN_CITY )
					{
						ASSERT( pAdj->GetCity() );
						if( iPlayer == 0 ||
							pAdj->GetCity()->IsOwnedByOrAlliedTo( iPlayer ) )
						{
							iAdjTerrain = TERRAIN_GRASS;
						}
					}
					*/

					if( pPathfinderStep->m_iTerrainType3 == TERRAIN_WATER &&
						iAdjTerrain3 == TERRAIN_GRASS )
					{
						iAdditionalSwitches=1;
					}
					if( pPathfinderStep->m_iTerrainType3 == TERRAIN_GRASS &&
						iAdjTerrain3 == TERRAIN_WATER )
					{
						iAdditionalSwitches=1;
					}
					if( pPathfinderStep->m_iTerrainType3 == TERRAIN_CITY &&
						pPathfinderStep->m_iTempIncomingTerrain == TERRAIN_GRASS &&
						iAdjTerrain3 == TERRAIN_WATER )
					{
						iAdditionalSwitches=1;
					}
					if( pPathfinderStep->m_iTerrainType3 == TERRAIN_CITY &&
						pPathfinderStep->m_iTempIncomingTerrain == TERRAIN_WATER &&
						iAdjTerrain3 == TERRAIN_GRASS )
					{
						iAdditionalSwitches=1;
					}
				}

				float fNewPathfindingValue = 
					pPathfinderStep->m_fTempPathfindingValue +
					//fDistanceCost +
					fAdditionalTime +
					//(m_fDistanceMultiplier * fAdditionalDistance) +
					fAdjustedAdditionalDanger +
					fAdjustedAdditionalPresence +
					fAdjustedAdditionalSightings +
					(m_fGeoSwitchMultiplier * iAdditionalSwitches);

				if( m_iMaxGeoSwitches >= 0 &&
					pPathfinderStep->m_iTempNumberOfGeoSwitches + iAdditionalSwitches > m_iMaxGeoSwitches )
				{
					continue; // too many geo switches
				}

				// If we have a pathway, and that pathway is already shorter than
				//    this NewPathway, then skip processing this one.
				if( pEndRegion && pEndRegion->m_fTempPathfindingValue < fNewPathfindingValue )
				{
					continue;
				}

				if( pAdjStep->m_iTempVisitedKey != m_iCurrentKey ||
					pAdjStep->m_fTempPathfindingValue > fNewPathfindingValue )
				{
					if( pPathfinderStep->m_fTempTotalDistance + fAdditionalDistance < m_fMaximumDistance )
					{
						bool bWasPreviouslyVisited = ( pAdjStep->m_iTempVisitedKey == m_iCurrentKey );
						if( bWasPreviouslyVisited == false )
						{
							pAdjStep->m_iTempVisitedKey = m_iCurrentKey;
							pAdjStep->m_bTempIsInTheList = false;
						}
						//
						pAdjStep->m_fTempPathfindingValue = fNewPathfindingValue;
						pAdjStep->m_fTempTotalDistance    = pAIRoad->m_fTempTotalDistance + fAdditionalDistance;
						pAdjStep->m_fTempTotalTime        = pAIRoad->m_fTempTotalTime + fAdditionalTime;
						pAdjStep->m_fTempTotalDanger      = pAIRoad->m_fTempTotalDanger + fRealAdditionalDanger;
						pAdjStep->m_fTempTotalPresence    = pAIRoad->m_fTempTotalPresence + fRealAdditionalPresence;
						pAdjStep->m_fTempTotalSighting    = pAIRoad->m_fTempTotalSighting + fRealAdditionalSighting;
						pAdjStep->m_iTempNumberOfGeoSwitches = pAIRoad->m_iTempNumberOfGeoSwitches + iAdditionalSwitches;
						pAdjStep->m_pTempPreviousStep     = pAIRoad;
						pAdjStep->m_iTempIncomingTerrain  = pAIRoad->m_iTerrainType3;
						if( pAdjStep->m_iTempIncomingTerrain == TERRAIN_CITY )
						{
							pAdjStep->m_iTempIncomingTerrain = pAIRoad->m_iTempIncomingTerrain;
						}

						if( pAdjStep->m_iTempEndKey == m_iCurrentKey )
						{
							CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
							if( pEndRegion == NULL )
							{
								pEndRegion = pAdjRegion;
							}
							else
							{
								if( pEndRegion->m_fTempPathfindingValue > pAdjStep->m_fTempPathfindingValue )
								{
									pEndRegion = pAdjRegion;
								}
							}
						}

						if( pAdjStep->m_iId == 810 )
						{
							int f=0;
						}

						// Push it back into the CurrentList if there aren't targets
						CurrentPathfindingRegions.AddTail( pAdjStep );
						pAdjStep->m_bTempIsInTheList = true;
					}
				}
			}
		}
		CEOSAIGenericRegion* pRegion = dynamic_cast< CEOSAIGenericRegion* >( pPathfinderStep );
		if( pRegion )
		{
			//if( pRegion->IsLand() || pRegion->IsCity() ){ iOriginalLandCount=1; }
			if( pRegion->IsLand() || 
				( pRegion->IsCity() && bStartWithLandMovementFromCities ) ){ iOriginalLandCount=1; }

			// Collect the PathwaySteps
			CList< CEOSAIPathfinderPoint* >  PathwaySteps;
			pos = pRegion->m_RoadsOnTheRegionEdge.GetHeadPosition();
			while( pos )
			{
				CEOSAIRoad2* pAdj = pRegion->m_RoadsOnTheRegionEdge.GetNext( pos );
				PathwaySteps.AddTail( pAdj );
			}
			pos = pRegion->m_AdjacentRegions.GetHeadPosition();
			while( pos )
			{
				CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pRegion->m_AdjacentRegions.GetNext( pos );
				CEOSAIGenericRegion* pAdj = pAdjRegionDesc->m_pRegion;
				//CEOSAIGenericRegion* pAdj = pRegion->m_AdjacentRegions.GetNext( pos );
				PathwaySteps.AddTail( pAdj );
			}

			pos = PathwaySteps.GetHeadPosition();
			while( pos )
			{
				CEOSAIPathfinderPoint* pAdjStep = PathwaySteps.GetNext( pos );
				//bool bAdjIsPassible = true;
				float fMovementRate = m_TerrainBasedMovementRates.m_fTerrainValue[ pAdjStep->m_iTerrainType9 ];
				if( fMovementRate <= 0.0f ) continue; // impassible

				float fAdditionalDistance = 0.0f;
				float fAdditionalTime = 0.0f;
				float fAdjustedAdditionalDanger = 0.0f;
				float fAdjustedAdditionalPresence = 0.0f;
				float fAdjustedAdditionalSightings = 0.0f;
				float fRealAdditionalDanger = 0.0f;
				float fRealAdditionalPresence = 0.0f;
				float fRealAdditionalSighting = 0.0f;
				long  iAdditionalSwitches = 0;
				long  iAdjTerrain3 = TERRAIN_GRASS;
				long  iAdjTerrain9 = TERRAIN_GRASS;
				float fDistanceCost = 0.0f;

				// Region -> AIRoad
				CEOSAIRoad2* pAdjRoad = dynamic_cast< CEOSAIRoad2* >( pAdjStep );
				if( pAdjRoad )
				{
					// Distance
					fAdditionalDistance = g_pWorldDistanceTool->GetDistance( pRegion->GetLocation(), pAdjStep->GetLocation() );
					fAdditionalTime = fAdditionalDistance / fMovementRate;
					// Danger, Presence, Sightings
					fAdjustedAdditionalDanger = 0.0f;
					fAdjustedAdditionalPresence = 0.0f;
					fAdjustedAdditionalSightings = 0.0f;
					fRealAdditionalDanger = 0.0f;
					fRealAdditionalPresence = 0.0f;
					fRealAdditionalSighting = 0.0f;

					// Adj Terrain
					iAdjTerrain3 = TERRAIN_GRASS;
					iAdjTerrain9 = TERRAIN_ROAD;

					// Distance Cost is only used in calculating distance
					//   It is not used in multiplying the Danger/Presence/Sightings values
					if( iOriginalLandCount == 0 ){ fDistanceCost = m_fSeaDistanceMultiplier * fAdditionalDistance; }
					else if( iOriginalLandCount == 1 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance; }
					else{ ASSERT( false ); }
				}
				// Region -> Region
				CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
				if( pAdjRegion )
				{
					iCurrentLandCount = iOriginalLandCount;
					//if( pAdjStep->IsLand() || pAdjStep->IsCity() ){ iCurrentLandCount++; }
					if( pRegion->IsLand() || 
						( pRegion->IsCity() && bStartWithLandMovementFromCities ) ){ iCurrentLandCount++; }

					// Distance, Danger, Presence, Sightings
					fAdditionalDistance = g_pWorldDistanceTool->GetDistance( pRegion->GetLocation(), pAdjRegion->GetLocation() );
					fAdditionalTime = fAdditionalDistance / fMovementRate;
					//
					fAdjustedAdditionalDanger = GetAdjustedDangerValueOfRegion( pAdjRegion );
					fAdjustedAdditionalPresence = GetAdjustedPresenceValueOfRegion( pAdjRegion );
					fAdjustedAdditionalSightings = GetAdjustedSightingValueOfRegion( pAdjRegion );
					fRealAdditionalDanger = GetRealDangerValueOfRegion( pAdjRegion );
					fRealAdditionalPresence = GetRealPresenceValueOfRegion( pAdjRegion );
					fRealAdditionalSighting = GetRealSightingValueOfRegion( pAdjRegion );

					// Adj Terrain
					iAdjTerrain3 = pAdjStep->m_iTerrainType3;
					iAdjTerrain9 = pAdjStep->m_iTerrainType9;

					// Distance Cost is only used in calculating distance
					//   It is not used in multiplying the Danger/Presence/Sightings values
					if( iCurrentLandCount == 0 ){ fDistanceCost = m_fSeaDistanceMultiplier * fAdditionalDistance; }
					else if( iCurrentLandCount == 1 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance/2.0f + m_fSeaDistanceMultiplier * fAdditionalDistance/2.0f; }
					else if( iCurrentLandCount == 2 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance; }
					else{ ASSERT( false ); }

					// Cities that I can't pass through are considered to be land (impassible to ships)
					long iAdjTerrain3 = pAdjStep->m_iTerrainType3;
					if( iAdjTerrain3 == TERRAIN_CITY )
					{
						ASSERT( pAdjRegion->GetEOSAICity() );
						if( m_iPlayer > 0 &&
							pAdjRegion->GetEOSAICity()->IsOwnedByOrAlliedTo( m_iPlayer ) == false )
						{
							// Treat it as grass because sea-units can't go through, but I could
							//   potentially capture it with a ground unit
							iAdjTerrain3 = TERRAIN_GRASS;
							//continue; // Terrain is impassible
						}
						/*
						if( iPlayer == 0 ||
							pAdjRegion->GetCity()->IsOwnedByOrAlliedTo( iPlayer ) )
						{
							iAdjTerrain3 = TERRAIN_GRASS;
						}
						*/
					}
				}

				// GeoSwitches
				//long iAdditionalSwitches = 0;
				if( pRegion->m_iTerrainType3 != pAdjStep->m_iTerrainType3 )
				{
					if( pRegion->m_iTerrainType3 == TERRAIN_WATER &&
						iAdjTerrain3 == TERRAIN_GRASS )
					{
						iAdditionalSwitches=1;
					}
					if( pRegion->m_iTerrainType3 == TERRAIN_GRASS &&
						iAdjTerrain3 == TERRAIN_WATER )
					{
						iAdditionalSwitches=1;
					}
					if( pRegion->m_iTerrainType3 == TERRAIN_CITY &&
						pRegion->m_iTempIncomingTerrain == TERRAIN_GRASS &&
						iAdjTerrain3 == TERRAIN_WATER )
					{
						iAdditionalSwitches=1;
					}
					if( pRegion->m_iTerrainType3 == TERRAIN_CITY &&
						pRegion->m_iTempIncomingTerrain == TERRAIN_WATER &&
						iAdjTerrain3 == TERRAIN_GRASS )
					{
						iAdditionalSwitches=1;
					}
				}

				float fNewPathfindingValue = 
					pRegion->m_fTempPathfindingValue +
					//fDistanceCost +
					fAdditionalTime +
					//(m_fDistanceMultiplier * fAdditionalDistance) +
					fAdjustedAdditionalDanger +
					fAdjustedAdditionalPresence +
					fAdjustedAdditionalSightings +
					(m_fGeoSwitchMultiplier * iAdditionalSwitches);

				if( m_iMaxGeoSwitches >= 0 &&
					pRegion->m_iTempNumberOfGeoSwitches + iAdditionalSwitches > m_iMaxGeoSwitches )
				{
					continue; // too many geo switches
				}

				// If we have a pathway, and that pathway is already shorter than
				//    this NewPathway, then skip processing this one.
				if( pEndRegion && pEndRegion->m_fTempPathfindingValue < fNewPathfindingValue )
				{
					continue;
				}

				if( pAdjStep->m_iTempVisitedKey != m_iCurrentKey ||
					pAdjStep->m_fTempPathfindingValue > fNewPathfindingValue )
				{
					if( pRegion->m_fTempTotalDistance + fAdditionalDistance < m_fMaximumDistance )
					{
						bool bWasPreviouslyVisited = ( pAdjStep->m_iTempVisitedKey == m_iCurrentKey );
						if( bWasPreviouslyVisited == false )
						{
							pAdjStep->m_iTempVisitedKey = m_iCurrentKey;
							pAdjStep->m_bTempIsInTheList = false;
						}
						//
						pAdjStep->m_fTempPathfindingValue = fNewPathfindingValue;
						pAdjStep->m_fTempTotalDistance    = pRegion->m_fTempTotalDistance + fAdditionalDistance;
						pAdjStep->m_fTempTotalTime        = pRegion->m_fTempTotalTime + fAdditionalTime;
						pAdjStep->m_fTempTotalDanger      = pRegion->m_fTempTotalDanger + fRealAdditionalDanger;
						pAdjStep->m_fTempTotalPresence    = pRegion->m_fTempTotalPresence + fRealAdditionalPresence;
						pAdjStep->m_fTempTotalSighting    = pRegion->m_fTempTotalSighting + fRealAdditionalSighting;
						pAdjStep->m_iTempNumberOfGeoSwitches = pRegion->m_iTempNumberOfGeoSwitches + iAdditionalSwitches;
						pAdjStep->m_pTempPreviousStep     = pRegion;
						pAdjStep->m_iTempIncomingTerrain = pRegion->m_iTerrainType3;
						if( pAdjStep->m_iTempIncomingTerrain == TERRAIN_CITY )
						{
							pAdjStep->m_iTempIncomingTerrain = pRegion->m_iTempIncomingTerrain;
						}

						if( pAdjStep->m_iTempEndKey == m_iCurrentKey )
						{
							CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
							if( pEndRegion == NULL )
							{
								pEndRegion = pAdjRegion;
							}
							else
							{
								if( pEndRegion->m_fTempPathfindingValue > pAdjStep->m_fTempPathfindingValue )
								{
									pEndRegion = pAdjRegion;
								}
							}
						}

						if( pAdjStep->m_iId == 810 )
						{
							int f=0;
						}

						// Push it back into the CurrentList if there aren't targets
						CurrentPathfindingRegions.AddTail( pAdjStep );
						pAdjStep->m_bTempIsInTheList = true;
					}
				}
			}
		}
	}

	//Stopwatch1a.Stop();
	//float fTime1a = Stopwatch1a.GetDeltaSeconds();
	int x=0;
}

//void CEOSAIRegionPathfinder::Solve3( long iPlayer, bool bStartWithLandMovementFromCities )
void CEOSAIRegionPathfinder::Solve3()
{
	ASSERT( m_bDebug_IsBeingSetup == true );
	m_bDebug_IsBeingSetup = false;

	//CEOSAIStopwatch2  Stopwatch1a;
	//Stopwatch1a.Start();

	// Setup the Blocked Regions
	//   How is this supposed to work?
	//   I should iterate through all blocking units (GroundUnits, SeaUnits)
	//   For each blocking unit, attach the unit to all AIRegions within a radius of 80(?)
	//   The unit now blocks that AIRegion.
	//   If I want to move into that AIRegion (for example, to attack a city with a militia),
	//     Then I have to mark each blocking unit in that area as "unblock"
	//   Or, I can mark all units as "unblocking", but I should accumulate those pointers as 
	//     "need to fight" units.  (Do I already have something like this?)

	// Setup the blocking units
	//
	/*
	if( m_bAddBlockingUnits )
	{
		ASSERT( dynamic_cast< CEOSAIRegionManager2* >( m_pRegionManager ) );
		if( dynamic_cast< CEOSAIRegionManager2* >( m_pRegionManager ) )
		{
			long iArraySize = m_pRegionManager->m_PathfinderPointArray.GetSize();
			for( long i=0; i<iArraySize; i++ )
			{
				CEOSAIPathfinderPoint* pPoint = m_pRegionManager->m_PathfinderPointArray.GetAt( i );
				if( pPoint == NULL ) continue;
				//
				CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pPoint );
				if( pAIRegion )
				{
					POSITION pos = pAIRegion->GetPoiObjectProximities()->GetHeadPosition();
					while( pos )
					{
						CEOSAIPoiObjectProximity* pProximity = pAIRegion->GetPoiObjectProximities()->GetNext( pos );
						if( pProximity->m_fDistance < 50.0f &&
							pProximity->m_pPoiObject->CanBlockAndBeBlocked() )
						{
							pAIRegion->IsBlocked( m_iCurrentKey );
						}
					}
				}
				CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pPoint );
				if( pAIRoad )
				{
					CEOSAIRegion2* pAIRegion1 = pAIRoad->GetAIRegion1();
					if( pAIRegion1 )
					{
						POSITION pos = pAIRegion1->GetPoiObjectProximities()->GetHeadPosition();
						while( pos )
						{
							CEOSAIPoiObjectProximity* pProximity = pAIRegion1->GetPoiObjectProximities()->GetNext( pos );
							if( pProximity->m_fDistance < 50.0f &&
								pProximity->m_pPoiObject->CanBlockAndBeBlocked() )
							{
								pAIRoad->IsBlocked( m_iCurrentKey );
							}
						}
					}
					//CEOSAIRegion2* pAIRegion1 = pAIRoad->GetAIRegion2();
				}
			}

			// Permit all the end-points
			for( long i=0; i<iArraySize; i++ )
			{
				CEOSAIPathfinderPoint* pPoint = m_pRegionManager->m_PathfinderPointArray.GetAt( i );
				if( pPoint == NULL ) continue;
				//
				CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pPoint );
				if( pAIRegion )
				{
					POSITION pos = pAIRegion->GetPoiObjectProximities()->GetHeadPosition();
					while( pos )
					{
						CEOSAIPoiObjectProximity* pProximity = pAIRegion->GetPoiObjectProximities()->GetNext( pos );
						if( pProximity->m_fDistance < 50.0f &&
							pProximity->m_pPoiObject->CanBlockAndBeBlocked() )
						{
							pAIRegion->IsBlocked( m_iCurrentKey );
						}
					}
				}
				CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pPoint );
				if( pAIRoad )
				{
					CEOSAIRegion2* pAIRegion1 = pAIRoad->GetAIRegion1();
					if( pAIRegion1 )
					{
						POSITION pos = pAIRegion1->GetPoiObjectProximities()->GetHeadPosition();
						while( pos )
						{
							CEOSAIPoiObjectProximity* pProximity = pAIRegion1->GetPoiObjectProximities()->GetNext( pos );
							if( pProximity->m_fDistance < 50.0f &&
								pProximity->m_pPoiObject->CanBlockAndBeBlocked() )
							{
								pAIRoad->IsBlocked( m_iCurrentKey );
							}
						}
					}
					//CEOSAIRegion2* pAIRegion1 = pAIRoad->GetAIRegion2();
				}
			}
		}
	}
	*/

	// Setup the Start Regions
	//
	CList< CEOSAIPathfinderPoint* >  CurrentPathfindingRegions;
	ASSERT( m_StartingPathfindingRegions.IsEmpty() == FALSE || m_StartingPathwayItems.IsEmpty() == FALSE );
	if( m_StartingPathfindingRegions.IsEmpty() && m_StartingPathwayItems.IsEmpty() ){ return; }
	while( m_StartingPathfindingRegions.IsEmpty() == FALSE )
	{
		CEOSAIPathfinderPoint* pPoint = m_StartingPathfindingRegions.RemoveTail();
		CEOSAIGenericRegion* pRegion = dynamic_cast< CEOSAIGenericRegion* >( pPoint );
		ASSERT( pRegion );
		//CEOSAIGenericRegion* pRegion = m_StartingPathfindingRegions.RemoveTail();

		float fMovementRate = m_TerrainBasedMovementRates.m_fTerrainValue[ pRegion->m_iTerrainType3 ];
		if( pRegion->m_iTerrainType3 == TERRAIN_CITY )
		{
			if( m_bTreatInitialCitiesAsWater ){ fMovementRate = m_TerrainBasedMovementRates.m_fTerrainValue[ TERRAIN_WATER ]; }
			if( m_bTreatInitialCitiesAsLand  ){ fMovementRate = m_TerrainBasedMovementRates.m_fTerrainValue[ TERRAIN_GRASS ]; }
		}
		if( fMovementRate <= 0.0f ){ continue; } // impassible

		pRegion->m_fTempPathfindingValue = 
			GetAdjustedDangerValueOfRegion( pRegion ) +
			GetAdjustedPresenceValueOfRegion( pRegion ) +
			GetAdjustedSightingValueOfRegion( pRegion );
		pRegion->m_fTempTotalDistance = 0.0f;
		pRegion->m_fTempTotalTime = 0.0f;
		pRegion->m_fTempTotalDanger = GetRealDangerValueOfRegion( pRegion );
		pRegion->m_fTempTotalPresence = GetRealPresenceValueOfRegion( pRegion );
		pRegion->m_fTempTotalSighting = GetRealSightingValueOfRegion( pRegion );
		pRegion->m_iTempNumberOfGeoSwitches = 0;
		pRegion->m_iTempIncomingTerrain = pRegion->m_iTerrainType3;
		if( pRegion->m_iTerrainType3 == TERRAIN_CITY )
		{
			CEOSAICity* pAICity = pRegion->GetEOSAICity();
			ASSERT( pAICity );
			if( pAICity )
			{
				if( m_bTreatInitialCitiesAsLand )
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_GRASS;
				}
				else if( m_bTreatInitialCitiesAsWater )
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_WATER;
				}
				else
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_CITY; // treated as nothing
				}
				/*
				if( pCity->IsOwnedByOrAlliedTo( m_iPlayer ) == false )
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_GRASS;
				}
				else if( bStartWithLandMovementFromCities )
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_GRASS;
				}
				else
				{
					pRegion->m_iTempIncomingTerrain = TERRAIN_WATER;
				}
				*/
			}
		}
		pRegion->m_iTempVisitedKey = m_iCurrentKey;
		pRegion->m_bTempIsInTheList = true;
		pRegion->m_pTempPreviousStep = NULL;

		CurrentPathfindingRegions.AddTail( pRegion );
	}
	while( m_StartingPathwayItems.IsEmpty() == FALSE )
	{
		CEOSAIRegionPathwayItem* pRegionPathwayItem = m_StartingPathwayItems.RemoveTail();
		CEOSAIPathfinderPoint* pPathfinderPoint = pRegionPathwayItem->m_pPathfinderPoint;
		CEOSAIGenericRegion* pRegion = dynamic_cast< CEOSAIGenericRegion* >( pRegionPathwayItem->m_pPathfinderPoint );

		float fMovementRate = m_TerrainBasedMovementRates.m_fTerrainValue[ pPathfinderPoint->m_iTerrainType3 ];
		if( fMovementRate <= 0.0f ){ continue; } // impassible

		pPathfinderPoint->m_fTempPathfindingValue = pRegionPathwayItem->m_fPathfindingValue;
		if( pRegion )
		{
			pPathfinderPoint->m_fTempPathfindingValue = 
				GetAdjustedDangerValueOfRegion( pRegion ) +
				GetAdjustedPresenceValueOfRegion( pRegion ) +
				GetAdjustedSightingValueOfRegion( pRegion ) +
				pRegionPathwayItem->m_fPathfindingValue;
		}
		pPathfinderPoint->m_fTempTotalDistance = 0.0f;
		pPathfinderPoint->m_fTempTotalTime = pRegionPathwayItem->m_fTime;
		if( pRegion )
		{
			pPathfinderPoint->m_fTempTotalDanger = GetRealDangerValueOfRegion( pRegion );
			pPathfinderPoint->m_fTempTotalPresence = GetRealPresenceValueOfRegion( pRegion );
			pPathfinderPoint->m_fTempTotalSighting = GetRealSightingValueOfRegion( pRegion );
		}
		else
		{
			pPathfinderPoint->m_fTempTotalDanger = 0.0f;
			pPathfinderPoint->m_fTempTotalPresence = 0.0f;
			pPathfinderPoint->m_fTempTotalSighting = 0.0f;
		}
		pPathfinderPoint->m_iTempNumberOfGeoSwitches = 0;
		pPathfinderPoint->m_iTempIncomingTerrain = pPathfinderPoint->m_iTerrainType3;
		if( pRegion && pRegion->m_iTerrainType3 == TERRAIN_CITY )
		{
			CEOSAICity* pCity = pRegion->GetEOSAICity();
			ASSERT( pCity );
			if( pCity )
			{
				if( m_bTreatInitialCitiesAsLand )
				{
					pPathfinderPoint->m_iTempIncomingTerrain = TERRAIN_GRASS;
				}
				else if( m_bTreatInitialCitiesAsWater )
				{
					pPathfinderPoint->m_iTempIncomingTerrain = TERRAIN_WATER;
				}
				else
				{
					pPathfinderPoint->m_iTempIncomingTerrain = TERRAIN_CITY; // treated as nothing
				}
			}
		}
		pPathfinderPoint->m_iTempVisitedKey = m_iCurrentKey;
		pPathfinderPoint->m_bTempIsInTheList = true;
		pPathfinderPoint->m_pTempPreviousStep = NULL;

		CurrentPathfindingRegions.AddTail( pPathfinderPoint );
	}

	POSITION pos;
	CEOSAIGenericRegion* pEndRegion = NULL;
	long iPathfinderChecks = 0;

	// Now "grow" those Regions until I run-out of Regions
	while( CurrentPathfindingRegions.IsEmpty() == FALSE )
	{
		CEOSAIPathfinderPoint* pPathfinderStep = CurrentPathfindingRegions.RemoveHead();
		iPathfinderChecks++;

		if( pPathfinderStep->m_iId == 2 )
		{
			int h=0;
		}

		long iOriginalLandCount = 0;
		long iCurrentLandCount = 0;
		pPathfinderStep->m_bTempIsInTheList = false;

		ASSERT( pPathfinderStep->m_fTempTotalDistance < 900000.0f );

		CEOSAIRoad2* pAIRoad = dynamic_cast< CEOSAIRoad2* >( pPathfinderStep );
		if( pAIRoad )
		{
			iOriginalLandCount=1;

			// Collect the PathwaySteps
			CList< CEOSAIPathfinderPoint* >  PathwaySteps;
			pos = pAIRoad->m_RoadsIConnectTo.GetHeadPosition();
			while( pos )
			{
				CEOSAIAdjacentAIRoad2Distance* pRoadDistance = pAIRoad->m_RoadsIConnectTo.GetNext( pos );
				CEOSAIRoad2* pAdjRoad = pRoadDistance->m_pAIRoad;
				PathwaySteps.AddTail( pAdjRoad );
			}
			CEOSAIGenericRegion* pAIRegion1 = m_pRegionManager->GetGenericRegion( pAIRoad->m_iAIRegion1 );
			PathwaySteps.AddTail( pAIRegion1 );
			CEOSAIGenericRegion* pAIRegion2 = m_pRegionManager->GetGenericRegion( pAIRoad->m_iAIRegion2 );
			PathwaySteps.AddTail( pAIRegion2 );

			// AIRoad -> Region
			pos = PathwaySteps.GetHeadPosition();
			while( pos )
			{
				CEOSAIPathfinderPoint* pAdjStep = PathwaySteps.GetNext( pos );

				float fMovementRate = m_TerrainBasedMovementRates.m_fTerrainValue[ pAdjStep->m_iTerrainType9 ];
				if( fMovementRate <= 0.0f ) continue; // impassible

				float fAdditionalDistance = 0.0f;
				float fAdditionalTime = 0.0f;
				float fAdjustedAdditionalDanger = 0.0f;
				float fAdjustedAdditionalPresence = 0.0f;
				float fAdjustedAdditionalSightings = 0.0f;
				float fRealAdditionalDanger = 0.0f;
				float fRealAdditionalPresence = 0.0f;
				float fRealAdditionalSighting = 0.0f;

				long  iAdditionalSwitches = 0;
				long  iAdjTerrain3 = TERRAIN_GRASS;
				long  iAdjTerrain9 = TERRAIN_GRASS;
				float fDistanceCost = 0.0f;

				// AIRoad -> AIRoad
				CEOSAIRoad2* pAdjRoad = dynamic_cast< CEOSAIRoad2* >( pAdjStep );
				if( pAdjRoad )
				{
					CEOSAIAdjacentAIRoad2Distance* pRoadDistance = pAIRoad->GetRoadDistance( pAdjRoad );
					ASSERT( pRoadDistance );
					CEOSAIGenericRegion* pAdjRegion = pRoadDistance->m_pIntermediateRegion;

					// Distance/Time
					fAdditionalDistance = pRoadDistance->m_fPixelDistance;
					fAdditionalTime = fAdditionalDistance / fMovementRate;
					// Danger/Presence/Sightings
					fAdjustedAdditionalDanger = GetAdjustedDangerValueOfRegion( pAdjRegion );
					fAdjustedAdditionalPresence = GetAdjustedPresenceValueOfRegion( pAdjRegion );
					fAdjustedAdditionalSightings = GetAdjustedSightingValueOfRegion( pAdjRegion );
					fRealAdditionalDanger = GetRealDangerValueOfRegion( pAdjRegion );
					fRealAdditionalPresence = GetRealPresenceValueOfRegion( pAdjRegion );
					fRealAdditionalSighting = GetRealSightingValueOfRegion( pAdjRegion );
					// Adj Terrain
					iAdjTerrain3 = TERRAIN_GRASS;
					iAdjTerrain9 = TERRAIN_ROAD;

					// Distance Cost is only used in calculating distance
					//   It is not used in multiplying the Danger/Presence/Sightings values
					fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance;
				}
				// AIRoad -> Region
				CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
				if( pAdjRegion )
				{
					iCurrentLandCount = iOriginalLandCount;
					if( pAdjRegion->IsLand() || pAdjRegion->IsCity() ){ iCurrentLandCount++; }

					// Distance, Time
					fAdditionalDistance = g_pWorldDistanceTool->GetDistance( pAIRoad->GetLocation(), pAdjRegion->GetLocation() );
					fAdditionalTime = fAdditionalDistance / fMovementRate;
					// Danger/Presence/Sightings
					fAdjustedAdditionalDanger = GetAdjustedDangerValueOfRegion( pAdjRegion );
					fAdjustedAdditionalPresence = GetAdjustedPresenceValueOfRegion( pAdjRegion );
					fAdjustedAdditionalSightings = GetAdjustedSightingValueOfRegion( pAdjRegion );
					fRealAdditionalDanger = GetRealDangerValueOfRegion( pAdjRegion );
					fRealAdditionalPresence = GetRealPresenceValueOfRegion( pAdjRegion );
					fRealAdditionalSighting = GetRealSightingValueOfRegion( pAdjRegion );

					// Adj Terrain
					iAdjTerrain3 = pAdjRegion->m_iTerrainType3;
					iAdjTerrain9 = pAdjRegion->m_iTerrainType9;

					// Distance Cost is only used in calculating distance
					//   It is not used in multiplying the Danger/Presence/Sightings values
					if( iCurrentLandCount == 1 ){ fDistanceCost = m_fSeaDistanceMultiplier * fAdditionalDistance; }
					else if( iCurrentLandCount == 2 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance; }
					else{ ASSERT( false ); }

					// Cities that I can't pass through are considered to be land
					long iAdjTerrain3 = pAdjRegion->m_iTerrainType3;
					if( iAdjTerrain3 == TERRAIN_CITY )
					{
						ASSERT( pAdjRegion->GetEOSAICity() );
						if( m_iPlayer == 0 ||
							pAdjRegion->GetEOSAICity()->IsOwnedByOrAlliedTo( m_iPlayer ) )
						{
							iAdjTerrain3 = TERRAIN_GRASS;
						}
					}
				}

				// GeoSwitches
				//long iAdditionalSwitches = 0;
				if( pPathfinderStep->m_iTerrainType3 != iAdjTerrain3 )
				{
					/*
					// Cities that I can't pass through are considered to be land
					//long iAdjTerrain = pAdj->m_iTerrainType3;
					if( iAdjTerrain == TERRAIN_CITY )
					{
						ASSERT( pAdj->GetCity() );
						if( iPlayer == 0 ||
							pAdj->GetCity()->IsOwnedByOrAlliedTo( iPlayer ) )
						{
							iAdjTerrain = TERRAIN_GRASS;
						}
					}
					*/

					if( pPathfinderStep->m_iTerrainType3 == TERRAIN_WATER &&
						iAdjTerrain3 == TERRAIN_GRASS )
					{
						iAdditionalSwitches=1;
					}
					if( pPathfinderStep->m_iTerrainType3 == TERRAIN_GRASS &&
						iAdjTerrain3 == TERRAIN_WATER )
					{
						iAdditionalSwitches=1;
					}
					if( pPathfinderStep->m_iTerrainType3 == TERRAIN_CITY &&
						pPathfinderStep->m_iTempIncomingTerrain == TERRAIN_GRASS &&
						iAdjTerrain3 == TERRAIN_WATER )
					{
						iAdditionalSwitches=1;
					}
					if( pPathfinderStep->m_iTerrainType3 == TERRAIN_CITY &&
						pPathfinderStep->m_iTempIncomingTerrain == TERRAIN_WATER &&
						iAdjTerrain3 == TERRAIN_GRASS )
					{
						iAdditionalSwitches=1;
					}
				}

				float fNewPathfindingValue = 
					pPathfinderStep->m_fTempPathfindingValue +
					//fDistanceCost +
					fAdditionalTime +
					//(m_fDistanceMultiplier * fAdditionalDistance) +
					fAdjustedAdditionalDanger +
					fAdjustedAdditionalPresence +
					fAdjustedAdditionalSightings +
					(m_fGeoSwitchMultiplier * iAdditionalSwitches);

				if( m_iMaxGeoSwitches >= 0 &&
					pPathfinderStep->m_iTempNumberOfGeoSwitches + iAdditionalSwitches > m_iMaxGeoSwitches )
				{
					continue; // too many geo switches
				}

				// If we have a pathway, and that pathway is already shorter than
				//    this NewPathway, then skip processing this one.
				if( pEndRegion && pEndRegion->m_fTempPathfindingValue < fNewPathfindingValue )
				{
					continue;
				}

				if( pAdjStep->m_iTempVisitedKey != m_iCurrentKey ||
					pAdjStep->m_fTempPathfindingValue > fNewPathfindingValue )
				{
					if( pPathfinderStep->m_fTempTotalDistance + fAdditionalDistance < m_fMaximumDistance )
					{
						bool bWasPreviouslyVisited = ( pAdjStep->m_iTempVisitedKey == m_iCurrentKey );
						if( bWasPreviouslyVisited == false )
						{
							pAdjStep->m_iTempVisitedKey = m_iCurrentKey;
							pAdjStep->m_bTempIsInTheList = false;
						}
						//
						pAdjStep->m_fTempPathfindingValue = fNewPathfindingValue;
						pAdjStep->m_fTempTotalDistance    = pAIRoad->m_fTempTotalDistance + fAdditionalDistance;
						pAdjStep->m_fTempTotalTime        = pAIRoad->m_fTempTotalTime + fAdditionalTime;
						pAdjStep->m_fTempTotalDanger      = pAIRoad->m_fTempTotalDanger + fRealAdditionalDanger;
						pAdjStep->m_fTempTotalPresence    = pAIRoad->m_fTempTotalPresence + fRealAdditionalPresence;
						pAdjStep->m_fTempTotalSighting    = pAIRoad->m_fTempTotalSighting + fRealAdditionalSighting;
						pAdjStep->m_iTempNumberOfGeoSwitches = pAIRoad->m_iTempNumberOfGeoSwitches + iAdditionalSwitches;
						pAdjStep->m_pTempPreviousStep     = pAIRoad;
						pAdjStep->m_iTempIncomingTerrain  = pAIRoad->m_iTerrainType3;
						if( pAdjStep->m_iTempIncomingTerrain == TERRAIN_CITY )
						{
							pAdjStep->m_iTempIncomingTerrain = pAIRoad->m_iTempIncomingTerrain;
						}

						if( pAdjStep->m_iTempEndKey == m_iCurrentKey )
						{
							CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
							if( pEndRegion == NULL )
							{
								pEndRegion = pAdjRegion;
							}
							else
							{
								if( pEndRegion->m_fTempPathfindingValue > pAdjStep->m_fTempPathfindingValue )
								{
									pEndRegion = pAdjRegion;
								}
							}
						}

						if( pAdjStep->m_iId == 810 )
						{
							int f=0;
						}

						// Push it back into the CurrentList if there aren't targets
						CurrentPathfindingRegions.AddTail( pAdjStep );
						pAdjStep->m_bTempIsInTheList = true;
					}
				}
			}
		}
		CEOSAIGenericRegion* pRegion = dynamic_cast< CEOSAIGenericRegion* >( pPathfinderStep );
		if( pRegion )
		{
			#ifdef _DEBUG
			if( pRegion->m_iId == 96 && m_iPlayer == 2 )
			{
				int g=0;
			}

			if( pRegion->m_iTerrainType3 == TERRAIN_UNKNOWN )
			{
				int h=0;
			}
			#endif _DEBUG

			//if( pRegion->IsLand() || pRegion->IsCity() ){ iOriginalLandCount=1; }
			if( pRegion->IsLand() ){ iOriginalLandCount=1; }
			if( pRegion->IsCity() && pRegion->m_iTempIncomingTerrain == TERRAIN_GRASS ){ iOriginalLandCount=1; }
			//if( pRegion->IsLand() || 
			//	( pRegion->IsCity() && bStartWithLandMovementFromCities ) ){ iOriginalLandCount=1; }

			// Collect the PathwaySteps
			CList< CEOSAIPathfinderPoint* >  PathwaySteps;
			pos = pRegion->m_RoadsOnTheRegionEdge.GetHeadPosition();
			while( pos )
			{
				CEOSAIRoad2* pAdj = pRegion->m_RoadsOnTheRegionEdge.GetNext( pos );
				PathwaySteps.AddTail( pAdj );
			}
			pos = pRegion->m_AdjacentRegions.GetHeadPosition();
			while( pos )
			{
				CEOSAIAdjacentRegionDesc* pAdjRegionDesc = pRegion->m_AdjacentRegions.GetNext( pos );
				CEOSAIGenericRegion* pAdj = pAdjRegionDesc->m_pRegion;
				//CEOSAIGenericRegion* pAdj = pRegion->m_AdjacentRegions.GetNext( pos );
				PathwaySteps.AddTail( pAdj );
			}

			pos = PathwaySteps.GetHeadPosition();
			while( pos )
			{
				CEOSAIPathfinderPoint* pAdjStep = PathwaySteps.GetNext( pos );

				if( pAdjStep->m_iTerrainType9 == 0 )
				{
					int h=0;
				}
				if( pAdjStep->GetId() == 96 )
				{
					int g=0;
				}
				//bool bAdjIsPassible = true;
				float fMovementRate = m_TerrainBasedMovementRates.m_fTerrainValue[ pAdjStep->m_iTerrainType9 ];
				if( fMovementRate <= 0.0f ) continue; // impassible

				float fAdditionalDistance = 0.0f;
				float fAdditionalTime = 0.0f;
				float fAdjustedAdditionalDanger = 0.0f;
				float fAdjustedAdditionalPresence = 0.0f;
				float fAdjustedAdditionalSightings = 0.0f;
				float fRealAdditionalDanger = 0.0f;
				float fRealAdditionalPresence = 0.0f;
				float fRealAdditionalSighting = 0.0f;
				long  iAdditionalSwitches = 0;
				long  iAdjTerrain3 = TERRAIN_GRASS;
				long  iAdjTerrain9 = TERRAIN_GRASS;
				float fDistanceCost = 0.0f;

				// Region -> AIRoad
				CEOSAIRoad2* pAdjRoad = dynamic_cast< CEOSAIRoad2* >( pAdjStep );
				if( pAdjRoad )
				{
					// Distance
					fAdditionalDistance = g_pWorldDistanceTool->GetDistance( pRegion->GetLocation(), pAdjStep->GetLocation() );
					fAdditionalTime = fAdditionalDistance / fMovementRate;
					// Danger, Presence, Sightings
					fAdjustedAdditionalDanger = 0.0f;
					fAdjustedAdditionalPresence = 0.0f;
					fAdjustedAdditionalSightings = 0.0f;
					fRealAdditionalDanger = 0.0f;
					fRealAdditionalPresence = 0.0f;
					fRealAdditionalSighting = 0.0f;

					// Adj Terrain
					iAdjTerrain3 = TERRAIN_GRASS;
					iAdjTerrain9 = TERRAIN_ROAD;

					// Distance Cost is only used in calculating distance
					//   It is not used in multiplying the Danger/Presence/Sightings values
					if( iOriginalLandCount == 0 ){ fDistanceCost = m_fSeaDistanceMultiplier * fAdditionalDistance; }
					else if( iOriginalLandCount == 1 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance; }
					else{ ASSERT( false ); }
				}
				// Region -> Region
				CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
				if( pAdjRegion )
				{
					iCurrentLandCount = iOriginalLandCount;
					//if( pAdjStep->IsLand() || pAdjStep->IsCity() ){ iCurrentLandCount++; }
					//if( pRegion->IsLand() || 
					//	( pRegion->IsCity() && bStartWithLandMovementFromCities ) ){ iCurrentLandCount++; }
					if( pAdjRegion->IsLand() ){ iCurrentLandCount++; }
					if( pAdjRegion->IsCity() )
					{
						if( pAdjRegion->m_iTempIncomingTerrain == TERRAIN_GRASS ){ iCurrentLandCount++; }
					}

					// Distance, Danger, Presence, Sightings
					fAdditionalDistance = g_pWorldDistanceTool->GetDistance( pRegion->GetLocation(), pAdjRegion->GetLocation() );
					fAdditionalTime = fAdditionalDistance / fMovementRate;
					//
					fAdjustedAdditionalDanger = GetAdjustedDangerValueOfRegion( pAdjRegion );
					fAdjustedAdditionalPresence = GetAdjustedPresenceValueOfRegion( pAdjRegion );
					fAdjustedAdditionalSightings = GetAdjustedSightingValueOfRegion( pAdjRegion );
					fRealAdditionalDanger = GetRealDangerValueOfRegion( pAdjRegion );
					fRealAdditionalPresence = GetRealPresenceValueOfRegion( pAdjRegion );
					fRealAdditionalSighting = GetRealSightingValueOfRegion( pAdjRegion );

					//ASSERT( fRealAdditionalDanger == 0.0f );
					//ASSERT( fRealAdditionalPresence == 0.0f );
					//ASSERT( fRealAdditionalSighting == 0.0f );

					// Adj Terrain
					iAdjTerrain3 = pAdjStep->m_iTerrainType3;
					iAdjTerrain9 = pAdjStep->m_iTerrainType9;

					// Distance Cost is only used in calculating distance
					//   It is not used in multiplying the Danger/Presence/Sightings values
					if( iCurrentLandCount == 0 ){ fDistanceCost = m_fSeaDistanceMultiplier * fAdditionalDistance; }
					else if( iCurrentLandCount == 1 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance/2.0f + m_fSeaDistanceMultiplier * fAdditionalDistance/2.0f; }
					else if( iCurrentLandCount == 2 ){ fDistanceCost = m_fLandDistanceMultiplier * fAdditionalDistance; }
					else{ ASSERT( false ); }

					// Cities that I can't pass through are considered to be land (impassible to ships)
					/*
					long iAdjTerrain3 = pAdjStep->m_iTerrainType3;
					if( iAdjTerrain3 == TERRAIN_CITY )
					{
						ASSERT( pAdjRegion->GetCity() );
						if( m_iPlayer > 0 &&
							pAdjRegion->GetCity()->IsOwnedByOrAlliedTo( m_iPlayer ) == false )
						{
							// Treat it as grass because sea-units can't go through, but I could
							//   potentially capture it with a ground unit
							iAdjTerrain3 = TERRAIN_GRASS;
							//continue; // Terrain is impassible
						}
						/-*
						if( iPlayer == 0 ||
							pAdjRegion->GetCity()->IsOwnedByOrAlliedTo( iPlayer ) )
						{
							iAdjTerrain3 = TERRAIN_GRASS;
						}
						*-/
					}
					*/
				}

				// GeoSwitches
				//long iAdditionalSwitches = 0;
				if( pRegion->m_iTerrainType3 != pAdjStep->m_iTerrainType3 )
				{
					if( pRegion->m_iTerrainType3 == TERRAIN_WATER &&
						iAdjTerrain3 == TERRAIN_GRASS )
					{
						iAdditionalSwitches=1;
					}
					if( pRegion->m_iTerrainType3 == TERRAIN_GRASS &&
						iAdjTerrain3 == TERRAIN_WATER )
					{
						iAdditionalSwitches=1;
					}
					if( pRegion->m_iTerrainType3 == TERRAIN_CITY &&
						pRegion->m_iTempIncomingTerrain == TERRAIN_GRASS &&
						iAdjTerrain3 == TERRAIN_WATER )
					{
						iAdditionalSwitches=1;
					}
					if( pRegion->m_iTerrainType3 == TERRAIN_CITY &&
						pRegion->m_iTempIncomingTerrain == TERRAIN_WATER &&
						iAdjTerrain3 == TERRAIN_GRASS )
					{
						iAdditionalSwitches=1;
					}
				}

				float fNewPathfindingValue = 
					pRegion->m_fTempPathfindingValue +
					//fDistanceCost +
					fAdditionalTime +
					//(m_fDistanceMultiplier * fAdditionalDistance) +
					fAdjustedAdditionalDanger +
					fAdjustedAdditionalPresence +
					fAdjustedAdditionalSightings +
					(m_fGeoSwitchMultiplier * iAdditionalSwitches);

				if( fAdjustedAdditionalDanger > 0.0f ||
					fAdjustedAdditionalPresence > 0.0f ||
					fAdjustedAdditionalSightings > 0.0f )
				{
					int g=0;
				}

				if( m_iMaxGeoSwitches >= 0 &&
					pRegion->m_iTempNumberOfGeoSwitches + iAdditionalSwitches > m_iMaxGeoSwitches )
				{
					continue; // too many geo switches
				}

				// If we have a pathway, and that pathway is already shorter than
				//    this NewPathway, then skip processing this one.
				if( pEndRegion && pEndRegion->m_fTempPathfindingValue < fNewPathfindingValue )
				{
					continue;
				}

				if( pAdjStep->m_iTempVisitedKey != m_iCurrentKey ||
					pAdjStep->m_fTempPathfindingValue > fNewPathfindingValue )
				{
					if( pRegion->m_fTempTotalDistance + fAdditionalDistance < m_fMaximumDistance )
					{
						bool bWasPreviouslyVisited = ( pAdjStep->m_iTempVisitedKey == m_iCurrentKey );
						if( bWasPreviouslyVisited == false )
						{
							pAdjStep->m_iTempVisitedKey = m_iCurrentKey;
							pAdjStep->m_bTempIsInTheList = false;
						}
						//
						pAdjStep->m_fTempPathfindingValue = fNewPathfindingValue;
						pAdjStep->m_fTempTotalDistance    = pRegion->m_fTempTotalDistance + fAdditionalDistance;
						pAdjStep->m_fTempTotalTime        = pRegion->m_fTempTotalTime + fAdditionalTime;
						pAdjStep->m_fTempTotalDanger      = pRegion->m_fTempTotalDanger + fRealAdditionalDanger;
						pAdjStep->m_fTempTotalPresence    = pRegion->m_fTempTotalPresence + fRealAdditionalPresence;
						pAdjStep->m_fTempTotalSighting    = pRegion->m_fTempTotalSighting + fRealAdditionalSighting;
						pAdjStep->m_iTempNumberOfGeoSwitches = pRegion->m_iTempNumberOfGeoSwitches + iAdditionalSwitches;
						pAdjStep->m_pTempPreviousStep     = pRegion;
						pAdjStep->m_iTempIncomingTerrain = pRegion->m_iTerrainType3;
						if( pAdjStep->m_iTempIncomingTerrain == TERRAIN_CITY )
						{
							pAdjStep->m_iTempIncomingTerrain = pRegion->m_iTempIncomingTerrain;
						}

						if( pAdjStep->m_iTempEndKey == m_iCurrentKey )
						{
							CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
							if( pEndRegion == NULL )
							{
								pEndRegion = pAdjRegion;
							}
							else
							{
								if( pEndRegion->m_fTempPathfindingValue > pAdjStep->m_fTempPathfindingValue )
								{
									pEndRegion = pAdjRegion;
								}
							}
						}

						if( pAdjStep->m_iId == 810 )
						{
							int f=0;
						}

						// If this is an enemy city, it is reachable, but not passable
						bool bInsertIntoTheList = true;
						/*
						I'm going to comment this out.  I can sometimes cause problems if the only
						pathway to a city is through other cities.  This is rare, but if someone made
						a custom map with cities packed around cities, this could happen.
						CEOSAIGenericRegion* pAdjRegion = dynamic_cast< CEOSAIGenericRegion* >( pAdjStep );
						if( m_iPlayer > 0 && 
							pAdjRegion &&
							pAdjRegion->IsCity() &&
							pAdjRegion->GetCity()->IsOwnedByOrAlliedTo( m_iPlayer ) == false )
						{
							bInsertIntoTheList = false; // this is impassible
						}
						*/

						// Push it back into the CurrentList if there aren't targets
						if( bInsertIntoTheList )
						{
							CurrentPathfindingRegions.AddTail( pAdjStep );
							pAdjStep->m_bTempIsInTheList = true;
						}
					}
				}
			}
		}
	}

	//Stopwatch1a.Stop();
	//float fTime1a = Stopwatch1a.GetDeltaSeconds();
	int x=0;
}

void CEOSAIRegionPathfinder::GetResult( bool bGetAllResults, CEOSAIRegionPathwayMap* pResultMap )
{
	//CEOSAIStopwatch2  Stopwatch1b;
	//Stopwatch1b.Start();

	//long iRegionCount = m_pRegionManager->m_GenericRegionList.GetCount();
	long iArraySize = m_pRegionManager->m_PathfinderPointArray.GetSize();

	// Fill the pResultList
	if( pResultMap )
	{
		pResultMap->Clear();
		pResultMap->SetRegionManager( m_pRegionManager );
		//pResultMap->AllocatePathfinderPointArray( iArraySize );
		pResultMap->SetArraySize( iArraySize );

		for( long i=0; i<iArraySize; i++ )
		{
			if( i == 85 )
			{
				int g=0;
			}
			CEOSAIPathfinderPoint* pPoint = m_pRegionManager->m_PathfinderPointArray.GetAt( i );
			if( pPoint == NULL ) continue;

			if( pPoint && pPoint->m_iTempVisitedKey == m_iCurrentKey )
			{
				// If there is an EndRegion, add only EndRegions to the result
				if( !bGetAllResults && pPoint->m_iTempEndKey == m_iCurrentKey )
				{
					static long iUniqueId = 0;
					iUniqueId++;

					if( iUniqueId == 18553 )
					{
						int h=0;
					}

					CEOSAIRegionPathwayItem* pResultObject = new CEOSAIRegionPathwayItem();
					pResultObject->m_iDebugId = iUniqueId;
					pResultObject->m_pRegionPathwayMap = pResultMap;
					pResultObject->m_pPathfinderPoint = pPoint;
					pResultObject->m_fPathfindingValue = pPoint->m_fTempPathfindingValue;
					pResultObject->m_fDistance = pPoint->m_fTempTotalDistance;
					pResultObject->m_fTime = pPoint->m_fTempTotalTime;
					pResultObject->m_fDanger = pPoint->m_fTempTotalDanger;
					pResultObject->m_fPresence = pPoint->m_fTempTotalPresence;
					pResultObject->m_fSightings = pPoint->m_fTempTotalSighting;
					pResultObject->m_iGeoSwitches = pPoint->m_iTempNumberOfGeoSwitches;
					//pResultObject->m_iPreviousRegion = pRegion->m_pTempPreviousStep->m_iId;
					//pResultObject->m_pPreviousStep = pRegion->m_pTempPreviousStep;

					if( pPoint->m_pTempPreviousStep )
					{
						ASSERT( pPoint != pPoint->m_pTempPreviousStep );
						pResultObject->m_iPreviousPathwayItem = pPoint->m_pTempPreviousStep->m_iId;
					}
					else
					{
						pResultObject->m_iPreviousPathwayItem = 0;
					}

					pResultMap->AddRegionPathwayItem( pResultObject );

					if( pPoint->m_iTempEndKey == m_iCurrentKey )
					{
						pResultMap->AddShortestStartToEndDistance( pResultObject->m_fDistance );
						pResultMap->AddShortestStartToEndTime( pResultObject->m_fTime );
					}
				}
				else // If there are no EndRegions, add all Regions to the result
				{
					static long iUniqueId = 0;
					iUniqueId++;
					if( iUniqueId == 18553 )
					{
						int h=0;
					}

					CEOSAIRegionPathwayItem* pResultObject = new CEOSAIRegionPathwayItem();
					pResultObject->m_iDebugId = iUniqueId;
					pResultObject->m_pRegionPathwayMap = pResultMap;
					pResultObject->m_pPathfinderPoint = pPoint;
					pResultObject->m_fPathfindingValue = pPoint->m_fTempPathfindingValue;
					pResultObject->m_fDistance = pPoint->m_fTempTotalDistance;
					pResultObject->m_fTime = pPoint->m_fTempTotalTime;
					pResultObject->m_fDanger = pPoint->m_fTempTotalDanger;
					pResultObject->m_fPresence = pPoint->m_fTempTotalPresence;
					pResultObject->m_fSightings = pPoint->m_fTempTotalSighting;
					pResultObject->m_iGeoSwitches = pPoint->m_iTempNumberOfGeoSwitches;

					if( pPoint->m_pTempPreviousStep )
					{
						ASSERT( pPoint != pPoint->m_pTempPreviousStep );
						pResultObject->m_iPreviousPathwayItem = pPoint->m_pTempPreviousStep->m_iId;
					}
					else
					{
						pResultObject->m_iPreviousPathwayItem = 0;
					}
					pResultMap->AddRegionPathwayItem( pResultObject );

					if( pPoint->m_iTempEndKey == m_iCurrentKey )
					{
						pResultMap->AddShortestStartToEndDistance( pResultObject->m_fDistance );
						pResultMap->AddShortestStartToEndTime( pResultObject->m_fTime );
					}
				}
			}
		}
		/*
		POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
		while( pos )
		{
			CEOSAIGenericRegion* pRegion = m_pRegionManager->m_GenericRegionList.GetNext( pos );
			//CEOSAIGenericRegion* pMultiRegion = pRegions->GetAt( i );
			if( pRegion && pRegion->m_iTempVisitedKey == m_iCurrentKey )
			{
				// If there is an EndRegion, add only EndRegions to the result
				//if( pEndMultiRegion && pMultiRegion->m_iTempEndKey == m_iCurrentKey )
				if( !bGetAllResults && pRegion->m_iTempEndKey == m_iCurrentKey )
				{
					CEOSAIRegionPathwayItem* pResultObject = new CEOSAIRegionPathwayItem();
					//pResultObject->m_pRegion = pRegion;
					pResultObject->m_pCurrentStep = pRegion;
					pResultObject->m_fPathfindingValue = pRegion->m_fTempPathfindingValue;
					pResultObject->m_fDistance = pRegion->m_fTempTotalDistance;
					pResultObject->m_fDanger = pRegion->m_fTempTotalDanger;
					pResultObject->m_fPresence = pRegion->m_fTempTotalPresence;
					pResultObject->m_fSightings = pRegion->m_fTempTotalSighting;
					pResultObject->m_iGeoSwitches = pRegion->m_iTempNumberOfGeoSwitches;
					//pResultObject->m_iPreviousRegion = pRegion->m_pTempPreviousStep->m_iId;
					//pResultObject->m_pPreviousStep = pRegion->m_pTempPreviousStep;
					pResultObject->m_iPreviousPathwayItem
					pResultMap->AddRegionPathwayItem( pResultObject );

					if( pRegion->m_iTempEndKey == m_iCurrentKey )
					{
						pResultMap->AddShortestStartToEndDistance( pResultObject->m_fDistance );
					}
				}
				else // If there are no EndRegions, add all Regions to the result
				{
					CEOSAIRegionPathwayItem* pResultObject = new CEOSAIRegionPathwayItem();
					//pResultObject->m_pRegion = pRegion;
					pResultObject->m_pCurrentStep = pRegion;
					pResultObject->m_fPathfindingValue = pRegion->m_fTempPathfindingValue;
					pResultObject->m_fDistance = pRegion->m_fTempTotalDistance;
					pResultObject->m_fDanger = pRegion->m_fTempTotalDanger;
					pResultObject->m_fPresence = pRegion->m_fTempTotalPresence;
					pResultObject->m_fSightings = pRegion->m_fTempTotalSighting;
					pResultObject->m_iGeoSwitches = pRegion->m_iTempNumberOfGeoSwitches;
					if( pRegion->m_pTempPreviousStep )
					{
						pResultObject->m_pPreviousStep = pRegion->m_pTempPreviousStep;
						//pResultObject->m_iPreviousRegion = pRegion->m_pTempPreviousStep->m_iId;
					}
					else
					{
						pResultObject->m_pPreviousStep = NULL;
						//pResultObject->m_iPreviousRegion = 0;
					}
					pResultMap->AddRegionPathwayItem( pResultObject );

					if( pRegion->m_iTempEndKey == m_iCurrentKey )
					{
						pResultMap->AddShortestStartToEndDistance( pResultObject->m_fDistance );
					}
				}
			}
		}
		*/
	}

	//Stopwatch1b.Stop();
	//float fTime1b = Stopwatch1b.GetDeltaSeconds();

	int x=0;
}

//float CEOSAIRegionPathfinder::GetAdjustedDangerValueOfMultiRegion( CMultiRegion* pMultiRegion )
float CEOSAIRegionPathfinder::GetAdjustedDangerValueOfRegion( CEOSAIGenericRegion* pRegion )
{
	float fRegionValue = 0.0f;
	for( long iPlayer=1; iPlayer<pRegion->m_PlayerVisibility01.m_iSize; iPlayer++ )
	{
		ASSERT( m_PlayerWeights.m_iSize > 0 );
		if( m_PlayerWeights[iPlayer]->m_fPlayerDangerPathfindingMultiplier > 0.0f )
		{
			/*
			float fNewValue = 
				m_PlayerWeights[iPlayer]->m_fPlayerDangerPathfindingMultiplier * 
				pRegion->GetUnitPower( iPlayer );

			fNewValue /= 1000.0f;
			if( fNewValue > 1.0f ) fNewValue = 1.0f;

			fRegionValue = 1.0f - ( ( 1.0f - fNewValue ) * ( 1.0f - fRegionValue ) );
			*/
			/*
			float fUnitPower01 = pRegion->GetUnitPower( iPlayer );
			fUnitPower01 /= 1000.0f;
			if( fUnitPower01 > 1.0f ) fUnitPower01 = 1.0f;

			float fNewValue = 
				m_PlayerWeights[iPlayer]->m_fPlayerDangerPathfindingMultiplier * 
				fUnitPower01;
			*/
			float fNewValue = 
				m_PlayerWeights[iPlayer]->m_fPlayerDangerPathfindingMultiplier * 
				pRegion->GetPlayerPower( iPlayer );

			fRegionValue += fNewValue;
		}
	}
	return fRegionValue;
}

float CEOSAIRegionPathfinder::GetAdjustedPresenceValueOfRegion( CEOSAIGenericRegion* pRegion )
{
	float fRegionValue = 0.0f;
	for( long iPlayer=1; iPlayer<pRegion->m_PlayerVisibility01.m_iSize; iPlayer++ )
	{
		if( m_PlayerWeights[iPlayer]->m_fPlayerPresencePathfindingMultiplier > 0.0f )
		{
			float fNewValue = 
				m_PlayerWeights[iPlayer]->m_fPlayerPresencePathfindingMultiplier * 
				pRegion->GetPlayerPresence01( iPlayer );

			fRegionValue += fNewValue;
		}
	}
	return fRegionValue;
}

float CEOSAIRegionPathfinder::GetAdjustedSightingValueOfRegion( CEOSAIGenericRegion* pRegion )
{
	float fRegionValue = 0.0f;
	for( long iPlayer=1; iPlayer<pRegion->m_PlayerVisibility01.m_iSize; iPlayer++ )
	{
		if( m_PlayerWeights[iPlayer]->m_fPlayerSightingPathfindingMultiplier > 0.0f )
		{
			float fNewValue = 
				m_PlayerWeights[iPlayer]->m_fPlayerSightingPathfindingMultiplier * 
				pRegion->GetPlayerVisibility01( iPlayer );

			fRegionValue += fNewValue;
		}
	}
	return fRegionValue;
}

float CEOSAIRegionPathfinder::GetRealDangerValueOfRegion( CEOSAIGenericRegion* pRegion )
{
	float fRegionValue = 0.0f;
	for( long iPlayer=1; iPlayer<pRegion->m_PlayerVisibility01.m_iSize; iPlayer++ )
	{
		if( m_PlayerWeights[iPlayer]->m_fPlayerDangerResultMultiplier > 0.0f )
		{
			//ASSERT( pRegion->GetUnitPower( iPlayer ) == 0.0f );
			//ASSERT( m_PlayerWeights[iPlayer]->m_fPlayerDangerResultMultiplier == 0.0f );
			float fNewValue = 
				m_PlayerWeights[iPlayer]->m_fPlayerDangerResultMultiplier * 
				//pRegion->GetUnitPower( iPlayer );
				pRegion->GetPlayerPower( iPlayer );

			//fRegionValue =
			//	1.0f - ( ( 1.0f - fNewValue ) * ( 1.0f - fRegionValue ) );
			fRegionValue += fNewValue;
		}
	}
	return fRegionValue;
}

float CEOSAIRegionPathfinder::GetRealPresenceValueOfRegion( CEOSAIGenericRegion* pRegion )
{
	float fRegionValue = 0.0f;
	for( long iPlayer=1; iPlayer<pRegion->m_PlayerVisibility01.m_iSize; iPlayer++ )
	{
		if( m_PlayerWeights[iPlayer]->m_fPlayerPresenceResultMultiplier > 0.0f )
		{
			long h=0;
		}

		if( m_PlayerWeights[iPlayer]->m_fPlayerPresenceResultMultiplier > 0.0f )
		{
			//float fDebug1 = pRegion->GetPresence01( iPlayer );
			//if( fDebug1 > 0.0f )
			//{
			//	//int h=0;
			//}

			float fNewValue = m_PlayerWeights[iPlayer]->m_fPlayerPresenceResultMultiplier * pRegion->GetPlayerPresence01( iPlayer );
			fRegionValue =
				1.0f - ( ( 1.0f - fNewValue ) * ( 1.0f - fRegionValue ) );
		}
	}
	return fRegionValue;
}

float CEOSAIRegionPathfinder::GetRealSightingValueOfRegion( CEOSAIGenericRegion* pRegion )
{
	float fRegionValue = 0.0f;
	for( long iPlayer=1; iPlayer<pRegion->m_PlayerVisibility01.m_iSize; iPlayer++ )
	{
		if( m_PlayerWeights[iPlayer]->m_fPlayerSightingResultMultiplier > 0.0f )
		{
			float fNewValue = m_PlayerWeights[iPlayer]->m_fPlayerSightingResultMultiplier * pRegion->GetPlayerVisibility01( iPlayer );
			fRegionValue =
				1.0f - ( ( 1.0f - fNewValue ) * ( 1.0f - fRegionValue ) );
		}
	}
	return fRegionValue;
}


#include "stdafx.h"
#include "EOSAISettings.h"
#include "EOSAICommonData.h"
//#include "CommonState.h"
//#include "WorldDescServer.h"
#include "EOSAIMultiRegionNationwidePathways.h"
//#include "EOSAIGeo.h"
#include "EOSAINationalSummary3.h"
#include "EOSAIUnitVsUnitValue.h"
//#include "WorldDescPlayer.h"
//#include "Player.h"
//#include "Checkpoint.h"
#include "EOSAIPlayerInteraction_DeclaredWar.h"
#include "EOSAIPlayerInteraction_SneakAttack.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIUnit2.h"
#include "EOSAIBuildOption.h"
#include "EOSAIBuildingDescription.h"
//#include "..\..\BCToolkit\LogFile.h"
#include "EOSAILogFile.h"
#include "EOSAIInterface.h"
#include "EOSAIStopwatch2.h"
#include "MessageFromAI_ForeignRelationsFeelings.h"
#include "AIPlayer.h"
extern CEOSAILogFile g_LogFile;
#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EOSAI::CCommonData* g_pEOSAICommonData = NULL;

using namespace EOSAI;
//

CCommonData::CCommonData()
{
	m_bNeedToRebuildData = true;
	m_bDataIsValid = false;

	m_bAllPlayersPermanentlyAtWar = false;

	m_fTotalCityProductionOnMap = 0.0f;
	m_fTotalCitResProductionOnMap = 0.0f;
	m_fTotalUnownedCitResProductionOnMap = 0.0f;
	m_fTotalResourcesOnMap = 0.0f;
	g_pEOSAICommonData = this;

	m_MultiRegionManager.SetAIRegionManager( &m_AIRegionManager );

	m_iUnitCombatCapabilities_LastTurnCalculated = -1;
/*
	m_Players.SetSize( 11 );
	for( long iPlayer=0; iPlayer<=10; iPlayer++ )
	{
		m_Players[iPlayer] = NULL;
	}
*/
	//m_pWorldDistanceTool = NULL;
}

CCommonData::~CCommonData()
{
	g_pEOSAICommonData = NULL;
	DeleteData();
}

// This sets the globals
void CCommonData::SetEOSAIInterface( EOSAI::CInterface* pEOSAIInterface ){ g_pEOSAIInterface = pEOSAIInterface; }
void CCommonData::SetEOSAIWorldDistanceTool( EOSAI::CWorldDistanceTool* pWorldDistanceTool ){ g_pWorldDistanceTool = pWorldDistanceTool; }

void CCommonData::DeleteData()
{
	m_MultiRegionManager.ClearPoiData();
	m_AIRegionManager.ClearPoiData();
	//while( m_AIGeoList.IsEmpty() == FALSE ){ delete m_AIGeoList.RemoveHead(); }

	for( long i=0; i<m_AINationalSummaries.m_iSize; i++ )
	{
		delete m_AINationalSummaries.Value( i );
		m_AINationalSummaries.Value( i ) = NULL;
	}
	for( long i=0; i<m_NationwidePlayerPathways.m_iSize; i++ )
	{
		delete m_NationwidePlayerPathways.Value( i );
		m_NationwidePlayerPathways.Value( i ) = NULL;
	}
	/*
	for( long i=0; i<m_AIThoughtDatabases.m_iSize; i++ )
	{
		delete m_AIThoughtDatabases.Value( i );
		m_AIThoughtDatabases.Value( i ) = NULL;
	}
	*/
}

void CCommonData::DeleteAIPoiObjects()
{
	while( m_AIPoiObjects.IsEmpty() == FALSE )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.RemoveHead();
		delete pAIPoiObject;
	}
}

void CCommonData::SetNeedToRebuildData( bool bNeedToRebuildData )
{
	m_bNeedToRebuildData = bNeedToRebuildData;
	m_bDataIsValid = !bNeedToRebuildData;
}

long CCommonData::GetNumberOfPlayers()
{
	return g_pEOSAIInterface->GetNumberOfPlayers();
}

void CCommonData::RebuildDataIfNecessary()
{
	//
	// The Common AI Data is being built inside the main application thread, so I no longer do this here.
	// In the "official" version of EOS, the AI could rebuild this data on its own (the ServerWorldDesc is constant during a turn)
	//
	/*
	if( m_bNeedToRebuildData )
	{
		Sleep( 1000 );
		ASSERT( false ); //CreateData();
	}
	*/
}
/*
void CCommonData::AddGamePlayer( long iPlayer, EOSAI::EnumGamePlayerType ePlayerType, bool bIsAlive )
{
	ASSERT( iPlayer <= 10 );
	ASSERT( m_Players[iPlayer] == NULL );

	EOSAI::CGamePlayer* pGPlayer = new EOSAI::CGamePlayer();
	pGPlayer->m_iPlayer = iPlayer;
	pGPlayer->m_ePlayerType = ePlayerType;
	pGPlayer->m_bIsAlive = bIsAlive;
	m_Players[iPlayer] = pGPlayer;
}
*/
/*
void CAICommonData::CreateData()
{
	CEOSAIStopwatch2  WatchTotal;
	CEOSAIStopwatch2  WatchGenerateAIRegions;
	CEOSAIStopwatch2  WatchGenerateMultiRegions;
	CEOSAIStopwatch2  WatchCalculateUnitCombatCapabilities;
	CEOSAIStopwatch2  WatchCalculateNationalSummaries;
	CEOSAIStopwatch2  WatchAddObjectIdsToAIRegionsAndMultiRegions;
	CEOSAIStopwatch2  WatchCalculateNationwidePathways;
	CEOSAIStopwatch2  WatchCalculateAIRegionOwnership;

	#ifdef _DEBUG
	//Beep( 100,100 );
	#endif _DEBUG

	m_bNeedToRebuildData = false;

	if( GetNumberOfActiveAIPlayers() == 0 ) return;
	//#ifdef _DEBUG
	//if( Public::ActiveAIPlayers() == false ) return;
	//#endif
	WatchTotal.Start();

	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	if( m_AIRegionManager.GetNumberOfGenericRegions() == 0 )
	{
		//pWorldDescServer->BuildGeoAndNodeMaps();

		WatchGenerateAIRegions.Start();
		m_AIRegionManager.ServerFunction_GenerateAIRegions( pWorldDescServer );
		WatchGenerateAIRegions.Stop();
		//
		m_MultiRegionManager.SetAIRegionManager( &m_AIRegionManager );
		m_MultiRegionManager.SetWorldDescServer( pWorldDescServer );

		WatchGenerateMultiRegions.Start();
		m_MultiRegionManager.GenerateMultiRegions( &m_AIRegionManager );
		WatchGenerateMultiRegions.Stop();
	}

	WatchCalculateUnitCombatCapabilities.Start();
	//GetCommonState()->CalculateUnitCombatCapabilities( pWorldDescServer->GetCurrentTurn() );
	CalculateAIUnitCombatCapabilities( pWorldDescServer->GetCurrentTurn() );
	WatchCalculateUnitCombatCapabilities.Stop();

	WatchCalculateNationalSummaries.Start();
	CalculateNationalSummaries();
	WatchCalculateNationalSummaries.Stop();

	WatchAddObjectIdsToAIRegionsAndMultiRegions.Start();
	AddObjectIdsToAIRegionsAndMultiRegions();
	WatchAddObjectIdsToAIRegionsAndMultiRegions.Stop();

	WatchCalculateNationwidePathways.Start();
	CalculateNationwidePathways();
	WatchCalculateNationwidePathways.Stop();

	WatchCalculateAIRegionOwnership.Start();
	CalculateAIRegionOwnership();
	CalculateMultiRegionOwnership();
	WatchCalculateAIRegionOwnership.Stop();
	WatchTotal.Stop();

	float fWatchGenerateAIRegions = WatchGenerateAIRegions.GetDeltaSeconds();
	float fWatchGenerateMultiRegions = WatchGenerateMultiRegions.GetDeltaSeconds();
	float fWatchCalculateUnitCombatCapabilities = WatchCalculateUnitCombatCapabilities.GetDeltaSeconds();
	float fWatchCalculateNationalSummaries = WatchCalculateNationalSummaries.GetDeltaSeconds();
	float fWatchAddObjectIdsToAIRegionsAndMultiRegions = WatchAddObjectIdsToAIRegionsAndMultiRegions.GetDeltaSeconds();
	float fWatchCalculateNationwidePathways = WatchCalculateNationwidePathways.GetDeltaSeconds();
	float fWatchCalculateAIRegionOwnership = WatchCalculateAIRegionOwnership.GetDeltaSeconds();
	float fWatchTotal = WatchTotal.GetDeltaSeconds();

	CString strText;
	strText.Format( _T("  WatchGenerateAIRegions = %0.2f Seconds"), WatchGenerateAIRegions.GetDeltaSeconds() ); g_LogFile.Write( strText );
	strText.Format( _T("  WatchGenerateMultiRegions = %0.2f Seconds"), WatchGenerateMultiRegions.GetDeltaSeconds() ); g_LogFile.Write( strText );
	strText.Format( _T("  WatchCalculateUnitCombatCapabilities = %0.2f Seconds"), WatchCalculateUnitCombatCapabilities.GetDeltaSeconds() ); g_LogFile.Write( strText );
	strText.Format( _T("  WatchCalculateNationalSummaries = %0.2f Seconds"), WatchCalculateNationalSummaries.GetDeltaSeconds() ); g_LogFile.Write( strText );
	strText.Format( _T("  WatchAddObjectIdsToAIRegionsAndMultiRegions = %0.2f Seconds"), WatchAddObjectIdsToAIRegionsAndMultiRegions.GetDeltaSeconds() ); g_LogFile.Write( strText );
	strText.Format( _T("  WatchCalculateNationwidePathways = %0.2f Seconds"), WatchCalculateNationwidePathways.GetDeltaSeconds() ); g_LogFile.Write( strText );
	strText.Format( _T("  WatchCalculateAIRegionOwnership = %0.2f Seconds"), WatchCalculateAIRegionOwnership.GetDeltaSeconds() ); g_LogFile.Write( strText );
	strText.Format( _T("  WatchTotal = %0.2f Seconds"), WatchTotal.GetDeltaSeconds() ); g_LogFile.Write( strText );

	m_bDataIsValid = true;
	int g=0;
}
*/

/*
long CCommonData::GetNumberOfHumanPlayers()
{
	long iPlayers = 0;
	for( long iPlayer=1; iPlayer<m_Players.m_iSize; iPlayer++ )
	{
		EOSAI::CGamePlayer* pPlayer = GetGamePlayer( iPlayer );
		if( pPlayer->IsHuman() ) iPlayers++;
	}
	return iPlayers;
}

long CCommonData::GetNumberOfActivePlayers() // Players who are alive
{
//	ASSERT( false );
//	return 0;

	long iPlayers = 0;
	for( long iPlayer=1; iPlayer<m_Players.m_iSize; iPlayer++ )
	//POSITION pos = m_PlayerList.GetHeadPosition();
	//while( pos )
	{
		//EOSAI::CGamePlayer* pPlayer = m_PlayerList.GetNext( pos );
		EOSAI::CGamePlayer* pPlayer = GetGamePlayer( iPlayer );
		if( pPlayer->IsAlive() ) iPlayers++;
		//ASSERT( pPlayer->GetPlayerNumber() <= GetNumberOfPlayers() );
	}
	return iPlayers;
}
*/
/*
long CCommonData::GetNumberOfActiveAIPlayers()
{
	int iCount = 0;
	/-*
	POSITION pos = m_pWorldBuildDesc->GetPlayerList()->GetHeadPosition();
	while( pos )
	{
		CPlayer* pPlayer = m_pWorldBuildDesc->GetPlayerList()->GetNext( pos );
		if( pPlayer->IsHuman() == false )
		{
			if( pPlayer->GetPlayerHasBeenEliminated() == false )
			{
				iCount++;
			}
		}
	}
	return iCount;
	*-/
	for( long i=0; i<m_Players.m_iSize; i++ )
	{
		EOSAI::CGamePlayer* pPlayer = m_Players[i];
		if( pPlayer && pPlayer->IsAI() ) iCount++;
	}
	return iCount;
}
*/

bool CCommonData::HasSetSneakAttack( long iAttacker, long iTarget )
{
	EOSAI::AIPlayer* pAIPlayer = g_pEOSAIInterface->GetAIPlayer( iAttacker );
	if( pAIPlayer )
	{
		return pAIPlayer->HasSetSneakAttackAgainst( iTarget );
	}
	ASSERT( false ); // this shouldn't happen
	return false;
}

void CCommonData::AddPlayerInteraction(CEOSAIPlayerInteraction* pPlayerInteraction)
{
	ASSERT(pPlayerInteraction->ValidateValues());
	if (pPlayerInteraction->ValidateValues() == false) return; // Cannot process incomplete player interactions

	ASSERT(pPlayerInteraction->m_iEventTurn > -1);

	ASSERT(pPlayerInteraction->m_iInteractionId == 0);
	if (pPlayerInteraction->m_iInteractionId == 0) { pPlayerInteraction->m_iInteractionId = m_iHighestPlayerInteractionIdInList +1;  }

	m_PlayerInteractions.AddTail(pPlayerInteraction);
	m_iHighestPlayerInteractionIdInList++;
}

void CCommonData::ProcessPlayerInteractionsIfNecessary()
{
	if (m_iHighestPlayerInteractionIdInList != m_iHighestPlayerInteractionIdProcessed)
	{
		// TODO: This needs to be changed - everything below this line needs to happen in a different thread.
		CalculateForeignRelationsFeelingsBasedOnPlayerInteractionHistoryAndSendFeelingsUpdate(g_pEOSAIInterface->GetCurrentTurn());
		m_iHighestPlayerInteractionIdProcessed = m_iHighestPlayerInteractionIdInList;
	}
}

long CCommonData::GetPlayerInteraction_WarDuration( long iPlayer1, long iPlayer2 )
{
	if( GetForeignRelations( iPlayer1, iPlayer2 ) != EOSAIEnumForeignRelations::enum_War ) return -1;

	//long iWarStart = GetCommonState()->GetWorldDescServer()->GetWarStartTurn( GetPlayerNumber(),iOtherPlayer );
	long iStartTurn = 0;
	POSITION pos = m_PlayerInteractions.GetHeadPosition();
	while( pos )
	{
		CEOSAIPlayerInteraction* pPlayerInteraction = m_PlayerInteractions.GetNext( pos );
		CEOSAIPlayerInteraction_DeclaredWar* pDeclaredWar = dynamic_cast< CEOSAIPlayerInteraction_DeclaredWar* >( pPlayerInteraction );
		if( pDeclaredWar )
		{
			if( ( pDeclaredWar->m_iActor == iPlayer1 && pDeclaredWar->m_iTarget == iPlayer2 ) ||
				( pDeclaredWar->m_iActor == iPlayer2 && pDeclaredWar->m_iTarget == iPlayer1 ) )
			{
				iStartTurn = max( iStartTurn, pDeclaredWar->m_iEventTurn );
			}
		}
		CEOSAIPlayerInteraction_SneakAttack* pSneakAttack = dynamic_cast< CEOSAIPlayerInteraction_SneakAttack* >( pPlayerInteraction );
		if( pSneakAttack )
		{
			if( ( pSneakAttack->m_iActor == iPlayer1 && pSneakAttack->m_iTarget == iPlayer2 ) ||
				( pSneakAttack->m_iActor == iPlayer2 && pSneakAttack->m_iTarget == iPlayer1 ) )
			{
				iStartTurn = max( iStartTurn, pSneakAttack->m_iEventTurn );
			}
		}
	}
	return g_pEOSAIInterface->GetCurrentTurn() - iStartTurn;
}

void CCommonData::CalculateForeignRelationsFeelingsBasedOnPlayerInteractionHistoryAndSendFeelingsUpdate(int iCurrentTurn)
{
	// Clear Feelings and Stance
	//m_AIGlobalForeignRelations.ResetFeelings();

	ASSERT(iCurrentTurn == g_pEOSAIInterface->GetCurrentTurn());

	CEOSAIGlobalForeignRelations ForeignRelations;
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CString strA = m_CurrentForeignRelations.OutputDebugString();
	POSITION pos = GetPlayerInteractions()->GetHeadPosition();
	while (pos)
	{
		CEOSAIPlayerInteraction* pInteraction = GetPlayerInteractions()->GetNext(pos);
		pInteraction->UpdateForeignRelationsState( iCurrentTurn,
			ForeignRelations.GetForeignRelations(),
			ForeignRelations.GetFeelings());
			//m_AIGlobalForeignRelations.GetForeignRelations(),
			//m_AIGlobalForeignRelations.GetFeelings());
	}
	m_AIGlobalForeignRelations = ForeignRelations;

	//float f1a = ForeignRelations.GetFeelings01(1, 2);
	//float f1b = m_AIGlobalForeignRelations.GetFeelings01(1, 2);
    //EOSAIEnumForeignRelations r1a = ForeignRelations.GetForeignRelations(1, 2);
	//EOSAIEnumForeignRelations r1b = m_AIGlobalForeignRelations.GetForeignRelations(1, 2);

	// Send Foreign Relations Feelings Update
	EOSAI::MessageFromAI_ForeignRelationsFeelings* pFeelings = new EOSAI::MessageFromAI_ForeignRelationsFeelings();
	pFeelings->Set(this->GetGlobalForeignRelations());
	g_pEOSAIInterface->SendMessageFromAI(pFeelings);
}

// Might need to fix this
//CEOSAIGeo*  CCommonData::GetGeo( long iGeo ){ return m_EOSAIGeos[iGeo]; }
CEOSAIGeo*  CCommonData::GetAIGeo( long iGeo ){ return m_AIGeoArray[iGeo]; }
CEOSAIGeo*  CCommonData::GetAIGeo( CEOSAILocation Location ){ return GetAIGeo( GetGeoId(Location) ); }
long        CCommonData::GetGeoId( CEOSAILocation Location ){ return g_pEOSAIInterface->GetGeoId( Location ); }

void  CCommonData::AddAdjacentWaterGeos( CEOSAIIntSet& Geos )
{
	//ASSERT( m_bAIPlayer == false );
	CEOSAIIntSet OldGeos;
	OldGeos.Set( Geos );

	POSITION pos = OldGeos.GetList()->GetHeadPosition();
	while( pos )
	{
		long iOldGeo = OldGeos.GetList()->GetNext( pos );
		CEOSAIGeo* pGeo = GetAIGeo( iOldGeo );
		ASSERT( pGeo );
		if( pGeo )
		{
			POSITION pos2 = pGeo->GetConnectedSet()->GetList()->GetHeadPosition();
			while( pos2 )
			{
				long iAdjGeo = pGeo->GetConnectedSet()->GetList()->GetNext( pos2 );
				CEOSAIGeo* pAdjGeo = GetAIGeo( iAdjGeo );
				ASSERT( pAdjGeo );
				if( pAdjGeo && pAdjGeo->GetGeoType() == CEOSAIGeo::Water && Geos.Exists( iAdjGeo ) == false )
				{
					Geos.Add( iAdjGeo );
				}
			}
		}
	}
}

void  CCommonData::AddAdjacentLandGeos( CEOSAIIntSet& Geos )
{
	//ASSERT( m_bAIPlayer == false );
	CEOSAIIntSet OldGeos;
	OldGeos.Set( Geos );

	POSITION pos = OldGeos.GetList()->GetHeadPosition();
	while( pos )
	{
		long iOldGeo = OldGeos.GetList()->GetNext( pos );
		CEOSAIGeo* pGeo = GetAIGeo( iOldGeo );
		ASSERT( pGeo );
		if( pGeo )
		{
			POSITION pos2 = pGeo->GetConnectedSet()->GetList()->GetHeadPosition();
			while( pos2 )
			{
				long iAdjGeo = pGeo->GetConnectedSet()->GetList()->GetNext( pos2 );
				CEOSAIGeo* pAdjGeo = GetAIGeo( iAdjGeo );
				ASSERT( pAdjGeo );
				if( pAdjGeo && pAdjGeo->GetGeoType() == CEOSAIGeo::Land )//&& Geos.Exists( iAdjGeo ) == false )
				{
					Geos.Add( iAdjGeo );
				}
			}
		}
	}
}

void CCommonData::SetPlayerResources(int iPlayer, CString strResource, float fResourceAmount)
{
	m_AINationalSummaries[iPlayer]->m_ResourceSummary.m_TotalResources.Set(strResource, fResourceAmount);
}

long CCommonData::GetNumberOfPoiOwnedByPlayer( long iPlayer )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	long iCount = 0;
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	//POSITION pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		CEOSAIPoiObject* pPoiObject = m_AIPoiObjects.GetNext( pos );
		if( pPoiObject && 
			pPoiObject->GetOwner() == iPlayer &&
			pPoiObject->IsAlive() )
		{
			iCount++;
		}
	}
	return iCount;

	/*
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	long iCount = 0;
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		CPoiObject* pPoiObject = pWorldDescServer->GetActorsList()->GetNext( pos );
		if( pPoiObject && 
			pPoiObject->GetOwner() == iPlayer &&
			pPoiObject->IsAlive() )
		{
			iCount++;
		}
	}
	return iCount;
	*/
}

long  CCommonData::GetNumberOfCitiesOwnedByPlayer( long iPlayer )
{
	/*
	ASSERT( false ); // Fix this function
	return 0;
	*/
	long iCount = 0;
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	//POSITION pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
	//POSITION pos = g_pAICommonState->GetAIPoiObjects()->GetHeadPosition();
	POSITION pos = this->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		//CPoiObject* pPoiObject = pWorldDescServer->GetActorsList()->GetNext( pos );
		CEOSAIPoiObject* pAIPoiObject = this->GetAIPoiObjects()->GetNext( pos );
		if( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City )
		{
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity && pAICity->GetOwner() == iPlayer )
			{
				iCount++;
			}
		}
	}
	return iCount;
}

long CCommonData::GetNumberOfBuildingsOwnedByPlayer( long iPlayer, CEOSAIBuildingDescription* pBuildingDesc )
{
	/*
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity && pAICity->GetAI
	}
	*/
	long iCount = 0;
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			// We use "GetOwner" instead of "GetCurrentOwner" because of the cost associated
			//   with reading all the events
			if( pAICity && pAICity->GetOwner() == iPlayer )
			{
				iCount = pAICity->GetBuildingCount( pBuildingDesc );
				/*
				POSITION pos = pAICity->GetBuildingList()->GetHeadPosition();
				while( pos )
				{
					CEOSAIBuildingDescription* pBuildingDescInList = pAICity->GetBuildingList()->GetNext( pos );
					if( pBuildingDesc == pBuildingDescInList )
					{
						iCount++;
					}
				}
				*/
			}
		}
	}
	return iCount;
}

long CCommonData::GetNumberOfBuildingsInProductionByPlayer( long iPlayer, CEOSAIBuildingDescription* pBuildingDesc )
{
	long iCount = 0;
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );

		//if( pPoiObject->GetPoiObjectType2() == EnumPoiObjectType::enum_City )
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			// We use "GetOwner" instead of "GetCurrentOwner" because of the cost associated
			//   with reading all the events
			//CCity* pCity = ( CCity* ) pPoiObject;
			if( pAICity && pAICity->GetOwner() == iPlayer )
			{
				return pAICity->GetBuildQueueCount( pBuildingDesc->GetInternalName() );
				/*
				POSITION pos = pAICity->GetBuildOrders()->GetHeadPosition();
				while( pos )
				{
					CBuildOrder* pBuildOrder = pAICity->GetBuildOrders()->GetNext( pos );
					if( pBuildOrder->GetBuildOption()->IsABuilding() &&
						pBuildOrder->GetBuildOption()->GetBuildingDescription() == pBuildingDesc )
					{
						iCount++;
					}
				}
				*/
			}
		}
	}
	return iCount;
}

CEOSAIPoiObject* CCommonData::GetAIPoiObject( long iObjectId )
{
	//CAIObject* pAIObject = GetAIObject( iObjectId );
	//return dynamic_cast< CEOSAIPoiObject* >( pAIObject );
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		if( pAIPoiObject->GetObjectId() == iObjectId )
		{
			return pAIPoiObject;
		}
	}
	return NULL;
}

void CCommonData::AddAIPoiObject( CEOSAIPoiObject* pAIPoiObject ) // Used by AIBrains
{
	// Look for this AIPoiObject in the list already
#ifdef _DEBUG
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObjectInList = m_AIPoiObjects.GetNext( pos );
		ASSERT( pAIPoiObjectInList->GetObjectId() != pAIPoiObject->GetObjectId() );
	}
#endif _DEBUG

	ASSERT( pAIPoiObject && pAIPoiObject->IsHypotheticalPoiObject() == false );
	m_AIPoiObjects.AddTail( pAIPoiObject );
}

void CCommonData::AddHypotheticalAIPoiObject( CEOSAIPoiObject* pAIPoiObject ) // Used by AIBrains
{
	ASSERT( pAIPoiObject && pAIPoiObject->IsHypotheticalPoiObject() );
	m_AIPoiObjects.AddTail( pAIPoiObject );
}

CEOSAIUnit2* CCommonData::GetAIUnit( long iObjectId )
{
	CEOSAIPoiObject* pAIObject = GetAIPoiObject( iObjectId );
	return dynamic_cast< CEOSAIUnit2* >( pAIObject );
}

CEOSAIThoughtDatabase* CCommonData::GetAIThoughtDatabase( long iPlayer )
{
/*
	//CEOSAIThoughtDatabase* pAIThoughtDatabase = g_pEOSAICommonData->GetAIThoughtDatabase( GetAIUnitPathwayFinder()->GetUnitOwner() );
	CEOSAIThoughtDatabase* pAIThoughtDatabase = g_pEOSAIInterface->GetAIPlayer(iPlayer)->GetAIBrain()->GetAIThoughtDatabase();
	return pAIThoughtDatabase;
*/
	EOSAI::AIPlayer* pAIPlayer = g_pEOSAIInterface->GetAIPlayer(iPlayer);
	CEOSAIBrain* pEOSAIBrain = pAIPlayer->GetAIBrain();
	CEOSAIThoughtDatabase* pAIThoughtDatabase = pEOSAIBrain->GetAIThoughtDatabase();
	//CEOSAIThoughtDatabase* pAIThoughtDatabase = g_pEOSAICommonData->GetAIThoughtDatabase( GetAIUnitPathwayFinder()->GetUnitOwner() );
	//CEOSAIThoughtDatabase* pAIThoughtDatabase = g_pEOSAIInterface->GetAIPlayer(iPlayer)->GetAIBrain()->GetAIThoughtDatabase();
	return pAIThoughtDatabase;
}

void  CCommonData::AddObjectIdsToAIRegionsAndMultiRegions()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	// Clear-out all the poi objects
	long iNumberOfAIRegions = m_AIRegionManager.GetAIRegionArray()->GetSize();
	for( long iAIRegion=0; iAIRegion<iNumberOfAIRegions; iAIRegion++ )
	{
		CEOSAIRegion2* pAIRegion = m_AIRegionManager.GetAIRegion( iAIRegion );
		if( pAIRegion ){ pAIRegion->ClearPoiData(); }
	}
	POSITION pos = m_MultiRegionManager.GetMultiRegionList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIMultiRegion2* pMultiRegion = m_MultiRegionManager.GetMultiRegionList()->GetNext( pos );
		if( pMultiRegion ){ pMultiRegion->ClearPoiData(); }
	}

	// Setup the PlayerControl values
	/*
	pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		//
		if( pPoiObject && 
			pPoiObject->IsCityResourceAirfieldOrUnit() &&
			pPoiObject->IsDead() == false )
		{
			CEOSAIRegion2* pAIRegion = m_AIRegionManager.GetAIRegion( pPoiObject->GetLocation() );
			ASSERT( pAIRegion );
			if( pAIRegion )
			{
				pAIRegion->AddToPlayerControl( pPoiObject );
			}
		}
	}
	*/

	// Setup PlayerCityUnitAIPower values
	//pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	//pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
	pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		//CPoiObject* pPoiObject = pWorldDescServer->GetActorsList()->GetNext( pos );
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		//
		if( pAIPoiObject && 
			pAIPoiObject->IsCityResourceAirfieldOrUnit() &&
			pAIPoiObject->IsDead() == false )
		{
			CEOSAIRegion2* pAIRegion = m_AIRegionManager.GetAIRegion( pAIPoiObject->GetLocation() );
			ASSERT( pAIRegion );
			if( pAIRegion )
			{
				pAIRegion->AddPoiObjectToSummary( pAIPoiObject );
				long i = pAIRegion->GetCitResUnitCount( pAIPoiObject->GetOwner() );
				ASSERT( i > 0 );
			}

			CEOSAIMultiRegion2* pMultiRegion = m_MultiRegionManager.GetMultiRegion( pAIPoiObject->GetLocation() );
			ASSERT( pMultiRegion );
			if( pMultiRegion )
			{
				pMultiRegion->AddPoiObjectToSummary( pAIPoiObject );
			}

			/*
			POSITION pos = m_MultiRegionManager.GetMultiRegionList()->GetHeadPosition();
			while( pos )
			{
				CMultiRegion* pMultiRegion = m_MultiRegionManager.GetMultiRegionList()->GetNext( pos );
				pMultiRegion->UpdateDistancesBasedOnPoiObjects( pPoiObject );
			}
			*/
		}
	}
}


void  CCommonData::CalculateNationwidePathways()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CWorldBuildDesc*  pWorldBuildDesc = pWorldDescServer->GetWorldBuildDesc();

	// Delete the existing nationwide pathways
	for( long i=0; i<m_NationwidePlayerPathways.m_iSize; i++ )
	{
		delete m_NationwidePlayerPathways.Value( i );
		m_NationwidePlayerPathways.Value( i ) = NULL;
	}
	if( m_NationwidePlayerPathways.m_iSize != g_pEOSAICommonData->GetNumberOfPlayers()+1 )
	{
		m_NationwidePlayerPathways.SetSize( g_pEOSAICommonData->GetNumberOfPlayers()+1 );
	}
	for( long iPlayer=0; iPlayer<m_NationwidePlayerPathways.m_iSize; iPlayer++ )
	{
		m_NationwidePlayerPathways[ iPlayer ] = NULL;
		if( iPlayer > 0 )
		{
			EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
			ASSERT( pPlayer );

			CEOSAIMultiRegionNationwidePathways* pNationwidePathway = new CEOSAIMultiRegionNationwidePathways();
			m_NationwidePlayerPathways[ iPlayer ] = pNationwidePathway;

			pNationwidePathway->SetPlayer( iPlayer );
			pNationwidePathway->SetMultiRegionManager( GetMultiRegionManager() );

			long iNumberOfPoiOwnedByPlayer = GetNumberOfPoiOwnedByPlayer( iPlayer );
			long iNumberOfCitiesOwnedByPlayer = GetNumberOfCitiesOwnedByPlayer( iPlayer );
			//if( pPlayer->GetPlayerHasBeenEliminated() == false && 
			if( pPlayer->IsAlive() && 
				iNumberOfPoiOwnedByPlayer > 0 &&
				iNumberOfCitiesOwnedByPlayer > 0 )
			{
				pNationwidePathway->Calculate();
				//pNationwidePathway->CalculateSightingAndPathwayInformation();
				//pNationwidePathway->CalculatePathwayPoiObjectsForEachMultiRegion();
			}
			else
			{
				// this will happen when a player is eliminated, but shouldn't happen otherwise
				ASSERT( pPlayer->IsAlive() == false );

				//CPlayer* pPlayer = GetCommonState()->GetPlayer( iPlayer );
				//ASSERT ( false ); // this will happen when a player is eliminated, but shouldn't happen otherwise
			}
		}
	}
}


void CCommonData::CalculateNationalSummaries()
{
	// Totals
	m_fTotalCityProductionOnMap = 0.0f;
	m_fTotalCitResProductionOnMap = 0.0f;
	m_fTotalUnownedCitResProductionOnMap = 0.0f;
	m_fTotalResourcesOnMap = 0.0f;
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	//POSITION pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		if( dynamic_cast< CEOSAICity* >( pAIPoiObject ) )
		{
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			//if( pCity->IsAlive() )
			{
				m_fTotalCityProductionOnMap += pAICity->GetTotalProduction();//GetTotalProduction( false,false );
				m_fTotalCitResProductionOnMap += pAICity->GetTotalProduction();//GetTotalProduction( false,false );
				if( pAICity->GetOwner() == 0 )
				{
					m_fTotalUnownedCitResProductionOnMap += pAICity->GetTotalProduction();//GetTotalProduction( false,false );
				}
			}
		}
		if( dynamic_cast< CEOSAIResource* >( pAIPoiObject ) )
		{
			CEOSAIResource* pRes = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
			if( pRes )
			{
				m_fTotalResourcesOnMap += (float) pRes->GetResourcePerTurn();
				m_fTotalCitResProductionOnMap += (float) pRes->GetResourcePerTurn();
				if( pRes->GetOwner() == 0 )
				{
					m_fTotalUnownedCitResProductionOnMap += (float) pRes->GetResourcePerTurn();
				}
			}
			else
			{
				ASSERT( false );
			}
		}
	}
	//

	//long iNumberOfPlayers = pWorldBuildDesc->GetNumberOfPlayers();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	if( m_AINationalSummaries.m_iSize == 0 )
	{
		m_AINationalSummaries.SetSize( iNumberOfPlayers+1 );
		m_AINationalSummaries[0] = NULL;
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			m_AINationalSummaries[iPlayer] = new CEOSAINationalSummary3();
			m_AINationalSummaries[iPlayer]->SetPlayer( iPlayer );
		}
	}
	ASSERT( iNumberOfPlayers+1 == m_AINationalSummaries.m_iSize );
	for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		if( m_AINationalSummaries[iPlayer] == NULL )
		{
			m_AINationalSummaries[iPlayer] = new CEOSAINationalSummary3();
			m_AINationalSummaries[iPlayer]->SetPlayer( iPlayer );
		}
		m_AINationalSummaries[iPlayer]->CalculatePlayerPower();
		m_AINationalSummaries[iPlayer]->CalculatePlayerToPlayerAccessibility();
		m_AINationalSummaries[iPlayer]->CalculateResourceDeltas();
	}
}

/*
void CCommonData::CalculateThoughtDatabases()
{
	// Delete the existing nationwide pathways
	for( long i=0; i<m_AIThoughtDatabases.m_iSize; i++ )
	{
		delete m_AIThoughtDatabases.Value( i );
		m_AIThoughtDatabases.Value( i ) = NULL;
	}
	if( m_AIThoughtDatabases.m_iSize != g_pEOSAICommonData->GetNumberOfPlayers()+1 )
	{
		m_AIThoughtDatabases.SetSize( g_pEOSAICommonData->GetNumberOfPlayers()+1 );
	}
	ASSERT( false );	
}
*/
void CCommonData::CalculateAIUnitCombatCapabilities( long iCurrentTurn )
{
	ASSERT( m_iUnitCombatCapabilities_LastTurnCalculated != iCurrentTurn );
	if( m_iUnitCombatCapabilities_LastTurnCalculated != iCurrentTurn )
	{
		m_iUnitCombatCapabilities_LastTurnCalculated = iCurrentTurn;

		/*
		CWorldDesc* pWorldDesc = g_pCommonState->GetWorldDescServer();//m_pWorldDescServer;
		if( pWorldDesc == NULL )
		{
			POSITION pos = g_pCommonState->GetLocalPlayerWorldDescList()->GetHeadPosition();
			while( pos )
			{
				CWorldDescPlayer* pWorldDescPlayer = g_pCommonState->GetLocalPlayerWorldDescList()->GetNext( pos );
				if( pWorldDescPlayer->GetCurrentTurn() == iCurrentTurn )
				{
					pWorldDesc = pWorldDescPlayer;
					break;
				}
			}
		}
		ASSERT( pWorldDesc );
		if( pWorldDesc == NULL ) return;
		*/

		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		//CWorldBuildDesc*  pWorldBuildDesc = pWorldDescServer->GetWorldBuildDesc();
		//CWorldBuildDesc* pWorldBuildDesc = pWorldDesc->GetWorldBuildDesc();

		// Compile a list of all UnitTemplates in the game
		CEOSAIUnitTemplateSet  AllUnitsInTheGame;
		POSITION pos = m_AIPoiObjects.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
			//CUnit* pUnit = dynamic_cast< CUnit* >( pAIPoiObject );
			//if( pUnit )
			if( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit )
			{
				CEOSAIUnit2* pAIUnit = (CEOSAIUnit2*) pAIPoiObject;
				AllUnitsInTheGame.AddUnitTemplate( pAIUnit->GetAIUnitTemplate() );//pUnit->GetUnitTemplate() );
			}
		}
/*
		I think I need to explicitly create the EOSAIBuildOptions in EOSAIInterface_EOS
			The original code in AICommonData uses CBuildOption (there's no AI equivalent in the regular code)
*/
		// Compile all the buildable units in the game
		long iNumberOfBuildableUnits = 0;
		//long iNumberOfPlayers = pWorldBuildDesc->GetNumberOfPlayers();
		long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
		//pos = g_pEOSAICommonData->GetActiveUnitset()->GetBuildOptionsList()->GetHeadPosition();
		ASSERT( g_pEOSAICommonData->GetAIBuildOptionList()->GetCount() > 0 );
		pos = g_pEOSAICommonData->GetAIBuildOptionList()->GetHeadPosition();
		while( pos )
		{
			CEOSAIBuildOption* pAIBuildOption = g_pEOSAICommonData->GetAIBuildOptionList()->GetNext( pos );
			if( pAIBuildOption->IsAUnit() == false ) continue;
			//
			bool bCanBuild = false;
			for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
			{
				if( g_pEOSAIInterface->GetAIGameRules()->CanBuild( iPlayer, pAIBuildOption, false ) )
				{
					bCanBuild = true;
					break;
				}
			}
			if( bCanBuild )
			{
				iNumberOfBuildableUnits++;
				AllUnitsInTheGame.AddUnitTemplate( pAIBuildOption->GetAIUnitTemplate() );
				//ASSERT( false );
			}
		}
		ASSERT( iNumberOfBuildableUnits > 0 );

		// Invoke all the CombatAttrition values between MyUnits and PotentialEnemy units
		ASSERT( AllUnitsInTheGame.m_List.IsEmpty() == FALSE );
		pos = AllUnitsInTheGame.m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pMyAIUnit = AllUnitsInTheGame.m_List.GetNext( pos );
			pMyAIUnit->GetAIUnitCombatCapability()->CalculateAverageVisibleRange( &AllUnitsInTheGame );
			int g=0;
		}
		pos = AllUnitsInTheGame.m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pMyAIUnit = AllUnitsInTheGame.m_List.GetNext( pos );
			pMyAIUnit->GetAIUnitCombatCapability()->CalculateRelativeValue( &AllUnitsInTheGame, &AllUnitsInTheGame );
			int g=0;
		}
	}
}

void CCommonData::CalculateAIRegionOwnership()
{
	// Clear-out the ownership
	long iPlayerArraySize = 0;//pAIRegion->m_PlayerControlValue.m_iSize;
	long iArraySize = m_AIRegionManager.GetAIRegionArray()->GetSize();//UpperBound();
	for( long iAIRegion=0; iAIRegion<iArraySize; iAIRegion++ )
	{
		CEOSAIRegion2* pAIRegion = m_AIRegionManager.GetAIRegion( iAIRegion );
		if( pAIRegion )
		{
			pAIRegion->m_iSingleOwner = 0;
			pAIRegion->m_fSingleOwnerControl = 0.0f;

			iPlayerArraySize = pAIRegion->m_PlayerCitResControlValue01.m_iSize;
			for( long iPlayer=0; iPlayer<iPlayerArraySize; iPlayer++ )
			{
				pAIRegion->m_PlayerCitResControlValue01[iPlayer] = 0.0f;
			}
		}
	}
	// Give players 1.0 control over all AIRegions where the player has a CitRes
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	for( long iPlayer=0; iPlayer<iPlayerArraySize; iPlayer++ )
	{
		//CPlayer* pPlayer = GetCommonState()->GetPlayer( iPlayer );
		//if( pPlayer && pPlayer->IsAlive() == false ) continue;
		// Only create a map if there are CitRes owned by this player
		/*
		bool bCitResFound = false;
		POSITION pos = pWorldDescServer->GetMapObjectList()->GetHeadPosition();
		while( pos )
		{
			CMapObject* pMapObject = pWorldDescServer->GetMapObjectList()->GetNext( pos );
			if( dynamic_cast< CCity* >( pMapObject ) ||
				dynamic_cast< CResourceSource* >( pMapObject ) )
			{
				CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pMapObject );
				if( pPoiObject && pPoiObject->GetOwner() == iPlayer ){ bCitResFound = true; break; }
			}
		}
		*/
		bool bCitResFound = false;
		POSITION pos = m_AIPoiObjects.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
			if( dynamic_cast< CEOSAICity* >( pAIPoiObject ) ||
				dynamic_cast< CEOSAIResource* >( pAIPoiObject ) )
			{
				//CEOSAIPoiObject* pPoiObject = dynamic_cast< CEOSAIPoiObject* >( pMapObject );
				if( pAIPoiObject && pAIPoiObject->GetOwner() == iPlayer ){ bCitResFound = true; break; }
			}
		}
		if( bCitResFound == false ) continue;

		//CEOSAIRegionPathfinder& RegionPathfinder = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetAIRegionManager()->GetPathfinder();
		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		//CEOSAIMultiRegionManager2* pMultiRegionManager = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager();
		//
		CEOSAIRegionPathwayMap  ResultMap;
		RegionPathfinder.ClearPathfindingSettings();
		//RegionPathfinder.SetDistanceMultiplier( 1.0f );
		RegionPathfinder.SetGeoSwitchMultiplier( 60.0f );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iPlayer );
		RegionPathfinder.AddPlayerCitResRegionsAsStartRegions( iPlayer );
		RegionPathfinder.Solve( iPlayer, true );
		RegionPathfinder.GetResult( true, &ResultMap );

		// Add all cities and resources to the starting points
		//pAIRegion->m_PlayerControlValue[iPlayer] = 0.0f;
		for( long iAIRegion=1; iAIRegion<iArraySize; iAIRegion++ )
		{
			CEOSAIRegionPathwayItem* pPathwayItem = ResultMap.GetRegionResult( iAIRegion );
			CEOSAIRegion2* pAIRegion = m_AIRegionManager.GetAIRegion( iAIRegion );
			if( pAIRegion )
			{
				//pAIRegion->m_iOwner = 0;
				//pAIRegion->m_fOwnerControl = 0.0f;

				//for( long iPlayer=0; iPlayer<pAIRegion->m_PlayerControlValue.m_iSize; iPlayer++ )
				//{
				//	pAIRegion->m_PlayerControlValue[iPlayer] = 0.0f;
				//}
				float fPathfindingValue = pPathwayItem->m_fPathfindingValue;
				if( fPathfindingValue < 350.0f )
				{
					float fValue = (350.f-fPathfindingValue)/350.0f;
					if( iPlayer == 0 ) fValue /= 2.0f;
					pAIRegion->m_PlayerCitResControlValue01[iPlayer] = fValue;
				}
				else
				{
					pAIRegion->m_PlayerCitResControlValue01[iPlayer] = 0.0f;
				}
			}
		}
	}

	// Now that we've set the PlayerControlValues for every AIRegion, determine 'control'
	for( long iAIRegion=1; iAIRegion<iArraySize; iAIRegion++ )
	{
		//CEOSAIRegionPathwayItem* pPathwayItem = ResultMap.GetRegionResult( iAIRegion );
		CEOSAIRegion2* pAIRegion = m_AIRegionManager.GetAIRegion( iAIRegion );
		if( pAIRegion )
		{
			long iTopPlayer = 0;
			float fTopControl = 0.0f;
			for( long iPlayer=0; iPlayer<iPlayerArraySize; iPlayer++ )
			{
				if( pAIRegion->m_PlayerCitResControlValue01[iPlayer] > fTopControl )
				{
					iTopPlayer = iPlayer;
					fTopControl = pAIRegion->m_PlayerCitResControlValue01[iPlayer];
				}
			}
			// Find the control gap between the strongest player and the second strongest
			float fSecondTopControl = 0.0f;
			for( long iPlayer=0; iPlayer<iPlayerArraySize; iPlayer++ )
			{
				if( iPlayer == iTopPlayer ) continue;
				float fControl = pAIRegion->m_PlayerCitResControlValue01[iPlayer];
				fSecondTopControl = max( fSecondTopControl, fControl );
			}
			float fControlGap = fTopControl - fSecondTopControl;

			pAIRegion->m_iSingleOwner = iTopPlayer;
			pAIRegion->m_fSingleOwnerControl = (fControlGap + fTopControl)/2.0f;
		}
	}

/*
	CArray< CEOSAIRegion2* >* pAIRegionArray = m_AIRegionManager.GetAIRegionArray();
	long iNumberOfAIRegions = pAIRegionArray->GetSize();
	m_AIRegionManager.m_AIRegionArray

	// Borders - players tend to assert 'ownership' over areas of land.
	long  m_iOwner;
	float m_fOwnerControl; // 0.0 - 1.0
*/

	// adaspkoadslk
	int g=0;
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
void  CCommonData::CalculateMultiRegionOwnership()
{
	//long iAIPlayer = GetAIBrain()->GetAIPlayerNumber();
	long PlayerControl[ EOSAI_MAX_NUMBER_OF_PLAYERS+1 ];
	for( long iPlayer=0; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS+1; iPlayer++ )
	{
		//PlayerControl[iPlayer] = 0.0f;
		PlayerControl[iPlayer] = 0;
	}

	//CWorldBuildDesc* pWorldBuildDesc = g_pCommonState->GetWorldBuildDesc();

	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();
	long iNumberOfMultiRegions = pMultiRegionManager->GetNumberOfMultiRegions();
	for( long iMultiRegion=1; iMultiRegion<iNumberOfMultiRegions; iMultiRegion++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = pMultiRegionManager->GetMultiRegion( iMultiRegion );
		if( pMultiRegion == NULL ) continue;

		float fTotalControl = 0.0f;
		POSITION pos2 = pMultiRegion->m_AIRegions.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIRegion2* pAIRegion = pMultiRegion->m_AIRegions.GetNext( pos2 );
			pMultiRegion->m_OwnerControl01[ pAIRegion->m_iSingleOwner ] += pAIRegion->m_fSingleOwnerControl;
			fTotalControl += 1.0f;
		}

		// Look at the nearby MultiRegions
		POSITION pos = pMultiRegionManager->GetMultiRegionList()->GetHeadPosition();
		while( pos )
		{
			CEOSAIMultiRegion2* pMultiRegion2 = pMultiRegionManager->GetMultiRegionList()->GetNext( pos );
			if( pMultiRegion2 == pMultiRegion ) continue;
			//float fDistance = pWorldBuildDesc->GetPixelDistance( pMultiRegion->GetLocation(), pMultiRegion2->GetLocation() );
			float fDistance = g_pWorldDistanceTool->GetDistance( pMultiRegion->GetLocation(), pMultiRegion2->GetLocation() );
			if( fDistance > 400.0f ) continue;
			float fSignificance01 = (500.0f - fDistance) / 500.0f;

			POSITION pos2 = pMultiRegion2->m_AIRegions.GetHeadPosition();
			while( pos2 )
			{
				CEOSAIRegion2* pAIRegion = pMultiRegion2->m_AIRegions.GetNext( pos2 );
				pMultiRegion->m_OwnerControl01[ pAIRegion->m_iSingleOwner ] += fSignificance01*pAIRegion->m_fSingleOwnerControl;
				fTotalControl += fSignificance01;
			}
		}


		// Compile the adjacent multiregions
		/*
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
		//long iAreaType = 0;
		//long iMyAIRegions = 0;
		//long iPlayer0AIRegions = 0;
		//long iOtherPlayerAIRegions = 0;
		float fTotalControl = 0.0f;
		pos = MultiRegions.GetHeadPosition();
		while( pos )
		{
			CMultiRegion* pMultiRegion2 = MultiRegions.GetNext( pos );
			POSITION pos2 = pMultiRegion2->m_AIRegions.GetHeadPosition();
			while( pos2 )
			{
				CEOSAIRegion2* pAIRegion = pMultiRegion2->m_AIRegions.GetNext( pos2 );
				pMultiRegion->m_OwnerControl[ pAIRegion->m_iOwner ] += pAIRegion->m_fOwnerControl;
				fTotalControl += 1.0f;
			}
		}
		*/
		if( fTotalControl > 0.0f )
		{
			pMultiRegion->m_OwnerControl01[ 0 ] /= fTotalControl;
			float fPlayer0Control01 = pMultiRegion->m_OwnerControl01[ 0 ];
			ASSERT( fPlayer0Control01 >= 0.0f && fPlayer0Control01 <= 1.0f );

			for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS+1; iPlayer++ )
			{
				float fNewValue = pMultiRegion->m_OwnerControl01[ iPlayer ];
				fNewValue /= fTotalControl;
				fNewValue -= 0.5f*fPlayer0Control01;
				if( fNewValue < 0.0f ){ fNewValue = 0.0f; }

				pMultiRegion->m_OwnerControl01[ iPlayer ] = fNewValue;
			}
		}
		/*
		if( iMyAIRegions > 0 )
		{
			iAreaType = 3;
		}
		*/
	}
}

/*
void CCommonData::CalculateUnitCombatCapabilities()
{
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CWorldBuildDesc*  pWorldBuildDesc = pWorldDescServer->GetWorldBuildDesc();

	// Compile a list of all UnitTemplates in the game
	CEOSAIUnitTemplateSet  AllUnitsInTheGame;
	POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CUnit* pUnit = dynamic_cast< CUnit* >( pPoi );
		if( pUnit )
		{
			AllUnitsInTheGame.AddUnitTemplate( pUnit->GetUnitTemplate() );
		}
	}
	// Compile all the buildable units in the game
	long iNumberOfBuildableUnits = 0;
	long iNumberOfPlayers = pWorldBuildDesc->GetNumberOfPlayers();
	pos = GetCommonState()->GetActiveUnitset()->GetBuildOptionsList()->GetHeadPosition();
	while( pos )
	{
		CBuildOption* pBuildOption = GetCommonState()->GetActiveUnitset()->GetBuildOptionsList()->GetNext( pos );
		if( pBuildOption->IsAUnit() == false ) continue;
		//
		bool bCanBuild = false;
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			if( GetCommonState()->GetActiveUnitset()->CanBuild( iPlayer, pBuildOption, false ) )
			{
				bCanBuild = true;
				break;
			}
		}
		if( bCanBuild )
		{
			iNumberOfBuildableUnits++;
			AllUnitsInTheGame.AddUnitTemplate( pBuildOption->GetUnitTemplate() );
			//ASSERT( false );
		}
	}
	ASSERT( iNumberOfBuildableUnits > 0 );

	// Invoke all the CombatAttrition values between MyUnits and PotentialEnemy units
	ASSERT( AllUnitsInTheGame.m_List.IsEmpty() == FALSE );
	pos = AllUnitsInTheGame.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );
		pMyUnit->GetUnitCombatCapability()->CalculateValue( &AllUnitsInTheGame );
		int g=0;
	}
	pos = AllUnitsInTheGame.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );
		//pMyUnit->GetUnitCombatCapability()->CalculateRelativeValue( &AllUnitsInTheGame, &AllUnitsInTheGame );
		int g=0;
	}

	// DEBUG - What's the relative value?
	#ifdef _DEBUG
	pos = AllUnitsInTheGame.m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );

		long iArraySize = pMyUnit->GetUnitCombatCapability()->GetArray()->m_iSize;
		for( long i=0; i<iArraySize; i++ )
		{
			CEOSAIUnit2VsUnitValue* pVal = pMyUnit->GetUnitCombatCapability()->GetArray()->Value(i);
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
	#endif _DEBUG
}
*/



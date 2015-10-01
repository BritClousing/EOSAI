
#include "stdafx.h"
#include "PublicAI.h"
#include "EOSAIInterface2.h"
#include "BCStopwatch2.h"
// EOSAI header files
//#ifdef USE_EOSAI_DLL
#ifdef EOSAI
#include "EOSAIIncludes.h"
//#include "Include\EOSAIInterface.h"
/*
#include <Include\EOSAIInterface.h>
#include "Include\EOSAIGamePlayer.h"
#include "Include\EOSAIUnitTemplate2.h"
#include "Include\EOSAIUnitTemplate2Set.h"
#include "Include\EOSAICity2.h"
#include "Include\EOSAIAirfield2.h"
#include "Include\EOSAIResource2.h"
#include "Include\EOSAIUnit2.h"
#include "Include\EOSAINationalSummary3.h"
#include "Include\EOSAIRoad2.h"
#include "Include\EOSAIUnitPathwayResultStep.h"
*/
#endif

//
//#include "..\..\Source\CommonState.h"
//#include "..\..\Source\WorldDescServer.h"
#include "CommonState.h"
#include "WorldDescServer.h"
#include "WorldDescPlayer.h"
#include "Mountain.h"
//class CAIRegionSeed;
class CEOSAICity2;

class CAIRegionSeed
{
	public:
		long m_iRegionId;
		long m_iTerrainType9;
		long m_iPosX;
		long m_iPosY;
};
/*
class CEOSAIAdjacentAIRoad2Distance
{
	public:
		CEOSAIAdjacentAIRoad2Distance()
		{
			m_pIntermediateRegion = NULL;
			m_pAIRoad = NULL;
			m_fPixelDistance = 0.0f;
		}

		CEOSAIGenericRegion*  m_pIntermediateRegion;
		CEOSAIRoad2*  m_pAIRoad;
		float         m_fPixelDistance;
};
*/

//CAIInterface* g_pAIInterface = NULL;
void   CreateRegionSeed( long iRegionId, long x, long y, CList< CAIRegionSeed* >* pRegionSeedList );
void   CreateRegion2( long iRegionId, long x, long y, CEOSAICity2* pAICity );
void   GrowRegionSeeds( CList< CAIRegionSeed* >& RegionSeedList );

// Conversion Routines
#include "EOSAI\Include\EOSAILocation.h"
CEOSAILocation ConvertToEOSAILocation( CLocation Location ){ return CEOSAILocation( CEOSAILocation::Pixel, Location.GetRealX(),Location.GetRealY() ); }

void ConvertToEOSAIUnitTemplate( CUnitTemplate* pUnitTemplate, CEOSAIUnitTemplate2* pEOSAIUnitTemplate )
{
	pEOSAIUnitTemplate->SetInternalName( pUnitTemplate->GetInternalName() );
}

void ConvertToEOSAICity2( CCity* pCity, CEOSAICity2* pAICity )
{
	//pAICity->GetInitialState()->SetLocationAndRegions( SetLocation( ConvertToEOSAILocation( pCity->GetLocation() ) );
	pAICity->GetInitialState()->SetLocationAndRegions( ConvertToEOSAILocation( pCity->GetLocation() ) );
}

void ConvertToEOSAIResource2( CResourceSource* pResource, CEOSAIResource2* pAIResource )
{
	pAIResource->GetInitialState()->SetLocationAndRegions( ConvertToEOSAILocation( pResource->GetLocation() ) );
}

void ConvertToEOSAIAirfield2( CAirfield* pAirfield, CEOSAIAirfield2* pAIAirfield )
{
	pAIAirfield->GetInitialState()->SetLocationAndRegions( ConvertToEOSAILocation( pAirfield->GetLocation() ) );
}

void ConvertToEOSAIUnit2( CUnit* pUnit, CEOSAIUnit2* pAIUnit )
{
	pAIUnit->GetInitialState()->SetLocationAndRegions( ConvertToEOSAILocation( pUnit->GetLocation() ) );
}


/*
void  CAIUnit::SetServerPoiObject( CPoiObject* pPoiObject )
{
	ASSERT( m_pServerUnit == NULL && m_bHypotheticalPoiObject == false );
	ASSERT( pPoiObject->IsDead() == false );

	ASSERT( dynamic_cast< CUnit* >( pPoiObject ) );
	m_pServerUnit = dynamic_cast< CUnit* >( pPoiObject );
	CAIPoiMobile::SetServerPoiObject( pPoiObject );

	//m_pUnit = pUnit;
	m_bHypotheticalPoiObject = false;

	CUnitTemplate* pUnitTemplate = m_pServerUnit->GetUnitTemplate();
	ASSERT( pUnitTemplate );
	SetUnitTemplate( pUnitTemplate );
	m_InitialState.SetHP( m_pServerUnit->GetCurrentHP() );
	m_InitialState.SetGroundUnit( pUnitTemplate->IsGroundUnit() );

	// HP01 = 1.0 -> m_fDesireForRepair01 = 0.0
	// HP01 = 0.8 -> m_fDesireForRepair01 = 0.25
	// HP01 = 0.6 -> m_fDesireForRepair01 = 0.5
	// HP01 = 0.4 -> m_fDesireForRepair01 = 0.75
	// HP01 = 0.2 -> m_fDesireForRepair01 = 1.0
	//m_fDesireForRepair01 = 1.25f * ( 1.0f - m_pServerUnit->GetCurrentHP01() );
	//if( m_fDesireForRepair01 > 1.0f ){ m_fDesireForRepair01 = 1.0f; }
	//CalculateDesires();

	//SetPoiMobile( m_pUnit );
	m_Real_TransportSpaceAllocations.SetAIUnit( this );
	m_pAIUnitPathway = CAIUnitPathwayFinder::CreatePathwayFinder( this );
}

void CAIPoiObject::SetServerPoiObject( CPoiObject* pPoiObject )
{
	m_pServerPoiObject = pPoiObject;

	pPoiObject->SetAIPoiObject( this );

	m_bIPlanOnCapturingThis = false;

	CLocation InitialLocation = pPoiObject->GetLocation();
	if( pPoiObject->GetContainer() == NULL )
	{
		if( pPoiObject->IsGroundUnit() )
		{
			CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
			pWorldDescServer->GetValidLocation( InitialLocation, 1, EnumMovementType3_Land, &InitialLocation );
		}
		if( pPoiObject->IsSeaUnit() )
		{
			CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
			pWorldDescServer->GetValidLocation( InitialLocation, 1, EnumMovementType3_Water, &InitialLocation );
		}
	}

	m_InitialState.SetGroundUnit( pPoiObject->IsGroundUnit() );
	m_InitialState.SetLocationAndRegions( InitialLocation );//pPoiObject->GetLocation() );
	m_InitialState.SetOwner( pPoiObject->GetOwner() );
	m_InitialState.SetTime( 0.0f );
	m_InitialState.SetContainer( NULL );
}
*/

void CEOSAIInterface2::CreateAIObjects()
{
	// This is just some debugging information so that users can track how long it takes to construct all the data
/*
	#ifdef _DEBUG
	CBCStopwatch2  WatchTotal;
	CBCStopwatch2  WatchGenerateAIRegions;
	CBCStopwatch2  WatchGenerateMultiRegions;
	CBCStopwatch2  WatchCalculateUnitCombatCapabilities;
	CBCStopwatch2  WatchCalculateNationalSummaries;
	CBCStopwatch2  WatchAddObjectIdsToAIRegionsAndMultiRegions;
	CBCStopwatch2  WatchCalculateNationwidePathways;
	CBCStopwatch2  WatchCalculateAIRegionOwnership;
	#endif _DEBUG
*/

	m_bNeedToRebuildData = false;

	//if( g_pAICommonData->GetNumberOfActiveAIPlayers() == 0 ) return;
	//#ifdef _DEBUG
	//if( Public::ActiveAIPlayers() == false ) return;
	//#endif
	//WatchTotal.Start();

	// This is the main hub for AI data
//	g_pAICommonData = new CAICommonData();

	// Players
//	g_pAICommonData->AddGamePlayer( 1, EnumGamePlayerType_Human, true );
//	g_pAICommonData->AddGamePlayer( 2, EnumGamePlayerType_AI, true );
//	g_pAICommonData->AddGamePlayer( 3, EnumGamePlayerType_AI, true );

	// AIPlayer details (intelligence, personality, etc)
//	g_pAICommonData->CreateAIPlayer( 2,  50 );
//	g_pAICommonData->CreateAIPlayer( 3, 100 );

	// Create the AIGameRules
	// This contains:
	//   AIUnitTemplates (which describes the unit types)
	//   AIBuildingDescs (which describes the city improvements)
	//   AICombatUnitTypes (which describes the combat-unit categories)
	//   AIMovementUnitTypes (which describes the movement-unit categories)
	CreateAICombatUnitTypes();
	CreateAIMovementUnitTypes();
	CreateAIUnitTemplates();
	CreateAIBuildingDescs();

	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
	//if( m_AIRegionManager.GetNumberOfGenericRegions() == 0 )
	{
		//pWorldDescServer->BuildGeoAndNodeMaps();

		//WatchGenerateAIRegions.Start();
		GenerateAIRegions( pWorldDescServer );
		//m_AIRegionManager.ServerFunction_GenerateAIRegions( pWorldDescServer );
		//WatchGenerateAIRegions.Stop();
		//
		//m_MultiRegionManager.SetAIRegionManager( &m_AIRegionManager );
		//m_MultiRegionManager.SetWorldDescServer( pWorldDescServer );

		//WatchGenerateMultiRegions.Start();
		//m_MultiRegionManager.GenerateMultiRegions( &m_AIRegionManager );
		//WatchGenerateMultiRegions.Stop();
	}

	// Create AIPoiObjects: these are the Units, Cities, Airfields, etc in the current game-world
	TurnPoiObjectsIntoAIPoiObjects();

	//WatchCalculateUnitCombatCapabilities.Start();
	//GetCommonState()->CalculateUnitCombatCapabilities( pWorldDescServer->GetCurrentTurn() );
	CalculateAIUnitCombatCapabilities(); // pWorldDescServer->GetCurrentTurn() );
	//WatchCalculateUnitCombatCapabilities.Stop();

	//WatchCalculateNationalSummaries.Start();
	CalculateNationalSummaries();
	//WatchCalculateNationalSummaries.Stop();

	CalculateThoughtDatabases();

	//WatchAddObjectIdsToAIRegionsAndMultiRegions.Start();
	AddObjectIdsToAIRegionsAndMultiRegions();
	//WatchAddObjectIdsToAIRegionsAndMultiRegions.Stop();

	//WatchCalculateNationwidePathways.Start();
	CalculateNationwidePathways();
	//WatchCalculateNationwidePathways.Stop();

	//WatchCalculateAIRegionOwnership.Start();
	CalculateAIRegionOwnership();
	CalculateMultiRegionOwnership();
	//WatchCalculateAIRegionOwnership.Stop();
	//WatchTotal.Stop();
/*
	#ifdef _DEBUG
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
	#endif _DEBUG
*/
	//m_bDataIsValid = true;
	int g=0;
}

void CEOSAIInterface2::CreateAIUnitTemplates()
{
	//CAIGameRules* pAIGameRules = g_pAICommonData->GetAIGameRules();
	//CAIGameRules* pAIGameRules = &m_AIGameRules;

	CUnitset* pUnitset = GetCommonState()->GetActiveUnitset();
	POSITION pos = pUnitset->GetUnitTemplateList()->GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = pUnitset->GetUnitTemplateList()->GetNext( pos );

		CEOSAIUnitTemplate2* pAIUnitTemplate = new CEOSAIUnitTemplate2( &m_AIGameRules );
		//pAIGameRules->AddAIUnitTemplate( pAIUnitTemplate );
		m_AIGameRules.AddAIUnitTemplate( pAIUnitTemplate );
	}
}

void CEOSAIInterface2::TurnPoiObjectsIntoAIPoiObjects()
{
	//ASSERT( m_pWorldDescPlayer );
	//ASSERT( m_AIPoiObjects.IsEmpty() );
	//ASSERT( m_AIObjectList.IsEmpty() );
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	float fCurrentTime = pWorldDescServer->GetGameTime().GetTime();

	POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pServerPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		if( pServerPoi->IsDead() ) continue; // Don't turn dead poi into AIObjects

		CPoiObject* pServerPoiObject = dynamic_cast< CPoiObject* >( pServerPoi );
		if( pServerPoiObject == NULL ) continue;
		pServerPoiObject->SetAIPoiObject( NULL );

		//CPoiObject* pPlayerPoiObject = GetWorldDescServer()->GetPoiObject( pServerPoiObject->GetObjectId() );
		//if( pPlayerPoiObject ){ pPlayerPoiObject->SetAIPoiObject( NULL ); }

		CCity* pCity = dynamic_cast< CCity* >( pServerPoiObject );
		if( pCity )
		{
			CEOSAICity2* pAICity = new CEOSAICity2();// this );
			ConvertToEOSAICity2( pCity, pAICity );
			//pAICity->SetServerPoiObject( pServerPoiObject );
			//pAICity->SetServerPoiObjectId( pServerPoiObject );
			//this->AddAIObject( pAICity );
			m_AICommonData.AddAIPoiObject( pAICity );

			// INCOMPLETE: Create UnitSymbols for the items in production
			/*
			if( pAICity->IsMine() )
			{
				int p=0;
				// NOTE: neither GetBuildCompletionList() nor GetBuildOrdersList()
				//   will give me quite what I want (unless I combine them and do
				//   calculations myself, like the CCityDialog does).
				//  I might want to create a CCity method that generates the information
				//   and passes back a list (probably of CBuildCompletion objects).
				/*
				POSITION pos = pCity->GetBuildCompletionList()->GetHeadPosition();
				while( pos )
				{
					CBuildCompletion* pBuildCompletion = pCity->GetBuildCompletionList()->GetNext( pos );
					if( pBuildCompletion->
				}
				*-/
			}
			*/
		}
		CResourceSource* pResourceSource = dynamic_cast< CResourceSource* >( pServerPoiObject );
		if( pResourceSource )
		{
			CEOSAIResource2* pAIResource = new CEOSAIResource2();// this );
			ConvertToEOSAIResource2( pResourceSource, pAIResource );
			//pAIResource->SetServerPoiObject( pServerPoiObject );
			m_AICommonData.AddAIPoiObject( pAIResource );
		}
		CAirfield* pAirfield = dynamic_cast< CAirfield* >( pServerPoiObject );
		if( pAirfield )
		{
			CEOSAIAirfield2* pAIAirfield = new CEOSAIAirfield2();// this );
			ConvertToEOSAIAirfield2( pAirfield, pAIAirfield );
			//pAIAirfield->SetServerPoiObject( pServerPoiObject );
			m_AICommonData.AddAIPoiObject( pAIAirfield );
		}
		CUnit* pUnit = dynamic_cast< CUnit* >( pServerPoiObject );
		if( pUnit )
		{
			CEOSAIUnit2* pAIUnit = new CEOSAIUnit2();// this );
			ConvertToEOSAIUnit2( pUnit, pAIUnit );
			//pAIUnit->SetServerPoiObject( pServerPoiObject );
			m_AICommonData.AddAIPoiObject( pAIUnit );
		}
	}

	// Setup desires
	pos = GetAIPoiObjectList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject2* pAIObject = GetAIPoiObjectList()->GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIObject );
		if( pAIUnit )
		{
			pAIUnit->CreatePersonalDesires();
		}
	}

	// Now setup the container information
	ASSERT( false );
	/*
	pos = GetAIPoiObjectList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject2* pAIObject = GetAIPoiObjectList()->GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIObject );
		if( pAIUnit )
		{
			// void SetContainer( CAIBrain* pAIPlan, long iContainer );

			//if( pAIUnit->GetServerUnit() &&
			//	pAIUnit->GetServerUnit()->GetAirBase() )
			if( pAIUnit &&
				pAIUnit->GetAirBase() )
			{
				//CEOSAIPoiObject2* pAIPoiObject = m_AICommonData.GetAIPoiObject( pAIUnit->GetServerUnit()->GetAirBase()->ObjectId() );
				CEOSAIPoiObject2* pAIPoiObject = m_AICommonData.GetAIPoiObject( pAIUnit->GetInitialState()->GetAirbase()->GetObjectId() );
				if( pAIPoiObject )
				{
					CEOSAIPoiObject2* pAIAirbase = pAIPoiObject;
					//CAIPoiObject2* pAIAirbase = dynamic_cast< CAIPoiObject2* >( pAIPoiObject );
					//ASSERT( pAIAirbase );
					//pUnitSymbol->SetInitialAirbase( this, pUnitSymbol->GetUnit()->GetAirBase()->ObjectId() );
					pAIUnit->SetInitialAirbase( pAIAirbase );
				}
			}
			if( pAIUnit &&
				pAIUnit->GetContainer() )
			{
				//CEOSAIPoiObject* pAIPoiObject = this->GetAIPoiObject( pAIUnit->GetServerUnit()->GetContainer()->ObjectId() );
				//CEOSAIPoiObject* pAIPoiObject = m_AICommonData.GetAIPoiObject( pAIUnit->GetServerUnit()->GetContainer()->ObjectId() );
				CEOSAIPoiObject2* pAIPoiObject = m_AICommonData.GetAIPoiObject( pAIUnit->GetInitialState()->GetContainer()->GetObjectId() );
				if( pAIPoiObject )
				{
					CEOSAIPoiObject2* pAIContainer = pAIObject;
					//CAIPoiObject2* pAIContainer = dynamic_cast< CAIPoiObject2* >( pAIObject );
					//pUnitSymbol->SetInitialContainer( this, pUnitSymbol->GetUnit()->GetContainer()->ObjectId() );
					pAIUnit->SetInitialContainer( pAIContainer );
				}
			}

			//CAIUnit2* pUnitSymbol = new CAIUnit2( m_pCurrentPlan, pUnit->GetOwner(), pUnit );
			//m_pCurrentPlan->AddPoiObjectSymbol( pUnitSymbol );
		}
	}
	*/
}

void CEOSAIInterface2::CalculateAIUnitCombatCapabilities()
{
	//ASSERT( m_iUnitCombatCapabilities_LastTurnCalculated != iCurrentTurn );
	//if( m_iUnitCombatCapabilities_LastTurnCalculated != iCurrentTurn )
	{
		//m_iUnitCombatCapabilities_LastTurnCalculated = iCurrentTurn;

		CWorldDesc* pWorldDesc = g_pCommonState->GetWorldDescServer();//m_pWorldDescServer;
		/*
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
		*/
		ASSERT( pWorldDesc );
		if( pWorldDesc == NULL ) return;

		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		//CWorldBuildDesc*  pWorldBuildDesc = pWorldDescServer->GetWorldBuildDesc();
		CWorldBuildDesc* pWorldBuildDesc = pWorldDesc->GetWorldBuildDesc();

		// Compile a list of all UnitTemplates in the game
		/*
		CEOSAIUnitTemplate2Set  AllUnitsInTheGame;
		POSITION pos = pWorldDesc->GetPoiList()->GetHeadPosition();
		while( pos )
		{
			CPoi* pPoi = pWorldDesc->GetPoiList()->GetNext( pos )->GetPtr();
			CUnit* pUnit = dynamic_cast< CUnit* >( pPoi );
			if( pUnit )
			{
				//AllUnitsInTheGame.AddUnitTemplate( pUnit->GetUnitTemplate() );
				AllUnitsInTheGame.AddUnitTemplate( pUnit->GetUnitTemplate() );
			}
		}
		*/
		CEOSAIUnitTemplate2Set  AllUnitsInTheGame;
		POSITION pos = GetCommonState()->GetActiveUnitset()->GetUnitTemplateList()->GetHeadPosition();
		while( pos )
		{
			CUnitTemplate* pUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplateList()->GetNext( pos );
			CEOSAIUnitTemplate2* pEOSAIUnitTemplate = new CEOSAIUnitTemplate2( &m_AIGameRules );
			ConvertToEOSAIUnitTemplate( pUnitTemplate, pEOSAIUnitTemplate );
			AllUnitsInTheGame.AddUnitTemplate( pEOSAIUnitTemplate );
		}

		// Compile all the buildable units in the game
		/*
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
		*/

		// Invoke all the CombatAttrition values between MyUnits and PotentialEnemy units
		ASSERT( AllUnitsInTheGame.m_List.IsEmpty() == FALSE );
		pos = AllUnitsInTheGame.m_List.GetHeadPosition();
		while( pos )
		{
			//CUnitTemplate* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );
			CEOSAIUnitTemplate2* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );
			pMyUnit->GetAIUnitCombatCapability()->CalculateAverageVisibleRange( &AllUnitsInTheGame );
			int g=0;
		}
		pos = AllUnitsInTheGame.m_List.GetHeadPosition();
		while( pos )
		{
			//CUnitTemplate* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );
			CEOSAIUnitTemplate2* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );
			pMyUnit->GetAIUnitCombatCapability()->CalculateRelativeValue( &AllUnitsInTheGame, &AllUnitsInTheGame );
			int g=0;
		}
	}
}

void CEOSAIInterface2::CalculateNationalSummaries()
{
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CWorldBuildDesc*  pWorldBuildDesc = pWorldDescServer->GetWorldBuildDesc();

	// Totals
	m_AICommonData.m_fTotalCityProductionOnMap = 0.0f;
	m_AICommonData.m_fTotalCitResProductionOnMap = 0.0f;
	m_AICommonData.m_fTotalUnownedCitResProductionOnMap = 0.0f;
	m_AICommonData.m_fTotalResourcesOnMap = 0.0f;
	//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	POSITION pos = pWorldDescServer->GetActorsList()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CPoiObject* pPoiObject = pWorldDescServer->GetActorsList()->GetNext( pos );
		if( dynamic_cast< CCity* >( pPoiObject ) )
		{
			CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
			if( pCity->IsAlive() )
			{
				m_AICommonData.m_fTotalCityProductionOnMap += pCity->GetTotalProduction( false,false );
				m_AICommonData.m_fTotalCitResProductionOnMap += pCity->GetTotalProduction( false,false );
				if( pCity->GetOwner() == 0 )
				{
					m_AICommonData.m_fTotalUnownedCitResProductionOnMap += pCity->GetTotalProduction( false,false );
				}
			}
			else
			{
				ASSERT( false );
			}
		}
		if( dynamic_cast< CResourceSource* >( pPoiObject ) )
		{
			CResourceSource* pRes = dynamic_cast< CResourceSource* >( pPoiObject );
			if( pRes )
			{
				m_AICommonData.m_fTotalResourcesOnMap += (float) pRes->GetResourcePerTurn();
				m_AICommonData.m_fTotalCitResProductionOnMap += (float) pRes->GetResourcePerTurn();
				if( pRes->GetOwner() == 0 )
				{
					m_AICommonData.m_fTotalUnownedCitResProductionOnMap += (float) pRes->GetResourcePerTurn();
				}
			}
			else
			{
				ASSERT( false );
			}
		}
	}
	//

	long iNumberOfPlayers = pWorldBuildDesc->GetNumberOfPlayers();
	if( m_AICommonData.m_AINationalSummaries.m_iSize == 0 )
	{
		m_AICommonData.m_AINationalSummaries.SetSize( iNumberOfPlayers+1 );
		m_AICommonData.m_AINationalSummaries[0] = NULL;
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			m_AICommonData.m_AINationalSummaries[iPlayer] = new CAINationalSummary3();
			m_AICommonData.m_AINationalSummaries[iPlayer]->SetPlayer( iPlayer );
		}
	}
	ASSERT( iNumberOfPlayers+1 == m_AICommonData.m_AINationalSummaries.m_iSize );
	for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		if( m_AICommonData.m_AINationalSummaries[iPlayer] == NULL )
		{
			m_AICommonData.m_AINationalSummaries[iPlayer] = new CAINationalSummary3();
			m_AICommonData.m_AINationalSummaries[iPlayer]->SetPlayer( iPlayer );
		}
		m_AICommonData.m_AINationalSummaries[iPlayer]->CalculatePlayerPower();
		m_AICommonData.m_AINationalSummaries[iPlayer]->CalculatePlayerToPlayerAccessibility();
	}
}

void CEOSAIInterface2::GenerateAIRegions( CWorldDescServer* pWorldDescServer )
{
	ASSERT( pWorldDescServer );

	CBCStopwatch2 watch;
	watch.Start();

	m_pWorldDesc = pWorldDescServer;
	m_pWorldDescServer = pWorldDescServer;

	m_AICommonData.GetAIRegionManager()->m_RegionMap.Allocate( m_pWorldDescServer->GetGridWidth(), m_pWorldDescServer->GetGridHeight() );
	m_AICommonData.GetAIRegionManager()->m_RegionMap.SetWrap( m_pWorldDescServer->GetWrapRight(), m_pWorldDescServer->GetWrapBottom() );
	m_AICommonData.GetAIRegionManager()->m_RegionMap.Clear( 0 );
	//m_RegionMap.Allocate( m_pWorldDescServer->GetGridWidth(), m_pWorldDescServer->GetGridHeight() );
	//m_RegionMap.SetWrap( m_pWorldDescServer->GetWrapRight(), m_pWorldDescServer->GetWrapBottom() );
	//m_RegionMap.Clear( 0 );

	// Delete all the old regions
	for( long i=0; i<m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( i );
		m_AICommonData.GetAIRegionManager()->m_AIRegionArray.SetAt( i, NULL );
		if( pRegion )
		{
			delete pRegion;
		}
	}
	m_AICommonData.GetAIRegionManager()->m_GenericRegionList.RemoveAll();

	long iNextPathfinderPointId = 1;
	CList< CAIRegionSeed* > RegionSeedList;

	long iRegionSeedsCreated = 0;
	long iRegionSeedsDeleted = 0;
	long iRegionsCreated = 0;

	// Create a region seed for each city
	POSITION pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CCity* pCity = dynamic_cast< CCity* >( pPoi );
		if( pCity )
		{
			CEOSAIPoiObject2* pAIPoiObject2 = m_AICommonData.GetAIPoiObject( pCity->GetObjectId() );
			CEOSAICity2* pEOSAICity = dynamic_cast< CEOSAICity2* >( pAIPoiObject2 );
			//
			iRegionSeedsCreated++;
			CreateRegionSeed( iNextPathfinderPointId, pCity->GetLocation().GetGridX(), pCity->GetLocation().GetGridY(), &RegionSeedList );
			//
			iRegionsCreated++;
			//CreateRegion( iNextPathfinderPointId, pCity->GetLocation().GetGridX(), pCity->GetLocation().GetGridY(), pCity );
			CreateRegion2( iNextPathfinderPointId, pCity->GetLocation().GetGridX(), pCity->GetLocation().GetGridY(), pEOSAICity );
			//
			iNextPathfinderPointId++;
		}
	}
	// Create Regions for Mountains
	pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CMountain* pMountain = dynamic_cast< CMountain* >( pPoi );
		if( pMountain )
		{
			iRegionSeedsCreated++;
			CreateRegionSeed( iNextPathfinderPointId, pMountain->GetLocation().GetGridX(), pMountain->GetLocation().GetGridY(), &RegionSeedList );
			//
			iRegionsCreated++;
			//CreateRegion( iNextPathfinderPointId, pMountain->GetLocation().GetGridX(), pMountain->GetLocation().GetGridY() );
			CreateRegion2( iNextPathfinderPointId, pMountain->GetLocation().GetGridX(), pMountain->GetLocation().GetGridY(), NULL );
			//
			iNextPathfinderPointId++;
		}
	}

	// Grow the seeds
	GrowRegionSeeds( RegionSeedList );

	// Create region seeds
	for( long x=0; x<m_pWorldDescServer->GetGridWidth(); x+=24 )
	{
		for( long y=0; y<m_pWorldDescServer->GetGridHeight(); y+=14 )
		{
			// plant 2 seeds on each pass
			CAIRegionSeed* pSeed = NULL;
			CAIRegion* pRegion = NULL;

			long iNextX = x+6;
			long iNextY = y+4;
			if( iNextX >= 0 && iNextX < m_pWorldDescServer->GetGridWidth() &&
			    iNextY >= 0 && iNextY < m_pWorldDescServer->GetGridHeight() &&
				m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( iNextX,iNextY ) == 0 )
			{
				iRegionSeedsCreated++;
				CreateRegionSeed( iNextPathfinderPointId, iNextX, iNextY, &RegionSeedList );
				//
				iRegionsCreated++;
				CreateRegion2( iNextPathfinderPointId, iNextX, iNextY, NULL );
				//
				iNextPathfinderPointId++;
			}
			//
			iNextX = x+18;
			iNextY = y+14-4;//y+15-2;
			if( iNextX >= 0 && iNextX < m_pWorldDescServer->GetGridWidth() &&
			    iNextY >= 0 && iNextY < m_pWorldDescServer->GetGridHeight() &&
				m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( iNextX,iNextY ) == 0 )
			{
				iRegionSeedsCreated++;
				CreateRegionSeed( iNextPathfinderPointId, iNextX, iNextY, &RegionSeedList );
				//
				iRegionsCreated++;
				CreateRegion2( iNextPathfinderPointId, iNextX, iNextY, NULL );
				//
				iNextPathfinderPointId++;
			}
		}
	}
	// Grow the seeds
	GrowRegionSeeds( RegionSeedList );

	// We've gone through all the seeds, but there are probably some regions that
	//   have not been explored (because it's a small geo and none of the seeds landed
	//   on it.
	// Create region seeds
	for( long x=0; x<m_pWorldDescServer->GetGridWidth(); x++ )
	{
		for( long y=0; y<m_pWorldDescServer->GetGridHeight(); y++ )
		{
			if( m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( x, y ) == 0 )
			{
				// plant 2 seeds on each pass
				CAIRegionSeed* pSeed = NULL;

				CreateRegionSeed( iNextPathfinderPointId, x,y, &RegionSeedList );
				CreateRegion2( iNextPathfinderPointId, x,y, NULL );
				iNextPathfinderPointId++;

				// Grow the seeds
				GrowRegionSeeds( RegionSeedList );
			}
		}
	}

	// Validate (make sure all map locations are part of a region)
	for( long x=0; x<m_pWorldDescServer->GetGridWidth(); x++ )
	{
		for( long y=0; y<m_pWorldDescServer->GetGridHeight(); y++ )
		{
			ASSERT( m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( x, y ) != 0 );
		}
	}

	for( long i=1; i<m_AICommonData.m_AIRegionManager.m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( i );
		if( pRegion == NULL ) continue;

		ASSERT( abs( pRegion->m_iMinX - pRegion->m_iMaxX ) < 25 );
		ASSERT( abs( pRegion->m_iMinY - pRegion->m_iMaxY ) < 25 );
	}

	// Re-center all the regions
	for( long i=1; i<m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( i );
		if( pRegion == NULL ) continue;

		long iMidX = pRegion->m_iMinX + (pRegion->m_iMaxX - pRegion->m_iMinX)/2;
		long iMidY = pRegion->m_iMinY + (pRegion->m_iMaxY - pRegion->m_iMinY)/2;

		long iExistingGap = 
			abs( pRegion->GetLocation().GetGridX() - iMidX ) +
			abs( pRegion->GetLocation().GetGridY() - iMidY );
		if( iExistingGap > 2 )
		{
			for( long x=pRegion->m_iMinX; x<=pRegion->m_iMaxX; x++ )
			{
				for( long y=pRegion->m_iMinY; y<=pRegion->m_iMaxY; y++ )
				{
					long iThisGap = abs( x - iMidX ) + abs( y - iMidY );
					if( iThisGap < iExistingGap )
					{
						long iRegionAtLocation = m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( x,y );
						if( iRegionAtLocation == pRegion->m_iId )
						{
							int x2 = x;
							int y2 = y;
							pRegion->SetLocationAndGeo( CEOSAILocation( CEOSAILocation::Grid, x2,y2 ) );
							iExistingGap = iThisGap;
						}
					}
				}
			}
		}
	}

	// Create AIRegion Adjacent information
	/*
	long iMaxX = m_pWorldDescServer->GetGridWidth();
	long iMaxY = m_pWorldDescServer->GetGridHeight();
	for( long x=0; x<iMaxX; x++ )
	{
		for( long y=0; y<iMaxY-1; y++ )
		{
			long iAIRegion1 = m_RegionMap.GridValue( x,y );
			long iAIRegion2 = m_RegionMap.GridValue( x,y+1 );

			//long iAIRegion2 = 0;
			//if( y+1 < iMaxY ){ iAIRegion2 = m_RegionMap.GridValue( x,y+1 ); }
			//else{              iAIRegion2 = m_RegionMap.GridValue( x,0   ); }
			if( iAIRegion1 == 191 || iAIRegion2 == 191 )
			{
				int g=0;
			}

			if( iAIRegion1 != iAIRegion2 )
			{
				CAIRegion* pAIRegion1 = m_AIRegionArray.GetAt( iAIRegion1 );
				CAIRegion* pAIRegion2 = m_AIRegionArray.GetAt( iAIRegion2 );

				ASSERT( pAIRegion1 );
				ASSERT( pAIRegion2 );
				ASSERT( pAIRegion1->IsAdjacentToRegion( pAIRegion2 ) );
				ASSERT( pAIRegion2->IsAdjacentToRegion( pAIRegion1 ) );

				pAIRegion1->AddAdjacentRegion( pAIRegion2 );
				pAIRegion2->AddAdjacentRegion( pAIRegion1 );
			}
		}
	}
	*/

	//
	// INCOMPLETE: What about road connections?
	//    I created a CAIRoad class to handle this
	//

	for( long i=1; i<m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( i );
		if( pRegion == NULL ) continue;

		ASSERT( abs( pRegion->m_iMinX - pRegion->m_iMaxX ) < 25 );
		ASSERT( abs( pRegion->m_iMinY - pRegion->m_iMaxY ) < 25 );
	}

	// Delete old CoastalLocations
	/*
	while( m_CoastLocations.IsEmpty() == FALSE ){ delete m_CoastLocations.RemoveTail(); }

	//
	// This should be deleted - I don't need it anymore
	// Create coastal locations procedurally
	for( long x1=0; x1<m_pWorldDescServer->GetGridWidth(); x1+=1 )
	{
		for( long y1=0; y1<m_pWorldDescServer->GetGridHeight(); y1+=2 )
		{
			long x2 = x1; 
			long y2 = y1; 
			if( x1%2 == 0 ){ x2 += 2; }
			else{            y2 += 2; }				
			if( x2 < m_pWorldDescServer->GetGridWidth() &&
				y2 < m_pWorldDescServer->GetGridHeight() )
			{
				CGeo* pGeo1 = m_pWorldDescServer->GetGeo( CLocation( Grid,x1,y1 ) );
				CGeo* pGeo2 = m_pWorldDescServer->GetGeo( CLocation( Grid,x2,y2 ) );
				if( pGeo1->GetGeoType() == CGeo::Water &&
					pGeo2->GetGeoType() == CGeo::Land )
				{
					long x3 = x1; x1 = x2; x2 = x3;
					long y3 = y1; y1 = y2; y2 = y3;
					CGeo* pGeo3 = pGeo1; pGeo1 = pGeo2; pGeo2 = pGeo3;
				}
				if( pGeo1->GetGeoType() == CGeo::Land &&
				  ( pGeo2->GetGeoType() == CGeo::Water || pGeo2->GetGeoType() == CGeo::Unknown ) )
				{
					CLocation CoastLocation = CLocation( Grid,x2,y2 );
					long       iLandRegion  = m_RegionMap.GridValue( x1,y1 );
					CAIRegion* pLandRegion  = m_AIRegionArray.GetAt( iLandRegion );
					long       iLandGeo     = pLandRegion->m_iGeo;
					long       iWaterRegion = m_RegionMap.GridValue( x2,y2 );
					CAIRegion* pWaterRegion = m_AIRegionArray.GetAt( iWaterRegion );
					long       iWaterGeo    = pWaterRegion->m_iGeo;

					if( this->HasNearbyCoastLocation( m_pWorldDesc, CoastLocation, iLandGeo, iWaterGeo ) == false )
					{
						CAIRegionCoastLocation* pNewLocation = new CAIRegionCoastLocation();
						pNewLocation->m_CoastLocationLand  = CLocation( Grid,x1,y1 );
						pNewLocation->m_CoastLocationWater = CLocation( Grid,x2,y2 );
						pNewLocation->m_pLandRegion = pLandRegion;
						pNewLocation->m_pWaterRegion = pWaterRegion;
						//
						pLandRegion->m_CoastLocations.AddTail( pNewLocation );
						this->m_CoastLocations.AddTail( pNewLocation );
					}
				}
			}
		}
	}
	*/

	// Debug: All "TERRAIN_CITY" AIRegions should have a CCity
	for( long i=1; i<m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetSize(); i++ )
	{
		CEOSAIRegion2* pRegion = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( i );
		if( pRegion == NULL ) continue;

		if( pRegion->m_iTerrainType9 == TERRAIN_CITY )
		{
			ASSERT( pRegion->GetCity() );
		}
	}

	//
	// This should be deleted - I don't need it anymore
	// All my coastal cities should be considered coastal-pickup/dropoff locations
	/*
	pos = m_pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		CCity* pCity = dynamic_cast< CCity* >( pPoi );
		//CAIObject* pAIObject = m_pAIBrain->GetAIPoiObjectList()->GetNext( pos );
		//CAICity2* pCitySymbol = dynamic_cast< CAICity2* >( pAIObject );
		//if( pCitySymbol && pCitySymbol->IsMine() )
		if( pCity )//&& pCity->GetOwner() == m_pAIBrain->GetAIPlayerNumber() ) //pCity->IsMine() )
		{
			// Only coastal cities serve as coastal locations
			if( pCity->GetWaterGeoNumber7c() != 0 )
			{
				CLocation LandLocation  = pCity->GetLocation();
				CLocation WaterLocation = pCity->GetLocation();

				long x1 = LandLocation.GetGridX();
				long y1 = LandLocation.GetGridY();
				long x2 = WaterLocation.GetGridX();
				long y2 = WaterLocation.GetGridY();

				CGeo* pGeo1 = m_pWorldDescServer->GetGeo( CLocation( Grid,x1,y1 ) );
				CGeo* pGeo2 = m_pWorldDescServer->GetGeo( CLocation( Grid,x2,y2 ) );
				if( pGeo1->GetGeoType() == CGeo::Water &&
					pGeo2->GetGeoType() == CGeo::Land )
				{
					long x3 = x1; x1 = x2; x2 = x3;
					long y3 = y1; y1 = y2; y2 = y3;
					CGeo* pGeo3 = pGeo1; pGeo1 = pGeo2; pGeo2 = pGeo3;
				}
				if( pGeo1->GetGeoType() == CGeo::Land &&
					pGeo2->GetGeoType() == CGeo::Water )
				{
					CLocation CoastLocation = CLocation( Grid,x2,y2 );
					long       iLandRegion  = m_RegionMap.GridValue( x1,y1 );
					CAIRegion* pLandRegion  = m_AIRegionArray.GetAt( iLandRegion );
					long       iLandGeo     = pLandRegion->m_iGeo;
					long       iWaterRegion = m_RegionMap.GridValue( x2,y2 );
					CAIRegion* pWaterRegion = m_AIRegionArray.GetAt( iWaterRegion );
					long       iWaterGeo    = pWaterRegion->m_iGeo;

					//if( this->HasNearbyCoastLocation( m_pWorldDesc, CoastLocation, iLandGeo, iWaterGeo ) == false )
					//{
						CAIRegionCoastLocation* pNewLocation = new CAIRegionCoastLocation();
						pNewLocation->m_CoastLocationLand  = CLocation( Grid,x1,y1 );
						pNewLocation->m_CoastLocationWater = CLocation( Grid,x2,y2 );
						pNewLocation->m_pLandRegion = pLandRegion;
						pNewLocation->m_pWaterRegion = pWaterRegion;
						//
						pLandRegion->m_CoastLocations.AddTail( pNewLocation );
						this->m_CoastLocations.AddTail( pNewLocation );
					//}
				}
			}
		}
	}
	*/

	// Calculate distances between adjacent regions
	/*
	for( long i=1; i<m_AIRegionArray.GetSize(); i++ )
	{
		CAIRegion* pRegion = m_AIRegionArray.GetAt( i );
		ASSERT( pRegion );
		if( pRegion )
		{
			// Add regions to the AIObjectList
			/-*
			if( m_pAIBrain )
			{
				//m_pAIBrain->AddAIObject( pRegion );
				m_pAIBrain->AddAIRegion( pRegion );
			}
			*-/

			// Calculate distances between regions
			/-*
			POSITION pos = pRegion->m_AdjacentRegions.GetHeadPosition();
			while( pos )
			{
				CAIAdjacentRegionDistance* pAdjRegionDist = pRegion->m_AdjacentRegionList.GetNext( pos );
				//CAIRegion* pAdjRegion = m_AIRegionArray.GetAt( pAdjRegionDist->m_iOtherRegion );
				CAIRegion* pAdjRegion = pAdjRegionDist->m_pRegion;
				ASSERT( pAdjRegion );
				if( pAdjRegion )
				{
					float fGridDistance = m_pWorldDescServer->GetGridDistance( pRegion->GetLocation().GetGridX(), pRegion->GetLocation().GetGridY(), pAdjRegion->GetLocation().GetGridX(), pAdjRegion->GetLocation().GetGridY() );
					pAdjRegionDist->m_fPixelDistance = fGridDistance*4.0f;
				}
			}
			*-/
		}
	}
	*/

	watch.Stop();
	float fBuildTime1 = watch.GetDeltaSeconds();

	//CBCStopwatch2 watch;
	/*
	CAISpatialSearchResult SpatialSearchResult;
	watch.Start();
	float fPixelDistance = GetPixelDistance( CLocation(Pixel,50,50), CLocation(Pixel,850,850), EnumMovementType3_Water, 0, &SpatialSearchResult );
	//float fPixelDistance = GetPixelDistance( CLocation(Pixel,200,50), CLocation(Pixel,900,900), EnumMovementType3_Land, 2 );
	//fPixelDistance = GetDistance( CLocation(Pixel,50,50), CLocation(Pixel,50,500) );
	watch.Stop();
	float fTime1 = watch.GetDeltaSeconds();
	*/

	/*
	CMovementDescription MovementDesc;
	MovementDesc.m_fTerrainMovementRateMultiplier[ TERRAIN_WATER ] = 10.0f;
	CList< CLocation*, CLocation* >  ResultPath;
	watch.Start();
	m_pWorldDesc->FindPath3( &MovementDesc, CLocation(Pixel,50,50), CLocation(Pixel,850,850), &ResultPath );
	watch.Stop();
	float fTime2 = watch.GetDeltaSeconds();
	*/

	// Movement from 50,50 -> 850,850 (note: 50,50 and 850,850 are on water)
	// Movement over unknown map, water movement:
	//   Release: AISpatialMap: 0.001548, FindPath3: 0.672696
	// Movement over known map, water movement:
	//   Debug:   AISpatialMap: 0.006878, FindPath3: 8.832127
	//   Release: AISpatialMap: 0.001036, FindPath3: 0.631510
	//CString strText;
	//strText.Format( "AISpatialMap: %f, FindPath3: %f", fTime1, fTime2 );
	//AfxMessageBox( strText );

	// Create CAIRoad objects
	long iRoadArraySize = pWorldDescServer->GetRoadChunksArray()->GetSize();
	for( long iRoadChunk=0; iRoadChunk<iRoadArraySize; iRoadChunk++ )
	{
		CRoadChunk* pRoadChunk = pWorldDescServer->GetRoadChunksArray()->GetAt( iRoadChunk );
		if( pRoadChunk == NULL ) continue;
		//CEOSAIRegion2* pRoadChunkAIRegion = m_AICommonData.GetAIRegionManager()->GetAIRegion( pRoadChunk->m_Location.GetGridX() .GetEOSAILocation() );
		//CEOSAIRegion2* pRoadChunkAIRegion = m_AICommonData.GetAIRegionManager()->GetAIRegion( pRoadChunk->m_Location.GetGridX(), pRoadChunk->m_Location.GetGridY() );
		CEOSAILocation EOSAILocation = ConvertToEOSAILocation( pRoadChunk->m_Location );
		CEOSAIRegion2* pRoadChunkAIRegion = m_AICommonData.GetAIRegionManager()->GetAIRegion( EOSAILocation );

		// Clear the road-chunk data
		pRoadChunk->m_iTempVisited = 0;
		pRoadChunk->m_fTempDistance = 0.0f;

		// Is the adjacent road on another AIRegion?
		long iSize = pRoadChunk->GetAdjacentRoadChunkList()->GetSize();
		POSITION pos = pRoadChunk->GetAdjacentRoadChunkList()->GetHeadPosition();
		while( pos )
		{
			long iAdjacentRoadChunk = pRoadChunk->GetAdjacentRoadChunkList()->GetNext( pos );
			if( iAdjacentRoadChunk >= iSize ) continue;

			// we only want to create one AIRoad
			if( iRoadChunk > iAdjacentRoadChunk ) continue;

			CRoadChunk* pAdjRoadChunk = pWorldDescServer->GetRoadChunksArray()->GetAt( iAdjacentRoadChunk );
			if( pAdjRoadChunk == NULL )
			{
				ASSERT( false ); // this means there is a problem somewhere else
				continue;
			}
			//CAIRegion2* pAdjRoadChunkAIRegion = m_AICommonData.GetAIRegionManager()->GetAIRegion( pAdjRoadChunk->m_Location );
			CEOSAIRegion2* pAdjRoadChunkAIRegion = m_AICommonData.GetAIRegionManager()->GetAIRegion( ConvertToEOSAILocation(pAdjRoadChunk->m_Location) );
			if( pRoadChunkAIRegion != pAdjRoadChunkAIRegion ) // They are on different regions
			{
				// Look for this road on the region edge already
				bool bFound = false;
				POSITION pos2 = pRoadChunkAIRegion->m_RoadsOnTheRegionEdge.GetHeadPosition();
				while( pos2 )
				{
					CEOSAIRoad2* pExistingAIRoad = pRoadChunkAIRegion->m_RoadsOnTheRegionEdge.GetNext( pos2 );
					if( pExistingAIRoad->m_iRoadChunkId == pRoadChunk->m_iRoadChunkId &&
						pExistingAIRoad->m_iAIRegion1 == pRoadChunkAIRegion->m_iId &&
						pExistingAIRoad->m_iAIRegion2 == pAdjRoadChunkAIRegion->m_iId )
					{
						bFound = true;
						break;
					}
				}
				if( bFound == false )
				{
					// This is an AIRoad
					CEOSAIRoad2* pAIRoad = new CEOSAIRoad2( iNextPathfinderPointId );
					pAIRoad->m_pRegionManager = this->m_AICommonData.GetAIRegionManager();
					pAIRoad->m_iAIRegion1 = pRoadChunkAIRegion->m_iId;
					pAIRoad->m_iAIRegion2 = pAdjRoadChunkAIRegion->m_iId;
					pAIRoad->m_iRoadChunkId = pRoadChunk->m_iRoadChunkId;
					pAIRoad->m_Location = ConvertToEOSAILocation( pRoadChunk->m_Location );
					//pAIRoad->m_Location = pRoadChunk->m_Location;

					// Attach the road to the AIRegions
					pRoadChunkAIRegion->AddAIRoad( pAIRoad );
					pAdjRoadChunkAIRegion->AddAIRoad( pAIRoad );
					//m_AIRoads.AddTail( pAIRoad ); // this owns the AIRoads
					m_AICommonData.GetAIRegionManager()->m_AIRoads.AddTail( pAIRoad );

					//m_AICommonData.GetAIRegionManager()->
					m_AICommonData.GetAIRegionManager()->m_PathfinderPointArray.SetAtGrow( iNextPathfinderPointId, pAIRoad );
					iNextPathfinderPointId++;
				}
			}
		}
	}
	pos = m_AICommonData.GetAIRegionManager()->m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIRegion2* pAIRegion = (CEOSAIRegion2*) m_AICommonData.GetAIRegionManager()->m_GenericRegionList.GetNext( pos );
		pAIRegion->SetupAIRoadConnections();
	}

	//CSectorMap SectorMap;
	watch.Start();
	//SectorMap.Build( pWorldDesc );
	watch.Stop();
	float fBuildTime2 = watch.GetDeltaSeconds();

	watch.Start();
	//float fPixelDistance2 = SectorMap.GetPixelDistance( CLocation(Pixel,200,50), CLocation(Pixel,900,900), EnumMovementType3_Water );
	watch.Stop();
	float fTime3 = watch.GetDeltaSeconds();

	long iItemsCreated = 0;
	pos = m_AICommonData.GetAIRegionManager()->m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		//CAIRegion* pAIRegion = (CAIRegion*) m_GenericRegionList.GetNext( pos );
		CEOSAIRegion2* pAIRegion = (CEOSAIRegion2*) m_AICommonData.GetAIRegionManager()->m_GenericRegionList.GetNext( pos );
		if( pAIRegion->IsLand() || pAIRegion->IsCity() )
		{
			POSITION pos2 = pAIRegion->m_AdjacentRegions.GetHeadPosition();
			while( pos2 )
			{
				//CAIRegion* pNextRegion = pAIRegion->m_AdjacentRegions.GetNext( pos2 );
				CAdjacentRegionDesc* pAdjRegionDesc = pAIRegion->m_AdjacentRegions.GetNext( pos2 );
				if( pAdjRegionDesc->m_pRegion->IsWater() )
				{
					// Create a GeoSwitch, attach it to the LandGeo
					//CGeoCoastalData*  pGeoCoastalData = InvokeGeoCoastalData( pAIRegion->GetGeoId() );
					CGeoCoastalData*  pGeoCoastalData = InvokeGeoCoastalData( pAIRegion->GetGeoId() );
					CGeoSwitchItem* pGeoSwitchItem = new CGeoSwitchItem();
					pGeoSwitchItem->m_iLandAIRegion = pAIRegion->m_iId;
					pGeoSwitchItem->m_iWaterAIRegion = pAdjRegionDesc->m_pRegion->m_iId;
					pGeoCoastalData->m_GeoSwitchList.AddTail( pGeoSwitchItem );
					iItemsCreated++;
				}
			}
		}
	}
	/* DEBUG
	long iMax = m_GeoCoastalArray.GetSize();
	for( long i=0; i<iMax; i++ )
	{
		CGeoCoastalData* pGeoCoastalData = m_GeoCoastalArray.GetAt( i );
		if( pGeoCoastalData )
		{
			POSITION pos = pGeoCoastalData->m_GeoSwitchList.GetHeadPosition();
			while( pos )
			{
				CGeoSwitchItem* pSwitchItem = pGeoCoastalData->m_GeoSwitchList.GetNext( pos );
				int h=0;
			}
			int g=0;
		}
	}
	*/
	int jrwe3=0;
/*
	// Iterate over that region's adjacent regions
	POSITION pos = pAIRegion->m_AdjacentRegions.GetHeadPosition();
	while( pos )
	{
		//CAIAdjacentRegionDistance* pDist = pAIRegion->m_AdjacentRegions.GetNext( pos );
		//CAIRegion* pNextRegion = pDist->m_pRegion;
		CAdjacentRegionDesc* pNextAdjRegionDesc = pAIRegion->m_AdjacentRegions.GetNext( pos );
		CAIRegion* pNextRegion = dynamic_cast< CAIRegion* >( pNextAdjRegionDesc->m_pRegion );//pAIRegion->m_AdjacentRegions.GetNext( pos );
*/

	int p=0;
}


void CEOSAIInterface2::CreateRegionSeed( long iRegionId, long x, long y, CList< CAIRegionSeed* >* pRegionSeedList )
{
	CAIRegionSeed* pSeed = new CAIRegionSeed;
	pSeed->m_iRegionId = iRegionId;
	pSeed->m_iPosX = x;
	pSeed->m_iPosY = y;
	pSeed->m_iTerrainType9 = m_pWorldDesc->GetActualTerrainMap().GridValue( x, y );
	/*
	if( pSeed->m_iTerrainType9 == TERRAIN_WATER || 
		pSeed->m_iTerrainType9 == TERRAIN_COAST )
	{
		pSeed->m_iTerrainType3 = TERRAIN_WATER;
	}
	else if( pSeed->m_iTerrainType9 == TERRAIN_CITY )
	{
		pSeed->m_iTerrainType3 = TERRAIN_CITY;
	}
	else if( pSeed->m_iTerrainType9 == TERRAIN_UNKNOWN )
	{
		pSeed->m_iTerrainType3 = TERRAIN_UNKNOWN; ASSERT( false );
	}
	else
	{
		pSeed->m_iTerrainType3 = TERRAIN_GRASS;
	}
	*/

	m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( x, y ) = iRegionId;
	pRegionSeedList->AddTail( pSeed );
}

void CEOSAIInterface2::CreateRegion( long iRegionId, long x, long y, CCity* pCity )
{
	// The server creates a spatial map + AIRegions which is used as an
	//   AI-cheat.  When the Server does this, it doesn't have an pAIPlan.
	//   So, we have two different code paths.
	if( iRegionId == 13 )
	{
		int h=0;
	}

	//CEOSAIRegion2* pRegion = new CEOSAIRegion2( this, CLocation( Grid, x,y ), iRegionId );//, m_pWorldDesc, this, CLocation( Grid, x, y ) );
	CEOSAIRegion2* pRegion = new CEOSAIRegion2( m_AICommonData.GetAIRegionManager(), CEOSAILocation( CEOSAILocation::Grid, x,y ), iRegionId );//, m_pWorldDesc, this, CLocation( Grid, x, y ) );

	pRegion->m_iGeo = m_pWorldDesc->GetGeoNumber( x, y );

	pRegion->m_iTerrainType9 = m_pWorldDesc->GetActualTerrainMap().GridValue( x, y );
	if( pRegion->m_iTerrainType9 == TERRAIN_WATER || 
		pRegion->m_iTerrainType9 == TERRAIN_COAST )
	{
		pRegion->m_iTerrainType3 = TERRAIN_WATER;
	}
	else if( pRegion->m_iTerrainType9 == TERRAIN_CITY )
	{
		pRegion->m_iTerrainType3 = TERRAIN_CITY;
	}
	else if( pRegion->m_iTerrainType9 == TERRAIN_UNKNOWN )
	{
		pRegion->m_iTerrainType3 = TERRAIN_UNKNOWN; ASSERT( false );
	}
	else
	{
		pRegion->m_iTerrainType3 = TERRAIN_GRASS;
	}

	if( pCity )
	{
		ASSERT( pRegion->m_iTerrainType9 == TERRAIN_CITY );
	}

	if( pRegion->m_iTerrainType9 == TERRAIN_CITY )
	{
		// Find the closest city
		CCity* pClosestCity = NULL;
		float fClosestCityDistance = 1000000.0f;
		POSITION pos = m_pWorldDesc->GetPoiList()->GetHeadPosition();
		while( pos )
		{
			CPoi* pPoi = m_pWorldDesc->GetPoiList()->GetNext( pos )->GetPtr();
			CCity* pCity = dynamic_cast< CCity* >( pPoi );
			if( pCity )
			{
				float fDist = m_pWorldDesc->GetPixelDistance( pCity->GetLocation(), CLocation( Grid,x,y ) );
				if( fDist < fClosestCityDistance )
				{
					fClosestCityDistance = fDist;
					pClosestCity = pCity;
				}
			}
		}
		ASSERT( pClosestCity );
		pRegion->m_pCity = pClosestCity;

		//ASSERT( pClosestCity->GetLocation() == CLocation( Grid, x,y ) );
		//pRegion->SetLocation( pClosestCity->GetLocation() );
		pRegion->SetLocation( ConvertToEOSAILocation( pClosestCity->GetLocation() ) );
	}

	pRegion->m_iRegionSize = 1;
	pRegion->m_iMinX = x;
	pRegion->m_iMaxX = x;
	pRegion->m_iMinY = y;
	pRegion->m_iMaxY = y;
	/*
	if( m_pAIBrain )
	{
		pRegion->UpdateAreaCitResOwnerBalance();
	}
	*/
	m_AICommonData.GetAIRegionManager()->m_AIRegionArray.SetAtGrow( pRegion->m_iId, pRegion );
	m_AICommonData.GetAIRegionManager()->m_PathfinderPointArray.SetAtGrow( pRegion->m_iId, pRegion );
	m_AICommonData.GetAIRegionManager()->m_GenericRegionList.AddTail( pRegion );
	//m_AIRegionArray.SetAt( pRegion->m_iId, pRegion );
}

void CEOSAIInterface2::GrowRegionSeeds( CList< CAIRegionSeed* >& RegionSeedList )
{
	// Grow the seeds
	long iSeedCount = (long) RegionSeedList.GetCount();
	while( RegionSeedList.IsEmpty() == FALSE )
	{
		CAIRegionSeed* pSeed = RegionSeedList.RemoveHead();

		int  iNextX;
		int  iNextY;
		long iGeo;
		long iTerrainType;
		long iRegion;
		//
		for( long i=0; i<8; i++ )
		{
			if( i==0 )
			{
				iNextX = pSeed->m_iPosX+1;
				iNextY = pSeed->m_iPosY;
			}
			if( i==1 )
			{
				iNextX = pSeed->m_iPosX-1;
				iNextY = pSeed->m_iPosY;
			}
			if( i==2 )
			{
				iNextX = pSeed->m_iPosX;
				iNextY = pSeed->m_iPosY+1;
			}
			if( i==3 )
			{
				iNextX = pSeed->m_iPosX;
				iNextY = pSeed->m_iPosY-1;
			}
			if( i==4 )
			{
				iNextX = pSeed->m_iPosX+1;
				iNextY = pSeed->m_iPosY+1;
			}
			if( i==5 )
			{
				iNextX = pSeed->m_iPosX+1;
				iNextY = pSeed->m_iPosY-1;
			}
			if( i==6 )
			{
				iNextX = pSeed->m_iPosX-1;
				iNextY = pSeed->m_iPosY-1;
			}
			if( i==7 )
			{
				iNextX = pSeed->m_iPosX-1;
				iNextY = pSeed->m_iPosY+1;
			}

			/*
			m_pWorldDesc->GetWorldBuildDesc()->TruncateGridLocation( &iNextX, &iNextY );
			if( m_pWorldDesc->GetWorldBuildDesc()->IsValidGridLocation( iNextX, iNextY ) == false )
			{
				continue;
			}
			*/

			// I want don't want the regions to cross map-wrap boundaries
			//   But, I do want regions on opposite sides of the map-edge to know they are "adjacent"
			if( m_pWorldDesc->GetWorldBuildDesc()->IsValidGridLocation( iNextX, iNextY ) )
			{
				// Make sure I don't stray too far from the Region center
				float fGridDistanceFromCenter = 0.0f;
				CEOSAIRegion2* pParentRegion = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( pSeed->m_iRegionId );
				ASSERT( pParentRegion );
				if( pParentRegion )
				{
					//pRegion->m_CenterLocation.SetGrid( pSeed->m_iPosX, pSeed->m_iPosY );
					fGridDistanceFromCenter = m_pWorldDesc->GetGridDistance( pSeed->m_iPosX, pSeed->m_iPosY, pParentRegion->GetLocation().GetGridX(), pParentRegion->GetLocation().GetGridY() );
					//fDistanceFromCenter = m_pWorldDesc->GetGridDistance( pSeed->m_iPosX,pSeed->m_iPosY, iNextX,iNextY );
				}

				iRegion = m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( iNextX, iNextY );

				//if( ( pSeed->m_iTerrainType != TERRAIN_COAST && fDistanceFromCenter < 12.0f ) ||
				//	( pSeed->m_iTerrainType == TERRAIN_COAST && fDistanceFromCenter <  6.0f ) )
				bool bValid = false;
				#ifdef _TERRAIN_COAST_IS_SEPARATE_AIREGION_
				if( pSeed->m_iTerrainType9 == TERRAIN_COAST )
				{
					bValid = ( fGridDistanceFromCenter < 6.0f );
				}
				else if( pSeed->m_iTerrainType9 == TERRAIN_CITY )
				{
					bValid = ( fGridDistanceFromCenter < 24.0f );
				}
				#else
				if( pSeed->m_iTerrainType9 == TERRAIN_CITY )
				{
					bValid = ( fGridDistanceFromCenter < 24.0f );
				}
				#endif _TERRAIN_COAST_IS_SEPARATE_AIREGION_
				else
				{
					bValid = ( fGridDistanceFromCenter < 12.0f );
				}
				//if( ( pSeed->m_iTerrainType9 != TERRAIN_COAST && fDistanceFromCenter < 13.0f ) ||
				//	( pSeed->m_iTerrainType9 == TERRAIN_COAST && fDistanceFromCenter <  6.0f ) )
				//	( pSeed->m_iTerrainType9 == TERRAIN_CITY  && fDistanceFromCenter <  6.0f ) )
				if( bValid )
				{
					iTerrainType = m_pWorldDesc->GetActualTerrainMap().GridValue( iNextX, iNextY );
					iGeo = m_pWorldDesc->GetGeoNumber( iNextX, iNextY );
					//if( iGeo == pSeed->m_iGeo && iRegion == 0 )
					//if( iGeo == pSeed->m_iGeo && iTerrainType == pSeed->m_iTerrainType && iRegion == 0 )
					/*
					bool bSameTerrain = (pSeed->m_iTerrainType == iTerrainType);
					if( ( pSeed->m_iTerrainType == TERRAIN_WATER || pSeed->m_iTerrainType == TERRAIN_COASTAL ) &&
						( iTerrainType == TERRAIN_WATER || iTerrainType == TERRAIN_COASTAL ) )
					{
						bSameTerrain = true;
					}
					*/
					bool bExpandIntoHere = ( iTerrainType == pSeed->m_iTerrainType9 );
					#ifndef _TERRAIN_COAST_IS_SEPARATE_AIREGION_
					if( ( iTerrainType == TERRAIN_WATER || iTerrainType == TERRAIN_COAST ) &&
						( pSeed->m_iTerrainType9 == TERRAIN_WATER || pSeed->m_iTerrainType9 == TERRAIN_COAST ) )
					{
						bExpandIntoHere = true;
					}
					#endif
					//if( iTerrainType == pSeed->m_iTerrainType9 && iRegion == 0 )
					if( bExpandIntoHere && iRegion == 0 )
					{
						//iRegionSeedsCreated++;
						//CreateRegionSeed( pSeed->m_iRegionId, iNextX, iNextY, &RegionSeedList );

						CAIRegionSeed* pSeed2 = new CAIRegionSeed;
						pSeed2->m_iRegionId = pSeed->m_iRegionId;
						pSeed2->m_iPosX = iNextX;
						pSeed2->m_iPosY = iNextY;
						//pSeed2->m_iGeo  = pSeed->m_iGeo;
						pSeed2->m_iTerrainType9 = pSeed->m_iTerrainType9;
						m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( iNextX, iNextY ) = pSeed->m_iRegionId;
						RegionSeedList.AddTail( pSeed2 );
						//RegionSeedList.AddHead( pSeed2 );

						CEOSAIRegion2* pRegion = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( pSeed->m_iRegionId );
						ASSERT( pRegion );
						if( pRegion )
						{
							pRegion->m_iRegionSize++;

							if( iNextX < pRegion->m_iMinX ){ pRegion->m_iMinX = iNextX; }
							if( iNextX > pRegion->m_iMaxX ){ pRegion->m_iMaxX = iNextX; }
							if( iNextY < pRegion->m_iMinY ){ pRegion->m_iMinY = iNextY; }
							if( iNextY > pRegion->m_iMaxY ){ pRegion->m_iMaxY = iNextY; }
						}
					}
				}
			}
			// Setup "adjacent" information
			m_pWorldDesc->GetWorldBuildDesc()->TruncateGridLocation( &iNextX, &iNextY );
			if( m_pWorldDesc->GetWorldBuildDesc()->IsValidGridLocation( iNextX, iNextY ) )
			{
				iRegion = m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( iNextX, iNextY );
				if( iRegion != pSeed->m_iRegionId && iRegion != 0 )
				{
					// Record the "adjacent region" information
					CEOSAIRegion2* pRegion1 = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( pSeed->m_iRegionId );
					CEOSAIRegion2* pRegion2 = m_AICommonData.GetAIRegionManager()->m_AIRegionArray.GetAt( iRegion );
					if( pRegion1 )
					{
						//pRegion1->AddAdjacentRegion( iRegion );
						long iDoubleCheckRegion = m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( pSeed->m_iPosX,pSeed->m_iPosY );
						ASSERT( pRegion1->m_iId == iDoubleCheckRegion );
						pRegion1->AddAdjacentRegion( pRegion2, CEOSAILocation( CEOSAILocation::Grid, pSeed->m_iPosX,pSeed->m_iPosY ) );
					}
					if( pRegion2 )
					{
						long iDoubleCheckRegion = m_AICommonData.GetAIRegionManager()->m_RegionMap.GridValue( iNextX,iNextY );
						ASSERT( pRegion2->m_iId == iDoubleCheckRegion );
						pRegion2->AddAdjacentRegion( pRegion1, CEOSAILocation( CEOSAILocation::Grid, iNextX,iNextY ) );
					}
				}
			}
		}
		//iRegionSeedsDeleted++;
		delete pSeed;
	}
}

void CEOSAIInterface2::SetupAIRoadConnections()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	if( m_RoadsOnTheRegionEdge.GetCount() >= 2 )
	{
		POSITION pos = m_RoadsOnTheRegionEdge.GetHeadPosition();
		while( pos )
		{
			CEOSAIRoad2* pAIRoad = m_RoadsOnTheRegionEdge.GetNext( pos );
			SetupAIRoadConnections( pAIRoad );
		}
	}

	int z=0;
}


void CEOSAIInterface2::SetupAIRoadConnections( CEOSAIRoad2* pAIRoad )
{
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	CList< CRoadChunk* >  TouchedRoadChunkList;

	//ASSERT( pAIRoad->m_RoadsIConnectTo.IsEmpty() );
	//while( pAIRoad->m_RoadsIConnectTo.IsEmpty() == false ){ delete pAIRoad->m_RoadsIConnectTo.RemoveHead(); }
	//if( pAIRoad->m_iRoadChunkId == 399 )
	//if( pAIRoad->GetId() == 399 )
	//{
	//	int g=0;
	//}

	static long iVisitedValue = 0;
	iVisitedValue++;
	if( iVisitedValue > 200000000 ){ iVisitedValue = 0; }

	CRoadChunk* pCurrentRoadChunk = pWorldDescServer->GetRoadChunk( pAIRoad->m_iRoadChunkId );
	pCurrentRoadChunk->m_iTempVisited = iVisitedValue;//true;
	pCurrentRoadChunk->m_fTempDistance = 0.0f;
	pCurrentRoadChunk->m_iRoadPathfinderPoint = pAIRoad->GetId();

	CList< CRoadChunk* >  RoadChunkList;
	RoadChunkList.AddTail( pCurrentRoadChunk );
	//TouchedRoadChunkList.AddTail( pCurrentRoadChunk );
	while( RoadChunkList.IsEmpty() == FALSE )
	{
		CRoadChunk* pCurrentRoadChunk = RoadChunkList.RemoveHead();
		//pCurrentRoadChunk->m_bTempVisited = true;
		pCurrentRoadChunk->m_iTempVisited = iVisitedValue;//true;
		TouchedRoadChunkList.AddTail( pCurrentRoadChunk );

		POSITION pos2 = pCurrentRoadChunk->GetAdjacentRoadChunkList()->GetHeadPosition();
		while( pos2 )
		{
			long iNextRoadChunk = pCurrentRoadChunk->GetAdjacentRoadChunkList()->GetNext( pos2 );
			CRoadChunk* pNextRoadChunk = pWorldDescServer->GetRoadChunk( iNextRoadChunk );
			if( pNextRoadChunk == NULL )
			{
				ASSERT( false );
				continue;
			}
			//
			if( pNextRoadChunk->m_iTempVisited == iVisitedValue ) continue;
			float fDistance = pWorldDescServer->GetPixelDistance( pCurrentRoadChunk->m_Location, pNextRoadChunk->m_Location );
			//float fDistance = g_pWorldDistanceTool->GetDistance( pCurrentRoadChunk->m_Location, pNextRoadChunk->m_Location );
			//
			pNextRoadChunk->m_fTempDistance = pCurrentRoadChunk->m_fTempDistance + fDistance;
			//pNextRoadChunk->m_bTempVisited = true;
			pNextRoadChunk->m_iTempVisited = iVisitedValue;
			pNextRoadChunk->m_iRoadPathfinderPoint = pAIRoad->GetId();

			// Is this NextChunk one of the AIRoads?
			POSITION pos3 = m_RoadsOnTheRegionEdge.GetHeadPosition();
			while( pos3 )
			{
				CEOSAIRoad2* pAIRoad3 = m_RoadsOnTheRegionEdge.GetNext( pos3 );
				if( pAIRoad3->m_iRoadChunkId == iNextRoadChunk )
				{
					// Create connection information - both directons
					CEOSAIAdjacentAIRoad2Distance* pRoadDistance;
					pRoadDistance = new CAdjacentAIRoadDistance();
					pRoadDistance->m_pIntermediateRegion = this;
					pRoadDistance->m_pAIRoad = pAIRoad3;
					pRoadDistance->m_fPixelDistance = pNextRoadChunk->m_fTempDistance;
					pAIRoad->m_RoadsIConnectTo.AddTail( pRoadDistance );
					ASSERT( pAIRoad->m_RoadsIConnectTo.GetCount() < 9 );

					/*
					pRoadDistance = new CAdjacentAIRoadDistance();
					pRoadDistance->m_pIntermediateRegion = this;
					pRoadDistance->m_pAIRoad = pAIRoad;
					pRoadDistance->m_fPixelDistance = pNextRoadChunk->m_fTempDistance;
					pAIRoad3->m_RoadsIConnectTo.AddTail( pRoadDistance );
					ASSERT( pAIRoad3->m_RoadsIConnectTo.GetCount() < 6 );
					*/
				}
			}
//test this
			CGenericRegion* pNextRegion = m_pRegionManager->GetGenericRegion( pNextRoadChunk->m_Location );
			//if( pNextRegion == this )
			if( pNextRegion->GetId() == pAIRoad->m_iAIRegion1 ||
				pNextRegion->GetId() == pAIRoad->m_iAIRegion2 )
			{
				//pNextRoadChunk->m_bTempVisited = true;
				RoadChunkList.AddTail( pNextRoadChunk );
			}
		}
	}

	// Reset the 'visited' flag because we'll need it set to false for other AIRegion::SetupAIRoadConnections
	while( TouchedRoadChunkList.IsEmpty() == FALSE )
	{
		CRoadChunk* pRoadChunk = TouchedRoadChunkList.RemoveHead();
		pRoadChunk->m_iTempVisited = 0;
	}
}
/*
CEOSAIPoiObject2* CEOSAIInterface2::GetAIPoiObject( long iPoiObjectId )
{
	// This is really inefficient.  I need a different data structure
	POSITION pos = m_AIPoiObjectList.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject2* pAIPoiObject = m_AIPoiObjectList.GetNext( pos );
		if( pAIPoiObject->GetObjectId() == iPoiObjectId )
		{
			return pAIPoiObject;
		}
	}
	return NULL;
}
*/

// Post-AI processing
/*
void CEOSAI::CreateOrders()
{
	ASSERT( false );
}
*/

bool CEOSAIInterface2::CreateAIUnitOrders( CEOSAIUnit2PathwayResult* pAIUnitPathwayResult )
{
	//CWorldDescPlayer* pWorldDescPlayer = m_pAIUnitPathwayFinder->GetAIBrain()->GetWorldDescPlayer();
	//CWorldDescServer* pWorldDescServer = m_pAIUnitPathwayFinder->GetAIBrain()->GetWorldDescServer();
	//CWorldDescPlayerProxy* pWorldDescPlayerProxy = m_pAIUnitPathwayFinder->GetAIBrain()->GetWorldDescPlayerProxy();

	CEOSAIUnit2* pAIUnitActor = m_pAIUnitPathwayFinder->GetAIUnitActor();//>GetAIPoiObjectActor();
	//CAIUnit2PathwayResult* pPath = &m_SimpleRoute;
	//CAIBrain* pAIBrain = pAIUnitActor->GetAIBrain();
	//long iAIPlayer = pAIBrain->GetAIPlayerNumber();
	long iAIPlayer = pAIUnitActor->GetOwner();

	ASSERT( m_pAIUnitPathwayFinder->GetStartLocation() == pAIUnitActor->GetInitialState()->GetLocation() );

	//CAIUnit2* pAIUnit = dynamic_cast< CAIUnit2* >( m_pActor );
	//if( m_pAIUnitActor == NULL ) return;
	CUnit* pUnit = pAIUnitActor->GetServerUnit();
	if( pUnit == NULL ) return false;
	pUnit->DeleteOrders();

	long iNumberOfOrdersCreated = 0;
	long iNumberOfOrdersFailed = 0;
	m_bAllOrdersWereCreated = false;

	CAIRegionManager* pAIRegionManager = g_pAICommonData->GetAIRegionManager();
	CLocation LastStepEnd = pAIUnitActor->GetInitialState()->GetLocation(); // LastStepEnd is for sanity check
	long iCurrentStep = 0;

/*
	// We want to check through the steps to figure out if I need to put any delays for simultaneous arrival
	POSITION pos = m_Steps.GetHeadPosition();
	while( pos )
	{
		CAIUnit2PathwayResultStep* pResultStep = m_Steps.GetNext( pos );
		if( pResultStep->m_pPredefinedStep &&
			pResultStep->m_pPredefinedStep->GetAIUnitActionIdea() &&
			pResultStep->m_pPredefinedStep->GetAIUnitActionIdea()->GetTacticalProject2() )
		{
			CAITacticalProject2* pTacticalProject2 = pResultStep->m_pPredefinedStep->GetAIUnitActionIdea()->GetTacticalProject2();
			//long id = pTacticalProject2->m_Id;
			if( pTacticalProject2->IsASecretInvasion() )
			{
				int h=0;
			}
			if( pTacticalProject2->IsASecretInvasion() &&
				pTacticalProject2->GetTaskForceWithPredefinedSteps() )
			{
				float fCompletionTime = pTacticalProject2->GetTaskForceWithPredefinedSteps()->GetCompletionTime();
				int h=0;
			}
		}
	}
*/
	if( m_pAIUnitPathwayFinder->GetAIUnitActor() )
	{
		float fWaitTime = m_pAIUnitPathwayFinder->GetAIUnitActor()->GetInitialWaitTime();
		if( fWaitTime > 0.1f )
		{
			CUnitOrder* pNewOrder1 = new CUnitOrder();
			pNewOrder1->SetWorldDesc( pWorldDescServer );//pUnit->GetWorldDesc() );
			pNewOrder1->SetParent( pUnit->ObjectId() );
			pNewOrder1->Wait( 0.5f );//fWaitTime );
			//pResultStep->AddSupportUnitOrder( pNewOrder1 );
			pUnit->AppendOrder( pNewOrder1 );
		}
	}

	//POSITION pos = pPath->m_PathwaySteps.GetHeadPosition();
	POSITION pos = m_Steps.GetHeadPosition();
	while( pos )
	{
		//CAIUnit2PathwayPredefinedStep* pPredefinedStep = m_Steps.GetNext( pos );
		CEOSAIUnit2PathwayResultStep* pResultStep = m_Steps.GetNext( pos );

		bool bSimultaneousArrival = false;
		CAITacticalProject2* pTacticalProject2 = NULL;
		if( pResultStep->m_pPredefinedStep &&
			pResultStep->m_pPredefinedStep->GetAIUnitActionIdea() &&
			pResultStep->m_pPredefinedStep->GetAIUnitActionIdea()->GetTacticalProject2() )
		{
			pTacticalProject2 = pResultStep->m_pPredefinedStep->GetAIUnitActionIdea()->GetTacticalProject2();
			if( pTacticalProject2->IsASecretInvasion() &&
				pTacticalProject2->GetTaskForceWithPredefinedSteps() )
			{
				bSimultaneousArrival = true;
				//float fCompletionTime = pTacticalProject2->GetTaskForceWithPredefinedSteps()->GetCompletionTime();
				//int h=0;
			}
		}

		if( pResultStep->m_bTransportNeeded )
		{
			// This can return false if this is a hypothetical (unbuilt) transport
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( pResultStep->m_pPredefinedStep->GetTransportToUse() );
			}
		}

		//
		if( pResultStep->m_pPredefinedStep && // Can be null if this is an AirMovement (land at airbase)
			pResultStep->m_pPredefinedStep->GetTransportToUse() )
		{
			// Make sure this transport has setup its orders
			CAIUnit2* pAITransport = pResultStep->m_pPredefinedStep->GetTransportToUse();
			pAITransport->GetAIUnitPathwayFinder()->CreateOrders();

			bool bUsingHypotheticalTransport = false;
			CLocation TransporteePickupLocation;
			//
			CEOSAIUnit2PathwayPredefinedStep* pPredefinedTransportPickupStep = pAITransport->GetAIUnitPathwayFinder()->GetTransportPickupStep( pResultStep->m_pPredefinedStep );
			CEOSAIUnit2PathwayResultStep* pResultTransportPickupStep = NULL;
			CUnitOrder* pTransportPickupOrder = NULL;
			if( pPredefinedTransportPickupStep ) // Not all transport tasks have a 'pickup'
			{
				pResultTransportPickupStep = pPredefinedTransportPickupStep->GetResultStep();
				if( pResultTransportPickupStep )
				{
					pTransportPickupOrder = pResultTransportPickupStep->GetUnitOrder();
					if( pTransportPickupOrder == NULL )
					{
						bUsingHypotheticalTransport = true;
						TransporteePickupLocation = pResultTransportPickupStep->GetGroundPickupLocation(); 
					}
				}
				else
				{
					if( Public::m_bAssertWhenAIPathwayFails ){ ASSERT( false ); }
				}
			}

			CAIUnit2PathwayPredefinedStep* pPredefinedTransportDropoffStep = pAITransport->GetAIUnitPathwayFinder()->GetTransportDropoffStep( pResultStep->m_pPredefinedStep );
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( pPredefinedTransportDropoffStep );
			}
			if( pPredefinedTransportDropoffStep == NULL ) continue;
			CAIUnit2PathwayResultStep* pResultTransportDropoffStep = pPredefinedTransportDropoffStep->GetResultStep();
			if( pResultTransportDropoffStep == NULL )
			{
				if( Public::m_bAssertWhenAIPathwayFails ){ ASSERT( false ); }
				continue;
			}
			CUnitOrder* pTransportDropoffOrder = pResultTransportDropoffStep->GetUnitOrder();
			if( pTransportDropoffOrder == NULL )
			{
				bUsingHypotheticalTransport = true;
			}

			// Given a WaterAIRegion and a LandAIRegion, return Water and Land Locations for Pickup/Dropoff
			/*
			CLocation WaterLocation, LandLocation;
			CAIUnit2PathwayFinder::FindPickupDropoffLocations( 
				pPredefinedTransportPickupStep->GetRealTransport_PickupWaterAIRegion(),
				pPredefinedTransportPickupStep->GetRealTransport_PickupLandAIRegion(),
				&WaterLocation, &LandLocation );
			*/

			if( bUsingHypotheticalTransport )
			{
				CUnitOrder* pNewOrder1 = new CUnitOrder();
				pNewOrder1->SetWorldDesc( pWorldDescServer );//pUnit->GetWorldDesc() );
				pNewOrder1->SetParent( pUnit->ObjectId() );
				pNewOrder1->MoveTo( TransporteePickupLocation );
				pResultStep->AddSupportUnitOrder( pNewOrder1 );
				pUnit->AppendOrder( pNewOrder1 );
				if( pNewOrder1->CalculateAStar() )
				{
					iNumberOfOrdersCreated++;
				}
				else
				{
					pUnit->DeleteOrder( pNewOrder1 );
					pNewOrder1 = NULL;
					iNumberOfOrdersFailed++;
					if( Public::m_bAssertWhenAIPathwayFails )
					{
						ASSERT( false );
					}
				}

				// Can't add any more orders after this one, so exit
				pos = NULL;
				continue;
			}

			// This will result in several orders
			if( pTransportPickupOrder )
			{
				ASSERT( pTransportPickupOrder->GetChildPoiPD() );

				CUnitOrder* pNewOrder1 = new CUnitOrder();
				pNewOrder1->SetWorldDesc( pWorldDescServer );//pUnit->GetWorldDesc() );
				pNewOrder1->SetParent( pUnit->ObjectId() );
				pNewOrder1->Enter( pTransportPickupOrder->GetChildPoiPD()->GetObjectId() );
				pUnit->AppendOrder( pNewOrder1 );
				if( pNewOrder1->CalculateAStar() )
				{
					pResultStep->AddSupportUnitOrder( pNewOrder1 );
					iNumberOfOrdersCreated++;
				}
				else
				{
					pUnit->DeleteOrder( pNewOrder1 );
					pNewOrder1 = NULL;
					iNumberOfOrdersFailed++;
					if( Public::m_bAssertWhenAIPathwayFails )
					{
						ASSERT( false );
					}
				}
			}

			// This will result in several orders
			{
				ASSERT( pTransportDropoffOrder );
				ASSERT( pTransportDropoffOrder->GetChildPoiPD() );
				if( pTransportDropoffOrder )
				{
					CUnitOrder* pNewOrder2 = new CUnitOrder();
					pNewOrder2->SetWorldDesc( pWorldDescServer );//pUnit->GetWorldDesc() );
					pNewOrder2->SetParent( pUnit->ObjectId() );
					pNewOrder2->Exit( pTransportDropoffOrder->GetChildPoiPD()->GetObjectId() );
					pResultStep->AddSupportUnitOrder( pNewOrder2 );
					pUnit->AppendOrder( pNewOrder2 );
					
					// Exit Orders don't need the AStar Calculation
					iNumberOfOrdersCreated++;
				}
				/*
				if( pNewOrder2->CalculateAStar() )
				{
					iNumberOfOrdersCreated++;
				}
				else
				{
					pUnit->DeleteOrder( pNewOrder2 );
					pNewOrder2 = NULL;
					iNumberOfOrdersFailed++;
				}
				*/
			}
		}
		//CAIUnit2PathwayResultStep* pResultStep = pPath->m_PathwaySteps.GetNext( pos );
		/*
		CAIUnit2PathwayResultStep* pResultStep = pPredefinedStep->GetResult( enum_FinalPath );
		if( pResultStep->m_bIsValid == false )
		{
			break; // this step isn't valid, and the subsequent steps won't be either
		}
		*/

		ASSERT( LastStepEnd == pResultStep->m_StartLocation );
		#ifdef _DEBUG
		if( LastStepEnd != pResultStep->m_StartLocation )
		{
			POSITION pos4 = m_pAIUnitPathwayFinder->GetResultPath()->m_Steps.GetHeadPosition();
			while( pos4 )
			{
				CAIUnit2PathwayResultStep* pResultStep = m_pAIUnitPathwayFinder->GetResultPath()->m_Steps.GetNext( pos4 );
				int hg=0;
			}
			CList< CAIUnit2PathwayResultStep* >* pResults = &m_pAIUnitPathwayFinder->GetResultPath()->m_Steps;
			int g=0;
			CList< CAIUnit2PathwayPredefinedStep* >* pPredefinedList = m_pAIUnitPathwayFinder->GetPreDefinedPath();
			int g2=0;
		}
		#endif _DEBUG
		//CAIUnit2PathwayPredefinedStep* pPoint = pResultStep->m_pPredefinedStep;
		//CAIUnit2PathwayPredefinedStep* pPredefinedStep = pResultStep->m_pPredefinedStep;
		CWorldDescBase* pDEBUG_WorldDesc = pUnit->GetWorldDesc();

		/*
		CAIUnit2PathwayFinder* pPathwayFinder = m_pAIUnitPathwayFinder;
		POSITION pos3 = pPathwayFinder->GetPreDefinedPath()->GetHeadPosition();
		while( pos3 )
		{
			CAIUnit2PathwayPredefinedStep* p = pPathwayFinder->GetPreDefinedPath()->GetNext( pos3 );
			int g=0;
		}
		*/
		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

		CUnitOrder* pNewOrder = new CUnitOrder();
		pNewOrder->SetWorldDesc( pWorldDescServer );
		pNewOrder->SetParent( pUnit->ObjectId() );

		//
		CLocation LocationEnd = pResultStep->m_EndLocation;
		ASSERT( LocationEnd.GetRealX() != 0.0f || LocationEnd.GetRealY() != 0.0f );
		if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_Waypoint )
		{
			pNewOrder->MoveTo( LocationEnd.GetRealX(), LocationEnd.GetRealY() );
		}
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_LandAtAirbase )
		{
			ASSERT( m_pAIUnitPathwayFinder->GetUnitTemplate()->IsAirUnit() );
			pNewOrder->Enter( pResultStep->m_pAITarget->GetObjectId() );
		}
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_Repair )
		{
			pNewOrder->Enter( pResultStep->m_pAITarget->GetObjectId() );
		}
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_Upgrade )
		{
			// The upgrade order is a separate order
			pNewOrder->Enter( pResultStep->m_pAITarget->GetObjectId() );
			//pNewOrder->Upgrade( pResultStep->m_pAITarget->GetObjectId() );
		}
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_BuildAirfield )
		{
			ASSERT( m_pAIUnitPathwayFinder->GetAIUnitTemplate()->IsGroundUnit() );
			pNewOrder->BuildAirfield( LocationEnd );
		}
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_CaptureCitRes )
		{
			pNewOrder->Attack( pResultStep->m_pAITarget->GetObjectId() );
			pNewOrder->SetAttackGoal( EnumAttackGoal_Capture );

			// DEBUG - I MUST either be at war with this player, or have 'sneak attack' setup
			long iTargetOwner = pResultStep->m_pAITarget->GetOwner();
			ASSERT(
				pWorldDescServer->GetForeignRelations( iAIPlayer, iTargetOwner ).IsEnemy() ||
				pWorldDescPlayerProxy->GetSneakAttack( iTargetOwner ) );
			// DEBUG
		}
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_AttackUnit )
		{
			//bool bIsABombing = pUnit->OneAttackPerLandingVs(pResultStep->m_pAITarget->GetServerPoiObject());
			bool bIsABombing = pUnit->OneAttackPerLandingVs( pResultStep->m_pAITarget );

			pNewOrder->Attack( pResultStep->m_pAITarget->GetObjectId() );
			pNewOrder->IsABombing( bIsABombing );
			pNewOrder->SetAttackGoal( EnumAttackGoal_KillUnits );

			// DEBUG - I MUST either be at war with this player, or have 'sneak attack' setup
			long iTargetOwner = pResultStep->m_pAITarget->GetOwner();
			ASSERT(
				pWorldDescServer->GetForeignRelations( iAIPlayer, iTargetOwner ).IsEnemy() ||
				pWorldDescPlayerProxy->GetSneakAttack( iTargetOwner ) );
			// DEBUG
		}
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_DegradeCityOrAirfield )
		{
			//bool bIsABombing = pUnit->OneAttackPerLandingVs(pResultStep->m_pAITarget->GetServerPoiObject());
			bool bIsABombing = pUnit->OneAttackPerLandingVs( pResultStep->m_pAITarget );

			pNewOrder->Attack( pResultStep->m_pAITarget->GetObjectId() );
			pNewOrder->IsABombing( bIsABombing );
			pNewOrder->SetAttackGoal( EnumAttackGoal_DegradeCity );

			// DEBUG - I MUST either be at war with this player, or have 'sneak attack' setup
			long iTargetOwner = pResultStep->m_pAITarget->GetOwner();
			ASSERT(
				pWorldDescServer->GetForeignRelations( iAIPlayer, iTargetOwner ).IsEnemy() ||
				pWorldDescPlayerProxy->GetSneakAttack( iTargetOwner ) );
			// DEBUG
		}

		// Pickup/Dropoff Orders
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_Pickup )
		{
			pNewOrder->MoveTo( LocationEnd.GetRealX(), LocationEnd.GetRealY() );
			pNewOrder->CreateChildPoiPD();
			//pResultStep->m_iPickupPoiPD = pNewOrder->CreateChildPoiPD();
			ASSERT( pNewOrder->GetChildPoiPD() );
		}
		else if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_Dropoff )
		{
			pNewOrder->MoveTo( LocationEnd.GetRealX(), LocationEnd.GetRealY() );
			pNewOrder->CreateChildPoiPD();
			ASSERT( pNewOrder->GetChildPoiPD() );

			//bCalculateAStar = false;
			//pResultStep->m_iDropoffPoiPD = pNewOrder->CreateChildPoiPD();
		}
		else
		{
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}
		}

		CAIUnit2PathwayPredefinedStep* pPredefinedStep = pResultStep->m_pPredefinedStep;
		//if( pUnit->GetObjectId() == 232 &&
		//	pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_Dropoff )
		//{
		CString strDebug;
		long iNumberOfGridsInAllowedPath = 0;
		if( pPredefinedStep )
		{
			//POSITION pos2 = pResultStep->m_AIRegionMovementPath.GetHeadPosition();
			CAIRegion* pAIRegionEnd = m_pAIUnitPathwayFinder->GetAIBrain()->GetAIRegionManager()->GetAIRegion( LocationEnd );

			if( pUnit->GetObjectId() == 11 ) // Transport=11, Infantry=16, Aircraft=24
			{
				bool bClearData = false;
				if( bClearData )
				{
					g_AIPathwayData.RemoveAll();
				}
				POSITION pos2 = pPredefinedStep->GetDirectMovementPath()->GetHeadPosition();
				while( pos2 )
				{
					CPathfinderPoint* pPathfinderPoint = pPredefinedStep->GetDirectMovementPath()->GetNext( pos2 );
					g_AIPathwayData.AddTail( pPathfinderPoint->m_iId );
				}
			}
			POSITION pos2 = pPredefinedStep->GetAIRegionAllowedMovementPath()->GetHeadPosition();
			while( pos2 )
			{
				CAIRegion* pAIRegion = pPredefinedStep->GetAIRegionAllowedMovementPath()->GetNext( pos2 );
				iNumberOfGridsInAllowedPath += pAIRegion->m_iRegionSize;
				//pNewOrder->AddAIRegionToPermittedPath( pAIRegion->m_iId );
				pNewOrder->AddAIRegionToPreferredPath( pAIRegion->m_iId );

				//if( pAIRegionEnd->m_iId == 307 )
				if( pUnit->GetObjectId() == 11 ) // Transport=11, Infantry=16, Aircraft=24
				{
					bool bClearData = false;
					if( bClearData )
					{
						g_HighlightAIRegions.RemoveAll();
					}
					g_HighlightAIRegions.AddTail( pAIRegion->m_iId );

					CString str; str.Format( _T("Added %d"), pAIRegion->m_iId );
					Checkpoint::Write( str );
					strDebug += str + _T("\n");
				}
			}
			if( pAIRegionEnd->m_iId == 307 )
			{
				CString str; str.Format( _T("Added UnitOrder ptr:%x id:%d"), pNewOrder, pNewOrder->GetObjectId() );
				Checkpoint::Write( str );
				strDebug += str + _T("\n");
			}
		}
		CAIRegion* pAIRegionStart = m_pAIUnitPathwayFinder->GetAIBrain()->GetAIRegionManager()->GetAIRegion( pNewOrder->GetStartingPoint() );
		CAIRegion* pAIRegionEnd = m_pAIUnitPathwayFinder->GetAIBrain()->GetAIRegionManager()->GetAIRegion( LocationEnd );

		// When the blocking-unit is an enemy unit, the AI might (sometimes) want to fight-through the unit
		// But, sometimes the blocking-unit is neutral, and the AI shouldn't try to walk through them
		pNewOrder->WalkAroundBlockingUnits( true );//false );
		pUnit->AppendOrder( pNewOrder );
		CString str; str.Format( _T("Added UnitOrder ptr:%x id:%d"), pNewOrder, pNewOrder->GetObjectId() );
		strDebug += str + _T("\n");
		if( pNewOrder->CalculateAStar() )
		{
			iNumberOfOrdersCreated++;
			pResultStep->SetUnitOrder( pNewOrder );

			// Add the upgrade order
			if( pResultStep->m_eTask == CAIUnit2PathwayResultStep::enum_Upgrade )
			{
				CUnitOrder* pNewOrder2 = new CUnitOrder();
				pNewOrder2->SetWorldDesc( pWorldDescServer );
				pNewOrder2->SetParent( pUnit->ObjectId() );
				pNewOrder2->Upgrade( pResultStep->m_pAITarget->GetObjectId() );
				pUnit->AppendOrder( pNewOrder2 );
			}
		}
		else
		{
			if( Public::m_bAssertWhenAIPathwayFails )
			{
				ASSERT( false );
			}

			//#ifdef _DEBUG
			//pNewOrder->CalculateAStar(); // try it again - for debugging purposes
			//#endif _DEBUG
			//#ifdef _DEBUG
			//pNewOrder->CalculateAStar(); // try it again - for debugging purposes
			//#endif _DEBUG

			pUnit->DeleteOrder( pNewOrder );
			pNewOrder = NULL;
			iNumberOfOrdersFailed++;
			break;
		}
		if( pNewOrder )
		{
			ASSERT( pNewOrder->GetType() != CUnitOrder::enumType_Undefined );
		}

		if( pTacticalProject2 && pNewOrder )
		{
			//if( pPredefinedStep )
			//{
			//	pPredefinedStep->GetEstimatedMovementTime();
			//}
			//float fEndTime1 = pResultStep->GetEndTime();
			pTacticalProject2->AddToResultSteps( pResultStep );
			//float fEndTime = pUnit->GetTimeUntilCompletionOfOrder( pNewOrder );
			//pResultStep->SetUnitOrderEndTime( fEndTime1 );
		}
		//if( pTacticalProject2 && pNewOrder && bSimultaneousArrival )
		//{
		//	float fEndTime = pUnit->GetTimeUntilCompletionOfOrder( pNewOrder );
		//	pTacticalProject2->SetResultStepCompletionTime( pResultStep, fEndTime );
		//}

		LastStepEnd = LocationEnd;
		iCurrentStep++;
	}

	if( iNumberOfOrdersCreated > 0 && iNumberOfOrdersFailed == 0 )
	{
		m_bAllOrdersWereCreated = true;
	}
	return m_bAllOrdersWereCreated;
}


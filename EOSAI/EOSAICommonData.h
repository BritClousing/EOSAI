
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIMultiRegionManager2.h"
#include "EOSAIRegionManager2.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAIWorldDistanceTool.h"
#include "EOSAIGlobalForeignRelations.h"
#include "EOSAIUnitTemplateSet.h"
#include "PoiObject.h"
#include "EOSAIGameRules.h"
#include "EOSAIThoughtDatabase.h"
#include "EOSAIRegionMapToEverywhere.h"
#include "EOSAIPlayerOwnershipMap.h"

#include "EOSAIMail.h"
#include "EOSAIMailResponse.h"
#include "EOSAITradeAgreement.h"
#include "EOSAITradeAgreementResponse.h"
#include "EOSAIGamePlayer.h"
//
namespace EOSAI
{
	class CInterface;
	class CWorldDistanceTool;
};

extern EOSAI::CWorldDistanceTool* g_pWorldDistanceTool;
extern EOSAI::CInterface* g_pEOSAIInterface;

//#include "
//class CEOSAIGeo;
class CEOSAIMultiRegionNationwidePathways;
class CEOSAINationalSummary3;
class CEOSAIPlayerInteraction;

namespace EOSAI
{
class DLLIMPEXP CCommonData
{
	public:
		CCommonData();
		~CCommonData();

		// This sets the globals
		void  SetEOSAIInterface( EOSAI::CInterface* pEOSAIInterface );
		void  SetEOSAIWorldDistanceTool( EOSAI::CWorldDistanceTool* pWorldDistanceTool );
		//void  SetWorldDistanceTool(EOSAI::CWorldDistanceTool* p);

		//
		void  DeleteData();
		void  DeleteAIPoiObjects();
		void  ResetAIPlayerDataInAIPoiObjects(int iPlayer);

		void  SetNeedToRebuildData( bool b );
		bool  GetNeedToRebuildData(){ return m_bNeedToRebuildData; }
		bool  GetDataIsValid(){ return m_bDataIsValid; }
		void  RebuildDataIfNecessary();

		// Players
		//
			// Game Players (humans and AI)
			//void                 AddGamePlayer( long iPlayer, EOSAI::EnumGamePlayerType ePlayerType, bool bIsAlive );
			//EOSAI::CGamePlayer*  GetGamePlayer( long iPlayer ){ return m_Players[iPlayer]; }
			// AI Players
			//void  CreateAIPlayer( long iPlayer, long iIntelligence100 ){}
			//
			long  GetNumberOfPlayers(); // temporary - just a pass-though function for EOSAIInterface
			//long  GetNumberOfHumanPlayers();
			//long  GetNumberOfActivePlayers(); // Players who are alive

		//void  CreateData();

		bool  AllowAutomaticTurnAdvancement(){ return true; } // During AI-vs-AI games

		//long  GetCurrentTurn(){ ASSERT( false ); return m_iCurrentTurn; }
		//bool  GetGameHasEnded(){ ASSERT( false ); return m_bGameHasEnded; }
		//bool  GetGameHasEnded(){ ASSERT( false ); return false; }

		//bool  AllActiveHumanPlayersHaveSubmittedTurn( long iCurrentTurn ){ ASSERT( false ); return m_bAllActiveHumanPlayersHaveSubmittedTurn; }

		//long  GetNumberOfActiveAIPlayers();


		//
		// Map
		//
			CEOSAIMultiRegionManager2* GetMultiRegionManager(){ return &m_MultiRegionManager; }
			CEOSAIRegionManager2*      GetAIRegionManager(){ return &m_AIRegionManager; }
			//CEOSAIGeo*              GetAIGeo( long iGeoId );
			//CList< CEOSAIGeo* >*    GetAIGeoList(){ return &m_AIGeoList; }

			CEOSAIRegionMapToEverywhere*  GetAIRegionMapToEverywhere() { return &m_AIRegionMapToEverywhere; }

			CEOSAIMultiRegionNationwidePathways*  GetNationwidePathways(long iPlayer) { return m_NationwidePlayerPathways.Value(iPlayer); }

			long                       m_OwnershipMapLastUpdatedTurn = -1;
			CEOSAIPlayerOwnershipMap*  GetCurrentOwnershipPlayerMap8() { return &m_CurrentOwnershipPlayerMap8; }
			CEOSAIPlayerOwnershipMap*  GetLastTurnOwnershipPlayerMap8() { return &m_LastTurnOwnershipPlayerMap8; }

		//
		// Foreign Relations
		//
			EOSAIEnumForeignRelations GetForeignRelations( long iPlayer1, long iPlayer2 ){ return m_AIGlobalForeignRelations.GetForeignRelations( iPlayer1, iPlayer2 ); }
			void                      SetForeignRelations( long iPlayer1, long iPlayer2, EOSAIEnumForeignRelations e ){ m_AIGlobalForeignRelations.SetForeignRelations( iPlayer1, iPlayer2, e ); }
			bool                      HasSetSneakAttack( long iAttacker, long iTarget );
			CEOSAIGlobalForeignRelations*  GetGlobalForeignRelations(){ return &m_AIGlobalForeignRelations; }

			void SetAllPlayersPermanentlyAtWar(bool b) { m_bAllPlayersPermanentlyAtWar = b; }
			bool GetAllPlayersPermanentlyAtWar() { return m_bAllPlayersPermanentlyAtWar; }

		// Player Interactions
		//
			//void  AddPlayerInteractionAndSendFeelingsUpdate(CEOSAIPlayerInteraction* p);
			void  AddPlayerInteraction(CEOSAIPlayerInteraction* p);
			long  GetPlayerInteraction_WarDuration( long iPlayer1, long iPlayer2 );
			CList< CEOSAIPlayerInteraction* >* GetPlayerInteractions(){ return &m_PlayerInteractions; }

			void ProcessPlayerInteractionsIfNecessary();
			void CalculateForeignRelationsFeelingsBasedOnPlayerInteractionHistoryAndSendFeelingsUpdate( int iTurn );

		//
		// AIGeos - These are the "player neutral" AIGeos. They should get (duplicated?) and set to the player number
		//
			//CList< CEOSAIGeo* >*  GetGeoList(){ return &m_EOSAIGeos; }
			CEOSAIBCDumbArray1D< CEOSAIGeo* >*  GetAIGeoArray(){ return &m_AIGeoArray; }
			CEOSAIGeo*  GetAIGeo( long iGeo );
			CEOSAIGeo*  GetAIGeo( CEOSAILocation Location );
			long        GetGeoId( CEOSAILocation Location );
			//CEOSAIGeo*  GetGeo( long iGeo ){ ASSERT( false ); return NULL; }
			//CEOSAIGeo*  GetGeo( CEOSAILocation Location ){ ASSERT( false ); return NULL; }
			//long        GetGeoId( CEOSAILocation Location ){ ASSERT( false ); return 0; }

		//	CList< CGeo* >*  GetGeoList(){ return &m_Geos; }
		//	CGeo*  GetGeo( long iGeo ){ ASSERT( false ); return NULL; }
		//	CGeo*  GetGeo( CEOSAILocation Location ){ ASSERT( false ); return NULL; }
		//	long   GetGeoId( CEOSAILocation Location ){ ASSERT( false ); return 0; }
			//void   AddAdjacentGeos( CEOSAIIntSet& Geos );
			void   AddAdjacentWaterGeos( CEOSAIIntSet& Geos );
			void   AddAdjacentLandGeos( CEOSAIIntSet& Geos );

		//
		// Do we still use the BuildOptions list?
		//
			CList< CEOSAIBuildOption* >* GetAIBuildOptionList(){ return &m_AIBuildOptionList; }

		//
		// Resources
		//
			float  OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X(){ return 1.0f; }
			float  OPENMARKET_COST_TO_BUY_RESOURCE_X(){ return 3.0f; }
			void   SetPlayerResources( int iPlayer, CString strResource, float fResourceAmount );

		//
		// Map Summary
		//
			CEOSAINationalSummary3* GetAINationalSummary3(long iPlayer) { return m_AINationalSummaries.Value(iPlayer); }
			CEOSAIThoughtDatabase*  GetAIThoughtDatabase(long iPlayer); // Just a utility function

			float  GetTotalCityProductionOnMap(){ return m_fTotalCityProductionOnMap; }
			float  GetTotalCitResProductionOnMap(){ return m_fTotalCitResProductionOnMap; }
			float  GetTotalUnownedCitResProductionOnMap(){ return m_fTotalUnownedCitResProductionOnMap; }
			float  GetTotalResourcesOnMap(){ return m_fTotalResourcesOnMap; }
			float  GetPercentageOfUnownedCitResOnMap01(){ return m_fTotalUnownedCitResProductionOnMap / max( 0.1f, m_fTotalCitResProductionOnMap ); }

			long   GetNumberOfPoiOwnedByPlayer( long iPlayer );
			long   GetNumberOfCitiesOwnedByPlayer( long iPlayer );

			long   GetNumberOfBuildingsOwnedByPlayer( long iPlayer, CEOSAIBuildingDescription* pBuildingDesc );
			long   GetNumberOfBuildingsInProductionByPlayer( long iPlayer, CEOSAIBuildingDescription* pBuildingDesc );

		//
		// Poi Objects
		//
			CList< CEOSAIPoiObject* >*  GetAIPoiObjects(){ return &m_AIPoiObjects; }
			void              AddAIPoiObject( CEOSAIPoiObject* pAIPoiObject );
			void              AddHypotheticalAIPoiObject( CEOSAIPoiObject* pAIPoiObject ); // Used by AIBrains
			CEOSAIPoiObject*  GetAIPoiObject( long iObjectId );
			CEOSAIUnit2*      GetAIUnit( long iObjectId );
			long   GetLastAIUnitCombatCapabilitiesCalculatedTurn() { return m_iUnitCombatCapabilities_LastTurnCalculated; }

			int   GetNumberOfPoi();
			int   GetNumberOfCities();
			int   GetNumberOfResources();
			int   GetNumberOfAirfields();
			int   GetNumberOfUnits();

		// Airbases
		//
			void  GetAirbases( long iPlayer, CEOSAIAirbasesSet* pAirbasesSet );
			void  GetAirbases( long iPlayer, CEOSAIUnitTemplate* pAirUnit, CEOSAIAirbasesSet* pAirbasesSet );
			void  GetAirbases( long iPlayer, CEOSAIUnitTemplate* pAirUnit, CEOSAILocation AirUnitLocation, float fLimitToDistance, CEOSAIAirbasesSet* pAirbasesSet );

		// Cheat Settings
		//
			//long  AICheat_GetGeoNumber( CEOSAILocation Location ){ return AICheat_GetGeoNumber( Location.GetGridX(), Location.GetGridY() ); }
			//long  AICheat_GetGeoNumber( long iGridX, long iGridY );
			//float AICheat_GetGeoDistanceFromPlayerCity( long iPlayer, long iGeoNumber );
			//void  AICheat_GetGeoPlayerCitResSummary( long iPlayer, long iGeoNumber, CEOSAICitResSummary* pCitResSummary );
			//float AICheat_GetGeoPlayerCityProduction( long iPlayer, long iGeoNumber );
			//float AICheat_GetGeoPlayerIronProduction( long iPlayer, long iGeoNumber );
			//float AICheat_GetGeoPlayerGoldProduction( long iPlayer, long iGeoNumber );
			//float AICheat_GetGeoPlayerFoodProduction( long iPlayer, long iGeoNumber );
			//float AICheat_GetGeoPlayerOilProduction(  long iPlayer, long iGeoNumber );
			//void  AICheat_GetGeoPlayerMilitarySummary( long iPlayer, long iGeoNumber, CEOSAIUnitSummary* pUnitSummary );

			// Total Values
			//float AICheat_GetPlayerCityProduction( long iPlayer );
			//float AICheat_GetTotalProductionOnMap();
			//float AICheat_GetTotalUnownedCitResOnMap();
			//float AICheat_GetTotalCitResOnMap();
			//void  AICheat_GetNationalSummary( long iPlayer, CEOSAINationalSummary* pSummary );
			//void  AICheat_GetPlayerCitResSummary( long iPlayer, CEOSAICitResSummary* pCitResSummary );
			//void  AICheat_GetPlayerUnitSummary( long iPlayer, CEOSAIUnitSummary* pUnitSummary );

	private:

		bool  m_bNeedToRebuildData;
		bool  m_bDataIsValid;

		bool  m_bAllPlayersPermanentlyAtWar;

	public:

		// PoiObjects -> MultiRegions + AIRegions
		void  AddObjectIdsToAIRegionsAndMultiRegions();

		// National Pathways + Summary
		void  CalculateNationwidePathways();
		void  CalculateNationalSummaries();

	private:

		// Combat
		void  CalculateAIUnitCombatCapabilities( long iCurrentTurn );
		long  m_iUnitCombatCapabilities_LastTurnCalculated;

		//
		// Map
		//
			CEOSAIBCDumbArray1D< CEOSAIGeo* >   m_AIGeoArray;
			CEOSAIRegionMapToEverywhere   m_AIRegionMapToEverywhere;
			CEOSAIBCDumbArray1D< CEOSAIMultiRegionNationwidePathways* >  m_NationwidePlayerPathways; // Pathway data for each player
			public:
			CEOSAIMultiRegionManager2  m_MultiRegionManager;
			CEOSAIRegionManager2       m_AIRegionManager;
			private:
			// This is a grid-level map, but I should probably change it to an AIRegion map
			//   I'm sure the AIRegion map would work almost as well (although there could be some edge-cutting issues)
			CEOSAIPlayerOwnershipMap  m_CurrentOwnershipPlayerMap8;
			CEOSAIPlayerOwnershipMap  m_LastTurnOwnershipPlayerMap8;
			public:
			float  m_fTotalCityProductionOnMap;
			float  m_fTotalCitResProductionOnMap;
			float  m_fTotalUnownedCitResProductionOnMap;
			float  m_fTotalResourcesOnMap;

	public:
		// National summary information for each player
		CEOSAIBCDumbArray1D< CEOSAINationalSummary3* >  m_AINationalSummaries;

	private:
		// Foreign Relations
		CList< CEOSAIPlayerInteraction* >  m_PlayerInteractions;
		long                               m_iHighestPlayerInteractionIdInList = 0;
		long                               m_iHighestPlayerInteractionIdProcessed = 0;

		CEOSAIGlobalForeignRelations       m_AIGlobalForeignRelations;

		// Poi Objects
		CList< CEOSAIPoiObject* >   m_AIPoiObjects;
		CList< CEOSAIBuildOption* > m_AIBuildOptionList;

		// Messages
		// These are stored here, rather than in the AIPlayer object because they should be processed sequentally
		//
			CList< CEOSAIMail* >                    m_UnprocessedMail;
			CList< CEOSAIMailResponse* >            m_UnprocessedMailResponses;
			CList< CEOSAITradeAgreement* >          m_UnprocessedTrade;
			CList< CEOSAITradeAgreementResponse* >  m_UnprocessedTradeAgreementResponses;

	private:
		void  CalculateAIRegionOwnership();
		void  CalculateMultiRegionOwnership();

};
};
extern EOSAI::CCommonData* g_pEOSAICommonData;

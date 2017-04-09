
#pragma once

/*
	The AIPlan is an object where the AI stores a plan.
		What happens is that the AI creates several plans and then choses the best one.

	Why does the AI create several plans rather than "growing" a single best plan?
		Unfortunately, strategy doesn't always find the best solution by a stepwise procedure.
		For example, take these two plans:

		* Capture unowned cities.
		* Invade an enemy island.

		If I begin with "Capture unowned cities", it's not always possible to via a
			stepwise procedure to "invade an enemy island".  The main problem is that
			the intermediate position (send a few units to invade) is bad.
			A few units will be destoyed and it will make the enemy mad - it's a waste.
		However, the best plan might be an all-out assault.  How does the AI arrive
			at that conclusion?
*/

	// Valuation of multi-class tasks
	//   e.g. If I build a tank, it is both an Explorer and a CityHunter
	//   both of these roles have value.  What value do I put on this?
	//   How do I score this against other tasks, such as "build aircraft" which
	//   fulfills the Explorer role better than a tank, but has no CityHunter role?
	// The value of the explorer role changes over time and depends on the situation.
	//   e.g. aircraft have a limited range around cities and airfields.
	//   e.g. tanks aren't that useful as explorers if I've already fully explored my
	//        island and I have no transports


namespace EOSAI{ class AIPlayer; };
//class CEOSAIBrain;
//class CWorldDesc;

#include "PoiObject.h"
#include "EOSAIUnit.h"
//#include "AISpatialMap.h"
#include "EOSAIRegionManager2.h"
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIActionScheduler.h"
#include "EOSAIResource.h"
//#include "EOSAIResourceEffectsSummary.h"
//#include "AIDefinedAction.h"
//#include "EOSAIBuildManager.h"
#include "EOSAIUnitSummary.h"
#include "EOSAIForeignRelationsSituation.h"
#include "EOSAIActionScheduler.h"
//#include "TradeAgreement.h"
//#include "MovementManager.h"
class CEOSAIGeo;
class CEOSAIGeoView;
class CEOSAIGeoLand;
#include "EOSAIThoughtDatabase.h"
#include "EOSAIBuildManager.h"
#include "EOSAIRegionMapToEverywhere.h"
#include "PlayerResourceSummary.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAIStopwatch2.h"
class CEOSAIMultiRegionPlan;
//class CWorldDescPlayer;
//class CAIProductionOption;
class CAIUnresolvedSituation;
//class CTradeAgreement;
class CEOSAITransportCombinedMap;
//class CWorldDescServer;
//class CWorldDescPlayerProxy;

//class CAIPlan
class CEOSAIBrain
{
	public:

		//
			CEOSAIBrain();
			void Initialize( EOSAI::AIPlayer* pAIPlayer );
			~CEOSAIBrain();

			void  DeleteAIData();

			EOSAI::AIPlayer* GetAIPlayer(){ return m_pAIPlayer; }
			long        GetAIPlayerNumber();
			//void                SetWorldDescPlayer( CWorldDescPlayer* p ){ m_pWorldDescPlayer = p; }
			//CWorldDesc*         GetWorldDesc(){ return (CWorldDesc*) m_pWorldDescPlayer; }
			//CWorldDescPlayer*   GetWorldDescPlayer(){ return m_pWorldDescPlayer; }
			//CWorldDescServer*      GetWorldDescServer();
			//CWorldDescPlayerProxy* GetWorldDescPlayerProxy();
			EOSAI::CActionScheduler* GetAIActionScheduler(){ return &m_AIActionScheduler; }
			CEOSAIThoughtDatabase*   GetAIThoughtDatabase(){ return &m_AIThoughtDatabase; }
			EOSAI::CAIBuildManager*  GetAIBuildManager(){ return &m_AIBuildManager; }
			//CEOSAIForeignRelationsSituation*  GetAIForeignRelationsSituation(){ return &m_ForeignRelationsSituation; }
			//CEOSAICitResValueMultiplier* GetCitResValueMultiplier(){ return &m_CitResValueMultiplier; }

			//CAIObject*     GetAIObject( long iObjectId );
			//CEOSAIPoiObject*  GetAIPoiObject( long iObjectId );
			//CEOSAIUnit*       GetAIUnit( long iObjectId );
			//
			//CList< CAIObject* >*     GetAIObjectList(){ return &m_AIObjectList; }
			//CList< CEOSAIPoiObject* >*  GetAIPoiObjectList(){ return &m_AIPoiObjects; }

			// Resources
			//
				//void  CalculateResourceConsumptionAndDeltas(bool bIncludeCityNoOrdersProduceWealth);
				//void    CalculateResourceDeltasAndResourceEffectsSummary();
				//float GetTotalMoney(){ ASSERT( false ); return 0.0f; }
				//float GetTotalFood(){ ASSERT( false ); return 0.0f; }
				//float GetTotalIron(){ ASSERT( false ); return 0.0f; }
				//float GetTotalOil(){ ASSERT( false ); return 0.0f; }
				//CEOSAIResourceEffectsSummary*  GetAIResourceEffectsSummary(){ return &m_ResourceSummary; }


			void  CreateAIProductionOptions();//{ ASSERT( false ); }
			//void  DeleteAIProductionOptions();

			void  ResetProcessingFlags()
			{
				m_bProcessInitialTurnState = true;
				m_bProcessTurnBasedOnForeignRelations = true;
			}
			void  SetFlag_ProcessTurnBasedOnForeignRelations( bool b ){ m_bProcessTurnBasedOnForeignRelations = b; }

			//void  AddAIRegion( CEOSAIRegion2* pObj ){ AddAIObject( pObj ); }
			//void  AddAIRegion( CEOSAIRegion2* pObj ){ m_AIRegionList.AddTail( pObj ); }

		// TacticalProject support functions
		//   Ideally, AllowHostility should have a turn attached to it (e.g. allow hostility against P1 on turnX)
		//
			bool  GetAllowHostilityAgainst( long iPlayer );
			void  SetAllowHostilityAgainst( long iPlayer, bool bAllowHostility );

		// TransportComboMaps
		//
			void  CreateTransportCombinedAIRegionMaps();
			CList< CEOSAITransportCombinedMap* >*  GetTransportCombinedAIRegionMaps(){ return &m_TransportCombinedAIRegionMaps; }
			void  DeleteTransportCombinedAIRegionMaps();

			void  CreateTransportCombinedMultiRegionMaps();
			CList< CEOSAITransportCombinedMap* >*  GetTransportCombinedMultiRegionMaps(){ return &m_TransportCombinedMultiRegionMaps; }
			void  DeleteTransportCombinedMultiRegionMaps();

		//
		// Trade and Negotiations
		//
			long  SendTradeOffersAndNegotiations();

		// Process
		//
			//void  ClearOldData();
			//void  ClearUnitOrders();
			//void  ClearCityBuildOrders();

			void  CreateInitialTurnData_AIPoiObjects_BuildOptions_NationalSummaries();
			//void  CalculateResourceShortageAndExcess();

			// AIGeos
			//void    GenerateAIGeos();
			void  SortAIGeosByDistance();

//			CEOSAIGeo* GetAIGeo( long iAIGeo ){ return m_AIGeoArray[iAIGeo]; }
//			CEOSAIBCDumbArray1D< CEOSAIGeo* >*  GetAIGeoArray(){ return &m_AIGeoArray; }
			void    CalculateAIGeoViewValues();
			void    CreateAIGeoPlans();

		//	void    AddDesiresAsMapLabels();
		//	void    AddAIGeoSituationsAsMapLabels();
		//	void    AddAIGeoPlansAsMapLabels();

			// AIMultiRegionPlans
			void  CreateMultiRegionPlans();
		//	void  AddAIMultiRegionPlansAsMapLabels();
		//	void  AddAIMultiRegionDangerAccessAsMapLabels();
			CEOSAIMultiRegionPlan* GetMultiRegionPlan( long iMultiRegion ){ return m_MultiRegionPlanArray[iMultiRegion]; }

			// Project Ideas
		//	void  AddAITacticalProjects2AsMapLabels();

			//
			//void  CreateDesiresAndTacticalProjects();
			void  Process();

			//
			void  BuyNeededResourcesFromOpenMarket();

		//

			//float GetUnexploredCountOfAllObjectsWithinPath( CEOSAILocation Start, float fViewRange, CEOSAILocation End );
			float GetEstimatedCitResProductionPerArea(){ return m_fEstimatedCitResProductionPerArea; }

		//
		// Processing
		//
			void  CalculateMyNationwidePathways();

			//void  EvaluateCurrentMilitaryCapability();
			void  EvaluateNationalNeeds();
			void  UpdateNationalInProductionSummary();
			void  UpdateNationalUnsatisfiedMilitaryCapability();
			CEOSAIUnitSummary* GetUnsatisfiedMilitaryCapability(){ return &m_UnsatisfiedMilitaryCapability; }

		//

		// AIProductionOptions
		//
			//CList< CAIProductionOption* >*  GetProductionOptions(){ return &m_AIProductionOptions; }
		//
		// Plan Score
		//
			//float GetTotalScore(); // based on task scores
			float CalculateGrowthScore();  // might want to remove this

		//
		// Spatial Map
		//
			float              GetSpatialMapDistance_NoTransports( EOSAI::PoiMobile* pActor, CEOSAILocation Start, CEOSAILocation End );
		//
		// Transport
		//
			/*
			void CreateTransportActions(
							CAIDesire* pDesire,
							CAIWeaklyDefinedAction* pWeaklyDefinedAction,
							EOSAI::PoiMobile* pUnitToBeTransported,
							EOSAI::PoiObjectState* pActorState,
							CAIObject* pTargetEndLocation );
			void DefineDecentSeaTransportActions(
							CAIDesire* pDesire,
							CAIWeaklyDefinedAction* pWeaklyDefinedAction,
							EOSAI::PoiMobile* pUnitToBeTransported,
							EOSAI::PoiObjectState* pActorState,
							CAIObject* pFinalTarget );
			*/

		// Unresolved Situations
		//
			//void  AddToUnresolvedForeignRelationsOffers( CAIUnresolvedSituation* pSit ){ m_UnresolvedForeignRelationsOffers.AddTail( pSit ); }

		// Trade Change
		//
			//void  Incoming_TradeAgreementStateChange( CTradeAgreement* pTradeAgreement, EOSAIEnumTradeAgreementState OldState, EOSAIEnumTradeAgreementState NewState );

		// AIObjects
		//
			//void AddAIPoiObject( CEOSAIPoiObject* pObj );

	protected:

		//
			//void TurnPoiObjectsIntoAIObjects();
			void AddAIObjectsToRegions();

		// Checkpoints
		//
			//void SetCheckpoint();
			//void RestoreToCheckpoint();
			//long GetCheckpointId(){ return m_iCheckpointId; }

		//
		//
		EOSAI::AIPlayer*         m_pAIPlayer;
		EOSAI::CActionScheduler  m_AIActionScheduler;
		CEOSAIThoughtDatabase    m_AIThoughtDatabase;
		EOSAI::CAIBuildManager   m_AIBuildManager;
		//bool                m_bAllowAIToCheat;
		//CEOSAIResourceEffectsSummary  m_AIResourceEffectsSummary;
		//EOSAI::PlayerResourceSummary  m_AIResourceSummary;

		//CEOSAIRegionMapToEverywhere   m_AIRegionMapToEverywhere;

		// AIGeos
		//CEOSAIBCDumbArray1D< CEOSAIGeo* >      m_AIGeoArray;
		CEOSAIListSort< CEOSAIGeoView*, CEOSAIGeoView* >  m_LandGeosSortedByDistanceCitResScore;
		CEOSAIBCDumbArray1D< CEOSAIMultiRegionPlan* >  m_MultiRegionPlanArray;

		// I should have a MovementManager for all players - including for the human
		//   players so AI players can figure out where human players might move
		//CEOSAIBCDumbArray1D< CMovementManager* >  m_MovementManagerArray;
		//CMovementManager        m_MovementManager;

	//
		CList< CEOSAITransportCombinedMap* >  m_TransportCombinedAIRegionMaps;
		CList< CEOSAITransportCombinedMap* >  m_TransportCombinedMultiRegionMaps;

	protected:

		// Processing control
		//
			bool  m_bProcessInitialTurnState;
			bool  m_bProcessTurnBasedOnForeignRelations;

		// These are *geo-only* summaries
		//
			CEOSAIUnitSummary   m_InitialNeed; // National needs (includes geo desires)
			CEOSAIUnitSummary   m_CurrentMilitaryCapability;
			CEOSAIUnitSummary   m_InProductionMilitaryCapability;
			CEOSAIUnitSummary   m_UnsatisfiedMilitaryCapability;

		// CitRes Estimations - Used by non-cheating AIs for estimating other-player power
		//
			//void   EstimateCitResProductionPerArea();
			float  GetEstimatedCitResProductionPerWaterArea(){ return m_fEstimatedCitResProductionPerWaterArea; }
			float  GetEstimatedCitResProductionPerLandArea(){ return m_fEstimatedCitResProductionPerLandArea; }
			//
			float  m_fEstimatedCitResProductionPerArea;
			float  m_fEstimatedCitResProductionPerLandArea;
			float  m_fEstimatedCitResProductionPerWaterArea;

			float GetPercentAssumedTotalExpansion(){ return m_fPercentAssumedTotalExpansion; }
			float m_fPercentageOfWorldDiscovered;
			float m_fPercentAssumedTotalExpansion;

		// AIProductionOptions
		//
			bool   AllMyCitiesHaveOrders();
			//
			//void   CreateAIProductionOptions();
			//CList< CAIProductionOption* >  m_AIProductionOptions;

		// Foreign Relations (Situation, IncomingOffers, OutgoingOffers)
		//
			//CEOSAIForeignRelationsSituation      m_ForeignRelationsSituation;
			//CList< CTradeAgreement* >         m_IncomingTradeAgreements;
			//CList< CAIUnresolvedSituation* >  m_UnresolvedForeignRelationsOffers;

		// Checkpointed Units and Cities
		//
			//CList< CEOSAIPoiObject* > m_CheckpointedPoiObjectList;
			//long m_iCheckpointId;

	public:
		// DEBUG
		//
			CEOSAIStopwatch2  m_stopwatchCreateDesires;
			CEOSAIStopwatch2  m_stopwatchCreateTacticalProjects;

			CEOSAIStopwatch2  m_AllocateTacticalProjectsTime;
			CEOSAIStopwatch2  m_CreateStrategiesAndUnitActionIdeasTime;
			CEOSAIStopwatch2    m_MyCities_InvokePathwaysToEverywhere;
			CEOSAIStopwatch2    m_MyUnits_CreateUnitActionIdeas;
			CEOSAIStopwatch2      m_MyUnits_CalculateTimeToTarget;
			CEOSAIStopwatch2        m_MyUnits_CalculateTimeToTarget_InvokePathwaysToEverywhere;
			CEOSAIStopwatch2        m_MyUnits_CalculateTimeToTarget_TargetDesire_InvokePathToEverywhere;
			CEOSAIStopwatch2        m_MyUnits_CalculateTimeToTarget_InvokePath2;
			CEOSAIStopwatch2        m_MyUnits_CalculateTimeToTarget_TransportComboAssistedMovementTime;
			long                   m_GetTransportAssistedMovementPath_Count;
			CEOSAIStopwatch2          m_GetTransportAssistedMovementPath;
			CEOSAIStopwatch2      m_MyUnits_InvokeTransportMovementDescs;
			CEOSAIStopwatch2        m_TransportCalculationsTime;
			CEOSAIStopwatch2  m_CreateBestCaseActionIdeasTime;
			CEOSAIStopwatch2  m_CreateTaskForcesAndPredefinedStepsTime;
			long           m_iAIUnitTransportMovementDescCount;
};



#pragma once

#include "EOSAIListSort.h"
#include "EOSAILocation.h"
//#include "EnumAIThoughtScoreCategory.h"
//#include "AIActionDefinitionUnitCapability.h"
#include "EOSAIUnitTemplateSet.h"
#include "EOSAITransportsSet.h"
#include "EOSAIUnitSummary.h"
//#include "AITacticalProjectStrategy.h"
#include "EOSAITacticalProject2Strategy.h"
//#include "AIFutureBuildIdeas.h"
//#include "BCHashSortedList.h"
class CEOSAITransportCombinedMap;
class CEOSAIGeo;
class CEOSAIBrain;
class CAIThought;
class CAIThought_WhatCityHuntersCanIBuild;
class CAIThought_WhatPlacesWouldILikeToExplore;
//class CEOSAIUnitTemplate;
//class CAIThought_AIPoiObjectFilter;
class CEOSAIMovementDescription;
class CAIThought_ShouldSendUnitsHere;
class CAITacticalProject;
class CEOSAITacticalProject2;
class CEOSAIUnitActionIdea;
class CEOSAIPoiObject;
class CEOSAICity;
class CEOSAIDesire2;
class CEOSAIDesireSpatial;
class CMultiRegionPathwayMap;
class CEOSAIAirbasesSet;
class CEOSAIDebugData;
class CEOSAIWarzoneLocation;

#define DEPTH_OF_THOUGHT_COUNT 11 // 0 - 10
#define SCORE_CATEGORY_COUNT 10   // 0 - 9

/*
	The general AI algorithm: See AIDesign.txt
*/

class CEOSAIThoughtDatabase
{
	public:

		enum EnumDebugDataOutput
		{
			EnumDebugDataOutput_TacticalProjectsSortedBySimpleInterest,
			EnumDebugDataOutput_TacticalProjectsSortedByInitialEvaluationScore,
			EnumDebugDataOutput_TacticalProjectsSortedByHypotheticalTaskForceScore,
			EnumDebugDataOutput_TacticalProjectsStrategies,
		};

	public:

		//
		CEOSAIThoughtDatabase();
		~CEOSAIThoughtDatabase();
		void      SetAIBrain( CEOSAIBrain* pAIBrain ){ m_pAIBrain = pAIBrain; }
		CEOSAIBrain* GetAIBrain(){ return m_pAIBrain; }
		void Clear();

		// Initialization
		//
			void   CalculateUnitsICanBuildOrHave();
			void   CalculatePotentialEnemyUnits();
			//void   CalculateCombatAndAttritionValues();
			void   CompileMyActorsIntoCategories();

		// Sorted ActionDefinitions
		//
			void  GetTacticalProjectsSortedBySimpleInterest( CEOSAIListSort< CEOSAITacticalProject2* >& SortedList );
			//void  GetTacticalProjectsSortedByBasicFeasibility( CEOSAIListSort< CAITacticalProject* >& SortedList );
			//void  GetTacticalProjectsSortedByInitialEvaluationScore( CEOSAIListSort< CAITacticalProject* >& SortedList );
			void  GetTacticalProjectsSortedByInitialEvaluationScore( CEOSAIListSort< CEOSAITacticalProject2* >& SortedList );
			//void  GetTacticalProjectsSortedByHypotheticalTaskForceScore( CEOSAIListSort< CAITacticalProject* >& SortedList );

		// Desires
		//
			void  CreateDesires();
			CList< CEOSAIDesire2* >*            GetDesires(){ return &m_Desires; }
			CEOSAIListSort< CEOSAIDesireSpatial* >*  GetDesiresSortedByScore(){ return &m_DesiresSortedByScore; } // not owned
			void  GetAllDesiresWithinRadius( CEOSAILocation Location, float fRadius, CList< CEOSAIDesireSpatial* >* pOutputList );
			void     WriteDesiresSortedByScoreItems( CList< CEOSAIDebugData* >* pData );
			void     WriteDesiresSortedBySimpleInterestItems( CList< CEOSAIDebugData* >* pData );
			void     WriteTacticalProjectsSortedBySimpleInterest( CList< CEOSAIDebugData* >* pData );
			void     WriteTacticalProjectsSortedByInitialEvaluation( CList< CEOSAIDebugData* >* pDataList );
			void     WriteTacticalProjects2( CList< CEOSAIDebugData* >* pDataList );
			CString  GetSortedDesiresText();

		//
		// Warzones
		//
			void  ClearWarzoneCalculations();
			void  FindWarzones();
			CList< CEOSAIWarzoneLocation* >* GetWarzoneLocations(){ return &m_WarzoneLocations; }
			//void  AddToWarzoneIdleUnitsList( CEOSAIUnit* pAIUnit );
			// Warzones, Nearby tasks, etc
			//void  CalculateTasksIdleUnits();
			//void  CalculatePreferedWarzoneForIdleUnits();
			//void  CreatePredefinedStepsToSendIdleUnitsToWarzones();

		// TacticalProjects, UnitActionValues
		//
			//void  InvokeMultiRegionPathwayDataForEachOfMyCitiesAndGroundAndSeaUnits();

			CList< CEOSAITacticalProject2* >* GetTacticalProjects2(){ return &m_TacticalProjects2; }
			//CList< CAITacticalProject* >*  GetTacticalProjects(){ return &m_TacticalProjects; }
			void    Debug_OutputTacticalProjectsToFile( EnumDebugDataOutput eOutput, CString strFile );
			CString GetSortedTacticalProjectsText( EnumDebugDataOutput eOutput );
			//void    EstimateWhenTacticalProjectsCanBeCompletedByUnitActionIdeas();
			//
			//void  CalculateRelativeTTTScoresForUnitActionIdeas();
			//void  CalculateUnitActionIdeaScores();
			void  ScheduleBuildOrders();
			//
			//void  AddTacticalProject( CAITacticalProject* pTacticalProject );
			void  AddTacticalProject2( CEOSAITacticalProject2* pTacticalProject2 );
			//void  AddToSortedBestCaseTacticalProjectStrategies( CAITacticalProject* pTacticalProject );
			//void  AddToSortedBestCaseTacticalProjectStrategies( CAITacticalProjectStrategy* pStrategy );
		//	void  AddToSortedBestCaseTacticalProjectStrategies( CEOSAITacticalProject2Strategy* pStrategy );
			void  AddToTacticalProjectStrategiesSortedByBestCaseValue( CEOSAITacticalProject2Strategy* pStrategy );
			//CEOSAIListSort< CAITacticalProjectStrategy* >*  GetSortedBestCaseTacticalProjectStrategies(){ return &m_SortedBestCaseTacticalProjectStrategies; }
			//CEOSAIListSort< CEOSAITacticalProject2Strategy* >* GetSortedBestCaseTacticalProject2Strategies(){ return &m_TacticalProject2StrategiesSortedByBestCaseValue; }
			CEOSAIListSort< CEOSAITacticalProject2Strategy* >* GetTacticalProject2StrategiesByBestCaseValue(){ return &m_TacticalProject2StrategiesSortedByBestCaseValue; }

			void  SetTacticalProjects2ScoreIndex();

		// Build Ideas
		//
			//CAIFutureBuildIdeas*  GetAIFutureBuildIdeas(){ return &m_AIFutureBuildIdeas; }
			//
			void  GetTransportTemplatesICanBuild( CEOSAIUnitTemplate* pTransportee, CList< CEOSAIUnitTemplate* >* pUnitTemplateList );

		// UnitTemplateSets ( can build, or currently exists )
		//
			// Instead of "GetFastestMovementRate", I should grab the m_UnitsICanBuildOrHave
			//   and ask what it's fastest movement rate is.
			CEOSAIUnitTemplateSet*  GetUnitsICanBuild(){ return &m_UnitsICanBuild; }
			CEOSAIUnitTemplateSet*  GetUnitsICanBuildOrHave(){ return &m_UnitsICanBuildOrHave; }
			CEOSAIUnitTemplateSet*  GetUnitsIHave(){ return &m_UnitsIHave; }

			//bool IsASubmarineUnitInTheGame( CUnitTemplate* pUnitTemplate );
			//CEOSAIUnitTemplateSet*  GetSubmarineUnitsInTheGame(){ return &m_SubmarineUnitsInTheGame; }

		// Cities
		//
			long  GetNumberOfCities(){ return m_MyCities.GetCount(); }
			long  GetNumberOfCitiesWithImprovement( CString strCityImprovement );
			//float GetPercentageOfCitiesWithCityImprovement( CString strCityImprovement );

		// Resources
		//
			float GetMyLandFoodResources();
			float GetMySeaFoodResources();

		// Airbases
		//
			void  GetAirbases( long iPlayer, CEOSAIAirbasesSet* pAirbasesSet );
			void  GetAirbases( long iPlayer, CEOSAIUnitTemplate* pAirUnit, CEOSAIAirbasesSet* pAirbasesSet );
			void  GetAirbases( long iPlayer, CEOSAIUnitTemplate* pAirUnit, CEOSAILocation AirUnitLocation, float fLimitToDistance, CEOSAIAirbasesSet* pAirbasesSet );

		// Upgrade Locations
		//
			void GetClosestUpgradeLocation( CEOSAIUnit* pAIUnit, CEOSAIPoiObject** pClosestUpgradeLocation, float* pfDistance );

		// Repair Locations
		//
			void GetClosestRepairLocation( CEOSAIUnit* pAIDamagedUnit, CEOSAIPoiObject** pClosestRepairLocation, float* pfDistance );

		// Existing Units by category
		//
			long                     GetNumberOfMyUnits();
			long                     GetNumberOfMyCities();
			CList< CEOSAIPoiObject* >*  GetMyActors(){ return &m_MyActors; } // My units, cities
			CList< CEOSAICity* >*       GetMyCities(){ return &m_MyCities; } // My cities
			CList< CEOSAIUnit* >*  GetMySeaTransports(){ return &m_MySeaTransports; }
			CList< CEOSAIUnit* >*  GetMyAirTransports(){ return &m_MyAirTransports; }
			CList< CEOSAIUnit* >*  GetMyAirUnits(){ return &m_MyAirUnits; }
			CList< CEOSAIUnit* >*  GetMyAircraftCarriers(){ return &m_MyAircraftCarriers; }

		// Idle Units
		//
			long  GetNumberOfMyIdleUnitsAndCities();
			long  GetNumberOfMyIdleCities();
			long  GetNumberOfMyIdleUnits();
			void  GetMyIdleUnitsAndCities( CList< CEOSAIPoiObject* >* pIdleUnitsAndCities );

		// Movement Rates and Ranges
		//
		// returns the fastest movement rate of the transports that can transport that can move any of the Transportees
		//
			float GetFastestTransportMovementRate( CList< CEOSAIUnitTemplate* >* pTransporteeList );
			//float GetFastestTransportMovementRate( CAIThought_AIPoiObjectFilter* pFilter );

			float GetFastestSeaTransportMovementRate(){ return m_UnitsICanBuildOrHave.GetFastestSeaTransportMovementRate(); }
			float GetFastestAirTransportMovementRate(){ return m_UnitsICanBuildOrHave.GetFastestAirTransportMovementRate(); }
			float GetLargestAirTransportMovementRange(){ return m_UnitsICanBuildOrHave.GetLargestAirTransportMovementRange(); }
			float GetFastestAircraftCarrierMovementRate(){ return m_UnitsICanBuildOrHave.GetFastestAircraftCarrierMovementRate(); }

			//float GetLargestRangeOfCurrentAirUnitTemplates();
			float GetMovementLimit_AirUnits( CEOSAIUnitTemplateSet* pUnits );

			//float GetMovementRate_FastestCityHunterMovementRate();
			float GetMovementRate_FastestAirfieldBuilderOrTransport( CEOSAIUnitTemplateSet* pUnits );
			float GetMovementRate_FastestGroundCitResHunterOrTransport( CEOSAIUnitTemplateSet* pUnits );
			float GetMovementRate_FastestSeaResHunters( CEOSAIUnitTemplateSet* pUnits );
			float GetMovementRate_FastestCityDegraders( CEOSAIUnitTemplateSet* pUnits );

			float GetMovementRate_FastestTransport( CEOSAIUnitTemplate* pUnitToTransport, CEOSAIUnitTemplateSet* pUnits );
			float GetMovementRate_FastestSeaTransport( CEOSAIUnitTemplate* pUnitToTransport, CEOSAIUnitTemplateSet* pUnits );

		// "Typical" Movement description
		//
			//bool  GetTypicalAirMovementDescription( CAIThought_AIPoiObjectFilter* pFilter, CMovementDescription* pMovementDescription );
			bool  GetTypicalGroundUnitMovementDescription( CEOSAIMovementDescription* pMovementDescription );
			//bool  GetTypicalGroundMovementDescription( CAIThought_AIPoiObjectFilter* pFilter, CMovementDescription* pMovementDescription );
			//bool  GetTypicalSeaMovementDescription( CAIThought_AIPoiObjectFilter* pFilter, CMovementDescription* pMovementDescription );
			bool  GetTypicalSeaTransportMovementDescription( CEOSAIMovementDescription* pMovementDescription );

		//
			//bool  DoIHaveAnyCityHuntersOrCitiesOnTheGeo( CEOSAILocation Location );

			// There is one map per Transport AIUnitTemplate
			//CList< CEOSAITransportCombinedMap* >*  GetTransportCombinedMultiRegionMaps_old(){ return &m_TransportCombinedMultiRegionMaps_old; }

		//
		// Unit Classifications:
		//   Land: CitRes Hunters, Attack, Defense, Ranged Attack, Airfield-Builder
		//   Sea:  Res Hunters, Transports, Combat, Aircraft Carrier, Submarine
		//   Air:  Strategic Bombers, Tactical Bombers, Fighters, Air Transports
		//     (Missiles are strategic or tactical.)

	private:

	//
	// Player-Specific Information (needs to be reconstructed for each AIPlayer)
	//
		//
			CEOSAIBrain* m_pAIBrain;
		//
		// Units
		//
			CEOSAIUnitTemplateSet  m_AllUnitsInTheGameOrBuildableByMe; // units 
			CEOSAIUnitTemplateSet  m_PotentialEnemyUnits; // units of all the non-teammate players

			CEOSAIUnitTemplateSet  m_UnitsICanBuild;       // MyBuildableUnits
			CEOSAIUnitTemplateSet  m_UnitsIHave;           // MyExistingUnits
			CEOSAIUnitTemplateSet  m_UnitsICanBuildOrHave; // MyBuildableToExistingUnits

			//CEOSAIUnitTemplateSet  m_SubmarineUnitsInTheGame; //

			CList< CEOSAIPoiObject* >  m_MyActors; // My units, cities (not-owned)
			CList< CEOSAICity* >       m_MyCities; // My units, cities (not-owned)

			CList< CEOSAIUnit* >   m_MySeaTransports; // (not-owned)
			CList< CEOSAIUnit* >   m_MyAirTransports; // (not-owned)
			CList< CEOSAIUnit* >   m_MyAirUnits;
			CList< CEOSAIUnit* >   m_MyTacticalBombers;
			CList< CEOSAIUnit* >   m_MyAircraftCarriers; // (not-owned)
		//
		// Build Ideas
		//
			// This is based off of TacticalProjects/Strategies (PlanToDoThisAction) and their needs.
			//   It also takes into account the current units, which ones are being expended, etc
			//CAIFutureBuildIdeas  m_AIFutureBuildIdeas;
		//
		// AI Desires, TacticalProjects, Strategies
		//
			CList< CEOSAIDesire2* >            m_Desires;
			CEOSAIListSort< CEOSAIDesireSpatial* >  m_DesiresSortedByScore; // (not-owned)
			//CList< CAITacticalProject* >    m_TacticalProjects; // owned
			CList< CEOSAITacticalProject2* >   m_TacticalProjects2; // owned

			CEOSAIListSort< CEOSAITacticalProject2* >  m_TacticalProjects2ByInitialEvaluation_NearbyScore;
			CEOSAIListSort< CEOSAITacticalProject2* >  m_TacticalProjects2ByInitialEvaluation_ValueScore;
			CEOSAIListSort< CEOSAITacticalProject2* >  m_TacticalProjects2ByInitialEvaluation_BasicScore;
			CEOSAIListSort< CEOSAITacticalProject2* >  m_TacticalProjects2ByInitialEvaluation_EasyTargets;
			CEOSAIListSort< CEOSAITacticalProject2* >  m_TacticalProjects2ByInitialEvaluation_NoResistance;

			//CEOSAIListSort< CAITacticalProjectStrategy* >  m_SortedBestCaseTacticalProjectStrategies; // not owned
			CEOSAIListSort< CEOSAITacticalProject2Strategy* > m_TacticalProject2StrategiesSortedByBestCaseValue; // not owned

		// Warzone
		//
			CList< CEOSAIWarzoneLocation* >  m_WarzoneLocations;
			//CList< CEOSAITransportCombinedMap* >  m_TransportCombinedMultiRegionMaps_old;

	//
		//
		// Each of my buildable/present units get rated in it's ability
		//   If his units are better than mine, I will know which units to use (even though all are bad)
		//CList< CEOSAIUnitTemplateCombatAndAttritionValues* >  m_CombatAndAttritionValues; // owned

		//CAITacticalProjectUnitCapability  m_NationalCapabilities;

		//CList< CAIThought* >  m_NecessaryThoughtList; // Score: 40-50
		//CList< CAIThought* >  m_ExcellentThoughtList; // Score: 30-40
		//CList< CAIThought* >  m_GoodThoughtList;      // Score: 20-30
		//CList< CAIThought* >  m_DecentThoughtList;    // Score: 10-20
		//CList< CAIThought* >  m_InconvenientThoughtList;  // Score:  0-10
		//CList< CAIThought* >  m_BadThoughtList; // don't do these.  These include ideas like: attacking an ally's city.
};

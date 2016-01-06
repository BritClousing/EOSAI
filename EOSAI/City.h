
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "PoiObject.h"
#include "EOSAIBuildOrder.h"
//#include "EOSAILimitedList.h"
#include "EOSAIRegionPathwayMap.h"
#include "EOSAILongSet.h"
//class CAIProductionTask;
class CEOSAIBrain;
class CCity;
class CAIProductionSymbol;
class CEOSAIBuildOption;
class CEOSAIUnit;
//class CAISymbolProcessor;
class CEOSAIGeo;
class CEOSAIGeoLand;
//class CAIProductionOption;
class CAIThought_ShouldICaptureThatCitRes;
//class CEOSAICityBuildAction;
class CEOSAIRegionPathwayMap;
class CEOSAIBuildingDescription;
class CEOSAITacticalProject2;
class CEOSAIBuildOrder;
class CEOSAIBuildCompletion;
class CEOSAIUnitTemplate;
class CEOSAIBuildingDescription;

// Setup:
//   CList< CBuildOrder*      >*  GetBuildOrders(){ return &m_BuildOrders; }
//   CList< CBuildCompletion* >*  GetBuildCompletionList(){ return &m_PartiallyCompletedList; }


class DLLIMPEXP CEOSAICity : public CEOSAIPoiObject
{
	public:
		enum EnumGoal
		{
			eGoal_AddAnotherBuildItem,
			eGoal_BuildThis
		};

		CEOSAICity();
		virtual ~CEOSAICity();

	//	virtual void        SetServerPoiObject( CPoiObject* pPoiObject );
	//	virtual CPoiObject* GetServerPoiObject(){ return (CPoiObject*) m_pServerCity; }
	//	CCity*              GetServerCity(){ return m_pServerCity; }

		//CString GetName(){ ASSERT( false ); return _T(""); } // name of city
		//CString GetName(){ return _T(""); } // name of city

		float   GetAIValueOfCity( long iAccordingToPlayer ); // based on pAIBrain valuation multipliers, production, strategic value (e.g. as an airbase)

		void    SetSortValue( float f ){ m_fSortValue = f; }
		float   GetSortValue(){ return m_fSortValue; }

		bool   HasPredefinedBuildOrders();//{ return false; }

		void   SetPopulation( float f ){ m_fPopulation = f; }
		float  GetPopulation(){ return m_fPopulation; }
		void   SetProduction( float f ){ m_fProduction = f; }
		float  GetProduction(){ return m_fProduction; }

	// Resources
	//
		virtual void GetResourcesProducedPerTurn(std::map<CString, float>& ResourcesProducedAssumingNoOrders, std::map<CString, float>& ResourcesProducedByCurrentOrders);
		virtual void GetResourcesConsumedPerTurn(std::map<CString, float>& ResourcesConsumedAssumingNoOrders, std::map<CString, float>& ResourcesConsumedByCurrentOrders);

		float GetProductionAndConsumptionPerTurn(CString strResource);

	// AITacticalProject methods
	//
		//float GetBuildTimeAndTimeToTarget( CAITacticalProject* pAITacticalProject, CUnitTemplate* pUnitTemplate );
		//CEOSAIUnitActionIdea* CreateAIUnitActionIdea( CAITacticalProject* pAITacticalProject, CUnitTemplate* pUnitTemplate );

		float GetBuildTimeAndTimeToTarget( CEOSAITacticalProject2* pAITacticalProject2, CEOSAIUnitTemplate* pUnitTemplate );
		CEOSAIUnitActionIdea* CreateAIUnitActionIdea( CEOSAITacticalProject2* pAITacticalProject2, CEOSAIUnitTemplate* pUnitTemplate );

	// Buildings
	//
		float GetBuildingValue( CEOSAIBuildingDescription* pBuildingDesc );
		long  GetBuildingCount( CString strBuilding );
		long  GetBuildingCount( CEOSAIBuildingDescription* pBuildingDesc );
		long  GetBuildingCountIncludingPartiallyCompleted( CString strBuilding );
		long  GetBuildingCountIncludingBuildQueue( CString strBuilding );
		long  GetBuildQueueCount( CString strItemName );

		long  GetBuildingLineCount( CString strBuildingLine );
		long  GetBuildingLineCurrentUpgradeLevel( CString strBuildingLine );

		CList< CEOSAIBuildingDescription* >*  GetBuildingList(){ return &m_BuildingList; } // not owned

	// Danger
	//
		float GetStrategicBomberDanger01();
		float GetGroundSeaDanger01();

	//
		float GetTotalProduction();
		#ifdef THINGS_TO_COMPILE_EVENTUALLY
		I set the GetTotalProduction(...) values to simplistically return GetTotalProduction()
		#endif THINGS_TO_COMPILE_EVENTUALLY
		float GetTotalProduction( bool bIncludeRefugeesInTheCalculation, bool bIncludeNationalResourceEffects ){ return GetTotalProduction(); }
		float GetTotalProduction( bool bIncludeRefugeesInTheCalculation, CEOSAIBuildOption* pBuildOption );
		float GetTotalProduction( bool bIncludeRefugeesInTheCalculation, CEOSAIUnitTemplate* pBuildUnitTemplate );

		// Resource Consumption
		//
			//float GetProductionAndConsumptionPerTurn( CString strResource );

	//	virtual float AmountOfOilNeededOverTimeperiod( float fDeltaTime );
	//	virtual float AmountOfOilNeededOverTimeperiodIgnoreProduction( float fDeltaTime );
	//	virtual float AmountOfFoodNeededOverTimeperiod( float fDeltaTime );
	//	virtual float AmountOfFoodNeededOverTimeperiodIgnoreProduction( float fDeltaTime );

		//virtual long  GetObjectId();

		//void		  AppendBuildAction( CEOSAICityBuildAction* pAICityBuildAction );
		//float         GetCompletionTimeForNewBuildOption( CEOSAIBuildOption* pNewBuildOption );

		virtual bool  CanContain_IgnoreForeignRelations( CEOSAIPoiObject* pAIPoiObject ){ return true; }

	//
		long  GetNumberOfGroundUnitsInside();

		//virtual float GetUnexploredPercent( float fRange );
		void          ActionScheduler_CreateBuildOrder( CEOSAIBuildOption* pAIBuildOption );
		void          ActionScheduler_CreateBuildOrder( CEOSAIUnitTemplate* pAIUnitTemplate );
		//virtual bool ActionScheduler_TurnTasksIntoOrders();

	// Geo
	//
		void  IAmACoastalCity( bool b ){ m_bIAmACoastalCity = b; }
		void  AddAdjacentWaterGeo(  long iWaterGeo ){ m_AdjacentWaterGeos.Add( iWaterGeo ); } // Must be setup ahead of time
		void  SetAdjacentWaterGeos( CEOSAILongSet* pWaterGeos ){ m_AdjacentWaterGeos = *pWaterGeos; } // Must be setup ahead of time
		void  GetAdjacentWaterGeos( CEOSAILongSet* pWaterGeos ){ *pWaterGeos = m_AdjacentWaterGeos; }

	// Containment
	//
		//CList< CEOSAIUnit* >*  GetUnitsInsideMe(){ return &m_UnitsInsideMe; }

	// Production
	//
		void  AppendBuildOrder(CEOSAIBuildOption* pEOSAIBuildOption);
		long  GetNumberOfBuildOrders(){ return m_AIBuildOrders.GetCount(); }
		void  RemoveAllItemsFromBuildQueue();
		//CList< CEOSAIBuildOrder*      >*  GetAIBuildOrders() { return &m_AIBuildOrders; }
		CList< CEOSAIBuildOption*     >*  GetAIBuildOrders() { return &m_AIBuildOrders; }
		CList< CEOSAIBuildCompletion* >*  GetAIBuildCompletionList(){ return &m_PartiallyCompletedList; }

		float GetTimeUntilCompletionOfBuildQueue();

		float GetPartiallyCompletedItems01();
		CEOSAIBuildCompletion* GetPartiallyCompletedItem( CEOSAIBuildOption* pBuildOption );
		CEOSAIBuildCompletion* GetPartiallyCompletedItem( CString strBuildTarget );

		bool  CanBuild( CEOSAIBuildOption* pBuildOption, EnumGoal eGoal, bool bAllowOldTechnologies, CStringList* pTextualReasonsForFailure = NULL, bool* pbRequiresBetterTechnology = NULL, bool* bpShowInBuildList = NULL );

		float GetTimeToBuild( CEOSAIUnitTemplate* pUnitTemplate, bool bIncludePartiallyBuiltItems ); // include partial build info

	// Used by AIFutureBuildIdeas
	//
		void  SetPreferenceForLowCostUnits01( float f ){ m_fPreferenceForLowCostUnits01 = f; }
		float GetPreferenceForLowCostUnits01(){ return m_fPreferenceForLowCostUnits01; }
		void  SetPreferenceForGroundUnits01( float f ){ m_fPreferenceForGroundUnits01 = f; }
		float GetPreferenceForGroundUnits01(){ return m_fPreferenceForGroundUnits01; }
		void  SetPreferenceForAirUnits01( float f ){ m_fPreferenceForAirUnits01 = f; }
		float GetPreferenceForAirUnits01(){ return m_fPreferenceForAirUnits01; }
		void  SetPreferenceForSeaUnits01( float f ){ m_fPreferenceForSeaUnits01 = f; }
		float GetPreferenceForSeaUnits01(){ return m_fPreferenceForSeaUnits01; }

		long  GetSizeOfAdjacentWater(){ return m_iAdjacentWaterSize; }
		void  SetSizeOfAdjacentWater( long iAdjacentWaterSize ){ m_iAdjacentWaterSize = iAdjacentWaterSize; }
		//void  IsOnAMajorBodyOfWater( bool b ){ m_bIsOnAMajorBodyOfWater = b; }
		//bool  IsOnAMajorBodyOfWater(){ return m_bIsOnAMajorBodyOfWater; }

	//
		//void SetCheckpoint( CAISymbolProcessor* Proc );
		//void RestoreToCheckpoint( CAISymbolProcessor* Proc );

		//virtual void AddToCombatGroup( CList< CEOSAIPoiObject* >* pCombatGroupList );

	// Pathfinding
	//
		virtual void        InvokeMultiRegionPathwaysToEverywhere();
		//
		CEOSAIRegionPathwayMap*  InvokeWaterAIRegionMapToEverywhere();
		CEOSAIRegionPathwayMap*  InvokeWaterMultiRegionMapToEverywhere();
		//
		CEOSAIRegionPathwayMap*  InvokeLandAIRegionMapToEverywhere();
		CEOSAIRegionPathwayMap*  InvokeLandAndSeaTransport1AIRegionMapToEverywhere();
		CEOSAIRegionPathwayMap*  InvokeLandAndSeaTransport2AIRegionMapToEverywhere(); // Favor sea-movement
		//
		CEOSAIRegionPathwayMap*  InvokeLandMultiRegionMapToEverywhere();
		CEOSAIRegionPathwayMap*  InvokeLandAndSeaTransport1MultiRegionMapToEverywhere();
		CEOSAIRegionPathwayMap*  InvokeLandAndSeaTransport2MultiRegionMapToEverywhere(); // Favor sea-movement
		//
		CEOSAIRegionPathwayMap*  InvokeAirAIRegionMapToEverywhere(); // Important for danger, expected resistance
		CEOSAIRegionPathwayMap*  InvokeAirMultiRegionMapToEverywhere(); // Important for danger, expected resistance

	//
		long  m_iTempItemsInTaskForceStack;

	protected:

	//	void AddRegion( float fDistance, long iSize );
	//	void AddUnexploredRegion( float fDistance, long iSize );

		// Must link to an actual city - this includes production information
		//CCity*      m_pServerCity;
		//CCity*      m_pPlayerCity;
		//CEOSAIGeo*     m_pAIGeo;
		//CEOSAIGeoLand* m_pAIGeoLand;

		//CList< CEOSAIBuildOrder*    >    m_AIBuildOrders; // A build order was basically just a pointer to a BuildOption
		CList< CEOSAIBuildOption*   >    m_AIBuildOrders;
		CList< CEOSAIBuildCompletion* >  m_PartiallyCompletedList;

		float  m_fPopulation;
		float  m_fProduction;

		bool   m_bIAmACoastalCity;
		CEOSAILongSet  m_AdjacentWaterGeos; // Must be setup ahead of time

		//CList< CEOSAIUnit* >  m_UnitsInsideMe;
		CList< CEOSAIBuildingDescription* >  m_BuildingList; // not owned

		float  m_fStrategicBomberDanger01;
		float  m_fGroundSeaDanger01;

		float  m_fSortValue;
		float  m_fPreferenceForLowCostUnits01;
		float  m_fPreferenceForGroundUnits01;
		float  m_fPreferenceForAirUnits01;
		float  m_fPreferenceForSeaUnits01;
		long   m_iAdjacentWaterSize;
		//bool   m_bIsOnAMajorBodyOfWater;

		//CList< CEOSAICityBuildAction* >  m_CityBuildActions; // owned

		CEOSAIRegionPathwayMap  m_WaterAIRegionMapToEverywhere;
		CEOSAIRegionPathwayMap  m_WaterMultiRegionMapToEverywhere;
		//
		CEOSAIRegionPathwayMap  m_LandAIRegionMapToEverywhere;
		CEOSAIRegionPathwayMap  m_LandAndImaginarySeaTransport1AIRegionMapToEverywhere;
		CEOSAIRegionPathwayMap  m_LandAndImaginarySeaTransport2AIRegionMapToEverywhere;
		//
		CEOSAIRegionPathwayMap  m_LandMultiRegionMapToEverywhere;
		CEOSAIRegionPathwayMap  m_LandAndImaginarySeaTransport1MultiRegionMapToEverywhere;
		CEOSAIRegionPathwayMap  m_LandAndImaginarySeaTransport2MultiRegionMapToEverywhere;
		//
		CEOSAIRegionPathwayMap  m_AirAIRegionMapToEverywhere;
		CEOSAIRegionPathwayMap  m_AirMultiRegionMapToEverywhere;
};



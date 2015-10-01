
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class CEOSAIUnit2;
class CEOSAIUnitTemplate2;
class CEOSAIBrain;
#include "EOSAIStringAndFloatSet.h"
//#include "StringIndexableList.h"
//class CEOSAITaskForceDesc;
class CEOSAICitResValueMultiplier;
class CEOSAIJobsToDo;
//class CUnitTemplateAndFloatSet;

/*
I need to make sure I can create thought-objects that link together.
	e.g. I want to capture a city - what units do I have?  what units can I build?
		what units are in production?  what am I going to build after the current
		build item at city x?  can I switch out the current production in city x
		for this other item that i need more?
*/
#define MAX_TRANSPORT_CAPACITY 10

class DLLIMPEXP CEOSAIUnitTemplate2Set
{
	public:
		enum EnumUnitTemplateFilter
		{
			AirfieldBuildersAndTheirTransports,
			GroundCitResHuntersAndTheirTransports,
			SeaResHunters,
			StrategicBombingAndTheirTransports,
		};

	public:
		CEOSAIUnitTemplate2Set()
		{
			m_pAICitResValueMultiplier = NULL;
			m_bSummaryInformationIsValid = false;

			m_fLowestCostDivGroundCitResHunter = 1000000.0f;
			m_fLowestCostDivSeaResHunter = 1000000.0f;
			m_fLowestCostDivAirfieldBuilder = 1000000.0f;
			//
			for( long i=0; i<MAX_TRANSPORT_CAPACITY+1; i++ ){ m_fLowestCostDivTransportCapacity[i] = 1000000.0f; }
			for( long i=0; i<MAX_TRANSPORT_CAPACITY+1; i++ ){ m_fLowestCostDivCarrierCapacity[i] = 1000000.0f; }

			// What's the lowest Cost / One unit of AntiSoftCombat?  How cheap can I get AntiSoftCombat?
			/*
			m_fLowestCostDivAntiSoftCombat = 1000000.0f;
			m_fLowestCostDivAntiArmorCombat = 1000000.0f;
			m_fLowestCostDivAntiAircraftCombat = 1000000.0f;
			m_fLowestCostDivAntiMissileCombat = 1000000.0f;
			m_fLowestCostDivAntiShipCombat = 1000000.0f;
			m_fLowestCostDivAntiSubmarineCombat = 1000000.0f;

			// What's the lowest ContainerSize / AntiSoftCombat?  How much AntiSoftCombat can
			//   I squeeze into one container slot?  (Used for transport calculations)
			m_fGroundUnits_LowestSizeInContainerDivAntiSoftCombat = 1000000.0f;
			m_fGroundUnits_LowestSizeInContainerDivAntiArmorCombat = 1000000.0f;
			*/
		}
		//
		//
		void  AddUnitTemplate( CEOSAIUnitTemplate2* pAIUnitTemplate );
		void  AddUnitTemplate_Unsafe( CEOSAIUnitTemplate2* pAIUnitTemplate ){ m_List.AddTail( pAIUnitTemplate ); }
		//
		void  Set( CEOSAIUnitTemplate2Set* pOtherSet );
		void  SetToUsefulUnits( CEOSAIJobsToDo& Jobs, CEOSAIUnitTemplate2Set* pOtherSet );
		void  operator=( CEOSAIUnitTemplate2Set& Other ){ Set( &Other ); }
		//
		void  Add( CEOSAIUnitTemplate2Set* pOtherSet );
		void  Add( EnumUnitTemplateFilter eFilter, CEOSAIUnitTemplate2Set* pOtherSet );
		void  AddUsefulUnits( CEOSAIJobsToDo& Jobs, CEOSAIUnitTemplate2Set* pOtherSet );
		//	
		void  OutputSets( EnumUnitTemplateFilter eFilter, CEOSAIUnitTemplate2Set* pOutPrimaryUnits, CEOSAIUnitTemplate2Set* pOutTransports = NULL );
		//void  OutputSets( CEOSAIJobsToDo& Jobs, CList< CEOSAIUnitTemplate2Set* >* pUnitTemplateSets );
		void  GetTransports( CEOSAIUnitTemplate2* pTraveller, CEOSAIUnitTemplate2Set* pOutTransports );
		//void  Filter( CEOSAITaskForceDesc* pSimpleTaskForceDesc );
		//
		//
		void  Clear();

		void  UpdateSummaryInformation();


		//
			CEOSAICitResValueMultiplier*  m_pAICitResValueMultiplier;
			CList< CEOSAIUnitTemplate2* >  m_List;

		//
		// Movement Rate, Range
		//
			float GetFastestUnitMovementRate();

			// Air Units
			float GetFastestAirUnitMovementRate(){ UpdateSummaryInformation(); return m_fFastestAirUnitMovementRate; }
			float GetLargestAirUnitMovementRange(){ UpdateSummaryInformation(); return m_fLargestAirUnitMovementRange; }
			float GetMaxAirUnitHP(){ UpdateSummaryInformation(); return m_fMaximumAirUnitHP; }
			// Ground/Sea Units
			float GetFastestGroundUnitMovementRate(){ UpdateSummaryInformation(); return m_fFastestGroundUnitMovementRate; }
			float GetFastestSeaUnitMovementRate(){ UpdateSummaryInformation(); return m_fFastestSeaUnitMovementRate; }
			float GetMaxGroundUnitHP(){ UpdateSummaryInformation(); return m_fMaximumGroundUnitHP; }

			float GetMaxSeaUnitHP(){ UpdateSummaryInformation(); return m_fMaximumSeaUnitHP; }

			float GetFastestAirfieldBuilderAndTransportMovementRate();
			float GetFastestGroundCitResHunterAndTransportMovementRate();
			float GetFastestSeaResHunterMovementRate();

			// Transports
			float GetFastestTransportMovementRate(){ UpdateSummaryInformation(); return max( m_fFastestSeaTransportMovementRate, m_fFastestAirTransportMovementRate ); }
			float GetFastestSeaTransportMovementRate(){ UpdateSummaryInformation(); return m_fFastestSeaTransportMovementRate; }
			float GetFastestAirTransportMovementRate(){ UpdateSummaryInformation(); return m_fFastestAirTransportMovementRate; }
			float GetLargestAirTransportMovementRange(){ UpdateSummaryInformation(); return m_fLargestAirTransportMovementRange; }
			// Aircraft Carriers
			float GetFastestAircraftCarrierMovementRate(){ UpdateSummaryInformation(); return m_fFastestAircraftCarrierMovementRate; }
		//
		//
			float GetFastestTransportMovementRate( CEOSAIUnitTemplate2* pTransportee );
			float GetFastestSeaTransportMovementRate( CEOSAIUnitTemplate2* pTransportee );

		//
		// Unit Cost / Capability
		//
			float GetLowestCostDivGroundCitResHunter(){ UpdateSummaryInformation(); return m_fLowestCostDivGroundCitResHunter; }
			float GetLowestCostDivSeaResHunter(){ UpdateSummaryInformation(); return m_fLowestCostDivSeaResHunter; }
			float GetLowestCostAirfieldBuilder(){ UpdateSummaryInformation(); return m_fLowestCostDivAirfieldBuilder; }
			//
			float GetLowestCostDivTransportCapacity( long iTransportCapacity ){ UpdateSummaryInformation(); return m_fLowestCostDivTransportCapacity[ min( MAX_TRANSPORT_CAPACITY, iTransportCapacity ) ]; }
			float GetLowestCostDivCarrierCapacity( long iCarrierCapacity ){ UpdateSummaryInformation(); return m_fLowestCostDivCarrierCapacity[ min( MAX_TRANSPORT_CAPACITY, iCarrierCapacity ) ]; }
			/*
			float GetLowestCostDivAntiSoftCombat(){ UpdateSummaryInformation(); return m_fLowestCostDivAntiSoftCombat; }
			float GetLowestCostDivAntiArmorCombat(){ UpdateSummaryInformation(); return m_fLowestCostDivAntiArmorCombat; }
			float GetLowestCostDivAntiAircraftCombat(){ UpdateSummaryInformation(); return m_fLowestCostDivAntiAircraftCombat; }
			float GetLowestCostDivAntiMissileCombat(){ UpdateSummaryInformation(); return m_fLowestCostDivAntiMissileCombat; }
			float GetLowestCostDivAntiShipCombat(){ UpdateSummaryInformation(); return m_fLowestCostDivAntiShipCombat; }
			float GetLowestCostDivAntiSubmarineCombat(){ UpdateSummaryInformation(); return m_fLowestCostDivAntiSubmarineCombat; }
			*/
			// For each UnitTemplate in this set, how much production do I need to spend to 
			//   cause 1 production damage to target?
			//void GetAttritionCostVs( CEOSAIUnitTemplate2* pTarget, CUnitTemplateAndFloatSet* pResults );
		//
			//float GetGroundUnits_LowestSizeInContainerDivAntiSoftCombat(){ UpdateSummaryInformation(); return m_fGroundUnits_LowestSizeInContainerDivAntiSoftCombat; }
			//float GetGroundUnits_LowestSizeInContainerDivAntiArmorCombat(){ UpdateSummaryInformation(); return m_fGroundUnits_LowestSizeInContainerDivAntiArmorCombat; }
		//
		// Unit Cost / ( HP * Defense )
		//   This is not very useful because it also depends on the attack values of enemies
		//   If ship-attack values are twice as high as submarine-attack values, then this will be off
		//
			CEOSAIStringAndFloatSet*  GetHighestHPDefenseDivCost_UnitTypeSet(){ UpdateSummaryInformation(); return &HighestHPDefenseDivCost_UnitTypeSet; }

		//
		// Summary information
		//
			bool  m_bSummaryInformationIsValid;

		//
		// Movement Rate, Range
		//
			float m_fFastestAirUnitMovementRate;
			float m_fLargestAirUnitMovementRange;
			float m_fMaximumAirUnitHP;
			//
			float m_fFastestGroundUnitMovementRate;
			float m_fMaximumGroundUnitHP;
			float m_fFastestSeaUnitMovementRate;
			float m_fMaximumSeaUnitHP;
			//
			float m_fFastestSeaTransportMovementRate;
			float m_fFastestAirTransportMovementRate;
			float m_fLargestAirTransportMovementRange;
			//
			float m_fFastestAircraftCarrierMovementRate;
		//
		// Unit Cost / Capability
		//
			float m_fLowestCostDivGroundCitResHunter;
			float m_fLowestCostDivSeaResHunter;
			float m_fLowestCostDivAirfieldBuilder;
			//
			float m_fLowestCostDivTransportCapacity[MAX_TRANSPORT_CAPACITY+1]; // Cost per Capacity
			float m_fLowestCostDivCarrierCapacity[MAX_TRANSPORT_CAPACITY+1]; // Cost per Capacity
			// Combat value is Unit Cost / ( HP * Defense * AttackToHit * AttackDamage )
			/*
			float m_fLowestCostDivAntiSoftCombat;
			float m_fLowestCostDivAntiArmorCombat;
			float m_fLowestCostDivAntiAircraftCombat;
			float m_fLowestCostDivAntiMissileCombat;
			float m_fLowestCostDivAntiShipCombat;
			float m_fLowestCostDivAntiSubmarineCombat;
		//
			float m_fGroundUnits_LowestSizeInContainerDivAntiSoftCombat;
			float m_fGroundUnits_LowestSizeInContainerDivAntiArmorCombat;
			*/
		//
		// Unit Cost / ( HP * Defense )
		//
			//
			// (HP+Defense) / ProductionCost
			//    Higher values are better - it means more (HP+Defense) for each production spent
			CEOSAIStringAndFloatSet  HighestHPDefenseDivCost_UnitTypeSet; // 
};


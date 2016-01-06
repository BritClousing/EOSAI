
#pragma once

class CEOSAIThoughtDatabase;
class CEOSAIUnitTemplateSet;
class CEOSAIJobCapability;
class CEOSAIPoiObject;
class CEOSAIUnit;
class CEOSAIUnitTemplate;
#include "EOSAIUnitTemplatesAndFloat.h"

class CEOSAIJobsToDo
{
	public:
		CEOSAIJobsToDo(){ Clear(); }

		bool  IsEmpty();
		void  Clear();

		void  Set( CEOSAIJobsToDo* pAIJobsToDo );
		void  operator+=( CEOSAIJobsToDo* pAIJobsToDo ){ Add( pAIJobsToDo ); }
		void  Add( CEOSAIJobsToDo* pAIJobsToDo );
		void  AddEnemyUnit( CEOSAIUnit* pAIUnit );
		//void  Subtract( CEOSAIJobCapability* pAIJobCapability );

		//
		float GetEstimatedNumberOfUnitsNeeded();
		bool  HasCombatTask();

		// General Job: All the category jobs (CitResHunter, etc)
		// NonCombat Tasks: GroundCitResHunter, SeaCitResHunter, AirfieldBuilder
		// Specific Combat Job: Can fight a specific enemy unit in the list

		// Used by individual desires to calculate desire-fulfillment
		//
			// Returns the lowest Fulfillment/Job ratio of all tasks
			// If any tasks are unfulfilled, MinimumFulfillment01 will return 0.0
			float GetMinimumFulfillment01( CEOSAIJobCapability* pCapability );

		// Used by TaskForce scoring
		//
			float GetMinimumNonCombatFulfillment02( CEOSAIJobCapability* pCapability );
			// Gives points for increased fulfillment of any tasks
			float GetAverageNonCombatFulfillment01( CEOSAIJobCapability* pCapability );

		bool  CanFulfillAJob( CEOSAIPoiObject* pActor );
		bool  CanFulfillAJob( CEOSAIUnitTemplate* pActor );
		bool  CanFulfillASpecificCombatJob( CEOSAIUnitTemplate* pMyUnitTemplate );
		//bool  CanFulfillANonCombatJob( CUnitTemplate* pActor );
		//bool  CanFulfillANonCombatJob( CEOSAIJobCapability* pCapability );
		//bool  CanFulfillANonCombatJob( CEOSAIJobCapability* pExistingCapability, CUnitTemplate* pAdditionalActor );
		bool  CanFulfillAGeneralJob( CEOSAIUnitTemplate* pActor );
		bool  CanFulfillAGeneralJob( CEOSAIJobCapability* pCapability );
		//bool  CanFulfillAGeneralJob( CEOSAIJobCapability* pExistingCapability, CUnitTemplate* pAdditionalActor );

		void  MultiplyExpandJobSignificance( float f );
		void  MultiplyConquestJobSignificance( float f );

		// Primary Jobs
		//
			//long  m_iDegradeCity;
			//long  m_iAirfieldBuilder;

			// Exploration, Sighting
			//
				float m_fExploration;
				float m_fSighting;

			// Degrade Cities
			//
				float  m_fKillPopulation;
				float  m_fDestroyBuildings;
				float  m_fInProductionDamage;

			// Aircraft
			//
				//long   m_iDegradeCity;
				float  m_fAirEscort;
				float  m_fAntiAir;  // anti-aircaft guns, fighters, SAM sites

			// Submarines
			//
				float  m_fSubmarineCapability;  // strength, speed, stealth of submarines
				float  m_fAntiSubmarine;

			// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
			//
				float  m_fGroundCitResHunter;
				float  m_fGroundAttackUnits;  // units that can attack ground units (ground units, aircraft, ..)
				float  m_fGroundDefenseUnits;
				float  m_fInvasionDefense;    // includes aircraft, coastal ships to attack transports, ...
				long   m_iAirfieldBuilder;

			// Sea
			//
				float  m_fSeaResHunter;
				float  m_fSeaSupremacy;  // ship-to-ship combat
				float  m_fShoreBombardment;

		// Specific Units to attack
		//
			CEOSAIUnitTemplatesAndFloat  m_UnitTemplatesAndFloat;
			CList< CEOSAIUnit* >  m_EnemyUnits;
};

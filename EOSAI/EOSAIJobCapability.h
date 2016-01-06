
#pragma once

//
// Actions have JobsToDo.  The 'AIJobCapability' describes the unit's possible role(s) in an action.
//   Just because a unit can do a job, doesn't mean it should - thus, a unit's role is different
//   from its capability.  For example, a transport can be involved in combat, but a better
//   idea would be to use it for transport and avoid the combat role.
//

#include "EOSAIUnitTemplatesAndFloat.h"
class CEOSAIUnitTemplateAndFloat;
class CEOSAIUnitTemplate;
class CEOSAIJobRole;

class CEOSAIJobCapability
{
	public:
		CEOSAIJobCapability(){ Clear(); }
		~CEOSAIJobCapability(){ Clear(); }

		//
		void  Clear();

		void  Add( CEOSAIJobCapability* pAIJobCapability );
		void  Remove( CEOSAIJobCapability* pAIJobCapability );
		void  Add( CEOSAIUnitTemplate* pAIUnitTemplate );//, CEOSAIJobRole* pJobRole = NULL );
		void  Remove( CEOSAIUnitTemplate* pAIUnitTemplate );//, CEOSAIJobRole* pJobRole = NULL );

		void  operator=( CEOSAIJobCapability& AIJobsToDo ){ Set( &AIJobsToDo ); }
		void  Set( CEOSAIJobCapability* pAIJobCapability );
		void  Set( CEOSAIUnitTemplate* pUnitTemplate );//, CEOSAIJobRole* pJobRole = NULL );

		//float GetCapabilityAgainst( CUnitTemplate* pUnitTemplate );
		//float GetAttritionValueAgainst( CUnitTemplate* pUnitTemplate );

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

		// Transports/Carriers
		//
			float  m_fTransportCapability;
			float  m_fCarrierCapability;

		// Primary Jobs (non-consuming; does not consume the units, so they can be reused)
		//long  m_iGroundCitResHunter;
		//long  m_iSeaResHunter;
		//long  m_iDegradeCity; // may or may not consume the unit
		//long  m_iAirfieldBuilder;

		// Secondary Jobs
		//long  m_iTransport;
		//long  m_iCarrier;


		// Generalized Combat (generalized combat against unknown threats - like protect CitRes)
		//   Contents will be things like "Ship" (combat vs. ships), "Aircraft" (combat vs. aircraft), etc
		//CStringList  m_CombatVs; // just an idea for defending against potential attacks
		//CList< EOSAI::StringAndFloat* >  m_CombatVs;
		//CList< CUnitTemplateAndFloat* >  m_AttritionValue01Versus; // attrition or chances of defeat? both are important
		//CEOSAIBCArray< CUnitTemplateAndFloat* >  m_AttritionValue01Versus;

		CEOSAIUnitTemplatesAndFloat  m_UnitTemplatesAndFloat;
		CList< CEOSAIUnitTemplate* >   m_UnitTemplatesForCombat;
};

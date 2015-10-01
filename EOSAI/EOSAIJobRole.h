
#pragma once

//
// Actions have JobsToDo.  The 'AIJobRole' describes the unit's assigned role(s) in an action.
//   Just because a unit can do a job, doesn't mean it should - thus, a unit's role is different
//   from its capability.  For example, a transport can be involved in combat, but a better
//   idea would be to use it for transport and avoid the combat role.
//
// This is supposed to act as a filter on the AIJobCapabilities
//

class CUnitTemplateAndFloat;

class CEOSAIJobRole
{
	public:
		CEOSAIJobRole()
		{
			// Primary Jobs
			//m_bGroundCitResHunter = false;
			//m_bSeaResHunter = false;
			//m_bDegradeCity = false;
			//m_bAirfieldBuilder = false;

			// Secondary Jobs
			//m_bTransport = false;
			//m_bCarrier = false;

			m_bCombat = false;
		}
		~CEOSAIJobRole()
		{
			//while( m_CombatVs.IsEmpty() == FALSE ){ delete m_CombatVs.RemoveHead(); }
		}

		// I commented out most of these roles, because I don't know why I wouldn't use a unit
		//   that is capable of GroundCitResHunter to actually do GroundCitResHunter.
		// The only questionable role is combat. Transport and carriers are capable, but I won't want
		//   to use that role in most cases.

		// Primary Jobs
		//bool  m_bGroundCitResHunter;
		//bool  m_bSeaResHunter;
		//bool  m_bDegradeCity;
		//bool  m_bAirfieldBuilder;

		// Secondary Jobs
		//bool  m_bTransport;
		//bool  m_bCarrier;

		// Set to true if I intend to use it for combat - 
		//   transports might be used for combat, but don't include them
		bool  m_bCombat;

		// Generalized Combat (generalized combat against unknown threats - like protect CitRes)
		//   Contents will be things like "Ship" (combat vs. ships), "Aircraft" (combat vs. aircraft), etc
		//CList< CUnitTemplateAndFloat* >  m_CombatValue01Versus; // attrition or chances of defeat? both are important
		//CList< CUnitTemplate* >  m_IntendedTargets; // attrition or chances of defeat? both are important

		// Specific Units to attack
		//CList< CUnit* >  m_EnemyUnits;
};


#pragma once
/*
#include "AIDesire.h"
#include "AIGeoLand.h"
class CAIPlan;

// Invade Geo involves one situation:
//   Another player controls part or all of a geo.
//   I am going to invade it.
//   This may involve several different objectives:
//      Invade and take complete control of the geo
//      Invade and capture just the unowned CitRes (don't ignite a war, but raise tensions)
//      Invade and capture all of one player's CitRes (don't attack other players)

class CAIDesire_InvadeGeo : public CAIDesire
{
	public:

		//
			CAIDesire_InvadeGeo( CAIPlan* pAIPlan, CEOSAIGeoLand* pAIGeoLand );

		//
			virtual float CalculateScore( CEOSAIPoiObject* pActor1, float fDesireSatisfiedTime );
			virtual float CalculateCombatBalance( CEOSAIPoiObject* pActor1, float fDesireSatisfiedTime ){ return 0.0f; }
			virtual void  CreateWeaklyDefinedActions( CEOSAIPoiObject* pMyPoiObject = NULL ){}

		//
		//
		//
			CEOSAIGeoLand* m_pAIGeoLand;

			//
			float  m_fDangerDuringTransportPhase; // if this is deep in enemy territory, my transports probably wont survive

			// do I need more transports?
			float  m_fEstimateOfNumberOfUnitsInInvasionForce;

			// Assumes I concentrate exclusively on invading this geo (and no others)
			float  m_fMyCapabilityToInvadeSuccessfully; 
};
*/

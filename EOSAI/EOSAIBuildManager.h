
#pragma once

namespace EOSAI{ class AIPlayer; }
//class CAIBuildNeed;
class CEOSAIUnitTemplate;
class CEOSAIBuildOption;
#include "EOSAILocation.h"

/*
Instead of having units 'allocate' a city to build a transport, I should simply create a 'build need' item.
This 'build need' will contain a UnitTemplate, a perfered area, force somebody to build that unit.  
(Maybe I could include a priority.)  As items get added to the 'build need' list, the AI would keep 
track of a 'tension' value -- too many build-needs is bad because they can't be fulfilled quickly.  
At the end, these 'build needs' would then be evaluated and scheduled as city build-orders.
The 'build-need' items can also take danger into account, so that cities on the front aren't 
building long-term units (like a battleship when I should build an infantry).  This will probably 
require that I setup some city-build orders early - so that the 'tension' is set correctly.  
(Example: if all my cities are on the front, they should ALL build ground-units.)

The Build-Need system is used to solve a complex problem: Where should I build units?
Influences:
  - Where are my buildings (shipyards, barracks) that give production bonuses to certain unit-types
  - Which cities are in danger (and, therefore, should not build long-term targets, but defensive units)
  - Which cities are vulnerable to air-attacks?
  - What's the iron cost (and availability?)

The BuildNeed system can also decide to build city-improvements.
  This calculation is based on a number of factors, including city danger (bombers, imminent capture, etc)
*/

namespace EOSAI
{

class CAIBuildManager
{
	public:
		CAIBuildManager();
		~CAIBuildManager();
		void SetAIPlayer( EOSAI::AIPlayer* pAIPlayer );

		void Clear();

		// If I need more warships or something, this is the place to add it
		void  AddNationalNeed_NavalPower( float fPriority ){}
		void  AddNationalNeed_Submarines( float fPriority ){}
		void  AddNationalNeed_Transports( float fPriority ){}
		void  AddNationalNeed_GroundUnits( float fPriority ){}
		void  AddNationalNeed_AirPower( float fPriority ){}

		// PreferedLocation is important when dealing with large maps (e.g. Pacific vs Atlantic Ocean)
		void  AddBuildNeed( float fPriority01, CEOSAIUnitTemplate* pUnitTemplate, CEOSAILocation PreferedLocation );

		// Tension gets higher as more build items are added, and as the number of available cities drops
		//   Tension value of 1 means no new build needs can be added (unless I bump something)
		float GetCurrentTension01(){ return 0.0f; }

		// FirstPass = before any orders are setup = everything more than 1/2 done + needed stuff
		//   FirstPass helps to establish tension, which is used later when setting up orders
		void  CreateBuildOrders_FirstPass();
		void  CreateBuildOrders_Final();

	private:
		EOSAI::AIPlayer*  m_pAIPlayer;
		//CList< CAIBuildNeed* >   m_BuildNeeds;
		long  m_iNumberOfAvailableCities; // cities that don't have build orders yet

		//CList< CEOSAIBuildOption* >  m_AIBuildOptions;
};

};

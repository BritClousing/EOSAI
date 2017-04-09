
#include <stdafx.h>
#include "EOSAIJobsToDo.h"
#include "EOSAIJobCapability.h"
#include "EOSAIUnitTemplateSet.h"
//#include "UnitTemplate.h"
#include "EOSAIThoughtDatabase.h"
#include "City.h"
#include "EOSAIUnit.h"
#include "EOSAIQuickCombatCalculation.h"
#include "EOSAICitResValueMultiplier.h"
//#include "AICombatPlanner.h"
#include "EOSAIUnitTemplatesAndFloat.h"
#include "EOSAIUnitVsUnitValue.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

bool  CEOSAIJobsToDo::IsEmpty()
{
	// Exploration, Sighting
	if( m_fExploration > 0.0 ) return false;
	if( m_fSighting > 0.0 ) return false;

	// Degrade Cities
	if( m_fKillPopulation > 0.0 ) return false;
	if( m_fDestroyBuildings > 0.0 ) return false;
	if( m_fInProductionDamage > 0.0 ) return false;

	// Aircraft
	if( m_fAirEscort > 0.0 ) return false;
	if( m_fAntiAir > 0.0 ) return false;

	// Submarines
	if( m_fSubmarineCapability > 0.0 ) return false;
	if( m_fAntiSubmarine > 0.0 ) return false;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	if( m_fGroundCitResHunter > 0.0 ) return false;
	if( m_fGroundAttackUnits > 0.0 ) return false;
	if( m_fGroundDefenseUnits > 0.0 ) return false;
	if( m_fInvasionDefense > 0.0 ) return false;
	if( m_iAirfieldBuilder > 0 ) return false;

	// Sea
	if( m_fSeaResHunter > 0.0 ) return false;
	if( m_fSeaSupremacy > 0.0 ) return false;
	if( m_fShoreBombardment > 0.0 ) return false;

	// Specific Units to attack
	if( m_EnemyUnits.IsEmpty() == FALSE ) return false;

	return true;
}

void  CEOSAIJobsToDo::Clear()
{
	// Exploration, Sighting
	m_fExploration = 0.0f;
	m_fSighting = 0.0f;

	// Degrade Cities
	m_fKillPopulation = 0.0f;
	m_fDestroyBuildings = 0.0f;
	m_fInProductionDamage = 0.0f;

	// Aircraft
	m_fAirEscort = 0.0f;
	m_fAntiAir = 0.0f;  // anti-aircaft guns, fighters, SAM sites

	// Submarines
	m_fSubmarineCapability = 0.0f;  // strength, speed, stealth of submarines
	m_fAntiSubmarine = 0.0f;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	m_fGroundCitResHunter = 0.0f;
	m_fGroundAttackUnits = 0.0f;  // units that can attack ground units (ground units, aircraft, ..)
	m_fGroundDefenseUnits = 0.0f;
	m_fInvasionDefense = 0.0f;    // includes aircraft, coastal ships to attack transports, ...
	m_iAirfieldBuilder = 0;

	// Sea
	m_fSeaResHunter = 0.0f;
	m_fSeaSupremacy = 0.0f;  // ship-to-ship combat
	m_fShoreBombardment = 0.0f;

	// Specific Units to attack
	m_EnemyUnits.RemoveAll();
}

void  CEOSAIJobsToDo::Set( CEOSAIJobsToDo* pAIJobsToDo )
{
	// Exploration, Sighting
	m_fExploration = pAIJobsToDo->m_fExploration;
	m_fSighting = pAIJobsToDo->m_fSighting;

	// Degrade Cities
	m_fKillPopulation = pAIJobsToDo->m_fKillPopulation;
	m_fDestroyBuildings = pAIJobsToDo->m_fDestroyBuildings;
	m_fInProductionDamage = pAIJobsToDo->m_fInProductionDamage;

	// Aircraft
	m_fAirEscort = pAIJobsToDo->m_fAirEscort;
	m_fAntiAir = pAIJobsToDo->m_fAntiAir;  // anti-aircaft guns, fighters, SAM sites

	// Submarines
	m_fSubmarineCapability = pAIJobsToDo->m_fSubmarineCapability;  // strength, speed, stealth of submarines
	m_fAntiSubmarine = pAIJobsToDo->m_fAntiSubmarine;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	m_fGroundCitResHunter = pAIJobsToDo->m_fGroundCitResHunter;
	m_fGroundAttackUnits = pAIJobsToDo->m_fGroundAttackUnits;  // units that can attack ground units (ground units, aircraft, ..)
	m_fGroundDefenseUnits = pAIJobsToDo->m_fGroundDefenseUnits;
	m_fInvasionDefense = pAIJobsToDo->m_fInvasionDefense;    // includes aircraft, coastal ships to attack transports, ...
	m_iAirfieldBuilder = pAIJobsToDo->m_iAirfieldBuilder;

	// Sea
	m_fSeaResHunter = pAIJobsToDo->m_fSeaResHunter;
	m_fSeaSupremacy = pAIJobsToDo->m_fSeaSupremacy;  // ship-to-ship combat
	m_fShoreBombardment = pAIJobsToDo->m_fShoreBombardment;

	// Units
	m_EnemyUnits.RemoveAll();
	POSITION pos = pAIJobsToDo->m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pUnit = pAIJobsToDo->m_EnemyUnits.GetNext( pos );
		m_EnemyUnits.AddTail( pUnit );
	}
}

void  CEOSAIJobsToDo::Add( CEOSAIJobsToDo* pAIJobsToDo )
{
	// Primary Jobs
	/*
	m_iGroundCitResHunter += pAIJobsToDo->m_iGroundCitResHunter;
	m_iSeaResHunter += pAIJobsToDo->m_iSeaResHunter;
	m_iDegradeCity += pAIJobsToDo->m_iDegradeCity;
	m_iAirfieldBuilder += pAIJobsToDo->m_iAirfieldBuilder;
	*/
	// Exploration, Sighting
	m_fExploration += pAIJobsToDo->m_fExploration;
	m_fSighting += pAIJobsToDo->m_fSighting;

	// Degrade Cities
	m_fKillPopulation += pAIJobsToDo->m_fKillPopulation;
	m_fDestroyBuildings += pAIJobsToDo->m_fDestroyBuildings;
	m_fInProductionDamage += pAIJobsToDo->m_fInProductionDamage;

	// Aircraft
	m_fAirEscort += pAIJobsToDo->m_fAirEscort;
	m_fAntiAir += pAIJobsToDo->m_fAntiAir;  // anti-aircaft guns, fighters, SAM sites

	// Submarines
	m_fSubmarineCapability += pAIJobsToDo->m_fSubmarineCapability;  // strength, speed, stealth of submarines
	m_fAntiSubmarine += pAIJobsToDo->m_fAntiSubmarine;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	m_fGroundCitResHunter += pAIJobsToDo->m_fGroundCitResHunter;
	m_fGroundAttackUnits += pAIJobsToDo->m_fGroundAttackUnits;  // units that can attack ground units (ground units, aircraft, ..)
	m_fGroundDefenseUnits += pAIJobsToDo->m_fGroundDefenseUnits;
	m_fInvasionDefense += pAIJobsToDo->m_fInvasionDefense;    // includes aircraft, coastal ships to attack transports, ...
	m_iAirfieldBuilder += pAIJobsToDo->m_iAirfieldBuilder;

	// Sea
	m_fSeaResHunter += pAIJobsToDo->m_fSeaResHunter;
	m_fSeaSupremacy += pAIJobsToDo->m_fSeaSupremacy;  // ship-to-ship combat
	m_fShoreBombardment += pAIJobsToDo->m_fShoreBombardment;

	// Units - don't add a unit if it already exists in the list
	POSITION pos = pAIJobsToDo->m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pAIUnit = pAIJobsToDo->m_EnemyUnits.GetNext( pos );

		bool bExistsInList = false;
		POSITION pos2 = m_EnemyUnits.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnit* pAIUnitAlreadyInList = m_EnemyUnits.GetNext( pos2 );
			if( pAIUnitAlreadyInList == pAIUnit )
			{
				bExistsInList = true;
				break;
			}
		}
		if( bExistsInList == false )
		{
			m_EnemyUnits.AddTail( pAIUnit );
			m_UnitTemplatesAndFloat.Add( pAIUnit->GetAIUnitTemplate(), 1.0f );
		}
	}
}

void  CEOSAIJobsToDo::AddEnemyUnit( CEOSAIUnit* pAIUnit )
{
	m_EnemyUnits.AddTail( pAIUnit );
	m_UnitTemplatesAndFloat.Add( pAIUnit->GetAIUnitTemplate(), 1.0f );
}

/*
void  CEOSAIJobsToDo::Subtract( CEOSAIJobCapability* pAIJobCapability )
{
	// Exploration, Sighting
	m_fExploration -= pAIJobsToDo->m_fExploration;
	m_fSighting -= pAIJobsToDo->m_fSighting;

	// Degrade Cities
	m_fKillPopulation -= pAIJobsToDo->m_fKillPopulation;
	m_fDestroyBuildings -= pAIJobsToDo->m_fDestroyBuildings;

	// Aircraft
	m_fAirEscort -= pAIJobsToDo->m_fAirEscort;
	m_fAntiAir -= pAIJobsToDo->m_fAntiAir;  // anti-aircaft guns, fighters, SAM sites

	// Submarines
	m_fSubmarineCapability -= pAIJobsToDo->m_fSubmarineCapability;  // strength, speed, stealth of submarines
	m_fAntiSubmarine -= pAIJobsToDo->m_fAntiSubmarine;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	m_iGroundCitResHunter -= pAIJobsToDo->m_iGroundCitResHunter;
	m_fGroundAttackUnits -= pAIJobsToDo->m_fGroundAttackUnits;  // units that can attack ground units (ground units, aircraft, ..)
	m_fGroundDefenseUnits -= pAIJobsToDo->m_fGroundDefenseUnits;
	m_fInvasionDefense -= pAIJobsToDo->m_fInvasionDefense;    // includes aircraft, coastal ships to attack transports, ...
	m_iAirfieldBuilder -= pAIJobsToDo->m_iAirfieldBuilder;

	// Sea
	m_iSeaResHunter -= pAIJobsToDo->m_iSeaResHunter;
	m_fSeaSupremacy -= pAIJobsToDo->m_fSeaSupremacy;  // ship-to-ship combat
	m_fShoreBombardment -= pAIJobsToDo->m_fShoreBombardment;

	// Units - don't add a unit if it already exists in the list
	POSITION pos = pAIJobsToDo->m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CUnit* pUnit = pAIJobsToDo->m_EnemyUnits.GetNext( pos );

		bool bExistsInList = false;
		POSITION pos2 = m_EnemyUnits.GetHeadPosition();
		while( pos2 )
		{
			CUnit* pUnitAlreadyInList = m_EnemyUnits.GetNext( pos2 );
			if( pUnitAlreadyInList == pUnit )
			{
				bExistsInList = true;
				break;
			}
		}
		if( bExistsInList == false )
		{
			m_EnemyUnits.AddTail( pUnit );
		}
	}
}
*/
/*
long  CEOSAIJobsToDo::GetNumberOfActiveJobs()
{
	long iReturn = 0;
	// Primary Jobs
	if( m_iGroundCitResHunter > 0 ) iReturn++;
	if( m_iSeaResHunter > 0 ) iReturn++;
	if( m_iDegradeCity > 0 ) iReturn++;
	if( m_iAirfieldBuilder > 0 ) iReturn++;
	// Secondary Jobs
	//if( m_iTransport > 0 ) iReturn++;
	//if( m_iCarrier > 0 ) iReturn++;
	//
	if( HasCombatTask() ) iReturn++;
	//if( m_UnitAppearanceLikelihood01.IsEmpty() == FALSE ||
	//	m_EnemyUnits.IsEmpty() == FALSE ) iReturn++;
	//
	return iReturn;
}
*/
float CEOSAIJobsToDo::GetEstimatedNumberOfUnitsNeeded()
{
	if( IsEmpty() ) return 0.0f;

	float fUnits = max( 1.0f, sqrt( m_fGroundCitResHunter + m_fSeaResHunter + m_fDestroyBuildings + m_iAirfieldBuilder ) );
	//
	long iNumberOfEnemyUnits = m_EnemyUnits.GetCount();
	if( iNumberOfEnemyUnits > 0 )
	{
		fUnits += (iNumberOfEnemyUnits+0.5f)/1.5f;
	}

	return fUnits;
}

bool  CEOSAIJobsToDo::HasCombatTask()
{
	return ( m_EnemyUnits.IsEmpty() == FALSE );
}

float CEOSAIJobsToDo::GetMinimumFulfillment01( CEOSAIJobCapability* pCapability )
{
	// Primary Jobs
	float fMinimumPercent = GetMinimumNonCombatFulfillment02( pCapability );
	if( fMinimumPercent == 0.0f ) return 0.0;

	// Combat
	if( m_EnemyUnits.IsEmpty() == FALSE )
	{
		CEOSAIQuickCombatCalculation  CombatCalc;
		POSITION pos = m_EnemyUnits.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit* pEnemyUnit = m_EnemyUnits.GetNext( pos );
			CombatCalc.AddToDefenders( pEnemyUnit );
		}
		pos = pCapability->m_UnitTemplatesForCombat.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pMyAIUnit = pCapability->m_UnitTemplatesForCombat.GetNext( pos );
			CombatCalc.AddToAttackers( pMyAIUnit );
		}
		CombatCalc.RunCalculation();
		float fCombatFulfillment = CombatCalc.GetAttackerSuccessValue02();
		fMinimumPercent = min( fMinimumPercent, fCombatFulfillment );
	}

	return min( 1.0f, fMinimumPercent );
}

float CEOSAIJobsToDo::GetMinimumNonCombatFulfillment02( CEOSAIJobCapability* pCapability )
{
	float fMinimumPercent = 2.0f;
	float fPercent;
	if( m_fGroundCitResHunter > 0.0 )
	{
		// I don't need one CitRes hunter for each CitRes.
		//   However, I want more than one CitRes hunter in several situations:
		//   (1) The enemy will try to destroy my CitResHunter
		//   (2) I want to make sure I can capture at least one CitRes from militia (which I can build-on)
		//   (3) I want to capture the CitRes quickly (they are spread out)
		//fPercent = 0.0f;
		//if( pCapability->m_fGroundCitResHunter > 0.0 ) fPercent = 1.0f;
		fPercent = min( 2.0f, pCapability->m_fGroundCitResHunter );
		fMinimumPercent = min( fMinimumPercent, fPercent );
	}
	if( m_fSeaResHunter > 0.0 )
	{
		// I don't need one CitRes hunter for each CitRes.
		//   However, I want more than one CitRes hunter in several situations:
		//   (1) The enemy will try to destroy my CitResHunter
		//   (2) I want to make sure I can capture at least one CitRes from militia (which I can build-on)
		//   (3) I want to capture the CitRes quickly (they are spread out)
		//fPercent = 0.0f;
		//if( pCapability->m_fSeaResHunter > 0.0 ) fPercent = 1.0f;
		fPercent = min( 2.0f, pCapability->m_fSeaResHunter );
		fMinimumPercent = min( fMinimumPercent, fPercent );
	}
	/*
	if( m_fDestroyBuildings > 0 )
	{
		fPercent = (float) pCapability->m_fDestroyBuildings / (float) m_fDestroyBuildings;
		fMinimumPercent = min( fMinimumPercent, fPercent );
	}
	if( m_fInProductionDamage > 0 )
	{
		fPercent = (float) pCapability->m_fInProductionDamage / (float) m_fInProductionDamage;
		fMinimumPercent = min( fMinimumPercent, fPercent );
	}
	*/
	if( m_iAirfieldBuilder > 0 )
	{
		// I don't need one AirfieldBuilder for each Airfield.  (I can reuse the same unit)
		//   However, I want more than one in several situations:
		//   (1) The enemy will try to destroy my AirfieldBuilder
		//   (2) I want to build all the airfields quickly
		//fPercent = 0.0f;
		//if( pCapability->m_iAirfieldBuilder > 0 ) fPercent = 1.0f;
		fPercent = min( 2.0f, pCapability->m_iAirfieldBuilder );
		fMinimumPercent = min( fMinimumPercent, fPercent );
	}

	return fMinimumPercent;
}

float CEOSAIJobsToDo::GetAverageNonCombatFulfillment01( CEOSAIJobCapability* pCapability )
{
	// Primary Jobs
	long iJobs = 0;
	float fCombinedPercent = 0.0f;
	float fPercent;
	if( m_fGroundCitResHunter > 0.0 )
	{
		fPercent = 0.0f;
		if( pCapability->m_fGroundCitResHunter > 0 ) fPercent = 1.0f;
		//fPercent = (float) pCapability->m_iGroundCitResHunter / (float) m_iGroundCitResHunter;
		fCombinedPercent += fPercent;
		iJobs++;
	}
	if( m_fSeaResHunter > 0.0 )
	{
		fPercent = 0.0f;
		if( pCapability->m_fSeaResHunter > 0 ) fPercent = 1.0f;
		//fPercent = (float) pCapability->m_iSeaResHunter / (float) m_iSeaResHunter;
		fCombinedPercent += fPercent;
		iJobs++;
	}
	/*
	if( m_fDestroyBuildings > 0.0 )
	{
		fPercent = (float) pCapability->m_fDestroyBuildings / (float) m_fDestroyBuildings;
		fCombinedPercent += fPercent;
		iJobs++;
	}
	if( m_fInProductionDamage > 0.0 )
	{
		fPercent = (float) pCapability->m_fInProductionDamage / (float) m_fInProductionDamage;
		fCombinedPercent += fPercent;
		iJobs++;
	}
	*/
	if( m_iAirfieldBuilder > 0 )
	{
		fPercent = 0.0f;
		if( pCapability->m_iAirfieldBuilder > 0 ) fPercent = 1.0f;
		//fPercent = (float) pCapability->m_iAirfieldBuilder / (float) m_iAirfieldBuilder;
		fCombinedPercent += fPercent;
		iJobs++;
	}

	if( iJobs == 0 ) return 1.0f;
	return fCombinedPercent / iJobs;
}

bool CEOSAIJobsToDo::CanFulfillAJob( CEOSAIPoiObject* pActor )
{
	CEOSAICity* pActorCity = dynamic_cast< CEOSAICity* >( pActor );
	if( pActorCity ){ return true; }

	CEOSAIUnit* pActorUnit = dynamic_cast< CEOSAIUnit* >( pActor );
	if( pActorUnit ){ return CanFulfillAJob( pActorUnit->GetAIUnitTemplate() ); }

	return false;
}

bool CEOSAIJobsToDo::CanFulfillAJob( CEOSAIUnitTemplate* pActor )
{
	return CanFulfillAGeneralJob( pActor ) || CanFulfillASpecificCombatJob( pActor );
}

bool CEOSAIJobsToDo::CanFulfillASpecificCombatJob( CEOSAIUnitTemplate* pMyUnitTemplate )
{
	if( pMyUnitTemplate->GetAIUnitCombatCapability()->IsACombatUnit() == false ) return false;

	POSITION pos = m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit* pEnemy = m_EnemyUnits.GetNext( pos );
		if( pMyUnitTemplate->GetAIUnitCombatCapability()->InvokeCombatValue( pEnemy->GetAIUnitTemplate() )->IsUsefulInCombat() )
		{
			return true;
		}
	}
	return false;
}

bool CEOSAIJobsToDo::CanFulfillAGeneralJob( CEOSAIUnitTemplate* pMyUnit )
{
	CEOSAIJobCapability Capability;
	Capability.Set( pMyUnit );
	return CanFulfillAGeneralJob( &Capability );
}

bool CEOSAIJobsToDo::CanFulfillAGeneralJob( CEOSAIJobCapability* pCapability )
{
	/*
	if( m_iGroundCitResHunter > 0 && pCapability->m_iGroundCitResHunter > 0 ) return true;
	if( m_iSeaResHunter       > 0 && pCapability->m_iSeaResHunter > 0 ) return true;
	if( m_iDegradeCity        > 0 && pCapability->m_iDegradeCity > 0 ) return true;
	if( m_iAirfieldBuilder    > 0 && pCapability->m_iAirfieldBuilder > 0 ) return true;
	return false;
	*/
	// Exploration, Sighting
	if( m_fExploration > 0.0f      && pCapability->m_fExploration > 0.0f ) return true;
	if( m_fSighting > 0.0f         && pCapability->m_fSighting > 0.0f ) return true;

	// Degrade Cities
	if( m_fKillPopulation > 0.0f   && pCapability->m_fKillPopulation > 0.0f ) return true;
	if( m_fDestroyBuildings > 0.0f && pCapability->m_fDestroyBuildings > 0.0f ) return true;
	if( m_fInProductionDamage > 0.0f && pCapability->m_fInProductionDamage > 0.0f ) return true;

	// Aircraft
	if( m_fAirEscort > 0.0f        && pCapability->m_fAirEscort > 0.0f ) return true;
	if( m_fAntiAir > 0.0f          && pCapability->m_fAntiAir > 0.0f ) return true;

	// Submarines
	if( m_fSubmarineCapability > 0.0f && pCapability->m_fSubmarineCapability > 0.0f ) return true;
	if( m_fAntiSubmarine > 0.0f       && pCapability->m_fAntiSubmarine > 0.0f ) return true;

	// Ground
	if( m_fGroundCitResHunter > 0.0f  && pCapability->m_fGroundCitResHunter > 0.0f ) return true;
	if( m_fGroundAttackUnits > 0.0f   && pCapability->m_fGroundAttackUnits > 0.0f ) return true;
	if( m_fGroundDefenseUnits > 0.0f  && pCapability->m_fGroundDefenseUnits > 0.0f ) return true;
	if( m_fInvasionDefense > 0.0f     && pCapability->m_fInvasionDefense > 0.0f ) return true;
	if( m_iAirfieldBuilder > 0        && pCapability->m_iAirfieldBuilder > 0.0f ) return true;

	// Sea
	if( m_fSeaResHunter > 0.0f     && pCapability->m_fSeaResHunter > 0.0f ) return true;
	if( m_fSeaSupremacy > 0.0f     && pCapability->m_fSeaSupremacy > 0.0f ) return true;
	if( m_fShoreBombardment > 0.0f && pCapability->m_fShoreBombardment > 0.0f ) return true;

	//
	return false;
}
/*
bool CEOSAIJobsToDo::CanFulfillAJob( CEOSAIJobCapability* pCapability )
{
	ASSERT( false );
	if( CanFulfillAGeneralJob( pCapability ) ) return true;

	POSITION pos = pCapability->m_UnitTemplatesForCombat.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplate = pCapability->m_UnitTemplatesForCombat.GetNext( pos );
		if( CanFulfillASpecificCombatJob( pUnitTemplate ) ) return true;
	}
	return false;
}
*/
/*
bool CEOSAIJobsToDo::CanFulfillAGeneralJob( CEOSAIJobCapability* pExistingCapability, CUnitTemplate* pAdditionalActor )
{
	// Primary Jobs
	//   Note: what this algoritm does is say "We have 4 GroundCitResHunter tasks, we currently have
	//         3 GroundCitResHunters, and if this new Unit is a GroundCitResHunter, it's useful.
	//         There is no "we can accomplish this with just one unit, no need to add more"
	//         So, there are actually two questions: 
	//             "Can we accomplish the tasks with these units?"
	//             "Would we want to add more units to this task if they are around?"
	if( m_fGroundCitResHunter - pExistingCapability->m_fGroundCitResHunter > 0 && 
		pAdditionalActor->IsGroundCitResHunter() ) return true;
	if( m_fSeaResHunter - pExistingCapability->m_fSeaResHunter > 0 && 
		pAdditionalActor->IsSeaResHunter() ) return true;
	//if( m_fDestroyBuildings - pExistingCapability->m_fDestroyBuildings > 0 && 
	//	pAdditionalActor->CanDegradeCities() ) return true;
	//if( m_fInProductionDamage - pExistingCapability->m_fInProductionDamage > 0 && 
	//	pAdditionalActor->CanDegradeCities() ) return true;

	if( m_iAirfieldBuilder - pExistingCapability->m_iAirfieldBuilder > 0 && 
		pAdditionalActor->CanBuildAirfields() ) return true;

	return false;
}
*/

void  CEOSAIJobsToDo::MultiplyExpandJobSignificance( float f )
{
	// Exploration, Sighting
	m_fExploration *= f;
	// m_fSighting;

	// Degrade Cities
	// m_fKillPopulation;
	// m_fDestroyBuildings;
	// m_fInProductionDamage;

	// Aircraft
	// m_fAirEscort;
	// m_fAntiAir;  // anti-aircaft guns, fighters, SAM sites

	// Submarines
	// m_fSubmarineCapability;  // strength, speed, stealth of submarines
	// m_fAntiSubmarine;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	m_fGroundCitResHunter *= f;
	// m_fGroundAttackUnits;  // units that can attack ground units (ground units, aircraft, ..)
	// m_fGroundDefenseUnits;
	// m_fInvasionDefense;    // includes aircraft, coastal ships to attack transports, ...
	// m_iAirfieldBuilder;

	// Sea
	//
	m_fSeaResHunter *= f;
	// m_fSeaSupremacy;  // ship-to-ship combat
	// m_fShoreBombardment;
}

void  CEOSAIJobsToDo::MultiplyConquestJobSignificance( float f )
{
	// Exploration, Sighting
	// m_fExploration *= f;
	m_fSighting *= f;

	// Degrade Cities
	m_fKillPopulation *= f;
	m_fDestroyBuildings *= f;
	m_fInProductionDamage *= f;

	// Aircraft
	m_fAirEscort *= f;
	m_fAntiAir *= f;  // anti-aircaft guns, fighters, SAM sites

	// Submarines
	m_fSubmarineCapability *= f;  // strength, speed, stealth of submarines
	m_fAntiSubmarine *= f;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	// m_iGroundCitResHunter;
	m_fGroundAttackUnits *= f;  // units that can attack ground units (ground units, aircraft, ..)
	m_fGroundDefenseUnits *= f;
	m_fInvasionDefense *= f;    // includes aircraft, coastal ships to attack transports, ...
	//m_iAirfieldBuilder *= f;

	// Sea
	// m_iSeaResHunter;
	m_fSeaSupremacy *= f;  // ship-to-ship combat
	m_fShoreBombardment *= f;
}


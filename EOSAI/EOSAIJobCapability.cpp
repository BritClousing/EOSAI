
#include "stdafx.h"
#include "EOSAIJobCapability.h"
#include "EOSAIUnitTemplateAndFloat.h"
//#include "UnitTemplate.h"
#include "EOSAIJobRole.h"
#include "EOSAIUnitTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAIJobCapability::Clear()
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

	// Transports/Carriers
	m_fTransportCapability = 0.0f;
	m_fCarrierCapability = 0.0f;
	
	/*
	// Primary Jobs
	m_iGroundCitResHunter = 0;
	m_iSeaResHunter = 0;
	m_iDegradeCity = 0;
	m_iAirfieldBuilder = 0;
*/
	m_UnitTemplatesForCombat.RemoveAll();

	// Secondary Jobs
	//m_iTransport = 0;
	//m_iCarrier = 0;

	// Generalized Combat (generalized combat against unknown threats - like protect CitRes)
	//   Contents will be things like "Ship" (combat vs. ships), "Aircraft" (combat vs. aircraft), etc
	/*
	while( m_AttritionValue01Versus.IsEmpty() == FALSE )
	{
		delete m_AttritionValue01Versus.RemoveHead();
	}
	*/
}

void  CEOSAIJobCapability::Add( CEOSAIJobCapability* pAIJobCapability )
{
	// Primary Jobs
	//m_iGroundCitResHunter += pAIJobCapability->m_iGroundCitResHunter;
	//m_iSeaResHunter += pAIJobCapability->m_iSeaResHunter;
	//m_iDegradeCity += pAIJobCapability->m_iDegradeCity;
	//m_iAirfieldBuilder += pAIJobCapability->m_iAirfieldBuilder;

	// Exploration, Sighting
	m_fExploration += pAIJobCapability->m_fExploration;
	m_fSighting += pAIJobCapability->m_fSighting;

	// Degrade Cities
	m_fKillPopulation += pAIJobCapability->m_fKillPopulation;
	m_fDestroyBuildings += pAIJobCapability->m_fDestroyBuildings;
	m_fInProductionDamage += pAIJobCapability->m_fInProductionDamage;

	// Aircraft
	m_fAirEscort += pAIJobCapability->m_fAirEscort;
	m_fAntiAir += pAIJobCapability->m_fAntiAir;  // anti-aircaft guns, fighters, SAM sites

	// Submarines
	m_fSubmarineCapability += pAIJobCapability->m_fSubmarineCapability;  // strength, speed, stealth of submarines
	m_fAntiSubmarine += pAIJobCapability->m_fAntiSubmarine;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	m_fGroundCitResHunter += pAIJobCapability->m_fGroundCitResHunter;
	m_fGroundAttackUnits += pAIJobCapability->m_fGroundAttackUnits;  // units that can attack ground units (ground units, aircraft, ..)
	m_fGroundDefenseUnits += pAIJobCapability->m_fGroundDefenseUnits;
	m_fInvasionDefense += pAIJobCapability->m_fInvasionDefense;    // includes aircraft, coastal ships to attack transports, ...
	m_iAirfieldBuilder += pAIJobCapability->m_iAirfieldBuilder;

	// Sea
	m_fSeaResHunter += pAIJobCapability->m_fSeaResHunter;
	m_fSeaSupremacy += pAIJobCapability->m_fSeaSupremacy;  // ship-to-ship combat
	m_fShoreBombardment += pAIJobCapability->m_fShoreBombardment;

	// Transports/Carriers
	m_fTransportCapability += pAIJobCapability->m_fTransportCapability;
	m_fCarrierCapability += pAIJobCapability->m_fCarrierCapability;

	POSITION pos = pAIJobCapability->m_UnitTemplatesForCombat.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = pAIJobCapability->m_UnitTemplatesForCombat.GetNext( pos );
		m_UnitTemplatesForCombat.AddTail( pAIUnitTemplate );
	}

	// Secondary Jobs
	//m_iTransport += pAIJobCapability->m_iTransport;
	//m_iCarrier += pAIJobCapability->m_iCarrier;

	/*
	POSITION pos = pAIJobCapability->m_AttritionValue01Versus.GetHeadPosition();
	while( pos )
	{
		CUnitTemplateAndFloat* pValue = pAIJobCapability->m_AttritionValue01Versus.GetNext( pos );
	}
	*/

	// Generalized Combat (generalized combat against unknown threats - like protect CitRes)
	//   Contents will be things like "Ship" (combat vs. ships), "Aircraft" (combat vs. aircraft), etc
	//CStringList  m_CombatVs; // just an idea for defending against potential attacks
	//CList< EOSAI::StringAndFloat* >  m_CombatVs;
	//CList< CUnitTemplateAndFloat* >  m_AttritionValue01Versus; // attrition or chances of defeat? both are important
}

void  CEOSAIJobCapability::Remove( CEOSAIJobCapability* pAIJobCapability )
{
	// Primary Jobs
	//m_iGroundCitResHunter -= pAIJobCapability->m_iGroundCitResHunter;
	//m_iSeaResHunter -= pAIJobCapability->m_iSeaResHunter;
	//m_iDegradeCity -= pAIJobCapability->m_iDegradeCity;
	//m_iAirfieldBuilder -= pAIJobCapability->m_iAirfieldBuilder;

	// Exploration, Sighting
	m_fExploration -= pAIJobCapability->m_fExploration;
	m_fSighting -= pAIJobCapability->m_fSighting;

	// Degrade Cities
	m_fKillPopulation -= pAIJobCapability->m_fKillPopulation;
	m_fDestroyBuildings -= pAIJobCapability->m_fDestroyBuildings;
	m_fInProductionDamage -= pAIJobCapability->m_fInProductionDamage;

	// Aircraft
	m_fAirEscort -= pAIJobCapability->m_fAirEscort;
	m_fAntiAir -= pAIJobCapability->m_fAntiAir;  // anti-aircaft guns, fighters, SAM sites

	// Submarines
	m_fSubmarineCapability -= pAIJobCapability->m_fSubmarineCapability;  // strength, speed, stealth of submarines
	m_fAntiSubmarine -= pAIJobCapability->m_fAntiSubmarine;

	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	m_fGroundCitResHunter -= pAIJobCapability->m_fGroundCitResHunter;
	m_fGroundAttackUnits -= pAIJobCapability->m_fGroundAttackUnits;  // units that can attack ground units (ground units, aircraft, ..)
	m_fGroundDefenseUnits -= pAIJobCapability->m_fGroundDefenseUnits;
	m_fInvasionDefense -= pAIJobCapability->m_fInvasionDefense;    // includes aircraft, coastal ships to attack transports, ...
	m_iAirfieldBuilder -= pAIJobCapability->m_iAirfieldBuilder;

	// Sea
	m_fSeaResHunter -= pAIJobCapability->m_fSeaResHunter;
	m_fSeaSupremacy -= pAIJobCapability->m_fSeaSupremacy;  // ship-to-ship combat
	m_fShoreBombardment -= pAIJobCapability->m_fShoreBombardment;

	// Transports/Carriers
	m_fTransportCapability -= pAIJobCapability->m_fTransportCapability;
	m_fCarrierCapability -= pAIJobCapability->m_fCarrierCapability;

	POSITION pos = pAIJobCapability->m_UnitTemplatesForCombat.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pUnitTemplateInOtherList = pAIJobCapability->m_UnitTemplatesForCombat.GetNext( pos );
		//m_UnitTemplatesForCombat.AddTail( pUnitTemplate );

		POSITION pos2 = m_UnitTemplatesForCombat.GetHeadPosition();
		while( pos2 )
		{
			POSITION prevPos = pos2;
			CEOSAIUnitTemplate* pUnitTemplateInMyList = m_UnitTemplatesForCombat.GetNext( pos2 );
			if( pUnitTemplateInOtherList == pUnitTemplateInMyList )
			{
				m_UnitTemplatesForCombat.RemoveAt( prevPos );
				break;
			}
		}
	}

	// Secondary Jobs
	//m_iTransport += pAIJobCapability->m_iTransport;
	//m_iCarrier += pAIJobCapability->m_iCarrier;

	/*
	POSITION pos = pAIJobCapability->m_AttritionValue01Versus.GetHeadPosition();
	while( pos )
	{
		CUnitTemplateAndFloat* pValue = pAIJobCapability->m_AttritionValue01Versus.GetNext( pos );
	}
	*/

	// Generalized Combat (generalized combat against unknown threats - like protect CitRes)
	//   Contents will be things like "Ship" (combat vs. ships), "Aircraft" (combat vs. aircraft), etc
	//CStringList  m_CombatVs; // just an idea for defending against potential attacks
	//CList< EOSAI::StringAndFloat* >  m_CombatVs;
	//CList< CUnitTemplateAndFloat* >  m_AttritionValue01Versus; // attrition or chances of defeat? both are important
}

void  CEOSAIJobCapability::Add( CEOSAIUnitTemplate* pAIUnitTemplate )//, CEOSAIJobRole* pJobRole )
{
	/*
	CEOSAIJobCapability  AIJobCapability;
	pAIUnitTemplate->CalculateAIJobCapability( &AIJobCapability );
	Add( &AIJobCapability );
	*/
	Add( pAIUnitTemplate->GetAIJobCapability() );
	m_UnitTemplatesAndFloat.Add( pAIUnitTemplate, 1.0f );
}

void  CEOSAIJobCapability::Remove( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	/*
	CEOSAIJobCapability  AIJobCapability;
	pAIUnitTemplate->GetAIJobCapability( &AIJobCapability );
	Remove( &AIJobCapability );
	*/
	Remove( pAIUnitTemplate->GetAIJobCapability() );
	m_UnitTemplatesAndFloat.Add( pAIUnitTemplate, -1.0f );

	/*
	if( pUnitTemplate->GetUnitCombatCapability()->IsACombatUnit() )
	{
		POSITION pos = m_UnitTemplatesForCombat.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CUnitTemplate* pTemplateInList = m_UnitTemplatesForCombat.GetNext( pos );
			if( pTemplateInList == pUnitTemplate )
			{
				m_UnitTemplatesForCombat.RemoveAt( prevPos );
				break;
			}
		}
	}*
	*/
}

void  CEOSAIJobCapability::Set( CEOSAIJobCapability* pAIJobCapability )
{
	Clear();
	Add( pAIJobCapability );
}

void  CEOSAIJobCapability::Set( CEOSAIUnitTemplate* pAIUnitTemplate )//, CEOSAIJobRole* pJobRole )
{
	Clear();
	Add( pAIUnitTemplate );
	//Add( pUnitTemplate, pJobRole );

	//CEOSAIJobCapability  AIJobCapability;
	//pUnitTemplate->GetAIJobCapability( &AIJobCapability );
	//Set( &AIJobCapability );
}


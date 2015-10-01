
#include "stdafx.h"
/*
#include "AIDesire_InvadeGeo.h"
#include "AIMilitarySummary.h"

CAIDesire_InvadeGeo::CAIDesire_InvadeGeo( CAIPlan* pAIPlan, CEOSAIGeoLand* pAIGeoLand )
	: CAIDesire( pAIPlan )
{
	m_pAIGeoLand = pAIGeoLand;

	m_bSingleActionSatisfiesThisDesire = false;

	//m_fProximityToMyBorders = 0.0f;
	m_fDangerDuringTransportPhase = 0.0f;
	m_fEstimateOfNumberOfUnitsInInvasionForce = 0.0f;
	m_fMyCapabilityToInvadeSuccessfully = 0.0f;

	//
	//m_fOtherPlayerMilitaryStrength = 0.0f;
	//m_fOtherPlayerMilitaryProductionValue = 0.0f;

	//m_fCitResValue = 0.0f;
	//m_fNotMineCitResValue = 0.0f;
}

float CAIDesire_InvadeGeo::CalculateScore( CEOSAIPoiObject* pActor1, float fDesireSatisfiedTime )
{
	// Look over my military (ground units, transports) and see if I can challenge him
	CEOSAINationalSummary* pMyNationalSummary = m_pAIPlan->GetAIBrain()->GetNationalSummary( m_pAIPlan->GetAIPlayerNumber() );

	// The national summary assumes I send every unit that I have
	//   This is unrealistic - I would like to retain some units to defend against
	//   an enemy invasion, bomber strikes, etc

	CAIMilitarySummary  GeoMilitarySummary = m_pAIGeoLand->GetEnemyAndNeutralMilitarySummary();

	// For an invasion, I should bring:
	//    ground-combat units,
	//    CitRes hunters,
	//    explore/sighting units, 
	//    anti-transport units (tactical bombers, ships, subs) to stop reinforcements
	// I need to take into account the new units his cities will be producing before
	//    the invasion is completed
	// I should include the possibility of bringing reinforcements after the first wave
	//    although that may be hampered by my enemies
	

	// Calculate a score based on zero pervious commitments
	//   (i.e. what would the score be if this was the only island I was invading?)
	return 0.0f;
}
*/


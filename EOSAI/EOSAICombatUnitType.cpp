
#include "stdafx.h"
#include "EOSAICombatUnitType.h"
//#include "BCString.h"
//#include "BCXMLReader.h"
//#include "EOSAISerial.h"

CEOSAICombatUnitType::CEOSAICombatUnitType()
{
	m_iCombatUnitType = 0;

	m_bCanBlockAndBeBlocked = false;
	m_iDrawOrder = 0;
	m_fFortificationDefenseMultiplier = 1.0f;

	m_iLandedOrDockedCombatType = 0;
	//m_fLandedOrDockedMultiplier = 1.0f;
	m_fEntrenchedDefenseMultiplierBonus = 0.0f;
	m_DefensiveValueMultiplier.SetAllValuesToOne();
}

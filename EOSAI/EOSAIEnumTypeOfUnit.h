
#pragma once

enum EOSAIEnumTypeOfUnit
{
	EOSAIEnumTypeOfUnit_Undefined,

	EOSAIEnumTypeOfUnit_All,

	EOSAIEnumTypeOfUnit_CityHunter,
	EOSAIEnumTypeOfUnit_CityHunterTransport,

	EOSAIEnumTypeOfUnit_GroundResourceHunter,
	//EOSAIEnumTypeOfUnit_GroundCombat,
	//EOSAIEnumTypeOfUnit_AirSupport,
	//EOSAIEnumTypeOfUnit_SeaCombat,
	EOSAIEnumTypeOfUnit_GroundUnitHunter, // includes ground units, air units
	EOSAIEnumTypeOfUnit_SeaUnitHunter,    // includes sea units, air units
	EOSAIEnumTypeOfUnit_SeaResourceHunter
};

class CEOSAITypeOfUnitCount
{
	public:
		EOSAIEnumTypeOfUnit  m_eAITypeOfUnit;
		float             m_fCount;
};


#pragma once

// Variables:
//   There are valuable CitRes
//   I have a presence
//   Other player(s) have a presence
//   There are unowned CitRes
//   Has been fully explored
//   Is in a high-danger zone

enum EOSAIEnumGeoPlan
{
	EOSAIEnumGeoPlan_Undefined,
	EOSAIEnumGeoPlan_SmallGeo_NoCitResUnits,
	EOSAIEnumGeoPlan_NoPlan,
	EOSAIEnumGeoPlan_Ignore,
//	EOSAIEnumGeoPlan_Ignore_NoCitResUnits,
//	EOSAIEnumGeoPlan_Ignore_TooDistant,
	//EOSAIEnumGeoPlan_Ignore_NoGroundOrSeaAccess,

	// Water Strategies
	EOSAIEnumGeoPlan_Water_NoPlanYet,
	EOSAIEnumGeoPlan_Water_NoCitResUnits,
	EOSAIEnumGeoPlan_Water_TooDistant,
	EOSAIEnumGeoPlan_Water_Complex, // Use this for large bodies of water
//	EOSAIEnumGeoPlan_Water_CherryPicker,
//	EOSAIEnumGeoPlan_Water_ProtectMyWaterways, // (Defensive) Good for defending against invasions
//	EOSAIEnumGeoPlan_Water_Submarine,    // (Offensive x1) Send submarines around
//	EOSAIEnumGeoPlan_Water_Aggressive,   // (Offensive x2) Make hits at enemy ships
//	EOSAIEnumGeoPlan_Water_TotalControl, // (Offensive x3) Take control of the seas, eliminate other player's navy

	// Land Strategies
	EOSAIEnumGeoPlan_Land_NoPlanYet,
	EOSAIEnumGeoPlan_Land_IgnoreLowRank,
	EOSAIEnumGeoPlan_Land_NoCitResUnits,
	EOSAIEnumGeoPlan_Land_TooDistant,
	EOSAIEnumGeoPlan_Land_ProtectMyGeo,
	EOSAIEnumGeoPlan_Land_FriendsGeo,
	//EOSAIEnumGeoPlan_Land_Mine, // Used when I completely control an AIGeo - need a better strategy
	EOSAIEnumGeoPlan_Land_Seed,         // (Unowned) Capture a city, build and expand until I capture it all
	EOSAIEnumGeoPlan_Land_Consolidate,  // (Mine+Unowned) I have a presence, no one else does.  Capture the rest of the island
	EOSAIEnumGeoPlan_Land_InvadeGeo,    // (Enemy Small Landmass) Other player has a presence here - need to bring in combat units
	EOSAIEnumGeoPlan_Land_InvadeSection,// (Enemy Large Landmass) Other player has a presence here - need to bring in combat units
	EOSAIEnumGeoPlan_Land_InvadeUnownedSection,// (Other+Unowned) Other player has a presence here - dont fight, just grab unowned
	//EnumAIGeoStrategy_Land_Degrade,       // Don't invade, just devalue it
};

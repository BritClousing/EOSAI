
#pragma once

// Variables:
//   There are valuable CitRes
//   I have a presence
//   Other player(s) have a presence
//   There are unowned CitRes
//   Has been fully explored
//   Is in a high-danger zone

enum EOSAIEnumMultiRegionPlan
{
	EOSAIEnumMultiRegionPlan_Undefined,
	EOSAIEnumMultiRegionPlan_NoPlan,
	EOSAIEnumMultiRegionPlan_Ignore,
//	EOSAIEnumMultiRegionPlan_Ignore_NoCitResUnits,
//	EOSAIEnumMultiRegionPlan_Ignore_TooDistant,
	//EOSAIEnumMultiRegionPlan_Ignore_NoGroundOrSeaAccess,

	// Water Strategies
	EOSAIEnumMultiRegionPlan_Water_NoPlanYet,
	EOSAIEnumMultiRegionPlan_Water_NoCitResUnits,
	EOSAIEnumMultiRegionPlan_Water_TooDistant,
	EOSAIEnumMultiRegionPlan_Water_SeaControl,
	EOSAIEnumMultiRegionPlan_Water_ConsiderCaptureAndControl,
	EOSAIEnumMultiRegionPlan_Water_Submarine,
//	EOSAIEnumMultiRegionPlan_Water_CherryPicker,
//	EOSAIEnumMultiRegionPlan_Water_ProtectMyWaterways, // (Defensive) Good for defending against invasions
//	EOSAIEnumMultiRegionPlan_Water_Submarine,    // (Offensive x1) Send submarines around
//	EOSAIEnumMultiRegionPlan_Water_Aggressive,   // (Offensive x2) Make hits at enemy ships
//	EOSAIEnumMultiRegionPlan_Water_TotalControl, // (Offensive x3) Take control of the seas, eliminate other player's navy

	// Land Strategies
	EOSAIEnumMultiRegionPlan_Land_NoPlanYet,
	EOSAIEnumMultiRegionPlan_Land_NoCitResUnits,
	EOSAIEnumMultiRegionPlan_Land_TooDistant,
	EOSAIEnumMultiRegionPlan_Land_ProtectMyGeo,
	EOSAIEnumMultiRegionPlan_Land_FriendsGeo,
	//EOSAIEnumMultiRegionPlan_Land_Mine, // Used when I completely control an AIGeo - need a better strategy
	EOSAIEnumMultiRegionPlan_Land_Seed,         // (Unowned) Capture a city, build and expand until I capture it all
	EOSAIEnumMultiRegionPlan_Land_Consolidate,  // (Mine+Unowned) I have a presence, no one else does.  Capture the rest of the island
	EOSAIEnumMultiRegionPlan_Land_InvadeGeo,    // (Enemy Small Landmass) Other player has a presence here - need to bring in combat units
	EOSAIEnumMultiRegionPlan_Land_InvadeSection,// (Enemy Large Landmass) Other player has a presence here - need to bring in combat units
	EOSAIEnumMultiRegionPlan_Land_InvadeUnownedSection,// (Other+Unowned) Other player has a presence here - dont fight, just grab unowned
	//EnumAIGeoStrategy_Land_Degrade,       // Don't invade, just devalue it
};


#pragma once

enum EOSAIEnumGeoSituation
{
	EOSAIEnumGeoSituation_Undefined,
	//EOSAIEnumGeoSituation_NoPoi,

	// Impassible
	EOSAIEnumGeoSituation_Impassible,

	// Water Situation
	EOSAIEnumGeoSituation_SmallWater_NoPoi,  // Small ponds
	EOSAIEnumGeoSituation_Water_WithPoi, // Ambiguous, it might contain an important pathway
	EOSAIEnumGeoSituation_Water_NoPoi,   // Ambiguous, it might contain an important pathway
	EOSAIEnumGeoSituation_Water_NoPoiNoPath,
	EOSAIEnumGeoSituation_Water_Large,

	// Land Situation
	EOSAIEnumGeoSituation_Land_NoPoi,
	EOSAIEnumGeoSituation_Land_Unowned,    // 
	EOSAIEnumGeoSituation_Land_Mine,       // Used when I completely control an AIGeo - need a better strategy
	EOSAIEnumGeoSituation_Land_MineUnowned,// (Mine+Unowned) I have a presence, no one else does.  Capture the rest of the island
	EOSAIEnumGeoSituation_Land_AT,         // 
	EOSAIEnumGeoSituation_Land_ATUnowned,  // 
	EOSAIEnumGeoSituation_Land_SAT,        // 
	EOSAIEnumGeoSituation_Land_SATUnowned, // 
	EOSAIEnumGeoSituation_Land_Neutral,    // 
	EOSAIEnumGeoSituation_Land_NeutralUnowned,// 
	EOSAIEnumGeoSituation_Land_Enemy,       // 
	EOSAIEnumGeoSituation_Land_EnemyUnowned,// 
	EOSAIEnumGeoSituation_Land_Complex,     // 

	// 
	// Unowned (true/false)
	// Neutral (true/false)
	// Enemy

	// Land Strategies
//	EOSAIEnumGeoSituation_Land_SAT,           // Used when I completely control an AIGeo - need a better strategy
//	EOSAIEnumGeoSituation_Land_UnownedCitRes, // (Unowned) Capture a city, build and expand until I capture it all
//	EOSAIEnumGeoSituation_Land_SATUnowned,// (Mine+Unowned) I have a presence, no one else does.  Capture the rest of the island
//	EOSAIEnumGeoSituation_Land_SAT,    // (Enemy Small Landmass) Other player has a presence here - need to bring in combat units
//	EOSAIEnumGeoSituation_Land_InvadeSection,// (Enemy Large Landmass) Other player has a presence here - need to bring in combat units
//	EOSAIEnumGeoSituation_Land_InvadeUnownedSection,// (Other+Unowned) Other player has a presence here - dont fight, just grab unowned
	//EOSAIEnumGeoSituation_Land_Degrade,       // Don't invade, just devalue it
};

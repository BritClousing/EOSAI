
#pragma once

enum EOSAIEnumAttackGoal
{
	EOSAIEnumAttackGoal_KillUnits = 0,   // Kill units, if this is a city, stop attacking when units are dead
	EOSAIEnumAttackGoal_DegradeCity = 1, // Bomb or Bombard a city to damage it (don't try to capture)
	EOSAIEnumAttackGoal_Capture = 2,     // Kill all units inside, then capture
	//EnumAttackGoal_Bombing = 3,  // Bombings can be "KillUnits" or "DegradeCity"
};

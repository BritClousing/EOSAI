
#pragma once
/*
#define ID_FIELDORDERS_CLEAR 0
#define ID_FIELDORDERS_ATTACK_WEAKER_UNITS 1
#define ID_FIELDORDERS_ATTACK_ALL_UNITS 2
#define ID_FIELDORDERS_ATTACK_GROUND_UNITS 3
#define ID_FIELDORDERS_ATTACK_AIR_UNITS 4
#define ID_FIELDORDERS_ATTACK_ANTIAIR_UNITS 5
#define ID_FIELDORDERS_ATTACK_SEA_UNITS 6
#define ID_FIELDORDERS_CUSTOM 10
*/
namespace EOSAI
{
enum EnumFieldOrders
{
	//enumFieldOrders_Undefined,
	//enumFieldOrders_Recon,
	enumFieldOrders_None = -1,
	enumFieldOrders_Default = 0,//enumFieldOrders_None = 0,
	//enumFieldOrders_MoveTo,
	enumFieldOrders_AttackWeaker = 1,
	enumFieldOrders_AttackAll = 2,
	enumFieldOrders_AttackGround = 3,
	enumFieldOrders_AttackAir = 4,
	//enumFieldOrders_AttackAntiAir = 5,
	enumFieldOrders_AttackSea = 6,
	//
	// GroundUnits:
	//   Tanks, Infantry, Artillery should attack other GroundUnits
	//   Destroyers should attack Transports, Destroyers, Submarines, but not Cruisers or Battleships
	// SeaUnits:
	//   Cruisers should attack Transports, Destroyers, Submarines, Cruisers but not Battleships
	// AirUnits:
	//   Fighters, Tactical Bombers, Strategic Bombers should attack enemies with good attrition
//	enumFieldOrders_Aggressive = 7,
	// Transports, Aircraft Carriers should avoid combat.  They should not ever attack enemies.
	// Zeppelins should avoid combat.
//	enumFieldOrders_Passive = 8,
	//
	enumFieldOrders_UnitsetCustom = 9, // prepend an "Attack " to this
	enumFieldOrders_Custom = 10,
};
};

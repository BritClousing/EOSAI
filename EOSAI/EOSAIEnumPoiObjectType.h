
#pragma once

class EnumAIPoiObjectType
{
	public:
		enum Value
		{
			enum_Undefined = 0,
			enum_MapObject = 1,
			enum_AnimationBase = 2,

			enum_Poi = 3,
			enum_City = 4,
			enum_Airfield = 5,
			enum_Fortification = 6,
			enum_Resource = 7,

			enum_PoiPD = 8,
			enum_Mountain = 9,
			//enum_Building = 5,

			// Units
			//enum_Soft = 6,
			//enum_Armor = 7,
			//enum_Aircraft = 8,
			//enum_Missile = 9,
			//enum_Ship = 10,
			//enum_Submarine = 11,
			//
			enum_Unit = 12, // I want to eliminate Soft,Armor,etc and replace it with enum_Unit
			enum_UnitGroup = 13, // I want to eliminate Soft,Armor,etc and replace it with enum_Unit
		};

	public:

		EnumAIPoiObjectType(){ m_v = enum_Undefined; }
		EnumAIPoiObjectType( EnumAIPoiObjectType::Value val ){ m_v = val; }

		void  operator=(  EnumAIPoiObjectType::Value val ){ m_v = val; }
		bool  operator==( EnumAIPoiObjectType        val ){ return m_v == val.m_v; }
		bool  operator==( EnumAIPoiObjectType::Value val ){ return m_v == val; }
		bool  operator!=( EnumAIPoiObjectType::Value val ){ return m_v != val; }

		bool  IsUnit(){ return //m_v == EnumPoiObjectType::enum_Soft || 
							   //m_v == EnumPoiObjectType::enum_Armor ||
							   //m_v == EnumPoiObjectType::enum_Aircraft ||
							   //m_v == EnumPoiObjectType::enum_Missile ||
							   //m_v == EnumPoiObjectType::enum_Ship ||
							   //m_v == EnumPoiObjectType::enum_Submarine ||
							   m_v == EnumAIPoiObjectType::enum_Unit; }// ||
							   //m_v == EnumPoiObjectType::enum_Unit; }

		Value m_v;
};
/*
enum EnumCombatUnitType
{
	EnumCombatUnitType_Undefined,

	EnumCombatUnitType_City,
	EnumCombatUnitType_Airfield,
	EnumCombatUnitType_Fortification,
	EnumCombatUnitType_Resource,

	EnumCombatUnitType_Building,

	// New classifications - note: the only reason for the classifications now
	//    is for combat.  This can be changed to the UnitTag system.
	EnumCombatUnitType_Soft,
	EnumCombatUnitType_Armor,
	EnumCombatUnitType_Aircraft,
	EnumCombatUnitType_Missile,
	EnumCombatUnitType_Ship,
	EnumCombatUnitType_Submarine,
};
*/


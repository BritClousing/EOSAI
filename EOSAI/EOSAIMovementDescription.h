
#pragma once

// This is used by the FindPath algorithm to figure out the fastest route.
//   In order to do this, the algorithm needs to know how quickly the unit
//   can move over the different types of terrain.
// (This is also used for building roads around mountains inside WorldBuilder.)

#include "EOSAIMovementUnitType.h"

#define TERRAIN_UNKNOWN    0 // special case ( returned by CWorldDesc::GetActualTerrainType )
#define TERRAIN_WATER      1 // special case ( returned by CWorldDesc::GetActualTerrainType )
#define TERRAIN_COAST      2 // special case ( this is used to keep ships off the coast )
#define TERRAIN_ARCTIC     3
#define TERRAIN_DESERT     4
#define TERRAIN_GRASS      5
#define TERRAIN_FOREST     6
/*
#define TERRAIN_SWAMP      7
#define TERRAIN_ROAD       8 // special case ( returned by CWorldDesc::GetActualTerrainType )
#define TERRAIN_CITY       9 // special case ( returned by CWorldDesc::GetActualTerrainType )
#define TERRAIN_MOUNTAIN  10 // special case ( returned by CWorldDesc::GetActualTerrainType )
#define TERRAIN_AIR       11 // special case
#define TERRAIN_COUNT     12
*/
#define TERRAIN_ROAD       7 // special case ( returned by CWorldDesc::GetActualTerrainType )
#define TERRAIN_CITY       8 // special case ( returned by CWorldDesc::GetActualTerrainType )
#define TERRAIN_MOUNTAIN   9 // special case ( returned by CWorldDesc::GetActualTerrainType )
#define TERRAIN_AIR       10 // special case
#define TERRAIN_SWAMP     11
#define TERRAIN_COUNT     12
#define TERRAIN_IMPASSIBLE 100


class CEOSAIMovementDescription
{
	public:
		static void Initialize();

	public:
		CEOSAIMovementDescription();

		void Set( CEOSAIMovementUnitType* pType );

		EOSAIEnumMovementType3  GetMovementType(){ return m_eMovementType; }
		bool MovesInAir(){ return m_eMovementType == EOSAIEnumMovementType3_Air; }
		bool MovesOnLand(){ return m_eMovementType == EOSAIEnumMovementType3_Land; }
		bool MovesInWater(){ return m_eMovementType == EOSAIEnumMovementType3_Water; }
		bool MovesInMountains(){ return MovesOnLand() && m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] > 0.0f; }

		void SetToRoadBuilder();
		void SetToRoadBuilderOverWater();
		void SetToGenericGroundUnit();
		void SetToGenericGroundUnitAvoidMountains();
		void SetToGenericSeaUnit();
		void SetToZero();
		void SetToOne();

		//void SetMovementType( EnumMovementType eMovementType ){ m_eMovementType = eMovementType; }
		//bool CanMoveOnLand();
		//bool CanMoveInMountains();

		// Set [this] equal to [pOther]
		void Copy( CEOSAIMovementDescription* pOther );
		void SetMovementUnitType( CEOSAIMovementUnitType* pMovementUnitType );

		// Limit the speed on each type of terrain (used by groups)
		void SetLimits( CEOSAIMovementDescription* pOther );

		// On a terrain-by-terrain basis, copy the value of pOther if
		//   the component is faster than the one currently stored
		void CopyFastestComponents( CEOSAIMovementDescription* pOther );

		// If e.g. roads and grass are very different costs, the Ratio will be large
		//   Value of 1.0 means that movement costs are the same for all valid terrain.
		//   Value of 0.5 means that the fastest terrain is twice the rate of the slowest.
		float  GetMovementCostRatio();
		float  GetLowestNonZeroMovementRate();
		float  GetHighestMovementRate();

		float  GetMovementRate(){ return m_fMovementRate; }
		//float  GetMovementRate( long iTerrain ){ return m_fMovementRate * m_fTerrainMovementRateMultiplier[iTerrain]; }
		float  GetMovementRate( long iTerrain ){ return m_fMovementRate * m_MovementRateMultiplier.m_fTerrainValue[ iTerrain ]; }
		//void   SetMovementMultiplier( long iTerrain, float fMult );

		CEOSAIMovementUnitType*  m_pMovementUnitType; // Used for Unit Maintenence values (ignored if UnitGroup)
		EOSAIEnumMovementType3     m_eMovementType; // Three types: Land, Air, Water
		float                 m_fMovementRate;
		CEOSAITerrainBasedValue  m_MovementRateMultiplier;
		//float  m_fTerrainMovementRateMultiplier[TERRAIN_COUNT]; // use TERRAIN_UNKNOWN, TERRAIN_GRASS, etc to access
};

extern CEOSAIMovementDescription  g_AIMovementDescriptionForRoadBuilding;

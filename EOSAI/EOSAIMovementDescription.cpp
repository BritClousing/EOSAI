
#include "stdafx.h"
#include "EOSAIMovementDescription.h"

CEOSAIMovementDescription g_MovementDescriptionForRoadBuilding;
CEOSAIMovementDescription g_MovementDescriptionForRoadBuildingOverWater;

void CEOSAIMovementDescription::Set( CEOSAIMovementUnitType* pType )
{
	m_pMovementUnitType = pType;
	m_eMovementType = pType->m_eMovementType;

	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_MovementRateMultiplier.m_fTerrainValue[ i ] = pType->GetMovementRateMultiplier( false, i );
	}
	/*
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_UNKNOWN ]  = pType->GetMovementRateMultiplier( false, TERRAIN_UNKNOWN );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_WATER ]    = pType->GetMovementRateMultiplier( false, TERRAIN_WATER );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_COAST ]    = pType->GetMovementRateMultiplier( false, TERRAIN_COAST );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ARCTIC ]   = pType->GetMovementRateMultiplier( false, TERRAIN_ARCTIC );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_DESERT ]   = pType->GetMovementRateMultiplier( false, TERRAIN_DESERT );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_GRASS ]    = pType->GetMovementRateMultiplier( false, TERRAIN_GRASS );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_FOREST ]   = pType->GetMovementRateMultiplier( false, TERRAIN_FOREST );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ROAD ]     = pType->GetMovementRateMultiplier( false, TERRAIN_ROAD );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_CITY ]     = pType->GetMovementRateMultiplier( false, TERRAIN_CITY );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] = pType->GetMovementRateMultiplier( false, TERRAIN_MOUNTAIN );
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]      = pType->GetMovementRateMultiplier( false, TERRAIN_AIR );
	*/
}

void CEOSAIMovementDescription::Initialize()
{
	//CMovementUnitType* pGenericGroundMovementUnitType = new CMovementUnitType();
	//pGenericGroundMovementUnitType->SetToGenericGroundUnit();

	//g_MovementDescriptionForRoadBuilding.SetToGenericGroundUnit();
	g_MovementDescriptionForRoadBuilding.SetToRoadBuilder();
	g_MovementDescriptionForRoadBuildingOverWater.SetToRoadBuilderOverWater();
}

CEOSAIMovementDescription::CEOSAIMovementDescription()
{
	m_pMovementUnitType = NULL;
	m_eMovementType = EOSAIEnumMovementType3_Land;
	m_fMovementRate = 1.0f;

	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_MovementRateMultiplier.m_fTerrainValue[i] = 0.0f;
	}
}

void CEOSAIMovementDescription::SetToRoadBuilder()
{
	m_pMovementUnitType = NULL;
	m_eMovementType = EOSAIEnumMovementType3_Land;
	m_fMovementRate = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_UNKNOWN ]  = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_WATER ]    = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_COAST ]    = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ARCTIC ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_DESERT ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_GRASS ]    = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_FOREST ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ROAD ]     = 2.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_CITY ]     = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] = 0.5f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_SWAMP ]    = 0.5f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]      = 0.0f;
}

void CEOSAIMovementDescription::SetToRoadBuilderOverWater()
{
	m_pMovementUnitType = NULL;
	m_eMovementType = EOSAIEnumMovementType3_Land;
	m_fMovementRate = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_UNKNOWN ]  = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_WATER ]    = 0.5f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_COAST ]    = 0.5f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ARCTIC ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_DESERT ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_GRASS ]    = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_FOREST ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ROAD ]     = 2.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_CITY ]     = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] = 0.5f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_SWAMP ]    = 0.5f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]      = 0.0f;
}

void CEOSAIMovementDescription::SetToGenericGroundUnit()
{
	m_pMovementUnitType = NULL;
	m_eMovementType = EOSAIEnumMovementType3_Land;
	m_fMovementRate = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_UNKNOWN ]  = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_WATER ]    = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_COAST ]    = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ARCTIC ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_DESERT ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_GRASS ]    = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_FOREST ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ROAD ]     = 2.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_CITY ]     = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] = 0.5f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]      = 0.0f;
}

void CEOSAIMovementDescription::SetToGenericGroundUnitAvoidMountains()
{
	m_pMovementUnitType = NULL;
	m_eMovementType = EOSAIEnumMovementType3_Land;
	m_fMovementRate = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_UNKNOWN ]  = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_WATER ]    = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_COAST ]    = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ARCTIC ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_DESERT ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_GRASS ]    = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_FOREST ]   = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ROAD ]     = 2.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_CITY ]     = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]      = 0.0f;
}

void CEOSAIMovementDescription::SetToGenericSeaUnit()
{
	m_pMovementUnitType = NULL;
	m_eMovementType = EOSAIEnumMovementType3_Water;
	m_fMovementRate = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_UNKNOWN ]  = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_WATER ]    = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_COAST ]    = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ARCTIC ]   = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_DESERT ]   = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_GRASS ]    = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_FOREST ]   = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ROAD ]     = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_CITY ]     = 1.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] = 0.0f;
	m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]      = 0.0f;
}

void CEOSAIMovementDescription::SetToZero()
{
	m_pMovementUnitType = NULL;
	m_eMovementType = EOSAIEnumMovementType3_Undefined;
	m_fMovementRate = 0.0f;
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_MovementRateMultiplier.m_fTerrainValue[i] = 0.0f;
	}
}

void CEOSAIMovementDescription::SetToOne()
{
	m_pMovementUnitType = NULL;
	m_eMovementType = EOSAIEnumMovementType3_Undefined;
	m_fMovementRate = 1.0f;
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_MovementRateMultiplier.m_fTerrainValue[i] = 1.0f;
	}
}

/*
bool CMovementDescription::CanMoveOnLand()
{
	return 
		m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ARCTIC ]   > 0.0f ||
		m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_DESERT ]   > 0.0f ||
		m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_GRASS ]    > 0.0f ||
		m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] > 0.0f ||
		m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_FOREST ]   > 0.0f;
}

bool CMovementDescription::CanMoveInMountains()
{
	return m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ] > 0.0f;
}
*/
void CEOSAIMovementDescription::Copy( CEOSAIMovementDescription* pOther )
{
	m_pMovementUnitType = pOther->m_pMovementUnitType;
	m_eMovementType = pOther->m_eMovementType;
	m_fMovementRate = pOther->m_fMovementRate;
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_MovementRateMultiplier.m_fTerrainValue[i] = pOther->m_MovementRateMultiplier.m_fTerrainValue[i];
	}
}

void CEOSAIMovementDescription::SetMovementUnitType( CEOSAIMovementUnitType* pMovementUnitType )
{
	m_pMovementUnitType = pMovementUnitType;
	m_eMovementType = pMovementUnitType->m_eMovementType;
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_MovementRateMultiplier.m_fTerrainValue[i] = pMovementUnitType->GetMovementRateMultiplier( false, i );
	}
}

void CEOSAIMovementDescription::SetLimits( CEOSAIMovementDescription* pOther )
{
	m_eMovementType = pOther->m_eMovementType;
	m_fMovementRate = 1.0f;//min( m_fMovementRate, pOther->m_fMovementRate );

	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		//m_fTerrainMovementRate[i] = min( m_fTerrainMovementRate[i], pOther->m_fTerrainMovementRate[i] );
		//SetMovementMultiplier( i, min( GetMovementRate(i), pOther->GetMovementRate(i) ) );
		m_MovementRateMultiplier.m_fTerrainValue[i] = 
			min( m_MovementRateMultiplier.m_fTerrainValue[i], pOther->m_MovementRateMultiplier.m_fTerrainValue[i] );
	}
}

void CEOSAIMovementDescription::CopyFastestComponents( CEOSAIMovementDescription* pOther )
{
	m_eMovementType = pOther->m_eMovementType;
	//m_fMovementRate = max( m_fMovementRate, pOther->m_fMovementRate );
	m_fMovementRate = 1.0f;//max( m_fMovementRate, pOther->m_fMovementRate );
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		//m_fTerrainMovementRate[i] = max( m_fTerrainMovementRate[i], pOther->m_fTerrainMovementRate[i] );
		//SetMovementMultiplier( i, max( GetMovementRate(i), pOther->GetMovementRate(i) ) );
		m_MovementRateMultiplier.m_fTerrainValue[i] = 
			min( m_MovementRateMultiplier.m_fTerrainValue[i], pOther->m_MovementRateMultiplier.m_fTerrainValue[i] );
	}
}


// If e.g. roads and grass are very different costs, the Ratio will be large
float CEOSAIMovementDescription::GetMovementCostRatio()
{
	if( m_eMovementType == EOSAIEnumMovementType3_Air ){ return m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]; }

	bool bHasBeenSet = false;
	float fMin = 0.0f;
	float fMax = 0.0f;
	float fVal;

	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_UNKNOWN ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_WATER ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_COAST ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ARCTIC ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_DESERT ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_GRASS ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_FOREST ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_MOUNTAIN ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ROAD ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_CITY ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	fVal = m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ];
	if( fVal > 0.0f )
	{
		if( bHasBeenSet == false || fVal < fMin ){ fMin = fVal; }
		if( bHasBeenSet == false || fVal > fMax ){ fMax = fVal; }
		bHasBeenSet = true;
	}
	ASSERT( fMax > 0.0f && fMin > 0.0f );

	if( fMax > 0.0f && fMin > 0.0f )
	{
		return (fMax + fMin) / fMax;
	}
	ASSERT( false );
	return 1.0f;
}

float CEOSAIMovementDescription::GetLowestNonZeroMovementRate()
{
	if( m_eMovementType == EOSAIEnumMovementType3_Air ){ return m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]; }

	bool bHasBeenSet = false;
	float fMin = 1000000.0f;

	for( long iTerrain=0; iTerrain<TERRAIN_COUNT; iTerrain++ )
	{
		if( GetMovementRate( iTerrain ) > 0.0f )
		{
			fMin = min( fMin, GetMovementRate( iTerrain ) );
		}
	}
	/*
	if( GetMovementRate( TERRAIN_UNKNOWN ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_UNKNOWN ) );
	}
	if( GetMovementRate( TERRAIN_WATER ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_WATER ) );
	}
	if( GetMovementRate( TERRAIN_COAST ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_COAST ) );
	}
	if( GetMovementRate( TERRAIN_ARCTIC ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_ARCTIC ) );
	}
	if( GetMovementRate( TERRAIN_DESERT ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_DESERT ) );
	}
	if( GetMovementRate( TERRAIN_GRASS ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_GRASS ) );
	}
	if( GetMovementRate( TERRAIN_FOREST ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_FOREST ) );
	}
	if( GetMovementRate( TERRAIN_MOUNTAIN ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_MOUNTAIN ) );
	}
	if( GetMovementRate( TERRAIN_ROAD ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_ROAD ) );
	}
	if( GetMovementRate( TERRAIN_CITY ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_CITY ) );
	}
	if( GetMovementRate( TERRAIN_AIR ) > 0.0f )
	{
		fMin = min( fMin, GetMovementRate( TERRAIN_AIR ) );
	}
	*/
	ASSERT( fMin != 1000000.0f );

	return fMin;
}


float CEOSAIMovementDescription::GetHighestMovementRate()
{
	if( m_eMovementType == EOSAIEnumMovementType3_Air ){ return m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_AIR ]; }

	//bool bHasBeenSet = false;
	float fMax = 0.0f;

	for( long iTerrain=0; iTerrain<TERRAIN_COUNT; iTerrain++ )
	{
		fMax = max( fMax, GetMovementRate( iTerrain ) );
	}
/*
	if( GetMovementRate( TERRAIN_UNKNOWN ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_UNKNOWN ) );
	}
	if( GetMovementRate( TERRAIN_WATER ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_WATER ) );
	}
	if( GetMovementRate( TERRAIN_COAST ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_COAST ) );
	}
	if( GetMovementRate( TERRAIN_ARCTIC ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_ARCTIC ) );
	}
	if( GetMovementRate( TERRAIN_DESERT ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_DESERT ) );
	}
	if( GetMovementRate( TERRAIN_GRASS ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_GRASS ) );
	}
	if( GetMovementRate( TERRAIN_FOREST ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_FOREST ) );
	}
	if( GetMovementRate( TERRAIN_MOUNTAIN ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_MOUNTAIN ) );
	}
	if( GetMovementRate( TERRAIN_ROAD ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_ROAD ) );
	}
	if( GetMovementRate( TERRAIN_CITY ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_CITY ) );
	}
	if( GetMovementRate( TERRAIN_AIR ) > 0.0f )
	{
		fMax = max( fMax, GetMovementRate( TERRAIN_AIR ) );
	}
*/
	//ASSERT( fMax > 0.0f );
	return fMax;
}

/*
void  CMovementDescription::SetMovementMultiplier( long iTerrain, float fMult )
{
	m_MovementRateMultiplier.m_fTerrainValue[iTerrain] = fMult;
	if( iTerrain == TERRAIN_WATER )
	{
		// We don't actually use COAST values when calculating movement on
		//   the server.  It's only used to help ships avoid the coast.
		m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_COAST ] = fMult / 3.0f;
	}
}
*/

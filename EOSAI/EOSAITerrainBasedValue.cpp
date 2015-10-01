
#include "stdafx.h"
#include "EOSAITerrainBasedValue.h"
//#include "BCXMLReader.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAISerial.h"

void CEOSAITerrainBasedValue::Serialize( CEOSAISerial* pSerial )
{
	char cVersion = 2;
	pSerial->Serialize( cVersion );

	if( cVersion == 1 )
	{
		ASSERT( TERRAIN_COUNT == 11 );
		for( long i=0; i<11; i++ )
		{
			pSerial->Serialize( m_fTerrainValue[i] );
		}
	}
	else if( cVersion == 2 )
	{
		ASSERT( TERRAIN_COUNT == 12 );
		for( long i=0; i<12; i++ )
		{
			pSerial->Serialize( m_fTerrainValue[i] );
		}
	}
}

void CEOSAITerrainBasedValue::Deserialize( CEOSAISerial* pSerial )
{
	char cVersion = 1;
	pSerial->Deserialize( cVersion );

	if( cVersion == 1 )
	{
		//ASSERT( TERRAIN_COUNT == 12 );
		//for( long i=0; i<TERRAIN_COUNT; i++ )
		for( long i=0; i<11; i++ )
		{
			pSerial->Deserialize( m_fTerrainValue[i] );
		}
	}
	else if( cVersion == 2 )
	{
		ASSERT( TERRAIN_COUNT == 12 );
		for( long i=0; i<12; i++ )
		{
			pSerial->Deserialize( m_fTerrainValue[i] );
		}
	}
}
/*
void CTerrainBasedValue::AppendDataToXMLString( CStringANSI8& strData )
{
	ASSERT( false ); // Depreciate this?

	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		CStringANSI8 strText;
		strText.Format( ANSI8("<i%d>%0.4f</i%d>"), i, m_fTerrainValue[i], i );
		strData += strText;
	}
}
*/
/*
void CEOSAITerrainBasedValue::ReadXMLData( CBCXMLItem* pXMLItem )
{
	/-*
	POSITION pos = pXMLItem->m_Items.GetHeadPosition();
	while( pos )
	{
		CBCXMLItem* pChild = pXMLItem->m_Items.GetNext( pos );
		if( pChild->m
		if( pChild->m_strValue
	}
	*-/
	//for( long i=0; i<TERRAIN_COUNT; i++ )
	for( long i=0; i<11; i++ )
	{
		CStringANSI8 strText;
		strText.Format( ANSI8("i%d"), i );
		m_fTerrainValue[i] = (float) atof( pXMLItem->GetValue( strText ) );
	}
}
*/

void CEOSAITerrainBasedValue::Set( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_fTerrainValue[ i ] = pAIUnitTemplate->GetMovementRate( i );
	}	
}

void CEOSAITerrainBasedValue::Set( float fMovementRate )
{
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_fTerrainValue[ i ] = fMovementRate;
	}	
}

void CEOSAITerrainBasedValue::SetGroundUnitMovementRate( float fMovementRate )
{
	ASSERT( TERRAIN_COUNT == 12 );
	//m_fTerrainValue[ TERRAIN_UNKNOWN ] = fMovementRate;
	//m_fTerrainValue[ TERRAIN_WATER ] = fMovementRate;
	//m_fTerrainValue[ TERRAIN_COAST ] = fMovementRate;
	m_fTerrainValue[ TERRAIN_ARCTIC ] = fMovementRate;
	m_fTerrainValue[ TERRAIN_DESERT ] = fMovementRate;
	m_fTerrainValue[ TERRAIN_GRASS ] = fMovementRate;
	m_fTerrainValue[ TERRAIN_FOREST ] = fMovementRate;
	m_fTerrainValue[ TERRAIN_ROAD ] = fMovementRate;
	m_fTerrainValue[ TERRAIN_CITY ] = fMovementRate;
	m_fTerrainValue[ TERRAIN_MOUNTAIN ] = fMovementRate;
	//m_fTerrainValue[ TERRAIN_AIR ] = fMovementRate;
	m_fTerrainValue[ TERRAIN_SWAMP ] = fMovementRate;
}

void CEOSAITerrainBasedValue::operator=( CEOSAITerrainBasedValue& Other )
{
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_fTerrainValue[ i ] = Other.m_fTerrainValue[ i ];
	}
}

void CEOSAITerrainBasedValue::SetAllValuesToZero()
{
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_fTerrainValue[ i ] = 0.0f;
	}
}

void CEOSAITerrainBasedValue::SetAllValuesToOne()
{
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_fTerrainValue[ i ] = 1.0f;
	}
}

void CEOSAITerrainBasedValue::MultiplyAllValuesBy( float fValue )
{
	for( long i=0; i<TERRAIN_COUNT; i++ )
	{
		m_fTerrainValue[ i ] *= fValue;
	}
}

void CEOSAITerrainBasedValue::SetAllWaterValuesToOne()
{
	SetAllValuesToZero();
	m_fTerrainValue[ TERRAIN_WATER ] = 1.0f;
	m_fTerrainValue[ TERRAIN_COAST ] = 1.0f;
	m_fTerrainValue[ TERRAIN_UNKNOWN ] = 1.0f;
}

void CEOSAITerrainBasedValue::SetAllLandValuesToOne()
{
	SetAllValuesToZero();
	m_fTerrainValue[ TERRAIN_ARCTIC ] = 1.0f;
	m_fTerrainValue[ TERRAIN_DESERT ] = 1.0f;
	m_fTerrainValue[ TERRAIN_GRASS ] = 1.0f;
	m_fTerrainValue[ TERRAIN_FOREST ] = 1.0f;
	m_fTerrainValue[ TERRAIN_ROAD ] = 1.0f;
	m_fTerrainValue[ TERRAIN_MOUNTAIN ] = 1.0f;
	m_fTerrainValue[ TERRAIN_UNKNOWN ] = 1.0f;
}

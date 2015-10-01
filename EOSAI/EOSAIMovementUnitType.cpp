
#include "stdafx.h"
#include "EOSAIMovementUnitType.h"
//#include "BCString.h"
//#include "BCXMLReader.h"
//#include "EOSAISerial.h"

CEOSAIMovementUnitType g_UnitMovementTypeForRoadBuilding;

CEOSAIMovementUnitType::CEOSAIMovementUnitType()
{
	m_iMovementUnitType = 0;
	m_eMovementType = EOSAIEnumMovementType3_Land;
	m_MovementRateMultiplier.SetAllValuesToOne();
	m_MaintenenceCostMultiplier.SetAllValuesToOne();
}
/*
void CEOSAIUnit2MovementType::Initialize()
{
	g_MovementUnitTypeForRoadBuilding.SetToGenericGroundUnit();
}
*/
/*
void CEOSAIUnit2MovementType::Serialize( CEOSAISerial* pSerial )
{
	/-*
	char cVersion = 3;
	pSerial->Serialize( cVersion );

	if( cVersion == 1 )
	{
		pSerial->SerializeANSI8( m_strExternalName );
	}
	else if( cVersion >= 2 )
	{
		pSerial->SerializeUnicode( m_strExternalName );
	}
	else if( cVersion >= 3 )
	{
		pSerial->Serialize( m_iMovementUnitType );
		pSerial->SerializeUnicode( m_strExternalName );
	}
	*-/
	char cVersion = 4;
	pSerial->Serialize( cVersion );

	if( cVersion == 1 )
	{
		pSerial->SerializeANSI8( m_strExternalName );
	}
	else if( cVersion == 2 )
	{
		pSerial->SerializeUnicode( m_strExternalName );
	}
	else if( cVersion == 3 )
	{
		// This contained a bug, so I'm bumping up to version 4
		//pSerial->Serialize( m_iMovementUnitType );
		pSerial->SerializeUnicode( m_strExternalName );
	}
	else if( cVersion >= 4 )
	{
		pSerial->Serialize( m_iMovementUnitType );
		pSerial->SerializeUnicode( m_strExternalName );
	}

	char cMovementType = (char) m_eMovementType;
	pSerial->Serialize( cMovementType );

	m_MovementRateMultiplier.Serialize( pSerial );
	m_MaintenenceCostMultiplier.Serialize( pSerial );
}

void CEOSAIUnit2MovementType::Deserialize( CEOSAISerial* pSerial )
{
	/-*
	char cVersion = 1;
	pSerial->Deserialize( cVersion );

	if( cVersion == 1 )
	{
		pSerial->DeserializeANSI8( m_strExternalName );
	}
	else if( cVersion >= 2 )
	{
		pSerial->DeserializeUnicode( m_strExternalName );
	}
	else if( cVersion >= 3 )
	{
		pSerial->Deserialize( m_iMovementUnitType );
		pSerial->DeserializeUnicode( m_strExternalName );
	}
	*-/
	char cVersion = 0;
	pSerial->Deserialize( cVersion );

	if( cVersion == 1 )
	{
		pSerial->DeserializeANSI8( m_strExternalName );
	}
	else if( cVersion == 2 )
	{
		pSerial->DeserializeUnicode( m_strExternalName );
	}
	else if( cVersion == 3 )
	{
		//pSerial->Deserialize( m_iMovementUnitType );
		pSerial->DeserializeUnicode( m_strExternalName );
	}
	else if( cVersion >= 4 )
	{
		pSerial->Deserialize( m_iMovementUnitType );
		pSerial->DeserializeUnicode( m_strExternalName );
	}

	char cMovementType = 0;
	pSerial->Deserialize( cMovementType );
	m_eMovementType = (EOSAIEnumMovementType3) cMovementType;

	if( m_eMovementType == EOSAIEnumMovementType3_Undefined )
	{
		m_eMovementType = EOSAIEnumMovementType3_Land;
	}
	if( m_eMovementType == EOSAIEnumMovementType3_Land )
	{
		m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_SWAMP ] = 0.20f;
	}

	m_MovementRateMultiplier.Deserialize( pSerial );
	m_MaintenenceCostMultiplier.Deserialize( pSerial );
}

void  CEOSAIUnit2MovementType::AppendDataToXMLString( CStringANSI8& strData )
{
	ASSERT( false ); // depreciate?

	strData += ANSI8("<MovementUnitType>");
	strData += ANSI8("<Name>") + CStringANSI8( m_strExternalName ) + ANSI8("</Name>");

	//strData += "<MovementType>" + CBCString::LongAsString( m_eMovementType ) + "</MovementType>";
	strData += ANSI8("<MovementType>");
	if( m_eMovementType == EOSAIEnumMovementType3_Air ){ strData += ANSI8("Air"); } // 1
	if( m_eMovementType == EOSAIEnumMovementType3_Land ){ strData += ANSI8("Land"); } // 2
	if( m_eMovementType == EOSAIEnumMovementType3_Water ){ strData += ANSI8("Water"); } // 3
	strData += ANSI8("</MovementType>");

	strData += ANSI8("<MovementRateMult>");
//	m_MovementRateMultiplier.AppendDataToXMLString( strData );
	strData += ANSI8("</MovementRateMult>");

	strData += ANSI8("<MaintenenceCostMult>");
//	m_MaintenenceCostMultiplier.AppendDataToXMLString( strData );
	strData += ANSI8("</MaintenenceCostMult>");

	strData += ANSI8("</MovementUnitType>");
}

void  CEOSAIUnit2MovementType::ReadXMLData( CBCXMLItem* pXMLItem )
{
	m_strExternalName = pXMLItem->GetValue( ANSI8("Name") );
	CString strMovementType; strMovementType = pXMLItem->GetValue( ANSI8("MovementType") );
	//m_eMovementType = (EnumMovementType) _ttol( pXMLItem->GetValue( "MovementType" ) );
	if( strMovementType == ANSI8("Air") ){ m_eMovementType = EOSAIEnumMovementType3_Air; }
	if( strMovementType == ANSI8("Land") ){ m_eMovementType = EOSAIEnumMovementType3_Land; }
	if( strMovementType == ANSI8("Water") ){ m_eMovementType = EOSAIEnumMovementType3_Water; }

	CBCXMLItem* pMovementRateMult = pXMLItem->GetItem( ANSI8("MovementRateMult") );
	m_MovementRateMultiplier.ReadXMLData( pMovementRateMult );

	CBCXMLItem* pMaintenenceCostMult = pXMLItem->GetItem( ANSI8("MaintenenceCostMult") );
	m_MaintenenceCostMultiplier.ReadXMLData( pMaintenenceCostMult );
}
*/
void CEOSAIMovementUnitType::SetToGenericGroundUnit()
{
	m_eMovementType = EOSAIEnumMovementType3_Land;
	//m_fMovementRate = 1.0f;
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

void CEOSAIMovementUnitType::SetToGenericGroundUnitAvoidMountains()
{
	m_eMovementType = EOSAIEnumMovementType3_Land;
	//m_fMovementRate = 1.0f;
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

void CEOSAIMovementUnitType::SetToGenericSeaUnit()
{
	m_eMovementType = EOSAIEnumMovementType3_Water;
	//m_fMovementRate = 1.0f;
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

float CEOSAIMovementUnitType::GetMovementRateMultiplier( bool bOnRoad, long iUnderlyingTerrain )
{
	if( bOnRoad ){ return m_MovementRateMultiplier.m_fTerrainValue[ TERRAIN_ROAD ]; }
	return m_MovementRateMultiplier.m_fTerrainValue[ iUnderlyingTerrain ];
}




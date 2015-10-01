
#pragma once

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

//class CBCXMLItem;
class CEOSAIUnitTemplate;
class CEOSAISerial;

class CEOSAITerrainBasedValue
{
	public:
		CEOSAITerrainBasedValue(){ SetAllValuesToZero(); }
		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );
		//void  AppendDataToXMLString( CStringANSI8& strData );
		//void  ReadXMLData( CBCXMLItem* pXMLItem );

		void  Set( CEOSAIUnitTemplate* pAIUnitTemplate );
		void  Set( float fMovementRate );
		void  SetGroundUnitMovementRate( float fMovementRate );

		void  operator=( CEOSAITerrainBasedValue& Other );

		//void   SetToZero();
		//void   SetToOne();
		void   SetAllValuesToZero();
		void   SetAllValuesToOne();
		void   MultiplyAllValuesBy( float fValue );

		void   SetAllWaterValuesToOne();
		void   SetAllLandValuesToOne();

		float  m_fTerrainValue[TERRAIN_COUNT];
};



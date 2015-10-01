
#pragma once

#include "EOSAITerrainBasedValue.h"
#include "EOSAIEnumMovementType.h"
class CBCXMLItem;
class CEOSAISerial;
typedef ATL::CStringT< char, StrTraitMFC_DLL<char> >  CStringANSI8;

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIMovementUnitType
{
	//public:
	//	CMovementUnitType::Initialize();

	public:
		CEOSAIMovementUnitType();

		//void Serialize( CEOSAISerial* pSerial );
		//void Deserialize( CEOSAISerial* pSerial );
		//void   AppendDataToXMLString( CStringANSI8& strData );
		//void   ReadXMLData( CBCXMLItem* pXMLItem );

		void   SetToGenericGroundUnit();
		void   SetToGenericGroundUnitAvoidMountains();
		void   SetToGenericSeaUnit();

		float  GetMovementRateMultiplier( bool bOnRoad, long iUnderlyingTerrain );
		void   SetMovementRateMultiplier( long iTerrain, float fVal ){ m_MovementRateMultiplier.m_fTerrainValue[ iTerrain ] = fVal; }
		float  GetMaintenenceCostMultiplier( long iTerrain ){ return m_MaintenenceCostMultiplier.m_fTerrainValue[ iTerrain ]; }
		void   SetMaintenenceCostMultiplier( long iTerrain, float fVal ){ m_MaintenenceCostMultiplier.m_fTerrainValue[ iTerrain ] = fVal; }

		long    m_iMovementUnitType; // auto-assigned
		CString m_strExternalName;
		EOSAIEnumMovementType3  m_eMovementType; // Three types: Land, Air, Water

	//private:
		//CMovementDescription  m_MovementDescription;
		CEOSAITerrainBasedValue    m_MovementRateMultiplier;
		CEOSAITerrainBasedValue    m_MaintenenceCostMultiplier;
};

extern CEOSAIMovementUnitType  g_AIMovementUnitTypeForRoadBuilding;

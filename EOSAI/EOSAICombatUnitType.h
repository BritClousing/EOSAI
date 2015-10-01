
#pragma once

// CombatUnitType is described by the unitset.desc file.  It describes the defensive multipliers
//   for each CombatUnitType (Soft,Armor,Aircraft,Missile,Ship,Submarine)

#include "EOSAITerrainBasedValue.h"
//#include "GLColor.h"
//class CBCXMLItem;
//class CEOSAISerial;

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAICombatUnitType
{
	public:
		CEOSAICombatUnitType();

	//	void Serialize( CEOSAISerial* pSerial );
	//	void Deserialize( CEOSAISerial* pSerial );
		//void AppendDataToXMLString( CStringANSI8& strData );
	//	void ReadXMLData( CBCXMLItem* pXMLItem );

		float GetDefenseMult( long iTerrain ){ return m_DefensiveValueMultiplier.m_fTerrainValue[ iTerrain ]; }
		//float GetDefenseMultiplier( bool bInFort, bool bEntrenched, long iUnderlyingTerrain );

		//bool      HasAColor();
		//CGLColor  GetColor();

		long    m_iCombatUnitType;
		CString m_strExternalName;

		bool    m_bCanBlockAndBeBlocked;
		long    m_iDrawOrder;
		float   m_fFortificationDefenseMultiplier;

		long    m_iLandedOrDockedCombatType;
		CString m_strLandedOrDockedCombatType_Old; // used for backward compatibility

		//float   m_fLandedOrDockedMultiplier;
		float   m_fEntrenchedDefenseMultiplierBonus;

		//CString m_strColor;

		CEOSAITerrainBasedValue  m_DefensiveValueMultiplier;
};

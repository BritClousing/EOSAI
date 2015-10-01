
#pragma once
#include "EOSAISerial.h"
class CEOSAIUnitTemplate;

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIAttackVs2
{
	public:
		CEOSAIAttackVs2();
		//
		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );
		//
		//void  AppendDataToXMLString( CStringANSI8& strData );
		//void  ReadXMLData( CBCXMLItem* pXMLItem );
		//
		bool  operator==( CEOSAIAttackVs2& AttackVs2 );
		bool  operator!=( CEOSAIAttackVs2& AttackVs2 ){ return ( (*this)==AttackVs2 ) == false; }
		//
		//CString  GetCombatUnitTypeString();
		long     GetCombatUnitTypeLong(){ return m_iAttackVsCombatUnitType; }
		void     SetCombatUnitTypeByExternalName( CString str );
		CString  GetCombatUnitTypeString();//{ return m_strCombatUnitType; }
		//
		//EnumPoiObjectType GetUnitType(){ return m_eCombatUnitType; }
		bool  CanTarget(){ return ( m_fToHit > 0.0f || m_bAlwaysHits ) && m_iDamage > 0; }

		//void     SetPauseBetweenAnimations( CString strPauses ); 
		//CString  GetPauseBetweenAnimationsString(); 

		//float GetRange(){ return m_fAdditionalRange; }
		//bool  GetOABL(){ return m_bOneAttackBetweenLandings; }
		bool  GetOABL();
		float GetToHit(){ return m_fToHit; }
		bool  GetAlwaysHits(){ return m_bAlwaysHits; }
		long  GetDamage(){ return m_iDamage; }
		long  CalculateDamage(); // m_iDamage + (maybe) m_iExtraDamage
		float GetRateOfFire(){ return m_fRateOfFire; }

		void  SetAttackRangeIsDirectlyOverTarget( bool b );
		bool  GetAttackRangeIsDirectlyOverTarget(){ return m_bStandardAttackRange == false && m_fAttackRange < 5.0f; }
		float GetAttackRange(){ return m_fAttackRange; }
		float GetFullAttackRange();
		//float GetAttackRange(){ if( m_bAttackRangeIsDirectlyOverTarget ) return 0.0f; return m_fAttackRange; }

	//	bool  IsWithinMeleeRange( CEOSAILocation Location1, CEOSAILocation Location2 );//, EnumAttackGoal eGoal );
	//	bool  IsWithinCombatRange( CEOSAILocation Location1, CEOSAILocation Location2, EnumAttackGoal eGoal );

		CEOSAIUnitTemplate* m_pParentAIUnitTemplate;

		long     m_iAttackVsCombatUnitType;
		//CString  m_strCombatUnitType;
		CString  m_strAnimation;

		// Units have a range of "Standard + AdditionalRange" or simply "AdditionalRange"
		//bool    m_bStandardRangePlus;
		//float   m_fAdditionalRange;

		//bool    m_bOneAttackBetweenLandings;
		float   m_fToHit;
		bool    m_bAlwaysHits;
		long    m_iDamage;
		long    m_iExtraDamagePercentage; // 0-100
		long    m_iExtraDamage;

		bool    m_bStandardAttackRange;  // ~36 pixels
		float   m_fAttackRange;
		float   m_fRateOfFire;

		long    m_iAircraftTargetPriority;
};

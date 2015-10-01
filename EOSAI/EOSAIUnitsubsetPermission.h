
#pragma once

//#include "Unitset.h"
//class CBCXMLItem;
class CEOSAISerial;
namespace EOSAI
{
	class CGameRules;
};

class CEOSAIUnitsubsetPermission
{
	public:
		CEOSAIUnitsubsetPermission(){ m_pGameRules = NULL; m_bListContainsPositiveItems = false; }
//		void  SetUnitset( CUnitset* pUnitset ){ m_pUnitset = pUnitset; }

//		void  Copy( CItemAndUnitsubset* pItemPlus );

		void  SetUnitsubsetToPositive( CString strUnitsubset );
		void  SetUnitsubsetToNegative( CString strUnitsubset );
		bool  UnitsubsetIsPositive( CString strUnitsubset );

		void  ToggleState( CString strUnitsubset );
		void  SwitchPositiveNegative();

		long  GetNumberOfPositiveUnitsubsets();
		bool  AllUnitsubsetsArePositive();

		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );

		//void  ReadXMLData( CBCXMLItem* pItem );
		//void  AppendDataToXMLString( CStringANSI8& strData );

		EOSAI::CGameRules* m_pGameRules;
		//CUnitset* m_pUnitset;
		//CString   m_strItemName;
		bool      m_bListContainsPositiveItems;

		bool  ExistsInList( CString strUnitsubset );
		CStringList  m_Unitsubsets;
};

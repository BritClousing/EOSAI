
#pragma once

class CEOSAISerial;

namespace EOSAI
{
class StringAndLong
{
	public:
		StringAndLong(){ m_iValue = 0; }
		StringAndLong( CString str, long iValue ){ m_str = str; m_iValue = iValue; }
		//
		CString m_str;
		long    m_iValue;
};

class StringAndLongSet
{
	public:
		StringAndLongSet(){}
		~StringAndLongSet(){ Clear(); }

		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );

		void  Clear(){ while( m_List.IsEmpty() == FALSE ){ delete m_List.RemoveHead(); } }

		void  operator=( EOSAI::StringAndLongSet& Other ){ Set( Other ); }

		void  Add( CString strText, long iDelta );
		long  GetValue( CString strText ); // returns 0 for non-existent strText values
		void  Set( EOSAI::StringAndLongSet& Other );
		void  Set( CString strText, long iValue );

		CList< EOSAI::StringAndLong* >  m_List;
};
}

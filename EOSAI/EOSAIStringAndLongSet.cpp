
#include <stdafx.h>
#include "EOSAIStringAndLongSet.h"
#include "EOSAISerial.h"

using namespace EOSAI;

void  StringAndLongSet::Serialize( CEOSAISerial* pSerial )
{
	long iCount = m_List.GetCount();
	pSerial->Serialize( iCount );

	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndLong* pStringAndLong = m_List.GetNext( pos );
		pSerial->SerializeANSI8( pStringAndLong->m_str );
		pSerial->Serialize( pStringAndLong->m_iValue );
	}
}

void  StringAndLongSet::Deserialize( CEOSAISerial* pSerial )
{
	long iCount = 0;
	pSerial->Deserialize( iCount );

	for( long i=0; i<iCount; i++ )
	{
		EOSAI::StringAndLong* pStringAndLong = new EOSAI::StringAndLong();
		pSerial->DeserializeANSI8( pStringAndLong->m_str );
		pSerial->Deserialize( pStringAndLong->m_iValue );
		m_List.AddTail( pStringAndLong );
	}
}

void  StringAndLongSet::Add( CString strText, long iDelta )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		EOSAI::StringAndLong* pStringAndLong = m_List.GetNext( pos );
		if( pStringAndLong->m_str == strText )
		{
			pStringAndLong->m_iValue += iDelta;
			if( pStringAndLong->m_iValue == 0 )
			{
				delete pStringAndLong;
				m_List.RemoveAt( prevPos );
			}
			return;
		}
	}
	EOSAI::StringAndLong* pStringAndLong = new EOSAI::StringAndLong( strText, iDelta );
	m_List.AddTail( pStringAndLong );
}

long  StringAndLongSet::GetValue( CString strText )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		EOSAI::StringAndLong* pStringAndLong = m_List.GetNext( pos );
		if( pStringAndLong->m_str == strText )
		{
			return pStringAndLong->m_iValue;
		}
	}
	return 0;
}

void  StringAndLongSet::Set( CString strText, long iValue )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndLong* pStringAndLong = m_List.GetNext( pos );
		if( pStringAndLong->m_str == strText )
		{
			pStringAndLong->m_iValue = iValue;
			return;
		}
	}
	EOSAI::StringAndLong* pStringAndLong = new EOSAI::StringAndLong( strText, iValue );
	m_List.AddTail( pStringAndLong );
}

void  StringAndLongSet::Set( EOSAI::StringAndLongSet& Other )
{
	Clear();
	POSITION pos = Other.m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndLong* pOtherStringAndLong = Other.m_List.GetNext( pos );

		EOSAI::StringAndLong* pNewStringAndLong = new EOSAI::StringAndLong( pOtherStringAndLong->m_str, pOtherStringAndLong->m_iValue );
		m_List.AddTail( pNewStringAndLong );
	}
}


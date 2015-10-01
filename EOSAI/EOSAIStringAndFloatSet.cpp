
#include <stdafx.h>
#include "EOSAIStringAndFloatSet.h"

using namespace EOSAI;

void  StringAndFloatSet::Clear()
{
	while( m_List.IsEmpty() == FALSE ){ delete m_List.RemoveHead(); }
}
/*
void  StringAndFloatSet::operator=( EOSAI::StringAndFloatSet& Other )
{
	Clear();

	POSITION pos = Other.m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = Other.m_List.GetNext( pos );
		m_List.AddTail( new EOSAI::StringAndFloat( pObj->m_strValue, pObj->m_fValue ) );
	}
}
*/
void  StringAndFloatSet::Set( StringAndFloatSet& Other )
{
	Clear();
	POSITION pos = Other.m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = Other.m_List.GetNext( pos );
		m_List.AddTail( new EOSAI::StringAndFloat( pObj->m_strValue, pObj->m_fValue ) );
	}
}
/*
void  EOSAI::StringAndFloatSet::operator+=( EOSAI::StringAndFloatSet& Other )
{
	POSITION pos = Other.m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = Other.m_List.GetNext( pos );
		Add( pObj->m_strValue, pObj->m_fValue );
	}
}
*/
void  StringAndFloatSet::operator-=( StringAndFloatSet& Other )
{
	POSITION pos = Other.m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = Other.m_List.GetNext( pos );
		Add( pObj->m_strValue, -pObj->m_fValue );
	}
}


void  StringAndFloatSet::Set( CString str, float f )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		if( pObj->m_strValue == str )
		{
			pObj->m_fValue = f;
			return;
		}
	}
	m_List.AddTail( new EOSAI::StringAndFloat( str, f ) );
}

void  StringAndFloatSet::Add( CString str, float f )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		if( pObj->m_strValue == str )
		{
			pObj->m_fValue += f;
			return;
		}
	}
	m_List.AddTail( new EOSAI::StringAndFloat( str, f ) );
}

void  StringAndFloatSet::Add( EOSAI::StringAndFloatSet& Other )
{
	POSITION pos = Other.m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = Other.m_List.GetNext( pos );
		Add( pObj->m_strValue, pObj->m_fValue );
	}
}

bool  StringAndFloatSet::Exists( CString str )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		if( pObj->m_strValue == str ){ return true; }
	}
	return false;
}

float StringAndFloatSet::Get( CString str )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		if( pObj->m_strValue == str ){ return pObj->m_fValue; }
	}
	return 0.0f;
}

void StringAndFloatSet::Remove( CString str )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		if( pObj->m_strValue == str )
		{
			m_List.RemoveAt( prevPos );
		}
	}
}

void  StringAndFloatSet::Multiply( float f )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		pObj->m_fValue *= f;
	}
}

void  StringAndFloatSet::NothingBelowZero()
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		if( pObj->m_fValue < 0.0f ){ pObj->m_fValue = 0.0f; }
	}
}

float StringAndFloatSet::GetHighestValue()
{
	float f = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		if( pObj->m_fValue > f ){ f = pObj->m_fValue; }
	}
	return f;
}

float StringAndFloatSet::GetCummulativeValue()
{
	float f = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pObj = m_List.GetNext( pos );
		f += pObj->m_fValue;
	}
	return f;
}

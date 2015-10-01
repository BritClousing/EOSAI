
#include "stdafx.h"

#include "EOSAIIntSet.h"
#include "EOSAISerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void CEOSAIIntSet::Serialize( CEOSAISerial* pSerial )
{
	long iCount = (long) m_IntList.GetCount();
	pSerial->Serialize( iCount );

	POSITION pos = m_IntList.GetHeadPosition();
	while( pos )
	{
		long i = m_IntList.GetNext( pos );
		pSerial->Serialize( i );
	}
}

void CEOSAIIntSet::Deserialize( CEOSAISerial* pSerial )
{
	Clear();
	long iCount;
	pSerial->Deserialize( iCount );

	for( long i=0; i<iCount; i++ )
	{
		long x;
		pSerial->Deserialize( x );
		//Add( x );
		m_IntList.AddTail( x );
	}
}

void CEOSAIIntSet::Set( CEOSAIIntSet& other )
{
	Clear();
	POSITION pos = other.GetList()->GetHeadPosition();
	while( pos )
	{
		int j = other.GetList()->GetNext(pos);
		m_IntList.AddTail( j );
	}
}
/*
void CEOSAIIntSet::operator=( CEOSAIIntSet& other )
{
	Clear();
	POSITION pos = other.GetList()->GetHeadPosition();
	while( pos )
	{
		int j = other.GetList()->GetNext(pos);
		m_IntList.AddHead( j );
	}
}
*/
bool CEOSAIIntSet::operator==( CEOSAIIntSet& other )
{
	if( GetSize() != other.GetSize() ){ return false; }
	POSITION pos1 = this->m_IntList.GetHeadPosition();
	POSITION pos2 = other.m_IntList.GetHeadPosition();
	while( pos1 )
	{
		int i1 = this->m_IntList.GetNext( pos1 );
		int i2 = other.m_IntList.GetNext( pos2 );
		if( i1 != i2 ) return false;
	}
	return true;
}

void CEOSAIIntSet::Add( int i )
{
	// Check if [i] already exists in the list
	POSITION pos = m_IntList.GetTailPosition();
	while( pos )
	{
		int j = m_IntList.GetPrev(pos);
		if( i == j ){ return; }
	}
	bool bWasAdded = false;
	if( m_IntList.IsEmpty() == FALSE )
	{
		if( m_IntList.GetTail() < i ){ m_IntList.AddTail( i ); return; }
	}
	pos = m_IntList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		int j = m_IntList.GetNext(pos);
		//if( i > j ){ m_IntList.InsertAfter( prevPos, i ); bWasAdded = true; break; }
		if( i < j ){ m_IntList.InsertBefore( prevPos, i ); return; }
	}
	if( bWasAdded == false ){ m_IntList.AddTail( i ); }
	//m_IntList.AddHead( i );
}

void CEOSAIIntSet::AddRange( int iStart, int iEnd )
{
	ASSERT( iStart <= iEnd );
	ASSERT( iEnd-iStart <= 100 );
	if( iStart <= iEnd && iEnd-iStart <= 100 )
	{
		for( int i=iStart; i<=iEnd; i++ )
		{
			Add( i );
		}
	}
}

/*
void CEOSAIIntSet::AddSet( CEOSAIIntSet* pSet )
{
	POSITION pos = pSet->GetList()->GetHeadPosition();
	while( pos )
	{
		int i = pSet->GetList()->GetNext( pos );
		Add( i );
	}
}
*/
/*
void CEOSAIIntSet::ForceAdd( int i ) // use only when you know the value doesn't exist in list
{
	m_IntList.AddHead( i );
}
*/
void CEOSAIIntSet::Remove( int i )
{
	POSITION pos = m_IntList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		int j = m_IntList.GetNext(pos);
		if( i == j ){ m_IntList.RemoveAt( prevPos ); return; }
	}
}

bool CEOSAIIntSet::Exists( int i )
{
	POSITION pos = m_IntList.GetHeadPosition();
	while( pos )
	{
		int j = m_IntList.GetNext(pos);
		if( i == j ){ return true; }
	}
	return false;
}

bool CEOSAIIntSet::Intersects( CEOSAIIntSet& otherSet )
{
	POSITION pos1 = m_IntList.GetHeadPosition();
	while( pos1 )
	{
		int int1 = m_IntList.GetNext( pos1 );
		POSITION pos2 = otherSet.m_IntList.GetHeadPosition();
		while( pos2 )
		{
			int int2 = otherSet.m_IntList.GetNext( pos2 );
			if( int1 == int2 ){ return true; }
		}
	}
	return false;
}

long CEOSAIIntSet::GetNumberOfIntersections( CEOSAIIntSet& otherSet )
{
	long iIntersections = 0;
	POSITION pos1 = m_IntList.GetHeadPosition();
	while( pos1 )
	{
		int int1 = m_IntList.GetNext( pos1 );
		POSITION pos2 = otherSet.m_IntList.GetHeadPosition();
		while( pos2 )
		{
			int int2 = otherSet.m_IntList.GetNext( pos2 );
			if( int1 == int2 ){ iIntersections++; }
		}
	}
	return iIntersections;
}

void CEOSAIIntSet::Clear()
{
	m_IntList.RemoveAll();
}

void CEOSAIIntSet::ReadCommaDelimitedText( CString str )
{
	Clear();

	if( str == _T("") ) return;

	long iPos = 0;
	while( iPos != -1 )
	{
		long iValue = _ttol( str );
		m_IntList.AddTail( iValue );
		//
		iPos = str.Find( _T(",") );
		if( iPos == -1 ){ return; }
		//
		iPos++;
		str = str.Right( str.GetLength()-iPos );
	}
	int x=0;
}

CString CEOSAIIntSet::WriteCommaDelimitedText()
{
	CString strData, strTemp;
	POSITION pos = m_IntList.GetHeadPosition();
	while( pos )
	{
		int i = m_IntList.GetNext( pos );
		strTemp.Format( _T("%d"), i );
		strData += strTemp;
		if( pos != NULL ){ strData += _T(","); }
	}
	return strData;
}

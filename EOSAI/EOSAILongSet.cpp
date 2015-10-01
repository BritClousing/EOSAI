
#include "stdafx.h"

#include "EOSAILongSet.h"
#include "EOSAISerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void CEOSAILongSet::Serialize( CEOSAISerial* pSerial )
{
	long iCount = (long) m_LongList.GetCount();
	pSerial->Serialize( iCount );

	POSITION pos = m_LongList.GetHeadPosition();
	while( pos )
	{
		long i = m_LongList.GetNext( pos );
		pSerial->Serialize( i );
	}
}

void CEOSAILongSet::Deserialize( CEOSAISerial* pSerial )
{
	Clear();
	long iCount;
	pSerial->Deserialize( iCount );

	for( long i=0; i<iCount; i++ )
	{
		long x;
		pSerial->Deserialize( x );
		//Add( x );
		m_LongList.AddTail( x );
	}
}

void CEOSAILongSet::operator=( CEOSAILongSet& other )
{
	Clear();
	POSITION pos = other.GetList()->GetHeadPosition();
	while( pos )
	{
		long j = other.GetList()->GetNext(pos);
		m_LongList.AddTail( j );
	}
}

bool CEOSAILongSet::operator==( CEOSAILongSet& other )
{
	if( GetSize() != other.GetSize() ){ return false; }
	POSITION pos1 = this->m_LongList.GetHeadPosition();
	POSITION pos2 = other.m_LongList.GetHeadPosition();
	while( pos1 )
	{
		long i1 = this->m_LongList.GetNext( pos1 );
		long i2 = other.m_LongList.GetNext( pos2 );
		if( i1 != i2 ) return false;
	}
	return true;
}

void CEOSAILongSet::Add( long i )
{
/*
	POSITION pos = m_LongList.GetHeadPosition();
	while( pos )
	{
		long j = m_LongList.GetNext(pos);
		if( i == j ){ return; }
	}
	m_LongList.AddHead( i );
*/
	POSITION pos = m_LongList.GetHeadPosition();
	while( pos )
	{
		int j = m_LongList.GetNext(pos);
		if( i == j ){ return; }
	}
	bool bWasAdded = false;
	if( m_LongList.IsEmpty() == FALSE )
	{
		if( m_LongList.GetTail() < i ){ m_LongList.AddTail( i ); return; }
	}
	pos = m_LongList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		int j = m_LongList.GetNext(pos);
		//if( i > j ){ m_IntList.InsertAfter( prevPos, i ); bWasAdded = true; break; }
		if( i < j ){ m_LongList.InsertBefore( prevPos, i ); return; }
	}
	if( bWasAdded == false ){ m_LongList.AddTail( i ); }
}

void CEOSAILongSet::Add( CEOSAILongSet& OtherList )
{
	POSITION pos2 = OtherList.GetList()->GetHeadPosition();
	while( pos2 )
	{
		long iObjectId = OtherList.GetList()->GetNext( pos2 );
		Add( iObjectId );
	}
}

/*
void CEOSAILongSet::ForceAdd( long i ) // use only when you know the value doesn't exist in list
{
	m_LongList.AddHead( i );
}
*/
void CEOSAILongSet::Remove( long i )
{
	POSITION pos = m_LongList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		int j = m_LongList.GetNext(pos);
		if( i == j ){ m_LongList.RemoveAt( prevPos ); return; }
	}
}

bool CEOSAILongSet::Exists( long i )
{
	POSITION pos = m_LongList.GetHeadPosition();
	while( pos )
	{
		int j = m_LongList.GetNext(pos);
		if( i == j ){ return true; }
	}
	return false;
}

bool CEOSAILongSet::Intersects( CEOSAILongSet& otherSet )
{
	POSITION pos1 = m_LongList.GetHeadPosition();
	while( pos1 )
	{
		int int1 = m_LongList.GetNext( pos1 );
		POSITION pos2 = otherSet.m_LongList.GetHeadPosition();
		while( pos2 )
		{
			int int2 = otherSet.m_LongList.GetNext( pos2 );
			if( int1 == int2 ){ return true; }
		}
	}
	return false;
}

void CEOSAILongSet::Clear()
{
	m_LongList.RemoveAll();
}




#pragma once

#include <afxtempl.h>
class CEOSAISerial;

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIIntSet
{
	public:
		CEOSAIIntSet(){}
		CEOSAIIntSet( int i ){ Add( i ); }
		CEOSAIIntSet( int i, int j ){ Add( i ); Add( j ); }
		CEOSAIIntSet( int i, int j, int k ){ Add( i ); Add( j ); Add( k ); }

		void Serialize( CEOSAISerial* pSerial );
		void Deserialize( CEOSAISerial* pSerial );

		void Set( CEOSAIIntSet& other );
		void operator=( CEOSAIIntSet& other ){ Set( other ); }
		bool operator==( CEOSAIIntSet& other );
		int  GetSize(){ return (int) m_IntList.GetCount(); }
		void Add( int i );
		void AddRange( int iStart, int iEnd );
		//void ForceAdd( int i ); // (optimization) use only when you know the value doesn't exist in list
		void Remove( int i );
		bool Exists( int i );
		bool Intersects( CEOSAIIntSet& otherSet );
		long GetNumberOfIntersections( CEOSAIIntSet& otherSet );
		void Clear();

		void    ReadCommaDelimitedText( CString str );
		CString WriteCommaDelimitedText();

		CList< int, int >* GetList(){ return &m_IntList; }

	private:
		CList< int, int > m_IntList;
};


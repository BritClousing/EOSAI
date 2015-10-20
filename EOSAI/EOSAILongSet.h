
#pragma once

#include <afxtempl.h>
class CEOSAISerial;

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAILongSet
{
	public:
		CEOSAILongSet(){}
		CEOSAILongSet( long i ){ Add( i ); }

		void Serialize( CEOSAISerial* pSerial );
		void Deserialize( CEOSAISerial* pSerial );

		void operator=( CEOSAILongSet& other );
		bool operator==( CEOSAILongSet& other );
		bool operator!=( CEOSAILongSet& other ){ return !( *this == other ); }

		bool IsEmpty(){ return m_LongList.IsEmpty() == TRUE; }
		long GetSize(){ return (long) m_LongList.GetCount(); }
		long GetCount(){ return (long) m_LongList.GetCount(); }
		void Add( long i );
		void Add( CEOSAILongSet& OtherList );
		//void ForceAdd( long i ); // use only when you know the value doesn't exist in list
		void Remove( long i );
		bool Exists( long i );
		bool Intersects( CEOSAILongSet& otherSet );
		void Clear();

		CList< long, long >* GetList(){ return &m_LongList; }

	private:
		CList< long, long > m_LongList;
};


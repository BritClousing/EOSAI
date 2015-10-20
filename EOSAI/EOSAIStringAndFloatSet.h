
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIStringAndFloat.h"

namespace EOSAI
{
class DLLIMPEXP StringAndFloatSet
{
	public:
		StringAndFloatSet(){}
		StringAndFloatSet( EOSAI::StringAndFloatSet& Other ){ *this = Other; }
		~StringAndFloatSet(){ Clear(); }

		void  Clear();

		void  operator=( EOSAI::StringAndFloatSet& Other ){ Set( Other ); }
		void  operator+=( EOSAI::StringAndFloatSet& Other ){ Add( Other ); }
		void  operator*=( float f ){ Multiply( f ); }
		void  operator-=( EOSAI::StringAndFloatSet& Other );

		void  Set( CString str, float f );
		void  Set( EOSAI::StringAndFloatSet& Other );
		void  Add( CString str, float f );
		void  Add( EOSAI::StringAndFloatSet& Other );
		bool  Exists( CString str );
		float Get( CString str );
		void  Remove( CString str );

		void  Multiply( float f );
		void  NothingBelowZero();
		float GetHighestValue();
		float GetCummulativeValue();

	//private:
		CList< EOSAI::StringAndFloat* >  m_List;
};
}


#pragma once

#include "EOSAIListSort.h"

template<class TYPE, class ARG_TYPE = TYPE& > class CEOSAIListOwned : public CEOSAIListSort<TYPE, ARG_TYPE> 
{
	public:
		~CEOSAIListOwned(){ while( !IsEmpty() ){ delete RemoveHead(); } }

		void DeleteAll(){ while( !IsEmpty() ){ delete RemoveHead(); } }
		void RemoveAll(){ ASSERT( false ); }
};


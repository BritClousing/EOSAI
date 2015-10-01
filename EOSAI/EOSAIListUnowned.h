
#pragma once

#include "afxtempl.h"
#include "EOSAIListSort.h"

//typedef CList<class TYPE, class ARG_TYPE> CEOSAIListUnowned<class TYPE, class ARG_TYPE>;

template<class TYPE, class ARG_TYPE = TYPE& > class CEOSAIListUnowned : public CEOSAIListSort<TYPE, ARG_TYPE> 
{};


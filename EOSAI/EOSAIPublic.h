
#pragma once

#define EOSAI_MAX_NUMBER_OF_PLAYERS 10
typedef ATL::CStringT< char, StrTraitMFC_DLL<char> >  CStringANSI8;

// This is used for serializing and deserializing objects
#define CEOSAISerial_INFORMATION_LONG( classid, currentclass ) \
	static void*   CEOSAISerialInstanciate( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ){ currentclass* pObj = new currentclass; if( pObj ){ pObj->Deserialize( pSerial, pWorldDesc ); } return pObj; } \
	static long    GetCEOSAISerialClassId(){ return classid; }

inline long EOSAIRoundToNearest( float x ){ if( x+0.5 > 0 ) return (long)(x+0.5); return (long)(x-0.5); }

#define eif else if


#pragma once

/*
#define MAX_NUMBER_OF_PLAYERS 10
typedef ATL::CStringT< char, StrTraitMFC_DLL<char> >  CStringANSI8;

// This is used for serializing and deserializing objects
#define CEOSAISerial_INFORMATION_LONG( classid, currentclass ) \
	static void*   CEOSAISerialInstanciate( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ){ currentclass* pObj = new currentclass; if( pObj ){ pObj->Deserialize( pSerial, pWorldDesc ); } return pObj; } \
	static long    GetCEOSAISerialClassId(){ return classid; }

inline long RoundToNearest( float x ){ if( x+0.5 > 0 ) return (long)(x+0.5); return (long)(x-0.5); }

#define eif else if
*/

// I put this into a class, rather than a namespace because as a class, I can change the values inside 
// "EOSSettings.cpp" without causing a complete recompile.  Also, I don't need to instanciate this class because
// all the members are static.
class EOSAISettings
{
	public:
		#ifdef _DEBUG
		// The release version always has active AI Players
		static bool m_bActiveAIPlayers;
		static bool m_bAIPlayerSubmitTurn;
		static bool m_bAIShouldAcceptAllTradeOffers;
		#endif _DEBUG

		static bool m_bAssertWhenAIPathwayFails;
		static bool m_bDoingTesting;

		//static bool m_bAIPlayerSubmitTurn;
};

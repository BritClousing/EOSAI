
#pragma once

#include "EOSAISimpleCharMap.h"
class CEOSAISerial;


#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIPlayerOwnershipMap
{
	public:
		CEOSAIPlayerOwnershipMap();

		void Serialize( CEOSAISerial* pSerial );
		void Deserialize( CEOSAISerial* pSerial );

		void Allocate( long iWidth, long iHeight, bool bWrapRight, bool bWrapBottom );
		void Clear();
		void Copy( CEOSAIPlayerOwnershipMap& PlayerMap );

		long  GetOwner( float fPixelX, float fPixelY );
		float GetDistanceToEscapeOwnedArea( float fPixelX, float fPixelY, long iOwner );

		long m_iTurn;
		CEOSAISimpleCharMap  m_Player;    // Owner (e.g. 1 = owned by Player 1)
		CEOSAISimpleCharMap  m_Ownership; // Ownership level (e.g. 200 = deep inside ownership area)
};


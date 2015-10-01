
#pragma once
//
// These are mail responses sent to or from the AI.

#include "EnumAIMailResponse.h"

class CEOSAIMailResponse
{
	public:
		CEOSAIMailResponse( long iFromPlayer, long iAIMessageUID, EOSAI::EnumAIMailResponse eResponse )
		{
			m_iToAIPlayer = 0;
			m_iAIMessageUID = iAIMessageUID;
			m_iFromPlayer = iFromPlayer;
			m_eResponse = eResponse;
		}
		~CEOSAIMailResponse(){}

		long m_iToAIPlayer;
		long m_iAIMessageUID;
		long m_iFromPlayer;
		EOSAI::EnumAIMailResponse  m_eResponse;
};



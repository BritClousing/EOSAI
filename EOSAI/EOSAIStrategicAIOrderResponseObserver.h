#pragma once

#include "EnumAIMailResponse.h"
#include "EOSAIIntSet.h"
#include "EOSAIMailResponse.h"

// Created so that CStategicOrder can track responses to multiple IMails
class CEOSAIStrategicOrderResponseObserver
{
public:
	CEOSAIStrategicOrderResponseObserver()
	{
		m_iAIMessageUID = 0;
		m_iTimerStart = 0;
		m_iTimerDuration = 0; // e.g. 60 seconds
	}
	virtual void                   SetResponse(long iFromPlayer, EOSAI::EnumAIMailResponse eResponse);
	virtual EOSAI::EnumAIMailResponse  GetResponse(long iFromPlayer);

	bool    AllResponsesReceived();
	bool    AllResponsesReceivedAndHaveApproved();

	long   m_iAIMessageUID;
	long   m_iTimerStart;
	long   m_iTimerDuration; // e.g. 60 seconds
	CList< CEOSAIMailResponse* >  m_Responses; // A list of EOSAI::EnumAIMailResponses from each player
	CEOSAIIntSet  m_PlayersIWantResponsesFrom;
};


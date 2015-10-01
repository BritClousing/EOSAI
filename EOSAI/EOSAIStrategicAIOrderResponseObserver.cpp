
#include <stdafx.h>
#include "EOSAIStrategicAIOrderResponseObserver.h"

void  CEOSAIStrategicOrderResponseObserver::SetResponse(long iFromPlayer, EOSAI::EnumAIMailResponse eResponse)
{
	// Make sure I want a response from this player, and I don't have a response from this player already
	if (m_PlayersIWantResponsesFrom.Exists(iFromPlayer) &&
		GetResponse(iFromPlayer) == EOSAI::EnumAIMailResponse_Undefined)
	{
		m_Responses.AddTail(new CEOSAIMailResponse(iFromPlayer, m_iAIMessageUID, eResponse));
	}
}

EOSAI::EnumAIMailResponse  CEOSAIStrategicOrderResponseObserver::GetResponse(long iFromPlayer)
{
	POSITION pos = m_Responses.GetHeadPosition();
	while (pos)
	{
		CEOSAIMailResponse* pResponse = m_Responses.GetNext(pos);
		if (pResponse->m_iFromPlayer == iFromPlayer)
		{
			return pResponse->m_eResponse;
		}
	}
	return EOSAI::EnumAIMailResponse_Undefined;
}

bool  CEOSAIStrategicOrderResponseObserver::AllResponsesReceived()
{
	POSITION pos = m_PlayersIWantResponsesFrom.GetList()->GetHeadPosition();
	while (pos)
	{
		long iPlayer = m_PlayersIWantResponsesFrom.GetList()->GetNext(pos);
		if (GetResponse(iPlayer) == EOSAI::EnumAIMailResponse_Undefined) return false;
	}
	return true;
}

bool  CEOSAIStrategicOrderResponseObserver::AllResponsesReceivedAndHaveApproved()
{
	POSITION pos = m_PlayersIWantResponsesFrom.GetList()->GetHeadPosition();
	while (pos)
	{
		long iPlayer = m_PlayersIWantResponsesFrom.GetList()->GetNext(pos);
		if (GetResponse(iPlayer) != EOSAI::EnumAIMailResponse_Approve) return false;
	}
	return true;
}


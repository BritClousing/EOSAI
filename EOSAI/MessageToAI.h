
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIIntSet.h"

// Messages to AI might be stuff like trade offers.

namespace EOSAI
{
class DLLIMPEXP MessageToAI
{
	public:
		MessageToAI(){}

		bool SendToAllPlayers() { return m_bSendToAllPlayers; }
		void SendToAllPlayers(bool b) { m_bSendToAllPlayers = b; } // send to all AIs?

		void SendToPlayer(int iPlayer) { m_SendToPlayers.Add(iPlayer); }
		CEOSAIIntSet* SendToPlayers() { return &m_SendToPlayers; }

		//virtual bool SendToAllAIs() { return false; } // send to all AIs?
		//bool m_bSendToAllAIs = false;
		//int  m_iSendToAIPlayer = 0; // Used if this is send to one specific player

	private:
		bool m_bSendToAllPlayers = false;
		CEOSAIIntSet m_SendToPlayers; // Example: a set of player numbers - e.g. player 1, player 4
		virtual void PolymorphicType(){}
		//int m_iSendToAIPlayer = 0;
};
}


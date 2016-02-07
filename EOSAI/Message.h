
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIIntSet.h"

// Messages to/from the AI. This can be iMessages, TradeOffers, DeclarationsOfWar, etc.

namespace EOSAI
{
	class DLLIMPEXP Message
	{
	public:
		Message(){}

		// Sent From
		void SetFrom(int iPlayer){ m_iSentFromPlayer = iPlayer; }
		int  GetFrom(){ return m_iSentFromPlayer; }

		// Send To
		void SetSendToAllPlayers(bool b) { m_bSendToAllPlayers = b; } // send to all AIs?
		void SetSendToPlayer(int iPlayer) { m_SendToPlayers.Add(iPlayer); }
		bool GetSendToAllPlayers() { return m_bSendToAllPlayers; }
		CEOSAIIntSet* GetSendToPlayers() { return &m_SendToPlayers; }

		//virtual bool SendToAllAIs() { return false; } // send to all AIs?
		//bool m_bSendToAllAIs = false;
		//int  m_iSendToAIPlayer = 0; // Used if this is send to one specific player

	private:
		int  m_iSentFromPlayer = 0;
		bool m_bSendToAllPlayers = false;
		CEOSAIIntSet m_SendToPlayers; // Example: a set of player numbers - e.g. player 1, player 4

		virtual void PolymorphicType(){}
		//int m_iSendToAIPlayer = 0;
	};
}


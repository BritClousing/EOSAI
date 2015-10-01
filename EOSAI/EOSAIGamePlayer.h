
//#pragma once
#ifndef EOSAI_GAME_PLAYER_H
#define EOSAI_GAME_PLAYER_H

// The GPlayer object represents one of the game players - which may be a human player or AI player
//   It is simply used to store some data about the player, like the player's name

//namespace EOSAI
//{
#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{

enum EnumGamePlayerType
{
	EnumGamePlayerType_Human,
	EnumGamePlayerType_AI
};

//class DLLIMPEXP CEOSAIGamePlayer
class DLLIMPEXP CGamePlayer
{
	public:
		CGamePlayer( long iPlayerNumber = 0 );
		/*
		CEOSAIGamePlayer( long iPlayerNumber = 0 )
		{
			m_iPlayer = iPlayerNumber;
			m_bIsAlive = true;
			m_ePlayerType = EOSAIEnumGamePlayerType_Human;
			m_bScenarioPlayerRestriction_CannotDeclareWar = false;
			m_bScenarioPlayerRestriction_CannotMakePeace = false;
		}
		*/
		long     GetTestNumber();
		//long     GetAIIntelligence(){ return m_iAIIntelligence; }
		CString  GetPlayerName(){ return m_strPlayerName; }
		long     GetPlayerNumber(){ return m_iPlayer; }
		bool     IsHuman(){ return m_ePlayerType == EOSAI::EnumGamePlayerType_Human; }
		bool     IsAI(){ return m_ePlayerType == EOSAI::EnumGamePlayerType_AI; }
		bool     IsAlive(){ return m_bIsAlive; }

		CString  GetUnitsubset(){ return m_strUnitsubset; }


	//
		long     m_iPlayer;
		CString  m_strPlayerName;
		bool     m_bIsAlive;

		EOSAI::EnumGamePlayerType  m_ePlayerType;
		//long     m_iIntelligence;

		bool     m_bScenarioPlayerRestriction_CannotDeclareWar;
		bool     m_bScenarioPlayerRestriction_CannotMakePeace;

		CString      m_strUnitsubset;
};
};

#endif EOSAI_GAME_PLAYER_H

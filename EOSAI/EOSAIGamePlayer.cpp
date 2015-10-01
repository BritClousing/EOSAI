
#include "stdafx.h"
#include "EOSAIGamePlayer.h"

using namespace EOSAI;

CGamePlayer::CGamePlayer( long iPlayerNumber )
{
	//ASSERT(false);

	m_iPlayer = iPlayerNumber;
	m_bIsAlive = true;
	m_ePlayerType = EOSAI::EnumGamePlayerType_Human;
	m_bScenarioPlayerRestriction_CannotDeclareWar = false;
	m_bScenarioPlayerRestriction_CannotMakePeace = false;
}

long CGamePlayer::GetTestNumber()
{
	return m_iPlayer;
}


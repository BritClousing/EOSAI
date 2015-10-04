
#include <stdafx.h>
#include "EOSAIStrategicAIOrder.h"
#include "EOSAIStrategicAI.h"
#include "EOSAICommonData.h"
#include "EOSAIMailResponse.h"
//#include "CommonState.h"
//#include "WorldDescPlayer.h"
//#include "GameEvent_IMail.h"
//#include "Player.h"

//
#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
//
//

EOSAI::AIPlayer* CEOSAIStrategicAIOrder::GetAIPlayer()
{
	return m_pStrategicAI->GetAIPlayer();
}

long  CEOSAIStrategicAIOrder::GetPlayerNumber()
{
	return m_pStrategicAI->GetPlayerNumber();
}

/*
CPlayer* CEOSAIStrategicAIOrder::GetPlayer()
{
	ASSERT( m_pStrategicAI );
	long iPlayer = m_pStrategicAI->GetPlayerNumber();
	return GetCommonState()->GetPlayer( iPlayer );
}

CWorldDescServer* CEOSAIStrategicAIOrder::GetWorldDescServer()
{
	return g_pCommonState->GetWorldDescServer();
}
*/
/*
CWorldDescPlayer* CEOSAIStrategicAIOrder::GetWorldDescPlayer()
{
	ASSERT( m_pStrategicAI );
	long iPlayer = m_pStrategicAI->GetPlayerNumber();
	return GetCommonState()->GetWorldDescPlayerByPlayerNumber( iPlayer );
}
*/
/*
CString  CEOSAIStrategicAIOrder::GetTargetPlayersString( CEOSAIIntSet* pPlayers )
{
	if( pPlayers->GetSize() == 0 )
	{
		ASSERT( false );
		return _T("");
	}

	CString strText;
	long iNumberOfTargets = pPlayers->GetSize();
	ASSERT( iNumberOfTargets > 0 );
	long iCurrent = 0;
	POSITION pos = pPlayers->GetList()->GetHeadPosition();
	while( pos )
	{
		iCurrent++;
		long iPlayer = pPlayers->GetList()->GetNext( pos );
		CPlayer* pPlayer = GetCommonState()->GetPlayer( iPlayer );
		if( pPlayer )
		{
			if( iNumberOfTargets == 1 )
			{
				strText += pPlayer->GetPlayerName();
			}
			else
			{
				strText += _T(", ");
				strText += pPlayer->GetPlayerName();
			}
			/-*
			else if( iNumberOfTargets == 2 )
			{
				if( iCurrent == 1 )
				{
					strText += pPlayer->GetPlayerName();
					strText += " and ";
				}
				else
				{
					strText += pPlayer->GetPlayerName();
				}
			}
			else if( iNumberOfTargets > 2 )
			{
				if( iCurrent < iNumberOfTargets-1 )
				{
					strText += pPlayer->GetPlayerName();
					strText += ", ";
				}
				else if( iCurrent == iNumberOfTargets-1 )
				{
					strText += pPlayer->GetPlayerName();
					strText += ", and ";
				}
				else
				{
					strText += pPlayer->GetPlayerName();
				}
			}
			*-/
		}
	}
	return strText;
}
*/

bool  CEOSAIStrategicAIOrder::AllPlayersExist( CEOSAIIntSet* pPlayers )
{
	POSITION pos = pPlayers->GetList()->GetHeadPosition();
	while( pos )
	{
		long iPlayer = pPlayers->GetList()->GetNext( pos );
		EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
		if( pPlayer == NULL ) return false;
	}
	return true;
}

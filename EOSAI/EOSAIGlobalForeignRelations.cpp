
#include <stdafx.h>
#include "EOSAIGlobalForeignRelations.h"
//#include "EOSAIPlayerInteractionEvent.h"
//#include "PlayerForeignRelations.h"
//#include "PlayerTeam.h"
//#include "PlayerForeignRelationsWithPlayer.h"
//#include "WorldDesc.h"
#include "EOSAIMath.h"
#include "EOSAIPublic.h"

CEOSAIGlobalForeignRelations::CEOSAIGlobalForeignRelations()
{
	//m_iNumberOfPlayers = 0;
	m_ForeignRelations.SetSize( EOSAI_MAX_NUMBER_OF_PLAYERS+1, EOSAI_MAX_NUMBER_OF_PLAYERS+1 );
	m_Feelings.SetSize( EOSAI_MAX_NUMBER_OF_PLAYERS+1, EOSAI_MAX_NUMBER_OF_PLAYERS+1 );

	for( long x=0; x<EOSAI_MAX_NUMBER_OF_PLAYERS+1; x++ )
	{
		for( long y=0; y<EOSAI_MAX_NUMBER_OF_PLAYERS+1; y++ )
		{
			m_ForeignRelations.Value( x,y ) = EOSAIEnumForeignRelations::enum_Neutral;
			m_Feelings.Value( x,y ) = 0.5f;
			if( x == y ){ m_ForeignRelations.Value( x,y ) = EOSAIEnumForeignRelations::enum_Self; }
		}
	}
}

CEOSAIGlobalForeignRelations::~CEOSAIGlobalForeignRelations()
{}

//
#ifdef DO_NOT_COMPILE
void CEOSAIGlobalForeignRelations::Serialize( CEOSAISerial* pSerial )
{
	//pSerial->Serialize( iForPlayer );
	//if( iForPlayer == 0 ) // serializing for the server (e.g. save game)
	//{
		// Store events
	//}

	//
	long iNumberOfPlayers = MAX_NUMBER_OF_PLAYERS;
	pSerial->Serialize( iNumberOfPlayers );
	//
	for( long iPlayer1=1; iPlayer1<=iNumberOfPlayers; iPlayer1++ )
	{
		for( long iPlayer2=1; iPlayer2<=iNumberOfPlayers; iPlayer2++ )
		{
			m_ForeignRelations.Value( iPlayer1, iPlayer2 ).Serialize( pSerial );

			float fFeelings = m_Feelings.Value( iPlayer1, iPlayer2 );
			pSerial->Serialize( fFeelings );
		}
	}
	//
	/*
	for( long i=1; i<=MAX_NUMBER_OF_PLAYERS; i++ )
	{
		CPlayerForeignRelations* pRel = m_Players[i];
		if( pRel )
		{
			pRel->Serialize( iForPlayer, pSerial );
		}
	}
	*/
	// Serialize Teams
	long iTeams = (long) m_Teams.GetCount();
	pSerial->Serialize( iTeams );
	POSITION pos = m_Teams.GetHeadPosition();
	while( pos )
	{
		CPlayerTeam* pTeam = m_Teams.GetNext( pos );
		pTeam->Serialize( pSerial );
	}
}

void CEOSAIGlobalForeignRelations::Deserialize( CEOSAISerial* pSerial )
{
	//long iForPlayer = 0;
	//pSerial->Deserialize( iForPlayer );
	//if( iForPlayer == 0 )
	//{
		// Deserialize events
	//}
	/*
	long iNumberOfPlayers;
	pSerial->Deserialize( iNumberOfPlayers );
	for( long i=0; i<iNumberOfPlayers; i++ )
	{
	}
	*/
	long iNumberOfSerializedPlayers = 0;
	pSerial->Deserialize( iNumberOfSerializedPlayers );
	//SetNumberOfPlayers( iNumberOfPlayers );

	//
	for( long iPlayer1=1; iPlayer1<=iNumberOfSerializedPlayers; iPlayer1++ )
	{
		for( long iPlayer2=1; iPlayer2<=iNumberOfSerializedPlayers; iPlayer2++ )
		{
			//long i = 0;
			//pSerial->Deserialize( i );
			//EOSAIEnumForeignRelations eRel = (EOSAIEnumForeignRelations) i;
			//m_ForeignRelations.Value( iPlayer1, iPlayer2 ) = eRel;
			m_ForeignRelations.Value( iPlayer1, iPlayer2 ).Deserialize( pSerial );

			float fFeelings;
			pSerial->Deserialize( fFeelings );
			m_Feelings.Value( iPlayer1, iPlayer2 ) = fFeelings;
		}
	}

	//
	/*
	for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		CPlayerForeignRelations* pRel = this->GetPlayerForeignRelations( iPlayer );
		if( pRel == NULL )
		{
			pRel = new CPlayerForeignRelations();
			m_Players[iPlayer] = pRel;
			//m_ForeignRelations.AddTail( pRel );
		}
		pRel->Deserialize( pSerial );
		ASSERT( pRel->m_iPlayer == iPlayer );
	}
	*/
	// Delete any existing teams
	while( m_Teams.IsEmpty() == FALSE ){ delete m_Teams.RemoveHead(); }

	// Deserialize Teams
	long iTeams = 0;
	pSerial->Deserialize( iTeams );
	for( long i=0; i<iTeams; i++ )
	{
		CPlayerTeam* pTeam = new CPlayerTeam();
		pTeam->Deserialize( pSerial );
		m_Teams.AddTail( pTeam );
	}
	UpdateForeignRelationsBasedOnTeams();
}
#endif DO_NOT_COMPILE

CString  CEOSAIGlobalForeignRelations::OutputDebugString()
{
	CString str;
	for( long x=1; x<m_ForeignRelations.m_iSizeX; x++ )
	{
		for( long y=1; y<m_ForeignRelations.m_iSizeY; y++ )
		{
			EOSAIEnumForeignRelations eRel = m_ForeignRelations.Value( x,y );
			if( eRel == EOSAIEnumForeignRelations::enum_Self )
			{
				str += _T("Self    ");
			}
			else if( eRel == EOSAIEnumForeignRelations::enum_Teammate )
			{
				str += _T("Team    ");
			}
			else if( eRel == EOSAIEnumForeignRelations::enum_Alliance )
			{
				str += _T("Ally    ");
			}
			else if( eRel == EOSAIEnumForeignRelations::enum_Neutral )
			{
				str += _T("Neutral ");
			}
			else if( eRel == EOSAIEnumForeignRelations::enum_War )
			{
				str += _T("War     ");
			}
			else
			{
				ASSERT( false );
			}
		}
		str += _T("\n");
	}
	return str;
}
/*
void CEOSAIGlobalForeignRelations::SetNumberOfPlayers( long iNumberOfPlayers )
{
	long iOldSize = m_iNumberOfPlayers;
	//bool bNumberOfPlayersWasZero = (m_iNumberOfPlayers == 0);
	bool bNumberOfPlayersChanged = (m_iNumberOfPlayers != iNumberOfPlayers);
	if( bNumberOfPlayersChanged == false ) return;

	if( iOldSize == 0 )
	{
		m_ForeignRelations.SetSize( iNumberOfPlayers+1, iNumberOfPlayers+1 );
		m_Feelings.SetSize( iNumberOfPlayers+1, iNumberOfPlayers+1 );
		for( long x=0; x<=iNumberOfPlayers; x++ )
		{
			for( long y=0; y<=iNumberOfPlayers; y++ )
			{
				m_ForeignRelations.Value( x,y ) = EOSAIEnumForeignRelations::enum_Neutral;
				m_Feelings.Value( x,y ) = 0.5f;

				if( x == y ){ m_ForeignRelations.Value( x,y ) = EOSAIEnumForeignRelations::enum_Self; }
			}
		}
	}
	if( iOldSize > 0 )
	{
		long iTempSize = min( iOldSize, iNumberOfPlayers );

		// Copy the old data to the new data block
		CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >  Temp;
		Temp.SetSize( iTempSize+1, iTempSize+1 );
		for( long x=0; x<=iTempSize; x++ )
		{
			for( long y=0; y<=iTempSize; y++ )
			{
				Temp.Value( x,y ) = m_ForeignRelations.Value( x,y );
			}
		}

		m_ForeignRelations.SetSize( iNumberOfPlayers+1, iNumberOfPlayers+1 );
		m_Feelings.SetSize( iNumberOfPlayers+1, iNumberOfPlayers+1 );
		for( long x=0; x<=iNumberOfPlayers; x++ )
		{
			for( long y=0; y<=iNumberOfPlayers; y++ )
			{
				m_ForeignRelations.Value( x,y ) = EOSAIEnumForeignRelations::enum_Neutral;
				m_Feelings.Value( x,y ) = 0.5f;
			}
		}

		for( long x=0; x<=iTempSize; x++ )
		{
			for( long y=0; y<=iTempSize; y++ )
			{
				m_ForeignRelations.Value( x,y ) = Temp.Value( x,y );

				if( x == y ){ m_ForeignRelations.Value( x,y ) = EOSAIEnumForeignRelations::enum_Self; }
			}
		}
	}
	m_iNumberOfPlayers = iNumberOfPlayers;

	/*
	for( long iPlayer=1; iPlayer<=MAX_NUMBER_OF_PLAYERS; iPlayer++ )
	{
		if( m_Players[iPlayer] == NULL && iPlayer<=iNumberOfPlayers )
		{
			CPlayerForeignRelations* pRelations = new CPlayerForeignRelations();
			pRelations->Initialize( iPlayer, iNumberOfPlayers );
			m_Players[iPlayer] = pRelations;
		}
		if( m_Players[iPlayer] != NULL && iPlayer>iNumberOfPlayers )
		{
			CPlayerForeignRelations* pRelations = m_Players[iPlayer];
			delete pRelations;
			m_Players[iPlayer] = NULL;
		}
	}
	*-/

	//ProcessApprovedTeams();
	/*
	// Delete any old information
	POSITION pos = m_Players.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CPlayerForeignRelations* pRelations = m_Players.GetNext( pos );
		if( pRelations->m_iPlayer > iNumberOfPlayers )
		{
			m_ForeignRelations.RemoveAt( prevPos );
			delete pRelations;
		}
	}

	for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		//if( GetLocalPlayerNumber() == i ) continue;
		if( GetPlayerForeignRelations( iPlayer ) ) continue;

		CPlayerForeignRelations* pRelations = new CPlayerForeignRelations();
		pRelations->Initialize( iPlayer, iNumberOfPlayers );
		m_Players.AddTail( pRelations );
	}
*-/
	/*
	m_Players.SetSize( iNumberOfPlayers+1 ); // +1 because we don't use the 0 player
	for( long iPlayer=0; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		if( iPlayer == 0 )
		{
			m_Players[iPlayer] = NULL;
		}
		else
		{
			m_Players[iPlayer] = new CForeignRelationsPlayerProxy();
			m_Players[iPlayer]->Initialize( iPlayer, iNumberOfPlayers );
		}
	}
	*-/
/*
	m_Players.SetSize( iNumberOfPlayers+1 ); // +1 because we don't use the 0 player
	for( long iPlayer=0; iPlayer<iNumberOfPlayers+1; iPlayer++ )
	{
		if( iPlayer == 0 )
		{
			m_Players[iPlayer] = NULL;
		}
		else
		{
			m_Players[iPlayer] = new CPlayerForeignRelations();
			m_Players[iPlayer]->Initialize( iPlayer, iNumberOfPlayers );
		}
	}
	*-/
}
*/
void  CEOSAIGlobalForeignRelations::Copy( CEOSAIGlobalForeignRelations* pGlobalForeignRelations )
{
	//for( long x=0; x<=m_iNumberOfPlayers; x++ )
	for( long x=0; x<m_ForeignRelations.m_iSizeX; x++ )
	{
		//for( long y=0; y<=m_iNumberOfPlayers; y++ )
		for( long y=0; y<m_ForeignRelations.m_iSizeY; y++ )
		{
			m_ForeignRelations.Value( x,y ) = pGlobalForeignRelations->m_ForeignRelations.Value( x,y );
			m_Feelings.Value( x,y ) = pGlobalForeignRelations->m_Feelings.Value( x,y );
		}
	}
	/*
	for( long iPlayer=0; iPlayer<=MAX_NUMBER_OF_PLAYERS; iPlayer++ )
	{
		if( pGlobalForeignRelations->m_Players[iPlayer] ){ ASSERT( m_Players[iPlayer] ); }
		if( m_Players[iPlayer] ){ ASSERT( pGlobalForeignRelations->m_Players[iPlayer] ); }

		if( pGlobalForeignRelations->m_Players[iPlayer] )
		{
			m_Players[iPlayer]->Copy( pGlobalForeignRelations->m_Players[iPlayer] );
		}
	}
	*/
}


void  CEOSAIGlobalForeignRelations::SetAllPlayersToWar()
{
	long iSize = min( EOSAI_MAX_NUMBER_OF_PLAYERS+1, m_ForeignRelations.m_iSizeX );
	for( long iPlayer1=1; iPlayer1<iSize; iPlayer1++ )
	{
		for( long iPlayer2=1; iPlayer2<iSize; iPlayer2++ )
		{
			if( iPlayer1 == iPlayer2 )
			{
				m_ForeignRelations.Value( iPlayer1,iPlayer2 ) = EOSAIEnumForeignRelations::enum_Self;
			}
			else
			{
				m_ForeignRelations.Value( iPlayer1,iPlayer2 ) = EOSAIEnumForeignRelations::enum_War;
			}
		}
	}
	//POSITION pos = m_Teams.GetHeadPosition();
	//while( pos )
	//{
	//	CPlayerTeam* pTeam = m_Teams.GetNext( pos );
	//	int g=0;
	//}
}

void  CEOSAIGlobalForeignRelations::SetForeignRelations( CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pRelationsArray )
{
	long iSize = min( EOSAI_MAX_NUMBER_OF_PLAYERS+1, pRelationsArray->m_iSizeX );
	for( long iPlayer1=0; iPlayer1<iSize; iPlayer1++ )
	{
		for( long iPlayer2=0; iPlayer2<iSize; iPlayer2++ )
		{
			m_ForeignRelations.Value( iPlayer1,iPlayer2 ) = pRelationsArray->Value( iPlayer1,iPlayer2 );
		}
	}
}

void  CEOSAIGlobalForeignRelations::SetForeignRelations( long iActor, long iTarget, EOSAIEnumForeignRelations eRel )
{
	ASSERT( iActor != iTarget );
	//
/*
	CPlayerForeignRelations* pRel = GetPlayerForeignRelations( iActor );
	ASSERT( pRel );
	if( pRel == NULL ) return;
	CPlayerForeignRelationsWithPlayer* pP2PRelations = pRel->GetPlayerToPlayerRelations( iTarget );
	ASSERT( pP2PRelations );
	if( pP2PRelations == NULL ) return;
	pP2PRelations->SetForeignRelations( eRel );
*/
	m_ForeignRelations.Value( iActor,iTarget ) = eRel;
}

EOSAIEnumForeignRelations  CEOSAIGlobalForeignRelations::GetForeignRelations( long iActor, long iTarget )
{
	if( iActor == 0 ) return EOSAIEnumForeignRelations::enum_Undefined;
	if( iTarget == 0 ) return EOSAIEnumForeignRelations::enum_War;
	//
	if( iActor == iTarget ) return EOSAIEnumForeignRelations::enum_Self;
	//
/*
	CPlayerForeignRelations* pRel = GetPlayerForeignRelations( iActor );
	ASSERT( pRel );
	CPlayerForeignRelationsWithPlayer* pP2PRelations = pRel->GetPlayerToPlayerRelations( iTarget );
	ASSERT( pP2PRelations );
	return pP2PRelations->GetForeignRelations();
*/
	return m_ForeignRelations.Value( iActor,iTarget );
}

void  CEOSAIGlobalForeignRelations::ResetFeelings()  // reset everyone back to 0.5
{
	for( long x=0; x<m_Feelings.m_iSizeX; x++ )
	{
		for( long y=0; y<m_Feelings.m_iSizeY; y++ )
		{
			m_Feelings.Value( x,y ) = 0.5f;

			if( x==y )
			{
				SetFeelings( x,y, 1.0f );
			}
		}
	}
}

void  CEOSAIGlobalForeignRelations::SetFeelings( CEOSAIBCDumbArray2D< float >* pFeelingsArray )
{
	long iSize = min( EOSAI_MAX_NUMBER_OF_PLAYERS+1, pFeelingsArray->m_iSizeX );
	for( long iPlayer1=0; iPlayer1<iSize; iPlayer1++ )
	{
		for( long iPlayer2=0; iPlayer2<iSize; iPlayer2++ )
		{
			m_Feelings.Value( iPlayer1,iPlayer2 ) = pFeelingsArray->Value( iPlayer1,iPlayer2 );
		}
	}
}

void   CEOSAIGlobalForeignRelations::SetFeelings( long iActor, long iTarget, float f )
{
	m_Feelings.Value( iActor, iTarget ) = f;
}

float  CEOSAIGlobalForeignRelations::GetRawFeelings( long iActor, long iTarget )
{
	return m_Feelings.Value( iActor, iTarget );
}

float  CEOSAIGlobalForeignRelations::GetFeelings01( long iActor, long iTarget )
{
	if( iActor == iTarget ) return 1.0f;

	float fFeelings = m_Feelings.Value( iActor, iTarget );
	//if( fFeelings >= 0.25f && fFeelings <= 0.75f ) return fFeelings;
	return CEOSAIMath::Sigma01( 0.25f,0.75f, fFeelings );
}

// Trade
/*
void CGlobalForeignRelations::AddEvent_Trade( long iActor, long iTargetPlayer, CGameTime Time, float fSignificance )
{
	CEOSAIPlayerInteractionEvent* pEvent;

	// Add a [Trade] event
	pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_Trade;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = 0;
	//
	pEvent->m_fSignificance = fSignificance;
	//
	m_PlayerInteractionHistory.AddTail( pEvent );
}

// War
void CGlobalForeignRelations::AddEvent_DeclarationOfWar( long iActor, long iTargetPlayer, CGameTime Time )
{
	ASSERT( WarStateExists( iActor, iTargetPlayer ) == false );
	ASSERT( PermanentAllianceStateExists( iActor, iTargetPlayer, Time ) == false );

	CEOSAIPlayerInteractionEvent* pEvent;

	// Add a [Declaration of War] event
	pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_DeclarationOfWar;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = 0;
	m_PlayerInteractionHistory.AddTail( pEvent );

	// Add a [War State]
	pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_WarState;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = 1000000;
	m_PlayerInteractionHistory.AddTail( pEvent );
}

void CGlobalForeignRelations::AddEvent_SneakAttack( long iActor, long iTargetPlayer, CGameTime Time )
{
	ASSERT( WarStateExists( iActor, iTargetPlayer ) == false );
	ASSERT( PermanentAllianceStateExists( iActor, iTargetPlayer, Time ) == false );

	CEOSAIPlayerInteractionEvent* pEvent;

	// Add a [Sneak Attack] event
	pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_SneakAttack;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = 0;
	m_PlayerInteractionHistory.AddTail( pEvent );

	// Add a [War State]
	pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_WarState;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = 1000000;
	m_PlayerInteractionHistory.AddTail( pEvent );
}

void  CGlobalForeignRelations::AddEvent_CombatEvent( long iActor, long iTargetPlayer, CGameTime Time, float fSignificance )
{
	ASSERT( WarStateExists( iActor, iTargetPlayer ) );
	ASSERT( PermanentAllianceStateExists( iActor, iTargetPlayer, Time ) == false );

	CEOSAIPlayerInteractionEvent* pEvent;

	// Add a [Sneak Attack] event
	pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_CombatEvent;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = 0;
	pEvent->m_fSignificance = fSignificance;
	m_PlayerInteractionHistory.AddTail( pEvent );
}

void CGlobalForeignRelations::AddEvent_PeaceTreaty( long iActor, long iTargetPlayer, CGameTime Time )
{
	// End the [War State]
	ASSERT( WarStateExists( iActor, iTargetPlayer ) );
	EndWarState( iActor, iTargetPlayer, Time );

	// Add a [Peace] event
	CEOSAIPlayerInteractionEvent* pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_PeaceTreaty;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = 0;
	m_PlayerInteractionHistory.AddTail( pEvent );
}

void CGlobalForeignRelations::AddEvent_NonAggressionPact( long iActor, long iTargetPlayer, CGameTime Time, long iDuration )
{
	// There should not be a war-state.  If there is a trade-offer that involves a peace-agreement
	//    and a non-aggression pact, the peace agreement must be handled first
	ASSERT( WarStateExists( iActor, iTargetPlayer ) == false );

	// Add a [Peace] event
	CEOSAIPlayerInteractionEvent* pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_NonAggressionPact;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = iDuration;
	m_PlayerInteractionHistory.AddTail( pEvent );
}

void CGlobalForeignRelations::AddEvent_TemporaryAlliance( long iActor, long iTargetPlayer, CGameTime Time, long iDuration )
{
	// There should not be a war-state.  If there is a trade-offer that involves a peace-agreement
	//    and a non-aggression pact, the peace agreement must be handled first
	ASSERT( WarStateExists( iActor, iTargetPlayer ) == false );

	// Add a [Peace] event
	CEOSAIPlayerInteractionEvent* pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_TemporaryAlliance;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	pEvent->m_iEventDuration = iDuration;
	m_PlayerInteractionHistory.AddTail( pEvent );
}

void CGlobalForeignRelations::AddEvent_PermanentAlliance( long iActor, long iTargetPlayer, CGameTime Time )
{
	// There should not be a war-state.  If there is a trade-offer that involves a peace-agreement
	//    and a non-aggression pact, the peace agreement must be handled first
	ASSERT( WarStateExists( iActor, iTargetPlayer ) == false );

	// Add a [Peace] event
	CEOSAIPlayerInteractionEvent* pEvent = new CEOSAIPlayerInteractionEvent();
	pEvent->m_eType = CEOSAIPlayerInteractionEvent::enum_PermanentAlliance;
	pEvent->m_iActor = iActor;
	pEvent->m_iTargetPlayer = iTargetPlayer;
	pEvent->m_BeginTime = Time;
	m_PlayerInteractionHistory.AddTail( pEvent );
}

bool CGlobalForeignRelations::WarStateExists( long iActor, long iTargetPlayer )
{
	bool bWarStateFound = false;
	POSITION pos = m_PlayerInteractionHistory.GetTailPosition();
	while( pos )
	{
		CEOSAIPlayerInteractionEvent* pEvent = m_PlayerInteractionHistory.GetPrev( pos );
		if( pEvent->m_eType == CEOSAIPlayerInteractionEvent::enum_WarState )
		{
			//ASSERT( Time.GetTime() >= pEvent->m_BeginTime.GetTime() );
			if( pEvent->m_iEventDuration == 1000000 )
			{
				bWarStateFound = true;
			}
			break;
		}
	}
	return bWarStateFound;
}

void CGlobalForeignRelations::EndWarState( long iActor, long iTargetPlayer, CGameTime Time )
{
	bool bWarStateFound = false;
	POSITION pos = m_PlayerInteractionHistory.GetTailPosition();
	while( pos )
	{
		CEOSAIPlayerInteractionEvent* pEvent = m_PlayerInteractionHistory.GetPrev( pos );
		if( pEvent->m_eType == CEOSAIPlayerInteractionEvent::enum_WarState )
		{
			ASSERT( Time.GetTime() >= pEvent->m_BeginTime.GetTime() );
			if( pEvent->m_iEventDuration == 1000000 )
			{
				pEvent->m_iEventDuration = (long)( Time.GetTime() - pEvent->m_BeginTime.GetTime() );
				bWarStateFound = true;
			}
			break;
		}
	}
	ASSERT( bWarStateFound );
}

bool CGlobalForeignRelations::PermanentAllianceStateExists( long iActor, long iTargetPlayer, CGameTime Time )
{
	bool bPAllianceStateFound = false;
	POSITION pos = m_PlayerInteractionHistory.GetTailPosition();
	while( pos )
	{
		CEOSAIPlayerInteractionEvent* pEvent = m_PlayerInteractionHistory.GetPrev( pos );
		if( pEvent->m_eType == CEOSAIPlayerInteractionEvent::enum_PermanentAlliance )
		{
			if( pEvent->m_BeginTime <= Time )
			{
				bPAllianceStateFound = true;
			}
			break;
		}
	}
	return bPAllianceStateFound;
}
*/

/*
void CGlobalForeignRelations::ProcessHistoricalEvents( CGameTime CurrentTime )
{
	// Reset/Clear feelings
	for( long iPlayer=1; iPlayer<=MAX_NUMBER_OF_PLAYERS; iPlayer++ )
	{
		if( m_Players[iPlayer] )
		{
			//m_Players[iPlayer]->ResetFeelings();
		}
	}

	// Look for permanent alliances
	/-*
	POSITION pos = m_PlayerInteractionHistory.GetHeadPosition();
	while( pos )
	{
		CEOSAIPlayerInteractionEvent* pEvent = m_PlayerInteractionHistory.GetNext( pos );
		if( pEvent->m_eType == CEOSAIPlayerInteractionEvent::enum_PermanentAlliance )
		{
		}
	}
	*-/

	// Process the events
	float fCurrentTime = -1.0f;
	POSITION pos = m_PlayerInteractionHistory.GetHeadPosition();
	while( pos )
	{
		CEOSAIPlayerInteractionEvent* pEvent = m_PlayerInteractionHistory.GetNext( pos );

		// DEBUG - I need to keep the events in order.  For most events, the order is
		//   irrelevant, but when an event has a "limit" value (like war), the mathematics
		//   depend on the order of events
		ASSERT( pEvent->m_BeginTime.GetTime() >= fCurrentTime );
		fCurrentTime = pEvent->m_BeginTime.GetTime();

		// DEBUG
		if( pEvent->m_BeginTime > CurrentTime ) continue;
		// DEBUG

		pEvent->AffectFeelings( this, CurrentTime.GetTime() );
	}
}

float CGlobalForeignRelations::GetPlayerFeelings( long iPlayer, long iFeelingsTowardsPlayer )
{
	ASSERT( iPlayer != iFeelingsTowardsPlayer );
	ASSERT( iPlayer > 0 && iPlayer <= MAX_NUMBER_OF_PLAYERS );
	ASSERT( iFeelingsTowardsPlayer > 0 && iFeelingsTowardsPlayer <= MAX_NUMBER_OF_PLAYERS );

	//CForeignRelationsPlayerProxy* pPlayerProxy = m_Players[ iPlayer ];
	CPlayerForeignRelations* pPlayerProxy = m_Players[ iPlayer ];
	ASSERT( pPlayerProxy );
	if( pPlayerProxy == NULL ) return -1.0f;
	//
	CPlayerForeignRelationsWithPlayer* pP2PRelations = pPlayerProxy->GetPlayerToPlayerRelations( iFeelingsTowardsPlayer );
	ASSERT( pP2PRelations );
	if( pP2PRelations == NULL ) return -1.0f;

	return pP2PRelations->GetAIFeelings();
}
*/


bool  CEOSAIGlobalForeignRelations::IsSendingOngoingSendMap( long iPlayer1, long iPlayer2 )
{
	// Lookup active trade agreements and find NonAggressionPact
	//CWorldDesc* pWorldDesc = GetCommonState()->GetWorldDescByPlayerNumber( m_iPlayer );
	ASSERT( false );
	return false;
#ifdef DO_NOT_COMPILE
	if( m_pWorldDesc )
	{
		//CWorldDescPlayer* pWorldDescPlayer = dynamic_cast< CWorldDescPlayer* >( pWorldDesc );
		//if( pWorldDescPlayer )
		{
			bool bValue = false;
			POSITION pos = m_pWorldDesc->GetTradeAgreements()->GetHeadPosition();
			while( pos )
			{
				CTradeAgreement* pTradeAgreement = m_pWorldDesc->GetTradeAgreements()->GetNext( pos );
				if( pTradeAgreement->m_TradeAgreementState == EOSAIEnumTradeAgreementResponse_Accept )
				{
					if( pTradeAgreement->m_iPlayer[0] == iPlayer1 &&
						pTradeAgreement->m_iPlayer[1] == iPlayer2 )
					{
						bValue = bValue | pTradeAgreement->m_bPlayerMapViewOngoing[0];
					}
					if( pTradeAgreement->m_iPlayer[0] == iPlayer2 &&
						pTradeAgreement->m_iPlayer[1] == iPlayer1 )
					{
						bValue = bValue | pTradeAgreement->m_bPlayerMapViewOngoing[1];
					}
				}
			}
			return bValue;
		}
	}
	return false;
#endif DO_NOT_COMPILE
}
/*
bool  CGlobalForeignRelations::IsReceivingOngoingSendMap()
{
	// Lookup active trade agreements and find NonAggressionPact
	CWorldDesc* pWorldDesc = GetCommonState()->GetWorldDescByPlayerNumber( m_iPlayer );
	if( pWorldDesc )
	{
		CWorldDescPlayer* pWorldDescPlayer = dynamic_cast< CWorldDescPlayer* >( pWorldDesc );
		if( pWorldDescPlayer )
		{
			bool bValue = false;
			POSITION pos = pWorldDescPlayer->GetTradeAgreements()->GetHeadPosition();
			while( pos )
			{
				CTradeAgreement* pTradeAgreement = pWorldDescPlayer->GetTradeAgreements()->GetNext( pos );
				//if( pTradeAgreement->IsActive() )
				{
					if( pTradeAgreement->m_iPlayer[0] == m_iPlayer )
					{
						bValue = bValue | pTradeAgreement->m_bPlayerMapViewOngoing[0];
					}
					if( pTradeAgreement->m_iPlayer[1] == m_iPlayer )
					{
						bValue = bValue | pTradeAgreement->m_bPlayerMapViewOngoing[1];
					}
				}
			}
			return bValue;
		}
	}
	return false;
}
*/
//bool  CGlobalForeignRelations::HasNonAggressionPact( long iPlayer1, long iPlayer2 )
long  CEOSAIGlobalForeignRelations::GetEndOfNonAggressionPact( long iPlayer1, long iPlayer2 )
{
	// Lookup active trade agreements and find NonAggressionPact
	//CWorldDesc* pWorldDesc = GetCommonState()->GetWorldDescByPlayerNumber( m_iPlayer );
	ASSERT( false );
#ifdef DO_NOT_COMPILE
	if( m_pWorldDesc )
	{
		long iCurrentTurn = m_pWorldDesc->GetCurrentTurn();
		//CWorldDescPlayer* pWorldDescPlayer = dynamic_cast< CWorldDescPlayer* >( pWorldDesc );
		//if( pWorldDescPlayer )
		{
			long iNonAggressionTurnEnd = 0;
			//bool bValue = false;
			POSITION pos = m_pWorldDesc->GetTradeAgreements()->GetHeadPosition();
			while( pos )
			{
				CTradeAgreement* pTradeAgreement = m_pWorldDesc->GetTradeAgreements()->GetNext( pos );
				if( pTradeAgreement->m_TradeAgreementState == EOSAIEnumTradeAgreementState_ActiveOngoingAgreement &&
					pTradeAgreement->m_bNonAggressionPact &&
					pTradeAgreement->PlayersInvolved( iPlayer1, iPlayer2 ) )
				{
					if( pTradeAgreement->m_iNonAggressionPactEndTurn > iCurrentTurn )
					{
						iNonAggressionTurnEnd = pTradeAgreement->m_iNonAggressionPactEndTurn;
					}
					//iCurrentTurn
					//bValue = bValue | pTradeAgreement->m_bNonAggressionPact;
				}
			}
			return iNonAggressionTurnEnd;
		}
	}
#endif DO_NOT_COMPILE
	return 0;
}

bool  CEOSAIGlobalForeignRelations::GetHasOngoingTrade( long iPlayer1, long iPlayer2 )
{
	// Lookup active trade agreements and find NonAggressionPact
	//CWorldDesc* pWorldDesc = GetCommonState()->GetWorldDescByPlayerNumber( m_iPlayer );
	//CWorldDescPlayer* pWorldDescPlayer = dynamic_cast< CWorldDescPlayer* >( pWorldDesc );
return false;
#ifdef DO_NOT_COMPILE
	//if( pWorldDescPlayer )
	{
		bool bValue = false;
		POSITION pos = m_pWorldDesc->GetTradeAgreements()->GetHeadPosition();
		while( pos )
		{
			CTradeAgreement* pTradeAgreement = m_pWorldDesc->GetTradeAgreements()->GetNext( pos );
			//if( pTradeAgreement->IsActive() )
			if( pTradeAgreement->m_TradeAgreementState == EOSAIEnumTradeAgreementState_ActiveOngoingAgreement &&
				pTradeAgreement->PlayersInvolved( iPlayer1, iPlayer2 ) )
			{
				bValue = bValue | pTradeAgreement->IsAnOngoingTradeAgreement();
			}
		}
		return bValue;
	}
#endif DO_NOT_COMPILE
	return false;
}

bool  CEOSAIGlobalForeignRelations::GetHasOngoingTrade_ResourceTrade( long iPlayer1, long iPlayer2 )
{
	// Lookup active trade agreements and find NonAggressionPact
	//CWorldDesc* pWorldDesc = GetCommonState()->GetWorldDescByPlayerNumber( m_iPlayer );
	//CWorldDescPlayer* pWorldDescPlayer = dynamic_cast< CWorldDescPlayer* >( pWorldDesc );
	//if( pWorldDescPlayer )
ASSERT( false );
#ifdef DO_NOT_COMPILE
	{
		bool bValue = false;
		POSITION pos = m_pWorldDesc->GetTradeAgreements()->GetHeadPosition();
		while( pos )
		{
			CTradeAgreement* pTradeAgreement = m_pWorldDesc->GetTradeAgreements()->GetNext( pos );
			//if( pTradeAgreement->IsActive() )
			if( pTradeAgreement->m_TradeAgreementState == EOSAIEnumTradeAgreementState_ActiveOngoingAgreement &&
				pTradeAgreement->PlayersInvolved( iPlayer1, iPlayer2 ) )
			{
				bValue = bValue | pTradeAgreement->IsAnOngoingTradeAgreement_AccordingToResourceTrade();
			}
		}
		return bValue;
	}
#endif DO_NOT_COMPILE
	return false;
}

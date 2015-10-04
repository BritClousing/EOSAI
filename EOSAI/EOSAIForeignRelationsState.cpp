
#include <stdafx.h>
#include "EOSAIForeignRelationsState.h"
#include "EOSAIStrategicAI.h"
#include "EOSAINationalSummary3.h"
//#include "WorldDescServer.h"
#include "EOSAIPlayerInteraction.h"
#include "EOSAIMath.h"
#include "EOSAICommonData.h"
#include "EOSAIGamePlayer.h"
#include "AIPlayer.h"
#include "EOSAIPlayerManager.h"
#include "EOSAIIntSet.h"
#include <math.h>

#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//

CEOSAIForeignRelationsState::CEOSAIForeignRelationsState()
{
	m_pStrategicAI = NULL;
	m_iNumberOfPlayers = 0;
	m_fDesireToConcedeGame = 0.0f;
	//m_bDesireToConcedeGame = false;
	ClearValues();
}

void  CEOSAIForeignRelationsState::SetNumberOfPlayers( long iNumberOfPlayers )
{
	// should be the *actual* number of players (e.g. "2")
	ASSERT( iNumberOfPlayers > 0 && iNumberOfPlayers <= EOSAI_MAX_NUMBER_OF_PLAYERS );
	if( m_iNumberOfPlayers == 0 )
	{
		m_iNumberOfPlayers = iNumberOfPlayers;

		//m_MaintenanceCost01.SetSize( m_iNumberOfPlayers+1 );
		//m_UnownedCitResAccessibility01.SetSize( m_iNumberOfPlayers+1 );
		//m_CityProductionPower.SetSize( m_iNumberOfPlayers+1 );
		//m_MilitaryPower.SetSize( m_iNumberOfPlayers+1 );
		//m_Power.SetSize( m_iNumberOfPlayers+1 );
		//m_SATPowerBalanceWithEnemies.SetSize( m_iNumberOfPlayers+1 );
		m_ExpectedConquest11.SetSize( m_iNumberOfPlayers+1 );
		m_ExpectedUnownedExpansion01.SetSize( m_iNumberOfPlayers+1 );
		m_SATGlobalPower01.SetSize( m_iNumberOfPlayers+1 );

		m_Feelings01.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );
		//m_PlayerAccessibility.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );
		m_PlayerRelationsMatrix.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );
		m_PlayerPowerAllocationMatrix.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );
		m_GeneralWarDesire.SetSize( m_iNumberOfPlayers+1 );
		m_RawWarDesire.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );
		m_NormalizedWarDesire.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );
		m_FinalWarDesire.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );

		//m_TopWarTarget.SetSize( m_iNumberOfPlayers+1 );
		//m_TopWarTargetValue.SetSize( m_iNumberOfPlayers+1 );
		m_PursueFriendlyRelations.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );
		m_ForeignRelations.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );

		m_RelationsPlan.SetSize( m_iNumberOfPlayers+1, m_iNumberOfPlayers+1 );
	}
	ASSERT( m_iNumberOfPlayers == iNumberOfPlayers );
}

void  CEOSAIForeignRelationsState::ClearValues()
{
	m_bExpectedUnownedExpansion01 = false;

	if( m_iNumberOfPlayers == 0 ) return;
	for( long i=0; i<m_iNumberOfPlayers+1; i++ )
	{
		m_GeneralWarDesire.Value( i ) = 0.0f;
		m_ExpectedConquest11.Value( i ) = 0.0f;
		m_ExpectedUnownedExpansion01.Value( i ) = 0.0f;
		m_SATGlobalPower01.Value( i ) = 0.0f;
	}
	for( long x=0; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=0; y<m_iNumberOfPlayers+1; y++ )
		{
			if( x==y )
			{
				m_Feelings01.Value( x,y ) = 1.0f;
				//m_PlayerAccessibility.Value( x,y ) = 1.0f;
				m_ForeignRelations.Value( x,y ) = EOSAIEnumForeignRelations::enum_Self;
			}
			if( x!=y )
			{
				m_Feelings01.Value( x,y ) = 0.5f;
				//m_PlayerAccessibility.Value( x,y ) = 0.0f;
				m_ForeignRelations.Value( x,y ) = EOSAIEnumForeignRelations::enum_Neutral;
			}
			//m_PlayerAccessibility.Value( x,y ) = 1.0f;
			m_PlayerRelationsMatrix.Value( x,y ) = 0.0f;
			m_PlayerPowerAllocationMatrix.Value( x,y ) = 0.0f;
			m_RawWarDesire.Value( x,y ) = 0.0f;
			m_NormalizedWarDesire.Value( x,y ) = 0.0f;
			m_FinalWarDesire.Value( x,y ) = 0.0f;
			//m_TopWarTarget.Value( x ) = 0;
			//m_TopWarTargetValue.Value( x ) = 0.0f;
			m_PursueFriendlyRelations.Value( x,y ) = 0.0f;
			m_RelationsPlan.Value( x,y ) = enum_None;
		}
	}
}

CString  CEOSAIForeignRelationsState::OutputDebugString()
{
	CString str, strTemp;

	strTemp.Format( _T("Player %d\n"), m_pStrategicAI->GetPlayerNumber() );
	str += strTemp;

	str += _T("Feelings\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			strTemp.Format( _T("  %0.2f"), m_Feelings01.Value( x,y ) );
			str += strTemp;
		}
		str += _T("\n");
	}
	/*
	str += "Accessibility\n";
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			strTemp.Format( "  %0.2f", m_PlayerAccessibility.Value( x,y ) );
			str += strTemp;
		}
		str += "\n";
	}
	*/
	str += _T("Foreign Relations\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_Self )
			{
				str += _T("  Self");
			}
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_Teammate )
			{
				str += _T("  Team");
			}
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_Alliance )
			{
				str += _T("  Ally");
			}
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_Neutral )
			{
				str += _T("  Neut");
			}
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_War )
			{
				str += _T("  War ");
			}
		}
		str += _T("\n");
	}
	
	if( m_pStrategicAI )
	{
		str += _T("Power\n");
		for( long x=1; x<m_iNumberOfPlayers+1; x++ )
		{
			strTemp.Format( _T("  %0.2f\n"), GetPower( x ) );
			str += strTemp;
		}
	}
	//str += "SATPowerBalance\n";
	//for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	//{
	//	strTemp.Format( "  %0.2f\n", m_SATPowerBalanceWithEnemies.Value( x ) );
	//	str += strTemp;
	//}
	str += _T("PlayerRelationsMatrix\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			strTemp.Format( _T("  %0.2f"), m_PlayerRelationsMatrix.Value( x,y ) );
			str += strTemp;
		}
		str += _T("\n");
	}
	str += _T("PlayerPowerAllocationMatrix\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			strTemp.Format( _T("  %0.2f"), m_PlayerPowerAllocationMatrix.Value( x,y ) );
			str += strTemp;
		}
		str += _T("\n");
	}
	str += _T("SATEMatrixPlayerPowerBalance\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		strTemp.Format( _T("  %0.2f\n"), GetSATEPowerBalanceBasedOnMatrix( x ) );
		str += strTemp;
	}
	str += _T("ExpectedConquest\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		strTemp.Format( _T("  %0.2f\n"), m_ExpectedConquest11.Value( x ) );
		str += strTemp;
	}
	str += _T("ExpectedUnownedExpansion01\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		strTemp.Format( _T("  %0.2f\n"), m_ExpectedUnownedExpansion01.Value( x ) );
		str += strTemp;
	}
	str += _T("SATGlobalPower\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		strTemp.Format( _T("  %0.2f\n"), m_SATGlobalPower01.Value( x ) );
		str += strTemp;
	}
	str += _T("ConcernThatPlayerIsTooPowerful\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		strTemp.Format( _T("  %0.2f\n"), GetConcernThatPlayerIsTooPowerful( x ) );
		str += strTemp;
	}
	str += _T("GeneralWarDesire\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		strTemp.Format( _T("  %0.2f"), m_GeneralWarDesire.Value( x ) );
		str += strTemp;
		str += _T("\n");
	}
	str += _T("RawWarDesire\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			strTemp.Format( _T("  %0.2f"), m_RawWarDesire.Value( x,y ) );
			str += strTemp;
		}
		str += _T("\n");
	}
	str += _T("NormalizedWarDesire\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			strTemp.Format( _T("  %0.2f"), m_NormalizedWarDesire.Value( x,y ) );
			str += strTemp;
		}
		str += _T("\n");
	}
	str += _T("FinalWarDesire\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			strTemp.Format( _T("  %0.2f"), m_FinalWarDesire.Value( x,y ) );
			str += strTemp;
		}
		str += _T("\n");
	}

	str += _T("TopTarget()\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		long iTopTarget;
		float fTopTargetValue;
		GetTopTarget( x, &iTopTarget, &fTopTargetValue );
		strTemp.Format( _T("  Player %d wants to attack Player %d %0.2f"), x, iTopTarget, fTopTargetValue );
		str += strTemp;
		str += _T("\n");
	}

	str += _T("PursueFriendlyRelations\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			strTemp.Format( _T("  %0.2f"), m_PursueFriendlyRelations.Value( x,y ) );
			str += strTemp;
		}
		str += _T("\n");
	}

	str += _T("RelationsPlan\n");
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( m_RelationsPlan.Value( x,y ) == enum_None )
			{
				strTemp.Format( _T(" None"), m_RelationsPlan.Value( x,y ) );
			}
			if( m_RelationsPlan.Value( x,y ) == enum_StartWar )
			{
				strTemp.Format( _T(" StartWar"), m_RelationsPlan.Value( x,y ) );
			}
			if( m_RelationsPlan.Value( x,y ) == enum_PlanWar )
			{
				strTemp.Format( _T(" PlanWar"), m_RelationsPlan.Value( x,y ) );
			}
			if( m_RelationsPlan.Value( x,y ) == enum_Neutral )
			{
				strTemp.Format( _T(" Neutral"), m_RelationsPlan.Value( x,y ) );
			}
			if( m_RelationsPlan.Value( x,y ) == enum_MakePeace )
			{
				strTemp.Format( _T(" MakePeace"), m_RelationsPlan.Value( x,y ) );
			}
			if( m_RelationsPlan.Value( x,y ) == enum_MaintainPeace )
			{
				strTemp.Format( _T(" MaintainPeace"), m_RelationsPlan.Value( x,y ) );
			}
			str += strTemp;
		}
		str += _T("\n");
	}

	return str;
}

//


//void  CEOSAIForeignRelationsState::CalculateValues()
/*
void  CEOSAIForeignRelationsState::RunCalculations()
{
	CalculateExpectedUnownedExpansion();
	CalculatePlayerRelationsMatrix();
	CalculateSATGlobalPower();
	CalculateRawWarDesire();
	CalculatePursueFriendlyRelations01();
}
*/

void  CEOSAIForeignRelationsState::ApplyHistoricalEventsToFeelings()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//POSITION pos = pWorldDescServer->GetPlayerInteractions()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetPlayerInteractions()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPlayerInteraction* pPlayerInteraction = g_pEOSAICommonData->GetPlayerInteractions()->GetNext( pos );
		ASSERT( pPlayerInteraction->m_iEventTurn >= 0 );
		pPlayerInteraction->UpdateForeignRelationsFeelings( g_pEOSAIInterface->GetCurrentTurn(),&m_ForeignRelations,&m_Feelings01 );
	}

	// Apply AICheat_BiasAgainstHumanPlayers
	for( long iPlayer1=1; iPlayer1<=m_iNumberOfPlayers; iPlayer1++ )
	{
		EOSAI::AIPlayer* pAIPlayer1 = g_pAIPlayerManager->GetAIPlayer( iPlayer1 );
		if( pAIPlayer1 )
		{
			//float fFeelingsBias = AICheat_LikesHumanPlayers();
			float fAIFeelingsBias = pAIPlayer1->AICheat_LikesAIPlayers();
			float fHumanFeelingsBias = pAIPlayer1->AICheat_LikesHumanPlayers();
			for( long iPlayer2=1; iPlayer2<=m_iNumberOfPlayers; iPlayer2++ )
			{
				//EOSAI::CGamePlayer2* pPlayer2 = GetCommonState()->GetPlayer( iPlayer2 );
				EOSAI::CGamePlayer* pPlayer2 = g_pEOSAIInterface->GetGamePlayer( iPlayer2 );
				if( pPlayer2 == NULL ) continue;
				if( m_ForeignRelations.Value( iPlayer1,iPlayer2 ).IsTeammate() ) continue;

				if( pPlayer2->IsAI() )
				{
					float fNewFeelings01 = m_Feelings01.Value( iPlayer1,iPlayer2 ) + fAIFeelingsBias;
					if( fNewFeelings01 < 0.0f ){ fNewFeelings01 = 0.0f; }
					if( fNewFeelings01 > 1.0f ){ fNewFeelings01 = 1.0f; }
					m_Feelings01.Value( iPlayer1,iPlayer2 ) = fNewFeelings01;
				}
				if( pPlayer2->IsHuman() )
				{
					float fNewFeelings01 = m_Feelings01.Value( iPlayer1,iPlayer2 ) + fHumanFeelingsBias;
					if( fNewFeelings01 < 0.0f ){ fNewFeelings01 = 0.0f; }
					if( fNewFeelings01 > 1.0f ){ fNewFeelings01 = 1.0f; }
					m_Feelings01.Value( iPlayer1,iPlayer2 ) = fNewFeelings01;
				}
			}
		}
	}

	// (the local foreign relations should match the WorldDescServer foreign relations)
	// Copy the ForeignRelations
	// I should validate that they match
	for( long i=1; i<=m_iNumberOfPlayers; i++ )
	{
		for( long j=1; j<=m_iNumberOfPlayers; j++ )
		{
			m_ForeignRelations.Value( i,j ) = g_pEOSAICommonData->GetForeignRelations( i,j );
			//CEOSAIPlayerInteraction* pPlayerInteraction = pWorldDescServer->GetPlayerInteractions()->GetNext( pos );
			//pPlayerInteraction->UpdateForeignRelationsState( 1,&m_ForeignRelations,&m_Feelings01 );
		}
	}
}

void  CEOSAIForeignRelationsState::CalculateExpectedUnownedExpansion()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	// What percentage of the world is unowned?
	float fUnownedPercentage01 =
		( g_pEOSAICommonData->GetTotalUnownedCitResProductionOnMap() ) /
		( g_pEOSAICommonData->GetTotalCitResProductionOnMap() + 0.1f );

	fUnownedPercentage01 *= 1.5f;
	if( fUnownedPercentage01 > 1.0f ){ fUnownedPercentage01 = 1.0f; }

	m_bExpectedUnownedExpansion01 = true;
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		float fUnownedAccessibility01 = GetUnownedCitResAccessibility01( x );
		m_ExpectedUnownedExpansion01.Value( x ) = fUnownedPercentage01 * fUnownedAccessibility01;
	}
}

void  CEOSAIForeignRelationsState::CalculatePlayerRelationsMatrix()
{
	// Clear the matrix first
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			m_PlayerRelationsMatrix.Value( x,y ) = 0.0f;
		}
	}
	// Setup the matrix based on current (one-degree) relationships
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			float fValue = 0.0f;

			EOSAIEnumForeignRelations  XYRelationship = m_ForeignRelations.Value( x,y );
			if( XYRelationship == EOSAIEnumForeignRelations::enum_Self     ){ fValue =  1.0f; }
			if( XYRelationship == EOSAIEnumForeignRelations::enum_Teammate ){ fValue =  1.0f; }
			if( XYRelationship == EOSAIEnumForeignRelations::enum_Alliance ){ fValue =  0.6f; }
			if( XYRelationship == EOSAIEnumForeignRelations::enum_War      ){ fValue = -1.0f; }			

			if( XYRelationship == EOSAIEnumForeignRelations::enum_Neutral ||
				XYRelationship == EOSAIEnumForeignRelations::enum_Alliance )
			{
				// Compare PlayerY's relationship to PlayerZ vs PlayerX's relationship to PlayerZ
				//float fCommutativeRelXY = 0.0f;
				for( long z=0; z<m_iNumberOfPlayers; z++ )
				{
					if( x == z || y == z ) continue;
					EOSAIEnumForeignRelations RelXZ = m_ForeignRelations.Value( x,z );
					EOSAIEnumForeignRelations RelYZ = m_ForeignRelations.Value( y,z );

					// PlayerX+PlayerZ are teammates/allies, PlayerZ+PlayerY are enemies = PlayerY is enemy
					if( ( RelXZ == EOSAIEnumForeignRelations::enum_Teammate ||
						  RelXZ == EOSAIEnumForeignRelations::enum_Alliance ) &&
						  RelYZ == EOSAIEnumForeignRelations::enum_War )
					{
						if( RelXZ == EOSAIEnumForeignRelations::enum_Teammate ){ fValue += -99.0f; }
						if( RelXZ == EOSAIEnumForeignRelations::enum_Alliance ){ fValue += -0.6f; }
					}
					// PlayerX+PlayerZ are enemies, PlayerY+PlayerZ are teammates/allies = PlayerX+PlayerY are enemies
					if( ( RelYZ == EOSAIEnumForeignRelations::enum_Teammate ||
						  RelYZ == EOSAIEnumForeignRelations::enum_Alliance ) &&
						  RelXZ == EOSAIEnumForeignRelations::enum_War )
					{
						if( RelYZ == EOSAIEnumForeignRelations::enum_Teammate ){ fValue += -99.0f; }
						if( RelYZ == EOSAIEnumForeignRelations::enum_Alliance ){ fValue += -0.6f; }
					}
					// PlayerX+PlayerZ are teammates/allies, PlayerY+PlayerZ are teammates/allies = PlayerY is ally????
					if( ( RelYZ == EOSAIEnumForeignRelations::enum_Teammate ||
						  RelYZ == EOSAIEnumForeignRelations::enum_Alliance ) &&
						( RelXZ == EOSAIEnumForeignRelations::enum_Teammate ||
						  RelXZ == EOSAIEnumForeignRelations::enum_Alliance ) )
					{
						if( RelYZ == EOSAIEnumForeignRelations::enum_Teammate &&
							RelXZ == EOSAIEnumForeignRelations::enum_Teammate ){ fValue +=  0.4f; }
						if( RelYZ == EOSAIEnumForeignRelations::enum_Teammate &&
							RelXZ == EOSAIEnumForeignRelations::enum_Alliance ){ fValue +=  0.3f; }
						if( RelYZ == EOSAIEnumForeignRelations::enum_Alliance &&
							RelXZ == EOSAIEnumForeignRelations::enum_Teammate ){ fValue +=  0.3f; }
						if( RelYZ == EOSAIEnumForeignRelations::enum_Alliance &&
							RelXZ == EOSAIEnumForeignRelations::enum_Alliance ){ fValue +=  0.2f; }
					}
					// PlayerX+PlayerZ are enemies, PlayerY+PlayerZ are enemies = PlayerY is ally
					if( RelYZ == EOSAIEnumForeignRelations::enum_War &&
						RelXZ == EOSAIEnumForeignRelations::enum_War )
					{
						fValue += 0.3f;
					}
				}
				// Limit Mult to -1 to 1
				if( fValue < -1.0f ){ fValue = -1.0f; }
				if( fValue >  1.0f ){ fValue =  1.0f; }
				//
				//m_PlayerRelationsMatrix.Value( x,y ) = fCommutativeRelXY;
			}
			m_PlayerRelationsMatrix.Value( x,y ) = fValue;
		}
	}

	// Player-Power allocation
	long t=0;
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			m_PlayerPowerAllocationMatrix.Value( x,y ) = 0.0f;
		}
	}

	//
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		float fThisPlayerPower = GetPower( x );
		float fCombinedEnemyVal = 0.0f;
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( m_PlayerRelationsMatrix.Value( x,y ) < 0.0f )
			{
				float fConduitSize = (float) fabs( m_PlayerRelationsMatrix.Value( x,y ) );
				float fOtherPlayerPower = GetPower( y );
				float fAccessibility = GetPlayerAccessibility( x,y );
				fCombinedEnemyVal += fConduitSize * fAccessibility * fOtherPlayerPower;
			}
		}
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( m_PlayerRelationsMatrix.Value( x,y ) < 0.0f )
			{
				float fConduitSize = (float) fabs( m_PlayerRelationsMatrix.Value( x,y ) );
				float fOtherPlayerPower = GetPower( y );
				float fAccessibility = GetPlayerAccessibility( x,y );
				float fWeight = fThisPlayerPower * fOtherPlayerPower * fConduitSize * fAccessibility;
				if( fWeight > 0.0f )
				{
					m_PlayerPowerAllocationMatrix.Value( x,y ) = 
						fThisPlayerPower * fOtherPlayerPower * fConduitSize * fAccessibility / fCombinedEnemyVal;
				}
			}
		}
	}
	// Balance the power allocation
	/*
	for( long iPlayer=1; iPlayer<m_iNumberOfPlayers+1; iPlayer++ )
	{
		float fThisPlayerPower = m_Power.Value( iPlayer );
		float fIncomingPower = 0.0f;
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			fIncomingPower += m_PlayerPowerAllocationMatrix.Value( y, iPlayer );
		}

		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			float fVal = m_PlayerPowerAllocationMatrix.Value( y, iPlayer );
			if( fVal > 0.0f )
			{
				//m_PlayerPowerAllocationMatrix.Value( x,y ) = fThisPlayerPower * fOtherPlayerPower * fVal / fCombinedEnemyVal;
				m_PlayerPowerAllocationMatrix.Value( y, iPlayer ) =
					fThisPlayerPower *
					m_PlayerPowerAllocationMatrix.Value( y, iPlayer ) /
					fIncomingPower;
			}
		}
	}
	*/

	// Calculate expected expansion
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		float fThisPlayerPower = GetPower( x );
		float fIncomingEnemyPower = 0.0f;
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			fIncomingEnemyPower += m_PlayerPowerAllocationMatrix.Value( y, x );
		}

		//if( x==1 ){ fThisPlayerPower = 10.0f; fIncomingEnemyPower = 100.0f; }
		//if( x==2 ){ fThisPlayerPower = 10.0f; fIncomingEnemyPower =  11.0f; }
		//if( x==3 ){ fThisPlayerPower = 10.0f; fIncomingEnemyPower =  10.0f; }
		//if( x==4 ){ fThisPlayerPower = 10.0f; fIncomingEnemyPower =   1.0f; }
		//if( x==5 ){ fThisPlayerPower = 10.0f; fIncomingEnemyPower =   0.0f; }

		float fPowerRatio = 0.0f;
		if( fIncomingEnemyPower > 0 )
		{
			fPowerRatio = fIncomingEnemyPower / fThisPlayerPower;
			if( fPowerRatio == 1.0 )
			{
				fPowerRatio = 0.0f;
			}
			else if( fPowerRatio < 1.0 )
			{
				//fPowerRatio = 1/fPowerRatio;
				fPowerRatio = -fPowerRatio;
				fPowerRatio += 1.0f;
			}
			else if( fPowerRatio > 1.0 )
			{
				fPowerRatio = 1/fPowerRatio;
				fPowerRatio -= 1.0f;
			}
		}
		int asdasd=0;

		//

		float fBestAccessibility = 0.0f;
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_War )
			{
				float fAccessibility = GetPlayerAccessibility( x,y );
				fBestAccessibility = max( fBestAccessibility, fAccessibility );
			}
		}
		m_ExpectedConquest11.Value( x ) = fPowerRatio * fBestAccessibility;
	}

	long t908=0;
}
/*
void  CEOSAIForeignRelationsState::CalculatePowerBalanceFromMatrix()
{
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		m_SATPowerBalanceWithEnemies.Value( x ) = 0.0f;

		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			// One-degree relationship
			EOSAIEnumForeignRelations  XYRelationship = m_ForeignRelations.Value( x,y );
			float fMult = 0.0f;
			if( XYRelationship == EOSAIEnumForeignRelations::enum_Self     ){ fMult =  1.0f; }
			if( XYRelationship == EOSAIEnumForeignRelations::enum_Teammate ){ fMult =  1.0f; }
			if( XYRelationship == EOSAIEnumForeignRelations::enum_Alliance ){ fMult =  0.6f; }
			if( XYRelationship == EOSAIEnumForeignRelations::enum_War      ){ fMult = -1.0f; }
			//
			if( XYRelationship == EOSAIEnumForeignRelations::enum_Neutral ||
				XYRelationship == EOSAIEnumForeignRelations::enum_Alliance )
			{
				for( long z=0; z<m_iNumberOfPlayers; z++ )
				{
					if( x == z || y == z ) continue;
					EOSAIEnumForeignRelations RelXZ = m_ForeignRelations.Value( x,z );
					EOSAIEnumForeignRelations RelYZ = m_ForeignRelations.Value( y,z );

					// PlayerZ is my teammate or ally, PlayerZ and PlayerY are enemies = PlayerY as enemy
					if( ( RelXZ == EOSAIEnumForeignRelations::enum_Teammate ||
						  RelXZ == EOSAIEnumForeignRelations::enum_Alliance ) &&
						  RelYZ == EOSAIEnumForeignRelations::enum_War )
					{
						if( RelXZ == EOSAIEnumForeignRelations::enum_Teammate ){ fMult += -99.0f; }
						if( RelXZ == EOSAIEnumForeignRelations::enum_Alliance ){ fMult += -0.6f; }
					}
					// PlayerZ is PlayerY's teammate or ally, PlayerZ and me are enemies = PlayerY is enemy
					if( ( RelYZ == EOSAIEnumForeignRelations::enum_Teammate ||
						  RelYZ == EOSAIEnumForeignRelations::enum_Alliance ) &&
						  RelXZ == EOSAIEnumForeignRelations::enum_War )
					{
						if( RelYZ == EOSAIEnumForeignRelations::enum_Teammate ){ fMult += -99.0f; }
						if( RelYZ == EOSAIEnumForeignRelations::enum_Alliance ){ fMult += -0.6f; }
					}
					// PlayerZ is PlayerY's teammate or ally, PlayerZ and me are teammates or allies = PlayerY is ally
					if( ( RelYZ == EOSAIEnumForeignRelations::enum_Teammate ||
						  RelYZ == EOSAIEnumForeignRelations::enum_Alliance ) &&
						( RelXZ == EOSAIEnumForeignRelations::enum_Teammate ||
						  RelXZ == EOSAIEnumForeignRelations::enum_Alliance ) )
					{
						if( RelYZ == EOSAIEnumForeignRelations::enum_Teammate &&
							RelXZ == EOSAIEnumForeignRelations::enum_Teammate ){ fMult +=  0.7f; }
						if( RelYZ == EOSAIEnumForeignRelations::enum_Teammate &&
							RelXZ == EOSAIEnumForeignRelations::enum_Alliance ){ fMult +=  0.5f; }
						if( RelYZ == EOSAIEnumForeignRelations::enum_Alliance &&
							RelXZ == EOSAIEnumForeignRelations::enum_Teammate ){ fMult +=  0.5f; }
						if( RelYZ == EOSAIEnumForeignRelations::enum_Alliance &&
							RelXZ == EOSAIEnumForeignRelations::enum_Alliance ){ fMult +=  0.2f; }
					}
				}
				// Limit Mult to -1 to 1
				if( fMult < -1.0f ){ fMult = -1.0f; }
				if( fMult >  1.0f ){ fMult =  1.0f; }
			}
			//
			//m_SATPowerBalanceWithEnemies.Value( x ) += fMult * GetPower( y );
			m_SATPowerBalanceWithEnemies.Value( x ) += fMult * m_Power.Value( y ); // Temporary
		}
	}
}
*/
void  CEOSAIForeignRelationsState::Copy( CEOSAIForeignRelationsState* pOther )
{
	m_pStrategicAI = pOther->m_pStrategicAI;

	ASSERT( pOther->m_iNumberOfPlayers > 0 );
	if( m_iNumberOfPlayers >  0 ){ ASSERT( m_iNumberOfPlayers == pOther->m_iNumberOfPlayers ); }
	if( m_iNumberOfPlayers == 0 ){ SetNumberOfPlayers( pOther->m_iNumberOfPlayers ); }
	//
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		//m_MaintenanceCost01.Value( x ) = pOther->m_MaintenanceCost01.Value( x );
		//m_UnownedCitResAccessibility01.Value( x ) = pOther->m_UnownedCitResAccessibility01.Value( x );
		//m_CityProductionPower.Value( x ) = pOther->m_CityProductionPower.Value( x );
		//m_MilitaryPower.Value( x ) = pOther->m_MilitaryPower.Value( x );
		//m_Power.Value( x ) = pOther->m_Power.Value( x );
		//m_SATPowerBalanceWithEnemies.Value( x ) = pOther->m_SATPowerBalanceWithEnemies.Value( x );

		m_ExpectedConquest11.Value( x ) = pOther->m_ExpectedConquest11.Value( x );
		m_ExpectedUnownedExpansion01.Value( x ) = pOther->m_ExpectedUnownedExpansion01.Value( x );
		m_SATGlobalPower01.Value( x ) = pOther->m_SATGlobalPower01.Value( x );
	}
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			m_Feelings01.Value( x,y ) = pOther->m_Feelings01.Value( x,y );
			m_ForeignRelations.Value( x,y ) = pOther->m_ForeignRelations.Value( x,y );
			m_PlayerRelationsMatrix.Value( x,y ) = pOther->m_PlayerRelationsMatrix.Value( x,y );

			m_PlayerPowerAllocationMatrix.Value( x,y ) = pOther->m_PlayerPowerAllocationMatrix.Value( x,y );
			m_RawWarDesire.Value( x,y ) = pOther->m_RawWarDesire.Value( x,y );
			m_PursueFriendlyRelations.Value( x,y ) = pOther->m_PursueFriendlyRelations.Value( x,y );
		}
	}
}
/*
void  CEOSAIForeignRelationsState::SetPower( long iPlayer, float fValue )
{
	m_Power.Value( iPlayer-1 ) = fValue;
}
*/
/*
void  CEOSAIForeignRelationsState::SetProductionPower( long iPlayer, float fValue )
{
	m_CityProductionPower.Value( iPlayer ) = fValue;
	m_Power.Value( iPlayer ) = m_CityProductionPower.Value( iPlayer ) + m_MilitaryPower.Value( iPlayer );
}

void  CEOSAIForeignRelationsState::SetMilitaryPower( long iPlayer, float fValue )
{
	m_MilitaryPower.Value( iPlayer ) = fValue;
	m_Power.Value( iPlayer ) = m_CityProductionPower.Value( iPlayer ) + m_MilitaryPower.Value( iPlayer );
}

void  CEOSAIForeignRelationsState::SetPower( long iPlayer, float fMilitaryPower, float fCityProduction )
{
	m_CityProductionPower.Value( iPlayer ) = fCityProduction;
	m_MilitaryPower.Value( iPlayer ) = fMilitaryPower;
	m_Power.Value( iPlayer ) = fCityProduction + fMilitaryPower;
}
*/
void  CEOSAIForeignRelationsState::SetFeelings( long iPlayer1, long iPlayer2, float fValue )
{
	ASSERT( iPlayer1 != iPlayer2 );
	m_Feelings01.Value( iPlayer1, iPlayer2 ) = fValue;
}

void  CEOSAIForeignRelationsState::AddFeelings( long iPlayer1, long iPlayer2, float fDelta )
{
	ASSERT( iPlayer1 != iPlayer2 );
	float fFeelings = m_Feelings01.Value( iPlayer1, iPlayer2 );
	fFeelings += fDelta;
	m_Feelings01.Value( iPlayer1, iPlayer2 ) = fFeelings;
}

float CEOSAIForeignRelationsState::GetFeelings( long iPlayer1, long iPlayer2 )
{
	ASSERT( iPlayer1 != iPlayer2 );
	return m_Feelings01.Value( iPlayer1, iPlayer2 );
}

void  CEOSAIForeignRelationsState::LimitFeelings01()
{
	for( long iPlayer1=1; iPlayer1<m_iNumberOfPlayers+1; iPlayer1++ )
	{
		for( long iPlayer2=1; iPlayer2<m_iNumberOfPlayers+1; iPlayer2++ )
		{
			float fCurr = m_Feelings01.Value( iPlayer1, iPlayer2 );
			if( fCurr >= 0.25f && fCurr <= 0.75f )
			{
				m_Feelings01.Value( iPlayer1, iPlayer2 ) = fCurr;
			}
			else
			{
				m_Feelings01.Value( iPlayer1, iPlayer2 ) = CEOSAIMath::Sigma01( 0.25f, 0.75f, fCurr );
			}
		}
	}
}

/*
void  CEOSAIForeignRelationsState::SetPlayerAccessibility( long iPlayer1, long iPlayer2, float fValue )
{
	ASSERT( iPlayer1 != iPlayer2 );
	m_PlayerAccessibility.Value( iPlayer1, iPlayer2 ) = fValue;
}
*/
float CEOSAIForeignRelationsState::GetPlayerAccessibility( long iPlayer1, long iPlayer2 )
{
	ASSERT( iPlayer1 != iPlayer2 );
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//return pWorldDescServer->GetAICommonData()->GetAINationalSummary3( iPlayer1 )->GetPlayerAccessibilty( iPlayer2 );
	return g_pEOSAICommonData->GetAINationalSummary3( iPlayer1 )->GetPlayerAccessibilty( iPlayer2 );

	//return m_PlayerAccessibility.Value( iPlayer1, iPlayer2 );
	//return m_pStrategicAI->GetNationalSummary2( iPlayer1 )->GetPlayerAccessibilty( iPlayer2 );
}

void  CEOSAIForeignRelationsState::GetPlayerTeammates( long iPlayer, CEOSAIIntSet* pPlayerTeammates )
{
	pPlayerTeammates->Clear();
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		EOSAIEnumForeignRelations  XYRelationship = m_ForeignRelations.Value( iPlayer,y );
		if( XYRelationship == EOSAIEnumForeignRelations::enum_Teammate ){ pPlayerTeammates->Add( y ); }
	}
}

void  CEOSAIForeignRelationsState::GetPlayerAllies( long iPlayer, CEOSAIIntSet* pPlayerAllies )
{
	pPlayerAllies->Clear();
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		EOSAIEnumForeignRelations  XYRelationship = m_ForeignRelations.Value( iPlayer,y );
		if( XYRelationship == EOSAIEnumForeignRelations::enum_Alliance ){ pPlayerAllies->Add( y ); }
	}
}

void  CEOSAIForeignRelationsState::GetPlayerEnemies( long iPlayer, CEOSAIIntSet* pPlayerEnemies )
{
	pPlayerEnemies->Clear();
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		EOSAIEnumForeignRelations  XYRelationship = m_ForeignRelations.Value( iPlayer,y );
		if( XYRelationship == EOSAIEnumForeignRelations::enum_War ){ pPlayerEnemies->Add( y ); }
	}
}

/*
float CEOSAIForeignRelationsState::GetPowerOfPlayerAndTeammates( long iPlayer )
{
	float fPower = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		// One-degree relationship
		EOSAIEnumForeignRelations  XYRelationship = m_ForeignRelations.Value( iPlayer,y );
		float fMult = 0.0f;
		if( XYRelationship == EOSAIEnumForeignRelations::enum_Self     ){ fMult =  1.0f; }
		if( XYRelationship == EOSAIEnumForeignRelations::enum_Teammate ){ fMult =  1.0f; }
		fPower = fMult * GetPower( y );
	}
	return fPower;
}

float CEOSAIForeignRelationsState::GetPowerOfPlayerTeammatesAllies( long iPlayer )
{
	float fPower = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		// One-degree relationship
		EOSAIEnumForeignRelations  XYRelationship = m_ForeignRelations.Value( iPlayer,y );
		float fMult = 0.0f;
		if( XYRelationship == EOSAIEnumForeignRelations::enum_Self     ){ fMult =  1.0f; }
		if( XYRelationship == EOSAIEnumForeignRelations::enum_Teammate ){ fMult =  1.0f; }
		if( XYRelationship == EOSAIEnumForeignRelations::enum_Alliance ){ fMult =  0.6f; }
		fPower = fMult * GetPower( y );
	}
	return fPower;
}

float CEOSAIForeignRelationsState::GetPowerOfPlayerEnemies( long iPlayer )
{
	float fPower = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		// One-degree relationship
		EOSAIEnumForeignRelations  XYRelationship = m_ForeignRelations.Value( iPlayer,y );
		float fMult = 0.0f;
		if( XYRelationship == EOSAIEnumForeignRelations::enum_War ){ fMult =  1.0f; }
		fPower = fMult * GetPower( y );
	}
	return fPower;
}
*/
float CEOSAIForeignRelationsState::GetSATEPowerBalanceBasedOnMatrix( long iPlayer )
{
	float fOutgoingPower = GetPower( iPlayer );
	float fIncomingPower = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		fIncomingPower += m_PlayerPowerAllocationMatrix.Value( y, iPlayer );
	}
	return fOutgoingPower - fIncomingPower;
}

float CEOSAIForeignRelationsState::GetSATEPowerBalanceBasedOnMatrix01( long iPlayer ) // 0 to 1
{
	float fOutgoingPower = GetPower( iPlayer );
	float fIncomingPower = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		fIncomingPower += m_PlayerPowerAllocationMatrix.Value( y, iPlayer );
	}
	if( fOutgoingPower + fIncomingPower == 0.0f ) return 0.0f;
	return fOutgoingPower / ( fOutgoingPower + fIncomingPower );
}

float CEOSAIForeignRelationsState::GetSATPower01( long iPlayer )
{
	return m_SATGlobalPower01.Value( iPlayer );
}

float CEOSAIForeignRelationsState::GetGlobalPower()
{
	float fPower = 0.0f;
	for( long iPlayer=1; iPlayer<m_iNumberOfPlayers+1; iPlayer++ )
	{
		fPower += GetPower( iPlayer );
	}
	return fPower;
}

long CEOSAIForeignRelationsState::GetNumberOfDirectEnemies( long iPlayer )
{
	long iCount = 0;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		if( m_ForeignRelations.Value( iPlayer,y ) == EOSAIEnumForeignRelations::enum_War ) iCount++;
	}
	return iCount;
}

long CEOSAIForeignRelationsState::GetNumberOfDirectAndIndirectEnemies( long iPlayer )
{
	long iCount = 0;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		EOSAI::CGamePlayer* pPlayerY = g_pEOSAIInterface->GetGamePlayer( y );
		if( pPlayerY->IsAlive() == false ) continue;

		if( m_PlayerRelationsMatrix.Value( iPlayer,y ) < -0.99f ) iCount++;
	}
	return iCount;
}

float CEOSAIForeignRelationsState::GetPlayerExpansion( long iPlayer )
{
	// The player's score depends on a number of factors, 
	//   including ExpectedExpansion and potential winner
	float fPlayerExpansion =
		m_ExpectedConquest11.Value( iPlayer ) +
		m_ExpectedUnownedExpansion01.Value( iPlayer );

	return fPlayerExpansion;
}

float CEOSAIForeignRelationsState::GetPositionalScore( long iPlayer )
{
	float fScore = 1.0f;

	float fSATPower = m_SATGlobalPower01.Value( iPlayer );
	fScore += (0.3f*fSATPower);

	// The player's score depends on a number of factors, 
	//   including ExpectedExpansion and potential winner
	float fPlayerExpansion = GetPlayerExpansion( iPlayer );
	fScore += fPlayerExpansion;

	// Make sure no other players are getting too powerful
	//   Score goes down if some players are powerful and growing
	//float fAttackingPlayerWhoIsGettingTooPowerful = 0.0f;
	//float fNotAttackingPlayerWhoIsGettingTooPowerful = 0.0f;
	//float fNumberOfTeammates = 0.0f; // We want the AI to score lower with more teammates (otherwise they'll try to create teams)
	for( long iOtherPlayer=1; iOtherPlayer<m_iNumberOfPlayers+1; iOtherPlayer++ )
	{
		if( iOtherPlayer==iPlayer ) continue;
		EOSAIEnumForeignRelations eRel = m_ForeignRelations.Value( iPlayer,iOtherPlayer );
		if( eRel == EOSAIEnumForeignRelations::enum_Teammate )
		{
			// Don't want teammates (would prefer to win alone)
			fScore -= 0.1f;
			//fNumberOfTeammates+=1.0f;
			continue;
		}
		if( eRel == EOSAIEnumForeignRelations::enum_War )
		{
			// Attacking player who is getting too powerful
			//fAttackingPlayerWhoIsGettingTooPowerful += GetConcernThatPlayerIsTooPowerful( x );
			fScore += GetConcernThatPlayerIsTooPowerful( iOtherPlayer );
		}
		if( eRel == EOSAIEnumForeignRelations::enum_Alliance ||
			eRel == EOSAIEnumForeignRelations::enum_Neutral )
		{
			// Not attacking player who is getting too powerful
			//fNotAttackingPlayerWhoIsGettingTooPowerful += GetConcernThatPlayerIsTooPowerful( x );
			fScore -= GetConcernThatPlayerIsTooPowerful( iOtherPlayer );
		}
	}

	// Am I at war?  Do I want to be at war?
	long iNumberOfDirectEnemies = GetNumberOfDirectAndIndirectEnemies( iPlayer );
	float fGeneralWarDesire = GetGeneralDesireForWar01( iPlayer );
	if( iNumberOfDirectEnemies == 0 )
	{
		// Approaches -1.0 if the fGeneralWarDesire01 is 1.0
		// Approaches +1.0 if the fGeneralWarDesire01 is 0.0
		fScore -= 2.0f * (-0.5f + CEOSAIMath::Sigma01( 0.4f,0.6f, fGeneralWarDesire ));
	}
	if( iNumberOfDirectEnemies > 0 )
	{
		// Approaches +1.0 if the fGeneralWarDesire01 is 1.0
		// Approaches -1.0 if the fGeneralWarDesire01 is 0.0
		fScore += 2.0f * (-0.5f + CEOSAIMath::Sigma01( 0.4f,0.6f, fGeneralWarDesire ));
	}

	// Add other player's feelings (the AIPlayer prefers other players to have good feelings)
	for( long iOtherPlayer=1; iOtherPlayer<m_iNumberOfPlayers+1; iOtherPlayer++ )
	{
		if( iOtherPlayer==iPlayer ) continue;
		fScore += 0.1f * (-0.5f + m_Feelings01.Value( iOtherPlayer, iPlayer ));
	}

	//return 1.0f + (0.3f*fSATPower) + fPlayerExpansion + fAttackingPlayerWhoIsGettingTooPowerful - 0.1f*fNumberOfTeammates;
	return fScore;
}

long  CEOSAIForeignRelationsState::GetPlayerIWouldMostLikeToAttackBasedOnRawWarDesire( long iPlayer )
{
	long  iBestTarget = 0;
	float fHighestWarDesire = 0.2f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		if( iPlayer == y ) continue;
		float fWarDesire = m_RawWarDesire.Value( iPlayer,y );
		if( fWarDesire > fHighestWarDesire )
		{
			iBestTarget = y;
			fHighestWarDesire = fWarDesire;
		}
	}
	return iBestTarget;
}

/*
void  CEOSAIForeignRelationsState::GetFeeling_PlayerIsGettingTooPowerful( long iActor, CEOSAIBCDumbArray1D< float >* pPlayerIsGettingTooPowerful )
{
	if( pPlayerIsGettingTooPowerful->m_iSize == 0 ){ pPlayerIsGettingTooPowerful->SetSize( m_iNumberOfPlayers+1 ); }
	ASSERT( pPlayerIsGettingTooPowerful->m_iSize == m_iNumberOfPlayers+1 );

	long iActivePlayers = 0;
	float fGlobalPower = 0.0f;
	float fMostPowerfulPT  = 0.0f;
	float fMostPowerfulPTA = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		if( GetPower( y ) > 0.0f )
		{
			fGlobalPower += GetPower( y );
			iActivePlayers++;
			float fPowerPT  = GetPowerOfPlayerAndTeammates( y );
			float fPowerPTA = GetPowerOfPlayerTeammatesAllies( y );
			if( fMostPowerfulPT < fPowerPT )
			{
				fMostPowerfulPT = fPowerPT;
			}
			if( fMostPowerfulPTA < fPowerPTA )
			{
				fMostPowerfulPTA = fPowerPTA;
			}
		}
	}
	float fAveragePowerPerActivePlayer = fGlobalPower / iActivePlayers;

	// Rank all non-teammates by power
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		EOSAIEnumForeignRelations  Rel = m_ForeignRelations.Value( iActor,y );
		if( Rel == EOSAIEnumForeignRelations::enum_Self ||
			Rel == EOSAIEnumForeignRelations::enum_Teammate )
		{
			pPlayerIsGettingTooPowerful->Value( y ) = 0.0f;
		}
		else
		{
			float fDesireToWeaken = 0.0f;
			if( Rel == EOSAIEnumForeignRelations::enum_Alliance ){ fDesireToWeaken = 0.0f; }
			if( Rel == EOSAIEnumForeignRelations::enum_Neutral ){  fDesireToWeaken = 0.2f; }
			if( Rel == EOSAIEnumForeignRelations::enum_War ){      fDesireToWeaken = 0.4f; }
			//
			fDesireToWeaken += ( 1.0f - m_Feelings01.Value( iActor,y ) );

			float fPowerPT = GetPowerOfPlayerAndTeammates( y );
			float fModPower = ( fPowerPT - fAveragePowerPerActivePlayer ) / fMostPowerfulPT;
			fDesireToWeaken += 0.5f * fModPower;

			float fPowerPTA = GetPowerOfPlayerTeammatesAllies( y );
			float fModPowerPTA = ( fPowerPTA - fAveragePowerPerActivePlayer ) / fMostPowerfulPTA;
			fDesireToWeaken += 0.5f * fModPowerPTA;

			pPlayerIsGettingTooPowerful->Value( y ) = fDesireToWeaken;
		}
	}
}
*/

/*
void  CEOSAIForeignRelationsState::GetDesireToWeakenPlayer( long iActor, CEOSAIBCDumbArray1D< float >* pDesireToWeaken ) // 0.0 = no desire, 1.0 = need to attack
{
	if( pDesireToWeaken->m_iSize == 0 ){ pDesireToWeaken->SetSize( m_iNumberOfPlayers+1 ); }
	ASSERT( pDesireToWeaken->m_iSize == m_iNumberOfPlayers+1 );

	float fGlobalPower = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		fGlobalPower += m_Power.Value( y );

		// Rank all non-teammates by power
	}
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		EOSAIEnumForeignRelations  Rel = m_ForeignRelations.Value( iActor,y );
		if( Rel == EOSAIEnumForeignRelations::enum_Self ||
			Rel == EOSAIEnumForeignRelations::enum_Teammate )
		{
			pDesireToWeaken->Value( y ) = 0.0f;
		}
		else
		{
			float fDesireToWeaken = 0.0f;
			if( Rel == EOSAIEnumForeignRelations::enum_Alliance ){ fDesireToWeaken = 0.1f; }
			if( Rel == EOSAIEnumForeignRelations::enum_Neutral ){  fDesireToWeaken = 0.2f; }
			if( Rel == EOSAIEnumForeignRelations::enum_War ){      fDesireToWeaken = 0.4f; }
			//
			fDesireToWeaken += ( 1.0f - m_Feelings01.Value( iActor,y ) );

			//
			fGlobalPower = m_Power.Value( y ) / fGlobalPower;
		}
	}
}
*/

void  CEOSAIForeignRelationsState::CalculateSATGlobalPower()
{
	// Clear the SATGlobalPower values
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		m_SATGlobalPower01.Value( x ) = 0.0f;
	}
	float fGlobalPower = 0.0f;
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		// Add up all the global power
		fGlobalPower += GetPower( x );
	}
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		float fSATPower = 0.0f;
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_Self )
			{
				fSATPower += GetPower( y );
			}
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_Teammate )
			{
				fSATPower += GetPower( y );
			}
			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_Alliance )
			{
				fSATPower += 0.6f*GetPower( y );
			}
		}
		if( fGlobalPower == 0 ){ m_SATGlobalPower01.Value( x ) = 0.0f; }
		else{ m_SATGlobalPower01.Value( x ) = fSATPower / fGlobalPower; }
	}
}

// GetGeneralDesireForWar01:
//   Based on Maintenance costs, Unowned CitRes accessibility
//   
/*
float CEOSAIForeignRelationsState::GetGeneralDesireForWar01( long iPlayer )
{
	//float fPlayerPower = GetPower( iPlayer );
	//if( fPlayerPower == 0.0f ) return 0.0f;
	if( GetPower( iPlayer ) == 0.0f ) return 0.0f;

	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CAINationalSummary2* pAINationalSummary2 = pWorldDescServer->GetAINationalSummary3( iPlayer );
	float fDesireForWar = 0.0f;

	// DesireForANewWar Increases with Maintenance cost
	fDesireForWar += 2.0f * GetUnitMaintenanceCostsPercentage( iPlayer );

	// Get the ratio of CityProduction to UnitProduction
	float fCityProduction = pAINationalSummary2->m_CitResSummary.GetCityValue();
	float fUnitCost = pAINationalSummary2->m_UnitSummary.GetProductionValue();
	fDesireForWar += ( fUnitCost/50.0f ) / ( fUnitCost/50.0f + fCityProduction );

	// Desire for war decreases with Unowned CitRes Accessibility
	fDesireForWar -= GetUnownedCitResAccessibility01( iPlayer );

	return CEOSAIMath::Sigma01( 0.2f,0.8f, fDesireForWar );
}
*/
/*
void  CEOSAIForeignRelationsState::GetDesireForANewWar01( long iActor, CEOSAIBCDumbArray1D< float >* pDesireForWar ) // 0.0 = no desire, 1.0 = wants war
{
	float fDesireForANewWar = GetDesireForANewWar01( iActor );
	float fGlobalPower = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		fGlobalPower += GetPower( y );
	}

	// Look over the other players, decide who I want to attack
	//   Depends on: 
	//     General desire for war, 
	//     Player's current power balance with enemies,
	//     Potential target's power balance with enemies,
	//     Accessibility, 
	//     Feelings,
	//     Global power percentage (one player with too much power is dangerous)
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		if( y == iActor ) continue;
		float fDesireToAttackThisPlayer =
			fDesireForANewWar *
			m_Feelings01.Value( iActor, y ) *
			m_PlayerAccessibility.Value( iActor, y );

		EOSAIEnumForeignRelations  Rel = m_ForeignRelations.Value( iActor,y );
		if( Rel == EOSAIEnumForeignRelations::enum_Alliance )
		{}

		pDesireForWar->Value( y ) = fDesireToAttackThisPlayer;
	}
}
*/

void  CEOSAIForeignRelationsState::CalculateRawWarDesire() // 0.0 = no desire for war, 1.0 = wants war
{
	int g=0;

	// Clear the RawWar desires
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			m_RawWarDesire.Value( x,y ) = 0.0f;
		}
	}
	//
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		EOSAI::CGamePlayer* pPlayerX = g_pEOSAIInterface->GetGamePlayer( x );
		if( pPlayerX->IsAlive() == false )
		{
			continue;
		}

		//float fGeneralDesireForWar = this->GetGeneralDesireForWar01( x );
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( x == y ) continue;

			EOSAI::CGamePlayer* pPlayerY = g_pEOSAIInterface->GetGamePlayer( y );
			if( pPlayerY->IsAlive() == false )
			{
				m_RawWarDesire.Value( x,y ) = 0.0f;
				continue;
			}

			EOSAIEnumForeignRelations eRelations = m_ForeignRelations.Value( x,y );
			if( eRelations == EOSAIEnumForeignRelations::enum_Teammate )
			{
				m_RawWarDesire.Value( x,y ) = 0.0f;
				continue;
			}

			// If player y is at war with one of player x's teammates or allies, 
			//   then player x is more likely to go to war against player y.
			float fEnemyOfMyTeammateOrAlly = 0.0f;
			if( eRelations <= EOSAIEnumForeignRelations::enum_Neutral )
			{
				CEOSAIIntSet PlayerXTeammates;
				CEOSAIIntSet PlayerXAllies;
				CEOSAIIntSet PlayerYEnemies;
				GetPlayerTeammates( x, &PlayerXTeammates );
				GetPlayerAllies( x, &PlayerXAllies );
				GetPlayerEnemies( y, &PlayerYEnemies );

				if( PlayerXTeammates.Intersects( PlayerYEnemies ) )
				{
					fEnemyOfMyTeammateOrAlly = 0.7f;
				}
				else if( PlayerXAllies.Intersects( PlayerYEnemies ) )
				{
					fEnemyOfMyTeammateOrAlly = 0.4f;
				}

				fEnemyOfMyTeammateOrAlly += 0.1f  * PlayerXTeammates.GetNumberOfIntersections( PlayerYEnemies );
				fEnemyOfMyTeammateOrAlly += 0.05f * PlayerXAllies.GetNumberOfIntersections( PlayerYEnemies );
			}

			// Calculate the desire for war with PlayerY
			//   Based on:
			//   General desire for war,
			//   Feelings towards player,
			//   Player Accessibility,
			// Does not take PowerBalance into account because we don't know who else will join attack
			// Feelings 0.0  => NegativeFeelings  2.5
			// Feelings 0.25 => NegativeFeelings  1.5
			// Feelings 0.49 => NegativeFeelings  0.5
			// Feelings 0.5  => NegativeFeelings  0.5
			// Feelings 0.75 => NegativeFeelings  0.25
			// Feelings 1.0  => NegativeFeelings  0.0
			//float fNegativeFeelings = ( 1.0f - m_Feelings01.Value( x,y ) );
			//if( fNegativeFeelings > 0.5f ){ fNegativeFeelings = 0.5f+(fNegativeFeelings-0.5f)*4.0f; }
			CEOSAIMathFunction  FeelingsToWarDesireFunc;
			FeelingsToWarDesireFunc.SetInputOutput( 0.0f, 2.5f );
			FeelingsToWarDesireFunc.SetInputOutput( 0.5f, 0.5f );
			FeelingsToWarDesireFunc.SetInputOutput( 1.0f, 0.0f );
			float fNegativeFeelings = FeelingsToWarDesireFunc.GetOutput( m_Feelings01.Value( x,y ) );

			// Don't go to heavy on this - to prevent wolfpack attacks on at-war players
			//float fPlayerEmbattlement = 0.2f*( 1.0f - GetSATEPowerBalanceBasedOnMatrix01( y ) );
			// I'm turning this around and giving it a negative value.  If a player is at war, then
			//   reduce the desire for the AI to attack the player
			float fPlayerEmbattlement = -0.1f*( 1.0f - GetSATEPowerBalanceBasedOnMatrix01( y ) );
			//int g=0;

			// The AI should want to attack if the player is all cities and no units
			// 1.0 means all cities and no units, 0.0 means lots of units and few cities
			float fLowUnitPowerRatio = 0.0f;
			if( GetPower( y ) > 0.0f )
			{
				float DEBUG_fLowUnitPowerRatio = 1.0f - ( GetCombatSignificance( y ) / GetPower( y ) );

				CEOSAINationalSummary3* pPlayerSummary = g_pEOSAICommonData->GetAINationalSummary3( y );
				float fCityValue = pPlayerSummary->m_CitResSummary.GetCityValue();
				float fResValue = pPlayerSummary->m_CitResSummary.GetResValue();
				float fUnitCombatSignificanceDiv20 = pPlayerSummary->m_UnitSummary.GetCombatSignificance()/20.0f;

				// This calculation can get out of whack when players have one or two cities
				//   So, I'm changing it to water-down the effects in that case
				//fLowUnitPowerRatio = 1.0f - ( GetCombatSignificance( y ) / GetPower( y ) );
				fLowUnitPowerRatio = 1.0f - ( (fUnitCombatSignificanceDiv20 + 10.0f) / (fUnitCombatSignificanceDiv20 + fCityValue + 10.0f) );
				int h=0;
			}

			float fPlayerAccessibility01 = GetPlayerAccessibility( x,y );
			CEOSAIMathFunction  PlayerAccessibilityFunc;
			PlayerAccessibilityFunc.SetInputOutput( 0.0f, 0.0f );
			PlayerAccessibilityFunc.SetInputOutput( 0.4f, 0.0f );
			PlayerAccessibilityFunc.SetInputOutput( 0.8f, 1.0f );
			PlayerAccessibilityFunc.SetInputOutput( 0.9f, 1.2f );
			PlayerAccessibilityFunc.SetInputOutput( 1.0f, 1.3f );
			fPlayerAccessibility01 = PlayerAccessibilityFunc.GetOutput( fPlayerAccessibility01 );

			float fCombatStrategy01 = m_pStrategicAI->GetStrategy_CombatOrPlanForCombat01();
			CEOSAIMathFunction  CombatStrategyFunc;
			CombatStrategyFunc.SetInputOutput( 0.0f, 0.0f );
			CombatStrategyFunc.SetInputOutput( 0.1f, 0.0f );
			CombatStrategyFunc.SetInputOutput( 0.7f, 1.0f );
			CombatStrategyFunc.SetInputOutput( 1.0f, 1.3f );
			fCombatStrategy01 = CombatStrategyFunc.GetOutput( fCombatStrategy01 );

			float fConcernThatPlayerIsTooPowerful = GetConcernThatPlayerIsTooPowerful( y );

			float fDesireToAttackThisPlayer =
				//fGeneralDesireForWar *
				( fPlayerAccessibility01 ) *
				( fCombatStrategy01 ) * //0.3+m_pStrategicAI->GetStrategy_CombatOrPlanForCombat01() ) *
				( fNegativeFeelings +
				  fEnemyOfMyTeammateOrAlly +
				  fLowUnitPowerRatio +
				  fPlayerEmbattlement +
				  fConcernThatPlayerIsTooPowerful );

			if( eRelations == EOSAIEnumForeignRelations::enum_Alliance )
			{
				// lower the desire to attack allies
				//   (players generally have better feelings towards allies, so that affects it too)
				fDesireToAttackThisPlayer *= 0.8f;
			}

			m_RawWarDesire.Value( x,y ) = fDesireToAttackThisPlayer;
		}
	}

}

void  CEOSAIForeignRelationsState::CalculateGeneralWarDesire()
{
	ASSERT( m_bExpectedUnownedExpansion01 );

	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	// General War Desires
	for( long iPlayer=1; iPlayer<m_iNumberOfPlayers+1; iPlayer++ )
	{
		// General war desire is based on:
		//   Player's Maintenance Costs
		//   Player's Military/Production balance
		//   Accessibility to unowned CitRes

		//CAINationalSummary2* pAINationalSummary2 = pWorldDescServer->GetAICommonData()->GetAINationalSummary3( iPlayer );
		CEOSAINationalSummary3* pAINationalSummary2 = g_pEOSAICommonData->GetAINationalSummary3( iPlayer );

		//float fPlayerPower = GetPower( iPlayer );
		//if( fPlayerPower == 0.0f ) return 0.0f;
		if( GetPower( iPlayer ) == 0.0f )
		{
			m_GeneralWarDesire.Value( iPlayer ) = 0.0f;
			//return;
			continue;
		}

		float fDesireForWar = 0.0f;
		float fCityProduction = pAINationalSummary2->m_CitResSummary.GetCityValue();
		float fUnitCost = pAINationalSummary2->m_UnitSummary.GetProductionValue();

		// DesireForANewWar Increases with Maintenance cost
		// One problem here: at the beginning of the game, every player has one city and multiple units
		//   This throws-off the UnitMaintenanceCostsPercentage, making it high.
		// Fix: if the city-production is low, then lower the UnitMaintenanceCostsPercentage
		float fUnitMaintenanceCostsPercentage = GetUnitMaintenanceCostsPercentage( iPlayer );
		CEOSAIMathFunction UnitMaintenanceCityMultFunc;
		UnitMaintenanceCityMultFunc.SetInputOutput(  0.0, 0.0 );
		UnitMaintenanceCityMultFunc.SetInputOutput( 30.0, 1.0 );
		float fUnitMaintenanceCityMultFunc = UnitMaintenanceCityMultFunc.GetOutput( fCityProduction );
		fDesireForWar = 2.0f * fUnitMaintenanceCostsPercentage * fUnitMaintenanceCityMultFunc;

		CEOSAIMathFunction UnitMultFunc;
		UnitMultFunc.SetInputOutput(   0.0, 0.0 );
		UnitMultFunc.SetInputOutput( 200.0, 0.0 );
		UnitMultFunc.SetInputOutput( 400.0, 1.0 );
		fDesireForWar *= UnitMultFunc.GetOutput( fUnitCost );

		// Get the ratio of CityProduction to UnitProduction
		float fOldCalculation = ( fUnitCost/50.0f ) / ( fUnitCost/50.0f + fCityProduction );
		float fNewCalculation = ( fUnitCost/25.0f ) / ( 1.0f + fUnitCost/50.0f + fCityProduction );
		fDesireForWar += ( fUnitCost/25.0f ) / ( 1.0f + fUnitCost/50.0f + fCityProduction );
		/*
		CEOSAIMathFunction UnitToCityMultFunc;
		UnitToCityMultFunc.SetInputOutput(   0.0, 0.0 );
		UnitToCityMultFunc.SetInputOutput(  10.0, 0.1 ); // City Production 40,  8 Infantry (400) = 
		UnitToCityMultFunc.SetInputOutput(  20.0, 1.0 ); // City Production 40, 16 Infantry (400) = 
		UnitToCityMultFunc.SetInputOutput(  50.0, 2.0 ); // City Production 40, 40 Infantry (2000) = 
		fDesireForWar += UnitToCityMultFunc.GetOutput( fUnitCost / (1.0f+fCityProduction) );
		*/

		// Desire for war decreases with Unowned CitRes Accessibility, Increases with few unowned CitRes
		//fDesireForWar -= GetUnownedCitResAccessibility01( iPlayer );
		//float fUnownedCitResAccessibility01 = GetUnownedCitResAccessibility01( iPlayer );
		//fDesireForWar += 2.0f * (0.5f - fUnownedCitResAccessibility01);
		//float fExpectedExpansion01 = this->GetPlayerExpansion( iPlayer );
		//float fExpectedExpansion01b = m_ExpectedUnownedExpansion01.Value( iPlayer );
		//fDesireForWar += 0.2f - m_ExpectedUnownedExpansion01.Value( iPlayer );
		CEOSAIMathFunction UnownedExpansionToWarFunc;
		/*
		UnownedExpansionToWarFunc.SetInputOutput( 1.0, -0.5 );
		UnownedExpansionToWarFunc.SetInputOutput( 0.5, -0.4 );
		UnownedExpansionToWarFunc.SetInputOutput( 0.3,  0.0 );
		UnownedExpansionToWarFunc.SetInputOutput( 0.1, +0.2 );
		UnownedExpansionToWarFunc.SetInputOutput( 0.0, +0.5 );
		float fExpectedUnownedExpansion01 = m_ExpectedUnownedExpansion01.Value( iPlayer );
		fDesireForWar += UnownedExpansionToWarFunc.GetOutput( fExpectedUnownedExpansion01 );
		*/
		UnownedExpansionToWarFunc.SetInputOutput( 1.0f, 0.0f );
		UnownedExpansionToWarFunc.SetInputOutput( 0.5f, 0.3f );
		UnownedExpansionToWarFunc.SetInputOutput( 0.3f, 0.7f );
		UnownedExpansionToWarFunc.SetInputOutput( 0.15f,1.0f );
		UnownedExpansionToWarFunc.SetInputOutput( 0.0f, 1.6f );
		float fExpectedUnownedExpansion01 = m_ExpectedUnownedExpansion01.Value( iPlayer );
		fDesireForWar *= UnownedExpansionToWarFunc.GetOutput( fExpectedUnownedExpansion01 );

/*
		CAICommonData* pAICommonData = GetCommonState()->GetWorldDescServer()->GetAICommonData();
		float fUnownedCitRes = pAICommonData->GetPercentageOfUnownedCitResOnMap01();
		CEOSAIMathFunction UnownedCitResFunc;
		UnownedCitResFunc.SetInputOutput( 0.0, +0.5 );
		UnownedCitResFunc.SetInputOutput( 0.2, +0.2 );
		UnownedCitResFunc.SetInputOutput( 0.5,  0.0 );
		UnownedCitResFunc.SetInputOutput( 0.8, -0.2 );
		UnownedCitResFunc.SetInputOutput( 1.0, -0.5 );
		fDesireForWar += UnownedCitResFunc.GetOutput( fUnownedCitRes );
*/
		//fDesireForWar *= 0.3f * m_pStrategicAI->GetStrategy_CombatOrPlanForCombat01();
		CEOSAIMathFunction PlanForCombatFunc;
		PlanForCombatFunc.SetInputOutput( 0.0f, -0.7f );
		PlanForCombatFunc.SetInputOutput( 0.3f, -0.7f );
		PlanForCombatFunc.SetInputOutput( 0.7f,  0.0f );
		PlanForCombatFunc.SetInputOutput( 0.9f, +0.3f );
		PlanForCombatFunc.SetInputOutput( 1.0f, +0.6f );
		float fCombatStrategy01 = m_pStrategicAI->GetStrategy_CombatOrPlanForCombat01();
		//float fCombatStrategy01 = pAIPlayer->m_StrategicAI.GetStrategy_CombatOrPlanForCombat01();
		fDesireForWar += PlanForCombatFunc.GetOutput( fCombatStrategy01 );

		//m_pStrategicAI->GetAIPlayer();
		EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iPlayer );
		if( pAIPlayer )
		{
			fDesireForWar += 0.2f * ( pAIPlayer->GetPersonality_Aggressive01() - 0.5f );
		}
		else
		{
			fDesireForWar += 0.1f;
		}

		fDesireForWar = CEOSAIMath::Sigma01( 0.2f,0.8f, fDesireForWar );
		m_GeneralWarDesire.Value( iPlayer ) = fDesireForWar;
	}
}

void  CEOSAIForeignRelationsState::NormalizeWarDesires()
{
	for( long iPlayer=1; iPlayer<m_iNumberOfPlayers+1; iPlayer++ )
	{
		float fMaxWarDesire = 0.0f;
		for( long iOtherPlayer=1; iOtherPlayer<m_iNumberOfPlayers+1; iOtherPlayer++ )
		{
			fMaxWarDesire = max( fMaxWarDesire, m_RawWarDesire.Value( iPlayer,iOtherPlayer ) );
		}

		// We want to normalize the values - not find a single target, not yet - we want to consider
		//   other player's positions before we make that final determination
		float fMinWarDesire = fMaxWarDesire - max( 0.1f, fMaxWarDesire/2 );
		CEOSAIMathFunction  MathFunc;
		MathFunc.SetInputOutput( fMinWarDesire, 0.0f );
		MathFunc.SetInputOutput( fMaxWarDesire, 1.0f );

		for( long iOtherPlayer=1; iOtherPlayer<m_iNumberOfPlayers+1; iOtherPlayer++ )
		{
			float fValue = 
				0.5f * MathFunc.GetOutput( m_RawWarDesire.Value( iPlayer,iOtherPlayer ) ) +
				0.5f * m_RawWarDesire.Value( iPlayer,iOtherPlayer );
			fValue *= GetGeneralDesireForWar01( iPlayer );
			m_NormalizedWarDesire.Value( iPlayer,iOtherPlayer ) = fValue;
		}
	}
}

void  CEOSAIForeignRelationsState::CalculateFinalWarDesiresUsingPlayerMatrix()
{
	long iNumberOfActivePlayers = g_pEOSAIInterface->GetNumberOfActivePlayers();

	// Calculate iPlayer's desire to attack iOtherPlayer
	for( long iPlayer=1; iPlayer<m_iNumberOfPlayers+1; iPlayer++ )
	{
		EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
		if( pPlayer->IsAlive() == false ) continue;

		long iTopTarget = 0;
		float fTopTargetValue = 0.0f;
		for( long iOtherPlayer=1; iOtherPlayer<m_iNumberOfPlayers+1; iOtherPlayer++ )
		{
			if( iOtherPlayer == iPlayer ) continue;

			EOSAI::CGamePlayer* pOtherPlayer = g_pEOSAIInterface->GetGamePlayer( iOtherPlayer );
			if( pOtherPlayer->IsAlive() == false ) continue;

			// if there are 2 players, then there is nothing to calculate (no third parties)
			// if there are 3 players, fMultiplier = 0.43
			// if there are 4 players, fMultiplier = 0.35
			// if there are 5 players, fMultiplier = 0.3
			// if there are 6 players, fMultiplier = 0.26
			float fMultiplier = 0.1f + ( 1.0f / iNumberOfActivePlayers );

			// How badly do the other players want to attack this player?
			// If the other players want to attack this guy, then I should too
			float fCummulativeWarDesire = m_NormalizedWarDesire.Value( iPlayer,iOtherPlayer );
			if( fCummulativeWarDesire > 0.0f )
			{
				for( long iOtherPlayer2=1; iOtherPlayer2<m_iNumberOfPlayers+1; iOtherPlayer2++ )
				{
					if( iPlayer == iOtherPlayer2 ) continue;
					//CPlayer* pOtherPlayer2 = GetCommonState()->GetPlayer( iOtherPlayer2 );
					EOSAI::CGamePlayer* pOtherPlayer2 = g_pEOSAIInterface->GetGamePlayer( iOtherPlayer2 );
					if( pOtherPlayer2->IsAlive() == false ) continue;

					fCummulativeWarDesire += fMultiplier * m_NormalizedWarDesire.Value( iOtherPlayer2,iOtherPlayer );
				}
				// How badly does iOtherPlayer want to attack me?
				fCummulativeWarDesire += 0.3f * m_NormalizedWarDesire.Value( iOtherPlayer,iPlayer );
			}

			if( m_ForeignRelations.Value( iPlayer,iOtherPlayer ) == EOSAIEnumForeignRelations::enum_Teammate )
			{
				fCummulativeWarDesire = 0.0f;
			}

			m_FinalWarDesire.Value( iPlayer,iOtherPlayer ) = fCummulativeWarDesire;

			/*
			if( fCummulativeWarDesire > fTopTargetValue )
			{
				iTopTarget = iOtherPlayer;
				fTopTargetValue = fCummulativeWarDesire;
			}
			*/
		}
		/*
		m_TopWarTarget.Value( iPlayer ) = iTopTarget;
		m_TopWarTargetValue.Value( iPlayer ) = fTopTargetValue;
		*/
	}
}

void CEOSAIForeignRelationsState::GetTopTarget( long iPlayer, long* piTopTarget, float* pfTopTargetValue )
{
	*piTopTarget = 0;
	//*pfTopTargetValue = -1000000.0f;
	*pfTopTargetValue = 0.0f;
	for( long y=1; y<m_iNumberOfPlayers+1; y++ )
	{
		float fValue = m_FinalWarDesire.Value( iPlayer,y );
		if( fValue > *pfTopTargetValue )
		{
			*piTopTarget = y;
			*pfTopTargetValue = fValue;
		}
	}
	ASSERT( m_ForeignRelations.Value( iPlayer, *piTopTarget ) != EOSAIEnumForeignRelations::enum_Teammate );
}


void  CEOSAIForeignRelationsState::CalculateRawPursueFriendlyRelations01()
{
	// Based on the RawWar desire, player power
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			m_PursueFriendlyRelations.Value( x,y ) = 0.0f;
		}
	}
	//
	// Desire for Friendly Relations factors:
	// - Want to be friendly with a certain number of players
	//   (If PlayerX is strong, can attack multiple players at the same time.  If weak, one enemy.)
	// - It's not simply the negative DesireForWar
	//
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		// If there are several players I don't like, show some friendliness towards them in
		//   proportion to how much better I like them better than my worst hated enemy
		float fStrongDesireToAttack = 0.8f;
		float fMaximumWarDesire = fStrongDesireToAttack;
		float fMinimumWarDesire = 0.2f;
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( x==y ) continue;
			fMaximumWarDesire = max( fMaximumWarDesire, m_RawWarDesire.Value( x,y ) );
			fMinimumWarDesire = min( fMinimumWarDesire, m_RawWarDesire.Value( x,y ) );
		}

		float fPlayerSATPower = m_SATGlobalPower01.Value( x );
		for( long y=1; y<m_iNumberOfPlayers+1; y++ )
		{
			if( x==y ) continue;

			if( m_ForeignRelations.Value( x,y ) == EOSAIEnumForeignRelations::enum_Teammate )
			{
				m_PursueFriendlyRelations.Value( x,y ) = 1.0f;
				continue;
			}

			float fMinMaxWarDesireGap = fMaximumWarDesire - fMinimumWarDesire;
			float fDistanceToMax = (float) fabs( fMaximumWarDesire - m_RawWarDesire.Value( x,y ) );
			float fDistanceToMin = (float) fabs( fMinimumWarDesire - m_RawWarDesire.Value( x,y ) );
			float fNormalizedInverseWarBasedFriendlyDesire = fDistanceToMax / ( fMinMaxWarDesireGap );
			//
			// Skew the numbers based on player power and target power
			//   (be friendly if I am weak, or enemy is strong)
			float fSATPower = m_SATGlobalPower01.Value( y );
			float fFear = fSATPower / ( fPlayerSATPower + fSATPower );
			float fFearBasedFriendliness = 0.0f;
			if( fFear <  0.35f ){ fFearBasedFriendliness = 0.0f; }
			if( fFear >= 0.35f ){ fFearBasedFriendliness = (fFear-0.35f)*2.0f; }
			//
			float fFeelings = 1.5f * m_Feelings01.Value( x,y );
			//
			m_PursueFriendlyRelations.Value( x,y ) = 
				fNormalizedInverseWarBasedFriendlyDesire +
				fFearBasedFriendliness +
				fFeelings;
		}
	}
}

void CEOSAIForeignRelationsState::CalculateCurrentStances()
{
	// Situations:
	//   - I am currently at war with players (x,y)
	//     - Start an (additional) war with the player Z
	//     - Make peace with player x and/or y
	//     - Make peace with player x and/or y, and if peace: Start a war with the player Z
	//   - Not at war
	//     - Start a war with player Z
	//
	//   * Maintain peace with player Z (to prevent gang-ups)
	//
	for( long iPlayer=1; iPlayer<m_iNumberOfPlayers+1; iPlayer++ )
	{
		EOSAI::CGamePlayer* pPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
		if( pPlayer->IsAlive() == false ) continue;

		long  iTopTarget = 0;
		float fTopTargetValue = 0.0f;
		GetTopTarget( iPlayer, &iTopTarget, &fTopTargetValue );
		ASSERT( iTopTarget > 0 );
/*
#ifdef _DEBUG
		// 987987
		fTopTargetValue = 0.0f;
#endif
*/
		ASSERT( m_ForeignRelations.Value( iPlayer, iTopTarget ) != EOSAIEnumForeignRelations::enum_Self );
		ASSERT( m_ForeignRelations.Value( iPlayer, iTopTarget ) != EOSAIEnumForeignRelations::enum_Teammate );

		//
		EnumRelationsPlan  eTopTargetRelationsPlan = enum_None;
		long iCurrentDirectEnemies = GetNumberOfDirectAndIndirectEnemies( iPlayer );
		if( iCurrentDirectEnemies == 0 )
		{
			if( fTopTargetValue  <  0.5f ){ eTopTargetRelationsPlan = enum_Neutral; }
			eif( fTopTargetValue <  0.7f ){ eTopTargetRelationsPlan = enum_PlanWar; }
			else //eif( fTopTargetValue >= 0.8f )
			{
				ASSERT( m_ForeignRelations.Value( iPlayer, iTopTarget ) != EOSAIEnumForeignRelations::enum_War );
				//ASSERT( GetCommonState()->GetWorldDescServer()->GetForeignRelations( iPlayer, iTopTarget ) != EOSAIEnumForeignRelations::enum_War );
				ASSERT( g_pEOSAICommonData->GetForeignRelations( iPlayer, iTopTarget ) != EOSAIEnumForeignRelations::enum_War );

				if( pPlayer->m_bScenarioPlayerRestriction_CannotDeclareWar == false )
				{
					eTopTargetRelationsPlan = enum_StartWar;
				}
			}
		}
		else
		{
			if( m_ForeignRelations.Value( iPlayer, iTopTarget ) == EOSAIEnumForeignRelations::enum_War )
			{
				if( g_pEOSAICommonData->GetAllPlayersPermanentlyAtWar() )
				{
					eTopTargetRelationsPlan = enum_ContinueWar;
				}
				eif( pPlayer->m_bScenarioPlayerRestriction_CannotMakePeace )
				{
					eTopTargetRelationsPlan = enum_ContinueWar;
				}
				else
				{
					if(  fTopTargetValue <  0.6f ){ eTopTargetRelationsPlan = enum_MakePeace; }
					eif( fTopTargetValue >= 0.6f ){ eTopTargetRelationsPlan = enum_ContinueWar; }
				}
			}
			else
			{
				float fMyPower = GetCombatSignificance( iPlayer );
				float fEnemyPower = 0.0f;
				for( long i=1;i<m_iNumberOfPlayers+1; i++ )
				{
					if( m_ForeignRelations.Value( iPlayer, i ) == EOSAIEnumForeignRelations::enum_War ){ fEnemyPower += GetCombatSignificance( i ); }
				}

				// Can I add this player as another enemy?
				//float fMyPower = GetSATEPowerBalanceBasedOnMatrix( iPlayer );
				float fTopTargetPower = GetCombatSignificance( iTopTarget );
				float fNewPowerBalance = ( fMyPower - fEnemyPower - fTopTargetPower ) / fMyPower;
				if( fNewPowerBalance > 0.1f ) // Don't want to add yet another enemy
				{
					if( fTopTargetValue  <  0.4f ){ eTopTargetRelationsPlan = enum_Neutral; }
					eif( fTopTargetValue <  0.8f ){ eTopTargetRelationsPlan = enum_PlanWar; }
					eif( fTopTargetValue >= 0.8f )
					{
						ASSERT( m_ForeignRelations.Value( iPlayer, iTopTarget ) != EOSAIEnumForeignRelations::enum_War );
						ASSERT( g_pEOSAICommonData->GetForeignRelations( iPlayer, iTopTarget ) != EOSAIEnumForeignRelations::enum_War );

						if( pPlayer->m_bScenarioPlayerRestriction_CannotDeclareWar == false )
						{
							eTopTargetRelationsPlan = enum_StartWar;
						}
					}
				}
				else
				{
					eTopTargetRelationsPlan = enum_None; // Don't attack. I can't fight my existing enemies + this one
				}
			}
		}
		//eTopTargetRelationsPlan = enum_MaintainPeace;//enum_MakePeace;//enum_StartWar;
		//eTopTargetRelationsPlan = enum_MakePeace;//enum_StartWar;

		for( long iOtherPlayer=1; iOtherPlayer<m_iNumberOfPlayers+1; iOtherPlayer++ )
		{
			if( iPlayer == iOtherPlayer ) continue;

			if( iOtherPlayer == iTopTarget )
			{
				if( eTopTargetRelationsPlan == enum_StartWar )
				{
					ASSERT( m_ForeignRelations.Value( iPlayer, iTopTarget ) != EOSAIEnumForeignRelations::enum_Teammate );
					if( m_ForeignRelations.Value( iPlayer, iTopTarget ) == EOSAIEnumForeignRelations::enum_Teammate )
					{
						eTopTargetRelationsPlan = enum_Neutral;
					}
				}

				m_RelationsPlan.Value( iPlayer,iOtherPlayer ) = eTopTargetRelationsPlan;

				// DEBUG
				if( eTopTargetRelationsPlan == enum_StartWar )
				{
					if( m_ForeignRelations.Value( iPlayer,iOtherPlayer ) == EOSAIEnumForeignRelations::enum_War )
					{
						ASSERT( false );
					}
				}
			}
			else
			{
				if( m_ForeignRelations.Value( iPlayer, iOtherPlayer ) == EOSAIEnumForeignRelations::enum_War )
				{
					// Keep fighting weak players (finish them off)
					if( 0.2f*GetCombatSignificance( iPlayer ) > GetCombatSignificance( iOtherPlayer ) )
					{
						m_RelationsPlan.Value( iPlayer,iOtherPlayer ) = enum_Neutral;
					}
					else
					{
						//if( g_pCommonState->GetWorldBuildDesc()->GetAllPlayersPermanentlyAtWar() == false &&
						if( g_pEOSAICommonData->GetAllPlayersPermanentlyAtWar() == false &&
							pPlayer->m_bScenarioPlayerRestriction_CannotMakePeace == false )
						{
							m_RelationsPlan.Value( iPlayer,iOtherPlayer ) = enum_MakePeace;
						}
					}
				}
				else
				{
					// I don't want to start a war with this player
					float fMyDesireForWar = m_FinalWarDesire.Value( iPlayer,iOtherPlayer );
					float fHisDesireForWar = m_FinalWarDesire.Value( iOtherPlayer,iPlayer );

					float fWarDesireGap = fMyDesireForWar - fHisDesireForWar;
					//if( fMyDesireForWar < 0.2f && fHisDesireForWar > 0.3f && fWarDesireGap < -0.1f )
					if( fHisDesireForWar > 0.3f && fWarDesireGap < -0.2f )
					{
						m_RelationsPlan.Value( iPlayer,iOtherPlayer ) = enum_MaintainPeace;
					}
				}
			}
		}
		/*
		if( iCurrentDirectEnemies > 0 )
		{
		}
		else
		{
			for( long iOtherPlayer=1; iOtherPlayer<m_iNumberOfPlayers+1; iOtherPlayer )
			{

			}
		}
		*/
	}
}

void CEOSAIForeignRelationsState::CalculateDesireToConcedeGame()
{
	m_fDesireToConcedeGame = 0.0f;
	//m_bDesireToConcedeGame = false;
	float fGlobalPower = GetGlobalPower();

	long iAIPlayer = m_pStrategicAI->GetPlayerNumber();
	float fAIPlayerSATPower01 = GetSATPower01( iAIPlayer );
	//float fAIPlayerSATEPower01 = GetSATEPowerBalanceBasedOnMatrix01( iAIPlayer );
	// Tipping point: When my SATE balance is 0.2 (my enemies have 4 power for every 1 power I have)
	// If the AI's SATE power is 0.0, multiply the AIs desire to concede by 1.5
	// If the AI's SATE power is 0.2, multiply the AIs desire to concede by 1.0
	// If the AI's SATE power is 0.3, multiply the AIs desire to concede by 0.0
	CEOSAIMathFunction  AIPlayerSATPower01;
	AIPlayerSATPower01.SetInputOutput( 0.0f, 1.5f );
	AIPlayerSATPower01.SetInputOutput( 0.2f, 1.0f );
	AIPlayerSATPower01.SetInputOutput( 0.3f, 0.0f );
	float fAIPlayerSATPowerMultiplier = AIPlayerSATPower01.GetOutput( fAIPlayerSATPower01 );
	//if( fAIPlayerSATPowerMultiplier < 0.01f ) return;

	// If   0% of the CitRes are unowned, multiply by 1.0
	// If  10% of the CitRes are unowned, multiply by 1.0
	// If  20% of the CitRes are unowned, multiply by 0.75
	// If  30% of the CitRes are unowned, multiply by 0.50
	// If  40% of the CitRes are unowned, multiply by 0.25
	// If  50% of the CitRes are unowned, multiply by 0.0
	//CAICommonData* pAICommonData = GetCommonState()->GetWorldDescServer()->GetAICommonData();
	float fPercentUnownedCitRes = g_pEOSAICommonData->GetPercentageOfUnownedCitResOnMap01();
	CEOSAIMathFunction  UnownedCitRes01;
	UnownedCitRes01.SetInputOutput( 0.0f, 1.0f );
	UnownedCitRes01.SetInputOutput( 0.2f, 1.0f );
	UnownedCitRes01.SetInputOutput( 0.5f, 0.0f );
	float fUnownedCitResMultiplier = UnownedCitRes01.GetOutput( fPercentUnownedCitRes );

	float fMaximumTeamPower01Multiplier = 0.0f;
	float fMaximumPlayerPower01Multiplier = 0.0f;
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		if( iPlayer == iAIPlayer ) continue;

		// The AI will want to resign if the [m_fDesireToConcedeGame] is 1.0 or higher
		// Values of 1.0 in any single category = has no effect on the outcome

		EOSAIEnumForeignRelations eRelations = m_ForeignRelations.Value( iAIPlayer, iPlayer );
		if( eRelations >= EOSAIEnumForeignRelations::enum_Alliance ) continue;

		// If there is a team with a lot of power, be more willing to concede
		float fTeamPower01 = GetSATPower01( iPlayer );// / fGlobalPower;
		if( fTeamPower01 == 0.0f ) continue;
		float fTeamPower01Multiplier = 0.0f;
		if( eRelations.IsEnemy() )
		{
			// Enemy team has 100% of global power, multiply by 1.5
			// Enemy team has  80% of global power, multiply by 1.0
			// Enemy team has  70% of global power, multiply by 0.0
			CEOSAIMathFunction  TeamPower01;
			TeamPower01.SetInputOutput( 1.0f, 1.5f );
			TeamPower01.SetInputOutput( 0.8f, 1.0f );
			TeamPower01.SetInputOutput( 0.7f, 0.0f );
			fTeamPower01Multiplier = TeamPower01.GetOutput( fTeamPower01 );// / fGlobalPower );
			fMaximumTeamPower01Multiplier = max( fMaximumTeamPower01Multiplier, fTeamPower01Multiplier );
		}
		eif( eRelations.IsNeutral() )
		{
			// Enemy team has 100% of global power, multiply by 1.5
			// Enemy team has  85% of global power, multiply by 1.0
			// Enemy team has  60% of global power, multiply by 0.0
			CEOSAIMathFunction  TeamPower01;
			TeamPower01.SetInputOutput( 1.0f,  1.5f );
			TeamPower01.SetInputOutput( 0.85f, 1.0f );
			TeamPower01.SetInputOutput( 0.60f, 0.0f );
			fTeamPower01Multiplier = TeamPower01.GetOutput( fTeamPower01 );// / fGlobalPower );
			fMaximumTeamPower01Multiplier = max( fMaximumTeamPower01Multiplier, fTeamPower01Multiplier );
		}

		// What percentage of global power does this player have?
		//   Tipping point: When my major enemy has 80% global power
		float fPlayerPower01 = GetPower( iPlayer ) / fGlobalPower;
		float fPlayerPower01Multiplier = 0.0f;
		if( eRelations.IsEnemy() || eRelations.IsNeutral() )
		{
			// Enemy player has 100% of global power, multiply by 2.0
			// Enemy player has  90% of global power, multiply by 1.5
			// Enemy player has  80% of global power, multiply by 1.0 (no effect)
			CEOSAIMathFunction  PlayerPower01;
			PlayerPower01.SetInputOutput( 1.0f, 2.0f );
			PlayerPower01.SetInputOutput( 0.8f, 1.0f );
			fPlayerPower01Multiplier = PlayerPower01.GetOutput( fPlayerPower01 );
		}
		fMaximumPlayerPower01Multiplier = max( fMaximumPlayerPower01Multiplier, fPlayerPower01Multiplier );
	}

	m_fDesireToConcedeGame = 
		fUnownedCitResMultiplier *
		fAIPlayerSATPowerMultiplier *
		fMaximumTeamPower01Multiplier *
		fMaximumPlayerPower01Multiplier; //TotalEnemyPower01.GetOutput( fTotalEnemyPower / fGlobalPower ) *
		//MaximumEnemyPower01.GetOutput( fMostPowerfulEnemyPower / fGlobalPower );

	//m_bDesireToConcedeGame = ( m_fDesireToConcedeGame > 0.99f );
	int h=0;

		/*
		m_fDesireToConcedeGame = 
			UnownedCitRes01.GetOutput( fPercentUnownedCitRes ) *
			AISATEPower01.GetOutput( fAIPlayerSATEPowerBalance ) *
			TotalEnemyPower01.GetOutput( fTotalEnemyPower / fGlobalPower ) *
			MaximumEnemyPower01.GetOutput( fMostPowerfulEnemyPower / fGlobalPower );
		*/

	/*
	if( GetCommonState()->GetNumberOfActivePlayers() == 2 )
	{
		long iNumberOfPlayers = GetCommonState()->GetNumberOfPlayers();
		long iAIPlayer = m_pStrategicAI->GetPlayerNumber();
		CAICommonData* pAICommonData = GetCommonState()->GetWorldDescServer()->GetAICommonData();

		// The AI will want to resign if the [m_fDesireToConcedeGame] is 1.0 or higher
		// Values of 1.0 in any single category = has no effect on the outcome

		// Tipping point: When my 80% of the map has been captured
		float fPercentUnownedCitRes = pAICommonData->GetPercentageOfUnownedCitResOnMap01();
		CEOSAIMathFunction  UnownedCitRes01;
		UnownedCitRes01.SetInputOutput( 0.0f, 1.0f );
		UnownedCitRes01.SetInputOutput( 0.2f, 1.0f );
		UnownedCitRes01.SetInputOutput( 1.0f, 0.0f );

		float fAIPlayerSATEPowerBalance = GetSATEPowerBalanceBasedOnMatrix01( iAIPlayer );
		// Tipping point: When my SATE balance is 0.2 (my enemies have 4 power for every 1 power I have)
		CEOSAIMathFunction  AISATEPower01;
		AISATEPower01.SetInputOutput( 0.0f, 2.0f );
		AISATEPower01.SetInputOutput( 0.2f, 1.0f );
		AISATEPower01.SetInputOutput( 0.4f, 0.0f );
		AISATEPower01.SetInputOutput( 1.0f, 0.0f );

		// What percentage of global power do all my enemies have?
		//   Tipping point: When my combined enemy have 75% global power
		CEOSAIMathFunction  TotalEnemyPower01;
		TotalEnemyPower01.SetInputOutput( 0.0f, 0.2f );
		TotalEnemyPower01.SetInputOutput( 0.5f, 0.4f );
		TotalEnemyPower01.SetInputOutput( 0.75f,1.0f );
		TotalEnemyPower01.SetInputOutput( 1.0f, 1.5f );
		float fTotalEnemyPower = 0.0f;

		// Find my most powerful enemy.  What percentage of global power does that player have?
		//   Tipping point: When my major enemy has 80% global power
		CEOSAIMathFunction  MaximumEnemyPower01;
		MaximumEnemyPower01.SetInputOutput( 0.0f, 0.0f );
		MaximumEnemyPower01.SetInputOutput( 0.6f, 0.7f );
		MaximumEnemyPower01.SetInputOutput( 0.8f, 1.0f );
		MaximumEnemyPower01.SetInputOutput( 0.9f, 1.5f );
		MaximumEnemyPower01.SetInputOutput( 1.0f, 1.5f );
		float fMostPowerfulEnemyPower = 0.0f;

		float fGlobalPower = 0.0f;
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
			float fPlayerPower = this->GetPower( iPlayer );
			fGlobalPower += fPlayerPower;

			EOSAIEnumForeignRelations eRelations = m_ForeignRelations.Value( iAIPlayer, iPlayer );
			if( eRelations.IsEnemy() )
			{
				fMostPowerfulEnemyPower = max( fPlayerPower, fMostPowerfulEnemyPower );
				fTotalEnemyPower += fPlayerPower;
			}
		}
		m_fDesireToConcedeGame = 
			UnownedCitRes01.GetOutput( fPercentUnownedCitRes ) *
			AISATEPower01.GetOutput( fAIPlayerSATEPowerBalance ) *
			TotalEnemyPower01.GetOutput( fTotalEnemyPower / fGlobalPower ) *
			MaximumEnemyPower01.GetOutput( fMostPowerfulEnemyPower / fGlobalPower );
		//
		int j=0;
	}
	*/
}

float CEOSAIForeignRelationsState::GetConcernThatPlayerIsTooPowerful( long iPlayer )
{
	float fPlayerSATPower = m_SATGlobalPower01.Value( iPlayer );
	float fAverageActiveSATPower = 0.0f;
	long  iNumberOfActivePlayers = 0;

	// Concern is dependent on the percentage of the map that is owned
	//CAICommonData* pAICommonData = GetCommonState()->GetWorldDescServer()->GetAICommonData();
	float fOwnedCitRes01 = 1.0f - g_pEOSAICommonData->GetPercentageOfUnownedCitResOnMap01();
	//   0% owned = Multiply concern by 0.0
	//  10% owned = Multiply concern by 0.0
	//  20% owned = Multiply concern by 0.2
	//  30% owned = Multiply concern by 0.4
	//  40% owned = Multiply concern by 0.6
	//  50% owned = Multiply concern by 0.8
	//  60% owned = Multiply concern by 1.0
	CEOSAIMathFunction  MapOwnedMultiplerFunc;
	MapOwnedMultiplerFunc.SetInputOutput( 0.0f, 0.0f );
	MapOwnedMultiplerFunc.SetInputOutput( 0.1f, 0.0f ); //  10% owned = Multiply concern by 0.0
	MapOwnedMultiplerFunc.SetInputOutput( 0.6f, 1.0f ); //  60% owned = Multiply concern by 1.0
	MapOwnedMultiplerFunc.SetInputOutput( 1.0f, 1.0f );
	float fMapOwnedMultiplier01 = MapOwnedMultiplerFunc.GetOutput( fOwnedCitRes01 );
	if( fMapOwnedMultiplier01 == 0.0f ) return 0.0f;
//	float fMapOwnedMultiplier01 = ( 1.0f-fUnownedCitRes01 ); // 0.0 = everything unowned, 1.0 = everything owned
//	fMapOwnedMultiplier01 *= 2.0f;
//	fMapOwnedMultiplier01 -= 0.5f;
//	if( fMapOwnedMultiplier01 < 0.0f ){ fMapOwnedMultiplier01 = 0.0f; }
//	if( fMapOwnedMultiplier01 > 1.0f ){ fMapOwnedMultiplier01 = 1.0f; }

	long iMorePowerfulActivePlayers = 0;
	long iLessPowerfulActivePlayers = 0;
	for( long x=1; x<m_iNumberOfPlayers+1; x++ )
	{
		if( GetPower( x ) > 0.0f )
		{
			iNumberOfActivePlayers++;
			fAverageActiveSATPower += m_SATGlobalPower01.Value( x );
		}

		if( x == iPlayer ) continue;
		if( m_ForeignRelations.Value( x,iPlayer ) == EOSAIEnumForeignRelations::enum_Teammate ) continue;
		if( GetPower( x ) > 0.0f )
		{
			if( fPlayerSATPower > m_SATGlobalPower01.Value( x ) )
			{
				iLessPowerfulActivePlayers++;
			}
			else
			{
				iMorePowerfulActivePlayers++;
			}
		}
	}
	fAverageActiveSATPower /= iNumberOfActivePlayers;

	float fConcern = 0.0f;
	float fMorePowerfulActivePlayers = (float)iMorePowerfulActivePlayers;
	float fLessPowerfulActivePlayers = (float)iLessPowerfulActivePlayers;
	if( iMorePowerfulActivePlayers == 0 ) fConcern += 0.1f;
	float fPercentile = fLessPowerfulActivePlayers / ( fMorePowerfulActivePlayers + fLessPowerfulActivePlayers );
	if( fPercentile > 0.8 ){ fConcern += 0.1f; }

	float fSATPower = m_SATGlobalPower01.Value( iPlayer );
	if( fSATPower > 0.2f ) fConcern += fMapOwnedMultiplier01 * (fSATPower-0.2f)*2.0f;

	if( fPlayerSATPower > 1.3f * fAverageActiveSATPower ) fConcern += fMapOwnedMultiplier01 * 0.1f;
	if( fPlayerSATPower > 1.6f * fAverageActiveSATPower ) fConcern += fMapOwnedMultiplier01 * 0.1f;
	if( fPlayerSATPower > 2.0f * fAverageActiveSATPower ) fConcern += fMapOwnedMultiplier01 * 0.1f;

	// If a player is on an upward trajectory, I should be more worried about him
	float fPlayerTrajectory = 0.2f*( m_ExpectedConquest11.Value( iPlayer ) );
	fConcern += fPlayerTrajectory;

	return fMapOwnedMultiplier01 * fConcern;
}

float CEOSAIForeignRelationsState::GetUnownedCitResAccessibility01( long iPlayer )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//return pWorldDescServer->GetAINationalSummary3( iPlayer )->m_fUnownedCitResAccessibility01;
	CEOSAINationalSummary3* pSummary = g_pEOSAICommonData->GetAINationalSummary3( iPlayer );
	return pSummary->GetPlayerAccessibilty( 0 );

	//return pSummary->m_fUnownedCitResAccessibility01;
	//return 0.2f; // TEMPORARY
	//return m_pStrategicAI->GetNationalSummary2( iPlayer )->m_fUnownedCitResAccessibility01;
}

float CEOSAIForeignRelationsState::GetCombatSignificance( long iPlayer )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//return pWorldDescServer->GetAICommonData()->GetAINationalSummary3( iPlayer )->m_UnitSummary.GetCombatSignificance();
	return g_pEOSAICommonData->GetAINationalSummary3( iPlayer )->GetCombatSignificanceDiv20();
	//return m_Power.Value( iPlayer ) / 2.0f;
	//return m_pStrategicAI->GetNationalSummary2( iPlayer )->GetMilitaryPower();
}

float CEOSAIForeignRelationsState::GetPower( long iPlayer )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CEOSAINationalSummary3* pAINationalSummary2 = g_pEOSAICommonData->GetAINationalSummary3( iPlayer );
	return pAINationalSummary2->GetPower();
	//return m_Power.Value( iPlayer ); // TEMPORARY
	//return m_pStrategicAI->GetNationalSummary2( iPlayer )->GetPower();
}

float CEOSAIForeignRelationsState::GetUnitMaintenanceCostsPercentage( long iPlayer )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CEOSAINationalSummary3* pSummary = g_pEOSAICommonData->GetAINationalSummary3( iPlayer );

	float fMaintenanceCosts = pSummary->m_fMaintenanceCosts;
	float fCitResValue = pSummary->m_CitResSummary.GetCitResValue();

	return fMaintenanceCosts / fCitResValue;
	//return 0.1f; // TEMPORARY
	//return m_pStrategicAI->GetNationalSummary2( iPlayer )->m_fMaintenanceCosts;
}

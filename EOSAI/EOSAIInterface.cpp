
#include "stdafx.h"
#include "EOSAIInterface.h"
#include "EOSAIGamePlayer.h"
#include "AIPlayerDesc.h"
#include "AIPlayer.h"
#include "MessageFromAI_ForeignRelationsFeelings.h"
//#include "EOSAILogFile.h"

// This will actually be pointing to the application's derived class
EOSAI::CInterface* g_pEOSAIInterface = NULL;
EOSAI::CWorldDistanceTool* g_pWorldDistanceTool = NULL;
//CEOSAILogFile g_LogFile;

using namespace EOSAI;
//

CInterface::CInterface()
{
	//g_LogFile.Initialize( _T("AILogFile.txt") );

	g_pEOSAIInterface = this;
	m_AICommonData.SetEOSAIInterface( this );
	m_pAIWorldDistanceTool = NULL;

	m_iNumberOfGamePlayers = 0;
	m_bAllActiveHumanPlayersHaveSubmittedTurn = false;
	//m_bNeedToRebuildData = true;
	//m_bAIObjectsHaveBeenCreated = false;

	m_iCurrentTurn = 0;
	m_bGameHasEnded = false;

	m_bGameRulesHaveBeenCreated = false;
	m_bTechTreeHasBeenCreated = false;
	m_bAIRegionsAndMultiRegionsHaveBeenCreated = false;
	m_bAIObjectsHaveBeenCreated = false;

	m_bProcessingIsDone = false;
	m_bPauseTheAI = false;
	m_bShutdownTheAI = false;

	m_AIPlayerManager.SetEOSAIInterface( this );
	for( long i=0; i<EOSAI_MAX_NUMBER_OF_PLAYERS; i++ )
	{
		m_GamePlayers[i] = NULL;
		m_AIPlayerDesc[i] = NULL;
	}

	//m_bCommonAIObjectsHaveBeenCreated = false;
}

void CInterface::SetAIWorldDistanceTool( EOSAI::CWorldDistanceTool* p )
{
	m_pAIWorldDistanceTool = p;
	m_AICommonData.SetEOSAIWorldDistanceTool( p );
}

void CInterface::SetNumberOfPlayers( long iNumberOfPlayers )
{
	//new EOSAI::CGamePlayer[iNumberOfPlayers+1];
	m_iNumberOfGamePlayers = iNumberOfPlayers;

//	m_GamePlayers = (EOSAI::CGamePlayer**) malloc( sizeof(EOSAI::CGamePlayer*)*iNumberOfPlayers+1 );
//	m_AIPlayersData = (CPlayerData**) malloc( sizeof(CPlayerData*)*iNumberOfPlayers+1 ); //new (CPlayerData*)[iNumberOfPlayers+1];

	//m_GamePlayers.SetSize( iNumberOfPlayers+1 );
	//m_AIPlayersData.SetSize( iNumberOfPlayers+1 );
	//for( long i=0; i<iNumberOfPlayers; i++ )
	/*
	for( long i=0; i<MAX_NUMBER_OF_PLAYERS; i++ )
	{
		m_GamePlayers[i] = NULL;
		m_AIPlayersData[i] = NULL;
	}
	*/
}

void CInterface::InstanciateAIPlayers()
{
	ASSERT( false );
}

void CInterface::ActivateAIPlayers()
{
	GetAIPlayerManager()->ThreadShouldBePaused( false );
}

void CInterface::AddGamePlayer( EOSAI::CGamePlayer* pGamePlayer )
{
	ASSERT( m_GamePlayers != NULL );
	ASSERT( pGamePlayer->m_iPlayer > 0 );
	m_GamePlayers[pGamePlayer->m_iPlayer] = pGamePlayer;
}

void CInterface::AddAIPlayerDesc( EOSAI::AIPlayerDesc* pAIPlayerDesc )
{
	ASSERT( m_AIPlayerDesc != NULL );
	ASSERT( pAIPlayerDesc->m_iPlayer > 0 );
	m_AIPlayerDesc[pAIPlayerDesc->m_iPlayer] = pAIPlayerDesc;
}
/*
void CInterface::CreateAICommonData()
{
	// This doesn't really do much in CWorldDescServer::CreateAICommonData()
	//m_pAICommonData->SetNeedToRebuildData( true );
	m_AICommonData.SetNeedToRebuildData( true );
}
*/

EOSAI::CGamePlayer* CInterface::GetGamePlayer( long iPlayer )
{
	ASSERT( iPlayer >= 0 && iPlayer <= EOSAI_MAX_NUMBER_OF_PLAYERS );
	if( iPlayer >= 0 && iPlayer <= EOSAI_MAX_NUMBER_OF_PLAYERS )
	{
		return m_GamePlayers[iPlayer];
	}
	return NULL;
}

EOSAI::AIPlayer* CInterface::GetAIPlayer( long iPlayer )
{
	return m_AIPlayerManager.GetAIPlayer( iPlayer );
}

EOSAI::AIPlayerDesc* CInterface::GetAIPlayerDesc( long iPlayer )
{
	ASSERT( iPlayer >= 0 && iPlayer < EOSAI_MAX_NUMBER_OF_PLAYERS );
	if( iPlayer >= 0 && iPlayer < EOSAI_MAX_NUMBER_OF_PLAYERS )
	{
		return m_AIPlayerDesc[iPlayer];
	}
	return NULL;
}


//

long CInterface::GetNumberOfPlayers() // Dead or Alive
{
	long iPlayers = 0;
	for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
	{
		EOSAI::CGamePlayer* pPlayer = GetGamePlayer( iPlayer );
		if( pPlayer ) iPlayers++;
	}
	return iPlayers;
}

long CInterface::GetNumberOfActivePlayers() // Players who are alive
{
	long iPlayers = 0;
	for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
	{
		EOSAI::CGamePlayer* pPlayer = GetGamePlayer( iPlayer );
		if( pPlayer && pPlayer->IsAlive() ) iPlayers++;
	}
	return iPlayers;
}

long CInterface::GetNumberOfHumanPlayers()
{
	long iPlayers = 0;
	for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
	{
		EOSAI::CGamePlayer* pPlayer = GetGamePlayer( iPlayer );
		if( pPlayer && pPlayer->IsHuman() ) iPlayers++;
	}
	return iPlayers;
}

long CInterface::GetNumberOfActiveAIPlayers()
{
	int iPlayers = 0;
	for( long i=0; i<EOSAI_MAX_NUMBER_OF_PLAYERS; i++ )
	{
		EOSAI::CGamePlayer* pPlayer = m_GamePlayers[i];
		if( pPlayer && pPlayer->IsAI() ) iPlayers++;
	}
	return iPlayers;
}

void CInterface::CalculateNationwidePathways()
{
	m_AICommonData.CalculateNationwidePathways();
}

void CInterface::AddObjectIdsToAIRegionsAndMultiRegions()
{
	m_AICommonData.AddObjectIdsToAIRegionsAndMultiRegions();
}

// AIPoiObjects
//
CEOSAIPoiObject* CInterface::GetAIPoiObject( long iAIPoiObject )
{
	return m_AICommonData.GetAIPoiObject( iAIPoiObject );
	//CEOSAIPoiObject* pAIPoiObject2 = m_AICommonData.GetAIPoiObject( iAIPoiObject );
	//return pAIPoiObject2;
	//m_AICommonData.
}

// Regions
//
CEOSAIRegionManager2* CInterface::GetAIRegionManager()
{
	return m_AICommonData.GetAIRegionManager();
}

//
long CInterface::GetProcessingAIPlayer() // The player that's being processed
{
	return m_AIPlayerManager.GetProcessingAIPlayer();
}

void CInterface::SetNeedToRebuildData( bool b )
{
	m_AICommonData.SetNeedToRebuildData(b);
}

void CInterface::KillTheAIPlayerThreads()
{
	m_AIPlayerManager.ShutdownThread();
}

void CInterface::DeleteAIPlayers()
{
	m_AIPlayerManager.DeleteAIPlayers();
}

long CInterface::GetNumberOfAIPlayersWhoHaveFinishedProcessing()
{
	long iHaveFinishedProcessing = 0;
	for( long iPlayer=1; iPlayer<EOSAI_MAX_NUMBER_OF_PLAYERS; iPlayer++ )
	{
		EOSAI::AIPlayer* pAIPlayer = GetAIPlayer( iPlayer );
		if( pAIPlayer && pAIPlayer->IsReadyToSendTurn() ){ iHaveFinishedProcessing++; }
	}
	return iHaveFinishedProcessing;
}

// 
void CInterface::SendTradeAgreementResponseToAI(long iToAIPlayer, CString strTradeAgreementId, long iPlayerWhoResponded, EOSAIEnumTradeAgreementResponse eEOSAIResponse, EOSAIEnumTradeAgreementState eEOSAINewState)
{
	EOSAI::AIPlayer* pAIPlayer = GetAIPlayer(iToAIPlayer);
	ASSERT(pAIPlayer);
	if (pAIPlayer)
	{
		pAIPlayer->Incoming_TradeAgreementResponse(strTradeAgreementId, iPlayerWhoResponded, eEOSAIResponse, eEOSAINewState);
	}
}

void CInterface::SendMessageResponseToAI(long iToAIPlayer, long iFromPlayer, long iAIMessageUID, EOSAI::EnumAIMailResponse eResponse)
{
	EOSAI::AIPlayer* pAIPlayer = GetAIPlayer(iToAIPlayer);
	ASSERT(pAIPlayer);
	/*
	CEOSAIMailResponse* pResponse = new CEOSAIMailResponse();
	pResponse->m_iToAIPlayer = iToAIPlayer;
	pResponse->m_iFromPlayer = iFromPlayer;
	pResponse->m_iAIMessageUID = iAIMessageUID;
	pResponse->m_eResponse = eReponse;
	*/
	//ASSERT(false);
	if (pAIPlayer)
	{
		pAIPlayer->Incoming_IMailResponse(iFromPlayer, iAIMessageUID, eResponse);
	}
	//pAIPlayer->SendMessageResponseToAI(pResponse);
}

void CInterface::AddPlayerInteractionEvent(CEOSAIPlayerInteraction* pPlayerInteraction)
{
	//ASSERT(false); // Test that I hit this
	m_AICommonData.AddPlayerInteractionAndSendFeelingsUpdate(pPlayerInteraction);
}

void CInterface::SendMessageToAI(EOSAI::MessageToAI* pMessageToAI)
{
	//if (dynamic_cast<EOSAI::MessageToAI_WarWasDeclared*>(pMessageToAI))
	if ( pMessageToAI->SendToAllPlayers() )
	{
		// What thread processes this message?
		m_MessagesToAI.AddTail(pMessageToAI);
		return;
	}

	ASSERT(pMessageToAI->SendToAllPlayers() == false);
	POSITION pos = pMessageToAI->SendToPlayers()->GetList()->GetHeadPosition();
	while (pos)
	{
		int iPlayer = pMessageToAI->SendToPlayers()->GetList()->GetNext(pos);

		EOSAI::AIPlayer* pAIPlayer = m_AIPlayerManager.GetAIPlayer(iPlayer);
		if (pAIPlayer)
		{
			pAIPlayer->Incoming_MessageToAI(pMessageToAI);
		}
		else
		{
			ASSERT(false);
		}
	}
}
/*
CEOSAIGlobalForeignRelations CInterface::GetCurrentForeignRelationsFeelingsBasedOnPlayerInteractionHistory()
{
	m_AICommonData.CalculateForeignRelationsFeelingsBasedOnPlayerInteractionHistory();
	return *m_AICommonData.GetGlobalForeignRelations();
	//
	//EOSAI::MessageFromAI_ForeignRelationsFeelings* p = new EOSAI::MessageFromAI_ForeignRelationsFeelings();
	//p->Set(m_AICommonData.GetGlobalForeignRelations());
}
*/
void CInterface::UpdateForeignRelationsFeelings(int iCurrentTurn)
{
	m_AICommonData.CalculateForeignRelationsFeelingsBasedOnPlayerInteractionHistoryAndSendFeelingsUpdate(iCurrentTurn);
}


void CInterface::GetAIPlayersOpinionOnTradeAgreement(int iHumanPlayerNumber, int iAIPlayer, CEOSAITradeAgreement* pTradeAgreement, CString* pstrOpinionText, long* piOpinionSum)
{
	EOSAI::AIPlayer* pAIPlayer = GetAIPlayer(iAIPlayer);
	ASSERT(pAIPlayer);
	if (pAIPlayer)
	{
		pAIPlayer->EvaluateTradeAgreement(pTradeAgreement, pstrOpinionText, piOpinionSum);
		//pAIPlayer->GetAIPlayersOpinionOnTradeAgreement(iHumanPlayerNumber, m_pTradeAgreement, pstrOpinionText, piOpinionSum);
	}
}


// Load/Save
//
// The AI needs to remember data (like what happened historically in the game), so data needs to be saved and loaded.
//   The AI must be paused when saving the data
/*
void  CAIInterface::SerializeData( CEOSAISerial* pSerial )
{
	ASSERT( false );
}

void  CAIInterface::DeserializeData( CEOSAISerial* pSerial )
{
	ASSERT( false );
}
*/

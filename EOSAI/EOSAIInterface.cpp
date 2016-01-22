
#include "stdafx.h"
#include "EOSAIMain.h"
#include "EOSAIInterface.h"
#include "EOSAIGamePlayer.h"
#include "AIPlayerDesc.h"
#include "AIPlayer.h"
#include "UserInterface.h"
#include "MessageFromAI_ForeignRelationsFeelings.h"
#include "Interproc.h"
//#include "EOSAILogFile.h"

// This will actually be pointing to the application's derived class
EOSAI::CInterface* g_pEOSAIInterface = NULL;
EOSAI::CWorldDistanceTool* g_pWorldDistanceTool = NULL;
//CEOSAILogFile g_LogFile;

using namespace EOSAI;
//

CInterface::CInterface()
{
	g_pEOSAIInterface = this;
	//g_LogFile.Initialize( _T("AILogFile.txt") );
	//if( g_pEOSAIMain == NULL ){ new EOSAI::Main(); }

	g_pEOSAIInterface = this;
	//m_AICommonData.SetEOSAIInterface( this );
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
	//m_bPauseEOSAI = false;
	m_bShutdownEOSAI = false;

	//m_AIPlayerManager.SetEOSAIInterface( this );
	for( long i=0; i<EOSAI_MAX_NUMBER_OF_PLAYERS; i++ )
	{
		m_GamePlayers[i] = NULL;
		m_AIPlayerDesc[i] = NULL;
	}


	//m_bCommonAIObjectsHaveBeenCreated = false;
}

void CInterface::CreateEOSAI()
{
	if (g_pEOSAIMain == NULL){ new EOSAI::Main(); }
	ASSERT(g_pEOSAIMain);
	g_pEOSAIMain->InitializeInterprocessCommunication();
	g_pEOSAIMain->GetAIPlayerManager()->InstanciateThreadAndPauseIt();
}

/*
void CInterface::InitializeEOSAIUserInterface()
{
	m_AIPlayerManager.InitializeInterprocessCommunication();
}
*/
/*
void CInterface::ShutdownEOSAI()
{
	//m_AIPlayerManager.ShutdownInterprocessCommunication();
}
*/
/*
EOSAI::Main* CInterface::GetEOSAIMain()
{
	return g_pEOSAIMain;
	//GetEOSAIMain();
}
*/
void CInterface::SetAIWorldDistanceTool( EOSAI::CWorldDistanceTool* pTool )
{
	m_pAIWorldDistanceTool = pTool;
	EOSAI::Main::p()->GetAICommonData()->SetEOSAIWorldDistanceTool(pTool);
	g_pEOSAIMain->GetAICommonData()->SetEOSAIWorldDistanceTool(pTool);
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
/*
void CInterface::InstanciateAIPlayers()
{
	ASSERT( false );
}
*/
void CInterface::PauseTheAI_GameDataIsBeingUpdated() //ActivateAIPlayers()
{
	g_pEOSAIMain->GetAIPlayerManager()->SetThreadShouldBePaused(true);
}

void CInterface::UnPauseTheAI_GameDataWasUpdated() //ActivateAIPlayers()
{
	g_pEOSAIMain->GetAIPlayerManager()->SetThreadShouldBePaused(false);
}

void CInterface::AddGamePlayer( EOSAI::CGamePlayer* pGamePlayer )
{
	ASSERT( m_GamePlayers != NULL );
	ASSERT(pGamePlayer->m_iPlayer > 0);
	ASSERT(pGamePlayer->m_strPlayerName != "");
	m_GamePlayers[pGamePlayer->m_iPlayer] = pGamePlayer;
}

void CInterface::AddAIPlayerDesc( EOSAI::AIPlayerDesc* pAIPlayerDesc )
{
	ASSERT( m_AIPlayerDesc != NULL );
	ASSERT( pAIPlayerDesc->m_iPlayer > 0 );
	m_AIPlayerDesc[pAIPlayerDesc->m_iPlayer] = pAIPlayerDesc;
}

void CInterface::DeleteAIPlayer(int iCurrentPlayer)
{
	g_pEOSAIMain->GetAIPlayerManager()->DeleteAIPlayer(iCurrentPlayer);
}

void CInterface::WaitUntilAIPlayerIsRemoved(int iCurrentPlayer)
{
	g_pEOSAIMain->GetAIPlayerManager()->WaitUntilAIPlayerIsRemoved(iCurrentPlayer);
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
	return g_pEOSAIMain->GetAIPlayerManager()->GetAIPlayer( iPlayer );
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

long CInterface::GetNumberOfGamePlayers() // Dead or Alive
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

int  CInterface::GetNumberOfTechnologyDescs(){ return g_pEOSAIMain->GetAIGameRules()->GetTechnologyDescsList()->GetCount(); }
void CInterface::AddTechnologyDesc(CEOSAITechnologyDesc* pAITechnologyDesc){ g_pEOSAIMain->GetAIGameRules()->AddTechnologyDesc(pAITechnologyDesc); }

//
void CInterface::CalculateAIUnitCombatCapabilities()
{
	//ASSERT( m_iUnitCombatCapabilities_LastTurnCalculated != iCurrentTurn );
	//if( m_iUnitCombatCapabilities_LastTurnCalculated != iCurrentTurn )
	{
		//m_iUnitCombatCapabilities_LastTurnCalculated = iCurrentTurn;

		//CWorldDesc* pWorldDesc = g_pCommonState->GetWorldDescServer();//m_pWorldDescServer;
		/*
		if( pWorldDesc == NULL )
		{
		POSITION pos = g_pCommonState->GetLocalPlayerWorldDescList()->GetHeadPosition();
		while( pos )
		{
		CWorldDescPlayer* pWorldDescPlayer = g_pCommonState->GetLocalPlayerWorldDescList()->GetNext( pos );
		if( pWorldDescPlayer->GetCurrentTurn() == iCurrentTurn )
		{
		pWorldDesc = pWorldDescPlayer;
		break;
		}
		}
		}
		*/
		//ASSERT( pWorldDesc );
		//if( pWorldDesc == NULL ) return;

		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		//CWorldBuildDesc*  pWorldBuildDesc = pWorldDescServer->GetWorldBuildDesc();
		//CWorldBuildDesc* pWorldBuildDesc = pWorldDesc->GetWorldBuildDesc();

		// Compile a list of all UnitTemplates in the game
		/*
		CEOSAIUnitTemplateSet  AllUnitsInTheGame;
		POSITION pos = pWorldDesc->GetPoiList()->GetHeadPosition();
		while( pos )
		{
		CPoi* pPoi = pWorldDesc->GetPoiList()->GetNext( pos )->GetPtr();
		CUnit* pUnit = dynamic_cast< CUnit* >( pPoi );
		if( pUnit )
		{
		//AllUnitsInTheGame.AddUnitTemplate( pUnit->GetUnitTemplate() );
		AllUnitsInTheGame.AddUnitTemplate( pUnit->GetUnitTemplate() );
		}
		}
		*/
		CEOSAIUnitTemplateSet  AllUnitsInTheGame;
		/*
		POSITION pos = GetCommonState()->GetActiveUnitset()->GetUnitTemplateList()->GetHeadPosition();
		while( pos )
		{
		CUnitTemplate* pUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplateList()->GetNext( pos );
		//CEOSAIUnitTemplate* pEOSAIUnitTemplate = new CEOSAIUnitTemplate( &m_AIGameRules );
		//ConvertToEOSAIUnitTemplate( pUnitTemplate, pEOSAIUnitTemplate );
		CEOSAIUnitTemplate* pEOSAIUnitTemplate = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplate( pUnitTemplate->GetInternalName() );
		AllUnitsInTheGame.AddUnitTemplate( pEOSAIUnitTemplate );
		}
		*/
		POSITION pos = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplates()->GetHeadPosition();
		while (pos)
		{
			//CUnitTemplate* pUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplateList()->GetNext( pos );
			//CEOSAIUnitTemplate* pEOSAIUnitTemplate = new CEOSAIUnitTemplate( &m_AIGameRules );
			//ConvertToEOSAIUnitTemplate( pUnitTemplate, pEOSAIUnitTemplate );
			//CEOSAIUnitTemplate* pEOSAIUnitTemplate = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplate( pUnitTemplate->GetInternalName() );

#ifdef THINGS_TO_COMPILE_EVENTUALLY
			This should include only units in the game, right ?
				I should also include all the buildable units in the game(like the code 10 lines down)
#endif THINGS_TO_COMPILE_EVENTUALLY

				CEOSAIUnitTemplate* pEOSAIUnitTemplate = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplates()->GetNext(pos);
			AllUnitsInTheGame.AddUnitTemplate(pEOSAIUnitTemplate);
		}

		// Compile all the buildable units in the game
		/*
		long iNumberOfBuildableUnits = 0;
		long iNumberOfPlayers = pWorldBuildDesc->GetNumberOfPlayers();
		pos = GetCommonState()->GetActiveUnitset()->GetBuildOptionsList()->GetHeadPosition();
		while( pos )
		{
		CBuildOption* pBuildOption = GetCommonState()->GetActiveUnitset()->GetBuildOptionsList()->GetNext( pos );
		if( pBuildOption->IsAUnit() == false ) continue;
		//
		bool bCanBuild = false;
		for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
		{
		if( GetCommonState()->GetActiveUnitset()->CanBuild( iPlayer, pBuildOption, false ) )
		{
		bCanBuild = true;
		break;
		}
		}
		if( bCanBuild )
		{
		iNumberOfBuildableUnits++;
		AllUnitsInTheGame.AddUnitTemplate( pBuildOption->GetUnitTemplate() );
		//ASSERT( false );
		}
		}
		ASSERT( iNumberOfBuildableUnits > 0 );
		*/

		// Invoke all the CombatAttrition values between MyUnits and PotentialEnemy units
		ASSERT(AllUnitsInTheGame.m_List.IsEmpty() == FALSE);
		pos = AllUnitsInTheGame.m_List.GetHeadPosition();
		while (pos)
		{
			//CUnitTemplate* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );
			CEOSAIUnitTemplate* pMyUnitTemplate = AllUnitsInTheGame.m_List.GetNext(pos);
			pMyUnitTemplate->GetAIUnitCombatCapability()->CalculateAverageVisibleRange(&AllUnitsInTheGame);
			int g = 0;
		}
		pos = AllUnitsInTheGame.m_List.GetHeadPosition();
		while (pos)
		{
			//CUnitTemplate* pMyUnit = AllUnitsInTheGame.m_List.GetNext( pos );
			CEOSAIUnitTemplate* pMyUnitTemplate = AllUnitsInTheGame.m_List.GetNext(pos);
			pMyUnitTemplate->GetAIUnitCombatCapability()->CalculateRelativeValue(&AllUnitsInTheGame, &AllUnitsInTheGame);
		}
	}
}

void CInterface::CalculateNationwidePathways()
{
	g_pEOSAIMain->GetAICommonData()->CalculateNationwidePathways();
}

void CInterface::AddObjectIdsToAIRegionsAndMultiRegions()
{
	g_pEOSAIMain->GetAICommonData()->AddObjectIdsToAIRegionsAndMultiRegions();
}

void                          CInterface::AddAIUnitTemplate(CEOSAIUnitTemplate* pAIUnitTemplate){ g_pEOSAIMain->GetAIGameRules()->AddAIUnitTemplate(pAIUnitTemplate); }
CEOSAIUnitTemplate*           CInterface::GetAIUnitTemplate(CString strUnitTemplate){ return g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplate(strUnitTemplate); }
CList< CEOSAIUnitTemplate* >* CInterface::GetAIUnitTemplates(){ return g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplates(); }
CEOSAIBuildingDescription*    CInterface::GetAIBuildingDescription(CString strBuildingDesc){ return g_pEOSAIMain->GetAIGameRules()->GetAIBuildingDescription(strBuildingDesc); }

void                     CInterface::AddCombatUnitType(CEOSAICombatUnitType* p){ g_pEOSAIMain->GetAIGameRules()->AddCombatUnitType(p); }
CEOSAICombatUnitType*    CInterface::GetCombatUnitType(long iCombatUnitType){ return g_pEOSAIMain->GetAIGameRules()->GetCombatUnitType(iCombatUnitType); }

void                     CInterface::AddMovementUnitType(CEOSAIMovementUnitType* pAIMovementUnitType){ g_pEOSAIMain->GetAIGameRules()->AddMovementUnitType(pAIMovementUnitType); }
CEOSAIMovementUnitType*  CInterface::GetMovementUnitType(long iMovementUnitType){ return g_pEOSAIMain->GetAIGameRules()->GetMovementUnitType(iMovementUnitType); }

void  CInterface::AddBuildingDescription(CEOSAIBuildingDescription* pAIBuildingDescription){ g_pEOSAIMain->GetAIGameRules()->AddBuildingDescription(pAIBuildingDescription); }

void  CInterface::SetInitialCanBuildUnit(CString strUnitsubset, CString strInternalUnitName){ g_pEOSAIMain->GetAIGameRules()->SetInitialCanBuildUnit(strUnitsubset, strInternalUnitName); }
void  CInterface::SetInitialCanBuildBuilding(CString strUnitsubset, CString strInternalBuildingName){ g_pEOSAIMain->GetAIGameRules()->SetInitialCanBuildBuilding(strUnitsubset,strInternalBuildingName); }


CEOSAIRegionManager2* CInterface::GetAIRegionManager() { return g_pEOSAIMain->GetAIRegionManager(); } // TODO: This is temporary, because we highlight pathways
EOSAI::CCommonData*   CInterface::GetAICommonData(){ return g_pEOSAIMain->GetAICommonData(); }
EOSAI::CGameRules*    CInterface::GetAIGameRules(){ return g_pEOSAIMain->GetAIGameRules(); }

void  CInterface::SetCurrentTurn(long iTurn)
{
	ASSERT(iTurn > 0);
	if (m_iCurrentTurn != iTurn)
	{
		m_iCurrentTurn = iTurn;
		//
		CString str; str.Format(_T("Beginning Turn %d"), iTurn);
		Interproc::AddStatusLine(str);
	}
}



// AIPoiObjects
//
CEOSAIPoiObject* CInterface::GetAIPoiObject( long iAIPoiObject )
{
	return g_pEOSAIMain->GetAICommonData()->GetAIPoiObject(iAIPoiObject);
}

// Regions
//
/*
CEOSAIRegionManager2* CInterface::GetAIRegionManager()
{
	return g_pEOSAIMain->GetAICommonData()->GetAIRegionManager();
}
*/
/*
void CInterface::ShowUI(bool b)
{
	m_AIPlayerManager.ShowUI(b);
}
*/

void CInterface::SetAllPlayersPermanentlyAtWar(bool b)
{
	g_pEOSAIMain->GetAICommonData()->SetAllPlayersPermanentlyAtWar(b);
}

//
long CInterface::GetProcessingAIPlayer() // The player that's being processed
{
	return g_pEOSAIMain->GetAIPlayerManager()->GetProcessingAIPlayer();
}

void CInterface::SetNeedToRebuildData( bool b )
{
	g_pEOSAIMain->GetAICommonData()->SetNeedToRebuildData(b);
}

bool CInterface::GetAllAIPlayersAreReadyToSendTurn(){ return g_pEOSAIMain->GetAIPlayerManager()->GetAllAIPlayersAreReadyToSendTurn(); }

void CInterface::ShutdownThread(){ g_pEOSAIMain->GetAIPlayerManager()->ShutdownThread(); }
void CInterface::DeleteAIPlayers(){ g_pEOSAIMain->GetAIPlayerManager()->DeleteAIPlayers(); }

void CInterface::SetDebugPause(bool bPause){ return g_pEOSAIMain->GetAIPlayerManager()->SetDebugPause(bPause); }
bool CInterface::GetDebugPause(){ return g_pEOSAIMain->GetAIPlayerManager()->GetDebugPause(); }

long CInterface::GetGameShouldPauseAtTheEndOfTurn(){ return g_pEOSAIMain->GetGameShouldPauseAtTheEndOfTurn(); } // Set by the EOSAIDebugger

void CInterface::SetThreadShouldBePaused_ForSaveGame(bool b){ g_pEOSAIMain->GetAIPlayerManager()->SetThreadShouldBePaused_ForSaveGame(b); }
bool CInterface::ThreadIsRunning(){ return g_pEOSAIMain->GetAIPlayerManager()->ThreadIsRunning(); }
bool CInterface::ThreadIsPaused(){ return g_pEOSAIMain->GetAIPlayerManager()->ThreadIsPaused(); }

bool CInterface::GetThreadShouldBePaused(){ return g_pEOSAIMain->GetAIPlayerManager()->GetThreadShouldBePaused(); }
void CInterface::SetThreadShouldBePaused(bool b){ g_pEOSAIMain->GetAIPlayerManager()->SetThreadShouldBePaused(b); }

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
	g_pEOSAIMain->GetAICommonData()->AddPlayerInteraction(pPlayerInteraction);
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

		//EOSAI::AIPlayer* pAIPlayer = m_AIPlayerManager.GetAIPlayer(iPlayer);
		EOSAI::AIPlayer* pAIPlayer = g_pEOSAIMain->GetAIPlayerManager()->GetAIPlayer(iPlayer);
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
void CInterface::UpdateForeignRelationsState2(int iCurrentTurn)
{
	g_pEOSAIMain->GetAICommonData()->CalculateForeignRelationsFeelingsBasedOnPlayerInteractionHistoryAndSendFeelingsUpdate(iCurrentTurn);
}

void CInterface::SendMessageFromAI(EOSAI::MessageFromAI* pAIMessage)
{
	m_MessagesFromAI.AddTail(pAIMessage); Notification_NewMessageFromAI();
}

CList< EOSAI::MessageToAI* >* CInterface::GetMessagesToAI()
{
	return &m_MessagesToAI;
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


#pragma once 

#include "EOSAIForeignRelationsState.h"
#include "EOSAIStrategicAIOrder.h"
//#include "EOSAIForeignRelationsStance.h"
class CEOSAIPlayerInteraction;
//class CGameEvent_IMail;
class CEOSAIMail;
class CEOSAIMailResponse;
class CEOSAITradeAgreement;
class CEOSAITradeAgreementResponse;
class CGPlayer;
namespace EOSAI
{
	class CAIPlayer2;
}
class CEOSAINationalSummary3;
//class CTechnologyDesc;
class CEOSAISerial;
class CAITradePrices;
class CAITradeEvaluationResult;
class CEOSAITechnologyDesc;
//class CWorldDescPlayerProxy;
//class CWorldDescServer;

//
// Input:
//   All Player's foreign relations, feelings, accessibility to other players,
//   Past StrategicAIOrders and responses to those orders (including rebuffed peace attempts, etc)
//   Interaction History between players (including attacks, destroyed units, etc)
//
// Output:
//   A series of StrategicAIOrders, which include high-level commands to:
//     DeclareWar,
//     SuggestMultilateralWar,
//     MakePeace,
//     etc.
//

class CEOSAIStrategicAI
{
	public:
		CEOSAIStrategicAI();
		void       SetAIPlayer( EOSAI::AIPlayer* pAIPlayer ){ m_pAIPlayer = pAIPlayer; }
		EOSAI::AIPlayer* GetAIPlayer(){ return m_pAIPlayer; }
		CGPlayer*   GetPlayer();
		long       GetPlayerNumber();
		long       GetCurrentTurn();
		//CWorldDescPlayerProxy* GetWorldDescPlayerProxy();
		//CWorldDescServer*      GetWorldDescServer();
		//CWorldDescPlayer* GetWorldDescPlayer();
		CEOSAIForeignRelationsState*  GetAIForeignRelationsState(){ return &m_CurrentForeignRelations; }

		// Serialize/Deserialize
		//void  Serialize( CEOSAISerial* pSerial );
		//void  Deserialize( CEOSAISerial* pSerial );

		CString  OutputDebugString();

		void  InvokePlayers( long iNumberOfPlayers );
		//void  SetupInputValues();
		void  CalculateForeignRelationFeelingsBasedOnPlayerInteractionHistory();
		//void  CreateAIThoughtDatabaseData();
		void  CreateForeignRelationsStances();
		void  MakeWarDeclarationsAndPeace();

		#ifdef THINGS_TO_COMPILE_EVENTUALLY
		I should set "HasSetSneakAttackAgainst()" so it actually works
		#endif THINGS_TO_COMPILE_EVENTUALLY
		bool  HasSetSneakAttackAgainst( int iTargetPlayer ){ return false; }

		long  GetWarStartTime( long iOtherPlayer );

		void  SetMostRecentMakePeaceMessage( long iOtherPlayer, long iTime );
		long  GetMostRecentMakePeaceMessage( long iOtherPlayer );
		void  SetMostRecentMaintainPeaceMessage( long iOtherPlayer, long iTime );
		long  GetMostRecentMaintainPeaceMessage( long iOtherPlayer );

		// Expansion or Conquest?
		float GetStrategy_CaptureUnownedCitRes01(){ return m_fStrategy_CaptureUnownedCitRes01; }
		float GetStrategy_CombatOrPlanForCombat01(){ return m_fStrategy_CombatOrPlanForCombat01; }

		//
		//void  CalculateTechnologyStrategy();

		//void  ProcessPlayerInteraction( CEOSAIPlayerInteraction* pPlayerInteraction );
		void  ProcessMail( CEOSAIMail* pIMail, bool* pbWasSignificant );
		void  ProcessMailResponse( CEOSAIMailResponse* pIMailResponse, bool* pbWasSignificant );
		void  ProcessTradeOffer( CEOSAITradeAgreement* pTrade, bool* pbWasSignificant );
		void  ProcessTradeAgreementResponse( CEOSAITradeAgreementResponse* pTradeAgreementResponse, bool* pbWasSignificant );

		//
		//float GetPercentageOfCapturedCitRes();

		// Inputs
		//CAINationalSummary2* GetNationalSummary2( long iPlayer ){ return m_AINationalSummaries[ iPlayer ]; }
		//void  AddPlayerInteraction( CEOSAIPlayerInteraction* pPlayerInteraction ){ m_PlayerInteractions.AddTail( pPlayerInteraction ); }

		// Outputs
		//CEOSAIStrategicAIOrder*  GetCurrentStrategicOrder(){ return m_pStrategicOrder; }
		//CList< CEOSAIStrategicAIOrder* >*  GetCurrentOrders(){ return &m_Orders; }

		// Trade
		void  CalculateAITradePrices( long iHumanPlayer, CAITradePrices* pPrices );
		void    GetAITradePrice( long iHumanPlayer, long iAIPlayer, float fFeelings01, float fTotalResource, float fDeltaResource, long* piWantsToBuyAndSellRes, float* pfPrice );
		void  IncomingEvent_RequestAITradeDesires( long iSendToPlayer );
		void  EvaluateTradeAgreement( CEOSAITradeAgreement* pTradeAgreement, CString* strOpinionText, long* pOpinionSum );
		void  EvaluateAndRespondToTradeAgreement( CEOSAITradeAgreement* pTradeAgreement );

		void  AddStrategicAIOrder( CEOSAIStrategicAIOrder* p ){ m_StrategicAIOrders.AddTail( p ); }
		CList< CEOSAIStrategicAIOrder* >*  GetStrategicAIOrders(){ return &m_StrategicAIOrders; } // Output: What should the AI do now?

		// Technology
		void  CalculateTechnologyStrategy();
		float GetTechValue( CEOSAITechnologyDesc* pTechDesc );
		void  SetMoneyBeingSpentOnResearch( long iMoney );//{ ASSERT( false ); } // The AI was calling WorldDescProxy directly
		void  ClearTechnologyResearchQueue();//{ ASSERT( false ); } // The AI was calling WorldDescProxy directly
		void  AddTechnologyAndPrerequisitesToResearchQueue( CString strTechnologyName );//{ ASSERT( false ); } // The AI was calling WorldDescProxy directly
		bool  GetTechnologyHasBeenDiscovered( CString strTech );//{ ASSERT( false ); return false; }
		bool  HasTechnologyHint( CString strTechnology ){ ASSERT( false ); return false; }
		float GetResearchPointsUntilCompletion( CString strTech );//{ ASSERT( false ); return false; }
		void  GetNearbyAppliedTechnologies( CList< CEOSAITechnologyDesc* >* p );//{ ASSERT( false ); }

	private:
		void CalculateTradeValue( CEOSAITradeAgreement* pTradeAgreement, CAITradeEvaluationResult* pResults );
				//float* pfTradeValue11, float* pfForeignRelationsBonus );

		EOSAI::AIPlayer* m_pAIPlayer;

		// ForeignRelations
		//
			CEOSAIForeignRelationsState  m_CurrentForeignRelations;

		// Buy/Sell Desires
		//
			// Resources
			//long  m_iWantsToBuyAndSellOil;    // +10 = wants to buy 10 oil,  -10 = wants to sell 10 oil
			//long  m_iWantsToBuyAndSellIron;   // +10 = wants to buy 10 iron, -10 = wants to sell 10 iron
			//long  m_iWantsToBuyAndSellFood;   // +10 = wants to buy 10 food, -10 = wants to sell 10 food
			// Technology
			//float m_fWantsToBuyTechnology01;  // 1.0 = wants to buy technology (higher when behind in tech)
			// Peace, Alliances
			//float m_fWantsPeace01;    // 0.0 = wants to continue war, 1.0 = overwhelmed, wants peace
			//float m_fWantsAlliance01; // Wants allies
			//float m_fWantsNonAggressionPact01;

		// Technology
		//
			float m_fMoneyToSpendOnResearch;

		// Output - Used by the Tactical AI
		//
			// Overall situation
			float  m_fStrategy_CaptureUnownedCitRes01; // 0.0-1.0  Based on the number/accessibility of unowned CitRes
			float  m_fStrategy_CombatOrPlanForCombat01;

	// What about making allies?
	//   (What's the logic for making allies?)
	// Want allies that don't change sides frequently
	// Allies who will have similar goals to me (i.e. common apparent enemy/neighbor)
	//   e.g. France's historical alliance with Russia against Germany
	// Don't want to box myself in with allies - I want some neighbor that I can attack
	// Basically, allies are the players I am least likely to declare war on
	//   (except I want to chose one or more neighbors to keep as potential enemies)

			// The StrategicAIOrders list contains a bunch of orders done by the AI. 
			//   It includes mail messages and trade agreements. The player will respond to these offers, and the 
			//   list allows the AI to lookup the sent-message.
			//   The list also lets the AI lookup how long ago certain messages (like CEOSAIStrategicAIOrder_AIOffersToResign) were sent.
			CList< CEOSAIStrategicAIOrder* >  m_StrategicAIOrders; // Output: What should the AI do now?
			//CEOSAIStrategicAIOrder*  m_pStrategicOrder;
			//CEOSAIBCDumbArray1D< CForeignRelationsStance* >  m_ForeignRelationsStance;

			// The last time I sent a message of a particular type to each player
			CEOSAIBCDumbArray1D< long >  m_LastMakePeaceMessage;
			CEOSAIBCDumbArray1D< long >  m_LastMaintainPeaceMessage;

			CStringList m_TechnologyToResearch;
};

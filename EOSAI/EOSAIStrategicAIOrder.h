
#pragma once

#include "EOSAIBCDumbArray2D.h"
#include "EOSAIEnumForeignRelations.h"

#include "EOSAIStrategicAIOrderResponseObserver.h"

class CGPlayer;
namespace EOSAI
{
	class AIPlayer;
}
class CWorldDescPlayer;
class CWorldDescServer;
class CEOSAIStrategicAI;
class CEOSAIForeignRelationsState;
class CEOSAIStrategicAIOrder;
class CEOSAIMailResponse;
class CEOSAITradeAgreementResponse;
class CEOSAIIntSet;
/*
// Created so that CStategicOrder can track responses to multiple IMails
class CEOSAIStrategicOrderResponseObserver
{
public:
	CEOSAIStrategicOrderResponseObserver()
	{
		m_iAIMessageUID = 0;
		m_iTimerStart = 0;
		m_iTimerDuration = 0; // e.g. 60 seconds
	}
	virtual void                SetResponse(long iPlayer, EOSAI::EnumAIMailResponse eResponse);
	virtual EOSAI::EnumAIMailResponse  GetResponse(long iPlayer);

	bool    AllResponsesReceived();
	bool    AllResponsesReceivedAndHaveApproved();

	long   m_iAIMessageUID;
	long   m_iTimerStart;
	long   m_iTimerDuration; // e.g. 60 seconds
	CList< CEOSAIMailResponse* >  m_Responses;
	CEOSAIIntSet  m_PlayersIWantResponsesFrom;
};
*/
class CEOSAIStrategicAIOrder
{
	public:
		enum EnumState
		{
			enumState_New,
			enumState_Pending,
			enumState_Completed
		};

	public:
		CEOSAIStrategicAIOrder( CEOSAIStrategicAI* pStrategicAI )
		{
			m_pStrategicAI = pStrategicAI;
			m_iExecutionTurn = 0;
			m_eCurrentState = enumState_New;
		}

		EOSAI::AIPlayer* GetAIPlayer();
		long  GetPlayerNumber();
		//CPlayer* GetPlayer();
		//CWorldDescPlayer* GetWorldDescPlayer();
		//CWorldDescServer* GetWorldDescServer();

		EnumState     GetCurrentState(){ return m_eCurrentState; }

		virtual float GetOddsOfSuccess(){ return 1.0f; }
		virtual void  Execute( long iCurrentTurn ){}

		// IMail Response
		virtual bool  ContainsAIMailId( long iAIMailId ){ return false; }
		virtual void  IncomingResponse( CEOSAIMailResponse* pIMailResponse ){}
		// TradeAgreement Response
		virtual bool  ContainsTradeAgreement( CString strTradeAgreement ){ return false; }
		virtual void  SetResponse( CEOSAITradeAgreementResponse* pResponse ){}
		//
		//virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState ){}
		virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings ){}

		// Utility function
		//CString  GetTargetPlayersString( CEOSAIIntSet* pPlayers );
		bool     AllPlayersExist( CEOSAIIntSet* pPlayers );

	protected:
		CEOSAIStrategicAI*  m_pStrategicAI;
		long                m_iExecutionTurn;

		EnumState           m_eCurrentState;

		// Optional
		/*
		long   m_iAIMessageUID;
		long   m_iTimerStart;
		long   m_iTimerDuration; // e.g. 60 seconds
		CList< CPlayerResponse* >  m_Responses;
		CEOSAIIntSet  m_PlayersIWantResponsesFrom;
		*/
};

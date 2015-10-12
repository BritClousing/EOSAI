
#pragma once

#include "EOSAIEnumTradeAgreementResponse.h"
#include "EOSAIIntSet.h"
//#include "EOSAISerial.h"
#include "EOSAIBCDumbArray2D.h"
#include "EOSAIEnumForeignRelations.h"
#include "EOSAISettings.h"
#include "EOSAIPublic.h"
class CGlobalForeignRelations;
class CEOSAIForeignRelationsSituation;
class CHypotheticalForeignPowerBalance;
class CEOSAIForeignRelationsState;
class CEOSAIStrategicAIOrder;
class CEOSAISerial;
class CWorldDescBase;

// CEOSAIPlayerInteractions track some thing that happened in the past:
//   e.g. Another player double-crossed me,
//        Trade (still in effect or ended),
//        Non-aggression pacts (still in effect or ended), etc
//        Asked to sign a peace agreement
//        Asked to join in an attack

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#define NORMAL_DECAY_TIME 8.0f
#define LONG_DECAY_TIME 16.0f

class DLLIMPEXP CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction(){}

		//
		CEOSAISerial_INFORMATION_LONG( 204, CEOSAIPlayerInteraction );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );

		// Make sure all the necessary values are set
		virtual bool ValidateValues(){ ASSERT(false); return true; }

		bool KnownByAllPlayers() { return m_bEveryoneKnowsAboutThisInteraction; }
		void KnownByAllPlayers(bool b) { m_bEveryoneKnowsAboutThisInteraction = b; } // send to all AIs?

		CEOSAIIntSet* KnownByPlayer() { return &m_PlayersWhoKnowAboutThisInteraction; }
		void          KnownByPlayer(int iPlayer) { m_PlayersWhoKnowAboutThisInteraction.Add(iPlayer); }

		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState ){}
		virtual void UpdateForeignRelationsState( 
			long iCurrentTurn,
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations,
			CEOSAIBCDumbArray2D< float >* pFeelings ){}
			//CGlobalForeignRelations* pForeignRelations ){}
			//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
			//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] ){}

		// Delta 1.0, DeltaTurns  0.0, HalfLife 5.0 = 1.00 * Weight
		// Delta 1.0, DeltaTurns  1.0, HalfLife 5.0 = 0.90 * Weight
		// Delta 1.0, DeltaTurns  2.0, HalfLife 5.0 = 0.80 * Weight
		// Delta 1.0, DeltaTurns  3.0, HalfLife 5.0 = 0.70 * Weight
		// Delta 1.0, DeltaTurns  4.0, HalfLife 5.0 = 0.60 * Weight
		// Delta 1.0, DeltaTurns  5.0, HalfLife 5.0 = 0.50 * Weight
		// Delta 1.0, DeltaTurns 10.0, HalfLife 5.0 = 0.33 * Weight
		// Delta 1.0, DeltaTurns 15.0, HalfLife 5.0 = 0.25 * Weight
		// Delta 1.0, DeltaTurns 20.0, HalfLife 5.0 = 0.20 * Weight
		// Delta 1.0, DeltaTurns 25.0, HalfLife 5.0 = 0.17 * Weight
		// Delta 1.0, DeltaTurns 30.0, HalfLife 5.0 = 0.14 * Weight
		// Delta 1.0, DeltaTurns 35.0, HalfLife 5.0 = 0.125* Weight
		//float AdjustDeltaAccordingToTime( long iTurnsSinceEvent, float fDelta, float fHalfLife );

		// Delta 1.0, DeltaTurns  0.0, HalfLife 5.0 = 1.00
		// Delta 1.0, DeltaTurns  5.0, HalfLife 5.0 = 1.00
		// Delta 1.0, DeltaTurns  6.0, HalfLife 5.0 = 0.90
		// Delta 1.0, DeltaTurns  7.0, HalfLife 5.0 = 0.80
		// Delta 1.0, DeltaTurns  8.0, HalfLife 5.0 = 0.70
		// Delta 1.0, DeltaTurns  9.0, HalfLife 5.0 = 0.60
		// Delta 1.0, DeltaTurns 10.0, HalfLife 5.0 = 0.50
		// Delta 1.0, DeltaTurns 15.0, HalfLife 5.0 = 0.33
		// Delta 1.0, DeltaTurns 20.0, HalfLife 5.0 = 0.25
		// Delta 1.0, DeltaTurns 25.0, HalfLife 5.0 = 0.20
		// Delta 1.0, DeltaTurns 30.0, HalfLife 5.0 = 0.17
		// Delta 1.0, DeltaTurns 35.0, HalfLife 5.0 = 0.14
		// Delta 1.0, DeltaTurns 40.0, HalfLife 5.0 = 0.125
		float AdjustDeltaAccordingToTime( long iTurnsSinceEvent, float fDelta, float fDecayTime );

		long m_iEventTurn = -1;

		long m_iInteractionId = 0; // This is set by EOSAI

		// Who should know about this Interaction?
		bool m_bEveryoneKnowsAboutThisInteraction = false;
		CEOSAIIntSet m_PlayersWhoKnowAboutThisInteraction;
};

// The interaction is encoded in a StrategicAIOrder
/*
class CEOSAIPlayerInteraction_StrategicAIOrder : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_StrategicAIOrder(){ m_pStrategicAIOrder = NULL; }
		//
		virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		CEOSAIStrategicAIOrder*  m_pStrategicAIOrder;
};
*/

class DLLIMPEXP CEOSAIPlayerInteraction_CreatedAlliance : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_CreatedAlliance(){ m_iPlayer1 = 0; m_iPlayer2 = 0; }
		CEOSAIPlayerInteraction_CreatedAlliance( long iPlayer1, long iPlayer2 ){ m_iPlayer1 = iPlayer1; m_iPlayer2 = iPlayer2; }

		//
		CEOSAISerial_INFORMATION_LONG( 205, CEOSAIPlayerInteraction_CreatedAlliance );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );

		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( 
			long iCurrentTurn, 
			CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, 
			CEOSAIBCDumbArray2D< float >* pFeelings );
			//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
			//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
			//CGlobalForeignRelations* pForeignRelations );

		long  m_iPlayer1;
		long  m_iPlayer2;
};


class CEOSAIPlayerInteraction_AskedToJoinInWar : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_AskedToJoinInWar()
		{
			m_iRequestingPlayer = 0;
			m_iSentTo = 0;
			m_eResponse = EOSAIEnumTradeAgreementResponse_NoResponse;
		}

		//
		CEOSAISerial_INFORMATION_LONG( 206, CEOSAIPlayerInteraction_AskedToJoinInWar );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );

		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, 
						CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, 
						CEOSAIBCDumbArray2D< float >* pFeelings );
						//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
						//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
						//CGlobalForeignRelations* pForeignRelations );

		long  m_iRequestingPlayer;
		long  m_iSentTo;
		EOSAIEnumTradeAgreementResponse   m_eResponse;
};
/*
class CEOSAIPlayerInteraction_SneakAttack : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_SneakAttack()
		{
			m_iActor = 0;
			m_iTarget = 0;
			//m_iAttackingPlayer = 0;
			//m_iDefendingPlayer = 0;
		}

		//
		CEOSAISerial_INFORMATION_LONG( 207, CEOSAIPlayerInteraction_SneakAttack );
		virtual void Serialize( CEOSAISerial* pSerial );
		virtual void Deserialize( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );

		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, 
						CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, 
						CEOSAIBCDumbArray2D< float >* pFeelings );
						//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
						//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
						//CGlobalForeignRelations* pForeignRelations );

		long  m_iActor;
		long  m_iTarget;
		//long  m_iAttackingPlayer;
		//long  m_iDefendingPlayer;
};
*/

class CEOSAIPlayerInteraction_AskedToMakePeace : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_AskedToMakePeace()
		{
			m_iRequestingPlayer = 0;
			m_iSentTo = 0;
			m_eResponse = EOSAIEnumTradeAgreementResponse_NoResponse;
		}
		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, 
						CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, 
						CEOSAIBCDumbArray2D< float >* pFeelings );
						//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
						//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
						//CGlobalForeignRelations* pForeignRelations );

		long  m_iRequestingPlayer;
		long  m_iSentTo;
		EOSAIEnumTradeAgreementResponse   m_eResponse;
};

class CEOSAIPlayerInteraction_MadePeace : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_MadePeace(){ m_iPlayer1 = 0; m_iPlayer2 = 0; }
		CEOSAIPlayerInteraction_MadePeace( long iPlayer1, long iPlayer2 ){ m_iPlayer1 = iPlayer1; m_iPlayer2 = iPlayer2; }
		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, 
						CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, 
						CEOSAIBCDumbArray2D< float >* pFeelings );
						//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
						//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
						//CGlobalForeignRelations* pForeignRelations );
		long  m_iPlayer1;
		long  m_iPlayer2;
};

class CEOSAIPlayerInteraction_AskedForNonAgressionPact : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_AskedForNonAgressionPact()
		{
			m_iRequestingPlayer = 0;
			m_iSentTo = 0;
			m_eResponse = EOSAIEnumTradeAgreementResponse_NoResponse;
			m_bHasADuration = false;
			m_iDuration = 0;
		}
		//virtual void UpdateForeignRelationsState( long iCurrentTurn, CEOSAIForeignRelationsState* pState );
		virtual void UpdateForeignRelationsState( long iCurrentTurn, 
						CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, 
						CEOSAIBCDumbArray2D< float >* pFeelings );
						//EOSAIEnumForeignRelations ForeignRelations[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1],
						//float Feelings[MAX_NUMBER_OF_PLAYERS+1][MAX_NUMBER_OF_PLAYERS+1] );
						//CGlobalForeignRelations* pForeignRelations );

		//
		long  m_iRequestingPlayer;
		long  m_iSentTo;
		EOSAIEnumTradeAgreementResponse   m_eResponse;

		// If accepted...
		bool  m_bHasADuration;
		long  m_iDuration;
};

// Used for tracking player flyovers - important for anticipating an attack
//   Note: see CAIHistoricalSightings for details
//   I probably want to store the MultiRegion(s) or AIRegions where the Unit was seen (not the location,
//     which is what I'm doing inside CAIHistoricalSightings)
class CEOSAIPlayerInteraction_PlayerSighting : public CEOSAIPlayerInteraction
{
	public:
		CEOSAIPlayerInteraction_PlayerSighting(){}
};


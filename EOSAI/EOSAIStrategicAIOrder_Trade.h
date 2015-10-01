
#pragma once

#include "EOSAIStrategicAIOrder.h"
#include "EOSAIEnumTradeAgreementResponse.h"
#include "EOSAIStringAndFloatSet.h"
#include "PoiObjectIdAndName.h"
class CEOSAITradeAgreementResponse;

class CEOSAIStrategicAIOrder_Trade : public CEOSAIStrategicAIOrder
{
	public:
		CEOSAIStrategicAIOrder_Trade( CEOSAIStrategicAI* pStrategicAI );
		//void  SetTradePartner( long iTradePartner ){ m_iTradePartner = iTradePartner; }

		virtual void  Execute( long iCurrentTurn ); // Send the offer
		//
		virtual bool  ContainsTradeAgreement( CString strTradeAgreement ){ return m_strTradeAgreement == strTradeAgreement; }
		virtual void  IncomingResponse( CEOSAITradeAgreementResponse* pResponse );
		//
		virtual void  UpdateForeignRelationsState( long iCurrentTurn, CEOSAIBCDumbArray2D< EOSAIEnumForeignRelations >* pForeignRelations, CEOSAIBCDumbArray2D< float >* pFeelings );

		long    m_iToPlayer;
		std::map<CString,float> m_ResourcesToSell;
		std::map<CString,float> m_ResourcesToBuy;

		CString m_strTradeAgreement;

		float   m_fTradeBalance; // Is this the AI's opinion of the trade offer?

		// Trade PoiObject
		CList< EOSAI::PoiObjectIdAndName* >  m_PlayerUnitObjectId[2];
		CList< EOSAI::PoiObjectIdAndName* >  m_PlayerCitiesObjectId[2];
		CList< EOSAI::PoiObjectIdAndName* >  m_PlayerResourcesObjectId[2];

		// Map Knowledge
		bool m_bPlayerMapPeek[2];
		bool m_bPlayerMapViewOngoing[2];

		// Technology
		CList< CString > m_PlayerTechnologyList[2];

		// Foreign Relations - Happens to both players
		bool m_bPeaceTreaty;
		bool m_bNonAggressionPact;
		long m_iNonAggressionPactEndTurn;
		bool m_bAlliance;
		//bool m_bPermanentAlliance;
		//long m_iAgreementToAttackPlayer; // 0 (if invalid) or a player number
		//CLongSet  m_AgreementToAttackPlayer[2]; // A set of attack targets for each player

		EOSAIEnumTradeAgreementResponse  m_eResponse; // My response or other player's response
};


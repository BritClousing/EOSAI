
#pragma once

class CEOSAITradeAgreement;

// This is mail sent to or from the AI.

class CEOSAIMail
{
	public:
		enum EnumAIMessageType
		{
			enumType_Undefined,
			enumType_Trade,
			enumType_SuggestTeam,
			enumType_PeaceAgreement,
			enumType_MultilateralWar,
			enumType_MultilateralWarConfirmationMessage,
			enumType_Alliance,
			//
			enumType_OfferSurrender,  // the AI wants to surrender
			enumType_DemandSurrender, // the AI demands that another player surrenders
		};

		//
		CEOSAIMail()
		{
			m_iFromPlayer = 0;
			m_iToPlayer = 0;
		}
		~CEOSAIMail(){ m_eAIMessageType = enumType_Undefined; }
		EnumAIMessageType  GetAIMessageType(){ return m_eAIMessageType; }
		CEOSAITradeAgreement* GetTradeAgreement(){ ASSERT( false ); return NULL; }

		long m_iFromPlayer;
		long m_iToPlayer;
		EnumAIMessageType  m_eAIMessageType;
};


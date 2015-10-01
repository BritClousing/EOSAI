
#pragma once

enum EOSAIEnumTradeAgreementState
{
	EOSAIEnumTradeAgreementState_Undefined,
	EOSAIEnumTradeAgreementState_BrandNew,
	EOSAIEnumTradeAgreementState_WaitingForApproval,
	EOSAIEnumTradeAgreementState_RejectedOffer,
	EOSAIEnumTradeAgreementState_ActiveOngoingAgreement,
	//EnumTradeAgreementState_ExpiredAgreement,   // Sumbitter created another Trade Agreement
	EOSAIEnumTradeAgreementState_ExpiredOffer,    // Sumbitter created another Trade Agreement
	EOSAIEnumTradeAgreementState_CancelledOffer,  // Cancelled by the Submitter before Approval
	EOSAIEnumTradeAgreementState_EndedOngoingTrade,  // Ended an ongoing trade
	EOSAIEnumTradeAgreementState_CompletedAgreement,
	EOSAIEnumTradeAgreementState_TradeFailed,     // One or both parties has insufficient money/resources/etc

	//EnumTradeAgreementState_IncomingOffer,
	//EnumTradeAgreementState_OutgoingOffer,
	//EnumTradeAgreementState_ActiveAgreement,
	//EnumTradeAgreementState_ExpiredAgreement,  // either a one-time trade or terminated ongoing agreement
	//EnumTradeAgreementState_CancelledAgreement // an offer that was cancelled
};



#pragma once
/*
enum EOSAIEnumTradeAgreementResponse
{
	EOSAIEnumTradeAgreementResponse_Undefined = 0,
	EOSAIEnumTradeAgreementResponse_Approve = 1,
	EOSAIEnumTradeAgreementResponse_Decline = 2,
	EOSAIEnumTradeAgreementResponse_Reject = 3,
};
*/

enum EOSAIEnumTradeAgreementResponse
{
	EOSAIEnumTradeAgreementResponse_NoResponse,
	EOSAIEnumTradeAgreementResponse_Accept,
	EOSAIEnumTradeAgreementResponse_Decline,
	EOSAIEnumTradeAgreementResponse_Reject,    // = Angry decline
	EOSAIEnumTradeAgreementResponse_CancelOffer,    // Sent a trade offer but cancelled it before it started
	EOSAIEnumTradeAgreementResponse_EndOngoingTrade // Cancelled an ongoing trade offer after it started
	/*
	enum_NoResponse,
	enum_Accept,
	enum_Decline,
	enum_Reject,    // = Angry decline
	enum_CancelOffer,    // Sent a trade offer but cancelled it before it started
	enum_EndOngoingTrade // Cancelled an ongoing trade offer after it started
	*/
};

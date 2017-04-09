
#include "stdafx.h"

#include "EOSAISerial.h"
#ifdef EOS

#include "City.h"
#include "Airfield.h"
#include "Poi.h"
#include "PoiPD.h"
#include "PoiObject.h"
#include "ResourceSource.h"
#include "Unit.h"
#include "UnitGroup.h"
#include "Airfield.h"
#include "Fortification.h"

#include "GameEventPoiObjectAnimation.h"
#include "GameEventLocation.h"
#include "GameEventLocationAndMovement.h"
#include "GameEventBeginCombat.h"
#include "GameEventCombat.h"
#include "GameEventCombatOccurred.h"
#include "GameEventEndCombat.h"
#include "GameEventInitialState.h"
#include "GameEventVisible.h"
#include "GameEventPoiObjectState.h"
#include "GameEventContainment.h"
#include "GameEventDeath.h"
#include "GameEventCapture.h"
#include "GameEventFoodAvailability.h"
#include "GameEventOilAvailability.h"
#include "GameEventIronAvailability.h"
#include "GameEventStarvation.h"
#include "GameEventStarvation2.h"
#include "GameEventIncProduction.h"
#include "GameEventBuildComplete.h"
#include "GameEventEntrenched.h"
#include "GameEventRepair.h"
#include "GameEventRepair2.h"

#include "GameEventUpgrade.h"
#include "GameEventLaunch.h"
#include "GameEventPopulationChange.h"
#include "GameEventUserUnitPlacement.h"
#include "GameEventBeginOrEndBuildingAirfield.h"
#include "NationalGameEventPlayerEliminated.h"
#include "NationalGameEventPlayerWon.h"
#include "NationalGameEventFinalResult.h"
#include "NationalGameEventAirAttack.h"
#include "ReinforcementsPoint.h"

#include "Mountain.h"
#include "NationalGameEventTeamFormed.h"
//#include "LanguagePack.h"
//extern CString Lang(long iItemID, CString strEnglish );
/*
#include "Checkpoint.h"
#include "Animation.h"
#include "GunAnimation.h"
#include "LaserAnimation.h"
#include "FlashAnimation.h"
#include "MissileAnimation.h"
#include "WorldDesc.h"
//#include "ForeignProposal.h"
#include "InternalMail.h"
#include "TradeAgreement.h"
#include "GameEventLocation3D.h"
#include "Message2_DeclareWarMessage.h"
#include "Message2_ForeignRelations.h"
#include "Message2_GenericXML.h"
#include "GameEvent_PlayerDeclaration.h"
#include "GameEvent_IMail.h"
#include "GameEvent_TradeAgreement.h"
#include "GameEvent_ForeignRelations.h"
#include "Message2_ForeignRelationsFeelings.h"
#include "Message2_NationalGameEvent.h"
#include "Message2_CreatingGameSetup.h"
#include "Message2_LoadingMultiplayerGame.h"
#include "Message2_EndedGameSetup.h"
#include "Message2_RakNetPeerDiscovered.h"
#include "Message2_EnterGameSetupChat.h"
#include "Message2_ExitGameSetupChat.h"
#include "Message2_RequestGameSetupUsers.h"
#include "Message2_GameServerUsers.h"
#include "Message2_TransferServerNotification.h"
#include "Message2_TransferServer.h"
#include "Message2_TransferServerCompleted.h"
#include "Message2_TradeAgreementResponse.h"
#include "Message2_RequestTradeAgreementOpinion.h"
#include "Message2_TradeAgreementOpinion.h"
#include "Message2_PauseGameTimer.h"
#include "Message2_OpenRakNetConnection.h"
#include "Message2_AIPlayerReadyToSubmitTurn.h"
#include "Message2_AIPlayerNotReadyToSubmitTurn.h"
#include "Message2_RequestUnitsetP2P.h"
#include "Message2_SendUnitsetP2P.h"
#include "Message2_UserDisconnectedNotification.h"
#include "Message2_UserConnectionLostNotification.h"
#include "Message2_ReJoiningGame.h"
#include "Message2_PlayerToMessageTargetMatches.h"

#include "Message2_RequestTradeDesires.h"
#include "Message2_TradeDesires.h"

#include "Message2_CreateDirectConnection.h"
#include "Message2_CreateDirectConnectionFailed.h"
#include "NationalGameEvent_SneakAttack.h"
#include "NationalGameEvent_NonAggressionPact.h"

#include "NationalGameEvent_PeaceAgreement.h"
#include "NationalGameEvent_AllianceFormed.h"

//#include "Message2_ServerToPlayerUpdate.h"

#include "EOSAIPlayerInteraction.h"
#include "EOSAIPlayerInteraction_Combat.h"
#include "EOSAIPlayerInteraction_DeclaredWar.h"
#include "EOSAIPlayerInteraction_BeginWar.h"
#include "EOSAIPlayerInteraction_CreatedTeam.h"
#include "EOSAIPlayerInteraction_Trade.h"
#include "EOSAIPlayerInteraction_BorderViolation.h"
*/
#endif EOS

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//class 

//#define ADD_CEOSAISerial_OBJ( classid, classname ) \
//	AddBCEOSAISerialObj( classid, classname::GetCEOSAISerialClassId, classname::CEOSAISerialInstanciate );
//#define ADD_CEOSAISerial_OBJ_STRING( classid, classname ) \
//	AddBCEOSAISerialObj( classid, &classname::m_strCEOSAISerialClassId, classname::CEOSAISerialInstanciate );
//#define ADD_CEOSAISerial_OBJ_LONG( classid, classname ) \
//	AddBCEOSAISerialObj( classid, classname::GetCEOSAISerialClassId, classname::CEOSAISerialInstanciate );
//#define ADD_BCEOSAISerial_OBJ_CONTEXT( classid, classname ) 
//	AddBCEOSAISerialObj( classid, &classname::m_iCEOSAISerialClassId, classname::CEOSAISerialInstanciate, classname::SetContext );


std::map< CStringANSI8, CBCEOSAISerialRec* > CEOSAISerial::s_ClassMap;
std::vector< CBCEOSAISerialRec* >       CEOSAISerial::s_ClassVector;
/*
void CEOSAISerial::Initialize()
{
	#ifdef EOS

	//ADD_CEOSAISerial_OBJ(  1, CMessage2_ForeignProposal );
	//ADD_CEOSAISerial_OBJ(  2, CForeignProposal_NonAggressionPact );
	ADD_CEOSAISerial_OBJ(  3, CGameEventLocation );
	ADD_CEOSAISerial_OBJ(  4, CMessage_TextMessage );
	ADD_CEOSAISerial_OBJ(  5, CGunAnimation );
	ADD_CEOSAISerial_OBJ(  6, CAnimation );
	ADD_CEOSAISerial_OBJ(  7, CMissileAnimation );
	//ADD_CEOSAISerial_OBJ(  8, CForeignProposal_FreeForm );
	//ADD_CEOSAISerial_OBJ(  9, CForeignProposal_GangupOnPlayer );
	ADD_CEOSAISerial_OBJ( 10, CMessage2_InternalMail );
	ADD_CEOSAISerial_OBJ( 11, CMessage2_PlayerSubmittedTurn );
	ADD_CEOSAISerial_OBJ( 12, CMessage2_PlayerUnSubmittedTurn );
	ADD_CEOSAISerial_OBJ( 13, CGameEventLocation3D );
	ADD_CEOSAISerial_OBJ( 14, CFlashAnimation );
	ADD_CEOSAISerial_OBJ( 15, CGameEventPoiObjectAnimation );
	ADD_CEOSAISerial_OBJ( 16, CMessage2_DeclareWarMessage );
	ADD_CEOSAISerial_OBJ( 17, CLaserAnimation );
	//ADD_CEOSAISerial_OBJ( 18, CMessage2_ForeignRelations );
	ADD_CEOSAISerial_OBJ( 19, CMessage2_GenericXML );

	ADD_CEOSAISerial_OBJ( 20, CGameEvent_PlayerDeclaration );
	ADD_CEOSAISerial_OBJ( 21, CGameEvent_IMail );
	ADD_CEOSAISerial_OBJ( 22, CGameEvent_ForeignRelations );
	ADD_CEOSAISerial_OBJ( 23, CMessage2_ForeignRelationsFeelings );

	ADD_CEOSAISerial_OBJ( 30, CMessage2_NationalGameEvent );
	ADD_CEOSAISerial_OBJ( 31, CMessage2_CreatingGameSetup );
	ADD_CEOSAISerial_OBJ( 32, CMessage2_EndedGameSetup );
	ADD_CEOSAISerial_OBJ( 33, CMessage2_ConnectedToGameServer );//CMessage2_EnterGameSetupChat );
	ADD_CEOSAISerial_OBJ( 34, CMessage2_DisconnectedFromGameServer );//CMessage2_ExitGameSetupChat );
	ADD_CEOSAISerial_OBJ( 35, CMessage2_LoadingMultiplayerGame );
	ADD_CEOSAISerial_OBJ( 36, CMessage2_RequestGameSetupUsers );
	ADD_CEOSAISerial_OBJ( 37, CMessage2_GameServerUsers );
	ADD_CEOSAISerial_OBJ( 38, CMessage2_TransferServerNotification );
	ADD_CEOSAISerial_OBJ( 39, CMessage2_TransferServer );
	ADD_CEOSAISerial_OBJ( 40, CMessage2_TransferServerCompleted );
	ADD_CEOSAISerial_OBJ( 41, CMessage2_PauseGameTimer );
	ADD_CEOSAISerial_OBJ( 42, CMessage2_TradeAgreementResponse );
	//ADD_CEOSAISerial_OBJ( 43, CMessage2_RequestTradeAgreementMultipliers );
	//ADD_CEOSAISerial_OBJ( 44, CMessage2_TradeAgreementMultipliers );
	ADD_CEOSAISerial_OBJ( 45, CMessage2_RequestTradeAgreementOpinion );
	ADD_CEOSAISerial_OBJ( 46, CMessage2_TradeAgreementOpinion );
	ADD_CEOSAISerial_OBJ( 47, CMessage2_OpenRakNetConnection );
	ADD_CEOSAISerial_OBJ( 48, CMessage2_AIPlayerReadyToSubmitTurn );
	ADD_CEOSAISerial_OBJ( 49, CMessage2_AIPlayerNotReadyToSubmitTurn );
	ADD_CEOSAISerial_OBJ( 50, CMessage2_RequestUnitsetP2P );
	ADD_CEOSAISerial_OBJ( 51, CMessage2_SendUnitsetP2P );
	//ADD_CEOSAISerial_OBJ( 52, CMessage2_NewRemoteMessageTarget );
	ADD_CEOSAISerial_OBJ( 52, CMessage2_UserDisconnectedNotification );
	ADD_CEOSAISerial_OBJ( 53, CMessage2_UserConnectionLostNotification );
	ADD_CEOSAISerial_OBJ( 54, CMessage2_ReJoiningGame );
	ADD_CEOSAISerial_OBJ( 55, CMessage2_PlayerToMessageTargetMatches );
	ADD_CEOSAISerial_OBJ( 56, CMessage2_CreateDirectConnection );
	ADD_CEOSAISerial_OBJ( 57, CMessage2_CreateDirectConnectionFailed );
	ADD_CEOSAISerial_OBJ( 58, CGameEvent_TradeAgreement );
	//
	ADD_CEOSAISerial_OBJ( 59, CMessage2_RequestTradeDesires );
	ADD_CEOSAISerial_OBJ( 60, CMessage2_TradeDesires );
	
	//ADD_CEOSAISerial_OBJ( 58, CMessage2_ServerToPlayerUpdate );

	// GameEvent objects
	ADD_CEOSAISerial_OBJ( 100, CGameEventLocationAndMovement );

	// Player Interaction objects
	ADD_CEOSAISerial_OBJ( 200, CEOSAIPlayerInteraction_DeclaredWar );
	ADD_CEOSAISerial_OBJ( 201, CEOSAIPlayerInteraction_BeginWar );
	ADD_CEOSAISerial_OBJ( 202, CEOSAIPlayerInteraction_CreatedTeam );
	ADD_CEOSAISerial_OBJ( 203, CEOSAIPlayerInteraction_Trade );
	ADD_CEOSAISerial_OBJ( 204, CEOSAIPlayerInteraction );
	ADD_CEOSAISerial_OBJ( 205, CEOSAIPlayerInteraction_CreatedAlliance );
	ADD_CEOSAISerial_OBJ( 206, CEOSAIPlayerInteraction_AskedToJoinInWar );
	ADD_CEOSAISerial_OBJ( 207, CEOSAIPlayerInteraction_SneakAttack );
	ADD_CEOSAISerial_OBJ( 208, CEOSAIPlayerInteraction_Combat );
	ADD_CEOSAISerial_OBJ( 209, CEOSAIPlayerInteraction_BorderViolation );

	// Player Interaction objects
	ADD_CEOSAISerial_OBJ( 300, CNationalGameEvent_PeaceAgreement );
	ADD_CEOSAISerial_OBJ( 301, CNationalGameEvent_AllianceFormed );
	ADD_CEOSAISerial_OBJ( 302, CNationalGameEvent_SneakAttack );
	ADD_CEOSAISerial_OBJ( 303, CNationalGameEvent_NonAggressionPact );

	#endif EOS
}
*/
/*
void CEOSAISerial::AddBCEOSAISerialObj( unsigned long id,
							  long* pClassId,
							  void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) )
{
	// Don't allow duplicate ids
	ASSERT( GetBCEOSAISerialRec( id ) == NULL );

	CBCEOSAISerialRec* pRec = new CBCEOSAISerialRec();
	pRec->m_iId = id;
	pRec->m_pInstanciateFunction = pInstanciateFunction;
	//pRec->m_pSetContext = pSetContext;
	//s_ClassMap.insert( std::pair<long, CBCEOSAISerialRec*>( id, pRec ) );
	if( s_ClassVector.size() <= id )
	{
		s_ClassVector.resize( id+10 );
	}
	s_ClassVector[id] = pRec;

	(*pClassId) = id;
}
*/

void CEOSAISerial::AddBCEOSAISerialObj( CStringANSI8 strClassId,
							  CStringANSI8* pClassId,
							  void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) )
{
	// Don't allow duplicate ids
	ASSERT( GetBCEOSAISerialRec( strClassId ) == NULL );

	CBCEOSAISerialRec* pRec = new CBCEOSAISerialRec();
	pRec->m_strClassId = strClassId;
	pRec->m_pInstanciateFunction = pInstanciateFunction;
	//pRec->m_pSetContext = pSetContext;

	s_ClassMap.insert( std::pair<CStringANSI8, CBCEOSAISerialRec*>( strClassId, pRec ) );
	/*
	if( s_ClassVector.size() <= id )
	{
		s_ClassVector.resize( id+10 );
	}
	s_ClassVector[id] = pRec;
	*/

	//(*pClassId) = id;
	(*pClassId) = strClassId;
}


void CEOSAISerial::AddBCEOSAISerialObj( CStringANSI8 strClassId,
							  CStringANSI8 (*pGetCEOSAISerialClassId)(),
							  void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) )
{
	// Don't allow duplicate ids
	ASSERT( GetBCEOSAISerialRec( strClassId ) == NULL );

	CStringANSI8 strClassIdAccordingToClass = (*pGetCEOSAISerialClassId)();
	ASSERT( strClassId == strClassIdAccordingToClass );

	CBCEOSAISerialRec* pRec = new CBCEOSAISerialRec();
	pRec->m_strClassId = strClassId;
	pRec->m_pInstanciateFunction = pInstanciateFunction;
	//pRec->m_pSetContext = pSetContext;

	s_ClassMap.insert( std::pair<CStringANSI8, CBCEOSAISerialRec*>( strClassId, pRec ) );
	/*
	if( s_ClassVector.size() <= id )
	{
		s_ClassVector.resize( id+10 );
	}
	s_ClassVector[id] = pRec;
	*/

	//(*pClassId) = id;
	//(*pClassId) = strClassId;
}

void CEOSAISerial::AddBCEOSAISerialObj( long  iClassId,
							  long* pClassId,
							  void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) )
{
	// Don't allow duplicate ids
	ASSERT( GetBCEOSAISerialRec( iClassId ) == NULL );

	CBCEOSAISerialRec* pRec = new CBCEOSAISerialRec();
	pRec->m_iClassId = iClassId;
	pRec->m_pInstanciateFunction = pInstanciateFunction;
	//pRec->m_pSetContext = pSetContext;

	//s_ClassMap.insert( std::pair<CString, CBCEOSAISerialRec*>( strClassId, pRec ) );
	if( s_ClassVector.size() <= (unsigned long) iClassId )
	{
		s_ClassVector.resize( iClassId+10 );
	}
	s_ClassVector[iClassId] = pRec;

	(*pClassId) = iClassId;
	//(*pClassId) = strClassId;
}


void CEOSAISerial::AddBCEOSAISerialObj( long  iClassId,
							  long  (*pGetCEOSAISerialClassId)(),
							  void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) )
{
	// Don't allow duplicate ids
	ASSERT( GetBCEOSAISerialRec( iClassId ) == NULL );

	long iClassIdAccordingToClass = (*pGetCEOSAISerialClassId)();
	ASSERT( iClassId == iClassIdAccordingToClass );

	CBCEOSAISerialRec* pRec = new CBCEOSAISerialRec();
	pRec->m_iClassId = iClassId;
	pRec->m_pInstanciateFunction = pInstanciateFunction;
	//pRec->m_pSetContext = pSetContext;

	//s_ClassMap.insert( std::pair<CString, CBCEOSAISerialRec*>( strClassId, pRec ) );
	if( s_ClassVector.size() <= (unsigned long) iClassId )
	{
		s_ClassVector.resize( iClassId+10 );
	}
	s_ClassVector[iClassId] = pRec;

	//(*pClassId) = iClassId;
	//(*pClassId) = strClassId;
}

//CBCEOSAISerialRec* CEOSAISerial::GetBCEOSAISerialRec( unsigned long id )
CBCEOSAISerialRec* CEOSAISerial::GetBCEOSAISerialRec( CStringANSI8 strClassId )
{
	/*
	if( id < s_ClassVector.size() )
	{
		return s_ClassVector[id];
	}
	return NULL;
	*/
	std::map< CStringANSI8, CBCEOSAISerialRec* >::iterator iter = CEOSAISerial::s_ClassMap.find( strClassId );
	if( iter != s_ClassMap.end() )
	{
		return iter->second;
	}
	return NULL;
}

CBCEOSAISerialRec* CEOSAISerial::GetBCEOSAISerialRec( unsigned long iClassId )
{
	if( iClassId < s_ClassVector.size() )
	{
		return s_ClassVector[iClassId];
	}
	return NULL;
}

//
//
//

CEOSAISerial::CEOSAISerial()
{
	m_iSerialExpandSize = DEFAULT_SERIAL_EXPAND_SIZE;

	m_bWriteCheckpointInformation = false;
	m_iCurrentLocation = 0;
	m_iUsedSize = 0;
	m_iAllocatedSize = 0;
	m_pData = 0;
}

CEOSAISerial::~CEOSAISerial()
{
	m_iCurrentLocation = 0;
	m_iUsedSize = 0;
	m_iAllocatedSize = 0;
	delete m_pData;
	m_pData = 0;
}

void  CEOSAISerial::SetCurrentLocation( long i )
{
	m_iCurrentLocation = i;
}

long  CEOSAISerial::GetCurrentLocation()
{
	return m_iCurrentLocation;
}

/*
void  CEOSAISerial::Serialize(   long     val ){ AppendLong( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Deserialize( long&    val ){ val = ExtractLong(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Serialize(   short    val ){ AppendShort( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Deserialize( short&   val ){ val = ExtractShort(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Serialize(   char     val ){ AppendChar( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Deserialize( char&    val ){ val = ExtractChar(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Serialize(   CString  val ){ AppendString( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Deserialize( CString& val ){ val = ExtractString(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Peek(        CString& val ){ val = PeekString(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Serialize(   bool     val ){ AppendBool( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Deserialize( bool&    val ){ val = ExtractBool(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Serialize(   float    val ){ AppendFloat( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }
void  CEOSAISerial::Deserialize( float&   val ){ val = ExtractFloat(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }

void  CEOSAISerial::Serialize(   DWORD    val ){ AppendDWORD( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( val ); }

void  CEOSAISerial::Serialize(   CString strDesc, long     val ){ AppendLong( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Deserialize( CString strDesc, long&    val ){ val = ExtractLong(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Serialize(   CString strDesc, short    val ){ AppendShort( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Deserialize( CString strDesc, short&   val ){ val = ExtractShort(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Serialize(   CString strDesc, char     val ){ AppendChar( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Deserialize( CString strDesc, char&    val ){ val = ExtractChar(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Serialize(   CString strDesc, CString  val ){ AppendString( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Deserialize( CString strDesc, CString& val ){ val = ExtractString(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Serialize(   CString strDesc, bool     val ){ AppendBool( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Deserialize( CString strDesc, bool&    val ){ val = ExtractBool(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Serialize(   CString strDesc, float    val ){ AppendFloat( val ); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
void  CEOSAISerial::Deserialize( CString strDesc, float&   val ){ val = ExtractFloat(); if( m_bWriteCheckpointInformation ) Checkpoint::Write( strDesc, val ); }
*/

//void  CEOSAISerial::Serialize(   unsigned long  val ){ AppendUnsignedLong( val ); }
//void  CEOSAISerial::Deserialize( unsigned long& val ){ val = ExtractUnsignedLong(); }
void  CEOSAISerial::Serialize(   long     val ){ AppendLong( val ); }
void  CEOSAISerial::Deserialize( long&    val ){ val = ExtractLong(); }
void  CEOSAISerial::Peek(        long&    val ){ val = PeekLong(); }

void  CEOSAISerial::Serialize(   DWORD    val ){ AppendDWORD( val ); }
void  CEOSAISerial::Deserialize( DWORD&   val ){ val = ExtractDWORD(); }

void  CEOSAISerial::Serialize(   short    val ){ AppendShort( val ); }
void  CEOSAISerial::Deserialize( short&   val ){ val = ExtractShort(); }
void  CEOSAISerial::Serialize(   unsigned short  val ){ AppendUnsignedShort( val ); }
void  CEOSAISerial::Deserialize( unsigned short& val ){ val = ExtractUnsignedShort(); }
void  CEOSAISerial::Serialize(   char     val ){ AppendChar( val ); }
void  CEOSAISerial::Deserialize( char&    val ){ val = ExtractChar(); }
void  CEOSAISerial::Serialize(   unsigned char     val ){ AppendUnsignedChar( val ); }
void  CEOSAISerial::Deserialize( unsigned char&    val ){ val = ExtractUnsignedChar(); }

void  CEOSAISerial::SerializeTCHAR(   CString  val ){ AppendStringTCHAR( val ); }
void  CEOSAISerial::DeserializeTCHAR( CString& val ){ val = ExtractStringTCHAR(); }
void  CEOSAISerial::PeekTCHAR(        CString& val ){ val = PeekStringTCHAR(); }

#ifdef UNICODE
void  CEOSAISerial::SerializeUnicode( CString& val ){ AppendStringUnicode( val ); }
void  CEOSAISerial::DeserializeUnicode( CString& val ){ val = ExtractStringUnicode(); }

void  CEOSAISerial::SerializeANSI8( CString& val ){ AppendStringANSI8( val ); }
void  CEOSAISerial::DeserializeANSI8( CString& val )
{
	#ifdef UNICODE
	USES_CONVERSION;
	CStringANSI8 strTextANSI8 = ExtractStringANSI8();
	val = A2T( strTextANSI8 );
	#else UNICODE
	val = ExtractStringANSI8();
	#endif UNICODE
}
#endif

void  CEOSAISerial::SerializeANSI8( CStringANSI8& val ){ AppendStringANSI8( val ); }
void  CEOSAISerial::DeserializeANSI8( CStringANSI8& val ){ val = ExtractStringANSI8(); }
void  CEOSAISerial::Serialize( const char*  val ){ AppendCharString( val ); } // Used by serialize class name
void  CEOSAISerial::SerializeANSI8( const char*  val ){ AppendCharString( val ); }

void  CEOSAISerial::Serialize(   bool     val ){ AppendBool( val ); }
void  CEOSAISerial::Deserialize( bool&    val ){ val = ExtractBool(); }
void  CEOSAISerial::Serialize(   float    val ){ AppendFloat( val ); }
void  CEOSAISerial::Deserialize( float&   val ){ val = ExtractFloat(); }
void  CEOSAISerial::Serialize(   double   val ){ AppendDouble( val ); }
void  CEOSAISerial::Deserialize( double&  val ){ val = ExtractDouble(); }

//void  CEOSAISerial::Serialize(   DWORD    val ){ AppendDWORD( val ); }

void  CEOSAISerial::Serialize(   CString strDesc, long     val ){ AppendLong( val ); }
void  CEOSAISerial::Deserialize( CString strDesc, long&    val ){ val = ExtractLong(); }
void  CEOSAISerial::Serialize(   CString strDesc, short    val ){ AppendShort( val ); }
void  CEOSAISerial::Deserialize( CString strDesc, short&   val ){ val = ExtractShort(); }
void  CEOSAISerial::Serialize(   CString strDesc, char     val ){ AppendChar( val ); }
void  CEOSAISerial::Deserialize( CString strDesc, char&    val ){ val = ExtractChar(); }
void  CEOSAISerial::SerializeTCHAR(   CString strDesc, CString  val ){ AppendStringTCHAR( val ); }
void  CEOSAISerial::DeserializeTCHAR( CString strDesc, CString& val ){ val = ExtractStringTCHAR(); }
void  CEOSAISerial::Serialize(   CString strDesc, char*    val ){ AppendCharString( val ); }
void  CEOSAISerial::Serialize(   CString strDesc, bool     val ){ AppendBool( val ); }
void  CEOSAISerial::Deserialize( CString strDesc, bool&    val ){ val = ExtractBool(); }
void  CEOSAISerial::Serialize(   CString strDesc, float    val ){ AppendFloat( val ); }
void  CEOSAISerial::Deserialize( CString strDesc, float&   val ){ val = ExtractFloat(); }


void CEOSAISerial::SerializeBlock( char* data, long iBlockSize )
{
	// Write the size of the block
	Serialize( iBlockSize );

	//long iAppendSize = iBlockSize;
	//
	Reallocate( m_iCurrentLocation + iBlockSize );
	memcpy( m_pData + m_iCurrentLocation, data, iBlockSize );
	m_iCurrentLocation += iBlockSize;
	// m_iUsedSize += iBlockSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

void CEOSAISerial::DeserializeBlock( char* data, long iExpectedBlockSize )
{
	// Read the size of the block
	long iBlockSize = 0;
	Deserialize( iBlockSize );
	ASSERT( iBlockSize == iExpectedBlockSize );

	char* pDebug_SerialStart = m_pData + m_iCurrentLocation;
	char* pDebug_SerialEnd   = m_pData + m_iCurrentLocation + iBlockSize;
	/*
	for( long iPos=0; iPos<iBlockSize; iPos+=10000 )
	{
		long iCopySize = iBlockSize-iPos;
		if( iCopySize > 10000 ){ iCopySize = 10000; }
		char* pSource = m_pData+m_iCurrentLocation+iPos;
		memcpy( data+iPos, pSource, iCopySize );
	}
	*/
	memcpy( data, m_pData + m_iCurrentLocation, iBlockSize );
	//memmove( data, m_pData + m_iCurrentLocation, iBlockSize );
	m_iCurrentLocation += iBlockSize;
}

void CEOSAISerial::SerializeBlockDirectly( char* data, long iBlockSize )
{
	Reallocate( m_iCurrentLocation + iBlockSize );
	memcpy( m_pData + m_iCurrentLocation, data, iBlockSize );
	m_iCurrentLocation += iBlockSize;
	//m_iUsedSize += iBlockSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

void CEOSAISerial::RemoveExcessAllocation()
{
	ASSERT( m_pData != NULL );
	ASSERT( m_iUsedSize != 0 );
	ASSERT( m_iAllocatedSize != 0 );

	if( m_iUsedSize != m_iAllocatedSize )
	{
		// 
		char* pNewData = new char[ m_iUsedSize ];
		if( pNewData == 0 )
		{
			ASSERT( false );
			return;
		}
		if( m_pData ) // always true
		{
			memcpy( pNewData, m_pData, m_iUsedSize );
		}
		delete m_pData;
		m_pData = pNewData;
		m_iAllocatedSize = m_iUsedSize;
	}
}

void* CEOSAISerial::InstanciateIfBlock( CWorldDescBase* pWorldDesc )
{
	/*
	// we will want to rewind the location
	int iCurrentLocation = m_iCurrentLocation;
	CString strClassName = this->ExtractString();
	// Now rewind the location
	m_iCurrentLocation = iCurrentLocation;
	*/
	CStringANSI8 strClassName = this->PeekStringANSI8();

	// Debug
#ifdef _DEBUG
	static CString strThisString;
	static CString strLastString;

	strLastString = strThisString;
	strThisString = strClassName;
#endif
#ifdef EOS

	if( strClassName == CCITY_SERVERTOPLAYERFLAT )
	{
		CCity_ServerToPlayerFlat* pObj = new CCity_ServerToPlayerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CCITY_PLAYERTOSERVERFLAT )
	{
		CCity_PlayerToServerFlat* pObj = new CCity_PlayerToServerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CUNIT_SERVERTOPLAYERFLAT )
	{
		CUnit_ServerToPlayerFlat* pObj = new CUnit_ServerToPlayerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CUNIT_PLAYERTOSERVERFLAT )
	{
		CUnit_PlayerToServerFlat* pObj = new CUnit_PlayerToServerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CUNITGROUP_SERVERTOPLAYERFLAT )
	{
		CUnitGroup_ServerToPlayerFlat* pObj = new CUnitGroup_ServerToPlayerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CUNITGROUP_PLAYERTOSERVERFLAT )
	{
		CUnitGroup_PlayerToServerFlat* pObj = new CUnitGroup_PlayerToServerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CUNITORDER_FLAT )
	{
		CUnitOrder_Flat* pObj = new CUnitOrder_Flat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CPoiPD_Flat_string )
	{
		CPoiPD_Flat* pObj = new CPoiPD_Flat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CPoiPD_string )
	{
		CPoiPD* pObj = new CPoiPD();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CAirfield_ServerToPlayerFlat_string )
	{
		CAirfield_ServerToPlayerFlat* pObj = new CAirfield_ServerToPlayerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CAirfield_PlayerToServerFlat_string )
	{
		CAirfield_PlayerToServerFlat* pObj = new CAirfield_PlayerToServerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CFortification_ServerToPlayerFlat_string )
	{
		CFortification_ServerToPlayerFlat* pObj = new CFortification_ServerToPlayerFlat;
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventBeginCombat_string )
	{
		CGameEventBeginCombat* pEvent = new CGameEventBeginCombat();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventEndCombat_string )
	{
		CGameEventEndCombat* pEvent = new CGameEventEndCombat();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventCombat_string )
	{
		CGameEventCombatHit* pEvent = new CGameEventCombatHit();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventCombatOccurred_string )
	{
		CGameEventCombatOccurred* pEvent = new CGameEventCombatOccurred();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventBlueGhostVisible_string )
	{
		CGameEventBlueGhostVisible* pEvent = new CGameEventBlueGhostVisible();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventNonVisible_string )
	{
		CGameEventNonVisible* pEvent = new CGameEventNonVisible();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventVisible_string )
	{
		CGameEventVisible* pEvent = new CGameEventVisible();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventPoiObjectState_string )
	{
		CGameEventPoiObjectState* pEvent = new CGameEventPoiObjectState();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventContainment_string )
	{
		CGameEventContainment* pEvent = new CGameEventContainment();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventDeath_string )
	{
		CGameEventDeath* pEvent = new CGameEventDeath();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventCapture_string )
	{
		CGameEventCapture* pEvent = new CGameEventCapture();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CResourceSource_string )
	{
		CResourceSource_ServerToPlayerFlat* pResource = new CResourceSource_ServerToPlayerFlat();
		pResource->Deserialize( this );
		return pResource;
	}
	else if( strClassName == CGameEventFoodAvailability_string )
	{
		CGameEventFoodAvailability* pEvent = new CGameEventFoodAvailability();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventOilAvailability_string )
	{
		CGameEventOilAvailability* pEvent = new CGameEventOilAvailability();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventIronAvailability_string )
	{
		CGameEventIronAvailability* pEvent = new CGameEventIronAvailability();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CGameEventStarvation_string )
	{
		CGameEventStarvation* pEvent = new CGameEventStarvation();
		pEvent->Deserialize( this );
		//return pEvent;
		// Depreciating this GameEvent
		delete pEvent;
		return NULL;
	}
	else if( strClassName == CGameEventStarvation2_string )
	{
		CGameEventStarvation2* pEvent = new CGameEventStarvation2();
		pEvent->Deserialize( this );
		return pEvent;
	}
	else if( strClassName == CCitySerialize_string )
	{
		CCity* pObj = new CCity();
		pObj->SetContext( pWorldDesc );
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CAirfieldSerialize_string )
	{
		CAirfield* pObj = new CAirfield();
		pObj->SetContext( pWorldDesc );
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CFortificationSerialize_string )
	{
		CFortification* pObj = new CFortification();
		pObj->SetContext( pWorldDesc );
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CUnitSerialize_string )
	{
		CUnit* pObj = new CUnit();
		pObj->SetContext( pWorldDesc );
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CUnitGroupSerialize_string )
	{
		CUnitGroup* pObj = new CUnitGroup();
		pObj->SetContext( pWorldDesc );
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CResourceSourceSerialize_string )
	{
		CResourceSource* pObj = new CResourceSource();
		pObj->SetContext( pWorldDesc );
		pObj->Deserialize( this );
		return pObj;
	}
	//
	else if( strClassName == CUnitOrderSerialize_string )
	{
		CUnitOrder* pObj = new CUnitOrder();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CBuildCompletionSerialize_string )
	{
		CBuildCompletion* pObj = new CBuildCompletion();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventInitialState_string )
	{
		CGameEventInitialState* pObj = new CGameEventInitialState();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventBuildComplete_string )
	{
		CGameEventBuildComplete* pObj = new CGameEventBuildComplete();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventEntrenched_string )
	{
		CGameEventEntrenched* pObj = new CGameEventEntrenched();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventUpgrade_string )
	{
		CGameEventUpgrade* pObj = new CGameEventUpgrade();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventRepair_string )
	{
		CGameEventRepair* pObj = new CGameEventRepair();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventRepair2_string )
	{
		CGameEventRepair2* pObj = new CGameEventRepair2();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventBeginOrEndBuildingAirfield_string )
	{
		CGameEventBeginOrEndBuildingAirfield* pObj = new CGameEventBeginOrEndBuildingAirfield();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CNationalGameEventAirAttack_string )
	{
		CNationalGameEventAirAttack* pObj = new CNationalGameEventAirAttack();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventLaunch_string )
	{
		CGameEventLaunch* pObj = new CGameEventLaunch();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CGameEventPopulationChange_string )
	{
		CGameEventPopulationChange* pObj = new CGameEventPopulationChange();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CNationalGameEventPlayerEliminated_string )
	{
		CNationalGameEventPlayerEliminated* pObj = new CNationalGameEventPlayerEliminated();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CNationalGameEventPlayerWon_string )
	{
		CNationalGameEventPlayerWon* pObj = new CNationalGameEventPlayerWon();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CNationalGameEventFinalResult_string )
	{
		CNationalGameEventFinalResult* pObj = new CNationalGameEventFinalResult();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CTradeAgreement_string )
	{
		CTradeAgreement* pObj = new CTradeAgreement();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CMountain_string )
	{
		CMountain* pObj = new CMountain();
		pObj->SetContext( pWorldDesc );
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CMountain_ServerToPlayerFlat_string )
	{
		CMountain_ServerToPlayerFlat* pObj = new CMountain_ServerToPlayerFlat();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CNationalGameEventTeamFormed_string )
	{
		CNationalGameEventTeamFormed* pObj = new CNationalGameEventTeamFormed();
		pObj->Deserialize( this );
		return pObj;
	}
	else if( strClassName == CReinforcementsPoint_string )
	{
		CReinforcementsPoint* pObj = new CReinforcementsPoint();
		pObj->Deserialize( this );
		return pObj;
	}
	#endif EOS

	ASSERT( false );
	return NULL;
}


void* CEOSAISerial::Instanciate( CWorldDescBase* pWorldDesc )
{
	// Peek at the id
	char cId = this->PeekChar();

	// if the first char is 0, then we know it isn't a string (strings have SOME length)
	if( cId == 0 )
	{
		long iClassId = this->PeekLong();
		long iClassIdCopy = iClassId;
		iClassId /= 256;

		// Debug
		#ifdef _DEBUG
		static long iThisClassId = 0;
		static long iLastClassId = 0;

		iLastClassId = iThisClassId;
		iThisClassId = iClassId;
		#endif

		CBCEOSAISerialRec* pRec = CEOSAISerial::GetBCEOSAISerialRec( iClassId );
		ASSERT( pRec );
		if( pRec )
		{
			return pRec->m_pInstanciateFunction( this, pWorldDesc );
		}
		return NULL;
	}
	else  
	{
		CStringANSI8 strClassId = this->PeekStringANSI8();

		//
		long iDebugClassId = strClassId.GetAt(0);

		// Debug
		#ifdef _DEBUG
		static CString strThisString;
		static CString strLastString;

		strLastString = strThisString;
		strThisString = strClassId;
		#endif

		CBCEOSAISerialRec* pRec = CEOSAISerial::GetBCEOSAISerialRec( strClassId );
		//ASSERT( pRec );
		if( pRec )
		{
			return pRec->m_pInstanciateFunction( this, pWorldDesc );
		}
		else
		{
			return InstanciateIfBlock( pWorldDesc );
		}
		return NULL;
	}
}

bool CEOSAISerial::WriteSanityCheckToFile( FILE* fp, long iSanityCheck )
{
	// Size of the data
	size_t iReturn = fwrite( &iSanityCheck, sizeof( long ), 1, fp );
	if( iReturn == 0 ) return false;
	return true;
}

bool CEOSAISerial::WriteToFile( FILE* fp )   // write a block of data from the CEOSAISerial object
{
	// size of the data
	size_t iReturn = fwrite( &( this->m_iUsedSize ), sizeof( long ), 1, fp );
	if( iReturn == 0 ) goto BadWriteError;

	// The data
	iReturn = fwrite( this->m_pData, sizeof( char ), this->m_iUsedSize, fp );
	if( iReturn == 0 ) goto BadWriteError;

	return true;

BadWriteError:

	CString strText;
	#ifdef EOS
	strText = Lang( 233, _T("Error: Save failed.") );
	#else
	strText = _T("Error: Save failed.");
	#endif
	AfxMessageBox( strText );
	return false;
}

long CEOSAISerial::ReadSanityCheckFromFile( FILE* fp )
{
	long iSanityCheck = 0;
	size_t iReturn = fread( &iSanityCheck, sizeof( long ), 1, fp );
	return iSanityCheck;
}

long CEOSAISerial::ReadSanityCheckFromFile( CFile* pFile )
{
	long iSanityCheck = 0;
	pFile->Read( &iSanityCheck, sizeof( long ) );
	//size_t iReturn = fread( &iSanityCheck, sizeof( long ), 1, fp );
	return iSanityCheck;
}

bool CEOSAISerial::ReadFromFile( FILE* fp )
{
	// Size of the data
	long iSizeOfData = 0;
	size_t iReturn = fread( &iSizeOfData, sizeof( long ), 1, fp );
	if( iReturn == 0 )
	{
		#ifdef EOS
		AfxMessageBox( Lang( 234, _T("Error: Load failed.  Bad data.") ) );
		#else
		AfxMessageBox( _T("Error: Load failed.  Bad data.") );
		#endif
		return false;
	}

	// The data
	this->ForceAllocationSize( iSizeOfData );
	iReturn = fread( this->m_pData, sizeof( char ), iSizeOfData, fp );
	if( iReturn != iSizeOfData )
	{
		#ifdef EOS
		AfxMessageBox( Lang( 234, _T("Error: Load failed.  Bad data.") ) );
		#else
		AfxMessageBox( _T("Error: Load failed.  Bad data.") );
		#endif
		return false;
	}
	return true;
}

bool CEOSAISerial::ReadFromFile( CFile* pFile )
{
	// Size of the data
	long iSizeOfData = 0;
	UINT iReturn = pFile->Read( &iSizeOfData, sizeof( long ) );
	//size_t iReturn = fread( &iSizeOfData, sizeof( long ), 1, fp );
	if( iReturn == 0 )
	{
		#ifdef EOS
		AfxMessageBox( Lang( 234, _T("Error: Load failed.  Bad data.") ) );
		#else
		AfxMessageBox( _T("Error: Load failed.  Bad data.") );
		#endif
		return false;
	}

	// The data
	this->ForceAllocationSize( iSizeOfData );
	iReturn = pFile->Read( this->m_pData, iSizeOfData );
	if( iReturn != iSizeOfData )
	{
		#ifdef EOS
		AfxMessageBox( Lang( 234, _T("Error: Load failed.  Bad data.") ) );
		#else
		AfxMessageBox( _T("Error: Load failed.  Bad data.") );
		#endif
		return false;
	}
	return true;
}

bool CEOSAISerial::WriteFile2( CString strFullFilename )
{
	CFile File;
	if( File.Open( strFullFilename, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary ) )
	{
		File.Write( this->m_pData, this->m_iUsedSize );
		File.Flush();
		return true;
	}
	return false;
}

bool CEOSAISerial::ReadFile2( CString strFullFilename, ULONG uMaximumSize )
{
	CFile File;
	if( File.Open( strFullFilename, CFile::modeRead | CFile::typeBinary ) )
	{
		ULONGLONG uBytesToRead = File.GetLength();
		if( uMaximumSize > 0 )
		{
			if( uBytesToRead > uMaximumSize )
			{
				uBytesToRead = uMaximumSize;
			}
		}

		// The data
		unsigned long iBytesToRead = (unsigned long) uBytesToRead;
		this->ForceAllocationSize( iBytesToRead );//uBytesToRead );
		UINT iReturn = File.Read( this->m_pData, iBytesToRead );//uBytesToRead );
		if( iReturn != uBytesToRead )
		{
			#ifdef EOS
			AfxMessageBox( Lang( 234, _T("Error: Load failed.  Bad data.") ) );
			#else
			AfxMessageBox( _T("Error: Load failed.  Bad data.") );
			#endif
			return false;
		}
		return true;
	}
	return false;
}

void CEOSAISerial::SerializeClassId( CStringANSI8 strClassId )
{
	SerializeANSI8( strClassId );
}

void CEOSAISerial::SerializeClassId( long iClassId )
{
	Serialize( iClassId*256 );  // I want the low-byte to be 0.
}

void CEOSAISerial::ExtractAndVerifyClassId( CStringANSI8 strClassId )
{
	// Peek at the id
	char cId = this->PeekChar();

	// if the first char is 0, then we know it isn't a string (strings have SOME length)
	if( cId == 0 )
	{
		ASSERT( false );
	}
	else  
	{
		CStringANSI8 strSerialClassId = this->ExtractStringANSI8();
		ASSERT( strSerialClassId == strClassId );
	}
}

void CEOSAISerial::ExtractAndVerifyClassId( long iClassId )
{
	// Peek at the id
	char cId = this->PeekChar();

	// if the first char is 0, then we know it isn't a string (strings have SOME length)
	if( cId == 0 )
	{
		long iSerialClassId = this->ExtractLong();
		ASSERT( iSerialClassId/256 == iClassId );
	}
	else  
	{
		ASSERT( false );
	}
}

void CEOSAISerial::AppendLong( long val )
{
	long iAppendSize = sizeof( long );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &val, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

long CEOSAISerial::ExtractLong()
{
	long iVal = 0;
	long iVarSize = sizeof( long );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return iVal;
}

long CEOSAISerial::PeekLong()
{
	long iVal = 0;
	long iVarSize = sizeof( long );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	//m_iCurrentLocation += iVarSize;
	ASSERT( m_iCurrentLocation + iVarSize <= m_iUsedSize );

	return iVal;
}

short CEOSAISerial::PeekShort()
{
	short sVal = 0;
	short iVarSize = sizeof( short );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &sVal, m_pData + m_iCurrentLocation, iVarSize );
	//m_iCurrentLocation += iVarSize;
	ASSERT( m_iCurrentLocation + iVarSize <= m_iUsedSize );

	return sVal;
}

/*
void CEOSAISerial::AppendDWORD( long val )
{
	long iAppendSize = sizeof( DWORD );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &val, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation+iAppendSize, m_iUsedSize );
}

DWORD CEOSAISerial::ExtractDWORD()
{
	DWORD iVal = 0;
	long  iVarSize = sizeof( DWORD );
	//
	memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return iVal;
}
*/

void CEOSAISerial::AppendShort( short val )
{
	long iAppendSize = sizeof( short );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &val, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

short CEOSAISerial::ExtractShort()
{
	short iVal = 0;
	long iVarSize = sizeof( short );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return iVal;
}


void CEOSAISerial::AppendUnsignedShort( unsigned short val )
{
	long iAppendSize = sizeof( unsigned short );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &val, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

unsigned short CEOSAISerial::ExtractUnsignedShort()
{
	unsigned short iVal = 0;
	long iVarSize = sizeof( unsigned short );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return iVal;
}

void CEOSAISerial::AppendChar( char val )
{
	long iAppendSize = sizeof( char );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &val, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

void CEOSAISerial::AppendUnsignedChar( unsigned char val )
{
	long iAppendSize = sizeof( unsigned char );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &val, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

char CEOSAISerial::ExtractChar()
{
	char iVal = 0;
	long iVarSize = sizeof( char );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return iVal;
}

unsigned char CEOSAISerial::ExtractUnsignedChar()
{
	unsigned char iVal = 0;
	long iVarSize = sizeof( unsigned char );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return iVal;
}


char CEOSAISerial::PeekChar()
{
	char iVal = 0;
	long iVarSize = sizeof( char );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	//m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return iVal;
}

#ifdef UNICODE
void CEOSAISerial::AppendStringUnicode( CString strText )
{
	ASSERT( sizeof(TCHAR) == sizeof(WCHAR) );
	long iAppendSize = sizeof(TCHAR) * (strText.GetLength() + 1); // include the null
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, (LPCTSTR) strText, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}
#endif

void CEOSAISerial::AppendStringTCHAR( CString strText )
{
	long iAppendSize = sizeof(TCHAR) * (strText.GetLength() + 1); // include the null
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, (LPCTSTR) strText, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

void CEOSAISerial::AppendStringANSI8( CStringANSI8 strText )
{
	long iAppendSize = strText.GetLength() + 1; // include the null
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, (const char*) strText, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

#ifdef UNICODE
void CEOSAISerial::AppendStringANSI8( CString strText )
{
	#ifdef UNICODE
	USES_CONVERSION;
	CStringANSI8 strTextANSI8 = T2A( strText );
	#else
	CStringANSI8 strTextANSI8 = strText;
	#endif

	long iAppendSize = strTextANSI8.GetLength() + 1; // include the null
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, (const char*) strTextANSI8, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}
#endif UNICODE

void CEOSAISerial::AppendCharString( const char* szString )
{
	long iAppendSize = strlen( szString ) + 1;//strText.GetLength() + 1; // include the null
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, (const char*) szString, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

#ifdef UNICODE
CString CEOSAISerial::ExtractStringUnicode()
{
	ASSERT( sizeof(TCHAR) == sizeof(WCHAR) );
	CString strText;
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	strText = (TCHAR*)( m_pData + m_iCurrentLocation );
	int iLength = strText.GetLength() + 1;
	//memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += sizeof(WCHAR) * iLength;
	ASSERT( m_iCurrentLocation <= m_iUsedSize );
	return strText;
}
#endif UNICODE

CString CEOSAISerial::ExtractStringTCHAR()
{
	CString strText;
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	strText = (TCHAR*)( m_pData + m_iCurrentLocation );
	int iLength = strText.GetLength() + 1;
	//memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += sizeof(TCHAR) * iLength;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return strText;
}

CString CEOSAISerial::PeekStringTCHAR()
{
	CString strText;
	//
	strText = m_pData + m_iCurrentLocation;
	int iLength = strText.GetLength() + 1;
	//m_iCurrentLocation += iLength;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return strText;
}

CStringANSI8 CEOSAISerial::ExtractStringANSI8()
{
	CStringANSI8 strText;
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	strText = m_pData + m_iCurrentLocation;
	int iLength = strText.GetLength() + 1;
	//memcpy( &iVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iLength;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return strText;
}

CStringANSI8 CEOSAISerial::PeekStringANSI8()
{
	CStringANSI8 strText;
	//
	strText = m_pData + m_iCurrentLocation;
	int iLength = strText.GetLength() + 1;
	//m_iCurrentLocation += iLength;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return strText;
}

void CEOSAISerial::AppendBool( bool val )
{
	char bVal = val;
	long iAppendSize = sizeof( char );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &bVal, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

bool CEOSAISerial::ExtractBool()
{
	char bVal = 0;
	long iVarSize = sizeof( char );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &bVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return ( bVal != 0 );
}

void CEOSAISerial::AppendFloat( float fVal )
{
	long iAppendSize = sizeof( float );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &fVal, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

float CEOSAISerial::ExtractFloat()
{
	float fVal = 0.0f;
	long iVarSize = sizeof( float );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &fVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return fVal;
}


void CEOSAISerial::AppendDouble( double fVal )
{
	long iAppendSize = sizeof( double );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &fVal, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

double CEOSAISerial::ExtractDouble()
{
	double fVal = 0.0f;
	long iVarSize = sizeof( double );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &fVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return fVal;
}

void CEOSAISerial::AppendDWORD( DWORD val )
{
	long iAppendSize = sizeof( DWORD );
	//
	Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( m_pData + m_iCurrentLocation, &val, iAppendSize );
	m_iCurrentLocation += iAppendSize;
	// m_iUsedSize += iAppendSize;
	m_iUsedSize = max( m_iCurrentLocation, m_iUsedSize );
}

DWORD CEOSAISerial::ExtractDWORD()
{
	DWORD dwVal = 0;
	long iVarSize = sizeof( DWORD );
	//
	//Reallocate( m_iCurrentLocation + iAppendSize );
	memcpy( &dwVal, m_pData + m_iCurrentLocation, iVarSize );
	m_iCurrentLocation += iVarSize;

	ASSERT( m_iCurrentLocation <= m_iUsedSize );

	return dwVal;
}

void CEOSAISerial::ForceAllocationSize( int iSize )
{
	Reallocate( iSize );
	m_iUsedSize = iSize;
}

void CEOSAISerial::Reallocate( int iMinimumNewSize )
{
	if( iMinimumNewSize > m_iAllocatedSize )
	{
		// 
		long iNewSize = 0;
		if( m_iAllocatedSize + m_iSerialExpandSize > iMinimumNewSize )
		{
			iNewSize = m_iAllocatedSize + m_iSerialExpandSize;
		}
		else
		{
			iNewSize = iMinimumNewSize + m_iSerialExpandSize;
		}

		char* pNewData = new char[ iNewSize ];
		if( pNewData == 0 )
		{
			ASSERT( false );
			return;
		}
		if( m_pData )
		{
			memcpy( pNewData, m_pData, m_iAllocatedSize );
		}
		delete m_pData;
		m_pData = pNewData;
		m_iAllocatedSize = iNewSize;
	}
}


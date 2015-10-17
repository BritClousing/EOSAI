
#include "stdafx.h"
#include "EOSAIBrain.h"
#include "AIPlayer.h"
//#include "ResourceSource.h"

//#include "City.h"
//#include "Unit.h"
//#include "Player.h"
#include "City.h"
//#include "EOSAIProductionTask.h"
//#include "EOSAIProductionOption.h"
#include "EOSAIAirfield.h"
#include "EOSAIUnit2.h"
#include "EOSAIRegion2.h"
#include "EOSAIResource.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"
//#include "WorldDescPlayerProxy.h"
//#include "AICombatPlanner.h"
#include "EOSAIUnit2.h"

//#include "EOSAIBrain.h"
#include "EOSAIThoughtDatabase.h"
#include "EOSAIGeo.h"
#include "EOSAIGeoView.h"
//#include "GeoLand.h"
//#include "MapTextOverlay.h"
//#include "GLMapWindow.h"
//#include "GLMapWindow2.h"
//#include "AITransportDefinition.h"
#include "EOSAIPlayerInteraction.h"
//#include "AIUnresolvedSituation.h"
#include "EOSAIUnitTask_MoveTo.h"
//#include "MovementManager.h"
//#include "MessageManager.h"
#include "EOSAIMultiRegion2.h"
//#include "InternalMail.h"
//#include "GameAppState.h"
//#include "MovementPathIdeasForAirUnit.h"
//#include "MovementPathIdeasForGroundUnit.h"
//#include "MovementPathIdeasForSeaUnit.h"
//#include "TWDx.h"
//#include "AICombatPlanner.h"
#include "EOSAICommonData.h"
//#include "GameEvent_IMail.h"

// DEBUG
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAIUnitPathwayResult.h"
#include "EOSAIAirUnitPathwayFinder.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAIAirbasesSet.h"
//#include "EOSAIQuickCombatCalculation.h"

#include "EOSAITransportComboMap.h"
#include "EOSAIStopwatch2.h"
//#include "MessageTargetManager.h"
//#include "GameEventVisible.h"
#include "EOSAIMultiRegionAttackAccess.h"
//#include "PoiObserver.h"
#include "EOSAIMultiRegionNationwidePathways.h"
//#include "MapLabel.h"
//#include "Checkpoint.h"
//#include "Geo.h"
#include "EOSAIMultiRegionPlan.h"
#include "EOSAITacticalProject2.h"
#include "EOSAIDesire2.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAIBuildOption.h"
#include "EOSAINationalSummary3.h"

#include <iostream>

// DEBUG
#include "EOSAITacticalProject2.h"
#include "EOSAISeaUnitPathwayFinder.h"
#include "EOSAIUnitTemplatesAndFloat.h"
// DEBUG
#include "EOSAIStrategicAIOrder_Trade.h"

#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

#include <math.h>

// DEBUG
CEOSAIUnit2* g_pAIUnit17 = NULL;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

bool SortByDistanceCitResScore( CEOSAIGeoView* p1, CEOSAIGeoView* p2 )
{
	if(      p2->GetGeoDistanceCitResScore() < p1->GetGeoDistanceCitResScore() ){ return true; }
	else if( p2->GetGeoDistanceCitResScore() > p1->GetGeoDistanceCitResScore() ){ return false; }
	return true;  // always return true when values are equal
}

class CAIResourceState
{
	public:
		CAIResourceState()
		{
			m_fCurrent = 0.0f;
			m_fDelta = 0.0f;
			m_fBuy = 0.0f;
			m_fSell = 0.0f;
		}
		float GetNext(){ return m_fCurrent + m_fDelta + m_fBuy - m_fSell; }
		void  RectifyBuyAndSellValues(){ float fSubtract = min( m_fBuy, m_fSell ); m_fBuy -= fSubtract; m_fSell -= fSubtract; }

		float m_fCurrent;
		float m_fDelta;
		float m_fBuy;
		float m_fSell;
};

class CAIResourceStateAll
{
	public:
		CAIResourceState  m_Money;
		CAIResourceState  m_Food;
		CAIResourceState  m_Iron;
		CAIResourceState  m_Oil;
};


CEOSAIBrain::CEOSAIBrain()
{
	//m_bAllowAIToCheat = true;

	//m_Desires.SetSortFunction( SortDesiresByValue );
	//m_BestAvailableActions.SetSortFunction( SortDesiresByScore );

	// For non-cheating AI
	m_fEstimatedCitResProductionPerArea = 0.0f;
	m_fEstimatedCitResProductionPerLandArea  = 0.0f;
	m_fEstimatedCitResProductionPerWaterArea = 0.0f;

	// Cities are more valuable early in the game because they can build things
//	m_CitResValueMultiplier.m_fCityProductionValueMultiplier = 2.0f;
//	m_CitResValueMultiplier.m_fIronValueMultiplier = 1.0f;
//	m_CitResValueMultiplier.m_fFoodValueMultiplier = 1.0f;
//	m_CitResValueMultiplier.m_fOilValueMultiplier = 1.0f;
//	m_CitResValueMultiplier.m_fGoldValueMultiplier = 1.0f;

	m_bProcessInitialTurnState = false;
	m_bProcessTurnBasedOnForeignRelations = false;

	// Game-State information (used for making build decisions)
	//m_fGameState_Expansion01 = 0.0f;
	//m_fGameState_Conquest01 = 0.0f;

	m_GetTransportAssistedMovementPath_Count = 0;

	//m_DecentLogicalActions.IOwnTheObjects( true );
	//m_DecentLogicalActions.SetMaximumSize( 20 );
}

void CEOSAIBrain::Initialize( EOSAI::AIPlayer* pAIPlayer )
{
	ASSERT( pAIPlayer );
	m_pAIPlayer = pAIPlayer;
	//m_pWorldDescPlayer = m_pAIPlayer->GetWorldDescPlayer();
	//m_BuildManager.SetAIBrain( this );
	m_AIActionScheduler.SetAIBrain( this );
	m_AIThoughtDatabase.SetAIBrain( this );
	m_AIBuildManager.SetAIPlayer( pAIPlayer );
}

CEOSAIBrain::~CEOSAIBrain()
{
	// Delete objects
	// Don't delete regions - they get deleted by the spatial map
	DeleteAIData();
/*
	for( long iAIGeo=0; iAIGeo<m_AIGeoArray.m_iSize; iAIGeo++ )
	{
		delete m_AIGeoArray[iAIGeo];
		m_AIGeoArray[iAIGeo] = NULL;
	}
	for( long iMultiRegion=0; iMultiRegion<m_MultiRegionPlanArray.m_iSize; iMultiRegion++ )
	{
		delete m_MultiRegionPlanArray[iMultiRegion];
		m_MultiRegionPlanArray[iMultiRegion] = NULL;
	}

	while( m_AIObjectList.IsEmpty() == FALSE ){ delete m_AIObjectList.RemoveHead(); }
	m_AIPoiObjects.RemoveAll();

	//m_AIThoughtDatabase.Clear();

	DeleteAIProductionOptions();
*/
	/*
	for( long i=0; i<m_AIGeoArray.m_iSize; i++ )
	{
		delete m_AIGeoArray[i];
		m_AIGeoArray[i] = NULL;
	}
	*/
}

void CEOSAIBrain::DeleteAIData()
{
	// Delete objects
	// Don't delete regions - they get deleted by the spatial map
	/*
	long iCount = 0;
	while( m_AIObjectList.IsEmpty() == FALSE )
	{
		delete m_AIObjectList.RemoveHead();
		iCount++;
		if( iCount == 50 )
		{
			Sleep(1);
			iCount = 0;
		}
	}
	*/
	//m_AIPoiObjects.RemoveAll();

	//this->DeleteAIProductionOptions();
	this->GetAIBuildManager()->Clear();
	//this->GetAIRegionMapToEverywhere()->Clear();

	// We keep the AIGeos around, so they shouldn't get deleted here
	//for( long i=0; i<m_AIGeoArray.m_iSize; i++ )
	//{
	//	delete m_AIGeoArray[i];
	//	m_AIGeoArray[i] = NULL;
	//}
	for( long i=0; i<m_MultiRegionPlanArray.m_iSize; i++ )
	{
		delete m_MultiRegionPlanArray[i];
		m_MultiRegionPlanArray[i] = NULL;
	}
}

long CEOSAIBrain::GetAIPlayerNumber()
{
	return m_pAIPlayer->GetPlayerNumber();
}
/*
CWorldDescServer* CEOSAIBrain::GetWorldDescServer()
{
	return m_pAIPlayer->GetWorldDescServer();
}

CWorldDescPlayerProxy* CEOSAIBrain::GetWorldDescPlayerProxy()
{
	return m_pAIPlayer->GetWorldDescPlayerProxy();
}
*/
/*
void  CEOSAIBrain::CalculateResourceDeltasAndResourceEffectsSummary()
{
	long iLocalPlayer = m_iLocalPlayerNumber;

	EOSAI::StringAndFloatSet ResourceTotals;
	ResourceTotals.Add(_T("Money"), m_fTotalMoney);
	ResourceTotals.Add(_T("Iron"), m_fTotalIron);
	ResourceTotals.Add(_T("Food"), m_fTotalFood);
	ResourceTotals.Add(_T("Oil"), m_fTotalOil);

	CEOSAINationalSummary3* pNationalSummary = g_pEOSAICommonData->GetAINationalSummary3(iLocalPlayer);// ->m_AIResourceEffectsSummary
	pNationalSummary->m_ResourceSummary.Calculate(m_pWorldDescServer, iLocalPlayer, &ResourceTotals);
	//m_ResourceEffectsSummary.Calculate(m_pWorldDescServer, iLocalPlayer, &ResourceTotals);
}
*/
/*
//void  CEOSAIBrain::CalculateResourceConsumptionAndDeltas(bool bIncludeCityNoOrdersProduceWealth)
void  CEOSAIBrain::CalculateResourceConsumptionAndDeltas(bool bIncludeCityNoOrdersProduceWealth)
{
	ASSERT(false);
	
	CalculateResourceDeltasAndResourceEffectsSummary();
	/-*
	ASSERT(bIncludeCityNoOrdersProduceWealth == false);

	float fOilProduction = m_pWorldDescServer->AmountOfOilProducedOverTimeperiod(m_iLocalPlayerNumber, 1.0f);
	float fOilConsumption = m_pWorldDescServer->AmountOfOilConsumedOverTimeperiod(m_iLocalPlayerNumber, 1.0f);
	//float fOilTrade = 0.0f;

	float fFoodProduction = m_pWorldDescServer->AmountOfFoodProducedOverTimeperiod(m_iLocalPlayerNumber, 1.0f);
	float fFoodConsumption = m_pWorldDescServer->AmountOfFoodConsumedOverTimeperiod(m_iLocalPlayerNumber, 1.0f);
	//float fFoodTrade = 0.0f;

	float fIronProduction = m_pWorldDescServer->AmountOfIronProducedOverTimeperiod(m_iLocalPlayerNumber, 1.0f);
	float fIronConsumption = m_pWorldDescServer->AmountOfIronConsumedOverTimeperiod(m_iLocalPlayerNumber, 1.0f);
	//float fIronTrade = 0.0f;

	float fMoneyProduction = m_pWorldDescServer->AmountOfMoneyProducedOverTimeperiod(m_iLocalPlayerNumber, 1.0f);
	float fMoneyConsumption = m_pWorldDescServer->AmountOfMoneyConsumedOverTimeperiod(m_iLocalPlayerNumber, 1.0f);
	//float fMoneyTrade = 0.0f;

	if (bIncludeCityNoOrdersProduceWealth == false)
	{
		float fImplicitProduceWealth = 0.0f;
		POSITION pos = m_pWorldDescServer->GetActorsList()->GetHeadPosition();
		while (pos)
		{
			//CPoiLink* pLink = m_PoiList.GetNext( pos );
			CPoiObject* pPoiObject = m_pWorldDescServer->GetActorsList()->GetNext(pos);
			if (pPoiObject->GetOwner() == m_iLocalPlayerNumber)
			{
				CCity* pCity = dynamic_cast< CCity* >(pPoiObject);
				if (pCity && pCity->GetBuildOrders()->IsEmpty())
				{
					fImplicitProduceWealth += pCity->GetMoneyProducedOverTimeperiod(1.0f);
				}
			}
		}
		fMoneyProduction -= fImplicitProduceWealth;
	}

	//m_fDeltaMoney = AmountOfMoneyProducedOverTimeperiod( m_iLocalPlayerNumber, 1.0f ) - AmountOfMoneyConsumedOverTimeperiod( m_iLocalPlayerNumber, 1.0f );
	//m_fDeltaIron  = AmountOfIronProducedOverTimeperiod( m_iLocalPlayerNumber, 1.0f )  - AmountOfIronConsumedOverTimeperiod( m_iLocalPlayerNumber, 1.0f );
	//m_fDeltaFood  = AmountOfFoodProducedOverTimeperiod( m_iLocalPlayerNumber, 1.0f )  - AmountOfFoodConsumedOverTimeperiod( m_iLocalPlayerNumber, 1.0f );

	//long iLocalPlayer = GetLocalPlayerNumber();
	EOSAI::StringAndFloatSet  ResourceTradeDelta;
	//POSITION pos = m_pWorldDescServer->GetOngoingTradeAgreements()->GetHeadPosition();
	POSITION pos = m_pWorldDescServer->GetTradeAgreements()->GetHeadPosition();
	while (pos)
	{
		//CTradeAgreement* pTradeAgreement = m_pWorldDescServer->GetOngoingTradeAgreements()->GetNext( pos );
		CTradeAgreement* pTradeAgreement = m_pWorldDescServer->GetTradeAgreements()->GetNext(pos);
		if (pTradeAgreement->m_TradeAgreementState == EnumTradeAgreementState_ActiveOngoingAgreement)
		{
			pTradeAgreement->AddOngoingMoneyAndResourceDeltas(m_iLocalPlayerNumber, &ResourceTradeDelta);
		}
	}

	float fOilTrade = ResourceTradeDelta.Get(_T("Oil"));
	float fFoodTrade = ResourceTradeDelta.Get(_T("Food"));
	float fIronTrade = ResourceTradeDelta.Get(_T("Iron"));
	float fMoneyTrade = ResourceTradeDelta.Get(_T("Money"));
	*-/
}
*/
void CEOSAIBrain::CreateAIProductionOptions()
{
	// Several variables are used to determine this -
	//   m_fDesireToExplore increases exploration units (airplanes, destroyers)
	//   m_fDesireToExpand increases expansion units (tanks, transports - good for taking over unowned cities and resources)
	//   m_fDesireToConquer increases war units (tanks, infantry, battleships, etc)
	//   "Danger" rating increases desire for strong or stealthy exploration units
	//   Islands with high danger ratings increase the creation of war units
	//     Islands with high danger of amphibious invasion produce some airplanes (monitoring + early attack), places infantry on the shores
	//     Unconsolidated islands increase the creation of ground invasion units (doesn't include artillery very much)
	//     Presence of enemies on on island increase creation of ground combat units (including artillery)
	//       Note: the AI needs to decide whether to adopt a defensive posture or an offensive one.
	//       A defensive posture aims to maintain control of it's portion of the island - either indefinately, as a speed-bump, or until reinforcements arrive.
	//       An offensive posture seeks to invade the island with superior numbers of offensive units (though could ignite a larger war).
	//   The current military state
	//   The nation's peace/war status with other nations.

	// There should also be a randomization so that AI opponents don't behave identically.
	//    Some should favor industrial warfare and subs, others might favor monitoring and mobility, ... (and the ability to combine two strategies)

	// Note: it's probably a good idea to assign attribute values to units for the benefit of AI calculations (the AI can also assign some of these numbers via calculations).
	//    Attributes might include: 
	//	exploration (discover new map information), 
	//	expansion (assumes no resistence), 
	//	ground defense (assumes resistence - defensive posture), 
	//	ground attack (assumes resistence - offensive posture), 
	//	sea-control, 
	//	industrial warfare (bombers, subs / targetting cities and freighters)
	//	fast-responder (planes), ...
	//   Include:
	//      Production, oil, and iron cost with iron and oil availability as a multiplier

	// Use the CurrentCapability and Desired Capability to construct an optimal build list
	//   Thing which might be useful to take into account:
	//   Units consumption of resources (including resources I might be low on).
	//   Build Cost
	//   How well it satisfies the DesiredCapability
	//   How can I organize build orders so that the correct cities get them 
	//     (i.e. short build times on the front, longer build times in the back,
	//           defensive/consolidation units on the front, build order reflect the
	//           city's local needs).

/*
	// Delete the existing production options
	//CString strUnitsubset = m_pAIPlayer->GetPlayer()->GetUnitsubset();
	EOSAI::CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer( GetAIPlayerNumber() );//m_pAIPlayer->GetPlayerNumber() );
	CString strUnitsubset = pGamePlayer->GetUnitsubset();
	//POSITION pos = GetCommonState()->GetBuildOptionList()->GetHeadPosition();
	POSITION pos = g_pEOSAIInterface->GetAICommonData()->GetAIBuildOptionList()->GetHeadPosition();
	while( pos )
	{
		//CBuildOption* pBuildOption = GetCommonState()->GetBuildOptionList()->GetNext( pos );
		CEOSAIBuildOption* pBuildOption = g_pEOSAIInterface->GetAICommonData()->GetAIBuildOptionList()->GetNext( pos );

		//if( GetCommonState()->GetActiveUnitset()->CanBuild( GetAIPlayerNumber(), pBuildOption, false ) )
		if( g_pEOSAIInterface->GetAIGameRules()->CanBuild( GetAIPlayerNumber(), pBuildOption, false ) )
		{
			CEOSAIProductionOption* pProdOption = new CEOSAIProductionOption();
			pProdOption->m_pBuildOption = pBuildOption;

			// Evaluate this production option
			pProdOption->Evaluate( GetAIPlayer() );

			m_AIProductionOptions.AddTail( pProdOption );
		}
	}
*/
}
/*
void CEOSAIBrain::DeleteAIProductionOptions()
{
	// Delete the existing production options
	while( m_AIProductionOptions.IsEmpty() == FALSE )
	{
		delete m_AIProductionOptions.RemoveHead();
	}
}
*/

bool CEOSAIBrain::GetAllowHostilityAgainst( long iPlayer )
{
	EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( GetAIPlayerNumber(), iPlayer );
	return eRel.IsEnemy();
}

void CEOSAIBrain::SetAllowHostilityAgainst( long iPlayer, bool bAllowHostility )
{
	// INCOMPLETE
}

void CEOSAIBrain::CreateTransportCombinedAIRegionMaps()
{
	DeleteTransportCombinedAIRegionMaps();
	//
	CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
	CList< CEOSAIPoiObject* >* pMyActors = GetAIThoughtDatabase()->GetMyActors();
	//long iNumberOfAIRegions = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetAIRegionManager()->GetNumberOfGenericRegions();
	//long iNumberOfAIRegions = pTransportPathwayMap->GetArraySize();
	long iNumberOfAIRegions = g_pEOSAICommonData->GetAIRegionManager()->m_PathfinderPointArray.GetSize();
	POSITION pos = pUnitsICanBuildOrHave->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pTransportUnitTemplate = pUnitsICanBuildOrHave->m_List.GetNext( pos );
		if( pTransportUnitTemplate->IsTransport() == false ) continue;
		if( pTransportUnitTemplate->IsSeaUnit() == false &&
			pTransportUnitTemplate->IsAirUnit() == false ) continue;

		CEOSAITransportCombinedMap* pAITransportCombinedMap = new CEOSAITransportCombinedMap();
		pAITransportCombinedMap->SetTransportUnitTemplate( pTransportUnitTemplate );
		pAITransportCombinedMap->m_PathwayMap.SetArraySize( iNumberOfAIRegions+1 );
		m_TransportCombinedAIRegionMaps.AddTail( pAITransportCombinedMap );

		// Lookup all the transports of this type, lookup all the cities, construct a map
		POSITION pos2 = pMyActors->GetHeadPosition();
		while( pos2 )
		{
			CEOSAIPoiObject* pMyActor = pMyActors->GetNext( pos2 );
			CEOSAIUnit2* pTransportAIUnit = dynamic_cast< CEOSAIUnit2* >( pMyActor );
			if( pTransportAIUnit )
			{
				if( pTransportAIUnit->GetAIUnitTemplate() == pTransportUnitTemplate )
				{
					CEOSAIRegionPathwayMap* pTransportPathwayMap = pTransportAIUnit->InvokeDirectAIRegionMapToEverywhere();

					// Now iterate over the coastal locations, find the results
					for( long iRegion=0; iRegion<iNumberOfAIRegions; iRegion++ )
					{
						CEOSAIRegionPathwayItem* pTransportRegion = pTransportPathwayMap->GetRegionResult( iRegion );
						if( pTransportRegion )
						{
							CEOSAIRegionPathwayItem* pComboTransportRegion = pAITransportCombinedMap->m_PathwayMap.GetRegionResult( iRegion );
							if( pComboTransportRegion == NULL )
							{
								pComboTransportRegion = new CEOSAIRegionPathwayItem();
								pComboTransportRegion->m_pPathfinderPoint = pTransportRegion->m_pPathfinderPoint;
								pComboTransportRegion->m_fDanger = pTransportRegion->m_fDanger;
								pComboTransportRegion->m_fDistance = pTransportRegion->m_fDistance;
								pComboTransportRegion->m_fPathfindingValue = pTransportRegion->m_fPathfindingValue;
								pComboTransportRegion->m_fPresence = pTransportRegion->m_fPresence;
								pComboTransportRegion->m_fSightings = pTransportRegion->m_fSightings;
								pComboTransportRegion->m_fTime = pTransportRegion->m_fTime;
								pComboTransportRegion->m_iGeoSwitches = pTransportRegion->m_iGeoSwitches;
								pAITransportCombinedMap->m_PathwayMap.AddRegionPathwayItem( pComboTransportRegion );
							}
							else
							{
								pComboTransportRegion->m_fDanger = min( pComboTransportRegion->m_fDanger, pTransportRegion->m_fDanger );
								pComboTransportRegion->m_fDistance = min( pComboTransportRegion->m_fDistance, pTransportRegion->m_fDistance );
								pComboTransportRegion->m_fPathfindingValue = min( pComboTransportRegion->m_fPathfindingValue, pTransportRegion->m_fPathfindingValue );
								pComboTransportRegion->m_fPresence = min( pComboTransportRegion->m_fPresence, pTransportRegion->m_fPresence );
								pComboTransportRegion->m_fSightings = min( pComboTransportRegion->m_fSightings, pTransportRegion->m_fSightings );
								pComboTransportRegion->m_fTime = min( pComboTransportRegion->m_fTime, pTransportRegion->m_fTime );
								pComboTransportRegion->m_iGeoSwitches = min( pComboTransportRegion->m_iGeoSwitches, pTransportRegion->m_iGeoSwitches );
							}
						}
					}
				}
			}
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pMyActor );
			if( pAICity )
			{
				CEOSAIRegionPathwayMap* pTransportPathwayMap = NULL;
				if( pTransportUnitTemplate->IsSeaUnit() )
				{
					pTransportPathwayMap = pAICity->InvokeWaterAIRegionMapToEverywhere();
				}
				if( pTransportUnitTemplate->IsAirUnit() )
				{
					pTransportPathwayMap = pAICity->InvokeAirAIRegionMapToEverywhere();
				}
				if( pTransportPathwayMap == NULL ) continue;
				if( pTransportUnitTemplate->GetMovementRate() <= 0.0f ) continue;

				float fTransportBuildTime = pAICity->GetTimeToBuild( pTransportUnitTemplate, true );

				// Now iterate over the coastal locations, find the results
				for( long iRegion=0; iRegion<iNumberOfAIRegions; iRegion++ )
				{
					CEOSAIRegionPathwayItem* pTransportRegion = pTransportPathwayMap->GetRegionResult( iRegion );
					if( pTransportRegion )
					{
						float fMoveTime = pTransportRegion->m_fDistance / pTransportUnitTemplate->GetMovementRate();
						CEOSAIRegionPathwayItem* pComboTransportRegion = pAITransportCombinedMap->m_PathwayMap.GetRegionResult( iRegion );
						if( pComboTransportRegion == NULL )
						{
							pComboTransportRegion = new CEOSAIRegionPathwayItem();
							pComboTransportRegion->m_pPathfinderPoint = pTransportRegion->m_pPathfinderPoint;
							pComboTransportRegion->m_fDanger = pTransportRegion->m_fDanger;
							pComboTransportRegion->m_fDistance = pTransportRegion->m_fDistance;
							pComboTransportRegion->m_fPathfindingValue = pTransportRegion->m_fPathfindingValue;
							pComboTransportRegion->m_fPresence = pTransportRegion->m_fPresence;
							pComboTransportRegion->m_fSightings = pTransportRegion->m_fSightings;
							pComboTransportRegion->m_fTime = fTransportBuildTime+fMoveTime;
							pComboTransportRegion->m_iGeoSwitches = pTransportRegion->m_iGeoSwitches;
							pAITransportCombinedMap->m_PathwayMap.AddRegionPathwayItem( pComboTransportRegion );
						}
						else
						{
							pComboTransportRegion->m_fDanger = min( pComboTransportRegion->m_fDanger, pTransportRegion->m_fDanger );
							pComboTransportRegion->m_fDistance = min( pComboTransportRegion->m_fDistance, pTransportRegion->m_fDistance );
							pComboTransportRegion->m_fPathfindingValue = min( pComboTransportRegion->m_fPathfindingValue, pTransportRegion->m_fPathfindingValue );
							pComboTransportRegion->m_fPresence = min( pComboTransportRegion->m_fPresence, pTransportRegion->m_fPresence );
							pComboTransportRegion->m_fSightings = min( pComboTransportRegion->m_fSightings, pTransportRegion->m_fSightings );
							pComboTransportRegion->m_fTime = min( pComboTransportRegion->m_fTime, fTransportBuildTime+fMoveTime );
							pComboTransportRegion->m_iGeoSwitches = min( pComboTransportRegion->m_iGeoSwitches, pTransportRegion->m_iGeoSwitches );
						}
					}
				}
			}
			//
		}
	}
}

void CEOSAIBrain::DeleteTransportCombinedAIRegionMaps()
{
	while( m_TransportCombinedAIRegionMaps.IsEmpty() == FALSE ){ delete m_TransportCombinedAIRegionMaps.RemoveHead(); }
}

void CEOSAIBrain::CreateTransportCombinedMultiRegionMaps()
{
	DeleteTransportCombinedMultiRegionMaps();
	//
	CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
	CList< CEOSAIPoiObject* >* pMyActors = GetAIThoughtDatabase()->GetMyActors();
	//long iNumberOfAIRegions = GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetAIRegionManager()->GetNumberOfGenericRegions();
	//long iNumberOfAIRegions = pTransportPathwayMap->GetArraySize();
	long iNumberOfMultiRegions = g_pEOSAICommonData->GetMultiRegionManager()->m_PathfinderPointArray.GetSize();
	POSITION pos = pUnitsICanBuildOrHave->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pTransportUnitTemplate = pUnitsICanBuildOrHave->m_List.GetNext( pos );
		if( pTransportUnitTemplate->IsTransport() == false ) continue;
		if( pTransportUnitTemplate->IsSeaUnit() == false &&
			pTransportUnitTemplate->IsAirUnit() == false ) continue;

		CEOSAITransportCombinedMap* pAITransportCombinedMap = new CEOSAITransportCombinedMap();
		pAITransportCombinedMap->SetTransportUnitTemplate( pTransportUnitTemplate );
		pAITransportCombinedMap->m_PathwayMap.SetArraySize( iNumberOfMultiRegions+1 );
		m_TransportCombinedMultiRegionMaps.AddTail( pAITransportCombinedMap );

		// Lookup all the transports of this type, lookup all the cities, construct a map
		POSITION pos2 = pMyActors->GetHeadPosition();
		while( pos2 )
		{
			CEOSAIPoiObject* pMyActor = pMyActors->GetNext( pos2 );
			CEOSAIUnit2* pTransportAIUnit = dynamic_cast< CEOSAIUnit2* >( pMyActor );
			if( pTransportAIUnit )//false )//pTransportAIUnit )
			{
				if( pTransportAIUnit->GetAIUnitTemplate() == pTransportUnitTemplate )
				{
					CEOSAIRegionPathwayMap* pTransportPathwayMap = pTransportAIUnit->InvokeDirectMultiRegionMapToEverywhere();

					// Now iterate over the coastal locations, find the results
					for( long iRegion=0; iRegion<iNumberOfMultiRegions; iRegion++ )
					{
						CEOSAIRegionPathwayItem* pTransportRegion = pTransportPathwayMap->GetRegionResult( iRegion );
						if( pTransportRegion )
						{
							CEOSAIRegionPathwayItem* pComboTransportRegion = pAITransportCombinedMap->m_PathwayMap.GetRegionResult( iRegion );
							if( pComboTransportRegion == NULL )
							{
								pComboTransportRegion = new CEOSAIRegionPathwayItem();
								pComboTransportRegion->m_pPathfinderPoint = pTransportRegion->m_pPathfinderPoint;
								pComboTransportRegion->m_fDanger = pTransportRegion->m_fDanger;
								pComboTransportRegion->m_fDistance = pTransportRegion->m_fDistance;
								pComboTransportRegion->m_fPathfindingValue = pTransportRegion->m_fPathfindingValue;
								pComboTransportRegion->m_fPresence = pTransportRegion->m_fPresence;
								pComboTransportRegion->m_fSightings = pTransportRegion->m_fSightings;
								pComboTransportRegion->m_fTime = pTransportRegion->m_fTime;
								pComboTransportRegion->m_iGeoSwitches = pTransportRegion->m_iGeoSwitches;
								pAITransportCombinedMap->m_PathwayMap.AddRegionPathwayItem( pComboTransportRegion );
							}
							else
							{
								pComboTransportRegion->m_fDanger = min( pComboTransportRegion->m_fDanger, pTransportRegion->m_fDanger );
								pComboTransportRegion->m_fDistance = min( pComboTransportRegion->m_fDistance, pTransportRegion->m_fDistance );
								pComboTransportRegion->m_fPathfindingValue = min( pComboTransportRegion->m_fPathfindingValue, pTransportRegion->m_fPathfindingValue );
								pComboTransportRegion->m_fPresence = min( pComboTransportRegion->m_fPresence, pTransportRegion->m_fPresence );
								pComboTransportRegion->m_fSightings = min( pComboTransportRegion->m_fSightings, pTransportRegion->m_fSightings );
								pComboTransportRegion->m_fTime = min( pComboTransportRegion->m_fTime, pTransportRegion->m_fTime );
								pComboTransportRegion->m_iGeoSwitches = min( pComboTransportRegion->m_iGeoSwitches, pTransportRegion->m_iGeoSwitches );
							}
						}
					}
				}
			}
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pMyActor );
			if( pAICity )
			{
				CEOSAIRegionPathwayMap* pTransportPathwayMap = NULL;
				if( pTransportUnitTemplate->IsSeaUnit() )
				{
					pTransportPathwayMap = pAICity->InvokeWaterMultiRegionMapToEverywhere();
				}
				if( pTransportUnitTemplate->IsAirUnit() )
				{
					pTransportPathwayMap = pAICity->InvokeAirMultiRegionMapToEverywhere();
				}
				if( pTransportPathwayMap == NULL ) continue;
				if( pTransportUnitTemplate->GetMovementRate() <= 0.0f ) continue;

				float fTransportBuildTime = pAICity->GetTimeToBuild( pTransportUnitTemplate, true );

				// Now iterate over the coastal locations, find the results
				for( long iRegion=0; iRegion<iNumberOfMultiRegions; iRegion++ )
				{
					CEOSAIRegionPathwayItem* pTransportRegion = pTransportPathwayMap->GetRegionResult( iRegion );
					if( pTransportRegion )
					{
						float fMoveTime = pTransportRegion->m_fDistance / pTransportUnitTemplate->GetMovementRate();
						CEOSAIRegionPathwayItem* pComboTransportRegion = pAITransportCombinedMap->m_PathwayMap.GetRegionResult( iRegion );
						if( pComboTransportRegion == NULL )
						{
							pComboTransportRegion = new CEOSAIRegionPathwayItem();
							pComboTransportRegion->m_pPathfinderPoint = pTransportRegion->m_pPathfinderPoint;
							pComboTransportRegion->m_fDanger = pTransportRegion->m_fDanger;
							pComboTransportRegion->m_fDistance = pTransportRegion->m_fDistance;
							pComboTransportRegion->m_fPathfindingValue = pTransportRegion->m_fPathfindingValue;
							pComboTransportRegion->m_fPresence = pTransportRegion->m_fPresence;
							pComboTransportRegion->m_fSightings = pTransportRegion->m_fSightings;
							pComboTransportRegion->m_fTime = fTransportBuildTime+fMoveTime;
							pComboTransportRegion->m_iGeoSwitches = pTransportRegion->m_iGeoSwitches;
							pAITransportCombinedMap->m_PathwayMap.AddRegionPathwayItem( pComboTransportRegion );
						}
						else
						{
							pComboTransportRegion->m_fDanger = min( pComboTransportRegion->m_fDanger, pTransportRegion->m_fDanger );
							pComboTransportRegion->m_fDistance = min( pComboTransportRegion->m_fDistance, pTransportRegion->m_fDistance );
							pComboTransportRegion->m_fPathfindingValue = min( pComboTransportRegion->m_fPathfindingValue, pTransportRegion->m_fPathfindingValue );
							pComboTransportRegion->m_fPresence = min( pComboTransportRegion->m_fPresence, pTransportRegion->m_fPresence );
							pComboTransportRegion->m_fSightings = min( pComboTransportRegion->m_fSightings, pTransportRegion->m_fSightings );
							pComboTransportRegion->m_fTime = min( pComboTransportRegion->m_fTime, fTransportBuildTime+fMoveTime );
							pComboTransportRegion->m_iGeoSwitches = min( pComboTransportRegion->m_iGeoSwitches, pTransportRegion->m_iGeoSwitches );
						}
					}
				}
			}
			//
		}
	}
}

void CEOSAIBrain::DeleteTransportCombinedMultiRegionMaps()
{
	while( m_TransportCombinedMultiRegionMaps.IsEmpty() == FALSE ){ delete m_TransportCombinedMultiRegionMaps.RemoveHead(); }
}

long CEOSAIBrain::SendTradeOffersAndNegotiations()
{
	/*
	long iNumberOfOtherPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers() - 1;


	// The AI will purchase sufficient resources
	//   (Note: "Sufficient" resources is dependent on what the AI decides to do.
	//    if the AI moves a lot of units, he needs more oil.  If he builds
	//    military units, he needs to buy more iron.)
	float fNeedToBuyIron = 0.0f;
	if( GetWorldDescPlayerProxy()->GetTotalIron() < GetWorldDescPlayerProxy()->GetDeltaIron() )
	{
		fNeedToBuyIron = GetWorldDescPlayerProxy()->GetDeltaIron() - GetWorldDescPlayerProxy()->GetTotalIron();
		fNeedToBuyIron += 2.0f;  // We want to round-up and buy a little extra
	}
	float fNeedToBuyOil = 0.0f;
	if( GetWorldDescPlayerProxy()->GetTotalOil() < GetWorldDescPlayerProxy()->GetDeltaOil() )
	{
		fNeedToBuyOil = GetWorldDescPlayerProxy()->GetDeltaOil() - GetWorldDescPlayerProxy()->GetTotalOil();
		fNeedToBuyOil += 2.0f;  // We want to round-up and buy a little extra
	}
	float fNeedToBuyFood = 0.0f;
	if( GetWorldDescPlayerProxy()->GetTotalFood() < GetWorldDescPlayerProxy()->GetDeltaFood() )
	{
		fNeedToBuyFood = GetWorldDescPlayerProxy()->GetDeltaFood() - GetWorldDescPlayerProxy()->GetTotalFood();
		fNeedToBuyFood += 2.0f;  // We want to round-up and buy a little extra
	}

	if( fNeedToBuyIron > 1.0f || fNeedToBuyOil > 0.0f || fNeedToBuyFood > 0.0f )
	{
		// Grab the data, stuff it into an InternalMail object, send it out.
		CGameEvent_IMail* pMail = new CGameEvent_IMail();

		CPlayer* pPlayer = GetCommonState()->GetPlayer( GetAIPlayerNumber() );
		ASSERT( pPlayer );

		pMail->SetSender( m_pAIPlayer->GetPlayer()->GetPlayerNumber() );
		pMail->AddSendTo( 0 );
		pMail->SetSubject( "" );
		pMail->SetToCurrentDateAndTurn();
		pMail->SetMessage( "" );

		// Trade Agreement
		pMail->SetTradeAgreement( new CTradeAgreement() );
		pMail->GetTradeAgreement()->m_TradeAgreementState = EOSAIEnumTradeAgreementState_BrandNew;
		pMail->GetTradeAgreement()->m_iPlayer[0] = m_pAIPlayer->GetPlayer()->GetPlayerNumber();
		pMail->GetTradeAgreement()->m_iPlayer[1] = 0;
		pMail->GetTradeAgreement()->m_iPlayerWhoMadeTheOffer = m_pAIPlayer->GetPlayer()->GetPlayerNumber();

		pMail->GetTradeAgreement()->m_iPlayerFoodLumpSum[1] = fNeedToBuyFood;
		pMail->GetTradeAgreement()->m_iPlayerIronLumpSum[1] = fNeedToBuyIron;
		pMail->GetTradeAgreement()->m_iPlayerOilLumpSum[1]  = fNeedToBuyOil;
		pMail->GetTradeAgreement()->AutoEqualize();

		ASSERT( false ); // This probably doesn't work
		if( pMail->GetTradeAgreement()->m_TradeAgreementState == EOSAIEnumTradeAgreementState_BrandNew )
		{
			//pMail->GetTradeAgreement()->m_iTradeAgreementId = m_pAIPlayer->GetWorldDescPlayer()->GetNextTradeAgreementId();
			pMail->GetTradeAgreement()->m_strTradeAgreementId = m_pAIPlayer->GetWorldDescServer()->GetNextTradeAgreementId();
			m_pAIPlayer->GetWorldDescServer()->AddTradeAgreement( pMail->GetTradeAgreement() );
		}

		pMail->m_ShouldBeKnownBy.SetValue( 0, true ); // Send to server
		pMail->m_ShouldBeKnownBy.SetValue( pPlayer->GetPlayerNumber(), true );
		pMail->HasBeenSent( pPlayer->GetPlayerNumber(), true );
		pMail->SetGameTime( GetWorldDescServer()->GetGameTime() );
		pMail->AttachEvent( GetWorldDescServer() );
		pMail->SendEventToShouldBeKnownByList( m_pAIPlayer->GetPlayer()->GetMessageTargetId() );

		pMail->GetTradeAgreement()->m_TradeAgreementState = EOSAIEnumTradeAgreementState_WaitingForApproval;
		/*
		if( pMail->GetTradeAgreement()->m_TradeAgreementState == EOSAIEnumTradeAgreementState_BrandNew )
		{
			//pMail->GetTradeAgreement()->m_iTradeAgreementId = m_pAIPlayer->GetWorldDescPlayer()->GetNextTradeAgreementId();
			pMail->GetTradeAgreement()->m_strTradeAgreementId = m_pAIPlayer->GetWorldDescServer()->GetNextTradeAgreementId();
			m_pAIPlayer->GetWorldDescServer()->AddTradeAgreement( pMail->GetTradeAgreement() );
		}

		pMail->m_ShouldBeKnownBy.SetValue( 0, true ); // Send to server
		pMail->m_ShouldBeKnownBy.SetValue( pPlayer->GetPlayerNumber(), true );
		pMail->HasBeenSent( pPlayer->GetPlayerNumber(), true );
		pMail->SetGameTime( g_pGameAppState->GetWorldDesc()->GetGameTime() );
		//g_pGameAppState->GetWorldDesc()->AttachNationalEvent( pMail );
		pMail->AttachEvent( g_pGameAppState->GetWorldDesc() );
		pMail->SendEventToShouldBeKnownByList( m_pAIPlayer->GetPlayer()->GetMessageTargetId() );

		pMail->GetTradeAgreement()->m_TradeAgreementState = EOSAIEnumTradeAgreementState_WaitingForApproval;
		*-/

		/*
		// Send it
		CMessage2_InternalMail InternalMailMessage;
		//InternalMailMessage.InitializeWithMessageIds( 
		//	pPlayer->GetMessageTargetId(), g_pMessageTargetManager->GetMessageTarget_ServerId(), pMail );
		//InternalMailMessage.SetSentBy( pPlayer->GetMessageTargetName() );
		InternalMailMessage.SetSenderId( pPlayer->GetMessageTargetId() );
		InternalMailMessage.AddSendTo( g_pMessageTargetManager->GetMessageTarget_ServerId() );
		InternalMailMessage.SetInternalMail( pMail );
		//InternalMailMessage.SetSenderId(  );
		//InternalMailMessage.AddReceiverId(  );
		//InternalMailMessage.SetInternalMail( pMail );
		g_pMessageManager->Send( InternalMailMessage );

		if( pMail->m_pTradeAgreement )
		{
			pMail->m_pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_WaitingForApproval;
		}

		// Record the message in the "Sent Messages" list
		//CWorldDescPlayer* pWorldDescPlayer = g_pGameAppState->GetCurrentWorldDesc();
		//m_pAIPlayer->GetWorldDescPlayer()->AddSentMail( pMail );
		*-/

		// If the communication dialog is open, add it to that list
		/*
		if( g_pCommunicationDialog )
		{
			pMail->m_bHasBeenRead = true;
			//g_pCommunicationDialog->AddSentMail( pMail );
		}
		*-/
	}
*/
	/*
	if( FALSE )
	{
		CAIUnresolvedSituation_ForeignRelationPlan* pUnresolvedSituation = 
			new CAIUnresolvedSituation_ForeignRelationPlan( m_pAIPlayer );
		m_pAIPlayer->GetAIBrain()->AddToUnresolvedForeignRelationsOffers( pUnresolvedSituation );

		// Send messages to other players

		// Make peace with some players?
		//POSITION pos = m_PlayersWhoMightBeWillingToMakePeace.GetList()->GetHeadPosition();
		//while( pos )
		{
			long iSendTo = 1; //m_PlayersWhoMightBeWillingToMakePeace.GetList()->GetNext( pos );

			CPlayer* pPlayer = GetCommonState()->GetPlayer( GetAIPlayerNumber() );
			ASSERT( pPlayer );

			CInternalMail* pMail = new CInternalMail();
			pMail->InitializeWithPlayerNumbers( GetAIPlayerNumber(), iSendTo,
				"55xDo you want to sign a peace agreement?", 
				"55x[This is a test message]\r\nDo you want to call a temporary peace agreement?  Maybe for 25 turns or something?\r\n[This is a test message]" );

			pMail->m_pTradeAgreement = new CTradeAgreement();
			pMail->m_pTradeAgreement->InitializeWithPlayerNumbers(
				GetAIPlayerNumber(), iSendTo, m_pAIPlayer->GetWorldDescPlayer()->GetNextTradeAgreementId() );

			pMail->m_pTradeAgreement->m_bAllowPlayerToCounteroffer = true;
			//pMail->m_pTradeAgreement->m_bThisTradeOfferShouldExpireExistingTradeOffers = false;
			pMail->m_pTradeAgreement->m_bThisTradeOfferMustBeExplicitlyExpired = true;
			pMail->m_pTradeAgreement->m_bPeaceTreaty = true;
			//pMail->m_pTradeAgreement->m_AgreementToAttackPlayer[1].Add( 1 );
			//pMail->m_pTradeAgreement->m_AgreementToAttackPlayer[1].Add( 2 );
			//pMail->m_pTradeAgreement->m_iPlayerFoodLumpSum[1] = 1.0f;

			pUnresolvedSituation->m_TradeAgreementsInvolved.Add( pMail->m_pTradeAgreement->m_iTradeAgreementId );

			// Send it to the server
			CMessage2_InternalMail InternalMailMessage;
			//InternalMailMessage.InitializeWithMessageIds( pPlayer->GetMessageTargetId(), g_pMessageTargetManager->GetMessageTarget_ServerId(), pMail );
			//InternalMailMessage.SetSentBy( pPlayer->GetMessageTargetName() );
			InternalMailMessage.SetSenderId( pPlayer->GetMessageTargetId() );
			InternalMailMessage.AddSendTo( g_pMessageTargetManager->GetMessageTarget_ServerId() );
			InternalMailMessage.SetInternalMail( pMail );
			g_pMessageManager->Send( InternalMailMessage );

			pMail->m_pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_WaitingForApproval;
			m_pAIPlayer->GetWorldDescPlayer()->AddTradeAgreement( pMail->m_pTradeAgreement );
		}
	}
	*/


	// If there is more than 1 other player, send some messages out to "feel-out" player attitudes.
	//   The most positive ones will probably make the best allies.
	/*
	float fTotalFeeling = 0.0f;

	//std::list< CEOSAINationalSummary* >::iterator iter;
	for( iter=m_NationalSummaries.begin(); iter!=m_NationalSummaries.end(); ++iter )
	{
		//CEOSAINationalSummary* pViewOfPlayer = *iter;
		if( pViewOfPlayer->m_iPlayer != m_pAIPlayer->GetPlayer()->GetPlayerNumber() )
		{
			//fTotalFeeling += pViewOfPlayer->m_fFeelingsTowardPlayer;
		}
	}
	fTotalFeeling /= iNumberOfOtherPlayers;
	*/

	// Send a request
	//   INCOMPLETE: only send a request if the player is still active
/*
	CForeignProposal_NonAggressionPact* pProposal = new CForeignProposal_NonAgressionPact();
	pProposal->Initialize( 
		m_pAIPlayer->GetWorldDesc()->GetCurrentTurn(), 10.0f, 
		m_pAIPlayer->GetPlayer()->GetPlayerNumber(), 1 );
	m_pAIPlayer->AddToPendingProposals( pProposal );
*/
	/*
	CForeignProposal_GangupOnPlayer* pProposal = new CForeignProposal_GangupOnPlayer();
	pProposal->Initialize( 
		m_pAIPlayer->GetPlayer()->GetPlayerNumber(), 1, 2 );
	m_pAIPlayer->AddToPendingProposals( pProposal );
	*/

/*
	if( m_pAIPlayer->GetWorldDescPlayer()->GetCurrentTurn() == 1 )
	{
		CInternalMail* pMail = new CInternalMail();
		long iSendTo = 1;
		if( m_pAIPlayer->GetPlayer()->GetPlayerNumber() == 1 )
		{
			iSendTo = 2; // send it to player 2 if I am player 1
		}
		pMail->Initialize( iSendTo, m_pAIPlayer->GetPlayer()->GetPlayerNumber(), "Do you want to sign a peace agreement?", "[This is a test message]\r\nDo you want to call a temporary peace agreement?  Maybe for 25 turns or something?\r\n[This is a test message]" );
		pMail->AssignCurrentTime();

		// Send it
		CMessage2_InternalMail InternalMailMessage;
		InternalMailMessage.SetSenderId( pMail->m_iFromMessageId );
		InternalMailMessage.SetReceiverSet( &pMail->m_ToMessageIds );
		InternalMailMessage.SetInternalMail( pMail );
		g_pMessageManager->Send( InternalMailMessage );
	}
*/

/*
	CForeignProposal_FreeForm* pProposal = new CForeignProposal_FreeForm();
	CString strText = "I think we should gang-up on Joe.  He's more powerful than either of us, so he's going to win unless we band together.  What do you think?";
	pProposal->Initialize( m_pAIPlayer->GetPlayer()->GetPlayerNumber(), 1, strText );
	pProposal->AddOption( 0, "Yes, I agree.  Let's work together." );
	pProposal->AddOption( 1, "I guess not.  Maybe a little later." );
	pProposal->AddOption( 2, "Not a chance.  He's my ally, and we're coming for you." );
	pProposal->SetId( m_pAIPlayer->GetWorldDesc()->GetNextForeignProposalId() );
	//pProposal->ShowResponseEditBox( true );
	m_pAIPlayer->AddToPendingProposals( pProposal );

	CPlayer* pPlayer = GetCommonState()->GetPlayer( pProposal->GetReceivingPlayer() );

	CMessage2_ForeignProposal ProposalMessage;
	ProposalMessage.SetSenderId( m_pAIPlayer->GetPlayer()->GetMessageTargetId() );
	ProposalMessage.AddReceiverId( pPlayer->GetMessageTargetId() );
	ProposalMessage.SetProposal( pProposal );
	g_pMessageManager->Send( ProposalMessage );

	// How does the AI want to play nations against each other?
	//   Maybe the AI can ally with one player and gang-up on another player
	//     Early in the game, this could be used to slow-down both players 
	//       (winning territory by combat is more expensive than capturing unowned territory)
	//     Later in the game, if the AI doesn't join in the fight, he might lose out in the spoils
	//       of war unless he actually joins in the fighting.

	// Feel-out player attitudes towards me
	// Consider asking for a temporary non-aggression pact (e.g. 20 turns)

/*
	// DEBUG: text message
	{
		CPlayer* pTargetPlayer = GetCommonState()->GetPlayer( 1 );
		CEOSAIIntSet RecievingPlayers( pTargetPlayer->GetMessageTargetId() );
		CString strText = "Heyyyy";
		g_pMessageManager->SendTextMessageToOtherPlayer( 
			m_pAIPlayer->GetPlayer()->GetMessageTargetId(),
			&RecievingPlayers,
			strText );
	}
*/
	return 0;

	// Make allies, trade deals, etc.
	/*
	CEOSAIIntSet RecievingPlayers( pTargetPlayer->GetMessageTargetId() );
	CString strText = "Heyyyy";
	g_pMessageManager->SendTextMessageToOtherPlayer( 
		m_pAIPlayer->GetPlayer()->GetMessageTargetId(),
		&RecievingPlayers,
		strText );
	*/
		//m_pAIPlayer->GetPlayer()->GetMessageTargetId(), 1, strText );
}


//void CEOSAIBrain::CreateAIThoughtDatabaseData()
void CEOSAIBrain::CreateInitialTurnData_AIPoiObjects_BuildOptions_NationalSummaries()
{
	long iMyPlayerNumber = m_pAIPlayer->GetPlayerNumber();

	//
	// Initial Turn processing processes the "static" information that won't need to be recalculated
	//   if the state of the world changes (due to emails, declarations of war, or something)
	//
	if( m_bProcessInitialTurnState )
	{
		m_bProcessInitialTurnState = false;


		/*
		m_fPercentageOfWorldDiscovered = m_pWorldDescPlayer->CalculatePercentageOfTheWorldDiscovered();
		*/
		/*
		if( m_bAllowAIToCheat == false )
		{
			// Expansion is assumed to be 10000 pixels per turn for all players ( this should be a function based on starting conditions and increases exponentially )
			//     At this rate of expansion, it takes 100 turns to expand into a 1000x1000 sized map.
			//     Note: this calculation works best when players start on opposite sides of the map.  It works poorly when the start next to each other.
			float fExpansionPerTurn = 10000.0f;
			float fAssumedTotalExpansion = ( fExpansionPerTurn * GetWorldDescServer()->GetCurrentTurn() * GetCommonState()->GetNumberOfPlayers() - 1 );
			float fTotalMapSize = GetWorldDescServer()->GetWorldBuildDesc()->GetPixelHeight() * GetWorldDescServer()->GetWorldBuildDesc()->GetPixelWidth();
			float fPercentAssumedOtherExpansion = fAssumedTotalExpansion / fTotalMapSize;
			if( fPercentAssumedOtherExpansion > 1.0f ) fPercentAssumedOtherExpansion = 1.0f;

			float fPercentAssumedTotalExpansion = ( m_fPercentageOfWorldDiscovered + fPercentAssumedOtherExpansion ) / 2.0f;
			if( fPercentAssumedTotalExpansion > 1.0f ) fPercentAssumedTotalExpansion = 1.0f;
			m_fPercentAssumedTotalExpansion = fPercentAssumedTotalExpansion;
		}
		*/

		//
		//if( Public::m_bDoingTesting )
		//{
		//	g_pGameAppState->GetMapWindow()->GetMapTextOverlay()->AddString( iMyPlayerNumber, enum_IconInfo, _T("CEOSAIBrain::Process() - begin") );
		//}

		//
		// Build AI representations of the map, poi, production options
		//
			//m_AIThoughtDatabase.CreateAIUnitTemplatesForAllUnitTemplates();
			m_AIThoughtDatabase.CalculateUnitsICanBuildOrHave();
			m_AIThoughtDatabase.CalculatePotentialEnemyUnits();
			//m_AIThoughtDatabase.CalculateCombatAndAttritionValues();
			CreateAIProductionOptions();

			// Delete objects
			// Don't delete regions - they get deleted by the spatial map
			/*
			while( m_AIObjectList.IsEmpty() == FALSE )
			{
				CAIObject* pObject = m_AIObjectList.RemoveHead();
				ASSERT( dynamic_cast< CEOSAIRegion2* >( pObject ) == NULL );
				if( dynamic_cast< CEOSAIRegion2* >( pObject ) == NULL ){ delete pObject; }
			}
			m_AIObjectList.RemoveAll();
			*/

			//TurnPoiObjectsIntoAIObjects(); // Requires the existence of AIRegions, MultiRegions
			m_AIThoughtDatabase.CompileMyActorsIntoCategories(); // Requires AIObjects


/*
		CMovementPathDetails* pMovementPathDetails;
		m_MovementManager.SetAIBrain( this );
		m_MovementManager.Clear();
		m_MovementManager.SetStart( EnumStartMultiRegions_MyCitiesAndCityHunters );
		//m_MovementManager.SetStart( EnumStartMultiRegions_MyCitiesAirfieldsCarriers );
		//m_MovementManager.SetStart( EnumStartMultiRegions_MyCities );
		//m_MovementManager.SetStart( EnumStartMultiRegions_MyCities );
		//m_MovementManager.SetEndMultiRegion( 22 ); // 22 = water, upper left corner of the map
		m_MovementManager.SetEndMultiRegion( 52 ); // 52 = land, upper left corner of the map
		//pMovementPathDetails = m_MovementManager.GetMovementPathDetails( EOSAIEnumMovementType3_Water );
		pMovementPathDetails = m_MovementManager.GetMovementPathDetails( EOSAIEnumMovementType3_Land );
		if( pMovementPathDetails->m_eMovementType3 == EOSAIEnumMovementType3_Water )
		{
			bool bIsValid = pMovementPathDetails->PathwayExists();
			float fDistance = pMovementPathDetails->m_fDirectAirRange;
			int x=0;
		}
		if( pMovementPathDetails->m_eMovementType3 == EOSAIEnumMovementType3_Land )
		{
			bool bIsValid = pMovementPathDetails->PathwayExists();
			bool bReqTrans = pMovementPathDetails->RequiresTransport();
			float fDistance = pMovementPathDetails->m_fDirectAirRange;

			CMovementPath* pMovementPath1 = pMovementPathDetails->GetShortestGroundPathway();
			CMovementPath* pMovementPath2 = pMovementPathDetails->GetShortestPathway();

			float fDiff = pMovementPathDetails->GetTransportValueBasedOnDistance();

			pMovementPathDetails->SetBestPath_DistanceMult( 1.0f );
			pMovementPathDetails->SetBestPath_SeaCombatMult( 1.0f );
			pMovementPathDetails->SetBestPath_GroundCombatMult( 1.0f );

			pMovementPathDetails->FindTransports();

			//float fCompareTransportPathToGroundPath = pMovementPathDetails->CompareTransportPathToGroundPath();

			//pMovementPathDetails->BestPath_DistanceMult( 1.0f );
			//pMovementPathDetails->BestPath_ShipCombatMult( 1.0f );
			//pMovementPathDetails->BestPath_GroundCombatMult( 1.0f );
			//CMovementPath* pBestPath = pMovementPathDetails->GetBestPath();
			int x=0;
		}
		if( pMovementPathDetails->m_eMovementType3 == EOSAIEnumMovementType3_Air )
		{
			bool bWithinRange = pMovementPathDetails->IsWithinRangeOfSomeOfMyAirUnits();
			float fDistance = pMovementPathDetails->m_fDirectAirRange;
			int x=0;
		}
*/
		// DEBUG
		if( EOSAISettings::m_bDoingTesting )
		{
			//for( long i=1; i<GetSpatialMap()->m_RegionArray.GetUpperBound(); i++ )
			for( long i=1; i<g_pEOSAICommonData->GetAIRegionManager()->m_AIRegionArray.GetSize(); i++ )
			{
				CEOSAIRegion2* pRegion = g_pEOSAICommonData->GetAIRegionManager()->m_AIRegionArray.GetAt( i );
				ASSERT( pRegion );
			}
			if( iMyPlayerNumber == 2 )
			{
				//g_pGameAppState->GetMapWindow()->GetMapTextOverlay()->AddString( 
				//	iMyPlayerNumber, enum_IconInfo, _T("CEOSAIBrain::Process() - 2") );
			}
		}

		// Evaluate my state (military power, production capacity), compare it to other nation's state
	//	m_ForeignRelationsSituation.CreateNationalSummaries( m_pAIPlayer );

		m_bProcessTurnBasedOnForeignRelations = true;
	}
}
/*
void CEOSAIBrain::GenerateAIGeos()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//ASSERT( pWorldDescServer->GetGeoList()->IsEmpty() == FALSE );
	//CAICommonData* pAICommonData = pWorldDescServer->GetAICommonData();

	//if( m_AIGeoArray.m_iSize > 0 ){ return; } // This only needs to be done once per game per AI

	long iHighestGeoNumber = 0;
	//POSITION pos = pWorldDescServer->GetGeoList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetGeoList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pGeo = g_pEOSAICommonData->GetGeoList()->GetNext( pos );
		if( pGeo->GetId() > iHighestGeoNumber ) iHighestGeoNumber = pGeo->GetId();
	}

	for( long iGeo=0; iGeo<m_AIGeoArray.m_iSize; iGeo++ )
	{
		// Clear-out the old data
		if( m_AIGeoArray[iGeo] )
		{
			delete m_AIGeoArray[iGeo];
			m_AIGeoArray[iGeo] = NULL;
		}
	}
	ASSERT( m_AIGeoArray.m_iSize == 0 || m_AIGeoArray.m_iSize == iHighestGeoNumber+1 );
	m_AIGeoArray.SetSize( iHighestGeoNumber+1 );
	for( long iGeo=0; iGeo<iHighestGeoNumber+1; iGeo++ )
	{
		if( iGeo == 0 )
		{
			m_AIGeoArray[iGeo] = NULL;
		}
		else
		{
			//ASSERT( m_AIGeoArray[iGeo] == NULL );
			m_AIGeoArray[iGeo] = new CEOSAIGeo( GetAIPlayer(), iGeo );//g_pEOSAICommonData->GetGeo( iGeo ) );
		}
	}
	// Iterate over the MultiRegions, add to the AIGeo's list of MultiRegions
	long iNumberOfMultiRegions = g_pEOSAICommonData->GetMultiRegionManager()->GetNumberOfMultiRegions();
	for( long iMultiRegion=1; iMultiRegion<=iNumberOfMultiRegions; iMultiRegion++ )
	{
		//CMultiRegion* pMultiRegion = pAICommonData->GetMultiRegionManager()->GetMultiRegionArray()->GetAt( i );
		CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( iMultiRegion );
		ASSERT( pMultiRegion );
		if( pMultiRegion )
		{
			long iGeo = pMultiRegion->m_pCenterRegion->m_iGeo;
			long iGeo2 = pMultiRegion->GetGeoId();
			ASSERT( iGeo == iGeo2 );
			if( iGeo == 0 )
			{
				ASSERT( pMultiRegion->m_pCenterRegion->m_iRegionSize == 1 );
				continue;
			}
			if( iGeo == 60 )
			{
				int g=0;
			}
			CEOSAIGeo* pAIGeo = GetAIGeo( iGeo );
			//CGeo*   pDebugGeo = g_pEOSAICommonData->GetGeo( iGeo );
			ASSERT( pAIGeo );
			//ASSERT( pDebugGeo );
			if( pAIGeo == NULL ) continue;
			//if( pAIGeo == NULL && pDebugGeo == NULL )
			//{
			//	ASSERT( pMultiRegion->m_pCenterRegion->m_iRegionSize == 1 );
			//}

			pAIGeo->AddMultiRegion( pMultiRegion );
		}
	}
	for( long iGeo=1; iGeo<iHighestGeoNumber+1; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = m_AIGeoArray[iGeo];
		pAIGeo->CalculateGeoDistanceCitResScore();
	}
	// Now, sort the AIGeos by GeoDistanceCitResScore
	// Then, assign them a rank based on that score
	//CEOSAIListSort< CEOSAIGeo*, CEOSAIGeo* >  LandGeosSortedByDistanceCitResScore;
	m_LandGeosSortedByDistanceCitResScore.RemoveAll();
	m_LandGeosSortedByDistanceCitResScore.SetSortFunction( SortByDistanceCitResScore );
	for( long iGeo=1; iGeo<iHighestGeoNumber+1; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = m_AIGeoArray[iGeo];
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Land )
		{
			m_LandGeosSortedByDistanceCitResScore.InsertSorted( pAIGeo );
		}
	}
	long iRank = 1;
	pos = m_LandGeosSortedByDistanceCitResScore.GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pAIGeo = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		pAIGeo->SetGeoDistanceCitResScoreRank( iRank );
		iRank++;
	}
}
*/

//void CEOSAIBrain::GenerateAIGeos()
void CEOSAIBrain::SortAIGeosByDistance()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//ASSERT( pWorldDescServer->GetGeoList()->IsEmpty() == FALSE );
	//CAICommonData* pAICommonData = pWorldDescServer->GetAICommonData();

	//if( m_AIGeoArray.m_iSize > 0 ){ return; } // This only needs to be done once per game per AI
/*
	long iHighestGeoNumber = 0;
	//POSITION pos = pWorldDescServer->GetGeoList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetGeoList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pGeo = g_pEOSAICommonData->GetGeoList()->GetNext( pos );
		if( pGeo->GetId() > iHighestGeoNumber ) iHighestGeoNumber = pGeo->GetId();
	}

	for( long iGeo=0; iGeo<m_AIGeoArray.m_iSize; iGeo++ )
	{
		// Clear-out the old data
		if( m_AIGeoArray[iGeo] )
		{
			delete m_AIGeoArray[iGeo];
			m_AIGeoArray[iGeo] = NULL;
		}
	}
	ASSERT( m_AIGeoArray.m_iSize == 0 || m_AIGeoArray.m_iSize == iHighestGeoNumber+1 );
	m_AIGeoArray.SetSize( iHighestGeoNumber+1 );
	for( long iGeo=0; iGeo<iHighestGeoNumber+1; iGeo++ )
	{
		if( iGeo == 0 )
		{
			m_AIGeoArray[iGeo] = NULL;
		}
		else
		{
			//ASSERT( m_AIGeoArray[iGeo] == NULL );
			m_AIGeoArray[iGeo] = new CEOSAIGeo( GetAIPlayer(), iGeo );//g_pEOSAICommonData->GetGeo( iGeo ) );
		}
	}
	// Iterate over the MultiRegions, add to the AIGeo's list of MultiRegions
	long iNumberOfMultiRegions = g_pEOSAICommonData->GetMultiRegionManager()->GetNumberOfMultiRegions();
	for( long iMultiRegion=1; iMultiRegion<=iNumberOfMultiRegions; iMultiRegion++ )
	{
		//CMultiRegion* pMultiRegion = pAICommonData->GetMultiRegionManager()->GetMultiRegionArray()->GetAt( i );
		CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( iMultiRegion );
		ASSERT( pMultiRegion );
		if( pMultiRegion )
		{
			long iGeo = pMultiRegion->m_pCenterRegion->m_iGeo;
			long iGeo2 = pMultiRegion->GetGeoId();
			ASSERT( iGeo == iGeo2 );
			if( iGeo == 0 )
			{
				ASSERT( pMultiRegion->m_pCenterRegion->m_iRegionSize == 1 );
				continue;
			}
			if( iGeo == 60 )
			{
				int g=0;
			}
			CEOSAIGeo* pAIGeo = GetAIGeo( iGeo );
			//CGeo*   pDebugGeo = g_pEOSAICommonData->GetGeo( iGeo );
			ASSERT( pAIGeo );
			//ASSERT( pDebugGeo );
			if( pAIGeo == NULL ) continue;
			//if( pAIGeo == NULL && pDebugGeo == NULL )
			//{
			//	ASSERT( pMultiRegion->m_pCenterRegion->m_iRegionSize == 1 );
			//}

			pAIGeo->AddMultiRegion( pMultiRegion );
		}
	}
*/
	long iHighestGeoNumber = g_pEOSAIInterface->GetAICommonData()->GetAIGeoArray()->m_iSize;
	for( long iGeo=1; iGeo<iHighestGeoNumber; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = g_pEOSAIInterface->GetAICommonData()->GetAIGeo( iGeo );
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( GetAIPlayerNumber() );
		pAIGeoView->CalculateGeoDistanceCitResScore();
	}
/*
	for( long iGeo=1; iGeo<iHighestGeoNumber+1; iGeo++ )
	{
		//CEOSAIGeo* pAIGeo = m_AIGeoArray[iGeo];
		//pAIGeo->CalculateGeoDistanceCitResScore();
		CEOSAIGeo* pAIGeo = g_pEOSAIInterface->GetAICommonData()->GetAIGeo( iGeo );
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( GetAIPlayerNumber() );
		pAIGeoView->CalculateGeoDistanceCitResScore();
	}
*/
	// Now, sort the AIGeos by GeoDistanceCitResScore
	// Then, assign them a rank based on that score
	//CEOSAIListSort< CEOSAIGeo*, CEOSAIGeo* >  LandGeosSortedByDistanceCitResScore;
	m_LandGeosSortedByDistanceCitResScore.RemoveAll();
	m_LandGeosSortedByDistanceCitResScore.SetSortFunction( SortByDistanceCitResScore );
	for( long iGeo=1; iGeo<iHighestGeoNumber; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = g_pEOSAIInterface->GetAICommonData()->GetAIGeo( iGeo );
	//for( long iGeo=1; iGeo<iHighestGeoNumber+1; iGeo++ )
	//{
	//	CEOSAIGeo* pAIGeo = g_pEOSAIInterface->GetAICommonData()->GetAIGeo( iGeo ); //m_AIGeoArray[iGeo];
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Land )
		{
			//m_LandGeosSortedByDistanceCitResScore.InsertSorted( pAIGeo );
			CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( GetAIPlayerNumber() );
			m_LandGeosSortedByDistanceCitResScore.InsertSorted( pAIGeoView );
		}
	}
	long iRank = 1;
	POSITION pos = m_LandGeosSortedByDistanceCitResScore.GetHeadPosition();
	while( pos )
	{
		//CEOSAIGeo* pAIGeo = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		//CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( GetAIPlayerNumber() );
		CEOSAIGeoView* pAIGeoView = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		//pAIGeo->SetGeoDistanceCitResScoreRank( iRank );
		pAIGeoView->SetGeoDistanceCitResScoreRank( iRank );
		iRank++;
	}
}

void CEOSAIBrain::CalculateAIGeoViewValues()
{
	long iGeoArraySize = g_pEOSAIInterface->GetAICommonData()->GetAIGeoArray()->m_iSize;
	for( long iGeo=1; iGeo<iGeoArraySize; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = g_pEOSAIInterface->GetAICommonData()->GetAIGeo( iGeo );
		CEOSAIGeoView* pAIGeoView = pAIGeo->CreateView( GetAIPlayerNumber() );

		pAIGeoView->CalculateAIGeoViewValues();
	}
}

void CEOSAIBrain::CreateAIGeoPlans()
{
	SortAIGeosByDistance();

	TRACE( _T("CEOSAIBrain::CreateAIGeoPlans() - Start") );
	//Checkpoint::Write( _T("CEOSAIBrain::CreateGeoPlans() - Start") );
	long iAIPlayer = GetAIPlayerNumber();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CAICommonData* pAICommonData = pWorldDescServer->GetAICommonData();
	CEOSAIMultiRegionNationwidePathways* pNationwidePathway = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );

	// Look over the geos, look at their distance from my nation, start making some strategies
	CEOSAIBCDumbArray1D<CEOSAIGeo*>* pAIGeoArray = g_pEOSAIInterface->GetAICommonData()->GetAIGeoArray();
	//for( long iGeo=1; iGeo<m_AIGeoArray.m_iSize; iGeo++ )
	for( long iGeo=1; iGeo<pAIGeoArray->m_iSize; iGeo++ )
	{
		if( iGeo == 18 )
		{
			int h=0;
		}

		CEOSAIGeo* pAIGeo = (*pAIGeoArray)[iGeo];
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );

		if( iGeo == 27 )
		{
			int h=0;
		}

		// Formulate a strategy for the AIGeo
		// How desireable is this geo?  How close is it?  What's its situation?

		// Is this AIGeo valuable?
		float fTotalCitResValue = pAIGeo->GetTotalCitResValue();

		// How accessible is this AIGeo?
		float fMyGroundSeaGeoAccess01 = 0.0f;
		float fMyAirGeoAccess01 = 0.0f;
		POSITION pos = pAIGeo->GetMultiRegions()->GetHeadPosition();
		while( pos )
		{
			CEOSAIMultiRegion2* pMultiRegion = pAIGeo->GetMultiRegions()->GetNext( pos );
			long iMultiRegion = pMultiRegion->GetId();

			CEOSAIMultiRegionAttackAccess* pAttackAccess = pNationwidePathway->GetMultiRegionAttackAccess( iMultiRegion );
			if( pAttackAccess )
			{
				float fGroundSeaAccess01 = pAttackAccess->GetGroundSeaUnitAccess01();
				float fAirAccess01 = pAttackAccess->GetAirUnitAccess01();
				fMyGroundSeaGeoAccess01 = max( fMyGroundSeaGeoAccess01, fGroundSeaAccess01 );
				fMyAirGeoAccess01 = max( fMyAirGeoAccess01, fAirAccess01 );
			}
		}
		pAIGeoView->SetMyGroundSeaUnitAccess01( fMyGroundSeaGeoAccess01 );
		pAIGeoView->SetMyAirUnitAccess01( fMyAirGeoAccess01 );
	}
	// Setup (Possible) Strategies
	int j=0;
	for( long iGeo=1; iGeo<pAIGeoArray->m_iSize; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = (*pAIGeoArray)[iGeo];
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );
		if( iGeo == 18 )
		{
			int h=0;
		}

		//if( pAIGeo->GetGeo()->GetGeoType() == CGeo::Impassible )
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Impassible )
		{
			pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Impassible );
		}
		//if( pAIGeo->GetGeo()->GetGeoType() == CGeo::Water )
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Water )
		{
			//long iGeoSize = pAIGeo->GetGeo()->GetSize();
			long iGeoSize = pAIGeo->GetSize();

			// Water strategy involves not only the Resources in the water, but also how the
			//   water connects geos together.  Large bodies of water need to be safe for traversal.
			bool bPoiExistInThisGeo = true;
			if( pAIGeo->GetAIPoiObjectList()->IsEmpty() ) bPoiExistInThisGeo = false;
			/*
			if( bPoiExistInThisGeo == false &&
				pAIGeo->GetGeo()->GetSize() < 1 )
			{
				pAIGeo->SetGeoSituation( EOSAIEnumGeoSituation_SmallWater_NoPoi );
				continue;
			}
			*/

			// Are there any poi in the water?  Does this geo connect to any other geos?
			//   If the answer to these questions is 'no' then I can ignore this water geo
			//CEOSAIIntSet* pConnectedSet = pAIGeo->GetGeo()->GetConnectedSet();
			CEOSAIIntSet* pConnectedSet = pAIGeo->GetConnectedSet();
			long iConnectedGeos = pConnectedSet->GetSize();

			if( iConnectedGeos == 1 && bPoiExistInThisGeo == false && iGeoSize < 40 )
			{
				pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Water_NoPoiNoPath );
			}
			eif( iGeoSize > 15000 )
			{
				// A 1000x1000 map has a total size of 62500
				pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Water_Large );
			}
			eif( bPoiExistInThisGeo )
			{
				pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Water_WithPoi );
			}
			else
			{
				pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Water_NoPoi );
			}
		}
		//if( pAIGeo->GetGeo()->GetGeoType() == CGeo::Road )
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Road )
		{
			pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_NoPoi );
		}
		//if( pAIGeo->GetGeo()->GetGeoType() == CGeo::Land )
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Land )
		{
			//bool bContainsPoi = true;
			//if( pAIGeo->GetPoiObjectList()->IsEmpty() ){ bContainsPoi = false; }
			if( pAIGeo->GetAIPoiObjectList()->IsEmpty() )
			{
				pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_NoPoi );
				continue;
			}
			ASSERT( pAIGeo->GetAIPoiObjectList()->IsEmpty() == FALSE );

			// I have no GroundSea access to this geo
			/*
			if( pAIGeo->GetGroundSeaUnitAccess01() == 0.0f &&
				pAIGeo->GetAirUnitAccess01() == 0.0f )
			{
				pAIGeo->SetGeoSituation( EOSAIEnumGeoPlan_Ignore_TooDistant );
				continue;
			}
			*/
			// There are no CitRes on this geo
			// Nothing of value here.  However, I might need to destroy some enemy units hanging around
			//if( pAIGeo->GetTotalCitResValue() == 0.0f )
			//{
			//	pAIGeo->SetGeoSituation( EOSAIEnumGeoPlan_Ignore_NoCitRes );
			//	continue;
			//}
			// There are some CitRes on this geo
			//if( pAIGeo->GetTotalCitResUnitValue() > 0.0f )

			{
				if( pAIGeoView->GetUnownedPoiObjects() > 0 &&
					pAIGeoView->GetSATPoiObjects() == 0 &&
					pAIGeoView->GetNeutralOrEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_Unowned );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() == 0 &&
					pAIGeoView->GetMyPoiObjects() >= 1 &&
					pAIGeoView->GetAlliesPoiObjects() == 0 &&
					pAIGeoView->GetTeamPoiObjects() == 0 &&
					pAIGeoView->GetNeutralOrEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_Mine );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() >= 1 &&
					pAIGeoView->GetMyPoiObjects() >= 1 &&
					pAIGeoView->GetAlliesPoiObjects() == 0 &&
					pAIGeoView->GetTeamPoiObjects() == 0 &&
					pAIGeoView->GetNeutralOrEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_MineUnowned );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() == 0 &&
					pAIGeoView->GetMyPoiObjects() == 0 &&
					pAIGeoView->GetATPoiObjects() >= 1 &&
					pAIGeoView->GetNeutralOrEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_AT );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() >= 1 &&
					pAIGeoView->GetMyPoiObjects() == 0 &&
					pAIGeoView->GetATPoiObjects() >= 1 &&
					pAIGeoView->GetNeutralOrEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_ATUnowned );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() == 0 &&
					pAIGeoView->GetSATPoiObjects() >= 1 &&
					pAIGeoView->GetNeutralOrEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_SAT );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() >= 1 &&
					pAIGeoView->GetSATPoiObjects() >= 1 &&
					pAIGeoView->GetNeutralOrEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_SATUnowned );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() == 0 &&
					pAIGeoView->GetSATPoiObjects() == 0 &&
					pAIGeoView->GetNeutralPoiObjects() >= 1 &&
					pAIGeoView->GetEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_Neutral );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() >= 1 &&
					pAIGeoView->GetSATPoiObjects() == 0 &&
					pAIGeoView->GetNeutralPoiObjects() >= 1 &&
					pAIGeoView->GetEnemyPoiObjects() == 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_NeutralUnowned );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() == 0 &&
					pAIGeoView->GetSATPoiObjects() == 0 &&
					pAIGeoView->GetNeutralPoiObjects() == 0 &&
					pAIGeoView->GetEnemyPoiObjects() >= 1 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_Enemy );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() >= 1 &&
					pAIGeoView->GetSATPoiObjects() == 0 &&
					pAIGeoView->GetNeutralPoiObjects() == 0 &&
					pAIGeoView->GetEnemyPoiObjects() >= 1 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_EnemyUnowned );
				}
				eif(pAIGeoView->GetUnownedPoiObjects() >= 0 &&
					pAIGeoView->GetSATPoiObjects() >= 0 &&
					pAIGeoView->GetNeutralOrEnemyPoiObjects() > 0 )
				{
					pAIGeoView->SetGeoSituation( EOSAIEnumGeoSituation_Land_Complex );
				}
				else
				{
					ASSERT( false );
				}

				// I control none of the CitRes on this geo
				/*
				if( pAIGeo->GetPlayerCitResControl01( iAIPlayer ) == 0.0f )
				{
					long iOtherPlayerPoiObjects = 0;
					POSITION pos = pAIGeo->GetPoiObjectList()->GetHeadPosition();
					while( pos )
					{
						CPoiObject* pPoiObject = pAIGeo->GetPoiObjectList()->GetNext( pos );
						if( pPoiObject->GetOwner() > 0 && pPoiObject->GetOwner() != iAIPlayer )
						{
							iOtherPlayerPoiObjects++;
						}
					}
					if( iOtherPlayerPoiObjects == 0 )
					{
						pAIGeo->SetSituation( EOSAIEnumGeoPlan_Land_Seed );
					}
					else
					{
						pAIGeo->SetSituation( EOSAIEnumGeoPlan_Land_InvadeGeo );
					}
				}
				*/
			}
		}
	}
	int h=0;
	for( long iGeo=1; iGeo<pAIGeoArray->m_iSize; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = (*pAIGeoArray)[iGeo];
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );
		ASSERT( pAIGeoView->GetGeoSituation() != EOSAIEnumGeoSituation_Undefined );
	}

	// Iterate over the Geos and categorize water geos as pathway geos

	// Create Plans for each Geo
	//long iAIPlayer = this->GetAIPlayerNumber();
	CList< CEOSAIGeo* >  AIGeosWithNoPlan;
	for( long iGeo=1; iGeo<pAIGeoArray->m_iSize; iGeo++ )
	{
		#ifdef _DEBUG
		if( iGeo == 27 )
		{
			int h=0;
		}
		#endif _DEBUG

		// Create a plan for each AIGeo
		//   Categorize each of them by proximity to danger, proximity to me/other players, value, 
		//   enemy power, access, etc
		CEOSAIGeo* pAIGeo = (*pAIGeoArray)[iGeo];
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );

		if( pAIGeo->GetGeoType() == CEOSAIGeo::Impassible )
		{
			pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Ignore );
		}
		else if( pAIGeo->GetGeoType() == CEOSAIGeo::Road )
		{
			pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Ignore );
		}
		else if( pAIGeo->GetGeoType() == CEOSAIGeo::Land )
		{
			// This is a land geo
			if( pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Mine )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_ProtectMyGeo );
			}
			eif( pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_Land_MineUnowned )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_Consolidate );
			}
			eif( pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_Land_NoPoi )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Ignore );
			}
			//eif(pAIGeo->GetMyGroundSeaUnitAccess01() > 0.0f || // Degraded by intervening CitRes
			eif(pAIGeoView->GetMyCitResDistance() < 1000.0f ||
				pAIGeoView->GetMyAirUnitAccess01() > 0.0f )
			{
				ASSERT( pAIGeo->GetAIPoiObjectList()->IsEmpty() == FALSE );
				//ASSERT( pAIGeo->GetMyPoiObjects() == 0 ); // this can happen with EOSAIEnumGeoSituation_Land_Complex

				// These land-geos are accessible, and have something of value
				//   I need to pair-down these results
				//   For example, some might be inaccessible because they are deep in enemy territory
				//   I also want to favor closer geos, and then ignore the more distant ones
				//     If I have a lot of jobs in my current geos (e.g. pangea), I might ignore all of them
				//   And, if something is really valuable (like a large island), then I can go out of my way

				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_NoPlanYet );
				AIGeosWithNoPlan.AddTail( pAIGeo );

				/*
				// I have no Poi on this geo
				if( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Unowned )
				{
					// The entire geo is unowned
					pAIGeo->SetPlan( EOSAIEnumGeoPlan_Land_Seed );
				}
				eif(pAIGeo->GetUnownedCitResCount() >= 1 )
				{
					// There are some unowned poi, but some other player is also here
					if( pAIGeo->GetAlliesPoiObjects() >= 1 ||
						pAIGeo->GetTeamPoiObjects() >= 1 )
					{
						pAIGeo->SetPlan( EOSAIEnumGeoPlan_Land_FriendsGeo );

						// Depending on the size of this geo, I may want to grab a chunk of this geo

					}
					eif(pAIGeo->GetNeutralPoiObjects() >= 1 ||
						pAIGeo->GetEnemyPoiObjects() >= 1 )
					{
						pAIGeo->SetPlan( EOSAIEnumGeoPlan_Land_InvadeGeo );

						// Depending on the size of this geo, I may want to grab a chunk of this geo
					}
					else
					{
						ASSERT( false );
					}
				}
				else
				{
					// There are no unowned poi, everything is owned
					if( pAIGeo->GetAlliesPoiObjects() >= 1 ||
						pAIGeo->GetTeamPoiObjects() >= 1 )
					{
						pAIGeo->SetPlan( EOSAIEnumGeoPlan_Land_FriendsGeo );
					}
					eif(pAIGeo->GetNeutralPoiObjects() >= 1 ||
						pAIGeo->GetEnemyPoiObjects() >= 1 )
					{
						pAIGeo->SetPlan( EOSAIEnumGeoPlan_Land_InvadeGeo );
					}
					else
					{
						ASSERT( false );
					}
				}
				*/
			}
			else
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_TooDistant );
			}
		}
		else
		{
			ASSERT( pAIGeo->GetGeoType() == CEOSAIGeo::Water );
			if( pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_SmallWater_NoPoi )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Ignore );
			}
			eif( pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_Water_NoPoiNoPath )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Ignore );
			}
			eif( pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_Water_Large )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Water_Complex );
			}
			eif( pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_Water_WithPoi )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Water_NoPlanYet );
				AIGeosWithNoPlan.AddTail( pAIGeo );
			}
			else
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Water_NoPlanYet );
				AIGeosWithNoPlan.AddTail( pAIGeo );
			}
		}
	}

	// The AI needs to 'spend' units on the plan (e.g. if the AI is on pangea, then other geos mean less)
	// Rank the Geos with military-effort (should I invade that geo?)
	// Rank the Geos with value (CitRes)
	// Rank the Geos with accessibility
	// Figure out if one of my geos is getting invaded - and oppose invasion
	// Start removing geos from the list of possibilities

	// Take the top 5 Land AIGeos with NoPlan no owned CitRes, and add it to the "to do" list, but
	//    only if they are reasonably nearby (don't add them if they're across the map)
	long iFound = 0;
	POSITION pos = m_LandGeosSortedByDistanceCitResScore.GetHeadPosition();
	while( pos )
	{
		//CEOSAIGeo* pAIGeo = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		//CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );
		CEOSAIGeoView* pAIGeoView = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		ASSERT( pAIGeoView->m_pAIGeo->GetGeoType() == CEOSAIGeo::Land );

		if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet &&
			pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Unowned )
		{
			//if( pAIGeo->GetGeoNumber() == 9 )
			//{
			//	int h=0;
			//}

			if( pAIGeoView->GetMyCitResDistance() < 250.0f || iFound < 5 )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_Seed );
				iFound++;
			}
			else
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_IgnoreLowRank );
			}
		}
	}
	// Add the possibility of capturing some unowned CitRes on a geo where Neutrals/Allies/Teammates are
	iFound = 0;
	pos = m_LandGeosSortedByDistanceCitResScore.GetHeadPosition();
	while( pos )
	{
		//CEOSAIGeo* pAIGeo = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		//CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );
		CEOSAIGeoView* pAIGeoView = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		CEOSAIGeo* pAIGeo = pAIGeoView->m_pAIGeo;
		ASSERT( pAIGeo->GetGeoType() == CEOSAIGeo::Land );

		if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet &&
			pAIGeo->GetUnownedCitResCount() >= 1 &&
			( pAIGeoView->GetNeutralPoiObjects() + pAIGeoView->GetAlliesPoiObjects() + pAIGeoView->GetTeamPoiObjects() >= 1 ) )
		{
			if( iFound < 3 )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_Seed );
				iFound++;
			}
			else
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_IgnoreLowRank );
			}
		}
	}
	// Add the possibility of invading geo where Neutrals/Enemies are
	iFound = 0;
	pos = m_LandGeosSortedByDistanceCitResScore.GetHeadPosition();
	while( pos )
	{
		//CEOSAIGeo* pAIGeo = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		//CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );
		CEOSAIGeoView* pAIGeoView = m_LandGeosSortedByDistanceCitResScore.GetNext( pos );
		CEOSAIGeo* pAIGeo = pAIGeoView->m_pAIGeo;
		ASSERT( pAIGeo->GetGeoType() == CEOSAIGeo::Land );
		//if( pAIGeo->GetGeo()->GetGeoNumber() == 27 )
		//{
		//	int h=0;
		//}

		if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet )
		{
			if( pAIGeoView->GetTeamPoiObjects() > 0 &&
				pAIGeoView->GetNeutralPoiObjects() == 0 &&
				pAIGeoView->GetEnemyPoiObjects() == 0 )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_FriendsGeo );
			}
		}
		if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet &&
			//pAIGeo->GetUnownedCitResCount() >= 1 &&
			//( pAIGeo->GetNeutralPoiObjects() + pAIGeo->GetEnemyPoiObjects() >= 1 ) )
			pAIGeoView->GetEnemyPoiObjects() >= 1 )
		{
			if( iFound <= 5 )
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_InvadeGeo );
				iFound++;
			}
			else
			{
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_IgnoreLowRank );
			}
		}
		if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet &&
			//pAIGeo->GetUnownedCitResCount() >= 1 &&
			//( pAIGeo->GetNeutralPoiObjects() + pAIGeo->GetEnemyPoiObjects() >= 1 ) )
			pAIGeoView->GetEnemyPoiObjects() == 0 )
		{
			pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_IgnoreLowRank );
		}
	}

	pos = AIGeosWithNoPlan.GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pAIGeo = AIGeosWithNoPlan.GetNext( pos );
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );
		ASSERT( pAIGeoView->GetPlan() != EOSAIEnumGeoPlan_Land_NoPlanYet );
	}

	pos = AIGeosWithNoPlan.GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pAIGeo = AIGeosWithNoPlan.GetNext( pos );
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Land )
		{
			ASSERT( pAIGeoView->GetPlan() != EOSAIEnumGeoPlan_Land_NoPlanYet );
			continue;
		}
		//if( pAIGeo->GetPlan() != EOSAIEnumGeoPlan_Land_NoPlanYet ) continue;
		ASSERT( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet ||
				pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Water_NoPlanYet );

		long  iTotalCitResCount = pAIGeo->GetTotalCitResCount();
		long  iUnownedCitResCount = pAIGeo->GetUnownedCitResCount();
		long  iOwnedCitResCount = iTotalCitResCount - iUnownedCitResCount;
		float fUnownedCitRes01 = (float)iUnownedCitResCount / (float)iTotalCitResCount;
		//float fMyCitResDistance = pAIGeo->GetMyCitResDistance();

		// I have no Poi on this geo
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Land )
		{
			if( pAIGeoView->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Unowned )
			{
				// The entire geo is unowned
				pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_Seed );
			}
			eif(pAIGeo->GetUnownedCitResCount() >= 1 )
			{
				// There are some unowned poi, but some other player is also here
				if( pAIGeoView->GetAlliesPoiObjects() >= 1 ||
					pAIGeoView->GetTeamPoiObjects() >= 1 )
				{
					pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_FriendsGeo );

					// Depending on the size of this geo, I may want to grab a chunk of this geo
//					if( 
				}
				eif(pAIGeoView->GetNeutralPoiObjects() >= 1 ||
					pAIGeoView->GetEnemyPoiObjects() >= 1 )
				{
					pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_InvadeGeo );

					// Depending on the size of this geo, I may want to grab a chunk of this geo
				}
				else
				{
					ASSERT( false );
				}
			}
			else
			{
				// There are no unowned poi, everything is owned
				if( pAIGeoView->GetAlliesPoiObjects() >= 1 ||
					pAIGeoView->GetTeamPoiObjects() >= 1 )
				{
					pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_FriendsGeo );
				}
				eif(pAIGeoView->GetNeutralPoiObjects() >= 1 ||
					pAIGeoView->GetEnemyPoiObjects() >= 1 )
				{
					pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Land_InvadeGeo );
				}
				else
				{
					ASSERT( false );
				}
			}
		}
		else
		{
			// This is water - maybe a very large body of water
			pAIGeoView->SetPlan( EOSAIEnumGeoPlan_Water_NoPlanYet );
		}
	}

	//
	//CEOSAIListSort< CEOSAIGeo* >  SortedGeos;

	int h2=0;
}

/*
void CEOSAIBrain::AddDesiresAsMapLabels()
{
	GetWorldDescServer()->DeleteMapLabels();
	CEOSAIThoughtDatabase* pAIThoughtDatabase = this->GetAIThoughtDatabase();
	//for( long iGeo=1; iGeo<m_AIGeoArray.m_iSize; iGeo++ )
	POSITION pos = pAIThoughtDatabase->GetDesires()->GetHeadPosition();
	while( pos )
	{
		CEOSAIDesire2* pDesire = pAIThoughtDatabase->GetDesires()->GetNext( pos );

		long iGridX = 0;
		long iGridY = 0;
		CString strText;

		float fSimpleInterest = pDesire->GetSimpleInterest();

		CString strDesireType;
		CEOSAIDesireSpatial* pSpatialDesire = dynamic_cast< CEOSAIDesireSpatial* >( pDesire );
		if( pSpatialDesire )
		{
			iGridX = pSpatialDesire->GetLocation().GetGridX();
			iGridY = pSpatialDesire->GetLocation().GetGridY();
			if( pSpatialDesire->GetType() == EnumCaptureCity ||
				pSpatialDesire->GetType() == EnumCaptureResource )
			{
				strDesireType = _T("Capture");
			}
		}
		else
		{
			ASSERT( false );
			continue;
		}

		strText.Format( _T("%s %1.1f"), strDesireType, fSimpleInterest );

		CMapLabel* pMapLabel = new CMapLabel();
		pMapLabel->m_strText = strText;
		pMapLabel->m_iSize = 2;
		pMapLabel->m_fOpacity01 = 0.8f;
		pMapLabel->m_Location.SetReal( 4*iGridX,4*iGridY );
		GetWorldDescServer()->AddMapLabel( pMapLabel );
	}
}

void CEOSAIBrain::AddAIGeoSituationsAsMapLabels()
{
	GetWorldDescServer()->DeleteMapLabels();

	for( long iGeo=1; iGeo<m_AIGeoArray.m_iSize; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = m_AIGeoArray[iGeo];
		if( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_SmallWater_NoPoi ) continue;
		//if( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_SmallLand_NoPoi ) continue;

		long iGridX = pAIGeo->GetGeo()->m_x;
		long iGridY = pAIGeo->GetGeo()->m_y;

		CString strText;
		CString strPlan;

		if( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Undefined ){ strPlan = _T("None"); }
		//
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Water_WithPoi ){ strPlan = _T("Water"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Water_Large ){ strPlan = _T("Water_Large"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Water_NoPoi ){ strPlan = _T("Water_NoPoi"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Water_NoPoiNoPath ){ strPlan = _T("Water_NoPoiNoPath"); }
		//
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_NoPoi ){ strPlan = _T("Land_NoPoi"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Unowned ){ strPlan = _T("Land_Unowned"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Mine ){ strPlan = _T("Land_Mine"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_MineUnowned ){ strPlan = _T("Land_MineUnowned"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_AT ){ strPlan = _T("Land_AT"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_ATUnowned ){ strPlan = _T("Land_ATUnowned"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_SAT ){ strPlan = _T("Land_SAT"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_SATUnowned ){ strPlan = _T("Land_SATUnowned"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Neutral ){ strPlan = _T("Land_Neutral"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_NeutralUnowned ){ strPlan = _T("Land_NeutralUnowned"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Enemy ){ strPlan = _T("Land_Enemy"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_EnemyUnowned ){ strPlan = _T("Land_EnemyUnowned"); }
		eif( pAIGeo->GetGeoSituation() == EOSAIEnumGeoSituation_Land_Complex ){ strPlan = _T("Land_Complex"); }
		else{ ASSERT( false ); }

		//strText.Format( _T("%d %s %1.0f"), pAIGeo->GetGeo()->GetGeoNumber(), strPlan, pAIGeo->GetMyCitResDistance() );
	//	strText.Format( _T("%d %s %1.0f/%1.0f"), 
	//		pAIGeo->GetGeo()->GetGeoNumber(), strPlan, 
	//		pAIGeo->GetMyCitResDistance(),
	//		pAIGeo->GetNeutralEnemyCitResDistance() );
		strText.Format( _T("%d %s (%d) %1.1f"), 
			pAIGeo->GetGeo()->GetGeoNumber(), strPlan, 
			pAIGeo->GetGeoDistanceCitResScoreRank(),
			pAIGeo->GetGeoDistanceCitResScore() );

		CMapLabel* pMapLabel = new CMapLabel();
		pMapLabel->m_strText = strText;
		pMapLabel->m_iSize = 2;
		pMapLabel->m_fOpacity01 = 0.8f;
		pMapLabel->m_Location.SetReal( 4*iGridX,4*iGridY );
		GetWorldDescServer()->AddMapLabel( pMapLabel );
	}
}

void CEOSAIBrain::AddAIGeoPlansAsMapLabels()
{
	GetWorldDescServer()->DeleteMapLabels();

	for( long iGeo=1; iGeo<m_AIGeoArray.m_iSize; iGeo++ )
	{
		CEOSAIGeo* pAIGeo = m_AIGeoArray[iGeo];

		long iGridX = pAIGeo->GetGeo()->m_x;
		long iGridY = pAIGeo->GetGeo()->m_y;

		CString strText;
		CString strPlan;
		if(  pAIGeo->GetPlan() == EOSAIEnumGeoPlan_NoPlan ){ strPlan = _T("NoPlan"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Ignore ){ strPlan = _T("Ignore"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_SmallGeo_NoCitResUnits ){ strPlan = _T("NoCitResUnits"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Ignore_NoCitResUnits ){ str = _T("NoPoi"); }
		//
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Water_NoPlanYet ){ strPlan = _T("NoPlanYet"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Water_Complex ){ strPlan = _T("WaterComplex"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Water_Submarine ){ str = _T("WaterSubmarine"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Water_Aggressive ){ str = _T("WaterAggressive"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Water_TotalControl ){ str = _T("WaterTotalControl"); }
		//
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet ){ strPlan = _T("NoPlanYet"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_IgnoreLowRank ){ strPlan = _T("IgnoreLowRank"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_TooDistant ){ strPlan = _T("TooDistant"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_ProtectMyGeo ){ strPlan = _T("ProtectMyGeo"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_Seed ){ strPlan = _T("LandSeed"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_Consolidate ){ strPlan = _T("LandConsolidate"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_InvadeGeo ){ strPlan = _T("LandInvadeGeo"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_InvadeSection ){ strPlan = _T("LandInvadeSection"); }
		eif( pAIGeo->GetPlan() == EOSAIEnumGeoPlan_Land_InvadeUnownedSection ){ strPlan = _T("LandInvadeOwned"); }
		else{ ASSERT( false ); }

		//strText.Format( _T("%d %s"), pAIGeo->GetGeo()->GetGeoNumber(), strPlan );
		/*
		strText.Format( _T("%d %s %1.0f/%1.0f"), 
			pAIGeo->GetGeo()->GetGeoNumber(), strPlan, 
			pAIGeo->GetMyCitResDistance(),
			pAIGeo->GetNeutralEnemyCitResDistance() );
		*-/
		/*
		strText.Format( _T("%d %s %0.2f*%1.0f"), 
			pAIGeo->GetGeo()->GetGeoNumber(), strPlan, 
			pAIGeo->GetDistanceMult(), pAIGeo->GetGeoCitResValue() );
		*-/
		strText.Format( _T("%d %s (%d) %1.1f"), 
			pAIGeo->GetGeo()->GetGeoNumber(), strPlan, 
			pAIGeo->GetGeoDistanceCitResScoreRank(),
			pAIGeo->GetGeoDistanceCitResScore() );
			//pAIGeo->GetDistanceMult() * sqrt( pAIGeo->GetGeoCitResValue() ) );

		CMapLabel* pMapLabel = new CMapLabel();
		pMapLabel->m_strText = strText;
		pMapLabel->m_iSize = 2;
		pMapLabel->m_fOpacity01 = 0.8f;
		pMapLabel->m_Location.SetReal( 4*iGridX,4*iGridY );
		GetWorldDescServer()->AddMapLabel( pMapLabel );
	}
}
*/

void CEOSAIBrain::CreateMultiRegionPlans()
{
	long iAIPlayer = GetAIPlayerNumber();
	//CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
	CEOSAIMultiRegionNationwidePathways* pMyMultiRegionNationwidePathways = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );

	CList< CEOSAIMultiRegionPlan* >  MultiRegionsWithNoPlan;
	long iNumberOfMultiRegions = g_pEOSAICommonData->GetMultiRegionManager()->GetNumberOfMultiRegions();
	m_MultiRegionPlanArray.SetSize( iNumberOfMultiRegions+1 );
	for( long iMultiRegion=0; iMultiRegion<iNumberOfMultiRegions+1; iMultiRegion++ )
	{
		if( iMultiRegion == 0 )
		{
			m_MultiRegionPlanArray[iMultiRegion] = NULL;
		}
		else
		{
			//ASSERT( m_MultiRegionPlanArray[iMultiRegion] == NULL );
			CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( iMultiRegion );

			CEOSAIMultiRegionPlan* pMultiRegionPlan = new CEOSAIMultiRegionPlan( this );
			m_MultiRegionPlanArray[iMultiRegion] = pMultiRegionPlan;
			pMultiRegionPlan->m_iMultiRegion = iMultiRegion;
			pMultiRegionPlan->m_pMultiRegion = pMultiRegion;
			pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Undefined;
		}
	}

	// Calculate Danger
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	for( long iMultiRegion=1; iMultiRegion<iNumberOfMultiRegions+1; iMultiRegion++ )
	{
		CEOSAIMultiRegionPlan* pMultiRegionPlan = m_MultiRegionPlanArray[iMultiRegion];
		ASSERT( pMultiRegionPlan );

		if( iMultiRegion == 72 )
		{
			int h=0;
		}

		for( long iPlayer=1; iPlayer<iNumberOfPlayers+1; iPlayer++ )
		{
			if( iPlayer == iAIPlayer ) continue;
			EOSAI::CGamePlayer* pGPlayer = g_pEOSAIInterface->GetGamePlayer( iPlayer );
			if( pGPlayer->IsAlive() == false ) continue;

			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayer, iAIPlayer );
			float fMultiplier = 1.0f;
			if( eRel >= EOSAIEnumForeignRelations::enum_Teammate ){ continue; }
			if( eRel.IsAlly() ){ fMultiplier = 0.1f; }
			if( eRel.IsNeutral() ){ fMultiplier = 0.7f; }
			if( eRel.IsEnemy() ){ fMultiplier = 1.0f; }

			CEOSAIMultiRegionNationwidePathways* pPlayerMultiRegionNationwidePathways = g_pEOSAICommonData->GetNationwidePathways( iPlayer );
			CEOSAIMultiRegionAttackAccess* pAttackAccess = pPlayerMultiRegionNationwidePathways->GetMultiRegionAttackAccess( iMultiRegion );
			pMultiRegionPlan->m_fOtherPlayerGroundSeaUnitAccess01x += fMultiplier * pAttackAccess->GetGroundSeaUnitAccess01();
			pMultiRegionPlan->m_fOtherPlayerAirUnitAccess01x += fMultiplier * pAttackAccess->GetAirUnitAccess01();
		}
		//
	}

	//
	for( long iMultiRegion=1; iMultiRegion<iNumberOfMultiRegions+1; iMultiRegion++ )
	{
		CEOSAIMultiRegion2* pMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( iMultiRegion );

		#ifdef _DEBUG
		if( iMultiRegion == 72 )
		{
			int sdfrsre=0;
		}
		#endif _DEBUG

		//CEOSAIMultiRegionPlan* pMultiRegionPlan = new CEOSAIMultiRegionPlan( this );
		CEOSAIMultiRegionPlan* pMultiRegionPlan = m_MultiRegionPlanArray[iMultiRegion];
		ASSERT( pMultiRegionPlan );
		//m_MultiRegionPlanArray[iMultiRegion] = pMultiRegionPlan;
		//pMultiRegionPlan->m_iMultiRegion = iMultiRegion;
		//pMultiRegionPlan->m_pMultiRegion = pMultiRegion;
		//pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Undefined;

		CEOSAIRegionPathwayItem* pMyCitResPathwayItem = 
			pMyMultiRegionNationwidePathways->GetDistGeoPathfindingValueFromMyCitRes( iMultiRegion );
		CEOSAIRegionPathwayItem* pMyCitResUnitsPathwayItem = 
			pMyMultiRegionNationwidePathways->GetDistGeoPathfindingValueFromMyCitResUnits( iMultiRegion );

		pMultiRegionPlan->m_fDistanceFromMyCitRes      = pMyCitResPathwayItem->m_fDistance;
		pMultiRegionPlan->m_fDistanceFromMyCitResUnits = pMyCitResUnitsPathwayItem->m_fDistance;

		long iGeo = pMultiRegion->GetGeoId();
		CEOSAIGeo* pAIGeo = g_pEOSAIInterface->GetAICommonData()->GetAIGeo( iGeo );
		CEOSAIGeoView* pAIGeoView = pAIGeo->GetView( iAIPlayer );
		ASSERT( pAIGeo );

		if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Undefined )
		{
			pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Undefined;
			continue;
		}
		eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_SmallGeo_NoCitResUnits )
		{
			pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Ignore;
			continue;
		}
		eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_NoPlan )
		{
			pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_NoPlan;
			continue;
		}
		eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Ignore )
		{
			pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Ignore;
			continue;
		}

		if( pAIGeo->GetGeoType() == CEOSAIGeo::Water )
		{
			if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Water_NoPlanYet )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Water_NoPlanYet;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Water_NoCitResUnits )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Water_NoCitResUnits;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Water_TooDistant )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Water_TooDistant;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Water_Complex )
			{
				// I don't know if I every use this
				ASSERT( pMultiRegion->m_PlayerCitResControlValue01[iAIPlayer] == 0.0f );

				CEOSAIRegionPathwayItem* pRegionPathwayItem = 
					pMyMultiRegionNationwidePathways->GetDistGeoPathfindingValueFromMyCitRes( iMultiRegion );
				float fDistance = pRegionPathwayItem->m_fDistance;
				if( fDistance < 250.0f )
				{
					// The AI should try to control this area, it's near my borders
					pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Water_SeaControl;
				}
				if( fDistance < 700.0f )
				{
					// The AI should try to control this area, it's near my borders
					pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Water_ConsiderCaptureAndControl;
				}
				else
				{
					//pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Water_NoPlanYet;
					pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Ignore;
				}
			}
			else
			{
				ASSERT( false );
			}
		}
		if( pAIGeo->GetGeoType() == CEOSAIGeo::Land )
		{
			if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_NoPlanYet;
			}
			if( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_IgnoreLowRank )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Ignore;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoPlanYet )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_NoPlanYet;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_NoCitResUnits )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_NoCitResUnits;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_TooDistant )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_TooDistant;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_TooDistant )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_TooDistant;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_ProtectMyGeo )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_ProtectMyGeo;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_FriendsGeo )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_FriendsGeo;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_Seed )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_Seed;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_Consolidate )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_Consolidate;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_InvadeGeo )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_InvadeGeo;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_InvadeSection )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_InvadeSection;
			}
			eif( pAIGeoView->GetPlan() == EOSAIEnumGeoPlan_Land_InvadeUnownedSection )
			{
				pMultiRegionPlan->m_eMultiRegionPlan = EOSAIEnumMultiRegionPlan_Land_InvadeUnownedSection;
			}
			else
			{
				ASSERT( false );
			}
		}

		if( pMultiRegionPlan->m_eMultiRegionPlan == EOSAIEnumMultiRegionPlan_Water_NoPlanYet )
		{
			MultiRegionsWithNoPlan.AddTail( pMultiRegionPlan );
		}
	}

	POSITION pos = MultiRegionsWithNoPlan.GetHeadPosition();
	while( pos )
	{
		CEOSAIMultiRegionPlan* pMultiRegionPlan = MultiRegionsWithNoPlan.GetNext( pos );
		int h=0;
	}
}
/*
void CEOSAIBrain::AddAIMultiRegionPlansAsMapLabels()
{
	GetWorldDescServer()->DeleteMapLabels();
	for( long iMultiRegion=1; iMultiRegion<m_MultiRegionPlanArray.m_iSize; iMultiRegion++ )
	{
		CEOSAIMultiRegionPlan* pMultiRegionPlan = m_MultiRegionPlanArray[iMultiRegion];
		if( pMultiRegionPlan == NULL ) continue;

		CMultiRegion* pMultiRegion = pMultiRegionPlan->m_pMultiRegion;

		//long iGridX = pAIGeo->GetGeo()->m_x;
		//long iGridY = pAIGeo->GetGeo()->m_y;

		CString strText;
		CString strPlan;
		if(  pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_NoPlan ){ strPlan = _T("NoPlan"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Ignore ){ strPlan = _T("Ignore"); }
		//eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_SmallGeo_NoCitResUnits ){ str = _T("NoCitResUnits"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumMultiRegionPlan_Ignore_NoCitResUnits ){ str = _T("NoPoi"); }
		//
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Water_NoPlanYet ){ strPlan = _T("NoPlanYet"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Water_NoCitResUnits ){ strPlan = _T("NoCitResUnits"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Water_TooDistant ){ strPlan = _T("TooDistant"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Water_SeaControl ){ strPlan = _T("SeaControl"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Water_ConsiderCaptureAndControl ){ strPlan = _T("CapAndControl?"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Water_Submarine ){ strPlan = _T("Submarine"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumMultiRegionPlan_Water_ProtectMyWaterways ){ str = _T("ProtectWaterways"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumMultiRegionPlan_Water_Submarine ){ str = _T("WaterSubmarine"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumMultiRegionPlan_Water_Aggressive ){ str = _T("WaterAggressive"); }
		//eif( pAIGeo->GetPlan() == EOSAIEnumMultiRegionPlan_Water_TotalControl ){ str = _T("WaterTotalControl"); }
		//
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_NoPlanYet ){ strPlan = _T("NoPlanYet"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_TooDistant ){ strPlan = _T("TooDistant"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_ProtectMyGeo ){ strPlan = _T("ProtectMyGeo"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_Seed ){ strPlan = _T("LandSeed"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_Consolidate ){ strPlan = _T("LandConsolidate"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_InvadeGeo ){ strPlan = _T("LandInvadeGeo"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_InvadeSection ){ strPlan = _T("LandInvadeSection"); }
		eif( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_InvadeUnownedSection ){ strPlan = _T("LandInvadeOwned"); }
		else{ ASSERT( false ); }

		CEOSAIGeo* pAIGeo = pMultiRegionPlan->GetAIGeo();
		if( pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Water_NoPlanYet ||
			pMultiRegionPlan->GetPlan() == EOSAIEnumMultiRegionPlan_Land_TooDistant )
		{
			if( pAIGeo->GetGeo()->GetSize() < 30 ) continue;
		}
		float fGeoDistanceCitResScore = pAIGeo->GetGeoDistanceCitResScore();
		long iNumberOfTasks = pAIGeo->GetNumberOfUnownedAndEnemyPoiObjects();

		/*
		strText.Format( _T("%d %s %1.0f %1.1f"), 
			pMultiRegion->GetGeo()->GetGeoNumber(), strPlan,
			pMultiRegionPlan->m_fDistanceFromMyCitResUnits,
			fGeoDistanceCitResScore );
		*-/
		strText.Format( _T("%d %s %d %1.1f"), 
			pMultiRegion->GetGeo()->GetGeoNumber(), strPlan,
			iNumberOfTasks,
			fGeoDistanceCitResScore );

		CMapLabel* pMapLabel = new CMapLabel();
		pMapLabel->m_strText = strText;
		pMapLabel->m_iSize = 2;
		pMapLabel->m_fOpacity01 = 0.8f;
		if( pMultiRegion->IsLand() )
		{
			pMapLabel->m_Color.Set( 255,255,255 );
		}
		if( pMultiRegion->IsWater() )
		{
			pMapLabel->m_Color.Set( 150,150,255 );
		}
		pMapLabel->m_Location = pMultiRegionPlan->m_pMultiRegion->GetLocation();
		GetWorldDescServer()->AddMapLabel( pMapLabel );
	}
}

void CEOSAIBrain::AddAIMultiRegionDangerAccessAsMapLabels()
{
	GetWorldDescServer()->DeleteMapLabels();
	for( long iMultiRegion=1; iMultiRegion<m_MultiRegionPlanArray.m_iSize; iMultiRegion++ )
	{
		CEOSAIMultiRegionPlan* pMultiRegionPlan = m_MultiRegionPlanArray[iMultiRegion];
		if( pMultiRegionPlan == NULL ) continue;
		CMultiRegion* pMultiRegion = pMultiRegionPlan->m_pMultiRegion;
		if( pMultiRegion->m_PoiObjectsInsideRegion.IsEmpty() && pMultiRegion->GetSize() < 20 ) continue;

		CString strText;
		strText.Format( _T("d%1.0f/a%1.0f"), 
			100.0f*pMultiRegionPlan->m_fOtherPlayerGroundSeaUnitAccess01x,
			100.0f*pMultiRegionPlan->m_fOtherPlayerAirUnitAccess01x );

		CMapLabel* pMapLabel = new CMapLabel();
		pMapLabel->m_strText = strText;
		pMapLabel->m_iSize = 2;
		pMapLabel->m_fOpacity01 = 0.8f;
		if( pMultiRegion->IsLand() )
		{
			pMapLabel->m_Color.Set( 255,255,255 );
		}
		if( pMultiRegion->IsWater() )
		{
			pMapLabel->m_Color.Set( 150,150,255 );
		}
		pMapLabel->m_Location = pMultiRegionPlan->m_pMultiRegion->GetLocation();
		GetWorldDescServer()->AddMapLabel( pMapLabel );
	}
}

// Project Ideas
void  CEOSAIBrain::AddAITacticalProjects2AsMapLabels()
{
	GetWorldDescServer()->DeleteMapLabels();
	POSITION pos = GetAIThoughtDatabase()->GetTacticalProjects2()->GetHeadPosition();
	while( pos )
	{
		CEOSAITacticalProject2* pAITacticalProject2 = GetAIThoughtDatabase()->GetTacticalProjects2()->GetNext( pos );
		CString strText;
		strText.Format( _T("%d."), pAITacticalProject2->GetId() );
		//CString strPrimaryAction;
		if( pAITacticalProject2->GetPrimaryAction() == CEOSAITacticalProject2::enum_CaptureArea )
		{
			strText += _T("Capture");
		}
		eif( pAITacticalProject2->GetPrimaryAction() == CEOSAITacticalProject2::enum_SubmarineWarzone )
		{
			strText += _T("Sub");
		}
		else
		{
			continue;
		}

		CString strTemp;

		strTemp.Format( _T(" (si %1.2f)"), pAITacticalProject2->GetSimpleInterest() );
		strText += strTemp;

		strText += _T("\n");
		if( pAITacticalProject2->GetShouldBeInAllocateTacticalProjectList( 5 ) )
		{
			strText += _T("*");
		}

		float fNationalPathwayTimeToTarget = pAITacticalProject2->GetNationalPathwayTimeToTarget();
		strTemp.Format( _T("%d/%d/%d/%d/%d"), 
			pAITacticalProject2->GetInitialEvaluation_NearbyScoreIndex(),
			pAITacticalProject2->GetInitialEvaluation_ValueScoreIndex(),
			pAITacticalProject2->GetInitialEvaluation_BasicScoreIndex(),
			pAITacticalProject2->GetInitialEvaluation_EasyTargetScoreIndex(),
			pAITacticalProject2->GetInitialEvaluation_NoResistanceScoreIndex() );
		//strTemp.Format( _T("\nNPTime %1.2f"), fNationalPathwayTimeToTarget );
		strText += strTemp;
		//strTemp.Format( _T("\nSimpleInterest %1.2f"), pAITacticalProject2->GetSimpleInterest() );
		//strText += strTemp;

		//strText += strPrimaryAction;
		//strText.Format( _T("xx%1.0f/a%1.0f"), 
		//	100.0f*pMultiRegionPlan->m_fOtherPlayerGroundSeaUnitAccess01x,
		//	100.0f*pMultiRegionPlan->m_fOtherPlayerAirUnitAccess01x );

		CMapLabel* pMapLabel = new CMapLabel();
		pMapLabel->m_strText = strText;
		pMapLabel->m_iSize = 2;
		pMapLabel->m_fOpacity01 = 0.8f;
		/*
		if( pMultiRegion->IsLand() )
		{
			pMapLabel->m_Color.Set( 255,255,255 );
		}
		if( pMultiRegion->IsWater() )
		{
			pMapLabel->m_Color.Set( 150,150,255 );
		}
		*-/
		//pMapLabel->m_Location = pAITacticalProject2->GetLocation();
		pMapLabel->m_Location = pAITacticalProject2->CalculateCenterLocation();
		GetWorldDescServer()->AddMapLabel( pMapLabel );
	}
/*
	GetWorldDescServer()->DeleteMapLabels();
	for( long iMultiRegion=1; iMultiRegion<m_MultiRegionPlanArray.m_iSize; iMultiRegion++ )
	{
		CEOSAIMultiRegionPlan* pMultiRegionPlan = m_MultiRegionPlanArray[iMultiRegion];
		if( pMultiRegionPlan == NULL ) continue;
		CMultiRegion* pMultiRegion = pMultiRegionPlan->m_pMultiRegion;
		if( pMultiRegion->m_PoiObjectsInsideRegion.IsEmpty() && pMultiRegion->GetSize() < 20 ) continue;

/*
how do I connect project ideas?
Example: if I'm going to invade, and I want to degrade the cities, and I want to destroy his units
	All of those ideas are connected.
I can't treat it like "levels", since degrade and capture are probably mutually exclusive.
Strategy 1:                  Degrade Cities
Strategy 2:                                   Destroy Units
Strategy 3:                  Degrade Cities + Destroy Units
Strategy 4: Capture CitRes +                  Destroy Units
Capture CitRes and Degrade Cities are sometimes mutually exclusive, but not always.

I need to track what I'm planning to do to enemy poiobjects
  - This way, I can track capturing and degrading and so on.
  - I'd also like to stop the AI from stepping on other unit's toes (don't send two units to capture, degrade+capture)
  - The PoiObjects should have these list variables:
    - IncomingDegradeCityAirfieldTasks<>
    - IncomingCaptureCitResTasks
	- IncomingDestroyOrDegradeUnitTasks
  - What about cases where desires are fulfilled in a group (e.g. capture/destroy is part of a TacticalProject)
*-/

		CString strText;
		strText.Format( _T("xx%1.0f/a%1.0f"), 
			100.0f*pMultiRegionPlan->m_fOtherPlayerGroundSeaUnitAccess01x,
			100.0f*pMultiRegionPlan->m_fOtherPlayerAirUnitAccess01x );

		CMapLabel* pMapLabel = new CMapLabel();
		pMapLabel->m_strText = strText;
		pMapLabel->m_iSize = 2;
		pMapLabel->m_fOpacity01 = 0.8f;
		if( pMultiRegion->IsLand() )
		{
			pMapLabel->m_Color.Set( 255,255,255 );
		}
		if( pMultiRegion->IsWater() )
		{
			pMapLabel->m_Color.Set( 150,150,255 );
		}
		pMapLabel->m_Location = pMultiRegionPlan->m_pMultiRegion->GetLocation();
		GetWorldDescServer()->AddMapLabel( pMapLabel );
	}
*-/
}
*/

/*
void CEOSAIBrain::CreateDesiresAndTacticalProjects()
{
	TRACE( _T("CEOSAIBrain::CreateDesiresAndTacticalProjects() - Start") );
	Checkpoint::Write( _T("CEOSAIBrain::CreateDesiresAndTacticalProjects() - Start") );

	m_stopwatchCreateDesires.Start();
	m_AIThoughtDatabase.CreateDesires();
	m_stopwatchCreateDesires.Stop();
	Sleep( 10 );

	#ifdef _DEBUG
	//m_pAIPlayer->m_strDebug_Desires = m_AIThoughtDatabase.GetSortedDesiresText();
	m_AIThoughtDatabase.WriteDesiresSortedBySimpleInterestItems( &m_pAIPlayer->m_DesiresBySimpleInterest );
	m_AIThoughtDatabase.WriteDesiresSortedByScoreItems( &m_pAIPlayer->m_DesiresByScore );
	#endif _DEBUG
	TRACE( _T("CEOSAIBrain::CreateDesiresAndTacticalProjects() - 1") );
	Checkpoint::Write( _T("CEOSAIBrain::CreateDesiresAndTacticalProjects() - 1") );
	m_stopwatchCreateTacticalProjects.Start();
	m_AIActionScheduler.CreateTacticalProjects();
	m_stopwatchCreateTacticalProjects.Stop();
	Sleep( 10 );
	#ifdef _DEBUG
	//m_pAIPlayer->m_strDebug_TacticalProjects_SortedBySimpleInterest = m_AIThoughtDatabase.GetSortedTacticalProjectsText( CEOSAIThoughtDatabase::EnumDebugDataOutput_TacticalProjectsSortedBySimpleInterest );
	//m_pAIPlayer->m_strDebug_TacticalProjects_SortedBySimpleInterest.Replace( "\n", "\r\n" );
	m_AIThoughtDatabase.WriteTacticalProjectsSortedBySimpleInterest( &m_pAIPlayer->m_TacticalProjectsBySimpleInterest );
	m_AIThoughtDatabase.WriteTacticalProjectsSortedByInitialEvaluation( &m_pAIPlayer->m_TacticalProjectsByInitialEvaluation );
	#endif _DEBUG
	Checkpoint::Write( _T("CEOSAIBrain::CreateDesiresAndTacticalProjects() - 2") );

	// Do some calculations to figure out what I need (for transports, combat units etc)
	//   High combat costs might lower the ActionDefinition score
	//   Use this information to order the ActionDefinitions
	//m_AIThoughtDatabase.CalculateBasicFeasibilityOfTacticalProjects();

	// DEBUG
	// Print by SimpleInterest
	m_AIThoughtDatabase.Debug_OutputTacticalProjectsToFile( 
		CEOSAIThoughtDatabase::EnumDebugDataOutput_TacticalProjectsSortedBySimpleInterest, 
		g_pApp->GetBaseDirectory() + _T("\\ThoughtDatabase1.txt") );

	Checkpoint::Write( _T("CEOSAIBrain::CreateDesiresAndTacticalProjects() - End") );
	TRACE( _T("CEOSAIBrain::CreateDesiresAndTacticalProjects() - End") );
}
*/

void CEOSAIBrain::Process()
{
	long iAIPlayer = m_pAIPlayer->GetPlayerNumber();
	CEOSAIBrain* pAIBrain = m_pAIPlayer->GetAIBrain();

	m_TransportCalculationsTime.Start(); // Reset the timer back to zero each turn
	m_TransportCalculationsTime.Stop();

	//
	// This section processes the AI thoughts based on particular foreign relations.
	//   If that information changes during the course of a turn, the AI can reprocess its ideas and orders.
	//
	if( m_bProcessTurnBasedOnForeignRelations )
	{
		m_bProcessTurnBasedOnForeignRelations = false;

		m_iAIUnitTransportMovementDescCount = 0;

		// Warzones
		m_AIThoughtDatabase.ClearWarzoneCalculations();
		m_AIThoughtDatabase.FindWarzones();

		m_AIBuildManager.CreateBuildOrders_FirstPass(); // Create some build urges for each city (based on city danger, build completion, etc)

		/*
		#ifdef _DEBUG
		POSITION pos = m_AIPoiObjects.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
			if( pAIUnit )
			{
				if( pAIUnit->IsSubmarine() )
				{
					int h=0;
				}
			}
		}
		int g=0;
		#endif
		*/

		//m_Stopwatch_CreateDesiresAndTacticalProjects.Start();
		//CreateDesiresAndTacticalProjects();
		//m_Stopwatch_CreateDesiresAndTacticalProjects.Stop();

		// Create Desires
		m_stopwatchCreateDesires.Start();
		m_AIThoughtDatabase.CreateDesires();
		m_stopwatchCreateDesires.Stop();
		Sleep( 10 );

		// Create Tactical Projects
		#ifdef _DEBUG
		m_AIThoughtDatabase.WriteDesiresSortedBySimpleInterestItems( &m_pAIPlayer->m_DesiresBySimpleInterest );
		m_AIThoughtDatabase.WriteDesiresSortedByScoreItems( &m_pAIPlayer->m_DesiresByScore );
		#endif _DEBUG

		this->CreateTransportCombinedMultiRegionMaps();

		m_AIActionScheduler.CreateTacticalProjects2();
		#ifdef _DEBUG
		m_AIThoughtDatabase.WriteTacticalProjects2( &m_pAIPlayer->m_TacticalProjects2 );
		#endif _DEBUG
		m_AIActionScheduler.AllocateTacticalProjects2();

		/*
		else
		{
			m_stopwatchCreateTacticalProjects.Start();
			m_AIActionScheduler.CreateTacticalProjects();
			m_stopwatchCreateTacticalProjects.Stop();
			Sleep( 10 );
			#ifdef _DEBUG
			m_AIThoughtDatabase.WriteTacticalProjectsSortedBySimpleInterest( &m_pAIPlayer->m_TacticalProjectsBySimpleInterest );
			m_AIThoughtDatabase.WriteTacticalProjectsSortedByInitialEvaluation( &m_pAIPlayer->m_TacticalProjectsByInitialEvaluation );

			m_AIThoughtDatabase.Debug_OutputTacticalProjectsToFile( 
				CEOSAIThoughtDatabase::EnumDebugDataOutput_TacticalProjectsSortedBySimpleInterest, 
				g_pApp->GetBaseDirectory() + _T("\\ThoughtDatabase1.txt") );
			#endif _DEBUG

			// Allocate Tactical Projects
			m_AllocateTacticalProjectsTime.Start();
			m_AIActionScheduler.AllocateTacticalProjects();
			m_AllocateTacticalProjectsTime.Stop();
		}
		*/
/*
		//
		float fTotalViewDistance = 0.0f;
		long iCount = 0;
		CEOSAIUnitTemplateSet* pUnitsICanBuildOrHave = m_AIThoughtDatabase.GetUnitsICanBuildOrHave();
		POSITION pos = pUnitsICanBuildOrHave->m_List.GetHeadPosition();
		while( pos )
		{
			CUnitTemplate* pUnitTemplate = pUnitsICanBuildOrHave->m_List.GetNext( pos );
			if( pUnitTemplate && pUnitTemplate->IsSeaUnit() )
			{
				fTotalViewDistance += pUnitTemplate->GetUnitCombatCapability()->GetAverageVisibleRange();
				iCount++;
			}
		}
		float fAverageViewDistance = 0.0f;
		if( iCount > 0 )
		{
			fAverageViewDistance = fTotalViewDistance / iCount;
		}
		CUnitTemplate* pSub = g_pCommonState->GetActiveUnitset()->GetUnitTemplate( _T("Submarine (Class 2)" );
		float fDist = pSub->GetUnitCombatCapability()->GetAverageVisibleRange();
		int h=0;
*/

		// Now that I've allocated TacticalProjects, find things for idle units to do
		//   Units that need repair should get repaired (I need to integrate this desire earlier, as well)
		//   Weak units should not stick around in dangerous areas (e.g. transports)
		//   Strong units should go hunting for targets - move towards the front, etc
		/*
		POSITION pos = m_AIPoiObjects.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );

			// DEBUG
			/-*
			#ifdef _DEBUG
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
			if( pAIUnit && pAIUnit->GetServerUnit() )
			{
				if( pAIUnit->GetObjectId() == 201 )
				{
					int g=0;
				}
			}
			#endif _DEBUG
			*-/
			// END-DEBUG
			/-*
			if( pAIPoiObject->GetInitialState()->GetOwner() == iAIPlayer )
			{
				CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
				if( pAIUnit && 
					pAIUnit->GetServerUnit() &&
					pAIUnit->GetAIUnitPathwayFinder()->GetPreDefinedPath()->IsEmpty() )
				{
					pAIUnit->AllocateMinorTaskToThisIdleUnit();
				}
			}
			*-/
		}
		*/

		//Checkpoint::Write( _T("CEOSAIBrain::Process() - A") );
		if( m_pAIPlayer->GetFlag_ShutdownAIPlayer() ){ return; }

		// Based on the Production and Combat balances, think about altering the allied/enemy balance
		//   Some possibilities: 
		//     May want to advocate ganging up on one or more players
		//     May want to steal an ally from another player
		//     May want to keep another player neutral
		//     May want to sign a truce with an enemy (especially if I'm currently outgunned)
		//ConsiderSomeHypotheticalForeignRelationScenarios();



/*
		// ScenTest2.scen
		// Nazi
		//   Object1  = Nazi City
		//     Object12 = Nazi Tactical Bomber
		//     Object13 = Nazi Bomber
		//   Object6  = Nazi Transport
		//   Object7  = Nazi Infantry
		//   Object11 = Nazi Coastal City
		// Neutral
		//   Object3  = Neutral Overseas City
		//   Object10 = Neutral Landpath City
		// US
		//   Object2  = US Overseas City
		//   Object8  = US Cruiser
		//----------------------------------
		// AITestScenario6.scen - Object Ids
		// US
		//   City - 217
		//   Destroyer - 229
		// Nazi
		//   City - 218
		//     Infantry - 221, 231
		//     Fighter - 222
		//     Bomber - 225
		//     Dreadnaught - 223, 228
		//     Transport - 230
		// Neutral
		//   Iron - 220
		//   Oil - 224
		//   City - 219
		CString strScenarioFilename = GetCommonState()->GetWorldBuildDesc()->GetScenarioName();
		if( Public::m_bDoingTesting &&
			strScenarioFilename == "AITestScenario6" ) //"ScenTest2" )
		{
			CEOSAIPoiObject* pAIPoiObject229 = GetAIPoiObject( 229 );
			CEOSAIUnit2* pAIUnit229 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject229 );

			long iAIPlayer = GetAIPlayerNumber();
			if( false ) // air unit
			{
				//CEOSAIPoiObject* pAIPoiObject3 = GetAIPoiObject( 3 );
				//CEOSAICity* pAICity3 = dynamic_cast< CEOSAICity* >( pAIPoiObject3 );
				//
				CEOSAIPoiObject* pAIPoiObject222 = GetAIPoiObject( 222 );
				CEOSAIUnit2* pAIUnit222 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject222 );
				CEOSAIPoiObject* pAIPoiObject219 = GetAIPoiObject( 219 );
				CEOSAICity* pAICity219 = dynamic_cast< CEOSAICity* >( pAIPoiObject219 );
				//
				if( pAIUnit222 && pAICity219 )
				{
					//CEOSAIUnit2PathwayFinder* pPathway = CEOSAIUnit2PathwayFinder::CreatePathwayFinder( pAIUnit222 );
					CEOSAIUnit2PathwayFinder* pPathway = pAIUnit222->GetAIUnitPathwayFinder();
					pPathway->ClearPredefinedAndResultPaths();

					pPathway->SetNeutralsPathwayStategy( CEOSAIRegionPathfinder::PoliteToNeutrals );
					pPathway->SetEnemiesPathwayStategy( CEOSAIRegionPathfinder::AvoidEnemyContact );
					pPathway->AddWaypoint( CEOSAILocation( Pixel,550,510 ) );
					//pPathway->AddWaypoint( CEOSAILocation( Pixel,900,900 ) );
					//pPathway->AppendStep( pPathway->CreateAttackUnit( pAIUnit229 ) );
					pPathway->LandAtAnyAirbase();

					float f1 = pPathway->GetEndTime();

					//pPathway->CalculateInitialEvaluation();
					pPathway->CalculateResultPath();
					float f2 = pPathway->GetEndTime();
					//float fScore = pPathway->GetInitialEvaluationTimeScore();
					if( pPathway->ResultPathIsValid() )
					{
						pPathway->GetResultPath()->CreateOrders();
						ASSERT( pPathway->GetResultPath()->AllOrdersWereCreated() );
					}
				}
				//
				int g=0;
			}
			if( false ) // air unit
			{
				//CEOSAIPoiObject* pAIPoiObject3 = GetAIPoiObject( 3 );
				//CEOSAICity* pAICity3 = dynamic_cast< CEOSAICity* >( pAIPoiObject3 );
				//
				CEOSAIPoiObject* pAIPoiObject225 = GetAIPoiObject( 225 );
				CEOSAIUnit2* pAIUnit225 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject225 );
				CEOSAIPoiObject* pAIPoiObject219 = GetAIPoiObject( 219 );
				CEOSAICity* pAICity219 = dynamic_cast< CEOSAICity* >( pAIPoiObject219 );
				//
				if( pAIUnit225 && pAICity219 )
				{
					//CEOSAIUnit2PathwayFinder* pPathway = CEOSAIUnit2PathwayFinder::CreatePathwayFinder( pAIUnit225 );
					CEOSAIUnit2PathwayFinder* pPathway = pAIUnit225->GetAIUnitPathwayFinder();
					pPathway->ClearPredefinedAndResultPaths();

					pPathway->SetNeutralsPathwayStategy( CEOSAIRegionPathfinder::PoliteToNeutrals );
					pPathway->SetEnemiesPathwayStategy( CEOSAIRegionPathfinder::AvoidEnemyContact );
					pPathway->AddWaypoint( CEOSAILocation( Pixel,500,450 ) );
					//pPathway->AppendStep( pPathway->CreateDegradeTarget( pAICity219 ) );
					//pPathway->AppendStep( pPathway->CreateAttackUnit( pAIUnit229 ) );
					pPathway->LandAtAnyAirbase();

					float f1 = pPathway->GetEndTime();

					//pPathway->CalculateInitialEvaluation();
					pPathway->CalculateResultPath();
					float f2 = pPathway->GetEndTime();
					//float fScore = pPathway->GetInitialEvaluationTimeScore();
					if( pPathway->ResultPathIsValid() )
					{
						pPathway->GetResultPath()->CreateOrders();
						ASSERT( pPathway->GetResultPath()->AllOrdersWereCreated() );
					}
				}
				//
				int g=0;
			}
			if( false ) // the ground unit
			{
				CEOSAIPoiObject* pAIPoiObject221 = GetAIPoiObject( 221 );
				CEOSAIUnit2* pAIInfantry221 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject221 );
				ASSERT( pAIInfantry221->IsGroundUnit() );

				CEOSAIPoiObject* pAIPoiObject231 = GetAIPoiObject( 231 );
				CEOSAIUnit2* pAIInfantry231 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject231 );
				ASSERT( pAIInfantry231->IsGroundUnit() );

				CEOSAIPoiObject* pAIPoiObject230 = GetAIPoiObject( 230 );
				CEOSAIUnit2* pAITransport230 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject230 );
				ASSERT( pAITransport230->IsTransport() );

				CEOSAIPoiObject* pAIPoiObject232 = GetAIPoiObject( 232 );
				CEOSAIUnit2* pAITransport232 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject232 );
				ASSERT( pAITransport232->IsTransport() );

				// Land-based Iron Resource
				CEOSAIPoiObject* pAIPoiObject220 = GetAIPoiObject( 220 );
				CEOSAIResource* pAIRes220 = dynamic_cast< CEOSAIResource* >( pAIPoiObject220 );
				// Overseas City
				CEOSAIPoiObject* pAIPoiObject219 = GetAIPoiObject( 219 );
				CEOSAICity* pAICity219 = dynamic_cast< CEOSAICity* >( pAIPoiObject219 );
				// Sea-based Oil Resource
				CEOSAIPoiObject* pAIPoiObject224 = GetAIPoiObject( 224 );
				CEOSAIResource* pAIRes224 = dynamic_cast< CEOSAIResource* >( pAIPoiObject224 );

				//CEOSAIPoiObject* pAIPoiObject3 = GetAIPoiObject( 3 );
				//CEOSAICity* pAICity3 = dynamic_cast< CEOSAICity* >( pAIPoiObject3 );
				if( pAIInfantry221 && pAIInfantry231 && pAIRes220 )
				{
					pAIInfantry221->GetAIUnitPathwayFinder()->ClearPredefinedAndResultPaths();
					pAIInfantry231->GetAIUnitPathwayFinder()->ClearPredefinedAndResultPaths();
					pAITransport230->GetAIUnitPathwayFinder()->ClearPredefinedAndResultPaths();
					pAITransport232->GetAIUnitPathwayFinder()->ClearPredefinedAndResultPaths();

					//CEOSAIUnit2PathwayFinder* pPathway = CEOSAIUnit2PathwayFinder::CreatePathwayFinder( pAIInfantry221 );
					CEOSAIUnit2PathwayFinder* pInfantry221Pathway = pAIInfantry221->GetAIUnitPathwayFinder();
					CEOSAIUnit2PathwayFinder* pInfantry231Pathway = pAIInfantry231->GetAIUnitPathwayFinder();

					pInfantry221Pathway->SetNeutralsPathwayStategy( CEOSAIRegionPathfinder::PoliteToNeutrals );
					pInfantry221Pathway->SetEnemiesPathwayStategy( CEOSAIRegionPathfinder::AvoidEnemyContact );
				//	pPathway->AppendStep( pPathway->CreateCaptureTarget( pAIRes220 ) );

					//pAITransport232->GetAIUnitPathwayFinder()->SetStartTime( 5.0f );
					//pInfantry221Pathway->SetStartTime( 5.0f );
				//	pInfantry221Pathway->AppendStep( pInfantry221Pathway->CreateCaptureTarget( pAIRes220 ) );
					CEOSAIUnit2PathwayPredefinedStep* pStepA = pInfantry221Pathway->CreateCaptureTarget( pAICity219 ); // Requires a transport
					pInfantry221Pathway->AppendStep( pStepA );
				//	pStepA->UseTransportAndCreatePickupDropoffSteps( pAITransport230 );

		//			CEOSAIUnit2PathwayPredefinedStep* pStepB = pInfantry231Pathway->CreateCaptureTarget( pAICity219 ); // Requires a transport
		//			pInfantry231Pathway->AppendStep( pStepB );
				//	pStepB->UseTransportAndCreatePickupDropoffSteps( pAITransport230 );
					//pStep2->SetTransportToUse( pAITransport229 );
					//pPathway->SetTransportToUse( pAITransport229 );
				//	pPathway->AddCaptureTarget( pAICity3 );
					//pPathway->CalculateInitialEvaluation();
					//pPathway->CalculateTransportSupportPath();

					//pPathway->AllocateBestTransportForAllSteps();

					float f1a = pAIInfantry221->GetAIUnitPathwayFinder()->GetEndTime();
					float f2a = pAIInfantry231->GetAIUnitPathwayFinder()->GetEndTime();
					float f3a = pAITransport230->GetAIUnitPathwayFinder()->GetEndTime();

			//		if( pInfantry231Pathway->NeedsTransport() )
			//		{
			//			pInfantry231Pathway->SetTransportToUse( pAITransport230 );
			//		}
					if( pInfantry221Pathway->NeedsTransport() )
					{
						pInfantry221Pathway->SetTransportToUse( pAITransport232 );
					}

					//pAITransport232->GetAIUnitPathwayFinder()->AppendStep( 
					//	pAITransport232->GetAIUnitPathwayFinder()->CreateCaptureTarget( pAIRes224 ) );
					//pInfantry221Pathway->SetTransportToUse( pAITransport232 );

					float fWaitTimeA = pStepA->GetGroundUnitWaitTimeAtPickupLocation();
			//		float fWaitTimeB = pStepB->GetGroundUnitWaitTimeAtPickupLocation();

					//
					float f1b = pAIInfantry221->GetAIUnitPathwayFinder()->GetEndTime();
			//		float f2b = pAIInfantry231->GetAIUnitPathwayFinder()->GetEndTime();
					float f3b = pAITransport230->GetAIUnitPathwayFinder()->GetEndTime();
					float f4b = pAITransport232->GetAIUnitPathwayFinder()->GetEndTime();

					float fWaitTimeA2 = pStepA->GetGroundUnitWaitTimeAtPickupLocation();
			//		float fWaitTimeB2 = pStepB->GetGroundUnitWaitTimeAtPickupLocation();

					pInfantry221Pathway->CalculateResultPath();
					pInfantry231Pathway->CalculateResultPath();
					pAITransport230->GetAIUnitPathwayFinder()->CalculateResultPath();
					pAITransport232->GetAIUnitPathwayFinder()->CalculateResultPath();

					float fWaitTimeA3 = pStepA->GetGroundUnitWaitTimeAtPickupLocation();
			//		float fWaitTimeB3 = pStepB->GetGroundUnitWaitTimeAtPickupLocation();

					// Infantry 1
					POSITION pos2 = pInfantry221Pathway->GetResultPath()->m_Steps.GetHeadPosition();
					while( pos2 )
					{
						EOSAI::UnitPathwayResultStep* pStep = pInfantry221Pathway->GetResultPath()->m_Steps.GetNext( pos2 );
						float f1 = pStep->GetEndTime();
						int g=0;
					}
					// Infantry 2
					pos2 = pInfantry231Pathway->GetPreDefinedPath()->GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnit2PathwayPredefinedStep* pStep = pInfantry231Pathway->GetPreDefinedPath()->GetNext( pos2 );
						float f1 = pStep->GetEndTime();
						int g=0;
					}
					pos2 = pInfantry231Pathway->GetResultPath()->m_Steps.GetHeadPosition();
					while( pos2 )
					{
						EOSAI::UnitPathwayResultStep* pStep = pInfantry231Pathway->GetResultPath()->m_Steps.GetNext( pos2 );
						float f1 = pStep->GetEndTime();
						int g=0;
					}
					// Transport
					pos2 = pAITransport230->GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnit2PathwayPredefinedStep* pStep = pAITransport230->GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetNext( pos2 );
						float f1 = pStep->GetEndTime();
						int g=0;
					}
					pAITransport230->GetAIUnitPathwayFinder()->CalculateResultPath();
					pos2 = pAITransport230->GetAIUnitPathwayFinder()->GetResultPath()->m_Steps.GetHeadPosition();
					while( pos2 )
					{
						EOSAI::UnitPathwayResultStep* pStep = pAITransport230->GetAIUnitPathwayFinder()->GetResultPath()->m_Steps.GetNext( pos2 );
						float f1 = pStep->GetEndTime();
						int g=0;
					}
					// Transport
					pos2 = pAITransport232->GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnit2PathwayPredefinedStep* pStep = pAITransport232->GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetNext( pos2 );
						float f1 = pStep->GetEndTime();
						int g=0;
					}
					pAITransport232->GetAIUnitPathwayFinder()->CalculateResultPath();
					pos2 = pAITransport232->GetAIUnitPathwayFinder()->GetResultPath()->m_Steps.GetHeadPosition();
					while( pos2 )
					{
						EOSAI::UnitPathwayResultStep* pStep = pAITransport232->GetAIUnitPathwayFinder()->GetResultPath()->m_Steps.GetNext( pos2 );
						float f1 = pStep->GetEndTime();
						int g=0;
					}
					int h=0;

					//
					float f1c = pAIInfantry221->GetAIUnitPathwayFinder()->GetEndTime();
				//	float f2c = pAIInfantry231->GetAIUnitPathwayFinder()->GetEndTime();
					float f3c = pAITransport230->GetAIUnitPathwayFinder()->GetEndTime();
					float f4c = pAITransport232->GetAIUnitPathwayFinder()->GetEndTime();
					int j=0;

					/*
					if( pPathway->TransportNeeded() )
					{
						CEOSAIPoiObject* pAIPoiObject230 = GetAIPoiObject( 230 );
						CEOSAIUnit2* pAITransport230 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject230 );
				//		pPathway->GetPathwayResult()->UseTransport( pAITransport230 );
					}
					*-/
				//	pAITransport230->GetAIUnitPathwayFinder()->CalculateResultPath();

					/-*
					store the transport support paths (each transport, each city+build combo)
						There's not very many of them - usually, one object for each transport and city
					do I want to factor danger into it?
						have transports avoid danger?
						calculate a final score based on something other than time?
					Leave those calculations for the Final?
						I should store the danger values (which poi will be threatening?)
					*-/

					if( pInfantry221Pathway->ResultPathIsValid() )
					{
						pInfantry221Pathway->CreateOrders();
						ASSERT( pInfantry221Pathway->GetResultPath()->AllOrdersWereCreated() );
					}
					//if( pInfantry231Pathway->ResultPathIsValid() )
					//{
					//	pInfantry231Pathway->CreateOrders();
					//	ASSERT( pInfantry231Pathway->GetResultPath()->AllOrdersWereCreated() );
					//}
					int h4=0;
				}
				//
				int g=0;
			}
			if( false ) // Sea-Unit
			{
				CEOSAIPoiObject* pAIPoiObject228 = GetAIPoiObject( 228 );
				CEOSAIUnit2* pAIDreadnaught228 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject228 );
				CEOSAIPoiObject* pAIPoiObject224 = GetAIPoiObject( 224 );
				CEOSAIResource* pAIRes224 = dynamic_cast< CEOSAIResource* >( pAIPoiObject224 );
				//ASSERT( pAIUnit6 );
				if( pAIDreadnaught228 )//&& pAIUnit6->GetUnitTemplate()->GetInternalName() == "Transport (Class 1)" )
				{
					//CEOSAIUnit2PathwayFinder* pPathway = CEOSAIUnit2PathwayFinder::CreatePathwayFinder( pAIUnit228 );
					CEOSAIUnit2PathwayFinder* pPathway = pAIDreadnaught228->GetAIUnitPathwayFinder();
					pAIDreadnaught228->GetAIUnitPathwayFinder()->ClearPredefinedAndResultPaths();

					//CAISeaUnitPathway* pSeaPathway = dynamic_cast< CAISeaUnitPathway* >( pPathway );
					//pSeaPathway->AddWaypoint( CEOSAILocation( Pixel,650,270 ) );
					//pSeaPathway->SetPathwayStategy( CEOSAIUnit2PathwayFinder::PoliteToNeutrals_AvoidEnemyContact );

					//pPathway->SetNeutralsPathwayStategy( CEOSAIUnit2Pathway::PoliteToNeutrals );
					pPathway->SetEnemiesPathwayStategy( CEOSAIRegionPathfinder::AllowSomeEnemyContact );
					pPathway->TreatAsEnemy( 1 );
					//pSeaPathway->SetDangerAvoidance01( 1.0f );
					//pSeaPathway->SetEnemySightingAvoidance01( 0.0f );
					//pSeaPathway->SetEnemyPresenceAvoidance01( 0.0f );
					pPathway->AppendStep( pPathway->CreateCaptureTarget( pAIRes224 ) );
					pPathway->AddWaypoint( CEOSAILocation( Pixel,650,600 ) );
					pPathway->AppendStep( pPathway->CreateAttackUnit( pAIUnit229 ) );
					pPathway->CalculateResultPath();

					//float fDanger = pSeaPathway->GetPathwayDanger();
					//float fSighting = pSeaPathway->GetPathwayEnemySighting();

					if( pPathway->ResultPathIsValid() )
					{
						pPathway->GetResultPath()->CreateOrders();//( pSeaPathway->GetInitialEvaluationRoute() );
						ASSERT( pPathway->GetResultPath()->AllOrdersWereCreated() );
					}
				}
				//
				int g=0;
			}

			if( false ) // used with ScenTest.scen
			{
				Sleep( 10 );
				Sleep( 10 );
				Sleep( 10 );

				CEOSAIPoiObject* pAIPoiObject7 = GetAIPoiObject( 7 );
				CEOSAIUnit2* pAIUnit7 = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject7 );
				ASSERT( pAIUnit7 );
				//CEOSAIUnit2PathwayFinder* pPathway = CEOSAIUnit2PathwayFinder::CreatePathwayFinder( pAIUnit7 );
				CEOSAIUnit2PathwayFinder* pPathway = pAIUnit7->GetAIUnitPathwayFinder();

				CAIObject* pAIObject3 = GetAIObject( 3 );
				CEOSAICity* pAICity3 = dynamic_cast< CEOSAICity* >( pAIObject3 );
				ASSERT( pAICity3 );
				pPathway->AppendStep( pPathway->CreateCaptureTarget( pAICity3 ) );

				//pPathway->CalculateInitialEvaluation();
				pPathway->CalculateResultPath();
				//pPathway->CalculateNextLevelPathway();
				//pPathway->CalculateNextLevelPathway();
				bool bPathIsValid = pPathway->ResultPathIsValid();
				//float fEndTime = pPathway->GetPathEndTime();
				//
				delete pPathway;
			}

			if( false )
			{
				CAIObject* pAIObject1 = GetAIObject( 1 );
				CEOSAICity* pAICity1 = dynamic_cast< CEOSAICity* >( pAIObject1 );
				CUnitTemplate* pUnitTemplate =
					GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Infantry (Class 1)" );
					//GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Bomber (Class 1)" );
					//GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Nuclear SRBM" );
				//
				CAIObject* pAIObject3 = GetAIObject( 3 );
				CEOSAICity* pAICity3 = dynamic_cast< CEOSAICity* >( pAIObject3 );
				//
				//CAIObject* pAIObject5 = GetAIObject( 5 );
				//CEOSAICity* pAICity5 = dynamic_cast< CEOSAICity* >( pAIObject5 );
				//
				//CAIAirUnitPathway  AirUnitPathway( this );
				//CAIGroundUnitPathway* pPathway = 
				//	(CAIGroundUnitPathway*) CEOSAIUnit2Pathway::CreatePathway( pAICity1,pUnitTemplate );
				CEOSAIUnit2PathwayFinder* pPathway = CEOSAIUnit2PathwayFinder::CreatePathwayFinder( pAICity1,pUnitTemplate );
				//AirUnitPathway.SetUnit( 1, pAICity1,pUnitTemplate );
				//AirUnitPathway.m_pAIBrain = this;//pActor->GetAIBrain();
				//AirUnitPathway.m_iUnitOwner = 1;
				//AirUnitPathway.m_pActor = pAICity1;//pActor;
				//AirUnitPathway.m_pUnitTemplate = pUnitTemplate;

				//AirUnitPathway.SetPathwayStartToCurrentUnitLocation();
				pPathway->AppendStep( pPathway->CreateCaptureTarget( pAICity3 ) );
				//pPathway->AddWaypoint( pAICity4->GetPoiObject()->GetLocation() );
				//AirUnitPathway.AppendALanding( true );
				//AirUnitPathway.CalculateStepDistances( pAICity1->GetInitialState()->GetLocation(), 0.0f, &AirUnitPathway.m_PreDefinedPath );
				//AirUnitPathway.CalculateStepDistances( &AirUnitPathway.m_PreDefinedPath );

				//pPathway->CalculateInitialEvaluation();
				pPathway->CalculateResultPath();
				//pPathway->CalculateNextLevelPathway();
				bool bPathIsValid = pPathway->ResultPathIsValid();
				//float fEndTime = pPathway->GetPathEndTime();
				//AirUnitPathway.SolvePathway();
				//AirUnitPathway.CalculateSimplePath();
				//
				delete pPathway;
				int h=0;
			}
			if( false )
			{
				CAIObject* pAIObject1 = GetAIObject( 1 );
				CEOSAICity* pAICity1 = dynamic_cast< CEOSAICity* >( pAIObject1 );
				CUnitTemplate* pUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Destroyer (Class 1)" );
				//CUnitTemplate* pUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Bomber (Class 1)" );
				//CUnitTemplate* pUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Nuclear SRBM" );
				//
				CAIObject* pAIObject4 = GetAIObject( 4 );
				CEOSAICity* pAICity4 = dynamic_cast< CEOSAICity* >( pAIObject4 );
				//
				CAIObject* pAIObject5 = GetAIObject( 5 );
				CEOSAICity* pAICity5 = dynamic_cast< CEOSAICity* >( pAIObject5 );
				CAIObject* pAIObject7 = GetAIObject( 7 );
				CEOSAIResource* pAIOil7 = dynamic_cast< CEOSAIResource* >( pAIObject7 );
				//
				//CAIAirUnitPathway  AirUnitPathway( this );
				//CAIGroundUnitPathway* pPathway = 
				//	(CAIGroundUnitPathway*) CEOSAIUnit2Pathway::CreatePathway( pAICity1,pUnitTemplate );
				CEOSAIUnit2PathwayFinder* pPathway = CEOSAIUnit2PathwayFinder::CreatePathwayFinder( pAICity1,pUnitTemplate );
				//AirUnitPathway.SetUnit( 1, pAICity1,pUnitTemplate );
				//AirUnitPathway.m_pAIBrain = this;//pActor->GetAIBrain();
				//AirUnitPathway.m_iUnitOwner = 1;
				//AirUnitPathway.m_pActor = pAICity1;//pActor;
				//AirUnitPathway.m_pUnitTemplate = pUnitTemplate;

				//AirUnitPathway.SetPathwayStartToCurrentUnitLocation();
				//pPathway->AddCaptureTarget( pAIOil7 );
				pPathway->AddWaypoint( pAICity5->GetPoiObject()->GetLocation() );
				//pPathway->AddWaypoint( pAICity4->GetPoiObject()->GetLocation() );
				//AirUnitPathway.AppendALanding( true );
				//AirUnitPathway.CalculateStepDistances( pAICity1->GetInitialState()->GetLocation(), 0.0f, &AirUnitPathway.m_PreDefinedPath );
				//AirUnitPathway.CalculateStepDistances( &AirUnitPathway.m_PreDefinedPath );

				//pPathway->CalculateInitialEvaluation();
				pPathway->CalculateResultPath();

				bool bPathIsValid = pPathway->ResultPathIsValid();
				//float fEndTime = pPathway->GetPathEndTime();
				//AirUnitPathway.SolvePathway();
				//AirUnitPathway.CalculateSimplePath();
				//
				delete pPathway;
				int h=0;
			}
			if( false )
			{
				CAIObject* pAIObject1 = GetAIObject( 1 );
				CEOSAICity* pAICity1 = dynamic_cast< CEOSAICity* >( pAIObject1 );
				CUnitTemplate* pUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Bomber (Class 1)" );
				//CUnitTemplate* pUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( "Nuclear SRBM" );
				//
				CAIObject* pAIObject4 = GetAIObject( 4 );
				CEOSAICity* pAICity4 = dynamic_cast< CEOSAICity* >( pAIObject4 );
				//
				CAIObject* pAIObject5 = GetAIObject( 5 );
				CEOSAICity* pAICity5 = dynamic_cast< CEOSAICity* >( pAIObject5 );
				//

				CEOSAIUnit2PathwayFinder* pPathway = CEOSAIUnit2PathwayFinder::CreatePathwayFinder( pAICity1,pUnitTemplate );
				pPathway->AppendStep( pPathway->CreateDegradeTarget( pAICity5 ) );
				pPathway->AppendALandingIfPossible(); //AppendALanding( true );

				//pPathway->CalculateInitialEvaluation();
				pPathway->CalculateResultPath();

				bool bPathIsValid = pPathway->ResultPathIsValid();
				//float fEndTime = pPathway->GetPathEndTime();
				delete pPathway;
				int h=0;
				/*
				CAIAirUnitPathway  AirUnitPathway( this );
				AirUnitPathway.SetUnit( 1, pAICity1,pUnitTemplate );
				//AirUnitPathway.m_pAIBrain = this;//pActor->GetAIBrain();
				//AirUnitPathway.m_iUnitOwner = 1;
				//AirUnitPathway.m_pActor = pAICity1;//pActor;
				//AirUnitPathway.m_pUnitTemplate = pUnitTemplate;

				//AirUnitPathway.SetPathwayStartToCurrentUnitLocation();
				AirUnitPathway.AddDegradeTarget( pAICity5 );
				AirUnitPathway.AppendALanding( true );
				//AirUnitPathway.CalculateStepDistances( pAICity1->GetInitialState()->GetLocation(), 0.0f, &AirUnitPathway.m_PreDefinedPath );
				//AirUnitPathway.CalculateStepDistances( &AirUnitPathway.m_PreDefinedPath );

				AirUnitPathway.CalculatePathway( 1 );
				bool bPathIsValid = AirUnitPathway.PathIsValid();
				float fEndTime = AirUnitPathway.GetPathEndTime();
				//AirUnitPathway.SolvePathway();
				//AirUnitPathway.CalculateSimplePath();
				//
				int h=0;
				*-/
			}
		}
*/

		//
		// 1313 bytes in -19 Normal Blocks.
		//
		if( EOSAISettings::m_bDoingTesting )
		{
			if( iAIPlayer == 2 )
			{
				//g_pGameAppState->GetMapWindow()->GetMapTextOverlay()->AddString( 
				//	iMyPlayerNumber, enum_IconInfo, "CEOSAIBrain::Process() - 3" );
			}
		}


		// The nation's current situation will influence which ActionDefinitions it's going
		//   to pursue.
		// Evaluate the nation's overall position (both history and current setup)
		//   This evaluation depends on evaluations of other nation's militaries, 
		//     production capabilities, foreign relations state, and MultiRegions (which near me 
		//     are captured).
		//   Is the nation being invaded and needs to devote everything to repelling it? (WW2 France)
		//   Is the nation invading and needs to devote everything to the invasion? (WW2 Germany)
		//   Is the nation insulated from it's foes so it can devote effort to buildup? (WW2 US)
		//   Is the nation unfettered and needs to devote effort to expansion? (typical random map)
		/*
		//CEOSAINationalSummary* pMySummary = this->GetAIForeignRelationsSituation()->GetNationalSummary( GetAIPlayerNumber() );
		if( pMySummary->m_fCurrentWarPowerBalance == 1.0f ) // no enemies
		{
			// Increase/Decrease AIActionDefinitions based on the situation
			//   Look at the proximity, production-value, military-value of enemies and allies
			// A more-powerful enemy might mean:
			//   Increase value of defense, maybe capture unowned cities
			//   But what is the long-term strategy?  
			//     Blunt his attacks (attrition) until later? Wait for other player to intervene?
			// A weaker enemy might mean (especially if the combat/production values are skewed):
			//   Go on the offense
			// If the nations are connected by land, favor ground/air combat units, less
			//   energy put into ships and transports.
			// If the nations are not connected by land, favor anti-transport weapons - tactical
			//   aircraft, ground units, ...
		}
		*/

//		m_AIThoughtDatabase.InvokeMultiRegionPathwayDataForEachOfMyCitiesAndGroundAndSeaUnits();
		//m_AIThoughtDatabase.InvokeCityTransportAndCarrierPathwayData();
		//m_AIThoughtDatabase.InvokeTransportPathwayData();

		/*
		// Do some calculations to figure out what I need (for transports, combat units etc)
		//   High combat costs might lower the ActionDefinition score
		//   Use this information to order the ActionDefinitions
		//m_AIThoughtDatabase.EvaluateDoingActionDefinitions();
		m_AIThoughtDatabase.CalculateBasicFeasibilityOfTacticalProjects();
		*/

		// Create UnitActionValues
		//   For each UnitActionValue, calculate time-to-target, Combat Efficiency, etc
		//   Create a usefulness value based on TTT, CombatEfficiency, m_TimeValueFunction
		//m_AIThoughtDatabase.CreateUnitActionIdeas();
//		m_AIThoughtDatabase.EstimateWhenTacticalProjectsCanBeCompletedByUnitActionIdeas();
		//
//		m_AIThoughtDatabase.CalculateRelativeTTTScoresForUnitActionIdeas();
//		m_AIThoughtDatabase.CalculateUnitActionIdeaScores();
		//m_AIThoughtDatabase.CreateHypotheticalTaskForce();

		// Print by Sorted by HypotheticalTaskForceScore
//		m_AIThoughtDatabase.Debug_OutputTacticalProjectsToFile( 
//			CEOSAIThoughtDatabase::EnumDebugDataOutput_TacticalProjectsSortedByHypotheticalTaskForceScore, 
//			g_pApp->GetBaseDirectory() + "\\ThoughtDatabase_HypotheticalScore.txt" );

		//m_AIThoughtDatabase.AllocateTaskForce();

		//
		/* ****
		I need to sort the ActionDefinitions by their BasicFeasibility scores
		Then I look at their UnitActionIdeas, figure out if it's still feasible and figure out
		  a new ActionDefinition score?
		//*/

		// Figure out which ActionDefinitions I'm going to perform, which ones get defered, which get dropped
		//m_AIThoughtDatabase.DecideWhichTacticalProjectsToSchedule();
		//m_AIThoughtDatabase.EvaluateSchedulingActionDefinitions();
		//m_AIThoughtDatabase.EvaluateSchedulingActionDefinitions();

		/*
		CAIObject* pAIObject17 = this->GetAIObject( 17 );
		CEOSAIUnit2* pAIUnit17 = dynamic_cast< CEOSAIUnit2* >( pAIObject17 );
		if( pAIUnit17 )
		{
			g_pAIUnit17 = pAIUnit17;
			if( pAIUnit17->GetUnitTemplate()->GetInternalName() == _T("Tank (Class 6)") )
			{
				int z=0;
			}
			// Check the ActionProgression information AIActions, AITasks
			int zz=0;
			// they're gone.  check this.  does the scheduling of other actions delete the existing ones?
			//   try scheduling just one ActionDef.
		}
		*/

		//m_AIBuildManager.CreateEOSAIBuildOptions();

		//m_AIThoughtDatabase.ScheduleAITacticalProjects();
		if( m_pAIPlayer->GetFlag_ShutdownAIPlayer() ){ return; }
		//m_AIThoughtDatabase.ScheduleBuildOrders();
		//GetAIThoughtDatabase()->GetAIFutureBuildIdeas()->SetBuildOrdersForCitiesWithNoBuildOrders( this );
		m_AIBuildManager.CreateBuildOrders_Final();

		// Buy Resources (if needed)
		BuyNeededResourcesFromOpenMarket();

		//
		#ifdef _DEBUG
		//m_pAIPlayer->m_strDebug_TacticalProjects_SortedByInitialEvaluationScore = m_AIThoughtDatabase.GetSortedTacticalProjectsText( CEOSAIThoughtDatabase::EnumDebugDataOutput_TacticalProjectsSortedByInitialEvaluationScore );
		//m_pAIPlayer->m_strDebug_TacticalProjects_SortedByInitialEvaluationScore.Replace( "\n", "\r\n" );
		#endif _DEBUG

		// Print by Basic Feasibility
//		m_AIThoughtDatabase.Debug_OutputTacticalProjectsToFile( 
//			CEOSAIThoughtDatabase::EnumDebugDataOutput_TacticalProjectsSortedByInitialEvaluationScore, 
//			g_pApp->GetBaseDirectory() + "\\ThoughtDatabase2.txt" );
		//

		// DEBUG: Iterate over the units, check the UnitActionValues list
		/*
		CAIObject* pDebugAIObject = GetAIObject( 183 );
		CEOSAIUnit2* pDebugAIUnit = dynamic_cast< CEOSAIUnit2* >( pDebugAIObject );
		if( pDebugAIUnit )
		{
			int z=0;
		}
		pDebugAIObject = GetAIObject( 184 );
		pDebugAIUnit = dynamic_cast< CEOSAIUnit2* >( pDebugAIObject );
		if( pDebugAIUnit )
		{
			int z=0;
		}
		*/

		//
		if( EOSAISettings::m_bDoingTesting )
		{
			if( iAIPlayer == 2 )
			{
				//g_pGameAppState->GetMapWindow()->GetMapTextOverlay()->AddString( 
				//	iAIPlayer, enum_IconInfo, _T("CEOSAIBrain::Process() - About to instanciate orders") );
			}
		}

		// Pause while I wait for an answer (if I return from this function, it means
		//    that I am done with my turn).  Maybe I should wait until all the human
		//    players have submitted their turns.  When that happens, they can't respond
		//    to my negotiations or trade deals anyway.

		// Instanciate orders
		//InstanciateOrders();

		if( EOSAISettings::m_bDoingTesting )
		{
			if( iAIPlayer == 2 )
			{
				//g_pGameAppState->GetMapWindow()->GetMapTextOverlay()->AddString( 
				//	iAIPlayer, enum_IconInfo, "CEOSAIBrain::Process() - Complete" );
			}
		}
	}
}

void SellExcessResources_MaxAndMinimumDelta( float fMax, float fMinimumDelta, CAIResourceState* pMoneyState, CAIResourceState* pRes );
void SellExcessResources_TurnUntilShortage( float fMax, float fTurns, CAIResourceState* pMoneyState, CAIResourceState* pRes );
void SellResource( float fAmount, CAIResourceState* pMoneyState, CAIResourceState* pRes );
void BuyNeededResource( CAIResourceState* pMoneyState, CAIResourceState* pRes );

void CEOSAIBrain::BuyNeededResourcesFromOpenMarket()
{
	//ASSERT( false );
	//while( true )
	//{
		long iAIPlayer = this->GetAIPlayerNumber();

		// What's my current resource delta, storage?
		ASSERT(	this->AllMyCitiesHaveOrders() );
		//GetWorldDescPlayerProxy()->AICalculateResourceConsumptionAndDeltas( false );
		//CalculateResourceConsumptionAndDeltas( false ); -> This should already have been calculated

		/*
		CAIResourceStateAll  BuySellPlan;
		BuySellPlan.m_Money.m_fCurrent = this->GetTotalMoney();
		BuySellPlan.m_Food.m_fCurrent  = this->GetTotalFood();
		BuySellPlan.m_Iron.m_fCurrent  = this->GetTotalIron();
		BuySellPlan.m_Oil.m_fCurrent   = this->GetTotalOil();
		*/
		CAIResourceStateAll  BuySellPlan;
		/*
		BuySellPlan.m_Money.m_fCurrent = m_AIResourceSummary.m_TotalResource.Get(_T("Money")); //this->GetTotalMoney();
		BuySellPlan.m_Food.m_fCurrent  = m_AIResourceSummary.m_TotalResource.Get(_T("Food")); //this->GetTotalFood();
		BuySellPlan.m_Iron.m_fCurrent  = m_AIResourceSummary.m_TotalResource.Get(_T("Iron")); //this->GetTotalIron();
		BuySellPlan.m_Oil.m_fCurrent   = m_AIResourceSummary.m_TotalResource.Get(_T("Oil")); //this->GetTotalOil();
		*/
		BuySellPlan.m_Money.m_fCurrent = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_ResourceSummary.m_TotalResources.Get(_T("Money")); //this->GetTotalMoney();
		BuySellPlan.m_Food.m_fCurrent  = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_ResourceSummary.m_TotalResources.Get(_T("Food")); //this->GetTotalFood();
		BuySellPlan.m_Iron.m_fCurrent  = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_ResourceSummary.m_TotalResources.Get(_T("Iron")); //this->GetTotalIron();
		BuySellPlan.m_Oil.m_fCurrent   = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_ResourceSummary.m_TotalResources.Get(_T("Oil")); //this->GetTotalOil();

		EOSAI::ResourceAmounts ResourceDelta = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->GetResourceSummary()->GetResourceDelta();
		//BuySellPlan.m_Money.m_fDelta = GetWorldDescPlayerProxy()->GetDeltaMoney();
		//BuySellPlan.m_Money.m_fDelta = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_ResourceSummary.m_ResourceDelta.Get(_T("Money"));
		//BuySellPlan.m_Food.m_fDelta = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_ResourceSummary.m_ResourceDelta.Get(_T("Food"));//m_AIResourceEffectsSummary.m_ResourceDelta.Get( _T("Food") );//GetDeltaFood();
		//BuySellPlan.m_Iron.m_fDelta = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_ResourceSummary.m_ResourceDelta.Get(_T("Iron"));//m_AIResourceEffectsSummary.m_ResourceDelta.Get( _T("Iron") );//GetDeltaIron();
		//BuySellPlan.m_Oil.m_fDelta = g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_ResourceSummary.m_ResourceDelta.Get(_T("Oil"));//m_AIResourceEffectsSummary.m_ResourceDelta.Get( _T("Oil") );//GetDeltaOil();
		BuySellPlan.m_Money.m_fDelta = ResourceDelta.Get(_T("Money"));
		BuySellPlan.m_Food.m_fDelta = ResourceDelta.Get(_T("Food"));//m_AIResourceEffectsSummary.m_ResourceDelta.Get( _T("Food") );//GetDeltaFood();
		BuySellPlan.m_Iron.m_fDelta = ResourceDelta.Get(_T("Iron"));//m_AIResourceEffectsSummary.m_ResourceDelta.Get( _T("Iron") );//GetDeltaIron();
		BuySellPlan.m_Oil.m_fDelta = ResourceDelta.Get(_T("Oil"));//m_AIResourceEffectsSummary.m_ResourceDelta.Get( _T("Oil") );//GetDeltaOil();

		// Run the cheats first
		{
			if( m_pAIPlayer->AICheat_BonusMoneyAndResourcesEachTurn() )
			{
				long iAIPlayer = m_pAIPlayer->GetPlayerNumber();
				//long iNumberOfOwnedCities = GetWorldDescServer()->GetNumberOfOwnedCities( iAIPlayer );
				long iNumberOfOwnedCities = (int) g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_CitResSummary.GetCityValue()/10;

				// Bonus money is $1 + $1 per city/turn
				BuySellPlan.m_Money.m_fBuy += iNumberOfOwnedCities;

				// Bonus resources is 1 per city + 20% bonus on all resources
				BuySellPlan.m_Food.m_fBuy += iNumberOfOwnedCities +
							0.2f*g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_CitResSummary.m_fFoodProduction;
							//GetWorldDescServer()->AmountOfFoodProducedOverTimeperiod( iAIPlayer, 1.0f );
				BuySellPlan.m_Iron.m_fBuy += iNumberOfOwnedCities +
							0.2f*g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_CitResSummary.m_fIronProduction;
							//0.2f*GetWorldDescServer()->AmountOfIronProducedOverTimeperiod( iAIPlayer, 1.0f );
				BuySellPlan.m_Oil.m_fBuy  += iNumberOfOwnedCities +
							0.2f*g_pEOSAICommonData->GetAINationalSummary3(iAIPlayer)->m_CitResSummary.m_fOilProduction;
							//0.2f*GetWorldDescServer()->AmountOfOilProducedOverTimeperiod( iAIPlayer, 1.0f );
			}
			if( m_pAIPlayer->AICheat_50PercentFreeResourcesInShortage() )
			{
				if( BuySellPlan.m_Food.GetNext() < 0.0f )
				{
					BuySellPlan.m_Food.m_fBuy  += 0.2f * ( - BuySellPlan.m_Food.GetNext() );
				}
				if( BuySellPlan.m_Iron.GetNext() < 0.0f )
				{
					BuySellPlan.m_Iron.m_fBuy  += 0.2f * ( - BuySellPlan.m_Iron.GetNext() );
				}
				if( BuySellPlan.m_Oil.GetNext() < 0.0f )
				{
					BuySellPlan.m_Oil.m_fBuy   += 0.2f * ( - BuySellPlan.m_Oil.GetNext() );
				}
			}

			ASSERT( BuySellPlan.m_Money.m_fCurrent >= BuySellPlan.m_Money.m_fSell );
			ASSERT( BuySellPlan.m_Food.m_fCurrent  >= BuySellPlan.m_Food.m_fSell );
			ASSERT( BuySellPlan.m_Iron.m_fCurrent  >= BuySellPlan.m_Iron.m_fSell );
			ASSERT( BuySellPlan.m_Oil.m_fCurrent   >= BuySellPlan.m_Oil.m_fSell );

			// Do legitimate buying/selling -- sell excess resources to the open-market
			SellExcessResources_MaxAndMinimumDelta( 300.0f, 5.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Food );
			SellExcessResources_MaxAndMinimumDelta( 300.0f, 5.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Iron );
			SellExcessResources_MaxAndMinimumDelta( 300.0f, 5.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Oil  );
			SellExcessResources_MaxAndMinimumDelta( 500.0f, 0.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Food );
			SellExcessResources_MaxAndMinimumDelta( 500.0f, 0.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Iron );
			SellExcessResources_MaxAndMinimumDelta( 500.0f, 0.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Oil  );

			ASSERT( BuySellPlan.m_Money.m_fCurrent >= BuySellPlan.m_Money.m_fSell );
			ASSERT( BuySellPlan.m_Food.m_fCurrent  >= BuySellPlan.m_Food.m_fSell );
			ASSERT( BuySellPlan.m_Iron.m_fCurrent  >= BuySellPlan.m_Iron.m_fSell );
			ASSERT( BuySellPlan.m_Oil.m_fCurrent   >= BuySellPlan.m_Oil.m_fSell );

			// Buy needed resources with existing money
			BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Food );
			BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Iron );
			BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Oil );

			// If I'm still having a shortage in one resource, then sell-off some more resources
			if( BuySellPlan.m_Money.GetNext() < 10.0f ||
				BuySellPlan.m_Food.GetNext() < 10.0f ||
				BuySellPlan.m_Iron.GetNext() < 10.0f ||
				BuySellPlan.m_Oil.GetNext() < 10.0f )
			{
				// Do legitimate buying/selling -- sell any resources with 50 total and a growing delta
				SellExcessResources_MaxAndMinimumDelta( 50.0f, 0.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Food );
				SellExcessResources_MaxAndMinimumDelta( 50.0f, 0.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Iron );
				SellExcessResources_MaxAndMinimumDelta( 50.0f, 0.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Oil  );

				ASSERT( BuySellPlan.m_Money.m_fCurrent >= BuySellPlan.m_Money.m_fSell );
				ASSERT( BuySellPlan.m_Food.m_fCurrent  >= BuySellPlan.m_Food.m_fSell );
				ASSERT( BuySellPlan.m_Iron.m_fCurrent  >= BuySellPlan.m_Iron.m_fSell );
				ASSERT( BuySellPlan.m_Oil.m_fCurrent   >= BuySellPlan.m_Oil.m_fSell );

				// Buy Resources with existing money
				BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Food );
				BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Iron );
				BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Oil );
			}
			// If I'm still having a shortage in one resource, then sell-off some more resources
			if( BuySellPlan.m_Money.GetNext() < 10.0f ||
				BuySellPlan.m_Food.GetNext() < 10.0f ||
				BuySellPlan.m_Iron.GetNext() < 10.0f ||
				BuySellPlan.m_Oil.GetNext() < 10.0f )
			{
				// Do legitimate buying/selling -- sell any resources with a 10 total and shortage in 8+ turns
				SellExcessResources_TurnUntilShortage( 10.0f, 8.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Food );
				SellExcessResources_TurnUntilShortage( 10.0f, 8.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Iron );
				SellExcessResources_TurnUntilShortage( 10.0f, 8.0f, &BuySellPlan.m_Money, &BuySellPlan.m_Oil  );

				ASSERT( BuySellPlan.m_Money.m_fCurrent >= BuySellPlan.m_Money.m_fSell );
				ASSERT( BuySellPlan.m_Food.m_fCurrent  >= BuySellPlan.m_Food.m_fSell );
				ASSERT( BuySellPlan.m_Iron.m_fCurrent  >= BuySellPlan.m_Iron.m_fSell );
				ASSERT( BuySellPlan.m_Oil.m_fCurrent   >= BuySellPlan.m_Oil.m_fSell );

				// Buy Resources with existing money
				BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Food );
				BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Iron );
				BuyNeededResource( &BuySellPlan.m_Money, &BuySellPlan.m_Oil );
			}

			BuySellPlan.m_Money.RectifyBuyAndSellValues();
			BuySellPlan.m_Food.RectifyBuyAndSellValues();
			BuySellPlan.m_Iron.RectifyBuyAndSellValues();
			BuySellPlan.m_Oil.RectifyBuyAndSellValues();

			ASSERT( BuySellPlan.m_Money.m_fBuy >= 0.0f && BuySellPlan.m_Money.m_fSell >= 0.0f &&
					BuySellPlan.m_Food.m_fBuy >= 0.0f  && BuySellPlan.m_Food.m_fSell >= 0.0f &&
					BuySellPlan.m_Iron.m_fBuy >= 0.0f  && BuySellPlan.m_Iron.m_fSell >= 0.0f &&
					BuySellPlan.m_Oil.m_fBuy >= 0.0f   && BuySellPlan.m_Oil.m_fSell >= 0.0f );

			if( BuySellPlan.m_Money.m_fBuy > 0.0f || BuySellPlan.m_Money.m_fSell > 0.0f ||
				BuySellPlan.m_Food.m_fBuy > 0.0f  || BuySellPlan.m_Food.m_fSell > 0.0f ||
				BuySellPlan.m_Iron.m_fBuy > 0.0f  || BuySellPlan.m_Iron.m_fSell > 0.0f ||
				BuySellPlan.m_Oil.m_fBuy > 0.0f   || BuySellPlan.m_Oil.m_fSell > 0.0f )
			{
				// Create a dummy proposal
				/*
				CTradeAgreement* pTradeAgreement;
				pTradeAgreement = new CTradeAgreement();
				pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_BrandNew;
				pTradeAgreement->m_iPlayer[0] = GetAIPlayerNumber();
				pTradeAgreement->m_iPlayer[1] = 0;
				pTradeAgreement->m_ePlayerResponse[0] = CTradeAgreement::enum_Accept;
				pTradeAgreement->m_iPlayerWhoMadeTheOffer = GetAIPlayerNumber();
				pTradeAgreement->m_iOfferedOnTurn = GetWorldDescServer()->GetCurrentTurn();

				pTradeAgreement->m_iPlayerMoneyLumpSum[0] = BuySellPlan.m_Money.m_fSell;//fSellMoney;
				pTradeAgreement->m_iPlayerFoodLumpSum[0] = BuySellPlan.m_Food.m_fSell;//fSellFood;
				pTradeAgreement->m_iPlayerIronLumpSum[0] = BuySellPlan.m_Iron.m_fSell;//fSellIron;
				pTradeAgreement->m_iPlayerOilLumpSum[0]  = BuySellPlan.m_Oil.m_fSell;//fSellOil;

				pTradeAgreement->m_iPlayerMoneyLumpSum[1] = BuySellPlan.m_Money.m_fBuy;//fBuyMoney;
				pTradeAgreement->m_iPlayerFoodLumpSum[1] = BuySellPlan.m_Food.m_fBuy;//fBuyFood;
				pTradeAgreement->m_iPlayerIronLumpSum[1] = BuySellPlan.m_Iron.m_fBuy;//fBuyIron;
				pTradeAgreement->m_iPlayerOilLumpSum[1]  = BuySellPlan.m_Oil.m_fBuy;//fBuyOil;

				//CWorldDescServer* pWorldDescServer = m_pAIPlayer->GetWorldDescServer();
				//ASSERT( pWorldDescServer );
				GetWorldDescServer()->DoLumpSumTrade( m_pAIPlayer->GetPlayer()->GetMessageTargetId(), pTradeAgreement );
				*/
				CEOSAIStrategicAIOrder_Trade* pTradeAgreement = new CEOSAIStrategicAIOrder_Trade(this->GetAIPlayer()->GetStrategicAI());
				pTradeAgreement->m_iToPlayer = 0;
				//pTradeAgreement->m_TradeAgreementState = EOSAIEnumTradeAgreementState_BrandNew;
				//pTradeAgreement->m_iPlayer[0] = GetAIPlayerNumber();
				//pTradeAgreement->m_iPlayer[1] = 0;
				//pTradeAgreement->m_ePlayerResponse[0] = CTradeAgreement::enum_Accept;
				//pTradeAgreement->m_iPlayerWhoMadeTheOffer = GetAIPlayerNumber();
				//pTradeAgreement->m_iOfferedOnTurn = GetWorldDescServer()->GetCurrentTurn();

				/*
				pTradeAgreement->m_ResourcesToBuy.Set( _T("Money"), BuySellPlan.m_Money.m_fBuy );
				pTradeAgreement->m_ResourcesToBuy.Set( _T("Food"), BuySellPlan.m_Food.m_fBuy );
				pTradeAgreement->m_ResourcesToBuy.Set( _T("Iron"), BuySellPlan.m_Iron.m_fBuy );
				pTradeAgreement->m_ResourcesToBuy.Set( _T("Oil"), BuySellPlan.m_Oil.m_fBuy );
				*/
				pTradeAgreement->m_ResourcesToBuy[_T("Money")] = BuySellPlan.m_Money.m_fBuy;
				pTradeAgreement->m_ResourcesToBuy[_T("Food")] = BuySellPlan.m_Food.m_fBuy;
				pTradeAgreement->m_ResourcesToBuy[_T("Iron")] = BuySellPlan.m_Iron.m_fBuy;
				pTradeAgreement->m_ResourcesToBuy[_T("Oil")] = BuySellPlan.m_Oil.m_fBuy;

				pTradeAgreement->Execute(g_pEOSAIInterface->GetCurrentTurn());
				/*
				pTradeAgreement->m_iPlayerMoneyLumpSum[0] = BuySellPlan.m_Money.m_fSell;//fSellMoney;
				pTradeAgreement->m_iPlayerFoodLumpSum[0] = BuySellPlan.m_Food.m_fSell;//fSellFood;
				pTradeAgreement->m_iPlayerIronLumpSum[0] = BuySellPlan.m_Iron.m_fSell;//fSellIron;
				pTradeAgreement->m_iPlayerOilLumpSum[0]  = BuySellPlan.m_Oil.m_fSell;//fSellOil;

				pTradeAgreement->m_iPlayerMoneyLumpSum[1] = BuySellPlan.m_Money.m_fBuy;//fBuyMoney;
				pTradeAgreement->m_iPlayerFoodLumpSum[1] = BuySellPlan.m_Food.m_fBuy;//fBuyFood;
				pTradeAgreement->m_iPlayerIronLumpSum[1] = BuySellPlan.m_Iron.m_fBuy;//fBuyIron;
				pTradeAgreement->m_iPlayerOilLumpSum[1]  = BuySellPlan.m_Oil.m_fBuy;//fBuyOil;

				//CWorldDescServer* pWorldDescServer = m_pAIPlayer->GetWorldDescServer();
				//ASSERT( pWorldDescServer );
				GetWorldDescServer()->DoLumpSumTrade( m_pAIPlayer->GetPlayer()->GetMessageTargetId(), pTradeAgreement );
				*/
			}
		}
	//}
}

void SellExcessResources_MaxAndMinimumDelta( float fMax, float fMinimumDelta, CAIResourceState* pMoneyState, CAIResourceState* pRes )
{
	// If the resource amount if growing...
	if( pRes->m_fCurrent > fMax && pRes->GetNext() > pRes->m_fCurrent+fMinimumDelta )
	{
		// Example: If fMax is 300, sell all excess resources above 300
		//ASSERT( pRes->m_fSell == 0.0f );
		/*
		float fNewSale  = pRes->m_fCurrent-fMax;
		float fNewMoney = fNewSale * OPENMARKET_MONEY_FOR_SELLING_RESOURCE; // $1 for every resource sold
		pRes->m_fSell += fNewSale;
		pMoneyState->m_fBuy += fNewMoney;
		*/
		//float fAmountToSell = pRes->m_fCurrent-pRes->m_fSell-fMax;
		float fAmountToSell = pRes->m_fCurrent + pRes->m_fBuy - pRes->m_fSell - fMax;
		if( fAmountToSell > 0.0f )
		{
			SellResource( fAmountToSell, pMoneyState, pRes );
			pRes->RectifyBuyAndSellValues();
		}
	}
}

void SellExcessResources_TurnUntilShortage( float fMax, float fTurns, CAIResourceState* pMoneyState, CAIResourceState* pRes )
{
	if( pRes->m_fCurrent > fMax )
	{
		// If it's growing or unchanging, then sell everything up to the fMax
		//if( pRes->GetNext() >= pRes->m_fCurrent )
		if( pRes->m_fDelta >= 0.0f )
		{
			SellResource( pRes->m_fCurrent + pRes->m_fBuy - pRes->m_fSell - fMax, pMoneyState, pRes );
		}
		// If it's shrinking, then sell everything up to the shortage in [fTurns]
		else //if( pRes->GetNext() < pRes->m_fCurrent )
		{
			//float fDelta = pRes->GetNext() - pRes->m_fCurrent;
			float fTurnsToShortage = pRes->m_fCurrent / -pRes->m_fDelta;
			if( fTurnsToShortage > 10.0f )
			{
				float fAmountToSell = (fTurnsToShortage - 10.0f) * (-pRes->m_fDelta);
				fAmountToSell = min( fAmountToSell, pRes->m_fCurrent + pRes->m_fBuy - pRes->m_fSell - 10.0f );
				if( fAmountToSell > 0.0f )
				{
					SellResource( fAmountToSell, pMoneyState, pRes );
					pRes->RectifyBuyAndSellValues();
				}
			}
		}
	}
}

void SellResource( float fAmount, CAIResourceState* pMoneyState, CAIResourceState* pRes )
{
	ASSERT( fAmount > 0.0 );
	ASSERT( pRes->m_fCurrent + pRes->m_fBuy - pRes->m_fSell - fAmount > 9.0 );

	// If the resource amount if growing...
	float fNewSale  = fAmount;
	float fNewMoney = fNewSale * g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X(); // $1 for every resource sold
	pRes->m_fSell += fNewSale;
	pMoneyState->m_fBuy += fNewMoney;

	pMoneyState->RectifyBuyAndSellValues();
	pRes->RectifyBuyAndSellValues();
}
/*
void SellExcessResources_MaxAndMinimumDelta( float fMax, float fMinimumDelta, CAIResourceState* pMoneyState, CAIResourceState* pRes )
{
	// If the resource amount if growing...
	if( pRes->m_fCurrent > fMax && pRes->GetNext() > pRes->m_fCurrent+fMinimumDelta )
	{
		// Example: If fMax is 300, sell all excess resources above 300
		//ASSERT( pRes->m_fSell == 0.0f );
		float fNewSale  = pRes->m_fCurrent-fMax;
		float fNewMoney = fNewSale * OPENMARKET_MONEY_FOR_SELLING_RESOURCE; // $1 for every resource sold
		pRes->m_fSell += fNewSale;
		pMoneyState->m_fBuy += fNewMoney;
	}
}
*/
void BuyNeededResource( CAIResourceState* pMoneyState, CAIResourceState* pRes )
{
	if( pRes->GetNext() < 10.0f && pRes->GetNext() < pRes->m_fCurrent )
	{
		float fAvailableMoney = min( pMoneyState->GetNext()-10.0f, pMoneyState->m_fCurrent-10.0f );
		if( fAvailableMoney < 0.0f ){ fAvailableMoney = 0.0f; }

		if( fAvailableMoney > 0.0f )
		{
			float fWantToBuyResource = 10.0f-pRes->GetNext();
			float fPrice = ((0.5f*g_pEOSAICommonData->OPENMARKET_MONEY_FOR_SELLING_RESOURCE_X())+(0.5f*g_pEOSAICommonData->OPENMARKET_COST_TO_BUY_RESOURCE_X()));
			float fWantToBuyResourceCost = fWantToBuyResource*fPrice;

			if( fWantToBuyResourceCost > fAvailableMoney )
			{
				fWantToBuyResource = fAvailableMoney / fPrice;
				fWantToBuyResourceCost = fWantToBuyResource*fPrice;
			}
			pRes->m_fBuy         += fWantToBuyResource;
			pMoneyState->m_fSell += fWantToBuyResourceCost;

			pRes->RectifyBuyAndSellValues();
			pMoneyState->RectifyBuyAndSellValues();
		}
	}
}

/*
void CEOSAIBrain::ClearOldData()
{
	m_AIThoughtDatabase.Clear();
	while( m_AIObjectList.IsEmpty() == FALSE ){ delete m_AIObjectList.RemoveHead(); }
	m_AIPoiObjects.RemoveAll();
	ClearUnitOrders();
	ClearCityBuildOrders();
}
*/
/*
void CEOSAIBrain::ClearUnitOrders()
{
	// Clear unit orders
	long iAIPlayer = GetAIPlayerNumber();
	POSITION pos = m_pWorldDescPlayer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pWorldDescPlayer->GetPoiList()->GetNext( pos )->GetPtr();
		CPoiMobile* pPoiMobile = dynamic_cast< CPoiMobile* >( pPoi );
		if( pPoiMobile &&
			pPoiMobile->GetOwner() == iAIPlayer &&
			pPoiMobile->GetOrderList()->IsEmpty() == false ) // >GetFirstOrder() )
		{
			// Delete the old orders
			pPoiMobile->DeleteOrders();

			// we might've deleted a PoiPD, which would invalidate [pos]
			pos = m_pWorldDescPlayer->GetPoiList()->GetHeadPosition();
		}
	}
}

void CEOSAIBrain::ClearCityBuildOrders()
{
	/-*
	POSITION pos = GetAIThoughtDatabase()->GetMyCities()->GetHeadPosition();
	while( pos )
	{
		CEOSAICity* pAICity = GetAIThoughtDatabase()->GetMyCities()->GetNext( pos );
		pAICity->GetCity()->RemoveAllItemsFromBuildQueue();
	}
	*-/
	// Clear unit orders
	long iAIPlayer = GetAIPlayerNumber();
	POSITION pos = m_pWorldDescPlayer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = m_pWorldDescPlayer->GetPoiList()->GetNext( pos )->GetPtr();
		CCity* pCity = dynamic_cast< CCity* >( pPoi );
		if( pCity &&
			pCity->GetOwner() == iAIPlayer )
		{
			pCity->RemoveAllItemsFromBuildQueue();
		}
	}
}
*/

#ifdef DO_NOT_COMPILE_PUT_IN_MAIN_GAME
void CEOSAIBrain::TurnPoiObjectsIntoAIObjects()
{
	//ASSERT( m_pWorldDescPlayer );
	ASSERT( m_AIPoiObjects.IsEmpty() );
	//ASSERT( m_AIObjectList.IsEmpty() );
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	float fCurrentTime = pWorldDescServer->GetGameTime().GetTime();

	POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pServerPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
		if( pServerPoi->IsDead() ) continue; // Don't turn dead poi into AIObjects

		CPoiObject* pServerPoiObject = dynamic_cast< CPoiObject* >( pServerPoi );
		if( pServerPoiObject == NULL ) continue;
		pServerPoiObject->SetAIPoiObject( NULL );

		//CPoiObject* pPlayerPoiObject = GetWorldDescServer()->GetPoiObject( pServerPoiObject->GetObjectId() );
		//if( pPlayerPoiObject ){ pPlayerPoiObject->SetAIPoiObject( NULL ); }

		CCity* pCity = dynamic_cast< CCity* >( pServerPoiObject );
		if( pCity )
		{
			CEOSAICity* pAICity = new CEOSAICity( this );
			pAICity->SetServerPoiObject( pServerPoiObject );
			this->AddAIObject( pAICity );

			// INCOMPLETE: Create UnitSymbols for the items in production
			/*
			if( pAICity->IsMine() )
			{
				int p=0;
				// NOTE: neither GetBuildCompletionList() nor GetBuildOrdersList()
				//   will give me quite what I want (unless I combine them and do
				//   calculations myself, like the CCityDialog does).
				//  I might want to create a CCity method that generates the information
				//   and passes back a list (probably of CBuildCompletion objects).
				/*
				POSITION pos = pCity->GetBuildCompletionList()->GetHeadPosition();
				while( pos )
				{
					CBuildCompletion* pBuildCompletion = pCity->GetBuildCompletionList()->GetNext( pos );
					if( pBuildCompletion->
				}
				*-/
			}
			*/
		}
		CResourceSource* pResourceSource = dynamic_cast< CResourceSource* >( pServerPoiObject );
		if( pResourceSource )
		{
			CEOSAIResource* pAIResource = new CAIResource( this );
			pAIResource->SetServerPoiObject( pServerPoiObject );
			this->AddAIObject( pAIResource );
		}
		CAirfield* pAirfield = dynamic_cast< CAirfield* >( pServerPoiObject );
		if( pAirfield )
		{
			CEOSAIAirfield* pAIAirfield = new CEOSAIAirfield( this );
			pAIAirfield->SetServerPoiObject( pServerPoiObject );
			this->AddAIObject( pAIAirfield );
		}
		CUnit* pUnit = dynamic_cast< CUnit* >( pServerPoiObject );
		if( pUnit )
		{
			CEOSAIUnit2* pAIUnit = new CEOSAIUnit2( this );
			pAIUnit->SetServerPoiObject( pServerPoiObject );
			this->AddAIObject( pAIUnit );
		}
	}

	// Setup desires
	pos = GetAIObjectList()->GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = GetAIObjectList()->GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIObject );
		if( pAIUnit )
		{
			pAIUnit->CreatePersonalDesires();
		}
	}

	// Now setup the container information
	pos = GetAIObjectList()->GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = GetAIObjectList()->GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIObject );
		if( pAIUnit )
		{
			// void SetContainer( CEOSAIBrain* pAIPlan, long iContainer );

			if( pAIUnit->GetServerUnit() &&
				pAIUnit->GetServerUnit()->GetAirBase() )
			{
				CAIObject* pAIObject = this->GetAIObject( pAIUnit->GetServerUnit()->GetAirBase()->ObjectId() );
				if( pAIObject )
				{
					CEOSAIPoiObject* pAIAirbase = dynamic_cast< CEOSAIPoiObject* >( pAIObject );
					//ASSERT( pAIAirbase );
					//pUnitSymbol->SetInitialAirbase( this, pUnitSymbol->GetUnit()->GetAirBase()->ObjectId() );
					pAIUnit->SetInitialAirbase( pAIAirbase );
				}
			}
			if( pAIUnit->GetServerUnit() &&
				pAIUnit->GetServerUnit()->GetContainer() )
			{
				CAIObject* pAIObject = this->GetAIObject( pAIUnit->GetServerUnit()->GetContainer()->ObjectId() );
				if( pAIObject )
				{
					CEOSAIPoiObject* pAIContainer = dynamic_cast< CEOSAIPoiObject* >( pAIObject );
					//pUnitSymbol->SetInitialContainer( this, pUnitSymbol->GetUnit()->GetContainer()->ObjectId() );
					pAIUnit->SetInitialContainer( pAIContainer );
				}
			}

			//CEOSAIUnit2* pUnitSymbol = new CEOSAIUnit2( m_pCurrentPlan, pUnit->GetOwner(), pUnit );
			//m_pCurrentPlan->AddPoiObjectSymbol( pUnitSymbol );
		}
	}
}
#endif

/*
void  CEOSAIBrain::AddAIObjectsToRegions()
{
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		//
		CEOSAIRegion2* pAIRegion = GetSpatialMap()->GetRegion( pAIPoiObject->GetInitialState()->GetLocation() );
		ASSERT( pAIRegion );
		if( pAIRegion )
		{
			pAIRegion->AddAIPoiObjectToRegion( pAIPoiObject );
		}
	}
}
*/

/*
void CEOSAIBrain::FindAllObjectsWithinArea( CString strLogicalObjectType, CEOSAILocation Center, EnumMovementType MovementType, float fDistance, CList< CAIObject* >* pResultsList )
{
	POSITION pos = m_AIObjectList.GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_AIObjectList.GetNext( pos );
		if( pAIObject->IAmOfLogicalObjectType( strLogicalObjectType ) )
		{
			float fDistance2 = m_pWorldDescPlayer->GetPixelDistance( Center, pAIObject->GetInitialLocation() );
			if( fDistance2 < fDistance )
			{
				// Can I get there?
				bool bCanIGetThere = false;
				if( MovementType == EOSAIEnumMovementType3_Air )
				{
					bCanIGetThere = true;
				}
				else
				{
					float fDist = m_SpatialMap.GetPixelDistance( Center, pAIObject->GetInitialLocation(), MovementType, 0 );
					if( fDist < 1000000.0f )
					{
						bCanIGetThere = true;
					}
				}

				pResultsList->AddTail( pAIObject );
			}
		}
	}
}
*/

/*
float CEOSAIBrain::GetUnexploredCountOfAllObjectsWithinPath( CEOSAILocation Start, float fViewRange, CEOSAILocation End )
{
	float fDistanceBetweenStartAndEnd = m_pWorldDescPlayer->GetPixelDistance( Start, End );
	float fVolume = 0.0f;

	long iCount = 0;
	POSITION pos = m_AIObjectList.GetHeadPosition();
	while( pos )
	{
		iCount++;
		CAIObject* pAIObject = m_AIObjectList.GetNext( pos );
		CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pAIObject );
		if( pAIRegion && pAIRegion->HasBeenExplored() == false )
		{
			// INCOMPLETE: this calculation isn't quite right.  It's a little larger than
			//   it should be.
			float fDistance1 = m_pWorldDescPlayer->GetPixelDistance( Start, pAIRegion->GetLocation() );
			float fDistance2 = m_pWorldDescPlayer->GetPixelDistance( End, pAIRegion->GetLocation() );
			bool  bAddIt = false;
			if( fDistance1 < fViewRange )
			{
				bAddIt = true;
			}
			if( fDistance2 < fViewRange )
			{
				bAddIt = true;
			}
			if( fDistance1 + fDistance2 < fViewRange + sqrt( fViewRange*fViewRange + fDistanceBetweenStartAndEnd*fDistanceBetweenStartAndEnd ) )
			{
				bAddIt = true;
			}
			if( bAddIt )
			{
				fVolume += pAIRegion->GetSizeOfRegion();
			}
		}
	}
	return fVolume;
}
*/
void  CEOSAIBrain::UpdateNationalInProductionSummary()
{
	m_InProductionMilitaryCapability.Clear();

	long iAIPlayer = this->GetAIPlayer()->GetPlayerNumber();

	// Iterate over the cities, read their "in production" information
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity && pAICity->GetOwner() == iAIPlayer )
		{
			if( pAICity->GetAIBuildOrders()->IsEmpty() == false )
			{
				//CEOSAIUnitTemplate* pAIUnitTemplate = pAICity->GetAIBuildOrders()->GetHead()->GetAIBuildOption()->GetAIUnitTemplate();
				CEOSAIUnitTemplate* pAIUnitTemplate = pAICity->GetAIBuildOrders()->GetHead()->GetAIUnitTemplate();

				// INCOMPLETE:
				// This needs to be modified by the time-until-completion
				//   Because a carrier done in 5 turns is different than one done in 50
				//CEOSAIUnitSummary UnitSummary;
				//UnitSummary.Set( pAICity->GetProductionTask()->m_pBuildOption->GetUnitTemplate() );
				//m_InProductionMilitaryCapability += UnitSummary;
				//CString strUnitTemplate = pAICity->GetProductionTask()->m_pBuildOption->GetUnitTemplate()->GetInternalName();
				//CEOSAIUnitTemplate* pAIUnitTemplate = m_AIThoughtDatabase.GetAIUnitTemplate( strUnitTemplate );
				//m_InProductionMilitaryCapability.AddToSummary( pAIUnitTemplate->GetUnitTemplate() );
				if( pAIUnitTemplate )
				{
					m_InProductionMilitaryCapability.AddToSummary( pAIUnitTemplate );
				}
			}
		}
	}
}

void  CEOSAIBrain::UpdateNationalUnsatisfiedMilitaryCapability()
{
	m_UnsatisfiedMilitaryCapability = m_InitialNeed - m_CurrentMilitaryCapability;

	// Subtract the InProduction values
	m_UnsatisfiedMilitaryCapability -= m_InProductionMilitaryCapability*0.5f;

	m_UnsatisfiedMilitaryCapability.NothingBelowZero();

	// Add a little bit of the InitialNeed
	//CAIMilitarySummary NationalInitialNeed = m_InitialNeed;
	//NationalInitialNeed.Multiply( 0.1f );
	//m_UnsatisfiedMilitaryCapability += NationalInitialNeed;
	m_UnsatisfiedMilitaryCapability += m_InitialNeed*0.1f;
}

//
// Desires
//

/*
void CEOSAIBrain::CreateDesires()
{
	ASSERT( false );
	ASSERT( m_Desires.IsEmpty() );
	POSITION pos = GetAIObjectList()->GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = GetAIObjectList()->GetNext( pos );

		CEOSAICity* pAICitySymbol = dynamic_cast< CEOSAICity* >( pAIObject );
		CEOSAIAirfield* pAIAirfield = dynamic_cast< CEOSAIAirfield* >( pAIObject );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIObject );
		CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pAIObject );
		CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIObject );

		if( pAICitySymbol )
		{
			if( pAICitySymbol->IsMine() )
			{
				pAICitySymbol->CalculateUnexploredArea();
				AddDesire( new CAIDesire_ExploreSurroundingsByAir( this, pAICitySymbol ) );
			}
			else
			{
				AddDesire( new CAIDesire_CaptureCity( this, pAICitySymbol ) );
			}
		}
		if( pAIResource && pAIResource->IsMine() == false )
		{
			AddDesire( new CAIDesire_CaptureResource( this, pAIResource ) );
		}
		if( pAIAirfield && pAIAirfield->IsMine() )
		{
			pAIAirfield->CalculateUnexploredArea();
			AddDesire( new CAIDesire_ExploreSurroundingsByAir( this, pAIAirfield ) );
		}

		if( pAIUnit && pAIUnit->IsMine() == false )
		{
			AddDesire( new CAIDesire_DestroyEnemy( this, pAIUnit ) );
		}

		if( pAIRegion && pAIRegion->HasBeenExplored() == false )
		{
			AddDesire( new CAIDesire_ExploreRegion( this, pAIRegion ) );
		}
	}

	pos = m_AIGeoList.GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pAIGeo = m_AIGeoList.GetNext( pos );
		CAIDesire_ControlGeo* pDesire = new CAIDesire_ControlGeo( this, pAIGeo );
		AddDesire( pDesire );
		m_ControlGeoDesireList.AddTail( pDesire );
	}

	// Group desires together (e.g. if an enemy unit is inside an enemy city)
	POSITION pos1 = m_Desires.GetHeadPosition();
	while( pos1 )
	{
		CAIDesire* pDesire = m_Desires.GetNext( pos1 );
		if( pDesire->ContainsTargetInformation() )
		{
			if( pDesire->GetTargetAIObject() &&
				pDesire->GetTargetAIObject()->GetPoiObject() )
			{
				CPoiObject* pTarget = pDesire->GetTargetAIObject()->GetPoiObject();
				if( pTarget->GetContainer() )
				{
					CPoiObject* pContainer = pTarget->GetContainer();
					CAIObject* pAIContainer = GetAIObject( pContainer->GetObjectId() );
					if( pAIContainer )
					{
						POSITION pos2 = pAIContainer->GetAttachedDesires()->GetHeadPosition();
						while( pos2 )
						{
							CAIDesire* pAIContainerDesire = pAIContainer->GetAttachedDesires()->GetNext( pos2 );
							pAIContainerDesire->AddSubDesire( pDesire );
							pDesire->m_bIsASubDesire = true;
						}
					}
				}
			}
		}
	}

	//
	pos = m_ControlGeoDesireList.GetHeadPosition();
	while( pos )
	{
		CAIDesire_ControlGeo* pControlGeoDesire = m_ControlGeoDesireList.GetNext( pos );
		//
		// INCOMPLETE: Pick and choose which geos I am going to try to control.
		//
		//if( pControlGeoDesire->
		pControlGeoDesire->m_bIAmGoingToTryToControlThisGeo = true;
	}

	int z=0;
}

void CEOSAIBrain::AddDesire( CAIDesire* pDesire )
{
	//pDesire->AttachToTarget();
	m_Desires.InsertSorted( pDesire );
}
*/
//
// Processing
//
/*
void CEOSAIBrain::CreateAPlan()
{
	CEOSAIStopwatch2 watch;
	watch.Start();

	//delete m_pAIThoughtDatabase;
	//m_pAIThoughtDatabase = NULL;
	//m_pAIThoughtDatabase = new CEOSAIThoughtDatabase( this );

	//UpdateTransportSpeeds();
	//CreateStrategiesForGeos();

	// How do I start the thought process?

	// Look at the CitRes that I don't own, consider capturing them;
	// Look at the unexplored areas, consider exploring them for CitRes
	/*
	POSITION pos;
	pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		pAIPoiObject->ThinkAboutThisObject();
	}
	*-/
	//
}
*/
/*
void CEOSAIBrain::CreateAPlan_UsingDesiresAndActions()
{
	CEOSAIStopwatch2 watch;
	watch.Start();

	//UpdateTransportSpeeds();
	CreateStrategiesForGeos();

	float fPreviousScore = this->GetTotalScore();

	//
	// 1313 bytes in -19 Normal Blocks.
	//

	// Create Weakly Defined Desires
	/*
	POSITION pos = m_Desires.GetHeadPosition();
	while( pos )
	{
		CAIDesire* pDesire = m_Desires.GetNext( pos );
		pDesire->CreateWeaklyDefinedActions();
	}
	*-/

	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );

		/*
		CMemoryState MemoryState1;
		MemoryState1.Checkpoint();
		*-/

		pAIPoiObject->GetFinalState()->CreateWeakAndDefinedActions( pAIPoiObject, this );
		/*
		for( long i=0; i<pAIPoiObject->GetFinalState()->GetDecentWeaklyDefinedActionsList()->GetMaximumSize(); i++ )
		{
			CAIWeaklyDefinedAction* pAction = pAIPoiObject->GetFinalState()->GetDecentWeaklyDefinedActionsList()->m_Array[i];
			long z=0;
		}
		for( long i=0; i<pAIPoiObject->GetFinalState()->GetDecentActionsList()->GetMaximumSize(); i++ )
		{
			CAIDefinedAction* pAction = pAIPoiObject->GetFinalState()->GetDecentActionsList()->m_Array[i];
			long z=0;
		}
		delete pAIPoiObject;
		//Sleep( 5000 );

		CMemoryState MemoryState2;
		MemoryState2.Checkpoint();
		CMemoryState DiffState;
		DiffState.Difference( MemoryState1, MemoryState2 );
		DiffState.DumpStatistics();
		MemoryState1.DumpAllObjectsSince();
		long z=0;
		*-/
		/*
		if( pAIPoiObject->IsMine() &&
			pAIPoiObject->IsDead() == false &&
			pAIPoiObject->GetFinalState()->GetWeaklyDefinedActionsWereCreated() == false )
		{
			pAIPoiObject->GetFinalState()->InvokeDecentWeaklyDefinedActions( pAIPoiObject, this );

			// Create DefinedActions
			m_bCreateDirectActions = true;
			m_bCreateIndirectActions = false;
			m_bCreateTransportActions = false;
			m_bCreateBuildActions = false;
			pAIPoiObject->GetFinalState()->CreateDecentActionsFromWeaklyDefinedActions( pAIPoiObject, this );

			m_bCreateDirectActions = false;
			m_bCreateIndirectActions = true;
			m_bCreateTransportActions = false;
			m_bCreateBuildActions = false;
			pAIPoiObject->GetFinalState()->CreateDecentActionsFromWeaklyDefinedActions( pAIPoiObject, this );

			//pAIPoiObject->GetFinalState()->SetDefinedActionsHaveBeenCreated( true );
		}
		*-/
	}

	//
	// 466275 bytes in 2228 Normal Blocks.
	//
	//return;


	// Iterate over the AIObject, collecting the decent actions into a single, ordered list
	//   so I can start scheduling them
	//m_BestAvailableActions.SetSortFunction( SortDesiresByScore );
	/*
	pos = m_AIObjectList.GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_AIObjectList.GetNext( pos );
		for( long i=0; i<pAIObject->GetFinalState()->GetDecentActionsList()->GetMaximumSize(); i++ )
		{
			CAIDefinedAction* pAction = pAIObject->GetFinalState()->GetDecentActionsList()->m_Array[i];
			if( pAction )
			{
				m_BestAvailableActions.InsertSorted( pAction );
			}
		}
	}
	*-/

	long iActionsAttemptedToSchedule = 0;
	long iActionsScheduled = 0;
	for( long iPass=0; iPass<1; iPass++ )
	{
		// Attempt to schedule it
		float fLastScore = 0.0f;
		pos = m_BestAvailableActions.GetHeadPosition();
		while( pos )
		{
			CAIDefinedAction* pAction = m_BestAvailableActions.GetNext( pos );
			ASSERT( pAction->ActionInitialStateMatchesActorCurrentState() );

			#ifdef _DEBUG
			long iCount = (long) m_BestAvailableActions.GetCount();
			float fPlanScore = GetTotalScore();
			float fActionScore = pAction->GetScore();
			if( fLastScore != 0.0f )
			{
				ASSERT( fLastScore > fActionScore );
			}
			long iNumberOfScheduledActions = pAction->GetActors_UpperNumberOfScheduledActions();
			#endif

			#ifdef _DEBUG
			{
				
				POSITION pos2 = m_BestAvailableActions.GetHeadPosition();
				while( pos2 )
				{
					CAIDefinedAction* pAct1 = m_BestAvailableActions.GetNext( pos2 );
					//CAIDefinedAction* pAct1 = dynamic_cast< CAIDefinedAction_1Actor* >( pAction );
					if( pAct1 )
					{
						ASSERT( pAct1->GetActor()->GetFinalState()->GetLocation() ==
								pAct1->GetMemoryOfActorInitialState()->GetLocation() );
						ASSERT( pAct1->GetActor()->GetFinalState()->GetTime() ==
								pAct1->GetMemoryOfActorInitialState()->GetTime() );
						int z=0;
					}
				}
				
			}
			#endif

			EOSAI::PoiMobile* pTank = dynamic_cast< EOSAI::PoiMobile* >( GetAIObject( 18 ) );
			if( pAction->GetActor()->GetObjectId() == 81 &&
				dynamic_cast< CAIDefinedAction_CaptureCity* >( pAction ) )
			{
				int z=0;
			}

			ASSERT( pAction->ActionInitialStateMatchesActorCurrentState() );

			// INCOMPLETE: if it doesn't get scheduled, what should happen?
			//   Should I flag this action as "not high enough score"?
			//   I used to delete it, but I don't do that now
			if( pAction->GetActor()->GetNumberOfScheduledActions() < 5 )
			{
				iActionsAttemptedToSchedule++;
				ASSERT( pAction->ActionInitialStateMatchesActorCurrentState() );
				if( pAction->TryToScheduleThisAction() )
				{
					iActionsScheduled++;
					CEOSAIPoiObject* pActor = dynamic_cast< CEOSAIPoiObject* >( pAction->GetActor() );
					if( pActor )
					{
						pActor->UpdateTaskProgression();
						pActor->GetFinalState()->CreateWeakAndDefinedActions( pActor, this );
					}

					// It was successfully scheduled.  This probably messed up my list, so reset my iterator
					pos = m_BestAvailableActions.GetHeadPosition();
				}
			}

			/*
			It seems that the DefinedAction is being created before the end Action is updated.
			    Do I need to recalculate PotentialAction times?
			    Maybe I am adjusting the end-time of a scheduled action, but not updating the times of the potential actions associated with it.

			Types of schedule changes:
				Append a DefinedAction, AITasks to an AIObject
				Remove a tail DefinedAction, AITasks from an AIObject
				Insert a DefinedAction, AITasks - update all trailing DefinedActions, Potential Actions
				Remove a DefinedAction, AITasks - update all trailing DefinedActions, Potential Actions
			*-/

			#ifdef _DEBUG
			{
				
				POSITION pos2 = m_BestAvailableActions.GetHeadPosition();
				while( pos2 )
				{
					CAIDefinedAction* pAct1 = m_BestAvailableActions.GetNext( pos2 );
					//CAIDefinedAction_1Actor* pAct1 = dynamic_cast< CAIDefinedAction_1Actor* >( pActionB );
					if( pAct1 )
					{
						CAIObject* pAIObject = pAct1->GetActor();
						ASSERT( pAct1->GetActor()->GetFinalState()->GetLocation() ==
								pAct1->GetMemoryOfActorInitialState()->GetLocation() );
						ASSERT( pAct1->GetActor()->GetFinalState()->GetTime() ==
								pAct1->GetMemoryOfActorInitialState()->GetTime() );
						//pAction->TryToScheduleThisAction();
						int z=0;
					}
				}
				
			}
			#endif

			/*
			EOSAI::PoiMobile* pPoiMobileActor = dynamic_cast< EOSAI::PoiMobile* >( pAction->GetPrimaryActor() );
			if( pPoiMobileActor )
			{
				//EOSAI::UnitTask_MoveTo* pMoveToTask = new EOSAI::UnitTask_MoveTo( pPoiMobileActor, NULL, pPoiMobileActor->GetInitialState(), CEOSAILocation( Pixel,950,600 ) );
				//pActor->GetFinalState()->CreateWeakAndDefinedActions( pActor, this );
				long iObjectId = pPoiMobileActor->GetObjectId();

				EOSAI::UnitTask_MoveTo* pMoveToTask = new EOSAI::UnitTask_MoveTo( pPoiMobileActor, NULL, pPoiMobileActor->GetInitialState(), CEOSAILocation( Pixel,950,700 ) );
				pMoveToTask->SetStartTime( pPoiMobileActor->GetInitialTime() );
				//pMoveToTask->IsTheFinalTaskInTheAction( true );
				//this->m_SubTasks.AddTail( pMoveToTask );
				pPoiMobileActor->InsertTask( 0, pMoveToTask );
				//pPoiMobileActor->UpdateTaskProgression();
				long x=0;
				//ValidateDetails();
			}
			*-/

			//
			/*
			#ifdef _DEBUG
			CAIObject* pActor = pAction->GetPrimaryActor();
			pActor->UpdateTaskProgression();
			#endif
			*-/
		}
	}

	// Iterate over the desires and construct actions
	for( long iPass=0; iPass<0; iPass++ )
	{
		//POSITION pos;

		/*
		pos = m_AIPoiObjects.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );

			if( pAIPoiObject->IsMine() &&
				pAIPoiObject->IsDead() == false &&
				pAIPoiObject->GetFinalState()->GetWeaklyDefinedActionsWereCreated() == false )
			{
				pAIPoiObject->GetFinalState()->InvokeDecentWeaklyDefinedActions( pAIPoiObject, this );

				// Create DefinedActions
				m_bCreateDirectActions = true;
				m_bCreateIndirectActions = false;
				m_bCreateTransportActions = false;
				m_bCreateBuildActions = false;
				pAIPoiObject->GetFinalState()->CreateDecentActionsFromWeaklyDefinedActions( pAIPoiObject, this );

				m_bCreateDirectActions = false;
				m_bCreateIndirectActions = true;
				m_bCreateTransportActions = false;
				m_bCreateBuildActions = false;
				pAIPoiObject->GetFinalState()->CreateDecentActionsFromWeaklyDefinedActions( pAIPoiObject, this );

				//pAIPoiObject->GetFinalState()->SetDefinedActionsHaveBeenCreated( true );
			}
		}
		*-/

		/*
		pos = m_AIPoiObjects.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
			pAIPoiObject->ClearDecentWeaklyDefinedActions();
		}
		
		pos = m_Desires.GetHeadPosition();
		while( pos )
		{
			CAIDesire* pDesire = m_Desires.GetNext( pos );
			pDesire->DeleteWeaklyDefinedActions();
			pDesire->CreateWeaklyDefinedActions();
		}
		*-/

		/*
		m_bCreateDirectActions = true;
		m_bCreateIndirectActions = false;
		m_bCreateTransportActions = false;
		m_bCreateBuildActions = false;

		// Create only non-transport tasks (at first)
		pos = m_Desires.GetHeadPosition();
		while( pos )
		{
			CAIDesire* pDesire = m_Desires.GetNext( pos );
			pDesire->CreateDecentActionsFromWeaklyDefinedActions();
		}
		*-/

		// DEBUG
		/*
		#ifdef _DEBUG
		CEOSAILimitedList< CAIDefinedAction* >  BestActions;
		BestActions.IOwnTheObjects( false );
		BestActions.SetMaximumSize( 50 );
		pos = m_Desires.GetHeadPosition();
		while( pos )
		{
			CAIDesire* pDesire = m_Desires.GetNext( pos );
			POSITION pos2 = pDesire->GetDecentActionsList()->GetHeadPosition();
			while( pos2 )
			{
				CAIDefinedAction* pDefinedAction = pDesire->GetDecentActionsList()->GetNext( pos2 );
				BestActions.Add( pDefinedAction, pDefinedAction->m_fScore );
			}
			/-*
			if( dynamic_cast< CAIDesire_CaptureCity* >( pDesire ) )
			{
				int x=0;
			}
			*-/
		}
		CAIDefinedAction_CaptureCity* pBestCaptureCityAction = NULL;
		for( long i=0; i<50; i++ )
		{
			CAIDefinedAction* pAction = BestActions.m_Array[i];
			if( dynamic_cast< CAIDefinedAction_CaptureCity* >( pAction ) )
			{
				pBestCaptureCityAction = dynamic_cast< CAIDefinedAction_CaptureCity* >( pAction );
				break;
			}
		}
		CAIDefinedAction* pDummyAction = NULL;

		int z=0;
		#endif
		*-/

		// Now try to create some TransportTasks
		/*
		m_bCreateDirectActions = false;
		m_bCreateIndirectActions = true;
		m_bCreateTransportActions = false;
		m_bCreateBuildActions = false;
		pos = m_Desires.GetHeadPosition();
		while( pos )
		{
			CAIDesire* pDesire = m_Desires.GetNext( pos );
			pDesire->CreateDecentActionsFromWeaklyDefinedActions();
			pDesire->CreateSomeHypotheticalActionsToSatisfyThisDesire();
		}
		*-/

		// Construct the best logical action
		//AttemptToApplyActionDefinitions( GetDecentLogicalActions() );
		//AttemptToApplyActionDefinitions();
		AttemptToScheduleDefinedActions();

		float fCurrentScore = this->GetTotalScore();
		fPreviousScore = fCurrentScore;
	}


	// Pass over each of my AIObjects and see if I can construct a task for it
	/*
	for( long iPass=0; iPass<0; iPass++ )
	{
		//CEOSAILimitedList< CAIDefinedAction* >  DecentLogicalActions;
		//DecentLogicalActions.IOwnTheObjects( true );
		//DecentLogicalActions.SetMaximumSize( 10 );
		m_bCreateNonTransportTasks = true;
		m_bCreateTransportTasks = true;
		m_bCreateBuildTasks = false;

		POSITION pos = GetAIObjectList()->GetHeadPosition();
		while( pos )
		{
			CAIObject* pAIObject = GetAIObjectList()->GetNext( pos );

			if( pAIObject->GetObjectId() == 21 )
			{
				int z=0;
			}

			if( dynamic_cast< EOSAI::PoiMobile* >( pAIObject ) )
			{
				POSITION pos = m_Desires.GetHeadPosition();
				while( pos )
				{
					CAIDesire* pDesire = m_Desires.GetNext( pos );
					pDesire->CreateSomeHypotheticalActionsToSatisfyThisDesire( pAIObject );
				}
				//AttemptToApplyActionDefinitions( GetDecentLogicalActions() );
				AttemptToApplyActionDefinitions();
			}
		}
	}
	*-/

	// Mainly deals with ships and aircraft
	//EvaluateNationalNeeds();
	//EvaluateCurrentMilitaryCapability();
	//CalculateUnsatisfiedMilitaryCapability();

	CAIObject* pTank = GetAIObject( 18 );
	int zz=0;

	// Create Build Orders for cities that haven't been assigned any yet
	m_BuildManager.PlanBuildActions();
	/*
	pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			// Handle landlocked cities first
			if( pAICity->IsMine() && pAICity->GetCity()->IsOnAMajorBodyOfWater() == false )
			{
				pAICity->PlanWhatToBuild();
			}
		}
	}
	pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			// Handle coastal cities
			if( pAICity->IsMine() && pAICity->GetCity()->IsOnAMajorBodyOfWater() )
			{
				pAICity->PlanWhatToBuild();
			}
		}
	}
	*-/
	// Make sure all my cities have build orders
	pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity && pAICity->IsMine() )
		{
			// Handle coastal cities
			ASSERT( pAICity->GetProductionTask() != NULL );
		}
	}

/*
	There is a problem with the artillery - it starts with orders to capture the Gold mines.
		It looses those actions and ends up with an explore action way off in the distance
		(near the last action)
	I need to clear all a unit's actions when I withdraw one earlier in the queue
*-/
	watch.Stop();
	if( Public::m_bShowAIPlayerTimes )
	{
		CString strText;
		strText.Format( "CEOSAIBrain::Process() time: %f milliseconds", watch.GetDeltaMilliseconds() );
		AfxMessageBox( strText );
	}

	// Run through the objects and make sure their times and durations look right
	#ifdef _DEBUG
	{
		POSITION pos = GetAIObjectList()->GetHeadPosition();
		while( pos )
		{
			CAIObject* pAIObject = GetAIObjectList()->GetNext( pos );
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIObject );
			if( pAIUnit )
			{
				long iObjectId = pAIUnit->GetUnit()->ObjectId();
				CString strTemplateName = pAIUnit->GetUnitTemplate()->GetInternalName();
				int z=0;
				if( strTemplateName == "Tank (Class 1)" )
				{
					int z=0;
				}
				if( pAIUnit->GetObjectId() == 20 )
				{
					int z=0;
				}
			}
		}
	}
	#endif _DEBUG

	// Instanciate orders
	//  INCOMPLETE: the orders have to know what sequence they appear in - and preserve that order
	/*
	bool bAllOrdersWereCreated = false;
	while( !bAllOrdersWereCreated )
	{
		bAllOrdersWereCreated = true;
		POSITION pos = GetAIObjectList()->GetHeadPosition();
		while( pos )
		{
			CAIObject* pAIObject = GetAIObjectList()->GetNext( pos );
			bool bVal = pAIObject->CreateOrders();
			if( bVal == false )
			{
				bAllOrdersWereCreated = false;
			}
		}
	}
	*-/
}
*/

/*
void CEOSAIBrain::GenerateAIGeos()// CAISpatialMap* pSpatialMap )
{
	ASSERT( m_pAIPlayer->GetWorldDescPlayer()->GetGeoList()->IsEmpty() == FALSE );

	// How many land geos are there?
	POSITION pos = m_pAIPlayer->GetWorldDescPlayer()->GetGeoList()->GetHeadPosition();
	while( pos )
	{
		CGeo* pGeo = m_pAIPlayer->GetWorldDescPlayer()->GetGeoList()->GetNext( pos );

		CEOSAIGeo* pAIGeo = new CEOSAIGeo( this, pGeo );
		m_AIGeoList.AddTail( pAIGeo );

		/-*
		if( pGeo->GetGeoType() == CGeo::Land )
		{
			CGeoLand* pGeoLand = dynamic_cast< CGeoLand* >( pGeo );
			ASSERT( pGeoLand );

			CEOSAIGeoLand* pAIGeoLand = new CEOSAIGeoLand( this, pGeoLand );

			m_AIGeoLandList.AddTail( pAIGeoLand );
		}
		*-/
	}
	// Iterate over the MultiRegions, add to the AIGeo's list of MultiRegions
	for( long i=0; i<m_MultiRegionManager.GetMultiRegionArray()->m_iArraySize; i++ )
	{
		CMultiRegion* pMultiRegion = m_MultiRegionManager.GetMultiRegionArray()->Get( i );
		if( pMultiRegion )
		{
			long iGeo = pMultiRegion->m_pCenterRegion->m_iGeo;
			CEOSAIGeo* pAIGeo = GetAIGeo( iGeo );
			pAIGeo->AddMultiRegion( pMultiRegion );
			pMultiRegion->SetAIGeo( pAIGeo );
		}
	}
	int z=0;
}

CEOSAIGeo* CEOSAIBrain::GetAIGeo( long iGeoId )
{
	POSITION pos = m_AIGeoList.GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pAIGeo = m_AIGeoList.GetNext( pos );
		if( pAIGeo->GetGeo()->GetGeoNumber() == iGeoId )
		{
			return pAIGeo;
		}
	}
	return NULL;
}
*/
/*
CEOSAIRegionManager2*  CEOSAIBrain::GetAIRegionManager()
{
	return GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetAIRegionManager();
}

CEOSAIMultiRegionManager2* CEOSAIBrain::GetMultiRegionManager()
{
	ASSERT( GetCommonState()->GetWorldDescServer() );
	ASSERT( GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager() );
	ASSERT( GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager()->GetNumberOfMultiRegions() > 0 );

	return GetCommonState()->GetWorldDescServer()->GetAICommonData()->GetMultiRegionManager();
}
*/
/*
CEOSAIGeoLand* CEOSAIBrain::GetAIGeoLand( long iGeoId )
{
	POSITION pos = m_AIGeoLandList.GetHeadPosition();
	while( pos )
	{
		CEOSAIGeoLand* pAIGeoLand = m_AIGeoLandList.GetNext( pos );
		if( pAIGeoLand->GetGeo()->GetGeoNumber() == iGeoId )
		{
			return pAIGeoLand;
		}
	}
	return NULL;
}
*/

/*
void CEOSAIBrain::CreateStrategiesForGeos()
{
	CList< CEOSAIGeoLand* > FullyExploredFullyCapturedAIGeos; // Geos that I have complete control over
	CList< CEOSAIGeoLand* > PartiallyExploredFullyCapturedAIGeos; // Geos that I need to consolidate, no apparent enemy

	// Look over the AIGeos and figure out what my strategy will be for each of them
	POSITION pos = GetAIGeoList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIGeo* pAIGeo = GetAIGeoList()->GetNext( pos );

		float m_fSendCitResHuntersToThisGeo = 0.0f;
		float m_fSendCombatUnitsToInvadeThisGeo = 0.0f;
		float m_fGeneralSupport = 0.0f;
		float m_fCaptureLocalUnownedCitRes = 0.0f;
		float m_fExploreThisGeo = 0.0f;
		float m_fPrepareAttackLocalEnemy = 0.0f;
		float m_fDefendAgainstLocalEnemy = 0.0f;
		float m_fPrepareAgainstPossibleInvasion = 0.0f;

		// I need to score them and figure out what to do with each of them
		//   This planning also has to take into account the war-peace positions relative to other players
		/*
		if( pAIGeoLand->GetGeo()->GeoHasBeenFullyExplored() )
		{
			if( pAIGeoLand->GetMyCitResPresence() > 0 && pAIGeoLand->GetUn
		}
		*-/
		// For debug purposes, I'm going to set some variables
		m_fExploreThisGeo = 1.0f;

		//
		/*
		pAIGeoLand->SetDesire_SendCitResHuntersToThisGeo( m_fSendCitResHuntersToThisGeo );
		pAIGeoLand->SetDesire_SendCombatUnitsToInvadeThisGeo( m_fSendCombatUnitsToInvadeThisGeo );
		//pAIGeoLand->SetDesire_GeneralSupport( m_fGeneralSupport );
		//pAIGeoLand->SetDesire_CaptureLocalUnownedCitRes( m_fCaptureLocalUnownedCitRes );
		//pAIGeoLand->SetDesire_ExploreThisGeo( m_fExploreThisGeo );
		pAIGeoLand->SetDesire_PrepareAttackLocalEnemy( m_fPrepareAttackLocalEnemy );
		//pAIGeoLand->SetDesire_DefendAgainstLocalEnemy( m_fDefendAgainstLocalEnemy );
		pAIGeoLand->SetDesire_PrepareAgainstPossibleInvasion( m_fPrepareAgainstPossibleInvasion );
		*-/
	}
}
*/


void CEOSAIBrain::EvaluateNationalNeeds()
{
	//ASSERT( m_ControlGeoDesireList.IsEmpty() == FALSE );

	m_InitialNeed.Clear();

	// Look at the situation, and figure out what I should do
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	long iTurn = g_pEOSAIInterface->GetCurrentTurn();
	//
	float fMapSize = 1000.0f; ASSERT( false ); //m_pAIPlayer->GetWorldDescServer()->GetPixelHeight() * m_pAIPlayer->GetWorldDescServer()->GetPixelWidth();
	float fMyExpectedShareOfTheWorld = fMapSize / iNumberOfPlayers;

	float fGameStrategy_Expand = 0.0f;
	float fGameStrategy_Combat = 1.0f;

	if( iTurn < 100.0f*sqrt(fMyExpectedShareOfTheWorld/500000.0f) )
	{
		fGameStrategy_Combat = ((float)iTurn) / 100.0f*sqrt(fMyExpectedShareOfTheWorld/500000.0f);
		fGameStrategy_Expand = 1.0f - fGameStrategy_Combat;
	}

	m_InitialNeed.m_fAirEscort         = 1.0f * fGameStrategy_Expand + 5.0f * fGameStrategy_Combat;
	m_InitialNeed.m_fAntiAir           = 1.0f * fGameStrategy_Expand + 5.0f * fGameStrategy_Combat;
	m_InitialNeed.m_fAntiSubmarine     = 0.0f * fGameStrategy_Expand + 5.0f * fGameStrategy_Combat;
	m_InitialNeed.m_fBombardment       = 1.0f * fGameStrategy_Expand + 1.0f * fGameStrategy_Combat;
	m_InitialNeed.m_fExploration       = 1.0f * fGameStrategy_Expand + 1.0f; // geos usually handle this
	m_InitialNeed.m_fSeaResourceHunter = 3.0f * fGameStrategy_Expand + 3.0f * fGameStrategy_Combat;
	m_InitialNeed.m_fSeaSupremacy      = 1.0f * fGameStrategy_Expand + 5.0f * fGameStrategy_Combat;
	m_InitialNeed.m_fSighting          = 2.0f * fGameStrategy_Expand + 1.0f;
	m_InitialNeed.m_fStrategicBombing  = 0.0f * fGameStrategy_Expand + 5.0f * fGameStrategy_Combat;
	m_InitialNeed.m_fTrade             = 0.0f * fGameStrategy_Expand + 3.0f * fGameStrategy_Combat;
	m_InitialNeed.m_fGroundTransportCapability = 5.0f * fGameStrategy_Expand + 5.0f;

	//
	/*
	POSITION pos = m_ControlGeoDesireList.GetHeadPosition();
	while( pos )
	{
		CAIDesire_ControlGeo* pDesireToControlGeo = m_ControlGeoDesireList.GetNext( pos );
		if( pDesireToControlGeo->m_bIAmGoingToTryToControlThisGeo )
		{
			m_InitialNeed += pDesireToControlGeo->m_UnsatisfiedMilitaryCapability * 0.5f;
		}
	}
	*/

	//
	// INCOMPLETE: I should add more information into this.
	//    If I am in a land battle with an enemy (e.g. France against Germany), I probably
	//    don't need to worry about building transports.  That production should be spent
	//    on aircraft and ground units.
	// However, if I am on multiple geos, the transports might be useful for bringing ground units
	//    to the geo where the battle is happening.
	//
	// The need for transports should be dependent on the LandGeos and the need to move
	//    units around (both now and also for a possible invasion).
	// Note: some of these issues might not be problems as long as the AI is playing
	//    on a randomly generated map (i.e. lots of small islands->same general strategy)

	int z=0;
}

/*
void CEOSAIBrain::EvaluateCurrentMilitaryCapability()
{
	m_CurrentMilitaryCapability.Clear();

	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		if( pAIPoiObject )
		{
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
			if( pAIUnit )
			{
				// Get Unit Values
				CAIMilitarySummary UnitMilitaryCapability;
				UnitMilitaryCapability.Set( pAIUnit->GetUnitTemplate() );

				m_CurrentMilitaryCapability += UnitMilitaryCapability;
			}
			// Units in production
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity )
			{
				CAIProductionOption* pAIProductionOption = pAICity->GetProductionTask();
				if( pAIProductionOption )
				{
					CUnitTemplate* pUnitTemplate = pAIProductionOption->m_pBuildOption->GetUnitTemplate();
					if( pUnitTemplate )
					{
						CAIMilitarySummary UnitMilitaryCapability;
						UnitMilitaryCapability.Set( pUnitTemplate );
						m_CurrentMilitaryCapability += UnitMilitaryCapability;
					}
				}
				//
				/-*
				POSITION pos = pAICity->GetAIProductionList()->GetHeadPosition();
				while( pos )
				{
					CAIProductionTask* pAIProductionTask = pAICity->GetAIProductionList()->GetNext( pos );

					CUnitTemplate* pUnitTemplate = pAIProductionTask->m_pProductionOption->m_pBuildOption->GetUnitTemplate();
					if( pUnitTemplate )
					{
						CAIMilitarySummary UnitMilitaryCapability;
						UnitMilitaryCapability.Set( pUnitTemplate );
						m_CurrentMilitaryCapability += UnitMilitaryCapability;
					}
				}
				*-/
			}
		}
	}
}
*/

/*
void CEOSAIBrain::CalculateUnsatisfiedMilitaryCapability()
{
	m_UnsatisfiedMilitaryCapability = m_InitialNeed - m_CurrentMilitaryCapability;
	m_UnsatisfiedMilitaryCapability.NothingBelowZero();

	// Add a little bit of the InitialNeed
	CAIMilitarySummary NationalInitialNeed = m_InitialNeed;
	NationalInitialNeed.Multiply( 0.1f );
	m_UnsatisfiedMilitaryCapability += NationalInitialNeed;

	// Add some post-values in case everything is zero
	//m_UnsatisfiedMilitaryCapability.m_fSighting += 1.0f;
	//m_UnsatisfiedMilitaryCapability.m_fTransport += 1.0f;
	//m_UnsatisfiedMilitaryCapability.m_fSeaSupremacy += 1.0f;
}
*/
/*
void CEOSAIBrain::InstanciateOrders()
{
	bool bAllOrdersWereCreated = false;
	while( !bAllOrdersWereCreated )
	{
		bAllOrdersWereCreated = true;
		POSITION pos = GetAIObjectList()->GetHeadPosition();
		while( pos )
		{
			CAIObject* pAIObject = GetAIObjectList()->GetNext( pos );

			if( pAIObject->GetObjectId() == 18 )
			{
				int zz=0;
			}
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIObject );
			if( pAIUnit && 
				pAIUnit->GetObjectId() == 17 &&
				pAIUnit->GetUnitTemplate()->GetInternalName() == "Tank (Class 6)" )
			{
				int zz=0;
			}

			bool bVal = pAIObject->CreateOrders();
			if( bVal == false )
			{
				bAllOrdersWereCreated = false;
			}
		}
	}
}
*/

/*
void CEOSAIBrain::AttemptToScheduleDefinedActions()
{
	// Iterate over the AIObject, collecting the decent actions into a single, ordered list
	CEOSAIListSort< CAIDefinedAction* >  BestDefinedActions;
	BestDefinedActions.SetSortFunction( SortDesiresByScore );
	POSITION pos = m_AIObjectList.GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_AIObjectList.GetNext( pos );
		for( long i=0; i<pAIObject->GetFinalState()->GetDecentActionsList()->GetMaximumSize(); i++ )
		{
			CAIDefinedAction* pAction = pAIObject->GetFinalState()->GetDecentActionsList()->m_Array[i];
			if( pAction )
			{
				BestDefinedActions.InsertSorted( pAction );
			}
		}
	}

	// Attempt to schedule it
	float fLastScore = 0.0f;
	pos = BestDefinedActions.GetHeadPosition();
	while( pos )
	{
		CAIDefinedAction* pAction = BestDefinedActions.GetNext( pos );
		float fScore = pAction->GetScore();
		#ifdef _DEBUG
		if( fLastScore != 0.0f )
		{
			ASSERT( fLastScore > fScore );
		}
		#endif
		long iNumberOfActions = pAction->GetActor()->GetNumberOfScheduledActions();
		if( iNumberOfActions < 5 )
		{
			pAction->TryToScheduleThisAction();
		}
	}

	// Now delete all the unscheduled ActionDefinitions
	/*
	pos = m_AIObjectList.GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_AIObjectList.GetNext( pos );
		pAIObject->DeleteUnscheduledDecentActions();
	}
	*-/
}


void CEOSAIBrain::AttemptToScheduleDefinedActions( CEOSAILimitedList< CAIDefinedAction* >* pDecentLogicalActions )
{
	for( long j=0; j<pDecentLogicalActions->GetMaximumSize(); j++ )
	{
		CAIDefinedAction* pActionDefinition = pDecentLogicalActions->m_Array[j];
		if( pActionDefinition )
		{
			/*
			if( pActionDefinition->m_pPrerequisiteTransportAction )
			{
				int z=0;
			}
			*-/

			//pActionDefinition->DumpObject();
			pActionDefinition->TryToScheduleThisAction();
			pDecentLogicalActions->m_Array[j] = NULL; // remove it from the limited list so it isnt deleted
			pDecentLogicalActions->m_ValueArray[j] = 0.0f;
		}
	}
}

//
void CEOSAIBrain::ActorScheduledADefinedAction( CAIObject* pActor )
{
	// Remove all DefinedActions that use pActor
	POSITION pos = m_BestAvailableActions.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CAIDefinedAction* pDefinedAction = m_BestAvailableActions.GetNext( pos );
		if( pDefinedAction->GetActor() == pActor )
		{
			m_BestAvailableActions.RemoveAt( prevPos );
		}
	}
}

void CEOSAIBrain::sRemoveDefinedAction( CAIDefinedAction* pAction )
{
	// Remove all DefinedActions that use pActor
	POSITION pos = m_BestAvailableActions.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CAIDefinedAction* pDefinedActionInList = m_BestAvailableActions.GetNext( pos );
		if( pDefinedActionInList == pAction )
		{
			m_BestAvailableActions.RemoveAt( prevPos );
		}
	}
}

//
//void CEOSAIBrain::RemoveDefinedActionsFromBestAvailableActionsList( CAIObject* pActor )
void CEOSAIBrain::ActorUnscheduledADefinedAction( CAIObject* pActor )
{
	// Remove all DefinedActions that use pActor
	POSITION pos = m_BestAvailableActions.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CAIDefinedAction* pDefinedAction = m_BestAvailableActions.GetNext( pos );
		if( pDefinedAction->GetActor() == pActor )
		{
			m_BestAvailableActions.RemoveAt( prevPos );
		}
	}
}

//
float CEOSAIBrain::GetTotalScore()
{
	float fTotalScore = 0.0f;
	POSITION pos = m_AIObjectList.GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_AIObjectList.GetNext( pos );
		fTotalScore += pAIObject->GetPrimaryActorScheduledScore();
	}
	return fTotalScore;
}
*/
//
// Plan Score
//

float CEOSAIBrain::CalculateGrowthScore()
{
	float fScore = 0.0f;
	//
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			/*
			if( pAICity->WillBeMine() )
			{
				float fFinalTime = pAICity->GetFinalState()->GetTime();
				if( fFinalTime < 100.0f )
				{
					fScore = (100.0f - fFinalTime) * pAICity->GetServerCity()->GetTotalProduction( true,true );
				}
			}
			*/
		}

		// INCOMPLETE: I am doing forward reasoning here instead of letting the AI figure out the
		//   value of a resource.
		CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
		if( pAIResource )
		{
			/*
			if( pAIResource->WillBeMine() )
			{
				float fFinalTime = pAIResource->GetFinalState()->GetTime();
				if( fFinalTime < 100.0f )
				{
					fScore = (100.0f - fFinalTime) * pAIResource->GetResource()->GetResourcePerTurn();
				}
			}
			*/
		}
	}
	return fScore;
}
/*
float CEOSAIBrain::GetSpatialMapDistance_NoTransports( EOSAI::PoiMobile* pActor, CEOSAILocation Start, CEOSAILocation End )
{
	return m_SpatialMap.GetPixelDistance( Start, End, pActor->GetMovementType(), 0 );
}
*/
//
//
//

/*
void CEOSAIBrain::CreateTransportActions(
				CAIDesire* pDesire,
				CAIWeaklyDefinedAction* pWeaklyDefinedAction,
				EOSAI::PoiMobile* pUnitToBeTransported,
				EOSAI::PoiObjectState* pUnitToBeTransportedState,
				CAIObject* pFinalTarget )
{
	//DefineDecentSeaTransportActions( pDesire, pWeaklyDefinedAction, pUnitToBeTransported, pUnitToBeTransportedState, pFinalTarget );

	CEOSAIDesireSatisfactionSummary  SatisfactionSummery;
	CAITransportDefinition::ConsiderCreatingATransportDefinition( 
			pDesire,
			pWeaklyDefinedAction,
			pUnitToBeTransported,
			pUnitToBeTransportedState,
			pFinalTarget,
			&SatisfactionSummery );


	/*
	// Look at the distance between the pUnitToBeTransported and the TargetEndLocation.
	//   Do a quick pre-test to figure out if the idea is even reasonable.
	CEOSAILocation Start = pActorState->GetLocation(); // pUnitToBeTransported->GetFinalLocation();
	CEOSAILocation End   = pEndTarget->GetInitialLocation();
	float fDistance = m_pWorldDescPlayer->GetPixelDistance( Start, End );
	float fGroundMovementRate = pUnitToBeTransported->GetMovementRate();
	float fTransportMovementRate = m_fFastestGroundUnitTransportSpeed;

	float fBestSpeed = fGroundMovementRate;
	if( fBestSpeed < fTransportMovementRate )
	{
		fBestSpeed = fTransportMovementRate;
	}

	float fFinishedTime = pActorState->GetTime() + fDistance / fBestSpeed;
	float fScore = pDesire->CalculateScore( pUnitToBeTransported, fFinishedTime );

	float fNeededScore = pWeaklyDefinedAction->GetActorState()->GetMinimumScoreNeededForTheDecentActionList();
	if( fScore > fNeededScore )
	{
		DefineDecentSeaTransportActions( pDesire, pWeaklyDefinedAction, pUnitToBeTransported, pActorState, pEndTarget );
	}
	*-/
}
*/
/*
void CEOSAIBrain::CreatePotentialTransportActions( 
			CAIDesire* pDesire,
			CAIDefinedAction* pAction,
			CEOSAIBrain* pAIPlan, 
			EOSAI::PoiMobile* pUnitToBeTransported,
			CAIObject* pTarget )
{
	if( pUnitToBeTransported->IsGroundUnit() )
	{
		DefineDecentSeaTransportActions( pDesire,
										 pAction,
										 pAIPlan,
										 pUnitToBeTransported,
										 pTarget );
	}
}
*/
/*
void CEOSAIBrain::DefineDecentSeaTransportActions(
							CAIDesire* pDesire,
							CAIWeaklyDefinedAction* pWeaklyDefinedAction,
							EOSAI::PoiMobile* pUnitToBeTransported,
							EOSAI::PoiObjectState* pUnitToBeTransportedState,
							CAIObject* pFinalTarget )
{
}
*/
/*
{
	CEOSAILocation GroundUnitStartLocation = pUnitToBeTransportedState->GetLocation(); //->GetFinalLocation();
	CEOSAILocation TargetLocation = pFinalTarget->GetInitialLocation();

	// Culling step:
	//   Ignoring the locations of all the transports, take a look at the distance
	//   between the GroundUnit and the Target.  Based on the distance alone, figuring
	//   out how long it will take based on the best transport, is the score within
	//   a reasonable value?  If not, don't bother processing this possibility.
	float fDistance = m_pWorldDescPlayer->GetPixelDistance( GroundUnitStartLocation, TargetLocation );
	float fGroundMovementRate = pUnitToBeTransported->GetMovementRate();
	float fTransportMovementRate = m_fFastestGroundUnitTransportSpeed;

	float fBestSpeed = fGroundMovementRate;
	if( fBestSpeed < fTransportMovementRate )
	{
		fBestSpeed = fTransportMovementRate;
	}

	float fFinishedTime = pUnitToBeTransportedState->GetTime() + fDistance / fBestSpeed;
	float fScore = pDesire->CalculateScore( pUnitToBeTransported, fFinishedTime );

	float fNeededScore = pWeaklyDefinedAction->GetActorState()->GetMinimumScoreNeededForTheDecentActionList();
	if( fScore < fNeededScore )
	{
		// The score is bad.  Bon't bother processing anymore.
		return;
	}

	// Culling step:
	//   If the final location is an unexplored area, then I need to get off 
	//   at a nearby known land location.  If there are no adjacent land regions,
	//   then just fail.  (INCOMPLETE: I should actually come up with a better
	//   solution to this.  Fortunately, this situation is unlikely to happen.)
	//   Situations where this can happen: If a citres is right on the edge of
	//     an unknown area, it can be in an unknown area.  The other situation where
	//     this can happen is when I want to explore an unknown area.  In that case,
	//     it's uncertain whether it's actually a land area (although if it is
	//     adjacent to a land area, it will still work).
	CEOSAIBrain* pAIPlan = pDesire->GetAIPlan();
	CEOSAIRegion2* pFinalRegion = pAIPlan->GetSpatialMap()->GetRegion( TargetLocation );
	if( pFinalRegion->GetGeoType() == CGeo::Unknown )
	{
		CEOSAIRegion2* pAdjacentLand = pFinalRegion->GetAdjacentLandRegion();
		if( pAdjacentLand == NULL )
		{
			// We can't find a decent landing location.  
			return;
		}
	}

	// If the unit is already inside a transport, use that transport
	CList< CEOSAIUnit2* > TransportUnits;
	if( pUnitToBeTransportedState->IsInsideATransport() )
	{
		//TransportUnits.AddTail( pUnitToBeTransportedState->GetHighestUnitContainer() );
		TransportUnits.AddTail( pUnitToBeTransportedState->GetAIUnitContainer() );
	}
	else
	{
		// Get a list of all the transports
		POSITION pos = GetAIObjectList()->GetHeadPosition();
		while( pos )
		{
			CAIObject* pAIObject = GetAIObjectList()->GetNext( pos );
			CEOSAIUnit2* pUnitSymbol = dynamic_cast< CEOSAIUnit2* >( pAIObject );
			if( pUnitSymbol )
			{
				if( pUnitSymbol->IsMine() &&
					pUnitSymbol->GetUnitTemplate()->IsSeaUnit() &&
					pUnitSymbol->GetUnitTemplate()->GetNumberOfGroundUnitsThatICanContain() > 0 &&
					!pUnitSymbol->IsDead() )
				{
					TransportUnits.AddTail( pUnitSymbol );
				}
			}
		}
	}
	// 
	POSITION pos = TransportUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pTransportUnit = TransportUnits.GetNext( pos );
		CEOSAILocation TransportLocation = pTransportUnit->GetFinalLocation();
		long iTransportGeo = pTransportUnit->GetFinalGeo();

		// If the unit is already inside a transport, use that transport
		/*
		if( pUnitToBeTransportedState->IsInsideATransport() &&
			pUnitToBeTransportedState->IAmContainedInside( pTransportUnit ) == false )
		{
			continue;
		}
		*-/

		// Culling step:
		//   a. If the transport route is worse than the direct route, ignore it.
		//   b. Check if the best-case scenario using this transport is still
		//      too bad to be put into the DecentActions list
		float fDistanceFromTransportToGroundUnit = 0.0f;
		if( pUnitToBeTransportedState->IAmContainedInside( pTransportUnit ) == false )
		{
			fDistanceFromTransportToGroundUnit = 
				GetSpatialMap()->GetPixelDistance( TransportLocation, GroundUnitStartLocation, pTransportUnit->GetMovementType(), 1 );
		}
		float fDistanceFromGroundUnitToTarget = 
			GetSpatialMap()->GetPixelDistance( GroundUnitStartLocation, TargetLocation, pUnitToBeTransported->GetMovementType(), 2 );

		// Rough estimate
		float fTimeToPickup = fDistanceFromTransportToGroundUnit /
							( pUnitToBeTransported->GetMovementRate() + 
							  pTransportUnit->GetUnitTemplate()->GetMovementRate() );
		float fTimeFromPickupToTarget = fDistanceFromGroundUnitToTarget / pTransportUnit->GetUnitTemplate()->GetMovementRate();

		// If there is a direct route, or a possibility for a direct route, factor this
		//   into the question of whether we should attempt to use a transport
		if( pWeaklyDefinedAction->GetOddsOfADirectRoute() > 0.0f )
		{
			float fOddsSqu = pWeaklyDefinedAction->GetOddsOfADirectRoute();
			if( fOddsSqu < 1.0f ){ fOddsSqu = fOddsSqu*fOddsSqu; }
			if( fOddsSqu < 0.01f ){ fOddsSqu = 0.01f; }
			if( fTimeToPickup + fTimeFromPickupToTarget >
				pWeaklyDefinedAction->GetTimeToReachViaDirectRoute()/fOddsSqu )
			{
				// Looks like the transport route will take longer than the direct route
				continue; 
			}
		}

		float fTimeOffset = pTransportUnit->GetFinalTime();
		if( fTimeOffset < pUnitToBeTransportedState->GetTime() )
		{
			fTimeOffset = pUnitToBeTransportedState->GetTime();
		}

		float fTimeToCompletion = fTimeToPickup + fTimeFromPickupToTarget;
 
		float fScore = pDesire->CalculateScore( pUnitToBeTransported, fTimeToCompletion );
		float fNeededScore = pWeaklyDefinedAction->GetActorState()->GetMinimumScoreNeededForTheDecentActionList();
		if( fScore < fNeededScore )
		{
			// The score of the path isn't good enough.  End processing this possibility.
			continue;
		}

		CAITransportDefinition::ConsiderCreatingATransportDefinition(
			pDesire, 
			pWeaklyDefinedAction, 
			pTransportUnit, 
			pUnitToBeTransported, 
			pUnitToBeTransportedState,
			TargetLocation );
		/*
		CAITransportDefinition* pTransportDefinition = new CAITransportDefinition( 
			pDesire, 
			pWeaklyDefinedAction, 
			pTransportUnit, 
			pUnitToBeTransported, 
			pUnitToBeTransportedState,
			TargetLocation );
		if( pTransportDefinition->CalculateDetails() )
		{
			pWeaklyDefinedAction->AddTransportDefinition( pTransportDefinition );
		}
		else
		{
			delete pTransportDefinition;
		}
		*-/

		/*
		CAIDefinedAction_SeaBasedTransport* pTransportAction = new CAIDefinedAction_SeaBasedTransport(
			//pDesire, NULL, pTransportUnit, pUnitToBeTransported, pFinalTarget->GetInitialLocation() );
			pDesire, pTransportUnit, pUnitToBeTransported, TargetLocation );
		if( pTransportAction->CalculateDetails() )
		{
			float fDuration = pTransportAction->GetDuration( pUnitToBeTransported );
			float fScore = pDesire->CalculateScore( pUnitToBeTransported, fDuration );
			//if( fScore > pUnitToBeTransported->GetMinimumScoreNeededForTheDecentActionList() )
			if( fScore > pWeaklyDefinedAction->GetActorState()->GetMinimumScoreNeededForTheDecentActionList() )
			{
				CAIDefinedAction* pAction =
					pDesire->InstanciateAction(
						pWeaklyDefinedAction,
						pUnitToBeTransported,
						pTransportAction,
						fDuration,
						fScore );
				pAction->UpdateScore();
			}
		}
		*-/
	}
}
*/

/*
void CEOSAIBrain::DefineDecentSeaTransportActions(
							CAIDesire* pDesire,
							CAIDefinedAction* pAction,
							CEOSAIBrain* pAIPlan,
							EOSAI::PoiMobile* pUnitToBeTransported,
							CAIObject* pFinalTarget )
{
	CEOSAILocation GroundUnitLocation = pUnitToBeTransported->GetFinalLocation();

	// Get a list of all the transports
	CList< CEOSAIUnit2* > TransportUnits;
	POSITION pos = pAIPlan->GetAIObjectList()->GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = pAIPlan->GetAIObjectList()->GetNext( pos );
		CEOSAIUnit2* pUnitSymbol = dynamic_cast< CEOSAIUnit2* >( pAIObject );
		if( pUnitSymbol )
		{
			if( pUnitSymbol->IsMine() &&
				pUnitSymbol->GetUnitTemplate()->IsSeaUnit() &&
				pUnitSymbol->GetUnitTemplate()->GetNumberOfGroundUnitsThatICanContain() > 0 &&
				!pUnitSymbol->IsDead() )
			{
				TransportUnits.AddTail( pUnitSymbol );
			}
		}
	}
	// 
	pos = TransportUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pTransportUnit = TransportUnits.GetNext( pos );
		CEOSAILocation TransportLocation = pTransportUnit->GetFinalLocation();
		long iTransportGeo = pTransportUnit->GetFinalGeo();

		// Step 1 (culling bad possibilities): Check if the best-case scenario is still
		//   too bad to be put into the DecentActions list
		float fDistanceFromTransportToGroundUnit = 0.0f;
		if( pUnitToBeTransported->IAmContainedInside( pTransportUnit ) == false )
		{
			fDistanceFromTransportToGroundUnit = 
				pAIPlan->GetSpatialMap()->GetPixelDistance( TransportLocation, GroundUnitLocation, pTransportUnit->GetMovementType(), 1 );
		}
		float fDistanceFromGroundUnitToTarget = 
			pAIPlan->GetSpatialMap()->GetPixelDistance( GroundUnitLocation, pFinalTarget->GetInitialLocation(), pUnitToBeTransported->GetMovementType(), 2 );

		// Rough estimate
		float fTimeToPickup = fDistanceFromTransportToGroundUnit /
							( pUnitToBeTransported->GetMovementRate() + 
							  pTransportUnit->GetUnitTemplate()->GetMovementRate() );
		float fTimeFromPickupToTarget = fDistanceFromGroundUnitToTarget / pTransportUnit->GetUnitTemplate()->GetMovementRate();

		float fTimeOffset = pTransportUnit->GetFinalTime();
		if( fTimeOffset < pUnitToBeTransported->GetFinalTime() )
		{
			fTimeOffset = pUnitToBeTransported->GetFinalTime();
		}

		//pAction->SetStartTime( fTimeOffset );
		//pAction->SetDuration( pTransportUnit, fTimeToPickup + fTimeFromPickupToTarget );
		pAction->SetDuration( pUnitToBeTransported, fTimeToPickup + fTimeFromPickupToTarget );
		//pAction->SetDuration( fTimeOffset + fTimeToPickup + fTimeFromPickupToTarget );
		//float fScore = pAction->GetScoreBasedOnTime( fTimeOffset + fTimeToPickup + fTimeFromPickupToTarget );
		pAction->UpdateScore();
		float fScore = pAction->GetScore();
		float fNeededScore = pAIPlan->GetDecentLogicalActions()->GetTailValue();
		if( fScore > fNeededScore )
		{
			bool bIsPossible = CAILogicalActionDefined_SeaBasedTransport::IsPossible(
				pAIPlan,
				pTransportUnit->GetFinalGeo(),
				pUnitToBeTransported->GetFinalGeo(),
				pFinalTarget->GetInitialLocation(),
				pFinalTarget->GetInitialGeo() );

			if( bIsPossible )
			{
				CAILogicalActionDefined_SeaBasedTransport* pTransportAction = new CAILogicalActionDefined_SeaBasedTransport(
					pDesire, pTransportUnit, pUnitToBeTransported, pFinalTarget->GetInitialLocation() );
				if( pTransportAction->CalculateDetails() )
				{
					CAIDefinedAction* pActionCopy = pAction->Duplicate();
					pActionCopy->m_pPrerequisiteTransportAction = pTransportAction;
					//pActionCopy->m_fEndTime = pTransportAction->m_fFinalLocationTime;
					//pActionCopy->m_fStartTime = pTransportAction->m_fStartTime;
					pActionCopy->UpdateScore();

					if( pActionCopy->GetScore() > pAIPlan->GetDecentLogicalActions()->GetTailValue() )
					{
						pAIPlan->GetDecentLogicalActions()->Add( pActionCopy, pActionCopy->GetScore() );
					}
					else
					{
						// delete it
						delete pActionCopy;
					}
				}
			}
		}
	}
}
*/
/*
void CEOSAIBrain::AddAIPoiObject( CEOSAIPoiObject* pObj )
{
	//m_AIObjectList.AddTail( pObj );

	//CEOSAIPoiObject* pAIPoiObject = dynamic_cast< CEOSAIPoiObject* >( pObj );
	//if( pAIPoiObject )
	{
		m_AIPoiObjects.AddTail( pAIPoiObject );
	}
	/-*
	CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pObj );
	if( pAICity && pAICity->IsMine() )
	{
		m_BuildManager.AddMyCity( pAICity );
	}
	*-/
}
*/
/*
CAIObject* CEOSAIBrain::GetAIObject( long iObjectId )
{
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		if( pAIPoiObject->GetServerPoiObject() )
		{
			if( pAIPoiObject->GetServerPoiObject()->GetObjectId() == iObjectId )
			{
				return pAIPoiObject;
			}
		}
		/-*
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
		if( pAICity )
		{
			if( pAICity->GetObjectId() == iObjectId )
			{
				return pAICity;
			}
		}
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
		if( pAIUnit )
		{
			if( pAIUnit->GetObjectId() == iObjectId )
			{
				return pAIUnit;
			}
		}
		*-/
	}
	return NULL;
}

CEOSAIPoiObject* CEOSAIBrain::GetAIPoiObject( long iObjectId )
{
	CAIObject* pAIObject = GetAIObject( iObjectId );
	return dynamic_cast< CEOSAIPoiObject* >( pAIObject );
}

CEOSAIUnit2* CEOSAIBrain::GetAIUnit( long iObjectId )
{
	CAIObject* pAIObject = GetAIObject( iObjectId );
	return dynamic_cast< CEOSAIUnit2* >( pAIObject );
}
*/
/*
CEOSAIUnit2* CEOSAIBrain::GetAIUnit( long iOwner, CString strUnitTemplate )
{
	POSITION pos = m_AIPoiObjects.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_AIPoiObjects.GetNext( pos );
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
		if( pAIUnit )
		{
			if( pAIUnit->GetOwner() == iOwner &&
				pAIUnit->GetUnitTemplate()->GetInternalName() == strUnitTemplate )
			{
				return pAIUnit;
			}
		}
	}
	return NULL;
}
*/
/*
void CEOSAIBrain::SetCheckpoint()
{
	// Drop all information relating to the last checkpoint 
	//  (we can't remember multiple checkpoints)
	m_CheckpointedPoiObjectList.RemoveAll();
	//m_CheckpointedCitiesList.RemoveAll();
	//m_ChangedDesires.RemoveAll();

	// Increment the checkpoint id
	m_iCheckpointId++;
	if( m_iCheckpointId > 2000000000 ) m_iCheckpointId = 1;
}

void CEOSAIBrain::RestoreToCheckpoint()
{}
*/
/*
void CEOSAIBrain::EstimateCitResProductionPerArea()
{
	// Clear information
	m_fEstimatedCitResProductionPerLandArea  = 0.0f;
	m_fEstimatedCitResProductionPerWaterArea = 0.0f;

	// Estimate the city production per region ratio
	float fKnownCitResLandProduction = 0.0f;
	float fKnownCitResWaterProduction = 0.0f;
	//CWorldDesc* pWorldDesc = m_pAIPlayer->GetWorldDescServer();

	//POSITION pos = pWorldDesc->GetPoiList()->GetHeadPosition();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CCity* pCity = dynamic_cast< CCity* >( pPoi );
		if( pCity )
		{
			fKnownCitResLandProduction += pCity->GetPopulation();
		}
		CResourceSource* pResourceSource = dynamic_cast< CResourceSource* >( pPoi );
		if( pResourceSource )
		{
			if( pResourceSource->IsOnLand3() )
			{
				fKnownCitResLandProduction += pResourceSource->GetResourcePerTurn();
			}
			else
			{
				fKnownCitResWaterProduction += pResourceSource->GetResourcePerTurn();
			}
		}
	}
	// Step 2: add up all the explored area you know about
	float fExploredWaterArea = 0.0f;
	float fExploredLandArea = 0.0f;
	pos = GetWorldDescServer()->GetGeoList()->GetHeadPosition();
	while( pos )
	{
		CGeo* pGeo = GetWorldDescServer()->GetGeoList()->GetNext( pos );
		if( pGeo->GetGeoType() == CGeo::Land )
		{
			fExploredLandArea += pGeo->GetSize();
		}
		if( pGeo->GetGeoType() == CGeo::Water )
		{
			fExploredWaterArea += pGeo->GetSize();
		}
	}
	/*
	for( long i=1; i<m_pCurrentPlan->GetSpatialMap()->m_RegionArray.GetUpperBound(); i++ )
	{
		CEOSAIRegion2* pRegion = m_pCurrentPlan->GetSpatialMap()->m_RegionArray.GetAt( i );
		ASSERT( pRegion );
		if( pRegion->HasBeenExplored() ){ fExploredArea += pRegion->GetSizeOfRegion(); }
	}
	*-/
	float fCitResProductionPerLandArea  = fKnownCitResLandProduction  / fExploredLandArea;
	float fCitResProductionPerWaterArea = fKnownCitResWaterProduction / fExploredWaterArea;

	// If there is only a small amount of area explored, my calculations will be a bit off.  Adjust them.
	// Based on "map is known" calculations (on a small map) the value is around 0.023 production for land,
	//     0.001 production for water, and 0.007 for all areas in general
	if( fExploredLandArea < 10000.0f )
	{
		//float fAdjustedExploredArea = fExploredLandArea - 5000.0f;
		float fWeight = fExploredLandArea / 10000.0f;
		fCitResProductionPerLandArea = (1.0f-fWeight) * 0.023f +
							           (     fWeight) * fCitResProductionPerLandArea;
	}

	if( fExploredWaterArea < 20000.0f )
	{
		//float fAdjustedExploredArea = fExploredWaterArea - 5000.0f;
		float fWeight = fExploredWaterArea / 20000.0f;
		fCitResProductionPerWaterArea = (1.0f-fWeight) * 0.001f +
							            (     fWeight) * fCitResProductionPerWaterArea;
	}

	m_fEstimatedCitResProductionPerLandArea  = fCitResProductionPerLandArea;
	m_fEstimatedCitResProductionPerWaterArea = fCitResProductionPerWaterArea;

	float fTotalExploredArea = fExploredLandArea + fExploredWaterArea;
	m_fEstimatedCitResProductionPerArea = 
		m_fEstimatedCitResProductionPerLandArea  * ( fExploredLandArea/fTotalExploredArea ) +
		m_fEstimatedCitResProductionPerWaterArea * ( fExploredWaterArea/fTotalExploredArea );

	ASSERT( m_fEstimatedCitResProductionPerLandArea != 0.0f );
	//ASSERT( m_fEstimatedCitResProductionPerWaterArea != 0.0f );
}
*/
bool CEOSAIBrain::AllMyCitiesHaveOrders()
{
	long iAIPlayer = this->GetAIPlayerNumber();
	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		if( pAIPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City && pAIPoiObject->GetOwner() == iAIPlayer )
		{
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			//if( pAICity && pAICity->GetServerCity()->GetBuildOrders()->IsEmpty() ) return false;
			if (pAICity && pAICity->GetAIBuildOrders()->IsEmpty())
			{
				return false;
			}
		}
	}
	return true;
}

//
/*
void CEOSAIBrain::Incoming_TradeAgreementStateChange( CTradeAgreement* pTradeAgreement, EOSAIEnumTradeAgreementState OldState, EOSAIEnumTradeAgreementState NewState )
{
	// Constains a list of situations where I'm waiting to hear-back an answer for
	//   This can include trade, peace agreements, war agreements, etc
	POSITION pos = m_UnresolvedForeignRelationsOffers.GetHeadPosition();
	while( pos )
	{
		CAIUnresolvedSituation* pUnresolved = m_UnresolvedForeignRelationsOffers.GetNext( pos );

		// Does this unresolved object depend on some TradeAgreement(s)?
		pUnresolved->ReceivedAgreementResponse( pTradeAgreement );
	}
	//
	int z=0;
}
*/
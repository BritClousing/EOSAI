
#include <stdafx.h>
#include "EOSAINationalSummary3.h"
//#include "WorldDescServer.h"
#include "EOSAICommonData.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIUnit2.h"
#include "EOSAIInterface.h"
#include "EOSAITechnologyDesc.h"
#include "EOSAIMain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAINationalSummary3::Clear()
{
	m_CitResSummary.Clear();
	m_UnitSummary.Clear();
	//
	m_fMaintenanceCosts = 0.0f;
	//
	//m_TotalResources.Clear();;
	m_ResourceSummary.Clear();
	//m_PlayerAccessibility.c;

	//
	m_fResearchPointsProducedThisTurn = 0.0f;
	m_ResearchCompletedList.RemoveAll();
}

void CEOSAINationalSummary3::CalculatePlayerPower()
{
	m_CitResSummary.Clear();
	m_UnitSummary.Clear();

	//Clear();
	ASSERT( m_iPlayer != 0 );
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
	while( pos )
	{
		//CPoi* pPoi = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
		//CPoiObject* pPoiObject = dynamic_cast< CPoiObject* >( pPoi );
		if( pAIPoiObject &&
			pAIPoiObject->IsAlive() &&
			pAIPoiObject->GetOwner() == m_iPlayer )
		{
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity )
			{
				m_CitResSummary.AddToSummary( pAICity );
			}
			CEOSAIResource* pRes = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
			if( pRes )
			{
				m_CitResSummary.AddToSummary( pRes );
			}
			CEOSAIUnit* pAIUnit = dynamic_cast< CEOSAIUnit* >( pAIPoiObject );
			if( pAIUnit )
			{
				// INCOMPLETE: this isn't a bad measure of military power, although it
				//   isn't quite accurate.  Non-combat units are valued as much as combat units.
				m_UnitSummary.AddToSummary( pAIUnit );

				POSITION pos2 = pAIUnit->GetAIUnitTemplate()->GetProductionAndConsumptionPerTurnList()->GetHeadPosition();
				while( pos2 )
				{
					EOSAI::StringAndFloat* pResDelta = pAIUnit->GetAIUnitTemplate()->GetProductionAndConsumptionPerTurnList()->GetNext( pos2 );
					m_fMaintenanceCosts += max( 0.0f, (-pResDelta->m_fValue));
				}
				pos2 = pAIUnit->GetAIUnitTemplate()->GetProductionAndConsumptionPerMoveList()->GetHeadPosition();
				while( pos2 )
				{
					EOSAI::StringAndFloat* pResDelta = pAIUnit->GetAIUnitTemplate()->GetProductionAndConsumptionPerMoveList()->GetNext( pos2 );
					m_fMaintenanceCosts += max( 0.0f, (-pResDelta->m_fValue));
				}
				//m_fMaintenanceCosts += pUnit->GetUnitTemplate()->GetFoodConsumption();
				//m_fMaintenanceCosts += pUnit->GetUnitTemplate()->GetOilConsumption();
			}
		}
	}
}

void CEOSAINationalSummary3::CalculatePlayerToPlayerAccessibility()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//long iNumberOfPlayers = GetCommonState()->GetWorldBuildDesc()->GetNumberOfPlayers();
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();

	if( m_PlayerAccessibility.m_iSize == 0 )
	{
		m_PlayerAccessibility.SetSize( iNumberOfPlayers+1 );
	}

	long iPlayer = m_iPlayer;
	//for( long iPlayer=1; iPlayer<=iNumberOfPlayers; iPlayer++ )
	{
		// Calculate the accessibility
		// Find all my CitRes first
		CList< CEOSAIPoiObject* > PlayerPoiObjectList;
		//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
		POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
		while( pos )
		{
			//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
			CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
			if( pAICity && pAICity->IsAlive() && pAICity->GetOwner() == iPlayer )
			{
				PlayerPoiObjectList.AddTail( pAICity );
			}
			CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
			if( pAIResource && pAIResource->IsAlive() && pAIResource->GetOwner() == iPlayer )
			{
				PlayerPoiObjectList.AddTail( pAIResource );
			}
		}
		// Find distances to other players
		for( long iOtherPlayer=0; iOtherPlayer<iNumberOfPlayers+1; iOtherPlayer++ )
		{
			//if( iOtherPlayer >  iPlayer ) continue; // don't calculate accessibility for self
			if( iOtherPlayer == iPlayer )
			{
				m_PlayerAccessibility.Value( iOtherPlayer ) = 0.0f;
				continue; // don't calculate accessibility for self
			}

			CList< CEOSAIPoiObject* > OtherPlayerPoiObjectList;
			//POSITION pos = pWorldDescServer->GetPoiList()->GetHeadPosition();
			POSITION pos = g_pEOSAICommonData->GetAIPoiObjects()->GetHeadPosition();
			while( pos )
			{
				//CPoi* pPoi = pWorldDescServer->GetPoiList()->GetNext( pos )->GetPtr();
				CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObjects()->GetNext( pos );
				CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
				if( pAICity && pAICity->GetOwner() == iOtherPlayer )
				{
					OtherPlayerPoiObjectList.AddTail( pAICity );
				}
				CEOSAIResource* pAIResource = dynamic_cast< CEOSAIResource* >( pAIPoiObject );
				if( pAIResource && pAIResource->GetOwner() == iOtherPlayer )
				{
					OtherPlayerPoiObjectList.AddTail( pAIResource );
				}
			}
			// Find distances
			float fTotalWeight = 0.0f;
			float fWeightedDistance = 0.0f;
			pos = PlayerPoiObjectList.GetHeadPosition();
			while( pos )
			{
				CEOSAIPoiObject* pPlayerPoiObject = PlayerPoiObjectList.GetNext( pos );
				POSITION pos2 = OtherPlayerPoiObjectList.GetHeadPosition();
				while( pos2 )
				{
					CEOSAIPoiObject* pOtherPlayerPoiObject = OtherPlayerPoiObjectList.GetNext( pos2 );
					//float fDistance = pWorldDescServer->GetPixelDistance( pPlayerPoiObject, pOtherPlayerPoiObject );
					float fDistance = g_pWorldDistanceTool->GetDistance( pPlayerPoiObject->GetLocation(), pOtherPlayerPoiObject->GetLocation() );

					// We want closer CitRes to be weighted heavier
					//   Otherwise, more distant CitRes (mine or his) can throw-off the average distance
					// fDistance of    0 = weight of 2
					// fDistance of  400 = weight of 0.67
					// fDistance of  800 = weight of 0.4
					// fDistance of 1200 = weight of 0.29
					float fWeight = 400.0f / (fDistance+200.0f);
					fWeightedDistance =
						( fDistance*fWeight + fWeightedDistance*fTotalWeight ) / ( fWeight + fTotalWeight );
					fTotalWeight += fWeight;
				}
			}

			// fWeightedDistance of    0 = fAccessibility of 1
			// fWeightedDistance of  200 = fAccessibility of 1
			// fWeightedDistance of  400 = fAccessibility of 0.67
			// fWeightedDistance of  600 = fAccessibility of 0.5
			// fWeightedDistance of  800 = fAccessibility of 0.4
			// fWeightedDistance of 1000 = fAccessibility of 0.33
			float fAccessibility = 400.0f/(fWeightedDistance+200.0f);
			if( fAccessibility > 1.0f ){ fAccessibility = 1.0f; }
			//m_aPositionTowardsPlayer[ iOtherPlayer ].m_fAccessibility = fAccessibility;

			m_PlayerAccessibility.Value( iOtherPlayer ) = fAccessibility;
			//m_PlayerAccessibility.Value( iOtherPlayer, iPlayer ) = fAccessibility;
		}
	}
}

// Resources
//void CEOSAINationalSummary3::AICalculateResourceConsumptionAndDeltas(bool bIncludeCityNoOrdersProduceWealth)
void CEOSAINationalSummary3::CalculateResourceDeltas()
{
	// Include city-production of wealth as a separate line so that we can ask for either one (depending on the sitaution)

	// Resources
	std::map<CString, float> m_ResourcesProducedAssumingNoOrders;
	std::map<CString, float> m_ResourcesProducedByCurrentOrders;
	std::map<CString, float> m_ResourcesConsumedAssumingNoOrders;
	std::map<CString, float> m_ResourcesConsumedByCurrentOrders;

	CList< CEOSAIPoiObject* >* pAIPoiObjects = g_pEOSAICommonData->GetAIPoiObjects();
	POSITION pos = pAIPoiObjects->GetHeadPosition();
	while (pos)
	{
		CEOSAIPoiObject* pPoiObject = pAIPoiObjects->GetNext(pos);
		if (pPoiObject->GetOwner() == m_iPlayer )
		{
			pPoiObject->GetResourcesProducedPerTurn(m_ResourcesProducedAssumingNoOrders, m_ResourcesProducedByCurrentOrders);
			pPoiObject->GetResourcesConsumedPerTurn(m_ResourcesConsumedAssumingNoOrders, m_ResourcesConsumedByCurrentOrders);
		}
	}
	//#ifdef THINGS_TO_COMPILE_EVENTUALLY
	//put some code in here. The values calculated here are used in a calculation to figure out if the AI is going into a resource shortage.
	//#endif
}


bool CEOSAINationalSummary3::AllTechnologiesHaveBeenDiscovered()
{
	EOSAI::CGameRules* pGameRules = g_pEOSAIMain->GetAIGameRules();
	EOSAI::CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer( m_iPlayer );

	CString strUnitsubset = pGamePlayer->GetUnitsubset();
	//CUnitset* pUnitset = GetCommonState()->GetActiveUnitset();

	POSITION pos = pGameRules->GetTechnologyDescsList()->GetHeadPosition();
	while( pos )
	{
		CEOSAITechnologyDesc* pTechDesc = pGameRules->GetTechnologyDescsList()->GetNext( pos );
		if( pTechDesc->ResearchIsAllowed( strUnitsubset ) )
		{
			if( GetTechnologyHasBeenDiscovered( pTechDesc->GetInternalName() ) == false )
			{
				return false;
			}
		}
	}
	return true;
}

float CEOSAINationalSummary3::GetDiscoveredTechnologyResearchPoints()
{
	float fResearchPoints = 0.0f;
	EOSAI::CGameRules* pGameRules = g_pEOSAIMain->GetAIGameRules();

	//EOSAI::CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer( m_iPlayer );
	POSITION pos = m_ResearchCompletedList.GetHeadPosition();
	while( pos )
	{
		CString strTech = m_ResearchCompletedList.GetNext( pos );
		CEOSAITechnologyDesc* pTechDesc = pGameRules->GetTechnologyDesc( strTech );
		fResearchPoints += pTechDesc->GetCost();
	}
	return fResearchPoints;
}

bool CEOSAINationalSummary3::GetTechnologyHasBeenDiscovered( CString strTechnology )
{
	POSITION pos = m_ResearchCompletedList.GetHeadPosition();
	while( pos )
	{
		CString strTechInList = m_ResearchCompletedList.GetNext( pos );
		if( strTechInList == strTechnology ) return true;
	}
	return false;
}

/*
float CEOSAINationalSummary3::ResearchPointsProducedThisTurn()
{
	//EOSAI::CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer( m_iPlayer );
	//return pGamePlayer->GetResearchPointsProducedThisTurn();
	return m_fResearchPointsProducedThisTurn;
}
*/


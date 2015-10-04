
#include "stdafx.h"
#include "EOSAICommonData.h"
#include "EOSAIGenericRegion.h"
//#include "PoiObject.h"
#include "EOSAIUnit2.h"
#include "City.h"
#include "EOSAIResource.h"
//#include "WorldDescServer.h"
#include "EOSAIMultiRegion2.h"
#include "EOSAIRoad2.h"
extern CList< long >  g_HighlightAIRegions;
extern CList< long >  g_AIPathwayData;

//#define THINGS_TO_COMPILE_EVENTUALLY

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//using namespace EOSAI;

CEOSAIGenericRegion::CEOSAIGenericRegion( CEOSAIGenericRegionManager* pRegionManager, long iId )
{
	Clear();
	m_pRegionManager = pRegionManager;
	m_iId = iId;

	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	m_PlayerCitResUnitSummaries.SetSize( iNumberOfPlayers+1 );
	m_PlayerVisibility01.SetSize( iNumberOfPlayers+1 ); // 0-1
	m_PlayerCityUnitAIPower.SetSize( iNumberOfPlayers+1 );
	m_PlayerCitResControlValue01.SetSize( iNumberOfPlayers+1 );

	m_fTempWarzoneValue01 = 0.0f;

	m_iSingleOwner = 0;
	m_fSingleOwnerControl = 0.0f; // 0.0 - 1.0

	for( long i=0; i<iNumberOfPlayers+1; i++ )
	{
		m_PlayerVisibility01[i] = 0.0f;
		m_PlayerCityUnitAIPower[i] = 0.0f;
		m_PlayerCitResControlValue01[i] = 0.0f;
		//m_CrowsDistanceFromPlayerCitResUnits[i] = 10e6;
		//m_PlayerHasGroundUnitsOnThisGeo[i] = false;
		//m_PlayerHasCitiesOnThisGeo[i] = false;
	}
}

CEOSAIGenericRegion::~CEOSAIGenericRegion()
{
	while( m_AdjacentRegions.IsEmpty() == FALSE ){ delete m_AdjacentRegions.RemoveHead(); }
	while( m_AIPoiObjectProximity.IsEmpty() == FALSE ){ delete m_AIPoiObjectProximity.RemoveHead(); }
}

void  CEOSAIGenericRegion::Clear()
{
	m_pRegionManager = NULL;
	//m_pAIGeo = NULL;
	m_iId = 0;
	m_fRegionRadius = 0.0f;

	m_iCityObjectId = 0;
	m_pAICity = NULL;

	m_iTerrainType3 = 0;
	m_iTerrainType9 = 0;

	m_AIPoiObjectsInsideRegion.RemoveAll();
	while( m_AIPoiObjectProximity.IsEmpty() == FALSE ){ delete m_AIPoiObjectProximity.RemoveHead(); }
	m_TotalCitResSummary.Clear();
	for( long i=0; i<m_PlayerCitResUnitSummaries.m_iSize; i++ )
	{
		m_PlayerCitResUnitSummaries[i].Clear();
		m_PlayerVisibility01[i] = 0.0f;
		m_PlayerCityUnitAIPower[i] = 0.0f;
		m_PlayerCitResControlValue01[i] = 0.0f;
	}
	/*
	m_fTempPathfindingValue = 0.0f;
	m_fTempTotalDistance = 0.0f;
	m_fTempTotalDanger = 0.0f;
	m_fTempTotalPresence = 0.0f;
	m_fTempTotalSighting = 0.0f;
	m_iTempVisitedKey = 0;
	m_iTempEndKey = 0;
	m_bTempIsInTheList = false;
	m_iTempNumberOfGeoSwitches = 0;
	m_iTempIncomingTerrain = 0;
	*/
	//m_pTempPreviousRegion = NULL;
}

void  CEOSAIGenericRegion::ClearPoiData()
{
	//while( m_PoiObjects.IsEmpty() == FALSE ){ delete m_PoiObjects.RemoveHead(); }
	m_AIPoiObjectsInsideRegion.RemoveAll();
	while( m_AIPoiObjectProximity.IsEmpty() == FALSE ){ delete m_AIPoiObjectProximity.RemoveHead(); }
	m_TotalCitResSummary.Clear();
	for( long i=0; i<m_PlayerCitResUnitSummaries.m_iSize; i++ )
	{
		m_PlayerCitResUnitSummaries[i].Clear();
		m_PlayerVisibility01[i] = 0.0f;
		m_PlayerCityUnitAIPower[i] = 0.0f;
		m_PlayerCitResControlValue01[i] = 0.0f;
	}
}

CEOSAIGeo* CEOSAIGenericRegion::GetGeo()
{
	return g_pEOSAICommonData->GetAIGeo( m_iGeo );
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//return pWorldDescServer->GetGeo( m_iGeo );
}

void  CEOSAIGenericRegion::SetCityObjectId( long iObjectId )
{
	m_iCityObjectId = iObjectId;
	/*
	#ifdef THINGS_TO_COMPILE_EVENTUALLY
	m_pAICity = some value
	ASSERT( false );
	#endif THINGS_TO_COMPILE_EVENTUALLY
	*/
}

CEOSAICity*  CEOSAIGenericRegion::GetEOSAICity()
{
	if( m_pAICity ){ return m_pAICity; }
	if( m_iCityObjectId == 0 ) return NULL;
	CEOSAIPoiObject* pAIPoiObject = g_pEOSAICommonData->GetAIPoiObject( m_iCityObjectId );
	m_pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
	ASSERT( m_pAICity );
	return m_pAICity;
}


bool  CEOSAIGenericRegion::IsAdjacentToRegion( CEOSAIGenericRegion* pOtherRegion )
{
	ASSERT( this != pOtherRegion );

	// See if it already exists in the list
	if( pOtherRegion == NULL ) return false;
	POSITION pos = m_AdjacentRegions.GetHeadPosition();
	while( pos )
	{
		CEOSAIAdjacentRegionDesc* pRegionInList = m_AdjacentRegions.GetNext( pos );
		if( pRegionInList->m_pRegion == pOtherRegion ){ return true; }
	}
	return false;
}

//void  CEOSAIGenericRegion::AddAdjacentMultiRegion( long iId )
void  CEOSAIGenericRegion::AddAdjacentRegion( CEOSAIGenericRegion* pRegion, CEOSAILocation JumpOffLocation )
{
	if( pRegion == NULL ) return;
	if( IsAdjacentToRegion( pRegion ) == false )
	{
		float fRealX = JumpOffLocation.GetRealX();
		float fRealY = JumpOffLocation.GetRealY();
		//GetCommonState()->GetWorldBuildDesc()->TruncatePixelLocation( &fRealX,&fRealY );
		g_pWorldDistanceTool->TruncateLocation( &fRealX,&fRealY );
		JumpOffLocation.SetReal( fRealX,fRealY );

		m_AdjacentRegions.AddTail( new CEOSAIAdjacentRegionDesc( pRegion, JumpOffLocation ) );
		//m_AdjacentRegions.AddTail( new CAdjacentRegion( pRegion );
	}
}

bool  CEOSAIGenericRegion::GetAdjacentRegionJumpOffLocation( long iAdjRegion, CEOSAILocation* pLocation )
{
	if( GetId() == iAdjRegion ) // This is the same region (sometimes happens when I use this to calculate transport pickup in a city)
	{
		*pLocation = GetLocation();
		return true;
	}
	POSITION pos = m_AdjacentRegions.GetHeadPosition();
	while( pos )
	{
		CEOSAIAdjacentRegionDesc* pAdjRegionDesc = m_AdjacentRegions.GetNext( pos );

		#ifdef _DEBUG
		CEOSAIGenericRegion* pGenericRegion = m_pRegionManager->GetGenericRegion( pAdjRegionDesc->m_JumpOffLocation );
		ASSERT( pGenericRegion == this ); // the Jump-off location must be inside [this]
		#endif

		if( pAdjRegionDesc->m_pRegion->m_iId == iAdjRegion )
		{
			*pLocation = pAdjRegionDesc->m_JumpOffLocation;
			return true;
		}
	}
	return false;
}

void  CEOSAIGenericRegion::AddPoiObjectToSummary( CEOSAIPoiObject* pPoiObject )
{
	ASSERT( pPoiObject->IsDead() == false );
	m_AIPoiObjectsInsideRegion.AddTail( pPoiObject );
	CEOSAILocation PoiObjectLocation = pPoiObject->GetLocation();

	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	// Update the Sighting value
	//if( pPoiObject->GetOwner() > 0 )
	{
		float fCurrentViewArea = pPoiObject->GetDefaultViewRange();
		float fOneTurnViewArea = fCurrentViewArea;
		float fCurrentAttackArea = 40.0f + pPoiObject->GetMaxAttackRange();
		float fTwoTurnAttackArea = fCurrentAttackArea;
		EOSAI::PoiMobile* pPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( pPoiObject );
		if( pPoiMobile )
		{
			if( pPoiMobile->HasARange() )
			{
				fOneTurnViewArea = pPoiObject->GetDefaultViewRange() +
					min( pPoiMobile->GetMovementRangeForAttack(), pPoiMobile->GetMovementRate() );
				fTwoTurnAttackArea = 20.0f + pPoiObject->GetMaxAttackRange() +
					min( pPoiMobile->GetMovementRangeForAttack(), 2.0f*pPoiMobile->GetMovementRate() );
			}
			else
			{
				fOneTurnViewArea = pPoiObject->GetDefaultViewRange() + pPoiMobile->GetMovementRate();
				fTwoTurnAttackArea = 20.0f + pPoiObject->GetMaxAttackRange() + 2.0f*pPoiMobile->GetMovementRate();
			}
		}
		ASSERT( m_fRegionRadius > 0.0f );

		float fMaxDistance = max( fOneTurnViewArea,fTwoTurnAttackArea);

		// Add this poi to all the neighboring Regions, using a PoiObjectProximity object
		CList< CEOSAIGenericRegion* >* pRegionList = &(m_pRegionManager->m_GenericRegionList);
		if( fMaxDistance < 400.0f )
		{
			// Use a quick-lookup list
			if( m_CachedListOfRegionsWithin400px.IsEmpty() )
			{
				POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
				while( pos )
				{
					CEOSAIGenericRegion* pRegionInList = m_pRegionManager->m_GenericRegionList.GetNext( pos );
					//float fDistance = pWorldBuildDesc->GetPixelDistance( this->GetLocation(), pRegionInList->GetLocation() );
					float fDistance = g_pWorldDistanceTool->GetDistance( this->GetLocation(), pRegionInList->GetLocation() );
					fDistance -= ( this->m_fRegionRadius + pRegionInList->m_fRegionRadius );
					if( fDistance < 420.0f )
					{
						m_CachedListOfRegionsWithin400px.AddTail( pRegionInList );
					}
				}
			}
			pRegionList = &m_CachedListOfRegionsWithin400px;
		}

		// Search through the list of regions within 400px
		POSITION pos = pRegionList->GetHeadPosition();
		while( pos )
		{
			CEOSAIGenericRegion* pRegionInList = pRegionList->GetNext( pos );

			//float fDistanceFromPoiObject = pWorldBuildDesc->GetPixelDistance( PoiObjectLocation, pRegionInList->GetLocation() );
			float fDistanceFromPoiObject = g_pWorldDistanceTool->GetDistance( PoiObjectLocation, pRegionInList->GetLocation() );
			fDistanceFromPoiObject -= 0.7f*pRegionInList->m_fRegionRadius;
			if( fDistanceFromPoiObject > fMaxDistance )
			{
				continue;
			}

			if( pRegionInList->GetId() == 32 )
			{
				int h=0;
			}
			if( pRegionInList->GetId() == 45 || pRegionInList->GetId() == 34 )
			{
				int h=0;
			}

			if( fDistanceFromPoiObject < fCurrentViewArea )
			{
				pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) = 1.0f;
			}
			else if( fDistanceFromPoiObject < fOneTurnViewArea )
			{
				float fDistanceFromOneTurnViewEdge = (fOneTurnViewArea + pRegionInList->m_fRegionRadius) - fDistanceFromPoiObject;
				float fGapBetweenCurrentAndOneView = fOneTurnViewArea - fCurrentViewArea;
				float fNewValue = 0.3f;// + ( 0.5f * (fGapBetweenCurrentAndOneView-fDistanceFromOneTurnViewEdge) / fGapBetweenCurrentAndOneView );

				if( pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) < fNewValue )
				{
					pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) = fNewValue;
				}

				/*
				float fDistanceFromOneTurnViewEdge = (fOneTurnViewArea + pRegionInList->m_fRegionRadius) - fDistanceFromPoiObject;
				float fGapBetweenCurrentAndOneView = fOneTurnViewArea - fCurrentViewArea;
				float fNewValue = 0.3f + ( 0.7f * fDistanceFromOneTurnViewEdge / fGapBetweenCurrentAndOneView );

				if( pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) < fNewValue )
				{
					pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) = fNewValue;
				}
				*/
			}

			if( fDistanceFromPoiObject < fCurrentAttackArea )
			{
				pRegionInList->AddPoiObjectProximity( pPoiObject, 1.0f, fDistanceFromPoiObject );
			}
			else if( fDistanceFromPoiObject < fTwoTurnAttackArea )
			{
				float fDistanceFromTwoTurnAttackEdge = (fTwoTurnAttackArea + pRegionInList->m_fRegionRadius) - fDistanceFromPoiObject;
				float fGapBetweenCurrentAndTwoTurnAttack = fTwoTurnAttackArea - fCurrentAttackArea;
				float fNewValue = 0.3f + ( 0.7f * fDistanceFromTwoTurnAttackEdge / fGapBetweenCurrentAndTwoTurnAttack );

				pRegionInList->AddPoiObjectProximity( pPoiObject, fNewValue, fDistanceFromPoiObject );
			}
		}

		/*
		POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
		while( pos )
		{
			CEOSAIGenericRegion* pRegionInList = m_pRegionManager->m_GenericRegionList.GetNext( pos );

			float fDistanceFromPoiObject = pWorldBuildDesc->GetPixelDistance( PoiObjectLocation, pRegionInList->GetLocation() );
			fDistanceFromPoiObject -= pRegionInList->m_fRegionRadius;
			if( fDistanceFromPoiObject > fMaxDistance )
			{
				continue;
			}

			if( fDistanceFromPoiObject < fCurrentViewArea )
			{
				pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) = 1.0f;
			}
			else if( fDistanceFromPoiObject < fOneTurnViewArea )
			{
				float fDistanceFromOneTurnViewEdge = (fOneTurnViewArea + pRegionInList->m_fRegionRadius) - fDistanceFromPoiObject;
				float fGapBetweenCurrentAndOneView = fOneTurnViewArea - fCurrentViewArea;
				float fNewValue = 0.3f + ( 0.7f * fDistanceFromOneTurnViewEdge / fGapBetweenCurrentAndOneView );

				if( pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) < fNewValue )
				{
					pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) = fNewValue;
				}
			}

			if( fDistanceFromPoiObject < fCurrentAttackArea )
			{
				pRegionInList->AddPoiObjectProximity( pPoiObject, 1.0f, fDistanceFromPoiObject );
			}
			else if( fDistanceFromPoiObject < fTwoTurnAttackArea )
			{
				float fDistanceFromTwoTurnAttackEdge = (fTwoTurnAttackArea + pRegionInList->m_fRegionRadius) - fDistanceFromPoiObject;
				float fGapBetweenCurrentAndTwoTurnAttack = fTwoTurnAttackArea - fCurrentAttackArea;
				float fNewValue = 0.3f + ( 0.7f * fDistanceFromTwoTurnAttackEdge / fGapBetweenCurrentAndTwoTurnAttack );

				pRegionInList->AddPoiObjectProximity( pPoiObject, fNewValue, fDistanceFromPoiObject );
			}
		}
		*/
	}

/*
	// Update the Sighting/OverflowVisibility value
	float fCurrentAttackArea = 40.0f + pPoiObject->GetMaxAttackRange();
	float fTwoTurnAttackArea = 0.0f;
	CPoiMobile* pPoiMobile = dynamic_cast< CPoiMobile* >( pPoiObject );
	if( pPoiMobile )
	{
		if( pPoiMobile->HasARange() )
		{
			fTwoTurnAttackArea = 
				fCurrentAttackArea +
				min( pPoiMobile->GetMovementRangeForAttack(), 2.0f*pPoiMobile->GetMovementRate() );
		}
		else
		{
			fTwoTurnAttackArea = fCurrentAttackArea + 2.0f*pPoiMobile->GetMovementRate();
		}
	}
	POSITION pos = m_pRegionManager->m_GenericRegionList.GetHeadPosition();
	while( pos )
	{
		CEOSAIGenericRegion* pRegionInList = m_pRegionManager->m_GenericRegionList.GetNext( pos );

		float fDistanceFromPoiObject = pWorldBuildDesc->GetPixelDistance( Center, pRegionInList->GetLocation() );
		if( fDistanceFromPoiObject < fCurrentAttackArea + pRegionInList->m_fRegionRadius )
		{
			pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) = 1.0f;
			pRegionInList->AddPoiObjectProximity( pPoiObject, 1.0f );
		}
		else if( fDistanceFromPoiObject < fTwoTurnAttackArea + pRegionInList->m_fRegionRadius )
		{
			float fDistanceFromOneTurnViewEdge = (fTwoTurnAttackArea + pRegionInList->m_fRegionRadius) - fDistanceFromPoiObject;
			float fGapBetweenCurrentAndOneView = fTwoTurnAttackArea - fCurrentAttackArea;
			float fNewValue = 0.3f + ( 0.7f * fDistanceFromOneTurnViewEdge / fGapBetweenCurrentAndOneView );

			pRegionInList->AddPoiObjectProximity( pPoiObject, fNewValue );
			if( pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) < fNewValue )
			{
				pRegionInList->m_PlayerVisibility01.Value( pPoiObject->GetOwner() ) = fNewValue;
			}
		}
	}
*/

	m_TotalCitResSummary.AddToSummary( pPoiObject );
	//m_PlayerCitResUnitSummaries.Value( pPoiObject->GetOwner() ).AddToSummary( pPoiObject );

	// Update the CitResUnitSummary object
	CEOSAICitResUnitSummary* pSummary = &m_PlayerCitResUnitSummaries[ pPoiObject->GetOwner() ];
	ASSERT( pSummary );
	if( pSummary )
	{
		pSummary->AddToSummary( pPoiObject );

		CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pPoiObject );
		if( pUnit )
		{
			ASSERT( GetUnitPower( pPoiObject->GetOwner() ) > 0.0f );

			pSummary->m_Units.m_fGroundTransportCapability += pUnit->GetAIGroundTransportCapability();
			pSummary->m_Units.m_UnitHPDefense_UnitTypeSet.Add( pUnit->GetCombatUnitTypeString(), pUnit->GetHPDefenseValue() );
			pSummary->m_Units.m_CombatPowerVsUnitTypes += pUnit->GetCombatPowerVsUnitTypes();
		}
	}
}

void  CEOSAIGenericRegion::AddAIRoad( CEOSAIRoad2* pAIRoad )
{
	// Make sure it doesn't already exist in the list
	POSITION pos = m_RoadsOnTheRegionEdge.GetHeadPosition();
	while( pos )
	{
		CEOSAIRoad2* pExistingAIRoad = m_RoadsOnTheRegionEdge.GetNext( pos );
		if( pExistingAIRoad == pAIRoad )
		{
			ASSERT( false );
			return;
		}
		if( pExistingAIRoad->m_iRoadChunkId == pAIRoad->m_iRoadChunkId )
		{
			ASSERT( false );
			return;
		}
	}

	m_RoadsOnTheRegionEdge.AddTail( pAIRoad );
}

#ifdef THINGS_TO_COMPILE_EVENTUALLY
//#ifdef DO_NOT_COMPILE
void CEOSAIGenericRegion::SetupAIRoadConnections()
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	if( m_RoadsOnTheRegionEdge.GetCount() >= 2 )
	{
		POSITION pos = m_RoadsOnTheRegionEdge.GetHeadPosition();
		while( pos )
		{
			CEOSAIRoad2* pAIRoad = m_RoadsOnTheRegionEdge.GetNext( pos );
			SetupAIRoadConnections( pAIRoad );
		}
	}

	int z=0;
}


void CEOSAIGenericRegion::SetupAIRoadConnections( CEOSAIRoad2* pAIRoad )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	What do I do with the road chunk?  Maybe I can make an EOSAI version of it?  
		Or maybe it should only exist in the interface class - in which case, I should rework the code there?
	CList< CRoadChunk* >  TouchedRoadChunkList;

	//ASSERT( pAIRoad->m_RoadsIConnectTo.IsEmpty() );
	//while( pAIRoad->m_RoadsIConnectTo.IsEmpty() == false ){ delete pAIRoad->m_RoadsIConnectTo.RemoveHead(); }
	//if( pAIRoad->m_iRoadChunkId == 399 )
	//if( pAIRoad->GetId() == 399 )
	//{
	//	int g=0;
	//}

	static long iVisitedValue = 0;
	iVisitedValue++;
	if( iVisitedValue > 200000000 ){ iVisitedValue = 0; }

	CRoadChunk* pCurrentRoadChunk = m_pWorldDescServer->GetRoadChunk( pAIRoad->m_iRoadChunkId );
	pCurrentRoadChunk->m_iTempVisited = iVisitedValue;//true;
	pCurrentRoadChunk->m_fTempDistance = 0.0f;
	pCurrentRoadChunk->m_iRoadPathfinderPoint = pAIRoad->GetId();

	CList< CRoadChunk* >  RoadChunkList;
	RoadChunkList.AddTail( pCurrentRoadChunk );
	//TouchedRoadChunkList.AddTail( pCurrentRoadChunk );
	while( RoadChunkList.IsEmpty() == FALSE )
	{
		CRoadChunk* pCurrentRoadChunk = RoadChunkList.RemoveHead();
		//pCurrentRoadChunk->m_bTempVisited = true;
		pCurrentRoadChunk->m_iTempVisited = iVisitedValue;//true;
		TouchedRoadChunkList.AddTail( pCurrentRoadChunk );

		POSITION pos2 = pCurrentRoadChunk->GetAdjacentRoadChunkList()->GetHeadPosition();
		while( pos2 )
		{
			long iNextRoadChunk = pCurrentRoadChunk->GetAdjacentRoadChunkList()->GetNext( pos2 );
			CRoadChunk* pNextRoadChunk = pWorldDescServer->GetRoadChunk( iNextRoadChunk );
			if( pNextRoadChunk == NULL )
			{
				ASSERT( false );
				continue;
			}
			//
			if( pNextRoadChunk->m_iTempVisited == iVisitedValue ) continue;
			//float fDistance = pWorldDescServer->GetPixelDistance( pCurrentRoadChunk->m_Location, pNextRoadChunk->m_Location );
			float fDistance = g_pAIWorldDistanceTool->GetDistance( pCurrentRoadChunk->m_Location, pNextRoadChunk->m_Location );
			//
			pNextRoadChunk->m_fTempDistance = pCurrentRoadChunk->m_fTempDistance + fDistance;
			//pNextRoadChunk->m_bTempVisited = true;
			pNextRoadChunk->m_iTempVisited = iVisitedValue;
			pNextRoadChunk->m_iRoadPathfinderPoint = pAIRoad->GetId();

			// Is this NextChunk one of the AIRoads?
			POSITION pos3 = m_RoadsOnTheRegionEdge.GetHeadPosition();
			while( pos3 )
			{
				CEOSAIRoad2* pAIRoad3 = m_RoadsOnTheRegionEdge.GetNext( pos3 );
				if( pAIRoad3->m_iRoadChunkId == iNextRoadChunk )
				{
					// Create connection information - both directons
					CEOSAIAdjacentAIRoad2Distance* pRoadDistance;
					pRoadDistance = new CEOSAIAdjacentAIRoad2Distance();
					pRoadDistance->m_pIntermediateRegion = this;
					pRoadDistance->m_pAIRoad = pAIRoad3;
					pRoadDistance->m_fPixelDistance = pNextRoadChunk->m_fTempDistance;
					pAIRoad->m_RoadsIConnectTo.AddTail( pRoadDistance );
					ASSERT( pAIRoad->m_RoadsIConnectTo.GetCount() < 9 );

					/*
					pRoadDistance = new CEOSAIAdjacentAIRoad2Distance();
					pRoadDistance->m_pIntermediateRegion = this;
					pRoadDistance->m_pAIRoad = pAIRoad;
					pRoadDistance->m_fPixelDistance = pNextRoadChunk->m_fTempDistance;
					pAIRoad3->m_RoadsIConnectTo.AddTail( pRoadDistance );
					ASSERT( pAIRoad3->m_RoadsIConnectTo.GetCount() < 6 );
					*/
				}
			}
//test this
			CEOSAIGenericRegion* pNextRegion = m_pRegionManager->GetGenericRegion( pNextRoadChunk->m_Location );
			//if( pNextRegion == this )
			if( pNextRegion->GetId() == pAIRoad->m_iAIRegion1 ||
				pNextRegion->GetId() == pAIRoad->m_iAIRegion2 )
			{
				//pNextRoadChunk->m_bTempVisited = true;
				RoadChunkList.AddTail( pNextRoadChunk );
			}
		}
	}

	// Reset the 'visited' flag because we'll need it set to false for other AIRegion::SetupAIRoadConnections
	while( TouchedRoadChunkList.IsEmpty() == FALSE )
	{
		CRoadChunk* pRoadChunk = TouchedRoadChunkList.RemoveHead();
		pRoadChunk->m_iTempVisited = 0;
	}
}
//#endif
#endif THINGS_TO_COMPILE_EVENTUALLY

void CEOSAIGenericRegion::ClearSightingAndPowerValueForEachPlayer()
{
	for( long iPlayer=0; iPlayer<m_PlayerVisibility01.m_iSize; iPlayer++ )
	{
		m_PlayerVisibility01[iPlayer] = 0.0f;
		m_PlayerCityUnitAIPower[iPlayer] = 0.0f;
		m_PlayerCitResControlValue01[iPlayer] = 0.0f;
	}
}

/*
void CEOSAIGenericRegion::CalculateSightingPresenceForEachPlayer()
{
	// INCOMPLETE:
	//   This should be redone - it should be based on unit view range, movement rates, and movement range
	//   Also, if I have a list of the nearby units, I can do better "sighting" calculations -
	//     for example, if a unit can't detect submarines, then it affects the "sighting" range.

	// Update the Sighting Presence for each Player
	/-*
	CEOSAIBCDumbArray1D< long >  PlayerUnitCount;
	PlayerUnitCount.SetSize( m_PlayerVisibility01.m_iSize );
	for( long iPlayer=1; iPlayer<m_PlayerVisibility01.m_iSize; iPlayer++ )
	{
		PlayerUnitCount[ iPlayer ] = GetCitResUnitCount( iPlayer );
		//m_PlayerVisibility01[iPlayer] = 1.0f - ( 1.0f / ( PlayerUnitCount[ iPlayer ] + 1.0f ) );
	}

	// Adjacent Regions have half value (at minimum)
	POSITION pos = m_AdjacentRegions.GetHeadPosition();
	while( pos )
	{
		CMultiRegion* pOtherRegion = m_AdjacentRegions.GetNext( pos );
		float fMult = 0.5f;
		for( long iPlayer=1; iPlayer<m_PlayerVisibility01.m_iSize; iPlayer++ )
		{
			if( PlayerUnitCount[ iPlayer ] == 0 ) continue;
			float fPresence = 1.0f - ( 1.0f / ( PlayerUnitCount[ iPlayer ] + 1.0f ) );

			float fVal = fMult * fPresence;
			if( fVal > pOtherRegion->m_PlayerVisibility01[iPlayer] )
			{
				pOtherRegion->m_PlayerVisibility01[iPlayer] = fVal;
			}
		}
	}

	// Update the surrounding areas with a Secondary Sighting Presence
	//CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();
	CWorldDesc* pWorldDesc = GetCommonState()->GetWorldDescServer();
	CEOSAIBCArray< CMultiRegion* >* pArray = m_pMultiRegionManager->GetMultiRegionArray();
	for( long iMultiRegion=0; iMultiRegion<pArray->m_iArraySize; iMultiRegion++ )
	{
		CMultiRegion* pOtherRegion = pArray->Get( iMultiRegion );
		if( pOtherRegion )
		{
			float fDistance = pWorldDesc->GetPixelDistance( GetLocation(), pOtherRegion->GetLocation() );

			if( fDistance < 200.0f )
			{
				float fMult = ( 200.0f - fDistance ) / 200.0f;
				if( fMult > 1.0f ){ fMult = 1.0f; }
				for( long iPlayer=1; iPlayer<m_PlayerVisibility01.m_iSize; iPlayer++ )
				{
					if( PlayerUnitCount[ iPlayer ] == 0 ) continue;
					float fPresence = 1.0f - ( 1.0f / ( PlayerUnitCount[ iPlayer ] + 1.0f ) );

					float fVal = fMult * fPresence;
					if( fVal > pOtherRegion->m_PlayerVisibility01[iPlayer] )
					{
						pOtherRegion->m_PlayerVisibility01[iPlayer] = fVal;
					}
				}
			}
		}
	}
	*-/
}
*/
float CEOSAIGenericRegion::GetPlayerAndSATPresence( long iPlayer )
{
	long iNumberOfPlayers = g_pEOSAICommonData->GetNumberOfPlayers();
	//long iMyPlayerNumber = m_pMultiRegionManager->GetAIBrain()->GetAIPlayerNumber();
	//CWorldDesc* pWorldDesc = m_pMultiRegionManager->GetAIBrain()->GetWorldDesc();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	float fPresence = 0.0f;
	for( long i = 1; i<iNumberOfPlayers+1; i++ )
	{
		if( i == iPlayer )
		{
			CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( i );
			fPresence += pSummary->GetCitResUnitPresence();
		}
		else
		{
			EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iPlayer, i );
			if( eRel.IsAlly() || eRel.IsTeammate() )
			{
				CEOSAICitResUnitSummary* pSummary = GetCitResUnitSummary( i );
				fPresence += pSummary->GetCitResUnitPresence();
			}
		}
	}
	return fPresence;
}

float CEOSAIGenericRegion::GetUnitPower( long iPlayer )
{
	CEOSAICitResUnitSummary* pSummary = &m_PlayerCitResUnitSummaries[iPlayer];
	return pSummary->GetUnitPower();
}

long CEOSAIGenericRegion::GetCitResUnitCount( long iPlayer )
{
	long iCount = 0;
	POSITION pos = m_AIPoiObjectsInsideRegion.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = m_AIPoiObjectsInsideRegion.GetNext( pos );
		if( pPoiObject->GetOwner() == iPlayer )
		{
			iCount++;
		}
	}
	return iCount;
}

long CEOSAIGenericRegion::GetCitGroundUnitCount( long iPlayer )
{
	long iCount = 0;
	POSITION pos = m_AIPoiObjectsInsideRegion.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = m_AIPoiObjectsInsideRegion.GetNext( pos );
		if( pPoiObject->GetOwner() == iPlayer )
		{
			if( dynamic_cast< CEOSAICity* >( pPoiObject ) )
			{
				iCount++;
			}
			if( dynamic_cast< CEOSAIUnit2* >( pPoiObject ) )
			{
				CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pPoiObject );
				if( pUnit->IsGroundUnit() )
				{
					iCount++;
				}
			}
		}
	}
	return iCount;
}

long CEOSAIGenericRegion::GetCitResCount( long iPlayer )
{
	long iCount = 0;
	POSITION pos = m_AIPoiObjectsInsideRegion.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = m_AIPoiObjectsInsideRegion.GetNext( pos );
		if( pPoiObject->GetOwner() == iPlayer )
		{
			if( dynamic_cast< CEOSAICity* >( pPoiObject ) )
			{
				iCount++;
			}
			if( dynamic_cast< CEOSAIResource* >( pPoiObject ) )
			{
				iCount++;
			}
		}
	}
	return iCount;
}

float CEOSAIGenericRegion::GetPlayerPresence01( long iPlayer )
{
	if( GetCitResUnitCount( iPlayer ) > 0 ) return 1.0f;
	return 0.0f;

	//return 1.0f - ( 1.0f / ( GetCitResUnitCount( iPlayer ) + 1.0f ) );
}

float  CEOSAIGenericRegion::GetWarzoneValue01( long iPlayer1, long iPlayer2 )
{
	float fValue01 = 0.0f;
	float fPlayer1Control01 = m_PlayerCitResControlValue01[ iPlayer1 ];
	float fPlayer2Control01 = m_PlayerCitResControlValue01[ iPlayer2 ];
	/*
	long iPlayers = pAIRegion->m_PlayerCitResControlValue01.m_iSize;
	for( long iOtherPlayer=1; iOtherPlayer<iPlayers; iOtherPlayer++ )
	{
		if( iLocalPlayer == iOtherPlayer ){ continue; }
		float fOtherPlayerControl01 = pAIRegion->m_PlayerCitResControlValue01[ iOtherPlayer ];
		fValue01 = max( fValue01, 2.0f * ( fLocalPlayerControl01 * fOtherPlayerControl01 ) );
	}
	*/
	fValue01 = 2.0f * ( fPlayer1Control01 * fPlayer2Control01 );
	if( fValue01 > 1.0f ){ fValue01 = 1.0f; }
	return fValue01;
}

void CEOSAIGenericRegion::AddPoiObjectProximity( CEOSAIPoiObject* pPoiObject, float fProximity01, float fActualDistance )
{
	ASSERT( fProximity01 > 0.0f );
	ASSERT( pPoiObject->IsDead() == false );

	// See if this is already in the list
	#ifdef _DEBUG
	/*
	POSITION pos = m_PoiObjectProximity.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObjectProximity* pProximity = m_PoiObjectProximity.GetNext( pos );
		if( pProximity->m_pPoiObject == pPoiObject )
		{
			ASSERT( false );
		}
	}
	*/
	#endif _DEBUG

	CEOSAIPoiObjectProximity* pProximity = new CEOSAIPoiObjectProximity();
	pProximity->m_pAIPoiObject = pPoiObject;
	pProximity->m_fProximity01 = fProximity01;
	pProximity->m_fDistanceFromRegionEdge = fActualDistance;
	m_AIPoiObjectProximity.AddTail( pProximity );

	// Set danger values (used by pathfinding)
	if( pPoiObject->GetOwner() > 0 )
	{
		//float fAIPowerValue = pPoiObject->GetAIPowerValue();
		m_PlayerCityUnitAIPower[ pPoiObject->GetOwner() ] += pPoiObject->GetAIPowerValue() * fProximity01;
	}
	//for( long iPlayer=0; iPlayer<m_PlayerVisibility01.m_iSize; iPlayer++ )
	//{
	//	m_AIPowerValue[iPlayer] += pPoiObject->GetAIPowerValue() * fProximity;
	//}
}
/*
void CEOSAIGenericRegion::AddToPlayerCitResControlValue01( CPoiObject* pPoiObject )
{
	//if( dynamic_cast< CCity* >( pPoiObject ) ||
	//	dynamic_cast< CResourceSource* >( pPoiObject ) )
	if( pPoiObject->GetAIPoiObjectType() == EnumPoiObjectType::enum_City ||
		pPoiObject->GetAIPoiObjectType() == EnumPoiObjectType::enum_Resource )
	{
		CEOSAILocation PoiObjectLocation = pPoiObject->GetLocation();
		CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

		// Update the 'player control' area
		CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		if( pWorldDescServer == NULL ){ ASSERT( false ); return; }

		CAICommonData* pAICommonData = pWorldDescServer->GetAICommonData();
		if( pAICommonData == NULL ){ ASSERT( false ); return; }

		long iPoiObjectOwner = pPoiObject->GetOwner();
		float fControl = 1.0f;
		if( iPoiObjectOwner == 0 ){ fControl = 0.5f; }
		m_PlayerCitResControlValue01[iPoiObjectOwner] = fControl;

		// Mark Nearby AIRegions as well
		CEOSAIRegionManager2* pAIRegionManager = (CEOSAIRegionManager2*) GetGenericRegionManager();
		POSITION pos = pAIRegionManager->m_GenericRegionList.GetHeadPosition();
		while( pos )
		{
			CEOSAIRegion2* pRegionInList = (CEOSAIRegion2*) pAIRegionManager->m_GenericRegionList.GetNext( pos );
			float fDistance = pWorldBuildDesc->GetPixelDistance( PoiObjectLocation, pRegionInList->GetLocation() );
			if( fDistance < 150.0f )
			{
				m_PlayerCitResControlValue01[iPoiObjectOwner] =
					max( m_PlayerCitResControlValue01[iPoiObjectOwner], fControl*(150.0f-fDistance)/150.0f );
			}
		}
		/-*
		long iNumberOfAIRegions = pAIRegionManager->GetAIRegionArray()->GetSize();
		POSITION pos = pAIRegionManager->GetAIRegionArray().GetHeadPosition();
		while( pos )
		{
			CEOSAIGenericRegion* pRegionInList = m_pRegionManager->m_GenericRegionList.GetNext( pos );
		}
		*-/
	}
}
*/

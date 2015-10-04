
#include "stdafx.h"
#include "EOSAITransportsSet.h"
//#include "CommonState.h"
//#include "WorldBuildDesc.h"
#include "EOSAIThoughtDatabase.h"
//#include "WorldDescServer.h"
#include "EOSAIBrain.h"
//#include "PoiObject.h"
//#include "Unit.h"
#include "City.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAITransportsSet::CEOSAITransportsSet( CEOSAIThoughtDatabase* pThoughtDatabase, CEOSAIUnitTemplate* pUnitToTransport )
{
	m_pThoughtDatabase = pThoughtDatabase;
	m_iPlayer = m_pThoughtDatabase->GetAIBrain()->GetAIPlayerNumber();
	m_pUnitToTransport = pUnitToTransport;

	// Right now, I can only calculate transports for the current player
	//ASSERT( pThoughtDatabase->GetAIBrain()->GetAIPlayerNumber() == iPlayer );

	// Create list of existing transports
	POSITION pos = m_pThoughtDatabase->GetMyActors()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pExistingPoiObject = m_pThoughtDatabase->GetMyActors()->GetNext( pos );
		CEOSAIUnit2* pExistingUnit = dynamic_cast< CEOSAIUnit2* >( pExistingPoiObject );
		if( pExistingUnit && pExistingUnit->CanContain_IgnoreForeignRelations( pUnitToTransport ) )
		{
			m_ExistingTransports.AddTail( pExistingUnit );
		}
	}

	// Create list of buildable transports
	CList< CEOSAIUnitTemplate* >* pBuildableUnits = &m_pThoughtDatabase->GetUnitsICanBuild()->m_List;
	pos = pBuildableUnits->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pBuildableUnit = pBuildableUnits->GetNext( pos );
		if( pBuildableUnit->CanContainUnits() && pBuildableUnit->CanContain( pUnitToTransport ) )
		{
			m_BuildableTransports.AddTail( pBuildableUnit );
		}
	}
	// Create list of cities (where new transports can be built)
	pos = m_pThoughtDatabase->GetMyCities()->GetHeadPosition();
	while( pos )
	{
		CEOSAICity* pAICity = m_pThoughtDatabase->GetMyCities()->GetNext( pos );
		m_Cities.AddTail( pAICity );
	}
}

float  CEOSAITransportsSet::GetEarliestArrivalTimeToLocation_UseCrowsFlight( CEOSAILocation Location )
{
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();

	float fShortestTimeToLocation = 1000000.0f;

	// Get the distance from existing transport to Location
	POSITION pos = m_ExistingTransports.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pTransport = m_ExistingTransports.GetNext( pos );
		//float fDistance = pWorldDescServer->GetPixelDistance( pTransport->GetInitialState()->GetLocation(), Location );
		float fDistance = g_pWorldDistanceTool->GetDistance( pTransport->GetInitialState()->GetLocation(), Location );
		//
		float fTimeToLocation = fDistance / pTransport->GetMovementRate();
		fShortestTimeToLocation = min( fShortestTimeToLocation, fTimeToLocation );
	}

	pos = m_Cities.GetHeadPosition();
	while( pos )
	{
		CEOSAICity* pAICity = m_Cities.GetNext( pos );
		//float fDistance = pWorldDescServer->GetPixelDistance( pAICity->GetInitialState()->GetLocation(), Location );
		float fDistance = g_pWorldDistanceTool->GetDistance( pAICity->GetInitialState()->GetLocation(), Location );
		//
		POSITION pos2 = m_BuildableTransports.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnitTemplate* pTransport = m_BuildableTransports.GetNext( pos2 );
			if( pTransport->GetMovementRate() <= 0.0f ) continue;

			float fTimeToLocation = fDistance / pTransport->GetMovementRate();
			if( fTimeToLocation < fShortestTimeToLocation )
			{
				// If there is a chance that this will beat-out the existing ShortestTime, calculate
				//   the build time
				//float fTimeToBuild = pAICity->GetServerCity()->GetTimeToBuild( pTransport, true );
				float fTimeToBuild = pAICity->GetTimeToBuild( pTransport, true );
				fTimeToLocation += fTimeToBuild;
				fShortestTimeToLocation = min( fShortestTimeToLocation, fTimeToLocation );
			}
		}
	}

	return fShortestTimeToLocation;
}

float  CEOSAITransportsSet::GetFastestMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos = m_ExistingTransports.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pAIUnit = m_ExistingTransports.GetNext( pos );
		fFastest = max( fFastest, pAIUnit->GetAIUnitTemplate()->GetMovementRate() );
	}
	pos = m_BuildableTransports.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_BuildableTransports.GetNext( pos );
		fFastest = max( fFastest, pAIUnitTemplate->GetMovementRate() );
	}
	return fFastest;
}

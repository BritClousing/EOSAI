
#include "stdafx.h"
#include "EOSAIAirbasesSet.h"
//#include "CommonState.h"
//#include "WorldBuildDesc.h"
//#include "PoiObject.h"
//#include "Unit.h"
#include "EOSAICommonData2.h"
#include "PoiObject.h"
#include "EOSAIUnit2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

float  CEOSAIAirbasesSet::GetClosestAirbaseDistance( CEOSAILocation Location )
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	float fShortestDistance = 1000000.0f;
	POSITION pos = m_Airbases.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = m_Airbases.GetNext( pos );
		//float fDistance = pWorldBuildDesc->GetPixelDistance( pPoiObject->GetLocation(), Location );
		float fDistance = g_pWorldDistanceTool->GetDistance( pPoiObject->GetLocation(), Location );
		fShortestDistance = min( fShortestDistance, fDistance );
	}
	return fShortestDistance;
}

float  CEOSAIAirbasesSet::GetClosestAirbaseDistanceX5Turns( CEOSAILocation Location ) // assuming carriers move towards point
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	float fShortestDistance = 1000000.0f;
	POSITION pos = m_Airbases.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = m_Airbases.GetNext( pos );
		//float fDistance = pWorldBuildDesc->GetPixelDistance( pPoiObject->GetLocation(), Location );
		float fDistance = g_pWorldDistanceTool->GetDistance( pPoiObject->GetLocation(), Location );

		float fMovementRate = 0.0f;
		CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pPoiObject );
		if( pAIUnit )
		{
			fMovementRate = pAIUnit->GetMovementRate();
		}
		fDistance -= fMovementRate*5.0f;
		if( fDistance < 0.0f ){ fDistance = 0.0f; }

		fShortestDistance = min( fShortestDistance, fDistance );
	}
	return fShortestDistance;
}

float  CEOSAIAirbasesSet::GetClosestAirbaseDistance_IgnoreOneAirbase( CEOSAIPoiObject* pAirbase, CEOSAILocation Location )
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	float fShortestDistance = 1000000.0f;
	POSITION pos = m_Airbases.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = m_Airbases.GetNext( pos );
		if( pPoiObject == pAirbase ) continue;

		//float fDistance = pWorldBuildDesc->GetPixelDistance( pPoiObject->GetLocation(), Location );
		float fDistance = g_pWorldDistanceTool->GetDistance( pPoiObject->GetLocation(), Location );
		fShortestDistance = min( fShortestDistance, fDistance );
	}
	return fShortestDistance;
}


void CEOSAIAirbasesSet::GetAirbasesWithRange( CEOSAILocation Location, float fRange, CEOSAIAirbasesSet* pNewAirbasesSet )
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	pNewAirbasesSet->m_Airbases.RemoveAll();
	POSITION pos = m_Airbases.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = m_Airbases.GetNext( pos );
		//float fDistance = pWorldBuildDesc->GetPixelDistance( Location, pPoiObject->GetLocation() );
		float fDistance = g_pWorldDistanceTool->GetDistance( Location, pPoiObject->GetLocation() );
		if( fDistance < fRange )
		{
			pNewAirbasesSet->m_Airbases.AddTail( pPoiObject );
		}
	}
}

CEOSAIPoiObject* CEOSAIAirbasesSet::GetClosestAirbase( CEOSAILocation Location )
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	CEOSAIPoiObject* pClosestAirbase = NULL;
	float fShortestDistance = 1000000.0f;
	POSITION pos = m_Airbases.GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pPoiObject = m_Airbases.GetNext( pos );

		//float fDistance = pWorldBuildDesc->GetPixelDistance( pPoiObject->GetLocation(), Location );
		float fDistance = g_pWorldDistanceTool->GetDistance( pPoiObject->GetLocation(), Location );
		if( fShortestDistance > fDistance )
		{
			fShortestDistance = fDistance;
			pClosestAirbase = pPoiObject;
		}
	}
	return pClosestAirbase;
}

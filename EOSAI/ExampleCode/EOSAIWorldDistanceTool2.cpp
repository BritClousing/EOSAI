
#include "stdafx.h"
//#include "AIWorldDistanceTool.h"
//#include "AIWorldDistanceTool2.h"
#include "EOSAIWorldDistanceTool2.h"
#include "WorldBuildDesc.h"

CEOSAIWorldDistanceTool* g_pAIWorldDistanceTool = NULL;
/*
CEOSAILocation CEOSAIWorldDistanceTool2::GetAverageLocation( CList< CEOSAILocation >* pLocations )
{
	ASSERT( pLocations->IsEmpty() == FALSE );

	CEOSAILocation CenterLocation;
	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;
	long iCount = 0;
	POSITION pos = pLocations->GetHeadPosition();
	while( pos )
	{
		//CEOSAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		CEOSAILocation Location = pLocations->GetNext( pos );
		iCount++;
		if( iCount == 1 )
		{
			CenterLocation = Location;
		}
		else
		{
			float fX,fY;
			/-*
			GetAIBrain()->GetWorldDescServer()->GetPixelDirection( 
				CenterLocation.GetRealX(), CenterLocation.GetRealY(),
				Location.GetRealX(), Location.GetRealY(),
				&fX, &fY );
			*-/
			m_pWorldBuildDesc->GetPixelDirection( 
				CenterLocation.GetRealX(), CenterLocation.GetRealY(),
				Location.GetRealX(), Location.GetRealY(),
				&fX, &fY );
			fOffsetX += fX;
			fOffsetY += fY;
		}
	}
	if( iCount > 1 )
	{
		fOffsetX /= iCount;
		fOffsetY /= iCount;
	}
	CenterLocation.SetReal( CenterLocation.GetRealX()-fOffsetX, CenterLocation.GetRealY()-fOffsetY );
	//GetAIBrain()->GetWorldDescServer()->GetWorldBuildDesc()->TruncateLocation( &CenterLocation );
	m_pWorldBuildDesc->TruncateLocation( &CenterLocation );
	return CenterLocation;

}
*/

float CEOSAIWorldDistanceTool2::GetDistance( CEOSAILocation Location1, CEOSAILocation Location2 )
{
	return m_pWorldBuildDesc->GetPixelDistance( 
		Location1.GetRealX(), Location1.GetRealY(),
		Location2.GetRealX(), Location2.GetRealY() );
}


CEOSAILocation CEOSAIWorldDistanceTool2::GetAverageLocation( CList< CEOSAILocation >* pLocations )
{
	ASSERT( pLocations->IsEmpty() == FALSE );

	CLocation CenterLocation;
	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;
	long iCount = 0;

	POSITION pos = pLocations->GetHeadPosition();
	while( pos )
	{
		//CAIDesireSpatial* pDesire = m_PrimaryGoals.GetNext( pos );
		CEOSAILocation EOSAILocation = pLocations->GetNext( pos );
		CLocation Location( Pixel, EOSAILocation.GetRealX(), EOSAILocation.GetRealY() );
		iCount++;
		if( iCount == 1 )
		{
			CenterLocation = Location;
		}
		else
		{
			float fX,fY;
			m_pWorldBuildDesc->GetPixelDirection( 
				CenterLocation.GetRealX(), CenterLocation.GetRealY(),
				Location.GetRealX(), Location.GetRealY(),
				&fX, &fY );
			fOffsetX += fX;
			fOffsetY += fY;
		}
	}
	if( iCount > 1 )
	{
		fOffsetX /= iCount;
		fOffsetY /= iCount;
	}
	CenterLocation.SetReal( CenterLocation.GetRealX()-fOffsetX, CenterLocation.GetRealY()-fOffsetY );
	m_pWorldBuildDesc->TruncateLocation( &CenterLocation );

	CEOSAILocation EOSAICenterLocation( CEOSAILocation::Pixel, CenterLocation.GetRealX(), CenterLocation.GetRealY() );
	return EOSAICenterLocation;
}



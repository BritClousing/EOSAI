
#include "stdafx.h"
#include "EOSAIWorldDistanceTool.h"

using namespace EOSAI;

//#ifdef GAME_CODE
// This is some example code
CEOSAILocation CWorldDistanceTool::GetAverageLocation( CList< CEOSAILocation >* pLocations )
{
	// This method mst be overridden
	ASSERT( FALSE );
	ASSERT( pLocations->IsEmpty() == FALSE );

	CEOSAILocation CenterLocation;
	return CenterLocation;
/*
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
			GetAIBrain()->GetWorldDescServer()->GetPixelDirection( 
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
	GetAIBrain()->GetWorldDescServer()->GetWorldBuildDesc()->TruncateLocation( &CenterLocation );
	return CenterLocation;
*/
}
//#endif


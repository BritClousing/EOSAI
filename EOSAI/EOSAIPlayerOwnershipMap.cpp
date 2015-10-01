
#include "stdafx.h"
#include "EOSAIPlayerOwnershipMap.h"
#include "EOSAISerial.h"
#include <math.h>

CEOSAIPlayerOwnershipMap::CEOSAIPlayerOwnershipMap()
{
	m_iTurn = -1;
}

void CEOSAIPlayerOwnershipMap::Allocate( long iWidth, long iHeight, bool bWrapRight, bool bWrapBottom )
{
	if( m_Player.GetWidth() == iWidth &&
		m_Player.GetHeight() == iHeight &&
		m_Player.WrapRight() == bWrapRight &&
		m_Player.WrapBottom() == bWrapBottom )
	{
		return;
	}

	m_Player.Allocate( iWidth, iHeight );
	m_Player.SetVirtualSize( iWidth*8, iHeight*8, EOSAIEnumStretchType_STRETCH );
	m_Player.WrapRight( bWrapRight );
	m_Player.WrapBottom( bWrapBottom );
	m_Player.Clear( 0 );

	m_Ownership.Allocate( iWidth, iHeight );
	m_Ownership.SetVirtualSize( iWidth*8, iHeight*8, EOSAIEnumStretchType_STRETCH );
	m_Ownership.WrapRight( bWrapRight );
	m_Ownership.WrapBottom( bWrapBottom );
	m_Ownership.Clear( 0 );
}

void CEOSAIPlayerOwnershipMap::Clear()
{
	m_Player.Clear( 0 );
	m_Ownership.Clear( 0 );
}

void CEOSAIPlayerOwnershipMap::Copy( CEOSAIPlayerOwnershipMap& OtherMap )
{
	if( OtherMap.m_Player.GetWidth() == 0 ){ ASSERT( false ); return; }

	m_iTurn = OtherMap.m_iTurn;

	Allocate( OtherMap.m_Player.GetWidth(), OtherMap.m_Player.GetHeight(), OtherMap.m_Player.WrapRight(), OtherMap.m_Player.WrapBottom() );
	m_Player.Copy( OtherMap.m_Player );
	m_Ownership.Copy( OtherMap.m_Ownership );
}

void CEOSAIPlayerOwnershipMap::Serialize( CEOSAISerial* pSerial )
{
	pSerial->Serialize( m_iTurn );
	m_Player.Serialize_TerrainCompression( pSerial );
	m_Ownership.Serialize_TerrainCompression( pSerial );
}

void CEOSAIPlayerOwnershipMap::Deserialize( CEOSAISerial* pSerial )
{
	pSerial->Deserialize( m_iTurn );
	m_Player.Deserialize_TerrainCompression( pSerial );
	m_Ownership.Deserialize_TerrainCompression( pSerial );
}

long CEOSAIPlayerOwnershipMap::GetOwner( float fPixelX, float fPixelY )
{
	int cx = (long) (fPixelX/8.0f);
	int cy = (long) (fPixelY/8.0f);
	if( m_Player.TruncateAndValidateGridLocation( &cx,&cy ) )
	{
		return m_Player.GridValue( cx,cy );
	}
	return 0;
}

float CEOSAIPlayerOwnershipMap::GetDistanceToEscapeOwnedArea( float fPixelX, float fPixelY, long iOwner )
{
	long iCenterX = (long) (fPixelX/8.0f);
	long iCenterY = (long) (fPixelY/8.0f);
	long iDistanceToCheck = 0;
	bool bEscapeLocationFound = false;
	int cx, cy;
	while( bEscapeLocationFound == false )
	{
		float fDistance = 1000000.0f;

		int iMinX = iCenterX-iDistanceToCheck;
		int iMaxX = iCenterX+iDistanceToCheck;
		int iMinY = iCenterY-iDistanceToCheck;
		int iMaxY = iCenterY+iDistanceToCheck;
		for( long x=iMinX; x<=iMaxX; x++ )
		{
			cx = x; cy = iMinY;
			if( m_Player.TruncateAndValidateGridLocation( &cx,&cy ) )
			{
				if( m_Player.GridValue( cx,cy ) != iOwner )
				{
					fDistance = min( fDistance, 8.0f*sqrt( (float) (iCenterX-x)*(iCenterX-x) + (iCenterY-iMinY)*(iCenterY-iMinY) ) );
				}
			}
			cx = x; cy = iMaxY;
			if( m_Player.TruncateAndValidateGridLocation( &cx,&cy ) )
			{
				//if( m_Player.GridValue( cx,cy ) != iOwner ){ return 8.0f*iDistanceToCheck; }
				if( m_Player.GridValue( cx,cy ) != iOwner )
				{
					fDistance = min( fDistance, 8.0f*sqrt( (float) (iCenterX-x)*(iCenterX-x) + (iCenterY-iMaxY)*(iCenterY-iMaxY) ) );
				}
			}
		}
		for( long y=iMinY; y<=iMaxY; y++ )
		{
			cx = iMinX; cy = y;
			if( m_Player.TruncateAndValidateGridLocation( &cx,&cy ) )
			{
				//if( m_Player.GridValue( cx,cy ) != iOwner ){ return 8.0f*iDistanceToCheck; }
				if( m_Player.GridValue( cx,cy ) != iOwner )
				{
					fDistance = min( fDistance, 8.0f*sqrt( (float) (iCenterX-iMinX)*(iCenterX-iMinX) + (iCenterY-y)*(iCenterY-y) ) );
				}
			}
			cx = iMaxX; cy = y;
			if( m_Player.TruncateAndValidateGridLocation( &cx,&cy ) )
			{
				//if( m_Player.GridValue( cx,cy ) != iOwner ){ return 8.0f*iDistanceToCheck; }
				if( m_Player.GridValue( cx,cy ) != iOwner )
				{
					fDistance = min( fDistance, 8.0f*sqrt( (float) (iCenterX-iMaxX)*(iCenterX-iMaxX) + (iCenterY-y)*(iCenterY-y) ) );
				}
			}
		}
		if( fDistance < 999999.0f )
		{
			return fDistance;
		}
		iDistanceToCheck++;
	}
	return 1000000.0f;
}

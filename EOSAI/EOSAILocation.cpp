
#include "stdafx.h"

#include "EOSAILocation.h"
#include "EOSAISerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CEOSAILocation::CEOSAILocation( EOSAILocationType type, float x, float y )
{
	if( type == Pixel )
	{
		m_fRealX = (float) x;
		m_fRealY = (float) y;
	}
	if( type == Grid )
	{
		m_fRealX = (float)( x*4+2.0 );
		m_fRealY = (float)( y*4+2.0 );
	}
}

CEOSAILocation::CEOSAILocation( EOSAILocationType type, int x, int y )
{ 
	if( type == Pixel )
	{
		m_fRealX = (float) x;
		m_fRealY = (float) y;
	}
	if( type == Grid )
	{
		m_fRealX = (float)( x*4+2.0 );
		m_fRealY = (float)( y*4+2.0 );
	}
	//m_fRealX = (float)( x*4+2.0 );
	//m_fRealY = (float)( y*4+2.0 );
}

void  CEOSAILocation::Serialize( CEOSAISerial* pSerial )
{
	// A version number
	//pSerial->AppendLong( 1 );

	pSerial->Serialize( m_fRealX );
	pSerial->Serialize( m_fRealY );
}

void  CEOSAILocation::Deserialize( CEOSAISerial* pSerial )
{
	// A version number
	//int iVersion = pSerial->ExtractLong();

	pSerial->Deserialize( m_fRealX );
	pSerial->Deserialize( m_fRealY );
}

/*
float CEOSAILocation::GetPixelDistance( CEOSAILocation& otherLocation )
{
	float fDistanceX = fabs( m_fRealX - otherLocation.m_fRealX );
	float fDistanceY = fabs( m_fRealY - otherLocation.m_fRealY );

	// take into account screen wrapping
	if( fDistanceX > 

	if( CurrentMap.WrapRight() && fDistanceX > CurrentMap.RealSizeX()/2.0f )
	{
		fDistanceX = CurrentMap.RealSizeX() - fDistanceX;
	}
	if( CurrentMap.WrapBottom() && fDistanceY > CurrentMap.RealSizeY()/2.0f )
	{
		fDistanceY = CurrentMap.RealSizeY() - fDistanceY;
	}
	return sqrt( fDistanceX*fDistanceX + fDistanceY*fDistanceY );
	}
	float GetGridDistance( CEOSAILocation& otherLocation )
	{ 
	return GetRealDistance( otherLocation ) / 4.0f;
	}

	void GetRealDirection( CEOSAILocation& otherLocation, float* x, float* y )
	{ 
	float fDirectionX = otherLocation.m_fRealX - m_fRealX;
	float fDirectionY = otherLocation.m_fRealY - m_fRealY;

	// take into account screen wrapping
	if( CurrentMap.WrapRight() && fabs( fDirectionX ) > CurrentMap.RealSizeX()/2.0f )
	{
		if( fDirectionX > 0 ) fDirectionX = fDirectionX - CurrentMap.RealSizeX();
		else                  fDirectionX = CurrentMap.RealSizeX() - fDirectionX;
	}
	if( CurrentMap.WrapBottom() && fabs( fDirectionY ) > CurrentMap.RealSizeY()/2.0f )
	{
		if( fDirectionY > 0 ) fDirectionY = fDirectionY - CurrentMap.RealSizeY();
		else                  fDirectionY = CurrentMap.RealSizeY() - fDirectionY;
	}

	*x = fDirectionX;
	*y = fDirectionY;
}
*/

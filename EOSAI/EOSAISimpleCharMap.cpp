
#include "stdafx.h"

#include <math.h>
#include "EOSAISimpleCharMap.h"
#include "EOSAISerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CEOSAISimpleCharMap::CEOSAISimpleCharMap()
{
	m_aMapData    = 0;
	m_iWidth      = 0;
	m_iHeight     = 0;

	m_bWrapRight  = false;
	m_bWrapBottom = false;
}

CEOSAISimpleCharMap::CEOSAISimpleCharMap( int width, int height )
{
	m_aMapData    = 0;
	m_iWidth      = 0;
	m_iHeight     = 0;

	m_bWrapRight  = false;
	m_bWrapBottom = false;

	Allocate( width, height );
}

CEOSAISimpleCharMap::~CEOSAISimpleCharMap()
{
	delete[] m_aMapData;
	m_aMapData = 0;
}

// Serialization
//
void CEOSAISimpleCharMap::Serialize( CEOSAISerial* pSerial )
{
	pSerial->Serialize( m_iWidth );
	pSerial->Serialize( m_iHeight );
	pSerial->SerializeBlock( m_aMapData, m_iWidth*m_iHeight );

	pSerial->Serialize( m_bWrapRight );
	pSerial->Serialize( m_bWrapBottom );

	long iVirtualStretchType = (long) m_eVirtualStretchType;
	pSerial->Serialize( iVirtualStretchType );
	pSerial->Serialize( m_iVirtualWidth );
	pSerial->Serialize( m_iVirtualHeight );
}

void CEOSAISimpleCharMap::Deserialize( CEOSAISerial* pSerial )
{
	long iWidth = 0;
	long iHeight = 0;
	pSerial->Deserialize( iWidth );
	pSerial->Deserialize( iHeight );
	Allocate( iWidth, iHeight );
	pSerial->DeserializeBlock( m_aMapData, m_iWidth*m_iHeight );

	pSerial->Deserialize( m_bWrapRight );
	pSerial->Deserialize( m_bWrapBottom );

	long iVirtualStretchType;
	pSerial->Deserialize( iVirtualStretchType );
	m_eVirtualStretchType = (EOSAIEnumStretchType) iVirtualStretchType;

	pSerial->Deserialize( m_iVirtualWidth );
	pSerial->Deserialize( m_iVirtualHeight );
}

void CEOSAISimpleCharMap::Serialize_ElevationCompression( CEOSAISerial* pSerial )
{
	pSerial->Serialize( m_iWidth );
	pSerial->Serialize( m_iHeight );
	//pSerial->SerializeBlock( m_aMapData, m_iWidth*m_iHeight );
	//long iState = 0; // State0 = elevation 0 or 1, State2 = elevation 2 or higher
	unsigned char cCurrentElevation = 0;
	unsigned char cCurrentSpan = 0;
	long iCount = 0;
	for( long x=0; x<m_iWidth; x++ )
	{
		for( long y=0; y<m_iHeight; y++ )
		{
			unsigned char cNewValue = GridValueUnsafe( x,y );
			if( cNewValue == cCurrentElevation ) // NewValue same as CurrentValue
			{
				if( cNewValue == 0 || cNewValue == 1 )
				{
					cCurrentSpan++;

					if( cCurrentSpan == 255 )
					{
						pSerial->Serialize( cCurrentElevation );
						pSerial->Serialize( cCurrentSpan );
						iCount += cCurrentSpan;
						cCurrentSpan = 0;
					}
				}
				else
				{
					iCount++;
					pSerial->Serialize( cNewValue );
					cCurrentSpan = 0;
				}
			}
			else  // NewValue different than CurrentValue
			{
				// The value changed compared to the previous one
				//   If there was a span being created, write it out
				if( cCurrentSpan > 0 )
				{
					pSerial->Serialize( cCurrentElevation );
					pSerial->Serialize( cCurrentSpan );
					iCount += cCurrentSpan;
					cCurrentSpan = 0;
				}

				if( cNewValue == 0 || cNewValue == 1 )
				{
					cCurrentElevation = cNewValue;
					cCurrentSpan = 1;
				}
				else
				{
					iCount++;
					cCurrentElevation = cNewValue;
					pSerial->Serialize( cCurrentElevation );
					cCurrentSpan = 0;
				}
			}
			cCurrentElevation = cNewValue;
		}
	}
	// Add the last span
	if( cCurrentSpan > 0 )
	{
		pSerial->Serialize( cCurrentElevation );
		pSerial->Serialize( cCurrentSpan );
		iCount += cCurrentSpan;
	}
	//

	pSerial->Serialize( m_bWrapRight );
	pSerial->Serialize( m_bWrapBottom );

	long iVirtualStretchType = (long) m_eVirtualStretchType;
	pSerial->Serialize( iVirtualStretchType );
	pSerial->Serialize( m_iVirtualWidth );
	pSerial->Serialize( m_iVirtualHeight );
}

void CEOSAISimpleCharMap::Deserialize_ElevationCompression( CEOSAISerial* pSerial )
{
	long iWidth = 0;
	long iHeight = 0;
	pSerial->Deserialize( iWidth );
	pSerial->Deserialize( iHeight );
	Allocate( iWidth, iHeight );
	//pSerial->DeserializeBlock( m_aMapData, m_iWidth*m_iHeight );
	unsigned char cCurrentValue = 0;
	unsigned char cCurrentSpan = 0;
	long iCurrentX = 0;
	long iCurrentY = 0;
	long iCount = 0;
	while( true )
	{
		pSerial->Deserialize( cCurrentValue );
		if( cCurrentValue <= 1 )
		{
			pSerial->Deserialize( cCurrentSpan );
			for( long i=0; i<cCurrentSpan; i++ )
			{
				iCount++;
				GridValueUnsafe( iCurrentX,iCurrentY ) = cCurrentValue;
				iCurrentY++;
				if( iCurrentY == iHeight )
				{
					iCurrentY = 0;
					iCurrentX+= 1;
				}
				ASSERT( iCurrentX < iWidth || iCurrentY == 0 );
			}
		}
		else
		{
			iCount++;
			GridValueUnsafe( iCurrentX,iCurrentY ) = cCurrentValue;
			iCurrentY++;
		}

		if( iCurrentY == iHeight )
		{
			iCurrentY = 0;
			iCurrentX+= 1;
		}
		ASSERT( iCurrentX < iWidth || iCurrentY == 0 );
		if( iCurrentX >= iWidth )
		{
			break;
		}
	}

	pSerial->Deserialize( m_bWrapRight );
	pSerial->Deserialize( m_bWrapBottom );

	long iVirtualStretchType;
	pSerial->Deserialize( iVirtualStretchType );
	m_eVirtualStretchType = (EOSAIEnumStretchType) iVirtualStretchType;

	pSerial->Deserialize( m_iVirtualWidth );
	pSerial->Deserialize( m_iVirtualHeight );
}

void CEOSAISimpleCharMap::Serialize_TerrainCompression( CEOSAISerial* pSerial )
{
	pSerial->Serialize( m_iWidth );
	pSerial->Serialize( m_iHeight );
	//pSerial->SerializeBlock( m_aMapData, m_iWidth*m_iHeight );
	unsigned char cCurrentValue = 0;
	unsigned char cCurrentSpan = 0;
	long iCount = 0;
	for( long x=0; x<m_iWidth; x++ )
	{
		for( long y=0; y<m_iHeight; y++ )
		{
			unsigned char cNewValue = GridValueUnsafe( x,y );
			if( cNewValue == cCurrentValue )
			{
				cCurrentSpan++;
			}
			else
			{
				// Value changed, write-out the values
				if( cCurrentSpan > 0 ) // cCurrentSpan == 0 can happen the first time
				{
					pSerial->Serialize( cCurrentValue );
					pSerial->Serialize( cCurrentSpan );
					iCount += cCurrentSpan;
				}
				cCurrentValue = cNewValue;
				cCurrentSpan = 1;
			}

			if( cCurrentSpan == 255 )
			{
				pSerial->Serialize( cCurrentValue );
				pSerial->Serialize( cCurrentSpan );
				iCount += cCurrentSpan;
				cCurrentSpan = 0;
			}
		}
	}
	// Add the last span
	pSerial->Serialize( cCurrentValue );
	pSerial->Serialize( cCurrentSpan );
	iCount += cCurrentSpan;
	//

	pSerial->Serialize( m_bWrapRight );
	pSerial->Serialize( m_bWrapBottom );

	long iVirtualStretchType = (long) m_eVirtualStretchType;
	pSerial->Serialize( iVirtualStretchType );
	pSerial->Serialize( m_iVirtualWidth );
	pSerial->Serialize( m_iVirtualHeight );
}

void CEOSAISimpleCharMap::Deserialize_TerrainCompression( CEOSAISerial* pSerial )
{
	long iWidth = 0;
	long iHeight = 0;
	pSerial->Deserialize( iWidth );
	pSerial->Deserialize( iHeight );
	Allocate( iWidth, iHeight );
	//pSerial->DeserializeBlock( m_aMapData, m_iWidth*m_iHeight );
	unsigned char cCurrentValue = 0;
	unsigned char cCurrentSpan = 0;

	long iCurrentX = 0;
	long iCurrentY = 0;
	long iCount = 0;
	while( true )
	{
		pSerial->Deserialize( cCurrentValue );
		pSerial->Deserialize( cCurrentSpan );
		ASSERT( cCurrentSpan != 0 );
		iCount += cCurrentSpan;
		for( long i=0; i<cCurrentSpan; i++ )
		{
			ASSERT( iCurrentX >= 0 && iCurrentX < iWidth );
			ASSERT( iCurrentY >= 0 && iCurrentY < iHeight );
			GridValueUnsafe( iCurrentX,iCurrentY ) = cCurrentValue;
			iCurrentY++;
			if( iCurrentY == iHeight )
			{
				iCurrentY = 0;
				iCurrentX+= 1;
			}
			ASSERT( iCurrentX < iWidth || iCurrentY == 0 );
		}
		if( iCurrentX >= iWidth )
		{
			break;
		}
	}

	//ASSERT( cCurrentSpan != 255 );
	//bool bDebug = false;
	//if( bDebug )
	if( cCurrentSpan == 255 ) // Workaround for an error in my compression algorithm
	{
		pSerial->Deserialize( cCurrentValue );
		pSerial->Deserialize( cCurrentSpan );
	}

	pSerial->Deserialize( m_bWrapRight );
	pSerial->Deserialize( m_bWrapBottom );

	long iVirtualStretchType;
	pSerial->Deserialize( iVirtualStretchType );
	m_eVirtualStretchType = (EOSAIEnumStretchType) iVirtualStretchType;

	pSerial->Deserialize( m_iVirtualWidth );
	pSerial->Deserialize( m_iVirtualHeight );
	ASSERT( m_iVirtualWidth < 10000 );
	ASSERT( m_iVirtualHeight < 10000 );
}


//
bool CEOSAISimpleCharMap::Allocate( int width, int height )
{
	delete[] m_aMapData;
	m_aMapData = 0;

	// Initialization
	m_aMapData = new char[ width * height ];
	if( m_aMapData )
	{
		m_iWidth  = width;
		m_iHeight = height;
		return true;
	}
	else
	{
		delete[] m_aMapData;
		m_aMapData = 0;
		m_iWidth  = 0;
		m_iHeight = 0;
		return false;
	}
}

void CEOSAISimpleCharMap::Free()
{
	delete[] m_aMapData;
	m_aMapData = 0;
	m_iWidth   = 0;
	m_iHeight  = 0;
}

// Does not stretch, just truncates or adds new space
void CEOSAISimpleCharMap::Resize( long iNewGridWidth, long iNewGridHeight, long iClearValue )
{
	// Create the new map
	char* aMapData = new char[ iNewGridWidth * iNewGridHeight ];

	// Clear it
	for( long y=0; y<iNewGridHeight; y++ )
	{
		for( long x=0; x<iNewGridWidth; x++ )
		{
			aMapData[x+y*iNewGridWidth] = (char) iClearValue;
		}
	}

	// Copy the old data
	long iMaxX = iNewGridWidth;
	if( iMaxX > m_iWidth ){ iMaxX = m_iWidth; }
	long iMaxY = iNewGridHeight;
	if( iMaxY > m_iHeight ){ iMaxY = m_iHeight; }
	for( long y=0; y<iMaxY; y++ )
	{
		for( long x=0; x<iMaxX; x++ )
		{
			aMapData[x+y*iNewGridWidth] = m_aMapData[x+y*m_iWidth];
		}
	}

	char* aMapDataTemp = m_aMapData;

	// Switch
	m_iWidth = iNewGridWidth;
	m_iHeight = iNewGridHeight;
	m_aMapData = aMapData;

	delete[] aMapDataTemp;
}

void CEOSAISimpleCharMap::Copy( CEOSAISimpleCharMap& other )
{
	ASSERT( m_iWidth == other.m_iWidth );
	ASSERT( m_iHeight == other.m_iHeight );
	
	if( m_iWidth == other.m_iWidth &&
		m_iHeight == other.m_iHeight )
	{
		long iDataSize = m_iWidth*m_iHeight;
		memcpy( m_aMapData, other.m_aMapData, iDataSize );

		m_bWrapRight = other.m_bWrapRight;
		m_bWrapBottom = other.m_bWrapBottom;

		// Virtual settings
		m_eVirtualStretchType = other.m_eVirtualStretchType;
		m_iVirtualWidth = other.m_iVirtualWidth;
		m_iVirtualHeight = other.m_iVirtualHeight;
	}
}

void CEOSAISimpleCharMap::Copy( CEOSAISimpleCharMap& map, int iNewPixelSize, EOSAIEnumStretchType stretchtype, bool bWrapRight, bool bWrapBottom )
{
	if( stretchtype == EOSAIEnumStretchType_STRETCH ){

		long newSpan = iNewPixelSize*map.GetWidth();

		long lSizeChangeX = iNewPixelSize;
		long lSizeChangeY = iNewPixelSize;
		for( long i=0; i<map.GetWidth(); i++ ){
			for( long j=0; j<map.GetHeight(); j++ ){

				for( long i2=0; i2<lSizeChangeX; i2++ ){
					for( long j2=0; j2<lSizeChangeY; j2++ ){
						int posX = i*lSizeChangeX + i2;
						int posY = j*lSizeChangeY + j2;
						GridValue( posX, posY ) = map.GridValue( i, j );
					}
				}
				
			}
		}
	}
	else if( stretchtype == EOSAIEnumStretchType_ANTIALIAS ){

		long newSpan = iNewPixelSize*map.GetWidth();

		long lSizeChangeX = iNewPixelSize;
		long lSizeChangeY = iNewPixelSize;

		float fSizeX = (float) lSizeChangeX;
		float fSizeY = (float) lSizeChangeY;

		float elevation0;
		float elevation1;
		float elevation2;
		float elevation3;

		for( long i=0; i<map.GetWidth(); i++ ){
			for( long j=0; j<map.GetHeight(); j++ ){

				if( i < map.GetWidth()-1 && j < map.GetHeight()-1 )
				{
					elevation0 = (float) map.GridValue( i,  j   );
					elevation1 = (float) map.GridValue( i+1,j   );
					elevation2 = (float) map.GridValue( i  ,j+1 );
					elevation3 = (float) map.GridValue( i+1,j+1 );
				}
				else
				{
					if( i == map.GetWidth()-1 && j == map.GetHeight()-1 )
					{
						if( bWrapRight && bWrapBottom )
						{
							elevation0 = (float) map.GridValue( i,j );
							elevation1 = (float) map.GridValue( 0,j );
							elevation2 = (float) map.GridValue( i,0 );
							elevation3 = (float) map.GridValue( 0,0 );
						}
						else if( bWrapRight )
						{
							elevation0 = (float) map.GridValue( i,j );
							elevation1 = (float) map.GridValue( 0,j );
							elevation2 = (float) map.GridValue( i,j );
							elevation3 = (float) map.GridValue( 0,j );
						}
						else if( bWrapBottom )
						{
							elevation0 = (float) map.GridValue( i,j );
							elevation1 = (float) map.GridValue( i,j );
							elevation2 = (float) map.GridValue( i,0 );
							elevation3 = (float) map.GridValue( i,0 );
						}
						else
						{
							elevation0 = (float) map.GridValue( i,j );
							elevation1 = (float) map.GridValue( i,j );
							elevation2 = (float) map.GridValue( i,j );
							elevation3 = (float) map.GridValue( i,j );
						}
					}
					else if( i == map.GetWidth()-1 )
					{
						if( bWrapRight )
						{
							elevation0 = (float) map.GridValue( i,j   );
							elevation1 = (float) map.GridValue( 0,j   );
							elevation2 = (float) map.GridValue( i,j+1 );
							elevation3 = (float) map.GridValue( 0,j+1 );
						}
						else
						{
							elevation0 = (float) map.GridValue( i,j   );
							elevation1 = (float) map.GridValue( i,j   );
							elevation2 = (float) map.GridValue( i,j+1 );
							elevation3 = (float) map.GridValue( i,j+1 );
						}
					}
					else if( j == map.GetHeight()-1 )
					{
						if( bWrapBottom )
						{
							elevation0 = (float) map.GridValue( i,  j );
							elevation1 = (float) map.GridValue( i+1,j );
							elevation2 = (float) map.GridValue( i  ,0 );
							elevation3 = (float) map.GridValue( i+1,0 );
						}
						else
						{
							elevation0 = (float) map.GridValue( i,  j );
							elevation1 = (float) map.GridValue( i+1,j );
							elevation2 = (float) map.GridValue( i  ,j );
							elevation3 = (float) map.GridValue( i+1,j );
						}
					}
				}

				for( long i2=0; i2<lSizeChangeX; i2++ ){
					for( long j2=0; j2<lSizeChangeY; j2++ ){
						int posX = i*lSizeChangeX + i2;
						int posY = j*lSizeChangeY + j2;

						float fX = (float) i2;
						float fY = (float) j2;

						float fCalculatedElevation1 =
							( ((fSizeX-fX)/fSizeX)*elevation0 + ( fX/fSizeX )*elevation1 ) * (fSizeY-fY)/fSizeY;
						float fCalculatedElevation2 =
							( ((fSizeX-fX)/fSizeX)*elevation2 + ( fX/fSizeX )*elevation3 ) * (fY)/fSizeY;

						GridValue( posX, posY ) = (int) ( fCalculatedElevation1 + fCalculatedElevation2 + 0.5f );
					}
				}
				
			}
		}
	}
}

void CEOSAISimpleCharMap::Change( int initialValue, int newValue )
{
	for( long x=0; x<GetWidth(); x++ ){
		for( long y=0; y<GetHeight(); y++ ){
			if( GridValue( x, y ) == initialValue )
			{
				GridValue( x, y ) = newValue;
			}
		}
	}
}

char CEOSAISimpleCharMap::PixelValue( int x, int y )
{
	if( m_eVirtualStretchType == EOSAIEnumStretchType_STRETCH )
	{
		ASSERT( m_iVirtualWidth > 0 );
		ASSERT( m_iVirtualHeight > 0 );
		float fx = (float)x * (float)m_iWidth  / (float)m_iVirtualWidth;
		float fy = (float)y * (float)m_iHeight / (float)m_iVirtualHeight;
		return GridValue( (int)fx, (int)fy );
	}

	if( m_bWrapRight || m_bWrapBottom )
	{
		float fx = (float)x * (float)m_iWidth  / (float)m_iVirtualWidth;
		float fy = (float)y * (float)m_iHeight / (float)m_iVirtualHeight;

		int   ix = (int) fx;
		int   iy = (int) fy;

		fx -= ix;
		fy -= iy;

		if( ix > 0 && iy > 0 && ix < m_iWidth-1 && iy < m_iHeight-1 )
		{
			float elevation;
			if( fx <= 0.5f && fy <= 0.5f )
			{
				fx += 0.5f;
				fy += 0.5f;
				elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix-1) + (iy-1) * m_iWidth ] ) +
				            (fx       )*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy-1) * m_iWidth ] ) +
							(1.0f - fx)*(fy       )*( m_aMapData[ (ix-1) + (iy  ) * m_iWidth ] ) +
							(fx       )*(fy       )*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( fx > 0.5f && fy <= 0.5f )
			{
				fx -= 0.5f;
				fy += 0.5f;
				elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy-1) * m_iWidth ] ) +
				            (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy-1) * m_iWidth ] ) +
							(1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
							(fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( fx <= 0.5f && fy > 0.5f )
			{
				fx += 0.5f;
				fy -= 0.5f;
				elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix-1) + (iy  ) * m_iWidth ] ) +
				            (fx       )*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
							(1.0f - fx)*(fy       )*( m_aMapData[ (ix-1) + (iy+1) * m_iWidth ] ) +
							(fx       )*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( fx > 0.5f && fy > 0.5f )
			{
				fx -= 0.5f;
				fy -= 0.5f;
				elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
				            (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] ) +
							(1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] ) +
							(fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy+1) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			ASSERT( false );
		}
		//if( ix < m_iWidth-1 && iy < m_iHeight-1 )
		else if( ix == 0 || iy == 0 )
		{
			// INCOMPLETE: This algorithm is not quite right.
			//   The block should cover 0,0 through 3,3.
			//   But our calculation makes 0,0 the "actual" point and it does gradation to 3,3.
			//   The four blocks above handle this correctly, but I should fix this problem universally.
			//   If I use the four block above I also need to code what happens along the upper
			//   and left edges of the map.
			float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
				               (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] ) +
							   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] ) +
							   (fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy+1) * m_iWidth ] );
			return (char) (elevation+0.5f);
		}
		/*
		if( ix < m_iWidth-1 && iy < m_iHeight-1 )
		{
			float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
				               (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] ) +
							   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] ) +
							   (fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy+1) * m_iWidth ] );
			return (char) (elevation+0.5f);
		}
		*/
		else
		{
			if( ix >= m_iWidth-1 && iy >= m_iHeight-1 )
			{
				float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
								   (fx       )*(1.0f - fy)*( m_aMapData[ (0   ) + (iy  ) * m_iWidth ] ) +
								   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (0   ) * m_iWidth ] ) +
								   (fx       )*(fy       )*( m_aMapData[ (0   ) + (0   ) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( ix >= m_iWidth-1 && iy <  m_iHeight-1 )
			{
				float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
					               (fx       )*(1.0f - fy)*( m_aMapData[ (0   ) + (iy  ) * m_iWidth ] ) +
								   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] ) +
								   (fx       )*(fy       )*( m_aMapData[ (0   ) + (iy+1) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( ix <  m_iWidth-1 && iy >= m_iHeight-1 )
			{
				float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
					               (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] ) +
								   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (0   ) * m_iWidth ] ) +
								   (fx       )*(fy       )*( m_aMapData[ (ix+1) + (0   ) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
		}

		return 0;
	}
	else
	{
		float fx = (float)x * (float)m_iWidth  / (float)m_iVirtualWidth;
		float fy = (float)y * (float)m_iHeight / (float)m_iVirtualHeight;

		int   ix = (int) fx;
		int   iy = (int) fy;

		fx -= ix;
		fy -= iy;

		if( ix > 0 && iy > 0 && ix < m_iWidth-1 && iy < m_iHeight-1 )
		{
			float elevation;
			if( fx <= 0.5f && fy <= 0.5f )
			{
				fx += 0.5f;
				fy += 0.5f;
				elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix-1) + (iy-1) * m_iWidth ] ) +
				            (fx       )*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy-1) * m_iWidth ] ) +
							(1.0f - fx)*(fy       )*( m_aMapData[ (ix-1) + (iy  ) * m_iWidth ] ) +
							(fx       )*(fy       )*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( fx > 0.5f && fy <= 0.5f )
			{
				fx -= 0.5f;
				fy += 0.5f;
				elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy-1) * m_iWidth ] ) +
				            (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy-1) * m_iWidth ] ) +
							(1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
							(fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( fx <= 0.5f && fy > 0.5f )
			{
				fx += 0.5f;
				fy -= 0.5f;
				elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix-1) + (iy  ) * m_iWidth ] ) +
				            (fx       )*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
							(1.0f - fx)*(fy       )*( m_aMapData[ (ix-1) + (iy+1) * m_iWidth ] ) +
							(fx       )*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( fx > 0.5f && fy > 0.5f )
			{
				fx -= 0.5f;
				fy -= 0.5f;
				elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
				            (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] ) +
							(1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] ) +
							(fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy+1) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			ASSERT( false );
		}
		//if( ix < m_iWidth-1 && iy < m_iHeight-1 )
		else if( ix == 0 || iy == 0 )
		{
			if( ix == 0 && iy == m_iHeight-1 )
			{
				float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy) * m_iWidth ] ) +
								   (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy) * m_iWidth ] ) +
								   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy) * m_iWidth ] ) +
								   (fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}

			// INCOMPLETE: This algorithm is not quite right.
			//   The block should cover 0,0 through 3,3.
			//   But our calculation makes 0,0 the "actual" point and it does gradation to 3,3.
			//   The four blocks above handle this correctly, but I should fix this problem universally.
			//   If I use the four block above I also need to code what happens along the upper
			//   and left edges of the map.
			float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
				               (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] ) +
							   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] ) +
							   (fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy+1) * m_iWidth ] );
			return (char) (elevation+0.5f);
		}
		/*
		if( ix < m_iWidth-1 && iy < m_iHeight-1 )
		{
			// INCOMPLETE: This algorithm is not quite right.
			//   The block should cover 0,0 through 3,3.
			//   But our calculation makes 0,0 the "actual" point and it does gradation to 3,3.
			//   The four blocks above handle this correctly, but I should fix this problem universally.
			//   If I use the four block above I also need to code what happens along the upper
			//   and left edges of the map.
			float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
				               (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] ) +
							   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] ) +
							   (fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy+1) * m_iWidth ] );
			return (char) (elevation+0.5f);
		}
		*/
		else
		{
			if( ix >= m_iWidth-1 && iy >= m_iHeight-1 )
			{
				float  elevation = m_aMapData[ (ix  ) + (iy  ) * m_iWidth ];
				return (char) (elevation+0.5f);
			}
			if( ix >= m_iWidth-1 && iy <  m_iHeight-1 )
			{
				float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
					               (fx       )*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
								   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] ) +
								   (fx       )*(fy       )*( m_aMapData[ (ix  ) + (iy+1) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
			if( ix <  m_iWidth-1 && iy >= m_iHeight-1 )
			{
				float  elevation = (1.0f - fx)*(1.0f - fy)*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
					               (fx       )*(1.0f - fy)*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] ) +
								   (1.0f - fx)*(fy       )*( m_aMapData[ (ix  ) + (iy  ) * m_iWidth ] ) +
								   (fx       )*(fy       )*( m_aMapData[ (ix+1) + (iy  ) * m_iWidth ] );
				return (char) (elevation+0.5f);
			}
		}

		return 0;
	}
}

void CEOSAISimpleCharMap::Clear( int value )
{
	/*
	for( long x=0; x<m_iWidth; x++ ){
		for( long y=0; y<m_iHeight; y++ ){
			m_aMapData[x+y*m_iWidth] = value;
		}
	}
	*/
	memset( m_aMapData, value, m_iWidth*m_iHeight );
}

void CEOSAISimpleCharMap::DrawCircle( float centerX, float centerY, float radius, int value )
{
	// INCOMPLETE: Doesn't handle wrapped maps, and it is slow
	long MinY = (long)( centerY - radius );
	long MaxY = (long)( centerY + radius );

	if( MinY < 0 ) MinY = 0;
	if( MaxY >= m_iHeight ) MaxY = m_iHeight-1;

	for( int y = MinY; y <= MaxY; y++ )
	{
		// calculate the start and end x
		float diffy = (float) fabs( (float) y - centerY );
		if( diffy >= radius ) continue;
		float deltaX = (float) sqrt( radius*radius - diffy*diffy );

		long StartX = (long)( centerX - deltaX );
		long EndX   = (long)( centerX + deltaX );

		if( StartX < 0 ) StartX = 0;
		if( EndX   >= m_iWidth ) EndX = m_iWidth - 1;

		for( int x = StartX; x <= EndX; x++ )
		{
			int location = (x + y*m_iWidth);
			/*
			if( (location%8) == 0 )
			{
				if( x + 7 <= EndX )
				{
					m_aMapData[ location/8 ] = 0xFF;
					x+=7;  // 7 here + 1 in the loop
				}
				else
				{
					m_aMapData[ location/8 ] |= ( 0x1 << location%8 );
				}
			}
			else
			{
				m_aMapData[ location/8 ] |= ( 0x1 << location%8 );
			}
			*/
			m_aMapData[ location ] = value;
		}
	}
}

char& CEOSAISimpleCharMap::GridValue( int x, int y )
{
	x = ( x + m_iWidth )  % m_iWidth;
	y = ( y + m_iHeight ) % m_iHeight;

	//x += m_iWidth;  x %= m_iWidth;
	//y += m_iHeight; y %= m_iHeight;

	return m_aMapData[ x + y*m_iWidth ]; 
}



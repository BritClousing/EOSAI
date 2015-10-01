
#pragma once

#include "EOSAISerial.h"

template< class TYPE > class CEOSAISimpleMap
{
	public:
		CEOSAISimpleMap()
		{
			m_aMapData      = 0;
			m_iWidth        = m_iHeight        = 0;
			m_bWrapX        = m_bWrapY         = false;
			m_iVirtualWidth = m_iVirtualHeight = 0;
		}
		CEOSAISimpleMap( int iWidth, int iHeight )
		{
			m_aMapData      = 0;
			m_iWidth        = m_iHeight        = 0;
			m_bWrapX        = m_bWrapY         = false;
			m_iVirtualWidth = m_iVirtualHeight = 0;
			Allocate( iWidth, iHeight );
		}
		~CEOSAISimpleMap()
		{
			delete[] m_aMapData;
			m_aMapData = 0;
		}
		//
		// Size and resize the map
		//
		bool Allocate( int iWidth, int iHeight )
		{
			delete[] m_aMapData;
			m_aMapData = 0;

			// Initialization
			m_aMapData = new TYPE[ iWidth * iHeight ];
			if( m_aMapData )
			{
				m_iWidth  = m_iVirtualWidth  = iWidth;
				m_iHeight = m_iVirtualHeight = iHeight;
				return true;
			}
			else
			{
				m_iWidth  = m_iVirtualWidth  = 0;
				m_iHeight = m_iVirtualHeight = 0;
				return false;
			}
		}
		void SetVirtualSize( long x, long y ){ m_iVirtualWidth = x; m_iVirtualHeight = y; }
		//
		// Wrap
		//
		void SetWrap( bool bWrapX, bool bWrapY ){ m_bWrapX = bWrapX; m_bWrapY = bWrapY; }
		//void WrapRight( bool val ){ m_bWrapRight = val; }
		//void WrapBottom( bool val ){ m_bWrapBottom = val; }
		//
		void Serialize( CEOSAISerial* pSerial )
		{
			pSerial->Serialize( m_iWidth );
			pSerial->Serialize( m_iHeight );
			pSerial->SerializeBlock( m_aMapData, sizeof(TYPE)*m_iWidth*m_iHeight );
			pSerial->Serialize( m_bWrapRight );
			pSerial->Serialize( m_bWrapBottom );
			pSerial->Serialize( m_iVirtualWidth );
			pSerial->Serialize( m_iVirtualHeight );
		}
		void Deserialize( CEOSAISerial* pSerial )
		{
			long iWidth = 0;
			long iHeight = 0;
			pSerial->Deserialize( iWidth );
			pSerial->Deserialize( iHeight );
			Allocate( iWidth, iHeight );
			pSerial->DeserializeBlock( m_aMapData, sizeof(TYPE)*m_iWidth*m_iHeight );
			pSerial->Deserialize( m_bWrapRight );
			pSerial->Deserialize( m_bWrapBottom );
			pSerial->Deserialize( m_iVirtualWidth );
			pSerial->Deserialize( m_iVirtualHeight );
		}
		//
		/*
		void Copy( CSimpleMap< TYPE >& map, int iNewPixelSize, bool bWrapRight, bool bWrapBottom )
		{
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
		*/
		//
		void Free()
		{
			delete[] m_aMapData;
			m_aMapData = 0;
			m_iWidth   = 0;
			m_iHeight  = 0;
		}
		void Clear( TYPE value )
		{
			for( long x=0; x<m_iWidth; x++ ){
				for( long y=0; y<m_iHeight; y++ ){
					m_aMapData[x+y*m_iWidth] = value;
				}
			}
		}
		//
		// Map Values
		//
		TYPE& PixelValue( int x, int y )
		{
			x /= (m_iVirtualWidth/m_iWidth);
			y /= (m_iVirtualHeight/m_iHeight);
			return GridValue( x,y );
		}
		TYPE& GridValue( int x, int y )
		{
			x += m_iWidth;  x %= m_iWidth;
			y += m_iHeight; y %= m_iHeight;
			return m_aMapData[ x + y*m_iWidth ]; 
		}

		bool  IsValidGridLocation( int x, int y )
		{
			return ( x >= 0 && y >= 0 && x <= m_iWidth && y <= m_iHeight );
		}
		bool  TruncateAndValidateGridLocation( int* x, int* y )
		{
			if( *x<0 || *x>=m_iWidth  ){ if( m_bWrapX  ){ *x = (*x+m_iWidth)%m_iWidth; } }
			if( *y<0 || *y>=m_iHeight ){ if( m_bWrapY ){ *y = (*y+m_iHeight)%m_iHeight; } }
			if(( *x<0 || *x>=m_iWidth  ) && !m_bWrapX ) return false; 
			if(( *y<0 || *y>=m_iHeight ) && !m_bWrapY ) return false; 
			return true;
		}

		//
		// Map information
		//
		TYPE* MapDataPtr(){return m_aMapData;}
		int   GetWidth(){  return m_iWidth;  }
		int   GetHeight(){ return m_iHeight; }

		bool  WrapX(){ return m_bWrapX; }
		bool  WrapY(){ return m_bWrapY; }

		int   GetVirtualWidth(){  return m_iVirtualWidth; }
		int   GetVirtualHeight(){ return m_iVirtualHeight; }
		
		/*
			// Stretch the existing map.
			void Stretch( int width, int height ){}

			void Copy( CSimpleShortMap& map, int iNewPixelSize, enumStretchType stretchtype, bool bWrapRight, bool bWrapBottom );
		//
		//
		//
			// Erase the map
			void Clear( int value );
		//
		// Wrap
		//
			void WrapRight( bool val ){ m_bWrapRight = val; }
			bool WrapRight(){ return m_bWrapRight; }
			void WrapBottom( bool val ){ m_bWrapBottom = val; }
			bool WrapBottom(){ return m_bWrapBottom; }
		//
		//
		//
			void SetVirtualSize( long x, long y, enumStretchType stretchtype ){ m_iVirtualWidth = x; m_iVirtualHeight = y; m_eVirtualStretchType = stretchtype; }
		//
		// Graphic Functions
		//
			//void Smooth();
		//
		//
		//
			ushort  PixelValue( int x, int y ); // eg. 800x800
			ushort& GridValue( int x, int y );      // eg. 200x200
			//inline char& Value( int x, int y ){ return m_aMapData[ x + y*m_iWidth ]; }

		//
		// Map information
		//
			inline ushort* MapDataPtr(){return m_aMapData;}
			inline int     GetWidth(){  return m_iWidth;  }
			inline int     GetHeight(){ return m_iHeight; }

			inline int   GetVirtualWidth(){  return m_iVirtualWidth; }
			inline int   GetVirtualHeight(){ return m_iVirtualHeight; }
*/
	private:
		TYPE*  m_aMapData;
		int    m_iWidth;
		int    m_iHeight;

		bool   m_bWrapX;
		bool   m_bWrapY;

		// Virtual settings
		// We only do "closest" value (since TYPE might not allow for gradients).
		int    m_iVirtualWidth;
		int    m_iVirtualHeight;
};


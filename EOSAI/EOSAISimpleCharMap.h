
#pragma once

class CEOSAISerial;

enum EOSAIEnumStretchType
{
	EOSAIEnumStretchType_ANTIALIAS,
	EOSAIEnumStretchType_STRETCH,
};

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAISimpleCharMap
{
	public:
		CEOSAISimpleCharMap();
		CEOSAISimpleCharMap( int width, int height );
		~CEOSAISimpleCharMap();

		// Serialization
		//
			void Serialize( CEOSAISerial* pSerial );
			void Deserialize( CEOSAISerial* pSerial );

			void Serialize_ElevationCompression( CEOSAISerial* pSerial );
			void Deserialize_ElevationCompression( CEOSAISerial* pSerial );

			void Serialize_TerrainCompression( CEOSAISerial* pSerial );
			void Deserialize_TerrainCompression( CEOSAISerial* pSerial );
		//
		// Size and resize the map
		//
			// Erase any existing information, allocate memory for new array.
			bool Allocate( int width, int height );
			void Free();

			// Does not stretch, just truncates or adds new space
			void Resize( long iNewGridWidth, long iNewGridHeight, long iClearValue );

			// Stretch the existing map.
			void Stretch( int width, int height ){}

			void Copy( CEOSAISimpleCharMap& other );
			void Copy( CEOSAISimpleCharMap& map, int iNewPixelSize, EOSAIEnumStretchType stretchtype, bool bWrapRight, bool bWrapBottom );

			void Change( int initialValue, int newValue );
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
			void SetVirtualSize( long x, long y, EOSAIEnumStretchType stretchtype ){ m_iVirtualWidth = x; m_iVirtualHeight = y; m_eVirtualStretchType = stretchtype; }
			EOSAIEnumStretchType  GetStretchType(){ return m_eVirtualStretchType; }
		//
		// Graphic Functions
		//
			//void Smooth();

			// Handles Grid coordinates!
			void DrawCircle( float centerX, float centerY, float radius, int value );
		//
		//
		//
			char  PixelValue( int x, int y ); // eg. 800x800
			char& GridValue( int x, int y );  // eg. 200x200
			char& GridValueUnsafe( int x, int y ){ return m_aMapData[ x + y*m_iWidth ]; }
			//inline char& Value( int x, int y ){ return m_aMapData[ x + y*m_iWidth ]; }
		//
		inline bool  TruncateAndValidateGridLocation( int* x, int* y )
		{
			if( m_bWrapRight  && ( *x<0 || *x>=m_iWidth ) ){ *x = (*x+m_iWidth)%m_iWidth; }
			if( m_bWrapBottom && ( *y<0 || *y>=m_iHeight ) ){ *y = (*y+m_iHeight)%m_iHeight; }
			if( ( *x<0 || *x>=m_iWidth  ) && !m_bWrapRight ) return false; 
			if( ( *y<0 || *y>=m_iHeight ) && !m_bWrapBottom ) return false; 
			return true;
		}

		//
		// Map information
		//
			inline char* MapDataPtr(){return m_aMapData;}
			inline int   GetWidth(){  return m_iWidth;  }
			inline int   GetHeight(){ return m_iHeight; }

			inline int   GetVirtualWidth(){  return m_iVirtualWidth; }
			inline int   GetVirtualHeight(){ return m_iVirtualHeight; }

	private:
		long  m_iWidth;
		long  m_iHeight;
		char* m_aMapData;

		bool  m_bWrapRight;
		bool  m_bWrapBottom;

		// Virtual settings
		EOSAIEnumStretchType  m_eVirtualStretchType = EOSAIEnumStretchType::EOSAIEnumStretchType_STRETCH;
		long                  m_iVirtualWidth = 0;
		long                  m_iVirtualHeight = 0;
};

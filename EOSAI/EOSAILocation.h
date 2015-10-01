
#ifndef EOSAI_LOCATION_H_
#define EOSAI_LOCATION_H_

//#ifdef THIS_MODULE
#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class CEOSAISerial;

class DLLIMPEXP CEOSAILocation // lots of warnings and errors in the EOSAI build - error LNK2019: unresolved external symbol "__declspec(dllimport) public: __thiscall CEOSAILocation::CEOSAILocation(class CEOSAILocation &)"
//class EXPORT CEOSAILocation // Still seeing all the link errors in EOS
//class CEOSAILocation // Builds, but I get link errors in EOS
{
	public:
		enum EOSAILocationType
		{
			Grid,
			Pixel
		};

		CEOSAILocation(){ m_fRealX = m_fRealY = 0.0f; }
		CEOSAILocation( CEOSAILocation& other ){ m_fRealX = other.m_fRealX; m_fRealY = other.m_fRealY; }
		CEOSAILocation( EOSAILocationType type, float x, float y );//{ m_fRealX = x;   m_fRealY = y; }
		CEOSAILocation( EOSAILocationType type, int   x, int   y );//{ m_fRealX = (float)( x*4+2.0 ); m_fRealY = (float)( y*4+2.0 ); }

		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );

		void  SetGrid( int x, int y ){ m_fRealX = (float) x*4.0f; m_fRealY = (float) y*4.0f; }
		int   GetGridX(){ return (int)( m_fRealX/4.0 ); }
		int   GetGridY(){ return (int)( m_fRealY/4.0 ); }

		void  SetReal( float x, float y ){ m_fRealX = x; m_fRealY = y; }
		void  AddRealX( float x ){ m_fRealX += x; }
		void  AddRealY( float y ){ m_fRealY += y; }
		float GetRealX(){ return m_fRealX; }
		float GetRealY(){ return m_fRealY; }

		void  SetLocation( CEOSAILocation& otherLocation )
		{
			m_fRealX = otherLocation.m_fRealX;
			m_fRealY = otherLocation.m_fRealY;
		}
		void  operator=( const CEOSAILocation& otherLocation )
		{
			m_fRealX = otherLocation.m_fRealX;
			m_fRealY = otherLocation.m_fRealY;
		}
		bool  operator==( CEOSAILocation& otherLocation )
		{
			if( m_fRealX == otherLocation.m_fRealX &&
				m_fRealY == otherLocation.m_fRealY ) return true;
			return false;
		}
		bool  operator!=( CEOSAILocation& otherLocation )
		{
			if( m_fRealX == otherLocation.m_fRealX &&
				m_fRealY == otherLocation.m_fRealY ) return false;
			return true;
		}

		void RoundToGrid()
		{
			m_fRealX = (float) 4.0f * ( (int)( m_fRealX / 4.0f ) );
			m_fRealY = (float) 4.0f * ( (int)( m_fRealY / 4.0f ) );
		}

		//float GetPixelDistance( CEOSAILocation& otherLocation );

	private:
		float m_fRealX;
		float m_fRealY;
};

#endif

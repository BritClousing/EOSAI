
#pragma once

class CGLColor
{
	public:
		CGLColor(){ m_bgra[0] = 0; m_bgra[1] = 0; m_bgra[2] = 0; m_bgra[3] = 255; }
		CGLColor( unsigned char r, unsigned char g, unsigned char b, unsigned char a ){ m_bgra[0] = b; m_bgra[1] = g; m_bgra[2] = r; m_bgra[3] = a; }
		CGLColor( unsigned char r, unsigned char g, unsigned char b                  ){ m_bgra[0] = b; m_bgra[1] = g; m_bgra[2] = r; m_bgra[3] = 255; }

		void Set( unsigned char r, unsigned char g, unsigned char b, unsigned char a ){ m_bgra[0] = b; m_bgra[1] = g; m_bgra[2] = r; m_bgra[3] = a; }
		void Set( unsigned char r, unsigned char g, unsigned char b                  ){ m_bgra[0] = b; m_bgra[1] = g; m_bgra[2] = r; }
		void Set( CGLColor& color            ){ m_bgra[0] = color.m_bgra[0]; m_bgra[1] = color.m_bgra[1]; m_bgra[2] = color.m_bgra[2]; m_bgra[3] = color.m_bgra[3]; }

		// val=0 means black, 1 means no change, 2 means double, ...
		void Brighten( float val );

		CGLColor  operator*( float fVal );

		unsigned char GetRed(){   return m_bgra[2]; }
		unsigned char GetGreen(){ return m_bgra[1]; }
		unsigned char GetBlue(){  return m_bgra[0]; }
		float GetRedF(){   return (float) m_bgra[2] / 255.0f; }
		float GetGreenF(){ return (float) m_bgra[1] / 255.0f; }
		float GetBlueF(){  return (float) m_bgra[0] / 255.0f; }
		float GetAlphaF(){ return (float) m_bgra[3] / 255.0f; }

		//int m_bgra[4];
		unsigned char m_bgra[4];
};



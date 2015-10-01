
#include "stdafx.h"

#include "GLColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CGLColor::Brighten( float val )
{
	long i;
	i = (long)( m_bgra[0]*val );
	m_bgra[0] = (unsigned char) max( 0, min( i,255 ));
	i = (long)( m_bgra[1]*val );
	m_bgra[1] = (unsigned char) max( 0, min( i,255 ));
	i = (long)( m_bgra[2]*val );
	m_bgra[2] = (unsigned char) max( 0, min( i,255 ));
	/*
	m_bgra[0] = (unsigned char)( m_bgra[0]*val );
	if( m_bgra[0] < 0   ) m_bgra[0] = 0;
	if( m_bgra[0] > 255 ) m_bgra[0] = 255;
	m_bgra[1] = (unsigned char)( m_bgra[1]*val );
	if( m_bgra[1] < 0   ) m_bgra[1] = 0;
	if( m_bgra[1] > 255 ) m_bgra[1] = 255;
	m_bgra[2] = (unsigned char)( m_bgra[2]*val );
	if( m_bgra[2] < 0   ) m_bgra[2] = 0;
	if( m_bgra[2] > 255 ) m_bgra[2] = 255;
	*/
}

CGLColor  CGLColor::operator*( float fVal )
{
	CGLColor color;

	ASSERT( fVal >= 0.0 );
	ASSERT( fVal <= 1.0 );
	if( fVal > 1.0 )
	{
		// Need some more logic here (if we want values > 1)
		//   We need to make sure values don't go over 255.
		//int iNewRed, iNewGreen, iNewBlue;
		ASSERT( false );
	}
	else
	{
		color.Set( (unsigned char)( m_bgra[2] * fVal ), 
			       (unsigned char)( m_bgra[1] * fVal ), 
				   (unsigned char)( m_bgra[0] * fVal ) );
	}
	return color;
}



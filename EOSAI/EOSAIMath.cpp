
#include <stdafx.h>
#include <math.h>
#include "EOSAIMath.h"

float CEOSAIMath::Sigma01( float fBottom, float fTop, float fValue )
{
	ASSERT( fTop > fBottom );
	float fGap = fTop - fBottom;
	float fCenter = fBottom + 0.5f*fGap;
	float fDeviations = (float) fabs( fValue - fCenter ) / ( 0.5f * fGap );
	if( fValue < fCenter )
	{
		return 0.5f / ( fDeviations + 1.0f );
	}
	else
	{
		return 1.0f - 0.5f / ( fDeviations + 1.0f );
	}
}

float CEOSAIMath::RoundDown( float f ){ return (float)((long)f); }

float CEOSAIMath::RoundUp( float f )
{
	long iLongValue = (long) f;
	float fDecimalPortion = f - ((float)iLongValue);
	if( fDecimalPortion > 0.0f ){ iLongValue += 1; }
	return (float) iLongValue;
}

float CEOSAIMath::GetRandomFloat( float fMin, float fMax )
{
	float fGap = fMax - fMin;
	if( fGap <= 0.0f ) return fMax;

	return fMin + fGap*((float)rand()/(float)RAND_MAX);
}

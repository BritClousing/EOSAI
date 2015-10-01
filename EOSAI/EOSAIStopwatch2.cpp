
#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#include "EOSAIStopwatch2.h"

LONGLONG CEOSAIStopwatch2::m_lTicksPerSecond = 0;   // 64-bit signed integer (can store ranges -9*10^13 to 9*10^13)
LONGLONG CEOSAIStopwatch2::m_lCorrectionValue = 0;

void CEOSAIStopwatch2::Initialize()
{
	if( m_lTicksPerSecond > 0 ) return;  // already initialized

	LARGE_INTEGER iTicksPerSecond;

	QueryPerformanceFrequency( &iTicksPerSecond );
	m_lTicksPerSecond = iTicksPerSecond.QuadPart;

	// If this assert fails, it means your processor doesn't support a high-precision timer
	//ASSERT( iFrequency.QuadPart > 0 );

	// Calibrate
	CEOSAIStopwatch2 watch;
	watch.Start();
	watch.Stop();

	// Start
	/*
	LONGLONG lAccumulatedTicks = 0;
	LARGE_INTEGER iStartTime;
	LARGE_INTEGER iStopTime;
	Sleep(0); // For the sake of accuracy, call Sleep(0).  This will increase the chance that this thread will get an uninterupted block of processing time.
	QueryPerformanceCounter( &iStartTime );  // Start
	QueryPerformanceCounter( &iStopTime );   // Stop
	lAccumulatedTicks += iStopTime.QuadPart - iStartTime.QuadPart;
	*/

	m_lCorrectionValue = watch.m_iStopTime.QuadPart - watch.m_iStartTime.QuadPart;
}

double CEOSAIStopwatch2::GetTime()
{
	Initialize();

	LARGE_INTEGER iTime;
	QueryPerformanceCounter( &iTime );
	double dValue = ((double) iTime.QuadPart ) / ((double) m_lTicksPerSecond);
	return dValue;
}

LARGE_INTEGER CEOSAIStopwatch2::GetTime_LargeInteger()
{
	LARGE_INTEGER  LargeInt;
	QueryPerformanceCounter( &LargeInt );
	return LargeInt;
}

CEOSAIStopwatch2::CEOSAIStopwatch2()
{
	Initialize();
	/*
	LARGE_INTEGER iTicksPerSecond;

	QueryPerformanceFrequency( &iTicksPerSecond );
	m_lTicksPerSecond = iTicksPerSecond.QuadPart;

   // If this assert fails, it means your processor doesn't support a high-precision timer
   //ASSERT( iFrequency.QuadPart > 0 );

	// Calibrate
	Start();
	Stop();
	m_lCorrectionValue = m_iStopTime.QuadPart - m_iStartTime.QuadPart;
	*/
	m_lAccumulatedTicks = 0;

	//
	m_iCurrentIndex = 0;
	for( long i=0; i<100; i++ )
	{
		m_Log[i] = 0.0;
	}
}

void CEOSAIStopwatch2::Start()
{
	m_lAccumulatedTicks = 0;

	// For the sake of accuracy, call Sleep(0).
   // This will increase the chance that this thread will get an uninterupted block of processing time.
	Sleep(0);
	QueryPerformanceCounter( &m_iStartTime );
}

void CEOSAIStopwatch2::Continue()
{
	// For the sake of accuracy, call Sleep(0).
   // This will increase the chance that this thread will get an uninterupted block of processing time.
	Sleep(0);
  	QueryPerformanceCounter( &m_iStartTime );
}

void CEOSAIStopwatch2::Stop()
{
	QueryPerformanceCounter( &m_iStopTime );

	m_lAccumulatedTicks += m_iStopTime.QuadPart - m_iStartTime.QuadPart - m_lCorrectionValue;
}

double CEOSAIStopwatch2::GetDeltaSeconds()
{
   double dValue = ((double) m_lAccumulatedTicks ) / ((double) m_lTicksPerSecond);

   // Strip off trailing numbers if they are past our known precision.
   // For example, if dValue is 0.01234, but our frequency is 1000 ticks per second, then
   //   the most accuracy we can get in our numbers is down to the millisecond.  Thus, 
   //   0.01234 should be adjusted to 0.012.  The rest of the digits are simply a rounding problem.
   // (On many computers, the frequency will be between 1 and 10 million, which means you can get 
   //   values down to the microsecond, but numbers beyond that lose significance.)

   double dPrecisionLog10 = log10( (double) m_lTicksPerSecond )+0.99;  // we want to round up
   long   iPrecisionLog10 = (long) dPrecisionLog10;

   // Move the decimal point (in preparation for cutting-off everything below the decimal).
   double dNewValue = dValue * pow( 10.0, iPrecisionLog10 );

   // If the value is higher than 10^13, our algorithm will fail because LONGLONG cannot hold values that large.
   //   Of course, if that ever happened, QueryPerformanceCounter would fail as well.
   if( dNewValue < 10000000000000.0 )
   {
      // Truncate everything below the decimal point
      LONGLONG iValue = (LONGLONG) dNewValue;

      // Now move the decimal point back to where it was.
      dValue          = ((double)iValue) / pow( 10.0, iPrecisionLog10 );
   }

   return dValue;
}

double CEOSAIStopwatch2::GetTimerPrecisionInSeconds()
{
	return 1.0 / ((double) m_lTicksPerSecond);
}


void CEOSAIStopwatch2::LogDeltaSeconds()
{
   if( m_iCurrentIndex < 100 )
   {
      m_Log[ m_iCurrentIndex ] = GetDeltaSeconds();
      m_iCurrentIndex++;
   }
}


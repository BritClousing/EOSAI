
#pragma once

#include <windows.h>

/*
   Using the CStopwatch class:

   CStopwatch allows for precise measurements of time.  It only works if your computer has a high-resolution
     timer, which means it's fine for debugging, but it might be bad to expect all your users to have it.
     (I'm unsure how common it is for computers to have high-resolution timers.  It's possible that all
      modern computers have them.)

   Calling Start() will start the timer, and Stop() will stop the timer.

   GetDeltaSeconds() returns the amount of time that elapsed between Start() and Stop().
     The GetDeltaSeconds() method limits the precision that is returned (based on 
     the frequency of the timer).
     More specifically, if the timer returns a duration of 0.1239730934098324, but we know the
     timer is only accurate to the microsecond, the method will return 0.123973.

   The Continue() function is useful when you want to measure one component of a loop.
     It's not necessary to call Start() before calling Continue().  It's okay to do this:

      CStopwatch watch;
      for( long i=0; i<1000000; i++ )
      {
         Function1();

         watch.Continue();
         Function2();
         watch.Stop();
      }
      double dResult = watch.GetDeltaSeconds();

   The LogDeltaSeconds() is something that will only be used with the debugger.  It provides a convenient
     way for the developer to record times for a series of events.  For example, if you are rendering
     an image in real-time and it seems to suddenly slow down after the first 10 frames of animation, the
     developer might want to do something like this (shown below).  At the breakpoint, the developer could
     look at the watch.m_Log array and it would contain a whole series of timings.

     CStopwatch watch;
     while( bRendering )
     {
        watch.Start();
        Render();
        watch.Stop();
        watch.LogDeltaSeconds();
     }
     <breakpoint here>
*/

class CEOSAIStopwatch2
{
	public:
		static void           Initialize();
		static double         GetTime();
		static LARGE_INTEGER  GetTime_LargeInteger();

	public:
		CEOSAIStopwatch2();

		// Start and Stop the stopwatch
		void Start();    // Start the timer. Reset accumulated time to 0.
		void Continue(); // Start the timer, Do not reset accumulated time.
		void Stop();

		// Get the results
		double GetDeltaSeconds();
		double GetDeltaMilliseconds(){ return 1000.0    * GetDeltaSeconds(); }
		double GetDeltaMicroseconds(){ return 1000000.0 * GetDeltaSeconds(); }

		double GetTimerPrecisionInSeconds();

		// Put the DeltaSeconds into the Log array.  This allows the developer to store a series of times,
		//   which is useful for debugging.
		void   LogDeltaSeconds();

	private:
		static LONGLONG m_lTicksPerSecond;   // 64-bit signed integer (can store ranges -9*10^13 to 9*10^13)
		static LONGLONG m_lCorrectionValue;

		LARGE_INTEGER m_iStartTime;
		LARGE_INTEGER m_iStopTime;

		LONGLONG m_lAccumulatedTicks;

		long     m_iCurrentIndex;
		double   m_Log[100];
};

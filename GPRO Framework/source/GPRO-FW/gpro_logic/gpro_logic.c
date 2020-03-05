// gpro_logic.c/.cpp

#include "GPRO-FW/gpro_logic/gpro_logic.h"


// adapted from animal3D's timer interface


#if (__PLATFORM == WINDOWS)
#include <Windows.h>
#if defined(MIDL_PASS)
#define Q	qs
#else	// !MIDL_PASS
#define Q	qu
#endif	// MIDL_PASS
#else   // !WINDOWS
#include <time.h>
#define Q   qs
#define BILLION 1000000000
#endif  // WINDOWS


//-----------------------------------------------------------------------------

flag gproTimerStart(gpro_Timer* timer)
{
	if (timer)
	{
		// raise started flag, take current measurement
		timer->started = 1;

#if (__PLATFORM == WINDOWS)
		QueryPerformanceFrequency((LARGE_INTEGER*)timer->tf);
		QueryPerformanceCounter((LARGE_INTEGER*)timer->t0);
#else   // !WINDOWS
		//	clock_getres(CLOCK_REALTIME, (struct timespec *)timer->tf);
		clock_gettime(CLOCK_REALTIME, (struct timespec*)timer->t0);

		// need to convert the entire thing to seconds
		// measurement every tf->q[1] nanoseconds
		// total number of nanoseconds = nano + 1B * t->q[0]
		timer->t0->Q = BILLION * (a3i64)(timer->t0->q[0]) + (a3i64)(timer->t0->q[1]);
		timer->tf->Q = BILLION;
#endif  // WINDOWS

		// reset tick
		timer->currentTickTime = 0.0;
		return 0;
	}
	return -1;
}

flag gproTimerPause(gpro_Timer* timer)
{
	if (timer)
	{
		timer->started = 0;
		return 0;
	}
	return -1;
}

flag gproTimerResume(gpro_Timer* timer)
{
	if (timer)
	{
		timer->started = 1;
		return 0;
	}
	return -1;
}

flag gproTimerStop(gpro_Timer* timer)
{
	if (timer)
	{
		// lower started flag
		timer->started = 0;

#ifdef _WIN32
		QueryPerformanceFrequency((LARGE_INTEGER*)timer->tf);
		QueryPerformanceCounter((LARGE_INTEGER*)timer->t);
#else   // !_WIN32
		//	clock_getres(CLOCK_REALTIME, (struct timespec *)timer->tf);
		clock_gettime(CLOCK_REALTIME, (struct timespec*)timer->t1);
		timer->t1->Q = BILLION * (a3i64)(timer->t->q[0]) + (a3i64)(timer->t->q[1]);
#endif  // _WIN32

		// measure tick
		timer->currentTickTime = (f64)(timer->t->Q - timer->t0->Q) / (f64)(timer->tf->Q);
		return 0;
	}
	return -1;
}


flag gproTimerSet(gpro_Timer* timer, f64 const ticksPerSecond)
{
	if (timer)
	{
		// reset
		timer->totalTime = 0.0;
		timer->previousTickTime = 0.0;
		timer->currentTickTime = 0.0;
		timer->ticks = 0;
		timer->started = 0;

		// set tick rate
		if (ticksPerSecond > 0.0)
		{
			timer->ticksPerSecond = ticksPerSecond;
			timer->secondsPerTick = 1.0 / ticksPerSecond;
		}
		else
		{
			timer->ticksPerSecond = 0.0;
			timer->secondsPerTick = 0.0;
		}
		return 0;
	}
	return -1;
}

flag gproTimerReset(gpro_Timer* timer)
{
	if (timer)
	{
		// set all values to zero
		timer->totalTime = 0.0;
		timer->currentTickTime = 0.0;
		timer->previousTickTime = 0.0;
		timer->ticks = 0;
		timer->started = 0;
		return 0;
	}
	return -1;
}


flag gproTimerUpdate(gpro_Timer* timer)
{
	flag tick = 0;
	if (timer)
	{
		if (timer->started)
		{
			// take measurement
#ifdef _WIN32
			QueryPerformanceFrequency((LARGE_INTEGER*)timer->tf);
			QueryPerformanceCounter((LARGE_INTEGER*)timer->t);
#else   // !_WIN32
		//	clock_getres(CLOCK_REALTIME, (struct timespec *)timer->tf);
			clock_gettime(CLOCK_REALTIME, (struct timespec*)timer->t1);
			timer->t1->Q = BILLION * (a3i64)(timer->t->q[0]) + (a3i64)(timer->t->q[1]);
#endif  // _WIN32

			// accumulate time
			timer->currentTickTime = (f64)(timer->t->Q - timer->t0->Q) / (f64)(timer->tf->Q);

			// if not continuously updating
			if (timer->secondsPerTick > 0.0)
			{
				// if the current measure exceeds the rate
				while (timer->currentTickTime >= timer->secondsPerTick)
				{
					// copy current time measurement to previous
					*(timer->t0) = *(timer->t);

					// update tick: transfer expected delta from tick to total 
					//	until the tick does not exceed rate
					timer->totalTime += timer->secondsPerTick;
					timer->previousTickTime = timer->currentTickTime;
					timer->currentTickTime -= timer->secondsPerTick;
					++timer->ticks;
					++tick;
				}
			}

			// "continuous" update, always tick and update time
			else
			{
				*(timer->t0) = *(timer->t);
				timer->totalTime += timer->currentTickTime;
				timer->previousTickTime = timer->currentTickTime;
				timer->currentTickTime = 0.0;
				++timer->ticks;
				++tick;
			}
		}
		return tick;
	}
	return -1;
}


//-----------------------------------------------------------------------------

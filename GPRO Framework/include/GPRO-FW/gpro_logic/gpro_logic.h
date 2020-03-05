// gpro_logic.h

#ifndef _GPRO_LOGIC_H_
#define _GPRO_LOGIC_H_


#include "_general/_typedefs.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct gpro_Timer	gpro_Timer;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// gpro_Timer
//	Simple timer interface.
//		member 'totalTime': total time accumulated
//		member 'previousTickTime': last tick duration
//		member 'currentTickTime': current tick duration
//		member 'ticksPerSecond': expected tick rate per second (tps)
//		member 'secondsPerTick': expected seconds per tick; 1/tps
//		member 'ticks': number of ticks counted
//		member 'started': timer running; interpret as boolean
//		member 't': internal time counter
struct gpro_Timer
{
	f64 totalTime;
	f64 previousTickTime;
	f64 currentTickTime;
	f64 ticksPerSecond;
	f64 secondsPerTick;
	ui64 ticks;
	flag started;

	union {
		struct {
			i32 q[2];
			i64 qs;
		};
		i64 qu;
	} tf[1], t0[1], t[1];
};


//-----------------------------------------------------------------------------

// gproTimerStart
//	Begin counting time. Resets current tick if already started.
//		param 'timer': non-null pointer to timer
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproTimerStart(gpro_Timer* timer);

// gproTimerPause
//	Pause timer without measuring current tick.
//		param 'timer': non-null pointer to timer
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproTimerPause(gpro_Timer* timer);

// gproTimerResume
//	Resume timer without measuring current tick.
//		param 'timer': non-null pointer to timer
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproTimerResume(gpro_Timer* timer);

// gproTimerStop
//	Stop counting time and measure current tick.
//		param 'timer': non-null pointer to timer
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproTimerStop(gpro_Timer* timer);


// gproTimerSet
//	Set tick rate and stop timing.
//		param 'timer': non-null pointer to timer
//		param 'ticksPerSecond': expected tick rate per second
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproTimerSet(gpro_Timer* timer, f64 const ticksPerSecond);

// gproTimerReset
//	Fully reset timer and stop timing.
//		param 'timer': non-null pointer to timer
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproTimerReset(gpro_Timer* timer);


// gproTimerUpdate
//	Measure current tick.
//		param 'timer': non-null pointer to timer
//		return SUCCESS: positive if timer ticked
//		return SUCCESS: zero if timer did not tick
//		return FAILURE: negative
flag gproTimerUpdate(gpro_Timer* timer);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#else	// !__cplusplus

#endif	// __cplusplus


#endif	// !_GPRO_LOGIC_H_
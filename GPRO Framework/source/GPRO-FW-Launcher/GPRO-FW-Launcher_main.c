// GPRO-FW-Launcher.c/.cpp

#include "GPRO-FW/GPRO-FW.h"


// link library
#pragma comment(lib, "GPRO-FW.lib")
//#pragma comment(lib, "GPRO-FW-TEST.lib")


// declare math functions
extern f32 __cdecl cosf(f32 x);
extern f32 __cdecl sinf(f32 x);

// quick matrix math
#define mat2_x_vec2(x_out,y_out,cos,sin,x,y)	x_out = (cos) * (x) - (sin) * (y); y_out = (sin) * (x) + (cos) * (y)


#ifdef __cplusplus

#else	// !__cplusplus
typedef struct gpro_LauncherState	gpro_LauncherState;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

struct gpro_LauncherState
{
	// window info
	///
	gpro_ConsoleColorChannel drawAreaClearColorFG, drawAreaClearColorBG;
	byte drawAreaClearCharLHS, drawAreaClearCharRHS;
	byte drawAreaWidth, drawAreaHeight;

	// graphical info
	///
	f32 x0_loc, y0_loc, x1_loc, y1_loc, x2_loc, y2_loc;
	f32 x0_ndc, y0_ndc, x1_ndc, y1_ndc, x2_ndc, y2_ndc;

	// animation info
	///
	f32 angle, angVel;

	// states
	flag displayTestPatch, displayAxes, displayShape, animate;
};


//-----------------------------------------------------------------------------

flag gproLauncherStateInit(gpro_LauncherState* const state)
{
	if (state)
	{
		byte const w = 48, h = 24, clearChar = ' ';
		gpro_ConsoleColorChannel const clearColor = gproGetConsoleTextColor(0, 0, 0, 1);

		// set window info
		///
		state->drawAreaClearColorFG = state->drawAreaClearColorBG = clearColor;
		state->drawAreaClearCharLHS = state->drawAreaClearCharRHS = clearChar;
		state->drawAreaWidth = w;
		state->drawAreaHeight = h;

		// set triangle point positions
		///
		state->x0_loc = +0.75f;
		state->y0_loc = +0.00f;
		state->x1_loc = -0.60f;
		state->y1_loc = +0.45f;
		state->x2_loc = -0.60f;
		state->y2_loc = -0.45f;

		// set animation
		///
		state->angle = 0.0f;
		state->angVel = 1.0f;

		// set flags
		///
		state->displayTestPatch = 0;
		state->displayAxes = 1;
		state->displayShape = 1;
		state->animate = 1;

		// done
		return 1;
	}
	return -1;
}

flag gproLauncherStateInput(gpro_LauncherState* const state)
{
	if (state)
	{
		// handle input
		if (!gproIsVirtualKeyPressed(27))
		{
			// toggle test patch
			if (gproIsVirtualKeyPressed('P'))
				state->displayTestPatch = !state->displayTestPatch;

			// toggle axes
			if (gproIsVirtualKeyPressed('X'))
				state->displayAxes = !state->displayAxes;

			// toggle test patch
			if (gproIsVirtualKeyPressed('S'))
				state->displayShape = !state->displayShape;

			// toggle animation
			if (gproIsVirtualKeyPressed('N'))
				state->animate = !state->animate;

			// done
			return 1;
		}
		// quit
		else
			return 0;
	}
	return -1;
}

flag gproLauncherStateUpdate(gpro_LauncherState* const state, f64 const dt)
{
	if (state)
	{
		f32 cos, sin;

		// animation
		///
		state->angle += (state->angVel * (f32)dt) * (f32)state->animate;
		cos = cosf(state->angle);
		sin = sinf(state->angle);

		// update corners
		///
		mat2_x_vec2(state->x0_ndc, state->y0_ndc, cos, sin, state->x0_loc, state->y0_loc);
		mat2_x_vec2(state->x1_ndc, state->y1_ndc, cos, sin, state->x1_loc, state->y1_loc);
		mat2_x_vec2(state->x2_ndc, state->y2_ndc, cos, sin, state->x2_loc, state->y2_loc);

		// done
		return 1;
	}
	return -1;
}

flag gproLauncherStateDisplay(gpro_LauncherState const* const state)
{
	if (state)
	{
		gpro_ConsoleColorChannel const r = gproGetConsoleTextColor(1, 0, 0, 1);
		gpro_ConsoleColorChannel const g = gproGetConsoleTextColor(0, 1, 0, 1);
		gpro_ConsoleColorChannel const b = gproGetConsoleTextColor(0, 0, 1, 1);
		gpro_ConsoleColorChannel const a = gproGetConsoleTextColor(1, 1, 1, 0);

		byte x0 = 0, y0 = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;

		gproClearConsoleArea(
			state->drawAreaClearColorFG, state->drawAreaClearColorBG,
			state->drawAreaClearCharLHS, state->drawAreaClearCharRHS,
			state->drawAreaWidth, state->drawAreaHeight,
			state->drawAreaWidth, state->drawAreaHeight, 0, 0);

		// test draw
		///
		if (state->displayTestPatch)
		{
			gproDrawTestPatch();
		}

		// draw shape
		///
		if (state->displayShape)
		{
			gproViewportTransform(&x0, &y0, state->x0_ndc, state->y0_ndc, state->drawAreaWidth, state->drawAreaHeight);
			gproViewportTransform(&x1, &y1, state->x1_ndc, state->y1_ndc, state->drawAreaWidth, state->drawAreaHeight);
			gproViewportTransform(&x2, &y2, state->x2_ndc, state->y2_ndc, state->drawAreaWidth, state->drawAreaHeight);
			gproDrawConsoleTriangle(b, b, state->drawAreaClearCharLHS, state->drawAreaClearCharRHS,
				state->drawAreaWidth, state->drawAreaHeight, x0, y0, x1, y1, x2, y2);
		}

		// draw axes
		///
		if (state->displayAxes)
		{
			gproViewportTransform(&x0, &y0, 0.00f, 0.00f, state->drawAreaWidth, state->drawAreaHeight);
			gproViewportTransform(&x1, &y1, 0.25f, 0.25f, state->drawAreaWidth, state->drawAreaHeight);
			gproDrawConsoleLine(r, r, state->drawAreaClearCharLHS, state->drawAreaClearCharRHS,
				state->drawAreaWidth, state->drawAreaHeight, x0 + 1, y0, x1, y0);
			gproDrawConsoleLine(g, g, state->drawAreaClearCharLHS, state->drawAreaClearCharRHS,
				state->drawAreaWidth, state->drawAreaHeight, x0, y0 - 1, x0, y1);
			gproDrawConsolePixel(a, a, state->drawAreaClearCharLHS, state->drawAreaClearCharRHS,
				state->drawAreaWidth, state->drawAreaHeight, x0, y0);
		}

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

typedef size const					arg_count;
typedef byte const* const* const	arg_vector;


i32 main(arg_count argc, arg_vector argv)
{
	flag running = 1;
	gpro_LauncherState launcherState[1];
	gpro_Timer displayTimer[1];

	gproLauncherStateInit(launcherState);
	gproTimerSet(displayTimer, 4.0);
	gproTimerStart(displayTimer);
	gproToggleConsoleCursor(0);

	while (running > 0)
	{
		if (gproTimerUpdate(displayTimer) > 0)
		{
			running = gproLauncherStateInput(launcherState);
			gproLauncherStateUpdate(launcherState, displayTimer->previousTickTime);
			gproLauncherStateDisplay(launcherState);
		}
	}

	gproTimerStop(displayTimer);
}


//-----------------------------------------------------------------------------

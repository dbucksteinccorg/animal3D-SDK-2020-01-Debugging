// gpro_display.c/.cpp

#include "GPRO-FW/gpro_display/gpro_display.h"


#include <stdio.h>
#include <string.h>

#if (__PLATFORM == WINDOWS)
#include <Windows.h>
#endif	// WINDOWS

// help from: 
//	https://thoughtsordiscoveries.wordpress.com/2017/04/26/set-and-read-cursor-position-in-terminal-windows-and-linux/ 
//	https://falsinsoft.blogspot.com.au/2014/05/set-console-cursor-position-in-windows.html 
//	https://stackoverflow.com/questions/18028808/remove-blinking-underscore-on-console-cmd-prompt 
//	https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c 


#if (__PLATFORM == WINDOWS)

//-----------------------------------------------------------------------------

#define testAxis(x,w)		x >= 0 && x < w
#define testCoord(x,y,w,h)	testAxis(x,w) && testAxis(y,h)

#define lerp(a,b,t)			( (a) + ((b) - (a)) * (t) )
#define unlerp(a,b,v)		(((v) - (a)) / ((b) - (a)))
#define remap(a0,b0,a,b,v)	lerp(a,b,unlerp(a0,b0,v))


//-----------------------------------------------------------------------------

inline flag gproInternalDrawConsolePixel(byte const lhs, byte const rhs, byte const x, byte const y)
{
	flag ret = 0;

	// set position
	gproSetConsolePosition((word)x, (word)y);

	// print
	ret = printf("%c", lhs);

	// done
	return (ret > 0 ? 0 : -1);
}

inline flag gproInternalDrawConsoleLine(byte const n, byte const lhs, byte const rhs, byte const x0, byte const y0, byte const x1, byte const y1)
{
	flag ret = 0;

	byte i, x = x0, y = y0, x_prev, y_prev;
	f32 t = 0.0f;
	f32 const dt = 1.0f / (f32)n;
	f32 const x0f = (f32)x0, y0f = (f32)y0, x1f = (f32)x1, y1f = (f32)y1;

	ret += gproInternalDrawConsolePixel(lhs, rhs, x, y);
	for (i = 1, t = dt; i <= n; i += 1, t += dt)
	{
		x_prev = x;
		y_prev = y;
		x = (byte)lerp(x0f, x1f, t);
		y = (byte)lerp(y0f, y1f, t);
		if (x != x_prev || y != y_prev)
			ret += gproInternalDrawConsolePixel(lhs, rhs, x, y);
	}

	// done
	return (ret > 0 ? 0 : -1);
}

inline flag gproInternalDrawConsoleTriangle(byte const n0, byte const n1, byte const n2, byte const n, byte const lhs, byte const rhs, byte const x0, byte const y0, byte const x1, byte const y1, byte const x2, byte const y2)
{
	flag ret = 0;

	byte i_edge, n_edge, x_edge, y_edge, x_prev_edge, y_prev_edge, x_anchor, y_anchor;
	f32 t_edge = 0.0f;
	f32 const dt_edge = 1.0f / (f32)n;
	f32 x0f_edge, y0f_edge, x1f_edge, y1f_edge, xf_anchor, yf_anchor;

	if (n == n0)
	{
		n_edge = maximum(n1, n2);
		x_edge = x0;
		y_edge = y0;
		x_anchor = x2;
		y_anchor = y2;
		x0f_edge = (f32)x0;
		y0f_edge = (f32)y0;
		x1f_edge = (f32)x1;
		y1f_edge = (f32)y1;
		xf_anchor = (f32)x2;
		yf_anchor = (f32)y2;
	}
	else if (n == n1)
	{
		n_edge = maximum(n2, n0);
		x_edge = x1;
		y_edge = y1;
		x_anchor = x0;
		y_anchor = y0;
		x0f_edge = (f32)x1;
		y0f_edge = (f32)y1;
		x1f_edge = (f32)x2;
		y1f_edge = (f32)y2;
		xf_anchor = (f32)x0;
		yf_anchor = (f32)y0;
	}
	else
	{
		n_edge = maximum(n0, n1);
		x_edge = x2;
		y_edge = y2;
		x_anchor = x1;
		y_anchor = y1;
		x0f_edge = (f32)x2;
		y0f_edge = (f32)y2;
		x1f_edge = (f32)x0;
		y1f_edge = (f32)y0;
		xf_anchor = (f32)x1;
		yf_anchor = (f32)y1;
	}

	ret += gproInternalDrawConsoleLine(n_edge, lhs, rhs, x_edge, y_edge, x_anchor, y_anchor);
	for (i_edge = 1, t_edge = dt_edge; i_edge <= n; i_edge += 1, t_edge += dt_edge)
	{
		x_prev_edge = x_edge;
		y_prev_edge = y_edge;
		x_edge = (byte)lerp(x0f_edge, x1f_edge, t_edge);
		y_edge = (byte)lerp(y0f_edge, y1f_edge, t_edge);
		if (x_edge != x_prev_edge || y_edge != y_prev_edge)
			ret += gproInternalDrawConsoleLine(n_edge, lhs, rhs, x_edge, y_edge, x_anchor, y_anchor);
	}

	// done
	return (ret > 0 ? 0 : -1);
}


//-----------------------------------------------------------------------------

flag gproGetConsolePosition(word* const x_out, word* const y_out)
{
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo[1];
	
	if (stdHandle && x_out && y_out)
	{
		if (GetConsoleScreenBufferInfo(stdHandle, screenBufferInfo))
		{
			*x_out = screenBufferInfo->dwCursorPosition.X;
			*y_out = screenBufferInfo->dwCursorPosition.Y;
			return 0;
		}
	}
	return -1;
}

flag gproSetConsolePosition(word const x, word const y)
{
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD const pos = { x, y };

	if (stdHandle)
	{
		if (SetConsoleCursorPosition(stdHandle, pos))
		{
			return 0;
		}
	}
	return -1;
}

flag gproToggleConsoleCursor(flag const visible)
{
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo[1];

	if (stdHandle)
	{
		if (GetConsoleCursorInfo(stdHandle, cursorInfo))
		{
			cursorInfo->bVisible = (BOOL)visible;
			if (SetConsoleCursorInfo(stdHandle, cursorInfo))
			{
				return 0;
			}
		}
	}
	return -1;
}


flag gproSetConsoleTextColor(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg)
{
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdHandle)
	{
		if (SetConsoleTextAttribute(stdHandle, (word)(fg << 0) | (word)(bg << 4)))
		{
			return 0;
		}
	}
	return -1;
}

flag gproResetConsoleTextColor()
{
	gpro_ConsoleColorChannel const fg = gproGetConsoleTextColor(1, 1, 1, 1), bg = console_color_channel_off;
	return gproSetConsoleTextColor(fg, bg);
}

#define csel(c) (c ? token_cat(console_color_channel_, c) : 0)
gpro_ConsoleColorChannel gproGetConsoleTextColor(flag const r, flag const g, flag const b, flag const a)
{
	gpro_ConsoleColorChannel const color = csel(r) | csel(g) | csel(b) | csel(a);
	return color;
}
#undef csel


flag gproClearConsoleArea(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg, byte const lhs, byte const rhs, byte const w, byte const h, byte const w_area, byte const h_area, byte const x_area, byte const y_area)
{
	if (testCoord(x_area, y_area, w_area, h_area) && w_area <= w && h_area <= h)
	{
		ubyte const x_pos = x_area * 2, y_pos = y_area * 1;
		ubyte x, y, ret = 0;
		byte line[256] = "", * buffer = line;

		gproSetConsoleTextColor(fg, bg);
		for (x = 0; x < w_area; ++x)
			buffer += sprintf(buffer, "%c", lhs);
		for (y = 0; y < h_area; ++y)
		{
			gproSetConsolePosition(x_pos, y_pos + y);
			ret += printf(line);
		}

		if (ret > 0)
		{
			return 0;
		}
	}
	return -1;
}

flag gproDrawConsolePixel(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg, byte const lhs, byte const rhs, byte const w, byte const h, byte const x, byte const y)
{
	if (testCoord(x, y, w, h))
	{
		// set color
		gproSetConsoleTextColor(fg, bg);
		
		// draw
		return gproInternalDrawConsolePixel(lhs, rhs, x, y);
	}
	return -1;
}

flag gproDrawConsoleLine(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg, byte const lhs, byte const rhs, byte const w, byte const h, byte const x0, byte const y0, byte const x1, byte const y1)
{
	if (testCoord(x0, y0, w, h) && testCoord(x1, y1, w, h))
	{
		// find largest dimension
		byte const x_diff = (x0 < x1 ? x1 - x0 : x0 - x1);
		byte const y_diff = (y0 < y1 ? y1 - y0 : y0 - y1);
		byte const n = maximum(x_diff, y_diff);
		
		// set color
		gproSetConsoleTextColor(fg, bg);
		
		// draw
		return gproInternalDrawConsoleLine(lhs, rhs, n, x0, y0, x1, y1);
	}
	return -1;
}


flag gproDrawConsoleTriangle(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg, byte const lhs, byte const rhs, byte const w, byte const h, byte const x0, byte const y0, byte const x1, byte const y1, byte const x2, byte const y2)
{
	if (testCoord(x0, y0, w, h) && testCoord(x1, y1, w, h) && testCoord(x2, y2, w, h))
	{
		// find largest edge
		byte const x_diff0 = (x0 < x1 ? x1 - x0 : x0 - x1);
		byte const y_diff0 = (y0 < y1 ? y1 - y0 : y0 - y1);
		byte const x_diff1 = (x1 < x2 ? x2 - x1 : x1 - x2);
		byte const y_diff1 = (y1 < y2 ? y2 - y1 : y1 - y2);
		byte const x_diff2 = (x2 < x0 ? x0 - x2 : x2 - x0);
		byte const y_diff2 = (y2 < y0 ? y0 - y2 : y2 - y0);
		byte const n0 = maximum(x_diff0, y_diff0);
		byte const n1 = maximum(x_diff1, y_diff1);
		byte const n2 = maximum(x_diff2, y_diff2);
		byte const n = maximum(n0, maximum(n1, n2));

		// set color
		gproSetConsoleTextColor(fg, bg);

		// draw
		return gproInternalDrawConsoleTriangle(n0, n1, n2, n, lhs, rhs, x0, y0, x1, y1, x2, y2);
	}
	return -1;
}


flag gproDrawTestPatch()
{
	byte const w = 16, h = 16;
	byte x, y;
	for (x = 0; x < w; ++x)
		for (y = 0; y < h; ++y)
			gproDrawConsolePixel(x, y, x + 'a', y + 'a', w, h, x, y);
	return ((flag)x * (flag)y);
}


//-----------------------------------------------------------------------------

flag gproViewportTransform(byte* const x_view_out, byte* const y_view_out, f32 const x_ndc, f32 const y_ndc, byte const w, byte const h)
{
	if (x_view_out && y_view_out)
	{
		*x_view_out = +(byte)(2.0f * (f32)x_ndc / (f32)w - 1.0f);
		*y_view_out = -(byte)(2.0f * (f32)y_ndc / (f32)h - 1.0f);
		return 0;
	}
	return -1;
}

flag gproViewportTransformInverse(f32* const x_ndc_out, f32* const y_ndc_out, byte const x_view, byte const y_view, byte const w, byte const h)
{
	if (x_ndc_out && y_ndc_out)
	{
		*x_ndc_out = ((+(f32)x_view * 0.5f + 0.5f) * (f32)w);
		*y_ndc_out = ((-(f32)y_view * 0.5f + 0.5f) * (f32)h);
		return 0;
	}
	return -1;
}


//-----------------------------------------------------------------------------

#endif	// WINDOWS

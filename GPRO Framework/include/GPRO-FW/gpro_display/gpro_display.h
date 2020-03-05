// gpro_display.h

#ifndef _GPRO_DISPLAY_H_
#define _GPRO_DISPLAY_H_


#include "_general/_typedefs.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef enum gpro_ConsoleColorChannel	gpro_ConsoleColorChannel;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// gpro_ConsoleColorChannel
//	List of channel flags for console color settings.
enum gpro_ConsoleColorChannel
{
	console_color_channel_off,		// color is disabled (black)
	console_color_channel_r = 4,	// color contains red channel
	console_color_channel_g = 2,	// color contains green channel
	console_color_channel_b = 1,	// color contains blue channel
	console_color_channel_a = 8,	// color is fully opaque/intense
};


//-----------------------------------------------------------------------------

// gproGetConsolePosition
//	Get position of cursor in console.
//		param 'x_out': non-null pointer to value to store X coordinate
//		param 'y_out': non-null pointer to value to store Y coordinate
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproGetConsolePosition(word* const x_out, word* const y_out);

// gproSetConsolePosition
//	Set position of cursor in console.
//		param 'x': value of X coordinate to set
//		param 'y': value of Y coordinate to set
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproSetConsolePosition(word const x, word const y);

// gproToggleConsoleCursor
//	Toggle blinking underscore at cursor location.
//		param 'visible': visible flag; interpreted as boolean
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproToggleConsoleCursor(flag const visible);


// gproSetConsoleTextColor
//	Set color of console text.
//		param 'fg': description of foreground (character) channels to use
//		param 'bg': description of background (console) channels to use
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproSetConsoleTextColor(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg);

// gproResetConsoleTextColor
//	Reset color of console text.
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproResetConsoleTextColor();

// gproGetConsoleTextColor
//	Get channel flag for requested color.
//		params 'r', 'g', 'b', 'a': flags for generating a color channel 
//			description using red, green, blue and intensity, respectively; 
//			interpreted as booleans
//		return SUCCESS: color channel descriptor
gpro_ConsoleColorChannel gproGetConsoleTextColor(flag const r, flag const g, flag const b, flag const a);


// gproClearConsoleArea
//	Clear console area by setting it to a predefined state.
//		param 'fg': description of foreground (character) channels to use
//		param 'bg': description of background (console) channels to use
//		param 'lhs': left-hand character to display
//		param 'rhs': right-hand character to display
//		param 'w': width of drawing area in console
//		param 'h': height of drawing area in console
//		param 'w_area': width of clear area (right < w)
//		param 'h_area': height of clear area (bottom < h)
//		param 'x_area': horizontal offset of clear area in console (left = 0)
//		param 'y_area': vertical offset of clear area in console (top = 0)
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproClearConsoleArea(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg, byte const lhs, byte const rhs, byte const w, byte const h, byte const w_area, byte const h_area, byte const x_area, byte const y_area);

// gproDrawConsolePixel
//	Draw a colored "pixel" in the console.
//		param 'fg': description of foreground (character) channels to use
//		param 'bg': description of background (console) channels to use
//		param 'lhs': left-hand character to display
//		param 'rhs': right-hand character to display
//		param 'w': width of drawing area in console
//		param 'h': height of drawing area in console
//		param 'x': horizontal pixel position index in console (left = 0)
//		param 'y': vertical pixel position index in console (top = 0)
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproDrawConsolePixel(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg, byte const lhs, byte const rhs, byte const w, byte const h, byte const x, byte const y);

// gproDrawConsoleLine
//	Draw a line between two pixels in the console.
//		param 'fg': description of foreground (character) channels to use
//		param 'bg': description of background (console) channels to use
//		param 'lhs': left-hand character to display
//		param 'rhs': right-hand character to display
//		param 'w': width of drawing area in console
//		param 'h': height of drawing area in console
//		param 'x0': first horizontal pixel position index in console (left = 0)
//		param 'y0': first vertical pixel position index in console (top = 0)
//		param 'x1': second horizontal pixel position index in console
//		param 'y1': second vertical pixel position index in console
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproDrawConsoleLine(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg, byte const lhs, byte const rhs, byte const w, byte const h, byte const x0, byte const y0, byte const x1, byte const y1);

// gproDrawConsoleTriangle
//	Draw a line between two pixels in the console.
//		param 'fg': description of foreground (character) channels to use
//		param 'bg': description of background (console) channels to use
//		param 'lhs': left-hand character to display
//		param 'rhs': right-hand character to display
//		param 'w': width of drawing area in console
//		param 'h': height of drawing area in console
//		param 'x0': first horizontal pixel position index in console (left = 0)
//		param 'y0': first vertical pixel position index in console (top = 0)
//		param 'x1': second horizontal pixel position index in console
//		param 'y1': second vertical pixel position index in console
//		param 'x2': third horizontal pixel position index in console
//		param 'y2': third vertical pixel position index in console
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproDrawConsoleTriangle(gpro_ConsoleColorChannel const fg, gpro_ConsoleColorChannel const bg, byte const lhs, byte const rhs, byte const w, byte const h, byte const x0, byte const y0, byte const x1, byte const y1, byte const x2, byte const y2);


// gproDrawTestPatch
//	Draw test grid of pixels.
//		return SUCCESS: number of pixels drawn
flag gproDrawTestPatch();


//-----------------------------------------------------------------------------

// gproViewportTransform
//	Convert a point in normalized device coordinates (NDC) to viewport.
//		param 'x_view_out': non-null pointer to value to store viewport X position: [0, w) = [left, right)
//		param 'y_view_out': non-null pointer to value to store viewport Y position: [0, h) = [top, bottom)
//		param 'x_ndc': NDC X position: [-1, +1) = [left, right)
//		param 'y_ndc': NDC Y position: [-1, +1) = [bottom, top)
//		param 'w': width of drawing area in console
//		param 'h': height of drawing area in console
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproViewportTransform(byte* const x_view_out, byte* const y_view_out, f32 const x_ndc, f32 const y_ndc, byte const w, byte const h);

// gproViewportTransformInverse
//	Convert a point in normalized device coordinates (NDC) to viewport.
//		param 'x_ndc_out': non-null pointer to value to store NDC X position: [-1, +1) = [left, right)
//		param 'y_ndc_out': non-null pointer to value to store NDC Y position: [-1, +1) = [bottom, top)
//		param 'x_view': viewport X position: [0, w) = [left, right)
//		param 'y_view': viewport Y position: [0, h) = [top, bottom)
//		param 'w': width of drawing area in console
//		param 'h': height of drawing area in console
//		return SUCCESS: zero
//		return FAILURE: negative
flag gproViewportTransformInverse(f32* const x_ndc_out, f32* const y_ndc_out, byte const x_view, byte const y_view, byte const w, byte const h);


//-----------------------------------------------------------------------------

#ifdef __cplusplus
}
#else	// !__cplusplus

#endif	// __cplusplus


#endif	// !_GPRO_DISPLAY_H_
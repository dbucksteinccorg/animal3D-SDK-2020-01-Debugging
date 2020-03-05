// gpro_input.c/.cpp

#include "GPRO-FW/gpro_input/gpro_input.h"


#if (__PLATFORM == WINDOWS)

#include <Windows.h>


flag gproIsVirtualKeyPressed(i32 const vk)
{
	return GetAsyncKeyState(vk);
}


#endif	// WINDOWS

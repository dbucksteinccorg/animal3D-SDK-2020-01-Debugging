// _platform.h
// Daniel S. Buckstein, 2020
// Determination of platform and architecture.

#ifndef __PLATFORM_H_
#define __PLATFORM_H_


// set macro for architecture
///
#if (defined _M_IX86 || defined __i386__)		// 32-bit
#define __INSTR_SET				x86
#define __ARCH_BITS				32

#elif (defined _M_X64 || defined __x86_64__)	// 64-bit
#define __INSTR_SET				x86_64
#define __ARCH_BITS				64

#else											// !32-/64-bit
#error "ERROR: UNKNOWN/INVALID INSTRUCTION SET AND ARCHITECTURE"

#endif	// 32-/64-bit


// set development platform
///
#if (defined _WIN32)	// Windows, MSVC
#define __PLATFORM				WINDOWS
#define __COMPILER				MSVC

#else
#error "ERROR: UNKNOWN/INVALID PLATFORM AND COMPILER"

#endif	// platform


#endif	// !__PLATFORM_H_
// _typedefs.h
// Daniel S. Buckstein, 2020
// General and all-purpose shorthand type definitions.

#ifndef __TYPEDEFS_H_
#define __TYPEDEFS_H_


#include "_platform.h"
#include "_macros.h"


#pragma warning(disable : 142)


// core signed integer type abbreviations
///
typedef	signed		__int8			i8,		byte;
typedef	signed		__int16			i16,	word;
typedef	signed		__int32			i32,	dword;
typedef	signed		__int64			i64,	qword;

// core unsigned integer type abbreviations
///
typedef	unsigned	__int8			ui8,	ubyte;
typedef	unsigned	__int16			ui16,	uword;
typedef	unsigned	__int32			ui32,	udword;
typedef	unsigned	__int64			ui64,	uqword;


// core floating point type abbreviations
///
typedef				float			f32,	flt;
typedef				double			f64,	dbl;


// core pointer type abbreviations
///
typedef				void			* ptr,	* addr;
typedef				void const		* kptr,	* kaddr;

// size, flag and pointer difference type abbreviations
///
typedef	token_cat( i, __ARCH_BITS)	index, flag, ptrdiff, addrdiff;
typedef token_cat(ui, __ARCH_BITS)	uindex, size, offset, unit;


// convenient string type abbreviations
///
typedef				byte			cstr[__ARCH_BITS], * pcstr, * pbyte;
typedef				uword			wcstr[__ARCH_BITS], * pwcstr;
typedef				byte const		* kpcstr, * kpbyte;
typedef				uword const		* kpwcstr;


// size abbreviations
///
#define				szbyte			((size)sizeof(byte))
#define				szword			((size)sizeof(word))
#define				szdword			((size)sizeof(dword))
#define				szqword			((size)sizeof(qword))
#define				szflt			((size)sizeof(flt))
#define				szdbl			((size)sizeof(dbl))
#define				szaddr			((size)sizeof(addr))
#define				szunit			((size)sizeof(unit))
#define				szcstr			((size)sizeof(cstr))
#define				szwcstr			((size)sizeof(wcstr))


#endif	// !__TYPEDEFS_H_
#ifndef THOR_PLATFORM_H_
#define THOR_PLATFORM_H_

#ifndef _CRT_SECURE_NO_WARNING
#define USE_SECURE_CRT 1
#endif // !_CRT_SECURE_NO_WARNING


// DLL export and import definitions.
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)


// Unsigned base types.
typedef unsigned char 		uint8;		// 8-bit  unsigned.
typedef unsigned short int	uint16;		// 16-bit unsigned.
typedef unsigned int		uint32;		// 32-bit unsigned.
typedef unsigned long long	uint64;		// 64-bit unsigned.

// Signed base types.
typedef	signed char			int8;		// 8-bit  signed.
typedef signed short int	int16;		// 16-bit signed.
typedef signed int	 		int32;		// 32-bit signed.
typedef signed long long	int64;		// 64-bit signed.

// Other types.
typedef unsigned char		byte;

// Character types.
typedef char				ANSICHAR;	// An ANSI character - 8-bit fixed-width representation of 7-bit characters.
typedef wchar_t				WIDECHAR;	// A wide character

// Function type macros.
#define CDECL	    __cdecl											/* Standard C function */
#define STDCALL		__stdcall										/* Standard calling convention */
#define FORCEINLINE __forceinline									/* Force code to be inline */
#define FORCENOINLINE __declspec(noinline)							/* Force code to NOT be inline */

// DLL export and import definitions
//#define DLLEXPORT __declspec(dllexport)
//#define DLLIMPORT __declspec(dllimport)
// If we are Microsoft C/C++ Compiler
#if defined(_MSC_VER)
#if defined(DLL_EXPORT)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
// If we are non Windows (Export by default) or compiling to a StaticLibrary
#else
#define API 
#endif

#endif // THOR_PLATFORM_H_

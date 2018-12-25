#ifndef THOR_CORE_TYPES_H_
#define THOR_CORE_TYPES_H_

#include <cstdint>

typedef int32_t			int32;
typedef int64_t			int64;
typedef uint32_t		uint32;
typedef uint64_t		uint64;
typedef unsigned char	byte;


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

#endif // THOR_CORE_TYPES_H_

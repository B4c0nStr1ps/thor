#ifndef THOR_ASSERTION_H_
#define THOR_ASSERTION_H_

#ifndef NDEBUG
#define ASSERTION_ENABLED
#endif // NDEBUG

 
#ifdef ASSERTION_ENABLED

#include <intrin.h>

#define DEBUG_BREAK() {__debugbreak();} 

#define T_ASSERT(expr) { \
	if (expr) {} \
	else { \
		DEBUG_BREAK(); \
	}\
}
#else
#define T_ASSERT(expr)
#endif // !ASSERTION_ENABLED

 
#endif // THOR_ASSERTION_H_

#ifndef THOR_STRING_UTILS_H_
#define THOR_STRING_UTILS_H_

#include "string.h"
 
namespace Thor
{
	namespace StringUtils
	{
		String FormatImplementation(const ANSICHAR* format, ...);

		template <typename ... Args>
		String Format(const ANSICHAR* format, Args const& ... args)
		{
			return FormatImplementation(format, args...);
		}

		template <typename ... Args>
		String Format(const Thor::String* format, Args const& ... args)
		{
			return FormatImplementation(format->RawData(), args...);
		}
	}
}
 
#endif // THOR_STRING_UTILS_H_

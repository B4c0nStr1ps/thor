#ifndef THOR_DIAGNOSTICS_H_
#define THOR_DIAGNOSTICS_H_

#include "platform/platform.h"
#include "string.h"
#include "string_utils.h"

#define THOR_TRACE(listener, verbosity, format, ...) \
{ \
	Thor::Diagnostics::Trace(__FILE__, __LINE__, verbosity, format,  ##__VA_ARGS__); \
}

#define THOR_DEFINE_TRACE_LISTENER_STATIC(listenerName) \
static class TraceListener##listenerName \
{ \
public: \
	FORCEINLINE TraceListener##listenerName() {}\
} listenerName;

namespace Thor
{
	namespace Diagnostics
	{
		enum class TraceVerbosity
		{
			Information,
			Warning,
			Error
		};

		const ANSICHAR* VerbosityString(TraceVerbosity verbosity)
		{
			switch (verbosity)
			{
			case TraceVerbosity::Information:
				return "INFORMATION";
			case TraceVerbosity::Warning:
				return "WARNING";
			case TraceVerbosity::Error:
				return "ERROR";
			default:
				break;
				return "UNKOWN VERBOSITY";
			}
		}

		template<typename ...Args>
		void Trace(const ANSICHAR* filename, int32 callerLine, TraceVerbosity verbosity, const ANSICHAR* format, Args... args)
		{
			Thor::String userMessage = Thor::StringUtils::Format(format, args...);
			Thor::String message = Thor::StringUtils::Format("[%s] %s\n|--File: %s Line: %d\n", VerbosityString(verbosity), userMessage.RawData(), filename, callerLine);
			OutputDebugStringA(message.RawData());
		}

	} // namespace Diagnostics.
} // namespace Thor.

#endif // THOR_DIAGNOSTICS_H_

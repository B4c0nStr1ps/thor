#include <thor/core/platform/platform_time.h>
#include <thor/core/platform/windows_platform_api.h>
#include <thor/core/assertion.h>

namespace Thor
{
	namespace Platform
	{
		namespace Time
		{
			namespace
			{
				double K_SECONDS_PER_CYCLE = 0.0;
			}
		}
	}
}


double Thor::Platform::Time::Seconds()
{
	LARGE_INTEGER cycles;
	QueryPerformanceCounter(&cycles);
	return cycles.QuadPart * GetSecondsPerCycle();
}

double Thor::Platform::Time::GetSecondsPerCycle()
{
	if (K_SECONDS_PER_CYCLE == 0.0)
	{
		LARGE_INTEGER frequency;
		T_ASSERT(QueryPerformanceFrequency(&frequency) == TRUE);
		K_SECONDS_PER_CYCLE = 1.0 / frequency.QuadPart;
	}
	return K_SECONDS_PER_CYCLE;
}

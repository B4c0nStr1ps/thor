#include <thor/thor.h>
#include <thor/core/memory.h>
#include <thor/core/platform/platform_time.h>

int main()
{
	double secondsPerCycle = Thor::Platform::Time::GetSecondsPerCycle();
	double seconds1 = Thor::Platform::Time::Seconds();
	_sleep(500);
	double seconds2 = Thor::Platform::Time::Seconds();
	double dt = seconds2 - seconds1;
	float fdt = seconds2 - seconds1;
	return 0;
}
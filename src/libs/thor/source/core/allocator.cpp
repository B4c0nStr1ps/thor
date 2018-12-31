#include <thor/core/allocator.h>



int32 Thor::Allocators::AllocationPolicies::CalculateGrow(int32 numRequiredElements)
{
	int32 grow = 4;
	if (numRequiredElements > grow)
	{
		//From UE4
		// Allocate slack for the array proportional to its size.
		grow = int32(numRequiredElements) + 3 * int32(numRequiredElements) / 8 + 16;
	}
	return grow;
}

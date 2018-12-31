#include <thor/core/allocator.h>
#include <thor/core/memory.h>
//#include <new>

//Overload global new
/*
void* operator new(size_t size)
{
	return Thor::Memory::Malloc(size, Thor::Allocators::K_DEFAULT_ALIGNMENT);
}

void* operator new[](size_t size)
{
	return Thor::Memory::Malloc(size, Thor::Allocators::K_DEFAULT_ALIGNMENT);
}

void* operator new(size_t size, size_t alignment)
{
	return Thor::Memory::Malloc(size, alignment);
}

void* operator new(size_t size, size_t alignment, const std::nothrow_t&) noexcept
{
	return Thor::Memory::Malloc(size, alignment);
}

void* operator new[](size_t size, size_t alignment)
{
	return Thor::Memory::Malloc(size, alignment);
}

void* operator new[](size_t size, size_t alignment, const std::nothrow_t&) noexcept
{
	return Thor::Memory::Malloc(size, alignment);
}

// C++14 deleter
void operator delete(void* memoryAddress, std::size_t size) noexcept
{
	Thor::Memory::Free(memoryAddress);
}

void operator delete[](void* memoryAddress, std::size_t size) noexcept
{
	Thor::Memory::Free(memoryAddress);
}

void operator delete(void* memoryAddress) noexcept
{
	Thor::Memory::Free(memoryAddress);
}

void operator delete[](void* memoryAddress) noexcept
{
	Thor::Memory::Free(memoryAddress);
}
*/
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

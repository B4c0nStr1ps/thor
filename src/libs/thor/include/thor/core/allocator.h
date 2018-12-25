#ifndef THOR_ALLOCATOR_H_
#define THOR_ALLOCATOR_H_

#include <malloc.h>
#include "assertion.h"
#include "platform/platform.h"

namespace Thor
{
	struct Allocator
	{
		explicit Allocator() { }

		// Aligned allocation function. IMPORTANT: 'alignment'
		// must be a power of 2 (typically 4 or 16).
		void* Allocate(size_t sizeBytes, size_t alignment)
		{
			T_ASSERT(alignment >= 1);
			T_ASSERT(alignment <= 128);
			T_ASSERT((alignment & (alignment - 1)) == 0); // pwr of 2
			// Determine total amount of memory to allocate.
			size_t expandedSizeBytes = sizeBytes + alignment;
			// Allocate unaligned block & convert address to uintptr_t.
			uintptr_t rawAddress = reinterpret_cast<uintptr_t>(malloc(expandedSizeBytes));
			// Calculate the adjustment by masking off the lower bits
			// of the address, to determine how "misaligned" it is.
			size_t mask = (alignment - 1);
			uintptr_t misalignment = (rawAddress & mask);
			ptrdiff_t adjustment = alignment - misalignment;
			// Calculate the adjusted address.
			uintptr_t alignedAddress = rawAddress + adjustment;
			// Store the adjustment in the byte immediately preceding the adjusted address.
			T_ASSERT(adjustment < 256);
			typedef unsigned char U8;
			U8* pAlignedMem = reinterpret_cast<U8*>(alignedAddress);
			pAlignedMem[-1] = static_cast<U8>(adjustment);
			return static_cast<void*>(pAlignedMem);
		}

		void FreeAligned(void* address)
		{
			T_ASSERT(address != nullptr);
			typedef unsigned char U8;
			const U8* pAlignedMem = reinterpret_cast<const U8*>(address);
			uintptr_t alignedAddress = reinterpret_cast<uintptr_t>(address);
			ptrdiff_t adjustment = static_cast<ptrdiff_t>(pAlignedMem[-1]);
			uintptr_t rawAddress = alignedAddress - adjustment;
			void* pRawMem = reinterpret_cast<void*>(rawAddress);
			free(pRawMem);
		}

		template<typename ElementType>
		ElementType* New()
		{
			auto address = Allocate(sizeof(ElementType), alignof(ElementType));
			return new (address) ElementType();
		}

		template<typename ElementType>
		void Delete(ElementType* address)
		{
			T_ASSERT(address != nullptr);
			FreeAligned(address);
		}

	};

	typedef Allocator DefaultAllocator;

	namespace AllocationPolicies
	{
		int32 CalculateGrow(int32 numRequiredElements);
	}
}

#endif // THOR_ALLOCATOR_H_

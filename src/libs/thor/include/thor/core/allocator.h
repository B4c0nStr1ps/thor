#ifndef THOR_ALLOCATOR_H_
#define THOR_ALLOCATOR_H_

#include <cstdint>
#include <malloc.h>

namespace Thor
{
	struct Allocator
	{
		explicit Allocator() { }

		// Aligned allocation function. IMPORTANT: 'alignment'
		// must be a power of 2 (typically 4 or 16).
		void* AllocateAligned(size_t sizeBytes, size_t alignment)
		{
			//ASSERT(alignment >= 1);
			//ASSERT(alignment <= 128);
			//ASSERT((alignment & (alignment - 1)) == 0); // pwr of 2
			// Determine total amount of memory to allocate.
			size_t expandedSizeBytes = sizeBytes + alignment;			// Allocate unaligned block & convert address to uintptr_t.
			uintptr_t rawAddress = reinterpret_cast<uintptr_t>(AllocateUnaligned(expandedSizeBytes));
			// Calculate the adjustment by masking off the lower bits
			// of the address, to determine how "misaligned" it is.
			size_t mask = (alignment - 1);
			uintptr_t misalignment = (rawAddress & mask);
			ptrdiff_t adjustment = alignment - misalignment;
			// Calculate the adjusted address.
			uintptr_t alignedAddress = rawAddress + adjustment;			// Store the adjustment in the byte immediately preceding the adjusted address.
			//ASSERT(adjustment < 256);
			typedef unsigned char U8;
			U8* pAlignedMem = reinterpret_cast<U8*>(alignedAddress);
			pAlignedMem[-1] = static_cast<U8>(adjustment);
			return static_cast<void*>(pAlignedMem);
		}

		template<typename ElementType>
		ElementType* AllocateAligned()
		{
			auto address = AllocateAligned(sizeof(ElementType), alignof(ElementType));
			return new (address) ElementType();
		}

		void FreeAligned(void* address)
		{
			typedef unsigned char U8;
			const U8* pAlignedMem = reinterpret_cast<const U8*>(address);
			uintptr_t alignedAddress = reinterpret_cast<uintptr_t>(address);
			ptrdiff_t adjustment = static_cast<ptrdiff_t>(pAlignedMem[-1]);
			uintptr_t rawAddress = alignedAddress - adjustment;
			void* pRawMem = reinterpret_cast<void*>(rawAddress);
			FreeUnaligned(pRawMem);
		}

		void* AllocateUnaligned(size_t sizeBytes)
		{
			auto address = malloc(sizeBytes);
			return address;
		}

		void FreeUnaligned(void* address)
		{
			//ASSERT(sizeBytes != nullptr);
			free(address);
		}

		template<typename ElementType>
		ElementType* New()
		{
			auto address = AllocateAligned(sizeof(ElementType), alignof(ElementType));
			return new (address) ElementType();
		}

		template<typename ElementType>
		void Delete(ElementType* address)
		{
			FreeAligned(address);
		}

	};
}

#endif // THOR_ALLOCATOR_H_

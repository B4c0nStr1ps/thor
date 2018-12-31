#ifndef THOR_ALLOCATOR_H_
#define THOR_ALLOCATOR_H_

#include "platform/platform.h"
#include "assertion.h"
#include "memory.h"

namespace Thor
{	
	namespace Allocators
	{
		namespace AllocationPolicies
		{
			int32 CalculateGrow(int32 numRequiredElements);
		}

		constexpr const size_t K_DEFAULT_ALIGNMENT = 16;

		struct HeapAllocator
		{
			explicit HeapAllocator() { }

			// Aligned allocation function. IMPORTANT: 'alignment'
			// must be a power of 2 (typically 4 or 16).
			void* Allocate(size_t sizeBytes, size_t alignment = K_DEFAULT_ALIGNMENT)
			{
				T_ASSERT(alignment >= 1);
				T_ASSERT(alignment <= 128);
				T_ASSERT((alignment & (alignment - 1)) == 0); // pwr of 2
				return Memory::Malloc(sizeBytes, alignment);
			}

			void Free(void* address)
			{
				T_ASSERT(address != nullptr);
				Memory::Free(address);
			}

			template<typename ElementType, typename... Args>
			ElementType* New(Args&&... args)
			{
				void* address = Allocate(sizeof(ElementType), alignof(ElementType));
				return new (address) ElementType(std::forward<Args>(args)...);
			}

			template<typename ElementType>
			void Delete(ElementType* address)
			{
				T_ASSERT(address != nullptr);
				FreeAligned(address);
			}

		};

		typedef HeapAllocator DefaultAllocator;
	}
}

#endif // THOR_ALLOCATOR_H_

#ifndef THOR_MEMORY_H_
#define THOR_MEMORY_H_
 
#include "core_types.h"
#include "assertion.h"

#include <type_traits>
#include <memory>

namespace Thor
{
	namespace Memory
	{
		template<typename ElementType>
		FORCEINLINE typename std::enable_if<std::is_trivially_copyable<ElementType>::value>::type
			CopyBuffer(ElementType* destination, ElementType* source, int64 numElements)
		{
			T_ASSERT(destination != nullptr);
			T_ASSERT(source != nullptr);

			memcpy(destination, source, numElements * sizeof(ElementType));
		}

		template<typename  ElementType>
		FORCEINLINE typename std::enable_if<!std::is_trivially_copyable<ElementType>::value>::type
			CopyBuffer(ElementType* destination, ElementType* source, int64 numElements)
		{
			T_ASSERT(destination != nullptr);
			T_ASSERT(source != nullptr);

			for (int i = 0; i < numElements; ++i)
			{
				new (destination + i) ElementType(*(source + i));
			}
		}

		template<typename ElementType>
		FORCEINLINE typename std::enable_if<std::is_trivially_move_constructible<ElementType>::value>::type
			MoveBuffer(ElementType* destination, ElementType* source, int64 numElements)
		{
			T_ASSERT(destination != nullptr);
			T_ASSERT(source != nullptr);

			destination = source;
			source = nullptr;
		}

		template<typename ElementType>
		FORCEINLINE typename std::enable_if<!std::is_trivially_move_constructible<ElementType>::value>::type
			MoveBuffer(ElementType* destination, ElementType* source, int64 numElements)
		{
			CopyBuffer(destination, source, numElements);
		}
	}
}
 
#endif // THOR_MEMORY_H_

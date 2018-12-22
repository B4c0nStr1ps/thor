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

			std::swap(source, destination);
		}

		template<typename ElementType>
		FORCEINLINE typename std::enable_if<!std::is_trivially_move_constructible<ElementType>::value>::type
			MoveBuffer(ElementType* destination, ElementType* source, int64 numElements)
		{
			CopyBuffer(*destination, source, numElements);
		}

		template<typename ElementType>
		typename std::enable_if<std::is_trivially_destructible<ElementType>::value>::type 
			DestroyItems(ElementType* element, int64 count)
		{
		}

		template<typename ElementType>
		FORCEINLINE typename std::enable_if<!std::is_trivially_destructible<ElementType>::value>::type
			DestroyItems(ElementType* element, int64 count)
		{
			//FROM UE4
			while (count)
			{
				// We need a typedef here because VC won't compile the destructor call below if ElementType itself has a member called ElementType
				typedef ElementType DestructItemsElementTypeTypedef;

				element->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
				++element;
				--count;
			}
		}

		template<typename ElementType>
		FORCEINLINE void RelocateBuffer(ElementType* destination, ElementType* source, int64 numElements)
		{
			T_ASSERT(destination != source);
			memcpy(destination, source, numElements * sizeof(ElementType));
		}

		template<typename ElementType>
		FORCEINLINE typename std::enable_if<!std::is_trivially_constructible<ElementType>::value>::type
			ConstructItems(void* destination, const ElementType* source, int32 numElements)
		{
			while (numElements)
			{
				new (destination) ElementType(*source);
				++(ElementType*&)destination;
				++source;
				--numElements;
			}
		}

		template<typename ElementType>
		FORCEINLINE typename std::enable_if<std::is_trivially_constructible<ElementType>::value>::type
			ConstructItems(void* destination, const ElementType* source, int32 numElements)
		{
			memcpy(destination, source, numElements * sizeof(ElementType));
		}

	}
}
 
#endif // THOR_MEMORY_H_

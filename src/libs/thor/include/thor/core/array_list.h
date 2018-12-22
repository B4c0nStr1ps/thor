#ifndef THOR_ARRAY_LIST_H_
#define THOR_ARRAY_LIST_H_

#include "assertion.h"
#include "allocator.h"
#include "core_types.h"
#include "memory.h" 

#include <initializer_list>
#include <memory>

namespace Thor
{
	template<typename InElementType, typename InAllocator = DefaultAllocator>
	class ArrayList
	{

	public:
		typedef InElementType		ElementType;
		typedef InAllocator			AllocatorType;

	private:

		template<typename SourceArrayType, typename DestinationArrayType>
		static FORCEINLINE typename std::enable_if<std::is_trivially_move_constructible<typename SourceArrayType::ElementType>::value>::type
			MoveOrCopy(DestinationArrayType& destination, SourceArrayType& source)
		{
			if (destination.m_begin == nullptr)
			{
				Memory::DestroyItems(destination.m_begin, destination.Length());
				destination.m_allocator.FreeAligned(destination.m_begin);
			}
			destination.m_begin = source.m_begin;
			source.m_begin = nullptr;
			source.m_end = nullptr;
			source.m_capacity = 0;
		}

		template<typename SourceArrayType, typename DestinationArrayType>
		static FORCEINLINE typename std::enable_if<!std::is_trivially_move_constructible<typename SourceArrayType::ElementType>::value>::type
			MoveOrCopy(DestinationArrayType& destination, SourceArrayType& source)
		{
			Memory::CopyBuffer<ElementType>(destination.m_begin, source.m_begin, source.Length());
		}


	public:

		ArrayList()
			:m_begin{ nullptr }, m_end{ nullptr }, m_capacity{ 0 }
		{}

		explicit ArrayList(int capacity)
		{
			m_begin = DoAllocate(capacity);
			m_end = m_begin;
			m_capacity = capacity;
		}

		ArrayList(const ArrayList& other)
		{
			m_capacity = other.m_capacity;
			m_begin = DoAllocate(m_capacity);
			int32 length = other.Length();
			Memory::CopyBuffer<ElementType>(m_begin, other.m_begin, length);
			m_end = m_begin + length;
		}

		ArrayList(ArrayList&& other)
		{
			m_capacity = other.m_capacity;
			m_begin = DoAllocate(m_capacity);
			int length = other.Length();
			MoveOrCopy(*this, other);
			m_end = m_begin + length;
		}

		ArrayList(std::initializer_list<ElementType> elements)
		{
			size_t length = elements.size();
			m_capacity = length;
			m_begin = DoAllocate(m_capacity);
			Memory::ConstructItems(m_begin, elements.begin(), length);
			m_end = m_begin + length;
		}

		FORCEINLINE int Add(ElementType&& item)
		{
			return Emplace(std::move(item));
		}

		FORCEINLINE int Add(const ElementType& item)
		{
			return Emplace(item);
		}

		FORCEINLINE int32 AddUninitialized(int32 count = 1)
		{
			T_ASSERT(count > 0);
			int32 length = Length();
			if ((length + count) > m_capacity)
			{
				ResizeGrow(length + count);
			}
			++m_end;
			return length;
		}

		template <typename... ArgsType>
		FORCEINLINE int32 Emplace(ArgsType&&... Args)
		{
			const int32 index = AddUninitialized(1);
			new (m_begin + index) ElementType(std::forward<ArgsType>(Args)...);
			return index;
		}

		int32 Length() const
		{
			return (int32)(m_end - m_begin);
		}

		int32 Capacity() const
		{
			return m_capacity;
		}

		bool IsEmpty() const
		{
			return (m_end - m_begin) == 0;
		}

		void Reserve(int32 capacity)
		{
			if (capacity > m_capacity)
			{
				Grow(capacity);
			}
		}

		void Clear()
		{
			if (m_begin != nullptr)
			{
				Memory::DestroyItems(m_begin, Length());
			}
			m_end = m_begin;
		}

		~ArrayList()
		{
			Clear();
			if (m_begin != nullptr)
			{
				m_allocator.FreeAligned(m_begin);
			}
			m_begin = nullptr;
			m_end = nullptr;
		}

		void RemoveAt(int32 index)
		{
			T_ASSERT(index < Length());
			Memory::DestroyItems(m_begin + index, 1);
			Memory::RelocateBuffer(m_begin + index, m_end - 1, 1);
			--m_end;
		}

		int32 Remove(const ElementType& item)
		{
			for (int32 i = 0; i < Length(); ++i)
			{
				if (item == *(m_begin + i))
				{
					RemoveAt(i);
					return i;
				}
			}
			return -1;
		}

		ElementType& operator[](int index)
		{
			T_ASSERT(index < Length());
			return *(m_begin + index);
		}

		const ElementType& operator[](int index) const
		{
			T_ASSERT(index < Length());
			return *(m_begin + index);
		}

		ElementType* Data() const
		{
			return m_begin;
		}

	private:

		ElementType* m_begin;
		ElementType* m_end;
		int32 m_capacity;
		AllocatorType m_allocator;

	private:

		ElementType* DoAllocate(int itemsCount)
		{
			T_ASSERT(itemsCount >= 0);
			return (ElementType*)m_allocator.Allocate(itemsCount * sizeof(ElementType), alignof(ElementType));
		}

		void ResizeGrow(int32 numElements)
		{
			int32 capacity = AllocationPolicies::CalculateGrow(numElements);
			Grow(capacity);
		}

		void Grow(int32 capacity)
		{
			T_ASSERT(capacity > m_capacity);
			void* newAddress = m_allocator.Allocate(capacity * sizeof(ElementType), alignof(ElementType));
			memcpy(newAddress, m_begin, Length() * sizeof(ElementType));
			int currentLength = Length();
			if (m_begin != nullptr)
			{
				m_allocator.FreeAligned(m_begin);
			}			
			m_begin = (ElementType*)newAddress;
			m_end = m_begin + currentLength;
			m_capacity = capacity;
		}
	};
}

#endif // THOR_ARRAY_LIST_H_
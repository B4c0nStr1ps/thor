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
	template<typename ElementType, typename AllocatorType = DefaultAllocator>
	class ArrayList
	{
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
			Memory::CopyBuffer<ElementType>(m_begin, m_begin, length);
			m_end = m_begin + length;
		}
		ArrayList(ArrayList&& other)
			:m_capacity(other.m_capacity)
		{
			if (m_begin != nullptr)
			{
				Clear();
			}
			int length = other.Length();
			Memory::MoveBuffer(m_begin, other.m_begin, length);
			m_end = m_begin + length;
		}
		//ArrayList(std::initializer_list<ElementType> elements);

		int Add(ElementType&& item)
		{
			int index = m_end - m_begin;

			if (index >= m_capacity)
			{
				DoGrow();
			}

			new (m_end) ElementType(std::move(item));
			++m_end;
			return index;
		}

		int Add(const ElementType& item)
		{
			int index = m_end - m_begin;

			if (index >= m_capacity)
			{
				DoGrow();
			}

			new (m_end) ElementType(item);
			++m_end;
			return index;
		}

		//int32 AddUninitialized(int32 Count = 1);

		int Length() const
		{
			return m_end - m_begin;
		}

		int Capacity() const
		{
			return m_capacity;
		}

		bool IsEmpty() const
		{
			return (m_end - m_begin) == 0;
		}

		void Reserve(int capacity)
		{
			if (capacity > m_capacity)
			{
				DoGrow();
			}
		}

		//@TODO -> REWORK
		void Clear()
		{
			int32 counter = Length();
			ElementType* element = m_begin;
			typedef ElementType DestructItemsElementTypeTypedef;
			while (counter)
			{
				m_begin->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
				++element;
				--counter;
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

		void RemoveAt(int index)
		{
			typedef ElementType DestructItemsElementTypeTypedef;
			T_ASSERT(index < Length());
			(m_begin + index)->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
			memcpy(m_begin + index, m_end - 1, sizeof(ElementType));
			--m_end;
		}

		int Remove(const ElementType& item)
		{
			for (int i = 0; i < Length(); ++i)
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
		int m_capacity;
		AllocatorType m_allocator;

	private:
		ElementType* DoAllocate(int itemsCount)
		{
			T_ASSERT(itemsCount >= 0);
			return (ElementType*)m_allocator.Allocate(itemsCount * sizeof(ElementType), alignof(ElementType));
		}

		void DoGrow()
		{
			m_capacity = m_capacity * 2;
			void* newAddress = m_allocator.Allocate(m_capacity * sizeof(ElementType), alignof(ElementType));
			memcpy(newAddress, m_begin, Length() * sizeof(ElementType));
			int currentLength = Length();
			if (m_begin != nullptr)
			{
				m_allocator.FreeAligned(m_begin);
			}			
			m_begin = (ElementType*)newAddress;
			m_end = m_begin + currentLength;
		}
	};
}

#endif // THOR_ARRAY_LIST_H_
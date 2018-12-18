#ifndef THOR_FIXED_ARRAY_H_
#define THOR_FIXED_ARRAY_H_
 
#include "assertion.h"

namespace Thor
{
	template<typename ElementType, size_t NumElements, size_t Alignment = alignof(ElementType)>
	struct alignas(Alignment) FixedArray
	{
		explicit FixedArray() noexcept {}
		explicit FixedArray(const FixedArray& rhs) noexcept {}
		FixedArray(FixedArray&&) = default;
		FixedArray& operator=(FixedArray&& Other) = default;
		FixedArray& operator=(const FixedArray& Other) = default;

		// Accessors.
		ElementType& operator[](size_t index)
		{
			T_ASSERT(index < NumElements);
			return _data[index];
		}

		const ElementType& operator[](size_t index) const
		{
			T_ASSERT(index < NumElements);
			return _data[index];
		}

		/** The number of elements in the array. */
		size_t Length() const { return NumElements; }

		// Comparisons.
		friend bool operator==(const FixedArray& A, const FixedArray& B)
		{
			if (A.Length() != B.Length())
			{
				return false;
			}
			for (int i = 0; i < NumElements; ++i)
			{
				if (!(A[i] == B[i]))
				{
					return false;
				}
			}
			return true;
		}

		friend bool operator!=(const FixedArray& A, const FixedArray& B)
		{
			if (A.Length() != B.Length())
			{
				return true;
			}
			for (int i = 0; i < NumElements; ++i)
			{
				if (!(A[i] == B[i]))
				{
					return true;
				}
			}
			return false;
		}

	private:
		ElementType _data[NumElements];
	};
}
 
#endif // THOR_FIXED_ARRAY_H_

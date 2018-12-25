#ifndef THOR_STRING_H_
#define THOR_STRING_H_

#include "core_types.h"
#include "platform/platform_string.h"
#include "thor_traits.h"
#include "array_list.h" 

namespace Thor
{
	class String
	{
	private:
		typedef ArrayList<ANSICHAR> DataType;
		DataType m_data;

	public:

		using CharType = ANSICHAR;

		String() = default;
		String(String&&) = default;
		String(const String&) = default;
		String& operator=(String&&) = default;
		String& operator=(const String&) = default;

		template<typename InCharType, typename = typename std::enable_if<TypeTraits::IsCharType<InCharType>::value>>
		String(const InCharType* source)
		{
			int32 length = Platform::String::Strlen(source) + 1; //+1 for null terminator.
			m_data.AddUninitialized(length);
			Platform::String::Convert(m_data.Data(), length, source, length);
		}

		FORCEINLINE String& operator=(const CharType* other)
		{
			m_data.Clear();
			int32 length = Platform::String::Strlen(other) + 1; //+1 for null terminator.
			m_data.AddUninitialized(length);
			Memory::Memcpy(m_data.Data(), other, length * sizeof(CharType));
			return *this;
		}

		template<typename InCharType, typename = typename std::enable_if<TypeTraits::IsCharType<InCharType>::value>>
		FORCEINLINE String& operator=(const InCharType* other)
		{
			m_data.Clear();
			int32 length = Platform::String::Strlen(other) + 1; //+1 for null terminator.
			m_data.AddUninitialized(length);
			Platform::String::Convert(m_data.Data(), length, other, length);
			return *this;
		}

		FORCEINLINE const CharType* RawData() const
		{
			return m_data.Data();
		}

		FORCEINLINE int32 Length() const
		{
			return m_data.Length();
		}

		FORCEINLINE CharType& operator[](int32 index)
		{
			return m_data[index];
		}

		FORCEINLINE const CharType& operator[](int32 index) const
		{
			return m_data[index];
		}
	};
}
 
#endif // THOR_STRING_H_

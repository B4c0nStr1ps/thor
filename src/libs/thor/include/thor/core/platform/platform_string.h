#ifndef THOR_PLATFORM_STRING_H_
#define THOR_PLATFORM_STRING_H_



#include "platform.h"
#include "platform_type_traits.h"
#include "../memory.h"
#include <stdlib.h>

namespace Thor
{
	namespace Platform
	{
		namespace String
		{

			int32 Strlen(const ANSICHAR* string);

			int32 Strlen(const WIDECHAR* string);

			template <typename SourceEncoding, typename DestEncoding>
			FORCEINLINE DestEncoding* ConvertBetweenEncoding(DestEncoding* destination, int32 destinationSize, const SourceEncoding* source, int32 sourceSize);

			template <>
			FORCEINLINE WIDECHAR* ConvertBetweenEncoding<ANSICHAR, WIDECHAR>(WIDECHAR* destination, int32 destinationSize, const ANSICHAR* source, int32 sourceSize)
			{
				size_t max_size = destinationSize * sizeof(WIDECHAR);
				size_t result = 0;
#if USE_SECURE_CRT
				mbstowcs_s(&result, destination, destinationSize, source, max_size);
#else
				result = mbstowcs(destination, source, max_size);
#endif // USE_SECURE_CRT
				return destination;
			}

			template <>
			FORCEINLINE ANSICHAR* ConvertBetweenEncoding<WIDECHAR, ANSICHAR>(ANSICHAR* destination, int32 destinationSize, const WIDECHAR* source, int32 sourceSize)
			{
				size_t max_size = destinationSize * sizeof(ANSICHAR);

				size_t result = 0;
#if USE_SECURE_CRT
				wcstombs_s(&result, destination, destinationSize, source, max_size);
#else
				result = wcstombs(destination, source, max_size);
#endif // USE_SECURE_CRT
				return destination;
			}

			template <typename SourceEncoding, typename DestEncoding>
			FORCEINLINE typename std::enable_if<TypeTraits::AreEncodingsCompatible<SourceEncoding, DestEncoding>::value, DestEncoding*>::type
				Convert(DestEncoding* destination, int32 destinationSize, const SourceEncoding* source, int32 sourceSize)
			{
				if (destinationSize < sourceSize)
				{
					return nullptr;
				}

				return (DestEncoding*)Memory::Memcpy(destination, source, sourceSize);
			}

			template <typename SourceEncoding, typename DestEncoding>
			FORCEINLINE typename std::enable_if<!TypeTraits::AreEncodingsCompatible<SourceEncoding, DestEncoding>::value, DestEncoding*>::type
				Convert(DestEncoding* destination, int32 destinationSize, const SourceEncoding* source, int32 sourceSize)
			{
				return ConvertBetweenEncoding(destination, destinationSize, source, sourceSize);
			}

			FORCEINLINE int32 StringFormatImpl(ANSICHAR* destination, size_t destinationSize, int32 maxCharactersCount, const ANSICHAR* format, va_list args)
			{
#if USE_SECURE_CRT
				int32 result = _vsnprintf_s(destination, destinationSize, maxCharactersCount, format, args);
#else
				int32 result = _vsnprintf(destination, maxCharactersCount, format, args);
#endif // USE_SECURE_CRT
				return result;
			}
		}// namespace String
	} // namespace Platform.
} // namespace Thor.

#endif // THOR_PLATFORM_STRING_H_

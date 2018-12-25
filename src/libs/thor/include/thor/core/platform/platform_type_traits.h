#ifndef THOR_PLATFORM_TYPE_TRAITS_H_
#define THOR_PLATFORM_TYPE_TRAITS_H_
 
#include "platform.h"
#include <type_traits>

namespace Thor
{
	namespace Platform
	{
		namespace TypeTraits
		{

			template<typename Type> struct IsCharType { enum { value = false }; };
			template<> struct IsCharType<ANSICHAR> { enum { value = true }; };
			template<> struct IsCharType<WIDECHAR> { enum { value = true }; };

			template<typename EncodingA, typename EncodingB>
			struct AreEncodingsCompatible
			{
				enum { value = IsCharType<EncodingA>::value && IsCharType<EncodingB>::value && sizeof(EncodingA) == sizeof(EncodingB) };
			};

		} // namespace TypeTraits.
	} // namespace Platform.
} // namespace Thor.
 
#endif // THOR_PLATFORM_TYPE_TRAITS_H_

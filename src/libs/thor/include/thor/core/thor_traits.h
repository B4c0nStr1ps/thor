#ifndef THOR_THOR_TRAITS_H_
#define THOR_THOR_TRAITS_H_

#include "core_types.h"
#include <type_traits>

namespace Thor
{
	namespace TypeTraits
	{
		template<typename Type> struct IsCharType { enum { value = false }; };

		template<typename EncodingA, typename EncodingB>
		struct AreEncodingsCompatible
		{
			enum { value = IsCharType<EncodingA>::value && IsCharType<EncodingB>::value && sizeof(EncodingA) == sizeof(EncodingB) };
		};
	}
} // namespace Thor
 
#endif // THOR_THOR_TRAITS_H_

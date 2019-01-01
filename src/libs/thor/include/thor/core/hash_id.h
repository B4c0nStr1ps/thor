#ifndef THOR_HASH_H_
#define THOR_HASH_H_

#include "platform/platform.h"

namespace Thor
{
	// FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
	// str should be a null terminated string literal, value should be left out 
	// e.g hash_32_fnv1a_const("example")
	// code license: public domain or equivalent
	// post: https://notes.underscorediscovery.com/constexpr-fnv1a/

	constexpr uint32 val_32_const = 0x811c9dc5;
	constexpr uint32 prime_32_const = 0x1000193;
	constexpr uint64 val_64_const = 0xcbf29ce484222325;
	constexpr uint64 prime_64_const = 0x100000001b3;

	inline constexpr uint32 HashFnv1_32(const char* const str, const uint32 value = val_32_const) noexcept {
		return (str[0] == '\0') ? value : HashFnv1_32(&str[1], (value ^ uint32(str[0])) * prime_32_const);
	}

	inline constexpr uint64 HashFnv1_64(const char* const str, const uint64 value = val_64_const) noexcept {
		return (str[0] == '\0') ? value : HashFnv1_64(&str[1], (value ^ uint64(str[0])) * prime_64_const);
	}

	typedef uint32 hid;

	inline constexpr hid HID(const ANSICHAR* const str) {
		return HashFnv1_32(str);
	}
}

#endif // THOR_HASH_H_

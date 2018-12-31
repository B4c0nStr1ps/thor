#include <thor/core/string_utils.h>
#include <thor/core/allocator.h>
#include <stdarg.h>

#define STARTING_BUFFER_SIZE		512
Thor::String Thor::StringUtils::FormatImplementation(const ANSICHAR* format, ...)
{
	using CharType = ANSICHAR;
	int32		bufferSize = STARTING_BUFFER_SIZE;
	CharType	startingBuffer[STARTING_BUFFER_SIZE];
	CharType*	buffer = startingBuffer;
	
	va_list args;
	va_start(args, format);
	//int32 result = _vsnprintf(buffer, bufferSize - 1, format, args);
	int32 result = Platform::String::StringFormatImpl(buffer, bufferSize, bufferSize - 1, format, args);
	va_end(args);

	if (result < 0)
	{
		//encoding error.
		return Thor::String();
	}
	
	Allocators::HeapAllocator allocator;

	if (result >= bufferSize)
	{
		//need to reallocate the buffer.
		buffer = nullptr;
		buffer = (CharType*)allocator.Allocate((result + 1) * sizeof(CharType), alignof(CharType));
		va_list args;
		va_start(args, format);
		//result = _vsnprintf(buffer, bufferSize - 1, format, args);
		result = Platform::String::StringFormatImpl(buffer, bufferSize, bufferSize - 1, format, args);
		va_end(args);
	}

	buffer[result] = 0;
	Thor::String formattedString(buffer);
	if (result >= bufferSize)
	{
		allocator.Free(buffer);
	}
	return formattedString;
}

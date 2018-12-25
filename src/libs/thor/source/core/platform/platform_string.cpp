#include <thor/core/platform/platform_string.h>
#include <string.h>

int32 Thor::Platform::String::Strlen(const ANSICHAR* string)
{
	return (int32)strlen(string);
}

int32 Thor::Platform::String::Strlen(const WIDECHAR* string)
{
	return (int32)wcslen(string);
}

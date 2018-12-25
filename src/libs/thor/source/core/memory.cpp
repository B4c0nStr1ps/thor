#include <thor/core/memory.h>

namespace Thor
{
	namespace Memory
	{
		void* Memcpy(void* destination, const void* source, size_t size)
		{
			T_ASSERT(destination != source);
			return memcpy(destination, source, size);
		}
		
		int TestAA()
		{
			return 101;
		}
	}
}
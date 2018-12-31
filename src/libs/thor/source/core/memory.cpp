#include <thor/core/memory.h>
#include <memory.h>

namespace Thor
{
	namespace Memory
	{
		size_t G_Malloc_Count = 0;

		void* Memcpy(void* destination, const void* source, size_t size)
		{
			T_ASSERT(destination != source);
			return memcpy(destination, source, size);
		}

		void* Malloc(size_t size, size_t alignment)
		{
			++G_Malloc_Count;
			return _aligned_malloc(size, alignment);;
		}

		void Free(void* source)
		{
			_aligned_free(source);
			--G_Malloc_Count;
		}

		size_t DebugGetMallocCount()
		{
			size_t value = G_Malloc_Count;
			return value;
		}
	}
}
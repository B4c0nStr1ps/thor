#include <gtest/gtest.h>
#include <thor/core/allocator.h>


struct TestFunctorProxyAllocator : public Thor::Allocators::DefaultAllocator
{
	using BaseAllocatorType = Thor::Allocators::DefaultAllocator;
	static int AllocationCounter;
	void* Allocate(size_t sizeBytes, size_t alignment = Thor::Allocators::K_DEFAULT_ALIGNMENT)
	{
		++AllocationCounter;
		return BaseAllocatorType::Allocate(sizeBytes, alignment);
	}

	void Free(void* address)
	{
		--AllocationCounter;
		BaseAllocatorType::Free(address);
	}

	template<typename ElementType, typename... Args>
	ElementType* New(Args&&... args)
	{
		return BaseAllocatorType::New(std::forward<Args>(args)...);
	}

	template<typename ElementType>
	void Delete(ElementType* address)
	{
		BaseAllocatorType::Delete(address);
	}
};

int TestFunctorProxyAllocator::AllocationCounter = 0;

#define USE_USER_FUNCTOR_ALLOCATOR
#define FUNCTOR_DEFAULT_ALLOCATOR TestFunctorProxyAllocator


#include <thor/core/functional.h>

struct StaticStats
{
	static int counter;
};

int StaticStats::counter = 0;

void TestFunc1()
{
	++StaticStats::counter;
}

TEST(TestsFunctors, InplaceFunctor) {
	{
		Thor::Function<void()> f1 = TestFunc1;
		f1();
		f1();
		EXPECT_EQ(StaticStats::counter, 2);
		Thor::Function<void()> f2 = []() { ++StaticStats::counter; };
		f2();
		f2();
		EXPECT_EQ(StaticStats::counter, 4);
		Thor::Function<int(int, int)> f3 = [](int a, int b) {return a + b; };
		int res = f3(100, 1);
		EXPECT_EQ(res, 101);
		StaticStats::counter = 0;
		Thor::Function<void()> f5 = TestFunc1;
		Thor::Function<void()> f4(f5);
		f4();
		f4();
		f4();
		EXPECT_EQ(StaticStats::counter, 3);
	}
	EXPECT_EQ(TestFunctorProxyAllocator::AllocationCounter, 0);
}

TEST(TestsFunctors, HeapFunctor) {
	{
		double d1 = 1.0;
		double d2 = 2.0;
		double d3 = 3.0;
		Thor::Function<double()> f1 = [d1, d2, d3]() { return d1 + d2 + d3; };
		double result = f1();
		EXPECT_EQ(result, 6.0);
	}
	EXPECT_EQ(TestFunctorProxyAllocator::AllocationCounter, 0);
}

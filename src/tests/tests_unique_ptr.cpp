#include <gtest/gtest.h>
#include <thor/core/smart_pointers.h>

struct MockUniquePtr
{
	int a;
	int b;
	MockUniquePtr(int pA, int pB)
		:a(pA), b(pB)
	{}
};

TEST(TestsUniquePtrs, Ctors) {
	auto mptr = Thor::MakeUnique<MockUniquePtr>(101, 202);
	EXPECT_EQ(mptr->a, 101);
	EXPECT_EQ(mptr->b, 202);
}
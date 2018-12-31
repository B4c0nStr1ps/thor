#include <gtest/gtest.h>
#include <thor/core/shared_ref.h>

struct SharedRefTester
{
	bool* controller;
	explicit SharedRefTester(bool* statusController)
		:controller(statusController)
	{
		*controller = true;
	}
	~SharedRefTester()
	{
		*controller = false;
	}
};

struct SharedRefTesterB
{
public:
	int instanceValue;
	static bool Constructed;
	SharedRefTesterB()
	{
		instanceValue = 303;
		Constructed = true;
	}
	SharedRefTesterB(int value)
	{
		instanceValue = value;
		Constructed = true;
	}
	~SharedRefTesterB()
	{
		Constructed = false;
	}
};

bool SharedRefTesterB::Constructed = false;

TEST(TestsSharedRefs, Ctors) {
	
	int* value = new int(101);

	Thor::SharedRef<int> intRef(value);
	EXPECT_EQ(intRef.UseCount(), 1);
	Thor::SharedRef<int> intRef2(intRef);
	EXPECT_EQ(intRef.UseCount(), 2);
	EXPECT_EQ(intRef2.UseCount(), 2);
	EXPECT_EQ(*intRef, 101);	
	Thor::SharedRef<int> intRef3(new int(202));
	EXPECT_EQ(intRef3.UseCount(), 1);
}

TEST(TestsSharedRefs, Destructor) {
	bool statusCheck = false;

	SharedRefTester* raw = new SharedRefTester(&statusCheck);

	EXPECT_EQ(statusCheck, true);
	{
		Thor::SharedRef<SharedRefTester> ref1(raw);
		EXPECT_EQ(statusCheck, true);
		Thor::SharedRef<SharedRefTester> ref2(ref1);
		EXPECT_EQ(statusCheck, true);
	}
	EXPECT_EQ(statusCheck, false);
}

TEST(TestsSharedRefs, MakeShared) {
	{
		{
			auto ref1 = Thor::MakeShared<SharedRefTesterB>();
			EXPECT_EQ(ref1.UseCount(), 1);
			EXPECT_EQ(ref1->instanceValue, 303);
			EXPECT_EQ(SharedRefTesterB::Constructed, true);
			auto ref2 = Thor::MakeShared<SharedRefTesterB>(505);
			EXPECT_EQ(ref1.UseCount(), 1);
			EXPECT_EQ(ref2.UseCount(), 1);
			EXPECT_EQ(ref2->instanceValue, 505);
		}
		EXPECT_EQ(SharedRefTesterB::Constructed, false);
	}
}

TEST(TestsSharedRefs, Operators) {
	{
		auto ref1 = Thor::MakeShared<SharedRefTesterB>(505);
		Thor::SharedRef<SharedRefTesterB> ref2(ref1);

		EXPECT_EQ(ref1, ref2);
		EXPECT_EQ(ref1->instanceValue, 505);
		EXPECT_EQ(ref2->instanceValue, 505);
		EXPECT_EQ((*ref1).instanceValue, 505);
		EXPECT_EQ((*ref2).instanceValue, 505);

		ref1->instanceValue = 707;

		EXPECT_EQ(ref1, ref2);
		EXPECT_EQ(ref1->instanceValue, 707);
		EXPECT_EQ(ref2->instanceValue, 707);
		EXPECT_EQ((*ref1).instanceValue, 707);
		EXPECT_EQ((*ref2).instanceValue, 707);
	}
}

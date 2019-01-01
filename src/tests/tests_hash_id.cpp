#include <gtest/gtest.h>
#include <thor/core/hash_id.h>

TEST(TestsHashIds, ConstexprFactory) {
	constexpr Thor::hid id1 = Thor::HID("TestName");
	Thor::hid id2 = Thor::HID("TestName");
	EXPECT_EQ(id1, id2);
	EXPECT_EQ(id1, 1743723078);
}
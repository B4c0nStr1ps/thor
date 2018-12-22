#include <gtest/gtest.h>
#include <thor/core/array_list.h>

TEST(TestsArrayList, Ctor) {
	Thor::ArrayList<int> collection;
	int length = collection.Length();
	int capacity = collection.Capacity();
	EXPECT_EQ(length, 0);
	EXPECT_EQ(capacity, 0);
	EXPECT_EQ(collection.Data(), nullptr);
}

TEST(TestsArrayList, ReserveCtor) {
	Thor::ArrayList<int> collection(10);
	int length = collection.Length();
	int capacity = collection.Capacity();
	EXPECT_EQ(length, 0);
	EXPECT_EQ(capacity, 10);
}

TEST(TestsArrayList, CopyCtor) {
	Thor::ArrayList<int> collection1(4);
	collection1.Add(101);
	collection1.Add(202);
	collection1.Add(303);
	collection1.Add(404);

	Thor::ArrayList<int> collection2(collection1);
	EXPECT_EQ(collection1.Capacity(), collection2.Capacity());
	EXPECT_EQ(collection1.Length(), collection2.Length());
	EXPECT_EQ(collection1[0], collection2[0]);
	EXPECT_EQ(collection1[1], collection2[1]);
	EXPECT_EQ(collection1[2], collection2[2]);
	EXPECT_EQ(collection1[3], collection2[3]);
}

TEST(TestsArrayList, MoveCtor) {
	Thor::ArrayList<int> collection1(4);
	collection1.Add(101);
	collection1.Add(202);
	collection1.Add(303);
	collection1.Add(404);

	Thor::ArrayList<int> collection2(std::move(collection1));
	EXPECT_EQ(collection2.Capacity(), 4);
	EXPECT_EQ(collection2.Length(), 4);
	EXPECT_EQ(collection2[0], 101);
	EXPECT_EQ(collection2[1], 202);
	EXPECT_EQ(collection2[2], 303);
	EXPECT_EQ(collection2[3], 404);
	EXPECT_TRUE(collection1.Data() == nullptr);
}

TEST(TestsArrayList, Operations) {
	Thor::ArrayList<int> collection(10);
	collection.Add(101);
	collection.Add(303);
	EXPECT_EQ(collection[0], 101);
	EXPECT_EQ(collection[1], 303);
	EXPECT_EQ(collection.Capacity(), 10);
	EXPECT_EQ(collection.Length(), 2);

	Thor::ArrayList<int> collection1(10);
	for (int i = 0; i < 10; ++i)
	{
		collection1.Add((100 * (i + 1)) + i + 1);
	}
	EXPECT_EQ(collection1.Capacity(), 10);
	EXPECT_EQ(collection1.Length(), 10);
	collection1.RemoveAt(3);
	int compareValue = (100 * (9 + 1)) + 9 + 1;
	EXPECT_EQ(collection1[3], compareValue);
	EXPECT_EQ(collection1.Capacity(), 10);
	EXPECT_EQ(collection1.Length(), 9);
}
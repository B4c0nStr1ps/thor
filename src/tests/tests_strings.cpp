#include <gtest/gtest.h>
#include <thor/core/string.h>
#include <thor/core/string_utils.h>
#include <cstring>

TEST(TestsStrings, Ctors) {
	Thor::String myString("Hello String");
	EXPECT_EQ(strcmp("Hello String", myString.RawData()), 0);
	Thor::String myString2 = "Hello String";
	EXPECT_EQ(strcmp("Hello String", myString2.RawData()), 0);
	Thor::String myWString(L"Hello String");
	EXPECT_EQ(strcmp(myWString.RawData(), myString.RawData()), 0);
	Thor::String myWString2 = L"Hello WString2";
	EXPECT_EQ(strcmp(myWString2.RawData(), "Hello WString2"), 0);
}

TEST(TestsStrings, Operations) {
	Thor::String myString1 = "QWERT YUIOP"; //11 chars plus null terminator = 12 chars.
	auto length1 = myString1.Length();
	EXPECT_EQ(length1, 12);

	Thor::String myString2("QWERT YUIOP"); //11 chars plus null terminator = 12 chars.
	auto length2 = myString2.Length();
	EXPECT_EQ(length2, 12);
}

TEST(TestsStrings, StringFormat) {
	Thor::String myString = Thor::StringUtils::Format("Hello String %d -> %s", 101, "TEST!");
	EXPECT_EQ(strcmp(myString.RawData(), "Hello String 101 -> TEST!"), 0);
}
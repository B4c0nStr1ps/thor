#include <thor/core/array_list.h>
#include <iostream>
#include <vector>


struct	MyStruct
{
	int value;
	MyStruct()
		:value{101}
	{
		std::cout << "MyStruct()" << std::endl;
	}

	MyStruct(int newvalue)
		:value{ newvalue }
	{
		std::cout << "MyStruct(int newvalue)" << std::endl;
	}

	MyStruct(const MyStruct& other)
		:value {other.value } 
	{
		std::cout << "MyStruct(const MyStruct& other)" << std::endl;
	}

	MyStruct(MyStruct&& other)
		:value(std::move(other.value))
	{
		std::cout << "MyStruct(MyStruct&& other)" << std::endl;
	}
};

int main()
{
	Thor::ArrayList<int> collection(10);

	int length = collection.Length();
	int capacity = collection.Capacity();
	std::cout << "Current capacity: " << capacity << std::endl;
	std::cout << "Current length: " << length << std::endl;

	collection.Add(11);
	collection.Add(22);
	collection.Add(33);
	collection.Add(44);
	collection.Add(55);
	collection.Add(66);
	collection.Add(77);
	collection.Add(88);
	collection.Add(99);
	collection.Add(100);

	length = collection.Length();
	capacity = collection.Capacity();
	std::cout << "Current capacity: " << capacity << std::endl;
	std::cout << "Current length: " << length << std::endl;

	collection.Add(200);
	collection.Add(300);
	collection.Add(400);
	collection.Add(500);

	length = collection.Length();
	capacity = collection.Capacity();
	std::cout << "Current capacity: " << capacity << std::endl;
	std::cout << "Current length: " << length << std::endl;

	std::cout << std::endl << std::endl;

	for (int i = 0; i < collection.Length(); ++i)
	{
		std::cout << "collection[" << i << "] -> " << collection[i] << std::endl;
	}

	return 0;
}
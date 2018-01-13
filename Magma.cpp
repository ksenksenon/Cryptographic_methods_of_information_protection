#include <iostream>
#include <vector>
#include <Windows.h>

void partition(uint64_t plain, uint32_t left, uint32_t right)
{
	left = plain >> 32;
	right = plain & 0xFFFFFFFF;
	std::cout << "a = " << std::hex << left << std::endl;  
	std::cout << "b = " << std::hex << right << std::endl;  
}


int main()
{
	uint64_t plaintext = 0x9531240265412210;
	
	unsigned long key[8] =
	{
		0x0123,
		0x4567,
		0x89AB,
		0xCDEF,
		0x0123,
		0x4567,
		0x89AB,
		0xCDEF
	};
	std::cout << "plain = " << std::hex << plaintext << std::endl;  
	uint32_t left = 0, right = 0;
	partition(plaintext, left, right);
	system("pause");
}


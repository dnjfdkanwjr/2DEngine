#include "assert.h"


void assert_if_false(bool condition)
{
	if (true == condition)
	{
		return;
	}

	int* ptr = nullptr;
	*ptr = 0xDEADBEEF;
}

void crash()
{
	int* ptr = nullptr;
	*ptr = 0xDEADBEEF;
}
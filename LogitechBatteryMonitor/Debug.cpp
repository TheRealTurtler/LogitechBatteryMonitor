#include "Debug.h"

#include <stdio.h>

void Debug::printHexBuffer(const unsigned char* buffer, size_t bufferSize)
{
	for (size_t idx = 0; idx < bufferSize; ++idx)
	{
		printf("0x%02X ", buffer[idx]);
	}

	printf("\n");
}

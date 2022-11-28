#pragma once
class Debug
{
public:
	Debug() = delete;

	static void printHexBuffer(const unsigned char* buffer, size_t bufferSize);
};


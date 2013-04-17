#pragma once

#include <fstream>

class File
{
private:
public:
	static void ReadText(const char* filePath, char*& bufferPointer, unsigned int& bufferCapacity)
	{
		std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
		
		if( !file.is_open() ){
			throw "File: is_open failed";
		}
		
		bufferCapacity = (unsigned int)file.tellg();
		bufferPointer = new char[bufferCapacity+1];
		file.seekg(0, std::ios::beg);
		file.read(bufferPointer, bufferCapacity);
		file.close();
		bufferPointer[bufferCapacity] = 0;
	}
};
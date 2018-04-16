#include <fstream>
#include <iostream>

std::string ReadShaderCode(const char* fileName)
{
	std::ifstream input(fileName);
	if(!input.good())
	{
		std::cout << "file failed to load... " << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(input), 
		std::istreambuf_iterator<char>()
	);
}
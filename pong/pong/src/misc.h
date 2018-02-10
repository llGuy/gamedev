#ifndef MISC_HEADER
#define MISC_HEADER

#include <iostream>

template<typename P>
void Log(P param)
{
	std::cout << "LOG> " << param << std::endl;
}
template<typename P1, typename ... Args>
void Log(P1 param1, Args ... otherArgs)
{
	std::cout << "LOG> " << param1 << std::endl;
	Log(otherArgs...);
}

#endif
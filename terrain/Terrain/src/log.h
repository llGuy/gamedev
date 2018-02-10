#ifndef LOG_HEADER
#define LOG_HEADER

#include <iostream>

template<typename T>
void Log(T t)
{
	std::cout << "LOG> " << t << std::endl;
}

template<typename T, typename... Args>
void Log(T t, Args... other)
{
	std::cout << "LOG> " << t << std::endl;
	Log(other...);
}

template<typename T>
void Error(T t)
{
	std::cout << "ERROR> " << t << std::endl;
}

template<typename T, typename... Args>
void Error(T t, Args... other)
{
	std::cout << "ERROR> " << t << std::endl;
	Error(other...);
}

#endif
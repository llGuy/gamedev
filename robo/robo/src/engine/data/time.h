#ifndef TIME_HEADER
#define TIME_HEADER

#include <chrono>

struct Time
{
	std::chrono::time_point<std::chrono::high_resolution_clock> beginning;
	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> previousTime;
	double deltaT;
};

#endif
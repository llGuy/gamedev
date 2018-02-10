#ifndef TIME_DATA_HEADER
#define TIME_DATA_HEADER

#include <chrono>

namespace minecraft
{
	namespace data
	{
		struct Time
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> beginning;
			std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
			std::chrono::time_point<std::chrono::high_resolution_clock> previousTime;
			double deltaT;
		};
	}
}

#endif
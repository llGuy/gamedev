#ifndef LOG_HEADER
#define LOG_HEADER

#include <iostream>
#include <chrono>
#include <thread>

template<typename P>
void Log(P param)
{
	std::cout << "LOG> " << param << std::endl;
}

template<typename P1, typename... Args>
void Log(P1 param1, Args... otherArgs)
{
	std::cout << "LOG> " << param1 << std::endl;

	Log(otherArgs...);
}

void Greeting(void)
{
	std::cout << "Welcome to Snake in 3D !!!\n";
	std::cout << "Controls : ";
	std::cout <<            "D to move snake to the right\n";
	std::cout << "           A to move snake to the left\n";
	std::cout << "           R to move the snake up\n";
	std::cout << "           F to move the snake down\n";
	std::cout << "           UP to zoom in to the scene\n";
	std::cout << "           DOWN to zoom out\n";
	std::cout << "           SPACE to toggle from snake view to world view\n\n";

	std::cout << "When ready, press any key to start... ";
	std::cin.get();
	
	std::chrono::duration<unsigned short> oneSecond(1);
	for (unsigned short time = 3; time > 0; --time)
	{
		std::cout << time << std::endl;
		std::this_thread::sleep_for(oneSecond);
	}
	std::cout << std::endl;
}

#endif
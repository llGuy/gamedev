#include "timer.h"

auto timer::start(void) -> void
{
	current_time = std::chrono::high_resolution_clock::now();
}
auto timer::reset(void) -> void
{
	current_time = std::chrono::high_resolution_clock::now();
}
auto timer::elapsed(void) -> float
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> seconds = now - current_time;
	float count = seconds.count();
	return count;
}

auto timer::accumulate(float factor) -> void
{
	accum += elapsed() * factor;
}

auto timer::accumulated(void) -> float &
{
	return accum;
}
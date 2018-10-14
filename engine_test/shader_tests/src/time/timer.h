#pragma once

#include <chrono>

class time_handler
{
public:
	time_handler(void) = default;
	auto start(void) -> void;
	auto reset(void) -> void;
	auto elapsed(void) -> float;
private:
	std::chrono::high_resolution_clock::time_point current_time;
};
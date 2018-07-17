#include "application.h"
#include <string>
#include <iostream>
#include <iterator>
#include <ctime>

application::application(int32_t w, int32_t h)
{
	srand(static_cast<uint32_t>(time(NULL)));
}

auto application::init(void) -> void
{
	grid.generate();
}

auto application::running(void) -> bool
{
	return true;
}

auto application::update(void) -> void
{
	std::cin.get();
	grid.update();
	grid.print();
}
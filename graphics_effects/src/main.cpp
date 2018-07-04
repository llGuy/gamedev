#include <iostream>
#include "application.h"

auto main(int32_t argc, char* argv[]) -> int32_t
{
	application app { 1400, 800 };

	glfw_init();
	app.init_window();
	glew_init();

	app.init();

	while (app.running())
	{
		app.render();
		app.update();
	}

	app.destroy();
	glfw_terminate();

	std::cin.get();
	return 0;
}
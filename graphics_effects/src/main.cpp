#include <iostream>
#include "application.h"
#include "log.h"

auto main(int32_t argc, char* argv[]) -> int32_t
{
	application app { 2500, 1800 };

	logger::new_log("initializing game");
	glfw_init();
	app.init_window();
	glew_init();

	app.init();

	logger::new_log("running game");
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

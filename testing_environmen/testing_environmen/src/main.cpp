#include "application.h"

auto main(i32 argc, char * argv[]) -> i32
{
	application app { 1440, 850 };
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
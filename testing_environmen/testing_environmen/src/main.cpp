#include "application.h"

auto main(i32 argc, char * argv[]) -> i32
{
	application app { (i32)(1500), (i32)(800) };

	app.set_max_framerate(80.0f);

	glfw_init();
	app.init_window();
	glew_init();
	app.init();

	while (app.is_running())
	{
		app.render();
		app.update();
	}

	app.destroy();
	glfw_terminate();

	std::cin.get();

	return 0;
}
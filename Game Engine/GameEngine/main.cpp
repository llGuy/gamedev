#include <iostream>
#include "src/app/application.h"
#include <glm/gtc/quaternion.hpp>
#include "src/console/console.h"

auto main(void) -> int32_t
{
	application app;
	app.init();

	while (app.running())
	{
		app.update();
		app.render();
	}
	app.clean_up();

	std::cin.get();

	return 0;
}
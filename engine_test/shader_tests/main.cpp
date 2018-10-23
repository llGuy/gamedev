#include <fstream>
#include <iostream>
#include "src/api/api.h"
#include "src/win/window.h"
#include "src/utils/types.h"
#include "src/graphics/graphics.h"

#include "src/app/application.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "src/scene/scene.h"
#include "src/scene/component_log.h"

auto main(i32 argc, char * argv[]) -> i32
{
	application app;

	app.init();

	while (app.running())
	{
		app.update();
		app.render();
	}

	return 0;
}
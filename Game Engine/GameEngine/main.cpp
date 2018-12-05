#include <iostream>
#include "src/app/application.h"
#include <glm/gtc/quaternion.hpp>
#include "src/console/console.h"

auto test(void) -> void
{
	ENG_INFO("Running Test");

	glm::vec3 view_position(0, -1, -2);
	glm::vec3 view_normal(0, 1, 0);

	glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f), 1500.0f / 800.0f, 0.1f, 1000.0f);

	glm::vec3 ray_direction = glm::reflect(glm::normalize(view_position), glm::normalize(view_normal));

	for (u32 i = 0; i < 10; ++i)
	{
		view_position += ray_direction * 0.1f;

		glm::vec4 projected = projection_matrix * glm::vec4(view_position, 1.0f);
		projected.x /= projected.w;
		projected.y /= projected.w;
		projected.x = projected.x * 0.5 + 0.5;
		projected.y = projected.y * 0.5 + 0.5;

		int x = 1;
	}
}

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

	test();

	std::cin.get();

	return 0;
}
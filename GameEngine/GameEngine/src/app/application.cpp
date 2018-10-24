#include <iostream>
#include "application.h"
#include "../xcp/exception.h"

#define DISPLAY_WIDTH 1500
#define DISPLAY_HEIGHT 800

application::application(void)
	: display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Game Engine")
{
}

auto application::init(void) -> void
{
	try
	{
		glfw_init();

		display.init(false);
		display.launch_input_handler();

		glew_init();

		init_game_objects();

		time_handler.start();
		is_running = true;
	}
	catch(xcp::gl_xcp const & exception)
	{
		glfw_terminate();
		display.destroy();
		std::cerr << exception.what();

		std::cin.get();
	}
}

auto application::update(void) -> void
{
	display.refresh();


	world.update(time_handler.elapsed());
	time_handler.reset();


	is_running = display.is_open();
}

auto application::render(void) -> void
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
}

auto application::running(void) -> bool
{
	return is_running;
}

auto application::clean_up(void) -> void
{
	display.destroy();
	glfw_terminate();
}

auto application::init_game_objects(void) -> void
{
	world.init();

	game_object & monkey = world.init_game_object({ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), "monkey" });

	component<component_log, game_object_data> log_comp;
	component<component_behavior_key, game_object_data> key_comp{ 
		key_bind{ GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT }, display.user_inputs() };
	monkey.add_component<component_log>(log_comp);
	monkey.add_component<component_behavior_key>(key_comp);
}
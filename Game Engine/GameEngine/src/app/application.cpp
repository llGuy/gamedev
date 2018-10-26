#include <iostream>
#include "../io/io.h"
#include "application.h"
#include "../xcp/exception.h"

#include <glm/gtc/type_ptr.hpp>

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
		display.set_window_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glew_init();

		init_game_objects();
		init_3D_test();

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

	auto camera = world.get_scene_camera();
	low_poly_shader->bind();
	low_poly_shader->send_uniform_vec3("camera_position", glm::value_ptr(camera.get_position()), 1);
	low_poly_shader->send_uniform_mat4("view_matrix", glm::value_ptr(camera.get_view_matrix()), 1);

	time_handler.reset();


	is_running = display.is_open();
}

auto application::render(void) -> void
{
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.9f, 1.0f, 1.0f);

	renderer.render();

	/*low_poly_shader->bind();
	low_poly_shader->send_uniform_mat4("model_matrix", glm::value_ptr(detail::identity_matrix), 1);
	low_poly_texture->bind(GL_TEXTURE_2D, 0);

	monkey_model->vao.bind();
	auto ibuffer = monkey_model.get_component<index_buffer_component>();
	ibuffer->value.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElements(GL_TRIANGLES, monkey_model->count, GL_UNSIGNED_INT, nullptr);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();*/
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

	game_object & player = world.init_game_object({ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), "player" });

	component<component_behavior_key, game_object_data> key_comp{ 
		key_bind{ GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT }, display.user_inputs() };

	component<component_behavior_mouse, game_object_data> mouse_comp{ display.user_inputs() };

	player.add_component(key_comp);
	player.add_component(mouse_comp);

	world.bind_camera_to_object(player);
}

auto application::init_3D_test(void) -> void
{
	shaders.init();

	models.init();

	monkey_model = models.init_model();
	shader_handle shader = models.load_model_from_obj("res/model/monkey.obj", monkey_model);
	shader.set(shader_property::linked_to_gsh, shader_property::sharp_normals);

	shader.set_name("shader.low_poly");
	low_poly_shader = shaders.create_program(shader, "3D");

	low_poly_texture = textures.init_texture("texture.low_poly");
	textures.load_texture_png("res/textures/low_poly.png", low_poly_texture);

	/* just initializing uniform variables */
	glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f), 
		(f32)display.pixel_width() / display.pixel_height(), 0.1f, 100000.0f);

	material_light_info light{ glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 20.0f, 0.0f };
	material_prototype monkey_skin{ light, low_poly_shader, lights, low_poly_texture };
	renderer.set_material_prototype(monkey_skin);

	material monkey1{ monkey_model, detail::identity_matrix };
	renderer.submit_material(monkey1);
	renderer.set_projection(projection_matrix);
}
#include <iostream>
#include "../io/io.h"
#include "application.h"
#include "../xcp/exception.h"
#include "../graphics/3D/model_comp/cube.h"

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
		init_textures();
		init_models();
		init_shaders();
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

	renderer.flush();

	world.update(time_handler.elapsed());

	auto camera = world.get_scene_camera();
	glm::mat4 view_matrix = camera.get_view_matrix();

	glsl_program * low_poly_shader = renderer.get_shader();
	low_poly_shader->bind();
	low_poly_shader->send_uniform_vec3("camera_position", glm::value_ptr(camera.get_position()), 1);
	low_poly_shader->send_uniform_mat4("view_matrix", glm::value_ptr(view_matrix), 1);



	glsl_program * sky_shader = sky_renderer.get_shader();
	view_matrix[3][0] = 0.0f;
	view_matrix[3][1] = 0.0f;
	view_matrix[3][2] = 0.0f;
	sky_shader->bind();
	sky_shader->send_uniform_mat4("view_matrix", glm::value_ptr(view_matrix), 1);



	time_handler.reset();


	is_running = display.is_open();
}

auto application::render(void) -> void
{
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.9f, 1.0f, 1.0f);

	renderer.render();

	sky_renderer.render();
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

	game_object & player = world.init_game_object({ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), "game_object.player" });
	component<component_behavior_key, game_object_data> key_comp{
		key_bind{ GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT }, display.user_inputs() };
	component<component_behavior_mouse, game_object_data> mouse_comp{ display.user_inputs() };
	player.add_component(key_comp);
	player.add_component(mouse_comp);

	world.bind_camera_to_object(player);

	game_object & monkey = world.init_game_object({ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(4.0f), "game_object.monkey" });
	component<component_model_matrix, game_object_data> model_matrix_comp;
	monkey.add_component(model_matrix_comp);
}

auto application::init_models(void) -> void
{
	models.init();

	monkey_model = models.init_model();
	models.load_model_from_obj("res/model/monkey.obj", monkey_model);

	cube_model_computation comp;
	cube_model = models.init_model();
	models.compute_model(comp, cube_model);
}

auto application::init_3D_test(void) -> void
{
	/* initializing normal 3D renderer */
	/* just initializing uniform variables */
	glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f), 
		(f32)display.pixel_width() / display.pixel_height(), 0.1f, 100000.0f);

	material_light_info light{ glm::vec3(1.0f), glm::vec3(0.7f), glm::vec3(0.5f), 20.0f, 1.0f };
	material_prototype monkey_skin{ light, shaders[shader_handle("shader.low_poly")], lights };
	monkey_skin.get_textures_2D().push_back(low_poly_texture);
	monkey_skin.get_textures_cubemap().push_back(sky_texture);
	renderer.set_material_prototype(monkey_skin);

	game_object & monkey = world.get_game_object("game_object.monkey");
	component<component_render, game_object_data> monkey_render_compnonent{ monkey_model, renderer };
	monkey.add_component(monkey_render_compnonent);

	renderer.set_projection(projection_matrix);
	


	/* initializing sky renderer */
	material_prototype sky_material{ material_light_info(), shaders[shader_handle("shader.sky")], lights };
	sky_material.get_textures_cubemap().push_back(sky_texture);
	sky_material.toggle_lighting();
	sky_renderer.set_material_prototype(sky_material);

	material sky{ cube_model, glm::scale(glm::vec3(1000.0f)) };
	sky_renderer.submit_material(sky);
	sky_renderer.set_projection(projection_matrix);
}

auto application::init_shaders(void) -> void
{
	shaders.init();

	shader_handle shader = models.create_shader_handle(monkey_model);
	shader.set(shader_property::linked_to_gsh, shader_property::sharp_normals);
	shader.set_name("shader.low_poly");
	shaders.create_program(shader, "3D");

	shader_handle sky_shader_handle("shader.sky");
	glsl_shader sky_vsh = shaders.create_shader(GL_VERTEX_SHADER, sky_shader_handle, extract_file("src/shaders/environment/vsh.shader"));
	glsl_shader sky_fsh = shaders.create_shader(GL_FRAGMENT_SHADER, sky_shader_handle, extract_file("src/shaders/environment/fsh.shader"));
	shaders.combine(sky_shader_handle, sky_vsh, sky_fsh);
}

auto application::init_textures(void) -> void
{
	low_poly_texture = textures.init_texture("texture.low_poly");
	textures.load_texture_png("res/textures/low_poly.png", low_poly_texture);

	sky_texture = textures.init_texture("texture.sky");
	textures.load_3D_texture_png("res/textures/sky", sky_texture);
}
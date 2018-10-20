#include <GL/glew.h>

#include <fstream>
#include "../io/io.h"
#include "application.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

application::application(void)
	: display(1000, 600, "Game Engine")
{
}

auto application::init(void) -> void
{
	is_running = false;

	try
	{
		glfw_init();

		display.init(false);
		display.launch_input_handler();

		display.set_window_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glew_init();

		glEnable(GL_DEPTH_TEST);

		init_data();
		init_textures();
		init_meshes();
		init_shaders();
		init_layers();

		is_running = true;

		timer.start();
	}
	catch (xcp::gl_xcp const & exception)
	{
		glfw_terminate();
		display.destroy();
		std::cerr << exception.what();

		std::cin.get();
	}
}

auto application::render(void) -> void
{
	main_target.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	main_target.clear_color(0.1f, 0.1f, 0.1f, 1.0f);
	main_target.render(meshes, shaders);
}
 
auto application::update(void) -> void
{
	display.refresh();

	entities.update(timer.elapsed());
	timer.reset();

	meshes.flush_renderers();

	main_target[0][0]->submit(detail::identity_matrix);
	main_target[1][0]->submit(world.env.calculate_translation(entities.get_camera().pos()));

	/* update view matrix of the camera */
	glm::mat4 view_matrix = entities.get_camera().matrix();
	main_target[0].get_view_matrix() = view_matrix;
	main_target[1].get_view_matrix() = world.env.calculate_view_matrix(view_matrix);

	is_running = display.is_open();

	unbind_all_textures(GL_TEXTURE_2D);
}

auto application::running(void) -> bool
{
	return is_running;
}

auto application::init_meshes(void) -> void
{
	meshes.init();

	/* process to creating a mesh : 
	-  create the handle for the mesh e.g. name ...
	-  load the model which will return a "shader handle" 
	-  set the name for the shader that you want to use for the handle 
	-  create the program with the handle (which will have all the properties selected) */

	u32 icosphere_mesh_id = meshes.create_mesh("icosphere");
	/* loads contents of ico.obj to the mesh "icosphere" */
	meshes.load_mesh("res/models/ico.obj", icosphere_mesh_id);

	u32 cube_mesh_id = meshes.create_mesh("sky box");
	cube_mesh_computation computation;
	meshes.compute_mesh(computation, cube_mesh_id);
}

auto application::init_textures(void) -> void
{
	auto image = extract_png("res/textures/low_poly.png");

	u32 low_poly_id = textures.create_texture("low poly");
	auto & low_poly_colors = textures[low_poly_id];

	create_color_texture(low_poly_colors, image.w, image.h, image.data);
}

auto application::init_data(void) -> void
{
	entities.create(display.user_inputs());

	world.env.init_texture(textures, "res/textures/environment/sky");
}

auto application::init_shaders(void) -> void
{
	shaders.init();

	shader_handle low_poly_shader = meshes.create_shader_handle(meshes.get_mesh_id("icosphere"));
	low_poly_shader.set(shader_property::linked_to_gsh, shader_property::sharp_normals);
	low_poly_shader.set_name("icosphere shader");

	shaders.create_program(low_poly_shader, "3D");



	auto & program = shaders[shader_handle("icosphere shader")];
	program.bind();
	/* send light information to shader */
	lights.prepare_shader(program);

	world.env.init_shader(shaders);
}

auto application::init_layers(void) -> void
{
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (f32)display.pixel_width() / display.pixel_height(), 0.1f, 100000.0f);



	u32 icosphere_mesh_id = meshes.get_mesh_id("icosphere");
	auto renderer = meshes.create_renderer<basic_renderer>(icosphere_mesh_id);
	renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_2D, 0, "low poly"));
	renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_CUBE_MAP, 0, "environment"));
	renderer->submit_pre_render(new material(glm::vec3(1.0f), glm::vec3(0.7f), glm::vec3(0.5f), 4.0f));
	renderer->submit_pre_render(&entities.get_pre_render_cam_pos(), false);



	u32 sky_box_mesh_id = meshes.get_mesh_id("sky box");
	auto sky_renderer = meshes.create_renderer<basic_renderer>(sky_box_mesh_id);
	sky_renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_CUBE_MAP, 0, "environment"));



	layer main_layer;
	main_layer.submit_renderer(renderer);
	main_layer.submit_shader(shader_handle("icosphere shader"));
	main_layer.get_projection_matrix() = projection;

	layer sky_layer;
	shader_handle sky_shader_handle{ "environment shader" };
	sky_layer.submit_renderer(sky_renderer);
	sky_layer.submit_shader(sky_shader_handle);
	sky_layer.get_projection_matrix() = projection;

	main_target.add_layer("main layer", main_layer);
	main_target.add_layer("sky layer", sky_layer);
}

auto application::clean_up(void) -> void
{
}
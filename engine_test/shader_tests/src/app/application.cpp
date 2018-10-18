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
	/* update view matrix of the camera */
	main_target[0].get_view_matrix() = entities.get_camera().matrix();

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

	u32 mesh_id = meshes.create_mesh("icosphere");
	/* loads contents of ico.obj to the mesh "icosphere" */
	meshes.load_mesh("res/models/ico.obj", mesh_id);
}

auto application::init_textures(void) -> void
{
	auto image = extract_png("res/textures/low_poly.png");

	u32 low_poly_id = textures.create_texture("low poly");
	auto & low_poly_colors = textures[low_poly_id];

	low_poly_colors.create();
	low_poly_colors.bind(GL_TEXTURE_2D);
	low_poly_colors.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, image.data, image.w, image.h);

	low_poly_colors.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	low_poly_colors.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

auto application::init_data(void) -> void
{
	entities.create(display.user_inputs());
}

auto application::init_shaders(void) -> void
{
	shaders.init();

	shader_handle shader = meshes.create_shader_handle(meshes.get_mesh_id("icosphere"));
	shader.set(shader_property::linked_to_gsh, shader_property::sharp_normals);
	shader.set_name("icosphere shader");

	shaders.create_program(shader, "3D");



	auto & program = shaders[shader_handle("icosphere shader")];
	program.bind();
	/* send light information to shader */
	lights.prepare_shader(program);
}

auto application::init_layers(void) -> void
{
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (f32)display.pixel_width() / display.pixel_height(), 0.1f, 1000.0f);


	u32 mesh_id = meshes.get_mesh_id("icosphere");
	auto renderer = meshes.create_renderer<basic_renderer>(mesh_id);
	renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_2D, 0, "low poly"));
	renderer->submit_pre_render(new material(glm::vec3(1.0f), glm::vec3(0.7f), glm::vec3(0.5f), 10.0f));
	renderer->submit_pre_render(&entities.get_pre_render_cam_pos(), false);



	layer main_layer;
	main_layer.submit_renderer(renderer);
	main_layer.submit_shader(shader_handle("icosphere shader"));
	main_layer.get_projection_matrix() = projection;

	main_target.add_layer("main layer", main_layer);
}

auto application::clean_up(void) -> void
{

}
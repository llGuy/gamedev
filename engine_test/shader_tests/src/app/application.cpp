#include <GL/glew.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "application.h"
#include "../io/io.h"
#include <fstream>

application::application(void)
	: display(1000, 600, "Shader Tests")
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

		init_data();
		init_shaders();
		init_meshes();

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	auto view_matrix = entities.get_camera().matrix();

	auto program = shaders[shader_handle("3D shader")];
	program.send_uniform_mat4("view_matrix", glm::value_ptr(view_matrix), 1);

	tex.bind(GL_TEXTURE_2D, 0);
	renderer.render(program, meshes);
}

auto application::update(void) -> void
{
	display.refresh();

	entities.update(timer.elapsed());
	timer.reset();

	is_running = display.is_open();
}

auto application::running(void) -> bool
{
	return is_running;
}

auto application::init_meshes(void) -> void
{
	auto image = extract_png("res/textures/low_poly.png");

	tex.create();
	tex.bind(GL_TEXTURE_2D);
	tex.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, image.data, image.w, image.h);

	tex.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	meshes.init();

	shaders.init();

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (f32)display.pixel_width() / display.pixel_height(), 0.1f, 1000.0f);


	/* process to creating a mesh : 
	-  create the handle for the mesh e.g. name ...
	-  load the model which will return a "shader handle" 
	-  set the name for the shader that you want to use for the handle 
	-  create the program with the handle (which will have all the properties selected) */
	meshes.create_mesh("icosphere");
	/* loads contents of ico.obj to the mesh "icosphere" */
	shader_handle handle = meshes.load_mesh("res/models/ico.obj", "icosphere");
	handle.set_name("3D shader");
	shaders.create_program(handle, "3D");


	auto program = shaders[shader_handle("3D shader")];
	program.bind();
	program.send_uniform_mat4("projection_matrix", glm::value_ptr(projection), 1);
	program.send_uniform_mat4("view_matrix", glm::value_ptr(detail::identity_matrix), 1);

	auto func = meshes.create_render_func("icosphere");
	renderer.set(func);

	renderer.submit(detail::identity_matrix);
}

auto application::init_data(void) -> void
{
	entities.create(display.user_inputs());
}

auto application::init_shaders(void) -> void
{

}
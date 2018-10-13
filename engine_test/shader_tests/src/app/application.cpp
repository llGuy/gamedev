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

		glew_init();

		init_meshes();
		init_shaders();

		is_running = true;
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

	vao.bind();
	tex.bind(GL_TEXTURE_2D, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

auto application::update(void) -> void
{
	display.refresh();

	is_running = display.is_open();
}

auto application::running(void) -> bool
{
	return is_running;
}

auto application::init_meshes(void) -> void
{
	f32 vertices[]
	{
		-1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
		1.0f, -1.0f, 0.0f,     1.0f, 0.0f,

        1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,      1.0f, 1.0f
	};

	buffer vbo;
	vbo.create();
	vbo.fill(sizeof vertices, vertices, GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	vao.create();
	vao.bind();

	attribute vertex_att(0, GL_FLOAT, 3, GL_FALSE, 5 * sizeof(f32), nullptr);
	attribute color_att(2, GL_FLOAT, 2, GL_FALSE, 5 * sizeof(f32), (void *)(sizeof(glm::vec3)));
	vao.attach(vbo, vertex_att, color_att);

	auto image = extract_png("res/textures/texture_test.png");
	create_color_texture(tex, image.w, image.h, image.data);



	meshes.init();

	meshes.create_mesh("icosphere");
	/* loads contents of ico.obj to the mesh "icosphere" */
	shader_handle handle = meshes.load_mesh("res/models/ico.obj", "icosphere");
	shaders.create_program(handle, "3D");
}

auto application::init_shaders(void) -> void
{
	shaders.init();


	shader_handle handle("basic 2D");
	/* configuring the shader to use vertex colors given by attributes instead of texture */
	handle.set(shader_property::texture_coords);
	shaders.create_program(handle, "2D");

	glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f);

	auto program = shaders[shader_handle("basic 2D")];
	program.bind();
	program.send_uniform_mat4("projection_matrix", glm::value_ptr(projection), 1);
}
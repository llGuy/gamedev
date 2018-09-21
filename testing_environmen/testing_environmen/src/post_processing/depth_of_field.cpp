#include "../render_func.h"
#include "depth_of_field.h"

auto depth_of_field::create(i32 width, i32 height) -> void 
{
	this->w = width;
	this->h = height;

	main.create(w, h);
	main.bind();

	create_texture(w, h);
	create_depth(w, h);

	main.attach(out, GL_COLOR_ATTACHMENT0, 0);
	main.attach(depth, GL_DEPTH_ATTACHMENT);

	/* create shaders */
	shaders.create_shader(GL_VERTEX_SHADER, "dof/vsh.shader");
	shaders.create_shader(GL_FRAGMENT_SHADER, "dof/fsh.shader");
	shaders.link_shaders();
	shaders.get_uniform_locations("scene_texture", "depth_texture", "blur_texture");
	shaders.use();
	shaders.uniform_1i(0, 0);
	shaders.uniform_1i(1, 1);
	shaders.uniform_1i(2, 2);
}

auto depth_of_field::render(quad_2D & quad, texture & prev, i32 w, i32 h) -> void
{
	bind();

	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0, 0, 0);

	shaders.use();

	/* bind other two to 1 and 2 */
	prev.bind(GL_TEXTURE_2D, 0);

	render_model(quad, GL_TRIANGLE_STRIP);

	unbind_all_framebuffers(w, h);
}

auto depth_of_field::bind(void) -> void
{
	main.bind();
}

auto depth_of_field::create_texture(i32 w, i32 h) -> void
{
	out.create();
	out.bind(GL_TEXTURE_2D);
	out.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr, w, h);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

auto depth_of_field::create_depth(i32 w, i32 h) -> void
{
	depth.create();
	depth.bind();
	depth.set_storage(GL_DEPTH_COMPONENT, w, h);
}
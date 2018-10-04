#include "hblur_stage.h"
#include "render_pipeline.h"

hblur_stage::hblur_stage(i32 s)
	: scale(s)
{
}

auto hblur_stage::create(i32 width, i32 height) -> void
{
	this->w = width / scale;
	this->h = height / scale;

	main.create(w, h);
	main.bind();

	create_texture(w, h);
	create_depth(w, h);

	main.attach(out, GL_COLOR_ATTACHMENT0, 0);
	main.attach(depth, GL_DEPTH_ATTACHMENT);

	shaders.attach(shader(GL_VERTEX_SHADER, "blur/horizontal_vsh.shader"));
	shaders.attach(shader(GL_FRAGMENT_SHADER, "blur/blur_fsh.shader"));
	shaders.link("vertex_position", "texture_coords");
	shaders.bind();
	shaders.send_uniform_float("target_width", static_cast<f32>(width));
}

auto hblur_stage::bind(void) -> void
{
	main.bind();
}

auto hblur_stage::render(quad_2D & quad, texture & prev, i32 w, i32 h) -> void
{
	bind();

	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0, 0, 0);

	shaders.bind();
	prev.bind(GL_TEXTURE_2D, 0);
	render_model(quad, GL_TRIANGLE_STRIP);
}

auto hblur_stage::create_texture(i32 w, i32 h) -> void
{
	out.create();
	out.bind(GL_TEXTURE_2D);
	out.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr, w, h);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

auto hblur_stage::create_depth(i32 w, i32 h) -> void
{
	depth.create();
	depth.bind();
	depth.set_storage(GL_DEPTH_COMPONENT, w, h);
}

auto hblur_stage::reset(i32 width, i32 height) -> void
{
	main.clean_up();
	out.clean_up();
	depth.clean_up();

	this->w = width / scale;
	this->h = height / scale;

	main.create(w, h);
	main.bind();

	create_texture(w, h);
	create_depth(w, h);

	main.attach(out, GL_COLOR_ATTACHMENT0, 0);
	main.attach(depth, GL_DEPTH_ATTACHMENT);

	shaders.bind();
	shaders.send_uniform_float(0, static_cast<f32>(width));
}
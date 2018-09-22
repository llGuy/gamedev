#include <GL/glew.h>
#include "../quad_2D.h"
#include "default_stage.h"

auto default_stage::create(i32 w, i32 h) -> void
{
	this->w = w;
	this->h = h;

	main.create(w, h);
	main.bind();

	create_texture(w, h);
	create_depth(w, h);

	main.attach(out, GL_COLOR_ATTACHMENT0, 0);
	main.attach(depth, GL_DEPTH_ATTACHMENT, 0);
}

auto default_stage::bind(void) -> void
{
	main.bind();
}

auto default_stage::clean(void) -> void
{

}

auto default_stage::render(quad_2D & quad, texture & prev, i32 w, i32 h) -> void
{
	/*the rendering of the actual scene will happen in the game state
	  class, it will bind to this  */
}

auto default_stage::create_texture(i32 w, i32 h) -> void
{
	out.create();
	out.bind(GL_TEXTURE_2D);
	out.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr, w, h);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

auto default_stage::create_depth(i32 w, i32 h) -> void
{
	depth.create();
	depth.bind(GL_TEXTURE_2D);
	depth.fill(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr, w, h);
	depth.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	depth.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	depth.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	depth.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

auto default_stage::reset(i32 w, i32 h) -> void
{
	main.clean_up();
	out.clean_up();
	depth.clean_up();

	this->w = w;
	this->h = h;

	main.create(w, h);
	main.bind();

	create_texture(w, h);
	create_depth(w, h);

	main.attach(out, GL_COLOR_ATTACHMENT0, 0);
	main.attach(depth, GL_DEPTH_ATTACHMENT, 0);
}
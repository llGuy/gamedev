#pragma once

#include "post_proc_stage.h"

class depth_of_field
{
private:
	framebuffer main;
	texture out;
	renderbuffer depth;

	program shaders;

	i32 w, h;
public:
	auto create(i32 width, i32 height) -> void;
	auto render(quad_2D & quad, texture & prev, i32 w, i32 h) -> void;
	auto bind(void) -> void;

	inline auto output(void) -> texture & { return out; };
	inline auto fbo(void) -> framebuffer & { return main; }

	inline auto width(void)  -> i32 { return w; };
	inline auto height(void) -> i32 { return h; };
private:
	auto create_texture(i32 w, i32 h) -> void;
	auto create_depth(i32 w, i32 h) -> void;
};
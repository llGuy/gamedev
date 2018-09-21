#pragma once

#include "post_proc_stage.h"
#include "../types.h"
#include "../quad_2D.h"
#include "../texture.h"

class default_stage
{
private:
	framebuffer main;
	texture out;
	texture depth;

	program shaders;

	i32 w, h;
public:
	auto create(i32 w, i32 h) -> void;
	auto bind(void) -> void;
	auto render(quad_2D & quad, texture & prev, i32 w, i32 h) -> void;

	inline auto depth_out(void) -> texture & { return depth; }
	inline auto output(void) -> texture & { return out; };
	inline auto fbo(void) -> framebuffer & { return main; }

	inline auto width(void)  -> i32 { return w; };
	inline auto height(void) -> i32 { return h; };
protected:
	auto create_texture(i32 w, i32 h) -> void;
	auto create_depth(i32 w, i32 h) -> void;
};
#pragma once

#include "../framebuffer.h"
#include "../texture.h"
#include "../shader_program.h"
#include "../quad_2D.h"
#include "../renderbuffer.h"

class postproc_stage
{
public:
	postproc_stage(void) = default;

	virtual auto create(i32 w, i32 h) -> void = 0;
	virtual auto bind(void) -> void = 0;
	virtual auto render(quad_2D & quad, texture & prev, i32 w, i32 h) -> void {}

	inline auto output(void) -> texture & { return out; };
	inline auto fbo(void) -> framebuffer & { return main; }
protected:
	virtual auto create_texture(i32 w, i32 h) -> void {}
	virtual auto create_depth(i32 w, i32 h) -> void {}
protected:
	framebuffer main;
	texture out;
	renderbuffer depth;

	program shaders;

	i32 w, h;
};
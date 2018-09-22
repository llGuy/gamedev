#pragma once

#include "post_proc_stage.h"

// test speed

class vblur_stage : public postproc_stage
{
private:
	framebuffer main;
	texture out;
	renderbuffer depth;

	program shaders;

	i32 w, h;
public:
	vblur_stage(i32 scale);

	auto create(i32 w, i32 h) -> void override;
	auto reset(i32 w, i32 h) -> void;
	auto bind(void) -> void override;
	auto render(quad_2D & quad, texture & prev, i32 w, i32 h) -> void override;

	inline auto output(void) -> texture & { return out; };
	inline auto fbo(void) -> framebuffer & { return main; }

	inline auto width(void)  -> i32 { return w; };
	inline auto height(void) -> i32 { return h; };
protected:
	auto create_texture(i32 w, i32 h) -> void override;
	auto create_depth(i32 w, i32 h) -> void override;
private:
	i32 scale;
};
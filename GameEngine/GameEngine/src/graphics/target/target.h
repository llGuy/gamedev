#pragma once

#include "../../api/api.h"
#include "../texture/texture_handler.h"

class target
{
private:
	framebuffer output;
public:
	virtual auto init(texture_handler & textures) -> void = 0;
	auto bind(void) -> void;
	auto clear_color(f32 r, f32 g, f32 b, f32 a) -> void;
	auto clear(u32 flags) -> void;

	auto add_color_output(texture * color) -> void;
	auto add_depth_output(texture * depth) -> void;
};
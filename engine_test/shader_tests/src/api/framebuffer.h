#pragma once

#include <vector>
#include "texture.h"
#include "renderbuffer.h"
#include "../utils/types.h"

class framebuffer
{
protected:
	u32 id;
	i32 w, h;

	std::vector<u32 /* texture ids */> outs;
public:
	framebuffer(void);

	auto create(i32 width, i32 height) -> u32;
	auto clean_up(void) -> void;
	auto bind(GLenum point) -> void;

	auto resolve(framebuffer & other) -> void;

	auto add_texture(u32 id) -> void;
    auto attach(texture & obj, u32 attachment, u32 level) -> void;
	auto attach(renderbuffer & obj, u32 attachment) -> void;

	auto select_color_buffer(u32 point) -> void;
	auto framebuffer_status(void) -> bool;

	auto width(void) -> i32 &;
	auto height(void) -> i32 &;
};
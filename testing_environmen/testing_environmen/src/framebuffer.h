#pragma once

#include <GL/glew.h>
#include "types.h"

class texture;
class renderbuffer;

extern auto unbind_all_framebuffers(i32 w, i32 h) -> void;

class framebuffer
{
public:
	framebuffer(void) = default;

	auto create(i32 width, i32 height) -> u32;
	auto destroy(void) -> void;
	auto bind(void) -> void;
	auto unbind(void) -> void;

	auto attach(texture & obj, u32 component, u32 level) -> void;
	auto attach(renderbuffer & rnd, u32 component) -> void;

	auto select_color_buffer(u32 point) -> void;
	auto framebuffer_status(void) -> bool;

	inline auto width(void)  -> i32 { return w; };
	inline auto height(void) -> i32 { return h; };
private:
	u32 id;

	i32 w, h;
};
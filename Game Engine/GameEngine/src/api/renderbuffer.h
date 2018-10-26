#pragma once

#include <GL/glew.h>
#include "../utils/types.h"

class renderbuffer
{
protected:
	friend class framebuffer;

	u32 id;
public:
	renderbuffer(void) = default;

	auto create(void) -> u32;
	auto clean_up(void) -> void;
	auto bind(void) -> void;

	auto set_storage_ms(i32 samples, GLenum format, i32 w, i32 h) -> void;
	auto set_storage(GLenum component, u32 w, u32 h) -> void;
};

extern auto create_depth_renderbuffer(renderbuffer & rnd, i32 w, i32 h) -> void;
extern auto create_depth_renderbuffer_ms(renderbuffer & rnd, i32 samples, i32 w, i32 h) -> void;

extern auto create_color_renderbuffer(renderbuffer & rnd, i32 w, i32 h) -> void;
extern auto create_color_renderbuffer_ms(renderbuffer & rnd, i32 samples, i32 w, i32 h) -> void;
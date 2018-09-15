#pragma once

#include "types.h"
#include <GL/glew.h>
#include <iostream>

class renderbuffer
{
	friend class framebuffer;
public:
	renderbuffer(void) = default;

	auto create(void) -> u32;
	auto bind(void) -> void;
	auto destroy(void) -> void;

	auto set_storage(GLenum component, u32 w, u32 h) -> void;
private:
	u32 id;
};

extern auto create_depth_buffer(renderbuffer & r, int32_t w, int32_t h) -> void;
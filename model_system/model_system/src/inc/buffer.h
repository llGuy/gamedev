#pragma once

#include <GL/glew.h>
#include "types.h"

extern auto unbind_buffers(GLenum point) -> void;

class buffer
{
public:
	buffer(void) = default;

	auto create(void) -> u32;
	auto destroy(void) -> void;
	auto bind(GLenum binding_point) -> void;

	template <typename T> auto fill(u32 size, T * data, GLenum usage, GLenum binding_point) -> void
	{
		bind(binding_point);
		glBufferData(binding_point, size, data, usage);
	}

	template <typename T> auto partial_fill(u32 offset, u32 size, T * data, GLenum binding_point) -> void
	{
		bind(binding_point);
		glBufferSubData(binding_point, offset, size, data);
	}
private:
	u32 id;
};
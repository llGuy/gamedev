#pragma once

#include <GL/glew.h>
#include "../utils/types.h"

extern auto unbind_buffers(GLenum point) -> void;
extern auto unmap_buffers(GLenum point) -> void;

class buffer
{
public:
	buffer(void) = default;

	auto create(void)->u32;
	auto destroy(void) -> void;
	auto bind(GLenum binding_point) -> void;
	auto map(GLenum binding_point, GLenum access) -> void *;

	template <typename T = void> auto fill(u32 size, T * data, GLenum usage, GLenum binding_point) -> void
	{
		bind(binding_point);
		glBufferData(binding_point, size, data, usage);
	}

	template <typename T> auto partial_fill(u32 offset, u32 size, T * data, GLenum binding_point) -> void
	{
		bind(binding_point);
		glBufferSubData(binding_point, offset, size, data);
	}
protected:
	friend class glsl_program;

	u32 id;
};
#include "buffer.h"

auto unbind_buffers(GLenum point) -> void
{
	glBindBuffer(point, 0);
}

auto buffer::create(void) -> u32
{
	glGenBuffers(1, &id);
	return id;
}

auto buffer::destroy(void) -> void
{
	glDeleteBuffers(1, &id);
}

auto buffer::bind(GLenum binding_point) -> void
{
	glBindBuffer(binding_point, id);
}
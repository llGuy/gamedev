#include "buffer.h"

auto unbind_buffers(GLenum point) -> void
{
	glBindBuffer(point, 0);
}

auto unmap_buffers(GLenum point) -> void
{
	glUnmapBuffer(point);
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

auto buffer::map(GLenum binding_point, GLenum access) -> void *
{
	return glMapBuffer(binding_point, access);
}
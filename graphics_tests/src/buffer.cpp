#include "buffer.h"

auto buffer::create(void) -> uint32_t
{
	glGenBuffers(1, &id);
	return id;
}
auto buffer::destroy(void) -> void
{
	if (id != 0) glDeleteBuffers(1, &id);
}
auto buffer::unbind(GLenum binding_point) -> void
{
	glBindBuffer(binding_point, 0);
}
auto buffer::bind(GLenum binding_point) -> void
{
	glBindBuffer(binding_point, id);
}
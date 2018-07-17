#include "vao.h"

auto vertex_array::create(void) -> uint32_t
{
	glGenVertexArrays(1, &id);
	return id;
}
auto vertex_array::destroy(void) -> void
{
	if (id != 0) glDeleteVertexArrays(1, &id);
}
auto vertex_array::bind(void) -> void
{
	glBindVertexArray(id);
}
auto vertex_array::unbind(void) -> void
{
	glBindVertexArray(0);
}
auto vertex_array::enable(uint32_t loc) -> void
{
	glEnableVertexAttribArray(loc);
}
auto vertex_array::divisor(uint32_t loc, uint32_t div) -> void
{
	glVertexAttribDivisor(loc, div);
}
template<>
auto vertex_array::push<float>(uint32_t index, uint32_t size, uint32_t stride, void * ptr) -> void
{
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, ptr);
}

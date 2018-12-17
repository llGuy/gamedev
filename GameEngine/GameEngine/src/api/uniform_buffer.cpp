#include "uniform_buffer.h"

uniform_buffer::uniform_buffer(u32 index)
	: index(index)
{
}

auto uniform_buffer::get_index(void) -> u32 &
{
	return index;
}

auto uniform_buffer::bind_base(GLenum binding_point) -> void
{
	glBindBufferBase(binding_point, index, id);
}
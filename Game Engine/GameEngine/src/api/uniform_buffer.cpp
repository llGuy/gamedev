#include "uniform_buffer.h"

u32 uniform_buffer::index_count = 0;

uniform_buffer::uniform_buffer(void)
{
	index = index_count++;
}

auto uniform_buffer::get_index(void) -> u32 &
{
	return index;
}

auto uniform_buffer::bind_base(GLenum binding_point) -> void
{
	glBindBufferBase(binding_point, index, id);
}
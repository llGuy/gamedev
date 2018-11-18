#pragma once

#include "buffer.h"

class uniform_buffer : public buffer
{
public:
	uniform_buffer(void);

	auto get_index(void) -> u32 &;

	auto bind_base(GLenum binding_point) -> void;
private:
	static u32 index_count;

	u32 index;
};
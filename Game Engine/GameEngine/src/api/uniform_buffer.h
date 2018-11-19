#pragma once

#include "buffer.h"

class uniform_buffer : public buffer
{
public:
	uniform_buffer(u32 index);

	auto get_index(void) -> u32 &;

	auto bind_base(GLenum binding_point) -> void;
private:
	u32 index;
};

/* user defined block indices */
#define LIGHT_BLOCK_INDEX 0
#define MATERIAL_PROTOTYPE_BLOCK_INDEX 1
#define ANIMATION_BLOCK_INDEX 2
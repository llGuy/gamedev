#ifndef _RENDERABLE_3D_
#define _RENDERABLE_3D_

#include "buffer.h"
#include "vao.h"

class renderable_3D
{
public:
	renderable_3D(void) = default;
	virtual ~renderable_3D(void) = default;

	virtual auto create(void) -> void = 0;
	virtual auto destroy(void) -> void = 0;
	virtual auto element_buffer(void) -> buffer & = 0;

	auto vao(void) -> vertex_array &
	{
		return buffer_layout;
	}
protected:
	vertex_array buffer_layout;
};

#endif
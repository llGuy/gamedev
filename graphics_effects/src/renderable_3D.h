#ifndef _RENDERABLE_3D_
#define _RENDERABLE_3D_

#ifdef cpp_no_optional
#include "optional.hpp"
#else
#include <optional>
#endif

#include "buffer.h"
#include "texture.h"
#include "vao.h"

#include "resources.h"

class renderable_3D
{
public:
	renderable_3D(void) = default;
	virtual ~renderable_3D(void) = default;

	virtual auto create(resource_handler &) -> void = 0;
	virtual auto destroy(void) -> void = 0;
	virtual auto element_buffer(void) -> buffer & = 0;

	virtual auto model_texture(uint32_t i) -> std::optional<texture *>
	{
		return std::optional <texture *> {};
	}

	auto vao(void) -> vertex_array &
	{
		return buffer_layout;
	}
protected:
	vertex_array buffer_layout;
};

#endif

#pragma once

#include "renderable.h"

class quad_2D : public renderable
{
public:
	quad_2D(void) = default;

	virtual auto create(resource_handler & rh) -> void override;
	auto destroy(void) -> void override;

	inline auto count(void) -> u32 override { return 4; };
	inline auto index_type(void) -> GLenum override { return 0; };
	inline auto index_ptr(void) -> void * override { return nullptr; }
	inline auto indexed(void) -> bool override { return false; };
protected:
	buffer vertex_buffer;
};

class gui_quad : public quad_2D
{
public:
	gui_quad(void) = default;

	auto create(resource_handler & rh) -> void override;
};
#pragma once

#include "renderable.h"

class cube : public renderable
{
public:
	cube(f32 radius);

	auto create(resource_handler & rh) -> void override;
	auto destroy(void) -> void override;
	
	inline auto indexed(void) -> bool override { return true; };
	inline auto count(void) -> u32 override { return 36; }; 
	inline auto index_type(void) -> GLenum override { return GL_UNSIGNED_SHORT; };
	inline auto index_ptr(void) -> void * override { return nullptr; };
	inline auto element_buffer(void) -> std::optional<buffer *> { return &index_buffer; };
private:
	f32 radius;

	buffer index_buffer;
	buffer vertex_buffer;
};
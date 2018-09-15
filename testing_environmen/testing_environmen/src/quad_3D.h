#pragma once

#include <array>
#include <glm/glm.hpp>
#include "buffer.h"
#include "renderable.h"

class quad_3D : public renderable
{
public:
	quad_3D(glm::vec3 const & p1, glm::vec3 const & p2, 
		glm::vec3 const & p3, glm::vec3 const & p4);

	auto create(resource_handler & rh) -> void override;
	auto destroy(void) -> void override;

	inline auto count(void) -> u32 override { return 4; };
	inline auto index_type(void) -> GLenum override { return 0; };
	inline auto index_ptr(void) -> void * override { return nullptr; }
	inline auto indexed(void) -> bool override { return false; };
private:
	buffer vertex_buffer;
	std::array<glm::vec3, 4> verts;
};
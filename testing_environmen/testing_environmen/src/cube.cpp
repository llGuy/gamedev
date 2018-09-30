/*#include "cube.h"
#include <glm/glm.hpp>

cube::cube(f32 radius)
	: radius(radius)
{
}

auto cube::create(resource_handler & rh) -> void
{
	attribute vertex_att { GL_FLOAT, 3, GL_FALSE, sizeof(glm::vec3), nullptr, false };
	
	glm::vec3 verts[]
	{
		// front
		glm::vec3(-radius, -radius,  radius),
		glm::vec3(radius, -radius,  radius),
		glm::vec3(radius,  radius,  radius),
		glm::vec3(-radius,  radius,  radius),
		// back
		glm::vec3(-radius, -radius, -radius),
		glm::vec3(radius, -radius, -radius),
		glm::vec3(radius,  radius, -radius),
		glm::vec3(-radius,  radius, -radius)
	};
	vertex_buffer.create();
	vertex_buffer.fill(sizeof(verts), verts, GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	u16 indices[]
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		5, 4, 1,
		0, 1, 4,
		// top
		2, 3, 6,
		7, 6, 3,
	};
	index_buffer.create();
	index_buffer.fill(sizeof(indices), indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

	layout.create();
	layout.bind();
	layout.attach(vertex_buffer, vertex_att);
	unbind_vertex_layouts();
}

auto cube::destroy(void) -> void
{
	
}*/
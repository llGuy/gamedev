#include "cube.h"
#include <array>

cube::cube(float rad, glm::vec3 const & col)
	: radius(rad), cube_color(col)
{
}

auto cube::create(resource_handler &) -> void
{
	create_buffers();
	create_vao();
}

auto cube::destroy(void) -> void
{
	vertex_buffer.destroy();
	index_buffer.destroy();
	buffer_layout.destroy();
}

auto cube::element_buffer(void) -> buffer &
{
	return index_buffer;
}

auto cube::create_buffers(void) -> void
{
	std::array<glm::vec3, 16> verts
	{
		// front

		glm::vec3(-radius, -radius,  radius), cube_color,
		glm::vec3(radius, -radius,  radius), cube_color,
		glm::vec3(radius,  radius,  radius), cube_color,
		glm::vec3(-radius,  radius,  radius), cube_color,
		// back
		glm::vec3(-radius, -radius, -radius), cube_color,
		glm::vec3(radius, -radius, -radius), cube_color,
		glm::vec3(radius,  radius, -radius), cube_color,
		glm::vec3(-radius,  radius, -radius), cube_color
	};
	vertex_buffer.create();
	vertex_buffer.fill(verts.size() * sizeof(glm::vec3), verts.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	std::array<uint16_t, 36> indices
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
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3, 
			};
	index_buffer.create();
	index_buffer.fill(indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
}

auto cube::create_vao(void) -> void
{
	buffer_layout.create();
	buffer_layout.bind();
	buffer_layout.enable(0);
	buffer_layout.enable(1);
	buffer_layout.push<float>(0, 3, 6 * sizeof(float), nullptr);
	buffer_layout.push<float>(1, 3, 6 * sizeof(float), (void *)(sizeof(float) * 3));
	index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	vertex_buffer.bind(GL_ARRAY_BUFFER);
}

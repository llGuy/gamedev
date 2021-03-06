#include "quad.h"

auto quad_3D::create(resource_handler &) -> void
{
	vertex_buffer.create();
	index_buffer.create();
	buffer_layout.create();

	// create vertices and indices
	glm::vec2 vertices[]
	{
		glm::vec2{-0.5f, -0.5f},
		glm::vec2{-0.5f, 0.5f},
		glm::vec2{0.5f, 0.5f},
		glm::vec2{0.5f, -0.5f}
	};

	vertex_buffer.fill<glm::vec2>(sizeof(vertices), vertices, GL_STATIC_DRAW, GL_ARRAY_BUFFER);
	
	uint16_t indices[]
	{
		0, 1, 2, 0, 2, 3
	};

	index_buffer.fill<uint16_t>(sizeof(indices), indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

	buffer_layout.bind();
	buffer_layout.enable(0);
	buffer_layout.push<float>(0, 2, 2 * sizeof(float), nullptr);
	index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	buffer_layout.unbind();
}
auto quad_3D::destroy(void) -> void
{
	vertex_buffer.destroy();
	index_buffer.destroy();
	buffer_layout.destroy();
}
auto quad_3D::element_buffer(void) -> buffer &
{
	return index_buffer;
}
#include "quad_2D.h"
#include <glm/glm.hpp>

auto quad_2D::create(resource_handler & rh) -> void
{
	attribute vertex_att{ GL_FLOAT, 2, GL_FALSE, sizeof(glm::vec2), nullptr, false };
	glm::vec2 verts[]
	{
		{ -1.0f, -1.0f },
	    { -1.0f, +1.0f },
	    { +1.0f, -1.0f },
	    { +1.0f, +1.0f },
	};

	vertex_buffer.create();
	vertex_buffer.fill(sizeof(verts), verts, GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	layout.create();
	layout.bind();
	layout.attach(vertex_buffer, vertex_att);
	unbind_vertex_layouts();
}

auto quad_2D::destroy(void) -> void
{
	vertex_buffer.destroy();
	layout.destroy();
}

auto gui_quad::create(resource_handler & rh) -> void
{
	attribute vertex_att{ GL_FLOAT, 2, GL_FALSE, 
		sizeof(glm::vec2) * 2, nullptr, false };
	attribute texture_att{ GL_FLOAT, 2, GL_FALSE, 
	    sizeof(glm::vec2) * 2, (void*)(2 * sizeof(float)), false };

	glm::vec2 verts[]
	{
		{ -1.0f, -1.0f }, { 0.0f, 0.0f },
	    { -1.0f, +1.0f }, { 0.0f, 1.0f },
		{ +1.0f, -1.0f }, { 1.0f, 0.0f },
		{ +1.0f, +1.0f }, { 1.0f, 1.0f }
	};

	vertex_buffer.create();
	vertex_buffer.fill(sizeof(verts), verts, GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	layout.create();
	layout.bind();
	layout.attach(vertex_buffer, vertex_att, texture_att);
	unbind_vertex_layouts();
}
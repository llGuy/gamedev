#include "gui.h"
#include <glm/gtx/transform.hpp>

quad_2D::quad_2D(glm::vec2 const & mi, glm::vec2 const & ma)
	: corner_min(mi), max_offset(ma)
{
}

auto quad_2D::create(void) -> void
{
	/* position and texture coords */
	struct vertex { glm::vec2 p, t; };

	// aliases
	auto & mi = corner_min;
	auto & of = max_offset;

	glm::vec2 vertices[]
	{
		mi,	glm::vec2(0.0f, 0.0f),
		mi + glm::vec2(0.0f, of.y),glm::vec2(0.0f, 1.0f), 
		mi + glm::vec2(of.x, 0.0f),glm::vec2(1.0f, 0.0f),
		mi + of, glm::vec2(1.0f, 1.0f),
	};

	vertex_buffer.create();
	vertex_buffer.bind(GL_ARRAY_BUFFER);
	vertex_buffer.fill(sizeof(vertices), vertices, GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	buffer_layout.create();
	buffer_layout.bind();
	buffer_layout.enable(0);
	buffer_layout.enable(1);
	buffer_layout.push<float>(0, 2, 4 * sizeof(float), nullptr);
	buffer_layout.push<float>(1, 2, 4 * sizeof(float), (void *)(sizeof(glm::vec2)));
}

auto quad_2D::vao(void) -> vertex_array &
{
	return buffer_layout;
}

auto quad_2D::quad_buffer(void) -> buffer &
{
	return vertex_buffer;
}


// gui handler

gui_handler::gui_handler(void)
	: gui_quad(glm::vec2(-0.5f, -0.5f), glm::vec2(1.0f, 1.0f))
{
}

auto gui_handler::create(void) -> void
{
	gui_quad.create();
	gui_program.create_shader(GL_VERTEX_SHADER, "gui_vsh.shader");
	gui_program.create_shader(GL_FRAGMENT_SHADER, "gui_fsh.shader");
	gui_program.link_shaders("vertex_position", "texture_coords");
	gui_program.get_uniform_locations("model_matrix", "projection_matrix");

	projection_matrix = glm::mat4(1.0f);
}

auto gui_handler::shaders(void)->program &
{
	return gui_program;
}

auto gui_handler::quad(void)->quad_2D &
{
	return gui_quad;
}

auto gui_handler::push(glm::vec2 const & position, float scale) -> void
{
	glm::vec3 trans3D { position.x, position.y, 0.0f };
	quads.emplace_back(glm::translate(trans3D) * glm::scale(glm::vec3(scale)));
}

auto gui_handler::prepare_render(uint32_t i) -> void
{
	auto & model_matrix = quads[i];
	gui_program.uniform_mat4(&model_matrix[0][0], 0);
	gui_program.uniform_mat4(&projection_matrix[0][0], 1);
}
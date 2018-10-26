#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "../../utils/types.h"

struct renderable2D_vertex
{
	glm::vec2 position;
	glm::vec2 uv;

	glm::vec4 color;
};

class renderable2D
{
private:
	/* varies each instance */
	std::array<renderable2D_vertex, 4> vertices;
	i32 texture_id;
public:
	auto set_position(glm::vec2 const & position, glm::vec2 const & size) -> void;
	auto set_uvs(glm::vec2 const & position, glm::vec2 const & size) -> void;
	auto set_colors(glm::ivec4 const & colors) -> void;

	auto set_alpha_negative(void) -> void;

	auto get_texture_id(void) -> i32 &;
};
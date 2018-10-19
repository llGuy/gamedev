#pragma once

#include "../../api/api.h"
#include "../../math/math.h"
#include "../../graphics/shader/shader_mapper.h"
#include "../../graphics/texture/texture_mapper.h"

/* cube map */
class environment
{
private:
	glm::mat4 translation_matrix;
public:
	auto init_shader(shader_mapper & shaders) -> void;
	auto init_texture(texture_mapper & textures, std::string const & base_dir) -> void;

	auto calculate_view_matrix(glm::mat4 const & view_matrix) -> glm::mat4;
	auto calculate_translation(glm::vec3 const & pos) -> glm::mat4;
};
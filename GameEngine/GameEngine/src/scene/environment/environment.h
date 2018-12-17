#pragma once

#include <glm/glm.hpp>
#include "../../api/api.h"
#include "../../graphics/3D/renderer/renderer3D.h"

/* cube map */
class environment
{
private:
	glm::mat4 translation_matrix;

	u32 renderer_id;
public:
	auto init_shader(glsl_program * shader) -> void;
	auto init_texture(texture * color, std::string const & base_dir) -> void;

	auto calculate_view_matrix(glm::mat4 const & view_matrix) -> glm::mat4;
	auto calculate_translation(void) -> glm::mat4;

	auto submit_to_renderer(renderer3D & renderer) -> void;
};
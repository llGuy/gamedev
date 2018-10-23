#pragma once

#include "../../api/api.h"
#include "../../math/math.h"
#include "../../graphics/shader/shader_mapper.h"
#include "../../graphics/texture/texture_mapper.h"
#include "../../graphics/renderer/handler/renderer_handler.h"

/* cube map */
class environment
{
private:
	glm::mat4 translation_matrix;

	u32 renderer_id;
public:
	auto renderer(void) -> u32 &;

	auto init_shader(shader_mapper & shaders) -> void;
	auto init_texture(texture_mapper & textures, std::string const & base_dir) -> void;

	auto calculate_view_matrix(glm::mat4 const & view_matrix) -> glm::mat4;
	auto calculate_translation(void) -> glm::mat4;

	auto submit_to_renderer(renderer_handler & renderers) -> void;
};
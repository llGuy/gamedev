#pragma once

#include "../renderer/renderer.h"
#include "../shader/shader_mapper.h"

/* all models that use the same shader */
class layer
{
private:
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;

	std::vector<renderer *> renderers;

	shader_handle shader;
public:
	/* renderers are on the heap so no need for concern as to whether
	   the vector of entities reallocates*/
	auto submit_renderer(renderer * mesh_renderer) -> void;
	auto submit_shader(shader_handle const & handle) -> void;
	auto operator[](u32 index) -> renderer *;

	auto refresh(shader_mapper & shaders, mesh_handler & meshes) -> void;

	auto get_view_matrix(void) -> glm::mat4 &;
	auto get_projection_matrix(void)->glm::mat4 &;
};
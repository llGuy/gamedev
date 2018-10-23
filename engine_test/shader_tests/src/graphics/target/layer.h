#pragma once

#include "../shader/shader_mapper.h"
#include "../renderer/3D/renderer.h"
#include "../renderer/handler/renderer_handler.h"

/* all models that use the same shader */
class layer_3D
{
private:
	renderer_handler * rnds_handler;

	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;

	/* ids */
	std::vector<u32> renderers;
	
	shader_handle shader;
public:
	layer_3D(renderer_handler & renderers);

	/* wrapper function for submit renderer, submit shader and set projection */
	auto init(std::string const & renderer_name, shader_handle const & handle, glm::mat4 const & projection) -> void;

	/* renderers are on the heap so no need for concern as to whether
	   the vector of entities reallocates*/
	auto submit_renderer(std::string const & name) -> void;
	auto submit_shader(shader_handle const & handle) -> void;

	auto refresh(shader_mapper & shaders, mesh_handler & meshes) -> void;

	auto get_view_matrix(void) -> glm::mat4 &;
	auto get_projection_matrix(void) -> glm::mat4 &;
};
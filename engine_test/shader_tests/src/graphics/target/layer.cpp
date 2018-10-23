#include "layer.h"

layer_3D::layer_3D(renderer_handler & renderers)
	: rnds_handler(&renderers)
{
}

auto layer_3D::init(std::string const & renderer_name, shader_handle const & handle, glm::mat4 const & projection) -> void
{
	submit_renderer(renderer_name);
	submit_shader(handle);
	projection_matrix = projection;
}

auto layer_3D::submit_renderer(std::string const & renderer_name) -> void
{
	u32 renderer_id = rnds_handler->get_renderer_3D_index(renderer_name);
	renderers.push_back(renderer_id);
}

auto layer_3D::submit_shader(shader_handle const & handle) -> void
{
	shader = handle;
}

auto layer_3D::refresh(shader_mapper & shaders, mesh_handler & meshes) -> void
{
	auto & shader_used = shaders[shader];

	shader_used.bind();

	shader_used.send_uniform_mat4("view_matrix", glm::value_ptr(view_matrix), 1);
	shader_used.send_uniform_mat4("projection_matrix", glm::value_ptr(projection_matrix), 1);

	for (auto & mesh_renderer : renderers)
	{
		renderer_3D * renderer = rnds_handler->get_renderer_3D(mesh_renderer);
		renderer->render(shader_used, meshes);
	}
}

auto layer_3D::get_view_matrix(void) -> glm::mat4 &
{
	return view_matrix;
}

auto layer_3D::get_projection_matrix(void) -> glm::mat4 &
{
	return projection_matrix;
}
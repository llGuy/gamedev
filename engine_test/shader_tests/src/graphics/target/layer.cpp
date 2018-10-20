#include "layer.h"

auto layer::init(renderer * mesh_renderer, shader_handle const & handle, glm::mat4 const & projection) -> void
{
	submit_renderer(mesh_renderer);
	submit_shader(handle);
	projection_matrix = projection;
}

auto layer::submit_renderer(renderer * mesh_renderer) -> void
{
	renderers.push_back(mesh_renderer);
}

auto layer::submit_shader(shader_handle const & handle) -> void
{
	shader = handle;
}

auto layer::refresh(shader_mapper & shaders, mesh_handler & meshes) -> void
{
	auto & shader_used = shaders[shader];

	shader_used.bind();

	shader_used.send_uniform_mat4("view_matrix", glm::value_ptr(view_matrix), 1);
	shader_used.send_uniform_mat4("projection_matrix", glm::value_ptr(projection_matrix), 1);

	for (auto & mesh_renderer : renderers)
	{
		mesh_renderer->render(shader_used, meshes);
	}
}

auto layer::operator[](u32 index) -> renderer *
{
	return renderers[index];
}

auto layer::get_view_matrix(void) -> glm::mat4 &
{
	return view_matrix;
}

auto layer::get_projection_matrix(void) -> glm::mat4 &
{
	return projection_matrix;
}
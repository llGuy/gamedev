#include "layer.h"

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

	for (auto & mesh_renderer : renderers)
	{
		mesh_renderer->render(shader_used, meshes);
	}
}

auto layer::operator[](u32 index) -> renderer *
{
	return renderers[index];
}
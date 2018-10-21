#include "renderer_2D.h"

auto renderer_2D::set_mesh(u32 mesh_id, mesh_handler & mh) -> void
{
	this->mesh_id = mesh_id;
	auto render_fun = mh.create_render_func(mesh_id);
	set_rf(render_fun);
}
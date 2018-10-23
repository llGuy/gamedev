#include "renderer_handler.h"

auto renderer_handler::flush_3D(void) -> void
{
	for (auto & rnd : renderers_3D)
	{
		rnd->flush();
	}
}

auto renderer_handler::flush_2D(void) -> void
{
	for (auto & rnd : renderers_2D)
	{
		rnd->flush();
	}
}

auto renderer_handler::get_renderer_3D_index(std::string const & name) -> u32
{
	return index_map_3D[name];
}

auto renderer_handler::get_renderer_2D_index(std::string const & name) -> u32
{
	return index_map_2D[name];
}

auto renderer_handler::get_renderer_3D(u32 index) -> renderer_3D *
{
	return renderers_3D[index];
}

auto renderer_handler::get_renderer_2D(u32 index) -> renderer_2D *
{
	return renderers_2D[index];
}
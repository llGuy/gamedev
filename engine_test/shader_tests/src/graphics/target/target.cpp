#include "target.h"

auto target::get_fbo(void) -> framebuffer &
{
	return render_target;
}

auto target::add_layer(std::string const & name, layer_3D const & layr) -> u32
{
	u32 index = layers_3D.size();

	layers_3D.push_back(layr);

	index_map[name] = index;

	return index;
}

auto target::bind(void) -> void
{
	render_target.bind(GL_FRAMEBUFFER);
}

auto target::clear(u32 bits) -> void
{
	glClear(bits);
}

auto target::clear_color(f32 r, f32 g, f32 b, f32 a) -> void
{
	glClearColor(r, g, b, a);
}

auto target::render(mesh_handler & meshes, shader_mapper & shaders) -> void
{
	for (auto & render_layer : layers_3D)
	{
		render_layer.refresh(shaders, meshes);
	}
}

auto target::get_layer_index(std::string const & name) -> u32
{
	return index_map[name];
}

auto target::operator[](u32 index) -> layer_3D &
{
	return layers_3D[index];
}
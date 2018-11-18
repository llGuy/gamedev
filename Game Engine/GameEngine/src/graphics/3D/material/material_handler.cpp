#include "material_handler.h"

auto material_handler::add_material(std::string mat_name, material_light_info const & light_info
	, glsl_program * shader, light_handler & lights) -> material_prototype *
{
	u32 material_type_index = materials.size();
	materials.push_back(new material_prototype(light_info, shader, lights, mat_name));
	material_indices[mat_name] = material_type_index;
	materials.back()->init();
	return materials.back();
}

auto material_handler::render_all(camera & scene_camera) -> void
{
	for (auto renderer : materials)
	{
		renderer->render(scene_camera);
	}
}

auto material_handler::submit(material * mat) -> void
{
	materials[mat->material_type_id]->submit_material(mat);
}

auto material_handler::get_material_id(std::string const & name)->u32
{
	return material_indices[name];
}

auto material_handler::operator[](u32 id)->material_prototype *
{
	return materials[id];
}

auto material_handler::operator[](std::string const & name)->material_prototype *
{
	return materials[material_indices[name]];
}

auto material_handler::get_size(void) const -> u32
{
	return materials.size();
}
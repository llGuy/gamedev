#include "material_handler.h"

auto material_handler::add_material(std::string mat_name, material_light_info const & light_info
	, glsl_program * shader, light_handler & lights) -> material_prototype *
{
	return (material_types[mat_name] = new material_prototype(light_info, shader, lights));
}

auto material_handler::get_material_type(std::string const & name)->material_prototype *
{
	return material_types[name];
}
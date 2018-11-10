#pragma once

#include <string>
#include <unordered_map>

#include "material.h"

class material_handler
{
private:
	std::unordered_map<std::string, material_prototype *> material_types;
public:
	auto add_material(std::string mat_name, material_light_info const & light_info
		, glsl_program * shader, light_handler & lights) -> material_prototype *;

	auto get_material_type(std::string const & name) -> material_prototype *;

	auto submit(material * mat) -> void;
};
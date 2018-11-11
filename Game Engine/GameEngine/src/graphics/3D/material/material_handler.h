#pragma once

#include <string>
#include <unordered_map>

#include "material.h"
#include "../renderer/renderer3D.h"

class material_handler
{
private:
	std::unordered_map<std::string, u32> material_indices;
	std::vector<material_prototype *> materials;
public:
	auto add_material(std::string mat_name, material_light_info const & light_info
		, glsl_program * shader, light_handler & lights, camera * cam) -> material_prototype *;

	auto render_all(void) -> void;

	auto submit(material * mat) -> void;

	auto get_material_id(std::string const & name) -> u32;

	auto get_size(void) const -> u32;

	auto operator[](u32 id) -> material_prototype *;
	auto operator[](std::string const & name) -> material_prototype *;
};
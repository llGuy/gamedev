#pragma once

#include <string>
#include <unordered_map>

#include "material.h"
#include "../../../json_loader.h"
#include "material_json_functor.h"
#include "../renderer/renderer3D.h"

class material_handler
{
private:
	std::unordered_map<std::string, u32> material_indices;
	std::vector<material_prototype *> materials;

	json_loader<material_loader_functor> loader;
public:
	auto init(texture_handler & textures, shader_handler & shaders, light_handler & lights) -> void;

	auto load_materials(void) -> void;

	auto init_material(std::string const & name) -> material_prototype *;

	auto add_material(std::string mat_name, material_light_info const & light_info
		, glsl_program * shader, light_handler & lights) -> material_prototype *;

	auto render_all(camera & scene_camera /* view matrix */) -> void;

	auto flush(void) -> void;

	auto submit(material * mat) -> void;

	auto get_material_id(std::string const & name) -> u32;

	auto get_size(void) const -> u32;

	auto operator[](u32 id) -> material_prototype *;
	auto operator[](std::string const & name) -> material_prototype *;
};
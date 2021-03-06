#pragma once

#include "../material/material.h"

class renderer3D
{
private:
	material_prototype * material_type;

	std::vector<material *> materials;
public:
	auto set_material_prototype(material_prototype * prototype) -> void;

	auto set_projection(glm::mat4 & projection) -> void;
	auto set_view(glm::mat4 & view) -> void;

	auto submit_material(material * mat) -> void;

	auto render(void) -> void;

	auto flush(void) -> void;

	auto get_shader(void) -> glsl_program *;
};
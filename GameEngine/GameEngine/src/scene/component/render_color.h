#pragma once

#include "../game_object.h"
#include "../../graphics/3D/material/material.h"
#include "../../graphics/3D/renderer/renderer3D.h"
#include "../../graphics/3D/material/material_handler.h"

#include "model_matrix.h"

template <> class component <struct component_render, game_object_data> : public component_base
{
private:
	model & renderable;

	material_handler * materials;

	material * mat;

	transparency is_transparent;
public:
	component(model & renderable, u32 material_type, material_handler & materials, transparency is_transparent)
		: renderable(renderable)
		, mat(new material(renderable, glm::mat4(1.0f), material_type, is_transparent))
		, materials(&materials)
		, is_transparent(is_transparent)
	{
	}

	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		auto & object = objects[entity_index];

		auto & model_matrix_component = object.get_component<component_model_matrix>();

		mat->get_transform() = model_matrix_component.get_trs();
		mat->get_is_transparent() = is_transparent;

		materials->submit(mat);
	}
};


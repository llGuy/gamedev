#pragma once

#include "../game_object.h"
#include "../../graphics/3D/material/material.h"
#include "../../graphics/3D/renderer/renderer3D.h"

#include "model_matrix.h"

template <> class component <struct component_render, game_object_data> : public component_base
{
private:
	model renderable;

	renderer3D * renderer;
public:
	component(model const & renderable, renderer3D & renderer)
		: renderable(renderable)
		, renderer(&renderer)
	{
	}

	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		auto & object = objects[entity_index];

		auto & model_matrix_component = object.get_component<component_model_matrix>();

		material mat{ renderable, model_matrix_component.get_trs() };

		renderer->submit_material(mat);
	}
};
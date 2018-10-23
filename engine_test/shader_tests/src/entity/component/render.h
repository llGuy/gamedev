#pragma once

#include "../ecs.h"
#include "model_matrix.h"
#include "../../math/math.h"
#include "../../graphics/shader/shader_mapper.h"
#include "../../graphics/renderer/handler/renderer_handler.h"

template <> class component <struct render_color> : public icomponent
{
private:
	renderer_3D * renderer;

	u32 model_matrix_index;
public:
	component(entity & ent, i32 entity_index, renderer_3D * renderer)
		: icomponent::icomponent(entity_index), renderer(renderer)
	{
		model_matrix_index = ent.get_component_index<model_matrix>();
	}

	component(void) = default;
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		auto transform_matrix = ecs.get_component<model_matrix>(model_matrix_index).get_trs();

		renderer->submit(transform_matrix);
	};
};
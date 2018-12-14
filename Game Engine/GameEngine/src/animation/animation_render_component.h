#pragma once

#include "animation.h"
#include "animation_handler.h"
#include "animation_wrapper.h"
#include "animated_material.h"
#include "animation_component.h"
#include "../api/uniform_buffer.h"
#include "../scene/component/component.h"
#include "../graphics/3D/renderer/renderer3D.h"

/* responsible for rendering the animation */
template <> struct component<struct component_animation3D_render, game_object_data> : component_base
{
	/* allow to select animations */
	skeletal_animation_handler * animations;

	material_handler * materials;
	skeletal_material * mat;
	i32 animation_component;

	component(skeletal_animation_handler * animations
		, skeletal_material const & mat
		, material_handler & materials)
		: animations(animations)
		, mat(new skeletal_material(mat))
		, animation_component(-1)
		, materials(&materials)
	{
	}

	auto update(f32 time, vec_dd<game_object> & objects) -> void override
	{
		auto & obj = objects[entity_index];

		if (animation_component == -1)
			animation_component = obj.get_component_index<component_animation3D>();

		auto & anim_component = obj.get_component<component_animation3D>(animation_component);
		auto & model_matrix_component = obj.get_component<component_model_matrix>();

		mat->get_transform() = model_matrix_component.get_trs();
		mat->get_transforms_array() = std::move(anim_component.final_matrices);
		mat->get_uniform_block() = anim_component.animation_uniform_block;

		materials->submit(mat);
	}
};
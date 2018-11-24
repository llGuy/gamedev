#pragma once

#include "animation.h"
#include "animation_handler.h"
#include "../window/input.h"
#include "animation_wrapper.h"
#include "animated_material.h"
#include "animation_component.h"
#include "../api/uniform_buffer.h"
#include "../scene/component/component.h"
#include "../graphics/3D/renderer/renderer3D.h"

struct animation_key_association
{
	std::vector<std::pair<u32, std::string>> associations;

	auto operator[](u32 i)
	{
		return associations[i];
	}
};

/* responsible for rendering the animation */
template <> struct component<struct component_animation3D_key_control, game_object_data> : component_base
{
	/* allow to select animations */
	skeletal_animation_handler * animations;

	input_handler * input;

	i32 animation_component;

	animation_key_association associations;

	component(skeletal_animation_handler * animations
		, animation_key_association const & association
		, input_handler & input)
		: animations(animations)
		, animation_component(-1)
		, input(&input)
		, associations(std::move(association))
	{
	}

	auto update(f32 time, vec_dd<game_object> & objects) -> void override
	{
		auto & obj = objects[entity_index];

		if (animation_component == -1)
			animation_component = obj.get_component_index<component_animation3D>();

		auto & anim_component = obj.get_component<component_animation3D>(animation_component);

		bool pressed = false;
		for (u32 i = 1; i < associations.associations.size(); ++i)
		{
			if (input->got_key(associations[i].first))
			{
				anim_component.set_animation(associations[i].second);
				pressed = true;
			}
		}
		if (!pressed) anim_component.set_animation(associations[0].second);
	}
};
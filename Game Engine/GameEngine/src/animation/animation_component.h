#pragma once

#include "animation.h"
#include "animation_handler.h"
#include "animation_wrapper.h"
#include "../scene/component/component.h"

/* component applies animation to model */
template <> struct component<struct component_animation3D, game_object_data> : component_base
{
	/* allow to select animations */
	skeletal_animation_handler * animations;

	animation_wrapper animation_handler;

	std::vector<glm::mat4> final_matrices;

	joint root;
	u32 joint_count;

	component(skeletal_animation_handler * animations
		, joint const & root, u32 joint_count
		, std::string const & first_animation)
		: animations(animations), root(root), joint_count(joint_count)
	{
		animation_handler.set_current_animation(animations->get_animation(first_animation));
	}

	auto update(f32 time, vec_dd<game_object> & objects) -> void override
	{
		final_matrices = animation_handler.update(time, &root, joint_count);
	}
};
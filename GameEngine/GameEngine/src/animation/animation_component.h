#pragma once

#include "animation.h"
#include "animation_handler.h"
#include "animation_wrapper.h"
#include "../api/uniform_buffer.h"
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
	f32 play_speed;

	uniform_buffer animation_uniform_block;

	component(skeletal_animation_handler * animations
		, joint const & root, u32 joint_count
		, std::string const & first_animation = "")
		: animations(animations)
		, root(root)
		, joint_count(joint_count)
		, play_speed(1.5f)
		, animation_uniform_block(ANIMATION_BLOCK_INDEX)
	{
		if (first_animation != "")
			animation_handler.set_current_animation(animations->get_animation(first_animation));

		animation_uniform_block.create();
		animation_uniform_block.fill<void>(sizeof(glm::mat4) * joint_count, nullptr, GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER);
	}
	auto set_animation(std::string const & animation_name) -> void
	{
		animation_handler.set_current_animation(animations->get_animation(animation_name));
	}

	auto update(f32 time, vec_dd<game_object> & objects) -> void override
	{
		final_matrices = animation_handler.update(time * play_speed, &root, joint_count);

		update_uniform_buffer();
	}
private:
	auto update_uniform_buffer(void) -> void
	{
		animation_uniform_block.fill<glm::mat4>(sizeof(glm::mat4) * final_matrices.size(), final_matrices.data(), GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER);
	}
};


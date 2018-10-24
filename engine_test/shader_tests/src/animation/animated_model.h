#pragma once

#include "../api/api.h"

#include "joint.h"
#include "animator.h"
#include "animation.h"

class animated_model
{
private:
	vertex_layout vao;
	texture color;

	joint root;
	u32 joint_count;

	animator animation_handler;
public:
	animated_model(vertex_layout const & vao, texture const & color, joint const & root, u32 count)
		: vao(vao), color(color), root(std::move(root)), joint_count(count)
	{
	}

	auto get_vao(void) -> vertex_layout &
	{
		return vao; 
	}

	auto get_root(void) -> joint &
	{
		return root;
	}

	auto get_texture(void) -> texture &
	{
		return color;
	}

	auto animate(animation ani)-> void
	{
		animation_handler.animate(ani);
	}

	auto update(f32 time) -> void
	{
		animation_handler.update(time);
	}

	auto get_joint_transforms(void) -> std::vector<glm::mat4>
	{
		std::vector<glm::mat4> transforms;
		transforms.resize(joint_count);

		add_joints_to_transform(root, transforms);

		return transforms;
	}

	auto add_joints_to_transform(joint & head, std::vector<glm::mat4> & matrices) -> void
	{
		matrices[head.get_id()] = head.get_animated_transform();

		for (joint & child : head.get_children())
			add_joints_to_transform(child, matrices);
	}
};
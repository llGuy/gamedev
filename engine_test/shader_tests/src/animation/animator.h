#pragma once

#include "../utils/detail.h"
#include "animation.h"
#include "animated_model.h"

class animator
{
private:
	animated_model model;
	animation current_animation;

	f32 time{ 0.0f };
public:
	animator(void) = default;

	animator(animated_model const & model)
		: model(model)
	{
	}

	auto animate(animation const & anim) -> void
	{
		time = 0.0f;
		current_animation = anim;
	}

	auto update(f32 elapsed) -> void
	{
		increase_animation_time(elapsed);
		std::unordered_map<std::string, glm::mat4> current_pose = calculate_current_animation_pose();
		apply_pose_to_joints(current_pose, model.get_root(), detail::identity_matrix);
	}

	auto increase_animation_time(f32 time) -> void
	{
		this->time += time;
		if (this->time > current_animation.get_length())
		{
			this->time = modf(this->time, &current_animation.get_length());
		}
	}

	auto calculate_current_animation_pose(void) -> std::unordered_map<std::string, glm::mat4>
	{
		std::vector<key_frame> frames = get_prev_and_next_frames();
		f32 progress = calculate_progress(frames[0], frames[1]);
		return interp_poses(frames[0], frames[1], progress);
	}

	auto apply_pose_to_joints(std::unordered_map<std::string, glm::mat4> const & current_pose, joint & jnt, glm::mat4 const & parent_transform) -> void
	{
		glm::mat4 current_local_transform = current_pose.at(jnt.get_name());
		glm::mat4 current_transform = parent_transform * current_local_transform;

		for (joint child : jnt.get_children())
		{
			apply_pose_to_joints(current_pose, child, current_transform);
		}
		current_transform = current_transform * jnt.get_inverse_animated_transform();

		jnt.get_animated_transform() = current_transform;
	}

	/* replace with pair */
	auto get_prev_and_next_frames(void) -> std::vector<key_frame>
	{
		auto & all_frames = current_animation.get_key_frames();
		key_frame prev = all_frames[0];
		key_frame next_frame = all_frames[0];

		for (int i = 0; i < all_frames.size(); ++i)
		{
			next_frame = all_frames[i];
			if (next_frame.time_stamp > time)
				break;

			prev = all_frames[i];
		}

		return { prev, next_frame };
	}

	auto calculate_progress(key_frame const & a, key_frame const & b) -> f32
	{
		return (time - a.time_stamp) / (b.time_stamp - a.time_stamp);
	}

	auto interp_poses(key_frame const & a, key_frame const & b, f32 progress) -> std::unordered_map<std::string, glm::mat4>
	{
		std::unordered_map<std::string, glm::mat4> current_pose;
		for (auto joint : a.pose)
		{
			joint_transform prev = joint.second;
			joint_transform next = b.pose.at(joint.first);

			joint_transform current = interpolate(prev, next, progress);

			current_pose[joint.first] = current.get_local_transform();
		}

		return current_pose;
	}

};
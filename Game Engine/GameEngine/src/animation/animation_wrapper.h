#pragma once

#include "animation.h"

class animation_wrapper
{
private:
	animation * current_animation;
	f32 current_time{ 0.0f };
public:
	auto set_current_animation(animation * new_current) -> void;

	auto update(f32 td, joint * root, u32 joint_count) -> std::vector<glm::mat4>;
private:
	auto increase_time(f32 td) -> void;

	auto get_surrounding_frames(void) -> std::pair<u32, u32>; 

	auto get_progress(u32 prev, u32 next) -> f32;

	auto interpolate(u32 prev, u32 next, f32 progress
		, std::unordered_map<std::string, glm::mat4> & transforms) -> void;

	auto update_joints(std::unordered_map<std::string, glm::mat4> & transforms_map
		, joint & parent, glm::mat4 const & parent_transform
		, std::vector<glm::mat4> & final_matrices) -> void;
};
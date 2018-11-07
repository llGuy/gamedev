#include <unordered_map>
#include "animation_wrapper.h"
#include <glm/gtx/transform.hpp>

auto animation_wrapper::set_current_animation(animation * new_current) -> void
{
	current_animation = new_current;
}

auto animation_wrapper::update(f32 td, joint * root, u32 joint_count) -> std::vector<glm::mat4>
{
	std::vector<glm::mat4> final_matrices;
	final_matrices.resize(joint_count);
	std::fill(final_matrices.begin(), final_matrices.end(), glm::mat4(1.0f));

	if (current_animation)
	{
		increase_time(td);
		auto prev_and_next_frames = get_surrounding_frames();
		f32 progress = get_progress(prev_and_next_frames.first, prev_and_next_frames.second);
		/* apply animation pose to joints */
		std::unordered_map<std::string, glm::mat4> transforms;
		interpolate(prev_and_next_frames.first, prev_and_next_frames.second, progress, transforms);
		update_joints(transforms, *root, glm::mat4(1.0f), final_matrices);
	}

	return final_matrices;
}

auto animation_wrapper::increase_time(f32 td) -> void
{
	current_time += td;

	f32 animation_length = current_animation->get_length();

	if (current_time > current_animation->get_length())
		current_time = 0.0f;
}

auto animation_wrapper::get_progress(u32 prev, u32 next) -> f32
{
	key_frame & prev_frame = current_animation->operator[](prev);
	key_frame & next_frame = current_animation->operator[](next);

	f32 progress = (current_time - prev_frame.get_time_stamp()) / (next_frame.get_time_stamp() - prev_frame.get_time_stamp());

	return progress;
}

auto animation_wrapper::get_surrounding_frames(void) -> std::pair<u32, u32>
{
	for (u32 i = 0; i < current_animation->get_frame_count() - 1; ++i)
	{
		f32 time_stamp_prev = current_animation->operator[](i).get_time_stamp();
		f32 time_stamp_next = current_animation->operator[](i + 1).get_time_stamp();

		if (time_stamp_prev < current_time && time_stamp_next > current_time)
		{
			return std::pair(i, i + 1);
		}
	}
	return std::pair(0, 1);
}

auto animation_wrapper::interpolate(u32 prev, u32 next, f32 progress
	, std::unordered_map<std::string, glm::mat4> & transforms) -> void
{
	key_frame & prev_frame = current_animation->operator[](prev);
	key_frame & next_frame = current_animation->operator[](next);

	for (auto transform_prev : prev_frame)
	{
		joint_transform & prev_transform = transform_prev.second;
		joint_transform & next_transform = next_frame[transform_prev.first];

		glm::vec3 translation = prev_transform.position + (next_transform.position - prev_transform.position) * progress;
		glm::quat rotation = glm::slerp(prev_transform.rotation, next_transform.rotation, progress);

		transforms[transform_prev.first] = glm::translate(translation) * glm::mat4_cast(rotation);
	}
}

auto animation_wrapper::update_joints(std::unordered_map<std::string, glm::mat4> & transforms_map
	, joint & current_joint, glm::mat4 const & parent_transform
	, std::vector<glm::mat4> & final_matrices) -> void
{ 
	std::string joint_name = current_joint.get_name();
 	glm::mat4 local_transform = transforms_map[joint_name];
	glm::mat4 current_transform = parent_transform * local_transform;
	for (u32 i = 0; i < current_joint.get_child_count(); ++i)
	{
		update_joints(transforms_map, *(current_joint[i]), current_transform, final_matrices);
	}
	
	glm::mat4 model_space_transform = current_transform * current_joint.get_inverse_bind_transform();

	current_joint.get_animated_transform() = model_space_transform;

	final_matrices[current_joint.get_id()] = model_space_transform;
}
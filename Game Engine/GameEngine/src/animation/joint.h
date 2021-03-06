#pragma once

#include <unordered_map>
#include "../utils/types.h"
#include <glm/gtx/quaternion.hpp>

struct joint_transform
{
	glm::quat rotation;
	glm::vec3 position;
};

class joint
{
private:
	std::vector<joint *> children;
	joint * parent;

	/* id used to map to joint from .dae file */
	u32 id;
	std::string name;

	glm::mat4 local_bind_transform;
	glm::mat4 inverse_bind_transform;

	glm::mat4 animated_transform;
public:
	joint(std::string const & name, u32 id);

	auto add_child(joint * child) -> void;
	auto operator[](u32 index) -> joint * &;

	auto calculate_inverse_bind(glm::mat4 const & inverse_parent = glm::mat4(1.0f)) -> void
	{
		inverse_bind_transform = inverse_parent * inverse_bind_transform;

		for (auto & child : children)
		{
			child->calculate_inverse_bind(inverse_bind_transform);
		}
	}

	auto calculate_inverses(glm::mat4 const & bind_parent = glm::mat4(1.0f)) -> void
	{

		glm::mat4 bind_transform = bind_parent * local_bind_transform;
		inverse_bind_transform = glm::inverse(bind_transform);
		for (auto child : children)
		{
			child->calculate_inverses(bind_transform);
		}
	}

	auto get_id(void) -> u32 &;
	auto get_name(void) -> std::string &;
	auto get_local_bind_transform(void) -> glm::mat4 &;
	auto get_inverse_bind_transform(void) -> glm::mat4 &;
	auto get_parent(void) -> joint * &;
	auto get_animated_transform(void) -> glm::mat4 &;
	auto get_child_count(void) const -> u32;
};
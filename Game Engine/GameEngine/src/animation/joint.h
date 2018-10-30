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

	auto get_id(void) -> u32 &;
	auto get_name(void) -> std::string &;
	auto get_local_bind_transform(void) -> glm::mat4 &;
	auto get_inverse_bind_transform(void) -> glm::mat4 &;
	auto get_parent(void) -> joint * &;
	auto get_animated_transform(void) -> glm::mat4 &;
	auto get_child_count(void) const -> u32;
};
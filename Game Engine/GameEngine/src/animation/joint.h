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
	/* joints are accessible arbitratilly through an external unordered map
	   therefore the children and all joints must be on the heap*/
	std::vector<joint *> children;
	joint * parent;

	/* id used to map to joint from .dae file */
	u32 id;
	std::string name;

	glm::mat4 local_bind_transform;
public:
	joint(std::string const & name, u32 id)
		: id(id), name(name)
	{
	}

	auto add_child(joint * child) -> void
	{
		children.push_back(child);
	}

	auto operator[](u32 index) -> joint * &
	{
		return children[index];
	}

	auto get_id(void) -> u32 &
	{
		return id;
	}

	auto get_name(void) -> std::string &
	{
		return name;
	}

	auto get_local_bind_transform(void) -> glm::mat4 &
	{
		return local_bind_transform;
	}

	auto get_parent(void) ->  joint * &
	{
		return parent;
	}
};
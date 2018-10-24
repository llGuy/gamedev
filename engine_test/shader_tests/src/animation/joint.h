#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../utils/types.h"

class joint
{
private:
	u32 id;
	std::string name;

	/* model space, loaded to uniform array */
	glm::mat4 animated_transform;

	/* relation to parent joint */
	/* bind = no animation applied
	   local = relation to parent*/
	glm::mat4 local_bind_transform;
	/* not in parent space */
	glm::mat4 inverse_bind_transform;

	std::vector<joint> children;
public:
	joint(u32 id, std::string const & name, glm::mat4 const & local_bind)
		: id(id), name(name), local_bind_transform(local_bind)
	{
	}

	auto get_children(void) -> std::vector<joint> &
	{
		return children;
	}

	auto add_child(joint const & child) -> void
	{
		children.push_back(std::move(child));
	}

	auto get_animated_transform(void) -> glm::mat4 &
	{
		return animated_transform;
	}

	auto get_inverse_animated_transform(void) -> glm::mat4 &
	{
		return inverse_bind_transform;
	}

	auto calculate_inverse_bind(glm::mat4 parent_bind_transform) -> void
	{
		glm::mat4 bind_trans = parent_bind_transform * local_bind_transform;

		inverse_bind_transform = glm::inverse(bind_trans);

		for (joint & child : children)
			child.calculate_inverse_bind(bind_trans);
	}

	auto get_id(void) -> u32 &
	{
		return id;
	}

	auto get_name(void) const -> std::string const &
	{
		return name;
	}
};
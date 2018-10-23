#pragma once

#include "../math/math.h"
#include "components.h"

struct game_object_data
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 scale;

	std::string name;

	auto operator->(void) -> game_object_data *
	{
		return this;
	}
};

/* anything that is in the world, can be manipulated ...
   almost like an entity */
using game_object = cs_dev::object<game_object_data>;
using game_object_component_system = cs_dev::component_system<game_object_data>;

using component_base = cs_dev::icomponent<game_object_data>;
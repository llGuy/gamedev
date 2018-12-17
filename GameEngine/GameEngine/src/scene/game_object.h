#pragma once

#include <string>
#include <glm/glm.hpp>
#include "../data/components.h"

struct game_object_data
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 scale;

	std::string name;
};

/* anything that is in the world, can be manipulated ...
   almost like an entity */
using game_object = object<game_object_data>;
using game_object_component_system = component_system<game_object_data>;

using component_base = icomponent<game_object_data>;
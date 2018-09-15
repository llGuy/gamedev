#pragma once

#include "ecs.h"
#include <glm/glm.hpp>

template <> struct component <struct position> : icomponent
{
	component(glm::vec3 const & p) : pos(p) {}

	glm::vec3 pos;
};

template <> struct component <struct direction> : icomponent
{
	component(glm::vec3 const & p) : dir(p) {}

	glm::vec3 dir;
};

class entity_handler
{
private:
public:
	auto init(void)
	{
		std::vector<entity> players(1);
		entity_component_system entity_component_system;
		entity_component_system.add_system<position>(30);
		entity_component_system.add_system<direction>(30);
		entity_component_system.add_component<position>(players[0], glm::vec3(0));
		entity_component_system.add_component<direction>(players[0], glm::vec3(0));

//		entity_component_system.update(0, players);

		entity_component_system.update_only<position>(0, players);

		entity_component_system.remove(players[0]);
	}
};
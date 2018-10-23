#pragma once

#include <string>
#include <iostream>
#include "components.h"
#include "game_object.h"

template <> struct cs_dev::component<struct component_log, game_object_data> : component_base
{
	component(void) = default;
	
	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		std::cout << glm::to_string(objects[entity_index]->position) << std::endl;
	}
};
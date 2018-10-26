#pragma once

#include <string>
#include <iostream>
#include "../game_object.h"
#include <glm/gtx/string_cast.hpp>
#include "../../data/components.h"

#define FRAME_COUNT_MAX_BEFORE_LOG 200

template <> struct component<struct component_log, game_object_data> : component_base
{
private:
	uint32_t frame_count{ 0 };
public:
	component(void) = default;

	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		auto & object = objects[entity_index];

		if (frame_count++ == FRAME_COUNT_MAX_BEFORE_LOG)
		{
			printf("logging game object \"%s\" data (id = %i)\n", object->name.c_str(), entity_index);

			printf("\tposition  %s\n", glm::to_string(objects[entity_index]->position).c_str());
			printf("\tdirection %s\n\n", glm::to_string(objects[entity_index]->direction).c_str());

			frame_count = 0;
		}
	}
};
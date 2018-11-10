#pragma once

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "../camera.h"
#include "../game_object.h"
#include "../../utils/detail.h"
#include "../../window/input.h"
#include <glm/gtx/string_cast.hpp>
#include "../../data/components.h"

template <> struct component<struct component_behavior_mouse, game_object_data> : component_base
{
private:
	input_handler * input;

	glm::vec2 previous_cursor_position;

	bool initialized{ false };
public:
	component(input_handler & inputs)
		: input(&inputs)
		, previous_cursor_position(0.0f)
	{
	}

	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		if (!initialized)
		{
			previous_cursor_position = input->cursor_position();
			initialized = true;
		}

		auto & object = objects[entity_index];

		if (input->cursor_moved())
		{
			glm::vec2 difference = input->cursor_position() - previous_cursor_position;
			previous_cursor_position = input->cursor_position();
			object->direction = glm::normalize(look_fps(difference, object->direction, td));
		}
	}
};
#pragma once

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "../game_object.h"
#include "../../utils/detail.h"
#include "../../window/input.h"
#include <glm/gtx/string_cast.hpp>
#include "../../data/components.h"

struct key_bind
{
	u32 forward;
	u32 left;
	u32 back;
	u32 right;
	u32 up;
	u32 down;
};

template <> struct component<struct component_behavior_key, game_object_data> : component_base
{
private:
	key_bind keys_affected;

	input_handler * input;

	u32 movements{ 0 };
public:
	component(key_bind const & keys, input_handler & inputs)
		: keys_affected(keys), input(&inputs)
	{
	}

	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		auto & object = objects[entity_index];

		glm::vec3 result (0.0f);
		glm::vec3 lateral_dir = glm::normalize(glm::vec3(object->direction.x, 0.0f, object->direction.z));

		if (input->got_key(keys_affected.forward)) accumulate_velocity(td, lateral_dir, result);
		if (input->got_key(keys_affected.left)) accumulate_velocity(td, -glm::cross(lateral_dir, detail::up), result);
		if (input->got_key(keys_affected.back)) accumulate_velocity(td, -lateral_dir, result);
		if (input->got_key(keys_affected.right)) accumulate_velocity(td, glm::cross(lateral_dir, detail::up), result);
		if (input->got_key(keys_affected.up)) accumulate_velocity(td, detail::up, result);
		if (input->got_key(keys_affected.down)) accumulate_velocity(td, -detail::up, result);

		if (movements > 0)
		{
			result = glm::normalize(result) * 20.0f * td;

			object->position += result;
		}

		movements = 0;
	}

	auto accumulate_velocity(f32 td, glm::vec3 const & dir, glm::vec3 & dest) -> void
	{
		++movements;

		dest += dir;
	}
};
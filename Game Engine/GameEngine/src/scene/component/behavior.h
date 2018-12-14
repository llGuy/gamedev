#pragma once

#include "../loader.h"
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../camera.h"
#include "../game_object.h"
#include "../../utils/detail.h"
#include "../../window/input.h"
#include <glm/gtx/string_cast.hpp>
#include "../../data/components.h"

#include "../scene.h"

struct key_bind
{
	u32 forward;
	u32 left;
	u32 back;
	u32 right;
	u32 up;
	u32 down;
};

#define DEFAULT_KEY_BINDINGS key_bind{ GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT }


template <> struct component<struct component_behavior_key, game_object_data> : component_base
{
private:
	key_bind keys_affected;

	input_handler * input;

	u32 movements{ 0 };
public:
	component(key_bind const & keys, input_handler & inputs)
		: keys_affected(keys)
		, input(&inputs)
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
			result = result * 15.0f * td;

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


template <> struct component<struct component_behavior_mouse, game_object_data> : component_base
{
private:
	input_handler * input;

	glm::vec2 previous_cursor_position;

	camera * scene_camera;

	bool initialized{ false };
public:
	component(input_handler & inputs, camera & scene_camera)
		: input(&inputs)
		, previous_cursor_position(0.0f)
		, scene_camera(&scene_camera)
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

			scene_camera->get_rotation_angles() -= difference * 0.5f;

			object->direction = glm::normalize(look_fps(difference, object->direction, td));
		}
	}
};

struct key_control_apply_func : apply_func
{
	input_handler & inputs;

	key_control_apply_func(input_handler & inputs)
		: inputs(inputs)
	{
	}

	auto apply(scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void override
	{
		if (it.value())
		{
			component<component_behavior_key, game_object_data> key_component(DEFAULT_KEY_BINDINGS, inputs);

			obj.add_component(key_component);
		}
	}
};

struct mouse_control_apply_func : apply_func
{
	input_handler & inputs;
	camera & scene_camera;

	mouse_control_apply_func(input_handler & inputs, camera & scene_camera)
		: inputs(inputs), scene_camera(scene_camera)
	{
	}

	auto apply(scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void override
	{
		if (it.value())
		{
			component<component_behavior_mouse, game_object_data> mouse_component(inputs, scene_camera);

			obj.add_component(mouse_component);
		}
	}
};

struct bind_camera_apply_func : apply_func
{
	camera & scene_camera;

	bind_camera_apply_func(camera & scene_camera)
		: scene_camera(scene_camera)
	{
	}

	auto apply(scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void override
	{
		if (it.value())
		{
			dest.bind_camera_to_object(obj);
		}
	}
};
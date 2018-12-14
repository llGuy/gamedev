#pragma once

#include <glm/glm.hpp>
#include "../game_object.h"
#include "../../utils/detail.h"
#include <glm/gtx/transform.hpp>

template <> class component<struct component_model_matrix, game_object_data> : public component_base
{
private:
	glm::mat4 trs;

	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scale;
public:
	component(void) = default;

	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		auto & obj = objects[entity_index];

		translation = glm::translate(obj->position);

		float y_axis_rot = detail::fequ(obj->direction.x, 0.0f) ? 0.00001f : -atan(obj->direction.z / obj->direction.x);
		rotation = glm::rotate(y_axis_rot, glm::vec3(0, 1, 0));

	//	rotation = detail::identity_matrix;

		scale = glm::scale(obj->scale);

		trs = translation * rotation * scale;
	}

	auto get_trs(void) -> glm::mat4 &
	{
		return trs;
	}

	auto get_translation(void) -> glm::mat4 &
	{
		return translation;
	}

	auto get_rotation(void) -> glm::mat4 &
	{
		return rotation;
	}

	auto get_scale(void) -> glm::mat4 &
	{
		return scale;
	}
};

struct model_matrix_apply_func : apply_func
{
	auto apply(class scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void override
	{
		if (it.value())
		{
			component<struct component_model_matrix, game_object_data> matrix_component;

			obj.add_component(matrix_component);
		}
	}
};
#pragma once

#include "../ecs.h"
#include "../../math/math.h"
#include "../../utils/detail.h"

template <> class component <struct model_matrix> : public icomponent
{
private:
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scale;

	glm::mat4 trs;
public:
	component(entity &, i32 entity_index)
		: icomponent::icomponent(entity_index)
	{
	}

	component(void) = default;
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		auto data = entities[entity_index].get_data();
		translation = glm::translate(data.pos);

		float y_axis_rot = detail::fequ(data.dir.x, 0.0f) ? 0.00001f : -atan(data.dir.z / data.dir.x);
		rotation = glm::rotate(y_axis_rot, glm::vec3(0, 1, 0));

		scale = glm::scale(data.size);

		trs = translation * rotation * scale;
	};

	auto get_translation(void) -> glm::mat4 const &
	{
		return translation;
	}

	auto get_rotation(void) -> glm::mat4 const &
	{
		return rotation;
	}

	auto get_scale(void) -> glm::mat4 const &
	{
		return scale;
	}

	auto get_trs(void) -> glm::mat4 const &
	{
		return trs;
	}
};
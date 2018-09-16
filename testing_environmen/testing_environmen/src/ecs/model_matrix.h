#pragma once

#include "ecs.h"
#include "../detail.h"
#include "basic_components.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

template <> class component<struct model_matrix> : public icomponent
{
private:
	i32 height_component_index;
	glm::mat4 translation;
	glm::mat4 rotation;
public:
	component(entity & subject, i32 index)
		: icomponent::icomponent(index)
	{
		height_component_index = subject.get_component_index<height>();
	}
	auto update(f32, vec_dd<entity> & entities, entity_cs & ecs) -> void
	{
		auto & ent_height = ecs.get_component<height>(height_component_index).value.val;
		auto & data = entities[entity_index].get_data();
		translation = glm::translate(glm::vec3(data.pos.x, data.pos.y + ent_height, data.pos.z));

		float y_axis_rot = detail::fequ(data.dir.x, 0.0f) ? 0.00001f : -atan(data.dir.z / data.dir.x);
		rotation = glm::rotate(y_axis_rot, glm::vec3(0, 1, 0));
	}
	auto get_translation(void) -> glm::mat4 const &
	{
		return translation;
	}
	auto get_rotation(void) -> glm::mat4 const &
	{
		return rotation;
	}
};
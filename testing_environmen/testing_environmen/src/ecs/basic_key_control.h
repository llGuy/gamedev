#pragma once

#include "ecs.h"
#include "../detail.h"
#include "../input_handler.h"

template <> class component <struct basic_key_control> : public icomponent
{
private:
	/* pointer to input handler */
	input_handler * inputs;
public:
	component(entity &, i32 entity_index, input_handler & ih) 
		: inputs(&ih), icomponent::icomponent(entity_index)
	{
	}
	component(void) = default;
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::up;
		entity_data & ent = entities[entity_index].get_data();
		glm::vec3 lateral_dir = glm::normalize(glm::vec3(ent.dir.x, 0, ent.dir.z));

		if (inputs->got_key(GLFW_KEY_W)) move(lateral_dir, ent.vel, td, ent);
		if (inputs->got_key(GLFW_KEY_A)) move(-glm::normalize(glm::cross(lateral_dir, up)), ent.vel, td, ent);
		if (inputs->got_key(GLFW_KEY_S)) move(-lateral_dir, ent.vel, td, ent);
		if (inputs->got_key(GLFW_KEY_D)) move(glm::normalize(glm::cross(lateral_dir, up)), ent.vel, td, ent);
		if (inputs->got_key(GLFW_KEY_SPACE)) move(up, ent.vel, td, ent);
		if (inputs->got_key(GLFW_KEY_LEFT_SHIFT)) move(-up, ent.vel, td, ent);

		ent.pos += ent.vel * td;

		ent.vel = glm::vec3(0);
	};

private:
	auto move(glm::vec3 const & dir, glm::vec3 & vel, f32 td, entity_data const & data) -> void
	{
		vel += dir * data.speed;
	}
};
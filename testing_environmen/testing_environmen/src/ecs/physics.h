#pragma once 

#include "ecs.h"

template <> class component<struct physics> : public icomponent
{
private:
	glm::vec3 velocity;
public:
	component(entity & subject, i32 index)
		: icomponent::icomponent(index)
	{
	}
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		auto & data = entities[entity_index].get_data();

		approach(data.vel, velocity, td);

		//data.pos += velocity;

		/* reset goal velocity */
		data.vel = glm::vec3(0);
	}
private:
	auto approach(glm::vec3 const & goal, glm::vec3 & vel, f32 td) -> void
	{
		if (goal.x > vel.x) 
			vel.x += goal.x * td;
		if (goal.z > vel.z) vel.z += goal.z * td;

		if (glm::all(glm::lessThan(glm::abs(goal), glm::vec3(0.00001f))))
		{
			velocity = glm::vec3(0);
		}
	}
};
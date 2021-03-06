#pragma once

#include "program.h"
#include "input_handler.h"
#include "puff_handler.h"
#include "tracer_handler.h"
#include "model_handler.h"
#include "ecs/ecs.h"
#include "vec_dd.h"
#include "camera.h"

class entity_handler
{
private:
	u32 bound_entity;
	camera entity_camera;
	vec_dd<entity> entities;
	entity_cs component_system;
public:
	entity_handler(void);
	auto create(input_handler & ih, program & depth, std::string const & model, 
		tracer_handler & th, puff_handler & ph, model_handler & mh) -> void;
	auto update(f32 td) -> void;
	auto cam(void) -> camera &;

	template <typename T> auto render(bool render_main, f32 td = 0.0f) -> void
	{
		component_system.update_only<T>(td, entities, [this, &render_main](i32 other)
		{
			if (render_main)
			{
				if (other == entity_camera.bound_entity())
				{
					return true;
				}
				else return false;
			}
			else return true;
		});
	}

	template <typename T> auto update_only(f32 td = 0.0f) -> void
	{
		component_system.update_only<T>(td, entities);
	}
	
	auto add_entity(glm::vec3 const & pos, glm::vec3 const & dir,
		std::string const & model, program & color, program & depth, glm::vec3 const & scale, model_handler & mh) -> void;

	auto main_player_lateral_direction(void) -> glm::vec3
	{
		auto & data = entities[0].get_data();
		return glm::normalize(glm::vec3(data.dir.x, 0.0f, data.dir.z));
	}
private:
	auto create_component_system(void) -> void;
	auto create_main_player(input_handler & ih, program & depth, std::string const & model, 
		tracer_handler & th, puff_handler & ph, model_handler & mh) -> void;
};
#pragma once

#include "renderable.h"
#include "program.h"
#include "input_handler.h"
#include "tracer_handler.h"
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
	auto create(input_handler & ih, program & depth, renderable & model, tracer_handler & th) -> void;
	auto update(f32 td) -> void;
	auto cam(void) -> camera &;

	template <typename T> auto update_only(f32 td = 0.0f) -> void
	{
		component_system.update_only<T>(td, entities);
	}
	
	auto add_entity(glm::vec3 const & pos, glm::vec3 const & dir,
		renderable & model, program & color, program & depth) -> void;
private:
	auto create_component_system(void) -> void;
	auto create_main_player(input_handler & ih, program & depth, renderable & model, tracer_handler & th) -> void;
};
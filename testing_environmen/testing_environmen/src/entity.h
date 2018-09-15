#pragma once

#include "input_handler.h"
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
	auto create(input_handler & ih) -> void;
	auto update(f32 td) -> void;
	auto cam(void) -> camera &;
private:
	auto create_component_system(void) -> void;
	auto create_main_player(input_handler & ih) -> void;
};
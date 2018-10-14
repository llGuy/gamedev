#pragma once

#include "ecs.h"
#include "camera.h"
#include "../api/api.h"
#include "../utils/types.h"
#include "../win/input_handler.h"

class entity_handler
{
private:
	u32 bound_entity;
	camera cam;
	vec_dd<entity> entities;
	entity_cs component_system;
public:
	entity_handler(void) = default;

	auto create(input_handler & ih) -> void;

	auto update(f32 td) -> void;
	auto get_camera(void) -> camera &;
private:
	auto create_main_player(input_handler & ih) -> void;
};
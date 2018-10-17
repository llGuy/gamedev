#pragma once

#include "ecs.h"
#include "camera.h"
#include "../api/api.h"
#include "../utils/types.h"
#include "../win/input_handler.h"
#include "../graphics/renderer/pre_render.h"

class entity_handler
{
private:
	u32 bound_entity;
	camera cam;
	vec_dd<entity> entities;
	entity_cs component_system;

	pre_render_camera_position pre_render_cam_pos;
public:
	entity_handler(void) = default;

	auto create(input_handler & ih) -> void;

	auto update(f32 td) -> void;
	auto get_camera(void) -> camera &;
	auto get_pre_render_cam_pos(void) -> pre_render_camera_position &;
private:
	auto create_main_player(input_handler & ih) -> void;
};
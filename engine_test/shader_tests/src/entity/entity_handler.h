#pragma once

#include "ecs.h"
#include "camera.h"
#include "../api/api.h"
#include "../utils/types.h"
#include "../win/input_handler.h"
#include "../graphics/renderer/pre_render.h"
#include "../graphics/renderer/3D/renderer.h"

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

	auto update(f32 td)-> void;

	auto get_camera(void) -> camera &;
	auto get_pre_render_cam_pos(void) -> pre_render_camera_position &;

	struct entity_init_data
	{
		glm::vec3 position, direction, scale;

		renderer_3D * renderer;
	};
	auto submit_entity(entity_init_data const & init_data) -> void;

	template <typename T> auto render_entities(bool render_main, f32 td = 0.0f) -> void
	{
		component_system.update_only<T>(td, entities, [this, &render_main](i32 other)
		{
			if (render_main)
			{
				if (other == cam.bound_entity())
				{
					return true;
				}
				else return false;
			}
			else return true;
		});
	}

private:
	auto create_main_player(input_handler & ih) -> void;
};
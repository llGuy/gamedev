#pragma once

#include "../api/api.h"
#include "../time/timer.h"
#include "../win/window.h"
#include "../graphics/graphics.h"
#include "../world/world_objects.h"
#include "../entity/entity_handler.h"
#include "../lighting/light_handler.h"

#include "../gui/font/font.h"

class application
{
private:

	bool is_running;

	font consolas;

	window display;

	shader_mapper shaders;
	texture_mapper textures;
	mesh_handler meshes;
	entity_handler entities;
	light_handler lights;
	time_handler timer;

	world_objects world;

	target first_out;
	/* targets in the middle for pfx */
	target final_out;

public:

	application(void);

	auto init(void) -> void;
	auto render(void) -> void;
	auto update(void) -> void;
	auto running(void) -> bool;
	auto clean_up(void) -> void;

private:

	auto init_data(void) -> void;
	auto init_meshes(void) -> void;
	auto init_shaders(void) -> void;
	auto init_textures(void) -> void;
	auto init_targets(void) -> void;
	auto init_layers(void) -> void;

};
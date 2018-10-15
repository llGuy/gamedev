#pragma once

#include "../api/api.h"
#include "../time/timer.h"
#include "../win/window.h"
#include "../graphics/graphics.h"
#include "../entity/entity_handler.h"
#include "../lighting/light_handler.h"

class application
{
private:

	bool is_running;

	window display;

	shader_mapper shaders;
	texture_mapper textures;

	mesh_handler meshes;

	entity_handler entities;

	light_handler lights;
	time_handler timer;

	layer main_layer;

public:

	application(void);

	auto init(void) -> void;
	auto render(void) -> void;
	auto update(void) -> void;
	auto running(void) -> bool;

private:

	auto init_data(void) -> void;
	auto init_meshes(void) -> void;
	auto init_shaders(void) -> void;
	auto init_textures(void) -> void;
	auto init_layers(void) -> void;

};
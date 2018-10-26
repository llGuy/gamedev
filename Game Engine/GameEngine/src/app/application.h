#pragma once

#include "../api/api.h"
#include "../time/timer.h"
#include "../scene/scene.h"
#include "../window/window.h"
#include "../graphics/shader/shader_handler.h"
#include "../graphics/texture/texture_handler.h"

#include "../graphics/2D/batch_renderer2D.h"
#include "../graphics/3D/renderer/renderer3D.h"

class application
{
private:
	bool is_running;
	window display;
	scene world;
	timer time_handler;
	model_handler models;
	shader_handler shaders;
	texture_handler textures;
	light_handler lights;

	renderer3D renderer;
	renderer3D sky_renderer;

	model monkey_model;
	model cube_model;

	texture * low_poly_texture;
	texture * sky_texture;
public:
	application(void);

	auto init(void) -> void;
	auto update(void) -> void;
	auto render(void) -> void;
	auto running(void) -> bool;
	auto clean_up(void) -> void;
private:
	auto init_game_objects(void) -> void;
	auto init_3D_test(void) -> void;
	auto init_models(void) -> void;
	auto init_shaders(void) -> void;
	auto init_textures(void) -> void;
};
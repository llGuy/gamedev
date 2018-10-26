#pragma once

#include "../api/api.h"
#include "../time/timer.h"
#include "../scene/scene.h"
#include "../window/window.h"
#include "../graphics/3D/model_handler.h"
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

	glsl_program * low_poly_shader;
	model monkey_model;
	texture * low_poly_texture;
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
};
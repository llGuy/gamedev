#pragma once

#include "../api/api.h"
#include "../time/timer.h"
#include "../scene/scene.h"
#include "../window/window.h"
#include "../graphics/2D/gui/panel/panel.h"
#include "../graphics/shader/shader_handler.h"
#include "../graphics/texture/texture_handler.h"
#include "../graphics/2D/gui/font/font_handler.h"

#include "../graphics/2D/gui/gui_handler.h"
#include "../graphics/2D/renderer/batch_renderer2D.h"

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
	//font_handler fonts;
	gui_handler guis;

	renderer3D renderer;
	renderer3D sky_renderer;
	//batch_renderer2D gui_renderer;

	model monkey_model;
	model cube_model;

	//gui * gui_panel;

	//font_stream * text;
	//font_stream * text2;
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
	auto init_2D_test(void) -> void;
	auto init_models(void) -> void;
	auto init_shaders(void) -> void;
	auto init_textures(void) -> void;
	auto init_fonts(void) -> void;
};
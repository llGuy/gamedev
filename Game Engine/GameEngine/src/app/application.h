#pragma once

#include "../api/api.h"
#include "../time/timer.h"
#include "../scene/scene.h"
#include "../window/window.h"
#include "../graphics/pipeline/pipeline.h"
#include "../graphics/2D/gui/panel/panel.h"
#include "../graphics/visual/shadow_handler.h"
#include "../graphics/shader/shader_handler.h"
#include "../graphics/texture/texture_handler.h"
#include "../graphics/2D/gui/font/font_handler.h"
#include "../graphics/3D/material/material_handler.h"
#include "../graphics/renderbuffers/renderbuffer_handler.h"

#include "../graphics/2D/gui/gui_handler.h"
#include "../graphics/2D/renderer/batch_renderer2D.h"

#include "../animation/animation_handler.h"

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
	gui_handler guis;
	material_handler materials;
	shadow_handler shadows;
	renderbuffer_handler renderbuffers;
	skeletal_animation_handler animations;

	pipeline render_pipeline;

	model platform_model;
	model monkey_model;
	model cube_model;
	model player_model;
	model quad3D_model;
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
	auto init_pipeline(void) -> void;
};
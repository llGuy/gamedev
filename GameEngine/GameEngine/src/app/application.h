#pragma once

#include "../api/api.h"
#include "../time/timer.h"
#include "../scene/scene.h"
#include "../window/window.h"
#include "../graphics/pipeline/pipeline.h"
#include "../graphics/2D/gui/panel/panel.h"
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
	renderbuffer_handler renderbuffers;
	skeletal_animation_handler animations;

	pipeline render_pipeline;

//	model platform_model;
	model cube_model;
	model player_model;
	
	struct
	{
		model cube;
		model sphere;
		model taurus;
		model monkey;
	} platform;

	uniform_mat4 * inverse_proj_matrix;
	uniform_mat4 * inverse_view_matrix;
	uniform_mat4 * previous_view_proj;
	uniform_float * current_fps;
	uniform_mat4 * view_matrix_command;
	uniform_vec3 * cam_pos_command;
	uniform_int * num_marches;
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
	auto init_animation(void) -> void;
	auto init_2D_test(void) -> void;
	auto init_models(void) -> void;
	auto init_shaders(void) -> void;
	auto init_textures(void) -> void;
	auto init_fonts(void) -> void;
	auto init_pipeline(void) -> void;
private:
	/* initialize pipeline stages */
	auto init_shadow_pass(void) -> void;
	auto init_scene_pass(void) -> void;
	auto init_blur_passes(void) -> void;
	auto init_dof_pass(void) -> void;
	auto init_bloom_pass(void) -> void;
	auto init_motion_blur_pass(void) -> void;
	auto init_light_pass(void) -> void;
	auto init_final_pass(void) -> void;

	auto init_deferred_renderer(void) -> void;
	auto init_ssr(void) -> void;
};
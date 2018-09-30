#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "ecs/ecs.h"
#include "window.h"
#include "gui.h"
#include "entity.h"
#include "quad_3D.h"
#include "quad_2D.h"
#include "resources.h"
//#include "shadows.h"
#include "shadow_handler.h"
#include "puff_handler.h"
#include "input_handler.h"
#include "model_handler.h"
#include "tracer_handler.h"
#include "timer.h"
#include "types.h"
#include "camera.h"
#include "gradient_sky.h"
#include "post_processing/vblur_stage.h"
#include "post_processing/hblur_stage.h"
#include "post_processing/default_stage.h"
#include "post_processing/depth_of_field.h"

struct blur_stage { hblur_stage h; vblur_stage v; };

class application
{
public:
	application(i32 w, i32 h);

	auto init(void) -> void;
	auto init_window(void) -> void;
	auto render(void) -> void;
	auto update(void) -> void;
	auto is_running(void) -> bool;
	auto destroy(void) -> void;
private:
	struct render_params { program & shaders; u32 model_matrix_id; u32 color_id; };
	auto render_depth(void) -> void;
	auto render_depth_gui(void) -> void;

	auto create_test_fbo(void) -> void;
	auto add_entity(glm::vec3 const & p, glm::vec3 const & d, glm::vec3 const & scale) -> void;

	auto create_textures(void) -> void;
private:
	window appl_window;
	resource_handler resources;
	timer time;
	glm::mat4 projection_matrix;

	model_instance tree_model_instance;
	model_instance rock_model_instance;
	model_instance rock_model_instance2;
	texture low_poly_map;

	model_handler model_loader;
	gui_handler guis;
	entity_handler entities;
	shadow_handler shadows;
	puff_handler puffs;

	/* for bullets */
	tracer_handler traces;
	program quad_3D_shaders;
	program quad_2D_shaders;
	quad_3D scene_platform;
	gui_quad render_quad;

	model_instance cube_model_instance;

	gradient_sky sky;

	/* render pipeline */
	default_stage default_target;
	std::array<blur_stage, 2> blur_stages;
	depth_of_field dof_stage;

	glm::vec3 light_position;
	glm::mat4 shadow_bias;

	framebuffer test_fbo;
	texture test_tex;

	bool running;
};
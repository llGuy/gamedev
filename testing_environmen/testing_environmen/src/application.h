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
#include "cube.h"
#include "resources.h"
//#include "shadows.h"
#include "shadow_handler.h"
#include "input_handler.h"
#include "timer.h"
#include "types.h"
#include "camera.h"

class application
{
public:
	application(i32 w, i32 h);

	auto init(void) -> void;
	auto init_window(void) -> void;
	auto render(void) -> void;
	auto update(void) -> void;
	auto running(void) -> bool;
	auto destroy(void) -> void;
private:
	auto render_depth(void) -> void;
	auto render_color(void) -> void;
	auto render_depth_gui(void) -> void;

	auto create_test_fbo(void) -> void;
	auto add_entity(glm::vec3 const & p, glm::vec3 const & d) -> void;
private:
	window appl_window;
	resource_handler resources;
	timer time;
	glm::mat4 projection_matrix;

	gui_handler guis;
	entity_handler entities;
	shadow_handler shadows;
	program quad_3D_shaders;
	program quad_2D_shaders;
	quad_3D scene_platform;
	quad_2D gui_quad;
	cube a_cube;

	glm::vec3 light_position;
	glm::mat4 shadow_bias;

	framebuffer test_fbo;
	texture test_tex;
};
#ifndef _SCENE_GAME_STATE_H_
#define _SCENE_GAME_STATE_H_

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game_state.h"
#include "program.h"
#include "camera.h"
#include "quad.h"
#include "cube.h"
#include "terrain.h"
#include "resources.h"
#include "shadows.h"
#include "water.h"
#include "gui.h"
#include "water.h"

class scene_state
	: public game_state
{
public:
	scene_state(int32_t w, int32_t h, glm::vec2 const & cursor_pos, resource_handler &);

	auto render(void) -> void override;
	auto update(input_handler &, timer &) -> game_state * override;
private:
	auto render_scene(glm::mat4 & view) -> void;
	auto render_depth(void) -> void;
	auto render_depth_gui(void) -> void;

	auto connect_texture_units(void) -> void;

	auto render_player(program & shaders, uint32_t udata_index) -> void;
private:
	glm::ivec2 resolution;
	glm::mat4 projection_matrix;

	camera main_camera;
	program terrain_program;
	program cube_program;
	terrain<256, 256> scene_terrain;
	cube test_cube;
	shadows shadow_handler;
	gui_handler guis;
	water water_handler;

	std::array<glm::vec3, 10> cube_positions;
};

#endif

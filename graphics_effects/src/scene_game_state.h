#ifndef _SCENE_GAME_STATE_
#define _SCENE_GAME_STATE_

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game_state.h"
#include "program.h"
#include "camera.h"
#include "quad.h"

class scene_state
	: public game_state
{
public:
	scene_state(int32_t w, int32_t h, glm::vec2 const & cursor_pos);

	auto render(void) -> void override;
	auto update(input_handler &, timer &) -> game_state * override;
private:
	auto render_scene(void) -> void;
private:
	glm::ivec2 resolution;
	glm::mat4 projection_matrix;

	camera main_camera;
	program scene_program;
	quad_3D test_quad;
};

#endif
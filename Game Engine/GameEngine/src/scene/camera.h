#pragma once

#include <glm/glm.hpp>
#include "../utils/types.h"
#include <glm/gtx/transform.hpp>

#include "game_object.h"

class camera
{
private:
	glm::mat4 view_matrix;

	glm::vec3 position;
	glm::vec3 direction;

	i32 bound_game_object{ -1 };
public:
	camera(void) = default;

	auto init(i32 bound_game_object) -> void;

	/* needs vector of objects to access object with index */
	auto update_view_matrix(vector_dyndel<game_object> & objects) -> void;

	auto get_view_matrix(void) -> glm::mat4 &;
	auto get_position(void) -> glm::vec3 &;
	auto get_direction(void) -> glm::vec3 &;
	auto get_bound_object(void)-> i32 &;
};

extern auto look_fps(glm::vec2 const & cursor_dif, glm::vec3 const & direction, f32 elapsed)->glm::vec3;

extern auto invert_matrix(camera & cam)->glm::mat4;
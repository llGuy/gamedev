#pragma once

#include <glm/glm.hpp>
#include "../utils/types.h"
#include <glm/gtx/transform.hpp>

#include "game_object.h"

class camera
{
private:
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;

	glm::vec3 position;
	glm::vec3 direction{ 1, 0, 0 };

	i32 bound_game_object{ -1 };

	bool is_fp{ true };

	glm::vec2 angles_third_person{ 0 };
public:
	camera(void) = default;

	auto init(i32 bound_game_object) -> void;

	/* needs vector of objects to access object with index */
	auto update_view_matrix(vector_dyndel<game_object> & objects) -> void;

	auto toggle_fp(void) -> void;

	auto get_projection_matrix(void) -> glm::mat4 &;
	auto get_view_matrix(void) -> glm::mat4 &;
	auto get_view_matrix_without_translation(void) const -> glm::mat4;
	auto get_position(void) -> glm::vec3 &;
	auto get_direction(void) -> glm::vec3 &;
	auto get_bound_object(void)-> i32 &;
	auto is_first_person(void) const -> bool;
	auto get_rotation_angles(void) -> glm::vec2 &;
};

extern auto look_fps(glm::vec2 const & cursor_dif, glm::vec3 const & direction, f32 elapsed)->glm::vec3;

extern auto invert_matrix(camera & cam)->glm::mat4;
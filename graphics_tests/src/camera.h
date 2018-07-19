
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "input_handler.h"
#include "detail.h"

class camera
{
public:
	camera(glm::vec2 const & cursor_xy);

	auto position(void) -> glm::vec3 &;
	auto direction(void) -> glm::vec3 &;
	auto cursor_position(void) -> glm::vec2 &;
	auto look_at(glm::vec2 const & cursor) -> void;

	auto view_matrix(void) -> glm::mat4 &;
	auto update_view_matrix(void) -> glm::mat4 &;
	auto handle_input(input_handler &, float time_delta) -> void;
private:
	glm::vec2 current_cursor_position;

	glm::vec3 world_position;
	glm::vec3 world_direction;
	glm::mat4 camera_view_matrix;
	float camera_speed;
};

#endif

#pragma once

#include "ecs.h"
#include "../utils/types.h"

#include <glm/glm.hpp>

class camera
{
public:
	camera(void) = default;

	auto bind_entity(i32 index, vec_dd<entity> & entities) -> void;
	auto update_view_matrix(vec_dd<entity> & entities) -> void;

	inline auto bound_entity(void) -> i32 { return bound_entity_index; };
	inline auto matrix(void) -> glm::mat4 & { return view_matrix; }
	inline auto pos(void) -> glm::vec3 & { return position; }
	inline auto dir(void) -> glm::vec3 & { return direction; }

	auto get_yaw(void)->f32 &;
	auto get_pitch(void)->f32 &;
private:
	glm::mat4 view_matrix;

	f32 yaw{ 0 };
	f32 pitch{ 0 };

	glm::vec3 position;
	glm::vec3 direction;

	/* using indices because dealing with vectors */
	i32 bound_entity_index{ -1 };

	i32 height_component_index{ 0 };
};

extern auto look_fps(glm::vec2 const & cursor_dif,
	glm::vec3 const & direction,
	f32 elapsed)->glm::vec3;

extern auto invert_matrix(camera & cam)->glm::mat4;
extern auto pitch_yaw(std::pair<f32, f32> const & pair, glm::vec2 const & diff)->std::pair<f32, f32>;
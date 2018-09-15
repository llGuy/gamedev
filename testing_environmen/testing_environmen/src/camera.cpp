#include "camera.h"
#include "detail.h"
#include <utility>
#include "ecs/basic_components.h"
#include <glm/gtx/transform.hpp>

#define SENSITIVITY 0.02f

auto look_fps(glm::vec2 const & cursor_dif, 
	glm::vec3 const & direction, 
	f32 elapsed) -> glm::vec3
{
	using detail::up;

	glm::vec3 res = direction;

	f32 x_angle = glm::radians(-cursor_dif.x) * SENSITIVITY;// *elapsed;
	f32 y_angle = glm::radians(-cursor_dif.y) * SENSITIVITY;// *elapsed;
	res = glm::mat3(glm::rotate(x_angle, up)) * res;
	glm::vec3 rotate_y = glm::cross(res, up);
	res = glm::mat3(glm::rotate(y_angle, rotate_y)) * res;

	return res;
}

auto pitch_yaw(std::pair<f32, f32> const & pair, glm::vec2 const & diff) -> std::pair<f32, f32>
{
	f32 pitch_change = diff.y * SENSITIVITY;
	f32 new_pitch = pair.first + pitch_change;

	f32 yaw_change = diff.x * SENSITIVITY;
	f32 new_yaw = pair.second + yaw_change;

	return std::make_pair( new_pitch, new_yaw );
}

auto invert_matrix(camera & cam) -> glm::mat4
{
	using detail::up;
	glm::vec3 inverted_position = cam.pos();
	glm::vec3 inverted_direction = cam.dir();
	inverted_position.y *= -1;
	inverted_direction.y *= -1;

	return glm::lookAt(inverted_position, inverted_position + inverted_direction, up);
}

auto camera::bind_entity(i32 index, vec_dd<entity> & entities) -> void
{
	bound_entity_index = index;
	height_component_index = entities[bound_entity_index].get_component_index<height>();
}

auto camera::update_view_matrix(vec_dd<entity> & entities, entity_cs & ecs) -> void
{
	entity_data & data = entities[bound_entity_index].get_data();

	if (bound_entity_index == -1) view_matrix = glm::lookAt(glm::vec3(0), glm::vec3(0.1, 0.1, 1.0), glm::vec3(0, 1, 0));
	else
	{
		(position = data.pos).y += ecs.get_component<height>(height_component_index).value.val + 2.0f;
		direction = data.dir;

		view_matrix = glm::lookAt(position, position + direction, detail::up);
	}
}

auto camera::get_yaw(void) -> f32 &
{
	return yaw;
}

auto camera::get_pitch(void) -> f32 &
{
	return pitch;
}
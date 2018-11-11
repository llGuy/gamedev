#include "camera.h"
#include "../utils/detail.h"

auto camera::init(i32 bound_game_object) -> void
{
	this->bound_game_object = bound_game_object;
}

/* needs vector of objects to access object with index */
auto camera::update_view_matrix(vector_dyndel<game_object> & objects) -> void
{
	auto & bound_object = objects[bound_game_object];

	if (bound_game_object == -1)
	{
		view_matrix = glm::lookAt(glm::vec3(0), glm::vec3(0.1f, 0.1f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		position = bound_object->position;
		direction = bound_object->direction;

		view_matrix = glm::lookAt(position, position + direction, detail::up);
	}
}

auto camera::get_projection_matrix(void)->glm::mat4 &
{
	return projection_matrix;
}

auto camera::get_view_matrix(void)->glm::mat4 &
{
	return view_matrix;
}

auto camera::get_view_matrix_without_translation(void) const -> glm::mat4
{
	glm::mat4 vmatrix = view_matrix;

	vmatrix[3][0] = 0.0f;
	vmatrix[3][1] = 0.0f;
	vmatrix[3][2] = 0.0f;

	return vmatrix;
}

auto camera::get_position(void)->glm::vec3 &
{
	return position;
} 

auto camera::get_direction(void)->glm::vec3 &
{
	return direction;
} 

auto camera::get_bound_object(void)->i32 &
{
	return bound_game_object;
}

#define SENSITIVITY 0.02f

auto look_fps(glm::vec2 const & cursor_dif, glm::vec3 const & direction, f32 elapsed)->glm::vec3
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

auto invert_matrix(camera & cam)->glm::mat4
{
	using detail::up;
	glm::vec3 inverted_position = cam.get_position();
	glm::vec3 inverted_direction = cam.get_direction();
	inverted_position.y *= -1;
	inverted_direction.y *= -1;

	return glm::lookAt(inverted_position, inverted_position + inverted_direction, up);
}
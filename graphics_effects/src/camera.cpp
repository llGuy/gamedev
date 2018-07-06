#include "camera.h"


camera::camera(glm::vec2 const & cursor_xy)
	: current_cursor_position(cursor_xy),
	world_position(0.001f, 0.001f, 0.001f),
	world_direction(1.0f, 0.001f, 0.001f)
{
}

auto camera::cursor_position(void) -> glm::vec2 &
{
	return current_cursor_position;
}
auto camera::look_at(glm::vec2 const & cursor) -> void
{
	static constexpr glm::vec3 up_vector { 0.0f, 1.0f, 0.0f };

	glm::vec2 cursor_position_diff = cursor - current_cursor_position;
	current_cursor_position = cursor;

	float x_angle = glm::radians(-cursor_position_diff.x) * 0.02f;
	float y_angle = glm::radians(-cursor_position_diff.y) * 0.02f;

	world_direction = glm::mat3(glm::rotate(x_angle, up_vector)) * world_direction;
	
	glm::vec3 rotate_y = glm::cross(world_direction, up_vector);
	world_direction = glm::mat3(glm::rotate(y_angle, rotate_y)) * world_direction;
}
	 
auto camera::view_matrix(void) -> glm::mat4 &
{
	return camera_view_matrix;
}
auto camera::update_view_matrix(void) -> glm::mat4 &
{
	static constexpr glm::vec3 up_vector{ 0.0f, 1.0f, 0.0f };
	return (camera_view_matrix = glm::lookAt(world_position, world_position + world_direction, up_vector));
}

auto camera::handle_input(input_handler & ih, float time_delta) -> void
{
	static constexpr float speed = 20.0f;
	static constexpr glm::vec3 up_vector{ 0.0f, 1.0f, 0.0f };

	glm::vec3 direction { 0.0f };
	if (ih.key(GLFW_KEY_W)) direction += world_direction;
	if (ih.key(GLFW_KEY_A)) direction += -glm::cross(world_direction, up_vector);
	if (ih.key(GLFW_KEY_S)) direction += -world_direction;
	if (ih.key(GLFW_KEY_D)) direction += glm::cross(world_direction, up_vector);
	if (ih.key(GLFW_KEY_SPACE)) direction += up_vector;
	if (ih.key(GLFW_KEY_LEFT_SHIFT)) direction += -up_vector;

	world_position += direction * time_delta * speed;
}
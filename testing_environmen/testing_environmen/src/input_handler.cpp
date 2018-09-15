#include "input_handler.h"
#include "detail.h"

input_handler::input_handler(void)
	: key_map{ 0 }, mouse_button_map{ 0 }, window_resized(0), mouse_moved(false), current_cursor_position(0)
{
}

auto input_handler::got_key(int32_t glfw_key) -> bool &
{
	return key_map[glfw_key];
}

auto input_handler::win_resized(void) -> bool &
{
	return window_resized;
}

auto input_handler::cursor_moved(void) -> bool &
{
	return mouse_moved;
}

auto input_handler::got_mouse_button(int32_t glfw_mb) -> bool &
{
	return mouse_button_map[glfw_mb];
}

auto input_handler::window_size(void)->glm::vec2
{
	using detail::construct;
	return construct<glm::vec2, float>(w, h);
}

auto input_handler::handle_window_resize(GLFWwindow * win, int32_t w, int32_t h) -> void
{
	input_handler* ih = (input_handler*)glfwGetWindowUserPointer(win);
	ih->w = w;
	ih->h = h;
	ih->win_resized() = true;
}

auto input_handler::handle_key_input(GLFWwindow * win, int32_t key, int32_t scancode, int32_t action, int32_t mods) -> void
{
	if (key < max_keys)
	{
		input_handler * ih = (input_handler *)glfwGetWindowUserPointer(win);
		if (action == GLFW_PRESS) ih->key_map[key] = true;
		else if (action == GLFW_RELEASE) ih->key_map[key] = false;
	}
}

auto input_handler::handle_mouse_movement(GLFWwindow * win, double x, double y) -> void
{
	using detail::construct;
	input_handler* ih = (input_handler *)glfwGetWindowUserPointer(win);
	ih->current_cursor_position = construct<glm::vec2, float>(x, y);
	ih->mouse_moved = true;
}

auto input_handler::handle_mouse_input(GLFWwindow * win, int32_t button, int32_t action, int32_t mods) -> void
{
	input_handler* ih = (input_handler *)glfwGetWindowUserPointer(win);
	if (action == GLFW_PRESS) ih->mouse_button_map[button] = true;
	else if (action == GLFW_RELEASE) ih->mouse_button_map[button] = false;
}

auto input_handler::cursor_position(void) -> glm::vec2 &
{
	return current_cursor_position;
}
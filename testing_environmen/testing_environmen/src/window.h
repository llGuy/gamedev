#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include "input_handler.h"

extern void glfw_init(void);
extern void glfw_terminate(void);
extern void glew_init(void);

using mb_callback_func = GLFWmousebuttonfun;
using key_callback_func = GLFWkeyfun;
using cursor_mv_callback_func = GLFWcursorposfun;
using window_resize_callback_func = GLFWwindowsizefun;

class window
{
public:
	window(int32_t w, int32_t h, std::string const & title);

	auto init(void) -> void;
	auto destroy(void) -> void;
	auto refresh(void) -> void;
	auto is_open(void) const -> bool;
	auto received_key(int32_t glfw_key) const -> bool;
public:
	auto pixel_width(void)->int32_t &;
	auto pixel_height(void)->int32_t &;
	auto cursor_position(void)->glm::vec2;
	auto user_inputs(void)->input_handler &;
public:
	auto launch_input_handler(void) -> void;
	auto window_hint(int32_t, int32_t) -> void;
	auto set_window_input_mode(int32_t mode, int32_t val) -> void;
	auto set_key_callback(key_callback_func func) -> void;
	auto set_mouse_button_callback(mb_callback_func func) -> void;
	auto set_cursor_mv_callback(cursor_mv_callback_func func) -> void;
	auto set_window_resize_callback(window_resize_callback_func func) -> void;

	template <typename T> auto set_user_pointer(T * ptr = nullptr) -> void;
private:
	int32_t width;
	int32_t height;
	std::string title;
	GLFWwindow* glfw_window;
	std::unique_ptr<input_handler> inputs;
};

template <typename T> auto window::set_user_pointer(T * ptr) -> void
{
	glfwSetWindowUserPointer(glfw_window, (!ptr) ? (T *)this : ptr);
}

#pragma once

#include <array>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "types.h"

class input_handler
{
public:
	input_handler(void);

	auto got_key(int32_t glfw_key) -> bool &;
	auto win_resized(void) -> bool &;
	auto cursor_moved(void) -> bool &;
	auto got_mouse_button(int32_t glfw_mb) -> bool &;
	auto window_size(void) -> glm::vec2;
	auto cursor_position(void) -> glm::vec2 &;
public:
	static auto handle_window_resize(GLFWwindow * win, int32_t w, int32_t h) -> void; 
	static auto handle_key_input(GLFWwindow * win, int32_t key, int32_t scancode, int32_t action, int32_t mods) -> void;
	static auto handle_mouse_movement(GLFWwindow * win, double x, double y) -> void;
	static auto handle_mouse_input(GLFWwindow * win, int32_t button, int32_t action, int32_t mods) -> void;
private:
	static constexpr u32 max_keys = 350;
	static constexpr u32 max_mouse_buttons = 5;

	std::array<bool, max_keys> key_map;
	std::array<bool, max_mouse_buttons> mouse_button_map;

	glm::vec2 current_cursor_position;
	bool mouse_moved;
	bool window_resized;

	int32_t w, h;
};
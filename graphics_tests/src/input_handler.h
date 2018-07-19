#ifndef _INPUT_HANDLER_
#define _INPUT_HANDLER_

#include <array>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class input_handler
{
public:
	input_handler(void);

	auto key(int32_t glfw_key) -> bool &;
	auto resized(void) -> bool &;
	auto cursor_moved(void) -> bool;
	auto mouse_button(int32_t glfw_mb) -> bool &;
	auto cursor_position(void) -> glm::vec2 &;
	auto window_size(void) -> glm::vec2;
public:
	static auto handle_window_resize(GLFWwindow * win, int32_t w, int32_t h) -> void;
	static auto handle_key_input(GLFWwindow * win, int32_t key, int32_t scancode, int32_t action, int32_t mods) -> void;
	static auto handle_mouse_movement(GLFWwindow* win, double x, double y) -> void;
	static auto handle_mouse_input(GLFWwindow * win, int32_t button, int32_t action, int32_t mods) -> void;
private:
	static constexpr uint32_t max_keys = 350;
	static constexpr uint32_t max_mouse_buttons = 5;

	std::array<bool, max_keys> key_map;
	std::array<bool, max_mouse_buttons> mouse_button_map;

	glm::vec2 current_cursor_position;
	bool cursor_mvd;
	bool window_resized;

	int32_t width;
	int32_t height;
};

#endif
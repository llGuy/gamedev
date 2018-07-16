#include <GL/glew.h>
#include "window.h"
#include "log.h"

void glfw_init(void)
{
	if (!glfwInit()) logger::error_log("unable to initialize GLFW");
	else logger::sub_log("initialized GLFW successfully");
}
void glfw_terminate(void)
{
	glfwTerminate();
}
void glew_init(void)
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) logger::error_log("unable to initialize GLEW");
	else logger::sub_log("initialized GLEW successfully");
}

window::window(int32_t w, int32_t h, std::string const & t)
	: width(w), height(h), title(t)
{
}
auto window::init(void) -> void
{
	if (!(glfw_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)))
		logger::error_log("unable to initialize window");
	else logger::sub_log("initialized window successfully");
	glfwMakeContextCurrent(glfw_window);
}
auto window::destroy(void) -> void
{
	glfwDestroyWindow(glfw_window);
}
auto window::refresh(void) -> void
{
	glfwSwapBuffers(glfw_window);
	glfwPollEvents();
}
auto window::is_open(void) const -> bool
{
	return !glfwWindowShouldClose(glfw_window) && !received_key(GLFW_KEY_ESCAPE);
}
auto window::received_key(int32_t glfw_key) const -> bool
{
	return glfwGetKey(glfw_window, glfw_key);
}
auto window::pixel_width(void) -> int32_t &
{
	return width;
}
auto window::pixel_height(void) -> int32_t &
{
	return height;
}
auto window::cursor_position(void) -> glm::vec2
{
	double x, y;
	glfwGetCursorPos(glfw_window, &x, &y);
	return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}
auto window::window_hint(int32_t hint, int32_t value) -> void
{
	glfwWindowHint(hint, value);
}
auto window::set_window_input_mode(int32_t mode, int32_t val) -> void
{
	glfwSetInputMode(glfw_window, mode, val);
}
auto window::set_key_callback(key_callback_func func) -> void
{
	glfwSetKeyCallback(glfw_window, func);
}
auto window::set_mouse_button_callback(mb_callback_func func) -> void
{
	glfwSetMouseButtonCallback(glfw_window, func);
}
auto window::set_cursor_mv_callback(cursor_mv_callback_func func) -> void
{
	glfwSetCursorPosCallback(glfw_window, func);
}
auto window::set_window_resize_callback(window_resize_callback_func func) -> void
{
	glfwSetWindowSizeCallback(glfw_window, func);
}
auto window::user_inputs(void)->input_handler &
{
	return *inputs;
}
auto window::launch_input_handler(void) -> void
{
	inputs = std::make_unique<input_handler>();
	set_user_pointer(inputs.get());
	set_key_callback(input_handler::handle_key_input);
	set_mouse_button_callback(input_handler::handle_mouse_input);
	set_cursor_mv_callback(input_handler::handle_mouse_movement);
	set_window_resize_callback(input_handler::handle_window_resize);
}
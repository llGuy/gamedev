#pragma once

#include "../utils/Types.h"

#include <string>
#include <GLFW/glfw3.h>

namespace glfw {

	using mouse_callback = GLFWmousebuttonfun;
	using key_callback = GLFWkeyfun;
	using cursorpos_callback = GLFWcursorposfun;
	using resize_callback = GLFWwindowsizefun;



	class window
	{
	private:
		i32 width;
		i32 height;

		std::string title;

		GLFWwindow * glfw_window;
	public:
		window(i32 width, i32 height, std::string const & title);
	};

}

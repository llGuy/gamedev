#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GLFW/glfw3.h>

namespace tmpg {

	class Window
	{
	public:
		Window(const char* title, int32_t width, int32_t height);
	};

}

#endif
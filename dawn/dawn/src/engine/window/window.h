#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <GLFW/glfw3.h>
#include <stdint.h>

namespace dawn { namespace window {
	
		class Window
		{
		public:
			Window(void) = default;
			Window(const Window& w) = delete;
			const Window& operator=(const Window& w) = delete;
			Window(const char* title, const int32_t& width, const int32_t& height);
		};

	} 
}

#endif
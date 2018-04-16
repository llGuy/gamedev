#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include "../../utils/logger/error.h"
#include "../../utils/logger/log.h"
#include "../engine.h"

namespace dawn { 
	
	class Window
	{
	public:
		Window(void) = default;
		Window(const Window& w) = delete;
		const Window& operator=(const Window& w) = delete;
		Window(const char* title, int32_t width, int32_t height);

		void Init(DawnEngine* ptr);
		
		void Destroy(void);
		void Update(void) const;
		const bool Open(void) const noexcept;
		const bool ValidWindow(void) const noexcept;

		int32_t Width(void) const noexcept;
		int32_t Height(void) const noexcept;

		glm::vec2 CursorPosition(void) const;
	private:
		void WindowInit(void);
		void MouseCallback(uint32_t button);
		void KeyboardCallback(uint32_t key);
		void PollKeys(void) const;
		void PollMouseMovement(void) const;
		void PollMouseInput(void) const;
	private:
		int32_t m_width;
		int32_t m_height;
		GLFWwindow* m_glfwWindow;
		DawnEngine* m_engineptr;
		const char* m_title;
	};

} 

#endif
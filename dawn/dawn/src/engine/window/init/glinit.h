#ifndef GL_INIT_HEADER
#define GL_INIT_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace dawn {

	class GLFWInitError
		: std::exception
	{
	public:
		GLFWInitError(void) = default;

		const char* what(void) const throw() override
		{
			return "Failed to initialize GLFW";
		}
	};

	class GLEWInitError
		: std::exception
	{
	public:
		GLEWInitError(void) = default;

		const char* what(void) const throw() override
		{
			return "Failed to initialize GLEW";
		}
	};

	class WindowInitError
		: std::exception
	{
	public:
		WindowInitError(void) = default;

		const char* what(void) const throw() override
		{
			return "Failed to initialize window";
		}
	};

	inline
		void GLEWInit(void)
	{
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			glfwTerminate();
			throw GLEWInitError{};
		}
	}
	inline
		void GLFWInit(void)
	{
		if (!glfwInit())
		{
			glfwTerminate();
			throw GLFWInitError{};
		}
	}

}

#endif
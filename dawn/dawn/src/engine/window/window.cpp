#include "init/glinit.h"
#include "window.h"

namespace dawn {
	
	Window::Window(const char* title, const int32_t& width, const int32_t& height)
		: m_title(title), m_width(width), m_height(height)
	{
	}

	void Window::Init(void)
	{
		try
		{
			utils::NewLog("initializing window...");
			GLFWInit();
			utils::Log("GLFW Initializing SUCCESS");
			WindowInit();
			utils::Log("Window Initializing SUCCESS");
			GLEWInit();
			utils::Log("Window Initializing SUCCESS");
		}
		catch (const GLFWInitError& e)
		{
			utils::Error(e.what());
			m_glfwWindow = nullptr;
		}
		catch (const GLEWInitError& e)
		{
			utils::Error(e.what());
			m_glfwWindow = nullptr;
		}
		catch (const WindowInitError& e)
		{
			utils::Error(e.what());
			m_glfwWindow = nullptr;
		}
	}
	void Window::WindowInit(void)
	{
		m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
		glfwMakeContextCurrent(m_glfwWindow);
		if (!m_glfwWindow)
		{
			glfwTerminate();
			throw WindowInitError{};
		}
		//glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetWindowUserPointer(m_glfwWindow, this);
		//glfwSetMouseButtonCallback(m_glfwWindow, CursorEnterCallback);
		//glfwSetKeyCallback(m_glfwWindow, KeyboardEnterCallback);
	}
	const int32_t& Window::Width(void) noexcept
	{
		return m_width;
	}
	const int32_t& Window::Height(void) noexcept
	{
		return m_height;
	}

	void Window::Destroy(void)
	{
		glfwDestroyWindow(m_glfwWindow);
		glfwTerminate();
	}
	void Window::Update(void) const
	{
		glfwSwapBuffers(m_glfwWindow);
		glfwPollEvents();
		PollKeys();
		PollMouseMovement();
	}
	const bool Window::Open(void) const noexcept
	{
		return !glfwWindowShouldClose(m_glfwWindow);
			// && m_glfwWindow != nullptr;
	}
	const bool Window::ValidWindow(void) const noexcept
	{
		return m_glfwWindow != nullptr;
	}

	void Window::MouseCallback(uint32_t button)
	{

	}
	void Window::KeyboardCallback(uint32_t key)
	{

	}
	void Window::PollKeys(void) const
	{

	}
	void Window::PollMouseMovement(void) const
	{

	}
	void Window::PollMouseInput(void) const
	{

	}

} 
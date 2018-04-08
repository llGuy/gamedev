#include "init/glinit.h"
#include "window.h"

namespace dawn {
	
	Window::Window(const char* title, int32_t width, int32_t height)
		: m_title(title), m_width(width), m_height(height)
	{
	}

	void Window::Init(DawnEngine* ptr)
	{
		m_engineptr = ptr;

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
		glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetWindowUserPointer(m_glfwWindow, this);
		//glfwSetMouseButtonCallback(m_glfwWindow, CursorEnterCallback);
		//glfwSetKeyCallback(m_glfwWindow, KeyboardEnterCallback);
	}
	int32_t Window::Width(void) const noexcept
	{
		return m_width;
	}
	int32_t Window::Height(void) const noexcept
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
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_W)) m_engineptr->RecieveAction(action_t::W);
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_A)) m_engineptr->RecieveAction(action_t::A);
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_S)) m_engineptr->RecieveAction(action_t::S);
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_D)) m_engineptr->RecieveAction(action_t::D);
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE)) m_engineptr->RecieveAction(action_t::SPACE);
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT)) m_engineptr->RecieveAction(action_t::LSHIFT);
	}
	glm::vec2 Window::CursorPosition(void) const
	{
		double x, y;
		glfwGetCursorPos(m_glfwWindow, &x, &y);
		glm::vec2 pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
		return pos;
	}
	void Window::PollMouseMovement(void) const
	{
		m_engineptr->MouseCursor(CursorPosition());
	}
	void Window::PollMouseInput(void) const
	{

	}

} 
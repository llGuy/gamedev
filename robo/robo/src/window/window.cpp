#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include "../engine/engine.h"
#include "../log.h"
#include "window.h"

void CursorEnterCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* p = (Window*)(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
	{
		p->MouseButtonInput(button);
	}
}

static void KeyCallback(GLFWwindow* window, int key, int, int action, int)
{
	Window* p = (Window*)(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_T)
		{
			if (p->CameraPlayerView())
				p->CameraPlayerView() = false;
			else p->CameraPlayerView() = true;
		}
	}
}
Window::Window(unsigned int width, unsigned int height, const char* title)
	: m_width(width), m_height(height), m_title(title)
{
	GLFWInit();
	WindowInit();
	GLEWInit();
	AfterGLEWInit();
}
Window::~Window(void)
{
}
void Window::Draw(void)
{
	m_engine->Draw();
}
void Window::Update(void)
{
	glfwSwapBuffers(m_glfwWindow);
	glfwPollEvents();
	glViewport(0, 0, m_width, m_height);
	m_engine->KeyInput(m_glfwWindow);
	m_engine->MouseInput(m_glfwWindow);
}
const bool Window::WindowOpen(void)
{
	return !glfwWindowShouldClose(m_glfwWindow)
		&& !(glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE));
}
void Window::MouseButtonInput(int button)
{
	m_engine->MouseButtonInput(button);
}
void Window::WindowInit(void)
{
	m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	glfwMakeContextCurrent(m_glfwWindow);
	if (!m_glfwWindow)
	{
		Error("failed to initialize window");
		glfwTerminate();
		std::cin.get();
		exit(1);
	}
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_glfwWindow, this);
	glfwSetKeyCallback(m_glfwWindow, KeyCallback);
	glfwSetMouseButtonCallback(m_glfwWindow, CursorEnterCallback);
}
void Window::GLFWInit(void)
{
	if (!glfwInit())
	{
		Error("failed to initialize GLFW");
		glfwTerminate();
		std::cin.get();
		exit(1);
	}
}
void Window::GLEWInit(void)
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		Error("failed to initialize GLEW");
		glfwTerminate();
		std::cin.get();
		exit(1);
	}
}
bool& Window::CameraPlayerView(void)
{
	return m_cameraPlayerView;
}
void Window::AfterGLEWInit(void)
{
	glEnable(GL_DEPTH_TEST);

	double x, y;
	glfwGetCursorPos(m_glfwWindow, &x, &y);
	glm::vec2 position = glm::vec2(static_cast<float>(x), static_cast<float>(y));
	m_engine = new RoboEngine(static_cast<float>(m_width), static_cast<float>(m_height), position);
}
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include "window.h"
#include "../log.h"

Window::Window(unsigned int width, unsigned int height, const char* title)
	: m_width(width), m_height(height), m_title(title), m_engine()
{
	// all initializations
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
	m_engine.Render();
}
void Window::Update(void)
{
	glfwSwapBuffers(m_glfwWindow);
	glfwPollEvents();
	PollKeys();
	PollMouseMovement();
}
const bool Window::WindowOpen(void)
{
	return !glfwWindowShouldClose(m_glfwWindow)
		&& !(glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE));
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
void Window::AfterGLEWInit(void)
{
	glEnable(GL_DEPTH_TEST);

	double x, y;
	glfwGetCursorPos(m_glfwWindow, &x, &y);
	m_engine.AfterGLEWInit(m_width, m_height, glm::vec2(x, y), m_glfwWindow);
}
void Window::PollKeys(void) 
{
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_W))			m_engine.RecieveKeyInput(minecraft::Engine::key_t::W); 
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_A))			m_engine.RecieveKeyInput(minecraft::Engine::key_t::A);
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_S))			m_engine.RecieveKeyInput(minecraft::Engine::key_t::S);
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_D))			m_engine.RecieveKeyInput(minecraft::Engine::key_t::D);
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE))		m_engine.RecieveKeyInput(minecraft::Engine::key_t::SPACE);
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT))	m_engine.RecieveKeyInput(minecraft::Engine::key_t::LSHIFT);
}
void Window::PollMouseMovement(void)
{
	double x, y;
	glfwGetCursorPos(m_glfwWindow, &x, &y);
	m_engine.RecieveMouseMovement(
		glm::vec2(static_cast<float>(x), static_cast<float>(y)));
}
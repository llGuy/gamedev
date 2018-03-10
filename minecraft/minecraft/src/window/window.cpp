#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include "callbacks/callbacks.h"
#include "window.h"
#include "../log.h"

Window::Window(uint32_t width, uint32_t height, const char* title)
	: m_width(width), m_height(height), m_title(title), m_engine()
{
	// all initializations
	GLFWInit();
	WindowInit();
	GLEWInit();
	AfterGLEWInit();
}
void Window::Destroy(void)
{
	m_engine.HEAPDelete();
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
	//PollMouseInput();
}
void Window::MouseCallback(uint32_t b)
{
	switch (b)
	{
	case GLFW_MOUSE_BUTTON_1:
		m_engine.RecieveMouseInput(minecraft::Engine::mbutton_t::MOUSEL); return;
	case GLFW_MOUSE_BUTTON_2:
		m_engine.RecieveMouseInput(minecraft::Engine::mbutton_t::MOUSER); return;
	}
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
	glfwSetMouseButtonCallback(m_glfwWindow, CursorEnterCallback);
	glfwSetKeyCallback(m_glfwWindow, KeyboardEnterCallback);
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
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_R))			m_engine.RecieveKeyInput(minecraft::Engine::key_t::R);
	if(glfwGetKey(m_glfwWindow, GLFW_KEY_F))			m_engine.RecieveKeyInput(minecraft::Engine::key_t::F);
}
void Window::PollMouseMovement(void)
{
	double x, y;
	glfwGetCursorPos(m_glfwWindow, &x, &y);
	m_engine.RecieveMouseMovement(
		glm::vec2(static_cast<float>(x), static_cast<float>(y)));
}
void Window::PollMouseInput(void)
{
	if (glfwGetMouseButton(m_glfwWindow, GLFW_MOUSE_BUTTON_1)) 
		m_engine.RecieveMouseInput(minecraft::Engine::mbutton_t::MOUSEL);
}
void Window::KeyboardCallback(uint32_t k)
{
	minecraft::Engine::key_t key = static_cast<minecraft::Engine::key_t>((k - GLFW_KEY_1) + 
		static_cast<int32_t>(minecraft::Engine::key_t::ONE));

	m_engine.RecieveKeyInput(key);
}
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "callbacks/callbacks.h"
#include "window.h"

Window::Window(uint32_t width, uint32_t height, const char* title)
	: m_width(width), m_height(height), m_title(title), m_engine(m_width, m_height)
{
	// all initializations
	GLFWInit();
	WindowInit();
	GLEWInit();
	AfterGLEWInit();
}
void Window::Destroy(void)
{
}
void Window::Draw(void)
{
	m_engine.Draw();
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
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
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
		std::cout << "failed to initialize window" << std::endl;
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
		std::cout << "failed to initialize GLFW" << std::endl;
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
		std::cout << "failed to initialize GLEW" << std::endl;
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
	m_engine.Init();
}
void Window::PollKeys(void) 
{
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_W)) m_engine.Pl().MoveForward();
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_A)) m_engine.Pl().MoveLeft();
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_S)) m_engine.Pl().MoveBackward();
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_D)) m_engine.Pl().MoveRight();
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE)) m_engine.Pl().MoveUp();
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT)) m_engine.Pl().MoveDown();
}
void Window::PollMouseMovement(void)
{
	double x, y;
	glfwGetCursorPos(m_glfwWindow, &x, &y);
	m_engine.MouseUpdate(x, y);
}
void Window::PollMouseInput(void)
{

}
void Window::KeyboardCallback(uint32_t k)
{
	
}
#include "src\window.h"

Window::Window(unsigned int width,
	unsigned int height,
	const char* title)
	: m_width(width), m_height(height), m_title(title)
{
	m_glfwwindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
}

unsigned int& Window::Width(void)
{
	return m_width;
}
unsigned int& Window::Height(void)
{

}
const char*& Window::Title(void)
{

}
GLFWwindow* Window::GLFWWindow(void)
{

}
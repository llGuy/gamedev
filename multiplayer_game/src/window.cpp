#include <GLFW/glfw3.h>
#include <iostream>
#include "window.h"

Window::Window(const char* title, int32_t width, int32_t height)
    : m_title(title), m_width(width), m_height(height)
{
}

void Window::Init(void)
{
    m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    if(!m_window)
    {
	std::cerr << "error initializing window\n";
    }
    glfwMakeContextCurrent(m_window);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

const bool Window::Open(void) const
{
    return !glfwWindowShouldClose(m_window);
}

void Window::Update(void)
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

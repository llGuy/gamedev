#include <iostream>
#include "window.h"
#include <GLFW/glfw3.h>

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
    PollKeys();
}

void Window::EForwarder(const mulgame::MULGEventForwarder& ef)
{
    m_eventForwarder = ef;
}

void Window::PollKeys(void)
{
    using mulgame::movement_t;
    using mulgame::EntitiesHandler;
    
    if(glfwGetKey(m_window, GLFW_KEY_W)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::FORWARD);
    else if(glfwGetKey(m_window, GLFW_KEY_S)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::BACKWARD);

    if(glfwGetKey(m_window, GLFW_KEY_D)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::RIGHT);
    else if(glfwGetKey(m_window, GLFW_KEY_A)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::LEFT);
}

void Window::PollMouse(void)
{
    using mulgame::EntitiesHandler;
    
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);

    m_eventForwarder.Handle<EntitiesHandler>(glm::vec2(x, y));
}



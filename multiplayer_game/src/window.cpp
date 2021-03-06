#define GLM_ENABLE_EXPERMINENTAL

#include <iostream>
#include "window.h"
#include <GLFW/glfw3.h>

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Window* p = (Window*)(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS)
    {
	p->MBCallback(button, action);
    }
}

Window::Window(const char* title, int32_t width, int32_t height)
    : m_title(title), m_width(width), m_height(height)
{
}

void Window::Init(void)
{
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    if(!m_window)
    {
	std::cerr << "error initializing window\n";
    }
    glfwMakeContextCurrent(m_window);
    
    glfwSetWindowUserPointer(m_window, this);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    PollMouse();
}

void Window::EForwarder(const mulgame::MULGEventForwarder& ef)
{
    m_eventForwarder = ef;
}

void Window::PollKeys(void)
{
    using mulgame::movement_t;
    using mulgame::EntitiesHandler;
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::RUN);

    if(glfwGetKey(m_window, GLFW_KEY_W)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::FORWARD);
    else if(glfwGetKey(m_window, GLFW_KEY_S)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::BACKWARD);

    if(glfwGetKey(m_window, GLFW_KEY_D)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::RIGHT);
    else if(glfwGetKey(m_window, GLFW_KEY_A)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::LEFT);

    if (glfwGetKey(m_window, GLFW_KEY_SPACE)) m_eventForwarder.Handle<EntitiesHandler>(movement_t::JUMP);
}

bool Window::ReceivedKey(int32_t key)
{
	return glfwGetKey(m_window, key);
}

void Window::PollMouse(void)
{
    using mulgame::EntitiesHandler;
    using mulgame::Terrain;
    using mulgame::action_t;
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    m_eventForwarder.Handle<EntitiesHandler>(glm::vec2(x, y));

    auto& ehandler = m_eventForwarder.Get<EntitiesHandler>();
    auto& camera = ehandler.Cam();
    auto& entity = ehandler[camera.BoundEntity()];
    if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2))  m_eventForwarder.Handle<Terrain>(action_t::START_TERRAFORM, entity);
    else  m_eventForwarder.Handle<Terrain>(action_t::END_TERRAFORMING, entity);
}

void Window::MBCallback(int32_t button, int32_t action)
{
    using mulgame::EntitiesHandler;
    using mulgame::ability_t;
    if(button == GLFW_MOUSE_BUTTON_1) m_eventForwarder.Handle<EntitiesHandler>(ability_t::SHOOT);
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include "../log.h"
#include "window.h"
#include "../camera/camera.h"

#include "../lpwater/lpwater.h"

#include <glm/gtx/transform.hpp>

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
	glm::mat4 view = m_camera->GetWorldToViewMatrix();
	m_water->Draw(m_perspective, view, m_width, m_height);
}
void Window::Update(void)
{
	glfwSwapBuffers(m_glfwWindow);
	glfwPollEvents();

	if (glfwGetKey(m_glfwWindow, GLFW_KEY_W))
	{
		m_camera->MoveForward();
	}
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_S))
	{
		m_camera->MoveBackward();
	}
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_A))
	{
		m_camera->MoveLeft();
	}
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_D))
	{
		m_camera->MoveRight();
	}
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE))
	{
		m_camera->MoveUp();
	}
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT))
	{
		m_camera->MoveDown();
	}
	double x, y;
	glfwGetCursorPos(m_glfwWindow, &x, &y);
	glm::vec2 p = glm::vec2(x, y);
	m_camera->MouseUpdate(p);
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
	glfwSetKeyCallback(m_glfwWindow, KeyCallback);
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
	m_perspective = glm::perspective(glm::radians(60.0f), (float)m_width / m_height, 0.1f, 200.0f);

	LPWater::WaterDimensions dim = {80.0f, 80.0f, 0.0f};
	m_water = new LPWater(dim);
	m_camera = new Camera();

	glEnable(GL_CLIP_DISTANCE0);
}
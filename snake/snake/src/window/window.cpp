#include "window.h"
#include "..\log.h"
#include "..\game\game.h"
#include "..\camera\camera.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

static void CursurPositionCallback(GLFWwindow* window, double x, double y)
{
	Window* p = (Window*)glfwGetWindowUserPointer(window);

	glm::vec2 newPosition = glm::vec2(x, y);
	//updates the view direction of the camera
	p->UpdateMouse(newPosition);
}

static void KeyCallback(GLFWwindow* window, int key, int, int action, int )
{
	if (action == GLFW_PRESS)
	{
		Window* p = (Window*)glfwGetWindowUserPointer(window);

		if (key == GLFW_KEY_A)
		{
			p->GameObj()->MoveSnake(Snake::MOVE_LEFT);
		}
		if (key == GLFW_KEY_D)
		{
			p->GameObj()->MoveSnake(Snake::MOVE_RIGHT);
		}
		if (key == GLFW_KEY_R)
		{
			p->GameObj()->MoveSnake(Snake::MOVE_UP);
		}
		if (key == GLFW_KEY_F)
		{
			p->GameObj()->MoveSnake(Snake::MOVE_DOWN);
		}
		if (key == GLFW_KEY_SPACE)
		{
			p->CameraObj()->ToggleLookingAtGridAndSnake();
		}
	}
}

//public:

Window::Window(unsigned int width, 
	unsigned int height, 
	const char* title)
	: m_width(width), m_height(height), 
	  m_title(title)
{
	GLFWInit();
	Init();
	GLEWInit();
	AfterGLEWInit();
}
Window::~Window(void)
{
	//deletions
}

//public:

void Window::Draw(void)
{
	m_windowIsOpen = !glfwWindowShouldClose(m_glfwWindow)
		&& !(glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE));
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_game->Draw(m_camera);
	if (m_game->GameOver())
	{
		glfwDestroyWindow(m_glfwWindow);
		glfwTerminate();
		m_windowIsOpen = false;

		std::cin.get();
	}
}

void Window::Update(void)
{
	glfwSwapBuffers(m_glfwWindow);
	glfwPollEvents();
	PollKeys();
}

const bool Window::WindowIsOpen(void) const
{
	return m_windowIsOpen;
}

void Window::DestroyWindow(void)
{
	glfwDestroyWindow(m_glfwWindow);
	//glfwTerminate();
}
void Window::UpdateMouse(glm::vec2 newPosition)
{
	m_camera->MouseUpdate(newPosition);
}
Game* Window::GameObj(void)
{
	return m_game;
}
Camera* Window::CameraObj(void)
{
	return m_camera;
}

//private:

void Window::Init(void)
{
	Log("initializing window");
	m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	glfwMakeContextCurrent(m_glfwWindow);
	if (!m_glfwWindow)
	{
		Log("failed to initialize window");
		std::cin.get();
		exit(1);
	}
	glfwSetCursorPosCallback(m_glfwWindow, CursurPositionCallback);
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_glfwWindow, this);
	glfwSetKeyCallback(m_glfwWindow, KeyCallback);

	m_windowIsOpen = true;
}
void Window::GLFWInit(void)
{
	Log("initializing GLFW");
	if (!glfwInit())
	{
		glfwTerminate();
		Log("failed to initialize GLFW");
		std::cin.get();
		exit(1);
	}

}
void Window::GLEWInit(void)
{
	Log("initializing GLEW");
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		glfwTerminate();
		Log("failed to initilize GLEW");
		std::cin.get();
		exit(1);
	}
}
void Window::AfterGLEWInit(void)
{
	glEnable(GL_DEPTH_TEST);
	m_game = new Game(m_width, m_height);
	m_game->Init();
	m_camera = new Camera;
	m_camera->Init(m_glfwWindow);
}
void Window::PollKeys(void)
{
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_UP))
	{
		m_camera->MoveForward();
	}
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_DOWN))
	{
		m_camera->MoveBackward();
	}
}
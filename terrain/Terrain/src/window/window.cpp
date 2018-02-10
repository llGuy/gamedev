#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include "../player/player.h"

#include "window.h"
#include "../log.h"
#include "../biome/biome.h"
#include "../camera/camera.h"
#include "../terrain/terrain.h"

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
	glm::vec3 sky = m_terrain->Sky();
	glClearColor(sky.r, sky.g, sky.b, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewMat;
	glm::vec3 eyePos;
	if (m_cameraPlayerView)
	{
		viewMat = m_camera->GetWorldToViewMatrix();
		eyePos = m_camera->GetEyePosition();
	}
	else
	{
		viewMat = m_player->ViewMatrix();
		eyePos = m_player->Position();
	}
	m_terrain->Draw(m_projMat, viewMat, eyePos);
}
void Window::Update(void)
{
	glfwSwapBuffers(m_glfwWindow);
	glfwPollEvents();

	PollKeys();
}
const bool Window::WindowOpen(void)
{
	return !glfwWindowShouldClose(m_glfwWindow)
		&& !(glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE));
}
void Window::PollKeys(void)
{
	PollMouseMovement();
	PollCameraMovement();
}
void Window::PollCameraMovement(void)
{
	if (m_cameraPlayerView)
	{
		if (!glfwGetKey(m_glfwWindow, GLFW_KEY_R))
			m_camera->SetSpeedBackToDefault();
		else
			m_camera->SpeedUp();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_W))
			m_camera->MoveForward();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_S))
			m_camera->MoveBackward();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_A))
			m_camera->MoveLeft();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_D))
			m_camera->MoveRight();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE))
			m_camera->MoveUp();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT))
			m_camera->MoveDown();
	}
	else
	{
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_W))
			m_player->Move(FPSPlayer::FORWARD, m_terrain->GetYPosOfPlayer(m_player->Position().x, m_player->Position().z, m_player->Position().y));
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_S))
			m_player->Move(FPSPlayer::BACKWARD, m_terrain->GetYPosOfPlayer(m_player->Position().x, m_player->Position().z));
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_A))
			m_player->Strafe(FPSPlayer::LEFT, m_terrain->GetYPosOfPlayer(m_player->Position().x, m_player->Position().z));
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_D))
			m_player->Strafe(FPSPlayer::RIGHT, m_terrain->GetYPosOfPlayer(m_player->Position().x, m_player->Position().z));
	}
}
void Window::PollMouseMovement(void)
{
	double x, y;
	glfwGetCursorPos(m_glfwWindow, &x, &y);
	if(m_cameraPlayerView) m_camera->MouseUpdate(glm::vec2(x, y));
	else m_player->Look(glm::vec2(x, y));
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

	Terrain::TerrainDimensions dim = { 100.0f, 100.0f, 20.0f };
	m_terrain = new Terrain(dim, Biome::PLANES);
	m_camera = new Camera;
	FPSPlayer::FPSPlayerData pd = { glm::vec3(0.0f, m_terrain->GetYPosOfPlayer(0.0f, 0.0f), 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.02f };
	m_player = new FPSPlayer(pd, m_glfwWindow);

	m_projMat = glm::perspective(glm::radians(60.0f), (float)m_width / m_height, 0.1f, 500.0f);

	m_cameraPlayerView = true;
}
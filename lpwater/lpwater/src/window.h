#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camerae.h"

class Window
{
public:
	Window(int32_t width, int32_t height, const char* title)
		: m_width(width), m_height(height), m_title(title)
	{
		Init();
	}
	~Window(void)
	{
		glfwDestroyWindow(m_glfwWindow);
		glfwTerminate();
	}

	bool Open(void)
	{
		return !glfwWindowShouldClose(m_glfwWindow)
			&& !(glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE));
	}
	void Init(void)
	{
		GLFWInit();
		WindowInit();
		GLEWInit();
	}
	void Update(void)
	{
		glfwSwapBuffers(m_glfwWindow);
		glfwPollEvents();
		PollKeys();
		PollMouseMovement();
	}
	Camera* m_camera;

private:
	void GLFWInit(void)
	{
		if (!glfwInit())
		{
			std::cout << "failed to initialize GLFW" << std::endl;
			glfwTerminate();
			std::cin.get();
			exit(1);
		}
	}
	void GLEWInit(void)
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
	void WindowInit(void)
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
		//glfwSetMouseButtonCallback(m_glfwWindow, CursorEnterCallback);
		//glfwSetKeyCallback(m_glfwWindow, KeyboardEnterCallback);
	}
	void PollKeys(void)
	{
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_W)) m_camera->MoveForward();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_A)) m_camera->MoveLeft();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_S)) m_camera->MoveBack();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_D)) m_camera->MoveRight();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_SPACE)) m_camera->MoveUp();
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_LEFT_SHIFT)) m_camera->MoveDown();
	}
	void PollMouseMovement(void)
	{
		double x, y;
		glfwGetCursorPos(m_glfwWindow, &x, &y);
		glm::vec2 pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
		m_camera->Look(pos, 10.0f);
	}
private:
	GLFWwindow* m_glfwWindow;
	int32_t m_width;
	int32_t m_height;
	const char* m_title;
};
#ifndef CALLBACKS_HEADER
#define CALLBACKS_HEADER

#include <GLFW/glfw3.h>

#include "../window.h"

static void CursorEnterCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* p = (Window*)(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			p->MouseCallback(GLFW_MOUSE_BUTTON_1);
		}
		else if (button == GLFW_MOUSE_BUTTON_2)
		{
			p->MouseCallback(GLFW_MOUSE_BUTTON_2);
		}
	}
}

static void KeyboardEnterCallback(GLFWwindow* window, int key, int, int action, int)
{
	Window* p = (Window*)(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
	{
		if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9)
		{
			p->KeyboardCallback(key);
		}
	}
}

#endif
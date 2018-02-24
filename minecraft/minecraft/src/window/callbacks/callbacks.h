#ifndef CALLBACKS_HEADER
#define CALLBACKS_HEADER

#include <GLFW/glfw3.h>

#include "../window.h"
void CursorEnterCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* p = (Window*)(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			p->MouseCallback(GLFW_MOUSE_BUTTON_1);
		}
	}
}

#endif
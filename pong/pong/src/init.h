#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "misc.h"
//#include "window\window.h"

void GLFWInit(void)
{
	if (!glfwInit())
	{
		glfwTerminate();
		Log("failed to initialize GLFW");
	}
}
void GLEWInit(void)
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		glfwTerminate();
		Log("failed to initialize GLEW");
	}
}
/*void Init(Window* windowp)
{
	GLFWInit();
	windowp->Init();
	GLEWInit();
	windowp->InitAfterGLEWInit();
}*/
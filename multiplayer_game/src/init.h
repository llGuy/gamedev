#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void GLFWInit(void)
{
    if(!glfwInit())
    {
	std::cerr << "error intializing glfw\n";
    }
}

void GLEWInit(void)
{
    auto err = glewInit();
    if(err != GLEW_OK)
    {
	std::cerr << "error initializing glew\n";
    }
}

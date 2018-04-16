#ifndef WINDOW_H
#define WINDOW_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	explicit Window(unsigned int width,
		unsigned int height,
		const char* title);
	~Window(void);

	unsigned int& Width(void);
	unsigned int& Height(void);
	const char*& Title(void);
	GLFWwindow* GLFWWindow(void);
private:
	unsigned int m_width;
	unsigned int m_height;
	const char* m_title;

	GLFWwindow* m_glfwwindow;
};

#endif
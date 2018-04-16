#ifndef GLFW_WINDOW
#define GLFW_WINDOW

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

class Window
{
public:
	explicit Window(unsigned int width, unsigned int height, const char* name);
	~Window(void);
public:
	void Draw();
	const bool IsWindowOpen(void);
	void Update(void);
	void sendDataToOpenGL(void);
private:
	const bool Initialize(void);
private:
	GLFWwindow* m_window;
	const char* m_name;
	unsigned int m_width, m_height;
};

#endif
#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <glm/glm.hpp>
#include "../engine/engine.h"

struct GLFWwindow;

class Window
{
public:
	explicit Window(unsigned int width, unsigned int height, const char* title);
	~Window(void);
public:
	void Draw(void);
	void Update(void);
	const bool WindowOpen(void);
private:
	void WindowInit(void);
	void GLFWInit(void);
	void GLEWInit(void);
	void PollKeys(void);
	void PollMouseMovement(void);
	void AfterGLEWInit(void);
private:
	unsigned int m_width;
	unsigned int m_height;
	const char* m_title;
	GLFWwindow* m_glfwWindow;
	minecraft::Engine m_engine;
	glm::mat4 m_projectionMatrix;
};

#endif
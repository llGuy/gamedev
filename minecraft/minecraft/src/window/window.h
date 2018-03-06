#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <glm/glm.hpp>
#include "../engine/engine.h"

struct GLFWwindow;

class Window
{
public:
	explicit Window(uint32_t width, uint32_t height, const char* title);
	//~Window(void);
	void Destroy(void);
public:
	void Draw(void);
	void Update(void);
	const bool WindowOpen(void);
	void MouseCallback(uint32_t);
	void KeyboardCallback(uint32_t);
private:
	void WindowInit(void);
	void GLFWInit(void);
	void GLEWInit(void);
	void PollKeys(void);
	void PollMouseMovement(void);
	void PollMouseInput(void);
	void AfterGLEWInit(void);
private:
	uint32_t m_width;
	uint32_t m_height;
	const char* m_title;
	GLFWwindow* m_glfwWindow;
	minecraft::Engine m_engine;
	glm::mat4 m_projectionMatrix;
};

#endif
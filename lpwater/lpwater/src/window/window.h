#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <glm/glm.hpp>

class GLFWwindow;
class LPWater;
class Camera;

class Window
{
public:
	explicit Window(unsigned int width, unsigned int height, const char* title);
	~Window(void);
public:
	void Draw(void);
	void Update(void);
	const bool WindowOpen(void);
	bool& CameraPlayerView(void);
private:
	void WindowInit(void);
	void GLFWInit(void);
	void GLEWInit(void);
	void AfterGLEWInit(void);
private:
	unsigned int m_width;
	unsigned int m_height;
	const char* m_title;
	GLFWwindow* m_glfwWindow;
	Camera* m_camera;
	LPWater* m_water;

	glm::mat4 m_perspective;

	bool m_cameraPlayerView;
};

#endif
#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <glm\glm.hpp>

class GLFWwindow;
class Camera;
class Game;

class Window
{
public:
	explicit Window(unsigned int width ,
		unsigned int height, 
		const char* title);
	~Window(void);
public:
	void Draw(void);
	void Update(void);
	const bool WindowIsOpen(void) const;
	void DestroyWindow(void);
	void UpdateMouse(glm::vec2 position);
	Game* GameObj(void);
	Camera* CameraObj(void);
private:
	void Init(void);
	void GLFWInit(void);
	void GLEWInit(void);
	void AfterGLEWInit(void);
	void PollKeys(void);
private:
	unsigned int m_width;
	unsigned int m_height;
	const char*  m_title;

	GLFWwindow* m_glfwWindow;
	Game* m_game;
	Camera* m_camera;

	bool m_windowIsOpen;
};

#endif
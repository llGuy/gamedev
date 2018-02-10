#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <GLFW\glfw3.h>

class Camera
{
public:
	explicit Camera(void)
		: m_UP_VECTOR(0.0f, 1.0f, 0.0f), m_viewDirection(0.0f, 0.0f, -1.0f), m_cameraPosition(0.0f, 0.0f, 0.0f)
	{
	}
public:
	void Init(GLFWwindow* window)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		m_oldMousePosition.x = x;
		m_oldMousePosition.y = y;
	}
	void MouseUpdate(const glm::vec2& newMousePosition)
	{
		glm::vec2 mouseDelta = newMousePosition - m_oldMousePosition;
		m_viewDirection = glm::mat3(glm::rotate(-glm::radians(mouseDelta.x) * 0.02f, m_UP_VECTOR)) * m_viewDirection;
		
		glm::vec3 toRotateAround = glm::cross(m_viewDirection, m_UP_VECTOR);
		m_viewDirection = glm::mat3(glm::rotate(-glm::radians(mouseDelta.y) * 0.02f, toRotateAround)) * m_viewDirection;

		m_oldMousePosition = newMousePosition;
	}
	glm::mat4 GetWorldToViewMatrix(void) const
	{
		//return glm::mat4(1.0f);
		return glm::lookAt(m_cameraPosition, m_cameraPosition + m_viewDirection, m_UP_VECTOR);
	}
	glm::vec2 MousePosition(void)
	{
		return m_oldMousePosition;
	}
private:
	glm::vec3 m_cameraPosition;
	glm::vec3 m_viewDirection;
	glm::vec2 m_oldMousePosition;
	
	const glm::vec3 m_UP_VECTOR;
};

#endif
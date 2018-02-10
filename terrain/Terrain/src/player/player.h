#ifndef PLAYER_HEADER
#define PLAYER_HEAEDER

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

class FPSPlayer
{
public:
	struct FPSPlayerData
	{
		glm::vec3 position;
		glm::vec3 viewDirection;
		float speed;
		float height;
	};
	enum move_t
	{
		FORWARD,

		BACKWARD
	};
	enum strafe_t
	{
		RIGHT,

		LEFT
	};

	explicit FPSPlayer(FPSPlayerData pData, GLFWwindow* window)
		: m_pData(pData)
	{
		m_pData.position.y += m_pData.height;
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		m_oldMousePosition.x = (float)x;
		m_oldMousePosition.y = (float)y;
	}
	glm::vec3& Position(void)
	{
		return m_pData.position;
	}
	glm::mat4 ViewMatrix(void)
	{
		return glm::lookAt(m_pData.position, m_pData.position + m_pData.viewDirection, m_up);
	}

	void Move(move_t move, float y)
	{
		if (move == FORWARD)
		{
			glm::vec3 toMoveVector = glm::vec3(m_pData.viewDirection.x, 0.0f, m_pData.viewDirection.z);
			m_pData.position += toMoveVector * m_pData.speed;
		}
		else if (move == BACKWARD)
		{
			glm::vec3 toMoveVector = glm::vec3(m_pData.viewDirection.x, 0.0f, m_pData.viewDirection.z);
			m_pData.position -= toMoveVector * m_pData.speed;
		}
		m_pData.position.y = y + 0.7f;
	}
	void Strafe(strafe_t strafe, float y)
	{
		if (strafe == RIGHT)
		{
			glm::vec3 strafeDirection = glm::cross(m_pData.viewDirection, m_up);
			m_pData.position += strafeDirection * m_pData.speed;
		}
		else if (strafe == LEFT)
		{
			glm::vec3 strafeDirection = glm::cross(m_pData.viewDirection, m_up);
			m_pData.position -= strafeDirection * m_pData.speed;
		}
		m_pData.position.y = y + 0.7f;
	}
	void Look(glm::vec2 newMousePosition)
	{
		glm::vec2 mouseDelta = newMousePosition - m_oldMousePosition;
		m_pData.viewDirection = glm::mat3(glm::rotate(glm::radians(-mouseDelta.x) * 0.02f, m_up)) * m_pData.viewDirection;
		glm::vec3 toRotateAround = glm::cross(m_pData.viewDirection, m_up);
		m_pData.viewDirection = glm::mat3(glm::rotate(glm::radians(-mouseDelta.y) * 0.02f, toRotateAround)) * m_pData.viewDirection;
		m_oldMousePosition = newMousePosition;
	}
private:
	FPSPlayerData m_pData;
	glm::vec3 m_up;
	glm::vec2 m_oldMousePosition;
};

#endif
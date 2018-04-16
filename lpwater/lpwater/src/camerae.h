#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	Camera(void)
		: m_cursorPosition(1.0f, 1.0f), m_position(0.1f, 1.0f, 0.2f), m_direction(1.0f, 1.0f, 1.0f)
	{
	}
	glm::mat4 ViewMatrix(void)
	{
		return glm::lookAt(m_position, m_position + m_direction, UP);
	}
	void CursorPosition(const glm::vec2& cursorPos)
	{
		m_cursorPosition = cursorPos;
	}

	void Look(const glm::vec2& newCursorPosition, float sensitivity)
	{
		glm::vec2 cursorPosDiff = newCursorPosition - m_cursorPosition;
		m_cursorPosition = newCursorPosition;

		glm::vec3& direction = m_direction;
		float xAngle = glm::radians(-cursorPosDiff.x) / sensitivity;
		direction = glm::mat3(glm::rotate(xAngle, UP)) * direction;

		glm::vec3 rotateAround = glm::cross(direction, UP);
		float yAngle = glm::radians(-cursorPosDiff.y) / sensitivity;
		direction = glm::mat3(glm::rotate(yAngle, rotateAround)) * direction;
	}

	void MoveUp(void)
	{
		m_position += UP * 0.02f;
	}
	void MoveDown(void)
	{
		m_position -= UP * 0.02f;
	}
	void MoveForward(void)
	{
		glm::vec3 dir{m_direction.x, 0.0f, m_direction.z};
		m_position += dir * 0.02f;
	}
	void MoveBack(void)
	{
		glm::vec3 dir{ m_direction.x, 0.0f, m_direction.z };
		m_position -= dir * 0.02f;
	}
	void MoveRight(void)
	{
		glm::vec3 dir = glm::cross(m_direction, UP);
		m_position += dir * 0.02f;
	}
	void MoveLeft(void)
	{
		glm::vec3 dir = glm::cross(m_direction, UP);
		m_position -= dir * 0.02f;
	}
private:
	const glm::vec3 UP{ 0.0f, 1.0f, 0.0f };
	glm::vec2 m_cursorPosition;
	glm::vec3 m_position;
	glm::vec3 m_direction;
};
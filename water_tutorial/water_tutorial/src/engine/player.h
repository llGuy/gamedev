#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Player
{
public:
	explicit Player(void) 
		: m_position(0.0f, 1.0f, 0.0f), m_direction(1.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f)
	{
	}
	void Look(glm::vec2 newCursorPosition, float sensitivity)
	{
		glm::vec2 mouseDelta = newCursorPosition - m_currentCursorPosition;
		m_direction = glm::mat3(glm::rotate(glm::radians(-mouseDelta.x) * 0.02f, m_up)) * m_direction;
		glm::vec3 toRotateAround = glm::cross(m_direction, m_up);
		m_direction = glm::mat3(glm::rotate(glm::radians(-mouseDelta.y) * 0.02f, toRotateAround)) * m_direction;
		m_currentCursorPosition = newCursorPosition;

		m_direction = glm::normalize(m_direction);
	}
	void MoveForward(void)
	{
		m_position += m_direction * 0.02f;
	}
	void MoveBackward(void)
	{
		m_position -= m_direction * 0.02f;
	}
	void MoveRight(void)
	{
		glm::vec3 v = glm::cross(m_direction, m_up);
		m_position += v * 0.02f;
	}
	void MoveLeft(void)
	{
		glm::vec3 v = glm::cross(m_direction, m_up);

		m_position -= v * 0.02f;
	}
	void MoveUp(void)
	{
		m_position += m_up * 0.02f;
	}
	void MoveDown(void)
	{
		m_position -= m_up * 0.02f;
	}

	const glm::vec3& Position(void) { return m_position; }
	const glm::vec3& Direction(void) { return m_direction; }
	const glm::vec3& Up(void) { return m_up; }
private:
	glm::vec3 m_up;
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec2 m_currentCursorPosition;
};

#endif
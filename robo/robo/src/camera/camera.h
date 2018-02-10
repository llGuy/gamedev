#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include "../engine/entities/entity.h"

class Camera
{
public:
	explicit Camera(glm::vec2 mousePosition)
		: m_up(0.0f, 1.0f, 0.0f)
	{
		m_oldMousePosition = mousePosition;
	}
public:
	void Bind(Entity* ent)
	{
		m_boundEnt = ent;
	}
	glm::mat4 ViewMat(void)
	{
		return glm::lookAt(m_boundEnt->Position(), m_boundEnt->Position() + *(m_boundEnt->ViewDirection()), m_up);
	}
	void Look(glm::vec2 newMousePosition)
	{
		if (m_oldMousePosition != newMousePosition)
		{
			glm::vec2 mouseDelta = newMousePosition - m_oldMousePosition;
			*(m_boundEnt->ViewDirection()) = glm::mat3(glm::rotate(glm::radians(-mouseDelta.x) * 0.02f, m_up)) * *(m_boundEnt->ViewDirection());
			glm::vec3 toRotateAround = glm::cross(*(m_boundEnt->ViewDirection()), m_up);
			*(m_boundEnt->ViewDirection()) = glm::mat3(glm::rotate(glm::radians(-mouseDelta.y) * 0.02f, toRotateAround))
				* *(m_boundEnt->ViewDirection());
			m_oldMousePosition = newMousePosition;
		}
	}
private:
	Entity* m_boundEnt;
	const glm::vec3 m_up;
	glm::vec2 m_oldMousePosition;
};

#endif
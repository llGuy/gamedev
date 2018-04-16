#include "camera.h"

namespace minecraft
{
	namespace ent
	{
		Camera::Camera(void)
			: UP(0.0f, 1.0f, 0.0f)
		{
		}
		Camera::Camera(glm::vec2 originalCursorPosition)
			: UP(0.0f, 1.0f, 0.0f), m_currentCursorPosition(originalCursorPosition)
		{
		}
		void Camera::Bind(Entity* ent)
		{
			m_boundEntity = ent;
		}
		glm::mat4& Camera::ViewMatrix(void)
		{
			glm::vec3 position = *m_boundEntity->EntityWorldPosition() + glm::vec3(0.0f, m_boundEntity->Height(), 0.0f);
			glm::vec3 center = position + *m_boundEntity->EntityViewDirection();
			m_viewMatrix = glm::lookAt(position, center, UP);
			return m_viewMatrix;
		}
		void Camera::Look(glm::vec2 newCursorPosition, float sensitivity)
		{
			glm::vec2 mouseDelta = newCursorPosition - m_currentCursorPosition;
			/* rotating around the y axis*/
			*m_boundEntity->EntityViewDirection() = glm::mat3(glm::rotate(glm::radians(-mouseDelta.x) * sensitivity, UP)) * 
				*m_boundEntity->EntityViewDirection();
			/* rotating around the x axis */
			glm::vec3 toRotateAround = glm::cross(*(m_boundEntity->EntityViewDirection()), UP);
			*(m_boundEntity->EntityViewDirection()) = glm::mat3(glm::rotate(glm::radians(-mouseDelta.y) * 0.02f, toRotateAround))
				* *(m_boundEntity->EntityViewDirection());
			m_currentCursorPosition = newCursorPosition;
		}
		Camera& Camera::operator=(Camera&& cam)
		{
			m_boundEntity = cam.m_boundEntity;
			m_currentCursorPosition = cam.m_currentCursorPosition;
			m_viewMatrix = cam.m_viewMatrix;

			cam.m_boundEntity = nullptr;
			cam.m_currentCursorPosition = glm::vec3(0.0f);
			cam.m_viewMatrix = glm::mat4(1.0f);

			return *this;
		}
	}
}
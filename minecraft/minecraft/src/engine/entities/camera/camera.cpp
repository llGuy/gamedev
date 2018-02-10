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
			glm::vec3 position = *m_boundEntity->EntityWorldPosition();
			glm::vec3 center = position + *m_boundEntity->EntityViewDirection();
			m_viewMatrix = glm::lookAt(position, center, UP);
			return m_viewMatrix;
		}
		void Camera::Look(glm::vec2 newCursorPosition, float sensitivity)
		{
			//if (glm::all(glm::lessThan(
				//newCursorPosition - m_currentCursorPosition, glm::vec2(0.00001f))))
			//{
				glm::vec2 mouseDelta = newCursorPosition - m_currentCursorPosition;
				/* rotating around the y axis*/
				*m_boundEntity->EntityViewDirection() = glm::mat3(glm::rotate(glm::radians(-mouseDelta.x) * sensitivity, UP)) * 
					*m_boundEntity->EntityViewDirection();
				/* rotating around the x axis */
				glm::vec3 toRotateAround = glm::cross(*(m_boundEntity->EntityViewDirection()), UP);
				*(m_boundEntity->EntityViewDirection()) = glm::mat3(glm::rotate(glm::radians(-mouseDelta.y) * 0.02f, toRotateAround))
					* *(m_boundEntity->EntityViewDirection());
				m_currentCursorPosition = newCursorPosition;
			//}
		}
		Camera& Camera::operator=(const Camera&& cam)
		{
			m_boundEntity = cam.m_boundEntity;
			m_currentCursorPosition = cam.m_currentCursorPosition;
			m_viewMatrix = cam.m_viewMatrix;

			return *this;
		}
	}
}
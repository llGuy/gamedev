#include "camera.h"

namespace dawn { namespace ent {

		Camera::Camera(void)
			: m_bound(nullptr), m_cursorPosition(1.0f, 1.0f)
		{
		}

		void Camera::Bind(Entity* e)
		{
			m_bound = e;
		}

		void Camera::CursorPosition(const glm::vec2& cursorPos)
		{
			m_cursorPosition = cursorPos;
		}

		void Camera::Look(const glm::vec2& newCursorPosition, float sensitivity)
		{
			static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

			glm::vec2 cursorPosDiff = newCursorPosition - m_cursorPosition;
			m_cursorPosition = newCursorPosition;

			glm::vec3& direction = m_bound->Direction();
			float xAngle = glm::radians(-cursorPosDiff.x) * sensitivity;
			direction = glm::mat3(glm::rotate(xAngle, UP)) * direction;

			glm::vec3 rotateAround = glm::cross(direction, UP);
			float yAngle = glm::radians(-cursorPosDiff.y) * sensitivity;
			direction = glm::mat3(glm::rotate(yAngle, rotateAround)) * direction;
		}

	}
}
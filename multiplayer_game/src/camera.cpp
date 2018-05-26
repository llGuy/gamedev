#define GLM_ENABLE_EXPERIMENTAL
#include "camera.h"

namespace mulgame {

    Camera::Camera(void)
	: m_entityID(0), m_cursorPosition(1.0f, 1.0f)
    {
    }

    void Camera::Bind(uint32_t index)
    {
		m_entityID = index;
    }

    void Camera::NewCPosition(const glm::vec2& pos)
    {
		m_cursorPosition = pos;
    }

    void Camera::Look(Entity& entity, const glm::vec2& ncpos, float sensitivity)
    {
		static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec2 cursorPosDiff = ncpos - m_cursorPosition;
		m_cursorPosition = ncpos;

		glm::vec3& edir = entity.Direction();
		glm::vec3 eye = entity.Eye();

		float xAngle = glm::radians(-cursorPosDiff.x) * sensitivity;
		edir = glm::mat3(glm::rotate(xAngle, UP)) * edir;

		glm::vec3 rotateYAx = glm::cross(edir, UP);
		float yAngle = glm::radians(-cursorPosDiff.y) * sensitivity;
		edir = glm::mat3(glm::rotate(yAngle, rotateYAx)) * edir;

		m_viewMatrix = glm::lookAt(eye, eye + edir, UP);
    }

}

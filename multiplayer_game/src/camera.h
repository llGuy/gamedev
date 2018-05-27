#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/transform.hpp>
#include "entity.h"

namespace mulgame {

    class Camera
    {
	public:
		Camera(void);

		void Bind(uint32_t index);
		void NewCPosition(const glm::vec2& pos);
		void Look(Entity& entity, const glm::vec2& ncpos, float sensitivity);

		inline
		uint32_t BoundEntity(void) const
		{
			return m_entityID;
		}
		inline
		glm::mat4 ViewMatrix(Entity& entity)
		{
			return m_viewMatrix;
		}
		inline
		glm::mat4 UpdateViewMatrix(Entity& entity)
		{
			static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
			return (m_viewMatrix = glm::lookAt(entity.Position(), entity.Position() + entity.Direction(), UP));
		}
	private:
		// entities are stored in a vector
		// and the camera references to the bound entity with 
		// the index of the entity in the vector
		uint32_t m_entityID;
		glm::vec2 m_cursorPosition;
		glm::mat4 m_viewMatrix;
    };

}

#endif /* _CAMERA_H_ */

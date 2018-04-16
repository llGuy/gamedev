#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "../entity.h"
#include <glm/gtx/transform.hpp>

namespace dawn { namespace ent {
		
		class Camera
		{
		public:
			Camera(const Camera&) = delete;
			const Camera& operator=(const Camera&) = delete;
			Camera(void);

			void Bind(Entity*);
			void CursorPosition(const glm::vec2& cursorPos);
			void Look(const glm::vec2& newCursorPosition, float sensitivity);

			inline
			glm::mat4 ViewMatrix(void) 
			{
				const glm::vec3& position = m_bound->Position();
				const glm::vec3& direction = m_bound->Direction();
				return glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
			}
		private:
			Entity* m_bound;
			glm::vec2 m_cursorPosition;
		};
	
	}
}

#endif
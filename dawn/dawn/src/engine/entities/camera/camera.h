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
			void Look(const glm::vec2& newCursorPosition, float sensitivity);

			inline
			glm::mat4& ViewMatrix(void) 
			{
				return m_viewMatrix;
			}
		private:
			Entity* m_bound;
			glm::vec2 m_cursorPosition;
			glm::mat4 m_viewMatrix;
		};
	
	}
}

#endif
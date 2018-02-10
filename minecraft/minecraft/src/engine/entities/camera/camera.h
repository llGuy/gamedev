#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "../entity.h"

namespace minecraft
{
	namespace ent
	{
		class Camera
		{
		public:
			explicit Camera(void);
			explicit Camera(glm::vec2 originalCursorPosition);
		public:
			void Bind(Entity* ent);
			glm::mat4& ViewMatrix(void);
			void Look(glm::vec2 newCursorPosition, float sensitivity);
			Camera& operator=(const Camera&& cam);
		private:
			const glm::vec3 UP;
			Entity* m_boundEntity;
			glm::mat4 m_viewMatrix;
			glm::vec2 m_currentCursorPosition;
		};
	}
}

#endif
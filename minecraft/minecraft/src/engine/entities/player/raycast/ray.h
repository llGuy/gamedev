#ifndef RAY_HEADER
#define RAY_HEADER

#include <glm/glm.hpp>

namespace minecraft
{
	namespace ent
	{
		class Ray
		{
		public:
			explicit Ray(const glm::vec3& vdirection, const glm::vec3& position);
			const glm::vec3& EndPosition(void);
			const float Distance(void);
			const glm::vec3& Position(void);
			const float MaxDistance(void);
			void Extend(float scale);
		private:
			glm::vec3 m_directionScaled;
			glm::vec3 m_position;
			glm::vec3 m_endPosition;
			float m_d;
		};
	}
}

#endif
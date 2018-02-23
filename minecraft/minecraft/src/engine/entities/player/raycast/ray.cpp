#include "ray.h"
#include "../../../utility/glm_vecio.h"
#include <iostream>

namespace minecraft
{
	namespace ent
	{
		Ray::Ray(const glm::vec3& direction, const glm::vec3& position)
			: m_position(position), m_d(6.0f), m_endPosition(position)
		{
			m_directionScaled = glm::normalize(direction) * m_d;
		}
		const glm::vec3& Ray::Position(void)
		{
			return m_position;
		}
		void Ray::Extend(float scale)
		{
			m_endPosition = m_endPosition + m_directionScaled * scale;
		}
		const glm::vec3& Ray::EndPosition(void) 
		{
			return m_endPosition;
		}
		const float Ray::Distance(void)
		{
			float d = glm::distance(m_endPosition, m_position);
			return d;
		}
		const float Ray::MaxDistance(void)
		{
			return m_d;
		}
	}
}
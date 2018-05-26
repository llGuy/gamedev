#include "ray.h"

namespace mulgame {

	Ray::Ray(const glm::vec3& direction, const glm::vec3& position, float scale)
		: m_position(position), m_d(6.0f), m_endPosition(position), m_scale(scale)
	{
		m_directionScaled = glm::normalize(direction) * m_d;
		m_endPosition = m_endPosition + m_directionScaled * scale;
		m_difference = glm::distance(m_endPosition, m_position);
		++m_interval;
	}
	const glm::vec3& Ray::StartPosition(void)
	{
		return m_position;
	}
	void Ray::Extend(void)
	{
		m_endPosition = m_endPosition + m_directionScaled * m_scale;
		++m_interval;
	}
	const glm::vec3& Ray::CurrentPosition(void)
	{
		return m_endPosition;
	}
	const float Ray::DistanceCovered(void)
	{
		return m_difference * m_interval;
	}
	const float Ray::DistancetoEnd(void)
	{
		return m_d;
	}

}
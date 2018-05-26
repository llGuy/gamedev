#include "ray.h"

namespace mulgame {

	Ray::Ray(const glm::vec3& direction, const glm::vec3& position, float scale, float maxDistance)
		: m_currentPosition(position), m_direction(glm::normalize(direction) * scale), m_maxDistance(maxDistance), m_interval(0)
	{
		glm::vec3 start = m_currentPosition;
		m_currentPosition += m_direction;
		m_distanceBetweenInterval = glm::distance(start, m_currentPosition);
		++m_interval;
	}
	void Ray::Extend(void)
	{
		m_currentPosition += m_direction;
		++m_interval;
	}
	const glm::vec3& Ray::CurrentPosition(void)
	{
		return m_currentPosition;
	}
	const float Ray::DistanceCovered(void)
	{
		return m_interval * m_distanceBetweenInterval;
	}
	const float Ray::DistancetoEnd(void)
	{
		return m_maxDistance;
	}

}
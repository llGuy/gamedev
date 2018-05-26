#ifndef _RAY_HEADER_
#define _RAY_HEADER_

#include <glm/glm.hpp>

namespace mulgame {

	class Ray
	{
	public:
		explicit Ray(const glm::vec3& vdirection, const glm::vec3& position, float scale, float maxdistance);
		// current position
		const glm::vec3& CurrentPosition(void);
		// total distance covered
		const float DistanceCovered(void);
		// start position
		const glm::vec3& StartPosition(void);
		// end destination
		const float DistancetoEnd(void);
		void Extend(void);
	private:
		uint32_t m_interval;
		float m_maxDistance;
		float m_distanceBetweenInterval;
		glm::vec3 m_currentPosition;
		glm::vec3 m_direction;
	};

}

#endif

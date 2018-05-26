#ifndef _RAY_HEADER_
#define _RAY_HEADER_

#include <glm/glm.hpp>

namespace mulgame {

	class Ray
	{
	public:
		explicit Ray(const glm::vec3& vdirection, const glm::vec3& position, float scale);
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
		glm::vec3 m_directionScaled;
		glm::vec3 m_position;
		glm::vec3 m_endPosition;
		float m_d;
		float m_scale;
		float m_difference;
		uint32_t m_interval;
	};

}

#endif

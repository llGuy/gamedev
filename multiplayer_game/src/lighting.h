#ifndef _LIGHTING_HEADER_
#define _LIGHTING_HEADER_

#include <glm/glm.hpp>

namespace mulgame {

	class Lighting
	{
	public:
		Lighting(const glm::vec3& lightpos)
			: m_lightPosition(lightpos)
		{
		}

		inline
		const glm::vec3& LightPosition(void)
		{
			return m_lightPosition;
		}
	private:
		glm::vec3 m_lightPosition;
	};
	
}

#endif
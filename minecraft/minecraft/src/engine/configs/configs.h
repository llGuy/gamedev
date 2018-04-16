#ifndef CONFIGS_HEADER
#define CONFIGS_HEADER

#include <glm/glm.hpp>

namespace minecraft
{
	namespace configs
	{
		struct VConfigs
		{
			float FOV;
			float renderDistance;
			float mouseSensitivity;
		};
		
		/* cannot change */
		struct CConfigs
		{
			glm::vec3 gravity;
		};
	}
}

#endif